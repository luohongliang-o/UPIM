// MainFrm.cpp : CMainFrame 类的实现
//

#include "stdafx.h"
#include "UPIMServer.h"
#include "MainFrm.h"
#include "Func.h"
#include "UPIMServerDoc.h"
#include "UPIMServerView.h"
#include "ThreadFunc.h"

// CMainFrame
CLinkManager		g_LinkMangaer;
CGroupChatManage	g_GroupChatMng;
CPublicManage		g_PublicMng;
CCustSvrMng			g_custSvrMng;
CMongoDBMgr			g_MongoDBMgr;
CFXSHDManage        g_FxsHDManage;

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWndEx)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWndEx)
	ON_WM_CREATE()
	ON_COMMAND(ID_VIEW_CUSTOMIZE, &CMainFrame::OnViewCustomize)
	ON_REGISTERED_MESSAGE(AFX_WM_CREATETOOLBAR, &CMainFrame::OnToolbarCreateNew)
	ON_COMMAND_RANGE(ID_VIEW_APPLOOK_WIN_2000, ID_VIEW_APPLOOK_WINDOWS_7, &CMainFrame::OnApplicationLook)
	ON_UPDATE_COMMAND_UI_RANGE(ID_VIEW_APPLOOK_WIN_2000, ID_VIEW_APPLOOK_WINDOWS_7, &CMainFrame::OnUpdateApplicationLook)
	ON_UPDATE_COMMAND_UI_RANGE(ID_SERVER_START,ID_SERVER_STOP,OnUpdateToolBar)
	ON_COMMAND(ID_SERVER_START,&CMainFrame::OnStart)
	ON_COMMAND(ID_SERVER_STOP,&CMainFrame::OnStop)
	ON_COMMAND(ID_SERVER_SET,&CMainFrame::OnSet)
	ON_COMMAND(ID_SERVER_HELP,&CMainFrame::OnHelp)
	ON_MESSAGE(USER_INFO_MSG,OnUserInfoMsg)
	ON_WM_SETTINGCHANGE()
	ON_WM_CLOSE()
	ON_WM_DESTROY()
	ON_WM_TIMER()
	ON_UPDATE_COMMAND_UI_RANGE(ID_32776,ID_32777, &CMainFrame::OnUpdateMenuState)
	ON_COMMAND(ID_32776, &CMainFrame::OnMenuStart)
	ON_COMMAND(ID_32777, &CMainFrame::OnMenuStop)
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // 状态行指示器
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

// CMainFrame 构造/析构
volatile LONG CMainFrame::m_bStart = FALSE;
volatile LONG CMainFrame::m_bOnLineOrOffLine = FALSE;
PackInfo CMainFrame::m_curPackInfo = PackInfo(0,0,0,0);
std::vector<int> CMainFrame::m_statList[5] = {std::vector<int>(0)};

#define _HTTPAPI(s,api) HOST(s)##api
#define HTTPAPI(s,api)  _HTTPAPI(s,api)

//#define GETUSERBYROOMRIGHT "http://121.14.69.17:801/RoomService.svc/GetRoomRights?content=F9fD%2fUep0H%2bedZzsFxnV984yhnp5UWJf&clientid=UPWEBSITE&sign=96BICwSaqJLXvpjrvP497g%3d%3d"
//#define GETUSERBYROOMRIGHT "http://121.14.69.17:801/RoomService.svc/GetRoomRights?content=KbY2f0uBbWHMWfnhS4eeS9JzgaV8fFrU&clientid=UPPRODUCT&sign=XOupHgFh6pHOGEXTrDAatw%3d%3d"
#define GETUSERBYROOMRIGHT "http://121.14.69.17:801/RoomService.svc/GetRoomRights?content=F9fD%2fUep0H9w42OxkPxyNYYepVxBE%2b0Y&clientid=UPWEBSITE&sign=yHYSbw9cEKAgZ%2bqBzxNEWQ%3d%3d"

UINT WINAPI HttpUpdate(LPVOID pv)
{
	CMainFrame* pMainFrame = (CMainFrame*)pv;
	while(WaitForSingleObject(pMainFrame->m_StartEvent,INFINITE) == WAIT_OBJECT_0){
		g_FxsHDManage.DeleteFxsToHousInfo();
 		LPCTSTR strJson = "";

		g_FxsHDManage.Request(HTTPAPI("upchinafund.com","getanalystinfo"),&strJson,CFXSHDManage::FXSINFO,NULL,0,
			GenericHTTPClient::RequestGetMethod,NULL);

		g_FxsHDManage.Request(HTTPAPI("upchinafund.com","getallroom"),
			&strJson,CFXSHDManage::FXS_HOUSEINFO,NULL,0,GenericHTTPClient::RequestGetMethod,NULL);
		
		for (int i = 1;i<4;i++)
		{
			CString httpstr = "";
			httpstr.Format(HTTPAPI("upchinafund.com","getroombymarket?market=%d"),i);
			g_FxsHDManage.Request(httpstr,
				&strJson,CFXSHDManage::FXS_VIP_HOUSEINFO,NULL,i,GenericHTTPClient::RequestGetMethod,NULL);
		}

		VHouseInfo* ptempFXSHouse = g_FxsHDManage.GetHouseInfo();
		for (int i = 0;i<ptempFXSHouse->size();i++)
		{
 			CString httpstr = "";
 			httpstr.Format(HTTPAPI("upchinafund.com","getanalystinfobyroom?roomId=%s"),(*ptempFXSHouse)[i].RoomId);	
			g_FxsHDManage.Request(httpstr,&strJson,CFXSHDManage::FXS_TO_HOUSEINFO,&(*ptempFXSHouse)[i],0,
				GenericHTTPClient::RequestGetMethod,NULL);
		}

		MapMarketToVipHouseinfo* ptempFXSVipHouse = g_FxsHDManage.GetVipHouseInfo();
		if (ptempFXSVipHouse->size()>0)
		{
			MapMarketToVipHouseinfo::iterator itmap_FxsVipHouse = ptempFXSVipHouse->begin();

			for (;itmap_FxsVipHouse!=ptempFXSVipHouse->end();itmap_FxsVipHouse++){
				for (int j = 0;j<itmap_FxsVipHouse->second.size();j++){
					CString httpstr = "";
					httpstr.Format(HTTPAPI("upchinafund.com","getanalystinfobyroom?roomId=%s"),itmap_FxsVipHouse->second[j].RoomId);
					g_FxsHDManage.Request(httpstr,&strJson,CFXSHDManage::ROOM_TO_FXSINFO,&(itmap_FxsVipHouse->second[j])
						,itmap_FxsVipHouse->first,GenericHTTPClient::RequestGetMethod,NULL);
				}
			}
			
			g_FxsHDManage.Request(GETUSERBYROOMRIGHT,
				&strJson,CFXSHDManage::ROOMRIGHT_TO_USER,NULL,0,GenericHTTPClient::RequestGetMethod,NULL);
// 			CString sGetuserByRoomright = g_FxsHDManage.GetUserByRoomRight();
// 			g_FxsHDManage.Request(sGetuserByRoomright.GetBuffer(0),
// 				&strJson,CFXSHDManage::ROOMRIGHT_TO_USER,NULL,0,GenericHTTPClient::RequestGetMethod,NULL);
		}

		

		Sleep(60*10000);
	}
	
	return 0;
}


CMainFrame::CMainFrame()
	
{
	// TODO: 在此添加成员初始化代码
	theApp.m_nAppLook = theApp.GetInt(_T("ApplicationLook"), ID_VIEW_APPLOOK_VS_2008);
	m_iStart = 1;
	m_iStop  = 0;
	m_StatThread = NULL;
	m_HttpThread = NULL;
	InterlockedExchange(&g_MainFrmThreadState, m_bStart);
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWndEx::OnCreate(lpCreateStruct) == -1)
		return -1;
	//设置标题栏的图标
	HICON m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	SetIcon(m_hIcon,TRUE);
	SetIcon(m_hIcon,FALSE);
	
	static CHAR *ss_MonthStr[12] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
	CHAR szTemp[16] = {0};
	static CHAR BuildDateTime[32] = "";
	LONG nYear, nMonth, nDay;
	strncpy( szTemp, __DATE__, 3 );
	szTemp[3] = '\0';
	for ( nMonth=1; nMonth<=12; nMonth++ )
	{
		if ( _stricmp( szTemp, ss_MonthStr[nMonth-1] ) == 0 )
		{
			break;
		}
	}
	strncpy( szTemp, __DATE__+4, 2 );
	szTemp[2] = '\0';
	nDay = atoi( szTemp );
	strncpy( szTemp, __DATE__+9, 2 );
	szTemp[4] = '\0';
	nYear = atoi( szTemp );
	_snprintf( BuildDateTime, 32, "v1.1.%02d.%02d%02d", nYear, nMonth, nDay);

	SetWindowText(BuildDateTime);
	::SetMainWnd(this);	
	// 基于持久值设置视觉管理器和样式
	OnApplicationLook(theApp.m_nAppLook);

	if (!m_wndMenuBar.Create(this))
	{
		TRACE0("未能创建菜单栏\n");
		return -1;      // 未能创建
	}

	m_wndMenuBar.SetPaneStyle(m_wndMenuBar.GetPaneStyle() | CBRS_SIZE_DYNAMIC | CBRS_TOOLTIPS | CBRS_FLYBY);

	// 防止菜单栏在激活时获得焦点
	CMFCPopupMenu::SetForceMenuFocus(FALSE);

	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME_256, 0, IDB_TOOLBAR, TRUE, 0, 0, IDB_TOOLBAR))
	{
		TRACE0("未能创建工具栏\n");
		return -1;      // 未能创建
	}

//	CString strToolBarName;
// 	bNameValid = strToolBarName.LoadString(IDS_TOOLBAR_STANDARD);
// 	ASSERT(bNameValid);
// 	m_wndToolBar.SetWindowText(strToolBarName);

// 	CString strCustomize;
// 	bNameValid = strCustomize.LoadString(IDS_TOOLBAR_CUSTOMIZE);
// 	ASSERT(bNameValid);
// 	m_wndToolBar.EnableCustomizeButton(TRUE, ID_VIEW_CUSTOMIZE, strCustomize);

	if (!m_wndStatusBar.Create(this))
	{
		TRACE0("未能创建状态栏\n");
		return -1;      // 未能创建
	}
	m_wndStatusBar.SetIndicators(indicators, sizeof(indicators)/sizeof(UINT));

	// TODO: 如果您不希望工具栏和菜单栏可停靠，请删除这五行
	m_wndMenuBar.EnableDocking(CBRS_ALIGN_ANY);
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&m_wndMenuBar);
	DockPane(&m_wndToolBar);


	// 启用 Visual Studio 2005 样式停靠窗口行为
	CDockingManager::SetDockingMode(DT_SMART);
	// 启用 Visual Studio 2005 样式停靠窗口自动隐藏行为
	EnableAutoHidePanes(CBRS_ALIGN_ANY);

	// 创建停靠窗口
	if (!CreateDockingWindows())
	{
		TRACE0("未能创建停靠窗口\n");
		return -1;
	}

	m_wndOutput.EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&m_wndOutput);


	// 启用工具栏和停靠窗口菜单替换
	//EnablePaneMenu(TRUE, ID_VIEW_CUSTOMIZE, strCustomize, ID_VIEW_TOOLBAR);

	// 启用快速(按住 Alt 拖动)工具栏自定义
	CMFCToolBar::EnableQuickCustomization();

// 	CList<UINT, UINT> lstBasicCommands;
// 
// 	lstBasicCommands.AddTail(ID_SERVER_START);
// 	lstBasicCommands.AddTail(ID_SERVER_STOP);
// 	lstBasicCommands.AddTail(ID_SERVER_SET);
// 	lstBasicCommands.AddTail(ID_SERVER_HELP);
// 	CMFCToolBar::SetBasicCommands(lstBasicCommands);

	LoadConfig();

	return InitInst();

	return 0;
}

BOOL CMainFrame::InitInst()
{
	m_StartEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	g_pList = new CInfoList;
	// 数据库初始化
	if(!g_LinkMangaer.Init()) 
	{
		g_nInitSuc = 0;
		LogMsg("***数据库初始化失败 -- LinkMangaer.Init ERROR");
		goto ErrorLable;
	}
	LogMsg("***数据库初始化成功");
	// mongodb初始化
	if (!g_MongoDBMgr.InitConnect())
	{
		g_nInitSuc = 0;
		LogMsg("***MONGODB初始化失败 -- MongoDB.Init ERROR");
		goto ErrorLable;
	}
	LogMsg("***MONGODB初始化成功");

	char szError[256] = {0};
	DWORD dwThreadID = 0;
	LPOPLINK pLinkInfo = NULL;	
	pLinkInfo = g_LinkMangaer.GetLink(szError,sizeof(szError),TRUE);
	if(!pLinkInfo)  
	{
		AfxMessageBox("g_LinkMangaer.GetLink ERROR");
		g_nInitSuc = 0;
	}
	else
	{
		g_nInitSuc = 1;
	}

	if(g_nInitSuc)
	{
		// 数据库连接成功
		// 初始化群组信息
		g_GroupChatMng.Init(pLinkInfo->pAdodb);
		// 初始化客服信息
		g_custSvrMng.Init(pLinkInfo->pAdodb);
		//初始化公众号信息
#ifdef PUBLICMNG
		g_PublicMng.Init(pLinkInfo->pAdodb);
#endif
		g_LinkMangaer.FreeLink(pLinkInfo);
	}
	else
	{
		// 失败
		g_threadInfo.hInitSvr = ::CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)_AfxInitSvr, (LPVOID)&g_threadInfo, 0, &dwThreadID);
		if(!g_threadInfo.hInitSvr) 
			goto ErrorLable;
	}

	g_threadInfo.hEventExit = CreateEvent(NULL, TRUE, FALSE, NULL);
	if(!g_threadInfo.hEventExit) 
		goto ErrorLable;

	g_threadInfo.hUserState = ::CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)_AfxSendUserState, (LPVOID)NULL, 0, &dwThreadID);
	if(!g_threadInfo.hUserState) goto ErrorLable;

	g_threadInfo.hOffMsg = ::CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)_AfxAddOffMsg, (LPVOID)NULL, 0, &dwThreadID);
	if(!g_threadInfo.hOffMsg) goto ErrorLable;

	g_threadInfo.hHisMsg = ::CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)_AfxAddHisMsg, (LPVOID)NULL, 0, &dwThreadID);
	if(!g_threadInfo.hHisMsg) goto ErrorLable;

	g_threadInfo.hMsgSend = ::CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)_AfxSendMsg, (LPVOID)NULL, 0, &dwThreadID);
	if(!g_threadInfo.hMsgSend) goto ErrorLable;

	SetTimer(1,500,NULL);
	return TRUE;

ErrorLable:
	g_threadInfo.bStartExit = TRUE;
	TDEL(g_pList);
	g_GroupChatMng.Uninit();

#ifdef PUBLICMNG
	g_PublicMng.Uninit();
#endif	
	
	return FALSE;
}

BOOL CMainFrame::ExitInst()
{
	g_nInitSuc = 0;
	g_threadInfo.bStartExit = TRUE;
	if(g_threadInfo.hEventExit) SetEvent(g_threadInfo.hEventExit);
	if(g_threadInfo.hUserState) 
	{
		while (WaitForSingleObject(g_threadInfo.hUserState, 50) == WAIT_TIMEOUT) ;
		CLOSEHANDLE(g_threadInfo.hUserState);
	}
	if(g_threadInfo.hOffMsg)
	{
		while (WaitForSingleObject(g_threadInfo.hOffMsg, 50) == WAIT_TIMEOUT) ;
		CLOSEHANDLE(g_threadInfo.hOffMsg);
	}
	if(g_threadInfo.hHisMsg)
	{
		while (WaitForSingleObject(g_threadInfo.hHisMsg, 50) == WAIT_TIMEOUT) ;
		CLOSEHANDLE(g_threadInfo.hHisMsg);
	}
	if(g_threadInfo.hInitSvr)
	{
		while (WaitForSingleObject(g_threadInfo.hInitSvr, 50) == WAIT_TIMEOUT) ;
		CLOSEHANDLE(g_threadInfo.hInitSvr);
	}
	if(g_threadInfo.hMsgSend)
	{
		while (WaitForSingleObject(g_threadInfo.hMsgSend, 50) == WAIT_TIMEOUT) ;
		CLOSEHANDLE(g_threadInfo.hMsgSend);
	}

	CLOSEHANDLE(g_threadInfo.hEventExit);
	CLOSEHANDLE(m_StartEvent);
	TDEL(g_pList);
	g_GroupChatMng.Uninit();
#ifdef PUBLICMNG
	g_PublicMng.Uninit();
#endif

	SaveConfig();
	SendSvrOffMSG();

	return TRUE;
}

VOID CMainFrame::SendSvrOffMSG()
{
	vector <UserNode> allNode;
	if (g_MongoDBMgr.GetAllOnlineUser(allNode))
	{
		if (allNode.size() > 0)
		{
			char pSend[ANSBUFFER_LEN] = {0};
			int nLen = sizeof(svroff_msg);
			IMANSBUFFER m_ansbuf = {0};
			InitAnsBufHead(m_ansbuf, UPIM_REQ_SVROFF, nLen, nLen);

			svroff_msg svroffmsg = {0};
			svroffmsg.nMsg = 0;

			memcpy(&(m_ansbuf.AnsBuffer), &svroffmsg, nLen);
			int nLenAll = sizeof(IMANSHEADER) + nLen;
			memcpy(pSend, &m_ansbuf, nLenAll);

			for (int nIndex_=0; nIndex_< allNode.size(); nIndex_++)
				g_MySever.m_Server.Send((CONNID)allNode[nIndex_].m_dwSessionID, (LPBYTE)pSend, nLenAll);
		}
	}
	return;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWndEx::PreCreateWindow(cs) )
		return FALSE;
	// TODO: 在此处通过修改
	cs.style &= ~FWS_ADDTOTITLE;
	return TRUE;
}

BOOL CMainFrame::CreateDockingWindows()
{
	BOOL bNameValid;
	// 创建输出窗口
	CString strOutputWnd;
	bNameValid = strOutputWnd.LoadString(IDS_OUTPUT_WND);
	ASSERT(bNameValid);
	if (!m_wndOutput.Create(strOutputWnd, this, CRect(0, 0, 100, 100), TRUE, ID_VIEW_OUTPUTWND, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_BOTTOM | CBRS_FLOAT_MULTI))
	{
		TRACE0("未能创建输出窗口\n");
		return FALSE; // 未能创建
	}

	SetDockingWindowIcons(theApp.m_bHiColorIcons);
	return TRUE;
}

void CMainFrame::SetDockingWindowIcons(BOOL bHiColorIcons)
{
	HICON hOutputBarIcon = (HICON) ::LoadImage(::AfxGetResourceHandle(), MAKEINTRESOURCE(bHiColorIcons ? IDI_OUTPUT_WND_HC : IDI_OUTPUT_WND), IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), 0);
	m_wndOutput.SetIcon(hOutputBarIcon, FALSE);

}

// CMainFrame 诊断

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWndEx::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWndEx::Dump(dc);
}
#endif //_DEBUG


// CMainFrame 消息处理程序

void CMainFrame::OnViewCustomize()
{
	CMFCToolBarsCustomizeDialog* pDlgCust = new CMFCToolBarsCustomizeDialog(this, TRUE /* 扫描菜单*/);
	pDlgCust->Create();
}

LRESULT CMainFrame::OnToolbarCreateNew(WPARAM wp,LPARAM lp)
{
	LRESULT lres = CFrameWndEx::OnToolbarCreateNew(wp,lp);
	if (lres == 0)
	{
		return 0;
	}

	CMFCToolBar* pUserToolbar = (CMFCToolBar*)lres;
	ASSERT_VALID(pUserToolbar);

	BOOL bNameValid;
	CString strCustomize;
	bNameValid = strCustomize.LoadString(IDS_TOOLBAR_CUSTOMIZE);
	ASSERT(bNameValid);

	pUserToolbar->EnableCustomizeButton(TRUE, ID_VIEW_CUSTOMIZE, strCustomize);
	return lres;
}

void CMainFrame::OnApplicationLook(UINT id)
{
	CWaitCursor wait;

	theApp.m_nAppLook = id;

	switch (theApp.m_nAppLook)
	{
	case ID_VIEW_APPLOOK_WIN_2000:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManager));
		break;

	case ID_VIEW_APPLOOK_OFF_XP:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOfficeXP));
		break;

	case ID_VIEW_APPLOOK_WIN_XP:
		CMFCVisualManagerWindows::m_b3DTabsXPTheme = TRUE;
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));
		break;

	case ID_VIEW_APPLOOK_OFF_2003:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOffice2003));
		CDockingManager::SetDockingMode(DT_SMART);
		break;

	case ID_VIEW_APPLOOK_VS_2005:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerVS2005));
		CDockingManager::SetDockingMode(DT_SMART);
		break;

	case ID_VIEW_APPLOOK_VS_2008:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerVS2008));
		CDockingManager::SetDockingMode(DT_SMART);
		break;

	case ID_VIEW_APPLOOK_WINDOWS_7:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows7));
		CDockingManager::SetDockingMode(DT_SMART);
		break;

	default:
		switch (theApp.m_nAppLook)
		{
		case ID_VIEW_APPLOOK_OFF_2007_BLUE:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_LunaBlue);
			break;

		case ID_VIEW_APPLOOK_OFF_2007_BLACK:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_ObsidianBlack);
			break;

		case ID_VIEW_APPLOOK_OFF_2007_SILVER:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_Silver);
			break;

		case ID_VIEW_APPLOOK_OFF_2007_AQUA:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_Aqua);
			break;
		}

		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOffice2007));
		CDockingManager::SetDockingMode(DT_SMART);
	}

	RedrawWindow(NULL, NULL, RDW_ALLCHILDREN | RDW_INVALIDATE | RDW_UPDATENOW | RDW_FRAME | RDW_ERASE);

	theApp.WriteInt(_T("ApplicationLook"), theApp.m_nAppLook);
}

void CMainFrame::OnUpdateApplicationLook(CCmdUI* pCmdUI)
{
	pCmdUI->SetRadio(theApp.m_nAppLook == pCmdUI->m_nID);
}

void CMainFrame::OnSettingChange(UINT uFlags, LPCTSTR lpszSection)
{
	CFrameWndEx::OnSettingChange(uFlags, lpszSection);
	m_wndOutput.UpdateFonts();
}


void CMainFrame::OnStart()
{
	InterlockedExchange(&m_bStart, TRUE);
	InterlockedExchange(&g_MainFrmThreadState, m_bStart);
	
	if(g_MySever.m_Server.Start(g_config.szSVRIP, (USHORT)g_config.dwSVRPORT))
	{
		SetEvent(m_StartEvent);
		m_iStart-=1;
		m_iStop+=1;
#ifdef PUBLICMNG
		g_PublicMng.StartIdleThread();
#endif
		if (!m_StatThread )//开启服务器监控线程
		{
			m_StatThread = (HANDLE)_beginthreadex(nullptr, 0, StatSever, (LPVOID)this, 0, nullptr);
		}

		if(!m_HttpThread)
			m_HttpThread = (HANDLE)_beginthreadex(nullptr, 0, HttpUpdate, (LPVOID)this, 0, nullptr);

		::LogServerStart(g_config.szSVRIP, (USHORT)g_config.dwSVRPORT);
	}
	else
	{
		::LogServerStartFail(g_MySever.m_Server.GetLastError(), g_MySever.m_Server.GetLastErrorDesc());
	}
	CString strTemp;
	strTemp.Format("%s 服务器开启！",GetNowTime());
	OP_LOG_INFO("%s",strTemp);
}


void CMainFrame::OnStop()
{
	InterlockedExchange(&m_bStart, FALSE);
	InterlockedExchange(&g_MainFrmThreadState, m_bStart);	
	if(g_MySever.m_Server.Stop())
	{
		ResetEvent(m_StartEvent);
		m_iStart+=1;
		m_iStop-=1;
		CUPIMServerView* pView = (CUPIMServerView*)this->GetActiveView();
		SDATA SData(0,0,0,0,0,0,0,0,0,0);
		pView->m_DlgStatistics.UpdateData(&SData);
		pView->m_DlgStatistics.UpdateListData();

		for (int i =0;i<5;i++)
		{
			while(!m_statList[i].empty()) m_statList[i].clear();
		}
#ifdef PUBLICMNG
		g_PublicMng.StopIdleThread();
#endif
	}
	else
	{
		::LogServerStartFail(g_MySever.m_Server.GetLastError(), g_MySever.m_Server.GetLastErrorDesc());
	}
	CString strTemp;
	strTemp.Format("%s 服务器被主动关闭！",GetNowTime());
	OP_LOG_INFO("%s",strTemp);
}

void CMainFrame::OnSet()
{

}


void CMainFrame::OnHelp()
{

}

void CMainFrame::OnUpdateToolBar(CCmdUI* pCmdUI)
{
	if (pCmdUI->m_nID==ID_SERVER_START)
	{
		pCmdUI->Enable(m_iStart);
	}
	if (pCmdUI->m_nID==ID_SERVER_STOP)
	{
		pCmdUI->Enable(m_iStop);
	}
	return;
}

LRESULT CMainFrame::OnUserInfoMsg(WPARAM wp, LPARAM lp)
{
	info_msg* msg = (info_msg*)wp;

	::LogInfoMsg(msg);

	return 0;

}

void CMainFrame::OnTimer(UINT_PTR nIDEvent)
{
	switch(nIDEvent){
	case 1:
		OnStart();
		KillTimer(1);
	}
}
void CMainFrame::OnClose()
{
	ExitInst();
	::SetMainWnd(NULL);
	InterlockedExchange(&m_bStart, FALSE);
	InterlockedExchange(&g_MainFrmThreadState, m_bStart);
	WaitForSingleObject(m_StatThread, 500);
	if( WaitForSingleObject(m_StatThread, 0) == WAIT_TIMEOUT )
		TerminateThread( m_StatThread, 0 );
	CloseHandle(m_StatThread);
	m_StatThread = NULL;

	WaitForSingleObject(m_HttpThread, 500);
	if( WaitForSingleObject(m_HttpThread, 0) == WAIT_TIMEOUT )
		TerminateThread( m_HttpThread, 0 );
	CloseHandle(m_HttpThread);
	m_HttpThread = NULL;
	CString strTemp;
	strTemp.Format("%s 服务器退出！",GetNowTime());
	OP_LOG_INFO("%s",strTemp);
	g_MySever.m_Server.Stop();
 	__super::OnClose();
}

void CMainFrame::OnDestroy()
{
	TDEL(g_pList); 	
	__super::OnDestroy();
}

UINT WINAPI CMainFrame::StatSever(LPVOID pv)
{
	CMainFrame* pMainFrame = (CMainFrame*)pv;
 	CUPIMServerView* pView = (CUPIMServerView*)pMainFrame->GetActiveView();
	
	while(WaitForSingleObject(pMainFrame->m_StartEvent,INFINITE) == WAIT_OBJECT_0)
	{
		PackInfo SecondpPackInfo = *g_MySever.GetPackInfo() - m_curPackInfo;
		m_curPackInfo = *g_MySever.GetPackInfo();
		SDATA SData(0,0,0,0,0,0,0,0,0,0);
		SData.SendFlow = SecondpPackInfo.SendDataLen;
		SData.SendPack = SecondpPackInfo.SendPackLen;
		SData.SendPeak = StatPeakData(m_statList[0],SecondpPackInfo.SendDataLen);
		SData.SendPackPeak = StatPeakData(m_statList[3],SecondpPackInfo.SendPackLen);;
		SData.ReceiveFlow = SecondpPackInfo.RecieveDataLen;
		SData.ReceivePack = SecondpPackInfo.RecievePackLen;
		SData.RecievePeak = StatPeakData(m_statList[1],SecondpPackInfo.RecieveDataLen);
		SData.ReceivePackPeak = StatPeakData(m_statList[4],SecondpPackInfo.RecievePackLen);
		SData.CurConnectNum = g_MySever.m_Server.GetConnectionCount();
		SData.ConnectPeak = StatPeakData(m_statList[2],SData.CurConnectNum);
		pView->m_DlgStatistics.UpdateData(&SData);

		Sleep(1000);
	}

	return 0;
}

int CMainFrame::StatPeakData(std::vector<int> &dv,int data)
{
	int maxData =0;
	if (!dv.empty()) maxData = dv.back();
	dv.push_back(data);
	if (data > maxData ){
		maxData = data;
	}
	else{
		sort(dv.begin(),dv.end());
		maxData = dv.back();
	}
	return maxData;
}


void CMainFrame::OnUpdateMenuState(CCmdUI *pCmdUI)
{
	// TODO: 在此添加命令更新用户界面处理程序代码
	if (pCmdUI->m_nID == ID_32776)
	{
		pCmdUI->Enable(m_iStart);
	}
	if (pCmdUI->m_nID == ID_32777)
	{
		pCmdUI->Enable(m_iStop);
	}
}


void CMainFrame::OnMenuStart()
{
	// TODO: 在此添加命令处理程序代码
	//只是为了让程序崩溃  
	//*(int*)0 = 1234;

	OnStart();
}


void CMainFrame::OnMenuStop()
{
	// TODO: 在此添加命令处理程序代码
	OnStop();
}

BOOL compare_by_online_name(CString strName1 ,CString strName2)
{
	int online1 = 0 ,online2 = 0 ; 

	MYSTATE m_state1 = (MYSTATE)0;
	DWORD dwConnectID1 = 0;
	g_MongoDBMgr.GetUserState(strName1, m_state1, dwConnectID1);
	MYSTATE m_state2 = (MYSTATE)0;
	DWORD dwConnectID2 = 0;
	g_MongoDBMgr.GetUserState(strName2, m_state2, dwConnectID2);
	online1 = (m_state1 == MYSTATE_NORMAL) ? 1 : 0 ; 
	online2 = (m_state2 == MYSTATE_NORMAL) ? 1 : 0 ; 
	
	if( online1 != online2 )
	{
		if( online1 > online2 )
			return TRUE ; 
		else
			return FALSE ; 
	}
	else
	{

		return strcmp(strName1.GetBuffer() ,strName2.GetBuffer()) < 0 ; 
	}
}