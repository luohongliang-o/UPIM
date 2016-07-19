namespace IMCRM.ClientDemo
{
    partial class FrmMain
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(FrmMain));
            this.skinTabControl1 = new CCWin.SkinControl.SkinTabControl();
            this.skinTabPage1 = new CCWin.SkinControl.SkinTabPage();
            this.clbDDuser = new CCWin.SkinControl.ChatListBox();
            this.skinTabPage2 = new CCWin.SkinControl.SkinTabPage();
            this.ClbSession = new CCWin.SkinControl.ChatListBox();
            this.UserDetialTab = new CCWin.SkinControl.SkinTabControl();
            this.skinTabControl1.SuspendLayout();
            this.skinTabPage1.SuspendLayout();
            this.skinTabPage2.SuspendLayout();
            this.SuspendLayout();
            // 
            // skinTabControl1
            // 
            this.skinTabControl1.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left)));
            this.skinTabControl1.AnimatorType = CCWin.SkinControl.AnimationType.HorizSlide;
            this.skinTabControl1.BackColor = System.Drawing.Color.Gray;
            this.skinTabControl1.CloseRect = new System.Drawing.Rectangle(2, 2, 12, 12);
            this.skinTabControl1.Controls.Add(this.skinTabPage1);
            this.skinTabControl1.Controls.Add(this.skinTabPage2);
            this.skinTabControl1.HeadBack = null;
            this.skinTabControl1.ImgTxtOffset = new System.Drawing.Point(0, 0);
            this.skinTabControl1.ItemSize = new System.Drawing.Size(95, 36);
            this.skinTabControl1.Location = new System.Drawing.Point(2, 31);
            this.skinTabControl1.Name = "skinTabControl1";
            this.skinTabControl1.PageArrowDown = ((System.Drawing.Image)(resources.GetObject("skinTabControl1.PageArrowDown")));
            this.skinTabControl1.PageArrowHover = ((System.Drawing.Image)(resources.GetObject("skinTabControl1.PageArrowHover")));
            this.skinTabControl1.PageCloseHover = ((System.Drawing.Image)(resources.GetObject("skinTabControl1.PageCloseHover")));
            this.skinTabControl1.PageCloseNormal = ((System.Drawing.Image)(resources.GetObject("skinTabControl1.PageCloseNormal")));
            this.skinTabControl1.PageDown = ((System.Drawing.Image)(resources.GetObject("skinTabControl1.PageDown")));
            this.skinTabControl1.PageHover = ((System.Drawing.Image)(resources.GetObject("skinTabControl1.PageHover")));
            this.skinTabControl1.PageImagePosition = CCWin.SkinControl.SkinTabControl.ePageImagePosition.Left;
            this.skinTabControl1.PageNorml = null;
            this.skinTabControl1.SelectedIndex = 1;
            this.skinTabControl1.Size = new System.Drawing.Size(310, 605);
            this.skinTabControl1.SizeMode = System.Windows.Forms.TabSizeMode.Fixed;
            this.skinTabControl1.TabIndex = 0;
            // 
            // skinTabPage1
            // 
            this.skinTabPage1.BackColor = System.Drawing.Color.White;
            this.skinTabPage1.Controls.Add(this.clbDDuser);
            this.skinTabPage1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.skinTabPage1.ForeColor = System.Drawing.Color.White;
            this.skinTabPage1.Location = new System.Drawing.Point(0, 36);
            this.skinTabPage1.Name = "skinTabPage1";
            this.skinTabPage1.Size = new System.Drawing.Size(310, 569);
            this.skinTabPage1.TabIndex = 0;
            this.skinTabPage1.TabItemImage = null;
            this.skinTabPage1.Text = "公众号";
            // 
            // clbDDuser
            // 
            this.clbDDuser.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(50)))), ((int)(((byte)(255)))), ((int)(((byte)(255)))), ((int)(((byte)(255)))));
            this.clbDDuser.Dock = System.Windows.Forms.DockStyle.Fill;
            this.clbDDuser.Font = new System.Drawing.Font("微软雅黑", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.clbDDuser.ForeColor = System.Drawing.Color.Black;
            this.clbDDuser.FriendsMobile = true;
            this.clbDDuser.ListSubItemMenu = null;
            this.clbDDuser.Location = new System.Drawing.Point(0, 0);
            this.clbDDuser.Name = "clbDDuser";
            this.clbDDuser.SelectSubItem = null;
            this.clbDDuser.Size = new System.Drawing.Size(310, 569);
            this.clbDDuser.SubItemMenu = null;
            this.clbDDuser.TabIndex = 0;
            this.clbDDuser.DoubleClickSubItem += new CCWin.SkinControl.ChatListBox.ChatListEventHandler(this.clbDDuser_DoubleClickSubItem);
            // 
            // skinTabPage2
            // 
            this.skinTabPage2.BackColor = System.Drawing.Color.White;
            this.skinTabPage2.Controls.Add(this.ClbSession);
            this.skinTabPage2.Dock = System.Windows.Forms.DockStyle.Fill;
            this.skinTabPage2.Location = new System.Drawing.Point(0, 36);
            this.skinTabPage2.Name = "skinTabPage2";
            this.skinTabPage2.Size = new System.Drawing.Size(310, 569);
            this.skinTabPage2.TabIndex = 1;
            this.skinTabPage2.TabItemImage = null;
            this.skinTabPage2.Text = "会话消息";
            // 
            // ClbSession
            // 
            this.ClbSession.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(50)))), ((int)(((byte)(255)))), ((int)(((byte)(255)))), ((int)(((byte)(255)))));
            this.ClbSession.Font = new System.Drawing.Font("微软雅黑", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(134)));
            this.ClbSession.ForeColor = System.Drawing.Color.Black;
            this.ClbSession.FriendsMobile = true;
            this.ClbSession.ListSubItemMenu = null;
            this.ClbSession.Location = new System.Drawing.Point(0, 0);
            this.ClbSession.Name = "ClbSession";
            this.ClbSession.SelectSubItem = null;
            this.ClbSession.Size = new System.Drawing.Size(310, 462);
            this.ClbSession.SubItemMenu = null;
            this.ClbSession.TabIndex = 0;
            this.ClbSession.Text = "ClbSession";
            this.ClbSession.DoubleClickSubItem += new CCWin.SkinControl.ChatListBox.ChatListEventHandler(this.ClbSession_DoubleClickSubItem);
            // 
            // UserDetialTab
            // 
            this.UserDetialTab.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.UserDetialTab.AnimatorType = CCWin.SkinControl.AnimationType.HorizSlide;
            this.UserDetialTab.BackColor = System.Drawing.Color.LightGray;
            this.UserDetialTab.CloseRect = new System.Drawing.Rectangle(2, 2, 12, 12);
            this.UserDetialTab.HeadBack = null;
            this.UserDetialTab.ImgTxtOffset = new System.Drawing.Point(0, 0);
            this.UserDetialTab.ItemSize = new System.Drawing.Size(70, 36);
            this.UserDetialTab.Location = new System.Drawing.Point(316, 33);
            this.UserDetialTab.Name = "UserDetialTab";
            this.UserDetialTab.PageArrowDown = ((System.Drawing.Image)(resources.GetObject("UserDetialTab.PageArrowDown")));
            this.UserDetialTab.PageArrowHover = ((System.Drawing.Image)(resources.GetObject("UserDetialTab.PageArrowHover")));
            this.UserDetialTab.PageCloseHover = ((System.Drawing.Image)(resources.GetObject("UserDetialTab.PageCloseHover")));
            this.UserDetialTab.PageCloseNormal = ((System.Drawing.Image)(resources.GetObject("UserDetialTab.PageCloseNormal")));
            this.UserDetialTab.PageCloseVisble = true;
            this.UserDetialTab.PageDown = ((System.Drawing.Image)(resources.GetObject("UserDetialTab.PageDown")));
            this.UserDetialTab.PageHover = ((System.Drawing.Image)(resources.GetObject("UserDetialTab.PageHover")));
            this.UserDetialTab.PageImagePosition = CCWin.SkinControl.SkinTabControl.ePageImagePosition.Left;
            this.UserDetialTab.PageNorml = null;
            this.UserDetialTab.Size = new System.Drawing.Size(688, 607);
            this.UserDetialTab.SizeMode = System.Windows.Forms.TabSizeMode.Fixed;
            this.UserDetialTab.TabIndex = 177;
            this.UserDetialTab.TabePageClosing += new CCWin.SkinControl.SkinTabControl.ClosingTabePageHandler(this.UserDetialTab_TabePageClosing);
            // 
            // FrmMain
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.BackColor = System.Drawing.Color.White;
            this.ClientSize = new System.Drawing.Size(997, 680);
            this.Controls.Add(this.UserDetialTab);
            this.Controls.Add(this.skinTabControl1);
            this.EffectCaption = CCWin.TitleType.EffectTitle;
            this.Name = "FrmMain";
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.FrmMain_FormClosing);
            this.Load += new System.EventHandler(this.FrmMain_Load);
            this.Resize += new System.EventHandler(this.FrmMain_ResizeEnd_1);
            this.skinTabControl1.ResumeLayout(false);
            this.skinTabPage1.ResumeLayout(false);
            this.skinTabPage2.ResumeLayout(false);
            this.ResumeLayout(false);

        }

        #endregion

        private CCWin.SkinControl.SkinTabControl skinTabControl1;
        private CCWin.SkinControl.SkinTabPage skinTabPage1;
        private CCWin.SkinControl.SkinTabControl UserDetialTab;
        private CCWin.SkinControl.ChatListBox clbDDuser;
        private CCWin.SkinControl.SkinTabPage skinTabPage2;
        private CCWin.SkinControl.ChatListBox ClbSession;
    }
}