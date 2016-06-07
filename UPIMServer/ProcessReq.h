#pragma once
//////////////////////////////////////////////////////////////////////////
// 集中处理收到的请求数据
void ReqLogFilter(int nProtoco,DWORD dwConnID);

BOOL OnReceive_Ex(DWORD dwConnID, const BYTE* pData, int iLength);

// 用户重登录
int OnReConnectReq(DWORD dwConnID, char* pReceive, ULONG leng);

// 用户校验
int OnCheckPwdReq(DWORD dwConnID, char* pReceive, ULONG leng);

// 分组人员(服务人员分组)
int OnGUserReq(DWORD dwConnID, char* pReceive, ULONG leng);

// 分组人员(服务人员分组)(Json)
int OnGUserReq2(DWORD dwConnID, char* pReceive, ULONG leng);

// 得到分组列表(客户)
int OnGListReq(DWORD dwConnID, char* pReceive, ULONG leng);

// 得到联系人列表
int OnListReq(DWORD dwConnID, char* pReceive, ULONG leng);

// 得到联系人列表(Json)
int OnListReq2(DWORD dwConnID, char* pReceive, ULONG leng);

// 消息发送(客户发送消息到服务器)
int OnSendMSGReq(DWORD dwConnID, char* pReceive, ULONG leng);

// 部门列表
int OnDeptReq(DWORD dwConnID, char* pReceive, ULONG leng);

// 得到固定聊天室
int OnGcInfoReq(DWORD dwConnID, char* pReceive, ULONG leng);

// 得到固定聊天室(Json)
int OnGcInfoReq2(DWORD dwConnID, char* pReceive, ULONG leng);

// 群组聊天,得到联系人列表
int OnGcGetListReq(DWORD dwConnID, char* pReceive, ULONG leng);

// 群组发送消息(客户发送消息到服务器)
int OnGcSendMSGReq(DWORD dwConnID, char* pReceive, ULONG leng);

// 群组聊天,创建
int OnGcCreateReq(DWORD dwConnID, char* pReceive, ULONG leng);

// 群组聊天,添加成员
int OnGcAddReq(DWORD dwConnID, char* pReceive, ULONG leng);

// 群组聊天，发送被添加消息给在线用户
int OnGcAdd_ex(DWORD dwConnID, ROOMID nRoomID, int nRoomType);

// 群组聊天,踢人
int OnGcKickReq(DWORD dwConnID, char* pReceive, ULONG leng);

// 群组聊天，发送被踢出消息给在线用户
int OnGcKick_ex(DWORD dwConnID, ROOMID nRoomID, int nRoomType);

// 群组聊天,离开
int OnGcLeaveReq(DWORD dwConnID, char* pReceive, ULONG leng);

// 获取公众号
int OnGPublic(DWORD dwConnID, char* pReceive, ULONG leng);

// 获取公众号信息
int OnPublicInfo(DWORD dwConnID, char* pReceive, ULONG leng);

// 客户断开公众号连接
int OnPublicConClose(DWORD dwConnID, char* pReceive, ULONG leng);

// 客户发送状态改变信息到他好友列表中的在线用户
int OnOLNotify(DWORD dwConnID, char* pReceive, ULONG leng);

// 客户从服务器上取得离线消息
int OnOFFMsgReq(DWORD dwConnID, char* pReceive, ULONG leng);

// 客户发二进制的包
int OnPkgReq(DWORD dwConnID, char* pReceive, ULONG leng);

// 检测用户状态
int OnSendUserState(DWORD dwConnID, char* pReceive, ULONG leng);

// 投顾获取聊天室
int OnGetAllTgRoom(DWORD dwConnID, char* pReceive, ULONG leng);

// 获取所有房间号对应分析师列表
int OnGetAllTgRoomEx(DWORD dwConnID, char* pReceive, ULONG leng);

// 获取所有房间号对应用户列表
int OnGetAllRoomToUser(DWORD dwConnID, char* pReceive, ULONG leng);

// 查询用户在线状态
int OnGetUserState(DWORD dwConnID, char* pReceive, ULONG leng);

// im2.0版本中服务人员自定义分组
int OnGetSelfGroup(DWORD dwConnID, char* pReceive, ULONG leng);

// im2.0版本中服务人员取得自定义分组下的客户列表
int OnGetSelfGroupUsers(DWORD dwConnID, char* pReceive, ULONG leng);

// im2.0版本中服务人员编辑自定义分组
int OnSetSelfGroup(DWORD dwConnID, char* pReceive, ULONG leng);

// im2.0版本中服务人员编辑自定义分组的客户列表
int OnSetSelfGroupUser(DWORD dwConnID, char* pReceive, ULONG leng);