#pragma once
#include "struct.h"
class CMyClient;

typedef struct UPIMID
{
	char	m_szUSERID[USERID_SIZE];

	UPIMID()
	{
		memset(m_szUSERID, 0, sizeof(m_szUSERID));
	}	
	UPIMID(LPCUSERID userid)
	{
		strncpy(m_szUSERID, userid, USERID_SIZE);
		m_szUSERID[USERID_SIZE-1] = 0;
	}
	UPIMID(const UPIMID& userid)
	{
		strncpy(m_szUSERID, userid.m_szUSERID, USERID_SIZE);
		m_szUSERID[USERID_SIZE-1] = 0;
	}

	const UPIMID& operator=(LPCUSERID userid)
	{
		strncpy(m_szUSERID,userid, USERID_SIZE);
		m_szUSERID[USERID_SIZE-1] = 0;
		return *this;
	}
	const UPIMID& operator=(const UPIMID& upid)
	{
		strncpy(m_szUSERID, upid.m_szUSERID, USERID_SIZE);
		m_szUSERID[USERID_SIZE-1] = 0;
		return *this;
	}
	operator LPCUSERID(){return m_szUSERID;}
	bool operator==(const UPIMID& t2)
	{
		return strcmp(m_szUSERID, t2.m_szUSERID)==0;
	}
	bool operator==(LPCUSERID t2)
	{
		return strcmp(m_szUSERID, t2)==0;
	}
}*LPUPIMID;

typedef struct tagMSGQUEUE
{
	int		nFormat;
	int		nLen;
	BYTE	ucType;			// 0,ÏûÏ¢,1£º¹ã²¥
	HWND	hWnd;
	char*	pBuf;
	vector<UPIMID>		vtId;
	COMPLEX_MSG_DATA*	pData;
	tagMSGQUEUE()
	{
		nFormat = eMsgFormat_Txt;
		nLen = 0;
		ucType = 0;
		pBuf = NULL;
		pData = NULL;
		vtId.clear();
	}
	~tagMSGQUEUE()
	{
		TDEL(pBuf);
		TDEL(pData);
		vtId.clear();
	}
}MSGQUEUE,*LPMSGQUEUE;

class CMsgQueueSendMng  
{
public:
	CMsgQueueSendMng();
	virtual ~CMsgQueueSendMng();
	void Init(CMyClient* pMyClient);
	void Exit();
	void PushMsg(LPCTSTR pszMsg, int format, std::vector<UPIMID>& vtId, HWND hWnd=NULL);
	void PushMsg(COMPLEX_MSG_DATA* pData, int format, std::vector<UPIMID>& vtId, HWND hWnd=NULL);
	void PushBroadCast(LPCTSTR pszMsg, std::vector<UPIMID>& vtId, HWND hWnd=NULL);
	BOOL PopMsg(LPMSGQUEUE* ppMsgQueue);

protected:
	void DeleteAll();
	
public:
	BOOL	m_bInit;
	CMyClient* m_pMyClient;
	HANDLE	m_hExitEvent;
	HANDLE	m_hStartEvent;
protected:
	vector<LPMSGQUEUE>	m_vtMsgQueue;
	CCriticalSection	m_cs;
	HANDLE	m_hThread;
};

