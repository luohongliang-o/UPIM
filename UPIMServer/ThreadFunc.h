#pragma once

// 推送用户状态
extern DWORD WINAPI _AfxSendUserState(LPVOID lpParam);

// 添加离线消息
extern DWORD WINAPI _AfxAddOffMsg(LPVOID lpParam);

// 添加历史消息
extern DWORD WINAPI _AfxAddHisMsg(LPVOID lpParam);

// 发送外部群组消息
extern DWORD WINAPI _AfxSendMsg(LPVOID lpParam);

// 初始化服务线程
extern DWORD WINAPI _AfxInitSvr(LPVOID lpParam);

// 线程定时器
extern DWORD WINAPI	_AfxOnTimer(LPVOID lParam);

//////////////////////////////////////////////////////////////////////////
// 得到数据库消息(组)
extern BOOL	GetDBGMsg(CADODatabase* pAdoDb);
// 得到数据库消息(个人)
extern BOOL GetDBPMsg(CADODatabase* pAdoDb);
// 得到数据库分组消息分组用户
extern BOOL GetDBGMsgUser(CADODatabase* pAdoDb, LPCTSTR pszGroupId, __int64 nSeqId, LPMSGGUSER pMsgGUser, long& nSize);
// 群组信息
extern DWORD WINAPI _AfxUpdateGroupRoom(LPVOID lParam);
// 发送数据库消息
extern DWORD WINAPI _AfxSendDBMsg(LPVOID lpParam);
// 写日志
extern DWORD WINAPI _AfxWriteLog(LPVOID lpParam);