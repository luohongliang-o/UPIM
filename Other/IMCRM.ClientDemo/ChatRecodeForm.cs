using CCWin.SkinControl;
using Common;
using IM.Core;
using IM.Core.Core;
using IMCRM.ClientDemo.Entity;
using IMCRM.ClientDemo.Helpers;
using IMWorkBench.SDK;
using Newtonsoft.Json.Linq;
using rcsdk_test;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace IMCRM.ClientDemo
{
    public partial class ChatRecodeForm : Form
    {
        public ChatRecodeForm()
        {
            InitializeComponent();
        }
        private int currentPageIndex = 1;
        private int pageSize = 25;
        /// <summary>
        /// 索引对应最后一条MsgId
        /// </summary>
        private Dictionary<int, int> IndexAndLastMsgId = new Dictionary<int, int>();
        private bool isLeftMsg = true;
        private bool isRightMsg = false;
        /// <summary>
        /// 客户信息
        /// </summary>
        private User userInfo;
        /// <summary>
        /// 客服信息
        /// </summary>
        private User CusServiceUser;
        public ChatRecodeForm(User _userInfo, User _CusServiceUser)
        {
            InitializeComponent();
            this.userInfo = _userInfo;
            this.CusServiceUser = _CusServiceUser;
        }
        private void ChatRecodeForm_Load(object sender, EventArgs e)
        {
            IndexAndLastMsgIdAdd(1, -1);
            LoadHisMsg();

        }
        /// <summary>
        /// 键值新增数据
        /// </summary>
        /// <param name="index"></param>
        /// <param name="msgId"></param>
        private void IndexAndLastMsgIdAdd(int index, int msgId)
        {
            if (!IndexAndLastMsgId.ContainsKey(index))
                IndexAndLastMsgId.Add(index, msgId);
        }
        #region 加载历史记录
        public void LoadHisMsg()
        {
            toolStripTextBox_pageIndex.Text = currentPageIndex.ToString();
            chatBox_history.Clear();
            MessageInfoEventHandler get_paged_callback = new MessageInfoEventHandler(get_paged_messagex_call_back);
            rcsdk.GetHistoryMessages(userInfo.userId, 1, "", IndexAndLastMsgId[currentPageIndex], pageSize, get_paged_callback);

        }
        public unsafe void get_paged_messagex_call_back([MarshalAs(UnmanagedType.LPWStr)] string json_str)
        {
            try
            {
                //在这里添加通知UI层代码
                JObject history = JObject.Parse(json_str);
                if (history["count"].ToString() == "0")
                {
                    isLeftMsg = false;
                    this.toolStripButton2.Enabled = isLeftMsg;
                    this.toolStripButton3.Enabled = isRightMsg;
                    this.toolStripButton1.Enabled = isRightMsg;
                    chatBox_history.AppendRichText("----------没有更多了-----------", new Font(ChatFormManager.messageFont, FontStyle.Regular), Color.Gray);
                    return;
                }
                if (history["data"].Children().Count() < pageSize)
                {
                    isLeftMsg = false;
                }
                else
                    isLeftMsg = true;
                int i = 0;
                foreach (JObject jobj in history["data"].Children().Reverse())
                {

                    //JObject jobj = JObject.Parse(chat);//聊天内容
                    if (i == 0)
                    {
                        int m_MessageId = Convert.ToInt32(jobj["m_MessageId"].ToString());
                        IndexAndLastMsgIdAdd(currentPageIndex + 1, m_MessageId);
                    }
                    i++;
                    string type = jobj["m_ClazzName"].ToString();//消息类型
                    if (type == "RC:CmdNtf")//通知信息暂时忽略
                        continue;
                    string msg_json_str = jobj["m_Message"].ToString();//消息内容
                    string m_SenderId = jobj["m_SenderId"].ToString();//发送人ID
                    DateTime m_SendTime = DateTime.Now;
                    DateTime.TryParse(jobj["m_SendTime"].ToString(), out m_SendTime);

                    JObject msg_jobj = JObject.Parse(msg_json_str);
                    string content_str = msg_jobj["content"].ToString();

                    if (type == "RC:TxtMsg")//文本消息
                    {
                        ChatBoxContent content = new ChatBoxContent(content_str, ChatFormManager.messageFont, Color.Black);
                        content = ChatFormManager.UpdateTxtFaceUpdate(content);
                        if (m_SenderId == CusServiceUser.userId.ToString())
                        {
                            ChatFormManager.AppendChatBoxContent(this.chatBox_history, string.IsNullOrEmpty(this.CusServiceUser.nickname) ? this.CusServiceUser.name : this.CusServiceUser.nickname, m_SendTime, content, Color.SeaGreen, false);
                        }
                        else
                        {
                            ChatFormManager.AppendChatBoxContent(this.chatBox_history, userInfo.nickname, m_SendTime, content, Color.Blue, false);
                        }
                    }
                    else if (type == "RC:ImgMsg")//图片消息处理
                    {
                        try
                        {
                            LogHelper.Info(content_str);
                            Image img = ImgHelper.Base64StringToImg(content_str);


                            if (m_SenderId == CusServiceUser.userId.ToString())
                            {
                                ChatFormManager.AppendChatBoxImg(this.chatBox_history, ShowMageImage_Click, string.IsNullOrEmpty(this.CusServiceUser.nickname) ? this.CusServiceUser.name : this.CusServiceUser.nickname, Color.SeaGreen, img, m_SendTime, false);
                            }
                            else
                            {
                                ChatFormManager.AppendChatBoxImg(this.chatBox_history, ShowMageImage_Click, userInfo.nickname, Color.Blue, img, m_SendTime, false);
                            }

                        }
                        catch (Exception ex) { LogHelper.Error(ex.ToString()); }
                    }

                }
                if (isLeftMsg == false)
                {
                    chatBox_history.AppendRichText("----------没有更多了-----------", new Font(ChatFormManager.messageFont, FontStyle.Regular), Color.Gray);
                }
                this.toolStripButton2.Enabled = isLeftMsg;
                this.toolStripButton3.Enabled = isRightMsg;
                this.toolStripButton1.Enabled = isRightMsg;
            }
            catch { }
        }
        /// <summary>
        /// 图片最大化处理
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        void ShowMageImage_Click(object sender, EventArgs e)
        {
            GifBox gifbox = (GifBox)sender;
            FrmPrintscreen cswin = new FrmPrintscreen(gifbox.Image,userInfo.userId);
            cswin.Show();
        }
        #endregion
        /// <summary>
        /// 左一页
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void toolStripButton2_Click(object sender, EventArgs e)
        {
            currentPageIndex++;
            isRightMsg = true;
            LoadHisMsg();
        }
        /// <summary>
        /// 右一页
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void toolStripButton3_Click(object sender, EventArgs e)
        {
            currentPageIndex--;
            if (currentPageIndex == 1)
            {
                isRightMsg = false;
            }
            else
            {
                isRightMsg = true;
            }
            LoadHisMsg();

        }

        private void toolStripButton1_Click(object sender, EventArgs e)
        {
            currentPageIndex = 1;
            isRightMsg = false;
            LoadHisMsg();
        }
    }
}
