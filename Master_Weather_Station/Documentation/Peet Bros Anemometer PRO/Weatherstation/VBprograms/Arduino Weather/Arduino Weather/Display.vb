
Public Class Display
#Region "system variables"
    Public temperature(1439) As Int32  'only 0 to 1023 possible
    Public humidity(1439) As Int32 'only 0 to 1023 possible
    Public temperatureOUT(1439) As Int32  'only 0 to 1023 possible
    Public humidityOUT(1439) As Int32 'only 0 to 1023 possible
    Public airpressure(1439) As Int32  'only 0 to 1023 possible
    Public battvolt(1439) As Int32  'only 0 to 1023 possible
    Public windspeed(1439) As Int32  'only 0 to 1023 possible
    Public winddir(1439) As Int32  'only 0 to 1023 possible
    Public rain(1439) As Int32  'only 0 to 1023 possible
    Public light(1439) As Int32  'only 0 to 1023 possible
    Public flashes(1439) As Int32  'only 0 to 1023 possible
    Public cursorTH As Integer = 375  'pass the cursor from mouse e.X
    Public cursorAR As Integer = 375  'pass the cursor from mouse e.X.............pessurelight
    Public cursorPL As Integer = 375  'pass the cursor from mouse e.X.........wind
    Public cursorRFV As Integer = 375  'pass the cursor from mouse e.X
    Public cursorTHO As Integer = 375  'pass the cursor from mouse e.X
    Public display_start_time As DateTime  'pass the right hand side time
    Public display_span_time As TimeSpan  'pass  the time span of display
    Private full_image As New Bitmap(1570, 1075, Drawing.Imaging.PixelFormat.Format24bppRgb) 'THin
    Private full_THout As New Bitmap(1570, 1075, Drawing.Imaging.PixelFormat.Format24bppRgb) 'THout
    Private full_RainFv As New Bitmap(1570, 1075, Drawing.Imaging.PixelFormat.Format24bppRgb) 'RainFv
    Private ARfull_image As New Bitmap(1570, 1075, Drawing.Imaging.PixelFormat.Format24bppRgb) 'PressL
    Private WSDfull_image As New Bitmap(1570, 1075, Drawing.Imaging.PixelFormat.Format24bppRgb) 'wind
    Private g As Graphics = Graphics.FromImage(full_image) 'THin
    Private THg As Graphics = Graphics.FromImage(full_THout) 'THout
    Private RFVg As Graphics = Graphics.FromImage(full_RainFv) 'RainFv
    Private ARg As Graphics = Graphics.FromImage(ARfull_image) 'PressL
    Private WSDg As Graphics = Graphics.FromImage(WSDfull_image) 'wind
    Private scale_image As New Bitmap(1570, 430, Drawing.Imaging.PixelFormat.Format24bppRgb) 'THin
    Private gs As Graphics = Graphics.FromImage(scale_image) 'THin
    Private scale_THout As New Bitmap(1570, 430, Drawing.Imaging.PixelFormat.Format24bppRgb) 'THout
    Private THgs As Graphics = Graphics.FromImage(scale_THout) 'THout
    Private scale_RainFv As New Bitmap(1570, 430, Drawing.Imaging.PixelFormat.Format24bppRgb) 'RainFv
    Private RFVgs As Graphics = Graphics.FromImage(scale_RainFv) 'RainFv
    Private ARscale_image As New Bitmap(1570, 430, Drawing.Imaging.PixelFormat.Format24bppRgb) 'PressL
    Private ARgs As Graphics = Graphics.FromImage(ARscale_image) 'PressL
    Private WSDscale_image As New Bitmap(1570, 430, Drawing.Imaging.PixelFormat.Format24bppRgb) 'wind
    Private WSDgs As Graphics = Graphics.FromImage(WSDscale_image) 'wind
    Private stemperature(1439) As Int32  'only 0 to 1023 possible
    Private shumidity(1439) As Int32  'only 0 to 1023 possible
    Private stemperatureOUT(1439) As Int32  'only 0 to 1023 possible
    Private shumidityOUT(1439) As Int32 'only 0 to 1023 possible
    Private sairpressure(1439) As Int32  'only 0 to 1023 possible
    Private sbattvolt(1439) As Int32 'only 0 to 1023 possible
    Private swindspeed(1439) As Int32  'only 0 to 1023 possible
    Private swinddir(1439) As Int32  'only 0 to 1023 possible
    Private srain(1439) As Int32  'only 0 to 1023 possible
    Private slight(1439) As Int32  'only 0 to 1023 possible
    Private sflashes(1439) As Int32  'only 0 to 1023 possible
    Const kl As Integer = 152 'the low constant for the light sensor
    Const kh As Double = 0.011 'the high constant for the light sensor
    Public topdisplay As String = "THin"
    Public middledisplay As String = "PressL"
    Public bottomdisplay As String = "wind"
#End Region
    Public Sub full_temp_hum() 'THin
        Dim tn = display_start_time 'righthand side current time
        Dim tnll = tn.Subtract(display_span_time)  'graph left side time
        g.FillRectangle(Brushes.Black, 0, 0, 1570, 1070)
        Dim Bpen As New System.Drawing.Pen(Color.Cyan, 3)
        Dim Rpen As New System.Drawing.Pen(Color.Crimson, 3)
        Dim Gpen As New System.Drawing.Pen(Color.Lime, 3)
        g.DrawLine(Bpen, 62, 22, 62, 1045)  'left
        g.DrawLine(Bpen, 1508, 22, 1508, 1045)  'right
        g.DrawLine(Bpen, 62, 22, 1508, 22)   'top
        g.DrawLine(Bpen, 62, 278, 1508, 278)
        g.DrawLine(Bpen, 62, 534, 1508, 534)  'middle
        g.DrawLine(Bpen, 62, 790, 1508, 790)
        g.DrawLine(Bpen, 62, 1045, 1508, 1045)  'bottom
        Dim nfont As New System.Drawing.Font("arial", 16, FontStyle.Regular)
        g.DrawString(tnll.ToString("h:mm tt..d MMM yy"), nfont, Brushes.Cyan, 58, 1046)
        g.DrawString(tn.ToString("h:mm tt..d MMM yy"), nfont, Brushes.Cyan, 1310, 1046)
        g.DrawString(tnll.ToString("h:mm tt..d MMM yy"), nfont, Brushes.Cyan, 58, 0)
        g.DrawString(tn.ToString("h:mm tt..d MMM yy"), nfont, Brushes.Cyan, 1310, 0)
        For i = 1439 To 1 Step -1
            If (temperature(i - 1) > 1023 Or temperature(i) > 1023) Then
            Else
                g.DrawLine(Rpen, 63 + i, 1045 - temperature(i - 1), 64 + i, 1045 - temperature(i))
            End If
            If (humidity(i - 1) > 1023 Or humidity(i) > 1023) Then
            Else
                g.DrawLine(Gpen, 63 + i, 1045 - humidity(i - 1), 64 + i, 1045 - humidity(i))
            End If
        Next
        g.DrawString("40", nfont, Brushes.Crimson, 30, 10)
        g.DrawString("30", nfont, Brushes.Crimson, 30, 260)
        g.DrawString("20", nfont, Brushes.Crimson, 30, 520)
        g.DrawString("10", nfont, Brushes.Crimson, 30, 778)
        g.DrawString("0", nfont, Brushes.Crimson, 40, 1020)  'all the left
        g.DrawString("40", nfont, Brushes.Crimson, 1508, 10)
        g.DrawString("30", nfont, Brushes.Crimson, 1508, 260)
        g.DrawString("20", nfont, Brushes.Crimson, 1508, 520)
        g.DrawString("10", nfont, Brushes.Crimson, 1508, 778)
        g.DrawString("0", nfont, Brushes.Crimson, 1508, 1020)  'all the right
        g.DrawString("100", nfont, Brushes.Lime, 20, 30)
        g.DrawString("75", nfont, Brushes.Lime, 30, 280)
        g.DrawString("50", nfont, Brushes.Lime, 30, 540)
        g.DrawString("25", nfont, Brushes.Lime, 30, 798)
        g.DrawString("0", nfont, Brushes.Lime, 40, 1040)  'all the left
        g.DrawString("100", nfont, Brushes.Lime, 1508, 30)
        g.DrawString("75", nfont, Brushes.Lime, 1508, 280)
        g.DrawString("50", nfont, Brushes.Lime, 1508, 540)
        g.DrawString("25", nfont, Brushes.Lime, 1508, 798)
        g.DrawString("0", nfont, Brushes.Lime, 1508, 1040)  'all the right
        Dim Tcurser As Integer = (cursorTH / ArduinoWeather.temphumid.Width) * 1570
        If Tcurser < 65 Then Tcurser = 65 'cant go further left
        If Tcurser > 1503 Then Tcurser = 1503 'cant go further right
        g.DrawLine(Pens.White, Tcurser, 22, Tcurser, 1045)
        Dim dtemp = (temperature(Tcurser - 65) / 25.6).ToString("f1")
        Dim dhumid = (humidity(Tcurser - 65) / 10.23).ToString("f0")
        If dhumid > 101 Then dhumid = 0 'for blank case
        If dtemp > 41 Then dtemp = 0
        g.DrawString(("humidity = " & dhumid & "%"), nfont, Brushes.Lime, 358, 0)
        g.DrawString(("humidity = " & dhumid & "%"), nfont, Brushes.Lime, 358, 1046)
        g.DrawString(("temperature = " & dtemp & "C"), nfont, Brushes.Crimson, 1000, 0)
        g.DrawString(("temperature = " & dtemp & "C"), nfont, Brushes.Crimson, 1000, 1046)
        Dim curtime = TimeSpan.FromMinutes(display_span_time.TotalMinutes - (display_span_time.TotalMinutes * (Tcurser - 65) / 1438))
        Dim tnl = tn.Subtract(curtime)  'graph mid time at cursor
        g.DrawString(tnl.ToString("h:mm tt..d MMM yy"), nfont, Brushes.Cyan, 650, 0)  'top
        g.DrawString(tnl.ToString("h:mm tt..d MMM yy"), nfont, Brushes.Cyan, 650, 1046)  'bottom
        If topdisplay = "THin" Then
            ArduinoWeather.temphumid.Image = full_image
        End If
        If middledisplay = "THin" Then
            ArduinoWeather.airrain.Image = full_image
        End If
        If bottomdisplay = "THin" Then
            ArduinoWeather.wind.Image = full_image
        End If
    End Sub
    Public Sub scale_temp_hum() 'THin
        Try
            For i = 0 To 1439
                stemperature(i) = temperature(i) / 2.7
                shumidity(i) = humidity(i) / 2.7  'scale down the display
            Next
            Dim tn = display_start_time 'righthand side current time
            Dim tnll = tn.Subtract(display_span_time)  'graph left side time
            gs.FillRectangle(Brushes.Black, 0, 0, 1570, 1070)
            Dim Bpen As New System.Drawing.Pen(Color.Cyan, 3)
            Dim Rpen As New System.Drawing.Pen(Color.Crimson, 3)
            Dim Gpen As New System.Drawing.Pen(Color.Lime, 3)
            gs.DrawLine(Bpen, 62, 22, 62, 400)  'left
            gs.DrawLine(Bpen, 1508, 22, 1508, 400)  'right
            gs.DrawLine(Bpen, 62, 22, 1508, 22)   'top
            gs.DrawLine(Bpen, 62, 116, 1508, 116)
            gs.DrawLine(Bpen, 62, 211, 1508, 211)  'middle
            gs.DrawLine(Bpen, 62, 306, 1508, 306)
            gs.DrawLine(Bpen, 62, 400, 1508, 400)  'bottom
            Dim nfont As New System.Drawing.Font("arial", 16, FontStyle.Regular)
            gs.DrawString(tnll.ToString("h:mm tt..d MMM yy"), nfont, Brushes.Cyan, 58, 401)
            gs.DrawString(tn.ToString("h:mm tt..d MMM yy"), nfont, Brushes.Cyan, 1310, 401)
            gs.DrawString(tnll.ToString("h:mm tt..d MMM yy"), nfont, Brushes.Cyan, 58, 0)
            gs.DrawString(tn.ToString("h:mm tt..d MMM yy"), nfont, Brushes.Cyan, 1310, 0)
            For i = 1439 To 1 Step -1
                If (stemperature(i - 1) > 1023 Or stemperature(i) > 1023) Then
                Else
                    gs.DrawLine(Rpen, 63 + i, 400 - stemperature(i - 1), 64 + i, 400 - stemperature(i))
                End If
                If (shumidity(i - 1) > 1023 Or shumidity(i) > 1023) Then
                Else
                    gs.DrawLine(Gpen, 63 + i, 400 - shumidity(i - 1), 64 + i, 400 - shumidity(i))
                End If
            Next
            gs.DrawString("40", nfont, Brushes.Crimson, 30, 10)
            gs.DrawString("30", nfont, Brushes.Crimson, 30, 96)
            gs.DrawString("20", nfont, Brushes.Crimson, 30, 190)
            gs.DrawString("10", nfont, Brushes.Crimson, 30, 286)
            gs.DrawString("0", nfont, Brushes.Crimson, 40, 380)  'all the left
            gs.DrawString("40", nfont, Brushes.Crimson, 1508, 10)
            gs.DrawString("30", nfont, Brushes.Crimson, 1508, 96)
            gs.DrawString("20", nfont, Brushes.Crimson, 1508, 190)
            gs.DrawString("10", nfont, Brushes.Crimson, 1508, 286)
            gs.DrawString("0", nfont, Brushes.Crimson, 1508, 380)  'all the right
            gs.DrawString("100", nfont, Brushes.Lime, 20, 30)
            gs.DrawString("75", nfont, Brushes.Lime, 30, 116)
            gs.DrawString("50", nfont, Brushes.Lime, 30, 210)
            gs.DrawString("25", nfont, Brushes.Lime, 30, 306)
            gs.DrawString("0", nfont, Brushes.Lime, 40, 400)  'all the left
            gs.DrawString("100", nfont, Brushes.Lime, 1508, 30)
            gs.DrawString("75", nfont, Brushes.Lime, 1508, 116)
            gs.DrawString("50", nfont, Brushes.Lime, 1508, 210)
            gs.DrawString("25", nfont, Brushes.Lime, 1508, 306)
            gs.DrawString("0", nfont, Brushes.Lime, 1508, 400)  'all the right
            Dim Tcurser As Integer = (cursorTH / ArduinoWeather.temphumid.Width) * 1570
            If Tcurser < 65 Then Tcurser = 65 'cant go further left
            If Tcurser > 1503 Then Tcurser = 1503 'cant go further right
            gs.DrawLine(Pens.White, Tcurser, 22, Tcurser, 400)
            Dim dtemp = (temperature(Tcurser - 65) / 25.6).ToString("f1")
            If dtemp > 41 Then dtemp = 0
            Dim dhumid = (humidity(Tcurser - 65) / 10.23).ToString("f0")
            If dhumid > 101 Then dhumid = 0 'for blank case
            gs.DrawString(("humidity = " & dhumid & "%"), nfont, Brushes.Lime, 358, 0)
            gs.DrawString(("humidity = " & dhumid & "%"), nfont, Brushes.Lime, 358, 401)
            gs.DrawString(("temperature = " & dtemp & "C"), nfont, Brushes.Crimson, 1000, 0)
            gs.DrawString(("temperature = " & dtemp & "C"), nfont, Brushes.Crimson, 1000, 401)
            Dim curtime = TimeSpan.FromMinutes(display_span_time.TotalMinutes - (display_span_time.TotalMinutes * (Tcurser - 65) / 1438))
            Dim tnl = tn.Subtract(curtime)  'graph mid time at cursor
            gs.DrawString(tnl.ToString("h:mm tt..d MMM yy"), nfont, Brushes.Cyan, 650, 0)  'top
            gs.DrawString(tnl.ToString("h:mm tt..d MMM yy"), nfont, Brushes.Cyan, 650, 401)  'bottom
            If topdisplay = "THin" Then
                ArduinoWeather.temphumid.Image = scale_image
            End If
            If middledisplay = "THin" Then
                ArduinoWeather.airrain.Image = scale_image
            End If
            If bottomdisplay = "THin" Then
                ArduinoWeather.wind.Image = scale_image
            End If
        Catch ex As Exception
        End Try
    End Sub
    Public Sub full_air_rain() 'PressL
        Dim tn = display_start_time 'righthand side current time
        Dim tnll = tn.Subtract(display_span_time)  'graph left side time
        ARg.FillRectangle(Brushes.Black, 0, 0, 1570, 1070)
        Dim Bpen As New System.Drawing.Pen(Color.Cyan, 3)
        Dim Rpen As New System.Drawing.Pen(Color.Crimson, 3)
        Dim Gpen As New System.Drawing.Pen(Color.Lime, 3)
        ARg.DrawLine(Bpen, 62, 22, 62, 1045)  'left
        ARg.DrawLine(Bpen, 1508, 22, 1508, 1045)  'right
        ARg.DrawLine(Bpen, 62, 22, 1508, 22)   'top
        ARg.DrawLine(Bpen, 62, 278, 1508, 278)
        ARg.DrawLine(Bpen, 62, 534, 1508, 534)  'middle
        ARg.DrawLine(Bpen, 62, 790, 1508, 790)
        ARg.DrawLine(Bpen, 62, 1045, 1508, 1045)  'bottom
        Dim nfont As New System.Drawing.Font("arial", 16, FontStyle.Regular)
        ARg.DrawString(tnll.ToString("h:mm tt..d MMM yy"), nfont, Brushes.Cyan, 58, 1046)
        ARg.DrawString(tn.ToString("h:mm tt..d MMM yy"), nfont, Brushes.Cyan, 1310, 1046)
        ARg.DrawString(tnll.ToString("h:mm tt..d MMM yy"), nfont, Brushes.Cyan, 58, 0)
        ARg.DrawString(tn.ToString("h:mm tt..d MMM yy"), nfont, Brushes.Cyan, 1310, 0)
        For i = 1439 To 1 Step -1
            If (airpressure(i - 1) > 1023 Or airpressure(i) > 1023) Then
            Else
                ARg.DrawLine(Rpen, 63 + i, 1045 - airpressure(i - 1), 64 + i, 1045 - airpressure(i))
            End If
            If (light(i - 1) > 1023 Or light(i) > 1023) Then
            Else
                ARg.DrawLine(Gpen, 63 + i, 1045 - light(i - 1), 64 + i, 1045 - light(i))
            End If
        Next
        ARg.DrawString("1070", nfont, Brushes.Crimson, 8, 10)
        ARg.DrawString("1040", nfont, Brushes.Crimson, 8, 260)
        ARg.DrawString("1010", nfont, Brushes.Crimson, 8, 520)
        ARg.DrawString("980", nfont, Brushes.Crimson, 20, 778)
        ARg.DrawString("950", nfont, Brushes.Crimson, 20, 1020)  'all the left
        ARg.DrawString("1070", nfont, Brushes.Crimson, 1508, 10)
        ARg.DrawString("1040", nfont, Brushes.Crimson, 1508, 260)
        ARg.DrawString("1010", nfont, Brushes.Crimson, 1508, 520)
        ARg.DrawString("980", nfont, Brushes.Crimson, 1508, 778)
        ARg.DrawString("950", nfont, Brushes.Crimson, 1508, 1020)  'all the right
        ARg.DrawString("100k", nfont, Brushes.Lime, 8, 30)
        ARg.DrawString("3000", nfont, Brushes.Lime, 8, 280)
        ARg.DrawString("150", nfont, Brushes.Lime, 20, 540)
        ARg.DrawString("30", nfont, Brushes.Lime, 20, 798)
        ARg.DrawString("1", nfont, Brushes.Lime, 40, 1040)  'all the left
        ARg.DrawString("100k", nfont, Brushes.Lime, 1508, 30)
        ARg.DrawString("3000", nfont, Brushes.Lime, 1508, 280)
        ARg.DrawString("150", nfont, Brushes.Lime, 1508, 540)
        ARg.DrawString("30", nfont, Brushes.Lime, 1508, 798)
        ARg.DrawString("1", nfont, Brushes.Lime, 1508, 1040)  'all the right
        Dim Tcurser As Integer = (cursorAR / ArduinoWeather.wind.Width) * 1570
        If Tcurser < 65 Then Tcurser = 65 'cant go further left
        If Tcurser > 1503 Then Tcurser = 1503 'cant go further right
        ARg.DrawLine(Pens.White, Tcurser, 22, Tcurser, 1045)
        Dim dtemp = (950 + (airpressure(Tcurser - 65) * 0.117)).ToString("f0")
        Dim dhumid = -((kl + Math.Exp(kh * light(Tcurser - 65))) * Math.Log(1 - light(Tcurser - 65) / 1638)).ToString("f0")
        If dhumid > 100000 Then dhumid = 0 'for blank case
        If dtemp > 1071 Then dtemp = 0
        ARg.DrawString(("light intensity = " & dhumid & "lux"), nfont, Brushes.Lime, 358, 0)
        ARg.DrawString(("light intensity = " & dhumid & "lux"), nfont, Brushes.Lime, 358, 1046)
        ARg.DrawString(("air pressure = " & dtemp & "mb"), nfont, Brushes.Crimson, 1000, 0)
        ARg.DrawString(("air pressure = " & dtemp & "mb"), nfont, Brushes.Crimson, 1000, 1046)
        Dim curtime = TimeSpan.FromMinutes(display_span_time.TotalMinutes - (display_span_time.TotalMinutes * (Tcurser - 65) / 1438))
        Dim tnl = tn.Subtract(curtime)  'graph mid time at cursor
        ARg.DrawString(tnl.ToString("h:mm tt..d MMM yy"), nfont, Brushes.Cyan, 650, 0)  'top
        ARg.DrawString(tnl.ToString("h:mm tt..d MMM yy"), nfont, Brushes.Cyan, 650, 1046)  'bottom
        If topdisplay = "PressL" Then
            ArduinoWeather.temphumid.Image = ARfull_image
        End If
        If middledisplay = "PressL" Then
            ArduinoWeather.airrain.Image = ARfull_image
        End If
        If bottomdisplay = "PressL" Then
            ArduinoWeather.wind.Image = ARfull_image
        End If
    End Sub
    Public Sub scale_air_rain() 'PressL
        Try
            For i = 0 To 1439
                sairpressure(i) = airpressure(i) / 2.7
                slight(i) = light(i) / 2.7  'scale down the display
            Next
            Dim tn = display_start_time 'righthand side current time
            Dim tnll = tn.Subtract(display_span_time)  'graph left side time
            ARgs.FillRectangle(Brushes.Black, 0, 0, 1570, 1070)
            Dim Bpen As New System.Drawing.Pen(Color.Cyan, 3)
            Dim Rpen As New System.Drawing.Pen(Color.Crimson, 3)
            Dim Gpen As New System.Drawing.Pen(Color.Lime, 3)
            ARgs.DrawLine(Bpen, 62, 22, 62, 400)  'left
            ARgs.DrawLine(Bpen, 1508, 22, 1508, 400)  'right
            ARgs.DrawLine(Bpen, 62, 22, 1508, 22)   'top
            ARgs.DrawLine(Bpen, 62, 116, 1508, 116)
            ARgs.DrawLine(Bpen, 62, 211, 1508, 211)  'middle
            ARgs.DrawLine(Bpen, 62, 306, 1508, 306)
            ARgs.DrawLine(Bpen, 62, 400, 1508, 400)  'bottom
            Dim nfont As New System.Drawing.Font("arial", 16, FontStyle.Regular)
            ARgs.DrawString(tnll.ToString("h:mm tt..d MMM yy"), nfont, Brushes.Cyan, 58, 401)
            ARgs.DrawString(tn.ToString("h:mm tt..d MMM yy"), nfont, Brushes.Cyan, 1310, 401)
            ARgs.DrawString(tnll.ToString("h:mm tt..d MMM yy"), nfont, Brushes.Cyan, 58, 0)
            ARgs.DrawString(tn.ToString("h:mm tt..d MMM yy"), nfont, Brushes.Cyan, 1310, 0)
            For i = 1439 To 1 Step -1
                If (sairpressure(i - 1) > 1023 Or sairpressure(i) > 1023) Then
                Else
                    ARgs.DrawLine(Rpen, 63 + i, 400 - sairpressure(i - 1), 64 + i, 400 - sairpressure(i))
                End If
                If (slight(i - 1) > 1023 Or slight(i) > 1023) Then
                Else
                    ARgs.DrawLine(Gpen, 63 + i, 400 - slight(i - 1), 64 + i, 400 - slight(i))
                End If
            Next
            ARgs.DrawString("1070", nfont, Brushes.Crimson, 8, 10)
            ARgs.DrawString("1040", nfont, Brushes.Crimson, 8, 96)
            ARgs.DrawString("1010", nfont, Brushes.Crimson, 8, 190)
            ARgs.DrawString("980", nfont, Brushes.Crimson, 20, 286)
            ARgs.DrawString("950", nfont, Brushes.Crimson, 20, 380)  'all the left
            ARgs.DrawString("1070", nfont, Brushes.Crimson, 1508, 10)
            ARgs.DrawString("1040", nfont, Brushes.Crimson, 1508, 96)
            ARgs.DrawString("1010", nfont, Brushes.Crimson, 1508, 190)
            ARgs.DrawString("980", nfont, Brushes.Crimson, 1508, 286)
            ARgs.DrawString("950", nfont, Brushes.Crimson, 1508, 380)  'all the right
            ARgs.DrawString("100k", nfont, Brushes.Lime, 8, 30)
            ARgs.DrawString("3000", nfont, Brushes.Lime, 8, 116)
            ARgs.DrawString("150", nfont, Brushes.Lime, 20, 210)
            ARgs.DrawString("30", nfont, Brushes.Lime, 30, 306)
            ARgs.DrawString("1", nfont, Brushes.Lime, 40, 400)  'all the left
            ARgs.DrawString("100k", nfont, Brushes.Lime, 1508, 30)
            ARgs.DrawString("3000", nfont, Brushes.Lime, 1508, 116)
            ARgs.DrawString("150", nfont, Brushes.Lime, 1508, 210)
            ARgs.DrawString("30", nfont, Brushes.Lime, 1508, 306)
            ARgs.DrawString("1", nfont, Brushes.Lime, 1508, 400)  'all the right
            Dim Tcurser As Integer = (cursorAR / ArduinoWeather.airrain.Width) * 1570
            If Tcurser < 65 Then Tcurser = 65 'cant go further left
            If Tcurser > 1503 Then Tcurser = 1503 'cant go further right
            ARgs.DrawLine(Pens.White, Tcurser, 22, Tcurser, 400)
            Dim dtemp = (950 + (airpressure(Tcurser - 65) * 0.117)).ToString("f0")
            Dim dhumid = -((kl + Math.Exp(kh * light(Tcurser - 65))) * Math.Log(1 - light(Tcurser - 65) / 1638)).ToString("f0")
            If dhumid > 100000 Then dhumid = 0 'for blank case
            If dtemp > 1071 Then dtemp = 0
            ARgs.DrawString(("light intensity = " & dhumid & "lux"), nfont, Brushes.Lime, 358, 0)
            ARgs.DrawString(("light intensity = " & dhumid & "lux"), nfont, Brushes.Lime, 358, 401)
            ARgs.DrawString(("air pressure = " & dtemp & "mb"), nfont, Brushes.Crimson, 1000, 0)
            ARgs.DrawString(("air pressure = " & dtemp & "mb"), nfont, Brushes.Crimson, 1000, 401)
            Dim curtime = TimeSpan.FromMinutes(display_span_time.TotalMinutes - (display_span_time.TotalMinutes * (Tcurser - 65) / 1438))
            Dim tnl = tn.Subtract(curtime)  'graph mid time at cursor
            ARgs.DrawString(tnl.ToString("h:mm tt..d MMM yy"), nfont, Brushes.Cyan, 650, 0)  'top
            ARgs.DrawString(tnl.ToString("h:mm tt..d MMM yy"), nfont, Brushes.Cyan, 650, 401)  'bottom
            If topdisplay = "PressL" Then
                ArduinoWeather.temphumid.Image = ARscale_image
            End If
            If middledisplay = "PressL" Then
                ArduinoWeather.airrain.Image = ARscale_image
            End If
            If bottomdisplay = "PressL" Then
                ArduinoWeather.wind.Image = ARscale_image
            End If
        Catch ex As Exception
        End Try
    End Sub
    Public Sub full_wind() 'wind
        Dim tn = display_start_time 'righthand side current time
        Dim tnll = tn.Subtract(display_span_time)  'graph left side time
        WSDg.FillRectangle(Brushes.Black, 0, 0, 1570, 1070)
        Dim Bpen As New System.Drawing.Pen(Color.Cyan, 3)
        Dim Rpen As New System.Drawing.Pen(Color.Crimson, 3)
        Dim Gpen As New System.Drawing.Pen(Color.Lime, 3)
        WSDg.DrawLine(Bpen, 62, 22, 62, 1045)  'left
        WSDg.DrawLine(Bpen, 1508, 22, 1508, 1045)  'right
        WSDg.DrawLine(Bpen, 62, 22, 1508, 22)   'top
        WSDg.DrawLine(Bpen, 62, 278, 1508, 278)
        WSDg.DrawLine(Bpen, 62, 534, 1508, 534)  'middle
        WSDg.DrawLine(Bpen, 62, 790, 1508, 790)
        WSDg.DrawLine(Bpen, 62, 1045, 1508, 1045)  'bottom
        Dim nfont As New System.Drawing.Font("arial", 16, FontStyle.Regular)
        WSDg.DrawString(tnll.ToString("h:mm tt..d MMM yy"), nfont, Brushes.Cyan, 58, 1046)
        WSDg.DrawString(tn.ToString("h:mm tt..d MMM yy"), nfont, Brushes.Cyan, 1310, 1046)
        WSDg.DrawString(tnll.ToString("h:mm tt..d MMM yy"), nfont, Brushes.Cyan, 58, 0)
        WSDg.DrawString(tn.ToString("h:mm tt..d MMM yy"), nfont, Brushes.Cyan, 1310, 0)
        For i = 1439 To 1 Step -1
            If (windspeed(i - 1) > 1023 Or windspeed(i) > 1023) Then
            Else
                WSDg.DrawLine(Gpen, 63 + i, 1045 - windspeed(i - 1), 64 + i, 1045 - windspeed(i))
            End If
            If (winddir(i - 1) > 1023 Or winddir(i) > 1023) Then
            Else
                WSDg.DrawLine(Rpen, 63 + i, 1045 - winddir(i - 1), 64 + i, 1045 - winddir(i))
            End If
        Next
        WSDg.DrawString("North", nfont, Brushes.Crimson, 5, 10)
        WSDg.DrawString("South", nfont, Brushes.Crimson, 5, 260)
        WSDg.DrawString("North", nfont, Brushes.Crimson, 5, 520)
        WSDg.DrawString("West", nfont, Brushes.Crimson, 5, 140)
        WSDg.DrawString("East", nfont, Brushes.Crimson, 5, 390)  'all the left
        WSDg.DrawString("North", nfont, Brushes.Crimson, 1508, 10)
        WSDg.DrawString("South", nfont, Brushes.Crimson, 1508, 260)
        WSDg.DrawString("North", nfont, Brushes.Crimson, 1508, 520)
        WSDg.DrawString("West", nfont, Brushes.Crimson, 1508, 140)
        WSDg.DrawString("East", nfont, Brushes.Crimson, 1508, 390)  'all the right
        WSDg.DrawString("100", nfont, Brushes.Lime, 20, 30)
        WSDg.DrawString("75", nfont, Brushes.Lime, 30, 280)
        WSDg.DrawString("50", nfont, Brushes.Lime, 30, 540)
        WSDg.DrawString("25", nfont, Brushes.Lime, 30, 798)
        WSDg.DrawString("0", nfont, Brushes.Lime, 40, 1040)  'all the left
        WSDg.DrawString("100", nfont, Brushes.Lime, 1508, 30)
        WSDg.DrawString("75", nfont, Brushes.Lime, 1508, 280)
        WSDg.DrawString("50", nfont, Brushes.Lime, 1508, 540)
        WSDg.DrawString("25", nfont, Brushes.Lime, 1508, 798)
        WSDg.DrawString("0", nfont, Brushes.Lime, 1508, 1040)  'all the right
        Dim Tcurser As Integer = (cursorPL / ArduinoWeather.wind.Width) * 1570
        If Tcurser < 65 Then Tcurser = 65 'cant go further left
        If Tcurser > 1503 Then Tcurser = 1503 'cant go further right
        WSDg.DrawLine(Pens.White, Tcurser, 22, Tcurser, 1045)
        Dim dtemp = ((winddir(Tcurser - 65) - 512) * 0.7).ToString("f0")
        If dtemp > 360 Or dtemp < 0 Then dtemp = 0
        Dim dhumid = (windspeed(Tcurser - 65) / 10.23).ToString("f0")
        If dhumid > 101 Then dhumid = 0 'for blank case
        WSDg.DrawString(("wind speed = " & dhumid & "kph"), nfont, Brushes.Lime, 358, 0)
        WSDg.DrawString(("wind speed = " & dhumid & "kph"), nfont, Brushes.Lime, 358, 1046)
        WSDg.DrawString(("direction = " & dtemp & "°"), nfont, Brushes.Crimson, 1000, 0)
        WSDg.DrawString(("direction = " & dtemp & "°"), nfont, Brushes.Crimson, 1000, 1046)
        Dim curtime = TimeSpan.FromMinutes(display_span_time.TotalMinutes - (display_span_time.TotalMinutes * (Tcurser - 65) / 1438))
        Dim tnl = tn.Subtract(curtime)  'graph mid time at cursor
        WSDg.DrawString(tnl.ToString("h:mm tt..d MMM yy"), nfont, Brushes.Cyan, 650, 0)  'top
        WSDg.DrawString(tnl.ToString("h:mm tt..d MMM yy"), nfont, Brushes.Cyan, 650, 1046)  'bottom
        If topdisplay = "wind" Then
            ArduinoWeather.temphumid.Image = WSDfull_image
        End If
        If middledisplay = "wind" Then
            ArduinoWeather.airrain.Image = WSDfull_image
        End If
        If bottomdisplay = "wind" Then
            ArduinoWeather.wind.Image = WSDfull_image
        End If
    End Sub

    Public Sub scale_wind()
        Try
            For i = 0 To 1439
                swindspeed(i) = windspeed(i) / 2.7 'angle is 0 to 360 deg
                swinddir(i) = winddir(i) / 2.7  'wind is 0 to 100 kph
            Next
            Dim tn = display_start_time 'righthand side current time
            Dim tnll = tn.Subtract(display_span_time)  'graph left side time
            WSDgs.FillRectangle(Brushes.Black, 0, 0, 1570, 1070)
            Dim Bpen As New System.Drawing.Pen(Color.Cyan, 3)
            Dim Rpen As New System.Drawing.Pen(Color.Crimson, 3)
            Dim Gpen As New System.Drawing.Pen(Color.Lime, 3)
            WSDgs.DrawLine(Bpen, 62, 22, 62, 400)  'left
            WSDgs.DrawLine(Bpen, 1508, 22, 1508, 400)  'right
            WSDgs.DrawLine(Bpen, 62, 22, 1508, 22)   'top
            WSDgs.DrawLine(Bpen, 62, 116, 1508, 116)
            WSDgs.DrawLine(Bpen, 62, 211, 1508, 211)  'middle
            WSDgs.DrawLine(Bpen, 62, 306, 1508, 306)
            WSDgs.DrawLine(Bpen, 62, 400, 1508, 400)  'bottom
            Dim nfont As New System.Drawing.Font("arial", 16, FontStyle.Regular)
            WSDgs.DrawString(tnll.ToString("h:mm tt..d MMM yy"), nfont, Brushes.Cyan, 58, 401)
            WSDgs.DrawString(tn.ToString("h:mm tt..d MMM yy"), nfont, Brushes.Cyan, 1310, 401)
            WSDgs.DrawString(tnll.ToString("h:mm tt..d MMM yy"), nfont, Brushes.Cyan, 58, 0)
            WSDgs.DrawString(tn.ToString("h:mm tt..d MMM yy"), nfont, Brushes.Cyan, 1310, 0)
            For i = 1439 To 1 Step -1
                If (swindspeed(i - 1) > 1023 Or swindspeed(i) > 1023) Then
                Else
                    WSDgs.DrawLine(Gpen, 63 + i, 400 - swindspeed(i - 1), 64 + i, 400 - swindspeed(i))
                End If
                If (swinddir(i - 1) > 1023 Or swinddir(i) > 1023) Then
                Else
                    WSDgs.DrawLine(Rpen, 63 + i, 400 - swinddir(i - 1), 64 + i, 400 - swinddir(i))
                End If
            Next
            WSDgs.DrawString("North", nfont, Brushes.Crimson, 5, 10)
            WSDgs.DrawString("West", nfont, Brushes.Crimson, 5, 54)
            WSDgs.DrawString("North", nfont, Brushes.Crimson, 5, 190)
            WSDgs.DrawString("South", nfont, Brushes.Crimson, 5, 96)
            WSDgs.DrawString("East", nfont, Brushes.Crimson, 5, 150)  'all the left
            WSDgs.DrawString("North", nfont, Brushes.Crimson, 1508, 10)
            WSDgs.DrawString("West", nfont, Brushes.Crimson, 1508, 54)
            WSDgs.DrawString("South", nfont, Brushes.Crimson, 1508, 96)
            WSDgs.DrawString("East", nfont, Brushes.Crimson, 1508, 150)
            WSDgs.DrawString("North", nfont, Brushes.Crimson, 1508, 190) 'all the right
            WSDgs.DrawString("100", nfont, Brushes.Lime, 20, 30)
            WSDgs.DrawString("75", nfont, Brushes.Lime, 30, 116)
            WSDgs.DrawString("50", nfont, Brushes.Lime, 30, 210)
            WSDgs.DrawString("25", nfont, Brushes.Lime, 30, 306)
            WSDgs.DrawString("0", nfont, Brushes.Lime, 40, 400)  'all the left
            WSDgs.DrawString("100", nfont, Brushes.Lime, 1508, 30)
            WSDgs.DrawString("75", nfont, Brushes.Lime, 1508, 116)
            WSDgs.DrawString("50", nfont, Brushes.Lime, 1508, 210)
            WSDgs.DrawString("25", nfont, Brushes.Lime, 1508, 306)
            WSDgs.DrawString("0", nfont, Brushes.Lime, 1508, 400)  'all the right
            Dim Tcurser As Integer = (cursorPL / ArduinoWeather.temphumid.Width) * 1570
            If Tcurser < 65 Then Tcurser = 65 'cant go further left
            If Tcurser > 1503 Then Tcurser = 1503 'cant go further right
            WSDgs.DrawLine(Pens.White, Tcurser, 22, Tcurser, 400)
            Dim dtemp = ((winddir(Tcurser - 65) - 512) * 0.7).ToString("f0")
            If dtemp > 360 Or dtemp < 0 Then dtemp = 0
            Dim dhumid = (windspeed(Tcurser - 65) / 10.24).ToString("f0")
            If dhumid > 101 Then dhumid = 0 'for blank case
            WSDgs.DrawString(("wind speed = " & dhumid & "kph"), nfont, Brushes.Lime, 358, 0)
            WSDgs.DrawString(("wind speed = " & dhumid & "kph"), nfont, Brushes.Lime, 358, 401)
            WSDgs.DrawString(("direction = " & dtemp & "°"), nfont, Brushes.Crimson, 1000, 0)
            WSDgs.DrawString(("direction = " & dtemp & "°"), nfont, Brushes.Crimson, 1000, 401)
            Dim curtime = TimeSpan.FromMinutes(display_span_time.TotalMinutes - (display_span_time.TotalMinutes * (Tcurser - 65) / 1438))
            Dim tnl = tn.Subtract(curtime)  'graph mid time at cursor
            WSDgs.DrawString(tnl.ToString("h:mm tt..d MMM yy"), nfont, Brushes.Cyan, 650, 0)  'top
            WSDgs.DrawString(tnl.ToString("h:mm tt..d MMM yy"), nfont, Brushes.Cyan, 650, 401)  'bottom
            If topdisplay = "wind" Then
                ArduinoWeather.temphumid.Image = WSDscale_image
            End If
            If middledisplay = "wind" Then
                ArduinoWeather.airrain.Image = WSDscale_image
            End If
            If bottomdisplay = "wind" Then
                ArduinoWeather.wind.Image = WSDscale_image
            End If
        Catch ex As Exception
        End Try
    End Sub

    Public Sub Sfull_THout() 'THout
        Dim tn = display_start_time 'righthand side current time
        Dim tnll = tn.Subtract(display_span_time)  'graph left side time
        THg.FillRectangle(Brushes.Black, 0, 0, 1570, 1070)
        Dim Bpen As New System.Drawing.Pen(Color.Cyan, 3)
        Dim Rpen As New System.Drawing.Pen(Color.Crimson, 3)
        Dim Gpen As New System.Drawing.Pen(Color.Lime, 3)
        THg.DrawLine(Bpen, 62, 22, 62, 1045)  'left
        THg.DrawLine(Bpen, 1508, 22, 1508, 1045)  'right
        THg.DrawLine(Bpen, 62, 22, 1508, 22)   'top
        THg.DrawLine(Bpen, 62, 278, 1508, 278)
        THg.DrawLine(Bpen, 62, 534, 1508, 534)  'middle
        THg.DrawLine(Bpen, 62, 790, 1508, 790)
        THg.DrawLine(Bpen, 62, 1045, 1508, 1045)  'bottom
        Dim nfont As New System.Drawing.Font("arial", 16, FontStyle.Regular)
        THg.DrawString(tnll.ToString("h:mm tt..d MMM yy"), nfont, Brushes.Cyan, 58, 1046)
        THg.DrawString(tn.ToString("h:mm tt..d MMM yy"), nfont, Brushes.Cyan, 1310, 1046)
        THg.DrawString(tnll.ToString("h:mm tt..d MMM yy"), nfont, Brushes.Cyan, 58, 0)
        THg.DrawString(tn.ToString("h:mm tt..d MMM yy"), nfont, Brushes.Cyan, 1310, 0)
        For i = 1439 To 1 Step -1
            If (temperatureOUT(i - 1) > 1023 Or temperatureOUT(i) > 1023) Then
            Else
                THg.DrawLine(Rpen, 63 + i, 1045 - temperatureOUT(i - 1), 64 + i, 1045 - temperatureOUT(i))
            End If
            If (humidityOUT(i - 1) > 1023 Or humidityOUT(i) > 1023) Then
            Else
                THg.DrawLine(Gpen, 63 + i, 1045 - humidityOUT(i - 1), 64 + i, 1045 - humidityOUT(i))
            End If
        Next
        THg.DrawString("50", nfont, Brushes.Crimson, 30, 10)
        THg.DrawString("35", nfont, Brushes.Crimson, 30, 260)
        THg.DrawString("20", nfont, Brushes.Crimson, 30, 520)
        THg.DrawString("5", nfont, Brushes.Crimson, 40, 778)
        THg.DrawString("-10", nfont, Brushes.Crimson, 20, 1020)  'all the left
        THg.DrawString("50", nfont, Brushes.Crimson, 1508, 10)
        THg.DrawString("35", nfont, Brushes.Crimson, 1508, 260)
        THg.DrawString("20", nfont, Brushes.Crimson, 1508, 520)
        THg.DrawString("5", nfont, Brushes.Crimson, 1508, 778)
        THg.DrawString("-10", nfont, Brushes.Crimson, 1508, 1020)  'all the right
        THg.DrawString("100", nfont, Brushes.Lime, 20, 30)
        THg.DrawString("75", nfont, Brushes.Lime, 30, 280)
        THg.DrawString("50", nfont, Brushes.Lime, 30, 540)
        THg.DrawString("25", nfont, Brushes.Lime, 30, 798)
        THg.DrawString("0", nfont, Brushes.Lime, 40, 1040)  'all the left
        THg.DrawString("100", nfont, Brushes.Lime, 1508, 30)
        THg.DrawString("75", nfont, Brushes.Lime, 1508, 280)
        THg.DrawString("50", nfont, Brushes.Lime, 1508, 540)
        THg.DrawString("25", nfont, Brushes.Lime, 1508, 798)
        THg.DrawString("0", nfont, Brushes.Lime, 1508, 1040)  'all the right
        Dim Tcurser As Integer = (cursorTHO / ArduinoWeather.wind.Width) * 1570
        If Tcurser < 65 Then Tcurser = 65 'cant go further left
        If Tcurser > 1503 Then Tcurser = 1503 'cant go further right
        THg.DrawLine(Pens.White, Tcurser, 22, Tcurser, 1045)
        Dim dtemp = (temperatureOUT(Tcurser - 65) / 17.06 - 10).ToString("f1")
        Dim dhumid = (humidityOUT(Tcurser - 65) / 10.23).ToString("f0")
        If dhumid > 101 Then dhumid = 0 'for blank case
        If dtemp > 41 Then dtemp = 0
        THg.DrawString(("humidity = " & dhumid & "%"), nfont, Brushes.Lime, 358, 0)
        THg.DrawString(("humidity = " & dhumid & "%"), nfont, Brushes.Lime, 358, 1046)
        THg.DrawString(("temperature = " & dtemp & "C"), nfont, Brushes.Crimson, 1000, 0)
        THg.DrawString(("temperature = " & dtemp & "C"), nfont, Brushes.Crimson, 1000, 1046)
        Dim curtime = TimeSpan.FromMinutes(display_span_time.TotalMinutes - (display_span_time.TotalMinutes * (cursorTH - 65) / 1438))
        Dim tnl = tn.Subtract(curtime)  'graph mid time at cursor
        THg.DrawString(tnl.ToString("h:mm tt..d MMM yy"), nfont, Brushes.Cyan, 650, 0)  'top
        THg.DrawString(tnl.ToString("h:mm tt..d MMM yy"), nfont, Brushes.Cyan, 650, 1046)  'bottom
        If topdisplay = "THout" Then
            ArduinoWeather.temphumid.Image = full_THout
        End If
        If middledisplay = "THout" Then
            ArduinoWeather.airrain.Image = full_THout
        End If
        If bottomdisplay = "THout" Then
            ArduinoWeather.wind.Image = full_THout
        End If
    End Sub

    Public Sub Sscale_THout() 'THout
        Try
            For i = 0 To 1439
                stemperatureOUT(i) = temperatureOUT(i) / 2.7
                shumidityOUT(i) = humidityOUT(i) / 2.7  'scale down the display
            Next
            Dim tn = display_start_time 'righthand side current time
            Dim tnll = tn.Subtract(display_span_time)  'graph left side time
            THgs.FillRectangle(Brushes.Black, 0, 0, 1570, 1070)
            Dim Bpen As New System.Drawing.Pen(Color.Cyan, 3)
            Dim Rpen As New System.Drawing.Pen(Color.Crimson, 3)
            Dim Gpen As New System.Drawing.Pen(Color.Lime, 3)
            THgs.DrawLine(Bpen, 62, 22, 62, 400)  'left
            THgs.DrawLine(Bpen, 1508, 22, 1508, 400)  'right
            THgs.DrawLine(Bpen, 62, 22, 1508, 22)   'top
            THgs.DrawLine(Bpen, 62, 116, 1508, 116)
            THgs.DrawLine(Bpen, 62, 211, 1508, 211)  'middle
            THgs.DrawLine(Bpen, 62, 306, 1508, 306)
            THgs.DrawLine(Bpen, 62, 400, 1508, 400)  'bottom
            Dim nfont As New System.Drawing.Font("arial", 16, FontStyle.Regular)
            THgs.DrawString(tnll.ToString("h:mm tt..d MMM yy"), nfont, Brushes.Cyan, 58, 401)
            THgs.DrawString(tn.ToString("h:mm tt..d MMM yy"), nfont, Brushes.Cyan, 1310, 401)
            THgs.DrawString(tnll.ToString("h:mm tt..d MMM yy"), nfont, Brushes.Cyan, 58, 0)
            THgs.DrawString(tn.ToString("h:mm tt..d MMM yy"), nfont, Brushes.Cyan, 1310, 0)
            For i = 1439 To 1 Step -1
                If (stemperatureOUT(i - 1) > 1023 Or stemperatureOUT(i) > 1023) Then
                Else
                    THgs.DrawLine(Rpen, 63 + i, 400 - stemperatureOUT(i - 1), 64 + i, 400 - stemperatureOUT(i))
                End If
                If (shumidityOUT(i - 1) > 1023 Or shumidityOUT(i) > 1023) Then
                Else
                    THgs.DrawLine(Gpen, 63 + i, 400 - shumidityOUT(i - 1), 64 + i, 400 - shumidityOUT(i))
                End If
            Next
            THgs.DrawString("50", nfont, Brushes.Crimson, 30, 10)
            THgs.DrawString("35", nfont, Brushes.Crimson, 30, 96)
            THgs.DrawString("20", nfont, Brushes.Crimson, 30, 190)
            THgs.DrawString("5", nfont, Brushes.Crimson, 40, 286)
            THgs.DrawString("-10", nfont, Brushes.Crimson, 20, 380)  'all the left
            THgs.DrawString("50", nfont, Brushes.Crimson, 1508, 10)
            THgs.DrawString("35", nfont, Brushes.Crimson, 1508, 96)
            THgs.DrawString("20", nfont, Brushes.Crimson, 1508, 190)
            THgs.DrawString("5", nfont, Brushes.Crimson, 1508, 286)
            THgs.DrawString("-10", nfont, Brushes.Crimson, 1508, 380)  'all the right
            THgs.DrawString("100", nfont, Brushes.Lime, 20, 30)
            THgs.DrawString("75", nfont, Brushes.Lime, 30, 116)
            THgs.DrawString("50", nfont, Brushes.Lime, 30, 210)
            THgs.DrawString("25", nfont, Brushes.Lime, 30, 306)
            THgs.DrawString("0", nfont, Brushes.Lime, 40, 400)  'all the left
            THgs.DrawString("100", nfont, Brushes.Lime, 1508, 30)
            THgs.DrawString("75", nfont, Brushes.Lime, 1508, 116)
            THgs.DrawString("50", nfont, Brushes.Lime, 1508, 210)
            THgs.DrawString("25", nfont, Brushes.Lime, 1508, 306)
            THgs.DrawString("0", nfont, Brushes.Lime, 1508, 400)  'all the right
            Dim Tcurser As Integer = (cursorTHO / ArduinoWeather.temphumid.Width) * 1570
            If Tcurser < 65 Then Tcurser = 65 'cant go further left
            If Tcurser > 1503 Then Tcurser = 1503 'cant go further right
            THgs.DrawLine(Pens.White, Tcurser, 22, Tcurser, 400)
            Dim dtemp = (temperatureOUT(Tcurser - 65) / 17.06 - 10).ToString("f1")
            If dtemp > 41 Then dtemp = 0
            Dim dhumid = (humidityOUT(Tcurser - 65) / 10.23).ToString("f0")
            If dhumid > 101 Then dhumid = 0 'for blank case
            THgs.DrawString(("humidity = " & dhumid & "%"), nfont, Brushes.Lime, 358, 0)
            THgs.DrawString(("humidity = " & dhumid & "%"), nfont, Brushes.Lime, 358, 401)
            THgs.DrawString(("temperature = " & dtemp & "C"), nfont, Brushes.Crimson, 1000, 0)
            THgs.DrawString(("temperature = " & dtemp & "C"), nfont, Brushes.Crimson, 1000, 401)
            Dim curtime = TimeSpan.FromMinutes(display_span_time.TotalMinutes - (display_span_time.TotalMinutes * (Tcurser - 65) / 1438))
            Dim tnl = tn.Subtract(curtime)  'graph mid time at cursor
            THgs.DrawString(tnl.ToString("h:mm tt..d MMM yy"), nfont, Brushes.Cyan, 650, 0)  'top
            THgs.DrawString(tnl.ToString("h:mm tt..d MMM yy"), nfont, Brushes.Cyan, 650, 401)  'bottom
            If topdisplay = "THout" Then
                ArduinoWeather.temphumid.Image = scale_THout
            End If
            If middledisplay = "THout" Then
                ArduinoWeather.airrain.Image = scale_THout
            End If
            If bottomdisplay = "THout" Then
                ArduinoWeather.wind.Image = scale_THout
            End If
        Catch ex As Exception
        End Try
    End Sub

    Public Sub Sfull_RainFv() 'RainFv
        Dim tn = display_start_time 'righthand side current time
        Dim tnll = tn.Subtract(display_span_time)  'graph left side time
        RFVg.FillRectangle(Brushes.Black, 0, 0, 1570, 1070)
        Dim Bpen As New System.Drawing.Pen(Color.Cyan, 3)
        Dim Rpen As New System.Drawing.Pen(Color.Crimson, 3)
        Dim Gpen As New System.Drawing.Pen(Color.Lime, 3)
        Dim Dpen As New System.Drawing.Pen(Color.DodgerBlue, 3)
        RFVg.DrawLine(Bpen, 62, 22, 62, 1045)  'left
        RFVg.DrawLine(Bpen, 1508, 22, 1508, 1045)  'right
        RFVg.DrawLine(Bpen, 62, 22, 1508, 22)   'top
        RFVg.DrawLine(Bpen, 62, 278, 1508, 278)
        RFVg.DrawLine(Bpen, 62, 534, 1508, 534)  'middle
        RFVg.DrawLine(Bpen, 62, 790, 1508, 790)
        RFVg.DrawLine(Bpen, 62, 1045, 1508, 1045)  'bottom
        Dim nfont As New System.Drawing.Font("arial", 16, FontStyle.Regular)
        RFVg.DrawString(tnll.ToString("h:mm tt..d MMM yy"), nfont, Brushes.Cyan, 58, 1046)
        RFVg.DrawString(tn.ToString("h:mm tt..d MMM yy"), nfont, Brushes.Cyan, 1310, 1046)
        RFVg.DrawString(tnll.ToString("h:mm tt..d MMM yy"), nfont, Brushes.Cyan, 58, 0)
        RFVg.DrawString(tn.ToString("h:mm tt..d MMM yy"), nfont, Brushes.Cyan, 1310, 0)
        For i = 1439 To 1 Step -1
            If (flashes(i - 1) > 1023 Or flashes(i) > 1023) Then
            Else
                RFVg.DrawLine(Rpen, 63 + i, 1045 - flashes(i - 1), 64 + i, 1045 - flashes(i))
            End If
            If (battvolt(i - 1) > 1023 Or battvolt(i) > 1023) Then
            Else
                RFVg.DrawLine(Dpen, 63 + i, 1045 - battvolt(i - 1), 64 + i, 1045 - battvolt(i))
            End If
            If (rain(i - 1) > 1023 Or rain(i) > 1023) Then
            Else
                'RFVg.DrawLine(Gpen, 63 + i, 1045 - rain(i - 1), 64 + i, 1045 - rain(i))
            End If
        Next
        RFVg.DrawString("120", nfont, Brushes.Crimson, 20, 10)
        RFVg.DrawString("18", nfont, Brushes.DodgerBlue, 30, 50)
        RFVg.DrawString("90", nfont, Brushes.Crimson, 30, 260)
        RFVg.DrawString("16", nfont, Brushes.DodgerBlue, 30, 300)
        RFVg.DrawString("60", nfont, Brushes.Crimson, 30, 520)
        RFVg.DrawString("14", nfont, Brushes.DodgerBlue, 30, 560)
        RFVg.DrawString("30", nfont, Brushes.Crimson, 30, 778)
        RFVg.DrawString("12", nfont, Brushes.DodgerBlue, 30, 818)
        RFVg.DrawString("0", nfont, Brushes.Crimson, 40, 1020)  'all the left
        RFVg.DrawString("10", nfont, Brushes.DodgerBlue, 30, 1000)
        RFVg.DrawString("120", nfont, Brushes.Crimson, 1508, 10)
        RFVg.DrawString("18", nfont, Brushes.DodgerBlue, 1508, 50)
        RFVg.DrawString("90", nfont, Brushes.Crimson, 1508, 260)
        RFVg.DrawString("16", nfont, Brushes.DodgerBlue, 1508, 300)
        RFVg.DrawString("60", nfont, Brushes.Crimson, 1508, 520)
        RFVg.DrawString("14", nfont, Brushes.DodgerBlue, 1508, 560)
        RFVg.DrawString("30", nfont, Brushes.Crimson, 1508, 778)
        RFVg.DrawString("12", nfont, Brushes.DodgerBlue, 1508, 818)
        RFVg.DrawString("0", nfont, Brushes.Crimson, 1508, 1020)  'all the right
        RFVg.DrawString("10", nfont, Brushes.DodgerBlue, 1508, 1000)
        RFVg.DrawString("100", nfont, Brushes.Lime, 20, 30)
        RFVg.DrawString("75", nfont, Brushes.Lime, 30, 280)
        RFVg.DrawString("50", nfont, Brushes.Lime, 30, 540)
        RFVg.DrawString("25", nfont, Brushes.Lime, 30, 798)
        RFVg.DrawString("0", nfont, Brushes.Lime, 40, 1040)  'all the left
        RFVg.DrawString("100", nfont, Brushes.Lime, 1508, 30)
        RFVg.DrawString("75", nfont, Brushes.Lime, 1508, 280)
        RFVg.DrawString("50", nfont, Brushes.Lime, 1508, 540)
        RFVg.DrawString("25", nfont, Brushes.Lime, 1508, 798)
        RFVg.DrawString("0", nfont, Brushes.Lime, 1508, 1040)  'all the right
        Dim Tcurser As Integer = (cursorRFV / ArduinoWeather.wind.Width) * 1570
        If Tcurser < 65 Then Tcurser = 65 'cant go further left
        If Tcurser > 1503 Then Tcurser = 1503 'cant go further right
        RFVg.DrawLine(Pens.White, Tcurser, 22, Tcurser, 1045)
        Dim dtemp = (flashes(Tcurser - 65) / 25.6).ToString("f1")
        Dim dhumid = (rain(Tcurser - 65) / 10.23).ToString("f0")
        If dhumid > 101 Then dhumid = 0 'for blank case
        If dtemp > 41 Then dtemp = 0
        RFVg.DrawString(("rainfall = " & dhumid & "mm"), nfont, Brushes.Lime, 358, 0)
        RFVg.DrawString(("rainfall = " & dhumid & "mm"), nfont, Brushes.Lime, 358, 1046)
        RFVg.DrawString(("lightning = " & dtemp & "f/m"), nfont, Brushes.Crimson, 1000, 0)
        RFVg.DrawString(("lightning = " & dtemp & "f/m"), nfont, Brushes.Crimson, 1000, 1046)
        Dim curtime = TimeSpan.FromMinutes(display_span_time.TotalMinutes - (display_span_time.TotalMinutes * (Tcurser - 65) / 1438))
        Dim tnl = tn.Subtract(curtime)  'graph mid time at cursor
        RFVg.DrawString(tnl.ToString("h:mm tt..d MMM yy"), nfont, Brushes.Cyan, 650, 0)  'top
        RFVg.DrawString(tnl.ToString("h:mm tt..d MMM yy"), nfont, Brushes.Cyan, 650, 1046)  'bottom
        If topdisplay = "RainFv" Then
            ArduinoWeather.temphumid.Image = full_RainFv
        End If
        If middledisplay = "RainFv" Then
            ArduinoWeather.airrain.Image = full_RainFv
        End If
        If bottomdisplay = "RainFv" Then
            ArduinoWeather.wind.Image = full_RainFv
        End If
    End Sub

    Public Sub Sscale_RainFv() 'RainFv
        Try
            For i = 0 To 1439
                sflashes(i) = flashes(i) / 2.7
                srain(i) = rain(i) / 2.7  'scale down the display
                sbattvolt(i) = battvolt(i) / 2.7  'scale down the display
            Next
            Dim tn = display_start_time 'righthand side current time
            Dim tnll = tn.Subtract(display_span_time)  'graph left side time
            RFVgs.FillRectangle(Brushes.Black, 0, 0, 1570, 1070)
            Dim Bpen As New System.Drawing.Pen(Color.Cyan, 3)
            Dim Rpen As New System.Drawing.Pen(Color.Crimson, 3)
            Dim Gpen As New System.Drawing.Pen(Color.Lime, 3)
            Dim Dpen As New System.Drawing.Pen(Color.DodgerBlue, 3)
            RFVgs.DrawLine(Bpen, 62, 22, 62, 400)  'left
            RFVgs.DrawLine(Bpen, 1508, 22, 1508, 400)  'right
            RFVgs.DrawLine(Bpen, 62, 22, 1508, 22)   'top
            RFVgs.DrawLine(Bpen, 62, 116, 1508, 116)
            RFVgs.DrawLine(Bpen, 62, 211, 1508, 211)  'middle
            RFVgs.DrawLine(Bpen, 62, 306, 1508, 306)
            RFVgs.DrawLine(Bpen, 62, 400, 1508, 400)  'bottom
            Dim nfont As New System.Drawing.Font("arial", 16, FontStyle.Regular)
            RFVgs.DrawString(tnll.ToString("h:mm tt..d MMM yy"), nfont, Brushes.Cyan, 58, 401)
            RFVgs.DrawString(tn.ToString("h:mm tt..d MMM yy"), nfont, Brushes.Cyan, 1310, 401)
            RFVgs.DrawString(tnll.ToString("h:mm tt..d MMM yy"), nfont, Brushes.Cyan, 58, 0)
            RFVgs.DrawString(tn.ToString("h:mm tt..d MMM yy"), nfont, Brushes.Cyan, 1310, 0)
            For i = 1439 To 1 Step -1
                If (sflashes(i - 1) > 1023 Or sflashes(i) > 1023) Then
                Else
                    RFVgs.DrawLine(Rpen, 63 + i, 400 - sflashes(i - 1), 64 + i, 400 - sflashes(i))
                End If
                If (sbattvolt(i - 1) > 1023 Or sbattvolt(i) > 1023) Then
                Else
                    RFVgs.DrawLine(Dpen, 63 + i, 400 - sbattvolt(i - 1), 64 + i, 400 - sbattvolt(i))
                End If
                If (srain(i - 1) > 1023 Or srain(i) > 1023) Then
                Else
                    ' RFVgs.DrawLine(Gpen, 63 + i, 400 - srain(i - 1), 64 + i, 400 - srain(i))
                End If
            Next
            RFVgs.DrawString("120", nfont, Brushes.Crimson, 20, 10)
            RFVgs.DrawString("18", nfont, Brushes.DodgerBlue, 30, 50)
            RFVgs.DrawString("90", nfont, Brushes.Crimson, 30, 96)
            RFVgs.DrawString("16", nfont, Brushes.DodgerBlue, 30, 136)
            RFVgs.DrawString("60", nfont, Brushes.Crimson, 30, 190)
            RFVgs.DrawString("14", nfont, Brushes.DodgerBlue, 30, 230)
            RFVgs.DrawString("30", nfont, Brushes.Crimson, 30, 286)
            RFVgs.DrawString("12", nfont, Brushes.DodgerBlue, 30, 326)
            RFVgs.DrawString("0", nfont, Brushes.Crimson, 40, 380)  'all the left
            RFVgs.DrawString("10", nfont, Brushes.DodgerBlue, 30, 360)
            RFVgs.DrawString("120", nfont, Brushes.Crimson, 1508, 10)
            RFVgs.DrawString("18", nfont, Brushes.DodgerBlue, 1508, 50)
            RFVgs.DrawString("90", nfont, Brushes.Crimson, 1508, 96)
            RFVgs.DrawString("16", nfont, Brushes.DodgerBlue, 1508, 136)
            RFVgs.DrawString("60", nfont, Brushes.Crimson, 1508, 190)
            RFVgs.DrawString("14", nfont, Brushes.DodgerBlue, 1508, 230)
            RFVgs.DrawString("30", nfont, Brushes.Crimson, 1508, 286)
            RFVgs.DrawString("12", nfont, Brushes.DodgerBlue, 1508, 326)
            RFVgs.DrawString("0", nfont, Brushes.Crimson, 1508, 380)  'all the right
            RFVgs.DrawString("10", nfont, Brushes.DodgerBlue, 1508, 360)
            RFVgs.DrawString("100", nfont, Brushes.Lime, 20, 30)
            RFVgs.DrawString("75", nfont, Brushes.Lime, 30, 116)
            RFVgs.DrawString("50", nfont, Brushes.Lime, 30, 210)
            RFVgs.DrawString("25", nfont, Brushes.Lime, 30, 306)
            RFVgs.DrawString("0", nfont, Brushes.Lime, 40, 400)  'all the left
            RFVgs.DrawString("100", nfont, Brushes.Lime, 1508, 30)
            RFVgs.DrawString("75", nfont, Brushes.Lime, 1508, 116)
            RFVgs.DrawString("50", nfont, Brushes.Lime, 1508, 210)
            RFVgs.DrawString("25", nfont, Brushes.Lime, 1508, 306)
            RFVgs.DrawString("0", nfont, Brushes.Lime, 1508, 400)  'all the right
            Dim Tcurser As Integer = (cursorRFV / ArduinoWeather.temphumid.Width) * 1570
            If Tcurser < 65 Then Tcurser = 65 'cant go further left
            If Tcurser > 1503 Then Tcurser = 1503 'cant go further right
            RFVgs.DrawLine(Pens.White, Tcurser, 22, Tcurser, 400)
            Dim dtemp = (flashes(Tcurser - 65) / 25.6).ToString("f1")
            If dtemp > 41 Then dtemp = 0
            Dim dhumid = (rain(Tcurser - 65) / 10.23).ToString("f0")
            If dhumid > 101 Then dhumid = 0 'for blank case
            RFVgs.DrawString(("rainfall = " & dhumid & "mm"), nfont, Brushes.Lime, 358, 0)
            RFVgs.DrawString(("rainfall = " & dhumid & "mm"), nfont, Brushes.Lime, 358, 401)
            RFVgs.DrawString(("lightning = " & dtemp & "f/m"), nfont, Brushes.Crimson, 1000, 0)
            RFVgs.DrawString(("lightning = " & dtemp & "f/m"), nfont, Brushes.Crimson, 1000, 401)
            Dim curtime = TimeSpan.FromMinutes(display_span_time.TotalMinutes - (display_span_time.TotalMinutes * (Tcurser - 65) / 1438))
            Dim tnl = tn.Subtract(curtime)  'graph mid time at cursor
            RFVgs.DrawString(tnl.ToString("h:mm tt..d MMM yy"), nfont, Brushes.Cyan, 650, 0)  'top
            RFVgs.DrawString(tnl.ToString("h:mm tt..d MMM yy"), nfont, Brushes.Cyan, 650, 401)  'bottom
            ArduinoWeather.wind.Image = WSDscale_image
            If topdisplay = "RainFv" Then
                ArduinoWeather.temphumid.Image = scale_RainFv
            End If
            If middledisplay = "RainFv" Then
                ArduinoWeather.airrain.Image = scale_RainFv
            End If
            If bottomdisplay = "RainFv" Then
                ArduinoWeather.wind.Image = scale_RainFv
            End If
        Catch ex As Exception
        End Try
    End Sub
End Class
