// 下列 ifdef 块是创建使从 DLL 导出更简单的
// 宏的标准方法。此 DLL 中的所有文件都是用命令行上定义的 WIN_RCSDK_EXPORTS
// 符号编译的。在使用此 DLL 的
// 任何其他项目上不应定义此符号。这样，源文件中包含此文件的任何其他项目都会将
// WIN_RCSDK_API 函数视为是从 DLL 导入的，而此 DLL 则将用此宏定义的
// 符号视为是被导出的。


#ifdef WIN_RCSDK_EXPORTS
#define WIN_RCSDK_API __declspec(dllexport)
#else
#define WIN_RCSDK_API __declspec(dllimport)
#endif
#include <stdint.h>
#include <string>

using namespace std;

namespace RongCloud
{
	struct RcTargetEntry
	{
		char targetId[64];		//交互方id(接收方id)
		char targetName[256];	//交互方的userid
	};

}
//定义全局callback
/*
jobj
{
"result":"falied/success",
"err_code":0/1,					//错误码   0-成功 非0-失败
"userId":"4378"					//描述信息 成功，返回用户id，失败，返回错误描述
}
*/
typedef void(__stdcall *ConnectAckListenerCallback)(const wchar_t* json_str);
ConnectAckListenerCallback connect_ack_callback;

/*
jobj
{
"result":"falied/success",
"err_code":0/1,
"blockUsers":"1122"
}
*/
typedef void(__stdcall *BlacklistInfoListenerCallback)(const wchar_t* json_str);
BlacklistInfoListenerCallback black_list_callback;

/*
jobj
{
"result":"falied/success",
"err_code":0/1,	//操作结果 0-成功 非0-失败
"msgUId":"",
"timestamp":12345
}
*/
typedef void(__stdcall *PublishAckListenerCallback)(const wchar_t* json_str);
PublishAckListenerCallback 
send_message_callback,
quit_discussion_callback,
invite_member_to_discussion_callback,
remove_from_discussion_callback,
rename_discussion_callback,
set_invite_status_callback,
join_chat_room_callback,
join_exist_chat_room_callback,
quit_chat_room_callback,
add_to_blacklist_callback,
remove_from_black_list_callback,
remove_push_setting_callback,
set_user_data_callback,
subscribe_account_callback,
sync_group_callback,
join_group_callback,
add_push_setting_callback,
quit_group_callback,
search_account_callback;
int msg_id = -1;
/*
jobj
{
"data":"hello",
}
*/

typedef void(__stdcall *ChatroomInfoListenerCallback)(const wchar_t* json_str);
ChatroomInfoListenerCallback query_chat_room_callback;

typedef void(__stdcall *NaviDataListenerCallback)(const wchar_t* json_str);
NaviDataListenerCallback set_nav_data_listener_callback;

/*
jobj
{
"result": "falid/success",
"err_code": 0/1,				//错误码
"startTime": 1122				//开始时间
"spanMins" : 11221				//时间间隔
}
*/
typedef void(__stdcall *PushSettingListenerCallback)(const wchar_t* json_str);
PushSettingListenerCallback query_push_setting_callback;

/*
jobj
{
"result":"falid/success",
"userId":"7788",					//用户id
"err_code":0/1,						//错误码 0-成功 1-失败
"userName":"1122"					//用户名称
"userPortrait" : "9988"				//用户头像
}
*/
typedef void(__stdcall *UserInfoListenerCallback)(const wchar_t* json_str);
UserInfoListenerCallback get_userinfo_callback;

/*
jobj
{
"result":"falid/success",
"discussionId":"7788",			//讨论组id
"err_code":0/1					//错误码
}
*/
typedef void(__stdcall *CreateDiscussionListenerCallback)(const wchar_t* json_str);
CreateDiscussionListenerCallback 
create_invite_discussion_callback,
get_user_data_callback;

/*
jobj
{
"opStatus":0,		//操作结果    0-成功 非0-失败
"bizStatus":0		//业务数据值  根据具体业务返回响应的业务数据值
}
*/
typedef void(__stdcall *BizAckListenerCallback)(const wchar_t* json_str);
BizAckListenerCallback 
set_block_callback,
get_block_callback,
get_black_list_callback;

/*
jobj
{
"process": 7  //进度值
}
jobj
{
"result":"falid/success",
"url":"http//....",             //信息描述 成功，返回图片uri 失败 返回错误描述
"err_code":0/1					//错误码 0 - 成功 非0 - 失败
}
*/
typedef void(__stdcall *ImageListenerCallback)(const wchar_t* json_str);
ImageListenerCallback 
send_file_with_url_callback,
send_file_with_url_process_callback,
down_file_with_url_callback,
down_file_with_url_process_callback;

/*
jobj
{
"ntype":1,
"data":"7788",
}
*/
typedef void(__stdcall *EnvironmentChangeNotifyListenerCallback)(const wchar_t* json_str);
EnvironmentChangeNotifyListenerCallback enviroment_change_callback;

/*
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
"m_MessageId":9,            //消息id   消息在数据库中的id号
"m_Direction":0,            //消息方向  0-发送的消息 1-接收的消息
"m_ReadStatus":0,           //读取状态  0-未读 1-已读
"m_SendStatus":20,          //发送状态  10-发送中 20-发送失败 30-发送成功
"m_SendTime":9,             //接收时间
"m_RcvTime":9,              //接收时间
"m_MessageType":0		    //信息类别  0-实时接收信息 1-历史漫游
}
*/
typedef void(__stdcall *MessageInfoCallback)(const wchar_t* json_str);

/*
jobj
{
ConversationInfo
{
"m_TargetId":"99",          //会话id
"m_Title": "" ,             //回话标题
"m_Draft":"",               //会话草稿
"m_ConversationType":9,     //会话类型 单聊 群聊 讨论组 。。。
"m_IsTop":9,                //会话是否置顶
"m_BlockPush":9,            //是否要push消息
"m_GroupStatus":9,          //讨论组状态
"m_UnreadCount":9,          //未读消息数
"m_LastTime":9,             //最近会话时间
}
CMessageInfo
{
"m_SenderId": "" ,          //发方id
"m_ClazzName":"",		    //类别
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
UserInfo
{
"m_Id":"99",                //用户id
"m_Name": "" ,              //用户名
"m_Portrait":"",            //用户url
"m_AccountExtra":"",        //用户account
"m_categoryId":9,           //类别id
"m_BlockPush":9,            //新消息通知阻止状态 0 - 未阻止 100 - 阻止
}
*/
typedef void(__stdcall *ConversationInfoCallback)(const wchar_t* json_str);

/*
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
typedef void(__stdcall *AccountInfoCallback)(const wchar_t* json_str);

/*
jobj
{
"m_TargetId":"99",          //交互方id
"m_SenderId": "" ,          //发方id
"m_ClazzName":"",		    //类别
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
*/
typedef void(__stdcall *MessageListenerCallback)(const wchar_t* json_str);
MessageListenerCallback 
message_listener_callback,
load_history_message_callback;

/*
jobj
{
"result":"falid/success",
"count":0					//数量
"data":jobj,				//CsharpDiscussionInfo 请参见下面讨论组数据结果
"err_code":0/1				//错误码
}
*/
typedef void(__stdcall *DiscussionInfoListenerCallback)(const wchar_t* json_str);
DiscussionInfoListenerCallback discussion_info_callback;

/*
jobj
{
"m_Id":"99",				//讨论组或群组id
"m_Name": "" ,				//讨论组名称
"m_AdminId":"",				//管理员id
"m_UserIds":"",				//群组成员id,使用\n分割
"m_ConversationType":1,		//通道类型 1-PRIVATE 2-DISCUSSION 3-GROUP 4-CHATROOM 5-CUSTOMSERVICE
"m_InviteStatus":0,			//开放邀请状态   0 - 开放邀请 1 - 关闭开放邀请
"m_BlockPush":0				//新消息通知阻止状态 0 - 未阻止 100 - 阻止
}
*/
typedef void(__stdcall *DiscussionInfoSyncCallback)(const wchar_t* json_str);

/*
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
typedef void(__stdcall *UserInfoCallback)(const wchar_t* json_str);

/*
jobj
{
"nstatus":1,
"data":"reason"
}
*/
typedef void(__stdcall *ExceptionListenerCallback)(const wchar_t* json_str);
ExceptionListenerCallback exception_listener_callback;

/*
jobj
{
"length":1,
"data":"hello"
}
*/
typedef void(__stdcall *DataBufferCallback)(const wchar_t* json_str);

/*
jobj
{
"result":"success/failed"
"err_code":1,
"data":"fdsgsdgs"
}
*/
typedef void(__stdcall *TokdenListenerCallback)(const wchar_t* json_str);
TokdenListenerCallback token_listener_callback;

using namespace RongCloud;
extern "C"
{
	/**
	*  初始化全局实例
	*
	*  @param appid        应用id(appkey)
	*  @param appName      应用名称
	*  @param deviceId     设备id
	*  @param localPath    缓存路径
	*  @param databasePath 数据库存储路径
	*
	*  @return client指针   NULL - 失败 非NULL - 成功
	*/

	WIN_RCSDK_API void* InitClient(const char *appId, const char* appName, const char* deviceId, const wchar_t* localPath, const wchar_t* databasePath);

	/**
	*  建立服务器连接
	*
	*  @param token    登录token，由后台配置从网站得到
	*  @param callback 监听连接结果通过监听中的函数通知
	*/

	WIN_RCSDK_API void Connect(const char* token, ConnectAckListenerCallback callback,bool ipv6 = false);

	/**
	*  断开服务器连接，不会删除全局实例
	*
	*  @param state 断开时指定的状态 
	*  2 断开连接收推送 
	*  4 断开连接不收推送
	*/
	WIN_RCSDK_API void Disconnect(int state);

	/**
	*  iOS设置DeviceToken
	*
	*  @param deviceId DeviceToken
	*/
	WIN_RCSDK_API void SetDeviceId(const char* deviceId);

	/**
	*  设置消息监听
	*
	*  @param callback 消息监听器
	*/
	WIN_RCSDK_API void SetMessageListener(MessageListenerCallback callback);

	/**
	*  设置异常监听
	*
	*  @param callback 异常监听器
	*/
	WIN_RCSDK_API void SetExceptionListener(ExceptionListenerCallback callback);

	/**
	*  注册信息类别
	*
	*  @param clazzName   信息类别
	*  @param operateBits 操作位 1(00000001) - 是否入库 2(00000010) - 是否标记为已读
	*
	*  @return 操作结果 true - 成功 false - 失败
	*/
	WIN_RCSDK_API bool RegisterMessageType(const char* clazzName, const unsigned int operateBits);

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
	*  @param targetId     交互方id(接收方id)
	*  @param categoryId   通道类型
	1 - PRIVATE             单人
	2 - DISCUSSION          讨论组
	3 - GROUP               群组
	4 - CHATROOM            聊天室
	5 - CUSTOMERSERVICE     客服
	6 - SYSTEM              系统消息
	7 - MC                  MC消息应用公众服务
	8 - MP                  MP消息公众服务平台
	*  @param transferType 传输类别
	1 - S 消息数据服务器不缓存，不保证信息到达接收方
	2 - N 消息数据服务器缓存，交互方会收到通知消息
	3 - P 消息数据服务器缓存，交互方不在线会收到PUSH消息
	*  @param clazzname    消息类别(RC:ImgMsg RC:VcMsg RC:NtfMsg RC:StMsg)
	*  @param message      消息内容 json内容
	*  @param messageId    消息id 消息在数据库中的Id
	*  @param callback     发送监听
	*/

	WIN_RCSDK_API void sendMessage(const char* targetId, int categoryId, int transferType, const char* clazzname, const wchar_t* message, const char* push, const char* appData, int messageId, PublishAckListenerCallback callback);
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

	WIN_RCSDK_API long SaveMessage(const char* targetId, int categoryId, const char* clazzName, const char* senderId, const wchar_t* message, const char* push, const char* appData, bool driection = false, int readStatus = 0);

	/**
	*  设置草稿
	*
	*  @param targetId     交互方id
	*  @param categoryId   通道类型
	*  @param draftMessage 草稿内容 NULL - 清除草稿 非NULL - 设置草稿
	*
	*  @return 操作结果 true - 成功 false - 失败
	*/
	WIN_RCSDK_API bool SaveTextMessageDraft(const char* targetId, int categoryId, const char* draftMessage);

	/**
	*  获取草稿
	*
	*  @param targetId     交互方id
	*  @param categoryId   通道类型
	*  @param callback	   草稿内容回调函数
	*
	*  @return 操作结果 true - 成功 false - 失败
	*/
	WIN_RCSDK_API bool GetTextMessageDraft(const char* targetId, int categoryId, DataBufferCallback callback);
	
	/**
	*  设置会话置顶
	*
	*  @param targetId   交互方id
	*  @param categoryId 通道类型
	*  @param bTop       是否置顶 true - 置顶 false - 取消置顶
	*
	*  @return 操作结果 true - 成功 false - 失败
	*/
	WIN_RCSDK_API bool SetConversationToTop(const char* targetId, int categoryId, bool bTop);

	/**
	*  删除消息
	*
	*  @param targetId   交互方id
	*  @param categoryId 通道类型
	*
	*  @return 操作结果 true - 成功 false - 失败
	*/
	WIN_RCSDK_API bool ClearMessages(const char* targetId, int categoryId);

	/**
	*  删除某个会话
	*
	*  @param targetId   交互方id
	*  @param categoryId 通道类型
	*
	*  @return 操作结果 true - 成功 false - 失败
	*/
	WIN_RCSDK_API bool RemoveConversation(const char* targetId, int categoryId);

	/**
	*  清除会话消息的未读状态
	*
	*  @param targetId   交互方id
	*  @param categoryId 通道类型
	*
	*  @return 操作结果 true - 成功 false - 失败
	*/
	WIN_RCSDK_API bool ClearMessagesUnreadStatus(const char* targetId, int categoryId);

	/**
	* 根据会话类型的目标 Id，回调方式获取某消息类型标识的N条历史消息记录。
	*
	* @param targetId			目标 Id。根据不同的  categoryId，可能是用户 Id、讨论组 Id、群组 Id 或聊天室 Id。
	* @param categoryId			会话类型。不支持传入 categoryId.CHATROOM。
	* @param recordTime         从该时间点开始获取消息。即：消息中的 sentTime；第一次可传 0，获取最新 count 条。
	* @param rowCount			要获取的消息数量，最多 20 条。
	* @param callback			获取历史消息记录的回调，按照时间顺序从新到旧排列。
	*/

	WIN_RCSDK_API bool GetRemoteHistoryMessage(const char* targetId, int categoryId, int64_t recordTime, int rowCount, MessageListenerCallback callback);

	/**
	* 订阅公共服务账号
	*
	* @param targetId			目标 Id。根据不同的  categoryId，可能是用户 Id、讨论组 Id、群组 Id 或聊天室 Id。
	* @param categoryId			会话类型。不支持传入 categoryId.CHATROOM。
	* @param subscribe			true-订阅 false-解订阅
	* @param callback			监听是否成功
	*/
	WIN_RCSDK_API void SubscribePublicService(const char* targetId, int categoryId, bool subscribe, PublishAckListenerCallback callback);

	/**
	*  @param targetId   交互方id
	*  @param categoryId 通道类型
	*  @param clazzname    消息类别(RC:ImgMsg RC:VcMsg RC:NtfMsg RC:StMsg)
	*  @param lastId     消息id
	*  @param count      选取消息条数
	*  @param callback     json结构的消息列表
	*
	*  @return 操作结果 true - 成功 false - 失败
	*/
	WIN_RCSDK_API bool GetHistoryMessages(const char* targetId, int categoryId, const char* clazzName, long lastId, int count, MessageInfoCallback callback);
	
	/**
	*  获取最近的会话列表
	*
	*  @param conversationDict	通道类型
	*  @param conversationCount 数目
	*  @param callback			json结构的会话列表
	*
	*  @return 操作结果 true - 成功 false - 失败
	*/
	WIN_RCSDK_API bool GetConversationList(int conversationDict[], int conversationCount, ConversationInfoCallback callback);

	/**
	*  获取最近的某类信息的会话列表
	*
	*  @param targetId   交互方id
	*  @param categoryId 通道类型
	*  @param callback     json结构的会话列表
	*
	*  @return 操作结果 true - 成功 false - 失败
	*/
	WIN_RCSDK_API bool GetConversation(const char* targetId, int categoryId, ConversationInfoCallback callback);

	/**
	*  获取未读消息数
	*
	*  @return >=0 未读信息条数-客服未读数 -1 数据库未初始化
	*/
	WIN_RCSDK_API int GetTotalUnreadCount();
	/**
	*  获取未读信息数
	*
	*  @param targetId   交互方id
	*  @param categoryId 通道类型
	*
	*  @return >=0 未读消息数目 -1 数据库未初始化
	*/
	WIN_RCSDK_API int GetUnreadCount(const char* targetId, int categoryId);

	/**
	*  check message count of conversation
	*
	*  @param targetId     target id
	*  @param categoryId   channel type
	*
	*  @return the message count of conversation
	*/
	WIN_RCSDK_API int GetMessageCount(const char* targetId, int categoryId);

	/**
	*  获取未读信息数
	*
	*  @param conversationDict  通道类型数组
	*  @param conversationCount 数组元素个数
	*
	*  @return >=0 未读消息数目 -1 数据库未初始化
	*/
	WIN_RCSDK_API int GetConversationUnreadCount(int conversationDict[], int conversationCount);

	/**
	*  设置阻止push信息
	*
	*  @param targetId   交互方id
	*  @param categoryId 通道类型
	*  @param block      是否阻止 true - 阻止 false - 取消阻止
	*  @param callback   监听器 推送值：0-未阻止 100-已阻止
	*/
	WIN_RCSDK_API void SetConversationNotificationStatus(const char* targetId, int categoryId, bool block, BizAckListenerCallback callback);

	/**
	*  获取阻止状态
	*
	*  @param targetId    交互方id
	*  @param categoryId  通道类型
	*  @param fetchRemote 是否从服务器获取 true - 服务器 false - 本地
	*  @param callback    监听器 推送值：0-未阻止 100-已阻止
	*/
	WIN_RCSDK_API void GetConversationNotificationStatus(const char* targetId, int categoryId, BizAckListenerCallback callback);


	/**
	*  获取用户信息 (请求服务器来获取用户信息)
	*
	*  @param userId      用户id
	*  @param callback    监听器 推送：用户id 用户名称 用户url
	*/
	WIN_RCSDK_API void GetUserInfo(const char* userId, UserInfoListenerCallback callback);

	/**
	*  获取用户信息 (本地数据库来获取用户信息)
	*/
	WIN_RCSDK_API bool GetUserInfoExSync(const char* userId, int category_id, UserInfoCallback callback);

	//清空所有会话及会话消息，回调方式通知是否清空成功
	WIN_RCSDK_API bool ClearConversations(int conversationDict[], int conversationCount);

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
	WIN_RCSDK_API bool DeleteMessage(long messageId[], int messageCount);

	/**
	*  设置消息内容
	*
	*  @param messageId 消息id
	*  @param content   消息内容
	*
	*  @return 操作结果：true - 成功 false - 失败
	*/
	WIN_RCSDK_API bool SetMessageContent(long messageId, const char* content);

	/**
	*  设置附加信息
	*
	*  @param messageId    消息id
	*  @param extraMessage 附加信息内容
	*
	*  @return 操作结果：true - 成功 false - 失败
	*/
	WIN_RCSDK_API bool SetMessageExtra(long messageId, const char* extraMessage);

	/**
	*  设置消息读取状态
	*
	*  @param messageId  消息id
	*  @param readStatus 消息读取状态 0 - 未读 1 - 已读
	*
	*  @return 操作结果：true - 成功 false - 失败
	*/
	WIN_RCSDK_API bool SetMessageReadStatus(long messageId, int readStatus);

	/**
	*  设置消息发送状态
	*
	*  @param messageId  消息id
	*  @param readStatus 消息读取状态 10 - 发送中 20 - 发送失败 30 - 发送成功
	*
	*  @return 操作结果：true - 成功 false - 失败
	*/
	WIN_RCSDK_API bool SetMessageSendStatus(long messageId, int sendStatus);

	/**
	*  获取本地时间与服务器时间的差值
	*
	*  @return 差值 = 本地时间 - 服务器时间
	*/
	WIN_RCSDK_API int64_t GetDeltaTime();


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
	*  @param callback  图片监听器 推送：process_callback 进度 操作结果：成功 callback推送网络地址 失败 失败原因
	*/
	WIN_RCSDK_API void UpLoadFile(const char* targetId, int categoryId, int nType, const unsigned char* pbData, long nl, ImageListenerCallback callback,ImageListenerCallback process_callback);

	/**
	*  下载文件
	*
	*  @param targetId   交互方id
	*  @param categoryId 通道类型
	*  @param nType      媒体类型 1 - Image 2 - Audio 3 - Video
	*  @param pszUrl     媒体网络地址
	*  @param callback  图片监听器 推送：进度 操作结果：成功 推送网络地址 失败 失败原因
	*/
	WIN_RCSDK_API void DownLoadFile(const char* targetId, int categoryId, int nType, const char* pszUrl, ImageListenerCallback callback, ImageListenerCallback process_callback);

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
	*  @param callback       监听器 推送 成功 - 讨论组id 失败 - 出错原因
	*/
	WIN_RCSDK_API void CreateDiscussion(const char*  discussionName, RcTargetEntry userIds[], int idCount, CreateDiscussionListenerCallback callback);

	/**
	*  退出讨论组
	*
	*  @param discussionId  讨论组id
	*  @param callback      监听器 推送 0 - 成功 非0 - 失败
	*/
	WIN_RCSDK_API void QuitDiscussion(const char* discussionId, PublishAckListenerCallback callback);

	/**
	*  邀请加入讨论组
	*
	*  @param discussionId 讨论组id
	*  @param userIds      用户id数组
	*  @param idCount      数组原始个数
	*  @param callback     监听器 推送 0 - 成功 非0 - 失败
	*/
	WIN_RCSDK_API void AddMemberToDiscussion(const char* discussionId, RcTargetEntry userIds[], int idCount, PublishAckListenerCallback callback);

	/**
	*  将某人踢出讨论组
	*
	*  @param discussionId 讨论组id
	*  @param userId       踢出的用户id
	*  @param callback     监听器 推送 0 - 成功 非0 - 失败
	*/

	WIN_RCSDK_API void RemoveMemberFromDiscussion(const char* discussionId, const char* userId, PublishAckListenerCallback callback);

	/**
	*  获取讨论组信息 从服务器获取
	*
	*  @param discussionId 讨论组id
	*  @param categoryId   通道类型 只能是 discussion ，此参数稍后将被去掉
	*  @param callback     监听器 推送 0 - 成功，返回讨论组信息 非0 - 失败
	*/
	WIN_RCSDK_API void GetDiscussionInfo(const char* discussionId, int categoryId, DiscussionInfoListenerCallback callback);

	//从本地数据库获取
	WIN_RCSDK_API bool GetDiscussionInfoSync(const char* discussionId, int categoryId, DiscussionInfoSyncCallback callback);

	/**
	*  讨论组改名
	*
	*  @param discussionId   讨论组id
	*  @param discussionName 讨论组名称
	*  @param callback       监听器 推送 0 - 成功 非0 - 失败
	*/

	WIN_RCSDK_API void RenameDiscussion(const char* discussionId, const char* discussionName, PublishAckListenerCallback callback);

	/**
	*  update conversation title
	*
	*  @param discussionId   discussion id
	*  @param discussionName discussion name
	*/
	WIN_RCSDK_API bool UpdateConversationTitle(const char* discussionId, const char* discussionName);

	/**
	*  update conversation title
	*
	*  @param targetId   the targetId of conversation
	*  @param categoryId channel type
	*  @param title      the title of conversation
	*  @param portraitUrl the url of portrait
	*/
	WIN_RCSDK_API bool UpdateConversationInfo(const char* targetId, int categoryId, const char* title, const char* portraitUrl);

	/**
	*  设置讨论组开放成员邀请
	*
	*  @param discussionId 讨论组id
	*  @param inviteStatus 邀请状态 0 - 开放邀请 1 - 关闭开放邀请
	*  @param callback     监听器 推送 0 - 成功 非0 - 失败
	*/
	WIN_RCSDK_API  void SetDiscussionInviteStatus(const char* discussionId, int inviteStatus, PublishAckListenerCallback callback);

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
	*  @param callback 监听器 推送 0 - 成功 非0 - 失败
	*/
	WIN_RCSDK_API void SyncGroups(RcTargetEntry groupIds[], int idCount, PublishAckListenerCallback callback);

	/**
	*  加入群组，将群组发送到服务器
	*
	*  @param groupIds 群组id数组
	*  @param idCount  数组元素个数
	*  @param callback 监听器 推送 0 - 成功 非0 - 失败
	*/
	WIN_RCSDK_API void JoinGroup(RcTargetEntry groupIds[], int idCount, PublishAckListenerCallback callback);

	/**
	*  退出群组，将群组发送到服务器
	*
	*  @param groupIds 群组id数组
	*  @param idCount  数组元素个数
	*  @param callback 监听器 推送 0 - 成功 非0 - 失败
	*/
	WIN_RCSDK_API void QuitGroup(RcTargetEntry groupIds[], int idCount, PublishAckListenerCallback callback);

	/**
	*  加入聊天室
	*
	*  @param targetId 聊天室id
	*  @param categoryId 通道类型
	*  @param callback 监听器 推送 0 - 成功 非0 - 失败
	*  @param count 要拉取聊天室的消息条数
	*  @param flag = false(一般情况下为false) 为true 不清除消息
	*/
	//加入聊天室
	WIN_RCSDK_API void JoinChatRoom(const char* targetId, int categoryId, int count, PublishAckListenerCallback callback, bool flag = false);

	/**
	*  加入已经存在的聊天室
	*
	*  @param targetId 聊天室id
	*  @param categoryId 通道类型
	*  @param count 要拉取聊天室的消息条数
	*  @param callback 监听器 推送 0 - 成功 非0 - 失败
	* 
	*/
	WIN_RCSDK_API void JoinExistingChatRoom(const char* targetId, int categoryId, int count, PublishAckListenerCallback callback);

	//退出聊天室

	WIN_RCSDK_API void QuitChatRoom(const char* targetId, int categoryId, PublishAckListenerCallback callback);

	/**
	*  查询聊天室成员信息
	*
	*  @param targetId 聊天室id
	*  @param count 拉取count成员的信息
	*  @param order - 1 升序返回最早加入的 - 2 降序返回最早加入聊天室的成员
	*  @param callback 返回聊天室的成员信息信息
	*
	*/
	WIN_RCSDK_API void QueryChatroomInfo(const char* targetId, int count, int order, ChatroomInfoListenerCallback callback);
	/*
	************************************************
	*  黑名单系列函数
	************************************************
	*/

	/**
	*  加入黑名单
	*
	*  @param userId   用户id
	*  @param callback 监听器 推送 0 - 成功 非0 - 失败
	*/

	WIN_RCSDK_API void AddToBlacklist(const char* userId, PublishAckListenerCallback callback);

	/**
	*  移出黑名单
	*
	*  @param userId   用户id
	*  @param callback 监听器 推送 0 - 成功 非0 - 失败
	*/

	WIN_RCSDK_API void RemoveFromBlacklist(const char* userId, PublishAckListenerCallback callback);

	/**
	*  获取用户黑名单状态
	*
	*  @param userId   用户id
	*  @param callback 监听器 推送 0 - 成功 非0 - 失败
	*/
	WIN_RCSDK_API void GetBlacklistStatus(const char* userId, BizAckListenerCallback callback);

	/**
	*  获取黑名单列表
	*
	*  @param callback 监听器 推送 0 - 黑名单列表信息 非0 - 失败
	*/

	WIN_RCSDK_API void GetBlacklist(BlacklistInfoListenerCallback callback);

	/**
	*  设置关闭push时间
	*
	*  @param startTime 关闭起始时间 格式 HH:MM:SS
	*  @param spanMins  间隔分钟数 0 < t < 1440
	*/

	WIN_RCSDK_API void SetNotificationQuietHours(const char* startTime, int spanMins, PublishAckListenerCallback callback);

	//通知服务器移除push设置
	WIN_RCSDK_API void RemoveNotificationQuietHours(PublishAckListenerCallback callback);

	//从服务器查询push设置
	WIN_RCSDK_API void GetNotificationQuietHours(PushSettingListenerCallback callback);

	/**
	*  接收应用的环境改变事件通知
	*
	*  @param nType     事件类型，101-网络切换，102-应用进入后台，103-应用进入前台，104-锁屏，105-心跳，106-屏幕解锁
	*  @param pbData    依据nType的事件附加数据，待定
	*  @param nDataSize 数据大小，字节数
	*  @param callback 事件改变的回调
	*/
	WIN_RCSDK_API  void EnvironmentChangeNotify(int nType, const char* pbData, int nDataSize);

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
	WIN_RCSDK_API void SetDeviceInfo(const char* manufacturer, const char* model, const char* osVersion, const char* network, const char* networkOperator);

	//查询公共账号信息
	WIN_RCSDK_API void SearchPublicServiceProfile(const char* targetId, int businessType, int searchType, PublishAckListenerCallback callback);

	//设置用户数据 发送到服务起
	WIN_RCSDK_API void SetUserData(const char* ext, PublishAckListenerCallback callback);

	//从服务起获取用户数据
	WIN_RCSDK_API void GetUserData(CreateDiscussionListenerCallback callback);

	//获取用户的id通过token
	WIN_RCSDK_API bool GetUserIdByToken(const char* token, char* data, int& length);

	/**
	*  设置导航数据监听(私有云 wps)
	*
	*  @param listener 监听器
	*/

	WIN_RCSDK_API void SetNaviDataListener(NaviDataListenerCallback callback);

	/**
	*  (本地数据库)从讨论组移除某人
	*
	*  @param discussionId  讨论组id
	*  @param userId        用户id
	*/
	WIN_RCSDK_API bool RemoveMemberFromDiscussionSync(const char* discussionId, const char* userId);

	/**
	*  用户信息Key值，用户自定义信息
	*
	*  @param name       名称
	*  @param info       信息内容
	*  @param callback   处理方法
	*/
	WIN_RCSDK_API void UploadUserInfo(const char* name, const char* info, bool flag, TokdenListenerCallback callback);
	/**
	*  通过消息id获取整条消息
	*
	*  @param msgId       消息id
	*/
	WIN_RCSDK_API bool GetMessageById(long msgId, MessageInfoCallback callback);
	/**
	*  删除制定id用户消息
	*
	*  @param messageUId  消息id
	*/
	WIN_RCSDK_API bool RecallMessage(const char* messageUId);

	// WAVE音频采样频率是8khz 
	// 音频样本单元数 = 8000*0.02 = 160 (由采样频率决定)
	// 声道数 1 : 160
	//        2 : 160*2 = 320
	// bps决定样本(sample)大小
	// bps = 8 --> 8位 unsigned char
	//       16 --> 16位 unsigned short
	WIN_RCSDK_API int EncodeWavToAmr(const char* pchWAVEFilename, const char* pchAMRFileName, int nChannels, int nBitsPerSample);

	// 将AMR文件解码成WAVE文件
	WIN_RCSDK_API int DecodeAmrToWav(const char* pchAMRFileName, const char* pchWAVEFilename);

	// 开始录制wav音频文件
	//m_nChannels 1/2 声道
	//m_nSamplesPerSec 8000/11025/44100 采样频率
	//m_wBitsPerSample 8/16  采样大小
	WIN_RCSDK_API bool RecordToWav(int m_nChannels,int m_nSamplesPerSec,WORD m_wBitsPerSample);

	// 保存wav录音文件 file_name: test.wav
	WIN_RCSDK_API bool SaveToWav(char* file_name);

	//"SELECT COUNT(*) FROM RCT_MESSAGE WHERE target_id='77552' AND category_id=1";
	WIN_RCSDK_API int ExecSql(const char* sql);
}