#pragma once
#include "struct.h"

class CPackageManage
{
public:
	CPackageManage(void);
	virtual ~CPackageManage(void);

	// 群组消息
	// groupid:群组ID，grouptype：群组类型
	// pData:COMPLEX_MSG_DATA*
	void CreateGroupMsg(int groupid, BYTE grouptype, LPVOID pData);

	// 个人消息
	// userid:接收者ID
	// pData:COMPLEX_MSG_DATA*
	// ucFormat:消息格式
	// ucType:消息发送类型 E_MSGSEND_TYPE
	// pszInnerID:内部ID
	void CreateMsg(LPCTSTR userid, LPVOID pData, BYTE ucFormat=eMsgFormat_Def, BYTE ucType=eMsgSend_Normal, LPCTSTR pszInnerID=_T(""), int nPublicID=-1, int nRouteID=-1);

	void FeedBackMsgPkg(DWORD dwSeq, char* guid=NULL);

	// 处理服务器应答
	void ProcessMsgAns(DWORD dwSeq, bool bOnline, long nError=0);

	BOOL CheckMsgTimeout();

	void SetAgengID(LPCTSTR pszAgentID);

protected:
	void SendGroup(char *guid);
	void SendMsgPkg(char *guid);

	void DeleteMsgPkg(char *guid);
	void DeleteMsgPkg(DWORD dwSeq);
	void DeleteAllMsgPkg();
	void DeleteMsgData(COMPLEX_MSG_DATA*& pMsgData);

	short GetFreePicInfo(LPVOID pData);
	BOOL VerifyMsgPkg(LPVOID pData);
	BOOL IsEndMsgPkg(LPVOID pData);

	BOOL SendOffImage(LPCUSERID userid, LPVOID pData, BYTE roomType=0);

protected:
	std::vector<MSG_PKG_INFO> m_vtMsgPkg;
};

