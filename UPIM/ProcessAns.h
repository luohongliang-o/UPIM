/*
* 此处对于服务器返回的数据进行集中处理
*/
#pragma once
//用户登录成功
void OnUserLoginSuc();

//获取分类用户
int  GetGroupUser();

BOOL OnFuncGList_Qs(int grouptype);

//////////////////////////////////////////////////////////////////////////
// 取得服务器推送来的数据，集中处理入口
int ReciveDataFromSVR(const BYTE* pData, int iLength);

// 用户校验返回
int OnCheckPwdAns(char* pReceive, ULONG leng);

// 分组人员(服务人员分组)返回
int OnGUserAns(char* pReceive, ULONG leng);

// 分组人员(服务人员分组)返回（JSON）
int OnGUserAns2(char* pReceive, ULONG leng);

// 得到分组列表(客户)返回
int OnGListAns(char* pReceive, ULONG leng);

// 客户得到联系人列表
int OnListAns(char* pReceive, ULONG leng);

// 客户得到联系人列表（JSON）
int OnListAns2(char* pReceive, ULONG leng);

// 消息发送(客户发送消息到服务器)返回
int On_SendMSG_Ans(char* pReceive, ULONG leng);

// 消息接收(服务器推送消息)
int On_S_SendMSG_Ans(char* pReceive, ULONG leng);

// 部门列表
int OnDeptAns(char* pReceive, ULONG leng);

// 得到固定聊天室
int OnGcInfoAns(char* pReceive, ULONG leng);

// 得到固定聊天室（JSON）
int OnGcInfoAns2(char* pReceive, ULONG leng);

// 群组聊天,得到联系人列表
int OnGcGetListAns(char* pReceive, ULONG leng);

// 群组发送消息(客户发送消息到服务器)返回
int OnSendGcMSGAns(char* pReceive, ULONG leng);

// 群组发送接收(服务器推送消息)
int OnRecvGcMSG(char* pReceive, ULONG leng);

// 群组聊天,创建
int OnGcCreateAns(char* pReceive, ULONG leng);

// 群组聊天,添加成员
int OnGcAddAns(char* pReceive, ULONG leng);

// 被添加进群组
int OnGetS_GCAddAns(char* pReceive, ULONG leng);

// 群组聊天,踢人
int OnGcKickAns(char* pReceive, ULONG leng);

// 被踢出群组
int OnGetS_GCKickAns(char* pReceive, ULONG leng);

// 群组聊天,离开
int OnGcLeaveAns(char* pReceive, ULONG leng);

// 获取公众号
int OnGetPublicAns(char* pReceive, ULONG leng);

// 获取公众号信息
int OnGetPublicInfoAns(char* pReceive, ULONG leng);

// 服务器推送来的客服上线消息
int OnGet_S_Online(char* pReceive, ULONG leng);

// 离线消息
int OnGetOffMSg(char* pReceive, ULONG leng);

// 异地登录的踢人消息
int OnKickOffMsg(char* pReceive, ULONG leng);

// 服务器关闭消息，避免服务器已经关闭，客户端还傻傻的在线
int OnSvrOffMsg(char* pReceive, ULONG leng);

////图片传送
int On_SendPKG_Ans(char* pReceive, ULONG leng);

// 服务器中转数据包
int On_S_SendPKG_Ans(char* pReceive, ULONG leng);

// 服务器检测客户端状态
int On_SendMsgCheck_Ans(char* pReceive, ULONG leng);

// 服务器取得聊天室信息
int On_GetAllTgRoom_Ans(char* pReceive, ULONG leng);

// 取得在线信息
int OnGetUserState_Ans(char* pReceive, ULONG leng);

// 重登录协议的返回包
int OnReconnect_Ans(char* pReceive, ULONG leng);

// im2.0取得所有聊天室信息的返回包
int On_GetAllTgRoomEx_Ans(char* pReceive, ULONG leng);

// im2.0取得房间下所有客户信息的返回包
int On_GetUserByRoomID_Ans(char* pReceive, ULONG leng);

// im2.0版本中服务人员自定义分组
int OnGetSelfGroup_Ans(char* pReceive, ULONG leng);

// im2.0取得分组下对应的用户列表
int OnGetSelfGroupUsers_Ans(char* pReceive, ULONG leng);

// im2.0添加/修改自定义分组信息
int OnSetSelfGroup_Ans(char* pReceive, ULONG leng);

// im2.0添加/修改用户的分组
int OnSetSelfGroupUser_Ans(char* pReceive, ULONG leng);