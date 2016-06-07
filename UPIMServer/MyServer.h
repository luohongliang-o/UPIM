#pragma once
#include "../Socket/HP-Socket/Src/TcpServer.h"
#include "../Socket/HP-Socket/Global/helper.h"

typedef struct PackInfo
{
	long SendDataLen;
	long SendPackLen;
	long RecieveDataLen;
	long RecievePackLen;
	PackInfo(long sdlen,long splen,long rdlen,long rplen)
	{
		SendDataLen = sdlen;
		SendPackLen = splen;
		RecieveDataLen = rdlen;
		RecievePackLen = rplen;
	}
	PackInfo operator -(PackInfo& packinfo)
	{
		packinfo.SendDataLen = SendDataLen - packinfo.SendDataLen;
		packinfo.SendPackLen = SendPackLen - packinfo.SendPackLen;
		packinfo.RecieveDataLen = RecieveDataLen - packinfo.RecieveDataLen;
		packinfo.RecievePackLen = RecievePackLen - packinfo.RecievePackLen;
		return packinfo;
	}
}*PPACKINFO;

class CMyServer:public CTcpServerListener
{
public:
	CMyServer(void);
	~CMyServer(void);

private:
	virtual EnHandleResult OnPrepareListen(SOCKET soListen);
	virtual EnHandleResult OnAccept(CONNID dwConnID, SOCKET soClient);
	virtual EnHandleResult OnSend(CONNID dwConnID, const BYTE* pData, int iLength);
	virtual EnHandleResult OnReceive(CONNID dwConnID, const BYTE* pData, int iLength);
	virtual EnHandleResult OnClose(CONNID dwConnID);
	virtual EnHandleResult OnError(CONNID dwConnID, EnSocketOperation enOperation, int iErrorCode);
	virtual EnHandleResult OnServerShutdown();
public:
	PPACKINFO GetPackInfo();
	LPPKGRECVINFO GetPkgInfoFromMap(LONG mConID);
	BOOL DelPkgInfoFromMap(LONG mConID);
	BOOL Disconnect(CONNID dwConnID, BOOL bForce = TRUE);
public:
	CString m_strAddress;
	CTcpServer m_Server;
private:
	PackInfo m_PackInfo;
	CCriSec	 m_csPkgMapItem;
};

