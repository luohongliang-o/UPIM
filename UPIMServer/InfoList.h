/*
* 消息队列
* Opander
* 2014年6月23日14:35:14
*/

#if !defined(_INFOLIST_H_)
#define _INFOLIST_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CInfoList  
{
public:
	CInfoList();
	virtual ~CInfoList();
public:
	// 用户状态
	BOOL PushUSER(USERSTATE* pUserState);
	BOOL PushUSER(LPCSTR pszID, BYTE ucState, LPCSTR pszToUser=_T(""));
	BOOL PopUSER(USERSTATE& mUserState);
public:
	// 用户状态
	long m_nHeaderUPID, m_nTailUPID;
	USERSTATE m_aUserState[MAX_STATE];
	CCriticalSection	m_csList;
	
public:
	// 数据库分组消息
	BOOL IsDbGMsgListFull();
	BOOL PushDBGMsg(LPDBGMSG pDBGMsg);
	BOOL PopDBGMsg(DBGMSG& dbgMsg);

	// 数据库个人消息
	BOOL IsDbPMsgListFull();
	BOOL PushDBPMsg(LPDBPMSG pDBPMsg);
	BOOL PopDBPMsg(DBPMSG& dbpMsg);

	// 外部群组消息
	BOOL PushRoomMsg(LPROOMMSG pRoomMsg);
	BOOL PopRoomMsg(ROOMMSG& roomMsg);
private:
	// 数据库分组消息
	long m_nHeaderDBGMsg, m_nTailDBGMsg;
	DBGMSG	m_aDBGMsg[MAX_GMSG];
	// 数据库个人消息
	long m_nHeaderDBPMsg, m_nTailDBPMsg;
	DBGMSG	m_aDBPMsg[MAX_PMSG];
	// 外部群组消息
	long m_nHeaderRoomMsg, m_nTailRoomMsg;
	ROOMMSG	m_aRoomMsg[MAX_RMSG];
	CCriticalSection	m_csMsg;

public:
	// 历史消息入库
	BOOL PushHisMsg(LPHISMSG pHisMsg);
	BOOL PushHisMsg(LPCUSERID pszSrcID, LPCUSERID pszDesID, BYTE ucFormat, long tmSend, LPCTSTR pszMsg);
	BOOL PushHisMsg(LPCUSERID pszSrcID, long nRoomID, long nRoomType, BYTE ucFormat, long tmSend, LPCTSTR pszMsg);
	BOOL PopHisMsg(HISMSG& hisMsg);
private:
	long	m_nHeaderHisMsg, m_nTailHisMsg;
	HISMSG	m_aHisMsg[MAX_HISMSG];
	CCriticalSection	m_csHisMsg;

public:
	// 离线消息
	BOOL PushOffMsg(LPOFFMSG pOffMsg);
	BOOL PushOffMsg(LPCUSERID pszSrcID, LPCUSERID pszDesID, BYTE ucFormat, long tmSend, LPCTSTR pszMsg);
	BOOL PopOffMsg(OFFMSG& offMsg);
private:
	// 离线消息
	long	m_nHeaderOffMsg, m_nTailOffMsg;
	OFFMSG	m_aOffMsg[MAX_OFFMSG];
	CCriticalSection	m_csOffMsg;

public:
	// 产品消息
	BOOL PushPrdtMsg(LPPRDTMSG pPrdtMsg);
	BOOL PushPrdtMsg(LPCTSTR pszPrdtID, LPCTSTR pszSrcID, BYTE ucFormat, long tmSend, LPCTSTR pszMsg);
	BOOL PopPrdtMsg(PRDTMSG& prdtMsg);
private:
	// 产品消息
	long	m_nHeaderPrdtMsg, m_nTailPrdtMsg;
	PRDTMSG	m_aPrdtMsg[MAX_PRDTMSG];
	CCriticalSection	m_csPrdtMsg;
	
public:
	// 群组离线消息
	BOOL PushRoomOffMsg(LPROOMOFFMSG pRoomOffMsg);
	BOOL PushRoomOffMsg(LPCTSTR pszSrcID, LPCTSTR pszDesID, LPCTSTR pszSrcName, ROOMID roomid, BYTE roomType, BYTE ucFormat, long tmSend, LPCTSTR pszMsg);
	BOOL PopRoomOffMsg(ROOMOFFMSG& roomOffMsg);
private:
	// 群组离线消息
	long m_nHeaderRoomOffMsg, m_nTailRoomOffMsg;
	ROOMOFFMSG m_aRoomOffMsg[MAX_ROOMOFFMSG];
	CCriticalSection	m_csRoomOffMsg;
};

#endif // !defined(_INFOLIST_H_)
