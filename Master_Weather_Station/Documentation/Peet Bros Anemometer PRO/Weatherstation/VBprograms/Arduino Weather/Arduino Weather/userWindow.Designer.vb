<Global.Microsoft.VisualBasic.CompilerServices.DesignerGenerated()> _
Partial Class ArduinoWeather
    Inherits System.Windows.Forms.Form

    'Form overrides dispose to clean up the component list.
    <System.Diagnostics.DebuggerNonUserCode()> _
    Protected Overrides Sub Dispose(ByVal disposing As Boolean)
        Try
            If disposing AndAlso components IsNot Nothing Then
                components.Dispose()
            End If
        Finally
            MyBase.Dispose(disposing)
        End Try
    End Sub

    'Required by the Windows Form Designer
    Private components As System.ComponentModel.IContainer

    'NOTE: The following procedure is required by the Windows Form Designer
    'It can be modified using the Windows Form Designer.  
    'Do not modify it using the code editor.
    <System.Diagnostics.DebuggerStepThrough()> _
    Private Sub InitializeComponent()
        Me.components = New System.ComponentModel.Container
        Dim resources As System.ComponentModel.ComponentResourceManager = New System.ComponentModel.ComponentResourceManager(GetType(ArduinoWeather))
        Me.comdisplay = New System.Windows.Forms.TextBox
        Me.GroupBox1 = New System.Windows.Forms.GroupBox
        Me.RadioButton9 = New System.Windows.Forms.RadioButton
        Me.RadioButton8 = New System.Windows.Forms.RadioButton
        Me.RadioButton7 = New System.Windows.Forms.RadioButton
        Me.RadioButton6 = New System.Windows.Forms.RadioButton
        Me.RadioButton5 = New System.Windows.Forms.RadioButton
        Me.RadioButton4 = New System.Windows.Forms.RadioButton
        Me.RadioButton3 = New System.Windows.Forms.RadioButton
        Me.RadioButton2 = New System.Windows.Forms.RadioButton
        Me.RadioButton1 = New System.Windows.Forms.RadioButton
        Me.temphumid = New System.Windows.Forms.PictureBox
        Me.zoomTH = New System.Windows.Forms.Button
        Me.clickoffset = New System.Windows.Forms.Label
        Me.airrain = New System.Windows.Forms.PictureBox
        Me.display_noconnect = New System.Windows.Forms.Timer(Me.components)
        Me.zoomAR = New System.Windows.Forms.Button
        Me.overflow = New System.Windows.Forms.Label
        Me.displayarchive = New System.Windows.Forms.Button
        Me.archivedisplay = New System.Windows.Forms.ComboBox
        Me.hasbeenoffset = New System.Windows.Forms.Label
        Me.MenuStrip1 = New System.Windows.Forms.MenuStrip
        Me.HelpToolStripMenuItem = New System.Windows.Forms.ToolStripMenuItem
        Me.HelpToolStripMenuItem1 = New System.Windows.Forms.ToolStripMenuItem
        Me.wind = New System.Windows.Forms.PictureBox
        Me.Topselect = New System.Windows.Forms.GroupBox
        Me.RadioButton14 = New System.Windows.Forms.RadioButton
        Me.RadioButton13 = New System.Windows.Forms.RadioButton
        Me.RadioButton12 = New System.Windows.Forms.RadioButton
        Me.RadioButton11 = New System.Windows.Forms.RadioButton
        Me.RadioButton10 = New System.Windows.Forms.RadioButton
        Me.middleselect = New System.Windows.Forms.GroupBox
        Me.RadioButton19 = New System.Windows.Forms.RadioButton
        Me.RadioButton15 = New System.Windows.Forms.RadioButton
        Me.RadioButton16 = New System.Windows.Forms.RadioButton
        Me.RadioButton17 = New System.Windows.Forms.RadioButton
        Me.RadioButton18 = New System.Windows.Forms.RadioButton
        Me.bottomselect = New System.Windows.Forms.GroupBox
        Me.RadioButton20 = New System.Windows.Forms.RadioButton
        Me.RadioButton21 = New System.Windows.Forms.RadioButton
        Me.RadioButton22 = New System.Windows.Forms.RadioButton
        Me.RadioButton23 = New System.Windows.Forms.RadioButton
        Me.RadioButton24 = New System.Windows.Forms.RadioButton
        Me.zoomPL = New System.Windows.Forms.Button
        Me.GroupBox1.SuspendLayout()
        CType(Me.temphumid, System.ComponentModel.ISupportInitialize).BeginInit()
        CType(Me.airrain, System.ComponentModel.ISupportInitialize).BeginInit()
        Me.MenuStrip1.SuspendLayout()
        CType(Me.wind, System.ComponentModel.ISupportInitialize).BeginInit()
        Me.Topselect.SuspendLayout()
        Me.middleselect.SuspendLayout()
        Me.bottomselect.SuspendLayout()
        Me.SuspendLayout()
        '
        'comdisplay
        '
        Me.comdisplay.BackColor = System.Drawing.SystemColors.ControlLightLight
        Me.comdisplay.Location = New System.Drawing.Point(4, 27)
        Me.comdisplay.Multiline = True
        Me.comdisplay.Name = "comdisplay"
        Me.comdisplay.ReadOnly = True
        Me.comdisplay.Size = New System.Drawing.Size(102, 16)
        Me.comdisplay.TabIndex = 3
        '
        'GroupBox1
        '
        Me.GroupBox1.Controls.Add(Me.RadioButton9)
        Me.GroupBox1.Controls.Add(Me.RadioButton8)
        Me.GroupBox1.Controls.Add(Me.RadioButton7)
        Me.GroupBox1.Controls.Add(Me.RadioButton6)
        Me.GroupBox1.Controls.Add(Me.RadioButton5)
        Me.GroupBox1.Controls.Add(Me.RadioButton4)
        Me.GroupBox1.Controls.Add(Me.RadioButton3)
        Me.GroupBox1.Controls.Add(Me.RadioButton2)
        Me.GroupBox1.Controls.Add(Me.RadioButton1)
        Me.GroupBox1.Location = New System.Drawing.Point(4, 461)
        Me.GroupBox1.Name = "GroupBox1"
        Me.GroupBox1.Size = New System.Drawing.Size(102, 231)
        Me.GroupBox1.TabIndex = 7
        Me.GroupBox1.TabStop = False
        Me.GroupBox1.Text = "display span"
        '
        'RadioButton9
        '
        Me.RadioButton9.AutoSize = True
        Me.RadioButton9.Location = New System.Drawing.Point(11, 203)
        Me.RadioButton9.Name = "RadioButton9"
        Me.RadioButton9.Size = New System.Drawing.Size(57, 17)
        Me.RadioButton9.TabIndex = 10
        Me.RadioButton9.TabStop = True
        Me.RadioButton9.Text = "4week"
        Me.RadioButton9.UseVisualStyleBackColor = True
        '
        'RadioButton8
        '
        Me.RadioButton8.AutoSize = True
        Me.RadioButton8.Location = New System.Drawing.Point(11, 180)
        Me.RadioButton8.Name = "RadioButton8"
        Me.RadioButton8.Size = New System.Drawing.Size(60, 17)
        Me.RadioButton8.TabIndex = 9
        Me.RadioButton8.TabStop = True
        Me.RadioButton8.Text = "2 week"
        Me.RadioButton8.UseVisualStyleBackColor = True
        '
        'RadioButton7
        '
        Me.RadioButton7.AutoSize = True
        Me.RadioButton7.Location = New System.Drawing.Point(12, 157)
        Me.RadioButton7.Name = "RadioButton7"
        Me.RadioButton7.Size = New System.Drawing.Size(51, 17)
        Me.RadioButton7.TabIndex = 8
        Me.RadioButton7.TabStop = True
        Me.RadioButton7.Text = "7 day"
        Me.RadioButton7.UseVisualStyleBackColor = True
        '
        'RadioButton6
        '
        Me.RadioButton6.AutoSize = True
        Me.RadioButton6.Location = New System.Drawing.Point(12, 134)
        Me.RadioButton6.Name = "RadioButton6"
        Me.RadioButton6.Size = New System.Drawing.Size(51, 17)
        Me.RadioButton6.TabIndex = 7
        Me.RadioButton6.Text = "4 day"
        Me.RadioButton6.UseVisualStyleBackColor = True
        '
        'RadioButton5
        '
        Me.RadioButton5.AutoSize = True
        Me.RadioButton5.Location = New System.Drawing.Point(12, 111)
        Me.RadioButton5.Name = "RadioButton5"
        Me.RadioButton5.Size = New System.Drawing.Size(51, 17)
        Me.RadioButton5.TabIndex = 6
        Me.RadioButton5.TabStop = True
        Me.RadioButton5.Text = "2 day"
        Me.RadioButton5.UseVisualStyleBackColor = True
        '
        'RadioButton4
        '
        Me.RadioButton4.AutoSize = True
        Me.RadioButton4.Location = New System.Drawing.Point(11, 88)
        Me.RadioButton4.Name = "RadioButton4"
        Me.RadioButton4.Size = New System.Drawing.Size(61, 17)
        Me.RadioButton4.TabIndex = 5
        Me.RadioButton4.TabStop = True
        Me.RadioButton4.Text = "24 hour"
        Me.RadioButton4.UseVisualStyleBackColor = True
        '
        'RadioButton3
        '
        Me.RadioButton3.AutoSize = True
        Me.RadioButton3.Location = New System.Drawing.Point(11, 65)
        Me.RadioButton3.Name = "RadioButton3"
        Me.RadioButton3.Size = New System.Drawing.Size(61, 17)
        Me.RadioButton3.TabIndex = 4
        Me.RadioButton3.TabStop = True
        Me.RadioButton3.Text = "12 hour"
        Me.RadioButton3.UseVisualStyleBackColor = True
        '
        'RadioButton2
        '
        Me.RadioButton2.AutoSize = True
        Me.RadioButton2.Location = New System.Drawing.Point(12, 41)
        Me.RadioButton2.Name = "RadioButton2"
        Me.RadioButton2.Size = New System.Drawing.Size(55, 17)
        Me.RadioButton2.TabIndex = 3
        Me.RadioButton2.TabStop = True
        Me.RadioButton2.Text = "8 hour"
        Me.RadioButton2.UseVisualStyleBackColor = True
        '
        'RadioButton1
        '
        Me.RadioButton1.AutoSize = True
        Me.RadioButton1.Checked = True
        Me.RadioButton1.Location = New System.Drawing.Point(12, 18)
        Me.RadioButton1.Name = "RadioButton1"
        Me.RadioButton1.Size = New System.Drawing.Size(55, 17)
        Me.RadioButton1.TabIndex = 2
        Me.RadioButton1.TabStop = True
        Me.RadioButton1.Text = "4 hour"
        Me.RadioButton1.UseVisualStyleBackColor = True
        '
        'temphumid
        '
        Me.temphumid.Cursor = System.Windows.Forms.Cursors.SizeNS
        Me.temphumid.Location = New System.Drawing.Point(119, 6)
        Me.temphumid.Name = "temphumid"
        Me.temphumid.Size = New System.Drawing.Size(885, 212)
        Me.temphumid.SizeMode = System.Windows.Forms.PictureBoxSizeMode.StretchImage
        Me.temphumid.TabIndex = 8
        Me.temphumid.TabStop = False
        '
        'zoomTH
        '
        Me.zoomTH.Location = New System.Drawing.Point(0, 83)
        Me.zoomTH.Name = "zoomTH"
        Me.zoomTH.Size = New System.Drawing.Size(99, 23)
        Me.zoomTH.TabIndex = 9
        Me.zoomTH.Text = "Full view" & Global.Microsoft.VisualBasic.ChrW(13) & Global.Microsoft.VisualBasic.ChrW(10)
        Me.zoomTH.UseVisualStyleBackColor = True
        '
        'clickoffset
        '
        Me.clickoffset.AutoSize = True
        Me.clickoffset.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me.clickoffset.Location = New System.Drawing.Point(479, 221)
        Me.clickoffset.Name = "clickoffset"
        Me.clickoffset.Size = New System.Drawing.Size(107, 15)
        Me.clickoffset.TabIndex = 10
        Me.clickoffset.Text = "click to offset display"
        '
        'airrain
        '
        Me.airrain.Cursor = System.Windows.Forms.Cursors.SizeNS
        Me.airrain.Location = New System.Drawing.Point(119, 240)
        Me.airrain.Name = "airrain"
        Me.airrain.Size = New System.Drawing.Size(885, 212)
        Me.airrain.SizeMode = System.Windows.Forms.PictureBoxSizeMode.StretchImage
        Me.airrain.TabIndex = 11
        Me.airrain.TabStop = False
        '
        'display_noconnect
        '
        Me.display_noconnect.Interval = 10000
        '
        'zoomAR
        '
        Me.zoomAR.Location = New System.Drawing.Point(1, 87)
        Me.zoomAR.Name = "zoomAR"
        Me.zoomAR.Size = New System.Drawing.Size(98, 24)
        Me.zoomAR.TabIndex = 12
        Me.zoomAR.Text = "Full view"
        Me.zoomAR.UseVisualStyleBackColor = True
        '
        'overflow
        '
        Me.overflow.AutoSize = True
        Me.overflow.Font = New System.Drawing.Font("Microsoft Sans Serif", 8.25!, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.overflow.ForeColor = System.Drawing.Color.Red
        Me.overflow.Location = New System.Drawing.Point(-2, 517)
        Me.overflow.Name = "overflow"
        Me.overflow.Size = New System.Drawing.Size(0, 13)
        Me.overflow.TabIndex = 13
        '
        'displayarchive
        '
        Me.displayarchive.Font = New System.Drawing.Font("Microsoft Sans Serif", 8.25!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.displayarchive.ForeColor = System.Drawing.SystemColors.ControlText
        Me.displayarchive.Location = New System.Drawing.Point(4, 396)
        Me.displayarchive.Name = "displayarchive"
        Me.displayarchive.Size = New System.Drawing.Size(102, 34)
        Me.displayarchive.TabIndex = 14
        Me.displayarchive.Text = "display" & Global.Microsoft.VisualBasic.ChrW(13) & Global.Microsoft.VisualBasic.ChrW(10) & "archived file"
        Me.displayarchive.UseVisualStyleBackColor = True
        '
        'archivedisplay
        '
        Me.archivedisplay.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList
        Me.archivedisplay.Font = New System.Drawing.Font("Microsoft Sans Serif", 8.25!, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.archivedisplay.FormattingEnabled = True
        Me.archivedisplay.Location = New System.Drawing.Point(4, 436)
        Me.archivedisplay.Name = "archivedisplay"
        Me.archivedisplay.Size = New System.Drawing.Size(102, 21)
        Me.archivedisplay.TabIndex = 15
        '
        'hasbeenoffset
        '
        Me.hasbeenoffset.AutoSize = True
        Me.hasbeenoffset.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D
        Me.hasbeenoffset.ForeColor = System.Drawing.Color.Red
        Me.hasbeenoffset.Location = New System.Drawing.Point(705, 221)
        Me.hasbeenoffset.Name = "hasbeenoffset"
        Me.hasbeenoffset.Size = New System.Drawing.Size(85, 15)
        Me.hasbeenoffset.TabIndex = 16
        Me.hasbeenoffset.Text = "display offset to "
        '
        'MenuStrip1
        '
        Me.MenuStrip1.Items.AddRange(New System.Windows.Forms.ToolStripItem() {Me.HelpToolStripMenuItem, Me.HelpToolStripMenuItem1})
        Me.MenuStrip1.Location = New System.Drawing.Point(0, 0)
        Me.MenuStrip1.Name = "MenuStrip1"
        Me.MenuStrip1.Size = New System.Drawing.Size(1016, 24)
        Me.MenuStrip1.TabIndex = 17
        Me.MenuStrip1.Text = "MenuStrip1"
        '
        'HelpToolStripMenuItem
        '
        Me.HelpToolStripMenuItem.Name = "HelpToolStripMenuItem"
        Me.HelpToolStripMenuItem.Size = New System.Drawing.Size(42, 20)
        Me.HelpToolStripMenuItem.Text = "Files"
        '
        'HelpToolStripMenuItem1
        '
        Me.HelpToolStripMenuItem1.Name = "HelpToolStripMenuItem1"
        Me.HelpToolStripMenuItem1.Size = New System.Drawing.Size(42, 20)
        Me.HelpToolStripMenuItem1.Text = "help"
        '
        'wind
        '
        Me.wind.Location = New System.Drawing.Point(119, 470)
        Me.wind.Name = "wind"
        Me.wind.Size = New System.Drawing.Size(885, 212)
        Me.wind.SizeMode = System.Windows.Forms.PictureBoxSizeMode.StretchImage
        Me.wind.TabIndex = 18
        Me.wind.TabStop = False
        '
        'Topselect
        '
        Me.Topselect.Controls.Add(Me.RadioButton14)
        Me.Topselect.Controls.Add(Me.RadioButton13)
        Me.Topselect.Controls.Add(Me.RadioButton12)
        Me.Topselect.Controls.Add(Me.RadioButton11)
        Me.Topselect.Controls.Add(Me.RadioButton10)
        Me.Topselect.Controls.Add(Me.zoomTH)
        Me.Topselect.Location = New System.Drawing.Point(6, 49)
        Me.Topselect.Name = "Topselect"
        Me.Topselect.Size = New System.Drawing.Size(109, 106)
        Me.Topselect.TabIndex = 19
        Me.Topselect.TabStop = False
        Me.Topselect.Text = "Top"
        '
        'RadioButton14
        '
        Me.RadioButton14.AutoSize = True
        Me.RadioButton14.Location = New System.Drawing.Point(0, 64)
        Me.RadioButton14.Name = "RadioButton14"
        Me.RadioButton14.Size = New System.Drawing.Size(74, 17)
        Me.RadioButton14.TabIndex = 3
        Me.RadioButton14.Text = "PressLight"
        Me.RadioButton14.UseVisualStyleBackColor = True
        '
        'RadioButton13
        '
        Me.RadioButton13.AutoSize = True
        Me.RadioButton13.Location = New System.Drawing.Point(47, 43)
        Me.RadioButton13.Name = "RadioButton13"
        Me.RadioButton13.Size = New System.Drawing.Size(59, 17)
        Me.RadioButton13.TabIndex = 3
        Me.RadioButton13.Text = "RainFv"
        Me.RadioButton13.UseVisualStyleBackColor = True
        '
        'RadioButton12
        '
        Me.RadioButton12.AutoSize = True
        Me.RadioButton12.Location = New System.Drawing.Point(0, 43)
        Me.RadioButton12.Name = "RadioButton12"
        Me.RadioButton12.Size = New System.Drawing.Size(50, 17)
        Me.RadioButton12.TabIndex = 2
        Me.RadioButton12.Text = "Wind"
        Me.RadioButton12.UseVisualStyleBackColor = True
        '
        'RadioButton11
        '
        Me.RadioButton11.AutoSize = True
        Me.RadioButton11.Location = New System.Drawing.Point(48, 19)
        Me.RadioButton11.Name = "RadioButton11"
        Me.RadioButton11.Size = New System.Drawing.Size(55, 17)
        Me.RadioButton11.TabIndex = 1
        Me.RadioButton11.Text = "THout"
        Me.RadioButton11.UseVisualStyleBackColor = True
        '
        'RadioButton10
        '
        Me.RadioButton10.AutoSize = True
        Me.RadioButton10.Checked = True
        Me.RadioButton10.Location = New System.Drawing.Point(0, 19)
        Me.RadioButton10.Name = "RadioButton10"
        Me.RadioButton10.Size = New System.Drawing.Size(48, 17)
        Me.RadioButton10.TabIndex = 0
        Me.RadioButton10.TabStop = True
        Me.RadioButton10.Text = "THin"
        Me.RadioButton10.UseVisualStyleBackColor = True
        '
        'middleselect
        '
        Me.middleselect.Controls.Add(Me.RadioButton19)
        Me.middleselect.Controls.Add(Me.RadioButton15)
        Me.middleselect.Controls.Add(Me.RadioButton16)
        Me.middleselect.Controls.Add(Me.RadioButton17)
        Me.middleselect.Controls.Add(Me.RadioButton18)
        Me.middleselect.Controls.Add(Me.zoomAR)
        Me.middleselect.Location = New System.Drawing.Point(5, 162)
        Me.middleselect.Name = "middleselect"
        Me.middleselect.Size = New System.Drawing.Size(109, 117)
        Me.middleselect.TabIndex = 20
        Me.middleselect.TabStop = False
        Me.middleselect.Text = "Middle"
        '
        'RadioButton19
        '
        Me.RadioButton19.AutoSize = True
        Me.RadioButton19.Location = New System.Drawing.Point(49, 46)
        Me.RadioButton19.Name = "RadioButton19"
        Me.RadioButton19.Size = New System.Drawing.Size(59, 17)
        Me.RadioButton19.TabIndex = 13
        Me.RadioButton19.Text = "RainFv"
        Me.RadioButton19.UseVisualStyleBackColor = True
        '
        'RadioButton15
        '
        Me.RadioButton15.AutoSize = True
        Me.RadioButton15.Checked = True
        Me.RadioButton15.Location = New System.Drawing.Point(0, 64)
        Me.RadioButton15.Name = "RadioButton15"
        Me.RadioButton15.Size = New System.Drawing.Size(74, 17)
        Me.RadioButton15.TabIndex = 3
        Me.RadioButton15.TabStop = True
        Me.RadioButton15.Text = "PressLight"
        Me.RadioButton15.UseVisualStyleBackColor = True
        '
        'RadioButton16
        '
        Me.RadioButton16.AutoSize = True
        Me.RadioButton16.Location = New System.Drawing.Point(0, 43)
        Me.RadioButton16.Name = "RadioButton16"
        Me.RadioButton16.Size = New System.Drawing.Size(50, 17)
        Me.RadioButton16.TabIndex = 2
        Me.RadioButton16.Text = "Wind"
        Me.RadioButton16.UseVisualStyleBackColor = True
        '
        'RadioButton17
        '
        Me.RadioButton17.AutoSize = True
        Me.RadioButton17.Location = New System.Drawing.Point(48, 19)
        Me.RadioButton17.Name = "RadioButton17"
        Me.RadioButton17.Size = New System.Drawing.Size(55, 17)
        Me.RadioButton17.TabIndex = 1
        Me.RadioButton17.Text = "THout"
        Me.RadioButton17.UseVisualStyleBackColor = True
        '
        'RadioButton18
        '
        Me.RadioButton18.AutoSize = True
        Me.RadioButton18.Location = New System.Drawing.Point(0, 19)
        Me.RadioButton18.Name = "RadioButton18"
        Me.RadioButton18.Size = New System.Drawing.Size(48, 17)
        Me.RadioButton18.TabIndex = 0
        Me.RadioButton18.Text = "THin"
        Me.RadioButton18.UseVisualStyleBackColor = True
        '
        'bottomselect
        '
        Me.bottomselect.Controls.Add(Me.RadioButton20)
        Me.bottomselect.Controls.Add(Me.RadioButton21)
        Me.bottomselect.Controls.Add(Me.RadioButton22)
        Me.bottomselect.Controls.Add(Me.RadioButton23)
        Me.bottomselect.Controls.Add(Me.RadioButton24)
        Me.bottomselect.Controls.Add(Me.zoomPL)
        Me.bottomselect.Location = New System.Drawing.Point(4, 279)
        Me.bottomselect.Name = "bottomselect"
        Me.bottomselect.Size = New System.Drawing.Size(109, 117)
        Me.bottomselect.TabIndex = 21
        Me.bottomselect.TabStop = False
        Me.bottomselect.Text = "Bottom"
        '
        'RadioButton20
        '
        Me.RadioButton20.AutoSize = True
        Me.RadioButton20.Location = New System.Drawing.Point(49, 46)
        Me.RadioButton20.Name = "RadioButton20"
        Me.RadioButton20.Size = New System.Drawing.Size(59, 17)
        Me.RadioButton20.TabIndex = 13
        Me.RadioButton20.Text = "RainFv"
        Me.RadioButton20.UseVisualStyleBackColor = True
        '
        'RadioButton21
        '
        Me.RadioButton21.AutoSize = True
        Me.RadioButton21.Location = New System.Drawing.Point(0, 64)
        Me.RadioButton21.Name = "RadioButton21"
        Me.RadioButton21.Size = New System.Drawing.Size(74, 17)
        Me.RadioButton21.TabIndex = 3
        Me.RadioButton21.Text = "PressLight"
        Me.RadioButton21.UseVisualStyleBackColor = True
        '
        'RadioButton22
        '
        Me.RadioButton22.AutoSize = True
        Me.RadioButton22.Checked = True
        Me.RadioButton22.Location = New System.Drawing.Point(0, 43)
        Me.RadioButton22.Name = "RadioButton22"
        Me.RadioButton22.Size = New System.Drawing.Size(50, 17)
        Me.RadioButton22.TabIndex = 2
        Me.RadioButton22.TabStop = True
        Me.RadioButton22.Text = "Wind"
        Me.RadioButton22.UseVisualStyleBackColor = True
        '
        'RadioButton23
        '
        Me.RadioButton23.AutoSize = True
        Me.RadioButton23.Location = New System.Drawing.Point(48, 19)
        Me.RadioButton23.Name = "RadioButton23"
        Me.RadioButton23.Size = New System.Drawing.Size(55, 17)
        Me.RadioButton23.TabIndex = 1
        Me.RadioButton23.Text = "THout"
        Me.RadioButton23.UseVisualStyleBackColor = True
        '
        'RadioButton24
        '
        Me.RadioButton24.AutoSize = True
        Me.RadioButton24.Location = New System.Drawing.Point(0, 19)
        Me.RadioButton24.Name = "RadioButton24"
        Me.RadioButton24.Size = New System.Drawing.Size(48, 17)
        Me.RadioButton24.TabIndex = 0
        Me.RadioButton24.Text = "THin"
        Me.RadioButton24.UseVisualStyleBackColor = True
        '
        'zoomPL
        '
        Me.zoomPL.Location = New System.Drawing.Point(1, 87)
        Me.zoomPL.Name = "zoomPL"
        Me.zoomPL.Size = New System.Drawing.Size(98, 24)
        Me.zoomPL.TabIndex = 12
        Me.zoomPL.Text = "Full view"
        Me.zoomPL.UseVisualStyleBackColor = True
        '
        'ArduinoWeather
        '
        Me.AutoScaleDimensions = New System.Drawing.SizeF(6.0!, 13.0!)
        Me.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font
        Me.ClientSize = New System.Drawing.Size(1016, 692)
        Me.Controls.Add(Me.bottomselect)
        Me.Controls.Add(Me.middleselect)
        Me.Controls.Add(Me.Topselect)
        Me.Controls.Add(Me.wind)
        Me.Controls.Add(Me.hasbeenoffset)
        Me.Controls.Add(Me.archivedisplay)
        Me.Controls.Add(Me.displayarchive)
        Me.Controls.Add(Me.overflow)
        Me.Controls.Add(Me.airrain)
        Me.Controls.Add(Me.clickoffset)
        Me.Controls.Add(Me.temphumid)
        Me.Controls.Add(Me.GroupBox1)
        Me.Controls.Add(Me.comdisplay)
        Me.Controls.Add(Me.MenuStrip1)
        Me.Icon = CType(resources.GetObject("$this.Icon"), System.Drawing.Icon)
        Me.MainMenuStrip = Me.MenuStrip1
        Me.Name = "ArduinoWeather"
        Me.StartPosition = System.Windows.Forms.FormStartPosition.Manual
        Me.Text = "Arduino Weather"
        Me.GroupBox1.ResumeLayout(False)
        Me.GroupBox1.PerformLayout()
        CType(Me.temphumid, System.ComponentModel.ISupportInitialize).EndInit()
        CType(Me.airrain, System.ComponentModel.ISupportInitialize).EndInit()
        Me.MenuStrip1.ResumeLayout(False)
        Me.MenuStrip1.PerformLayout()
        CType(Me.wind, System.ComponentModel.ISupportInitialize).EndInit()
        Me.Topselect.ResumeLayout(False)
        Me.Topselect.PerformLayout()
        Me.middleselect.ResumeLayout(False)
        Me.middleselect.PerformLayout()
        Me.bottomselect.ResumeLayout(False)
        Me.bottomselect.PerformLayout()
        Me.ResumeLayout(False)
        Me.PerformLayout()

    End Sub
    Friend WithEvents comdisplay As System.Windows.Forms.TextBox
    Friend WithEvents GroupBox1 As System.Windows.Forms.GroupBox
    Friend WithEvents RadioButton9 As System.Windows.Forms.RadioButton
    Friend WithEvents RadioButton8 As System.Windows.Forms.RadioButton
    Friend WithEvents RadioButton7 As System.Windows.Forms.RadioButton
    Friend WithEvents RadioButton6 As System.Windows.Forms.RadioButton
    Friend WithEvents RadioButton5 As System.Windows.Forms.RadioButton
    Friend WithEvents RadioButton4 As System.Windows.Forms.RadioButton
    Friend WithEvents RadioButton3 As System.Windows.Forms.RadioButton
    Friend WithEvents RadioButton2 As System.Windows.Forms.RadioButton
    Friend WithEvents RadioButton1 As System.Windows.Forms.RadioButton
    Friend WithEvents temphumid As System.Windows.Forms.PictureBox
    Friend WithEvents zoomTH As System.Windows.Forms.Button
    Friend WithEvents clickoffset As System.Windows.Forms.Label
    Friend WithEvents airrain As System.Windows.Forms.PictureBox
    Friend WithEvents display_noconnect As System.Windows.Forms.Timer
    Friend WithEvents zoomAR As System.Windows.Forms.Button
    Friend WithEvents overflow As System.Windows.Forms.Label
    Friend WithEvents displayarchive As System.Windows.Forms.Button
    Friend WithEvents archivedisplay As System.Windows.Forms.ComboBox
    Friend WithEvents hasbeenoffset As System.Windows.Forms.Label
    Friend WithEvents MenuStrip1 As System.Windows.Forms.MenuStrip
    Friend WithEvents HelpToolStripMenuItem As System.Windows.Forms.ToolStripMenuItem
    Friend WithEvents HelpToolStripMenuItem1 As System.Windows.Forms.ToolStripMenuItem
    Friend WithEvents wind As System.Windows.Forms.PictureBox
    Friend WithEvents Topselect As System.Windows.Forms.GroupBox
    Friend WithEvents RadioButton10 As System.Windows.Forms.RadioButton
    Friend WithEvents RadioButton11 As System.Windows.Forms.RadioButton
    Friend WithEvents RadioButton14 As System.Windows.Forms.RadioButton
    Friend WithEvents RadioButton12 As System.Windows.Forms.RadioButton
    Friend WithEvents RadioButton13 As System.Windows.Forms.RadioButton
    Friend WithEvents middleselect As System.Windows.Forms.GroupBox
    Friend WithEvents RadioButton15 As System.Windows.Forms.RadioButton
    Friend WithEvents RadioButton16 As System.Windows.Forms.RadioButton
    Friend WithEvents RadioButton17 As System.Windows.Forms.RadioButton
    Friend WithEvents RadioButton18 As System.Windows.Forms.RadioButton
    Friend WithEvents RadioButton19 As System.Windows.Forms.RadioButton
    Friend WithEvents bottomselect As System.Windows.Forms.GroupBox
    Friend WithEvents RadioButton20 As System.Windows.Forms.RadioButton
    Friend WithEvents RadioButton21 As System.Windows.Forms.RadioButton
    Friend WithEvents RadioButton22 As System.Windows.Forms.RadioButton
    Friend WithEvents RadioButton23 As System.Windows.Forms.RadioButton
    Friend WithEvents RadioButton24 As System.Windows.Forms.RadioButton
    Friend WithEvents zoomPL As System.Windows.Forms.Button

End Class
