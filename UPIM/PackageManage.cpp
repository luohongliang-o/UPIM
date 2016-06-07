#include "StdAfx.h"
#include "PackageManage.h"
#include "Func.h"

CPackageManage::CPackageManage(void)
{
}

CPackageManage::~CPackageManage(void)
{
	DeleteAllMsgPkg();
}

// pData:COMPLEX_MSG_DATA*
void CPackageManage::CreateGroupMsg(int groupid, BYTE grouptype, LPVOID pData)
{
	MSG_PKG_INFO info = {0};
	info.dwMsgSeq = InterlockedIncrement(&g_nMsgSeq);
	char guid[GUID_SIZE] = {0};
	CreateGUID(guid);
	memcpy(info.guid, guid, GUID_SIZE);
	info.nRoomID = groupid;
	info.ucRoomType = grouptype;
	info.nSendCount = 0;
	info.ucFormat = eMsgFormat_Def;
	info.lpData = pData;
	m_vtMsgPkg.push_back(info);
	SendGroup(guid);
}

void CPackageManage::SendGroup(char *guid)
{
	for(int i=0; i<(int)m_vtMsgPkg.size(); i++)
	{
		if(memcmp(m_vtMsgPkg[i].guid, guid, GUID_SIZE)==0)
		{
			COMPLEX_MSG_DATA* pData = (COMPLEX_MSG_DATA*)m_vtMsgPkg[i].lpData;
			if(pData->sSend == -1)
			{
				char buf[MAX_SEND_BUF_LEN] = {0};
				gcsendmsg_req *preq=(gcsendmsg_req*)buf;
				preq->dwMsgSeq = m_vtMsgPkg[i].dwMsgSeq;
				preq->roomid = m_vtMsgPkg[i].nRoomID;
				preq->roomtype = m_vtMsgPkg[i].ucRoomType;
				m_vtMsgPkg[i].tmSend = time(NULL);
				preq->tmSend = m_vtMsgPkg[i].tmSend;
				preq->format = m_vtMsgPkg[i].ucFormat;
				preq->msglen = pData->nTextLen;
				if(preq->msglen>0 && preq->msglen<MAX_TEXT_LEN)
				{
					strncpy(preq->msg, pData->pBuf, preq->msglen);
					preq->msg[preq->msglen] = 0;
				}
				else
				{
					preq->msglen = 0;
					preq->msg[0] = 0;
				}
				m_vtMsgPkg[i].nSendCount++;
				g_MyClient.SendGroupMsgReq(preq);
				return;
			}
		}
	}
}

// pData:COMPLEX_MSG_DATA*
void CPackageManage::CreateMsg(LPCTSTR userid, LPVOID pData, BYTE ucFormat/* =eMsgFormat_Def */, BYTE ucType/* =eMsgSend_Normal */, LPCTSTR pszInnerID/* =_T("") */, int nPublicID/* =-1 */, int nRouteID/* =-1 */)
{
	MSG_PKG_INFO info = {0};
	info.dwMsgSeq = InterlockedIncrement(&g_nMsgSeq);;
	char guid[GUID_SIZE] = {0};
 	CreateGUID(guid);
 	memcpy(info.guid, guid, GUID_SIZE);
 	COPY_USERID(info.userid, userid);
	info.ucRoomType = eCR_None;
	info.ucMsgType = ucType;
	COPY_USERID(info.szInnerID, pszInnerID);
	info.nPublicID = nPublicID;
	info.nRouteID = nRouteID;
	info.nSendCount = 0;
	info.ucFormat = ucFormat;
	info.lpData = pData;
	m_vtMsgPkg.push_back(info);

	SendMsgPkg(guid);
}

void CPackageManage::SendMsgPkg(char *guid)
{
	for(int i=0; i < (int)m_vtMsgPkg.size(); i++)
	{
		if(memcmp(m_vtMsgPkg[i].guid, guid, GUID_SIZE)==0)
		{
			COMPLEX_MSG_DATA* pData = (COMPLEX_MSG_DATA*)m_vtMsgPkg[i].lpData;
			if (pData->sSend == -1)
			{
				// 发送文本
 				char buf[MAX_SEND_BUF_LEN] = {0};
 				sendmsg_req *preq = (sendmsg_req *)buf;
 				preq->dwMsgSeq = m_vtMsgPkg[i].dwMsgSeq;
 				COPY_USERID(preq->cTouserid, m_vtMsgPkg[i].userid);
				COPY_USERID(preq->innerID, m_vtMsgPkg[i].szInnerID);
 				m_vtMsgPkg[i].tmSend = time(NULL);
 				preq->tmSend = m_vtMsgPkg[i].tmSend;
 				preq->format = m_vtMsgPkg[i].ucFormat;
 				preq->ucMsgType = m_vtMsgPkg[i].ucMsgType;
				preq->nPublicID = m_vtMsgPkg[i].nPublicID;
				preq->nRouteID = m_vtMsgPkg[i].nRouteID;
 				preq->msglen = pData->nTextLen;
 				if(preq->msglen>0 && preq->msglen<MAX_TEXT_LEN)
 				{
 					strncpy(preq->msg, pData->pBuf, preq->msglen);
 					preq->msg[preq->msglen] = 0;
 				}
 				else 
 				{
 					preq->msglen = 0;
 					preq->msg[0] = 0;
 				}
 				m_vtMsgPkg[i].nSendCount++;
				g_MyClient.SendMsgReq(preq);
				return;
			}

			short sSend = GetFreePicInfo(pData);
			if(sSend<0) return;
			PIC_OBJECT_INFO* pObjInfo = (PIC_OBJECT_INFO*)(&pData->pObjectInfo[sSend]);

			CClientPackage *pkg;

			pkg = CreatePackage(UPIM_MSG_PKG_TRANSFER);
			MSG_PKG_TRANSFER_REQ *p = (MSG_PKG_TRANSFER_REQ*)g_chBuffer;
			memcpy(p->guid, guid, GUID_SIZE);			
			p->sSend = sSend;
			strcpy(p->filename, pObjInfo->szFile);
			p->request = true;
			p->totallength = pObjInfo->nTotalLen;
			if(pObjInfo->nTotalLen-pObjInfo->nLen > DATA_TRANSFER_UNIT)
				p->length = DATA_TRANSFER_UNIT;
			else
				p->length = pObjInfo->nTotalLen - pObjInfo->nLen;
			memcpy(p->data, pObjInfo->pBuf + pObjInfo->nLen, p->length);
			pkg->SetContent(p, sizeof(MSG_PKG_TRANSFER_REQ) + p->length - 1);
			pObjInfo->nLen += p->length;
			m_vtMsgPkg[i].tmSend = time(NULL);
			g_MyClient.SendPkgReq(m_vtMsgPkg[i].userid, true, pkg);
			break;

		}
	}
}

void CPackageManage::DeleteMsgPkg(char *guid)
{
	for(int i=0; i<(int)m_vtMsgPkg.size(); i++)
	{
		if(memcmp(m_vtMsgPkg[i].guid, guid, GUID_SIZE)==0)
		{
			COMPLEX_MSG_DATA* pData = (COMPLEX_MSG_DATA*)m_vtMsgPkg[i].lpData;
			DeleteMsgData(pData);
			m_vtMsgPkg.erase(m_vtMsgPkg.begin()+i);
			break;
		}
	}
}

void CPackageManage::DeleteMsgPkg(DWORD dwSeq)
{
	for(int i=0; i< (int)m_vtMsgPkg.size(); i++)
	{
		if(dwSeq == m_vtMsgPkg[i].dwMsgSeq)
		{
			COMPLEX_MSG_DATA* pData = (COMPLEX_MSG_DATA*)m_vtMsgPkg[i].lpData;
			DeleteMsgData(pData);
			m_vtMsgPkg.erase(m_vtMsgPkg.begin()+i);
			break;
		}
	}
}

void CPackageManage::DeleteAllMsgPkg()
{
	for(int i=0; i < (int)m_vtMsgPkg.size(); i++)
	{
		COMPLEX_MSG_DATA* pData = (COMPLEX_MSG_DATA*)m_vtMsgPkg[i].lpData;
		DeleteMsgData(pData);
	}
	m_vtMsgPkg.clear();
}


void CPackageManage::FeedBackMsgPkg(DWORD dwSeq, char* guid)
{
	for(int i=0; i<(int)m_vtMsgPkg.size(); i++)
	{
		if(dwSeq > 0)
		{
			if(m_vtMsgPkg[i].dwMsgSeq != dwSeq) 
				continue;
		}
		else
		{
			if(guid == NULL) 
				continue;
			if(memcmp(m_vtMsgPkg[i].guid, guid, GUID_SIZE) != 0) 
				continue;
		}
		COMPLEX_MSG_DATA* pMsgData = (COMPLEX_MSG_DATA* )m_vtMsgPkg[i].lpData;
		short sSend = GetFreePicInfo(pMsgData);
		if(sSend<0) 
		{
			DeleteMsgPkg(m_vtMsgPkg[i].guid);
			return;
		}
		if(eCR_None == m_vtMsgPkg[i].ucRoomType)
			SendMsgPkg(m_vtMsgPkg[i].guid);
		else 
			SendGroup(m_vtMsgPkg[i].guid);
		return ;
	}
}

void CPackageManage::ProcessMsgAns(DWORD dwSeq, bool bOnline, long nError)
{
	LPMSG_PKG_INFO lpMsgInfo = NULL;
	int nIdx = 0;
	for(nIdx=0; nIdx<(int)m_vtMsgPkg.size(); nIdx++)
	{
		if(m_vtMsgPkg[nIdx].dwMsgSeq == dwSeq)
		{
			lpMsgInfo = &m_vtMsgPkg[nIdx];
			break;
		}
	}
	if(nIdx >= (int)m_vtMsgPkg.size() || !lpMsgInfo) 
		return;
	COMPLEX_MSG_DATA* pMsgData = (COMPLEX_MSG_DATA* )lpMsgInfo->lpData;
	/*
	if(nError!=0)	// 出错
	{
		DeleteMsgPkg(dwSeq);
		return;
	}*/
	if(eCR_None == lpMsgInfo->ucRoomType)
	{
		int nType = lpMsgInfo->ucMsgType;
		int nRoomID = 0;
		int nRecordType = eRecord_Single;
		if (nType == eMsgSend_Public)
			nRecordType = eRecord_Public;
		if (nType == eMsgSend_Analyst)
		{
			nRecordType = eRecord_TgRoom;
			nRoomID = atoi(lpMsgInfo->szInnerID);
		}
		g_SqliteOp.DBInsertRecord(g_MyClient.m_strUserid, lpMsgInfo->userid, nRoomID, (long)lpMsgInfo->tmSend, nRecordType, lpMsgInfo->ucFormat, pMsgData->pBuf);
		if(!bOnline)
		{
			DeleteMsgData(pMsgData);
			m_vtMsgPkg.erase(m_vtMsgPkg.begin() + nIdx);
			return;
		}
	}
	else
	{
		g_SqliteOp.DBInsertGRecord(g_MyClient.m_strUserid, lpMsgInfo->nRoomID, (long)lpMsgInfo->tmSend, pMsgData->pBuf);
	}
}

short CPackageManage::GetFreePicInfo(LPVOID pData)
{
//	if(g_clientConfig.m_bSendOffImage) return -1;
	if(g_userconfig.m_bSendOffImage) return -1;
	COMPLEX_MSG_DATA* pMsgData = (COMPLEX_MSG_DATA* )pData;
	while (!IsEndMsgPkg(pData))
	{
		if(VerifyMsgPkg(pData)) 
		{
			return pMsgData->sSend;
		}
		pMsgData->sSend++;
	}
	return -1;
}

BOOL CPackageManage::VerifyMsgPkg(LPVOID pData)
{
	COMPLEX_MSG_DATA* pMsgData = (COMPLEX_MSG_DATA* )pData;
	if(pMsgData->sSend >= pMsgData->nObjCount) 
		return FALSE;
	if(pMsgData->sSend < 0) 
		return FALSE;
	PIC_OBJECT_INFO* pObjInfo = &(pMsgData->pObjectInfo[pMsgData->sSend]);
	if(pObjInfo->nType == eImgType_SysFace) return FALSE;
	if(pObjInfo->nTotalLen <= 0)
	{
		CFile theFile;
		CString strFile;
		strFile.Format("%s\\%s",g_config.szUserImage, pObjInfo->szFile);
		if(!theFile.Open(strFile,CFile::modeRead|CFile::shareDenyNone))
		{
			return FALSE;
		}
		int nLen = theFile.GetLength();
		pObjInfo->pBuf = new char[nLen+1];
		memset(pObjInfo->pBuf,0,nLen+1);
		theFile.Read(pObjInfo->pBuf,nLen);
		theFile.Close();
		pObjInfo->nTotalLen = nLen;
		return TRUE;
	}
	if(pObjInfo->nLen<pObjInfo->nTotalLen) return TRUE;
	return FALSE;
}

BOOL CPackageManage::IsEndMsgPkg(LPVOID pData)
{
	COMPLEX_MSG_DATA* pMsgData = (COMPLEX_MSG_DATA* )pData;
	if(pMsgData->sSend>=pMsgData->nObjCount) 
		return TRUE;
	return FALSE;
}

void CPackageManage::DeleteMsgData(COMPLEX_MSG_DATA*& pMsgData)
{
	if(!pMsgData) return;
	if(pMsgData->pObjectInfo)
	{
		for(int j=0; j<pMsgData->nObjCount; j++)
			TDEL(pMsgData->pObjectInfo[j].pBuf);
		TDELARRAY(pMsgData->pObjectInfo);
	}
	TDEL(pMsgData->pBuf);
	TDEL(pMsgData);
}

BOOL CPackageManage::SendOffImage(LPCUSERID tqid,LPVOID pData,BYTE roomType)
{
	if(pData==NULL) return FALSE;
	COMPLEX_MSG_DATA* pMsgData = (COMPLEX_MSG_DATA* )pData;
	if(pMsgData->pObjectInfo==NULL) return FALSE;
	for (int i=0;i<pMsgData->nObjCount;i++)
	{
		if (pMsgData->pObjectInfo[i].nType==eImgType_SysFace) continue;
		//g_MyClient->SendOffImage(tqid,pMsgData->pObjectInfo[i].szFile,roomType);
	}
	return TRUE;
}
