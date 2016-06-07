// LinkManager.cpp: implementation of the CLinkManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "LinkManager.h"
#include "Func.h"

CLinkManager::CLinkManager()
{
	m_nDbCount = 0;
	m_nDefaultSel = -1;
	memset(m_aLinkInfo, 0, sizeof(OPLINK)*MAX_LINK_NUM );
	memset(&m_linkHisMsg,0,sizeof(OPLINK));
	memset(&m_linkOffMsg,0,sizeof(OPLINK));
	m_nFailCount = 0;
	::CoInitialize(NULL);
}

CLinkManager::~CLinkManager()
{
	::CoUninitialize();
	Exit();
}

BOOL CLinkManager::_Init(LPOPLINK pLinkInfo)
{
	if(!pLinkInfo) 
		return FALSE;
	pLinkInfo->nSel = m_nDefaultSel;
	TDEL(pLinkInfo->pAdodb);
	pLinkInfo->bConnected = FALSE;
	pLinkInfo->bBusy = FALSE;
	pLinkInfo->pAdodb = new CADODatabase();
	if(!pLinkInfo->pAdodb) 
		return FALSE;
	return TRUE;
}

// 初始化
BOOL CLinkManager::Init()
{
	m_nDbCount = g_config.nInfoDBCount;
	m_nDefaultSel = g_config.nInfoSel;
	if(m_nDefaultSel<0 || m_nDefaultSel>=m_nDbCount) m_nDefaultSel = 0;

	memcpy(m_dbConfig, g_config.aDBInfoConfig, sizeof(DBCONFIG)*MAX_DBCOUNT);

	for ( DWORD dwLinkIndex=0; dwLinkIndex<MAX_LINK_NUM; dwLinkIndex++ )
	{
		_Init(&m_aLinkInfo[dwLinkIndex]);
	}
 	_Init(&m_linkHisMsg);
 	_Init(&m_linkOffMsg);
	return TRUE;
}

// 退出
BOOL CLinkManager::Exit()
{
	for ( DWORD dwLinkIndex=0; dwLinkIndex<MAX_LINK_NUM; dwLinkIndex++ )
	{
		TDEL(m_aLinkInfo[dwLinkIndex].pAdodb);
	}
 	TDEL(m_linkHisMsg.pAdodb);
 	TDEL(m_linkOffMsg.pAdodb);
	return TRUE;
}

LPOPLINK CLinkManager::GetLink( char *lpszErrMsg,int nSize,BOOL bPrimary)
{
	DWORD	dwLinkIndex = 0;
	DWORD	dwFirstFreeIndex = MAX_LINK_NUM;
	LPOPLINK pLinkInfo = NULL;
	m_csLink.Lock();
	for (dwLinkIndex=0; dwLinkIndex<MAX_LINK_NUM; dwLinkIndex++ )
	{
		if(!m_aLinkInfo[dwLinkIndex].bBusy) 
		{
			if ( dwFirstFreeIndex >= MAX_LINK_NUM ) dwFirstFreeIndex = dwLinkIndex;
			if(!bPrimary)	break;
			if(m_aLinkInfo[dwLinkIndex].nSel==m_nDefaultSel) break;
		}
	}
	if(dwLinkIndex>=MAX_LINK_NUM) 
	{
		if ( dwFirstFreeIndex >= MAX_LINK_NUM )
		{
			if(lpszErrMsg)	
				strncpy(lpszErrMsg,"后台繁忙,请稍后再试",nSize);
			m_csLink.Unlock();
			return NULL;	
		}
		else
		{
			pLinkInfo = &m_aLinkInfo[dwFirstFreeIndex];
		}
	}
	else
	{
		pLinkInfo = &m_aLinkInfo[dwLinkIndex];
	}
	pLinkInfo->bBusy = TRUE;
	m_csLink.Unlock();

	if(bPrimary)
	{
		if(pLinkInfo->nSel!=m_nDefaultSel)
		{
			pLinkInfo->pAdodb->Close();
			pLinkInfo->bConnected = FALSE;
		}
		if(!pLinkInfo->bConnected)
		{
			if(ConnectDataBase(&m_dbConfig[m_nDefaultSel],pLinkInfo->pAdodb))
			{
				pLinkInfo->bConnected = TRUE;
				pLinkInfo->nSel = m_nDefaultSel;
				if (m_nFailCount>0)
				{
					char szError[256] = {0};
					__nsprintf(szError,256,"连接[%s]-%s 数据库成功", m_dbConfig[m_nDefaultSel].szDataSource,m_dbConfig[m_nDefaultSel].szDataBase);
					InterlockedExchange(&m_nFailCount,0);
				}
			}
		}
	}
	if(pLinkInfo->bConnected)
	{
		pLinkInfo->nBusyTime = (long)time(NULL);
		return pLinkInfo;
	}

	if(bPrimary) 
	{
		char szError[256] = {0};
		__nsprintf(szError,256,"连接[%s]-%s 数据库失败", m_dbConfig[m_nDefaultSel].szDataSource,m_dbConfig[m_nDefaultSel].szDataBase);
		if(lpszErrMsg) 
			strncpy(lpszErrMsg,"连接数据库失败!",nSize);
		InterlockedIncrement(&m_nFailCount);
		// 连接未成功
		m_csLink.Lock();
		pLinkInfo->bBusy = FALSE;
		m_csLink.Unlock();
		return NULL;
	}

	if(_Connect2(pLinkInfo)) 
	{
		if (m_nFailCount > 0)
		{
			long nSel = pLinkInfo->nSel;
			char szError[256] = {0};
			__nsprintf(szError,256,"连接[%s]-%s 数据库成功", m_dbConfig[nSel].szDataSource,m_dbConfig[nSel].szDataBase);
			InterlockedExchange(&m_nFailCount,0);
		}
		return pLinkInfo;
	}

	InterlockedIncrement(&m_nFailCount);

	// 连接都未成功
	m_csLink.Lock();
	pLinkInfo->bBusy = FALSE;
	m_csLink.Unlock();

	if (lpszErrMsg)		
		strncpy(lpszErrMsg,"连接数据库失败!",nSize);		

	return NULL;
}

LPOPLINK CLinkManager::GetHisMsgLink()
{
	if(m_linkHisMsg.bConnected) 
	{
		return &m_linkHisMsg;
	}
	if(_Connect2(&m_linkHisMsg,FALSE)) 
	{
		return &m_linkHisMsg;
	}

	return NULL;
}

LPOPLINK CLinkManager::GetOffMsgLink()
{
	if(m_linkOffMsg.nSel==m_nDefaultSel && m_linkOffMsg.bConnected)
	{
		return &m_linkOffMsg;
	}
	if(_Connect(&m_linkOffMsg,FALSE)) 
	{
		return &m_linkOffMsg;
	}

	return NULL;
}

// 释放连接
void CLinkManager::FreeLink( LPOPLINK pLink)
{
	if (!pLink) return;

	m_csLink.Lock();
	pLink->nBusyTime = 0;
	pLink->bBusy = FALSE;
	m_csLink.Unlock();
}

// 断开连接
void CLinkManager::DisConnect( LPOPLINK pLink)
{
	if(!pLink) return;
	pLink->pAdodb->Close();
	pLink->bConnected = FALSE;

	m_csLink.Lock();
	pLink->nBusyTime = 0;
	pLink->bBusy = FALSE;
	m_csLink.Unlock();
}

void CLinkManager::CheckConnect(LPOPLINK pLink)
{
	if(!pLink) 
		return;
	if(!pLink->pAdodb)
	{
		pLink->bConnected = FALSE;
		return;
	}
	pLink->pAdodb->Close();
	pLink->bConnected = FALSE;
}

BOOL CLinkManager::_Connect(LPOPLINK pLink,BOOL bFailLog)
{
	if(!pLink) 
		return FALSE;
	if(ConnectDataBase(&m_dbConfig[m_nDefaultSel],pLink->pAdodb))
	{
		pLink->bConnected = TRUE;
		pLink->nSel = m_nDefaultSel;
		pLink->nBusyTime = (long)time(NULL);
		return TRUE;
	}
	else
	{	
		if(bFailLog)
		{
			char szError[256] = {0};
			__nsprintf(szError,256,"连接[%s]-%s 数据库失败", m_dbConfig[m_nDefaultSel].szDataSource,m_dbConfig[m_nDefaultSel].szDataBase);
		}		
	}

	for(short i=0;i<m_nDbCount;i++)
	{
		if(i==m_nDefaultSel) continue;
		if(ConnectDataBase(&m_dbConfig[i],pLink->pAdodb))
		{
			pLink->bConnected = TRUE;
			pLink->nSel = i;
			pLink->nBusyTime = (long)time(NULL);
			return TRUE;
		}
		else
		{
			if(bFailLog)
			{
				char szError[256] = {0};
				__nsprintf(szError,256,"连接[%s]-%s 数据库失败",m_dbConfig[i].szDataSource, m_dbConfig[i].szDataBase);
			}			
		}
	}
	return FALSE;
}

BOOL	CLinkManager::_Connect2(LPOPLINK pLink,BOOL bFailLog)
{
	if(!pLink) 
		return FALSE;
	pLink->pAdodb->Close();
	for(short i=0;i<m_nDbCount;i++)
	{
		if(ConnectDataBase(&m_dbConfig[i],pLink->pAdodb))
		{
			pLink->bConnected = TRUE;
			pLink->nSel = i;
			pLink->nBusyTime = (long)time(NULL);
			return TRUE;
		}
		else
		{
			if(bFailLog)
			{
				char szError[256] = {0};
				__nsprintf(szError,256,"连接[%s]-%s 数据库失败",m_dbConfig[i].szDataSource, m_dbConfig[i].szDataBase);
			}			
		}
	}
	return FALSE;
}