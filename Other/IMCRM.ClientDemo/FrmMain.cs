using CCWin;
using Common;
using IMCRM.ClientDemo.Entity;
using IMCRM.ClientDemo.Helpers;
using IMWorkBench.SDK;
using Newtonsoft.Json.Linq;
using System;
using System.Collections.Concurrent;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using DAL.Redis;
using Model.Allocate;
using Model.Entity;
using Model.Enum;
using Newtonsoft.Json;
using io.rong;
using rcsdk_test;
using SecurityHelper = Security.SecurityHelper;

namespace IMCRM.ClientDemo
{
    public partial class FrmMain : BaseForm
    {
        //public EventHandler mainevent;
        #region Method

        /// <summary>
        /// 客户信息
        /// </summary>
        private User CusServiceUser;
        /// <summary>
        /// 客户聊天子窗体
        /// </summary>
        Dictionary<string, FrmChat> UserFromList;
        Dictionary<string, User> UserWaitList;
        static CCWin.SkinControl.ChatListItem _chatPublic = null;
        static CCWin.SkinControl.ChatListItem _chatSession = null;
        public static FrmMain frm;

        ConnectAckListenerEventHandler connectAckCallBack = connect_ack_callback_delegate.connect_call_back;
        MessageListenerEventHandler msg_callback = new MessageListenerEventHandler(set_message_listener_callback_delegate.set_message_listener_call_back);
        ExceptionListenerEventHandler exception_callback = new ExceptionListenerEventHandler(exception_listener_callback_delegate.exception_listener_call_back);
        PublishAckListenerEventHandler send_call_back = new PublishAckListenerEventHandler(send_message_callback_delegate.send_message_call_back);
        UserInfoListenerEventHandler user_info_callback = new UserInfoListenerEventHandler(get_user_info_callback_delegate.get_whait_userinfo_call_back);

        #endregion

        public FrmMain()
        {
            InitializeComponent();
        }
        public FrmMain(User _cusServiceUser)
        {
            InitializeComponent();
            frm = this;
            CusServiceUser = _cusServiceUser;
        }
        private void FrmMain_Load(object sender, EventArgs e)
        {
            //this.IsMdiContainer = true;//设置父窗体是容器
            _chatPublic = new CCWin.SkinControl.ChatListItem { Text = "公众号", Tag = "", IsOpen = true };
            _chatSession = new CCWin.SkinControl.ChatListItem { Text = "消息会话", Tag = "", IsOpen = true };
            clbDDuser.Items.Add(_chatPublic);
            ClbSession.Items.Add(_chatSession);
            UserWaitList = new Dictionary<string, User>();
            UserInit();
            InitFrendsList();
        }

        #region 客服信息初始化

        /// <summary>
        /// 客服信息加载
        /// </summary>
        private void UserInit()
        {
            string path = System.Environment.CurrentDirectory;
            //string path = "C:\\workspace\\中 文路径";
            rcsdk.InitClient(ConfigHelper._appKey, "dd你好宁", "deviceid", path, path);
            rcsdk.SetDeviceInfo("Apple", "mac pro", "win10", "WIFI", "");
            //文本消息
            rcsdk.RegisterMessageType("RC:TxtMsg", 3);
            //图片消息
            rcsdk.RegisterMessageType("RC:ImgMsg", 3);
            //图文消息
            rcsdk.RegisterMessageType("RC:VcMsg", 3);
            //位置消息
            rcsdk.RegisterMessageType("RC:LBSMsg", 3);
            //添加联系人消息
            rcsdk.RegisterMessageType("RC:ContactNtf", 3);
            //提示条（小灰条）通知消息
            rcsdk.RegisterMessageType("RC:InfoNtf", 3);
            //资料通知消息
            rcsdk.RegisterMessageType("RC:ProfileNtf", 3);
            //通用命令通知消息
            rcsdk.RegisterMessageType("RC:CmdNtf", 3);
            //设置消息监听
            rcsdk.SetMessageListener(msg_callback);
            //设置网络异常监听
            rcsdk.SetExceptionListener(exception_callback);
            rcsdk.Connect(CusServiceUser.token, connectAckCallBack);
            UserFromList = new Dictionary<string, FrmChat>();
        }
        #endregion

        #region 消息监听及委托
        /// <summary>
        /// 消息回调UI处理
        /// </summary>
        /// <param name="json_str"></param>
        public void message_dealwith(string json_str)
        {
            try
            {
                Shell.WriteLine("info客服收到消息：" + json_str);
                JObject jobj = JObject.Parse(json_str);
                if (jobj["result"].ToString() != "success")
                {
                    return;
                }
                string m_SenderId = jobj["m_SenderId"].ToString();//发送人ID
                string m_ClazzName = jobj["m_ClazzName"].ToString();//类型

                if (m_ClazzName == "RC:CmdNtf")// 通用命令通知消息
                {
                    #region
                    JObject m_Message = JObject.Parse(jobj["m_Message"].ToString());
                    string name = m_Message["name"].ToString();
                    if (name == "AtPublic" || name == "AsSale")// 等待用户队列推送
                    {
                        JObject data = JObject.Parse(m_Message["data"].ToString());
                        string fromUserId = data["fromUserId"].ToString();
                        JObject LastContent = JObject.Parse(data["content"].ToString());
                        Shell.WriteLine("需要处理的消息：" + LastContent);
                        if (UserFromList.ContainsKey(fromUserId))//已有打开窗口
                        {
                            UserFromList[fromUserId].GetNoticeMessage(data);
                        }
                        else//没有打开窗口
                        {
                            if (!UserWaitList.ContainsKey(fromUserId))
                            {
                                string[] xCode = LastContent["extra"].ToString().Split('|');
                                string pubCode = xCode.Count() > 0 ? xCode[0] : "";
                                string accessCode = xCode.Count() > 1 ? xCode[1] : "";
                                //UserWaitList.Add(m_SenderId, new User() { userId = fromUserId, personalMsg ="", hisMsg = data["content"].ToString() });
                                UserWaitList.Add(fromUserId,
                                  new User()
                                  {
                                      userId = fromUserId,
                                      //personalMsg = "[" + ResouceHelper.GetPubName(pubCode) + "]",
                                      pubNumber = pubCode,
                                      accessCode = accessCode,
                                      isNewAccessUser = true,
                                      hisMsg = new List<string> { data.ToString() }
                                  });
                                UserInfoListenerEventHandler user_info_callback2 = new UserInfoListenerEventHandler(get_user_info_callback_delegate.get_whait_userinfo_call_back);
                                rcsdk.GetUserInfo(fromUserId, user_info_callback2);
                            }
                            else
                            {
                                UserWaitList[fromUserId].hisMsg.Add(data.ToString());
                            }
                        }
                    }
                    else if (name == "AtUser")
                    {
                        var jobContent = JObject.Parse(m_Message["data"].ToString())["content"];
                        var sendId = JObject.Parse(jobContent.ToString())["extra"].ToString();
                        Shell.WriteLine("info:AtUser#当前用户ID：" + sendId);
                        if (UserFromList.ContainsKey(sendId))
                        {
                            UserFromList[sendId].GetMessage(m_Message, true);
                        }
                        //{ "count":0,"m_ClazzName":"RC:CmdNtf","m_ConversationType":1,"m_Direction":true,"m_Message":"{\"name\":\"AtPublic\",\"data\":{\"appId\":\"\",\"fromUserId\":\"69494\",\"targetId\":\"69108\",\"targetType\":1,\"GroupId\":\"\",\"classname\":\"RC:TxtMsg\",\"content\":\"{\\\"content\\\":\\\"哆啦A梦\\\",\\\"extra\\\":\\\"888\\\"}\",\"dateTime\":\"2016-06-20 17:24:28\",\"msgUID\":\"\"}}","m_MessageId":14,"m_MessageType":0,"m_RcvTime":"2016-06-20 17:25:49","m_ReadStatus":0,"m_SendStatus":30,"m_SendTime":"2016-06-20 17:25:49","m_SenderId":"10000","m_TargetId":"10000","result":"success"}
                    }
                    #endregion
                }
                else if (UserFromList.ContainsKey(m_SenderId))
                {
                    UserFromList[m_SenderId].GetMessage(jobj);
                }
                else if (UserWaitList.ContainsKey(m_SenderId))//没有聊天窗口，但是在等待区域
                {
                    UserWaitList[m_SenderId].msgCount++;
                    //做消息条数处理
                }
                else//没有聊天窗口，也不在等待区域 等待用户
                {
                    UserWaitList.Add(m_SenderId, new User() { userId = m_SenderId, personalMsg = "" });
                    rcsdk.GetUserInfo(m_SenderId, user_info_callback);
                }
            }
            catch (Exception ex) { LogHelper.Error("消息监听及委托||set_message_listener_call_back||" + ex.ToString()); }

        }
        /// <summary>
        /// 网络异常监听
        /// </summary>
        /// <param name="json_str"></param>
        public void exception_listener_call_back(string json_str)
        {
            //在这里添加通知UI层代码
            JObject jobj = JObject.Parse(json_str);
        }
        /// <summary>
        /// 查找客户信息（并新增等待等待用户）UI控制
        /// </summary>
        /// <param name="json_str"></param>
        public void whait_userinfo_dealwith(string json_str)
        {
            //在这里添加通知UI层代码
            //{"result":"success","userId":"1","userName":"测试账号","userPortrait":"http://www.qqw21.com/article/UploadPic/2012-11/201211259378560.jpg"}
            Shell.WriteLine("客户接收到消息：" + json_str);
            JObject jobj = JObject.Parse(json_str);
            string result = jobj["result"].ToString();
            if (result == "success")
            {
                string userId = jobj["userId"].ToString();
                string userName = jobj["userName"].ToString();
                string userPortrait = jobj["userPortrait"].ToString();
                User user = new User() { userId = userId, name = userName, portraitUri = userPortrait };
                user.msgCount++;
                if (UserWaitList.ContainsKey(userId))
                {
                    UserWaitList[userId].userId = user.userId;
                    UserWaitList[userId].name = user.name;
                    UserWaitList[userId].portraitUri = user.portraitUri;
                }
                else
                {
                    UserWaitList.Add(userId, user);
                }
                //新增一条等待客户(等于1证明是第一次有信息)
                if (user.msgCount == 1)
                {
                    AddWaitUserInControl(user);
                }
            }
        }

        /// <summary>
        /// 新增等待客户
        /// </summary>
        /// <param name="user"></param>
        public void AddWaitUserInControl(User user)
        {
            //if (ClbSession.Items.Count == 0)
            //{
            //    ClbSession.Items.Add(_chatSession);
            //}
            CCWin.SkinControl.ChatListSubItem itemSub = new CCWin.SkinControl.ChatListSubItem
            {
                DisplayName = user.name,
                NicName = user.userId,
                PersonalMsg = user.personalMsg,
                HeadImage = Image.FromStream(ImgHelper.DownLoadPicToImage(user.portraitUri))
            };
            ClbSession.Items[0].SubItems.Add(itemSub);
            //_chatSession.SubItems.Add(itemSub);
        }
        /// <summary>
        /// 查找客户信息（会话列表）
        /// </summary>
        /// <param name="json"></param>
        public void Session_UserInfo_DealWith(string json)
        {
            JObject jobj2 = JObject.Parse(json);
            string result = jobj2["result"].ToString();
            if (result == "success")
            {
                string userId = jobj2["userId"].ToString();
                string userName = jobj2["userName"].ToString();
                string userPortrait = jobj2["userPortrait"].ToString();
                var itemSub = new CCWin.SkinControl.ChatListSubItem
                {
                    DisplayName = userName,
                    NicName = userId,
                    PersonalMsg = "",
                    HeadImage = Image.FromStream(ImgHelper.DownLoadPicToImage(userPortrait))
                };
                _chatSession.SubItems.Add(itemSub);
            }
        }
        /// <summary>
        /// 获取会话列表
        /// </summary>
        /// <param name="json"></param>
        public void GetConversationList(string json)
        {
            //{ "count":3,"data":[{"m_Conversation":{"m_ConversationType":1,"m_Draft":"","m_IsTop":0,"m_LastTime":"2016-06-28 14:23:05","m_PortraitUrl":"","m_TargetId":"69494","m_Title":"","m_UnreadCount":0},"m_Message":{"m_ClazzName":"RC:TxtMsg","m_Direction":false,"m_Message":"{\r\n  \"content\": \"砥砺风节\",\r\n  \"extra\": \"69494|011\"\r\n}","m_MessageId":25,"m_MsgUId":"5APK-2E53-841H-7A5C","m_RcvTime":"2016-06-29 10:42:58","m_ReadStatus":1,"m_SendStatus":30,"m_SendTime":"2016-06-29 10:42:59","m_SenderId":"2"}},{"m_Conversation":{"m_ConversationType":1,"m_Draft":"","m_IsTop":0,"m_LastTime":"2016-06-25 15:53:02","m_PortraitUrl":"","m_TargetId":"10000","m_Title":"系统管理�?,"m_UnreadCount":14},"m_Message":{"m_ClazzName":"RC:CmdNtf","m_Direction":true,"m_Message":"{\"name\":\"AtUser\",\"data\":{\"appId\":\"\",\"fromUserId\":\"2\",\"targetId\":\"KBCCF0110000\",\"targetType\":1,\"GroupId\":\"\",\"classname\":\"RC:TxtMsg\",\"content\":\"{\\r\\n  \\\"content\\\": \\\"test66\\\",\\r\\n  \\\"extra\\\": \\\"69494|011\\\"\\r\\n}\",\"dateTime\":\"2016-06-28 18:17:24\",\"msgUID\":\"\"}}","m_MessageId":24,"m_MsgUId":"5APD-0RK4-0400-001I","m_RcvTime":"2016-06-28 18:17:25","m_ReadStatus":0,"m_SendStatus":30,"m_SendTime":"2016-06-28 18:17:25","m_SenderId":"10000"}},{"m_Conversation":{"m_ConversationType":1,"m_Draft":"","m_IsTop":0,"m_LastTime":"2016-06-25 16:12:25","m_PortraitUrl":"","m_TargetId":"KBCCF0110000","m_Title":"","m_UnreadCount":1},"m_Message":{"m_ClazzName":"RC:TxtMsg","m_Direction":true,"m_Message":"{\r\n  \"content\": \"sfdads\"\r\n}","m_MessageId":5,"m_MsgUId":"5AOD-7FGF-4400-001I","m_RcvTime":"2016-06-25 17:35:02","m_ReadStatus":0,"m_SendStatus":30,"m_SendTime":"2016-06-25 16:12:25","m_SenderId":"KBCCF0110000"}}],"result":"success"}

            Shell.WriteLine("warn会话列表信息:"+json);
            JObject jobj = JObject.Parse(json);
            if (jobj["result"].ToString().Equals("success"))
            {
                if (jobj["count"].ToString().Equals("0"))
                {
                    return;
                }
                var data = jobj["data"].Children();
                foreach (var jToken in data)
                {
                    JObject jobt = JObject.Parse(jToken.ToString());
                    JObject jobc = JObject.Parse(jobt["m_Conversation"].ToString());
                    //Task.Run(() => rcsdk.GetUserInfo(jobc["m_TargetId"].ToString(), get_user_info_callback_delegate.Get_Session_UserInfo_CallBack));
                    Task.Run(() => rcsdk.GetUserInfo(jobc["m_TargetId"].ToString(), str =>
                    {
                        JObject jobj2 = JObject.Parse(str);
                        string result = jobj2["result"].ToString();
                        if (result == "success")
                        {
                            string userId = jobj2["userId"].ToString();
                            string userName = jobj2["userName"].ToString();
                            string userPortrait = jobj2["userPortrait"].ToString();
                            var itemSub = new CCWin.SkinControl.ChatListSubItem
                            {
                                DisplayName = userName,
                                NicName = userId,
                                PersonalMsg = "",
                                HeadImage = Image.FromStream(ImgHelper.DownLoadPicToImage(userPortrait))
                            };
                            _chatSession.SubItems.Add(itemSub);
                        }
                    }));
                }
            }
        }

        /// <summary>
        /// 连接成功监听
        /// </summary>
        /// <param name="json"></param>
        public void ConnectOpenCallBack(string json)
        {
            JObject jobj = JObject.Parse(json);
            string result = jobj["result"].ToObject<string>();
            //string userId = jobj["userId"].ToString();
            //int err_code = jobj["err_code"].ToObject<int>();
            Shell.WriteLine(jobj.ToString());
            if (result == "success")
            {
                InitCurDaySession();
            }
        }

        /// <summary>
        /// 发送消息回调UI控制
        /// </summary>
        /// <param name="json_str"></param>
        public void sendmessage_call_dealwith(string json_str)
        {
            Shell.WriteLine(string.Format("sendmessage_call_dealwith:{0}", json_str));
            //在这里添加通知UI层代码
            JObject jobj = JObject.Parse(json_str);
            if (jobj["result"].ToObject<string>() == "falied")
            {
                MessageBox.Show("发送信息失败");
            }
        }
        #endregion

        /// <summary>
        /// 初始化好友列表
        /// </summary>
        private void InitFrendsList()
        {
            var users = new List<User>();
            var security = new SecurityHelper();
            var url = InterfaceXmlHelper.GetInterFaceUrlFromXmlConfig(InterfaceXmlHelper.InterFaceConfig.GetList_PublicNumber);
            var json = "{\"PUBLICNUMS\":[\"PHQTG01G0001\",\"PUJMD01G0002\",\"PUJHD01G0003\",\"PBCCF01G0004\"]}";
            Shell.WriteLine("公众号参数" + json);
            var content = security.CreateSignEncodingStr("im_crm", json);
            Shell.WriteLine("公众号URL：" + url + "?" + content);
            //{ "result":true,"retcode":10000,"retmsg":[{"PUBLICNUM":"23839","PUBLICCODE":"23839","PUBLICNAME":"环球公众号23839","ACCESSARRAY":[{"ACCESSCODE":"23839","ACCESSNAME":"步步为营23839"}],"KEY":"REDIS_KEY_ACCESSCODE:"},{"PUBLICNUM":"23838","PUBLICCODE":"23838","PUBLICNAME":"环球公众号23838","ACCESSARRAY":[{"ACCESSCODE":"23838","ACCESSNAME":"步步为营23838"}],"KEY":"REDIS_KEY_ACCESSCODE:"}]}
            var result = Utility.GetData(url, content, Encoding.UTF8);
            //Shell.WriteLine(result);
            //Shell.WriteLine(security.decyptData("im_crm", JsonConvert.DeserializeObject<string>(result)));
            //var obj = JObject.Parse(security.decyptData("im_crm", JsonConvert.DeserializeObject<string>(result)));
            Shell.WriteLine("获取公众号结果：" + security.decyptData("im_crm", result.Trim(new char[] {'"'})));
            var obj = JObject.Parse(security.decyptData("im_crm", result.Trim(new char[] {'"'})));
            if ((bool) obj["result"])
            {
                var publicUser = JsonConvert.DeserializeObject<List<PublicUser>>(obj["retmsg"].ToString());
                foreach (var user in publicUser)
                {
                    foreach (var accessArray in user.ACCESSARRAY)
                    {
                        users.Add(new User()
                        {
                            userId = user.PUBLICNUM + "|" + accessArray.AccessCode,
                            //name = user.PUBLICNAME + "(" + accessArray.AccessName + ")",
                            name = user.PUBLICNAME,
                            nickname = user.PUBLICNUM,
                            personalMsg = accessArray.AccessName,
                            portraitUri = "http://7xs3cl.com1.z0.glb.clouddn.com/001.png"
                        });
                    }
                }
                foreach (var user in users)
                {
                    var itemSub = new CCWin.SkinControl.ChatListSubItem
                    {
                        DisplayName = user.name,
                        NicName = user.userId,
                        PersonalMsg = user.personalMsg,
                        HeadImage = Image.FromStream(ImgHelper.DownLoadPicToImage(user.portraitUri))
                    };
                    _chatPublic.SubItems.Add(itemSub);
                }
            }
        }

        private void InitCurDaySession()
        {
            rcsdk.GetConversationList(new int[] {1}, 1, get_con_listex_callback_delegate.get_con_listex_call_back);
        }

        #region 双击打开客户聊天窗口

        /// <summary>
        /// 双击客户打开客户聊天窗口
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        /// <param name="es"></param>
        private void clbDDuser_DoubleClickSubItem(object sender, CCWin.SkinControl.ChatListEventArgs e, MouseEventArgs es)
        {
            NewChatFrom(e, es);
        }
        private void ClbSession_DoubleClickSubItem(object sender, CCWin.SkinControl.ChatListEventArgs e, MouseEventArgs es)
        {
            NewChatFrom(e, es);
        }
        /// <summary>
        /// 类型
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="ev"></param>
        /// <param name="type">0：等待用户 1:当天用户 2： 我的分组</param>
        public void NewChatFrom(object sender, EventArgs ev)
        {
            CCWin.SkinControl.ChatListEventArgs e = sender as CCWin.SkinControl.ChatListEventArgs;
            string userid = e.SelectSubItem.NicName.ToString();
            if (e == null)
                return;
            if (UserFromList.ContainsKey(e.SelectSubItem.NicName))
            {
                foreach (TabPage item in UserDetialTab.TabPages)
                {
                    if (item.Tag == null)
                        continue;
                    if (item.Tag.ToString() == userid)
                    {
                        UserDetialTab.SelectedTab = item;
                        break;
                    }
                }

            }
            else
            {
                TabPage page = new TabPage(e.SelectSubItem.DisplayName);
                page.ImageKey = userid;
                UserDetialTab.TabPages.Add(page);
                UserDetialTab.SelectedTab = page;

                FrmChat chatFrom = new FrmChat(e.SelectSubItem, CusServiceUser);
                chatFrom.Tag = userid;
                chatFrom.Dock = DockStyle.Fill;
                chatFrom.TopLevel = false;
                chatFrom.Parent = page;
                chatFrom.FormClosing += chatFrom_FormClosing;
                UserFromList.Add(userid, chatFrom);
                page.Tag = e.SelectSubItem.NicName;
                chatFrom.Show();
            }
        }

        #endregion

        #region 窗体关闭事件
        void chatFrom_FormClosing(object sender, FormClosingEventArgs e)
        {
            object userTag = (sender as FrmChat).Tag;
            if (userTag == null)
                return;
            string userid = userTag.ToString();
            if (UserFromList.ContainsKey(userid))
            {
                UserFromList.Remove(userid);
            }
            //关闭对应的Tab
            if (string.IsNullOrEmpty(userid))
                return;
            string pageUserId = "";
            foreach (TabPage npage in UserDetialTab.TabPages)
            {
                if (npage.Tag == null)
                    continue;
                pageUserId = npage.Tag.ToString();
                if (pageUserId == userid)
                {
                    UserDetialTab.TabPages.Remove(npage);
                }
            }
        }
        #endregion

        #region 调整窗体大小事件
        private void FrmMain_ResizeEnd_1(object sender, EventArgs e)
        {
            if (this.Width <= 800)
            {
                this.Width = 800;
            }
            if (this.Height <= 400)
            {
                this.Height = 400;
            }
        }
        #endregion
        

        #region 控制台关闭事件
        /// <summary>
        /// 控制台关闭事件
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void FrmMain_FormClosing(object sender, FormClosingEventArgs e)
        {

        }
        #endregion

        #region 用户聊天Tab关闭事件

        /// <summary>
        /// 关闭选项卡是，清除窗体，清除用户数据
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="e"></param>
        private void UserDetialTab_TabePageClosing(object sender, CCWin.SkinControl.TabPageEventArgs e)
        {
            if (e.ColseTabPage.Tag == null)
                return;
            string userid = e.ColseTabPage.Tag.ToString();
            if (UserFromList.ContainsKey(userid))
            {
                //UserFromList[userid].Close();
                UserFromList.Remove(userid);
            }
        }

        #endregion

        
    }
}
