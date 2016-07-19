#include "stdafx.h"
#include <windows.h>
#if !defined(UNDER_CE)
#include <shellapi.h>
#include<algorithm>
//#include "macro.h"
#endif

#include "UIRecentLink.h"
#include "MainFrame.h"
#include "ColorSkin.h"
#include "ChatDialog.h"
#include "AddMember.h"
#include "HistoryMsg.h"
#include "GeneralDialog.h"
#include "UserMrgDialog.h"
#include "MsgNotifyDialog.h"
#include "..\MainPort.h"
#include "..\resource.h"
#include "UIMenu.h"
#include "UserOpenRights.h"

//主面板
const TCHAR* const kTitleControlName		= _T("mainframe_apptitle");				// 总标题（例如QQ2014,UPIM，YY）
const TCHAR* const kCloseButtonControlName	= _T("mainframe_closebtn");				// 关闭
const TCHAR* const kMinButtonControlName	= _T("mainframe_minbtn");				// 最小化
const TCHAR* const kMaxButtonControlName	= _T("mainframe_maxbtn");				// 最大化
const TCHAR* const kRestoreButtonControlName= _T("mainframe_restorebtn");			// 还原
const TCHAR* const kSearchButtonControlName= _T("mainframe_searchbtn");			// 搜索好友 
const TCHAR* const kSelfNameControlName		= _T("mainframe_mainselfname");			// 自己昵称
const TCHAR* const kWeatherButtonControlName= _T("mainframe_weather");				// 天气
const TCHAR* const kFaceButtonControlName	= _T("mainframe_selfPic");				// 头像
const TCHAR* const kStateButtonControlName	= _T("mainframe_userstate");			// 在线状态图标

const TCHAR* const kFriendButtonControlName = _T("mainframe_friendbtn");
const TCHAR* const kGroupButtonControlName = _T("mainframe_groupbtn");
const TCHAR* const kRecentButtonControlName = _T("mainframe_recentbtn");

const TCHAR* const kTabControlName = _T("mainframe_tabs");
const TCHAR* const kFriendsListControlName = _T("mainframe_friends");
const TCHAR* const kGroupsListControlName = _T("mainframe_selfgroup");  // wuchao modify _T("mainframe_groups") to _T("mainframe_selfgroup")
const TCHAR* const kSelfGroupsListControlName = _T("mainframe_selfgroup");
const TCHAR* const kRecentListControlName = _T("mainframe_recent");

const TCHAR* const kHideLeftMainPannelControlName = _T("mainframe_btnleft");
const TCHAR* const kShowLeftMainPannelControlName = _T("mainframe_btnright");
const TCHAR* const kLeftMainPannelControlName = _T("mainframe_LeftMainPanel");

const TCHAR* const kSignatureTipsControlName = _T("mainframe_signaturetip");
const TCHAR* const kSignatureControlName = _T("mainframe_signature");

const TCHAR* const kSearchEditTipControlName = _T("mainframe_search_tip");
const TCHAR* const kSearchEditControlName = _T("mainframe_search_edit");

const TCHAR* const kChangeBkSkinControlName = _T("mainframe_bkskinbtn");
const TCHAR* const kChangeColorSkinControlName = _T("mainframe_colorskinbtn");

const TCHAR* const kBackgroundControlName = _T("mainframe_bg");
const TCHAR* const kMainMenuControlName = _T("mainframe_MainMenu");
const TCHAR* const kHisMessageControlName = _T("mainframe_message");

// 循环更换背景图片的图片数
const int kBackgroundSkinImageCount = 2;

MainFrame::MainFrame()
	: bk_image_index_(0),m_NotifyDlg(eCreate_TG2)/*,trayIconMessage(TrayIconMessage::WM_MOUSE_ICON_LEAVE)*/
{
	m_AppTitle = "";
	m_bSetSelfInfo = false;
	memset(&m_MainSelectNode, 0, sizeof(SELECTNODE));
	pLoginDialog = NULL;

	m_pMinSysBtn = m_pMaxSysBtn = m_pRestoreSysBtn = m_pCloseSysBtn = NULL;
	m_pFriendsList = NULL;
	m_pGroupsList = NULL;
	m_pSelfGroup = NULL;
	m_pRecentLinkList = NULL;
	m_pTabControl = NULL;
	m_pBackground = NULL;
	m_pUserMrgDialog = NULL ; 
	nDeirction = 0;
	bLoginDialogClosed = FALSE;
	m_bIsIcon = true;
	m_bRecvFriendsMsg = FALSE ; 

	m_hUserBitMap = NULL;
	m_bMouseEnter   = FALSE ; 
	m_eUserState = eUSER_ONLINE ;   // 启动后 默认就是正常在线状态了 

	m_bIsMsgDlgShow = FALSE ;
	m_isEnterLeave  = FALSE ;

	m_nSwitchTab = 0 ; 
}

MainFrame::~MainFrame()
{
	PostQuitMessage(0);
	if( m_pUserMrgDialog )
	{
		delete m_pUserMrgDialog ; 
		m_pUserMrgDialog = NULL ; 
	}
	if( m_hUserBitMap)
	{
		delete m_hUserBitMap ; 
		m_hUserBitMap = NULL ; 
	}

	KillTimer(m_hWnd ,T_TRAY_HOVER_TIMER_ID) ; 
	KillTimer(m_hWnd ,T_TRAY_LEAVE_TIMER_ID) ;
}

LPCTSTR MainFrame::GetWindowClassName() const
{
	return _T("UPGuiFoundation");
}

CControlUI* MainFrame::CreateControl(LPCTSTR pstrClass)
{
	if (_tcsicmp(pstrClass, _T("FriendList")) == 0)
	{
		return new CFriendsUI(m_PaintManager);
	}
	else if(_tcsicmp(pstrClass ,_T("FindFriendList")) == 0)
	{
		return new CFriendsUI(m_PaintManager) ; 
	}
	else if (_tcsicmp(pstrClass, _T("GroupList")) == 0)
	{
		return new CGroupsUI(m_PaintManager, eUI_Group);
	}
	else if (_tcsicmp(pstrClass, _T("SelfList")) == 0)
	{
		// 如果是分析师列表中看见的【群组】，此处为他自定义的分组
		return new CFriendsUI(m_PaintManager);
	}
	else if (_tcsicmp(pstrClass, _T("RecentLink")) == 0)
	{
		//return new CRecentLinkUI(m_PaintManager);
		return new CGroupsUI(m_PaintManager, eUI_Recent);
	}

	return NULL;
}

void MainFrame::OnFinalMessage(HWND hWnd)
{
	WindowImplBase::OnFinalMessage(hWnd);
	delete this;
}

CDuiString MainFrame::GetSkinFile()
{
#ifdef VER_UPIM_CLIENT
	return _T("dlg_MainFrame2.xml");
#endif // VER_UPIM_CLIENT
#ifdef VER_UPIM_RONGYUN
	return _T("dlg_MainFrame3.xml");
#endif
	return _T("dlg_MainFrame.xml");
}

CDuiString MainFrame::GetSkinFolder()
{
	return  _T("skin\\");
}

LRESULT MainFrame::OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	bHandled = FALSE;
	return 0;
}

LRESULT MainFrame::OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	BOOL bZoomed = ::IsZoomed(m_hWnd);
	LRESULT lRes = CWindowWnd::HandleMessage(uMsg, wParam, lParam);
	if (::IsZoomed(m_hWnd) != bZoomed)
	{
		if (!bZoomed)
		{
			if( m_pMaxSysBtn ) 
				m_pMaxSysBtn->SetVisible(false);
			if( m_pRestoreSysBtn ) 
				m_pRestoreSysBtn->SetVisible(true);
		}
		else 
		{
			if( m_pMaxSysBtn ) 
				m_pMaxSysBtn->SetVisible(true);
			if( m_pRestoreSysBtn ) 
				m_pRestoreSysBtn->SetVisible(false);
		}
	}

	return 0;
}


void MainFrame::InitWindow()
{
	m_pFriendsList = static_cast<CFriendsUI*>(m_PaintManager.FindControl(kFriendsListControlName));
	m_pSelfGroup = static_cast<CFriendsUI*>(m_PaintManager.FindControl(kSelfGroupsListControlName));
	m_pGroupsList = static_cast<CGroupsUI*>(m_PaintManager.FindControl(kGroupsListControlName));
	m_pRecentLinkList = static_cast<CGroupsUI*>(m_PaintManager.FindControl(kRecentListControlName));
	m_pTabControl = static_cast<CTabLayoutUI*>(m_PaintManager.FindControl(kTabControlName));
	m_pBackground = static_cast<CControlUI*>(m_PaintManager.FindControl(kBackgroundControlName));
	m_pControlSwitch = static_cast<CTabLayoutUI*>(m_PaintManager.FindControl(_T("mainframe_switch")));
	m_pFindFriendList = static_cast<CFriendsUI*>(m_PaintManager.FindControl(_T("mainframe_findfriends")));
	m_pMinSysBtn = static_cast<CButtonUI*>(m_PaintManager.FindControl(kMinButtonControlName));
	m_pMaxSysBtn = static_cast<CButtonUI*>(m_PaintManager.FindControl(kMaxButtonControlName));
	m_pRestoreSysBtn = static_cast<CButtonUI*>(m_PaintManager.FindControl(kRestoreButtonControlName));
	m_pCloseSysBtn = static_cast<CButtonUI*>(m_PaintManager.FindControl(kCloseButtonControlName));
	m_pSearchBtn   = static_cast<CButtonUI*>(m_PaintManager.FindControl(kSearchButtonControlName));
	m_pSearch_edit = static_cast<CRichEditUI*>(m_PaintManager.FindControl(kSearchEditControlName));
	m_pSearch_edit_friend = static_cast<CRichEditUI*>(m_PaintManager.FindControl(_T("mainframe_search_edit_friend")));
	m_pSearchCloseBtn  = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("mainframe_closebtn_friend"))); 

	HRSRC	_h	= FindResource(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_WAVE_NOTIFY), "WAVE");
	HGLOBAL _hg	= LoadResource(AfxGetInstanceHandle(), _h);
	m_pWave[0]	= LockResource(_hg);

	_h	= FindResource(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_WAVE_MSG), "WAVE");
	_hg	= LoadResource(AfxGetInstanceHandle(), _h);
	m_pWave[1] = LockResource(_hg);

	_h	= FindResource(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_WAVE_ONLINE), "WAVE");
	_hg	= LoadResource(AfxGetInstanceHandle(), _h);
	m_pWave[2] = LockResource(_hg);

	_h	= FindResource(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_WAVE_SHAKE), "WAVE");
	_hg	= LoadResource(AfxGetInstanceHandle(), _h);
	m_pWave[3] = LockResource(_hg);

	AddTimer(T_FLASHITEM, eTE_FlashItem);
	AddTimer(T_CONNECT, eTE_Connect);

	if (VER_UPIM_NUMS == 4)
	{
		USER_LOG("添加定时器：客户每隔20秒主动从服务器获取分析师的状态") ;
		AddTimer(T_REFRESHSTATUS, eTE_RefreshStatus);
	}
}


LRESULT MainFrame::ResponseDefaultKeyEvent(WPARAM wParam)
{
	if (wParam == VK_RETURN)
	{
		return FALSE;
	}
	else if (wParam == VK_ESCAPE)
	{
		return TRUE;
	}
	return FALSE;
}

void MainFrame::OnTimer(TNotifyUI& msg)
{
	switch(msg.wParam)
	{
	case T_FLASHITEM:
		{
			FlashAllItem();
		}
		break;
	case T_OFFMSG:
		{
			USER_LOG("T_OFFMSG DEL");
			m_PaintManager.KillTimer(m_pBackground, T_OFFMSG);
#ifndef VER_UPIM_RONGYUN
			g_MyClient.SendOFFMSGReq();
#endif
		}
		break;
	case T_NOTIFYICON:
		{
			NotifyiconFlash();
		}
		break;
	case T_CONNECT:
		{
#ifndef VER_UPIM_RONGYUN
			CheckConnectStatus();
#endif
		}
		break;

	case T_TRAY_HOVER_TIMER_ID: // 鼠标悬停在托盘图标上面 
		{
			KillTimer(m_hWnd ,T_TRAY_HOVER_TIMER_ID) ; 
			POINT pt ; 
			if( GetCursorPos(&pt))
			{
				// 如果鼠标在图标上面  或者 鼠标在提示框内部时  显示提示框 
				if( (pt.x == m_ptMouseHover.x && pt.y == m_ptMouseHover.y))  
				{ 
					MessageNotify(TrayIconMessage::WM_MOUSE_ICON_HOVER);  
				}
			}
		}
		break ; 
	case T_TRAY_LEAVE_TIMER_ID:  // 鼠标离开托盘图标
		{
			if( m_isEnterLeave == FALSE )
			{
				POINT pt;  
				if(GetCursorPos(&pt))  
				{  
					if(pt.x != m_ptMouseHover.x || pt.y != m_ptMouseHover.y)  
					{  
						m_bMouseEnter = FALSE;  
						KillTimer(m_hWnd, T_TRAY_HOVER_TIMER_ID);  
						KillTimer(m_hWnd, T_TRAY_LEAVE_TIMER_ID);   
						MessageNotify(TrayIconMessage::WM_MOUSE_ICON_LEAVE);  
					}  
				}  
			}
	
		}
		break ;
		// 客户登录后  每隔20秒  主动从服务器获取分析师的状态 
	case T_REFRESHSTATUS:
		{
			GetAllFXSState();
		}
		break ; 
	default:
		break;
	}
}

// 分析师每隔20s会想服务器发送请求 获取在线分析师
void MainFrame::GetAllFXSState()
{
	if (m_vtInfo.size() <= 0)
		return;
	Value data_value;
	Value item;  
	Value array;
	FastWriter writer;
	string strdata;
	int nIndex = 0;
	for (std::vector<AnalystInfo>::iterator iterator = m_vtInfo.begin(); iterator != m_vtInfo.end(); ++iterator)
	{
		item = (int)iterator->AnalystId;
		array.append(item);

		data_value["ct"] = ++nIndex;		// count
		data_value["it"] = array;			// item
	}

	strdata = writer.write(data_value);
	int packlen = strdata.length();

	g_MyClient.SendGetUserState(strdata.c_str(), 999);
}


void MainFrame::AddTimer(UINT nTimerID, UINT uElapse)
{
	m_PaintManager.SetTimer(m_pBackground, nTimerID, uElapse);
}

void MainFrame::CheckConnectStatus()
{
	if (g_MyClient.m_Client.GetState() != SS_STARTED)
	{
		if (g_MyClient.m_nUserType == eUser_UPAnalyst || g_MyClient.m_nUserType == eUser_UPAnalyst2 || g_MyClient.m_nUserType == eUser_Broker || g_MyClient.m_nUserType == eUser_UPClient2)
		{
			if (!g_bUserKickOff)
			{
				ReConnect();
				USER_LOG("ReConnect");
			}
		}
	}
	return;
}

void MainFrame::ConvertImageToHicon(CString strFileName)
{
	WCHAR buf[1024]; 
	CStringW strWide = CT2CW(strFileName); // 将T字符转换成宽字符 wcscpy(buf, strWide); 
	wcscpy(buf, strWide); 
	if( m_hUesrIcon )
	{
		delete m_hUserBitMap ; 
		m_hUserBitMap = NULL ; 
		m_hUesrIcon = NULL ;
	}
	m_hUserBitMap  = Bitmap::FromFile(buf) ;  
	m_hUserBitMap->GetHICON(&m_hUesrIcon) ; 
}
void MainFrame::NotifyiconFlash()
{
	if (m_bIsIcon)
	{
		CString strtooltip = GetNotifyiconToolTip();

		if( m_vecWhoSendMsg.size())
		{
			FriendList_ITER itLastItem = m_vecWhoSendMsg.end() - 1 ; 
			ConvertImageToHicon(((*itLastItem).logo).GetData()) ; 
			if( m_hUesrIcon == NULL )
			{
				NotifyMessage(NIM_MODIFY, m_hIcon, ""); 
			}
			else 
				NotifyMessage(NIM_MODIFY, m_hUesrIcon, ""); 
			m_bIsIcon = false;
		}
	}
	else
	{
		CString strtooltip = GetNotifyiconToolTip();
		NotifyMessage(NIM_MODIFY, NULL, strtooltip); 
		m_bIsIcon = true;
	}
	return;
}

void MainFrame::SetNotifyiconFlash()
{
	AddTimer(T_NOTIFYICON, eTE_NotifyIcon);
}

// 取得用户的权限，分析师不需要调用此接口
#include "../GenericHTTPClient.h"
#define WEB_RIGHT_JK "http://ssoapi.upchinafund.com/ssodb/uid/"
void MainFrame::GetClientRights(CString strUpID)
{
	CString strURL = "";
	strURL.Format("%s%s", WEB_RIGHT_JK, strUpID);
	GenericHTTPClient* httpClient = new GenericHTTPClient;
	if(httpClient->Request(strURL))
	{
		LPCTSTR szResult = httpClient->QueryHTTPResponse();    //返回的数据流

		Reader reader;
		Value value;
		string ssValue = szResult;
		// 数组类型结果返回
		if (reader.parse(ssValue, value))
		{
			Value val_array = value["rights"];  
			int nSize = val_array.size();
			if (nSize > 0)
			{
				for (int i = 0; i < nSize; i ++)
				{
					int nRightNum = atoi(val_array[i]["mid"].asString().c_str());
					CString strEndDate = val_array[i]["end_date"].asString().c_str();
					strEndDate.Replace(_T("-"), _T(""));
					m_mapCilentRights[nRightNum] = atoi(strEndDate);
				}
			}
		}
	}
	TDEL(httpClient);
	return;
}

void MainFrame::OpenRightsDlg()
{
	CUserOpenRights* pUserOpenRights = new CUserOpenRights();
	if( pUserOpenRights == NULL )
		return;

	pUserOpenRights->Create(m_hWnd, "", UI_WNDSTYLE_FRAME | WS_POPUP, NULL, 0, 0, 600, 800);
	pUserOpenRights->CenterWindow();
	pUserOpenRights->ShowModal();

	return;
}

// 根据客户ID取得用户头像URL
#define WEB_USERPIC_URL "http://loginapi.upchinafund.com/usercenter/GetUserPic.aspx?userName="
CString MainFrame::GetUserPicURL(CString strUpID)
{
	CString strReturn = "";
	CString strURL = "";
	strURL.Format("%s%s&pictype=48", WEB_USERPIC_URL, strUpID);
	GenericHTTPClient* httpClient = new GenericHTTPClient;
	if(httpClient->Request(strURL))
	{
		LPCTSTR szResult = httpClient->QueryHTTPResponse();    //返回的数据流
		strReturn = szResult;
	}
	TDEL(httpClient);
	return strReturn;
}

void MainFrame::StopNotifyiconFlash()
{
	// 删除定时器
	m_PaintManager.KillTimer(m_pBackground, T_NOTIFYICON);
	CString strtooltip = GetNotifyiconToolTip();

	if(m_eUserState==eUSER_ONLINE)
		NotifyMessage(NIM_MODIFY, m_hIcon, strtooltip);
	else if( m_eUserState == eUSER_OFFLINE || m_eUserState == eUSER_LIKAI)
	{
		NotifyMessage(NIM_MODIFY, m_hOffLineIcon, strtooltip);
	}
	else
		NotifyMessage(NIM_MODIFY, m_hUesrIcon, strtooltip);

	m_bIsIcon = true;
	return;
}

void MainFrame::FlashAllItem()
{
	nDeirction = (nDeirction+1) % 4;
	if (m_vtFlashNodeInfo.size() > 0)
	{
		m_csNodeFlash.Lock();
		for (FlashNode_ITER citer = m_vtFlashNodeInfo.begin(); citer != m_vtFlashNodeInfo.end(); citer++)
		{
			Node* FlashNode = *citer;
			m_pFriendsList->SetNodeFlash(FlashNode, true, nDeirction);
		}
		m_csNodeFlash.Unlock();
	}
	if (m_vtFlashNodeInfo2.size() > 0)
	{
		m_csNodeFlash2.Lock();
		for (FlashNode_ITER citer = m_vtFlashNodeInfo2.begin(); citer != m_vtFlashNodeInfo2.end(); citer++)
		{
			Node* FlashNode = *citer;
			m_pGroupsList->SetNodeFlash(FlashNode, true, nDeirction);
		}
		m_csNodeFlash2.Unlock();
	}
	return;
}

// 图标的闪烁需要修改
void MainFrame::InsertIntoFlashVt(CDuiString strID, EPANELTYPE e_pType)
{
	if (e_pType == ePanel_Single)
	{
		Node* m_nodeuser = m_pFriendsList->SelectNodeByID(strID.GetData());
		if (m_nodeuser)
		{
			BOOL bIsInVt = FALSE;
			m_csNodeFlash.Lock();
			if (m_vtFlashNodeInfo.size() > 0)
			{
				for (FlashNode_ITER citer = m_vtFlashNodeInfo.begin(); citer != m_vtFlashNodeInfo.end(); citer++)
				{
					Node *FlashNode = *citer;
					if (m_nodeuser == FlashNode)
					{
						// 已经添加了闪烁图标 不用再次添加
						bIsInVt = TRUE;
						break;
					}
				}
			}
			if (!bIsInVt)
				m_vtFlashNodeInfo.push_back(m_nodeuser);
			m_csNodeFlash.Unlock();
		}
	}
	else if (e_pType == ePanel_Group)
	{
		Node* m_nodeuser = m_pGroupsList->SelectNodeByID(strID.GetData());
		if (m_nodeuser)
		{
			BOOL bIsInVt = FALSE;
			m_csNodeFlash2.Lock();
			if (m_vtFlashNodeInfo2.size() > 0)
			{
				for (FlashNode_ITER citer = m_vtFlashNodeInfo2.begin(); citer != m_vtFlashNodeInfo2.end(); citer++)
				{
					Node *FlashNode = *citer;
					if (m_nodeuser == FlashNode)
					{
						// 已经添加了闪烁图标 不用再次添加
						bIsInVt = TRUE;
						break;
					}
				}
			}
			if (!bIsInVt)
				m_vtFlashNodeInfo2.push_back(m_nodeuser);
			m_csNodeFlash2.Unlock();
		}
	}
	return;
}

void MainFrame::StopFlashItem(CDuiString strID, EPANELTYPE e_pType)
{
	if (e_pType == ePanel_Single)
	{
		if (m_vtFlashNodeInfo.size() > 0)
		{
			Node* m_nodeuser = m_pFriendsList->SelectNodeByID(strID.GetData());
			if (m_nodeuser)
			{
				m_csNodeFlash.Lock();
				for (FlashNode_ITER citer = m_vtFlashNodeInfo.begin(); citer != m_vtFlashNodeInfo.end(); citer++)
				{
					Node *FlashNode = *citer;
					if (m_nodeuser == FlashNode)
					{
						m_vtFlashNodeInfo.erase(citer);
						m_pFriendsList->SetNodeFlash(FlashNode, false);
						break;
					}
				}
				m_csNodeFlash.Unlock();
			}
		}
	}
	else if (e_pType == ePanel_Group)
	{
		if (m_vtFlashNodeInfo2.size() > 0)
		{
			Node* m_nodeuser = m_pGroupsList->SelectNodeByID(strID.GetData());
			if (m_nodeuser)
			{
				m_csNodeFlash2.Lock();
				for (FlashNode_ITER citer = m_vtFlashNodeInfo2.begin(); citer != m_vtFlashNodeInfo2.end(); citer++)
				{
					Node *FlashNode = *citer;
					if (m_nodeuser == FlashNode)
					{
						m_vtFlashNodeInfo2.erase(citer);
						m_pGroupsList->SetNodeFlash(FlashNode, false);
						break;
					}
				}
				m_csNodeFlash2.Unlock();
			}
		}
	}
	return;
}

// 自己的状态改变之后，直接推送到好友列表中在线的好友
BOOL MainFrame::SendStateNotify(int nstate)
{
#ifndef VER_UPIM_RONGYUN
	if (friends_.size() > 0)
	{
		int nIdx = 0;
		Reader read;
		Value data_value;
		Value array;
		FastWriter writer;
		string strdata = "";
		int i = 0 ; 
		for (FriendList_ITER citer = friends_.begin(); citer != friends_.end(); citer++)
		{
			if (citer->nstate == 1)
			{
				if (stricmp(citer->id.GetData(), g_MyClient.m_strUserid) != 0)
				{
					array.append(citer->id.GetData());
					data_value["ct"] = ++nIdx;					// count
					data_value["it"] = array;					// item
					strdata = writer.write(data_value);
					if (nIdx >= REFRESH_USER_COUNT)
					{
						g_MyClient.SendStateNotifyReq(strdata, nstate);
						array.clear();
						nIdx = 0;
					}
				}
			}
		}
		g_MyClient.SendStateNotifyReq(strdata, nstate);
	}

#endif
	return TRUE;
}
////////////////////////////////////////////////////////////////////////////
// 客户登录 分析师上下线提醒 
// strID 分析师ID 
// nState  分析师的状态 
void MainFrame::AnalystStateChange(CDuiString strID, int nstate)
{
	if( strID.IsEmpty() )
		return ; 

	// 房间号ID 
	int nRoomID = WhitchRoomForAnalys(m_mapAllClientRoom ,strID.GetData()) ;

	CString strAnalystName = _T(" ") ;
	strAnalystName.Format(_T("TGUSER$%d$%s") ,nRoomID ,strID.GetData()) ;  

	CDuiString strDuiAnalystName = _T("") ;
	strDuiAnalystName.Format(_T("%s") ,strAnalystName) ; 

	Node* m_nodeuser = m_pFriendsList->SelectNodeByID(strAnalystName);
	if( !m_nodeuser)
		return ; 

	if( m_pFriendsList )
	{
		// (1) 更新头像 
		m_pFriendsList->SetNodeState(m_nodeuser, nstate);

		// (2) 改变存储的用户信息
		//SetIteratorState(strDuiAnalystName, eVT_Friend, nstate);

		// (3) 更新分组标题 获取父节点 
		Node* pParentNode = m_nodeuser->parent() ;
		if(!pParentNode)
			return ; 

		int totalOnlineNum = pParentNode->data().onlinenum ; 
		if( nstate == 1 )
			pParentNode->data().onlinenum = totalOnlineNum + 1  ;
		else
		{
			if(totalOnlineNum > 0)
				pParentNode->data().onlinenum = totalOnlineNum - 1  ;
		}
		m_pFriendsList->SetNodeName(pParentNode,pParentNode->data().onlinenum,pParentNode->data().totlenum) ; 
		// (4) 排序 暂时还没有实现 

		// (5) 如果聊天窗口已经打开，改变左上角大图标
		if (allopenwnd[strDuiAnalystName] != NULL)
		{
			ChatDialog* pChatDialog = NULL;
			pChatDialog = (ChatDialog*)allopenwnd[strDuiAnalystName];

			// wuchao modify at 2016-03-26  把状态和用户名传给已经打开的对话框  根据状态设置对话框上面的头像 
			// 设置对话框左上方的头像  
			pChatDialog->PostMessage(UM_USER_USERSTATE, (WPARAM)nstate, (LPARAM)(new CString(strID.GetData())));
		}
		// 通知用户上线 下线就不同通知了 
		if( nstate == 1 ) 
		{
			g_pFrame->PostMessage(UM_SOUND_PLAY, IDR_WAVE_ONLINE); 
		}

	}
}

// im2.0 当分析师好友登录时，更新好友列表  分组  最近联系人  
void MainFrame::UpdateFriendsListInFxs(CDuiString strID, int nstate)
{
	Node* m_nodeuser = m_pFriendsList->SelectNodeByID(strID.GetData());
	if (m_nodeuser)
	{
		// 第一步，改变好友列表中的头像状态
		// m_pFriendsList->SetNodeState(m_nodeuser, nstate);
		CString strNomalPic = _T("") , strOfflinePic = _T("") ; 
		strNomalPic.Format("%s\\%s_n.jpg" ,g_config.szUserHead,m_nodeuser->data().numid.GetData()) ; 
		strOfflinePic.Format("%s\\%s_g.jpg" ,g_config.szUserHead,m_nodeuser->data().numid.GetData()) ; 
		if( GetFileAttributes(strNomalPic) == -1 )
		{
			strNomalPic = _T("public.png") ;
		}
		if( GetFileAttributes(strOfflinePic) == -1 )
		{
			strOfflinePic = _T("public_g.png") ; 
		}
		// 根据nstate状态  设置头像  
		if( nstate)
		{
			m_pFriendsList->SetNodePic(m_nodeuser,strNomalPic);
		}
		else
		{
			m_pFriendsList->SetNodePic(m_nodeuser,strOfflinePic) ; 
		}

		// 第二步，改变存储的用户信息
		SetIteratorState(strID.GetData(), eVT_Friend, nstate);
		SetClientOnLine((m_nodeuser->data().numid).GetData() ,nstate) ;  // m_vtRoomClient列表中  状态改变  否则在OnListItemClick有影响   

		// 第三步，将上线的客户移动到未上线客户之前 
		// 第四步,更改好友列表分组标题和自定义分组标题 
		if( m_pFriendsList)
		{
			std::map<CString, Node*>::iterator sortItem = m_pFriendsList->allNodeInfo.begin(); 
			CDuiString strAnayNode = _T("") ; 
			strAnayNode.Format("ANALYST$%d" ,g_nRoomID_im2_0) ; 
			Node *pNode = m_pFriendsList->GetNodeItem(strAnayNode) ;
			if( !pNode)
				return ;
			int onlineNums = pNode->data().onlinenum ;
			int totalNums = pNode->data().totlenum ; 

			USER_ROOMCLIENT_INFO stateClientInfo ; 
			memset(&stateClientInfo,0,sizeof(USER_ROOMCLIENT_INFO)) ; 
			COPY_USERID(stateClientInfo.m_szUPID ,m_nodeuser->data().numid) ; 
			if( nstate)
				stateClientInfo.m_bIsOnLine = nstate ; 

			std::vector<USER_ROOMCLIENT_INFO>::iterator findIt = find( m_vtOnlineClient.begin() ,m_vtOnlineClient.end() ,stateClientInfo) ; 
			if( findIt == m_vtOnlineClient.end() )
			{
				// 在当前在线的用户集合里面找到了改用户  那么分组节点在线人数不能加1 
				// 没有找到的话 在线人数+1 
				onlineNums = onlineNums + 1 ; 
				m_vtOnlineClient.push_back(stateClientInfo) ; 
			}
			m_pFriendsList->SetNodeName(pNode,onlineNums,totalNums) ;  
			m_pFriendsList->SortList() ; 
		}

		// 第五步，如果聊天窗口已经打开，改变左上角大图标
		if (allopenwnd[strID] != NULL)
		{
			ChatDialog* pChatDialog = NULL;
			pChatDialog = (ChatDialog*)allopenwnd[strID];

			// wuchao modify at 2016-03-26  把状态和用户名传给已经打开的对话框  根据状态设置对话框上面的头像 
			// 设置对话框左上方的头像  
			pChatDialog->PostMessage(UM_USER_USERSTATE, (WPARAM)nstate, (LPARAM)(new CString(strID.GetData())));
		}

		// 第六部，声音通知用户在线
		g_pFrame->PostMessage(UM_SOUND_PLAY, IDR_WAVE_ONLINE);
	}

}

////////////////////////////////////////////////////////////////////////
//wuchao add at 2016-03-29 
//有客户上线 重新加载好友列表
////////////////////////////////////////////////////////////////////////
void MainFrame::ReloadFriendList(Node *pParendNode)
{
	if( !pParendNode)
	{
		return ;
	}
	if (m_vtRoomClient.size() == 0)
		return;
	if (m_pFriendsList != NULL)
	{
		for (int idex=0; idex<(int)m_vtRoomClient.size(); idex++)
		{

			CString mGroupID = _T("");
			FriendListItemInfo item;
			Node* retrunptr = NULL;
			int nRoomID = g_nRoomID_im2_0;
			CString strUser = m_vtRoomClient[idex].m_szUPID;

			mGroupID.Format("ANALYST$%d", nRoomID);
			Node* m_mode = m_pFriendsList->SelectNodeByID(mGroupID);
			CDuiString mTgRoomID = _T("");
			mTgRoomID.Format("TGUSER$%d$%s", nRoomID, strUser);

			if(m_vtRoomClient[idex].m_bIsOnLine )
			{
				item.logo = m_vtRoomClient[idex].m_szLNomalImgName ; 
			}
			else 
			{

				item.logo = m_vtRoomClient[idex].m_szLGrayImgName ; 
			}

			if( item.logo.IsEmpty())
			{
				item.logo = _T("public.png");
			}

			// 将节点的在线状态  加进好友列表集合中 
			item.nstate = m_vtRoomClient[idex].m_bIsOnLine ; 
			item.nodetype = eNode_Analyst;
			item.type = eNode_Analyst;
			item.id = mTgRoomID;
			item.Parentid = pParendNode->data().numid;
			item.numid = strUser;
			item.nick_name = strUser;

#ifdef NOT_DISPLAY_ID_LIKE_MOBLIE_NUM
			strUser = ConvertIdToNoPhonrNum(strUser);
#endif // NOT_DISPLAY_ID_LIKE_MOBLIE_NUM
			item.showname = strUser;

			retrunptr = m_pFriendsList->AddNode(item, pParendNode);
			if (retrunptr)
			{
				m_vtRoomClient[idex].m_bLoad = TRUE;
				m_pFriendsList->SetChildVisible(pParendNode, true); 
			}
		}
	}
	return;
}
void MainFrame::UpdateGroupsListInFxs(Node* pGroupNode ,CDuiString strID, int nstate)
{
	// 父节点就是传进来的分组节点
	if(!pGroupNode)
		return ; 

	int nRoomID = g_nRoomID_im2_0;
	CDuiString strGroupClientName = _T("");
	strGroupClientName.Format("TGUSER$%d$%s", nRoomID, strID.GetData());

	// 要插入的节点 
	Node* pInsertNode = m_pSelfGroup->SelectNodeByID(strGroupClientName.GetData()) ;
	if(!pInsertNode)
		return ; 
	
	std::vector<USER_ROOMCLIENT_INFO> vecOnline ,vecOffline ; 
	GetClientInfoInGroup(pGroupNode,vecOnline,vecOffline) ; 
	USER_ROOMCLIENT_INFO curClientInfo  ; 
	memset(&curClientInfo,0,sizeof(USER_ROOMCLIENT_INFO)) ; 
	COPY_USERID(curClientInfo.m_szUPID,strID.GetData()) ; 

	// 排序  
	std::vector<Node*> ChildrenList = pGroupNode->GetChildList() ; 

	// 在孩子节点中找到要插入的客户节点 
	std::vector<Node*>::iterator curInsertIt = find(ChildrenList.begin() ,ChildrenList.end() ,pInsertNode) ;

	// 当客户属于这个分组 才去排序 
	if( curInsertIt != ChildrenList.end()) 
		m_pSelfGroup->SortList(pGroupNode) ; 
	
	CString strNomalPic = _T("") , strOfflinePic = _T("") ; 
	strNomalPic.Format("%s\\%s_n.jpg" ,g_config.szUserHead,strID) ; 
	strOfflinePic.Format("%s\\%s_g.jpg" ,g_config.szUserHead,strID) ; 

	  
	if( GetFileAttributes(strNomalPic) == -1 )
	{
		strNomalPic = _T("public.png") ;
	}
	if( GetFileAttributes(strOfflinePic) == -1 )
	{
		strOfflinePic = _T("public_g.png") ; 
	}
	// 根据nstate状态  设置头像  
	if( nstate)
	{
		m_pSelfGroup->SetNodePic(pInsertNode,strNomalPic);
	}
	else
	{
		m_pSelfGroup->SetNodePic(pInsertNode,strOfflinePic) ; 
	}

	int onlineNums = vecOnline.size(); 
	int totalNums  = pGroupNode->data().totlenum ; 
	m_pSelfGroup->SetNodeName(pGroupNode ,onlineNums ,totalNums) ; 
}

///////////////////////////////////////////////////////////////////
//wuchao add at 2016-03-31 
//friendList中将两个节点的位置交换 
/////////////////////////////////////////////////////////////////
void MainFrame::ExchangeNode(CFriendsUI* friendList ,Node* firstNode ,Node* secondNode)
{
	if( !friendList)
	{
		return ; 
	}

	int nFirstIndex   = firstNode->data().list_elment_->GetIndex() ;
	int nSecondIndex  = secondNode->data().list_elment_->GetIndex() ; 

	if( nFirstIndex == -1 || nSecondIndex == -1)
	{
		return ; 
	}
	CControlUI *pFirstControlUI = friendList->GetItemAt(nFirstIndex) ; 
	CControlUI *pSecondControlUI = friendList->GetItemAt(nSecondIndex) ; 

	// 交换两个控件 
	friendList->SetItemIndex(pFirstControlUI ,nSecondIndex) ; 
	friendList->SetItemIndex(pSecondControlUI ,nFirstIndex) ; 
	firstNode->data().list_elment_->SetIndex(nFirstIndex) ; 
	secondNode->data().list_elment_->SetIndex(nSecondIndex) ; 
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//// 在好友列表中找到满足条件的位置 
// wuchao add at 2016-03-29 
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
int MainFrame::FindInsertNode(std::vector<USER_ROOMCLIENT_INFO> clientList ,Node *pInsertNode)
{
	int index = 0 ; 
	int iSize = clientList.size() ; 
	for( ; index < iSize ; index++)
	{
		// 如果第一个节点不在线，则返回index 
		if( !clientList[index].m_bIsOnLine)
			return index ; 
		else
		{
			// 节点在线  比较用户名大小关系 
			if( pInsertNode->data().numid.Compare(clientList[index].m_szUPID) < 0 )
				return index ; 
		}
	}
	return index ;
}
void MainFrame::UpdateRecentListInFxs(CDuiString strID, int nstate)
{

}

// 客户上线提醒  
void MainFrame::UserStateChange(CDuiString strID, int nstate)
{
	STATE_INFO mState = {0};
	COPYSTRARRAY(mState.userid, strID.GetData());
	mState.nUserstate = nstate;
	m_csStateInfo.Lock();
	m_vtStateInfo.push_back(mState);
	m_csStateInfo.Unlock();
	int nRoomID = g_nRoomID_im2_0 ; 
	CString strClientName = _T(" ") ;
	strClientName.Format(_T("TGUSER$%d$%s") ,nRoomID ,strID.GetData()) ; 
	CDuiString strDuiClientName = _T("") ;
	strDuiClientName.Format(_T("%s") ,strClientName) ; 

	// 更新朋友列表 
	UpdateFriendsListInFxs(strDuiClientName,nstate) ; 

	for (int n=0; n<=USERMRG_FENZU; n++)
	{
		if ( m_groupInfoStc[n].bUsed && m_groupInfoStc[n].bGetUserInfo)
		{
			CDuiString mGroupID = _T("");
			mGroupID.Format("SELFGROUP$%d", m_groupInfoStc[n].nGroupID);
			Node* pGroupNode = m_pSelfGroup->SelectNodeByID(mGroupID.GetData()) ; 
			
			// 更新分组 
			if(pGroupNode)
			{
				UpdateGroupsListInFxs(pGroupNode,strID,nstate) ; 
			}
		}
	}
	// 更新最近联系人 
	UpdateRecentListInFxs(strDuiClientName,nstate) ; 

	// 如果与该用户的聊天窗口已经打开  那么需要更新聊天窗口的头像 

	return;
}

// 分析师获取房间在线人数 
int MainFrame::GetClientOnlineCount()
{
	// 获取在线客户具体信息 


	return 0 ; 
}

std::vector<USER_ROOMCLIENT_INFO>* MainFrame::GetOnlineClientInfo()
{
/*	std::vector<USER_ROOMCLIENT_INFO> *pVecClientInfo; 

	int iCount = m_vtRoomClient.size() ; 
	for( int i = 0 ; i < iCount ; i++)
	{
		if( m_vtRoomClient[i].m_bIsOnLine)
		{
			pVecClientInfo->push_back(m_vtRoomClient[i]) ; 
		}
	}
	return pVecClientInfo ;*/
	return NULL ; 
}
void MainFrame::UserKickOff()
{
	g_bUserKickOff = TRUE;
	PostMessage(UM_USER_OFFLINE, 0, 0);
	OnMenu_UserOffline();
	::MessageBox(m_hWnd, _T("您的账号正在另一台设备上登录，您被迫下线。"), _T("提示"), MB_OK);
}

void MainFrame::ImSvrOff()
{
	PostMessage(UM_USER_OFFLINE, 0, 0);
	OnMenu_UserOffline();
	//::MessageBox(m_hWnd, _T("服务器异常关闭！"), _T("提示"), MB_OK);
}

void MainFrame::OnExit()
{
	ReleaseUserData();
	SaveUserConfig();
	NotifyMessage(NIM_DELETE, m_hIcon, "");
	m_PaintManager.KillTimer(m_pBackground);
	SendStateNotify(MYSTATE_OFFLINE);
	//SetClientOnLine(myself_info_.id.GetData(),false) ; 
	Close();
}

void MainFrame::NotifyMessage(DWORD dwMessage, HICON hIcon, LPCSTR szTips)
{
	memset(&m_noti,0,sizeof(NOTIFYICONDATA));
	m_noti.cbSize=sizeof(NOTIFYICONDATA);
	m_noti.hIcon = hIcon;
	m_noti.hWnd = m_hWnd;
	strcpy(m_noti.szTip, szTips);
	m_noti.uCallbackMessage = WM_NOTIFYICON;
	m_noti.uFlags = NIF_ICON|NIF_MESSAGE|NIF_TIP;
	m_noti.uID=0;
	Shell_NotifyIcon(dwMessage, &m_noti) ; 
}

CString MainFrame::GetNotifyiconToolTip()
{
	CString strtooltip = "";
	CString strID = myself_info_.id.GetData();
	CString strName = myself_info_.nick_name.GetData();
	CString strAddress = g_config.aHostCfg[g_config.nMainHost].szSVRIP;
	USHORT usPort = (USHORT)g_config.aHostCfg[g_config.nMainHost].dwSVRPORT;

	CHAR szData[64] = {0};
	_snprintf( szData, sizeof(szData), "V%0.2f.%04d",NOW_CVERSION, g_config.dwBuildNo );
	CString strVer = szData;

	strtooltip.Format("UPIM:%s \n用户:%s \n连接:%s:%d \n版本:%s", strID, strName, strAddress, usPort, strVer);
	return strtooltip;
}
// 根据不同的状态  设置不同的图标
void MainFrame::SetNotifyIcon( EUSERSTATE state ) 
{
	if (!myself_info_.id.IsEmpty())
	{
		CString strtooltip = GetNotifyiconToolTip();
		switch( state )
		{
		case eUSER_ONLINE:
			{
				NotifyMessage(NIM_MODIFY, m_hIcon, strtooltip);
			}
			break; 
		case eUSER_OFFLINE:
			{
				NotifyMessage(NIM_MODIFY, m_hOffLineIcon, strtooltip);
			}
			break; 
		case eUSER_LIKAI:
			{
				NotifyMessage(NIM_MODIFY, m_hOffLineIcon, strtooltip);
			}
			break; 
		}
	}
}
void MainFrame::SetNotifyIcon()
{
#ifdef _EXE_MODE
	m_hIcon = LoadIcon(AfxGetApp()->m_hInstance, MAKEINTRESOURCE(IDI_MAINFRAME_NEW));
	m_hOffLineIcon = LoadIcon(AfxGetApp()->m_hInstance, MAKEINTRESOURCE(IDI_MAINFRANE_ONLINE)); 
#else
	m_hIcon = LoadIcon(g_hInstance, MAKEINTRESOURCE(IDI_MAINFRAME_NEW));
	m_hOffLineIcon = LoadIcon(g_hInstance, MAKEINTRESOURCE(IDI_MAINFRANE_ONLINE)); 
#endif
	if (!myself_info_.id.IsEmpty())
	{
		CString strtooltip = GetNotifyiconToolTip();
		NotifyMessage(NIM_ADD, m_hIcon, strtooltip); 
	}
}

// 取头像说明：
// 投顾端的投顾头像是在取房间的协议中返回的投顾头像，客户头像需要根据接口取。
// 客户端的客户头像是根据接口取的，投顾头像是取房间的时候取的。

void MainFrame::SetBkInfo()
{
	if (m_pBackground != NULL)
	{
		// 背景图片
		if (g_userconfig.m_bkInfo.m_nBkType == 0)
		{	
			TCHAR szBuf[MAX_PATH] = {0};
#ifndef VER_UPIM_RONGYUN
			_stprintf_s(szBuf, MAX_PATH - 1, _T("file='bg%d.jpg' corner='600,200,1,1'"), g_userconfig.m_bkInfo.m_nBkIndex);
#else
			_stprintf_s(szBuf, MAX_PATH - 1, _T("file='top3.png' corner='600,200,1,1'"));
#endif
			
			m_pBackground->SetBkImage(szBuf);

			bk_image_index_ = g_userconfig.m_bkInfo.m_nBkIndex;
			SkinChangedParam param;
			if (m_pBackground != NULL)
			{
				param.bkcolor = m_pBackground->GetBkColor();
				if (_tcslen(m_pBackground->GetBkImage()) > 0)
					_stprintf_s(szBuf, MAX_PATH - 1, _T("bg%d.jpg"), bk_image_index_);
				param.bgimage = szBuf;
			}
			skin_changed_observer_.Broadcast(param);
		}
		// RGB颜色
		else if (g_userconfig.m_bkInfo.m_nBkType == 1)
		{
			m_pBackground->SetBkImage(_T(""));
			m_pBackground->SetBkColor(g_userconfig.m_bkInfo.dwBackColor);
			m_pBackground->NeedUpdate();

			SkinChangedParam param;
			param.bkcolor = m_pBackground->GetBkColor();
			param.bgimage = m_pBackground->GetBkImage();
			skin_changed_observer_.Broadcast(param);
		}
	}
	return;
}

void MainFrame::SetBkColor(DWORD dwBackColor)
{
	if (m_pBackground != NULL)
	{
		m_pBackground->SetBkImage(_T(""));
		m_pBackground->SetBkColor(dwBackColor);
		m_pBackground->NeedUpdate();

		SkinChangedParam param;
		param.bkcolor = m_pBackground->GetBkColor();
		param.bgimage = m_pBackground->GetBkImage();
		skin_changed_observer_.Broadcast(param);

		// 保存配置
		g_userconfig.m_bkInfo.m_nBkType = 1;
		g_userconfig.m_bkInfo.dwBackColor = dwBackColor;
		SaveUserConfig();
	}
}

void MainFrame::UpdateMyselfInfo(FriendListItemInfo* m_unit, CDuiString AppTitle)
{
	myself_info_ = *m_unit;
	m_AppTitle = AppTitle;
	return;
}

//创建好友节点
void MainFrame::CreateFriendNode()
{
	if (m_vtFriendInfo.size() == 0)
		return;
	if (m_pFriendsList != NULL)
	{
		for (int idex=0; idex<(int)m_vtFriendInfo.size(); idex++)
		{
			if (!m_vtFriendInfo[idex].m_bLoad)
			{
				CString mGroupID = _T("");
				FriendListItemInfo item;
				Node* retrunptr = NULL;

				if (m_vtFriendInfo[idex].type == 4)
					mGroupID.Format("DEPT$%d", m_vtFriendInfo[idex].relation);
				else
					mGroupID.Format("GROUP$%d", m_vtFriendInfo[idex].relation);

				USER_LOG("CreateFriendNode[%d] UID:%s UNAME:%s RE:%d",
					m_vtFriendInfo[idex].type, m_vtFriendInfo[idex].userid, m_vtFriendInfo[idex].nickname, m_vtFriendInfo[idex].relation);

				Node* m_mode = m_pFriendsList->SelectNodeByID(mGroupID);
				if (m_mode)
				{
					item.folder = false;

					if (m_vtFriendInfo[idex].online == 1)
						item.logo = _T("default.png");
					else
						item.logo = _T("default_offline.png");

					item.nodetype = eNode_Single;
					item.nstate = m_vtFriendInfo[idex].online;
					item.type = m_vtFriendInfo[idex].type;
					item.id = m_vtFriendInfo[idex].userid;
					item.numid = m_vtFriendInfo[idex].userid;
					item.Parentid = mGroupID;
					item.nick_name = m_vtFriendInfo[idex].nickname;

					if (g_userconfig.m_nNickType == eNICK_nameandid)
						item.showname.Format("%s(%s)", m_vtFriendInfo[idex].nickname, m_vtFriendInfo[idex].userid);
					else if (g_userconfig.m_nNickType == eNICK_id)
						item.showname = m_vtFriendInfo[idex].userid;
					else if (g_userconfig.m_nNickType == eNICK_name)
						item.showname = m_vtFriendInfo[idex].nickname;

					item.description = m_vtFriendInfo[idex].sign;
					retrunptr = m_pFriendsList->AddNode(item, m_mode);
					if (retrunptr)
						m_vtFriendInfo[idex].m_bLoad = TRUE;
					friends_.push_back(item);
				}
			}
		}
	}
	return;
}

// 分析师登录之后看见的房间节点
void MainFrame::CreateTgRoomNode()
{
	if (m_pFriendsList != NULL)
	{
		FriendListItemInfo item;
		int m_nNodeID = 0;
		CDuiString mRoomID = _T("");
		CDuiString m_szGroupName = _T("");
		if (g_MyClient.m_nUserType == eUser_UPAnalyst)
			m_szGroupName = _T("分析师互动");

		mRoomID.Format("ANALYST$%d", m_nNodeID);
		Node * hasNode = m_pFriendsList->SelectNodeByID(mRoomID.GetData());
		if (hasNode == NULL)
		{
			item.numid = IntToStr(m_nNodeID);
			item.id = mRoomID;
			item.folder = true;
			item.empty = false;
			item.nick_name = m_szGroupName;
			item.showname =  m_szGroupName;
			item.nodetype = eNode_Analyst;
			item.type = eNode_Analyst;

			Node* retrunptr = NULL;
			retrunptr = m_pFriendsList->AddNode(item, NULL);
			if (retrunptr)
				m_pFriendsList->SetChildVisible(retrunptr, true);

			USER_LOG("CreateANALYST[%d] NAME:%s GroupID:%s", m_nNodeID, m_szGroupName.GetData(), mRoomID.GetData());
		}
	}
}

// im2.0客户看见房间下的所有分析师列表
void MainFrame::CreateRoomAnalysNode()
{
	if (m_mapAllClientRoom.size() == 0)
		return;
	FriendListItemInfo item;
	Node* retrunptr = NULL;Node* retrunptr2 = NULL;
	if (m_pFriendsList != NULL)
	{
		int nTgRoomNum = m_mapAllClientRoom.size();
		ACRI_ITER iterator;
		for (iterator = m_mapAllClientRoom.begin(); iterator != m_mapAllClientRoom.end(); iterator++)
		{
			if (!iterator->second.m_bLoad)
			{
				//USER_LOG("CreateRoomAnalysNode PID:%d PNAME:%s PNOTE:%s", iterator->second.nRoomID, iterator->second.szName, iterator->second.szInfo);
				//////////////////////////////////////////////////////////////////////////
				// 先添加房间节点
				CDuiString mTgRoomID = _T("");
				mTgRoomID.Format("ANALYST$%d", iterator->second.m_nRoomID);
				int nRoomID = iterator->second.m_nRoomID;

				item.folder = true;
				item.empty = false;
				item.nodetype = eNode_Analyst;
				item.type = eNode_Analyst;
				item.id = mTgRoomID;
				item.numid = IntToStr(iterator->second.m_nRoomID);
				item.nick_name = iterator->second.m_szRoomName;
				item.showname = iterator->second.m_szRoomName;
				item.totlenum = iterator->second.m_nAnalysNum;
				item.isvip = iterator->second.m_nIsVip;

				// 当前房间在线分析师人数
				item.onlinenum = iterator->second.m_nOnLineAnalysNum ; 

				int nVipRight = m_mapAllClientRoom[nRoomID].m_nVipRight;
				item.hasright = IsHasRights(nVipRight);

				retrunptr = m_pFriendsList->AddNode(item, NULL);
				if (retrunptr)
				{
					m_pFriendsList->SetChildVisible(retrunptr, NULL);
					iterator->second.m_bLoad = TRUE;
					friends_.push_back(item);
				}
				//////////////////////////////////////////////////////////////////////////
				// 在房间节点下面添加所有的分析师节点
				if (retrunptr && iterator->second.m_nAnalysNum > 0)
				{
					int nAllNums = iterator->second.m_nAnalysNum;

					std::vector<ROOMANALYS_INFO>::iterator iter_info;
					for (iter_info = iterator->second.pVtAnalys->begin(); iter_info != iterator->second.pVtAnalys->end(); iter_info++)
					{
						///////////////////////////////////////////////////////////////////
						// 分析师集合  客户获取他们的在线信息需要 
						AnalystInfo pAnalystInfo  ;
						//memset(pAnalystInfo ,0,sizeof(AnalystInfo)) ; 
						CString strAnalystName = _T("") ;
						strAnalystName.Format(_T("%s") ,iter_info->m_szAnalysName) ; 
						pAnalystInfo.AnalystId = iter_info->m_nAnalysID ; 
						//strcpy(pAnalystInfo->AnalystName.GetBuffer(),iter_info->m_szAnalysName); 

						pAnalystInfo.bIsOnLine = iter_info->m_bIsOnLine ; 
						m_vtInfo.push_back(pAnalystInfo) ; 

						CDuiString mTgRoomID = _T("");
						CString strUser = IntToStr(iter_info->m_nAnalysID);
						mTgRoomID.Format("TGUSER$%d$%s", nRoomID, strUser);

						FriendListItemInfo item2;
						// 如果存在分析的头像，那么就设置头像，否则设置默认的图片
						CDuiString strLogoFile = _T("public.png") ;  
						CString strDown = "";		// 下载的头像
						CString strHead = "";		// 在线的正常头像
						CString strHead2 = "";		// 离线时的灰化头像
						strLogoFile.Format("%s\\%d_n.jpg", g_config.szUserHead,  iter_info->m_nAnalysID);

						// 在硬盘中找到了图片 
						if(GetFileAttributes(strLogoFile) != -1 )
						{
							item2.logo = strLogoFile ;  
						}
						else
						{
							item2.logo = _T("public.png");
						}

						item2.nodetype = eNode_Analyst;
						item2.type = eNode_Analyst;
						item2.id = mTgRoomID;
						item2.Parentid = IntToStr(nRoomID);
						item2.numid = strUser;
						item2.nick_name = iter_info->m_szAnalysName;
						item2.showname = iter_info->m_szAnalysName;
						item2.nstate   = iter_info->m_bIsOnLine ; 

						//////////////////////////////////////////////////////////////////////////
						// 擅长品种
						CString strGoodAt = iter_info->m_szGood;
						strGoodAt.Trim(",");
						CString strGoodAtResult = _T("");
						if (!strGoodAt.IsEmpty())
							strGoodAtResult.Format("(擅长:%s)", strGoodAt);
						item2.GoodAt = strGoodAtResult;

						retrunptr2 = m_pFriendsList->AddNode(item2, retrunptr);
						if (retrunptr2)
						{
							m_pFriendsList->SetChildVisible(retrunptr, true);
							iter_info->m_bLoad = TRUE;
							friends_.push_back(item2);
						}
					}
					// 对每个房间的成员进行排序

					//m_pFriendsList->SortItems(PULVCompareFunc ,dwData) ; 
				}
			}
		}
	}
	return;
}

// im2.0投顾自定义分组以及客户列表
void MainFrame::CreatSelfGroup()
{
	if (m_pSelfGroup != NULL)
	{
		FriendListItemInfo item;
		Node* retrunptr = NULL;

		for (int n=0; n<=USERMRG_FENZU; n++)
		{
			if ( !m_groupInfoStc[n].bCreateNode &&m_groupInfoStc[n].bUsed && m_groupInfoStc[n].bGetUserInfo)
			{
				CDuiString mGroupID = _T("");
				mGroupID.Format("SELFGROUP$%d", m_groupInfoStc[n].nGroupID);

				item.folder = true;
				item.empty = false;
				item.nodetype = eNode_Analyst;
				item.type = eNode_Analyst;
				item.id = mGroupID;
				item.numid.Format(_T("%d") ,m_groupInfoStc[n].nGroupID) ; 
				item.nick_name = m_groupInfoStc[n].GroupName;
				item.showname = m_groupInfoStc[n].GroupName;
				item.totlenum = GetGroupClientNums(m_groupInfoStc[n].nGroupID);

				retrunptr = m_pSelfGroup->AddNode(item, NULL);
				if (retrunptr)
				{
					m_groupInfoStc[n].bCreateNode = true;
					//////////////////////////////////////////////////////////////////////////
					// 每个分析师房间下面显示历史联系人
					int nGroupID = m_groupInfoStc[n].nGroupID;
					AddSelfGroupClientNode(nGroupID, retrunptr);
				}
				m_pSelfGroup->SetNodeName(retrunptr,GetGroupClientOnlineNums(m_groupInfoStc[n].nGroupID),GetGroupClientNums(m_groupInfoStc[n].nGroupID)) ; 
			}
		}
	}
	return;
}

// 向自定义分组nGroupID里面增加一个好友strClintID 
void MainFrame::AddSelfGroupClientNode( int nGroupID ,CString strClientID)
{
	CDuiString mGroupID = _T("");
	mGroupID.Format("SELFGROUP$%d",nGroupID);

	// （1）先找到分组ID为nGroupID的分组 
	if( !m_pSelfGroup)
		return ;
	Node* findNode = m_pSelfGroup->GetNodeItem(mGroupID) ; 
	if(!findNode)
		return ; 

	// （2）在该分组的好友列表的最后面添加好友 
	FriendListItemInfo item ; 

	int nRoomID = g_nRoomID_im2_0 ; 
	CDuiString strRoomID = _T(" ") ;
	strRoomID.Format("TGUSER$%d$%s",nRoomID,strClientID) ; 
	FriendList_ITER it ;
	for( it = friends_.begin() ; it != friends_.end() ; it++)
	{
		string s1 =  (it->id).GetData() ;
		string s2 = strRoomID.GetData() ;
		if( strcmp(s1.c_str(),s2.c_str()) == 0 )
		{
			item = *it ;
			break ; 
		}
	}
	if( it == friends_.end()) 
	{
		return ; 
	}
	Node* insertNode = m_pSelfGroup->GetNodeItem(strRoomID); 
	if( !insertNode )
		return ; 

	// 将inserNode节点插入到分组findNode的孩子节点的最后面 
	m_pSelfGroup->AddNode(item,findNode) ; 
	m_pSelfGroup->SetNodeName(findNode,GetGroupClientOnlineNums(nGroupID),findNode->num_children()) ; 
}
// 在分组ID==nGroupID的分组中删除strClientID
void MainFrame::DelSelfGroupClientNode( int nGroupID ,CString strClientID)
{
	// 获取组节点 
	CDuiString mGroupID = _T("");
	mGroupID.Format("SELFGROUP$%d",nGroupID);
	Node* pGroupNode = m_pSelfGroup->SelectNodeByID(mGroupID.GetData()) ; 
	if( !pGroupNode )
		return ; 

	CDuiString mNodeID = _T("");
	int nRoomID = g_nRoomID_im2_0;
	mNodeID.Format("TGUSER$%d$%s", nRoomID, strClientID);
	// 好友节点 
	Node* pChildNode = m_pSelfGroup->SelectNodeByID(mNodeID.GetData());
	if( pChildNode == NULL)
		return ; 

	// 先将分组下面的组成员删除
	m_pSelfGroup->RemoveNode(pChildNode) ; 
	// 再删除分组

	if( pGroupNode->num_children() == 0 )
		m_pSelfGroup->RemoveNode(pGroupNode) ;
	else
	{
		// 统计分组在线人数 
		// 更新分组的标题 
		m_pSelfGroup->SetNodeName(pGroupNode,GetGroupClientOnlineNums(nGroupID),pGroupNode->num_children()) ; 
	}
}

// wuchao add at 20160419 
// 删除分组 
void MainFrame::DeleGroup(CString strGroupName ,CString strGroupID)
{
	CDuiString strID = _T("") ; 
	strID.Format("SELFGROUP$%s",strGroupID) ; 

	if( !m_pSelfGroup)
	{
		return ; 
	}

	Node* findNode = m_pSelfGroup->SelectNodeByID(strID.GetData()) ; 
	if(!findNode)
		return ; 
	std::vector<Node*> pChildList = findNode->GetChildList() ;

	// 删除孩子节点 
	for( int i = 0 ; i < pChildList.size() ;i++)
	{
		m_pSelfGroup->RemoveNode(pChildList.at(i)) ; 
	}

	// 最后删除分组节点 
	m_pSelfGroup->RemoveNode(findNode) ; 

}
// im2.0投顾自定义分组下的客户列表  
void MainFrame::AddSelfGroupClientNode(int nGroupID, Node* nodeptr /* = NULL */)
{
	if (nodeptr == NULL)
		return;
	if (m_vtRoomClient.size() == 0)
		return;
	if (m_pSelfGroup != NULL)
	{
		for (int idex=0; idex<(int)m_vtRoomClient.size(); idex++)
		{
			if (!m_vtRoomClient[idex].m_bGroupLoad && m_vtRoomClient[idex].m_nGroupID == nGroupID)
			{
				CString mGroupID = _T("");
				FriendListItemInfo item;
				Node* retrunptr = NULL;
				int nRoomID = g_nRoomID_im2_0;
				CString strUser = m_vtRoomClient[idex].m_szUPID;
				CDuiString mTgRoomID = _T("");
				mTgRoomID.Format("TGUSER$%d$%s", nRoomID, strUser);
				if( GetFileAttributes(m_vtRoomClient[idex].m_szImgName)!=-1 )
				{
					if(m_vtRoomClient[idex].m_bIsOnLine)
					{
						item.logo = m_vtRoomClient[idex].m_szLNomalImgName ;
					}
					else
						item.logo= m_vtRoomClient[idex].m_szLGrayImgName ; 
				}
				else
				{
					if(m_vtRoomClient[idex].m_bIsOnLine)
						item.logo = _T("public.png") ;
					else
						item.logo = _T("public_g.png") ; 
				}
				item.nodetype = eNode_Analyst;
				item.type = eNode_Analyst;
				item.id = mTgRoomID;
				item.Parentid = IntToStr(nRoomID);
				item.numid = strUser;
				item.nick_name = strUser;

#ifdef NOT_DISPLAY_ID_LIKE_MOBLIE_NUM
				strUser = ConvertIdToNoPhonrNum(strUser);
#endif // NOT_DISPLAY_ID_LIKE_MOBLIE_NUM
				item.showname = strUser;

				retrunptr = m_pSelfGroup->AddNode(item, nodeptr);
				if (retrunptr)
				{
					m_vtRoomClient[idex].m_bGroupLoad = TRUE;
					// m_pSelfGroup->SetChildVisible(nodeptr, true);
					// friends_.push_back(item);
				}
			}
		}
	}
	return;
}

// im2.0投顾看见自己的房间下的所有客户列表
void MainFrame::CreateRoomClientNode()
{
	if (m_vtRoomClient.size() == 0)
		return;
	if (m_pFriendsList != NULL)
	{
		for (int idex=0; idex<(int)m_vtRoomClient.size(); idex++)
		{
			if (!m_vtRoomClient[idex].m_bLoad)
			{
				CString mGroupID = _T("");
				FriendListItemInfo item;
				Node* retrunptr = NULL;
				int nRoomID = g_nRoomID_im2_0;
				CString strUser = m_vtRoomClient[idex].m_szUPID;

				mGroupID.Format("ANALYST$%d", nRoomID);
				Node* m_mode = m_pFriendsList->SelectNodeByID(mGroupID);
				if (m_mode)
				{
					CDuiString mTgRoomID = _T("");
					mTgRoomID.Format("TGUSER$%d$%s", nRoomID, strUser);

					FriendListItemInfo item;
					CString logoName =  _T(" ") ;
					CString strDown  =  _T(" ") ;		// 下载的头像
					CString strHead  =  _T(" ") ;		// 在线的正常头像
					CString strHead2 =  _T(" ") ;		// 离线时的灰化头像
			 
					if( strlen(m_vtRoomClient[idex].m_szUPID) != 0 )
					{
						strDown.Format(_T("%s\\%s.jpg"), g_config.szUserHead,  m_vtRoomClient[idex].m_szUPID);
						strHead.Format(_T("%s\\%s_n.jpg"), g_config.szUserHead,  m_vtRoomClient[idex].m_szUPID);
						strHead2.Format(_T("%s\\%s_g.jpg"), g_config.szUserHead,  m_vtRoomClient[idex].m_szUPID);
					}
					if(GetFileAttributes(strHead) != -1 )
					{
						COPYSTRARRAY(m_vtRoomClient[idex].m_szImgName , strDown) ; 
						COPYSTRARRAY(m_vtRoomClient[idex].m_szLNomalImgName , strHead) ;
						COPYSTRARRAY(m_vtRoomClient[idex].m_szLGrayImgName , strHead2) ;
					}
					if(m_vtRoomClient[idex].m_bIsOnLine && strlen(m_vtRoomClient[idex].m_szLNomalImgName)!=0 )
					{
						logoName.Format(_T("%s"),m_vtRoomClient[idex].m_szLNomalImgName) ; 
						item.logo = logoName ; 
					}
					else 
					{
						if( strlen(m_vtRoomClient[idex].m_szLGrayImgName)!=0)
						{
							logoName.Format(_T("%s"),m_vtRoomClient[idex].m_szLGrayImgName) ;
							item.logo = logoName ; 
						}
					}

					if( item.logo.IsEmpty())
					{
						if( m_vtRoomClient[idex].m_bIsOnLine ) 
							item.logo = _T("public.png");
						else
							item.logo = _T("public_g.png") ; 
					}

					// 将节点的在线状态  加进好友列表集合中 
					item.nstate = m_vtRoomClient[idex].m_bIsOnLine ; 
					item.nodetype = eNode_Analyst;
					item.type = eNode_Analyst;
					item.id = mTgRoomID;
					item.Parentid = IntToStr(nRoomID);
					item.numid = strUser;
					item.nick_name = strUser;

#ifdef NOT_DISPLAY_ID_LIKE_MOBLIE_NUM
					strUser = ConvertIdToNoPhonrNum(strUser);
#endif // NOT_DISPLAY_ID_LIKE_MOBLIE_NUM
					item.showname = strUser;

					retrunptr = m_pFriendsList->AddNode(item, m_mode);
					if (retrunptr)
					{
						m_vtRoomClient[idex].m_bLoad = TRUE;
						m_pFriendsList->SetChildVisible(m_mode, true); 
						friends_.push_back(item);

						// wuchao add at 2016-03-29 
						// 在线的客户 添加到m_vtOnlineClient
						if(m_vtRoomClient[idex].m_bIsOnLine)
						{
							m_vtOnlineClient.push_back(m_vtRoomClient[idex]) ; 
						}
						else
							m_vtOfflineClient.push_back(m_vtRoomClient[idex]) ; 
					}
				}
			}
		}
	}
	int nOnlineNums = GetRoomClientOnlineNums() ;
	SetAnalysNodeNickName(g_nRoomID_im2_0,nOnlineNums,m_vtRoomClient.size()) ;
	USER_LOG("增加房间客户成功") ; 

	return;
}

// im2.0投顾看见自己的房间
void MainFrame::CreateTgRoomNode2()
{
	FriendListItemInfo item;
	Node* retrunptr = NULL;
	int nTgRoomNum = m_mapTgRoomInfo.size();
	TR_ITER iterator;
	int totalClientNumInRoom  = 0 ; 
	totalClientNumInRoom = m_vtRoomClient.size() ;
	for (iterator = m_mapTgRoomInfo.begin(); iterator != m_mapTgRoomInfo.end(); iterator++)
	{
		if (!iterator->second.m_bLoad)
		{
			USER_LOG("CreateTgRoomAccNode PID:%d PNAME:%s PNOTE:%s", iterator->second.nRoomID, iterator->second.szName, iterator->second.szInfo);

			CDuiString mTgRoomID = _T("");
			mTgRoomID.Format("ANALYST$%d", iterator->second.nRoomID);
			g_nRoomID_im2_0 = iterator->second.nRoomID;

			item.folder = true;
			item.empty = false;
			item.nodetype = eNode_Analyst;
			item.type = eNode_Analyst;
			item.id = mTgRoomID;
			item.numid = IntToStr(iterator->second.nRoomID);
			item.nick_name = iterator->second.szName;
			item.showname = iterator->second.szName;
			item.description = iterator->second.szInfo;
			item.onlinenum = GetRoomClientOnlineNums() ; 
			item.totlenum  = totalClientNumInRoom ; 
			retrunptr = m_pFriendsList->AddNode(item, NULL);
			if (retrunptr)
			{
				m_pFriendsList->SetChildVisible(retrunptr, NULL);
				iterator->second.m_bLoad = TRUE;
				friends_.push_back(item);
			}
		}
	}
}

//
void MainFrame::AddTgRoomAccNode()
{
	if (m_pFriendsList != NULL)
	{
		CString mRoomID = _T("ANALYST$0");
		FriendListItemInfo item;
		Node* retrunptr = NULL;

		Node* m_mode = m_pFriendsList->SelectNodeByID(mRoomID);
		if (m_mode)
		{
			int nTgRoomNum = m_mapTgRoomInfo.size();
			TR_ITER iterator;
			for (iterator = m_mapTgRoomInfo.begin(); iterator != m_mapTgRoomInfo.end(); iterator++)
			{
				if (!iterator->second.m_bLoad)
				{
					USER_LOG("CreateTgRoomAccNode PID:%d PNAME:%s PNOTE:%s", iterator->second.nRoomID, iterator->second.szName, iterator->second.szInfo);

					CDuiString mTgRoomID = _T("");
					mTgRoomID.Format("ANALYST$%d", iterator->second.nRoomID);

					item.folder = true;
					item.empty = false;
					item.nodetype = eNode_Analyst;
					item.type = eNode_Analyst;
					item.id = mTgRoomID;
					item.numid = IntToStr(iterator->second.nRoomID);
					item.nick_name = iterator->second.szName;
					item.showname = iterator->second.szName;

					item.description = iterator->second.szInfo;
					retrunptr = m_pFriendsList->AddNode(item, m_mode);
					if (retrunptr)
					{
						m_pFriendsList->SetChildVisible(retrunptr, true);
						iterator->second.m_bLoad = TRUE;
						friends_.push_back(item);
					}
				}
			}
		}
	}
}

//////////////////////////////////////////////////////////////////////////
// 分析师登录之后看见的房间节点
void MainFrame::CreateTgHisNode()
{
	if (m_pFriendsList != NULL)
	{
		FriendListItemInfo item;
		int m_nNodeID = 0;
		CDuiString mRoomID = _T("");
		CDuiString m_szGroupName = _T("");
		if (g_MyClient.m_nUserType == eUser_UPAnalyst)
			m_szGroupName = _T("历史联系人");

		mRoomID.Format("HISANALYST$%d", m_nNodeID);
		Node * hasNode = m_pFriendsList->SelectNodeByID(mRoomID.GetData());
		if (hasNode == NULL)
		{
			item.numid = IntToStr(m_nNodeID);
			item.id = mRoomID;
			item.folder = true;
			item.empty = false;
			item.nick_name = m_szGroupName;
			item.showname =  m_szGroupName;
			item.nodetype = eNode_Analyst;
			item.type = eNode_Analyst;

			Node* retrunptr = NULL;
			retrunptr = m_pFriendsList->AddNode(item, NULL);
			if (retrunptr)
				m_pFriendsList->SetChildVisible(retrunptr, true);
		}
	}
}

void MainFrame::CreateTgRoomHisNode()
{
	if (m_pFriendsList != NULL)
	{
		CString mRoomID = _T("HISANALYST$0");
		FriendListItemInfo item;
		Node* retrunptr = NULL;

		Node* m_mode = m_pFriendsList->SelectNodeByID(mRoomID);
		if (m_mode)
		{
			int nTgRoomNum = m_mapTgRoomInfo.size();
			TR_ITER iterator;
			for (iterator = m_mapTgRoomInfo.begin(); iterator != m_mapTgRoomInfo.end(); iterator++)
			{
				CDuiString mTgRoomID = _T("");
				mTgRoomID.Format("HISANALYST$%d", iterator->second.nRoomID);

				item.folder = true;
				item.empty = false;
				item.nodetype = eNode_Analyst;
				item.type = eNode_Analyst;
				item.id = mTgRoomID;
				item.numid = IntToStr(iterator->second.nRoomID);
				item.nick_name = iterator->second.szName;
				item.showname = iterator->second.szName;

				item.description = iterator->second.szInfo;
				retrunptr = m_pFriendsList->AddNode(item, m_mode);
				if (retrunptr)
					m_pFriendsList->SetChildVisible(retrunptr, true);

				//////////////////////////////////////////////////////////////////////////
				// 每个分析师房间下面显示历史联系人
				long Roomid = iterator->second.nRoomID;
				AddTgRoomClientNode(Roomid, retrunptr);
			}
		}
	}
}

void MainFrame::AddTgRoomClientNode(long nRoomID, Node* nodeptr /* = NULL */)
{
	if (nodeptr == NULL)
		return;

	std::vector<CString> vtUsers;
	if(!g_SqliteOp.QueryAllAnalystUser(vtUsers, eRecord_TgRoom, nRoomID))
		return ;

	for(int index=0; index < (int)vtUsers.size(); ++index)
	{
		CString strUser = vtUsers[index];
		if (strUser.CompareNoCase(g_MyClient.m_strUserid) == 0)
			;
		else
		{
			CDuiString mTgRoomID = _T("");
			mTgRoomID.Format("TGUSER$%d$%s", nRoomID, strUser);

			FriendListItemInfo item;
			item.logo = _T("public.png");
			item.nodetype = eNode_Analyst;
			item.type = eNode_Analyst;
			item.id = mTgRoomID;
			item.Parentid = IntToStr(nRoomID);
			item.numid = strUser;
			item.nick_name = strUser;

#ifdef NOT_DISPLAY_ID_LIKE_MOBLIE_NUM
			strUser = ConvertIdToNoPhonrNum(strUser);
#endif // NOT_DISPLAY_ID_LIKE_MOBLIE_NUM
			item.showname = strUser;

			m_pFriendsList->AddNode(item, nodeptr);
			friends_.push_back(item);
		}
	}
}

//公众号节点
void MainFrame::CreatePublicNode()
{
	if (m_pFriendsList != NULL)
	{
		FriendListItemInfo item;
		int m_nPublicID = 0;
		CDuiString mPublicID = _T("");
		CDuiString m_szGroupName = _T("");
		if (g_MyClient.IsClientLoginIn())
			m_szGroupName = _T("公众号服务");
		else
			m_szGroupName = _T("我的公众号客户");

		mPublicID.Format("PUBLIC$%d", m_nPublicID);
		Node * hasNode = m_pFriendsList->SelectNodeByID(mPublicID.GetData());
		if (hasNode == NULL)
		{
			item.numid = IntToStr(m_nPublicID);
			item.id = mPublicID;
			item.folder = true;
			item.empty = false;
			item.nick_name = m_szGroupName;
			item.showname =  m_szGroupName;
			item.nodetype = eNode_Public;
			item.type = eList_Public;

			m_pFriendsList->AddNode(item, NULL);

			USER_LOG("CreatePUBLIC[%d] NAME:%s GroupID:%s", m_nPublicID, m_szGroupName.GetData(), mPublicID.GetData());
		}
	}
}


void MainFrame::ModifyDisRecentInfo(CDuiString m_strUserID, CDuiString m_strUserName, CDuiString m_LastWord, int n_Lastwordtime, int nRoomID)
{
	CDuiString mNodeID = _T("");
	CString strUser = m_strUserName.GetData();
	int nlRoomID = g_nRoomID_im2_0;
	mNodeID.Format("TGUSER$%d$%s", nRoomID, m_strUserID);
	Node* m_nodeuser1 = NULL;
	Node* m_nodeuser2 = NULL;
	//////////////////////////////////////////////////////////////////////////
	// 好友列表中的节点
	if (m_pFriendsList)
	{
		m_nodeuser1 = m_pFriendsList->SelectNodeByID(mNodeID.GetData());
		if (m_nodeuser1)
			m_pFriendsList->SetNodeLastWord(m_nodeuser1, m_LastWord.GetData(), n_Lastwordtime );
	}

	//////////////////////////////////////////////////////////////////////////
	// 自定义分组中的节点
	if (m_pSelfGroup)
	{
		m_nodeuser2 = m_pSelfGroup->SelectNodeByID(mNodeID.GetData());
		if (m_nodeuser2)
			m_pSelfGroup->SetNodeLastWord(m_nodeuser2, m_LastWord.GetData(), n_Lastwordtime );
	}

	//////////////////////////////////////////////////////////////////////////
	// 最近联系人节点
	if (m_pRecentLinkList)
	{
		CreateRecentNode(m_strUserID, m_strUserName, m_LastWord, n_Lastwordtime);
	}

	return;
}

//////////////////////////////////////////////////////////////////////////
// 最近联系人 创建最近联系人 列表 
// m_strUserID  好友ID 
// m_strUserID  自己的ID 
// m_lastWord   聊天记录中最后一条信息 
// n_Lastwordtime 发表事件 
void MainFrame::CreateRecentNode(CDuiString m_strUserID, CDuiString m_strUserName, CDuiString m_LastWord, long n_Lastwordtime)
{
	// 先查找此ID是否已经在列表中
	//	1.若是在首位，直接更新最后聊天时间以及最后聊天记录
	//	2.若是在其他位置，删除它，再重新添加一个
	if (m_pRecentLinkList != NULL)
	{
		CDuiString mNodeID = _T("");
		CString strUser = m_strUserName.GetData();
		int nRoomID = g_nRoomID_im2_0;
		mNodeID.Format("TGUSER$%d$%s", nRoomID, m_strUserID);

		Node* m_nodeuser = m_pRecentLinkList->SelectNodeByID(mNodeID.GetData());
		if (m_nodeuser)
			m_pRecentLinkList->SetNodeLastWord(m_nodeuser, m_LastWord.GetData(), n_Lastwordtime );
		else
		{
			FriendListItemInfo item;
			int iSize = m_vtRoomClient.size() ;
			item.logo = _T("public.png");
			for( int idex = 0 ; idex < iSize ; idex++)
			{
				if( strcmp(m_vtRoomClient[idex].m_szUPID , (m_strUserID.GetData()) ) == 0 )
				{
					CDuiString strHead = _T(" ") ;
					strHead.Format("%s\\%s.jpg", g_config.szUserHead,  m_vtRoomClient[idex].m_szUPID);
					CDuiString strOnlinePicName= _T(" ") ;
					CDuiString strOfflinePicName= _T(" ") ;
					strOnlinePicName.Format("%s\\%s_n.jpg", g_config.szUserHead,  m_vtRoomClient[idex].m_szUPID);
					strOfflinePicName.Format("%s\\%s_g.jpg", g_config.szUserHead,  m_vtRoomClient[idex].m_szUPID);
					if( GetFileAttributes(strHead) != -1)
					{
						if( m_vtRoomClient[idex].m_bIsOnLine == 1 )
						{
							item.logo =  strOnlinePicName;
						}
						else
						{
							item.logo =  strOfflinePicName;
						}
					}
					else
					{
						if( m_vtRoomClient[idex].m_bIsOnLine == 1 )
						{
							item.logo =  "public.png";
						}
						else
						{
							item.logo =  "public_g.png";
						}
					}
					break ; 
				}
			}

			item.nodetype = eNode_Analyst;
			item.type = eNode_Analyst;
			item.id = mNodeID;
			item.numid = m_strUserID;
			item.nick_name = m_strUserName;

#ifdef NOT_DISPLAY_ID_LIKE_MOBLIE_NUM
			strUser = ConvertIdToNoPhonrNum(m_strUserID.GetData());
#endif // NOT_DISPLAY_ID_LIKE_MOBLIE_NUM

			item.showname = strUser;
			item.lastwordtime = n_Lastwordtime;
			item.Lastword = m_LastWord;

			m_pRecentLinkList->AddNode(item, NULL);
		}
	}
	return;
}


//公众号
void MainFrame::AddPublicAccNode()
{
	if (m_pFriendsList != NULL)
	{
		CString mGroupID = _T("PUBLIC$0");
		FriendListItemInfo item;
		Node* retrunptr = NULL;

		Node* m_mode = m_pFriendsList->SelectNodeByID(mGroupID);
		if (m_mode)
		{
			int nPubNum = m_mapPublicInfo.size();
			PUB_ITER iterator;
			for (iterator = m_mapPublicInfo.begin(); iterator != m_mapPublicInfo.end(); iterator++)
			{
				if (!iterator->second.m_bLoad)
				{
					USER_LOG("CreatePublicAccNode PID:%d PNAME:%s PNOTE:%s", iterator->second.nPublicID, iterator->second.szName, iterator->second.szInfo);

					CDuiString mPublicID = _T("");
					mPublicID.Format("PUBLIC$%d", iterator->second.nPublicID);

					if (g_MyClient.IsClientLoginIn())
					{
						item.folder = false;
					}
					else
					{
						item.folder = true;
						item.empty = false;
					}
					item.logo = _T("public.png");
					item.nodetype = eNode_Public;
					item.type = eList_Public;
					item.id = mPublicID;
					item.numid = IntToStr(iterator->second.nPublicID);
					item.nick_name = iterator->second.szName;
					item.showname = iterator->second.szName;

					item.description = iterator->second.szInfo;
					retrunptr = m_pFriendsList->AddNode(item, m_mode);
					if (retrunptr)
					{
						iterator->second.m_bLoad = TRUE;
						friends_.push_back(item);
					}
				}
			}
		}
	}
	return;
}

void MainFrame::AddStdGroupNode()
{
	USER_GROUP_INFO m_groupunit = {0};
	// 	m_groupunit.m_nGroupID = 0;
	// 	COPYSTRARRAY(m_groupunit.m_szGroupName, STR_CLIENT);			// 我的客户
	// 	m_vtUserGroup.push_back(m_groupunit);
	/*
	m_groupunit.m_nGroupID = -1;
	COPYSTRARRAY(m_groupunit.m_szGroupName, STR_FRIEND);			// 我的好友
	m_vtUserGroup.push_back(m_groupunit);

	m_groupunit.m_nGroupID = -2;
	COPYSTRARRAY(m_groupunit.m_szGroupName, STR_STRANGER);			// 陌生人 
	m_vtUserGroup.push_back(m_groupunit);

	m_groupunit.m_nGroupID = -3;
	COPYSTRARRAY(m_groupunit.m_szGroupName, STR_BLACKLIST);			// 黑名单
	m_vtUserGroup.push_back(m_groupunit);
	*/
	return;
}

//添加分组节点
void MainFrame::CreateGroupNode()
{
	if (m_pFriendsList != NULL)
	{
		FriendListItemInfo item;
		int n = m_vtUserGroup.size();
		for (std::vector<USER_GROUP_INFO>::iterator iterator = m_vtUserGroup.begin(); iterator != m_vtUserGroup.end() && !iterator->m_bCreateNode; ++iterator)
		{
			CDuiString mGroupID = _T("");
			mGroupID.Format("GROUP$%d",iterator->m_nGroupID);

			//USER_LOG("CreateGROUP[%d] NAME:%s GroupID:%s", iterator->m_nGroupID, iterator->m_szGroupName, mGroupID);

			item.numid = IntToStr(iterator->m_nGroupID);
			item.nodetype = eNode_Single;
			item.id = mGroupID;
			item.folder = true;
			item.empty = false;
			Node* retrunptr = NULL;
			item.nick_name = iterator->m_szGroupName;
			item.showname =  iterator->m_szGroupName;
			if (iterator->m_nParentID == 0)
			{
				retrunptr = m_pFriendsList->AddNode(item, NULL);
				if (retrunptr)
				{
					// 展开节点
					//m_pFriendsList->SetChildVisible(retrunptr, true);
					iterator->m_bCreateNode = TRUE;
				}
			}
			else
			{
				CString mPreGroupID = _T("");
				mPreGroupID.Format("GROUP$%d",iterator->m_nParentID);

				Node* m_mode = m_pFriendsList->SelectNodeByID(mPreGroupID);
				if (m_mode)
				{
					retrunptr = m_pFriendsList->AddNode(item, m_mode);
					if (retrunptr)
						iterator->m_bCreateNode = TRUE;
				}
			}
			friends_.push_back(item);
		}
	}
	return;
}

//添加部门节点
void MainFrame::CreateDeptNode()
{
	if (m_pFriendsList != NULL)
	{
		FriendListItemInfo item;
		int n = m_vtUserGroup.size();
		for (std::vector<USER_GROUP_INFO>::iterator iterator = m_vtDeptInfo.begin(); iterator != m_vtDeptInfo.end() && !iterator->m_bCreateNode; ++iterator)
		{
			CDuiString mGroupID = _T("");
			mGroupID.Format("DEPT$%d",iterator->m_nGroupID);

			item.numid = IntToStr(iterator->m_nGroupID);
			item.id = mGroupID;
			item.folder = true;
			item.empty = false;
			Node* retrunptr = NULL;
			item.nick_name = iterator->m_szGroupName;
			item.showname =  iterator->m_szGroupName;
			if (iterator->m_nParentID == 0)
			{
				retrunptr = m_pFriendsList->AddNode(item, NULL);
				if (retrunptr)
				{
					// 展开节点
					m_pFriendsList->SetChildVisible(retrunptr, true);
					iterator->m_bCreateNode = TRUE;
				}
			}
			else
			{
				CString mPreGroupID = _T("");
				mPreGroupID.Format("DEPT$%d", iterator->m_nParentID);
				Node* m_mode = m_pFriendsList->SelectNodeByID(mPreGroupID);
				if (m_mode)
				{
					retrunptr = m_pFriendsList->AddNode(item, m_mode);
					if (retrunptr)
						iterator->m_bCreateNode = TRUE;
				}
			}
			friends_.push_back(item);
		}
	}
	return;
}

void MainFrame::UpdateFriendsList()
{
	if (m_pFriendsList != NULL)
	{
		CreateGroupNode();
		CreateDeptNode();
		CreateFriendNode();
	}
}

// 添加群组节点
void MainFrame::CreateRoomNode()
{
	if (m_pGroupsList != NULL)
	{
		int n = m_mapChatRoom.size();
		if (n > 0)
		{
			GCR_ITER iterator;

			for (iterator = m_mapChatRoom.begin(); iterator != m_mapChatRoom.end() ; iterator++)
			{
				if (!iterator->second.m_bLoadFlag)
				{
					FriendListItemInfo item;
					item.folder = false;
					item.logo = _T("groups.png");

					CDuiString mRoomID = _T("");
					mRoomID.Format("ROOM$%d", iterator->second.m_nRoomId);
					item.nodetype = eNode_Group;
					item.id = mRoomID;
					item.numid = IntToStr(iterator->second.m_nRoomId);

					item.nick_name = iterator->second.m_szRoomName;
					item.showname =  iterator->second.m_szRoomName;
					item.groupnote = iterator->second.m_szNote;
					item.roomtype = iterator->second.m_nRoomType;

					item.description = _T("");

					Node* retrunptr = NULL;
					retrunptr = m_pGroupsList->AddNode(item, NULL);
					if (retrunptr)
						iterator->second.m_bLoadFlag = TRUE;

					groups_.push_back(item);
				}
			}
		}
	}
}

void MainFrame::UpdateGroupsList()
{
	if (m_pGroupsList != NULL)
	{
		CreateRoomNode();
	}
}

void MainFrame::UpdateRecentList()
{
	CRecentLinkUI* pRecentList = static_cast<CRecentLinkUI*>(m_PaintManager.FindControl(kRecentListControlName));
	if (pRecentList != NULL)
	{}
}

void MainFrame::OnPrepare(TNotifyUI& msg)
{
	SetBkInfo();
	//USER_LOG("MainFrame::OnPrepare()") ; 
	if (VER_UPIM_NUMS == 4)
	{
		//USER_LOG("添加定时器：客户每隔20秒主动从服务器获取分析师的状态") ;
		AddTimer(T_REFRESHSTATUS, eTE_RefreshStatus);
	}

	UpdateFriendsList();
	UpdateGroupsList();
	UpdateRecentList();
}

//主界面的最小化隐藏窗口
void MainFrame::OnBtn_Min(TNotifyUI& msg)
{
	ShowWindow(FALSE);
}

void MainFrame::OnBtn_Max(TNotifyUI& msg)
{
	SendMessage(WM_SYSCOMMAND, SC_MAXIMIZE, 0);
}

void MainFrame::OnBtn_Restore(TNotifyUI& msg)
{
	SendMessage(WM_SYSCOMMAND, SC_RESTORE, 0);
}

void MainFrame::OnBtn_Close(TNotifyUI& msg)
{
	OnExit();
}
void MainFrame::OnBtn_SearchFriend( TNotifyUI &msg)
{
	CRichEditUI* search_edit = static_cast<CRichEditUI*>(m_PaintManager.FindControl(kSearchEditControlName)); 
	CDuiString strClientName = _T(" ") ; 
	if( m_pSearch_edit )
	{
		strClientName = m_pSearch_edit->GetText() ; 
		if( strClientName.IsEmpty())
		{	
			::MessageBox(NULL,_T("请输入您要查找的好友"),_T("警告") ,MB_OK) ;
			return ;
		}
		FriendList_ITER itBegin = friends_.begin() ; 
		while( itBegin != friends_.end())
		{
			if( strcmp(strClientName.GetData(),itBegin->numid.GetData()) == 0 )
			{
				// 找到了就打开对话框 
				OpenChatDiolog(itBegin->id, itBegin ) ;  
				break; 
			}
			itBegin++ ; 
		}
		if( itBegin == friends_.end())
		{
			// 没有找到 
			CString strFind  = _T(" ") ;
			strFind.Format(_T("没有找到客户（%s）的任何信息！"),strClientName.GetData()) ; 
			::MessageBox(NULL,strFind,_T("提示") ,MB_OK) ; 
			ShowWindow(TRUE,TRUE);
			return ; 
		}
	}
}
void MainFrame::OnBtn_ChangeBkSkin(TNotifyUI& msg)
{
	if (m_pBackground != NULL)
	{
		TCHAR szBuf[MAX_PATH] = {0};
		++bk_image_index_;
		if (kBackgroundSkinImageCount < bk_image_index_)
			bk_image_index_ = 0;

		_stprintf_s(szBuf, MAX_PATH - 1, _T("file='bg%d.jpg' corner='600,200,1,1'"), bk_image_index_);
		m_pBackground->SetBkImage(szBuf);

		SkinChangedParam param;
		if (m_pBackground != NULL)
		{
			param.bkcolor = m_pBackground->GetBkColor();
			if (_tcslen(m_pBackground->GetBkImage()) > 0)
			{
				_stprintf_s(szBuf, MAX_PATH - 1, _T("bg%d.jpg"), bk_image_index_);
			}

			param.bgimage = szBuf;
		}
		skin_changed_observer_.Broadcast(param);

		// 保存配置
		g_userconfig.m_bkInfo.m_nBkType = 0;
		g_userconfig.m_bkInfo.m_nBkIndex = bk_image_index_;
		SaveUserConfig();
	}
}

void MainFrame::OnBtn_UserMrg(TNotifyUI& msg)
{
	//CUserMrgDialog *g_UserMrgDialog = new CUserMrgDialog(eCreate_Node);
	m_pUserMrgDialog = new CUserMrgDialog(eCreate_Node);
	m_pUserMrgDialog->Create(NULL, "", UI_WNDSTYLE_FRAME | WS_POPUP, NULL, 0, 0, 600, 800);
	m_pUserMrgDialog->CenterWindow();
	m_pUserMrgDialog->ShowModal() ; 
	return;
}

void MainFrame::CreateMsgNotifyDlg()
{
	CMsgNotifyDialog *g_MsgNotifyDialog = new CMsgNotifyDialog(eCreate_Node);
	g_MsgNotifyDialog->Create(NULL, "", UI_WNDSTYLE_FRAME | WS_POPUP, NULL, 0, 0, 600, 800);
	g_MsgNotifyDialog->CenterWindow();
	return;
}

//////////////////////////////////////////////////////////////////////////
// wuchao add at 2016-04-05 
// 点击好友搜索栏 切换界面 
//////////////////////////////////////////////////////////////////////////
void MainFrame::SwitchTable()
{
}
void MainFrame::Notify(TNotifyUI& msg)
{
	if (_tcsicmp(msg.sType, _T("windowinit")) == 0)
	{
		OnPrepare(msg);
	}
	// 响应得到焦点事件  wuchao add at 2016-04-05 
	else if( _tcsicmp(msg.sType,_T("setfocus")) == 0 )
	{
		if(_tcsicmp(msg.pSender->GetName(), kSearchEditControlName) == 0)
		{ 
			if (m_pControlSwitch && m_pControlSwitch->GetCurSel() != 0)
			{
				m_nSwitchTab = 0 ; 
				m_pControlSwitch->SelectItem(0);
			}
			else
			{
				m_nSwitchTab = 1 ; 
				m_pControlSwitch->SelectItem(1);
				if( m_pSearch_edit_friend)
				{
					m_pSearch_edit_friend->SetText(_T("")) ; 
					m_pSearch_edit_friend->SetFocus() ; 
				}
			}
		}
	}
	else if (_tcsicmp(msg.sType, _T("killfocus")) == 0)
	{
		CDuiString strtmo = msg.pSender->GetName();
		if (_tcsicmp(msg.pSender->GetName(), kSignatureControlName) == 0)
		{
			msg.pSender->SetVisible(false);
			CControlUI* signature_tip = m_PaintManager.FindControl(kSignatureTipsControlName);
			if (signature_tip != NULL)
			{
				CRichEditUI* signature = static_cast<CRichEditUI*>(msg.pSender);
				if (signature != NULL)
				{
					CDuiString m_strNowSignature = signature->GetText();
					if (m_strNowSignature.IsEmpty())
					{
						signature_tip->SetText("编辑个性签名");
						ModifySign("");
					}
					else
					{
						signature_tip->SetText(m_strNowSignature);
						ModifySign(m_strNowSignature);
					}

				}
				signature_tip->SetVisible(true);
			}
		}
		else if (_tcsicmp(msg.pSender->GetName(), kSearchEditControlName) == 0)
		{
			msg.pSender->SetVisible(false);
			CControlUI* search_tip = static_cast<CRichEditUI*>(m_PaintManager.FindControl(kSearchEditTipControlName));
			if (search_tip != NULL)
			{
				if (m_pSearch_edit != NULL)
				{
					if( m_nSwitchTab == 0 )
						search_tip->SetText("搜索联系人");
				}
				search_tip->SetVisible(true);
			}
		}
	}

	else if (_tcsicmp(msg.sType, _T("click")) == 0)
	{
		if (msg.pSender == m_pCloseSysBtn)
			OnExit();
		else if (msg.pSender == m_pMinSysBtn)
			OnBtn_Min(msg);
		else if (msg.pSender == m_pMaxSysBtn)
			OnBtn_Max(msg);
		else if (msg.pSender == m_pRestoreSysBtn)
			OnBtn_Restore(msg);
		else if( msg.pSender == m_pSearchBtn )
		{
			//OnBtn_SearchFriend(msg) ; 
		}
		//////////////////////////////////////////////////////////////////////////
		// 左侧栏~
		else if (_tcsicmp(msg.pSender->GetName(), kHideLeftMainPannelControlName) == 0)
		{
			CControlUI* left_main_pannel = m_PaintManager.FindControl(kLeftMainPannelControlName);
			CControlUI* hide_left_main_pannel = m_PaintManager.FindControl(kHideLeftMainPannelControlName);
			CControlUI* show_left_main_pannel = m_PaintManager.FindControl(kShowLeftMainPannelControlName);
			if ((left_main_pannel != NULL) && (show_left_main_pannel != NULL) && (hide_left_main_pannel != NULL))
			{
				hide_left_main_pannel->SetVisible(false);
				left_main_pannel->SetVisible(false);
				show_left_main_pannel->SetVisible(true);
			}
		}
		else if (_tcsicmp(msg.pSender->GetName(), kShowLeftMainPannelControlName) == 0)
		{
			CControlUI* left_main_pannel = m_PaintManager.FindControl(kLeftMainPannelControlName);
			CControlUI* hide_left_main_pannel = m_PaintManager.FindControl(kHideLeftMainPannelControlName);
			CControlUI* show_left_main_pannel = m_PaintManager.FindControl(kShowLeftMainPannelControlName);
			if ((left_main_pannel != NULL) && (show_left_main_pannel != NULL) && (hide_left_main_pannel != NULL))
			{
				hide_left_main_pannel->SetVisible(true);
				left_main_pannel->SetVisible(true);
				show_left_main_pannel->SetVisible(false);
			}
		}
		else if (_tcsicmp(msg.pSender->GetName(), kSignatureTipsControlName) == 0)
		{
			msg.pSender->SetVisible(false);
			CRichEditUI* signature = static_cast<CRichEditUI*>(m_PaintManager.FindControl(kSignatureControlName));
			if (signature != NULL)
			{
				CDuiString m_strNowSignature = msg.pSender->GetText();
				if (m_strNowSignature.CompareNoCase("编辑个性签名") == 0)
					m_strNowSignature = "";
				signature->SetText(m_strNowSignature);
				signature->SetVisible(true);
				signature->SetFocus();
			}

			if (m_pSearch_edit != NULL)
			{
				m_pSearch_edit->SetText("");
				m_pSearch_edit->SetVisible(true);
				m_pSearch_edit->SetFocus() ; 
			}

		}
		else if (_tcsicmp(msg.pSender->GetName(), kSearchEditTipControlName) == 0)
		{
			msg.pSender->SetVisible(false);
			CRichEditUI* search_edit = static_cast<CRichEditUI*>(m_PaintManager.FindControl(kSearchEditControlName));
			if (m_pSearch_edit != NULL)
			{
				m_pSearch_edit->SetText("");
				m_pSearch_edit->SetVisible(true);
				m_pSearch_edit->SetFocus() ; 
			}
		}
		else if( _tcsicmp(msg.pSender->GetName(), _T("mainframe_search_edit_friend")) == 0 )
		{
			msg.pSender->SetVisible(false);
			if (m_pSearch_edit_friend != NULL)
			{
				m_pSearch_edit_friend->SetText("");
				m_pSearch_edit_friend->SetVisible(true);
				m_pSearch_edit_friend->SetFocus() ; 
			}
		}
		else if (_tcsicmp(msg.pSender->GetName(), kChangeBkSkinControlName) == 0)
		{
			OnBtn_ChangeBkSkin(msg);
		}
		else if (_tcsicmp(msg.pSender->GetName(), _T("mainframe_usermrg")) == 0)
		{
			OnBtn_UserMrg(msg);
		}
		else if (_tcsicmp(msg.pSender->GetName(), kChangeColorSkinControlName) == 0)
		{
			CDuiRect rcWindow;
			GetWindowRect(m_hWnd, &rcWindow);
			rcWindow.top = rcWindow.top + msg.pSender->GetPos().bottom;
			new ColorSkinWindow(this, rcWindow);
		}
		else if( _tcsicmp( msg.pSender->GetName() ,_T("mainframe_closebtn_friend")) == 0 ) 
		{
			if(m_pControlSwitch)
			{
				m_nSwitchTab = 0 ; 
				m_pControlSwitch->SelectItem(0) ; 

				ClearFindFriendList() ; 
			}
		}
		//主菜单
		else if (_tcsicmp(msg.pSender->GetName(), kMainMenuControlName) == 0)
			OnMainMenuClick(msg);
		//消息管理器
		else if (_tcsicmp(msg.pSender->GetName(), kHisMessageControlName) == 0)
			OnMenu_FriendHistroy();
		// 状态图标菜单
		else if (_tcsicmp(msg.pSender->GetName(), kStateButtonControlName) == 0)
			OnStateMenuClick(msg);
	}
	else if (_tcsicmp(msg.sType, _T("timer")) == 0)
	{
		return OnTimer(msg);
	}
	else if (_tcsicmp(msg.sType, _T("selectchanged")) == 0)
	{
		if (_tcsicmp(msg.pSender->GetName(), kFriendButtonControlName) == 0)
		{
			if (m_pTabControl && m_pTabControl->GetCurSel() != 0)
			{
				m_pTabControl->SelectItem(0);
				UpdateFriendsList();
			}
		}
		else if (_tcsicmp(msg.pSender->GetName(), kGroupButtonControlName) == 0)
		{
			if (m_pTabControl && m_pTabControl->GetCurSel() != 1)
			{
				m_pTabControl->SelectItem(1);
				UpdateGroupsList();
			}
		}
		else if (_tcsicmp(msg.pSender->GetName(), kRecentButtonControlName) == 0)
		{
			if (m_pTabControl && m_pTabControl->GetCurSel() != 2)
			{
				m_pTabControl->SelectItem(2);
				UpdateRecentList();
			}
		}
	}
	//CUIList双击消息
	else if (_tcsicmp(msg.sType, _T("itemactivate")) == 0)
	{
		OnListItemActivate(msg);
	}
	//CUIList单击消息
	else if (_tcsicmp(msg.sType, _T("itemclick")) == 0)
	{
		OnListItemClick(msg);
	}
	// CUIList控件会回传rbuttondown消息，此处是右键消息
	else if (_tcsicmp(msg.sType, _T("rbuttondown")) == 0)
	{
		OnUserListRClick(msg);
	}
}

/////////////////////////////////////////////////////////////////////
// wuchao add at 2016-04-06 
// 清除找到的好友列表
/////////////////////////////////////////////////////////////////////
void MainFrame::ClearFindFriendList() 
{
	if(m_pFindFriendList)
	{
		m_pFindFriendList->RemoveAll() ; 

		m_pFindFriendList->allNodeInfo.clear() ; 
	}


}
// 创建分组
void MainFrame::OnMenu_CreateSubGroup()
{
	if (m_MainSelectNode.lpNode != NULL)
	{
		if (m_pFriendsList)
		{
			FriendListItemInfo item;
			int m_nPublicID = 1;
			CDuiString mPublicID = _T("");
			CDuiString m_szGroupName = _T("创建分组");
			mPublicID.Format("SELF$%d", m_nPublicID);

			item.numid = IntToStr(m_nPublicID);
			item.id = m_nPublicID;
			item.folder = true;
			item.empty = false;
			Node* retrunptr = NULL;
			item.nick_name = m_szGroupName;
			item.showname =  m_szGroupName;

			retrunptr = m_pFriendsList->AddNode(item, (Node*)m_MainSelectNode.lpNode);
			USER_LOG("CreatePUBLIC[%d] NAME:%s GroupID:%s", m_nPublicID, m_szGroupName.GetData(), mPublicID.GetData());
		}
	}
}

// 修改分组
void MainFrame::OnMenu_ModifySubGroup()
{}
// 删除分组
void MainFrame::OnMenu_DeleteSubGroup()
{}

// 用户上线
void MainFrame::OnMenu_UserOnline()
{
	//只是为了让程序崩溃  
	//*(int*)0 = 1234;  

	// wuchao modify at 2016-03-25   
	// 设置上线了  就要启动定时器  避免上次设置下线  
	// 删除了定时器  而使定时器失效 
	AddTimer(T_CONNECT,eTE_Connect) ; 

	// wuchao add 
	SendStateNotify(eUSER_ONLINE) ; // 发送上线消息给好友 
#ifndef VER_UPIM_RONGYUN
	if (!g_MyClient.m_bConnect)
		ReConnect();
#endif

	SetMainFace(eUSER_ONLINE) ; 
	return;
}

// 用户离线
void MainFrame::OnMenu_UserOffline()
{

	//// wuchao modify at 2016-03-25  
	//// 人为设置下线  关闭定时器 
	//KillTimer(m_hWnd,T_CONNECT) ; 

	//if (g_MyClient.m_bConnect)
	//{
	//	SendStateNotify(MYSTATE_OFFLINE);
	//	SetMainFace(eUSER_OFFLINE) ; 
	//	g_MyClient.StopClient();
	//}
	
	OnMenu_UserLiKai() ; 
	return;
}

// 用户离开
void MainFrame::OnMenu_UserLiKai()
{
	// wuchao modify at 2016-03-25  
	// 人为设置下线  关闭定时器 
	KillTimer(m_hWnd,T_CONNECT) ; 
	SetMainFace(eUSER_LIKAI);
}

void MainFrame::OnMenu_FriendOpen()
{
	//此处是获取不到节点信息的，使用点击时候存储的点击node信息
	memset(g_szOpenUID, 0, sizeof(g_szOpenUID));
	strncpy(g_szOpenUID, m_MainSelectNode.m_szID, strlen(m_MainSelectNode.m_szID));
	g_pFrame->PostMessage(UM_USER_OPENWND, (WPARAM)eMSG_Single, (LPARAM)0);
	return;
}

void MainFrame::OnMenu_GroupOpen()
{
	//此处是获取不到节点信息的，使用点击时候存储的点击node信息
	memset(g_szOpenUID, 0, sizeof(g_szOpenUID));
	strncpy(g_szOpenUID, m_MainSelectNode.m_szID, strlen(m_MainSelectNode.m_szID));
	g_pFrame->PostMessage(UM_USER_OPENWND, (WPARAM)eMSG_Group, (LPARAM)0);
	return;
}

void MainFrame::OnMenu_GroupQuit()
{
	g_MyClient.SendGcLeaveReq(atol(m_MainSelectNode.m_szID), m_MainSelectNode.m_nGroupType);
	return;
}

void MainFrame::OnMenu_FriendInfo()
{
	return;
}

void MainFrame::OnMenu_FriendHistroy()
{
	CHistoryMsgDialog* pHistoryMsgDialog = new CHistoryMsgDialog(GetMainBkImage(), GetMainBkColor());
	if( pHistoryMsgDialog == NULL )
		return;

	pHistoryMsgDialog->Create(NULL, "消息管理器", UI_WNDSTYLE_FRAME | WS_POPUP, NULL, 0, 0, 600, 800);
	g_pFrame->skin_changed_observer_.AddReceiver(pHistoryMsgDialog);
	pHistoryMsgDialog->SetIcon(IDI_MAINFRAME_NEW);

	pHistoryMsgDialog->CenterWindow();
	::ShowWindow(*pHistoryMsgDialog, SW_SHOW);

	return;
}

CDuiString MainFrame::GetMainBkImage()
{
	CDuiString strBkImage = _T("");
	TCHAR szBuf[MAX_PATH] = {0};
	if (_tcslen(m_pBackground->GetBkImage()) > 0)
	{
		_stprintf_s(szBuf, MAX_PATH - 1, _T("bg%d.jpg"), bk_image_index_);
		
	}
	strBkImage = szBuf;
	return strBkImage;
}

DWORD MainFrame::GetMainBkColor()
{
	return m_pBackground->GetBkColor();
}

BOOL MainFrame::SetIteratorState(CDuiString strID, EVTTYPE eType, int nState, BOOL id /* = TRUE */)
{
	BOOL bFind = FALSE;
	if (eType == eVT_Friend)
	{
		CDuiString strOnlinePic = _T(" ") ,strOffLinePic = _T(" ") ,strNomalPic = _T(" ") ;
		CDuiString strNumID  = _T(" ") ; 
		for (FriendList_ITER citer = friends_.begin(); citer != friends_.end(); citer++)
		{
			if (id)
			{
				if (_tcsicmp(citer->id, strID.GetData()) == 0)
				{
					bFind = TRUE;
					citer->nstate = nState;
					strNumID =  citer->numid ; 
					strOnlinePic.Format(_T("%s\\%s_n.jpg"), g_config.szUserHead,strNumID) ; 
					strOffLinePic.Format(_T("%s\\%s_g.jpg"), g_config.szUserHead, strNumID) ; 

					// 本地如果有盖图标，就显示  否则显示默认图片 
					if( GetFileAttributes(strOnlinePic) != INVALID_FILE_ATTRIBUTES || GetFileAttributes(strOffLinePic) != INVALID_FILE_ATTRIBUTES)
					{
						if (nState == 1)
							citer->logo = strOnlinePic;
						else
							citer->logo = strOffLinePic;
					}
					else
					{
						if (nState == 1)
							citer->logo = _T("public.png");
						else
							citer->logo =_T("public_g.png") ;
					}

					return bFind;
				}
			}
			else
			{
				if (_tcsicmp(citer->numid, strID.GetData()) == 0)
				{
					bFind = TRUE;
					citer->nstate = nState;
					strNumID =  citer->numid ; 
					strOnlinePic.Format(_T("%s\\%s_n.jpg"), g_config.szUserHead,strNumID) ; 
					strOffLinePic.Format(_T("%s\\%s_g.jpg"), g_config.szUserHead, strNumID) ; 

					if( GetFileAttributes(strOnlinePic) != INVALID_FILE_ATTRIBUTES || GetFileAttributes(strOffLinePic) != INVALID_FILE_ATTRIBUTES)
					{
						if (nState == 1)
							citer->logo = strOnlinePic;
						else
							citer->logo = strOffLinePic;
					}
					else
					{
						if (nState == 1)
							citer->logo = _T("public.png");
						else
							citer->logo = _T("public.png");
					}

					return bFind;
				}
			}
		}
	}
	return bFind;
}

BOOL MainFrame::GetIteratorOfID(FriendList_ITER &friend_iterator, CDuiString strID, EVTTYPE eType, BOOL id /* = TRUE */)
{
	BOOL bFind = FALSE;
	if (eType == eVT_Friend)
	{
		for (FriendList_ITER citer = friends_.begin(); citer != friends_.end(); citer++)
		{
			if (id)
			{
				if (_tcsicmp(citer->id, strID.GetData()) == 0)
				{
					friend_iterator = citer;
					bFind = TRUE;
					return bFind;
				}
			}
			else
			{
				if (_tcsicmp(citer->numid, strID.GetData()) == 0)
				{
					friend_iterator = citer;
					bFind = TRUE;
					return bFind;
				}
			}
		}
	}
	else if (eType == eVT_Group)
	{
		for (FriendList_ITER citer = groups_.begin(); citer != groups_.end(); citer++)
		{
			if (id)
			{
				if (_tcsicmp(citer->id, strID.GetData()) == 0)
				{
					friend_iterator = citer;
					bFind = TRUE;
					return bFind;
				}
			}
			else
			{
				if (_tcsicmp(citer->numid, strID.GetData()) == 0)
				{
					friend_iterator = citer;
					bFind = TRUE;
					return bFind;
				}
			}
		}
	}
	else if (eType == eVT_Recent)
	{
		for (FriendList_ITER citer = Recentfriends_.begin(); citer != Recentfriends_.end(); citer++)
		{
			if (id)
			{
				if (_tcsicmp(citer->id, strID.GetData()) == 0)
				{
					friend_iterator = citer;
					bFind = TRUE;
					return bFind;
				}
			}
			else
			{
				if (_tcsicmp(citer->numid, strID.GetData()) == 0)
				{
					friend_iterator = citer;
					bFind = TRUE;
					return bFind;
				}
			}
		}

	}
	return bFind;
}

void MainFrame::OnMenu_CreateGroup()
{
	CGeneralDialog* pCreateGroupDialog = new CGeneralDialog(GetMainBkImage(), GetMainBkColor(), eCreate_Group, _T("创建群组"));
	if( pCreateGroupDialog == NULL )
		return;

	pCreateGroupDialog->Create(NULL, "创建群组", UI_WNDSTYLE_FRAME | WS_POPUP, NULL, 0, 0, 600, 800);
	skin_changed_observer_.AddReceiver(pCreateGroupDialog);
	pCreateGroupDialog->SetIcon(IDI_MAINFRAME_NEW);

	pCreateGroupDialog->CenterWindow();
	::ShowWindow(*pCreateGroupDialog, SW_SHOW);

	return;
}

// 创建登录窗口
void MainFrame::StartLogin()
{
	int nLoginMode_ = -1;

	if(VER_UPIM_NUMS == 0)			// 测试的普通版，打开登录框登录
		nLoginMode_ = 2;
	else if (VER_UPIM_NUMS == 1)	//(DLL模式)不弹出登录框，后台登录
		nLoginMode_ = 0;
	else if (VER_UPIM_NUMS == 2)	//老版本的投顾，打开登录框登录
		nLoginMode_ = 2;
	else if (VER_UPIM_NUMS == 3)	// im2.0投顾版，打开登录框登录
		nLoginMode_ = 2;
	else if (VER_UPIM_NUMS == 4)	// im2.0的客户版，exe传参数调用，后台登录
		nLoginMode_ = 1;
	else if (VER_UPIM_NUMS == 5)	// im3.0的客户版，exe传参数调用，后台登录
		nLoginMode_ = 3;

	//////////////////////////////////////////////////////////////////////////
	if (nLoginMode_ == 0)
	{
		DWORD t1 = GetTickCount() ; 
		USER_LOG("发送登录请求:%d" ,t1); 
		// 客户端会卡，先屏蔽登录，网络层改成线程
		g_MyClient.SetClientInfo(eUser_UPClient);
		CString strLoginName = g_loginname;
		strLoginName.Trim();
		g_MyClient.SendCheckPwdReq(strLoginName, STR_NOCHECKPWD, eLogin_EmpSpecial);
	}
	else if (nLoginMode_ == 1)
	{
		// 先去网络接口取得用户的权限
		GetClientRights(g_loginname);
		//
		InitConnect();
		g_MyClient.SetClientInfo(eUser_UPClient2);
		CString strLoginName = g_loginname;
		strLoginName.Trim();
		// wuchao modify at 20160509 
		// 产品新需求，考虑到用户注册的用户名不是微信号就是手机，为了保护用户的私密信息，用户登录采取的是username 
		// 而向服务器发送的是userid，去验证密码
		// 这种情况只能在IM集成到UP投资卫士的前提情况下  其他case另行考虑 采取宏控制 
#ifndef IM_EXE_TO_UP
		g_MyClient.SendCheckPwdReq(strLoginName, STR_NOCHECKPWD, eLogin_EmpSpecial);
#else
		g_MyClient.SendCheckPwdReq(g_khID, STR_NOCHECKPWD, eLogin_EmpSpecial);
		USER_LOG("IM send userid to server : %s " ,g_khID) ; 
#endif 

	}
	else if (nLoginMode_ == 2)
	{
		pLoginDialog = new CGeneralDialog(GetMainBkImage(), GetMainBkColor(), eCreate_LoginIn, _T("用户登录"));
		if( pLoginDialog == NULL )
			return;

		pLoginDialog->Create(NULL, "用户登录", UI_WNDSTYLE_FRAME | WS_POPUP, NULL, 0, 0, 600, 800);
		skin_changed_observer_.AddReceiver(pLoginDialog);
		pLoginDialog->SetIcon(IDI_MAINFRAME_NEW);

		pLoginDialog->CenterWindow();
		::ShowWindow(*pLoginDialog, SW_SHOW);
	}
	else if (nLoginMode_ == 3)
	{
		InitConnect();
	}

	return;
}

void MainFrame::OnUserListRClick(TNotifyUI& msg)
{
	if (m_pTabControl != NULL)
	{
		POINT pt = {msg.ptMouse.x, msg.ptMouse.y};
		// 【联系人】
		if (m_pTabControl->GetCurSel() == 0)
		{
			m_MainSelectNode.m_nPanelType = ePanel_Single;
			if ((m_pFriendsList != NULL) &&  m_pFriendsList->GetItemIndex(msg.pSender) != -1)
			{
				if (_tcsicmp(msg.pSender->GetClass(), _T("ListContainerElementUI")) == 0)
				{
					Node* node = (Node*)msg.pSender->GetTag();
					COPYSTRARRAY(m_MainSelectNode.m_szID, node->data().value);
					m_MainSelectNode.lpNode = (LPVOID)node;

					if (g_MyClient.m_nUserType != eUser_UPAnalyst && g_MyClient.m_nUserType != eUser_UPClient2 && g_MyClient.m_nUserType != eUser_UPAnalyst2)
					{
						// 如果是好友
						if (!m_pFriendsList->CanExpand(node)) 
						{
							m_MainSelectNode.m_nNodeType = eNode_Node;
							CMenuWnd* pMenu = new CMenuWnd();
							::ClientToScreen(m_hWnd, &pt);
							pMenu->Init(NULL, _T("menu_Friend.xml"), pt, &m_PaintManager);
							// 如果节点是自己的名字，没有“发送即时消息”
							if (strcmp(node->data().id, g_MyClient.m_strUserid) == 0)
								pMenu->EnableMenuItem(_T("Menu_Friend_Send"), FALSE);
						}
						// 如果是节点
						else
						{
							m_MainSelectNode.m_nNodeType = eNode_None;
							CMenuWnd* pMenu = new CMenuWnd();
							::ClientToScreen(m_hWnd, &pt);
							pMenu->Init(NULL, _T("menu_FriendNode.xml"), pt, &m_PaintManager);

							//未开发功能
							pMenu->EnableMenuItem(_T("Menu_FZ_Mod"), FALSE);
							pMenu->EnableMenuItem(_T("Menu_FZ_Del"), FALSE);
							pMenu->EnableMenuItem(_T("Menu_FD_Manger"), FALSE);

							if (g_userconfig.m_nNickType == eNICK_nameandid)
								pMenu->SetIconAndSize(_T("Menu_XS_NameAndID"), _T("right.png"), 9, 9);
							else if (g_userconfig.m_nNickType == eNICK_id)
								pMenu->SetIconAndSize(_T("Menu_XS_Id"), _T("right.png"), 9, 9);
							else if (g_userconfig.m_nNickType == eNICK_name)
								pMenu->SetIconAndSize(_T("Menu_XS_Name"), _T("right.png"), 9, 9);
						}
					}
				}
			}
		}
		// 【群/讨论组】
		else if (m_pTabControl->GetCurSel() == 1)
		{
			m_MainSelectNode.m_nPanelType = ePanel_Group;
			if ((m_pGroupsList != NULL) &&  m_pGroupsList->GetItemIndex(msg.pSender) != -1)
			{
				if (_tcsicmp(msg.pSender->GetClass(), _T("ListContainerElementUI")) == 0)
				{
					Node* node = (Node*)msg.pSender->GetTag();

					FriendList_ITER iterator;
					BOOL bFindNode = GetIteratorOfID(iterator, node->data().value, eVT_Group);
					if( bFindNode == FALSE)
						return ; 

					FriendListItemInfo groups_info = *iterator;

					m_MainSelectNode.lpNode = (LPVOID)node;
					m_MainSelectNode.m_nGroupType = (E_GROUPCHAT_TYPE)groups_info.roomtype;
					COPYSTRARRAY(m_MainSelectNode.m_szID, groups_info.numid);

					// 如果是群
					if (!m_pGroupsList->CanExpand(node)) 
					{
						CMenuWnd* pMenu = new CMenuWnd();
						::ClientToScreen(m_hWnd, &pt);
						pMenu->Init(NULL, _T("menu_GroupListNode.xml"), pt, &m_PaintManager );
					}
				}
			}
		}
		// 【最近联系人】
		else if (m_pTabControl->GetCurSel() == 2)
		{
			m_MainSelectNode.m_nPanelType = ePanel_Recent;
		}
	}
	return;
}

//菜单点击消息
void MainFrame::OnMainMenuClick(TNotifyUI& msg)
{
	//主菜单出现的位置相对于菜单按钮是固定的
	POINT m_menuPoint = {0};
	CControlUI * pControl = this->m_PaintManager.FindControl(msg.ptMouse);
	RECT m_menurect = pControl->GetPos();
	m_menuPoint.x = m_menurect.left;
	m_menuPoint.y = m_menurect.bottom - 195;

	if (pControl != NULL)
	{		
		CMenuWnd* pMenu = new CMenuWnd();
		::ClientToScreen(m_hWnd, &m_menuPoint);
		pMenu->Init(NULL, _T("menu_MainBtn.xml"), m_menuPoint, &m_PaintManager );
	}
	return;
}

// 在线图标上点击的菜单
void MainFrame::OnStateMenuClick(TNotifyUI& msg)
{
	POINT m_menuPoint = {0};
	CControlUI * pControl = this->m_PaintManager.FindControl(msg.ptMouse);
	RECT m_menurect = pControl->GetPos();
	m_menuPoint.x = m_menurect.left;
	m_menuPoint.y = m_menurect.bottom;

	if (pControl != NULL)
	{		
		CMenuWnd* pMenu = new CMenuWnd();
		::ClientToScreen(m_hWnd, &m_menuPoint);
		pMenu->Init(NULL, _T("menu_StateBtn.xml"), m_menuPoint, &m_PaintManager );
	}
	return;
}

// List双击消息
void MainFrame::OnListItemActivate(TNotifyUI& msg)
{
	//if(_tcsicmp( msg.pSender->GetName() , )
	//{

	//}
	// m_pTabControl 好友 分组 历史 
	if (m_pTabControl != NULL)
	{
		if (m_pTabControl->GetCurSel() == 0)
		{
			FriendNodeClick(msg);
		}
		else if (m_pTabControl->GetCurSel() == 1)
		{
			GroupNodeClick(msg);
		}
		else if (m_pTabControl->GetCurSel() == 2)
		{
			RecentNodeClick(msg);
		}
		//else 
		//{
		//	FindFriendNodeClick(msg) ; 
		//}
	}
	FindFriendNodeClick(msg) ; 
	//if( m_pFindFriendList != NULL )
	return;
}

// List单击消息
void MainFrame::OnListItemClick(TNotifyUI& msg)
{
	if (m_pTabControl != NULL)
	{
		if (m_pTabControl->GetCurSel() == 0)
		{
			if ((m_pFriendsList != NULL) &&  m_pFriendsList->GetItemIndex(msg.pSender) != -1)
			{
				if (_tcsicmp(msg.pSender->GetClass(), _T("ListContainerElementUI")) == 0)
				{
					Node* node = (Node*)msg.pSender->GetTag();

					if (m_pFriendsList->CanExpand(node)) // 如果是节点
					{
						int nOnlineNums = GetRoomClientOnlineNums() ;
						int nTotalNums  = m_vtRoomClient.size() ; 
						SetAnalysNodeNickName(g_nRoomID_im2_0,nOnlineNums,nTotalNums) ; 
						m_pFriendsList->SetChildVisible(node, !node->data().child_visible_);
					}
					else // 如果是好友
					{
						// wuchao add. fixed #17225  itemactivate与itemclick有何区别？？？
						//OnListItemActivate(msg);
					}
				}
			}
		}
		else if (m_pTabControl->GetCurSel() == 1)
		{
			if (VER_UPIM_NUMS == 3)
			{
				if ((m_pSelfGroup != NULL) &&  m_pSelfGroup->GetItemIndex(msg.pSender) != -1)
				{
					if (_tcsicmp(msg.pSender->GetClass(), _T("ListContainerElementUI")) == 0)
					{
						Node* node = (Node*)msg.pSender->GetTag();

						if (m_pSelfGroup->CanExpand(node)) // 如果是节点
						{
							m_pSelfGroup->SetChildVisible(node, !node->data().child_visible_);
						}
						else // 如果是好友
						{
							OnListItemActivate(msg);
						}
					}
				}
			}
			else
			{
				if ((m_pGroupsList != NULL) &&  m_pGroupsList->GetItemIndex(msg.pSender) != -1)
				{
					if (_tcsicmp(msg.pSender->GetClass(), _T("ListContainerElementUI")) == 0)
					{
						Node* node = (Node*)msg.pSender->GetTag();

						if (m_pGroupsList->CanExpand(node))  // 如果是节点
						{
							m_pGroupsList->SetChildVisible(node, !node->data().child_visible_);
						}
						else // 如果是好友
						{
							OnListItemActivate(msg);
						}
					}
				}
			}
		}
		else if (m_pTabControl->GetCurSel() == 2)
		{
		}
	}
}

// 最近联系人分组双击事件
void MainFrame::RecentNodeClick(TNotifyUI& msg)
{
	if ((m_pRecentLinkList != NULL) &&  m_pRecentLinkList->GetItemIndex(msg.pSender) != -1)
	{
		if (_tcsicmp(msg.pSender->GetClass(), _T("ListContainerElementUI")) == 0)
		{
			Node* node = (Node*)msg.pSender->GetTag();
			//////////////////////////////////////////////////////////////////////////
			if (!m_pRecentLinkList->CanExpand(node) && (m_pBackground != NULL))
			{
				// 如果节点是自己的名字，直接返回
				if (strcmp(node->data().id, g_MyClient.m_strUserid) == 0)
					return;

				FriendList_ITER _iterator;
				BOOL bFind = GetIteratorOfID(_iterator, node->data().value, eVT_Friend);
				if(!bFind)
					return ; 
				FriendListItemInfo friend_info = *_iterator;

				// 如果窗口已经打开，再次点击联系人列表，则把打开的窗口置顶
				if (allopenwnd[friend_info.id] != NULL)
				{
					ChatDialog* pChatDialog = (ChatDialog*)allopenwnd[friend_info.id];
					::ShowWindow(*pChatDialog, SW_SHOW);
					::SetWindowPos(*pChatDialog, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE);
					pChatDialog->ShowWindow(true);
				}
				// 没打开的话，新建窗口并将窗口句柄保存在全局map中
				else
				{
					ChatDialog* pChatDialog = NULL;
					if (friend_info.nodetype == eNode_Single)
						pChatDialog = new ChatDialog(GetMainBkImage(), GetMainBkColor(), myself_info_, friend_info);
					else if (friend_info.nodetype == eNode_Analyst)
						pChatDialog = new ChatDialog(GetMainBkImage(), GetMainBkColor(), myself_info_, friend_info, eDIALOG_Analys);
					if( pChatDialog == NULL )
						return;

					pChatDialog->friend_iterator = _iterator;
					allopenwnd[friend_info.id] = (LPVOID)pChatDialog;

					// 打开窗口的同时，停止图标闪烁
					StopFlashItem(friend_info.id, ePanel_Single);
					// 停止托盘图标闪烁 
					StopNotifyiconFlash() ; 
					CDuiString strDialogName = friend_info.nick_name;
					pChatDialog->Create(NULL, strDialogName, UI_WNDSTYLE_FRAME | WS_POPUP, NULL, 0, 0, 600, 800);
					skin_changed_observer_.AddReceiver(pChatDialog);
					pChatDialog->SetIcon(IDI_MAINFRAME_NEW);

					pChatDialog->CenterWindow();
					::ShowWindow(*pChatDialog, SW_SHOW);
				}
			}
		}
	}
	return;
}

// 群组列表双击
void MainFrame::GroupNodeClick(TNotifyUI& msg)
{
	// 自定义分组
	if (VER_UPIM_NUMS == 3)
	{
		if ((m_pSelfGroup != NULL) &&  m_pSelfGroup->GetItemIndex(msg.pSender) != -1)
		{
			if (_tcsicmp(msg.pSender->GetClass(), _T("ListContainerElementUI")) == 0)
			{
				Node* node = (Node*)msg.pSender->GetTag();
				//////////////////////////////////////////////////////////////////////////
				if (!m_pSelfGroup->CanExpand(node) && (m_pBackground != NULL))
				{
					// 如果节点是自己的名字，直接返回
					if (strcmp(node->data().id, g_MyClient.m_strUserid) == 0)
						return;

					FriendList_ITER _iterator;
					BOOL bFind = GetIteratorOfID(_iterator, node->data().value, eVT_Friend);
					if( !bFind)
						return ; 
					FriendListItemInfo friend_info = *_iterator;

					// 如果窗口已经打开，再次点击联系人列表，则把打开的窗口置顶
					if (allopenwnd[friend_info.id] != NULL)
					{
						ChatDialog* pChatDialog = (ChatDialog*)allopenwnd[friend_info.id];
						::ShowWindow(*pChatDialog, SW_SHOW);
						::SetWindowPos(*pChatDialog, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE);
						pChatDialog->ShowWindow(true);
					}
					// 没打开的话，新建窗口并将窗口句柄保存在全局map中
					else
					{
						ChatDialog* pChatDialog = NULL;
						if (friend_info.nodetype == eNode_Single)
							pChatDialog = new ChatDialog(GetMainBkImage(), GetMainBkColor(), myself_info_, friend_info);
						else if (friend_info.nodetype == eNode_Analyst)
							pChatDialog = new ChatDialog(GetMainBkImage(), GetMainBkColor(), myself_info_, friend_info, eDIALOG_Analys);
						if( pChatDialog == NULL )
							return;

						pChatDialog->friend_iterator = _iterator;
						allopenwnd[friend_info.id] = (LPVOID)pChatDialog;

						// 打开窗口的同时，停止图标闪烁
						StopFlashItem(friend_info.id, ePanel_Single);

						CDuiString strDialogName = friend_info.nick_name;
						pChatDialog->Create(NULL, strDialogName, UI_WNDSTYLE_FRAME | WS_POPUP, NULL, 0, 0, 600, 800);
						skin_changed_observer_.AddReceiver(pChatDialog);
						pChatDialog->SetIcon(IDI_MAINFRAME_NEW);

						pChatDialog->CenterWindow();
						::ShowWindow(*pChatDialog, SW_SHOW);
					}
				}
			}
		}
	}
	// 群组，按群处理
	else
	{
		if ((m_pGroupsList != NULL) &&  m_pGroupsList->GetItemIndex(msg.pSender) != -1)
		{
			if (_tcsicmp(msg.pSender->GetClass(), _T("ListContainerElementUI")) == 0)
			{
				Node* node = (Node*)msg.pSender->GetTag();

				//////////////////////////////////////////////////////////////////////////
				//
				if ( !m_pGroupsList->CanExpand(node) && (m_pBackground != NULL))
				{
					CDuiString strtmp = node->data().value;

					FriendList_ITER _iterator;
					BOOL bFind = GetIteratorOfID(_iterator, node->data().value, eVT_Group);
					FriendListItemInfo groups_info = *_iterator;

					// 如果窗口已经打开，再次点击联系人列表，则把打开的窗口置顶
					if (allopenwnd[groups_info.id] != NULL)
					{
						ChatDialog* pChatDialog = (ChatDialog*)allopenwnd[groups_info.id];
						::ShowWindow(*pChatDialog, SW_SHOW);
						::SetWindowPos(*pChatDialog, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE);
						pChatDialog->ShowWindow(true);
					}
					// 没打开的话，新建窗口并将窗口句柄保存在全局map中
					else
					{
						ChatDialog* pChatDialog = new ChatDialog(GetMainBkImage(), GetMainBkColor(), myself_info_, groups_info, eDIALOG_Group);
						if( pChatDialog == NULL )
							return;

						pChatDialog->friend_iterator = _iterator;
						allopenwnd[groups_info.id] = (LPVOID)pChatDialog;

						// 打开窗口的同时，停止图标闪烁
						StopFlashItem(groups_info.id, ePanel_Group);

						CDuiString strDialogName = groups_info.nick_name;
						pChatDialog->Create(NULL, strDialogName, UI_WNDSTYLE_FRAME | WS_POPUP, NULL, 0, 0, 600, 800);
						skin_changed_observer_.AddReceiver(pChatDialog);
						pChatDialog->SetIcon(IDI_MAINFRAME_NEW);

						pChatDialog->CenterWindow();
						::ShowWindow(*pChatDialog, SW_SHOW);

						g_MyClient.SendGcUserListReq(atol(groups_info.numid), groups_info.roomtype, 0);
					}
				}
			}
		}
	}
	return;
}

//////////////////////////////////////////////////////////////////////////
//wuchao add at 2016-04-06 
// 点击查找好友列表 
//////////////////////////////////////////////////////////////////////////
void MainFrame::FindFriendNodeClick(TNotifyUI& msg )
{
	if( m_pFindFriendList)
	{
		Node* node = (Node*)msg.pSender->GetTag();

		if (!m_pFindFriendList->CanExpand(node) && (m_pBackground != NULL))
		{
			// 如果节点是自己的名字，直接返回
			if (strcmp(node->data().id, g_MyClient.m_strUserid) == 0)
				return;

			FriendList_ITER _iterator; 
			BOOL bFind = GetIteratorOfID(_iterator, node->data().value, eVT_Friend);
			if( !bFind)
				return  ;
			FriendListItemInfo friend_info = *_iterator;

			// 如果窗口已经打开，再次点击联系人列表，则把打开的窗口置顶
			if (allopenwnd[friend_info.id] != NULL)
			{
				ChatDialog* pChatDialog = (ChatDialog*)allopenwnd[friend_info.id];
				::ShowWindow(*pChatDialog, SW_SHOW);
				::SetWindowPos(*pChatDialog, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE);
				pChatDialog->ShowWindow(true);
			}
			// 没打开的话，新建窗口并将窗口句柄保存在全局map中
			else
			{
				ChatDialog* pChatDialog = NULL;
				if (friend_info.nodetype == eNode_Single)
					pChatDialog = new ChatDialog(GetMainBkImage(), GetMainBkColor(), myself_info_, friend_info);
				else if (friend_info.nodetype == eNode_Public)
					pChatDialog = new ChatDialog(GetMainBkImage(), GetMainBkColor(), myself_info_, friend_info, eDIALOG_Public);
				else if (friend_info.nodetype == eNode_Analyst)
				{
					if (VER_UPIM_NUMS == 4)
					{
						int nRoomID = atoi(friend_info.Parentid);
						int nVipRight = m_mapAllClientRoom[nRoomID].m_nVipRight;
						if (!IsHasRights(nVipRight))
						{
							PostMessage(UM_USER_OPENRIGHTS, 0, 0);
							return;
						}
					}
					pChatDialog = new ChatDialog(GetMainBkImage(), GetMainBkColor(), myself_info_, friend_info, eDIALOG_Analys);
				}
				else if (friend_info.nodetype == eNode_RYPublic)
					pChatDialog = new ChatDialog(GetMainBkImage(), GetMainBkColor(), myself_info_, friend_info,eDIALOG_RYPublic);
				if( pChatDialog == NULL )
					return;

				pChatDialog->friend_iterator = _iterator;
				allopenwnd[friend_info.id] = (LPVOID)pChatDialog;

				CDuiString strDialogName = friend_info.nick_name;
				pChatDialog->Create(NULL, strDialogName, UI_WNDSTYLE_FRAME | WS_POPUP, NULL, 0, 0, 600, 800);
				skin_changed_observer_.AddReceiver(pChatDialog);
				pChatDialog->SetIcon(IDI_MAINFRAME_NEW);

				pChatDialog->CenterWindow();
				::ShowWindow(*pChatDialog, SW_SHOW);

				if (friend_info.nodetype == eNode_Public && g_MyClient.IsClientLoginIn())
					g_MyClient.SendGetPublicInfoReq(atoi(friend_info.numid));
			}
		}
	}
}

// 好友列表双击
void MainFrame::FriendNodeClick(TNotifyUI& msg)
{
	if ((m_pFriendsList != NULL) &&  m_pFriendsList->GetItemIndex(msg.pSender) != -1)
	{
		if (_tcsicmp(msg.pSender->GetClass(), _T("ListContainerElementUI")) == 0)
		{
			Node* node = (Node*)msg.pSender->GetTag();
			//////////////////////////////////////////////////////////////////////////
			//
			if (m_pFriendsList->CanExpand(node) && (m_pBackground != NULL))
			{
				// 服务人员才允许群体发送
				if (!g_MyClient.IsClientLoginIn() && node->data().nodetype == eNode_Single )					
				{
					//假如点击的是父节点的话，弹出界面，此对话框中
					FriendList_ITER _iterator;
					BOOL bFind = GetIteratorOfID(_iterator, node->data().value, eVT_Friend);
					FriendListItemInfo friend_info = *_iterator;

					// 如果窗口已经打开，再次点击联系人列表，则把打开的窗口置顶
					if (allopenwnd[friend_info.id] != NULL)
					{
						ChatDialog* pChatDialog = (ChatDialog*)allopenwnd[friend_info.id];
						::ShowWindow(*pChatDialog, SW_SHOW);
						::SetWindowPos(*pChatDialog, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE);
						pChatDialog->ShowWindow(true);
					}
					// 没打开的话，新建窗口并将窗口句柄保存在全局map中
					else
					{
						ChatDialog* pChatDialog = new ChatDialog(GetMainBkImage(), GetMainBkColor(), myself_info_, friend_info, eDIALOG_GroupSend);
						if( pChatDialog == NULL )
							return;

						pChatDialog->friend_iterator = _iterator;
						allopenwnd[friend_info.id] = (LPVOID)pChatDialog;

						CDuiString strDialogName = "";
						strDialogName.Format("%s[消息群发]", friend_info.nick_name);

						//////////////////////////////////////////////////////////////////////////
						for (std::vector<FriendListItemInfo>::const_iterator citer = friends_.begin(); citer != friends_.end(); citer++)
						{
							if (_tcsicmp(citer->Parentid, friend_info.id) == 0)
							{
								pChatDialog->m_vtUPIMID.push_back(citer->id.GetData());
							}
						}

						pChatDialog->Create(NULL, strDialogName, UI_WNDSTYLE_FRAME | WS_POPUP, NULL, 0, 0, 0, 0);
						skin_changed_observer_.AddReceiver(pChatDialog);
						pChatDialog->SetIcon(IDI_MAINFRAME_NEW);

						pChatDialog->CenterWindow();
						::ShowWindow(*pChatDialog, SW_SHOW);
					}
				}
			}
			//////////////////////////////////////////////////////////////////////////
			if (!m_pFriendsList->CanExpand(node) && (m_pBackground != NULL))
			{
				// 如果节点是自己的名字，直接返回
#ifndef VER_UPIM_RONGYUN
				if (strcmp(node->data().id, g_MyClient.m_strUserid) == 0)
					return;
#endif
				FriendList_ITER _iterator;
				//// wuchao test 
				//char *str ="1142" ; 
				//OnOpenChatDialog((WPARAM)str,0) ; 
				BOOL bFind = GetIteratorOfID(_iterator, node->data().value, eVT_Friend);
				if( !bFind)
					return  ;
				FriendListItemInfo friend_info = *_iterator;

				// 如果窗口已经打开，再次点击联系人列表，则把打开的窗口置顶
				if (allopenwnd[friend_info.id] != NULL)
				{
					ChatDialog* pChatDialog = (ChatDialog*)allopenwnd[friend_info.id];
					::ShowWindow(*pChatDialog, SW_SHOW);
					::SetWindowPos(*pChatDialog, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE);
					pChatDialog->ShowWindow(true);
				}
				// 没打开的话，新建窗口并将窗口句柄保存在全局map中
				else
				{
					ChatDialog* pChatDialog = NULL;
					if (friend_info.nodetype == eNode_Single)
						pChatDialog = new ChatDialog(GetMainBkImage(), GetMainBkColor(), myself_info_, friend_info);
					else if (friend_info.nodetype == eNode_Public)
						pChatDialog = new ChatDialog(GetMainBkImage(), GetMainBkColor(), myself_info_, friend_info, eDIALOG_Public);
					else if (friend_info.nodetype == eNode_Analyst)
					{
						if (VER_UPIM_NUMS == 4)
						{
							int nRoomID = atoi(friend_info.Parentid);
							int nVipRight = m_mapAllClientRoom[nRoomID].m_nVipRight;
							if (!IsHasRights(nVipRight))
							{
								PostMessage(UM_USER_OPENRIGHTS, 0, 0);
								return;
							}
						}
						pChatDialog = new ChatDialog(GetMainBkImage(), GetMainBkColor(), myself_info_, friend_info, eDIALOG_Analys);
					}
					else if (friend_info.nodetype == eNode_RYPublic)
					{
						pChatDialog = new ChatDialog(GetMainBkImage(), GetMainBkColor(), myself_info_, friend_info,eDIALOG_RYPublic);
					}
					if( pChatDialog == NULL )
						return;

					pChatDialog->friend_iterator = _iterator;
					allopenwnd[friend_info.id] = (LPVOID)pChatDialog;

					// 打开窗口的同时，停止图标闪烁  好友列表上面闪动的图标 
					StopFlashItem(friend_info.id, ePanel_Single);
					// 停止托盘图标闪烁 
					StopNotifyiconFlash() ; 
					CDuiString strDialogName = friend_info.nick_name;
					pChatDialog->Create(NULL, strDialogName, UI_WNDSTYLE_FRAME | WS_POPUP, NULL, 0, 0, 600, 800);
					skin_changed_observer_.AddReceiver(pChatDialog);
					pChatDialog->SetIcon(IDI_MAINFRAME_NEW);

					pChatDialog->CenterWindow();
					::ShowWindow(*pChatDialog, SW_SHOW);

					if (friend_info.nodetype == eNode_Public && g_MyClient.IsClientLoginIn())
						g_MyClient.SendGetPublicInfoReq(atoi(friend_info.numid));
				}
			}
		}
	}
}

//获取群组列表
BOOL MainFrame::GCGetList(ROOMID mRoomID)
{
	CDuiString strRoomID = _T("");
	strRoomID.Format("ROOM$%d", mRoomID);
	ChatDialog* pChatDialog = (ChatDialog*)allopenwnd[strRoomID];

	pChatDialog->PostMessage(UM_USER_ADDROOMMEMBER, 0, 0);
	return TRUE;
}

BOOL MainFrame::GetPublicRoute(PUBLICID mPublicID)
{
	CDuiString strPublicID = _T("");
	strPublicID.Format("PUBLIC$%d", mPublicID);
	ChatDialog* pChatDialog = (ChatDialog*)allopenwnd[strPublicID];

	pChatDialog->PostMessage(UM_USER_PUBLICINFO, 0, 0);
	return TRUE;
}

void MainFrame::CreateTgRoomUserNode(int nRID, LPCSTR szUID)
{
	CDuiString strPUid = _T("");
	strPUid.Format("TGUSER$%d$%s", nRID, szUID);

	FriendList_ITER iterator;
	BOOL bFindNode = GetIteratorOfID(iterator, strPUid, eVT_Friend);
	if (!bFindNode)
	{
		CDuiString strPublicNode = _T("");
		strPublicNode.Format("ANALYST$%d", nRID);

		if (m_pFriendsList != NULL)
		{
			Node* m_pnode = m_pFriendsList->SelectNodeByID(strPublicNode.GetData());
			if (m_pnode)
			{
				FriendListItemInfo item;
				Node* retrunptr = NULL;

				item.folder = false;
				item.logo = _T("public.png");
				item.nodetype = eNode_Analyst;
				item.type = eNode_Analyst;
				item.id = strPUid;
				item.numid = szUID;
				item.nick_name = szUID;

#ifdef NOT_DISPLAY_ID_LIKE_MOBLIE_NUM
				CString strUser = ConvertIdToNoPhonrNum(szUID);
				item.showname = strUser;
#else
				item.showname = szUID;
#endif // NOT_DISPLAY_ID_LIKE_MOBLIE_NUM
				item.Parentid = IntToStr(nRID);

				retrunptr = m_pFriendsList->AddNode(item, m_pnode);
				if (retrunptr)
					friends_.push_back(item);
			}
		}
	}
	return;
}

void MainFrame::CreatePublicUserNode(int nPID, LPCSTR szUID)
{
	CDuiString strPUid = _T("");
	strPUid.Format("PUSER$%d$%s", nPID, szUID);

	FriendList_ITER iterator;
	BOOL bFindNode = GetIteratorOfID(iterator, strPUid, eVT_Friend);
	if (!bFindNode)
	{
		CDuiString strPublicNode = _T("");
		strPublicNode.Format("PUBLIC$%d", nPID);

		if (m_pFriendsList != NULL)
		{
			Node* m_pnode = m_pFriendsList->SelectNodeByID(strPublicNode.GetData());
			if (m_pnode)
			{
				FriendListItemInfo item;
				Node* retrunptr = NULL;

				item.folder = false;
				item.logo = _T("public.png");
				item.nodetype = eNode_Public;
				item.type = eList_Public;
				item.id = strPUid;
				item.numid = szUID;
				item.nick_name = szUID;
				item.showname = szUID;
				item.Parentid = IntToStr(nPID);

				retrunptr = m_pFriendsList->AddNode(item, m_pnode);
				if (retrunptr)
					friends_.push_back(item);
			}
		}
	}
	return;
}

//收到服务器消息
BOOL MainFrame::ProcessRecvMsg(LPRECVMSG2 lpRecvMsg, BOOL bOffMsg/* =FALSE */)
{
	if(!lpRecvMsg) 
		return FALSE;

	if(lpRecvMsg->m_ucFormat == eMsgFormat_DbMsg){}
	else if(lpRecvMsg->m_ucFormat == eMsgFormat_OffimageReceipt){}

	RECEIVED_MSG txtmsg;
	strcpy(txtmsg.imid, lpRecvMsg->m_szUSERID);
	strcpy(txtmsg.innerID, lpRecvMsg->m_szInnerID);
	int nMsgLen = lpRecvMsg->m_nMsglen;
	txtmsg.msg = new char[nMsgLen+1];
	strncpy(txtmsg.msg, lpRecvMsg->m_szMsg, nMsgLen);
	txtmsg.msg[nMsgLen] = 0;
	txtmsg.msgtype = lpRecvMsg->m_ucMsgType;
	txtmsg.format = lpRecvMsg->m_ucFormat;
	txtmsg.senddate = lpRecvMsg->m_nSendDate;
	txtmsg.sendtime = lpRecvMsg->m_nSendTime;

	//////////////////////////////////////////////////////////////////////////
	// 如果是服务器发来的公众号消息
	if (txtmsg.msgtype == eMsgSend_Public)
	{
		int nPublicID = lpRecvMsg->nPublicID;
		CString strPUid = _T("");
		//如果是客户收到消息回执
		if (g_MyClient.IsClientLoginIn())
		{
			// 直接找到公众号节点
			strPUid.Format("PUBLIC$%d", nPublicID);
		}
		// 如果是客服收到处理消息
		else
		{
			// 创建节点
			CreatePublicUserNode(nPublicID, lpRecvMsg->m_szUSERID);
			strPUid.Format("PUSER$%d$%s", nPublicID, lpRecvMsg->m_szUSERID);
		}

		FriendList_ITER iterator;
		BOOL bFindNode = GetIteratorOfID(iterator, (CDuiString)strPUid, eVT_Friend);
		if (bFindNode)
		{
			if(iterator->pMsg == NULL)
				iterator->pMsg = new VECTOR_RECEIVED_MSG;
			//iterator->m_csLock.Lock();
			iterator->pMsg->push_back(txtmsg);
			//iterator->m_csLock.Unlock();
		}

		//////////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////////
		// 判断当前联系人的对话窗口是否已经打开
		ChatDialog* pChatDialog = NULL;
		if (allopenwnd[(CDuiString)strPUid] != NULL)
		{
			// 已经打开，直接填充消息
			pChatDialog = (ChatDialog*)allopenwnd[(CDuiString)strPUid];
			pChatDialog->PostMessage(UM_USER_RECVMSG, 0, 0);
		}
		else
		{
			if (txtmsg.format != eMsgFormat_Shake)
			{
				// 若未打开，图标闪烁/托盘图标闪烁
				InsertIntoFlashVt((CDuiString)strPUid, ePanel_Single);
				g_pFrame->SetNotifyiconFlash();

				// 收到了好友发送来的消息 
				m_bRecvFriendsMsg = TRUE ; 
			}
		}

		//////////////////////////////////////////////////////////////////////////
		// 声音提示，在不关闭的情况下
		g_pFrame->PostMessage(UM_SOUND_PLAY, IDR_WAVE_MSG);

		//////////////////////////////////////////////////////////////////////////
		int nRecordType = eRecord_Public;
		if(lpRecvMsg->m_ucFormat != eMsgFormat_File)
		{
			// 写入历史消息库,文件消息单独处理
			g_SqliteOp.DBInsertRecordRaw(lpRecvMsg->m_szUSERID, g_MyClient.m_strUserid, 0, lpRecvMsg->m_nSendDate, 
				lpRecvMsg->m_nSendTime, nRecordType, lpRecvMsg->m_ucFormat, lpRecvMsg->m_szMsg);
		}

		return TRUE;
	}

	//////////////////////////////////////////////////////////////////////////
	// 如果是服务器发来的分析师消息
	if (txtmsg.msgtype == eMsgSend_Analyst)
	{
		int nTgRoomID = atoi(lpRecvMsg->m_szInnerID);
		CString strPUid = _T("");
		//如果是客户收到消息回执
		if (g_MyClient.m_nUserType == eUser_UPClient)
		{
			if (g_TGChatDialog)
			{
				if(g_TGChatDialog->pMsg == NULL)
					g_TGChatDialog->pMsg = new VECTOR_RECEIVED_MSG;
				g_TGChatDialog->pMsg->push_back(txtmsg);

				g_TGChatDialog->PostMessage(UM_USER_RECVMSG, 0, 0);

				//////////////////////////////////////////////////////////////////////////
				int nRecordType = eRecord_TgRoom;
				if(lpRecvMsg->m_ucFormat != eMsgFormat_File)
				{
					// 写入历史消息库,文件消息单独处理
					g_SqliteOp.DBInsertRecordRaw(lpRecvMsg->m_szUSERID, g_MyClient.m_strUserid, nTgRoomID, lpRecvMsg->m_nSendDate, 
						lpRecvMsg->m_nSendTime, nRecordType, lpRecvMsg->m_ucFormat, lpRecvMsg->m_szMsg);
				}
			}
		}
		// 如果是投顾收到处理消息
		else if (g_MyClient.m_nUserType == eUser_UPAnalyst)
		{
			// 创建节点
			CreateTgRoomUserNode(nTgRoomID, lpRecvMsg->m_szUSERID);
			strPUid.Format("TGUSER$%d$%s", nTgRoomID, lpRecvMsg->m_szUSERID);

			FriendList_ITER iterator;
			BOOL bFindNode = GetIteratorOfID(iterator, (CDuiString)strPUid, eVT_Friend);
			if (bFindNode)
			{
				if(iterator->pMsg == NULL)
					iterator->pMsg = new VECTOR_RECEIVED_MSG;
				//iterator->m_csLock.Lock();
				iterator->pMsg->push_back(txtmsg);
				//iterator->m_csLock.Unlock();
			}

			//////////////////////////////////////////////////////////////////////////
			//////////////////////////////////////////////////////////////////////////
			// 判断当前联系人的对话窗口是否已经打开
			ChatDialog* pChatDialog = NULL;
			if (allopenwnd[(CDuiString)strPUid] != NULL)
			{
				// 已经打开，直接填充消息
				pChatDialog = (ChatDialog*)allopenwnd[(CDuiString)strPUid];
				pChatDialog->PostMessage(UM_USER_RECVMSG, 0, 0);
			}
			else
			{
				if (txtmsg.format != eMsgFormat_Shake)
				{
					// 若未打开，图标闪烁/托盘图标闪烁
					InsertIntoFlashVt((CDuiString)strPUid, ePanel_Single);
					g_pFrame->SetNotifyiconFlash();
				}
			}

			//////////////////////////////////////////////////////////////////////////
			// 声音提示，在不关闭的情况下
			g_pFrame->PostMessage(UM_SOUND_PLAY, IDR_WAVE_MSG);

			//////////////////////////////////////////////////////////////////////////
			int nRecordType = eRecord_TgRoom;
			if(lpRecvMsg->m_ucFormat != eMsgFormat_File)
			{
				// 写入历史消息库,文件消息单独处理
				g_SqliteOp.DBInsertRecordRaw(lpRecvMsg->m_szUSERID, g_MyClient.m_strUserid, nTgRoomID, lpRecvMsg->m_nSendDate, 
					lpRecvMsg->m_nSendTime, nRecordType, lpRecvMsg->m_ucFormat, lpRecvMsg->m_szMsg);
			}
		}
		// 如果是投顾收到处理消息
		else if (g_MyClient.m_nUserType == eUser_UPAnalyst2 || g_MyClient.m_nUserType == eUser_UPClient2)
		{

			USER_ROOMCLIENT_INFO m_info ;
			memset(&m_info,0,sizeof(USER_ROOMCLIENT_INFO)) ;
			COPYSTRARRAY(m_info.m_szUPID, lpRecvMsg->m_szUSERID);
			m_info.m_bIsOnLine = TRUE ;
			// 查找m_vtRoomClient中有没有这个客户 
			std::vector<USER_ROOMCLIENT_INFO>::iterator findIt = find( m_vtRoomClient.begin() ,m_vtRoomClient.end() ,m_info) ; 
			if( findIt == m_vtRoomClient.end() )
			{
				m_vtRoomClient.push_back(m_info);
				// 将新客户插入到分析师的朋友列表中  
				// 1 好友列表的父节点   好友列表  
				CString mGroupID = _T("");
				Node* retrunptr = NULL;
				int nRoomID = g_nRoomID_im2_0;
				CString strUser = m_info.m_szUPID;
				mGroupID.Format("ANALYST$%d", nRoomID);
				if( !m_pFriendsList)
				{
					USER_LOG("新开通好友插入好友列表失败，好友列表不存在") ; 
					return FALSE ; 
				}
				Node* m_mode = m_pFriendsList->SelectNodeByID(mGroupID);
				// 2 AddNode 
				if(!m_mode)
				{
					USER_LOG("找不到插入节点的父节点");
					return false; 
				}
				CDuiString  mTgRoomID = _T(" ");
				mTgRoomID.Format("TGUSER$%d$%s", nRoomID, strUser);

				FriendListItemInfo item;
				CString logoName =  _T(" ") ;
				CString strDown  =  _T(" ") ;		// 下载的头像
				CString strHead  =  _T(" ") ;		// 在线的正常头像
				CString strHead2 =  _T(" ") ;		// 离线时的灰化头像

				if( strlen(m_info.m_szUPID) != 0 )
				{
					strDown.Format(_T("%s\\%s.jpg"), g_config.szUserHead,  m_info.m_szUPID);
					strHead.Format(_T("%s\\%s_n.jpg"), g_config.szUserHead,  m_info.m_szUPID);
					strHead2.Format(_T("%s\\%s_g.jpg"), g_config.szUserHead,  m_info.m_szUPID);
				}
				if(GetFileAttributes(strHead) != -1 )
				{
					COPYSTRARRAY(m_info.m_szImgName , strDown) ; 
					COPYSTRARRAY(m_info.m_szLNomalImgName , strHead) ;
					COPYSTRARRAY(m_info.m_szLGrayImgName , strHead2) ;
				}
				if(m_info.m_bIsOnLine && strlen(m_info.m_szLNomalImgName)!=0 )
				{
					logoName.Format(_T("%s"),m_info.m_szLNomalImgName) ; 
					item.logo = logoName ; 
				}
				else 
				{
					if( strlen(m_info.m_szLGrayImgName)!=0)
					{
						logoName.Format(_T("%s"),m_info.m_szLGrayImgName) ;
						item.logo = logoName ; 
					}
				}
				if( item.logo.IsEmpty())
				{
					if( m_info.m_bIsOnLine ) 
						item.logo = _T("public.png");
					else
						item.logo = _T("public_g.png") ; 
				}
				// 将节点的在线状态  加进好友列表集合中 
				item.nstate = m_info.m_bIsOnLine ; 
				item.nodetype = eNode_Analyst;
				item.type = eNode_Analyst;
				item.id = mTgRoomID;
				item.Parentid = IntToStr(nRoomID);
				item.numid = strUser;
				item.nick_name = strUser;

#ifdef NOT_DISPLAY_ID_LIKE_MOBLIE_NUM
				strUser = ConvertIdToNoPhonrNum(strUser);
#endif // NOT_DISPLAY_ID_LIKE_MOBLIE_NUM
				item.showname = strUser;

				retrunptr = m_pFriendsList->AddNode(item, m_mode);
				if (retrunptr)
				{
					m_info.m_bLoad = TRUE;
					m_pFriendsList->SetChildVisible(m_mode, true); 
					friends_.push_back(item);
					if(m_info.m_bIsOnLine)
					{
						m_vtOnlineClient.push_back(m_info) ; 
					}
					else
						m_vtOfflineClient.push_back(m_info) ; 
				}

				// 设置标题 
				int nOnlineNums = GetRoomClientOnlineNums() ;
				SetAnalysNodeNickName(g_nRoomID_im2_0,nOnlineNums,m_vtRoomClient.size()) ;


				//// 分组中  父节点  默认插入到未分组列表中 
				CDuiString strGroupID = _T(" ") ;
				int nGroupID = 0 ; 
				strGroupID.Format("SELFGROUP$%d",nGroupID);

				// （1）先找到分组ID为nGroupID的分组 
				if( !m_pSelfGroup)
					return FALSE;
				Node* findNode = m_pSelfGroup->GetNodeItem(strGroupID) ; 
				if(!findNode)
					return FALSE; 

				// （2）在该分组的好友列表的最后面添加好友 
				m_pSelfGroup->AddNode(item,findNode) ; 
				m_pSelfGroup->SetNodeName(findNode,GetGroupClientOnlineNums(0),findNode->num_children()) ; 
				//AddSelfGroupClientNode(0,m_info.m_szUPID) ; 

			} // end of if(findIt == m_vtRoomClient.end())

			strPUid.Format("TGUSER$%d$%s", nTgRoomID, lpRecvMsg->m_szUSERID);
			FriendList_ITER iterator;
			// 在好友列表中找到发消息来的好友
			BOOL bFindNode = GetIteratorOfID(iterator, (CDuiString)strPUid, eVT_Friend);
			if (bFindNode)
			{
				if(iterator->pMsg == NULL)
					iterator->pMsg = new VECTOR_RECEIVED_MSG;
				//iterator->m_csLock.Lock();
				iterator->pMsg->push_back(txtmsg);
				//iterator->m_csLock.Unlock();

				FriendList_ITER it ;
				BOOL isEqual = FALSE ; 

				for( it = m_vecWhoSendMsg.begin() ; it != m_vecWhoSendMsg.end(); it++)
				{
					if( strcmp((*it).id ,(*iterator).id) == 0 )
					{
						isEqual = TRUE ;
						break ; 
					}
				}
				if( isEqual == FALSE )
					m_vecWhoSendMsg.push_back(*iterator) ;

			}

			//////////////////////////////////////////////////////////////////////////
			//////////////////////////////////////////////////////////////////////////
			// 判断当前联系人的对话窗口是否已经打开
			ChatDialog* pChatDialog = NULL;
			if (allopenwnd[(CDuiString)strPUid] != NULL)
			{
				// 已经打开，直接填充消息
				pChatDialog = (ChatDialog*)allopenwnd[(CDuiString)strPUid];
				pChatDialog->PostMessage(UM_USER_RECVMSG, 0, 0);
			}
			else
			{
				if (txtmsg.format != eMsgFormat_Shake)
				{
					// 若未打开，图标闪烁/托盘图标闪烁
					InsertIntoFlashVt((CDuiString)strPUid, ePanel_Single);
					// 托盘闪动 
					g_pFrame->SetNotifyiconFlash();
				}
			}

			//////////////////////////////////////////////////////////////////////////
			// 声音提示，在不关闭的情况下
			g_pFrame->PostMessage(UM_SOUND_PLAY, IDR_WAVE_MSG);

			//////////////////////////////////////////////////////////////////////////
			int nRecordType = eRecord_TgRoom;
			if(lpRecvMsg->m_ucFormat != eMsgFormat_File)
			{
				// 写入历史消息库,文件消息单独处理
				g_SqliteOp.DBInsertRecordRaw(lpRecvMsg->m_szUSERID, g_MyClient.m_strUserid, nTgRoomID, lpRecvMsg->m_nSendDate, 
					lpRecvMsg->m_nSendTime, nRecordType, lpRecvMsg->m_ucFormat, lpRecvMsg->m_szMsg);
			}

			//////////////////////////////////////////////////////////////////////////
			// 文本消息中转换成简要信息显示在界面上，图片显示[图片],表情显示[表情]
			CString strDispairMsg = ConvertMsgToChiefMsg(lpRecvMsg->m_ucFormat, lpRecvMsg->m_szMsg);
			// 填充最近联系人
			ModifyDisRecentInfo(CDuiString(lpRecvMsg->m_szUSERID), "", CDuiString(strDispairMsg), GetNowTimeHourAndMin(), nTgRoomID);
		}

		return TRUE;
	}


	//////////////////////////////////////////////////////////////////////////
	// 如果是服务器发来的一般消息
	else if (txtmsg.msgtype == eMsgSend_Normal)
	{
		FriendList_ITER iterator;
		BOOL bFindNode = GetIteratorOfID(iterator, lpRecvMsg->m_szUSERID, eVT_Friend);

		// 能在好友列表中找到，说明是好友，找不到按照陌生人逻辑处理
		if (bFindNode)
		{
			if(iterator->pMsg == NULL)
				iterator->pMsg = new VECTOR_RECEIVED_MSG;
			//iterator->m_csLock.Lock();
			iterator->pMsg->push_back(txtmsg);
			//iterator->m_csLock.Unlock();
		}
		//////////////////////////////////////////////////////////////////////////
		// 判断当前联系人的对话窗口是否已经打开
		ChatDialog* pChatDialog = NULL;
		if (allopenwnd[lpRecvMsg->m_szUSERID] != NULL)
		{
			// 已经打开，直接填充消息
			pChatDialog = (ChatDialog*)allopenwnd[lpRecvMsg->m_szUSERID];
			pChatDialog->PostMessage(UM_USER_RECVMSG, 0, 0);
		}
		else
		{
			if (txtmsg.format != eMsgFormat_Shake)
			{
				// 若未打开，图标闪烁/托盘图标闪烁
				InsertIntoFlashVt(lpRecvMsg->m_szUSERID, ePanel_Single);
				g_pFrame->SetNotifyiconFlash();
			}
		}

		//////////////////////////////////////////////////////////////////////////
		// 声音提示，在不关闭的情况下
		if (txtmsg.format == eMsgFormat_Shake)
		{
			g_pFrame->PostMessage(UM_SOUND_PLAY, IDR_WAVE_SHAKE);
			OpenChatWnd(lpRecvMsg->m_szUSERID, eMSG_Single);
		}
		else
			g_pFrame->PostMessage(UM_SOUND_PLAY, IDR_WAVE_MSG);

		//////////////////////////////////////////////////////////////////////////
		int nRecordType = eRecord_Single;
		if(lpRecvMsg->m_ucFormat != eMsgFormat_File)
		{
			// 写入历史消息库,文件消息单独处理
			g_SqliteOp.DBInsertRecordRaw(lpRecvMsg->m_szUSERID, g_MyClient.m_strUserid, 0, lpRecvMsg->m_nSendDate, 
				lpRecvMsg->m_nSendTime, nRecordType, lpRecvMsg->m_ucFormat, lpRecvMsg->m_szMsg);
		}
	}
	return TRUE;
}

BOOL MainFrame::ProcessRecvMsg(LPNEWRECVMSGPUBLIC lpRecvMsg)
{
	if(!lpRecvMsg) 
		return FALSE;

	if(lpRecvMsg->m_ucFormat == eMsgFormat_DbMsg){}
	else if(lpRecvMsg->m_ucFormat == eMsgFormat_OffimageReceipt){}

	RECEIVED_MSG txtmsg;
	strcpy(txtmsg.imid, lpRecvMsg->m_szUSERID);
	strcpy(txtmsg.innerID, lpRecvMsg->m_szInnerID);
	int nMsgLen = lpRecvMsg->m_nMsglen;
	txtmsg.msg = new char[nMsgLen+1];
	strncpy(txtmsg.msg, lpRecvMsg->m_szMsg, nMsgLen);
	txtmsg.msg[nMsgLen] = 0;
	txtmsg.msgtype = lpRecvMsg->m_ucMsgType;
	txtmsg.format = lpRecvMsg->m_ucFormat;
	txtmsg.senddate = lpRecvMsg->m_nSendDate;
	txtmsg.sendtime = lpRecvMsg->m_nSendTime;

	//////////////////////////////////////////////////////////////////////////
	// 如果是服务器发来的公众号消息
	if (txtmsg.msgtype == eMsgSend_NewPublic)
	{		
		CString strKFID(lpRecvMsg->m_szUSERID);
		CString strPublicID(lpRecvMsg->nPublicID);
		FriendList_ITER iterator;
		BOOL bFindNode;
		if (strPublicID.IsEmpty())
		{
			bFindNode= GetIteratorOfID(iterator, (CDuiString)strKFID, eVT_Recent);
			if (bFindNode)
			{
				if(iterator->pMsg == NULL)
					iterator->pMsg = new VECTOR_RECEIVED_MSG;
				iterator->pMsg->push_back(txtmsg);
			}
		}
		else
		{
			bFindNode = GetIteratorOfID(iterator, (CDuiString)strPublicID, eVT_Friend);
			if (bFindNode)
			{
				if(iterator->pMsg == NULL)
					iterator->pMsg = new VECTOR_RECEIVED_MSG;
				iterator->pMsg->push_back(txtmsg);
			}
		}
		ChatDialog* pChatDialog = NULL;
		if (allopenwnd[(CDuiString)strPublicID] != NULL)
		{
			//公众号窗口没关闭直接填消息
			pChatDialog = (ChatDialog*)allopenwnd[(CDuiString)strPublicID];
			pChatDialog->PostMessage(UM_USER_RECVMSG, 0, 0);
		}
		else{
			// 公众号窗口关闭 直接与客服窗口发送消息
			if (allopenwnd[(CDuiString)strKFID] != NULL)
			{
				pChatDialog = (ChatDialog*)allopenwnd[(CDuiString)strKFID];
				pChatDialog->PostMessage(UM_USER_RECVMSG, 0, 0);
			}
			else
			{
				if (txtmsg.format != eMsgFormat_Shake)
				{
					// 若未打开，图标闪烁/托盘图标闪烁
					InsertIntoFlashVt((CDuiString)strKFID, ePanel_Single);
					g_pFrame->SetNotifyiconFlash();
					// 收到了好友发送来的消息 
					m_bRecvFriendsMsg = TRUE ; 
				}
			}
		}
		// 声音提示，在不关闭的情况下
		g_pFrame->PostMessage(UM_SOUND_PLAY, IDR_WAVE_MSG);
		int nRecordType = eRecord_NewPublic;
		if(lpRecvMsg->m_ucFormat != eMsgFormat_File)
		{
			// 写入历史消息库,文件消息单独处理
			g_SqliteOp.DBInsertRecordRaw(lpRecvMsg->m_szUSERID, g_loginname, 0, lpRecvMsg->m_nSendDate, 
				lpRecvMsg->m_nSendTime, nRecordType, lpRecvMsg->m_ucFormat, lpRecvMsg->m_szMsg);
		}
		CString strDispairMsg = ConvertMsgToChiefMsg(lpRecvMsg->m_ucFormat, lpRecvMsg->m_szMsg);
		// 填充最近联系人
		ModifyDisRecentInfo(CDuiString(lpRecvMsg->m_szUSERID), "", CDuiString(strDispairMsg), GetNowTimeHourAndMin(), 0);
		return TRUE;
	}
	return 1;
}

BOOL MainFrame::ProcessRecvGroupMsg(GROUP_CHAT_HIS_MSG* lpHisMsg)
{
	char buf[MAX_SEND_BUF_LEN] = {0};
	gcrecvmsg_ans *pgcrecvmsg = (gcrecvmsg_ans *)buf;

	COPYSTRARRAY(pgcrecvmsg->cFromuserid, lpHisMsg->cFromuserid);
	pgcrecvmsg->format = lpHisMsg->format;
	memcpy(pgcrecvmsg->msg, lpHisMsg->msg, lpHisMsg->msglen);
	pgcrecvmsg->msglen = lpHisMsg->msglen;
	COPYSTRARRAY(pgcrecvmsg->nickname, lpHisMsg->nickname);
	pgcrecvmsg->nUsertype = lpHisMsg->usertype;
	pgcrecvmsg->roomid = lpHisMsg->roomid;
	pgcrecvmsg->roomtype = lpHisMsg->roomtype;
	pgcrecvmsg->tmSend = lpHisMsg->tmSend;
	ProcessRecvGroupMsg(pgcrecvmsg, TRUE);

	return TRUE;
}

//收到服务器发来的群组消息
BOOL MainFrame::ProcessRecvGroupMsg(gcrecvmsg_ans* lpRecvMsg, BOOL bOffMsg /* = FALSE */)
{
	if(!lpRecvMsg) 
		return FALSE;

	RECEIVED_MSG groupmsg;
	int nMsgLen = lpRecvMsg->msglen;
	groupmsg.msg = new char[nMsgLen+1];
	strncpy(groupmsg.msg, lpRecvMsg->msg, nMsgLen);
	groupmsg.msg[nMsgLen] = 0;
	groupmsg.format = lpRecvMsg->format;
	ConvertTimeToLong(lpRecvMsg->tmSend, groupmsg.senddate, groupmsg.sendtime);
	COPY_USERID(groupmsg.imid, lpRecvMsg->cFromuserid);
	COPY_NICKNAME(groupmsg.nickname, lpRecvMsg->nickname);
	CString strRoomID = IntToStr(lpRecvMsg->roomid);

	FriendList_ITER iterator;
	BOOL bFindNode = GetIteratorOfID(iterator, (CDuiString)strRoomID, eVT_Group, FALSE);

	// 能在好友列表中找到，说明是好友，找不到按照陌生人逻辑处理
	if (bFindNode)
	{
		if(iterator->pMsg == NULL)
			iterator->pMsg = new VECTOR_RECEIVED_MSG;
		//iterator->m_csLock.Lock();
		iterator->pMsg->push_back(groupmsg);
		//iterator->m_csLock.Unlock();
	}
	else
	{
		//陌生群组，先把消息存起来，去服务器获取群组信息
		//AfxMessageBox("节点未被创建");
	}

	ChatDialog* pChatDialog = NULL;
	CDuiString cDuisRoomID =  _T("");
	cDuisRoomID.Format("ROOM$%d", lpRecvMsg->roomid);

	if (allopenwnd[cDuisRoomID] != NULL)
	{
		// 已经打开，直接填充消息
		pChatDialog = (ChatDialog*)allopenwnd[cDuisRoomID];
		pChatDialog->PostMessage(UM_USER_RECVMSG, 0, 0);
	}
	else
	{
		// 若未打开，图标闪烁/托盘图标闪烁
		InsertIntoFlashVt(cDuisRoomID, ePanel_Group);
	}
	// 消息提示音
	g_pFrame->PostMessage(UM_SOUND_PLAY, IDR_WAVE_MSG);
	// 写入历史消息库,文件消息单独处理
	g_SqliteOp.DBInsertGRecord(lpRecvMsg->cFromuserid, lpRecvMsg->roomid, (long)lpRecvMsg->tmSend, lpRecvMsg->msg);

	return TRUE;
}

// 关闭释放
void MainFrame::ReleaseUserData()
{
	//释放收到消息的用户Msg容器
	FriendList_ITER iterator;
	for (iterator = friends_.begin(); iterator != friends_.end() ; iterator++)
	{
		if ( iterator->pMsg )
		{
			for(int i=0; i<(int)iterator->pMsg->size(); i++)
			{
				TDELARRAY( (*iterator->pMsg)[i].msg );
			}
			TDEL(iterator->pMsg);
		}
	}
	//释放收到消息的用户Msg容器
	for (iterator = groups_.begin(); iterator != groups_.end() ; iterator++)
	{
		if ( iterator->pMsg )
		{
			for(int i=0; i<(int)iterator->pMsg->size(); i++)
			{
				TDELARRAY( (*iterator->pMsg)[i].msg );
			}
			TDEL(iterator->pMsg);
		}
	}

	//关闭已经打开的单人聊天窗口
	map<CDuiString, LPVOID>::iterator iterator_map;
	for (iterator_map = allopenwnd.begin(); iterator_map != allopenwnd.end() ; iterator_map++)
	{
		if ( iterator_map->second != NULL )
		{
			ChatDialog *pdlg = (ChatDialog*)iterator_map->second;
			pdlg->OnExit();
			iterator_map->second = NULL;
		}
	}

	// 获取的全局群组状态释放
	GCR_ITER iter_room;
	for (iter_room = m_mapChatRoom.begin(); iter_room != m_mapChatRoom.end() ; iter_room++)
	{
		if ( iter_room->second.pVtUser )
		{
			TDEL(iter_room->second.pVtUser);
		}
	}

	// im2.0全局房间分析师信息释放
	ACRI_ITER iter_analys;
	for (iter_analys = m_mapAllClientRoom.begin(); iter_analys != m_mapAllClientRoom.end() ; iter_analys++)
	{
		if ( iter_analys->second.pVtAnalys )
		{
			TDEL(iter_analys->second.pVtAnalys);
		}
	}

	// 公众号的路由信息释放
	PUB_ITER iter_public;
	for (iter_public = m_mapPublicInfo.begin(); iter_public != m_mapPublicInfo.end() ; iter_public++)
	{
		if ( iter_public->second.pVtRoute )
		{
			TDEL(iter_public->second.pVtRoute);
		}
	}

	//g_fileUserLog.Close();
	return;
}

//播放声音
LRESULT MainFrame::OnSoundPlay(WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	void *p=NULL;
	switch(wParam)
	{
	case IDR_WAVE_NOTIFY:
		{
			if(!g_userconfig.m_bNotifySound) 
				return 0;
			p=m_pWave[0];
		}
		break;
	case IDR_WAVE_MSG:
		{
			if(!g_userconfig.m_bMsgSound) 
				return 0;
			p=m_pWave[1];
		}
		break;
	case IDR_WAVE_ONLINE:
		{
			if(!g_userconfig.m_bOnlineSound) 
				return 0;
			p=m_pWave[2];
		}
		break;
	case IDR_WAVE_SHAKE:
		{
			if(!g_userconfig.m_bShakeSound) 
				return 0;
			p=m_pWave[3];
		}
		break;
	default:
		break;
	}

	// 线程播放音频
	AfxBeginThread(PlaySoundThread, p, THREAD_PRIORITY_NORMAL);
	return 1;
}

// 打开跟某个用户的聊天窗口
BOOL MainFrame::OpenChatDiolog(CDuiString m_strUserID, FriendList_ITER iterator)
{
	ChatDialog* pChatDialog = NULL;
	if (allopenwnd[m_strUserID] != NULL)
	{
		pChatDialog = (ChatDialog*)allopenwnd[m_strUserID];

		::ShowWindow(*pChatDialog, SW_SHOW);
		::ShowWindow(*pChatDialog, SW_RESTORE);
		::SetWindowPos(*pChatDialog, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE);

		pChatDialog->ShowWindow(true);
		pChatDialog->PostMessage(UM_USER_RECVMSG, 0, 0);
	}
	//////////////////////////////////////////////////////////////////////////
	else
	{
		if (m_pFriendsList != NULL)
		{
			BOOL bFind = FALSE;
			FriendListItemInfo friend_info;

			for (std::vector<FriendListItemInfo>::const_iterator citer = friends_.begin(); citer != friends_.end(); citer++)
			{
				if (_tcsicmp(citer->id, m_strUserID) == 0)
				{
					friend_info = *citer;
					bFind = TRUE;
					break;
				}
			}

			// 如果在取得的好友列表里面
			if (bFind)
			{
				if (friend_info.nodetype == eNode_Single)
					pChatDialog = new ChatDialog(GetMainBkImage(), GetMainBkColor(), myself_info_, friend_info);
				else if (friend_info.nodetype == eNode_Public)
					pChatDialog = new ChatDialog(GetMainBkImage(), GetMainBkColor(), myself_info_, friend_info, eDIALOG_Public);
				else if (friend_info.nodetype == eNode_Analyst)
				{
					if (VER_UPIM_NUMS == 4)
					{
						int nRoomID = atoi(friend_info.Parentid);
						int nVipRight = m_mapAllClientRoom[nRoomID].m_nVipRight;
						if (!IsHasRights(nVipRight))
						{
							PostMessage(UM_USER_OPENRIGHTS, 0, 0);
							return FALSE;
						}
					}
					pChatDialog = new ChatDialog(GetMainBkImage(), GetMainBkColor(), myself_info_, friend_info, eDIALOG_Analys);
				}

				if( pChatDialog == NULL ) 	return FALSE;

				pChatDialog->friend_iterator = iterator;
				allopenwnd[friend_info.id] = (LPVOID)pChatDialog;

				// 打开窗口的同时，停止图标闪烁  停止好友列表中好友头像的闪动 
				StopFlashItem(friend_info.id, ePanel_Single);

				CDuiString strDialogName = friend_info.nick_name;
				pChatDialog->Create(NULL, strDialogName, UI_WNDSTYLE_FRAME | WS_POPUP, NULL, 0, 0, 0, 0);
				skin_changed_observer_.AddReceiver(pChatDialog);
				pChatDialog->SetIcon(IDI_MAINFRAME_NEW);
				pChatDialog->CenterWindow();
				::ShowWindow(*pChatDialog, SW_SHOW);

				if (friend_info.nodetype == eNode_Public && g_MyClient.IsClientLoginIn())
					g_MyClient.SendGetPublicInfoReq(atoi(friend_info.numid));

				//// 将该节点添加到最近联系人窗口 
				//if( m_pRecentLinkList)
				//{
				//	//CreateRecentNode(iterator->numid ,myself_info_.id ,iterator->Lastword ,iterator->lastwordtime)	; 								
				//}
			}
			else
			{
				USER_LOG("[OpenChatDiolog]失败，找不到节点：%s", m_strUserID);
				return FALSE;
			}
		}
	}

	return TRUE;
}

BOOL MainFrame::OpenGroupChatDiolog(CDuiString m_strRoomID, FriendList_ITER iterator)
{
	ChatDialog* pChatDialog = NULL;
	CDuiString strRoomID =  _T("");
	strRoomID.Format("ROOM$%s", m_strRoomID);

	if (allopenwnd[strRoomID] != NULL)
	{
		pChatDialog = (ChatDialog*)allopenwnd[strRoomID];

		::ShowWindow(*pChatDialog, SW_SHOW);
		::ShowWindow(*pChatDialog, SW_RESTORE);
		::SetWindowPos(*pChatDialog, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE);

		pChatDialog->ShowWindow(true);
		pChatDialog->PostMessage(UM_USER_RECVMSG, 0, 0);
	}
	//////////////////////////////////////////////////////////////////////////
	else
	{
		if (m_pFriendsList != NULL)
		{
			FriendList_ITER iterator;
			BOOL bFind = GetIteratorOfID(iterator, (CDuiString)strRoomID, eVT_Group);
			// 如果在取得的好友列表里面
			if (bFind)
			{
				FriendListItemInfo group_info = *iterator;

				pChatDialog = new ChatDialog(GetMainBkImage(), GetMainBkColor(), myself_info_, group_info, eDIALOG_Group);
				if( pChatDialog == NULL )	return FALSE;

				pChatDialog->friend_iterator = iterator;
				allopenwnd[group_info.id] = (LPVOID)pChatDialog;

				// 打开窗口的同时，停止图标闪烁
				StopFlashItem(group_info.id, ePanel_Group);

				CDuiString strDialogName = group_info.nick_name;
				pChatDialog->Create(NULL, strDialogName, UI_WNDSTYLE_FRAME | WS_POPUP, NULL, 0, 0, 0, 0);
				skin_changed_observer_.AddReceiver(pChatDialog);
				pChatDialog->SetIcon(IDI_MAINFRAME_NEW);
				pChatDialog->CenterWindow();
				::ShowWindow(*pChatDialog, SW_SHOW);

				g_MyClient.SendGcUserListReq(atol(group_info.numid), group_info.roomtype, 0);
			}
			else
			{
				USER_LOG("[OpenGroupChatDiolog]失败，找不到节点：%s", m_strRoomID);
				return FALSE;
			}
		}
	}

	return TRUE;
}

LRESULT MainFrame::OnOpenWND(WPARAM wParam, LPARAM lParam)
{
	CString str((CHAR*)g_szOpenUID); 
	CDuiString strDuiStr = str;

	FriendList_ITER iterator;
	switch(wParam)
	{
	case eMSG_Single:
		{
			BOOL bFindNode = GetIteratorOfID(iterator, strDuiStr, eVT_Friend);
			OpenChatDiolog(strDuiStr, iterator);
		}
		break;
	case eMSG_Group:
		{
			BOOL bFindNode = GetIteratorOfID(iterator, strDuiStr, eVT_Group, FALSE);
			OpenGroupChatDiolog(strDuiStr, iterator);
		}
		break;
	default:
		break;
	}

	return 1;
}

LRESULT MainFrame::OnAddFriendInfo(WPARAM, LPARAM)
{
	CreateFriendNode();
	return 1;
}

// 创建聊天室
LRESULT MainFrame::OnAddRoom(WPARAM wParam, LPARAM lParam)
{
	CreateRoomNode();
	return 1;
}

// 用户列表
void MainFrame::DeleteRoomMember(ROOMID mRoomID, LPCTSTR szID, short nUserType)
{
	CDuiString strRoomID = _T("");
	strRoomID.Format("ROOM$%d", mRoomID);

	if (allopenwnd[strRoomID] != NULL)
	{
		ChatDialog* pChatDialog = NULL;
		pChatDialog = (ChatDialog*)allopenwnd[strRoomID];
		pChatDialog->DeleteMember(szID, nUserType);
	}
}

BOOL MainFrame::DownLoadPic(CString strUrl, CString strFile)
{
	GenericHTTPClient *httpClient = new GenericHTTPClient;
	if( strUrl.IsEmpty())
		return FALSE ; 

	if(httpClient->Request(strUrl, GenericHTTPClient::RequestGetMethod, ""))
	{
		LPCTSTR szResult;
		szResult = httpClient->QueryHTTPResponse();    //返回的数据流
		int nSize = httpClient->GetResponseBodySize();
		if (nSize > 1200)
		{
			CFile file;
			if(file.Open((LPCTSTR)strFile, CFile::modeCreate|CFile::modeWrite))
			{
				file.Write((PBYTE)szResult, httpClient->GetResponseBodySize());
				TDEL(httpClient);
				USER_LOG("FILE: [%s]", strFile);
				file.Close();
			}

		}
		else
		{
			TDEL(httpClient);
			USER_LOG("Pic size Error [%s](%d)", strUrl, nSize);
			return FALSE;
		}
	}
	else
	{
		TDEL(httpClient);
		USER_LOG("http req Error [%s]", strUrl);
		return FALSE;
	}
	return TRUE;
}

// 打开开通权限窗口
LRESULT MainFrame::OnOpenRight(WPARAM wParam, LPARAM lParam)
{
	OpenRightsDlg();
	return 1;
}
// 加载最近联系人
LRESULT MainFrame::OnLoadRecentClientNode(WPARAM wParam, LPARAM lParam)
{
	// 查询所有联系人
	std::vector<CString> vtUsers;
	if(!g_SqliteOp.QueryAllAnalystUser(vtUsers, eRecord_TgRoom, g_nRoomID_im2_0))
		return 0;
	// 当前分析师的ID 
	CString strAnalyName = g_MyClient.m_strUserid ; 

	for(int index=0; index < (int)vtUsers.size(); ++index)
	{
		CString strUser = vtUsers[index];
		if (strUser.CompareNoCase(g_MyClient.m_strUserid) == 0)
		{
			// 最近联系人是自己 排除 
		}
		else
		{
			std::vector< CHAT_RECORD2 > vtRecord ; 
			long currentDate = 0 ;
			CString year ,month ,day ;
			GetCurrentDate(year,month,day) ; 
			year = year + month + day ; 
			currentDate = atol(year.GetBuffer()) ; 
			// 其他人，按照时间先后顺序添加进m_pRecentLinkList
			if( g_SqliteOp.QueryLastRecordByEachID(vtRecord ,eRecord_TgRoom ,strAnalyName ,strUser,currentDate,0 ,g_nRoomID_im2_0) )
			{
				int nRecordNum = vtRecord.size() ; 
				// 取第一条就OK了 
				if( nRecordNum < 0 )
					return 0 ; 

				CHAT_RECORD2 char_record = vtRecord[0] ;
				CString strLastWord1 = _T(" ") ; 
				strLastWord1.Format("%s",char_record.msg) ;


				CDuiString strNodeID = _T(" ") ;
				strNodeID.Format("%s",strUser) ; 
				CDuiString strUserName = _T(" "); 
				strUserName.Format("%s" ,strAnalyName) ;

				// 发送的文字消息 
				CString msg = ConvertMsgToChiefMsg(char_record.format,strLastWord1) ; 
				CDuiString strLastWord = _T(" ") ;
				strLastWord.Format("%s" ,msg) ; 
				int date = char_record.senddate ; 
				int time = char_record.sendtime ; 

				long long_time ; 
				// 要将date time 转化为时间戳long_time存起来 
				ConvertTimeToLong2(date,time,long_time) ; 
				CreateRecentNode(strNodeID, strUserName, strLastWord, long_time);
			}
		}
	}
	// 选择最后一条信息  
	return 1 ; 
}

//////////////////////////////////////////////////////////////////////////////////
//wuchao add at 2016-04-05 
// 好友列表中查找好友
//////////////////////////////////////////////////////////////////////////////////
LRESULT MainFrame::OnFindFriends(WPARAM wParam, LPARAM lParam)
{

	return 1 ; 
}
LRESULT MainFrame::OnAddNewLastUser(WPARAM wParam, LPARAM lParam)
{
	char* saleid = (char*)wParam;
	FriendListItemInfo item;
	item.numid =saleid;
	item.id = saleid;
	item.folder = false;
	item.empty = false;
	item.logo = _T("public.png");
	item.nick_name = saleid;
	item.showname =  saleid;
	item.nodetype = eNode_RYPublic;
	item.type = eNode_RYPublic;
	item.description = "";
	friends_.push_back(item);
	return 1;
}

LRESULT MainFrame::OnAddNewPublic(WPARAM wParam, LPARAM lParam)
{
	if (m_pFriendsList != NULL)
	{
		Node* retrunptr = NULL;
		std::map<CString,NEWPUBLIC_INFO>::iterator iter_ = g_mapNewPublicInfo.begin();
		for (;iter_!=g_mapNewPublicInfo.end();iter_++)
		{
			FriendListItemInfo item;
			NEWPUBLIC_INFO newPublicInfo = iter_->second;
			COPYSTRARRAY(newPublicInfo.publicnum,iter_->second.publicnum);               //IM公众帐号编码
			COPYSTRARRAY(newPublicInfo.publiccode,iter_->second.publiccode);             //公众号编号
			COPYSTRARRAY(newPublicInfo.publicname,iter_->second.publicname);             //公众号名称
			std::vector<IncodeInfo>* pincode = newPublicInfo.pIncode;
		
			item.numid =newPublicInfo.publicnum;
			item.id = newPublicInfo.publicnum;
			item.folder = false;
			item.empty = false;
			item.logo = _T("public.png");
			item.nick_name = newPublicInfo.publicname;
			item.showname =  newPublicInfo.publicname;
			item.nodetype = eNode_RYPublic;
			item.type = eNode_RYPublic;
			item.description = newPublicInfo.publicname;

			retrunptr = m_pFriendsList->AddNode(item, NULL);
				
			if (retrunptr)
				friends_.push_back(item);
		
		}
		//USER_LOG("CreatePUBLIC[%d] NAME:%s GroupID:%s", m_nPublicID, m_szGroupName.GetData(), mPublicID.GetData());
	}
	PostMessage(UM_USER_GETUSERPIC, 0, 0);
	return 1;
}

LRESULT MainFrame::OnOpenChatDialog(WPARAM wParam, LPARAM lParam) 
{
	if( VER_UPIM_NUMS != 4)
	{
		return 0; 
	}
	USER_LOG("开始响应UP客户端发过来的消息") ; 
	if(!m_pFriendsList)
		return NULL ;
	USER_LOG("m_pFriendsList朋友列表存在") ;
	USER_LOG("分析师ID%d" ,(DWORD)(wParam)) ;

	CString strClientID = _T(" ") ; 
	strClientID.Format(_T("%d") ,(DWORD)(wParam)) ; 
	FriendList_ITER _iterator ; 
	FriendListItemInfo friend_info  ; 
	memset(&friend_info ,0,sizeof(FriendListItemInfo)) ;

	std::vector<FriendListItemInfo>::iterator itBegin = friends_.begin() ; 
	while( itBegin != friends_.end())
	{
		friend_info = *itBegin ; 
		if( strcmp(friend_info.numid.GetData() ,strClientID.GetBuffer())==0 && (!friend_info.folder))
		{
			USER_LOG("分析师ID%d" ,(DWORD)(wParam)) ;
			_iterator = itBegin ;
			USER_LOG("在朋友列表中找到分析师节点,ID=%s",friend_info.numid.GetData()) ;
			break ;
		}
		itBegin++ ; 
	}
	if( itBegin == friends_.end())
	{
		USER_LOG("在朋友列表中没有找到该分析师") ;
		return 0 ; 
	}
	if( allopenwnd[friend_info.id] != NULL )
	{
		USER_LOG("分析师对话框已经打开，直接聊天即可") ;
		ChatDialog* pChatDialog = (ChatDialog*)allopenwnd[friend_info.id];
		::SetWindowPos(*pChatDialog, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE);
		//::ShowWindow(*pChatDialog, SW_SHOW);
		//pChatDialog->ShowWindow(true);
	}
	else
	{
		ChatDialog* pChatDialog = NULL;
		if( friend_info.nodetype == eNode_Analyst)
		{
			if (VER_UPIM_NUMS == 4)
			{
				USER_LOG("新建与分析师对话窗口") ;
				int nRoomID = atoi(friend_info.Parentid);
				int nVipRight = m_mapAllClientRoom[nRoomID].m_nVipRight;
				if (!IsHasRights(nVipRight))
				{
					PostMessage(UM_USER_OPENRIGHTS, 0, 0);
					return 0;
				}
			}
			pChatDialog = new ChatDialog(GetMainBkImage(), GetMainBkColor(), myself_info_, friend_info, eDIALOG_Analys);
		}

		if( pChatDialog == NULL )
			return 0 ;
		pChatDialog->friend_iterator = _iterator;
		allopenwnd[friend_info.id] = (LPVOID)pChatDialog;
		CDuiString strDialogName = friend_info.nick_name;
		pChatDialog->Create(NULL, strDialogName, UI_WNDSTYLE_FRAME | WS_POPUP, NULL, 0, 0, 600, 800);
		skin_changed_observer_.AddReceiver(pChatDialog);
		pChatDialog->SetIcon(IDI_MAINFRAME_NEW);

		pChatDialog->CenterWindow();
		::ShowWindow(*pChatDialog, SW_SHOW);
	}
	return 1 ; 
}
// 获取自己的头像
LRESULT MainFrame::OnGetSelfPic(WPARAM wParam, LPARAM lParam)
{
	CString strDown = "";		// 下载的头像
	CString strHead = "";		// 在线的正常头像
	CString strHead2 = "";		// 离线时的灰化头像
	strDown.Format("%s\\my.jpg", g_config.szUserHead);
	strHead.Format("%s\\my_n.jpg", g_config.szUserHead);
	strHead2.Format("%s\\my_g.jpg", g_config.szUserHead);
#ifdef VER_UPIM_RONGYUN
	BOOL bIsImgDownLoad = DownLoadPic(g_userHeadUrl, strDown);
	// 将头像转换成62*62像素
	if (bIsImgDownLoad)
	{
		bool bHandImg = HandleHeadImage2(strDown, "my_n.jpg", "my_g.jpg");
		if (bHandImg)
			SetMainFace2(eUSER_ONLINE, strHead, strHead2);
	}
	else
		SetMainFace2(eUSER_ONLINE, "self2.png", "self2.png");
#endif

	// 分析师新版本
	if (VER_UPIM_NUMS == 3)
	{
		// 先吧原始头像下载在strDown my.jpg中
		BOOL bIsImgDownLoad = DownLoadPic(g_analysheadurl, strDown);
		// 将头像转换成62*62像素
		if (bIsImgDownLoad)
		{
			bool bHandImg = HandleHeadImage2(strDown, "my_n.jpg", "my_g.jpg");
			if (bHandImg)
				SetMainFace2(eUSER_ONLINE, strHead, strHead2);
		}
		else
			SetMainFace2(eUSER_ONLINE, "self2.png", "self2.png");
	}
	// 客户新版本
	else if (VER_UPIM_NUMS == 4)
	{
#ifndef IM_EXE_TO_UP
		CString strURL = GetUserPicURL(g_loginname);
		if( strURL.IsEmpty())
			return 0 ; 
		// 先吧原始头像下载在strDown my.jpg中
		BOOL bIsImgDownLoad = DownLoadPic(strURL, strDown);
		// 将头像转换成62*62像素
		if (bIsImgDownLoad)
		{
			bool bHandImg = HandleHeadImage2(strDown, "my_n.jpg", "my_g.jpg");
			if (bHandImg)
				SetMainFace2(eUSER_ONLINE, strHead, strHead2);
		}
		else
			SetMainFace2(eUSER_ONLINE, "self2.png", "self2.png");
#endif 
	}
	return 1;
}

LRESULT MainFrame::OnGetUserPic(WPARAM wParam, LPARAM lParam)
{
	// 分析师版本客户信息存放在m_vtRoomClient中，是每一个客户的信息
	if (VER_UPIM_NUMS == 3)
	{
		AfxBeginThread(DownUserPicFromWeb,this, THREAD_PRIORITY_NORMAL) ; 
	}
	// 客户版本的分析师信息
	else if (VER_UPIM_NUMS == 4)
	{
		USER_LOG("[UM_USER_GETUSERPIC]客户获取分析师头像 ");
		AfxBeginThread(DownAnalysPicFromWeb,this ,THREAD_PRIORITY_NORMAL) ;
	}
	return 1 ; 
}

// 离开聊天室
LRESULT MainFrame::OnLeaveRoom(WPARAM wParam, LPARAM lParam)
{
	ROOMID mRoomID = (ROOMID)lParam;

	// 如果已经打开了这个群组聊天界面，Close
	CDuiString strRoomID = _T("");
	strRoomID.Format("ROOM$%d", mRoomID);

	if (allopenwnd[strRoomID] != NULL)
	{
		ChatDialog* pChatDialog = NULL;
		pChatDialog = (ChatDialog*)allopenwnd[strRoomID];

		pChatDialog->OnExit();
	}

	// 在群组列表中删除这个节点
	if (m_pGroupsList != NULL)
	{
		Node *m_thisNode =  m_pGroupsList->SelectNodeByID(strRoomID.GetData());
		if (m_thisNode)
		{
			BOOL bRemoveSuccess = FALSE;
			bRemoveSuccess = m_pGroupsList->RemoveNode(m_thisNode);
		}
	}

	//从m_mapChatRoom中删除数据
	GCR_ITER iter_ = m_mapChatRoom.find(mRoomID);
	if (iter_->second.pVtUser)
		iter_->second.pVtUser->clear();
	m_mapChatRoom.erase(iter_);

	return 1;
}

LRESULT MainFrame::OnUserColse(WPARAM wParam, LPARAM lParam)
{
	ReleaseUserData();
	NotifyMessage(NIM_DELETE, m_hIcon, "");
	Close();
	return 1;
}

// 根据不同的用户类型，不同的权限，隐藏部分控件
void MainFrame::SetControlVisable()
{
	CButtonUI* m_pGroupBtn = static_cast<CButtonUI*>(m_PaintManager.FindControl( kGroupButtonControlName ));
	CButtonUI* m_pWeatherBtn = static_cast<CButtonUI*>(m_PaintManager.FindControl( kWeatherButtonControlName ));
	if (m_pWeatherBtn)
		m_pWeatherBtn->SetVisible(false);
#ifdef VER_UPIM_ANALYS
	if (g_MyClient.m_nUserType == eUser_UPAnalyst || g_MyClient.m_nUserType == eUser_Client)
	{
		if (m_pGroupBtn)
			m_pGroupBtn->SetVisible(false);
	}
#endif
}

LRESULT MainFrame::OnUserLoginIN(WPARAM wParam, LPARAM lParam)
{
	SetControlVisable();

	if (g_nUserType != eUser_UPClient)
	{

#ifdef IM_EXE_TO_UP
		::ShowWindow(*g_pFrame, SW_HIDE);
#else
		::ShowWindow(*g_pFrame, SW_SHOW);
#endif 
		// wuchao add at 2016-04-06
		if( VER_UPIM_NUMS == 3 )
			g_pFrame->m_pSearch_edit->SetFocus() ; 
	}
	if (pLoginDialog)
	{
		pLoginDialog->Close();
		bLoginDialogClosed = TRUE;
	}

	return 1;
}

LRESULT MainFrame::OnUserOffLine(WPARAM wParam, LPARAM lParam)
{
	SetMainFace(eUSER_OFFLINE);
	return 1;
}

LRESULT MainFrame::OnUserOnLine(WPARAM wParam, LPARAM lParam)
{
	SetMainFace(eUSER_ONLINE);
#ifndef VER_UPIM_RONGYUN    // 取得离线消息
	g_MyClient.SendOFFMSGReq();
#endif
	return 1;
}

//设置自己头像
BOOL MainFrame::SetMainFace2(EUSERSTATE nstate, LPCSTR szFile1 /* = "" */, LPCSTR szFile2 /* = "" */)
{
	CButtonUI* m_pFaceBtn = static_cast<CButtonUI*>(m_PaintManager.FindControl( kFaceButtonControlName ));
	CButtonUI* m_pStateBtn = static_cast<CButtonUI*>(m_PaintManager.FindControl( kStateButtonControlName ));

	TCHAR szBuf[MAX_PATH] = {0};
	if (m_pFaceBtn)
	{
		if (nstate == eUSER_ONLINE)
		{
			_stprintf_s(szBuf, MAX_PATH - 1, _T("file='%s'"), szFile1);
			m_pFaceBtn->SetBkImage(szBuf);
			_stprintf_s(szBuf, MAX_PATH - 1, _T("file='%s' mask='0xffffffff'"), szFile1);
			m_pFaceBtn->SetNormalImage(szBuf);
		}
		else if (nstate == eUSER_OFFLINE)
		{
			_stprintf_s(szBuf, MAX_PATH - 1, _T("file='%s'"), szFile2);
			m_pFaceBtn->SetBkImage(szBuf);
			_stprintf_s(szBuf, MAX_PATH - 1, _T("file='%s' mask='0xffffffff'"), szFile2);
			m_pFaceBtn->SetNormalImage(szBuf);
		}
	}
	return TRUE;
}

// 设置分析师界面中分析师房间的名字 
BOOL MainFrame::SetAnalysNodeNickName(int nRoomID ,int onlineNums /*= 0*/ ,int totalNums /*= 0*/)
{
	//ANALYST$1149
	CDuiString mUserNode = _T("") ;
	mUserNode.Format("ANALYST$%d", nRoomID);
	CString strName = _T("") ;
	strName.Format(" %d/%d",onlineNums,totalNums) ; 

	Node* m_nodeuser = NULL ;
	if( m_pFriendsList )
	{
		m_nodeuser = m_pFriendsList->SelectNodeByID(mUserNode.GetData());
		m_pFriendsList->SetNodeName(m_nodeuser ,onlineNums,totalNums) ; 
	}
	return TRUE; 
}


//设置好友头像
BOOL MainFrame::SetAnalysNodeHead(EUSERSTATE nstate, int nRoomID, int nAnalysID, LPCSTR szFile1 /* = "" */, LPCSTR szFile2 /* = "" */)
{
	CDuiString mUserNode = _T("");
	mUserNode.Format("TGUSER$%d$%d", nRoomID, nAnalysID);

	Node* m_nodeuser = NULL;
	//////////////////////////////////////////////////////////////////////////
	// 好友列表中的节点
	if (m_pFriendsList)
	{
		m_nodeuser = m_pFriendsList->SelectNodeByID(mUserNode.GetData());
		if( nstate == eUSER_ONLINE )
			m_pFriendsList->SetNodePic(m_nodeuser, szFile1);
		else if( nstate == eUSER_OFFLINE )
			m_pFriendsList->SetNodePic(m_nodeuser ,szFile2) ; 
		else if( nstate == eUSER_LIKAI )
		{
			// 离开也设置成离线的状态 
			m_pFriendsList->SetNodePic(m_nodeuser ,szFile2) ;
		}
	}

	return TRUE;
}

BOOL MainFrame::SetGroupClientNodeHead(EUSERSTATE nstate, USER_ROOMCLIENT_INFO userClientInfo, LPCSTR szFile1, LPCSTR szFile2 )
{
	if( !m_pSelfGroup )
		return FALSE ;

	CDuiString mNodeID = _T("");
	int nRoomID = g_nRoomID_im2_0;
	mNodeID.Format("TGUSER$%d$%s", nRoomID, userClientInfo.m_szUPID);

	Node* pNode = m_pSelfGroup->SelectNodeByID(mNodeID.GetData()) ; 
	if(!pNode)
		return FALSE; 
	
	if( nstate == eUSER_ONLINE )
		m_pSelfGroup->SetNodePic(pNode, szFile1);
	else if( nstate == eUSER_OFFLINE )
		m_pSelfGroup->SetNodePic(pNode ,szFile2) ; 
	else if( nstate == eUSER_LIKAI )
	{
		// 离开也设置成离线的状态 
		m_pSelfGroup->SetNodePic(pNode ,szFile2) ;
	}

	return TRUE ; 
}
//设置好友头像 分析师界面  设置客户的头像
// szFile1 上线头像 
// szFile2 离线头像  
BOOL MainFrame::SetClientNodeHead(EUSERSTATE nstate, CString strClientID, LPCSTR szFile1, LPCSTR szFile2 )
{
	CDuiString mNodeID = _T("");
	int nRoomID = g_nRoomID_im2_0;
	mNodeID.Format("TGUSER$%d$%s", nRoomID, strClientID);

	Node* m_nodeuser = NULL ; 
	if ( m_pFriendsList) 
	{
		// 在好友列表中找到strClientID节点 
		m_nodeuser = m_pFriendsList->SelectNodeByID(mNodeID.GetData());

		if( !m_nodeuser)
			return FALSE ; 

		if( nstate == eUSER_ONLINE )
			m_pFriendsList->SetNodePic(m_nodeuser, szFile1);
		else if( nstate == eUSER_OFFLINE )
			m_pFriendsList->SetNodePic(m_nodeuser ,szFile2) ; 
		else if( nstate == eUSER_LIKAI )
		{
			// 离开也设置成离线的状态 
			m_pFriendsList->SetNodePic(m_nodeuser ,szFile2) ;
		}
	}
	return TRUE;
}
BOOL MainFrame::SetMainFace(EUSERSTATE nstate, LPCSTR szFile /* = "" */)
{
	CButtonUI* m_pFaceBtn = static_cast<CButtonUI*>(m_PaintManager.FindControl( kFaceButtonControlName ));
	CButtonUI* m_pStateBtn = static_cast<CButtonUI*>(m_PaintManager.FindControl( kStateButtonControlName ));

	TCHAR szBuf[MAX_PATH] = {0};
	if (m_pFaceBtn)
	{
		// 获取用户的头像  

		CString strHead = "";		// 在线的正常头像
		CString strHead2 = "";		// 离线时的灰化头像

		strHead.Format("%s\\%s.jpg", g_config.szUserHead, "my_n");
		strHead2.Format("%s\\%s.jpg", g_config.szUserHead, "my_g");


		// 在线 
		if (nstate == eUSER_ONLINE)
		{
			//_stprintf_s(szBuf, MAX_PATH - 1, _T("file='man_big.png'"));
			//m_pFaceBtn->SetBkImage(szBuf);
			//_stprintf_s(szBuf, MAX_PATH - 1, _T("file='man_big.png' mask='0xffffffff'"));
			m_pFaceBtn->SetNormalImage((LPCTSTR)strHead);
		}
		// 离线 
		else if (nstate == eUSER_OFFLINE)
		{
			//_stprintf_s(szBuf, MAX_PATH - 1, _T("file='man_big_off.png'"));
			//m_pFaceBtn->SetBkImage(szBuf);
			//_stprintf_s(szBuf, MAX_PATH - 1, _T("file='man_big_off.png' mask='0xffffffff'"));
			m_pFaceBtn->SetNormalImage((LPCTSTR)strHead2);
		}
		// 离开
		else if( nstate == eUSER_LIKAI)
		{
			m_pFaceBtn->SetNormalImage((LPCTSTR)strHead2); 
		}
	}
	if (m_pStateBtn)
	{
		if (nstate == eUSER_ONLINE)
		{
			_stprintf_s(szBuf, MAX_PATH - 1, _T("file='icon_online.png' dest='3,3,17,17'"));
			m_pStateBtn->SetBkImage(szBuf);
		}
		else if (nstate == eUSER_OFFLINE)
		{
			_stprintf_s(szBuf, MAX_PATH - 1, _T("file='icon_offline.png' dest='3,3,17,17'"));
			m_pStateBtn->SetBkImage(szBuf);
		}
		else if (nstate == eUSER_LIKAI)
		{
			_stprintf_s(szBuf, MAX_PATH - 1, _T("file='icon_likai.png' dest='3,3,17,17'"));
			m_pStateBtn->SetBkImage(szBuf);
		}
	}
	m_eUserState = nstate ; 
	// 设置托盘图标 
	SetNotifyIcon(nstate) ; 
	return TRUE;
}

LRESULT MainFrame::OnUserADDGroup(WPARAM,LPARAM)
{
	CreateGroupNode();
	CreateDeptNode();
	return 1;
}

LRESULT MainFrame::OnUserADDPublic(WPARAM, LPARAM)
{
	CreatePublicNode();
	AddPublicAccNode();
	return 1;
}

LRESULT MainFrame::OnUserADDTgRoom(WPARAM, LPARAM)
{
	CreateTgRoomNode();
	AddTgRoomAccNode();

	CreateTgHisNode();
	CreateTgRoomHisNode();
	return 1;
}

LRESULT MainFrame::OnCreatSelfGroup(WPARAM wParam, LPARAM lParam)
{
	CreatSelfGroup();
	return 1;
}

LRESULT MainFrame::OnUserADDTgRoom2(WPARAM, LPARAM)
{
	CreateTgRoomNode2();
	return 1;
}

LRESULT MainFrame::OnADDRoomClient(WPARAM, LPARAM)
{
	CreateRoomClientNode();
	PostMessage(UM_USER_GETUSERPIC, 0, 0);
	return 1;
}

// 客户登录时，添加每个分组（服务器传回来的分组，不是定义的分组，理解成房间）
LRESULT MainFrame::OnADDRoomAnalys(WPARAM, LPARAM)
{
// wuchao modify on 20160509 不加载好友列表 
	DWORD t1 = GetTickCount() ; 
	USER_LOG("开始添加分析师节点:%d" ,t1);
	CreateRoomAnalysNode();
	DWORD t2 = GetTickCount() ;
	USER_LOG("添加分析师节点结束:%d" ,t2);

	PostMessage(UM_USER_GETUSERPIC, 0, 0);
	return 1;
}

LRESULT MainFrame::OnUserStateChange(WPARAM wParam, LPARAM lParam)
{
	if (g_TGChatDialog)
		g_TGChatDialog->OnUserStateChange(wParam, lParam);
	//	g_TGChatDialog->ShowFXS(TRUE, (int)wParam, (int)lParam);

	//  std::map<CDuiString, LPVOID>		allopenwnd
	std::map<CDuiString, LPVOID>::iterator itBegin = allopenwnd.begin() ; 
	while( itBegin != allopenwnd.end() )
	{
		ChatDialog *pOpenedChatDlg = (ChatDialog *)itBegin->second ; 
		if(pOpenedChatDlg)
		{
			pOpenedChatDlg->OnUserStateChange(wParam, lParam);
		}
		itBegin++ ; 
	}
	return 1;
}

LRESULT MainFrame::OnSetSelfInfo(WPARAM, LPARAM)
{
	SetAppTitle(m_AppTitle);
	SetSelfInfo();

	return 1;
}

LRESULT MainFrame::OnUIMessageLoop(WPARAM, LPARAM)
{
	CPaintManagerUI::SetCloseFlag(false);
	g_UIMessageLoopStart = TRUE;
	CPaintManagerUI::MessageLoop();
	return 1;
}

void MainFrame::SetAppTitle(CDuiString m_strApptitle)
{
	CLabelUI* pLabelText = static_cast<CLabelUI*>(m_PaintManager.FindControl(kTitleControlName));
	if( pLabelText == NULL ) return;
	pLabelText->SetText(m_strApptitle);
	return;
}

void MainFrame::SetSelfInfo()
{
	if (!m_bSetSelfInfo)
	{
		CControlUI* selfname = m_PaintManager.FindControl(kSelfNameControlName);
		if (selfname != NULL)
		{
			if (!myself_info_.nick_name.IsEmpty())
			{
				selfname->SetText(myself_info_.nick_name);
				m_bSetSelfInfo = true;
			}
		}

		//如果个性签名不为空
		if (!myself_info_.description.IsEmpty())
		{
			CControlUI* signature_tip = m_PaintManager.FindControl(kSignatureTipsControlName);
			if (signature_tip != NULL)
			{
				signature_tip->SetText(myself_info_.description);
				signature_tip->SetToolTip(myself_info_.description.GetData());
			}
		}
	}

	//#ifndef IM_EXE_TO_UP
	//////////////////////////////////////////////////////////////////////////
	// 如果是客户聊天室，不显示托盘图标
	if (g_nUserType != eUser_UPClient)
		SetNotifyIcon();
	//#else

	//#endif


	return;
}

void MainFrame::ModifySign(CDuiString m_strSign)
{
	//发送消息通知服务器，更新服务器上的签名！~
	CControlUI* signature_tip = m_PaintManager.FindControl(kSignatureTipsControlName);
	if (signature_tip != NULL)
	{
		signature_tip->SetToolTip(m_strSign.GetData());
	}
	return;
}

void MainFrame::OnMenuClick(WPARAM wParam , LPARAM lParam)
{
	UNREFERENCED_PARAMETER(wParam);
	CDuiString *strMenuName = (CDuiString*)wParam;
	bool bChecked = (bool)(lParam == 1);		 

	if (*strMenuName == _T("Menu_FZ_New"))
		OnMenu_CreateSubGroup();
	else if (*strMenuName == _T("Menu_FZ_Mod"))
		OnMenu_ModifySubGroup();
	else if (*strMenuName == _T("Menu_FZ_Del"))
		OnMenu_DeleteSubGroup();
	else if (*strMenuName == _T("Main_Menu_exit"))
		OnExit();
	else if (*strMenuName == _T("Main_Menu_online"))
		OnMenu_UserOnline(); // 用户在线 
	else if (*strMenuName == _T("Main_Menu_offline"))
		OnMenu_UserOffline(); // 离线 
	else if (*strMenuName == _T("Main_Menu_outline"))
		OnMenu_UserLiKai();  // 离开 
	else if (*strMenuName == _T("Notify_Menu_OpenMain"))
		g_pFrame->ShowWindow(TRUE);
	else if (*strMenuName == _T("Menu_Friend_Send"))
		OnMenu_FriendOpen();
	else if (*strMenuName == _T("Menu_Friend_Info"))
		OnMenu_FriendInfo();
	else if (*strMenuName == _T("Menu_Friend_Histroy"))
		OnMenu_FriendHistroy();
	else if (*strMenuName == _T("Group_List_his"))
		OnMenu_FriendHistroy();
	else if (*strMenuName == _T("Main_Menu_Histroy"))
		OnMenu_FriendHistroy();
	else if (*strMenuName == _T("Group_List_create"))
		OnMenu_CreateGroup();
	else if (*strMenuName == _T("Group_List_send"))
		OnMenu_GroupOpen();
	else if (*strMenuName == _T("Group_List_quit"))
		OnMenu_GroupQuit();
	else if (*strMenuName == _T("Menu_XS_NameAndID"))
	{
		g_userconfig.m_nNickType = eNICK_nameandid;
		if (m_pFriendsList != NULL)
			m_pFriendsList->SetNickNameType(0);
	}
	else if (*strMenuName == _T("Menu_XS_Id"))
	{
		g_userconfig.m_nNickType = eNICK_id;
		if (m_pFriendsList != NULL)
			m_pFriendsList->SetNickNameType(1);
	}
	else if (*strMenuName == _T("Menu_XS_Name"))
	{
		g_userconfig.m_nNickType = eNICK_name;
		if (m_pFriendsList != NULL)
			m_pFriendsList->SetNickNameType(2);
	}
	TDEL(strMenuName);
	return;
}

// 处理鼠标在托盘上面的事件
LRESULT MainFrame::MessageNotify( TrayIconMessage uMsg )
{
	switch(uMsg)
	{
	case WM_MOUSE_ICON_HOVER:
		OnShowNotifyDialog() ;
		break ; 

	case WM_MOUSE_ICON_ENTRY:
		//OnShowNotifyDialog() ;
		break ; 

	case WM_MOUSE_ICON_LEAVE :
		{
			POINT pt ; 
			GetCursorPos(&pt) ; 

			if( m_rtMsgDlg.PtInRect(pt))
			{
				::ShowWindow((HWND)m_NotifyDlg,TRUE) ;
				m_rtMsgDlg = CRect(0,0,0,0) ;
			}
			else
				OnCloseNotifyDialog() ;
		}
		break ; 
	}
	return 0 ; 
}
// 响应托盘事件 
void MainFrame::OnNotifyIcon(WPARAM wParam , LPARAM lParam)
{
	UNREFERENCED_PARAMETER(wParam);
	switch (lParam)
	{
		// 当UP调用IM时  屏蔽托盘图标的右键功能  
#ifndef IM_EXE_TO_UP
	case WM_RBUTTONDOWN:
		{
			POINT pt = {0};
			POINT m_menuPoint = {0};
			GetCursorPos(&pt);
			m_menuPoint.y = pt.y-180;
			m_menuPoint.x = pt.x-135;
			CMenuWnd* pMenu = new CMenuWnd();
			pMenu->Init(NULL, _T("menu_NotifyIcon.xml"), m_menuPoint, &m_PaintManager );
			pMenu->SetIconAndSize(_T("Notify_Menu_gbsy"), _T("right.png"), 9, 9);
			StopNotifyiconFlash();
		}
		break;
#endif 
	case WM_LBUTTONUP:
		{
			if( m_vecWhoSendMsg.size() == 0 )
			{
#ifdef IM_EXE_TO_UP
				ShowWindow(FALSE, FALSE);
#else
				ShowWindow(TRUE, TRUE);
#endif 
				SetForegroundWindow(m_hWnd);
			}
			else
			{
				// 打开消息发送方的ChatDialog
				FriendList_ITER iter ; 
				for ( iter = m_vecWhoSendMsg.begin() ; iter != m_vecWhoSendMsg.end() ; iter++)
				{
					OpenChatDiolog(iter->id,iter) ; 
				}
				m_vecWhoSendMsg.clear() ; 
			}
			StopNotifyiconFlash();
		}
		break;
	case WM_LBUTTONDBLCLK:
		{
			if( m_vecWhoSendMsg.size() == 0 )
			{
#ifdef IM_EXE_TO_UP
				ShowWindow(FALSE, FALSE);
#else
				ShowWindow(TRUE, TRUE);
#endif 
			}
			else
			{
				// 打开消息发送方的ChatDialog
				FriendList_ITER iter ; 
				for ( iter = m_vecWhoSendMsg.begin() ; iter != m_vecWhoSendMsg.end() ; iter++)
				{
					OpenChatDiolog(iter->id,iter) ; 
				}
				m_vecWhoSendMsg.clear() ; 
			}
			SetForegroundWindow(m_hWnd);
			StopNotifyiconFlash();
		}
		break;
	case WM_MOUSEMOVE:
		{	
			TRACE("鼠标悬停\n");
			// 刚开始 m_bMouseEnter为 false 
			// 鼠标进入托盘图标
			if( !m_bIsIcon)
			{
				if( !m_bMouseEnter )
				{
					m_bMouseEnter = TRUE ;
					MessageNotify(WM_MOUSE_ICON_ENTRY) ; 
				}

				::CPoint pt ; 
				GetCursorPos(&pt) ; 

				if( pt.x != m_ptMouseHover.x || pt.y != m_ptMouseHover.y )
				{
					KillTimer(m_hWnd, T_TRAY_HOVER_TIMER_ID);  
					KillTimer(m_hWnd, T_TRAY_LEAVE_TIMER_ID);  
					AddTimer(T_TRAY_HOVER_TIMER_ID, 300U);  
					AddTimer(T_TRAY_LEAVE_TIMER_ID, 2000U);
					m_ptMouseHover = pt ; 
				}			 
			}
		}
	default:
		break;
	}
	return;
}


void MainFrame::KillMyTimer()
{
	KillTimer(m_hWnd, T_TRAY_LEAVE_TIMER_ID);   
}


void MainFrame::AddMyTimer()
{
	AddTimer(T_TRAY_LEAVE_TIMER_ID,2000U);   
}


void MainFrame::OnShowNotifyDialog()
{
	m_bIsMsgDlgShow = TRUE ;
	// 这里可以做一个像QQ一样的消息提示框 
	if (!::IsWindow((HWND)m_NotifyDlg))
	{
		m_NotifyDlg.Create(GetDesktopWindow(), NULL, WS_POPUP, WS_EX_TOOLWINDOW);
	}
	if( m_vecWhoSendMsg.size() == 0 )
		return ; 


	std::vector<FriendListItemInfo>::iterator itBegin = m_vecWhoSendMsg.begin() ; 
	while( itBegin != m_vecWhoSendMsg.end() )
	{
		FriendListItemInfo friendInfo = *itBegin ;
		std::vector<RECEIVED_MSG>* pVecRecord = friendInfo.pMsg ;

		if( friendInfo.id.IsEmpty())
			return ; 
		RECEIVED_MSG msgItem ;
		char* msg ; 
		int nRecordCount = pVecRecord->size() ; 
		if( nRecordCount)
		{
			msgItem = pVecRecord->at(nRecordCount-1) ;
			CString strLastWord1 = _T(" ") ; 
			strLastWord1.Format("%s",msgItem.msg) ;
			// 发送的文字消息 
			CString msg = ConvertMsgToChiefMsg(msgItem.format,strLastWord1) ; 
			CDuiString strLastWord = _T(" ") ;
			strLastWord.Format(_T("%s") ,msg) ; 
			friendInfo.Lastword = strLastWord ; 
		}
		m_NotifyDlg.AddFriend(friendInfo) ; 
		itBegin++ ; 
	}
	m_NotifyDlg.InsertFrindToList() ;
	m_NotifyDlg.UpdateUserName(myself_info_.nick_name) ;   // 显示分析师自己的名字 

	int iSize = m_vecWhoSendMsg.size() ; 

	::CPoint pt ,ptScreen; 
	GetCursorPos(&pt) ; 
	::ClientToScreen(GetDesktopWindow() ,&pt) ;
	RECT rt;  
	SystemParametersInfo(SPI_GETWORKAREA,   0,   &rt,   0);    // 获得工作区大小 
	// 消息提示框的坐上点坐标 
	// 消息提示框大小固定 258 * ( 108 + (iSize - 1) * 50 ) 
	::CPoint startPoint ; 
	startPoint.x = pt.x - 258 / 2 - 20; 
	startPoint.y = rt.bottom-rt.top  - ( 108 + (iSize - 1) * 50 );  
	m_rtMsgDlg = CRect(startPoint.x ,startPoint.y ,startPoint.x + 258 ,startPoint.y + ( 108 + (iSize - 1) * 50 )) ; 

	::SetWindowPos((HWND)m_NotifyDlg ,HWND_TOPMOST,startPoint.x,startPoint.y,258,108 +( iSize - 1 ) * 50 ,SWP_SHOWWINDOW) ; 
	::ShowWindow((HWND)m_NotifyDlg,TRUE) ; 
}


///////////////////////////////////////////////////////////////////////
//向m_vecFriend列表中删除一个好友 
///////////////////////////////////////////////////////////////////////
void MainFrame::DeleteFriend(CDuiString strName) 
{
	std::vector<FriendListItemInfo>::iterator itBegin = m_vecWhoSendMsg.begin() ; 
	while( itBegin != m_vecWhoSendMsg.end())
	{
		if( itBegin->numid == strName )
		{
			itBegin = m_vecWhoSendMsg.erase(itBegin) ; 
		}
		else
			itBegin++ ; 
	}
}
void MainFrame::OnCloseNotifyDialog()
{
	if (::IsWindow((HWND)m_NotifyDlg))
	{
		::ShowWindow((HWND)m_NotifyDlg,FALSE) ; 
		KillTimer(m_hWnd ,T_TRAY_HOVER_TIMER_ID) ; 
		m_bIsMsgDlgShow = FALSE ; 
	}
}
void MainFrame::CloseWhoSendMsg()
{
	if( m_vecWhoSendMsg.size() )
		m_vecWhoSendMsg.clear() ; 
}
//HandleMessage中处理WINDOWS消息
LRESULT MainFrame::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LRESULT lRes = 0 ; 
	BOOL bHandle = TRUE ; 
	
	if (uMsg == WM_RBUTTONDOWN)
	{
		CControlUI * pControl = (CControlUI *)lParam;
		lRes = __super::HandleMessage(uMsg, wParam, lParam);
		OnRButtonDown(uMsg, wParam, lParam);
		return lRes;
	}
	else if (uMsg == WM_NOTIFYICON)
	{
		OnNotifyIcon(wParam, lParam);
		return 1;
	}
	else if( uMsg == WM_KEYUP)
	{
		OnKeyUp(uMsg ,wParam,lParam,bHandle) ;
		return 1 ; 
	}
	//else if( uMsg == WM_SETFOCUS)
	//{
	//	OnSetFocus(uMsg,wParam,lParam,bHandle) ; 
	//}
	return __super::HandleMessage(uMsg, wParam, lParam);
}

LRESULT MainFrame::OnSetFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if( m_pSearch_edit ) 
	{
		m_pSearch_edit->SetFocus() ;
		return 1 ; 
	}
	if( m_pSearch_edit_friend)
	{
		m_pSearch_edit_friend->SetFocus() ;
		return 1 ; 
	}
	return 0 ; 
}
///////////////////////////////////////////////////////////////////////////////////
//wuchao add at 2016-03-28 
//搜索框响应Enter按键 
///////////////////////////////////////////////////////////////////////////////////
LRESULT MainFrame::OnEnterUp()
{
	//CRichEditUI* search_edit = static_cast<CRichEditUI*>(m_PaintManager.FindControl(kSearchEditControlName)); 
	CDuiString strClientName = _T(" ") ; 
	if( /*m_pSearch_edit || */m_pSearch_edit_friend )
	{
		//if( m_pSearch_edit )
		//	strClientName = m_pSearch_edit->GetText() ; 
		if( m_pSearch_edit_friend)
			strClientName = m_pSearch_edit_friend->GetText() ; 

		if( strClientName.IsEmpty())
		{	
			::MessageBox(NULL,_T("请输入您要查找的好友"),_T("警告") ,MB_OK) ;
			return 0;
		}
		FriendList_ITER itBegin = friends_.begin() ; 
		FriendListItemInfo findFriendInfo ; 
		memset(&findFriendInfo ,0 ,sizeof(FriendListItemInfo) ) ; 
		findFriendInfo.numid.Format(_T("%s") ,strClientName) ; 

		FriendList_ITER itFind = find( friends_.begin() ,friends_.end() ,findFriendInfo) ;
		
		if( itFind != friends_.end())
		{
			// 找到了就打开对话框 
			//OpenChatDiolog(itFind->id, itFind ) ;
			// 如果节点已经在列表中显示了 就不要再添加了 
			CDuiString strName = _T("") ;
			strName.Format(_T("TGUSER$%d$%s") ,g_nRoomID_im2_0,itFind->numid) ; // TGUSER$1149$591
			Node *pIsExistNode = m_pFindFriendList->GetNodeItem(strName) ; 
			if( !pIsExistNode)
			{
				// 先清空历史搜索记录 
				m_pFindFriendList->RemoveAll() ;
				// 再添加节点 
				m_pFindFriendList->AddNode(*itFind ,NULL) ;
			}
			return 1 ; 	
		}
		if( itFind == friends_.end())
		{
			// 没有找到 
			CString strFind  = _T(" ") ;
			strFind.Format(_T("没有找到客户（%s）的任何信息！"),strClientName.GetData()) ; 
			::MessageBox(NULL,strFind,_T("提示") ,MB_OK) ; 
			ShowWindow(TRUE,TRUE);
			return 0; 
		}
		return 1 ; 
	}
}
LRESULT MainFrame::OnKeyUp(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	switch(wParam)
	{
	case VK_RETURN:
		{
			OnEnterUp() ; 
			break; 
		}
	}
	return 1 ; 
}
//HandleCustomMessage中处理自定义消息
LRESULT MainFrame::HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	bHandled = TRUE;
	switch (uMsg)
	{
	case UM_UI_MESSAGELOOP:
		OnUIMessageLoop(wParam, lParam);
		break;
	case UM_USER_LOGIN:
		OnUserLoginIN(wParam, lParam);
		break;
	case UM_USER_OFFLINE:
		OnUserOffLine(wParam, lParam);
		break;
	case UM_USER_ONLINE:
		OnUserOnLine(wParam, lParam);
		break;
	case UM_USER_CLOSE:
		OnUserColse(wParam, lParam);
		break;
	case UM_USER_ADDGROUP:
		OnUserADDGroup(wParam, lParam);
		break;
	case UM_USER_SETSELFINFO:
		OnSetSelfInfo(wParam, lParam);
		break;
	case UM_USER_ADDFRIEND:
		OnAddFriendInfo(wParam, lParam);
		break;
	case UM_USER_ADDPUBLIC:
		OnUserADDPublic(wParam, lParam);
		break;
	case UM_USER_ADDTGROOM:
		OnUserADDTgRoom(wParam, lParam);
		break;
	case UM_USER_ADDTGROOM2: 
		OnUserADDTgRoom2(wParam, lParam);
		break;
	case UM_USER_ADDROOMCLIENT:
		OnADDRoomClient(wParam, lParam);
		break;
	case UM_USER_ADDROOMANALYS:
		OnADDRoomAnalys(wParam, lParam);
		break;
	case UM_USER_STATECHANGE:
		OnUserStateChange(wParam, lParam);
		break;
	case UM_USER_OPENWND:
		OnOpenWND(wParam, lParam);
		break;
	case UM_SOUND_PLAY:
		OnSoundPlay(wParam, lParam);
		break;
	case UM_USER_ADDROOM:
		OnAddRoom(wParam, lParam);
		break;
	case UM_USER_LEAVEROOM:
		OnLeaveRoom(wParam, lParam);
		break;
	case WM_MENUCLICK:
		OnMenuClick(wParam, lParam);
		break;
	case UM_USER_GETUSERPIC:
		OnGetUserPic(wParam, lParam);
		break;
	case UM_USER_ADDSELFGROUP:
		OnCreatSelfGroup(wParam, lParam);
		break;
	case UM_USER_GETSELFPIC:
		OnGetSelfPic(wParam, lParam);
		break;
	case UM_USER_OPENRIGHTS:
		OnOpenRight(wParam, lParam);
		break;
	case UM_USER_ADDHISTORYCLENT:
		OnLoadRecentClientNode(wParam, lParam) ; 
		break ; 
	case WM_IM_USERID:
		OnOpenChatDialog( wParam, lParam) ; 
		break ;
	case WM_USER_FINDFRIEND:
		OnFindFriends(wParam,lParam) ;
		break;
	case UM_USER_ADDNEWPUBLIC:
		OnAddNewPublic(wParam,lParam);
		break;
	case UM_USER_ADDNEWLASTUSER:
		OnAddNewLastUser(wParam,lParam);
		break;
	default:
		bHandled = FALSE;
		break;
	}
	return 0;
}

UILIB_RESOURCETYPE MainFrame::GetResourceType() const
{
	return UILIB_ZIPRESOURCE;
}

LPCTSTR MainFrame::GetResourceID() const
{
	return MAKEINTRESOURCE(IDR_ZIPRES);
}

//非List控件右键响应
LRESULT MainFrame::OnRButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	POINT pt = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };
	CControlUI* pControl = static_cast<CControlUI*>(m_PaintManager.FindControl(pt));
	RECT rcControl = pControl->GetPos();
	RECT rcTabControl = m_pTabControl->GetPos();

	// 如果是好友列表中的空白区域，那么会往下执行； 如果是好友列表中的节点被选中，不理会这个消息，会有CListUI处理
#if 0
	if (EqualRECT(rcControl, rcTabControl))
	{
		CRect m_rcTmp(rcTabControl.left, rcTabControl.top, rcTabControl.right, rcTabControl.bottom);
		if(m_rcTmp.PtInRect(pt)) 
		{ 
			if (m_pTabControl->GetCurSel() == 0)
			{
				CMenuWnd* pMenu = new CMenuWnd();
				::ClientToScreen(m_hWnd, &pt);
				pMenu->Init(NULL, _T("menu_FriendPanel.xml"), pt, &m_PaintManager);

				if (g_userconfig.m_nNickType == eNICK_nameandid)
					pMenu->SetIconAndSize(_T("Menu_XS_NameAndID"), _T("right.png"), 9, 9);
				else if (g_userconfig.m_nNickType == eNICK_id)
					pMenu->SetIconAndSize(_T("Menu_XS_Id"), _T("right.png"), 9, 9);
				else if (g_userconfig.m_nNickType == eNICK_name)
					pMenu->SetIconAndSize(_T("Menu_XS_Name"), _T("right.png"), 9, 9);
			}
			else if (m_pTabControl->GetCurSel() == 1)
			{
				CMenuWnd* pMenu = new CMenuWnd();
				::ClientToScreen(m_hWnd, &pt);
				pMenu->Init(NULL, _T("menu_GroupPanel.xml"), pt, &m_PaintManager);
			}
			else if (m_pTabControl->GetCurSel() == 2)
			{
				//暂时不需要处理
			}
		}
	}
#endif

	return 0;
}

UINT DownAnalysPicFromWeb(PVOID pData) 
{
	DWORD t1 = GetTickCount() ; 
	USER_LOG("获取分析师头像开始时间:%d" ,t1) ; 
	MainFrame* pFrame = (MainFrame*)pData ;
	int nTgRoomNum = m_mapAllClientRoom.size();
	ACRI_ITER iterator;

	for (iterator = m_mapAllClientRoom.begin(); iterator != m_mapAllClientRoom.end(); iterator++)
	{
		int nAllNums = iterator->second.m_nAnalysNum;
		int nRoomID =  iterator->second.m_nRoomID;
		std::vector<ROOMANALYS_INFO>::iterator iter_info;
		for (iter_info = iterator->second.pVtAnalys->begin(); iter_info != iterator->second.pVtAnalys->end(); iter_info++)
		{
			int nAnalysID = iter_info->m_nAnalysID;
			CString strImgURL = iter_info->m_szImgURL;

			CString strDown = "";		// 下载的头像
			CString strHead = "";		// 在线的正常头像
			CString strHeadName = "";	// 在线的正常头像名称
			CString strHead2 = "";		// 离线时的灰化头像
			CString strHeadName2 = "";	// 离线时的灰化头像名称

			strDown.Format("%s\\%d.jpg", g_config.szUserHead, iter_info->m_nAnalysID);
			strHead.Format("%s\\%d_n.jpg", g_config.szUserHead, iter_info->m_nAnalysID);
			strHeadName.Format("%d_n.jpg", iter_info->m_nAnalysID);
			strHead2.Format("%s\\%d_g.jpg", g_config.szUserHead, iter_info->m_nAnalysID);
			strHeadName2.Format("%d_g.jpg", iter_info->m_nAnalysID);

			if(GetFileAttributes(strDown) == -1)
			{
				// 先吧原始头像下载在strDown my.jpg中
				BOOL bIsImgDownLoad = pFrame->DownLoadPic(strImgURL, strDown);
				// 将头像转换成62*62像素
				if (bIsImgDownLoad)
				{
					bool bHandImg = HandleHeadImage2(strDown, strHeadName, strHeadName2);
					if (bHandImg)
					{
						// 在线头像设置 
						if( iter_info->m_bIsOnLine )
						{
							pFrame->SetAnalysNodeHead(eUSER_ONLINE, nRoomID, nAnalysID, strHead, strHead2);
						}
						else
						{
							// 离线头像 
							pFrame->SetAnalysNodeHead(eUSER_OFFLINE, nRoomID, nAnalysID, strHead, strHead2);
						}
					}
				}
			}
			else
			{
				// 将头像转换成62*62像素
				// bool bHandImg = HandleHeadImage2(strDown, strHeadName, strHeadName2); 本地已经有了，不需要再去处理一次 
				// 在线头像设置 
				if( iter_info->m_bIsOnLine )
				{
					pFrame->SetAnalysNodeHead(eUSER_ONLINE, nRoomID, nAnalysID, strHead, strHead2);
				}
				else
				{
					// 离线头像 
					pFrame->SetAnalysNodeHead(eUSER_OFFLINE, nRoomID, nAnalysID, strHead, strHead2);
				}
			}
		}
	}


	DWORD t2 = GetTickCount() ; 
	USER_LOG("获取分析师头像结束时间:%d" ,t2) ;

	return 1 ; 
}
UINT DownUserPicFromWeb(PVOID pData ) 
{
	MainFrame* pFrame = (MainFrame*)pData ; 
	int iSize = m_vtRoomClient.size(); 
	for (int idex=0; idex<iSize; idex++)
	{
		CString strDown = "";		// 下载的头像
		CString strHead = "";		// 在线的正常头像
		CString strHead2 = "";		// 离线时的灰化头像
		CString strThisID = "" ; 

		CString strHeadUrl = pFrame->GetUserPicURL(m_vtRoomClient[idex].m_szUPID);
		if (!strHeadUrl.IsEmpty())
		{
			COPYSTRARRAY(m_vtRoomClient[idex].m_szImgURL, strHeadUrl);
			strThisID = m_vtRoomClient[idex].m_szUPID;

			strDown.Format("%s\\%s.jpg", g_config.szUserHead, strThisID);
			strHead.Format("%s\\%s_n.jpg", g_config.szUserHead, strThisID);
			strHead2.Format("%s\\%s_g.jpg", g_config.szUserHead, strThisID);

			// 第一次加载IM时要从Http请求中down图片 ，后面就只需从本地加载图像即可 
			if(GetFileAttributes(strHead) == -1)
			{
				TRACE("%s \n", strThisID);
				// 第二次登录  如果已经下载过了该用户的头像，那么就不再下载头像 
				if( !m_vtRoomClient[idex].m_bIsImgDownLoad )
				{
					m_vtRoomClient[idex].m_bIsImgDownLoad = pFrame->DownLoadPic(strHeadUrl, strDown);
				}
				else
					continue ;
				// 将头像转换成62*62像素
				if (m_vtRoomClient[idex].m_bIsImgDownLoad)
				{
					bool bHandImg = HandleHeadImage3(strDown,strHead, strHead2); 

					// 吴超 2016-03-02 用户的头像设置 
					COPYSTRARRAY(m_vtRoomClient[idex].m_szImgName , strDown) ; 
					COPYSTRARRAY(m_vtRoomClient[idex].m_szLNomalImgName , strHead) ;
					COPYSTRARRAY(m_vtRoomClient[idex].m_szLGrayImgName , strHead2) ;
				}
			}
			else // 从本地加载头像 
			{
				COPYSTRARRAY(m_vtRoomClient[idex].m_szImgName , strDown) ; 
				COPYSTRARRAY(m_vtRoomClient[idex].m_szLNomalImgName , strHead) ;
				COPYSTRARRAY(m_vtRoomClient[idex].m_szLGrayImgName , strHead2) ;
			}

			// 如果用户设置了头像  才去给用户设置头像 
			if( strHead.IsEmpty())
				strHead = _T("public.png") ; 
			if( strHead2.IsEmpty())
			{
				strHead2 =  _T("public_g.png"); 
			}
			if( m_vtRoomClient[idex].m_bIsOnLine)
			{
				// 在线  
				pFrame->SetClientNodeHead(eUSER_ONLINE ,m_vtRoomClient[idex].m_szUPID ,strHead,strHead2) ;
			}
			else
			{
				// 离线 
				pFrame->SetClientNodeHead(eUSER_OFFLINE ,m_vtRoomClient[idex].m_szUPID ,strHead,strHead2) ;
			}

		}
		//if( strHead.IsEmpty())
		//	strHead = _T("public.png") ; 
		//if( strHead2.IsEmpty())
		//{
		//	strHead2 =  _T("public_g.png"); 
		//}
		//if( m_vtRoomClient[idex].m_bIsOnLine)
		//{
		//	// 在线  
		//	pFrame->SetClientNodeHead(eUSER_ONLINE ,m_vtRoomClient[idex].m_szUPID ,strHead,strHead2) ;
		//}
		//else
		//{
		//	// 离线 
		//	pFrame->SetClientNodeHead(eUSER_OFFLINE ,m_vtRoomClient[idex].m_szUPID ,strHead,strHead2) ;
		//}
	} 
	return 1 ; 
}