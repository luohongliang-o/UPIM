#pragma once

#include "struct.h"
// 消息大型包接收
struct MSG_PKG_INFO2
{
    char guid[GUID_SIZE];	// 包的唯一标识
	char filename[48];		// 文件名
	int	originallen;		// 原始长度
	char tqid[USERID_SIZE];	// 包的接收方,如果是群组，则是发送方ID
	int	totallength;		// 包的总长度
	int length_recv;		// 已经接收到的长度
	char* data;				// 包的内容
};

class CClientPackage  
{
public:
	CClientPackage();
	virtual ~CClientPackage();
	int SaveToBuffer(char *p);
	LPSTR GetPkBuffer(DWORD& dwSize);

	void Cleanup();
	void Copy(CClientPackage &package);
	void SetContent(void *pContent,long len);
	void SetHeader(PACKAGE_HEADER *pHeader);
private:
	DWORD m_nLen;		// 包头与内容共有多长
	PACKAGE_HEADER m_header;	// 包头
	char *m_pBuf;				// 包含包体的缓冲区
};

class CPackageHandle
{
public:
	CPackageHandle(CClientPackage* pKg){m_pkg=pKg;}
	~CPackageHandle(){TDEL(m_pkg);}
protected:
	CClientPackage* m_pkg;
};


class CPackageRecvManage  
{
public:
	CPackageRecvManage();
	virtual ~CPackageRecvManage();

	void HandleMsgPkg(LPCUSERID tqid, MSG_PKG_TRANSFER_REQ *preq);
	void HandleGroupMsgPkg(GROUP_CHAT_CLIENT_RECV_PKG_ANS* pAns);

	void SendAnsMsgPkg(MSG_PKG_INFO2 *pinfo);
	void SendAnsMsgPkg(LPCUSERID tqid, DWORD dwSeq);
	
	void SendAnsMsg(LPRECVMSG2 pRecvMsg2);
protected:
	void ProcessMsgPkg(MSG_PKG_INFO2 *pinfo);
	void DeleteMsgPkg(char* guid, char* filename);
	void DeleteAllMsgPkg();

protected:
	std::vector<MSG_PKG_INFO2> m_vtMsgPkg;
};
