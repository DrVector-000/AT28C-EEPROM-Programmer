namespace AT28CProgrammer
{
    partial class Form1
    {
        /// <summary>
        /// Variabile di progettazione necessaria.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Pulire le risorse in uso.
        /// </summary>
        /// <param name="disposing">ha valore true se le risorse gestite devono essere eliminate, false in caso contrario.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Codice generato da Progettazione Windows Form

        /// <summary>
        /// Metodo necessario per il supporto della finestra di progettazione. Non modificare
        /// il contenuto del metodo con l'editor di codice.
        /// </summary>
        private void InitializeComponent()
        {
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(Form1));
            this.tBInfo = new System.Windows.Forms.TextBox();
            this.bDisconnetti = new System.Windows.Forms.Button();
            this.bConnetti = new System.Windows.Forms.Button();
            this.label1 = new System.Windows.Forms.Label();
            this.cBSerialPorts = new System.Windows.Forms.ComboBox();
            this.pictureBox1 = new System.Windows.Forms.PictureBox();
            this.progressBar1 = new System.Windows.Forms.ProgressBar();
            this.statusStrip1 = new System.Windows.Forms.StatusStrip();
            this.toolStripStatusLabel1 = new System.Windows.Forms.ToolStripStatusLabel();
            this.label2 = new System.Windows.Forms.Label();
            this.cBTipoEEPROM = new System.Windows.Forms.ComboBox();
            this.bRead = new System.Windows.Forms.Button();
            this.bWrite = new System.Windows.Forms.Button();
            this.saveFileDialog1 = new System.Windows.Forms.SaveFileDialog();
            this.openFileDialog1 = new System.Windows.Forms.OpenFileDialog();
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.button2 = new System.Windows.Forms.Button();
            this.button1 = new System.Windows.Forms.Button();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).BeginInit();
            this.statusStrip1.SuspendLayout();
            this.groupBox1.SuspendLayout();
            this.SuspendLayout();
            // 
            // tBInfo
            // 
            this.tBInfo.BackColor = System.Drawing.SystemColors.Info;
            this.tBInfo.Location = new System.Drawing.Point(9, 12);
            this.tBInfo.Multiline = true;
            this.tBInfo.Name = "tBInfo";
            this.tBInfo.ReadOnly = true;
            this.tBInfo.Size = new System.Drawing.Size(280, 297);
            this.tBInfo.TabIndex = 13;
            // 
            // bDisconnetti
            // 
            this.bDisconnetti.Enabled = false;
            this.bDisconnetti.Location = new System.Drawing.Point(415, 55);
            this.bDisconnetti.Name = "bDisconnetti";
            this.bDisconnetti.Size = new System.Drawing.Size(91, 23);
            this.bDisconnetti.TabIndex = 11;
            this.bDisconnetti.Text = "Disconnetti";
            this.bDisconnetti.UseVisualStyleBackColor = true;
            this.bDisconnetti.Click += new System.EventHandler(this.bDisconnetti_Click);
            // 
            // bConnetti
            // 
            this.bConnetti.Enabled = false;
            this.bConnetti.Location = new System.Drawing.Point(315, 55);
            this.bConnetti.Name = "bConnetti";
            this.bConnetti.Size = new System.Drawing.Size(91, 23);
            this.bConnetti.TabIndex = 12;
            this.bConnetti.Text = "Connetti";
            this.bConnetti.UseVisualStyleBackColor = true;
            this.bConnetti.Click += new System.EventHandler(this.bConnetti_Click);
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(312, 12);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(65, 13);
            this.label1.TabIndex = 10;
            this.label1.Text = "Porta seriale";
            // 
            // cBSerialPorts
            // 
            this.cBSerialPorts.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.cBSerialPorts.FormattingEnabled = true;
            this.cBSerialPorts.Location = new System.Drawing.Point(315, 28);
            this.cBSerialPorts.Name = "cBSerialPorts";
            this.cBSerialPorts.Size = new System.Drawing.Size(191, 21);
            this.cBSerialPorts.TabIndex = 9;
            this.cBSerialPorts.SelectedIndexChanged += new System.EventHandler(this.cBSerialPorts_SelectedIndexChanged);
            // 
            // pictureBox1
            // 
            this.pictureBox1.Image = ((System.Drawing.Image)(resources.GetObject("pictureBox1.Image")));
            this.pictureBox1.Location = new System.Drawing.Point(522, 28);
            this.pictureBox1.Name = "pictureBox1";
            this.pictureBox1.Size = new System.Drawing.Size(92, 93);
            this.pictureBox1.TabIndex = 8;
            this.pictureBox1.TabStop = false;
            // 
            // progressBar1
            // 
            this.progressBar1.Location = new System.Drawing.Point(5, 315);
            this.progressBar1.Name = "progressBar1";
            this.progressBar1.Size = new System.Drawing.Size(606, 16);
            this.progressBar1.Step = 1;
            this.progressBar1.TabIndex = 15;
            // 
            // statusStrip1
            // 
            this.statusStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.toolStripStatusLabel1});
            this.statusStrip1.Location = new System.Drawing.Point(0, 334);
            this.statusStrip1.Name = "statusStrip1";
            this.statusStrip1.Size = new System.Drawing.Size(623, 22);
            this.statusStrip1.TabIndex = 14;
            this.statusStrip1.Text = "statusStrip1";
            // 
            // toolStripStatusLabel1
            // 
            this.toolStripStatusLabel1.Name = "toolStripStatusLabel1";
            this.toolStripStatusLabel1.Size = new System.Drawing.Size(142, 17);
            this.toolStripStatusLabel1.Text = "Dispositivo non connesso";
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(312, 163);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(77, 13);
            this.label2.TabIndex = 17;
            this.label2.Text = "Tipo EEPROM";
            // 
            // cBTipoEEPROM
            // 
            this.cBTipoEEPROM.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.cBTipoEEPROM.FormattingEnabled = true;
            this.cBTipoEEPROM.Items.AddRange(new object[] {
            "AT28C64",
            "AT28C64B",
            "AT28C256"});
            this.cBTipoEEPROM.Location = new System.Drawing.Point(315, 179);
            this.cBTipoEEPROM.Name = "cBTipoEEPROM";
            this.cBTipoEEPROM.Size = new System.Drawing.Size(296, 21);
            this.cBTipoEEPROM.TabIndex = 16;
            this.cBTipoEEPROM.SelectedIndexChanged += new System.EventHandler(this.cBTipoEEPROM_SelectedIndexChanged);
            // 
            // bRead
            // 
            this.bRead.Location = new System.Drawing.Point(415, 206);
            this.bRead.Name = "bRead";
            this.bRead.Size = new System.Drawing.Size(92, 23);
            this.bRead.TabIndex = 18;
            this.bRead.Text = "Leggi";
            this.bRead.UseVisualStyleBackColor = true;
            this.bRead.Click += new System.EventHandler(this.bRead_Click);
            // 
            // bWrite
            // 
            this.bWrite.Location = new System.Drawing.Point(513, 206);
            this.bWrite.Name = "bWrite";
            this.bWrite.Size = new System.Drawing.Size(92, 23);
            this.bWrite.TabIndex = 18;
            this.bWrite.Text = "Scrivi";
            this.bWrite.UseVisualStyleBackColor = true;
            this.bWrite.Click += new System.EventHandler(this.bWrite_Click);
            // 
            // saveFileDialog1
            // 
            this.saveFileDialog1.Filter = "File bin|*.bin|Tutti i file|*.*";
            this.saveFileDialog1.Title = "Salva EEPROM";
            // 
            // openFileDialog1
            // 
            this.openFileDialog1.Filter = "File bin|*.bin|Tutti i file|*.*";
            this.openFileDialog1.Title = "Scrivi EEPROM";
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.button2);
            this.groupBox1.Controls.Add(this.button1);
            this.groupBox1.Enabled = false;
            this.groupBox1.Location = new System.Drawing.Point(315, 246);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(296, 54);
            this.groupBox1.TabIndex = 19;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "Software Data Protection";
            // 
            // button2
            // 
            this.button2.Location = new System.Drawing.Point(198, 19);
            this.button2.Name = "button2";
            this.button2.Size = new System.Drawing.Size(92, 23);
            this.button2.TabIndex = 0;
            this.button2.Text = "Disabilita";
            this.button2.UseVisualStyleBackColor = true;
            // 
            // button1
            // 
            this.button1.Location = new System.Drawing.Point(100, 19);
            this.button1.Name = "button1";
            this.button1.Size = new System.Drawing.Size(92, 23);
            this.button1.TabIndex = 0;
            this.button1.Text = "Abilita";
            this.button1.UseVisualStyleBackColor = true;
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(623, 356);
            this.Controls.Add(this.groupBox1);
            this.Controls.Add(this.bWrite);
            this.Controls.Add(this.bRead);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.cBTipoEEPROM);
            this.Controls.Add(this.progressBar1);
            this.Controls.Add(this.statusStrip1);
            this.Controls.Add(this.tBInfo);
            this.Controls.Add(this.bDisconnetti);
            this.Controls.Add(this.bConnetti);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.cBSerialPorts);
            this.Controls.Add(this.pictureBox1);
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.Name = "Form1";
            this.Text = "AT28C Programmer v.1.03";
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).EndInit();
            this.statusStrip1.ResumeLayout(false);
            this.statusStrip1.PerformLayout();
            this.groupBox1.ResumeLayout(false);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.TextBox tBInfo;
        private System.Windows.Forms.Button bDisconnetti;
        private System.Windows.Forms.Button bConnetti;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.ComboBox cBSerialPorts;
        private System.Windows.Forms.PictureBox pictureBox1;
        private System.Windows.Forms.ProgressBar progressBar1;
        private System.Windows.Forms.StatusStrip statusStrip1;
        private System.Windows.Forms.ToolStripStatusLabel toolStripStatusLabel1;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.ComboBox cBTipoEEPROM;
        private System.Windows.Forms.Button bRead;
        private System.Windows.Forms.Button bWrite;
        private System.Windows.Forms.SaveFileDialog saveFileDialog1;
        private System.Windows.Forms.OpenFileDialog openFileDialog1;
        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.Button button2;
        private System.Windows.Forms.Button button1;
    }
}

