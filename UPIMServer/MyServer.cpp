#include "StdAfx.h"
#include "MyServer.h"
#include "globalvar.h"
#include "ProcessReq.h"

CMyServer::CMyServer(void)
	:m_Server(this),m_PackInfo(0,0,0,0)
{
}

CMyServer::~CMyServer(void)
{
}

EnHandleResult CMyServer::OnPrepareListen(SOCKET soListen)
{
	TCHAR szAddress[40];
	int iAddressLen = sizeof(szAddress) / sizeof(TCHAR);
	USHORT usPort;

	m_Server.GetListenAddress(szAddress, iAddressLen, usPort);
	::PostOnPrepareListen(szAddress, usPort);
	return HR_OK;
}

EnHandleResult CMyServer::OnAccept(CONNID dwConnID, SOCKET soClient)
{
	BOOL bPass = TRUE;
	TCHAR szAddress[40];
	int iAddressLen = sizeof(szAddress) / sizeof(TCHAR);
	USHORT usPort;

	m_Server.GetRemoteAddress(dwConnID, szAddress, iAddressLen, usPort);

	if(!m_strAddress.IsEmpty())
	{
		if(m_strAddress.CompareNoCase(szAddress) == 0)
			bPass = FALSE;
	}

	//::PostOnAccept(dwConnID, szAddress, usPort, bPass);

	return bPass ? HR_OK : HR_ERROR;
}

LPPKGRECVINFO CMyServer::GetPkgInfoFromMap(LONG mConID)
{
	LPPKGRECVINFO ppkgInfo = NULL;
	m_csPkgMapItem.Lock();
	if (g_mapPKGRECVINFO.size() > 0)
	{
		PKGRECV_ITER pkg_iter = g_mapPKGRECVINFO.find(mConID);
		if (pkg_iter != g_mapPKGRECVINFO.end())			// 找到了，以前存储过
			ppkgInfo = pkg_iter->second;
	}
	m_csPkgMapItem.Unlock();
	return ppkgInfo;
}

BOOL CMyServer::Disconnect(CONNID dwConnID, BOOL bForce /* = TRUE */)
{
	return m_Server.Disconnect(dwConnID, bForce);
}

BOOL CMyServer::DelPkgInfoFromMap(LONG mConID)
{
	m_csPkgMapItem.Lock();
	PKGRECV_ITER pkg_iter = g_mapPKGRECVINFO.find(mConID);
	if (pkg_iter != g_mapPKGRECVINFO.end())
	{
		// 找到了，以前存储过
 		delete pkg_iter->second;
  		pkg_iter->second = NULL;
		g_mapPKGRECVINFO.erase(pkg_iter);
	}
	m_csPkgMapItem.Unlock();
	return TRUE;
}

EnHandleResult CMyServer::OnReceive(CONNID dwConnID, const BYTE* pData, int iLength)
{
	INITCOM();				//init_com
	BOOL bSendSuc = FALSE;
	//::PostOnReceive(dwConnID, pData, iLength);
	m_PackInfo.RecieveDataLen+=iLength;
	m_PackInfo.RecievePackLen+=1;

#ifdef __NEW_MODE
	//////////////////////////////////////////////////////////////////////////
	// 网络环境很烂的客户，或者xp下的用户，一个数据表可能在网络层被截成几个包发送，需要先组包
	// 如果需要再次接收
	LPPKGRECVINFO pPkgInfo = NULL;
	pPkgInfo = GetPkgInfoFromMap((LONG)dwConnID);

	if (pPkgInfo && pPkgInfo->bNeedRecvMore)
	{
		int nNeedrecv = pPkgInfo->nNeedRecvLen - pPkgInfo->nRecvLen;
		// 接收完毕
		if (nNeedrecv == iLength)
		{
			memcpy(pPkgInfo->m_btInfo + pPkgInfo->nRecvLen, pData, iLength);
			pPkgInfo->nRecvLen = pPkgInfo->nRecvLen + iLength;
			OnReceive_Ex((DWORD)dwConnID, pPkgInfo->m_btInfo, pPkgInfo->nNeedRecvLen);
			DelPkgInfoFromMap((LONG)dwConnID);
			OP_LOG_INFO("bNeedRecvMore # 0--- dwConnID:%d ",dwConnID);
		}
		// 包没有接收完
		else if (nNeedrecv > iLength)
		{
			memcpy(pPkgInfo->m_btInfo + pPkgInfo->nRecvLen, pData, iLength);
			pPkgInfo->nRecvLen = pPkgInfo->nRecvLen + iLength;
			OP_LOG_INFO("bNeedRecvMore # 1--- dwConnID:%d nNeedrecv:%d iLength:%d",dwConnID,nNeedrecv,iLength);
		}
		//我屮艸芔茻，接收到的长度比我需要的大！！！ 碎包后面又粘包？
		else if (nNeedrecv < iLength)
		{
			memcpy(pPkgInfo->m_btInfo + pPkgInfo->nRecvLen, pData, iLength);
			pPkgInfo->nRecvLen = pPkgInfo->nRecvLen + iLength;
			OnReceive_Ex((DWORD)dwConnID, pPkgInfo->m_btInfo, pPkgInfo->nNeedRecvLen);
			DelPkgInfoFromMap((LONG)dwConnID);
			OP_LOG_INFO("dwConnID:%d 我屮艸芔茻,粗大事了！！！",dwConnID);
		}
	}
	else
	{
		// 组包，有可能是复杂网络环境下，一次OnReceive没有发送完所有的数据，是分次发送的，此时我的pkglen>iLength,需要吧接收到的数据存下来，等数据到齐之后在处理，处理完了，删除缓冲区
		struct tagIMReqHeader* pHead = (struct tagIMReqHeader*)(char*)pData;
		int CompLen = pHead->PacketLen + sizeof(IMREQHEADER);
		if (CompLen > iLength ){
			if (pHead->PacketLen < ANSBUFFER_LEN) //防止包长被冲爆导致崩溃
			{
				LPPKGRECVINFO m_pPkgInfo = new PKGRECVINFO(); 
				m_pPkgInfo->bNeedRecvMore = TRUE;
				m_pPkgInfo->nRecvLen = iLength;									// 包的实际接收长度
				m_pPkgInfo->nNeedRecvLen = pHead->PacketLen + sizeof(IMREQHEADER);
				OP_LOG_INFO("dwConnID %d 数据包长度%d",dwConnID,pHead->PacketLen);
				memcpy(m_pPkgInfo->m_btInfo, pData, iLength);
				LONG lConid = (LONG)dwConnID;
				m_csPkgMapItem.Lock();
				g_mapPKGRECVINFO.insert(map<LONG, LPPKGRECVINFO>::value_type(lConid, m_pPkgInfo));
				m_csPkgMapItem.Unlock();
			}
		}
		// 刚好一个包,正确
		else if (CompLen == iLength)
		{
			OnReceive_Ex((DWORD)dwConnID, pData, iLength);
		}
		// 解包，此处怕粘包现象，根据包长度进行解包
		else
		{
			int nIndex =0;
			int restLen = iLength;
			while (restLen > 0)
			{
				struct tagIMReqHeader* pHead = (struct tagIMReqHeader*)(char*)(pData+nIndex);
				if (pHead->PacketLen < ANSBUFFER_LEN)
				{
					BYTE* pAllData = new BYTE[64*1024];
					memset(pAllData, 0, 64*1024);

					int thisLength = pHead->PacketLen + sizeof(IMREQHEADER);
					memcpy(pAllData, pData + nIndex, thisLength);

					OnReceive_Ex((DWORD)dwConnID, pAllData, thisLength);
					nIndex = nIndex + thisLength;
					restLen = restLen - thisLength;
					TDEL(pAllData);
				}
				else
					break;
			}
		}
	}
#else
	// 解包，此处怕粘包现象，根据包长度进行解包
	int nIndex =0;
	int restLen = iLength;
	while (restLen > 0)
	{
		struct tagIMReqHeader* pHead = (struct tagIMReqHeader*)(char*)(pData+nIndex);
		if (pHead->PacketLen < ANSBUFFER_LEN)
		{
			BYTE* pAllData = new BYTE[64*1024];
			memset(pAllData, 0, 64*1024);

			int thisLength = pHead->PacketLen + sizeof(IMREQHEADER);
			memcpy(pAllData, pData + nIndex, thisLength);

			OnReceive_Ex((DWORD)dwConnID, pAllData, thisLength);
			nIndex = nIndex + thisLength;
			restLen = restLen - thisLength;
			TDEL(pAllData);
		}
		else
			break;
	}
#endif // __NEW_MODE
	UNINITCOM();	//uninit_com

	return HR_OK;
}

EnHandleResult CMyServer::OnSend(CONNID dwConnID, const BYTE* pData, int iLength)
{
	//::PostOnSend(dwConnID, pData, iLength);
	m_PackInfo.SendDataLen += iLength;
	m_PackInfo.SendPackLen+=1;
	return HR_OK;
}

#include "MainFrm.h"
EnHandleResult CMyServer::OnClose(CONNID dwConnID)
{
	//::PostOnClose(dwConnID);
	TCHAR szAddress[40];
	int iAddressLen = sizeof(szAddress) / sizeof(TCHAR);
	USHORT usPort;
	m_Server.GetRemoteAddress(dwConnID, szAddress, iAddressLen, usPort);
	OP_LOG_INFO("dwConnID:%d szAddress:%s usPort:%d 离线",dwConnID,szAddress,usPort);

	CMainFrame* pMainFrame = (CMainFrame*)AfxGetApp()->m_pMainWnd;
	InterlockedExchange(&pMainFrame->m_bOnLineOrOffLine,TRUE);
	g_MongoDBMgr.UserLoginIn((DWORD)dwConnID,"",FALSE);
	return HR_OK;
}

EnHandleResult CMyServer::OnError(CONNID dwConnID, EnSocketOperation enOperation, int iErrorCode)
{
	//::PostOnError(dwConnID, enOperation, iErrorCode);
	CMainFrame* pMainFrame = (CMainFrame*)AfxGetApp()->m_pMainWnd;
	InterlockedExchange(&pMainFrame->m_bOnLineOrOffLine,TRUE);
	g_MongoDBMgr.UserLoginIn((DWORD)dwConnID,"",FALSE);
	return HR_OK;
}

EnHandleResult CMyServer::OnServerShutdown()
{
	::PostOnShutdown();
	return HR_OK;
}

PPACKINFO CMyServer::GetPackInfo()
{
	return &m_PackInfo;
}