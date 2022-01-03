Imports System.IO
Public Class ArduinoWeather
#Region "initialisation"
    Private d As New Display
    Private com8 As IO.Ports.SerialPort  'com8 could be any number 
    Private readthread As Threading.Thread
    Private cd = My.Computer.FileSystem.CurrentDirectory & "\met_data_log"
    Private span_define() As Integer = {1, 2, 3, 6, 12, 24, 42, 84, 168}
    '4 hours to 4 weeks . The buffers are 4 weeks + spare for online display of samples
    Private display_span As Integer = 0  'the total width of the display as a factor * 1440
    Private temp_buffer(241919) As Int32  'fill this buffer to use in display of temp
    Private humid_buffer(241919) As Int32  'fill this buffer to use in display of humid
    Private out_temp_buffer(241919) As Int32  'fill this buffer to use in display of temp
    Private out_humid_buffer(241919) As Int32  'fill this buffer to use in display of humid
    Private air_buffer(241919) As Int32  'fill this buffer to use in display of air pressure
    Private volt_buffer(241919) As Int32  'fill this buffer to use in display of battery voltage
    Private daylight_buffer(241919) As Int32  'fill this buffer to use in display of battery voltage
    Private lightning_buffer(241919) As Int32  'fill this buffer to use in display of battery voltage
    Private rain_buffer(241919) As Int32  'fill this buffer to use in display of battery voltage
    Private windspeed_buffer(241919) As Int32  'fill this buffer to use in display of battery voltage
    Private winddirection_buffer(241919) As Int32  'fill this buffer to use in display of battery voltage
    Private A_temp_buffer(241919) As Int32  'fill this buffer to use in display of archived temp
    Private A_humid_buffer(241919) As Int32  'fill this buffer to use in display of  archived humid
    Private A_air_buffer(241919) As Int32  'fill this buffer to use in display of archived  air pressure
    Private A_volt_buffer(241919) As Int32  'fill this buffer to use in display of archived battery voltage
    Private A_out_temp_buffer(241919) As Int32  'fill this buffer to use in display of temp
    Private A_out_humid_buffer(241919) As Int32  'fill this buffer to use in display of humid
    Private A_windspeed_buffer(241919) As Int32  'fill this buffer to use in display of battery voltage
    Private A_winddirection_buffer(241919) As Int32  'fill this buffer to use in display of battery voltage
    Private A_daylight_buffer(241919) As Int32  'fill this buffer to use in display of battery voltage
    Private A_lightning_buffer(241919) As Int32  'fill this buffer to use in display of battery voltage
    Private A_rain_buffer(241919) As Int32  'fill this buffer to use in display of battery voltage
    Private last_pointer As Integer = 0 'pointer to the last sample in the buffer
    Private A_last_pointer As Integer = 0 'pointer to the last sample in the buffer
    Private last_sector As Integer = 0  'the number of last sector in memory
    Private lastblock As Integer = 0  'the number of the sector used in buffer filling
    Private overflow_sector As Integer = 0  'the sector number that filled the tempory buffer
    Private archived(100) As Int64  'list of archived files
    Private display_start_time As DateTime = DateTime.Now 'the start time of the display
    Private file_start_time As DateTime = DateTime.Now  'the last file date time
    Private cursor_time As Integer 'the cursor when picture clicked
    Private zoom_TH As Boolean = False  'zoom up temp humid display
    Private zoom_AR As Boolean = False  'zoom up air rain display
    Private zoom_W As Boolean = False  'zoom up air rain display
    Private current_display As Boolean = True  'do we use current or archived data
    Const t_sam As Int16 = 10   '10 second sample time
    Const n_sam As Int16 = 11     'number of samples
    Const n_sec As Int16 = 23   'number of samples of n_sam in sector(256 total)
    'TH temp _humid is top dfisplay
    'AR air_rain is middle display
    'PL wind is bottom display
#End Region
    Private Sub ArduinoWeather_FormClosing(ByVal sender As Object, ByVal e As System.Windows.Forms.FormClosingEventArgs) Handles Me.FormClosing
        Try
            My.Computer.FileSystem.DeleteFile(cd & "\ram")
        Catch ex As Exception
        End Try  'only used for current session
    End Sub
    Private Sub ArduinoWeather_Load(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles MyBase.Load
        Dim f As New showstart
        f.ShowDialog()  'startup form to connect to arduino
        While My.Computer.FileSystem.FileExists(cd & "\" & last_sector)
            last_sector += 1 'find the next block
        End While
        archivedisplay.Text = "0000"
        If My.Computer.FileSystem.FileExists(cd & "\archive") Then
            Dim st As Stream = File.Open(cd & "\archive", FileMode.Open, FileAccess.Read)
            Dim br As New BinaryReader(st)  'to get samples from stream
            Try
                Dim n As Integer = 0  'a counter
                Do
                    archived(n) = br.ReadInt64()
                    archivedisplay.Items.Add(archived(n))
                    If archived(n) > lastblock Then lastblock = archived(n)
                    n += 1  'we get the largest archive block
                    If n = 100 Then Exit Do 'no more room
                Loop
            Catch ex As Exception  'exception of none left
            Finally
                br.Close()   'must close
                st.Close()
            End Try  'exit try when all read
        End If
        fill_buffers() 'get all the samples into thir buffers
        If overflow.Text = "" Then  'enable displays
            Try
                com8 = My.Computer.Ports.OpenSerialPort("com5", 9600)
                readthread = New Threading.Thread(AddressOf read)
                readthread.Start()  'thread runs for whole program to get samples every 10 sec
            Catch ex As Exception
                comdisplay.Text = "no connection"
                display_noconnect.Enabled = True  'just use a timer to display
            End Try
        End If

    End Sub
    Private Sub fill_buffers()
        Dim running_time As DateTime 'the time for each sample as we fill buffer
        Dim interim_buffer(255) As Int32 'interim buffer 
        Dim gap_fill As Integer = 256  'the sample gap for ech sector should be 256
        For i = 0 To 241919
            temp_buffer(i) = 999999
            humid_buffer(i) = 999999
            out_temp_buffer(i) = 999999
            out_humid_buffer(i) = 999999
            air_buffer(i) = 999999
            volt_buffer(i) = 999999
            lightning_buffer(i) = 999999
            daylight_buffer(i) = 999999
            rain_buffer(i) = 999999
            windspeed_buffer(i) = 999999
            winddirection_buffer(i) = 999999

        Next  'this number will not be displayed by Display.vb
        While My.Computer.FileSystem.FileExists(cd & "\" & lastblock)
            Dim st As Stream = File.Open(cd & "\" & lastblock, FileMode.Open, FileAccess.Read)
            Dim br As New BinaryReader(st)  'to get samples from stream
            For i = 0 To 255  'all old samples
                interim_buffer(i) = br.ReadInt32()
            Next 'get this whole sector into buffer
            running_time = CDate(br.ReadString) 'time at end of this sector
            If lastblock = 0 Then
                For i = 0 To n_sec - 1  'all 85 * 3 old samples
                    temp_buffer(i) = interim_buffer(i * n_sam)
                    humid_buffer(i) = interim_buffer(i * n_sam + 1)
                    air_buffer(i) = interim_buffer(i * n_sam + 2)
                    volt_buffer(i) = interim_buffer(i * n_sam + 3)
                    lightning_buffer(i) = interim_buffer(i * n_sam + 4)
                    rain_buffer(i) = interim_buffer(i * n_sam + 5)
                    daylight_buffer(i) = interim_buffer(i * n_sam + 6)
                    out_temp_buffer(i) = interim_buffer(i * n_sam + 7)
                    out_humid_buffer(i) = interim_buffer(i * n_sam + 8)
                    windspeed_buffer(i) = interim_buffer(i * n_sam + 9)
                    winddirection_buffer(i) = interim_buffer(i * n_sam + 10)
                Next 'get this whole sector into buffer
                display_start_time = running_time
                last_pointer = n_sec - 1  'number of samples * 3 in sector
            Else
                gap_fill = (running_time - display_start_time).TotalSeconds / t_sam
                For i = 0 To gap_fill - (n_sec + 1)
                    If gap_fill - (n_sec + 1) = 0 Then Exit For 'dont need for 0
                    temp_buffer(last_pointer) = 999999 'put blanks in missing samples
                    humid_buffer(last_pointer) = 999999 'put blanks in missing samples
                    air_buffer(last_pointer) = 999999 'put blanks in missing samples
                    volt_buffer(last_pointer) = 999999 'put blanks in missing samples
                    lightning_buffer(last_pointer) = 999999 'put blanks in missing samples
                    rain_buffer(last_pointer) = 999999 'put blanks in missing samples
                    daylight_buffer(last_pointer) = 999999 'put blanks in missing samples
                    out_temp_buffer(last_pointer) = 999999 'put blanks in missing samples
                    out_humid_buffer(last_pointer) = 999999 'put blanks in missing samples
                    windspeed_buffer(last_pointer) = 999999 'put blanks in missing samples
                    winddirection_buffer(last_pointer) = 999999 'put blanks in missing samples
                    last_pointer += 1  'we have one more missed sample
                    If last_pointer = 241920 Then  '28 days of samples
                        overflow_sector = lastblock  'where the overflow occured
                        MsgBox("The 4 week buffer has filled." & vbNewLine & "This occured at sector " & _
                        lastblock & vbNewLine & "Go to file management and archive this file." & vbNewLine & _
                        "Then restart program.")
                        overflow.Text = "overflow at sector " & vbNewLine & overflow_sector
                        Exit Sub
                    End If
                Next
                For i = 0 To n_sec - 1  'all old samples
                    temp_buffer(last_pointer) = interim_buffer(i * n_sam)
                    humid_buffer(last_pointer) = interim_buffer(i * n_sam + 1)
                    air_buffer(last_pointer) = interim_buffer(i * n_sam + 2)
                    volt_buffer(last_pointer) = interim_buffer(i * n_sam + 3)
                    lightning_buffer(last_pointer) = interim_buffer(i * n_sam + 4)
                    rain_buffer(last_pointer) = interim_buffer(i * n_sam + 5)
                    daylight_buffer(last_pointer) = interim_buffer(i * n_sam + 6)
                    out_temp_buffer(last_pointer) = interim_buffer(i * n_sam + 7)
                    out_humid_buffer(last_pointer) = interim_buffer(i * n_sam + 8)
                    windspeed_buffer(last_pointer) = interim_buffer(i * n_sam + 9)
                    winddirection_buffer(last_pointer) = interim_buffer(i * n_sam + 10)
                    last_pointer += 1  'we have one more samples
                    If last_pointer = 241920 Then
                        overflow_sector = lastblock  'where the overflow occured
                        MsgBox("The 4 week buffer has filled." & vbNewLine & "This occured at sector " & _
                        lastblock & vbNewLine & "Go to file management and archive this file." & vbNewLine & _
                        "Then restart program.")
                        overflow.Text = "overflow at sector " & vbNewLine & overflow_sector
                        Exit Sub
                    End If
                Next 'get this whole sector into buffers
                display_start_time = running_time
                file_start_time = running_time
            End If
            br.Close()
            st.Close()
            lastblock += 1  'see if have another sector
        End While
        If My.Computer.FileSystem.FileExists(cd & "\ram") Then
            Dim st As Stream = File.Open(cd & "\ram", FileMode.Open, FileAccess.Read)
            Dim br As New BinaryReader(st)  'to get samples from stream
            Dim ram_time = DateTime.Now.AddSeconds(-st.Length / (4 * n_sam) * t_sam) '4bytes and 3samples
            gap_fill = (ram_time - display_start_time).TotalSeconds / t_sam
            For i = 0 To gap_fill  'there is a time gap to ram
                If gap_fill < 3 Then Exit For 'dont need for small
                temp_buffer(last_pointer) = 999999 'put blanks in missing samples
                humid_buffer(last_pointer) = 999999 'put blanks in missing samples
                air_buffer(last_pointer) = 999999 'put blanks in missing samples
                volt_buffer(last_pointer) = 999999 'put blanks in missing samples
                lightning_buffer(last_pointer) = 999999
                rain_buffer(last_pointer) = 999999 'put blanks in missing samples
                daylight_buffer(last_pointer) = 999999 'put blanks in missing samples
                out_temp_buffer(last_pointer) = 999999 'put blanks in missing samples
                out_humid_buffer(last_pointer) = 999999 'put blanks in missing samples
                windspeed_buffer(last_pointer) = 999999 'put blanks in missing samples
                winddirection_buffer(last_pointer) = 999999 'put blanks in missing samples
                last_pointer += 1  'we have one more missed sample
                If last_pointer = 241920 Then
                    overflow_sector = lastblock  'where the overflow occured
                    MsgBox("The 4 week buffer has filled." & vbNewLine & "This occured at sector " & _
                    lastblock & vbNewLine & "Go to file management and archive this file." & vbNewLine & _
                    "Then restart program.")
                    overflow.Text = "overflow at sector " & vbNewLine & overflow_sector
                    Exit Sub
                End If
            Next
            Try
                For i = 0 To 255 'possible maximun is 255
                    temp_buffer(last_pointer) = br.ReadInt32() 'exception of none left
                    humid_buffer(last_pointer) = br.ReadInt32() 'exception of none left
                    air_buffer(last_pointer) = br.ReadInt32() 'exception of none left
                    volt_buffer(last_pointer) = br.ReadInt32() 'exception of none left
                    lightning_buffer(last_pointer) = br.ReadInt32() 'exception of none left
                    rain_buffer(last_pointer) = br.ReadInt32() 'exception of none left
                    daylight_buffer(last_pointer) = br.ReadInt32() 'exception of none left
                    out_temp_buffer(last_pointer) = br.ReadInt32() 'exception of none left
                    out_humid_buffer(last_pointer) = br.ReadInt32() 'exception of none left
                    windspeed_buffer(last_pointer) = br.ReadInt32() 'exception of none left
                    winddirection_buffer(last_pointer) = br.ReadInt32() 'exception of none left
                    last_pointer += 1  'we have one more sample
                    If last_pointer = 241920 Then
                        overflow_sector = lastblock  'where the overflow occured
                        MsgBox("The 4 week buffer has filled." & vbNewLine & "This occured at sector " & _
                        lastblock & vbNewLine & "Go to file management and archive this file." & vbNewLine & _
                        "Then restart program.")
                        overflow.Text = "overflow at sector " & vbNewLine & overflow_sector
                        Exit Sub
                    End If
                Next
            Catch ex As Exception  'this will read up to the last sample in ram
            Finally
                br.Close()   'must close
                st.Close()
            End Try  'exit try when all read
            display_start_time = DateTime.Now
        End If
    End Sub
    Private Sub fill_archive_buffers(ByVal start_sector As Int64, ByVal end_sector As Int64)
        Dim running_time As DateTime 'the time for each sample as we fill buffer
        Dim interim_buffer(255) As Int32 'interim buffer 
        Dim A_lastblock As Int64 = 0  'the number of the sector used in buffer filling
        Dim gap_fill As Integer = 256  'the sample gap for ech sector should be 256
        For i = 0 To 241919
            A_temp_buffer(i) = 999999
            A_temp_buffer(i) = 999999
            A_humid_buffer(i) = 999999
            A_out_temp_buffer(i) = 999999
            A_out_humid_buffer(i) = 999999
            A_air_buffer(i) = 999999
            A_volt_buffer(i) = 999999
            A_lightning_buffer(i) = 999999
            A_daylight_buffer(i) = 999999
            A_rain_buffer(i) = 999999
            A_windspeed_buffer(i) = 999999
            A_winddirection_buffer(i) = 999999
        Next  'this number will not be displayed by Display.vb
        For A_lastblock = start_sector To end_sector - 2
            Dim st As Stream = File.Open(cd & "\" & A_lastblock, FileMode.Open, FileAccess.Read)
            Dim br As New BinaryReader(st)  'to get samples from stream
            For i = 0 To 255  'all old samples
                interim_buffer(i) = br.ReadInt32()
            Next 'get this whole sector into buffer
            running_time = CDate(br.ReadString) 'time at end of this sector
            If A_lastblock = start_sector Then
                For i = 0 To n_sec - 1  'all 85 * 3 old samples
                    A_temp_buffer(last_pointer) = interim_buffer(i * n_sam)
                    A_humid_buffer(last_pointer) = interim_buffer(i * n_sam + 1)
                    A_air_buffer(last_pointer) = interim_buffer(i * n_sam + 2)
                    A_volt_buffer(last_pointer) = interim_buffer(i * n_sam + 3)
                    A_lightning_buffer(last_pointer) = interim_buffer(i * n_sam + 4)
                    A_rain_buffer(last_pointer) = interim_buffer(i * n_sam + 5)
                    A_daylight_buffer(last_pointer) = interim_buffer(i * n_sam + 6)
                    A_out_temp_buffer(last_pointer) = interim_buffer(i * n_sam + 7)
                    A_out_humid_buffer(last_pointer) = interim_buffer(i * n_sam + 8)
                    A_windspeed_buffer(last_pointer) = interim_buffer(i * n_sam + 9)
                    A_winddirection_buffer(last_pointer) = interim_buffer(i * n_sam + 10)
                Next 'get this whole sector into buffer
                display_start_time = running_time.AddSeconds(n_sec * t_sam)
                A_last_pointer = n_sec - 1  'number of samples * 3 in sector
            Else
                gap_fill = (running_time - display_start_time).TotalSeconds / t_sam
                For i = 0 To gap_fill - n_sec + 1
                    If gap_fill - n_sec +1 = 0 Then Exit For 'dont need for 0
                    A_temp_buffer(A_last_pointer) = 999999 'put blanks in missing samples
                    A_humid_buffer(A_last_pointer) = 999999 'put blanks in missing samples
                    A_air_buffer(A_last_pointer) = 999999 'put blanks in missing samples
                    A_volt_buffer(A_last_pointer) = 999999 'put blanks in missing samples
                    A_last_pointer += 1  'we have one more missed sample
                    If A_last_pointer = 241920 Then
                        overflow_sector = A_lastblock  'where the overflow occured
                        MsgBox("The 4 week buffer has filled." & vbNewLine & "This occured at sector " & _
                        lastblock & vbNewLine & "Go to file management and archive this file." & vbNewLine & _
                        "Then restart program.")
                        overflow.Text = "overflow at sector " & vbNewLine & overflow_sector
                        Exit Sub
                    End If
                Next
                For i = 0 To n_sec - 1  'all old samples
                    A_temp_buffer(A_last_pointer) = interim_buffer(i * n_sam)
                    A_humid_buffer(A_last_pointer) = interim_buffer(i * n_sam + 1)
                    A_air_buffer(A_last_pointer) = interim_buffer(i * n_sam + 2)
                    A_volt_buffer(A_last_pointer) = interim_buffer(i * n_sam + 3)
                    A_last_pointer += 1  'we have one more samples
                    If A_last_pointer = 241920 Then
                        overflow_sector = A_lastblock  'where the overflow occured
                        MsgBox("The 4 week buffer has filled." & vbNewLine & "This occured at sector " & _
                        A_lastblock & vbNewLine & "Go to file management and archive this file." & vbNewLine & _
                        "Then restart program.")
                        overflow.Text = "overflow at sector " & vbNewLine & overflow_sector
                        Exit Sub
                    End If
                Next 'get this whole sector into buffers
                file_start_time = running_time
                display_start_time = running_time.AddSeconds(n_sec * t_sam)
            End If
            br.Close()
            st.Close()
        Next   'do all the sectors
    End Sub
    Dim upload As String = ""
    Private Sub read()
        Do
            Try
                Do
                    upload = com8.ReadLine
                    com8.ReadTimeout = 8000
                    upload = upload.Substring(0, upload.Length - 1) 'remove audinos last char
                    Invoke(New messdelegate(AddressOf showmessage))
                Loop
            Catch ex As Exception  'we dont have arduino connection
                If Not ex.Message = "The operation has timed out." Then
                    Try
                        Invoke(New messdelegate(AddressOf disconnected))
                    Catch exI As Exception
                    End Try  'need for after form closing 
                    readthread.Abort()
                End If
                Try
                    Invoke(New messdelegate(AddressOf clearmessage))
                Catch ex1 As Exception
                End Try 'need for invoke on form close
            End Try  'dispose of com8 and catch any exceptions
        Loop
    End Sub
    Delegate Sub messdelegate() 'a delegate will run on the main window thread
    Private Sub disconnected()
        comdisplay.Text = "no connection" & vbNewLine & "or" & vbNewLine & "no communication"
        display_noconnect.Enabled = True  'just use a timer to display  
    End Sub
    Private Sub showmessage() 'this runs on the main window thread because control not thread safe        
        If comdisplay.Text.Length > 150 Then
            comdisplay.Text = ""  'we dont want to fill textbox during a long load
        End If
        comdisplay.Text = comdisplay.Text & upload & vbNewLine
        Dim words() As String = comdisplay.Text.Split(vbNewLine)
        If (words.Length = 4 + n_sam) Then 'it could be valid data
            If words(0) = "sensors" Then  'its valid data
                temp_buffer(last_pointer) = CInt(words(1))
                humid_buffer(last_pointer) = CInt(words(2))
                air_buffer(last_pointer) = CInt(words(3))
                volt_buffer(last_pointer) = CInt(words(4))
                lightning_buffer(last_pointer) = CInt(words(5))
                rain_buffer(last_pointer) = CInt(words(6))
                daylight_buffer(last_pointer) = CInt(words(7))
                out_temp_buffer(last_pointer) = CInt(words(8))
                out_humid_buffer(last_pointer) = CInt(words(9))
                windspeed_buffer(last_pointer) = CInt(words(10))
                winddirection_buffer(last_pointer) = CInt(words(11))
                last_pointer += 1  'we have one more sample
                If last_pointer = 241920 Then
                    overflow_sector = lastblock  'where the overflow occured
                    MsgBox("The 4 week buffer has filled." & vbNewLine & "This occured at sector " & _
                    lastblock & vbNewLine & "Go to file management and archive this file." & vbNewLine & _
                    "Then restart program.")
                    overflow.Text = "overflow at sector " & vbNewLine & overflow_sector
                    Exit Sub
                End If
                If current_display Then
                    display_start_time = DateTime.Now
                    display_all()
                Else
                    archive_display_all()
                End If
                comdisplay.Text = "" 'clear this after use
            End If  'its "ready"
        End If  '7 lines received
    End Sub
    Private Sub clearmessage() 'this runs on the main window thread because control not thread safe        
        comdisplay.Text = ""
    End Sub
    Private Sub display_all()
        Try
            Dim Tinterrim_buffer(241919) As Int32  'interim buffer for temperature display
            Dim Hinterrim_buffer(241919) As Int32  'interim buffer for humidity display
            Dim out_Tinterrim_buffer(241919) As Int32  'interim buffer for out temperature display
            Dim out_Hinterrim_buffer(241919) As Int32  'interim buffer for out humidity display
            Dim Ainterrim_buffer(241919) As Int32  'interim buffer for air pressure display
            Dim Vinterrim_buffer(241919) As Int32  'interim buffer for voltage pressure display
            Dim Rinterrim_buffer(241919) As Int32  'interim buffer for rain display
            Dim Dinterrim_buffer(241919) As Int32  'interim buffer for daylight display
            Dim Linterrim_buffer(241919) As Int32  'interim buffer for lightning display
            Dim WSinterrim_buffer(241919) As Int32  'interim buffer for wind speed display
            Dim WDinterrim_buffer(241919) As Int32  'interim buffer for wind direction display
            Dim Cdisplay_start_time As DateTime  'the current display start time
            Select Case True
                Case RadioButton10.Checked
                    d.topdisplay = "THin"
                Case RadioButton11.Checked
                    d.topdisplay = "THout"
                Case RadioButton12.Checked
                    d.topdisplay = "wind"
                Case RadioButton13.Checked
                    d.topdisplay = "RainFv"
                Case RadioButton14.Checked
                    d.topdisplay = "PressL"
            End Select
            Select Case True
                Case RadioButton18.Checked
                    d.middledisplay = "THin"
                Case RadioButton17.Checked
                    d.middledisplay = "THout"
                Case RadioButton16.Checked
                    d.middledisplay = "wind"
                Case RadioButton19.Checked
                    d.middledisplay = "RainFv"
                Case RadioButton15.Checked
                    d.middledisplay = "PressL"
            End Select
            Select Case True
                Case RadioButton24.Checked 'up to here curser no work on scale only shows wind and rainfv
                    d.bottomdisplay = "THin" 'full size shows all
                Case RadioButton23.Checked
                    d.bottomdisplay = "THout"
                Case RadioButton22.Checked
                    d.bottomdisplay = "wind"
                Case RadioButton20.Checked
                    d.bottomdisplay = "RainFv"
                Case RadioButton21.Checked
                    d.bottomdisplay = "PressL"
            End Select
            Select Case True
                Case RadioButton1.Checked
                    display_span = span_define(0)  '4 hours
                Case RadioButton2.Checked
                    display_span = span_define(1)  '8 hours
                Case RadioButton3.Checked
                    display_span = span_define(2)  '12 hours
                Case RadioButton4.Checked
                    display_span = span_define(3)  '24 hours
                Case RadioButton5.Checked
                    display_span = span_define(4) '2 days
                Case RadioButton6.Checked
                    display_span = span_define(5)  '4 days
                Case RadioButton7.Checked
                    display_span = span_define(6) '7 days
                Case RadioButton8.Checked
                    display_span = span_define(7)  '2 weeks
                Case RadioButton9.Checked
                    display_span = span_define(8)  '4 weeks
            End Select
            For i = 0 To 241919
                If i < last_pointer + 1 Then
                    Tinterrim_buffer(241919 - i) = temp_buffer(last_pointer - i)
                    Hinterrim_buffer(241919 - i) = humid_buffer(last_pointer - i)
                    out_Tinterrim_buffer(241919 - i) = out_temp_buffer(last_pointer - i)
                    out_Hinterrim_buffer(241919 - i) = out_humid_buffer(last_pointer - i)
                    Ainterrim_buffer(241919 - i) = air_buffer(last_pointer - i)
                    Rinterrim_buffer(241919 - i) = rain_buffer(last_pointer - i)
                    Vinterrim_buffer(241919 - i) = volt_buffer(last_pointer - i)
                    Dinterrim_buffer(241919 - i) = daylight_buffer(last_pointer - i)
                    Linterrim_buffer(241919 - i) = lightning_buffer(last_pointer - i)
                    WSinterrim_buffer(241919 - i) = windspeed_buffer(last_pointer - i)
                    WDinterrim_buffer(241919 - i) = winddirection_buffer(last_pointer - i)
                Else
                    Tinterrim_buffer(241919 - i) = 999999
                    Hinterrim_buffer(241919 - i) = 999999
                    out_Tinterrim_buffer(241919 - i) = 999999
                    out_Hinterrim_buffer(241919 - i) = 999999
                    Ainterrim_buffer(241919 - i) = 999999
                    Vinterrim_buffer(241919 - i) = 999999
                    Rinterrim_buffer(241919 - i) = 999999
                    Dinterrim_buffer(241919 - i) = 999999
                    Linterrim_buffer(241919 - i) = 999999
                    WSinterrim_buffer(241919 - i) = 999999
                    WDinterrim_buffer(241919 - i) = 999999
                End If
            Next
            d.display_span_time = TimeSpan.FromMinutes(240 * display_span)
            Dim number_display As Integer = 1440 * display_span - 1  'the width of current span
            If cursor_time + number_display < 241920 Then
                Cdisplay_start_time = display_start_time.AddDays(-28 * cursor_time / 241919)
                Dim counter As Integer = 0
                For i = 241919 - cursor_time To 0 Step -display_span  'copy working to display
                    Dim average = 0
                    For j = 0 To display_span - 1
                        average += Tinterrim_buffer(i - j)
                    Next  'straight average of the number of samples required
                    d.temperature(1439 - counter) = average / display_span
                    average = 0
                    For j = 0 To display_span - 1
                        average += out_Tinterrim_buffer(i - j)
                    Next  'straight average of the number of samples required
                    d.temperatureOUT(1439 - counter) = average / display_span
                    average = 0
                    For j = 0 To display_span - 1
                        average += Hinterrim_buffer(i - j)
                    Next  'straight average of the number of samples required
                    d.humidity(1439 - counter) = average / display_span
                    average = 0
                    For j = 0 To display_span - 1
                        average += out_Hinterrim_buffer(i - j)
                    Next  'straight average of the number of samples required
                    d.humidityOUT(1439 - counter) = average / display_span
                    average = 0
                    For j = 0 To display_span - 1
                        average += Dinterrim_buffer(i - j)
                    Next  'straight average of the number of samples required
                    d.light(1439 - counter) = average / display_span
                    average = 0
                    For j = 0 To display_span - 1
                        average += Linterrim_buffer(i - j)
                    Next  'straight average of the number of samples required
                    d.flashes(1439 - counter) = average / display_span
                    average = 0
                    For j = 0 To display_span - 1
                        average += Rinterrim_buffer(i - j)
                    Next  'straight average of the number of samples required
                    d.rain(1439 - counter) = average / display_span
                    average = 0
                    For j = 0 To display_span - 1
                        average += WSinterrim_buffer(i - j)
                    Next  'straight average of the number of samples required
                    d.windspeed(1439 - counter) = average / display_span
                    average = 0
                    For j = 0 To display_span - 1
                        average += WDinterrim_buffer(i - j)
                    Next  'straight average of the number of samples required
                    d.winddir(1439 - counter) = average / display_span
                    average = 0
                    For j = 0 To display_span - 1
                        average += Ainterrim_buffer(i - j)
                    Next  'straight average of the number of samples required
                    d.airpressure(1439 - counter) = average / display_span
                    average = 0
                    For j = 0 To display_span - 1
                        average += Vinterrim_buffer(i - j)
                    Next  'straight average of the number of samples required
                    d.battvolt(1439 - counter) = average / display_span
                    counter += 1 'we have done one
                    If counter = 1440 Then Exit For
                Next
            Else
                hasbeenoffset.Text = "selected offset out of range"
                cursor_time = 0 'reset the value
            End If
            d.display_start_time = Cdisplay_start_time
            If zoom_TH Or zoom_AR Or zoom_W Then
                d.full_temp_hum() 'expand temp humid
                d.full_air_rain()
                d.full_wind()
                d.Sfull_THout()
                d.Sfull_RainFv()
            Else
                d.scale_temp_hum()
                d.scale_air_rain()
                d.scale_wind()
                d.Sscale_THout()
                d.Sscale_RainFv()
            End If
        Catch ex As Exception
        End Try
    End Sub
    Private Sub archive_display_all()
        Try
            Dim Tinterrim_buffer(241919) As Int32  'interim buffer for temperature display
            Dim Hinterrim_buffer(241919) As Int32  'interim buffer for humidity display
            Dim Ainterrim_buffer(241919) As Int32  'interim buffer for air pressure display
            Dim Vinterrim_buffer(241919) As Int32  'interim buffer for air pressure display
            Dim Cdisplay_start_time As DateTime  'the current display start time
            Select Case True
                Case RadioButton1.Checked
                    display_span = span_define(0)  '4 hours
                Case RadioButton2.Checked
                    display_span = span_define(1)  '8 hours
                Case RadioButton3.Checked
                    display_span = span_define(2)  '12 hours
                Case RadioButton4.Checked
                    display_span = span_define(3)  '24 hours
                Case RadioButton5.Checked
                    display_span = span_define(4) '2 days
                Case RadioButton6.Checked
                    display_span = span_define(5)  '4 days
                Case RadioButton7.Checked
                    display_span = span_define(6) '7 days
                Case RadioButton8.Checked
                    display_span = span_define(7)  '2 weeks
                Case RadioButton9.Checked
                    display_span = span_define(8)  '4 weeks
            End Select
            For i = 0 To 241919
                If i < A_last_pointer + 1 Then
                    Tinterrim_buffer(241919 - i) = A_temp_buffer(A_last_pointer - i)
                    Hinterrim_buffer(241919 - i) = A_humid_buffer(A_last_pointer - i)
                    Ainterrim_buffer(241919 - i) = A_air_buffer(A_last_pointer - i)
                    Vinterrim_buffer(241919 - i) = A_volt_buffer(A_last_pointer - i)
                Else
                    Tinterrim_buffer(241919 - i) = 999999
                    Hinterrim_buffer(241919 - i) = 999999
                    Ainterrim_buffer(241919 - i) = 999999
                    Vinterrim_buffer(241919 - i) = 999999
                End If
            Next
            d.display_span_time = TimeSpan.FromMinutes(240 * display_span)
            Dim number_display As Integer = 1440 * display_span - 1  'the width of current span
            If cursor_time + number_display < 241920 Then
                Cdisplay_start_time = display_start_time.AddDays(-28 * cursor_time / 241919)
                Dim counter As Integer = 0
                For i = 241919 - cursor_time To 0 Step -display_span  'copy working to display
                    Dim average = 0
                    For j = 0 To display_span - 1
                        average += Tinterrim_buffer(i - j)
                    Next  'straight average of the number of samples required
                    d.temperature(1439 - counter) = average / display_span
                    average = 0
                    For j = 0 To display_span - 1
                        average += Hinterrim_buffer(i - j)
                    Next  'straight average of the number of samples required
                    d.humidity(1439 - counter) = average / display_span
                    average = 0
                    For j = 0 To display_span - 1
                        average += Ainterrim_buffer(i - j)
                    Next  'straight average of the number of samples required
                    d.airpressure(1439 - counter) = average / display_span
                    average = 0
                    For j = 0 To display_span - 1
                        average += Vinterrim_buffer(i - j)
                    Next  'straight average of the number of samples required
                    d.battvolt(1439 - counter) = average / display_span
                    counter += 1 'we have done one
                    If counter = 1440 Then Exit For
                Next
            Else
                hasbeenoffset.Text = "selected offset out of range"
                cursor_time = 0 'reset the value
            End If
            d.display_start_time = Cdisplay_start_time
            If zoom_TH Or zoom_AR Then
                If zoom_TH Then
                    d.full_temp_hum() 'expand temp humid
                Else
                    d.full_air_rain()   'expand air rain
                End If
            Else  'normal display
                d.scale_temp_hum()
                d.scale_air_rain()
            End If
        Catch ex As Exception
        End Try  'overall catch exceptions

    End Sub
    Private Sub temphumid_MouseClick(ByVal sender As Object, ByVal e As System.Windows.Forms.MouseEventArgs) Handles temphumid.MouseClick
        cursor_time = e.X
        cursor_time = 958 - (cursor_time / temphumid.Width) * 1000
        If cursor_time < 0 Then cursor_time = 0 'cant go further right
        If cursor_time > 917 Then cursor_time = 917 'cant go further left
        cursor_time *= 263.8157  ' 0 to 241919
        Dim ct As DateTime = display_start_time.AddDays(-28 * cursor_time / 241919)
        hasbeenoffset.Text = "offset to  " & ct.ToString("h:mm tt..d MMM yy")
    End Sub
    Private Sub temphumid_MouseMove(ByVal sender As Object, ByVal e As System.Windows.Forms.MouseEventArgs) Handles temphumid.MouseMove
        If d.topdisplay = "THin" Then
            d.cursorTH = e.X
        End If
        If d.topdisplay = "THout" Then
            d.cursorTHO = e.X
        End If
        If d.topdisplay = "wind" Then
            d.cursorPL = e.X
        End If
        If d.topdisplay = "RainFv" Then
            d.cursorRFV = e.X
        End If
        If d.topdisplay = "PressL" Then
            d.cursorAR = e.X
        End If
        If zoom_TH Then
            d.full_temp_hum() 'expand temp humid
            d.full_air_rain()
            d.full_wind()
            d.Sfull_THout()
            d.Sfull_RainFv()
        Else
            d.scale_temp_hum()
            d.scale_air_rain()
            d.scale_wind()
            d.Sscale_THout()
            d.Sscale_RainFv()
        End If
        Dim Cursor = e.X
        Cursor = 958 - (Cursor / temphumid.Width) * 1000
        If Cursor < 0 Then Cursor = 0 'cant go further right
        If Cursor > 917 Then Cursor = 917 'cant go further left
        Cursor *= 263.8157  ' 0 to 241919
        Dim ct As DateTime = display_start_time.AddDays(-28 * Cursor / 241919)
        clickoffset.Text = "offset to  " & ct.ToString("h:mm tt..d MMM yy")
    End Sub
    Private Sub airrain_MouseClick(ByVal sender As Object, ByVal e As System.Windows.Forms.MouseEventArgs) Handles airrain.MouseClick
        cursor_time = e.X
        cursor_time = 958 - (cursor_time / airrain.Width) * 1000
        If cursor_time < 0 Then cursor_time = 0 'cant go further right
        If cursor_time > 917 Then cursor_time = 917 'cant go further left
        cursor_time *= 263.8157  ' 0 to 241919
        Dim ct As DateTime = display_start_time.AddDays(-28 * cursor_time / 241919)
        hasbeenoffset.Text = "offset to  " & ct.ToString("h:mm tt..d MMM yy")
    End Sub
    Private Sub airrain_MouseMove(ByVal sender As Object, ByVal e As System.Windows.Forms.MouseEventArgs) Handles airrain.MouseMove
        If d.middledisplay = "THin" Then
            d.cursorTH = e.X
        End If
        If d.middledisplay = "THout" Then
            d.cursorTHO = e.X
        End If
        If d.middledisplay = "wind" Then
            d.cursorPL = e.X
        End If
        If d.middledisplay = "RainFv" Then
            d.cursorRFV = e.X
        End If
        If d.middledisplay = "PressL" Then
            d.cursorAR = e.X
        End If
        If zoom_AR Then
            d.full_temp_hum() 'expand temp humid
            d.full_air_rain()
            d.full_wind()
            d.Sfull_THout()
            d.Sfull_RainFv()
        Else
            d.scale_temp_hum()
            d.scale_air_rain()
            d.scale_wind()
            d.Sscale_THout()
            d.Sscale_RainFv()
        End If
        Dim Cursor = e.X
        Cursor = 958 - (Cursor / airrain.Width) * 1000
        If Cursor < 0 Then Cursor = 0 'cant go further right
        If Cursor > 917 Then Cursor = 917 'cant go further left
        Cursor *= 263.8157  ' 0 to 241919
        Dim ct As DateTime = display_start_time.AddDays(-28 * Cursor / 241919)
        clickoffset.Text = "offset to  " & ct.ToString("h:mm tt..d MMM yy")
    End Sub

    Private Sub wind_MouseClick(ByVal sender As Object, ByVal e As System.Windows.Forms.MouseEventArgs) Handles wind.MouseClick
        cursor_time = e.X
        cursor_time = 958 - (cursor_time / wind.Width) * 1000
        If cursor_time < 0 Then cursor_time = 0 'cant go further right
        If cursor_time > 917 Then cursor_time = 917 'cant go further left
        cursor_time *= 263.8157  ' 0 to 241919
        Dim ct As DateTime = display_start_time.AddDays(-28 * cursor_time / 241919)
        hasbeenoffset.Text = "offset to  " & ct.ToString("h:mm tt..d MMM yy")
    End Sub
    Private Sub wind_MouseMove(ByVal sender As Object, ByVal e As System.Windows.Forms.MouseEventArgs) Handles wind.MouseMove
        If d.bottomdisplay = "THin" Then
            d.cursorTH = e.X
        End If
        If d.bottomdisplay = "THout" Then
            d.cursorTHO = e.X
        End If
        If d.bottomdisplay = "wind" Then
            d.cursorPL = e.X
        End If
        If d.bottomdisplay = "RainFv" Then
            d.cursorRFV = e.X
        End If
        If d.bottomdisplay = "PressL" Then
            d.cursorAR = e.X
        End If
        If zoom_W Then
            d.full_temp_hum() 'expand temp humid
            d.full_air_rain()
            d.full_wind()
            d.Sfull_THout()
            d.Sfull_RainFv()
        Else
            d.scale_temp_hum()
            d.scale_air_rain()
            d.scale_wind()
            d.Sscale_THout()
            d.Sscale_RainFv()
        End If
        Dim Cursor = e.X
        Cursor = 958 - (Cursor / wind.Width) * 1000
        If Cursor < 0 Then Cursor = 0 'cant go further right
        If Cursor > 917 Then Cursor = 917 'cant go further left
        Cursor *= 263.8157  ' 0 to 241919
        Dim ct As DateTime = display_start_time.AddDays(-28 * Cursor / 241919)
        clickoffset.Text = "offset to  " & ct.ToString("h:mm tt..d MMM yy")
    End Sub
    Private Sub display_noconnect_Tick(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles display_noconnect.Tick
        If current_display Then
            display_start_time = file_start_time
            display_all()
        Else
            archive_display_all()
        End If
    End Sub
    Private Sub zoomTH_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles zoomTH.Click
        If zoom_TH Then  'toggle values 
            airrain.Visible = True
            wind.Visible = True
            zoom_TH = False
            zoomAR.Enabled = True  'enable the AR button
            zoomPL.Enabled = True
            temphumid.Height = 212  'restore normal size
            clickoffset.Top = 221  'return to its position
            hasbeenoffset.Top = 221 'return to its position
            d.scale_temp_hum()
            d.scale_air_rain()
            d.scale_wind()
            d.Sscale_THout()
            d.Sscale_RainFv()
        Else
            airrain.Visible = False
            wind.Visible = False
            zoom_TH = True
            zoomAR.Enabled = False  'disable AR button
            zoomPL.Enabled = False
            temphumid.Height = 662   'expand height
            clickoffset.Top = 672  'move to bottom
            hasbeenoffset.Top = 672  'move to bottom
            d.full_temp_hum()
            d.full_air_rain()
            d.full_wind()
            d.Sfull_THout()
            d.Sfull_RainFv()
        End If
    End Sub
    Private Sub zoomAR_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles zoomAR.Click
        If zoom_AR Then  'toggle values 
            temphumid.Visible = True
            wind.Visible = True
            zoom_AR = False
            zoomTH.Enabled = True  'enable the TH button
            zoomPL.Enabled = True
            airrain.Top = 240  'return to its position
            airrain.Height = 212  'restore normal size
            clickoffset.Top = 221  'return to its position
            hasbeenoffset.Top = 221 'return to its position
            d.scale_temp_hum()
            d.scale_air_rain()
            d.scale_wind()
            d.Sscale_THout()
            d.Sscale_RainFv()
        Else
            temphumid.Visible = False
            wind.Visible = False
            zoom_AR = True
            zoomTH.Enabled = False  'disable TH button
            zoomPL.Enabled = False
            airrain.Top = 6  'send to top of page
            airrain.Height = 662  'expand height
            clickoffset.Top = 672  'move to bottom
            hasbeenoffset.Top = 672  'move to bottom
            d.full_temp_hum()
            d.full_air_rain()
            d.full_wind()
            d.Sfull_THout()
            d.Sfull_RainFv()
        End If
    End Sub
    Private Sub zoomPL_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles zoomPL.Click
        If zoom_W Then  'toggle values 
            temphumid.Visible = True
            airrain.Visible = True
            zoom_W = False
            zoomTH.Enabled = True  'enable the TH button
            zoomAR.Enabled = True
            wind.Top = 470  'return to its position
            wind.Height = 212  'restore normal size
            clickoffset.Top = 221  'return to its position
            hasbeenoffset.Top = 221 'return to its position
            d.scale_temp_hum()
            d.scale_air_rain()
            d.scale_wind()
            d.Sscale_THout()
            d.Sscale_RainFv()
        Else
            temphumid.Visible = False
            airrain.Visible = False
            zoom_W = True
            zoomTH.Enabled = False  'disable TH button
            zoomAR.Enabled = False
            wind.Top = 6  'send to top of page
            wind.Height = 662  'expand height
            clickoffset.Top = 672  'move to bottom
            hasbeenoffset.Top = 672  'move to bottom
            d.full_temp_hum()
            d.full_air_rain()
            d.full_wind()
            d.Sfull_THout()
            d.Sfull_RainFv()
        End If
    End Sub
    Private Sub displayarchive_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles displayarchive.Click
        displayarchive.BackColor = Color.Red
        displayarchive.Text = "Wait" & vbNewLine & "loading files"  'show that we are busy
        Me.Enabled = False
        Dim n = archivedisplay.SelectedIndex
        If n = -1 Then
            MsgBox("you must select a start sector " & vbNewLine & _
                   "for the start of the archive")
        Else
            If archived(n + 1) = 0 Then  'we have the highest so use current display
                current_display = True
            Else
                current_display = False  'we use archived files with start and end sectors
                fill_archive_buffers(archived(n), archived(n + 1))
            End If
        End If
        displayarchive.Text = "display" & vbNewLine & "archived file"
        displayarchive.BackColor = Color.Gainsboro
        Me.Enabled = True
    End Sub
    Private Sub HelpToolStripMenuItem_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles HelpToolStripMenuItem.Click
        Dim df As New Dump_files
        df.Show()
    End Sub
    Private Sub HelpToolStripMenuItem1_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles HelpToolStripMenuItem1.Click
        Dim df As New help
        df.Show()
    End Sub

   
   
    
End Class
