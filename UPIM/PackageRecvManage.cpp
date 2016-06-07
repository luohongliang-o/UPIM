#include "stdafx.h"
#include "PackageRecvManage.h"

CPackageRecvManage::CPackageRecvManage()
{
}

CPackageRecvManage::~CPackageRecvManage()
{
   DeleteAllMsgPkg();
}

void CPackageRecvManage::HandleMsgPkg(LPCUSERID tqid,MSG_PKG_TRANSFER_REQ *preq)
{
	int i = 0;
	for(i=0;i<(int)m_vtMsgPkg.size();i++)
	{
		if(memcmp(m_vtMsgPkg[i].guid,preq->guid,GUID_SIZE)==0 && stricmp(m_vtMsgPkg[i].filename,preq->filename)==0)
		{
			if(m_vtMsgPkg[i].length_recv+preq->length>m_vtMsgPkg[i].totallength)
			{
				// ³ö´í
				DeleteMsgPkg(preq->guid,preq->filename);
				return;
			}
			memcpy(m_vtMsgPkg[i].data+m_vtMsgPkg[i].length_recv,preq->data,preq->length);
			m_vtMsgPkg[i].length_recv+=preq->length;
			SendAnsMsgPkg(&m_vtMsgPkg[i]);
			if(m_vtMsgPkg[i].length_recv>=m_vtMsgPkg[i].totallength)
			{
				ProcessMsgPkg(&m_vtMsgPkg[i]);
				DeleteMsgPkg(preq->guid,preq->filename);
			}
			break;
		}
	}
	
	if(i==m_vtMsgPkg.size())
	{
		if(preq->length>preq->totallength)
		{
			// ³ö´í
			DeleteMsgPkg(preq->guid,preq->filename);
			return;
		}
		MSG_PKG_INFO2 info = {0};
		info.data = new char[preq->totallength];
		memcpy(info.guid,preq->guid,GUID_SIZE);
		COPY_USERID(info.tqid,tqid);
		info.length_recv = preq->length;
		info.totallength = preq->totallength;
		memcpy(info.data,preq->data,preq->length);
		COPYSTRARRAY(info.filename,preq->filename);
		SendAnsMsgPkg(&info);
		if(info.length_recv >= info.totallength)
		{
			ProcessMsgPkg(&info);
			TDEL(info.data);
		}
		else
		{
			m_vtMsgPkg.push_back(info);
		}
	}
}

void CPackageRecvManage::DeleteMsgPkg(char* guid,char* filename)
{
	for(int i=0;i<(int)m_vtMsgPkg.size();i++)
	{
		if(memcmp(m_vtMsgPkg[i].guid,guid,GUID_SIZE)==0 && stricmp(m_vtMsgPkg[i].filename,filename)==0)
		{
			TDELARRAY(m_vtMsgPkg[i].data);
			m_vtMsgPkg.erase(m_vtMsgPkg.begin()+i);
			break;
		}
	}
}

void CPackageRecvManage::DeleteAllMsgPkg()
{
	for(int i=0;i<(int)m_vtMsgPkg.size();i++)
	{
		TDELARRAY(m_vtMsgPkg[i].data);
	}
	m_vtMsgPkg.clear();
}

void CPackageRecvManage::SendAnsMsgPkg(MSG_PKG_INFO2 *pinfo)
{
	CClientPackage *pkg;
	pkg = CreatePackage(UPIM_MSG_PKG_TRANSFER);
	MSG_PKG_TRANSFER_ANS *p=(MSG_PKG_TRANSFER_ANS*)g_chBuffer;
    memcpy(p->guid,pinfo->guid,GUID_SIZE);
	p->dwSeq = 0;
	strcpy(p->filename,pinfo->filename);
	p->length = pinfo->length_recv;
 	pkg->SetContent(p,sizeof(MSG_PKG_TRANSFER_ANS));

	g_MyClient.SendPkgReq(pinfo->tqid, false, pkg);
}

void CPackageRecvManage::SendAnsMsgPkg(LPCUSERID tqid,DWORD dwSeq)
{
	if(dwSeq==0) return;
	CClientPackage *pkg;
	pkg = CreatePackage(UPIM_MSG_PKG_TRANSFER);
	MSG_PKG_TRANSFER_ANS p = {0};
	memset(p.guid,0,GUID_SIZE);
	p.dwSeq = dwSeq;
 	pkg->SetContent(&p,sizeof(MSG_PKG_TRANSFER_ANS));

	g_MyClient.SendPkgReq(tqid, false, pkg);
}

void CPackageRecvManage::SendAnsMsg(LPRECVMSG2 pRecvMsg2)
{
	if(pRecvMsg2==NULL) return;
	SendAnsMsgPkg(pRecvMsg2->m_szUSERID, pRecvMsg2->m_dwSeq);
}

void CPackageRecvManage::ProcessMsgPkg(MSG_PKG_INFO2 *pinfo)
{
	char szFile[MAX_PATH] = {0};
	sprintf(szFile,"%s\\%s",g_config.szUserImage,pinfo->filename);
	CFile theFile;
	if(!theFile.Open(szFile,CFile::modeWrite|CFile::typeBinary|CFile::modeCreate) ) return;
	theFile.Write(pinfo->data,pinfo->totallength);
	theFile.Close();
}

void CPackageRecvManage::HandleGroupMsgPkg(GROUP_CHAT_CLIENT_RECV_PKG_ANS* pAns)
{
	int i = 0;
	for(i=0;i<(int)m_vtMsgPkg.size();i++)
	{
		if(memcmp(m_vtMsgPkg[i].guid,pAns->guid,GUID_SIZE)==0 && stricmp(m_vtMsgPkg[i].filename,pAns->filename)==0)
		{
			if(pAns->pos>(DWORD)m_vtMsgPkg[i].totallength)
			{
				DeleteMsgPkg(pAns->guid,pAns->filename);
				return;
			}
			memcpy(m_vtMsgPkg[i].data+pAns->pos,pAns->msg,pAns->msglen);
			m_vtMsgPkg[i].length_recv += pAns->msglen;
			if(m_vtMsgPkg[i].length_recv>=m_vtMsgPkg[i].totallength)
			{
				ProcessMsgPkg(&m_vtMsgPkg[i]);
				DeleteMsgPkg(pAns->guid,pAns->filename);
			}
			break;
		}
	}
	if(i==m_vtMsgPkg.size())
	{
		if(pAns->pos > (DWORD)pAns->totallen) return;
		if(pAns->msglen>pAns->totallen) return;
		MSG_PKG_INFO2 info = {0};
		info.data = new char[pAns->totallen];
		memcpy(info.guid,pAns->guid,GUID_SIZE);
		COPY_USERID(info.tqid,pAns->tqid);
		info.length_recv = pAns->msglen;
		info.totallength = pAns->totallen;
		memcpy(info.data+pAns->pos,pAns->msg,pAns->msglen);
		COPYSTRARRAY(info.filename,pAns->filename);
		if(info.length_recv >= info.totallength)
		{
			ProcessMsgPkg(&info);
			TDEL(info.data);
		}
		else
		{
			m_vtMsgPkg.push_back(info);
		}
	}
}


CClientPackage::CClientPackage()
{
	m_nLen = 0;
	m_pBuf = NULL;
}

void CClientPackage::Copy(CClientPackage &package)
{
	Cleanup();
	SetHeader(&package.m_header);
	SetContent(package.m_pBuf,package.m_nLen);
}

CClientPackage::~CClientPackage()
{
	Cleanup();
}

void CClientPackage::SetHeader(PACKAGE_HEADER *pHeader)
{
	memcpy(&m_header,pHeader,sizeof(m_header));
}

void CClientPackage::SetContent(void *pContent, long len)
{
	if(!pContent || len<=0) return;
	TDEL(m_pBuf);

	m_header.packagelen = len;
	m_nLen = len+sizeof(PACKAGE_HEADER);
	m_pBuf = new char[m_nLen];
	memcpy(m_pBuf,&m_header,sizeof(PACKAGE_HEADER));
	memcpy(m_pBuf+sizeof(PACKAGE_HEADER),pContent,len);
}

void CClientPackage::Cleanup()
{
	TDEL(m_pBuf);
	m_nLen=0;
}

int CClientPackage::SaveToBuffer(char *p)
{
	memcpy(p,&m_header,sizeof(m_header));
	if(m_nLen>0)
		memcpy(p+sizeof(m_header),m_pBuf,m_nLen);
	int len=m_nLen+sizeof(PACKAGE_HEADER);
	return len;
}

LPSTR CClientPackage::GetPkBuffer(DWORD& dwSize)
{
	dwSize = m_nLen;
	return m_pBuf;
}
