#include "stdafx.h"
#include "UPIM.h"
#include "MsgQueueSendMng.h"

CMsgQueueSendMng::CMsgQueueSendMng()
{
	m_pMyClient = NULL;
	m_hThread = NULL;
	m_hExitEvent = NULL;
	m_hStartEvent = NULL;
	m_bInit = FALSE;
}

CMsgQueueSendMng::~CMsgQueueSendMng()
{
	CLOSEHANDLE(m_hExitEvent);
	CLOSEHANDLE(m_hStartEvent);
}

//核心处理
DWORD WINAPI _AfxMsgQueueSend(LPVOID pVoid)
{
	CMsgQueueSendMng* pMsgQueueMng = (CMsgQueueSendMng*)pVoid;
	CMyClient* pMyClient = pMsgQueueMng->m_pMyClient;
	if(!pMyClient) return 0;
	HANDLE hExitEvent = pMsgQueueMng->m_hExitEvent;
	HANDLE hStartEvent = pMsgQueueMng->m_hStartEvent;
	char szUPID[USERID_SIZE]={0}, szMyUPID[USERID_SIZE]={0};
	
	char buf[MAX_SEND_BUF_LEN] = {0};
	sendmsg_req *pMsgReq = (sendmsg_req*)buf;
	broadcast_req* pBCReq = (broadcast_req*)buf;

	LPMSGQUEUE pMsgQueue = NULL;
	int i=0;
	BOOL bRet = FALSE;
	while(::WaitForSingleObject(hExitEvent,0) != WAIT_OBJECT_0)
	{
		::WaitForSingleObject(hStartEvent,INFINITE);
		ResetEvent ( hStartEvent );
		if ( ::WaitForSingleObject(hExitEvent,0) == WAIT_OBJECT_0 )
			break;
	
		while(pMsgQueueMng->PopMsg(&pMsgQueue))
		{
			TRACE("vec:%d id:%s name:%s PopMsg \n", pMsgQueue->vtId.size(), pMyClient->m_strUserid, pMyClient->m_strUserName);
			if(pMsgQueue->ucType == 0)
			{
				COPY_USERID(szMyUPID, pMyClient->m_strUserid);

				pMsgReq->format = pMsgQueue->nFormat;
				pMsgReq->msglen = pMsgQueue->nLen;
				strncpy(pMsgReq->msg, pMsgQueue->pBuf, pMsgReq->msglen);

				// LOGLOG
				USER_LOG("_AfxMsgQueueSend DesID:%s msg:%s", pMsgReq->msg, pMsgReq->cTouserid);

				pMsgReq->msg[pMsgReq->msglen] = 0;
				for (i=0; i < (int)pMsgQueue->vtId.size(); i++)
				{
					if(::WaitForSingleObject(hExitEvent,0)==WAIT_OBJECT_0) break;
					pMsgReq->tmSend=time(NULL);
					pMsgReq->dwMsgSeq = InterlockedIncrement(&g_nMsgSeq);
					COPY_USERID(pMsgReq->cTouserid, pMsgQueue->vtId[i].m_szUSERID);

					//////////////////////////////////////////////////////////////////////////
					// 发送
					bRet = pMyClient->SendMsgReq(pMsgReq);
					// 发送成功就写入文件库
 					if(bRet)
 						g_SqliteOp.DBInsertRecord(szMyUPID, pMsgQueue->vtId[i].m_szUSERID, 0, eRecord_Single, pMsgQueue->nFormat, pMsgQueue->pBuf);	
					Sleep(5);
				}
			}
			else
			{
				pBCReq->type = 0;
				COPYSTRARRAY(pBCReq->nickname, pMyClient->m_strUserid);
				pBCReq->msglen = pMsgQueue->nLen;
				strncpy(pBCReq->msg, pMsgQueue->pBuf, pBCReq->msglen);
				pBCReq->msg[pBCReq->msglen] = 0;
				for(i=0; i < (int)pMsgQueue->vtId.size(); i++)
				{
					if(::WaitForSingleObject(hExitEvent,0)==WAIT_OBJECT_0) 
						break;
					COPYSTRARRAY(pBCReq->toid, pMsgQueue->vtId[i].m_szUSERID);
					//pLoginClient->SendBoradCast(pBCReq);
					Sleep(5);
				}
			}
			TDEL(pMsgQueue);
			if(WaitForSingleObject(hExitEvent, 0) == WAIT_OBJECT_0) 
				break;
		}
	}
	return 0;
}

void CMsgQueueSendMng::Init(CMyClient* pMyClient)
{
	/*if(IsClientVer()) return;*/
	m_bInit = TRUE;
	m_pMyClient = pMyClient;
	m_hExitEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	m_hStartEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

	DWORD dwThreadID = 0;
	m_hThread = ::CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)_AfxMsgQueueSend, (LPVOID)this, 0, &dwThreadID);
}

void CMsgQueueSendMng::Exit()
{
	DeleteAll();
	if(m_hExitEvent) 
		SetEvent(m_hExitEvent);
	if(m_hStartEvent) 
		SetEvent(m_hStartEvent);
	if (m_hThread)
	{
		WaitForSingleObject(m_hThread, 5000);
		if( WaitForSingleObject(m_hThread, 0) == WAIT_TIMEOUT )
			TerminateThread( m_hThread, 0 );
		CloseHandle(m_hThread);
		m_hThread = NULL;
	}
	CLOSEHANDLE(m_hExitEvent);
	CLOSEHANDLE(m_hStartEvent);
	m_bInit = FALSE;
}

void CMsgQueueSendMng::PushMsg(LPCTSTR pszMsg, int format,std::vector<UPIMID>& vtId,HWND hWnd)
{
	if(!m_bInit) 
		return;
	int nLen = 0;
	if(!pszMsg || (nLen=strlen(pszMsg))<=0 || nLen>=MAX_TEXT_LEN) 
		return;
	if(vtId.size()<=0) return;
	LPMSGQUEUE  pMsgQueue = new MSGQUEUE;
	pMsgQueue->nFormat = format;
	pMsgQueue->hWnd = hWnd;
	pMsgQueue->nLen = nLen;
	pMsgQueue->pBuf = new char[nLen+1];
	strncpy(pMsgQueue->pBuf, pszMsg, nLen);
	pMsgQueue->pBuf[nLen] = 0;
	pMsgQueue->vtId.assign(vtId.begin(), vtId.end());
	m_cs.Lock();
	m_vtMsgQueue.push_back(pMsgQueue);
	m_cs.Unlock();
	SetEvent(m_hStartEvent);
}

void CMsgQueueSendMng::PushMsg(COMPLEX_MSG_DATA* pData, int format, std::vector<UPIMID>& vtId, HWND hWnd/* =NULL */)
{
	if(!m_bInit) return;
	int nLen = 0;
	if(!pData) return;
	if(vtId.size()<=0) return;
	LPMSGQUEUE  pMsgQueue = new MSGQUEUE;
	pMsgQueue->nFormat = format;
	pMsgQueue->hWnd = hWnd;
	pMsgQueue->pData = pData;
	nLen = pData->nTextLen;
 	pMsgQueue->nLen = nLen;
 	pMsgQueue->pBuf = new char[nLen+1];
 	strncpy(pMsgQueue->pBuf, pData->pBuf, nLen);
 	pMsgQueue->pBuf[nLen] = 0;

	pMsgQueue->vtId.assign(vtId.begin(), vtId.end());
	m_cs.Lock();
	m_vtMsgQueue.push_back(pMsgQueue);
	m_cs.Unlock();
	SetEvent(m_hStartEvent);
}

void CMsgQueueSendMng::PushBroadCast(LPCTSTR pszMsg, std::vector<UPIMID>& vtId,HWND hWnd)
{
	if(!m_bInit) 
		return;
	int nLen = 0;
	if((nLen=strlen(pszMsg))<=0 ) 
		return;
	if(vtId.size()<=0) 
		return;
	LPMSGQUEUE  pMsgQueue = new MSGQUEUE;
	pMsgQueue->nFormat = eMsgFormat_Txt;
	pMsgQueue->ucType = 1;
	pMsgQueue->hWnd = hWnd;
	pMsgQueue->nLen = nLen;
	pMsgQueue->pBuf = new char[nLen+1];
	strncpy(pMsgQueue->pBuf,pszMsg,nLen);
	pMsgQueue->pBuf[nLen] = 0;
	pMsgQueue->vtId.assign(vtId.begin(), vtId.end());
	m_cs.Lock();
	m_vtMsgQueue.push_back(pMsgQueue);
	m_cs.Unlock();
	SetEvent(m_hStartEvent);
}

BOOL CMsgQueueSendMng::PopMsg(LPMSGQUEUE* ppMsgQueue)
{
	if(!m_bInit) 
		return FALSE;
	m_cs.Lock();
	if(m_vtMsgQueue.size()<=0) 
	{
		m_cs.Unlock();
		return FALSE;
	}
	*ppMsgQueue = m_vtMsgQueue[0];
	m_vtMsgQueue.erase(m_vtMsgQueue.begin());
	m_cs.Unlock();
	return TRUE;
}

void CMsgQueueSendMng::DeleteAll()
{
	m_cs.Lock();
	LPMSGQUEUE pMsgQueue = NULL;
	for (int i=0; i < (int)m_vtMsgQueue.size(); i++)
	{
		pMsgQueue = m_vtMsgQueue[i];
		TDEL(pMsgQueue);
	}
	m_vtMsgQueue.clear();
	m_cs.Unlock();
}