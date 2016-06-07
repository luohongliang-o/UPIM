/*
* 数据库连接池
* Opander
* 2014年6月10日16:52:55
*/
#if !defined(__LINKMANAGER_H__)
#define __LINKMANAGER_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "globalvar.h"

class CLinkManager  
{
public:
	CLinkManager();
	virtual ~CLinkManager();

public:
	BOOL		Init(); // 初始化
	BOOL		Exit(); // 退出
	LPOPLINK	GetLink(char *lpszErrMsg, int nSize, BOOL bPrimary=FALSE);// 取得连接(事物处理)(返回NULL为失败,相应错误信息保存在lpszErrMsg中)
	LPOPLINK	GetHisMsgLink();					// 历史消息入库连接
	LPOPLINK	GetOffMsgLink();					// 离线消息入库连接
	void		FreeLink( LPOPLINK pLink);			// 释放连接
	void		DisConnect( LPOPLINK pLink);		// 断开连接
	void		CheckConnect(LPOPLINK pLink);

protected:
	BOOL		_Init(LPOPLINK pLinkInfo);						// 初始化连接
	BOOL		_Connect(LPOPLINK pLink, BOOL bFailLog=TRUE);	// 优先连接默认数据库,再连接其他数据库
	BOOL		_Connect2(LPOPLINK pLink, BOOL bFailLog=TRUE);	// 依次连接各个数据库

protected:
	short			m_nDbCount;					// 数据库个数
	short			m_nDefaultSel;				// 默认数据库
	DBCONFIG		m_dbConfig[MAX_DBCOUNT];

	OPLINK			m_aLinkInfo[MAX_LINK_NUM];	// 数据库连接池
	OPLINK			m_linkHisMsg;				// 历史消息入库连接,消息入库线程调用
	OPLINK			m_linkOffMsg;				// 离线消息,离线消息线程调用

	long				m_nFailCount;
	CCriticalSection	m_csLink;
};

#endif // !defined(__LINKMANAGER_H__)
