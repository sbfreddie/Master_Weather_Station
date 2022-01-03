Imports Microsoft.VisualBasic.FileIO
Public Class Form1
    Private Const myport As String = "com6"  'this is the users arduino port number
    Private com9 As IO.Ports.SerialPort  'the serial port to Arduino
    Private upload As String = ""
    Private dir = My.Computer.FileSystem.SpecialDirectories.MyDocuments & "\monitor.txt"
    Private d As New Display
    Private up As Boolean = False
    Private down As Boolean = False
    Private start As Integer = 0
    Dim final As Integer = 1439

    Private Sub Form1_Load(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles MyBase.Load
        Dim readthread As Threading.Thread = New Threading.Thread(AddressOf read)
        readthread.Start()  'thread runs for whole program to get data
    End Sub
    Private Sub read()

        System.Threading.Thread.CurrentThread.Priority = Threading.ThreadPriority.Highest
        Try
            com9 = My.Computer.Ports.OpenSerialPort(myport, 9600)
            com9.ReadTimeout = 20000  'should get in 10 seconds

            Do
                Dim uploadline As String = com9.ReadLine
                If uploadline.Substring(0, 1) = "T" Then
                    upload = uploadline.Replace("?", "0")
                    'newsoftserial can change last one or two characters to "?"
                    Invoke(New messdelegate(AddressOf showmessage))
                End If

            Loop
        Catch ex As Exception  'we have lost arduino connection 
            upload = ex.Message
            Try
                com9.Dispose()
                Invoke(New messdelegate(AddressOf showmessage))
            Catch exi As Exception
            End Try
        End Try

    End Sub
    Delegate Sub messdelegate() 'a delegate will run on the main window thread
    Private Sub showmessage() 'this runs on the main window thread because control not thread safe
        My.Computer.FileSystem.WriteAllText(dir, upload & vbNewLine, True)
        TextBox1.Text = TextBox1.Text & upload & vbNewLine
        If TextBox1.Lines.Length > 28 Then
            TextBox1.SelectAll()
            TextBox1.Cut()
        End If
        Button1_Click(Nothing, Nothing)
    End Sub

    Private Sub Button1_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Button1.Click
        Dim data = My.Computer.FileSystem.ReadAllText(dir) 'All the data in txt file
        Dim sdata = data.Split("="c)  'sdata is string array with 14 data components seperated
        Dim T As New ArrayList 'string arraylist of all the temperatures
        For i = 0 To sdata.Length - 14 Step 13 '0 is temperature
            T.Add(sdata(i + 1).Substring(1, sdata(i + 1).Length - 2)) 'only need 23.3
        Next
        Dim N As New ArrayList
        For i = 2 To sdata.Length - 14 Step 13 ' 2 is north
            N.Add(sdata(i + 1).Substring(1, sdata(i + 1).Length - 2))
        Next
        d.maxN = 8000
        d.minN = 4000
        Dim S As New ArrayList
        For i = 3 To sdata.Length - 14 Step 13 ' 3 is south
            S.Add(sdata(i + 1).Substring(1, sdata(i + 1).Length - 2))
        Next
        Dim Dns As New ArrayList
        For i = 4 To sdata.Length - 14 Step 13 ' 4 is NS 9 is EW
            Dns.Add(sdata(i + 1).Substring(1, sdata(i + 1).Length - 2))
        Next
        Dim Dew As New ArrayList
        For i = 9 To sdata.Length - 14 Step 13 ' 4 is NS 9 is EW
            Dew.Add(sdata(i + 1).Substring(1, sdata(i + 1).Length - 2))
        Next
        d.maxS = 8000
        d.minS = 4000
        Dim EW As New ArrayList
        For i = 7 To sdata.Length - 14 Step 13 '7 is east
            EW.Add(sdata(i + 1).Substring(1, sdata(i + 1).Length - 2))
        Next
        Dim W As New ArrayList
        For i = 8 To sdata.Length - 14 Step 13 ' 8 is west
            W.Add(sdata(i + 1).Substring(1, sdata(i + 1).Length - 2))
        Next
        Dim Wdata(W.Count) As Integer
        Dim Edata(EW.Count) As Integer
        Dim Ndata(N.Count) As Integer
        Dim SAdata(S.Count) As Integer
        Dim dnsdata(Dns.Count) As Integer
        Dim Tdata(T.Count - 1) As Integer
        Dim Errdata(Dns.Count - 1) As Integer
        If up And final < T.Count - 1 Then
            start = start + 1439
            final = final + 1439
        End If
        If down And start >= 1439 Then
            start = start - 1439
            final = final - 1439
        End If
        d.start = start
        d.final = final
        d.maxT = 50
        d.minT = -10
        For i = 0 To 1439
            d.display_west(i) = 0
            d.display_temp(i) = 0
            d.display_south(i) = 0
            d.display_north(i) = 0
            d.display_east(i) = 0
            d.display_nsdiff(i) = 0
            d.display_nserror(i) = 0
        Next
        For i = start To T.Count - 2
            Tdata(i) = CType(T(i), String) * 10
            If i < final Then
                d.display_temp(i Mod 1439) = 1.733 * Tdata(i) + 173
            End If
        Next
        For i = start To T.Count - 2
            Ndata(i) = CType(N(i), String)
            If i < final Then
                d.display_north(i Mod 1439) = (Ndata(i) - 4000) / 4 + 400
            End If
        Next
        For i = start To T.Count - 2
            SAdata(i) = CType(S(i), String)
            If i < final Then
                d.display_south(i Mod 1439) = (SAdata(i) - 4000) / 4 + 400
            End If
        Next
        For i = start To T.Count - 2
            dnsdata(i) = CType(Dns(i), String)
            If i < final Then
                d.display_nsdiff(i Mod 1439) = dnsdata(i) / 2 + 800
            End If
        Next
        For i = start To T.Count - 2
            Edata(i) = CType(EW(i), String)
            If i < final Then
                d.display_east(i Mod 1439) = (Edata(i) - 4000) / 4 + 300
            End If
        Next
        For i = start To T.Count - 2
            Wdata(i) = CType(W(i), String)
            If i < final Then
                d.display_west(i Mod 1439) = (Wdata(i) - 4000) / 4 + 300
            End If
        Next


        For i = start To T.Count - 2
            Errdata(i) = CType(Dew(i), String)
            If i < final Then
                d.display_nserror(i Mod 1439) = Errdata(i) / 2 + 800
            End If
        Next
        d.show_now()
    End Sub

    Private Sub Button2_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Button2.Click
        up = True
        Button1_Click(Nothing, Nothing)
        up = False
    End Sub

    Private Sub Button3_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Button3.Click
        down = True
        Button1_Click(Nothing, Nothing)
        down = False
    End Sub
End Class
