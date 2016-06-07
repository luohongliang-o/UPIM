#include "stdafx.h"
#include "InfoList.h"
#include "globalvar.h"
CInfoList::CInfoList()
{
	m_nHeaderUPID=0, m_nTailUPID=0;
	memset(m_aUserState, 0, sizeof(USERSTATE)*MAX_STATE);

	m_nHeaderDBGMsg=0, m_nTailDBGMsg=0;
	memset(m_aDBGMsg, 0, sizeof(DBGMSG)*MAX_GMSG);

	m_nHeaderDBPMsg=0, m_nTailDBPMsg=0;
	memset(m_aDBPMsg, 0, sizeof(DBPMSG)*MAX_PMSG);

	m_nHeaderRoomMsg=0, m_nTailRoomMsg=0;
	memset(m_aRoomMsg, 0, sizeof(ROOMMSG)*MAX_RMSG);

	m_nHeaderHisMsg=0, m_nTailHisMsg=0;
	memset(m_aHisMsg, 0, sizeof(HISMSG)*MAX_HISMSG);

	m_nHeaderOffMsg=0, m_nTailOffMsg=0;
	memset(m_aOffMsg, 0, sizeof(OFFMSG)*MAX_OFFMSG);

	m_nHeaderPrdtMsg=0, m_nTailPrdtMsg=0;
	memset(m_aPrdtMsg, 0, sizeof(PRDTMSG)*MAX_PRDTMSG);

	m_nHeaderRoomOffMsg=0, m_nTailRoomOffMsg=0;
	memset(m_aRoomOffMsg, 0, sizeof(ROOMOFFMSG)*MAX_ROOMOFFMSG);
}

CInfoList::~CInfoList()
{
}

BOOL CInfoList::PushUSER(USERSTATE* pUserState)
{
 	if(m_nHeaderUPID+1==m_nTailUPID||m_nTailUPID==0&&m_nHeaderUPID+1==MAX_STATE) {
		OP_LOG_INFO("人太多了，处理不过来了...！！！");
		return FALSE;
	}
	memcpy(&m_aUserState[m_nHeaderUPID], pUserState, sizeof(USERSTATE));
	InterlockedExchange(&m_nHeaderUPID, ((m_nHeaderUPID+1)==MAX_STATE) ? 0 : m_nHeaderUPID+1);
	return TRUE;
}

BOOL CInfoList::PushUSER(LPCSTR pszID, BYTE ucState, LPCSTR pszToUser/* =_T("") */)
{
	USERSTATE mUserState = {0};
	COPYSTRARRAY(mUserState.m_szUSERID, pszID);
	mUserState.m_ucState = ucState;
	COPYSTRARRAY(mUserState.m_szToUser, pszToUser);
	return PushUSER(&mUserState);
}

BOOL CInfoList::PopUSER(USERSTATE& mUserState)
{
	m_csList.Lock();
	if(m_nHeaderUPID == m_nTailUPID) 
	{
		m_csList.Unlock();
		return FALSE;
	}
	memcpy(&mUserState, &m_aUserState[m_nTailUPID], sizeof(USERSTATE));
	InterlockedExchange(&m_nTailUPID, ((m_nTailUPID+1)==MAX_STATE) ? 0 : m_nTailUPID+1);
	m_csList.Unlock();
	return TRUE;
}

BOOL CInfoList::IsDbGMsgListFull()
{
	if(m_nHeaderDBGMsg+1 == m_nTailDBGMsg || (m_nTailDBGMsg==0 && m_nHeaderDBGMsg+1==MAX_GMSG) ) 
		return TRUE;
	return FALSE;
}

BOOL CInfoList::PushDBGMsg(LPDBGMSG pDBGMsg)
{
	if( m_nHeaderDBGMsg+1 == m_nTailDBGMsg || m_nTailDBGMsg==0 && m_nHeaderDBGMsg+1==MAX_GMSG ) 
		return FALSE;
	if(!pDBGMsg) 
		return FALSE;
	memcpy(&m_aDBGMsg[m_nHeaderDBGMsg], pDBGMsg, sizeof(DBGMSG));
	InterlockedExchange(&m_nHeaderDBGMsg, ((m_nHeaderDBGMsg+1)==MAX_GMSG) ? 0 : m_nHeaderDBGMsg+1);
	return TRUE;
}

BOOL CInfoList::PopDBGMsg(DBGMSG& dbgMsg)
{
	m_csMsg.Lock();
	if(m_nHeaderDBGMsg == m_nTailDBGMsg) 
	{
		m_csMsg.Unlock();
		return FALSE;
	}
	memcpy(&dbgMsg, &m_aDBGMsg[m_nTailDBGMsg], sizeof(DBGMSG));
	InterlockedExchange(&m_nTailDBGMsg, ((m_nTailDBGMsg+1)==MAX_GMSG) ? 0 : m_nTailDBGMsg+1);
	m_csMsg.Unlock();
	return TRUE;
}

BOOL CInfoList::IsDbPMsgListFull()
{
	if(m_nHeaderDBPMsg+1 == m_nTailDBPMsg || m_nTailDBPMsg==0 && m_nHeaderDBPMsg+1==MAX_PMSG ) 
		return TRUE;
	return FALSE;
}

BOOL CInfoList::PushDBPMsg(LPDBPMSG pDBPMsg)
{
	if(m_nHeaderDBPMsg+1 == m_nTailDBPMsg || m_nTailDBPMsg==0 && m_nHeaderDBPMsg+1==MAX_PMSG ) 
		return FALSE;
	if(!pDBPMsg) 
		return FALSE;
	memcpy(&m_aDBPMsg[m_nHeaderDBPMsg], pDBPMsg, sizeof(DBPMSG));
	InterlockedExchange(&m_nHeaderDBPMsg, ((m_nHeaderDBPMsg+1)==MAX_PMSG) ? 0 : m_nHeaderDBPMsg+1);
	return TRUE;
}

BOOL CInfoList::PopDBPMsg(DBPMSG& dbpMsg)
{
	m_csMsg.Lock();
	if(m_nHeaderDBPMsg==m_nTailDBPMsg) 
	{
		m_csMsg.Unlock();
		return FALSE;
	}
	memcpy(&dbpMsg, &m_aDBPMsg[m_nTailDBPMsg], sizeof(DBPMSG));
	InterlockedExchange(&m_nTailDBPMsg, ((m_nTailDBPMsg+1)==MAX_PMSG) ? 0 : m_nTailDBPMsg+1);
	m_csMsg.Unlock();
	return TRUE;
}

BOOL CInfoList::PushRoomMsg(LPROOMMSG pRoomMsg)
{
	if(m_nHeaderRoomMsg+1==m_nTailRoomMsg || m_nTailRoomMsg==0&&m_nHeaderRoomMsg+1==MAX_RMSG) 
		return FALSE;
	memcpy(&m_aRoomMsg[m_nHeaderRoomMsg], pRoomMsg, sizeof(ROOMMSG));
	InterlockedExchange(&m_nHeaderRoomMsg,((m_nHeaderRoomMsg+1)==MAX_RMSG)?0:m_nHeaderRoomMsg+1);
	return TRUE;
}

BOOL CInfoList::PopRoomMsg(ROOMMSG& roomMsg)
{
	m_csMsg.Lock();
	if(m_nHeaderRoomMsg==m_nTailRoomMsg) 
	{
		m_csMsg.Unlock();
		return FALSE;
	}
	memcpy(&roomMsg,&m_aRoomMsg[m_nTailRoomMsg],sizeof(ROOMMSG));
	InterlockedExchange(&m_nTailRoomMsg,((m_nTailRoomMsg+1)==MAX_RMSG)?0:m_nTailRoomMsg+1);
	m_csMsg.Unlock();
	return TRUE;
}

BOOL CInfoList::PushHisMsg(LPHISMSG pHisMsg)
{
	if(m_nHeaderHisMsg+1==m_nTailHisMsg || m_nTailHisMsg==0 && m_nHeaderHisMsg+1==MAX_HISMSG ) 
		return FALSE;
	memcpy(&m_aHisMsg[m_nHeaderHisMsg], pHisMsg, sizeof(HISMSG));
	InterlockedExchange(&m_nHeaderHisMsg, ((m_nHeaderHisMsg+1)==MAX_HISMSG) ? 0 : m_nHeaderHisMsg+1);
	return TRUE;
}

BOOL CInfoList::PushHisMsg(LPCUSERID pszSrcID, LPCUSERID pszDesID, BYTE ucFormat,  long tmSend, LPCTSTR pszMsg)
{
	if(m_nHeaderHisMsg+1 == m_nTailHisMsg || m_nTailHisMsg==0 && m_nHeaderHisMsg+1==MAX_HISMSG) 
		return FALSE;
	HISMSG hisMsg = {0};
	COPYSTRARRAY(hisMsg.m_szSrcID, pszSrcID);
	COPYSTRARRAY(hisMsg.m_szDesID, pszDesID);
	hisMsg.m_ucMsgType = 0;
	hisMsg.m_ucFormat = ucFormat;
	CTime m_Time(tmSend);
	hisMsg.m_nSendDate = m_Time.GetYear() * 10000 + m_Time.GetMonth() * 100 + m_Time.GetDay();
	hisMsg.m_nSendTime = m_Time.GetHour() * 10000 + m_Time.GetMinute() * 100 + m_Time.GetSecond();
	COPYSTRARRAY(hisMsg.m_szMsg,pszMsg);
	memcpy(&m_aHisMsg[m_nHeaderHisMsg],&hisMsg,sizeof(HISMSG));
	InterlockedExchange(&m_nHeaderHisMsg,((m_nHeaderHisMsg+1)==MAX_HISMSG)?0:m_nHeaderHisMsg+1);
	return TRUE;
}

BOOL CInfoList::PushHisMsg(LPCUSERID pszSrcID, long nRoomID, long nRoomType, BYTE ucFormat, long tmSend, LPCTSTR pszMsg)
{
	if(m_nHeaderHisMsg+1==m_nTailHisMsg||m_nTailHisMsg==0&&m_nHeaderHisMsg+1==MAX_HISMSG) 
		return FALSE;
	HISMSG hisMsg = {0};
	COPYSTRARRAY(hisMsg.m_szSrcID,pszSrcID);
	sprintf(hisMsg.m_szDesID,"%d",nRoomID);
	hisMsg.m_ucMsgType = (BYTE)nRoomType;
	hisMsg.m_ucFormat = ucFormat;
	CTime m_Time(tmSend);
	hisMsg.m_nSendDate = m_Time.GetYear() * 10000 + m_Time.GetMonth() * 100 + m_Time.GetDay();
	hisMsg.m_nSendTime = m_Time.GetHour() * 10000 + m_Time.GetMinute() * 100 + m_Time.GetSecond();
	COPYSTRARRAY(hisMsg.m_szMsg,pszMsg);
	memcpy(&m_aHisMsg[m_nHeaderHisMsg], &hisMsg, sizeof(HISMSG));
	InterlockedExchange(&m_nHeaderHisMsg, ((m_nHeaderHisMsg+1)==MAX_HISMSG) ? 0 : m_nHeaderHisMsg+1);
	return TRUE;
}

BOOL CInfoList::PopHisMsg(HISMSG& hisMsg)
{
	m_csHisMsg.Lock();
	if(m_nHeaderHisMsg==m_nTailHisMsg) 
	{
		m_csHisMsg.Unlock();
		return FALSE;
	}
	memcpy(&hisMsg,&m_aHisMsg[m_nTailHisMsg],sizeof(HISMSG));
	InterlockedExchange(&m_nTailHisMsg,((m_nTailHisMsg+1)==MAX_HISMSG)?0:m_nTailHisMsg+1);
	m_csHisMsg.Unlock();
	return TRUE;
}

BOOL CInfoList::PushOffMsg(LPOFFMSG pOffMsg)
{
	if(m_nHeaderOffMsg+1==m_nTailOffMsg||m_nTailOffMsg==0&&m_nHeaderOffMsg+1==MAX_OFFMSG) 
		return FALSE;
	memcpy(&m_aOffMsg[m_nHeaderOffMsg],pOffMsg,sizeof(OFFMSG));
	InterlockedExchange(&m_nHeaderOffMsg,((m_nHeaderOffMsg+1)==MAX_OFFMSG)?0:m_nHeaderOffMsg+1);
	return TRUE;
}

BOOL CInfoList::PushOffMsg(LPCUSERID pszSrcID,LPCUSERID pszDesID,BYTE ucFormat,long tmSend, LPCTSTR pszMsg)
{
	if(m_nHeaderOffMsg+1==m_nTailOffMsg||m_nTailOffMsg==0&&m_nHeaderOffMsg+1==MAX_OFFMSG)
		return FALSE;
	OFFMSG offMsg = {0};
	COPYSTRARRAY(offMsg.m_szSrcID,pszSrcID);
	COPYSTRARRAY(offMsg.m_szDesID,pszDesID);
	offMsg.m_ucFormat = ucFormat;
	offMsg.m_nSendTime = tmSend;
	COPYSTRARRAY(offMsg.m_szMsg,pszMsg);
	memcpy(&m_aOffMsg[m_nHeaderOffMsg],&offMsg,sizeof(OFFMSG));
	InterlockedExchange(&m_nHeaderOffMsg,((m_nHeaderOffMsg+1)==MAX_OFFMSG)?0:m_nHeaderOffMsg+1);
	return TRUE;
}

BOOL CInfoList::PopOffMsg(OFFMSG& offMsg)
{
	m_csOffMsg.Lock();
	if(m_nHeaderOffMsg==m_nTailOffMsg) 
	{
		m_csOffMsg.Unlock();
		return FALSE;
	}
	memcpy(&offMsg,&m_aOffMsg[m_nTailOffMsg],sizeof(OFFMSG));
	InterlockedExchange(&m_nTailOffMsg,((m_nTailOffMsg+1)==MAX_OFFMSG)?0:m_nTailOffMsg+1);
	m_csOffMsg.Unlock();
	return TRUE;
}

// 产品消息
BOOL CInfoList::PushPrdtMsg(LPPRDTMSG pPrdtMsg)
{
	if(m_nHeaderPrdtMsg+1==m_nTailPrdtMsg||m_nTailPrdtMsg==0&&m_nHeaderPrdtMsg+1==MAX_PRDTMSG) return FALSE;
	memcpy(&m_aPrdtMsg[m_nHeaderPrdtMsg],pPrdtMsg,sizeof(PRDTMSG));
	InterlockedExchange(&m_nHeaderPrdtMsg,((m_nHeaderPrdtMsg+1)==MAX_PRDTMSG)?0:m_nHeaderPrdtMsg+1);
	return TRUE;
}

BOOL CInfoList::PushPrdtMsg(LPCTSTR pszPrdtID,LPCTSTR pszSrcID,BYTE ucFormat,long tmSend, LPCTSTR pszMsg)
{
	if(m_nHeaderPrdtMsg+1==m_nTailPrdtMsg||m_nTailPrdtMsg==0&&m_nHeaderPrdtMsg+1==MAX_PRDTMSG) return FALSE;
	PRDTMSG prdtMsg = {0};
	COPYSTRARRAY(prdtMsg.m_szPrdtID,pszPrdtID);
	COPYSTRARRAY(prdtMsg.m_szSrcID,pszSrcID);
	prdtMsg.m_ucFormat = ucFormat;
	prdtMsg.m_tmSend = tmSend;
	COPYSTRARRAY(prdtMsg.m_szMsg,pszMsg);
	memcpy(&m_aPrdtMsg[m_nHeaderPrdtMsg],&prdtMsg,sizeof(PRDTMSG));
	InterlockedExchange(&m_nHeaderPrdtMsg,((m_nHeaderPrdtMsg+1)==MAX_PRDTMSG)?0:m_nHeaderPrdtMsg+1);
	return TRUE;
}

BOOL CInfoList::PopPrdtMsg(PRDTMSG& prdtMsg)
{
	m_csPrdtMsg.Lock();
	if(m_nHeaderPrdtMsg==m_nTailPrdtMsg) 
	{
		m_csPrdtMsg.Unlock();
		return FALSE;
	}
	memcpy(&prdtMsg,&m_aPrdtMsg[m_nTailPrdtMsg],sizeof(PRDTMSG));
	InterlockedExchange(&m_nTailPrdtMsg,((m_nTailPrdtMsg+1)==MAX_PRDTMSG)?0:m_nTailPrdtMsg+1);
	m_csPrdtMsg.Unlock();
	return TRUE;
}


// 群组离线消息
BOOL CInfoList::PushRoomOffMsg(LPROOMOFFMSG pRoomOffMsg)
{
	if(m_nHeaderRoomOffMsg+1==m_nTailRoomOffMsg||m_nTailRoomOffMsg==0&&m_nHeaderRoomOffMsg+1==MAX_ROOMOFFMSG) return FALSE;
	memcpy(&m_aRoomOffMsg[m_nHeaderRoomOffMsg],pRoomOffMsg,sizeof(ROOMOFFMSG));
	InterlockedExchange(&m_nHeaderRoomOffMsg,((m_nHeaderRoomOffMsg+1)==MAX_ROOMOFFMSG)?0:m_nHeaderRoomOffMsg+1);
	return TRUE;
}

BOOL CInfoList::PushRoomOffMsg(LPCTSTR pszSrcID,LPCTSTR pszDesID,LPCTSTR pszSrcName,ROOMID roomid,BYTE roomType,BYTE ucFormat,long tmSend,LPCTSTR pszMsg)
{
	if(m_nHeaderRoomOffMsg+1==m_nTailRoomOffMsg||m_nTailRoomOffMsg==0&&m_nHeaderRoomOffMsg+1==MAX_ROOMOFFMSG) return FALSE;
	ROOMOFFMSG roomOffMsg = {0};
	COPYSTRARRAY(roomOffMsg.m_szSrcID,pszSrcID);
	COPYSTRARRAY(roomOffMsg.m_szDesID,pszDesID);
	COPYSTRARRAY(roomOffMsg.m_szSrcName,pszSrcName);
	roomOffMsg.m_nRoomid=roomid;
	roomOffMsg.m_ucRoomType=roomType;
	roomOffMsg.m_ucFormat=ucFormat;
	roomOffMsg.m_nSendTime=tmSend;
	COPYSTRARRAY(roomOffMsg.m_szMsg,pszMsg);
	memcpy(&m_aRoomOffMsg[m_nHeaderRoomOffMsg],&roomOffMsg,sizeof(ROOMOFFMSG));
	InterlockedExchange(&m_nHeaderRoomOffMsg,((m_nHeaderRoomOffMsg+1)==MAX_ROOMOFFMSG)?0:m_nHeaderRoomOffMsg+1);
	return TRUE;
}

BOOL CInfoList::PopRoomOffMsg(ROOMOFFMSG& roomOffMsg)
{
	m_csRoomOffMsg.Lock();
	if(m_nHeaderRoomOffMsg==m_nTailRoomOffMsg) 
	{
		m_csRoomOffMsg.Unlock();
		return FALSE;
	}
	memcpy(&roomOffMsg,&m_aRoomOffMsg[m_nTailRoomOffMsg],sizeof(ROOMOFFMSG));
	InterlockedExchange(&m_nTailRoomOffMsg,((m_nTailRoomOffMsg+1)==MAX_ROOMOFFMSG)?0:m_nTailRoomOffMsg+1);
	m_csRoomOffMsg.Unlock();
	return TRUE;
}