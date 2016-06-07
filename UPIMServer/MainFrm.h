
// MainFrm.h : CMainFrame 类的接口
//

#pragma once
#include "OutputWnd.h"
#include "LinkManager.h"
#include "GroupChatManage.h"
#include "PublicManage.h"
#include "CustSvrMng.h"
#include "MongoDBMgr.h"
#include <vector>
#include "FXSHDManage.h"

extern CLinkManager		g_LinkMangaer;
extern CGroupChatManage g_GroupChatMng;
extern CPublicManage	g_PublicMng;
extern CFXSHDManage     g_FxsHDManage;
extern CCustSvrMng		g_custSvrMng;
extern CMongoDBMgr		g_MongoDBMgr;
extern LONG g_MainFrmThreadState;

#define LIST_USERINFO_TIMER 1
class CMainFrame : public CFrameWndEx
{

protected: // 仅从序列化创建
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// 特性
private:
	
	static PackInfo m_curPackInfo;
	HANDLE m_StatThread;
	HANDLE m_HttpThread;
	
// 操作
public:
	static std::vector<int>    m_statList[5];
	HANDLE m_StartEvent;
	volatile static LONG m_bOnLineOrOffLine;
	volatile static LONG m_bStart;
// 重写
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// 实现
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // 控件条嵌入成员
	CMFCMenuBar       m_wndMenuBar;
	CMFCToolBar       m_wndToolBar;
	CMFCStatusBar     m_wndStatusBar;
	COutputWnd        m_wndOutput;
	int               m_iStart;// 0开始 1 停止
	int               m_iStop;
	CMenu             m_sysMenu[3];

public:
	BOOL InitInst();
	BOOL ExitInst();
	VOID SendSvrOffMSG();

	CFXSHDManage     m_FxsHDManage;
// 生成的消息映射函数
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnViewCustomize();
	afx_msg LRESULT OnToolbarCreateNew(WPARAM wp, LPARAM lp);
	afx_msg void OnApplicationLook(UINT id);
	afx_msg void OnUpdateApplicationLook(CCmdUI* pCmdUI);
	afx_msg void OnSettingChange(UINT uFlags, LPCTSTR lpszSection);
	afx_msg void OnStart();
	afx_msg void OnStop();
	afx_msg void OnSet();
	afx_msg void OnHelp();
	afx_msg void OnUpdateToolBar(CCmdUI* pCmdUI);
	afx_msg LRESULT OnUserInfoMsg(WPARAM wp, LPARAM lp);
	afx_msg void OnClose();
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	DECLARE_MESSAGE_MAP()

	BOOL CreateDockingWindows();
	void SetDockingWindowIcons(BOOL bHiColorIcons);
	static int StatPeakData(std::vector<int> &dList,int data);
private:
	static UINT WINAPI StatSever(LPVOID pv);
	

public:
	afx_msg void OnUpdateMenuState(CCmdUI *pCmdUI);
	afx_msg void OnMenuStart();
	afx_msg void OnMenuStop();
};


