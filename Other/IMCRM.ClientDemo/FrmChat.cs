
using CCWin;
using CCWin.SkinControl;
using System;
using System.Net;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Drawing.Drawing2D;
using System.Text;
using System.Windows.Forms;
using CCWin.SkinClass;
using System.IO;
using IMCRM.ClientDemo.Helpers;
using System.Threading;
using IMCRM.ClientDemo.Entity;
using Newtonsoft.Json.Linq;
using Common;
using IMWorkBench.SDK;
using System.Text.RegularExpressions;
using System.Globalization;
using System.Runtime.InteropServices;
using System.Linq;
using System.Threading.Tasks;
using Qiniu.IO;
using Qiniu.RS;
using rcsdk_test;
using IM.Core.Core;

namespace IMCRM.ClientDemo
{
    public partial class FrmChat : CCSkinMain
    {
        /// <summary>
        /// 客户信息
        /// </summary>
        private User userInfo;
        /// <summary>
        /// 客服信息
        /// </summary>
        private User CusServiceUser;

        /// <summary>
        /// 接收消息
        /// </summary>
        /// <param name="jobj"></param>
        /// <param name="isSystemMsg"></param>
        public void GetMessage(JObject jobj, bool isSystemMsg = false)
        {
            if (isSystemMsg)
            {
                var data = jobj["data"].ToString();
                var targetId = JObject.Parse(data)["targetId"].ToString();
                Shell.WriteLine("info:匹配到的客服ID：" + targetId);
                SendSystemDelegate sendSystem = AppendSysMessage;
                this.Invoke(sendSystem, "请等待客服(" + targetId + ")回复");
            }
            else
            {
                string m_SenderId = jobj["m_SenderId"].ToString(); //发送人ID
                if (m_SenderId != userInfo.userId.ToString()) //不是自己的消息拒绝接受
                    return;
                string type = jobj["m_ClazzName"].ToString(); //消息类型

                string msg_json_str = jobj["m_Message"].ToString();
                JObject msg_jobj = JObject.Parse(msg_json_str);
                string content_str = msg_jobj["content"].ToString();
                if (type == "RC:TxtMsg") //文本消息
                {
                    //LogHelper.Info(content_str);
                    ChatBoxContent content = new ChatBoxContent(content_str, messageFont, Color.Black);
                    content = UpdateTxtFaceUpdate(content);
                    SendTextDelegate sd = AppendChatBoxContent;
                    this.Invoke(sd, userInfo.nickname, null, content, Color.Blue);
                    //Task.Factory.StartNew(() => AppendChatBoxContent(userInfo.nickname, null, content, Color.Blue));
                }
                else if (type == "RC:ImgMsg") //图片消息处理
                {
                    try
                    {
                        LogHelper.Info(content_str);
                        Image img = ImgHelper.Base64StringToImg(content_str);
                        SendImgDelegate sc = AppendChatBoxImg;
                        this.Invoke(sc, userInfo.nickname, Color.Black, img, null);
                        //Task.Factory.StartNew(() => AppendChatBoxImg(userInfo.nickname, Color.Black, img, null));
                    }
                    catch (Exception ex)
                    {
                        LogHelper.Error(ex.ToString());
                    }
                }
            }
        }
        /// <summary>
        /// 接收消息
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        public void GetNoticeMessage(JObject jobj)
        {
            string m_SenderId = jobj["fromUserId"].ToString();//发送人ID
            if (m_SenderId != userInfo.userId.ToString())//不是自己的消息拒绝接受
                return;
            string type = jobj["classname"].ToString();//消息类型
            string msg_json_str = jobj["content"].ToString();
            JObject msg_jobj = JObject.Parse(msg_json_str);
            string content_str = msg_jobj["content"].ToString();
            if (type == "RC:TxtMsg")//文本消息
            {
                //LogHelper.Info(content_str);
                ChatBoxContent content = new ChatBoxContent(content_str, ChatFormManager.messageFont, Color.Black);
                content = ChatFormManager.UpdateTxtFaceUpdate(content);
                SendTextDelegate sd = AppendChatBoxContent;
                this.Invoke(sd, userInfo.nickname, null, content, Color.Blue);
                //Task.Factory.StartNew(() => AppendChatBoxContent(userInfo.nickname, null, content, Color.Blue));
            }
            else if (type == "RC:ImgMsg")//图片消息处理
            {
                try
                {
                    LogHelper.Info(content_str);
                    Image img = ImgHelper.Base64StringToImg(content_str);
                    SendImgDelegate sc = AppendChatBoxImg;
                    this.Invoke(sc, userInfo.nickname, Color.Black, img, null);
                    //Task.Factory.StartNew(() => AppendChatBoxImg(userInfo.nickname, Color.Black, img, null));
                }
                catch (Exception ex) { LogHelper.Error(ex.ToString()); }
            }
        }

        #region 变量
        /// <summary>
        /// 文本格式
        /// </summary>
        private Font messageFont = new Font("微软雅黑", 9);

        private ChatListSubItem qqUser;
        /// <summary>
        /// QQ聊天用户
        /// </summary>
        public ChatListSubItem QQUser
        {
            get { return qqUser; }
            set
            {
                if (qqUser != value)
                {
                    qqUser = value;
                    lblChatName.Tag = lblChatName.Text = string.IsNullOrEmpty(qqUser.DisplayName) ? qqUser.NicName : qqUser.DisplayName;
                    lblChatQm.Text = qqUser.PersonalMsg;
                    pnlImgTx.BackgroundImage = qqUser.HeadImage;
                    //imgQQShow.Image = qqUser.QQShow;
                }
            }
        }
        #endregion

        #region FrmChat_Load
        /// <summary>
        /// 界面初始化 加载历史记录
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void FrmChat_Load(object sender, EventArgs e)
        {
            MessageInfoEventHandler get_paged_callback = new MessageInfoEventHandler(get_paged_messagex_call_back);
            rcsdk.GetHistoryMessages(userInfo.userId.ToString(), 1, "", -1, 5, get_paged_callback);
        }
        #endregion

        #region 无参构造
        public FrmChat()
        {
            InitializeComponent();
            //加载表情到文本框
            this.chatBoxSend.Initialize(GlobalResourceManager.EmotionDictionary);
            this.chatBox_history.Initialize(GlobalResourceManager.EmotionDictionary);
        }
        #endregion

        #region 带参构造
        public FrmChat(ChatListSubItem QQUser, User _CusServiceUser)
        {
            InitializeComponent();
            this.QQUser = QQUser;
            //加载表情到文本框
            this.chatBoxSend.Initialize(GlobalResourceManager.EmotionDictionary);
            this.chatBox_history.Initialize(GlobalResourceManager.EmotionDictionary);
            if (QQUser.NicName.IndexOf('|') >= 0)
            {
                var arr = QQUser.NicName.Split(new char[] { '|' });
                userInfo = new User
                {
                    userId = arr[0],
                    name = qqUser.DisplayName,
                    nickname = QQUser.DisplayName,
                    extra = arr[1]
                };
            }
            else
            {
                userInfo = new User
                {
                    userId = QQUser.NicName,
                    name = qqUser.DisplayName,
                    nickname = QQUser.DisplayName,
                    extra = ""
                };
            }

            this.CusServiceUser = _CusServiceUser;
        }

        private void LoadData()
        {
            chatBox_history.AppendText("妈的");
        }
        #endregion

        #region 窗体重绘时
        private void FrmQQChat_Paint(object sender, PaintEventArgs e)
        {
            Graphics g = e.Graphics;
            g.SmoothingMode = SmoothingMode.None;
            //全屏蒙浓遮罩层
            g.FillRectangle(new SolidBrush(Color.FromArgb(80, 255, 255, 255)), new Rectangle(0, 0, this.Width, this.chatBox_history.Top));
            g.FillRectangle(new SolidBrush(Color.FromArgb(80, 255, 255, 255)), new Rectangle(0, this.chatBox_history.Top, this.chatBox_history.Width + this.chatBox_history.Left, this.Height - this.chatBox_history.Top));
            //线条
            g.DrawLine(new Pen(Color.FromArgb(180, 198, 221)), new Point(0, this.chatBox_history.Top - 1), new Point(chatBox_history.Right, this.chatBox_history.Top - 1));
            g.DrawLine(new Pen(Color.FromArgb(180, 198, 221)), new Point(0, this.chatBox_history.Bottom), new Point(chatBox_history.Right, this.chatBox_history.Bottom));
        }
        #endregion

        #region 发送信息
        private void btnSend_Click(object sender, EventArgs e)
        {
            ChatBoxContent content = this.chatBoxSend.GetContent();

            //发送内容为空时，不做响应
            if (content.IsEmpty())
            {
                return;
            }
            //content.EmotionDictionary = new Dictionary<uint, uint>();

            //string text = content.Text;
            //string newText = string.Empty;
            //List<uint> removeList = new List<uint>();
            //Dictionary<uint, string> dic = new Dictionary<uint, string>();
            //int i = 0;
            //foreach (var item in content.ForeignImageDictionary)
            //{
            //    tag = item.Value.Tag as string;
            //    if (!string.IsNullOrEmpty(tag) && FaceHelper.FaceDic.ContainsKey(tag))
            //    {
            //        uint x = content.PicturePositions[i];
            //        dic.Add(x, tag);
            //    }
            //    i++;
            //}
            //uint w = 0;
            //foreach (char chartxt in text)
            //{
            //    if (dic.ContainsKey(w))
            //    {
            //        string hexValues = FaceHelper.FaceDic[dic[w]];
            //        int value = Convert.ToInt32(hexValues, 16);
            //        string stringValue = Char.ConvertFromUtf32(value);
            //        char charValue = (char)value;
            //        newText += charValue;
            //    }
            //    else
            //        newText += chartxt;
            //    w++;
            //}
            //content.Text = newText;
            //请求小黄鸡回复
            string tag = string.Empty;
            if (content.ContainsForeignImage())//含有图片信息
            {
                this.AppendChatBoxContent(string.IsNullOrEmpty(this.CusServiceUser.nickname) ? this.CusServiceUser.name : this.CusServiceUser.nickname, null, content, Color.SeaGreen);
                foreach (var item in content.ForeignImageDictionary)
                {
                    tag = item.Value.Tag as string;
                    if (tag == null || !FaceHelper.FaceDic.ContainsKey(tag))
                    {
                        System.Threading.ThreadPool.QueueUserWorkItem((s) => SendImgMsg(item.Value));
                    }
                }
            }
            else
            {
                this.AppendChatBoxContent(string.IsNullOrEmpty(this.CusServiceUser.nickname) ? this.CusServiceUser.name : this.CusServiceUser.nickname, null, content, Color.SeaGreen);

            }
            string sendTxt = ChatFormManager.GetTxtAndContainFace(content);
            if (!string.IsNullOrWhiteSpace(sendTxt))
                System.Threading.ThreadPool.QueueUserWorkItem((s) => SendTxtMsg(sendTxt));
            //清空发送输入框
            this.chatBoxSend.Text = string.Empty;
            this.chatBoxSend.Focus();
        }
        #endregion
        private ChatBoxContent UpdateTxtFaceUpdate(ChatBoxContent txt)
        {
            string path = Path.Combine(Application.StartupPath, "Expression\\");
            int index = -1;
            int faceLength = 0;//表情字符占用长度
            string newTxt = string.Empty;
            foreach (var item in FaceHelper.ImgDic)
            {
                txt.Text = txt.Text.Replace(item.Key, "[" + item.Value + "]");
            }
            List<FaceMsg> faceList = new List<FaceMsg>();

            foreach (var item in FaceHelper.FaceDic)
            {
                index = txt.Text.IndexOf("[" + item.Key + "]");
                faceLength = item.Key.Length + 2;
                while (index != -1)
                {
                    if (!txt.ForeignImageDictionary.ContainsKey((uint)index))
                    {
                        faceList.Add(new FaceMsg { index = index, image = Image.FromFile(path + item.Key + ".png"), length = item.Key.Length + 1, imgeName = item.Key });
                        newTxt = string.Empty;
                        if (index > 0)
                            newTxt = txt.Text.Substring(0, index);
                        newTxt += "<" + item.Key + ">";//作为占位符
                        if (index + faceLength < txt.Text.Length)
                            newTxt += txt.Text.Substring(index + faceLength, txt.Text.Length - index - faceLength);
                        txt.Text = newTxt;
                        index = txt.Text.IndexOf("[" + item.Key + "]");
                    }
                    else
                    {
                        index = -1;
                    }
                }
            }
            int uindex = 0;
            foreach (FaceMsg item in faceList.OrderByDescending(m => m.index))
            {
                uindex = item.index - faceList.Where(m => m.index < item.index).Sum(m => m.length);
                txt.AddForeignImage((uint)uindex, item.image);
                txt.PicturePositions.Add((uint)uindex);
                txt.Text = txt.Text.Replace("<" + item.imgeName + ">", "Q");//更换占位符
            }

            return txt;
        }
        /// <summary>  
        /// 字符串转为UniCode码字符串  
        /// </summary>  
        /// <param name="s"></param>  
        /// <returns></returns>  
        public static string StringToUnicode(string s)
        {
            char[] charbuffers = s.ToCharArray();
            byte[] buffer;
            StringBuilder sb = new StringBuilder();
            for (int i = 0; i < charbuffers.Length; i++)
            {
                buffer = System.Text.Encoding.Unicode.GetBytes(charbuffers[i].ToString());
                sb.Append(String.Format("//u{0:X2}{1:X2}", buffer[1], buffer[0]));
            }
            return sb.ToString();
        }


        #region 接收信息封装
        private void OnReceivedMsg(ChatBoxContent content, DateTime? originTime)
        {
            this.AppendChatBoxContent(lblChatName.Tag == null ? "小黄鸡" : lblChatName.Tag.ToString(), originTime, content, Color.Blue);
        }
        #endregion

        #region 发送消息封装
        /// <summary>
        /// 接收图片委托
        /// </summary>
        /// <param name="userName"></param>
        /// <param name="color"></param>
        /// <param name="img"></param>
        delegate void SendImgDelegate(string userName, Color color, Image img, DateTime? originTime);
        /// <summary>
        /// 接收文本委托
        /// </summary>
        /// <param name="userName"></param>
        /// <param name="originTime"></param>
        /// <param name="content"></param>
        /// <param name="color"></param>
        /// <param name="followingWords"></param>
        delegate void SendTextDelegate(string userName, DateTime? originTime, ChatBoxContent content, Color color);

        delegate void SendSystemDelegate(string msg);

        /// <summary>
        /// 添加聊天图片
        /// </summary>
        /// <param name="userName"></param>
        /// <param name="color"></param>
        /// <param name="img"></param>
        private void AppendChatBoxImg(string userName, Color color, Image img, DateTime? originTime)
        {
            string showTime = DateTime.Now.ToLongTimeString();
            if (originTime != null)
                showTime = originTime.Value.ToLongTimeString();
            this.chatBox_history.AppendRichText(string.Format("{0}  {1}\n", userName, showTime), new Font(this.messageFont, FontStyle.Regular), color);
            this.chatBox_history.AppendText("    ");
            this.chatBox_history.InsertImage(img);
            this.chatBox_history.AppendText("\n");
            this.chatBox_history.Select(this.chatBox_history.Text.Length, 0);
            this.chatBox_history.ScrollToCaret();
            //return;
        }
        /// <summary>
        /// 发送信息文本到内容框
        /// </summary>
        /// <param name="userName">名字</param>
        /// <param name="originTime">时间</param>
        /// <param name="content">发送内容</param>
        /// <param name="color">字体颜色</param>
        /// <param name="followingWords">是否有用户名</param>
        /// <param name="offlineMessage">是否在线消息</param>
        private void AppendChatBoxContent(string userName, DateTime? originTime, ChatBoxContent content, Color color)
        {

            string showTime = DateTime.Now.ToLongTimeString();
            if (originTime != null)
                showTime = originTime.Value.ToLongTimeString();
            this.chatBox_history.AppendRichText(string.Format("{0}  {1}\n", userName, showTime), new Font(this.messageFont, FontStyle.Regular), color);
            this.chatBox_history.AppendText("    ");
            this.chatBox_history.AppendChatBoxContent(content);
            this.chatBox_history.AppendText("\n");
            this.chatBox_history.Select(this.chatBox_history.Text.Length, 0);
            this.chatBox_history.ScrollToCaret();
        }

        /// <summary>
        /// 发送信息文本到内容框
        /// </summary>
        /// <param name="userName">名称</param>
        /// <param name="color">字体颜色</param>
        /// <param name="msg">信息</param>
        private void AppendMessage(string userName, Color color, string msg)
        {
            DateTime showTime = DateTime.Now;
            this.chatBox_history.AppendRichText(string.Format("{0}  {1}\n", userName, showTime.ToLongTimeString()), new Font(this.messageFont, FontStyle.Regular), color);
            this.chatBox_history.AppendText("    ");

            this.chatBox_history.AppendText(msg);
            this.chatBox_history.Select(this.chatBox_history.Text.Length, 0);
            this.chatBox_history.ScrollToCaret();
        }

        /// <summary>
        /// 发送系统消息
        /// </summary>
        /// <param name="msg">信息</param>
        public void AppendSysMessage(string msg)
        {
            this.AppendMessage("系统", Color.Gray, msg);
            this.chatBox_history.AppendText("\n");
        }
        #endregion

        #region 退出当前聊天
        private void btnClose_Click(object sender, EventArgs e)
        {
            this.Close();
        }
        #endregion

        #region 发送信息
        private void SendImgMsg(Image img)
        {
            try
            {
                string url = "";
                /*
                 ==========================================
                 ====这里做图片处理（上传到我们的服务器返回URL）==
                 ==========================================
                */
                var exten = FileHelper.GetImageExtension(img);
                string name = DateTime.Now.ToString("yyyyMMddHHmmss");
                var path = System.Environment.CurrentDirectory + "\\" + name + "." + exten;
                img.Save(path);
                url = UploadImage(path, name);
                //先生成缩略图

                //Image newImg = image_helper.ReturnThumbnail(img, 15, 15, "HW");
                //进行base64编码
                byte[] thum_img_bytes = image_helper.ImageToBytes(img);
                string thum_img_base64 = Convert.ToBase64String(thum_img_bytes);

                JObject msg_jobj = new JObject();
                msg_jobj["content"] = thum_img_base64;
                msg_jobj["imageUri"] = url;
                msg_jobj["extra"] = userInfo.userId + "|" + userInfo.extra;
                int messageId = rcsdk.SaveMessage(userInfo.userId.ToString(), 1, "RC:ImgMsg", CusServiceUser.userId.ToString(), msg_jobj.ToString(), "", "", false, 0);
                PublishAckListenerEventHandler send_img_msg_call_back = new PublishAckListenerEventHandler(send_message_callback_delegate.send_message_call_back);
                rcsdk.sendMessage(userInfo.userId.ToString(), 1, 2, "RC:ImgMsg", msg_jobj.ToString(), "", "", messageId, send_img_msg_call_back);

            }
            catch (Exception ex)
            {
                LogHelper.Error(ex.ToString());
            }
        }
        /// <summary>
        /// 发送信息
        /// </summary>
        /// <param name="msg"></param>
        private void SendTxtMsg(string msg)
        {
            try
            {
                JObject jobj = new JObject();
                jobj["content"] = msg;
                jobj["extra"] = userInfo.userId + "|" + userInfo.extra;
                //单聊文本消息
                int messageId = rcsdk.SaveMessage(userInfo.userId, 1, "RC:TxtMsg", CusServiceUser.userId, jobj.ToString(), "", "", false, 0);
                rcsdk.sendMessage(userInfo.userId, 1, 2, "RC:TxtMsg", jobj.ToString(), "", "", messageId, send_call_back);

            }
            catch (Exception ex)
            {
                LogHelper.Error(ex.ToString());
            }
        }
        PublishAckListenerEventHandler send_call_back = new PublishAckListenerEventHandler(send_message_callback_delegate.send_message_call_back);
        #endregion

        #region 截图
        /// <summary>
        /// 截图
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void toolPrintScreen_ButtonClick(object sender, EventArgs e)
        {
            this.StartCapture();
        }

        //截图方法
        private void StartCapture()
        {
            FrmCapture imageCapturer = new FrmCapture();
            if (imageCapturer.ShowDialog() == System.Windows.Forms.DialogResult.OK)
            {
                System.Windows.Forms.IDataObject iData = Clipboard.GetDataObject();
                if (iData.GetDataPresent(DataFormats.Bitmap))
                {//如果剪贴板中的数据是文本格式
                    GifBox gif = this.chatBoxSend.InsertImage((Bitmap)iData.GetData(DataFormats.Bitmap));
                    this.chatBoxSend.Focus();
                    this.chatBoxSend.ScrollToCaret();
                    imageCapturer.Close();
                    imageCapturer = null;
                }
            }
        }
        #endregion

        #region 表情窗体与事件
        public FrmCountenance _faceForm = null;
        public FrmCountenance FaceForm
        {
            get
            {
                if (this._faceForm == null)
                {
                    this._faceForm = new FrmCountenance(this)
                    {
                        ImagePath = "Expression\\",
                        CustomImagePath = "Expression\\Custom\\",
                        CanManage = true,
                        ShowDemo = true,
                    };

                    this._faceForm.Init(24, 24, 8, 8, 12, 8);
                    this._faceForm.Selected += this._faceForm_AddFace;

                }

                return this._faceForm;
            }
        }

        string imgName = "";
        void _faceForm_AddFace(object sender, SelectFaceArgs e)
        {
            //this.imgName = e.Img.FullName.Replace(Application.StartupPath + "\\", "");
            //Image img = e.Img.Image;
            //img.Tag = e.Img.FileName.Substring(0, e.Img.FileName.IndexOf("."));
            //this.chatBoxSend.InsertImage(img);
            //this.chatBoxSend.InsertDefaultEmotion((uint)e.ImageIndex);
            this.imgName = e.Img.FullName.Replace(Application.StartupPath + "\\", "");
            this.chatBoxSend.InsertDefaultEmotion((uint)e.ImageIndex);
        }
        #endregion

        #region 表情按钮事件
        private void toolCountenance_Click(object sender, EventArgs e)
        {
            Point pt = this.PointToScreen(new Point(skToolMenu.Left + 30 - this.FaceForm.Width / 2, (skToolMenu.Top - this.FaceForm.Height)));
            this.FaceForm.Show(pt.X, pt.Y, skToolMenu.Height);
        }
        #endregion

        #region 震动按钮事件
        private void toolZhenDong_Click(object sender, EventArgs e)
        {
            this.AppendMessage(CusServiceUser.nickname, Color.Green, "您发送了一个抖动提醒。\n");
            this.chatBoxSend.Focus();
            Vibration();
        }
        #endregion

        #region 震动方法
        //震动方法
        private void Vibration()
        {
            Point pOld = this.Location;//原来的位置
            int radius = 3;//半径
            for (int n = 0; n < 3; n++) //旋转圈数
            {
                //右半圆逆时针
                for (int i = -radius; i <= radius; i++)
                {
                    int x = Convert.ToInt32(Math.Sqrt(radius * radius - i * i));
                    int y = -i;

                    this.Location = new Point(pOld.X + x, pOld.Y + y);
                    Thread.Sleep(10);
                }
                //左半圆逆时针
                for (int j = radius; j >= -radius; j--)
                {
                    int x = -Convert.ToInt32(Math.Sqrt(radius * radius - j * j));
                    int y = -j;

                    this.Location = new Point(pOld.X + x, pOld.Y + y);
                    Thread.Sleep(10);
                }
            }
            //抖动完成，恢复原来位置
            this.Location = pOld;
        }
        #endregion

        #region 发送键更多选择 及 发送键更多选择菜单关闭时
        //发送键更多选择
        private void btnSendMenu_Click(object sender, EventArgs e)
        {
            btnSendMenu.StopState = StopStates.Pressed;
            SendMenu.Show(btnSendMenu, new Point(0, btnSendMenu.Height + 5));
        }

        //发送键更多选择菜单关闭时
        private void SendMenu_Closing(object sender, ToolStripDropDownClosingEventArgs e)
        {
            btnSendMenu.StopState = StopStates.NoStop;
            btnSendMenu.ControlState = ControlState.Normal;
        }
        #endregion

        #region 字体
        //显示字体对话框
        private void toolFont_Click(object sender, EventArgs e)
        {

        }
        #endregion

        #region 自定义系统按钮事件
        private void FrmQQChat_SysBottomClick(object sender, SysButtonEventArgs e)
        {
            if (e.SysButton.Name == "SysSet")
            {
                Point l = PointToScreen(e.SysButton.Location);
                l.Y += e.SysButton.Size.Height + 1;
                SysMenu.Show(l);
            }
        }
        #endregion

        #region 加载历史记录
        public unsafe void get_paged_messagex_call_back([MarshalAs(UnmanagedType.LPWStr)] string json_str)
        {
            //在这里添加通知UI层代码
            JObject history = JObject.Parse(json_str);
            if (history["count"].ToString() == "0")
                return;
            foreach (JObject jobj in history["data"].Children().Reverse())
            {
                //JObject jobj = JObject.Parse(chat);//聊天内容

                string type = jobj["m_ClazzName"].ToString();//消息类型
                string msg_json_str = jobj["m_Message"].ToString();//消息内容
                string m_SenderId = jobj["m_SenderId"].ToString();//发送人ID
                DateTime m_SendTime = DateTime.Now;
                DateTime.TryParse(jobj["m_SendTime"].ToString(), out m_SendTime);
                if (type == "RC:CmdNtf")
                    continue;

                JObject msg_jobj = JObject.Parse(msg_json_str);
                string content_str = msg_jobj["content"].ToString();

                if (type == "RC:TxtMsg")//文本消息
                {
                    ChatBoxContent content = new ChatBoxContent(content_str, ChatFormManager.messageFont, Color.Black);
                    content = ChatFormManager.UpdateTxtFaceUpdate(content);
                    if (m_SenderId == CusServiceUser.userId.ToString())
                    {
                        this.AppendChatBoxContent(string.IsNullOrEmpty(this.CusServiceUser.nickname) ? this.CusServiceUser.name : this.CusServiceUser.nickname, m_SendTime, content, Color.SeaGreen);
                    }
                    else
                    {
                        this.AppendChatBoxContent(userInfo.nickname, m_SendTime, content, Color.Blue);
                    }

                    //this.AppendChatBoxContent(userInfo.nickname, null, content, Color.Blue, false);
                }
                else if (type == "RC:ImgMsg")//图片消息处理
                {
                    try
                    {
                        LogHelper.Info(content_str);
                        Image img = ImgHelper.Base64StringToImg(content_str);


                        if (m_SenderId == CusServiceUser.userId.ToString())
                        {
                            this.AppendChatBoxImg(string.IsNullOrEmpty(this.CusServiceUser.nickname) ? this.CusServiceUser.name : this.CusServiceUser.nickname, Color.SeaGreen, img, m_SendTime);
                        }
                        else
                        {
                            this.AppendChatBoxImg(userInfo.nickname, Color.Blue, img, m_SendTime);
                        }

                    }
                    catch (Exception ex) { LogHelper.Error(ex.ToString()); }
                }

            }

        }
        #endregion
        private void toolStripSendImg_Click(object sender, EventArgs e)
        {
            try
            {
                string file = Helpers.FileHelper.GetFileToOpen2("请选择图片", null, ".jpg", ".bmp", ".png", ".gif");
                if (file == null)
                {
                    return;
                }

                Image img = Image.FromFile(file);
                Image img2 = image_helper.ReturnThumbnail(img);
                this.chatBoxSend.InsertImage(img2);
                //ChangeImageToString(img);
            }
            catch (Exception ee)
            {
                MessageBoxEx.Show(ee.Message, "GG2014");
            }
        }

        /// <summary>
        /// 设置上传的文件的key值
        /// </summary>
        /// <param name="key"></param>
        /// <returns></returns>
        private string UploadImage(string path, string imgKey)
        {
            //设置账号的AK和SK
            Qiniu.Conf.Config.ACCESS_KEY = ConfigHelper._qiniuAppKey;
            Qiniu.Conf.Config.SECRET_KEY = ConfigHelper._qiniuSecret;
            IOClient target = new IOClient();
            PutExtra extra = new PutExtra();
            //设置上传的空间
            String bucket = ConfigHelper._qiniuUserAgent;
            //设置上传的文件的key值
            String key = imgKey;

            //普通上传,只需要设置上传的空间名就可以了,第二个参数可以设定token过期时间
            PutPolicy put = new PutPolicy(bucket);

            //调用Token()方法生成上传的Token
            string upToken = put.Token();
            //上传文件的路径
            String filePath = path;

            //调用PutFile()方法上传
            PutRet ret = target.PutFile(upToken, key, filePath, extra);
            //打印出相应的信息
            Shell.WriteLine(ret.Response.ToString());
            Shell.WriteLine(ret.key);
            return ret.key;
        }

        private void toolStripDropDownButton4_ButtonClick(object sender, EventArgs e)
        {

        }

        private void toolStripButton1_Click(object sender, EventArgs e)
        {
            if (hischatPanl.Visible == false)
            {
                ChatRecodeForm form = new ChatRecodeForm(userInfo, CusServiceUser);
                form.Dock = DockStyle.Fill;
                form.TopLevel = false;
                hischatPanl.Visible = true;
                hischatPanl.Controls.Add(form);
                form.Show();
            }
            else
            {
                hischatPanl.Visible = false;
                hischatPanl.Controls.Clear();
            }
        }

        private void button1_Click(object sender, EventArgs e)
        {
            var work = "测试自定义消息";
            var senderId = "2";
            var targetId = "KBCCF016";
            var content = "{\"name\":\"AtOther\",\"data\":\"" + work + "\"}";
            var clazzName = "RC:CmdNtf";
            //JObject jobj = new JObject();
            //jobj["content"] = "sadflkj";
            //var content = jobj.ToString();
            //var clazzName = "RC:TxtMsg";
            PublishAckListenerEventHandler send_call_back2 = new PublishAckListenerEventHandler(send_message_callback_delegate.send_message_call_back);

            int messageId = rcsdk.SaveMessage(targetId, 1, clazzName, senderId, content, "", "", false, 0);
            rcsdk.sendMessage(targetId, 1, 2, clazzName, content, "", "", messageId, send_call_back2);
        }
    }
}
