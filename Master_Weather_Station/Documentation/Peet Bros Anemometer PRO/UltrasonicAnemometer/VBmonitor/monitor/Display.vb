
Public Class Display   
    Private part As New Bitmap(1570, 1070, Drawing.Imaging.PixelFormat.Format24bppRgb)
    Public display_temp(1439) As Int16  'only 0 to 1023 possible
    Public dt As String = ""  'the time used on the current display
    Public start As Integer = 0
    Public final As Integer = 0
    Public maxT As Integer = 0
    Public minT As Integer = 0
    Public display_north(1439) As Int16  'only 0 to 1023 possible
    Public maxN As Integer = 0
    Public minN As Integer = 0
    Public display_south(1439) As Int16  'only 0 to 1023 possible
    Public display_nsdiff(1439) As Int16  'only 0 to 1023 possible
    Public display_nserror(1439) As Int16  'only 0 to 1023 possible
    Public maxS As Integer = 0
    Public minS As Integer = 0
    Public display_east(1439) As Int16  'only 0 to 1023 possible
    Public display_west(1439) As Int16  'only 0 to 1023 possible
    Public Sub show_now()
        Dim tn As String = start
        Dim te As String = final
        Dim tm As String = CType((final - start) / 2 + start, Integer)
        Dim midT As Integer = (maxT - minT) / 2 + minT
        Dim midN As Integer = (maxN - minN) / 2 + minN
        Dim g As Graphics = Graphics.FromImage(part)
        g.FillRectangle(Brushes.Black, 0, 0, 1570, 1070)
        Dim Bpen As New System.Drawing.Pen(Color.Cyan, 3)
        Dim Rpen As New System.Drawing.Pen(Color.Crimson, 3)
        Dim Mpen As New System.Drawing.Pen(Color.DarkMagenta, 3)
        Dim Gpen As New System.Drawing.Pen(Color.LimeGreen, 3)
        Dim Open As New System.Drawing.Pen(Color.DarkOrange, 3)
        Dim Dpen As New System.Drawing.Pen(Color.DodgerBlue, 3)
        Dim nsdpen As New System.Drawing.Pen(Color.Gold, 3)
        Dim nsepen As New System.Drawing.Pen(Color.Lime, 3)
        g.DrawLine(Bpen, 62, 22, 62, 1040)
        g.DrawLine(Bpen, 1508, 22, 1508, 1040)
        g.DrawLine(Bpen, 785, 22, 785, 1040)
        g.DrawLine(Bpen, 62, 22, 1508, 22)
        g.DrawLine(Bpen, 62, 1040, 1508, 1040)
        g.DrawLine(nsdpen, 62, 240, 1508, 240) ' zero line

        Dim nfont As New System.Drawing.Font("arial", 16, FontStyle.Regular)
        g.DrawString("0", nfont, Brushes.DarkOrange, 40, 230)
        g.DrawString(tn, nfont, Brushes.Cyan, 58, 1041)
        g.DrawString(te, nfont, Brushes.Cyan, 1464, 1041)
        g.DrawString(tm, nfont, Brushes.Cyan, 765, 1041)
        g.DrawString(tn, nfont, Brushes.Cyan, 58, 0)
        g.DrawString(te, nfont, Brushes.Cyan, 1464, 0)
        g.DrawString(tm, nfont, Brushes.Cyan, 765, 0)

        For i = 1 To 23
            g.DrawLine(Pens.Cyan, 62 + 60 * i, 22, 62 + 60 * i, 1040)
        Next
        For i = 0 To 1438
            If display_temp(i + 1) = 0 Then Exit For 'only display the real samples
            g.DrawLine(Rpen, 62 + i, 1040 - display_temp(i), 63 + i, 1040 - display_temp(i + 1))
        Next
        g.DrawString(maxT, nfont, Brushes.Crimson, 0, 15)
        g.DrawString(midT, nfont, Brushes.Crimson, 0, 520)
        g.DrawString(minT, nfont, Brushes.Crimson, 0, 1020)
        g.DrawString(maxT, nfont, Brushes.Crimson, 1508, 15)
        g.DrawString(midT, nfont, Brushes.Crimson, 1508, 520)
        g.DrawString(minT, nfont, Brushes.Crimson, 1508, 1020)
        g.DrawLine(Bpen, 62, 531, 1508, 531)  'middle y line
        For i = 1 To 40  'y lines
            g.DrawLine(Pens.Cyan, 62, CInt(22 + 25.4 * i), 1508, CInt(22 + 25.4 * i))
        Next
        g.DrawString(maxN, nfont, Brushes.DarkOrange, 0, 40)
        g.DrawString(midN, nfont, Brushes.DarkOrange, 0, 545)
        g.DrawString(minN, nfont, Brushes.DarkOrange, 0, 1045)
        g.DrawString(maxN, nfont, Brushes.DarkOrange, 1508, 40)
        g.DrawString(midN, nfont, Brushes.DarkOrange, 1508, 545)
        g.DrawString(minN, nfont, Brushes.DarkOrange, 1508, 1045)
        For i = 0 To 1438
            If display_north(i + 1) = 0 Then Exit For 'only display the real samples
            g.DrawLine(Open, 62 + i, 1040 - display_north(i), 63 + i, 1040 - display_north(i + 1))
        Next
        For i = 0 To 1438
            If display_south(i + 1) = 0 Then Exit For 'only display the real samples
            g.DrawLine(Dpen, 62 + i, 1040 - display_south(i), 63 + i, 1040 - display_south(i + 1))
        Next
        For i = 0 To 1438
            If display_east(i + 1) = 0 Then Exit For 'only display the real samples
            g.DrawLine(Mpen, 62 + i, 1040 - display_east(i), 63 + i, 1040 - display_east(i + 1))
        Next
        For i = 0 To 1438
            If display_west(i + 1) = 0 Then Exit For 'only display the real samples
            g.DrawLine(Gpen, 62 + i, 1040 - display_west(i), 63 + i, 1040 - display_west(i + 1))
        Next
        For i = 0 To 1438
            If display_nsdiff(i + 1) = 0 Then
                display_nsdiff(i + 1) = 800  'the centre line
            End If
            g.DrawLine(nsdpen, 62 + i, 1040 - display_nsdiff(i), 63 + i, 1040 - display_nsdiff(i + 1))
        Next
        For i = 0 To 1438
            If display_nserror(i + 1) = 0 Then
                display_nserror(i + 1) = 800  'the centre line
            End If
            g.DrawLine(nsepen, 62 + i, 1040 - display_nserror(i), 63 + i, 1040 - display_nserror(i + 1))
        Next
        g.Dispose()
        Form1.PictureBox1.Image = part
    End Sub
End Class
