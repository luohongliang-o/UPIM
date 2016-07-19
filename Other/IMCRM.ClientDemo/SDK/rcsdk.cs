using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace rcsdk_test
{
   public struct RcTargetEntry
    {
        [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 64)]
        public string targetId;
        [MarshalAs(UnmanagedType.ByValTStr, SizeConst = 256)]
        public string targetName;
    }

    //rcsdk callbacks
    /*
    jobj
    {
    "result":"falied/success",
    "err_code":0/1, //错误码   0-成功 非0-失败
    "userId":"4378" //描述信息 成功，返回用户id，失败，返回错误描述
    }
    */
    public delegate void ConnectAckListenerEventHandler([MarshalAs(UnmanagedType.LPWStr)] string json_str);

    /*
    jobj
    {
    "result":"success/failed"
    "err_code":0,   //错误码   0-成功 非0-失败
    "data":""       //黑名单用户列表 多个id以回车分割
    }
    */
    public delegate void BlacklistInfoListenerEventHandler([MarshalAs(UnmanagedType.LPWStr)] string json_str);

    /*
    jobj
    {
    "opStatus":0, //操作结果    0-成功 非0-失败
    "bizStatus":0 //业务数据值  根据具体业务返回响应的业务数据值
    }
    */
    public delegate void BizAckListenerEventHandler([MarshalAs(UnmanagedType.LPWStr)] string json_str);

    /*
    jobj
    {
    "result":"falied/success",
    "err_code":0/1 //操作结果 0-成功 非0-失败
    }
    */
    public delegate void PublishAckListenerEventHandler([MarshalAs(UnmanagedType.LPWStr)] string json_str);

    public delegate void ChatroomInfoListenerEventHandler([MarshalAs(UnmanagedType.LPWStr)] string json_str);
    /*
    jobj
    {
    "data":"hello", //信息描述
    }
    */
    public delegate void NaviDataListenerEventHandler([MarshalAs(UnmanagedType.LPWStr)] string json_str);

    /*
    jobj
    {
    "result":"falid/success",
    "userId":"7788",//用户id
    "err_code":0/1,//错误码 0-成功 1-失败
    "userName":"1122"//用户名称
    "userPortrait" : "http://wejgilgldja.jfa" //用户头像
    }
    */
    public delegate void UserInfoListenerEventHandler([MarshalAs(UnmanagedType.LPWStr)] string json_str);

    /*
    jobj
    {
    "result":"falid/success",
    "discussionId":"7788",  //讨论组id
    "err_code":0/1          //错误码
    }
    */
    public delegate void CreateDiscussionListenerEventHandler([MarshalAs(UnmanagedType.LPWStr)] string json_str);

    /*
    jobj
    {
    "process": 7       //进度值
    }
    jobj
    {
        "result":"falid/success",
        "url":"http//....",               //信息描述 成功，返回图片uri 失败 返回错误描述
        "err_code":0/1              //错误码 0 - 成功 非0 - 失败
    }
    */
    public delegate void ImageListenerEventHandler([MarshalAs(UnmanagedType.LPWStr)] string json_str);

    /*
    jobj
    {
    "result":"falid/success",
    "err_code":0/1,     //错误码
    "startTime":"1122"  //开始时间
    "spanMins" : 11221  //时间间隔
    }
    */
    public delegate void PushSettingListenerEventHandler([MarshalAs(UnmanagedType.LPWStr)] string json_str);

    /*
    CsharpCMessageInfo
    jobj
    {
    "m_TargetId":"99",          //交互方id
    "m_SenderId": "" ,          //发方id
    "m_ClazzName":"",		    //类别
    "m_ExtraMessage":"",        //信息
    "m_ExtraMessage":"",        //附加信息
    "m_Push":"",                //推送消息
    "m_AppData":"",             //AppData
    "m_ConversationType":0,     //通道类型  1-PRIVATE 2-DISCUSSION 3-GROUP 4-CHATROOM 5-CUSTOMSERVICE
    "m_MessageId":9,            //消息id    消息在数据库中的id号
    "m_Direction":0,            //消息方向  0-发送的消息 1-接收的消息
    "m_ReadStatus":0,           //读取状态  0-未读 1-已读
    "m_SendStatus":20,          //发送状态  10-发送中 20-发送失败 30-发送成功
    "m_SendTime":9,             //接收时间
    "m_RcvTime":9,              //接收时间
    "m_MessageType":0		    //信息类别  0-实时接收信息 1-历史漫游
    }
    */
    public delegate void MessageInfoEventHandler([MarshalAs(UnmanagedType.LPWStr)] string json_str);
    //json_str同上 
    public delegate void MessageListenerEventHandler([MarshalAs(UnmanagedType.LPWStr)] string json_str);

    /*
    jobj
    {
    "length":1, //数据长度
    "data":""   //数据
    }
    */
    public delegate void DataBufferEventHander([MarshalAs(UnmanagedType.LPWStr)] string json_str);

    /*
    jobj
    {
    "nstatus": 31010 (RC_DISCONN_KICK)网络被踢  30002 (RC_NET_UNAVAILABLE)网络不可用  30011(RC_SOCKET_DISCONNECTED)网络超时 30013(RC_PONG_RECV_FAIL)心跳包超时 timeout of recv of socket   //异常状态码
    "data":""       //异常描述
    }
    */
    public delegate void ExceptionListenerEventHandler([MarshalAs(UnmanagedType.LPWStr)] string json_str);

    /*
    AccountInfo
    jobj
    {
    "m_AccountId":"99",         //账号id
    "m_AccountName": "" ,       //账号id
    "m_AccountUri":"",			//uri
    "m_Extra":"",			    //附加信息
    "m_AccountType":9,          //账号类别
    "m_LastTime":9				//最近时间
    }
    */
    public delegate void AccountInfoEventHandler([MarshalAs(UnmanagedType.LPWStr)] string json_str);

    /*
    jobj
    {
    "result":"falid/success",
    "count":0       //数量
    "data":jobj,    //CsharpDiscussionInfo 请参见下面讨论组数据结果
    "err_code":0/1  //错误码
    }
    */
    public delegate void DiscussionInfoListenerEventHandler([MarshalAs(UnmanagedType.LPWStr)] string json_str);

    /*
    CsharpDiscussionInfo
    jobj
    {
    "m_Id":"99",            //讨论组或群组id
    "m_Name": "" ,          //讨论组名称
    "m_AdminId":"",         //管理员id
    "m_UserIds":"",         //群组成员id,使用\n分割
    "m_ConversationType":1, //通道类型 1-PRIVATE 2-DISCUSSION 3-GROUP 4-CHATROOM 5-CUSTOMSERVICE
    "m_InviteStatus":0,     //开放邀请状态   0 - 开放邀请 1 - 关闭开放邀请
    "m_BlockPush":0         //新消息通知阻止状态 0 - 未阻止 100 - 阻止
    }
    */
    public delegate void DisInfoSyncEventHandler([MarshalAs(UnmanagedType.LPWStr)] string json_str);

    /*
    CsharpUserInfo
    jobj
    {
    "m_Id":"99",                //用户id
    "m_Name": "" ,              //用户名
    "m_Portrait":"",            //用户url
    "m_AccountExtra":"",        //用户account
    "m_categoryId":9,           //类别id
    "m_BlockPush":9,            //新消息通知阻止状态 0 - 未阻止 100 - 阻止
    }
    */
    public delegate void UserInfoEventHandler([MarshalAs(UnmanagedType.LPWStr)] string json_str);

    /*
    CsharpConversation
    {
    CsharpConversationInfo
    "m_TargetId":"99",          //会话id
    "m_Title": "" ,             //回话标题
    "m_Draft":"",               //会话草稿
    "m_ConversationType":9,     //会话类型 单聊 群聊 讨论组 。。。
    "m_IsTop":9,                //会话是否置顶
    "m_BlockPush":9,            //是否要push消息
    "m_GroupStatus":9,          //讨论组状态
    "m_UnreadCount":9,          //未读消息数
    "m_LastTime":9,             //最近会话时间

    CsharpCMessageInfo
    {
    "m_TargetId":"99",          //交互方id
    "m_SenderId": "" ,          //发方id
    "m_ClazzName":"",		    //类别
    "m_ExtraMessage":"",        //信息
    "m_ExtraMessage":"",        //附加信息
    "m_Push":"",                //推送消息
    "m_AppData":"",             //AppData
    "m_ConversationType":0,     //通道类型  1-PRIVATE 2-DISCUSSION 3-GROUP 4-CHATROOM 5-CUSTOMSERVICE
    "m_MessageId":9,            //消息id   消息在数据库中的id号
    "m_Direction":0,            //消息方向  0-发送的消息 1-接收的消息
    "m_ReadStatus":0,           //读取状态  0-未读 1-已读
    "m_SendStatus":20,          //发送状态  10-发送中 20-发送失败 30-发送成功
    "m_SendTime":9,             //接收时间
    "m_RcvTime":9,              //接收时间
    "m_MessageType":0		    //信息类别  0-实时接收信息 1-历史漫游
    }

    CsharpUserInfo
    {
    "m_Id":"99",                //用户id
    "m_Name": "" ,              //用户名
    "m_Portrait":"",            //用户url
    "m_AccountExtra":"",        //用户account
    "m_categoryId":9,           //类别id
    "m_BlockPush":9,            //新消息通知阻止状态 0 - 未阻止 100 - 阻止
    }
    */
    public delegate void ConversationInfoEventHandler([MarshalAs(UnmanagedType.LPWStr)] string json_str);

    /*
    jobj
    {
    "result":"success/failed"
    "err_code":1,           //错误码 0 - 成功 非0 - 失败
    "data":""               //信息描述 成功，上传返回上传token，下载返回图片uri 失败 返回错误描述
    }
   */
    public delegate void TokenListenerEventHandler([MarshalAs(UnmanagedType.LPWStr)] string json_str);

    unsafe class rcsdk
    {
        /**
        *  初始化全局实例
        *
        *  @param appid        应用id
        *  @param appName      应用名称
        *  @param deviceId     设备id
        *  @param localPath    缓存路径
        *  @param databasePath 数据库存储路径
        *
        *  @return client指针   NULL - 失败 非NULL - 成功
        */

        [DllImport("rcsdk.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern IntPtr InitClient(string appId, string appName, string deviceId, [MarshalAs(UnmanagedType.LPWStr)] string localPath, [MarshalAs(UnmanagedType.LPWStr)] string databasePath);

        /**
        *  建立服务器连接
        *
        *  @param token    登录token，由后台配置从网站得到
        *  @param listener 监听 连接结果通过监听中的函数通知
        */

        [DllImport("rcsdk.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern void Connect(string token, ConnectAckListenerEventHandler callback,bool ipv6 = false);
        /**
        *  断开服务器连接，不会删除全局实例
        *
        *  @param state 断开时指定的状态 2断开连接接受推送 4断开连接不接收推送
        */

        [DllImport("rcsdk.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern void Disconnect(int state);

        /**
        *  iOS设置DeviceToken
        *
        *  @param deviceId DeviceToken
        */

        [DllImport("rcsdk.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern void SetDeviceId(string deviceId);

        /**
        *  设置消息监听
        *
        *  @param listener 消息监听器
        */

        [DllImport("rcsdk.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern void SetMessageListener(MessageListenerEventHandler callback);

        /**
        *  设置异常监听
        *
        *  @param listener 监听器
        */

        [DllImport("rcsdk.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern void SetExceptionListener(ExceptionListenerEventHandler callback);

        /**
        *  注册信息类别
        *
        *  @param clazzName   信息类别
        *  @param operateBits 操作位 1 - 是否入库 2 - 是否标记为已读
        *
        *  @return 操作结果 true - 成功 false - 失败
        */

        [DllImport("rcsdk.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern bool RegisterMessageType(string clazzName, uint operateBits);

        /**
        *  备注：
        *  上层定义的 PRIVATE DISCUSSION 等值需要与此对应，因为暂时的CHATROOM和TEMPGROUP都未使用，
        *  使用时，这两个值需匹配上。
        *  PB中通道的定义如下
        *  enum ChannelType
        *  {
        *      PERSON = 1,
        *      PERSONS = 2,
        *      GROUP = 3,
        *      TEMPGROUP = 4,
        *      CUSTOMERSERVICE = 5
        *  };
        */

        /*
        ************************************************
        *  会话处理相关函数
        ************************************************
        */

        /**
        *  发送消息
        *
        *  @param targetId     交互方id
        *  @param categoryId   通道类型
        1 - PRIVATE             单人
        2 - DISCUSSION          讨论组
        3 - GROUP               群组
        4 - CHATROOM            聊天室
        5 - CUSTOMERSERVICE     客服
        6 - SYSTEM              系统消息
        7 - MC                  MC消息
        8 - MP                  MP消息
        *  @param transferType 传输类别
        1 - S 消息数据服务器不缓存，不保证信息到达接收方
        2 - N 消息数据服务器缓存，交互方会收到通知消息
        3 - P 消息数据服务器缓存，交互方不在线会收到PUSH消息
        *  @param clazzname    消息类别
        *  @param message      消息内容 json内容
        *  @param messageId    消息id 消息在数据库中的Id
        *  @param callback     发送监听
        */

        [DllImport("rcsdk.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern void sendMessage(string targetId, int categoryId, int transferType, string clazzname, [MarshalAs(UnmanagedType.LPWStr)] string message, string push, string appData, int messageId, PublishAckListenerEventHandler callback);

        /**
         *  信息入库
         *
         *  @param targetId   交互方id
         *  @param categoryId 通道类型
         *  @param clazzName  消息类别
         *  @param senderId   发送方id
         *  @param message    消息内容
         *  @param driection = false 使用 m_userId作为senderId 当发送消息时 true 不使用 m_userId 做senderId
         *  @param readStatus = 0
         *  @return 信息在库中的id -1 失败 >0 成功
         */
        [DllImport("rcsdk.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern int SaveMessage(string targetId, int categoryId, string clazzName, string senderId, [MarshalAs(UnmanagedType.LPWStr)] string message, string push, string appData, bool driection, int readStatus);

        /**
        *  设置草稿
        *
        *  @param targetId     交互方id
        *  @param categoryId   通道类型
        *  @param draftMessage 草稿内容 NULL - 清除草稿 非NULL - 设置草稿
        *
        *  @return 操作结果 true - 成功 false - 失败
        */
        [DllImport("rcsdk.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern bool SaveTextMessageDraft(string targetId, int categoryId, string draftMessage);

        /**
        *  获取草稿
        *
        *  @param targetId     交互方id
        *  @param categoryId   通道类型
        *  @param draftMessage 草稿内容
        *
        *  @return 操作结果 true - 成功 false - 失败
        */
        [DllImport("rcsdk.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern bool GetTextMessageDraft(string targetId, int categoryId, DataBufferEventHander callback);

        /**
        *  设置会话置顶
        *
        *  @param targetId   交互方id
        *  @param categoryId 通道类型
        *  @param bTop       是否置顶 true - 置顶 false - 取消置顶
        *
        *  @return 操作结果 true - 成功 false - 失败
        */
        [DllImport("rcsdk.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern bool SetConversationToTop(string targetId, int categoryId, bool bTop);

        /**
        *  删除消息
        *
        *  @param targetId   交互方id
        *  @param categoryId 通道类型
        *
        *  @return 操作结果 true - 成功 false - 失败
        */
        [DllImport("rcsdk.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern bool ClearMessages(string targetId, int categoryId);

        /**
        *  删除某个会话
        *
        *  @param targetId   交互方id
        *  @param categoryId 通道类型
        *
        *  @return 操作结果 true - 成功 false - 失败
        */
        [DllImport("rcsdk.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern bool RemoveConversation(string targetId, int categoryId);

        /**
        *  清除会话消息的未读状态
        *
        *  @param targetId   交互方id
        *  @param categoryId 通道类型
        *
        *  @return 操作结果 true - 成功 false - 失败
        */

        [DllImport("rcsdk.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern bool ClearMessagesUnreadStatus(string targetId, int categoryId);


        /**
	    * 根据会话类型的目标 Id，回调方式获取某消息类型标识的N条历史消息记录。
	    *
	    * @param targetId			目标 Id。根据不同的  categoryId，可能是用户 Id、讨论组 Id、群组 Id 或聊天室 Id。
	    * @param categoryId			会话类型。不支持传入 categoryId.CHATROOM。
	    * @param recordTime         从该时间点开始获取消息。即：消息中的 sentTime；第一次可传 0，获取最新 count 条。
	    * @param rowCount			要获取的消息数量，最多 20 条。
	    * @param callback			获取历史消息记录的回调，按照时间顺序从新到旧排列。
	    */

        [DllImport("rcsdk.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern bool GetRemoteHistoryMessage(string targetId, int categoryId, System.Int64 recordTime, int rowCount, MessageListenerEventHandler callback);

        [DllImport("rcsdk.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern void SubscribePublicService(string targetId, int categoryId, bool subscribe, PublishAckListenerEventHandler callback);

        [DllImport("rcsdk.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern void DownloadAccounts();

        [DllImport("rcsdk.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern bool LoadAccountInfo(AccountInfoEventHandler callback);

        /**
        *  获取分页消息，result需要在外面delete
        *
        *  @param targetId   交互方id
        *  @param categoryId 通道类型
        *  @param lastId     消息id
        *  @param count      选取消息条数
        *  @param result     json结构的消息列表
        *
        *  @return 操作结果 true - 成功 false - 失败
        */

        [DllImport("rcsdk.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern bool GetHistoryMessages(string targetId, int categoryId, string clazzName, int lastId, int count, MessageInfoEventHandler callback);
        /**
        *  获取最近的会话列表
        *
        *  @param conversationDict 通道类型
        *  @param conversationCount 通道数
        *  @param DataBuffer        json结构的会话列表
        *
        *  @return 操作结果 true - 成功 false - 失败
        */

        [DllImport("rcsdk.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern bool GetConversationList(int[] conversationDict, int conversationCount, ConversationInfoEventHandler callback);

        /**
        *  获取最近的某类信息的会话列表
        *
        *  @param targetId   交互方id
        *  @param categoryId 通道类型
        *  @param result     json结构的会话列表
        *
        *  @return 操作结果 true - 成功 false - 失败
        */

        [DllImport("rcsdk.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern bool GetConversation(string targetId, int categoryId, ConversationInfoEventHandler callback);

        /**
        *  获取未读消息数
        *
        *  @return >=0 未读信息条数-客服未读数 -1 数据库未初始化
        */

        [DllImport("rcsdk.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern int GetTotalUnreadCount();
        /**
        *  获取未读信息数
        *
        *  @param targetId   交互方id
        *  @param categoryId 通道类型
        *
        *  @return >=0 未读消息数目 -1 数据库未初始化
        */

        [DllImport("rcsdk.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern int GetUnreadCount(string targetId, int categoryId);

        /**
        *  获取未读信息数
        *
        *  @param conversationDict  通道类型数组
        *  @param conversationCount 数组元素个数
        *
        *  @return >=0 未读消息数目 -1 数据库未初始化
        */

        [DllImport("rcsdk.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern int GetConversationUnreadCount(int[] conversationDict, int conversationCount);

        /**
        *  设置阻止push信息
        *
        *  @param targetId   交互方id
        *  @param categoryId 通道类型
        *  @param block      是否阻止 true - 阻止 false - 取消阻止
        *  @param listener   监听器 推送值：0-未阻止 100-已阻止
        */

        [DllImport("rcsdk.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern void SetConversationNotificationStatus(string targetId, int categoryId, bool block, BizAckListenerEventHandler callback);

        /**
        *  获取阻止状态
        *
        *  @param targetId    交互方id
        *  @param categoryId  通道类型
        *  @param fetchRemote 是否从服务器获取 true - 服务器 false - 本地
        *  @param listener    监听器 推送值：0-未阻止 100-已阻止
        */

        [DllImport("rcsdk.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern void GetConversationNotificationStatus(string targetId, int categoryId, BizAckListenerEventHandler callback);

        [DllImport("rcsdk.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern bool GetBlockPushSync(string targetId, int categoryId, ref int status);

        /**
        *  获取用户信息
        *
        *  @param userId      用户id
        *  @param listener    监听器 推送：用户id 用户名称 用户url
        */

        [DllImport("rcsdk.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern void GetUserInfo(string userId, UserInfoListenerEventHandler callback);

        [DllImport("rcsdk.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern bool GetUserInfoExSync(string userId, int category_id, UserInfoEventHandler callback);

        [DllImport("rcsdk.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern bool ClearConversations(int[] conversationDict, int conversationCount);

        /*
        ************************************************
        *  单条信息处理函数
        ************************************************
        */

        /**
        *  删除消息
        *
        *  @param messageId    消息id数组
        *  @param messageCount 消息数组数目
        *
        *  @return 操作结果：true - 成功 false - 失败
        */

        [DllImport("rcsdk.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern bool DeleteMessage(int[] messageId, int messageCount);

        /**
        *  设置消息内容
        *
        *  @param messageId 消息id
        *  @param content   消息内容
        *
        *  @return 操作结果：true - 成功 false - 失败
        */

        [DllImport("rcsdk.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern bool SetMessageContent(int messageId, string content);

        [DllImport("rcsdk.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern bool SetMessageDisplayText(int messageId, string content);

        /**
        *  设置附加信息
        *
        *  @param messageId    消息id
        *  @param extraMessage 附加信息内容
        *
        *  @return 操作结果：true - 成功 false - 失败
        */

        [DllImport("rcsdk.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern bool SetMessageExtra(int messageId, string extraMessage);

        /**
        *  设置消息读取状态
        *
        *  @param messageId  消息id
        *  @param readStatus 消息读取状态 0 - 未读 1 - 已读
        *
        *  @return 操作结果：true - 成功 false - 失败
        */

        [DllImport("rcsdk.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern bool SetMessageReadStatus(int messageId, int readStatus);

        /**
        *  设置消息发送状态
        *
        *  @param messageId  消息id
        *  @param readStatus 消息读取状态 10 - 发送中 20 - 发送失败 30 - 发送成功
        *
        *  @return 操作结果：true - 成功 false - 失败
        */

        [DllImport("rcsdk.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern bool SetMessageSendStatus(int messageId, int sendStatus);

        /**
        *  获取本地时间与服务器时间的差值
        *
        *  @return 差值 = 本地时间 - 服务器时间
        */

        [DllImport("rcsdk.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern System.Int64 GetDeltaTime();

        /*
        ************************************************
        *  文件处理函数
        ************************************************
        */

        /**
        *  上传文件
        *
        *  @param targetId   交互方id
        *  @param categoryId 通道类型
        *  @param nType      媒体类型 1 - Image 2 - Audio 3 - Video
        *  @param pbData     媒体数据
        *  @param nl         数据长度
        *  @param pListener  图片监听器 推送：进度 操作结果：成功 推送网络地址 失败 失败原因
        */
        //////////////////////////////////////////////////////////////////////////////////////////////////////const unsigned char* pbData 主要类型转换

        [DllImport("rcsdk.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern void UpLoadFile(string targetId, int categoryId, int nType, byte[] pbData/*string data*/, int nl, ImageListenerEventHandler callback, ImageListenerEventHandler process_callback);
        
        /**
        *  下载文件
        *
        *  @param targetId   交互方id
        *  @param categoryId 通道类型
        *  @param nType      媒体类型 1 - Image 2 - Audio 3 - Video
        *  @param pszUrl     媒体网络地址
        *  @param pListener  图片监听器 推送：进度 操作结果：成功 推送网络地址 失败 失败原因
        */

       [DllImport("rcsdk.dll", CallingConvention = CallingConvention.Cdecl)]
       public static extern void DownLoadFile(string targetId, int categoryId, int nType, string pszUrl, ImageListenerEventHandler callback, ImageListenerEventHandler process_callback);

        /*
        ************************************************
        *  讨论组相关函数
        ************************************************
        */

        /**
        *  创建讨论组
        *
        *  @param discussionName 讨论组名称
        *  @param userIds        邀请成员id数组
        *  @param idCount        数组元素个数
        *  @param listener       监听器 推送 成功 - 讨论组id 失败 - 出错原因
        */

        [DllImport("rcsdk.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern void CreateDiscussion(string discussionName, RcTargetEntry[] userIds, int idCount, CreateDiscussionListenerEventHandler callback);

        /**
        *  退出讨论组
        *
        *  @param discussionId  讨论组id
        *  @param listener      监听器 推送 0 - 成功 非0 - 失败
        */

        [DllImport("rcsdk.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern void QuitDiscussion(string discussionId, PublishAckListenerEventHandler callback);

        /**
        *  邀请加入讨论组
        *
        *  @param discussionId 讨论组id
        *  @param userIds      用户id数组
        *  @param idCount      数组原始个数
        *  @param listener     监听器 推送 0 - 成功 非0 - 失败
        */

        [DllImport("rcsdk.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern void AddMemberToDiscussion(string discussionId, RcTargetEntry[] userIds, int idCount, PublishAckListenerEventHandler callback);

        /**
        *  将某人踢出讨论组
        *
        *  @param discussionId 讨论组id
        *  @param userId       踢出的用户id
        *  @param listener     监听器 推送 0 - 成功 非0 - 失败
        */

        [DllImport("rcsdk.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern void RemoveMemberFromDiscussion(string discussionId, string userId, PublishAckListenerEventHandler callback);

        /**
        *  获取讨论组信息
        *
        *  @param discussionId 讨论组id
        *  @param categoryId   通道类型 只能是 discussion ，此参数稍后将被去掉
        *  @param listener     监听器 推送 0 - 成功，返回讨论组信息 非0 - 失败
        */

        [DllImport("rcsdk.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern void GetDiscussionInfo(string discussionId, int categoryId, DiscussionInfoListenerEventHandler callback);

        [DllImport("rcsdk.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern bool GetDiscussionInfoSync(string discussionId, int categoryId, DisInfoSyncEventHandler callback);

        /**
        *  讨论组改名
        *
        *  @param discussionId   讨论组id
        *  @param discussionName 讨论组名称
        *  @param listener       监听器 推送 0 - 成功 非0 - 失败
        */
        [DllImport("rcsdk.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern void RenameDiscussion(string discussionId, string discussionName, PublishAckListenerEventHandler callback);

        /**
        *  设置讨论组开放成员邀请
        *
        *  @param discussionId 讨论组id
        *  @param inviteStatus 邀请状态 0 - 开放邀请 1 - 关闭开放邀请
        *  @param listener     监听器 推送 0 - 成功 非0 - 失败
        */

        [DllImport("rcsdk.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern void SetDiscussionInviteStatus(string discussionId, int inviteStatus, PublishAckListenerEventHandler callback);

        /**
        *  设置讨论组、群组会话状态
        *
        *  @param targetId    讨论组id
        *  @param categoryId  类别
        *  @param groupStatus 状态值 406-不在讨论组中 407-讨论组不存在
        *
        *  @return 返回 成功 失败
        */

        [DllImport("rcsdk.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern bool SetGroupStatus(string targetId, int categoryId, int groupStatus);

        /*
        ************************************************
        *  群组相关函数
        ************************************************
        */

        /**
        *  同步群组，将群组发送到服务器
        *
        *  @param groupIds 群组id数组
        *  @param idCount  数组元素个数
        *  @param listener 监听器 推送 0 - 成功 非0 - 失败
        */

        [DllImport("rcsdk.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern void SyncGroups(RcTargetEntry[] groupIds, int idCount, PublishAckListenerEventHandler callback);

        /**
        *  加入群组，将群组发送到服务器
        *
        *  @param groupIds 群组id数组
        *  @param idCount  数组元素个数
        *  @param listener 监听器 推送 0 - 成功 非0 - 失败
        */

        [DllImport("rcsdk.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern void JoinGroup(RcTargetEntry[] groupIds, int idCount, PublishAckListenerEventHandler callback);

        /**
        *  退出群组，将群组发送到服务器
        *
        *  @param groupIds 群组id数组
        *  @param idCount  数组元素个数
        *  @param listener 监听器 推送 0 - 成功 非0 - 失败
        */

        [DllImport("rcsdk.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern void QuitGroup(RcTargetEntry[] groupIds, int idCount, PublishAckListenerEventHandler callback);

        /**
	    *  加入聊天室
	    *
	    *  @param targetId 聊天室id
	    *  @param categoryId 通道类型
	    *  @param callback 监听器 推送 0 - 成功 非0 - 失败
	    *  @param count 要拉取聊天室的消息条数
	    *  @param flag = false(一般情况下为false) 为true 不清除消息
	    */

        [DllImport("rcsdk.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern void JoinChatRoom(string targetId, int categoryId, int count, PublishAckListenerEventHandler callback,bool flag = false);

        /**
	    *  加入已经存在的聊天室
	    *
	    *  @param targetId 聊天室id
	    *  @param categoryId 通道类型
	    *  @param count 要拉取聊天室的消息条数
	    *  @param callback 监听器 推送 0 - 成功 非0 - 失败
	    * 
	    */
        [DllImport("rcsdk.dll", CallingConvention = CallingConvention.Cdecl)]                       
        public static extern void JoinExistingChatRoom(string targetId, int categoryId, int count, PublishAckListenerEventHandler callback);

        [DllImport("rcsdk.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern void QueryChatroomInfo(string targetId, int count, int order, ChatroomInfoListenerEventHandler callback);

        [DllImport("rcsdk.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern void QuitChatRoom(string targetId, int categoryId, PublishAckListenerEventHandler callback);

        [DllImport("rcsdk.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern bool GetMessageById(int msgId, MessageInfoEventHandler callback);
        /*
        ************************************************
        *  黑名单系列函数
        ************************************************
        */

        /**
        *  加入黑名单
        *
        *  @param userId   用户id
        *  @param listener 监听器 推送 0 - 成功 非0 - 失败
        */
        [DllImport("rcsdk.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern void AddToBlacklist(string userId, PublishAckListenerEventHandler callback);

        /**
        *  移出黑名单
        *
        *  @param userId   用户id
        *  @param listener 监听器 推送 0 - 成功 非0 - 失败
        */
        [DllImport("rcsdk.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern void RemoveFromBlacklist(string userId, PublishAckListenerEventHandler callback);

        /**
        *  获取用户黑名单状态
        *
        *  @param userId   用户id
        *  @param listener 监听器 推送 0 - 成功 非0 - 失败
        */
        [DllImport("rcsdk.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern void GetBlacklistStatus(string userId, BizAckListenerEventHandler callback);

        /**
        *  获取黑名单列表
        *
        *  @param listener 监听器 推送 0 - 黑名单列表信息 非0 - 失败
        */

        [DllImport("rcsdk.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern void GetBlacklist(BlacklistInfoListenerEventHandler callback);

        /**
        *  设置关闭push时间
        *
        *  @param startTime 关闭起始时间 格式 HH:MM:SS
        *  @param spanMins  间隔分钟数 0 < t < 1440
        */

        [DllImport("rcsdk.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern void SetNotificationQuietHours(string startTime, int spanMins, PublishAckListenerEventHandler callback);

        [DllImport("rcsdk.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern void RemoveNotificationQuietHours(PublishAckListenerEventHandler callback);

        [DllImport("rcsdk.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern void GetNotificationQuietHours(PushSettingListenerEventHandler callback);


        /**
        *  接收应用的环境改变事件通知
        *
        *  @param nType     事件类型，101-网络切换，102-应用进入后台，103-应用进入前台，104-锁屏，105-心跳，106-屏幕解锁
        *  @param pbData    依据nType的事件附加数据，待定
        *  @param nDataSize 数据大小，字节数
        *  @param pListener 事件改变的回调
        */

        [DllImport("rcsdk.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern void EnvironmentChangeNotify(int nType, string pbData, int nDataSize);

        /**
        *  设置设备信息
        *
        *  @param manufacturer 设备生产商
        *
        *  @param model 设备型号
        *
        *  @param osVersion 设备系统版本
        *
        *  @param network 设备当前使用的网络
        *
        *  @param networkOperator 网络运营商
        */

        [DllImport("rcsdk.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern void SetDeviceInfo(string manufacturer, string model, string osVersion, string network, string networkOperator);

        [DllImport("rcsdk.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern void SearchPublicServiceProfile(string targetId, int businessType, int searchType, PublishAckListenerEventHandler callback);


        [DllImport("rcsdk.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern void SetUserData(string ext, PublishAckListenerEventHandler callback);

        [DllImport("rcsdk.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern void GetUserData(CreateDiscussionListenerEventHandler callback);

        [DllImport("rcsdk.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern bool GetUserIdByToken(string token, char* data, ref int length);

        /**
        *  设置导航数据监听 
        *
        *  @param listener 监听器
        */
        [DllImport("rcsdk.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern void SetNaviDataListener(NaviDataListenerEventHandler callback);

        [DllImport("rcsdk.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern bool RemoveMemberFromDiscussionSync(string discussionId, string userId);

        [DllImport("rcsdk.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern void UploadUserInfo(string name, string info, bool flag, TokenListenerEventHandler callback);

        [DllImport("rcsdk.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern void RecallMessage(string messageId);

        [DllImport("rcsdk.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern void EncodeWavToAmr(string pchWAVEFilename, string pchAMRFileName, int nChannels, int nBitsPerSample);

        [DllImport("rcsdk.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern void DecodeAmrToWav(string pchAMRFileName, string pchWAVEFilename);

        // 开始录制wav音频文件
        //m_nChannels 1/2 声道
        //m_nSamplesPerSec 8000/11025/44100 采样频率
        //m_wBitsPerSample 8/16  采样大小
        [DllImport("rcsdk.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern int RecordToWav(int m_nChannels, int m_nSamplesPerSec, ushort m_wBitsPerSample);

        //file_name 文件名称 如：test.wav
        [DllImport("rcsdk.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern int SaveToWav(string file_name);
    }
}
