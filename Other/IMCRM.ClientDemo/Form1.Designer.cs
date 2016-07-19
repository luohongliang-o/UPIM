namespace IMCRM.ClientDemo
{
    partial class Form1
    {
        /// <summary>
        /// 必需的设计器变量。
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// 清理所有正在使用的资源。
        /// </summary>
        /// <param name="disposing">如果应释放托管资源，为 true；否则为 false。</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows 窗体设计器生成的代码

        /// <summary>
        /// 设计器支持所需的方法 - 不要修改
        /// 使用代码编辑器修改此方法的内容。
        /// </summary>
        private void InitializeComponent()
        {
            this.TxtUserId = new System.Windows.Forms.TextBox();
            this.TxtName = new System.Windows.Forms.TextBox();
            this.BtnLogin = new System.Windows.Forms.Button();
            this.label1 = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.BtnSoftLogin = new System.Windows.Forms.Button();
            this.TxtSoftUser = new System.Windows.Forms.TextBox();
            this.label3 = new System.Windows.Forms.Label();
            this.SuspendLayout();
            // 
            // TxtUserId
            // 
            this.TxtUserId.Location = new System.Drawing.Point(85, 37);
            this.TxtUserId.Name = "TxtUserId";
            this.TxtUserId.Size = new System.Drawing.Size(100, 21);
            this.TxtUserId.TabIndex = 0;
            // 
            // TxtName
            // 
            this.TxtName.Location = new System.Drawing.Point(274, 35);
            this.TxtName.Name = "TxtName";
            this.TxtName.Size = new System.Drawing.Size(100, 21);
            this.TxtName.TabIndex = 1;
            // 
            // BtnLogin
            // 
            this.BtnLogin.Location = new System.Drawing.Point(410, 35);
            this.BtnLogin.Name = "BtnLogin";
            this.BtnLogin.Size = new System.Drawing.Size(75, 23);
            this.BtnLogin.TabIndex = 2;
            this.BtnLogin.Text = "登录";
            this.BtnLogin.UseVisualStyleBackColor = true;
            this.BtnLogin.Click += new System.EventHandler(this.BtnLogin_Click);
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(17, 40);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(47, 12);
            this.label1.TabIndex = 3;
            this.label1.Text = "UserId:";
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(210, 40);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(35, 12);
            this.label2.TabIndex = 4;
            this.label2.Text = "Name:";
            // 
            // BtnSoftLogin
            // 
            this.BtnSoftLogin.Location = new System.Drawing.Point(401, 133);
            this.BtnSoftLogin.Name = "BtnSoftLogin";
            this.BtnSoftLogin.Size = new System.Drawing.Size(75, 23);
            this.BtnSoftLogin.TabIndex = 5;
            this.BtnSoftLogin.Text = "登录";
            this.BtnSoftLogin.UseVisualStyleBackColor = true;
            this.BtnSoftLogin.Click += new System.EventHandler(this.BtnSoftLogin_Click);
            // 
            // TxtSoftUser
            // 
            this.TxtSoftUser.Location = new System.Drawing.Point(85, 133);
            this.TxtSoftUser.Name = "TxtSoftUser";
            this.TxtSoftUser.Size = new System.Drawing.Size(289, 21);
            this.TxtSoftUser.TabIndex = 6;
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(17, 138);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(53, 12);
            this.label3.TabIndex = 7;
            this.label3.Text = "用户名：";
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(507, 252);
            this.Controls.Add(this.label3);
            this.Controls.Add(this.TxtSoftUser);
            this.Controls.Add(this.BtnSoftLogin);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.BtnLogin);
            this.Controls.Add(this.TxtName);
            this.Controls.Add(this.TxtUserId);
            this.Name = "Form1";
            this.Text = "Form1";
            this.Load += new System.EventHandler(this.Form1_Load);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.TextBox TxtUserId;
        private System.Windows.Forms.TextBox TxtName;
        private System.Windows.Forms.Button BtnLogin;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Button BtnSoftLogin;
        private System.Windows.Forms.TextBox TxtSoftUser;
        private System.Windows.Forms.Label label3;
    }
}

