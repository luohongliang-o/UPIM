namespace IMCRM.ClientDemo
{
    partial class BaseForm
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
            this.SuspendLayout();
            // 
            // BaseForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.BackColor = System.Drawing.SystemColors.Control;
            this.BorderColor = System.Drawing.Color.FromArgb(((int)(((byte)(150)))), ((int)(((byte)(183)))), ((int)(((byte)(183)))), ((int)(((byte)(183)))));
            this.ClientSize = new System.Drawing.Size(585, 508);
            this.CloseBoxSize = new System.Drawing.Size(30, 27);
            this.CloseDownBack = global::IMCRM.ClientDemo.Properties.Resources.sysbtn_close_down;
            this.CloseMouseBack = global::IMCRM.ClientDemo.Properties.Resources.sysbtn_close_hover;
            this.CloseNormlBack = global::IMCRM.ClientDemo.Properties.Resources.sysbtn_close_normal;
            this.ControlBoxOffset = new System.Drawing.Point(0, 0);
            this.DropBack = false;
            this.EffectCaption = CCWin.TitleType.None;
            this.InnerBorderColor = System.Drawing.Color.Transparent;
            this.MaxDownBack = global::IMCRM.ClientDemo.Properties.Resources.sysbtn_max_down;
            this.MaxMouseBack = global::IMCRM.ClientDemo.Properties.Resources.sysbtn_max_hover;
            this.MaxNormlBack = global::IMCRM.ClientDemo.Properties.Resources.sysbtn_max_normal;
            this.MaxSize = new System.Drawing.Size(30, 27);
            this.MiniDownBack = global::IMCRM.ClientDemo.Properties.Resources.sysbtn_min_down;
            this.MiniMouseBack = global::IMCRM.ClientDemo.Properties.Resources.sysbtn_min_hover;
            this.MinimumSize = new System.Drawing.Size(585, 508);
            this.MiniNormlBack = global::IMCRM.ClientDemo.Properties.Resources.sysbtn_min_normal;
            this.MiniSize = new System.Drawing.Size(30, 27);
            this.Mobile = CCWin.MobileStyle.Mobile;
            this.Name = "BaseForm";
            this.Opacity = 0.98D;
            this.Radius = 2;
            this.RestoreDownBack = global::IMCRM.ClientDemo.Properties.Resources.sysbtn_restore_down;
            this.RestoreMouseBack = global::IMCRM.ClientDemo.Properties.Resources.sysbtn_restore_hover;
            this.RestoreNormlBack = global::IMCRM.ClientDemo.Properties.Resources.sysbtn_restore_normal;
            this.Shadow = false;
            this.ShadowPalace = global::IMCRM.ClientDemo.Properties.Resources.ShadowPalace;
            this.ShadowWidth = 6;
            this.ShowDrawIcon = false;
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "";
            this.ResumeLayout(false);

        }

        #endregion
    }
}