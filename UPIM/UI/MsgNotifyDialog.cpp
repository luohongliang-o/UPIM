#include "stdafx.h"
#include "../Func.h"
#include "../MainPort.h"
#include "MsgNotifyDialog.h"
#include "RichEditUtil.h"
#include <algorithm> 
using namespace  std ;
const TCHAR* const kSetControlName		= _T("general_set");
const TCHAR* const kCloseControlName		= _T("general_closebtn");
const TCHAR* const kBackgroundControlName	= _T("general_bg");
const TCHAR* const kOKButtonControlName		= _T("general_ok");
const TCHAR* const kCloseButtonControlName	= _T("general_close");
const TCHAR* const KClientNameControlName   = _T("msgnotify_name") ;

const TCHAR* const KUserNameControlName     = _T("msgnotify_name") ; 
const TCHAR* const kFriendsListControlName  = _T("msgnotify_friends"); 
const TCHAR* const kIgnoreAllMsgControlName = _T("msgnotify_hlsy");    // 查看全部消息  
const TCHAR* const kOpenAllMsgControlName   =  _T("msgnotify_cksy");   // 忽略所有消息 


CMsgNotifyDialog::CMsgNotifyDialog(ECREATEDIALOGTYPE m_CreateType)
	:CreateType(m_CreateType)
{
	m_pMinSysBtn = m_pMaxSysBtn = m_pRestoreSysBtn = m_pCloseSysBtn = NULL;
	m_pCloseBtn = m_pOKBtn = NULL;
	m_pSetBtn = NULL ; 
	m_pLabelClientName = NULL ; 
	m_pFriendList = NULL ;
}

CMsgNotifyDialog::~CMsgNotifyDialog()
{
	return;
}

LPCTSTR CMsgNotifyDialog::GetWindowClassName() const
{
	return _T("UPGuiFoundation");
}

CControlUI* CMsgNotifyDialog::CreateControl(LPCTSTR pstrClass)
{
	if (_tcsicmp(pstrClass, _T("MessageList")) == 0)
	{
		return new CFriendsUI(m_PaintManager);
	}

	return NULL;
}

void CMsgNotifyDialog::InitWindow()
{
	//////////////////////////////////////////////////////////////////////////
	// 共用
	//m_pOKBtn = static_cast<CButtonUI*>(m_PaintManager.FindControl(kOKButtonControlName));
	m_pSetBtn = static_cast<CButtonUI*>(m_PaintManager.FindControl(kSetControlName));
	
	// title 自己的名字 
	m_pLabelUsername = static_cast<CLabelUI*>(m_PaintManager.FindControl(KClientNameControlName)) ; 
	m_pFriendList      = static_cast<CFriendsUI*>(m_PaintManager.FindControl(_T("msgnotify_friends")));
	m_pOpenAllMsgBtn   = static_cast<CButtonUI*>(m_PaintManager.FindControl(kOpenAllMsgControlName)) ;
	m_pIgnoreAllMsgBtn = static_cast<CButtonUI*>(m_PaintManager.FindControl(kIgnoreAllMsgControlName)) ;

}

void CMsgNotifyDialog::OnFinalMessage(HWND hWnd)
{
	WindowImplBase::OnFinalMessage(hWnd);	
	delete this;
}

CDuiString CMsgNotifyDialog::GetSkinFile()
{
	return _T("dlg_MsgNotify.xml");
}

CDuiString CMsgNotifyDialog::GetSkinFolder()
{
	return CDuiString(CPaintManagerUI::GetInstancePath()) + _T("skin\\");
}

LRESULT CMsgNotifyDialog::OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	bHandled = FALSE;
	return 0;
}

LRESULT CMsgNotifyDialog::OnSetFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	bHandled = FALSE;
	return 0;
}

LRESULT CMsgNotifyDialog::OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	BOOL bZoomed = ::IsZoomed(m_hWnd);
	LRESULT lRes = CWindowWnd::HandleMessage(uMsg, wParam, lParam);
	return 0;
}

LRESULT CMsgNotifyDialog::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	// 屏蔽标题栏双击消息
	if(WM_NCLBUTTONDBLCLK != uMsg)
		return __super::HandleMessage(uMsg, wParam, lParam);
	return 0;
}

LRESULT CMsgNotifyDialog::HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	bHandled = FALSE;
	switch(uMsg)
	{
	case WM_MOUSEMOVE:
		{
			g_pFrame->m_isEnterLeave= TRUE ; 
			g_pFrame->KillMyTimer() ; 
		}
		break ; 
	case WM_MOUSELEAVE:
		{
			g_pFrame->m_isEnterLeave = FALSE ; 
			g_pFrame->AddMyTimer() ;
		}
		break ; 
	}
	return 0;
}

LRESULT CMsgNotifyDialog::ResponseDefaultKeyEvent(WPARAM wParam)
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

void CMsgNotifyDialog::OnTimer(TNotifyUI& msg)
{
}

void CMsgNotifyDialog::OnExit()
{
	Close();
}

void CMsgNotifyDialog::OnPrepare(TNotifyUI& msg)
{
	TCHAR szBuf[MAX_PATH] = {0};

	if (CreateType == eCreate_TG1 || CreateType == eCreate_TG2)
		return;
}

void CMsgNotifyDialog::Notify(TNotifyUI& msg)
{
	if (_tcsicmp(msg.sType, _T("windowinit")) == 0)
	{
		OnPrepare(msg);
	}
	else if (_tcsicmp(msg.sType, _T("killfocus")) == 0)
	{

	}
	else if (_tcsicmp(msg.sType, _T("click")) == 0)
	{
		if(_tcsicmp(msg.pSender->GetName(), kOpenAllMsgControlName) == 0)
		{
			OnOpenAllMsgBtnClick(msg) ; 
		}
		else if(_tcsicmp(msg.pSender->GetName(), kIgnoreAllMsgControlName) == 0)
		{
			OnIgnoreAllMsgBtnClick(msg) ; 
		}
	}
    else if( _tcsicmp(msg.sType, _T("return")) == 0 ) 
    {
    }
	else if (_tcsicmp(msg.sType, _T("timer")) == 0)
	{
		return OnTimer(msg);
	}
	else if (_tcsicmp(msg.sType, _T("selectchanged")) == 0)
	{
	}
	else if (_tcsicmp(msg.sType, _T("itemselect")) == 0)
	{
	}
	else if (_tcsicmp(msg.sType, _T("itemclick")) == 0)
	{
		OnListItemClick(msg);
	}

}

void CMsgNotifyDialog::OnCloseBtnClick(TNotifyUI& msg)
{
	OnExit();
	return;
}


// 忽略全部消息    
void CMsgNotifyDialog::OnIgnoreAllMsgBtnClick(TNotifyUI& msg)
{
	// 1 停止好友列表中的头像闪动
	std::vector<FriendListItemInfo>::iterator itBegin = m_vecFriend.begin() ; 
	while( itBegin != m_vecFriend.end() )
	{
		CDuiString strName = itBegin->id ; 
		g_pFrame->StopFlashItem(strName ,ePanel_Single) ;  
		itBegin++ ; 
	}
	// 2 关闭消息提示框 
	this->ShowWindow(false) ;
	// 3 隐藏消息提示框  

	g_pFrame->StopNotifyiconFlash() ;
	itBegin = m_vecFriend.begin() ; 
	while( itBegin != m_vecFriend.end() )
	{
		CDuiString strNumID = itBegin->numid ;
		g_pFrame->DeleteFriend(strNumID) ; 
		itBegin++ ; 
	}
	m_vecFriend.clear() ; 

}

// 查看所有消息 
void CMsgNotifyDialog::OnOpenAllMsgBtnClick(TNotifyUI& msg)
{
	// 打开所有的好友聊天对话框  
	std::vector<FriendListItemInfo>::iterator itBegin = m_vecFriend.begin() ; 
	while( itBegin != m_vecFriend.end() )
	{
		CDuiString strName = itBegin->id ; 
		g_pFrame->OpenChatDiolog(strName ,itBegin) ;
		itBegin++;
	}
	// 关闭消息提示框 
	this->ShowWindow(false) ;
	// 通知停止闪动托盘  
	g_pFrame->StopNotifyiconFlash() ;

	itBegin = m_vecFriend.begin() ; 
	while( itBegin != m_vecFriend.end() )
	{
		CDuiString strNumID = itBegin->numid ;
		g_pFrame->DeleteFriend(strNumID) ; 
		itBegin++ ; 
	}
	m_vecFriend.clear() ; 

}

void CMsgNotifyDialog::OnOKBtnClick(TNotifyUI& msg)
{
	return;
}

void CMsgNotifyDialog::UpdateClientName( CDuiString strClientName ) 
{
	//if( m_pLabelClientName )
	//{
	//	m_pLabelClientName->SetText(strClientName) ; 
	//}
}

void CMsgNotifyDialog::UpdateUserName(CDuiString strUserName)
{
	if( m_pLabelUsername)
	{
		m_pLabelUsername->SetText(strUserName) ;
	}
}
///////////////////////////////////////////////////////////////////////
//向m_vecFriend列表中删除一个好友 
///////////////////////////////////////////////////////////////////////
void CMsgNotifyDialog::DeleteFriend(CDuiString strName) 
{
	std::vector<FriendListItemInfo>::iterator itBegin = m_vecFriend.begin() ; 
	while( itBegin != m_vecFriend.end())
	{
		if( itBegin->numid == strName )
		{
			itBegin = m_vecFriend.erase(itBegin) ; 
		}
		else
			itBegin++ ; 
	}

	std::vector<FriendListItemInfo>::iterator itBeginFriend = friends_.begin() ; 
	while( itBeginFriend != friends_.end())
	{
		if( itBeginFriend->numid == strName )
		{
			itBeginFriend = friends_.erase(itBeginFriend) ; 
		}
		else
			itBeginFriend++ ; 
	}
}
///////////////////////////////////////////////////////////////////////
//向m_vecFriend列表中添加一个好友 
///////////////////////////////////////////////////////////////////////
void CMsgNotifyDialog::AddFriend(FriendListItemInfo friendInfo)
{
	std::vector<FriendListItemInfo>::iterator itBegin  = m_vecFriend.begin(); 
	bool isAdd = TRUE ; 
	while( itBegin != m_vecFriend.end())
	{
		if( friendInfo.numid == itBegin->numid)
		{
			itBegin->Lastword = friendInfo.Lastword ; 
			isAdd = FALSE ; 
			break ; 
		}
		itBegin++ ; 
	}
	if( isAdd)
		m_vecFriend.push_back(friendInfo) ; 
}

///////////////////////////////////////////////////////////////////////
// 向CMsgNotifyDialog 窗口中插入来消息的好友，不止一个好友 
///////////////////////////////////////////////////////////////////////
void CMsgNotifyDialog::InsertFrindToList()
{
	const int nItemHeight = 50 ; 
	if( !m_pFriendList )
		return ; 
	Node* retrunptr = NULL;
	FriendListItemInfo friendInfo ; 
	int iSize = m_vecFriend.size() ;

	std::vector<FriendListItemInfo>::iterator itBegin = m_vecFriend.begin() ; 
	m_pFriendList->RemoveAll() ; 

	while( itBegin != m_vecFriend.end())
	{
		friendInfo = *itBegin ; 
		retrunptr = m_pFriendList->AddNode( friendInfo ,NULL,nItemHeight) ;
		if(retrunptr)
		{
			m_pFriendList->SetChildVisible(retrunptr,true) ; 

			BOOL isInsertNode = TRUE ; 
			std::vector<FriendListItemInfo>::iterator itBeginFriend  = friends_.begin();
			while( itBeginFriend != friends_.end())
			{
				if( itBeginFriend->numid == friendInfo.numid)
				{
					itBeginFriend->Lastword = friendInfo.Lastword ; 
					isInsertNode = FALSE ; 
				}
				itBeginFriend++ ;
			}
			if(isInsertNode)
				friends_.push_back(friendInfo) ;
		}
		itBegin++ ; 
	}

#if 0 
	if( iSize)
	{
		// 去最后一个  插入 
		friendInfo = m_vecFriend.at( iSize - 1 ) ;
		retrunptr = m_pFriendList->AddNode( friendInfo ,NULL,nItemHeight) ;
		if(retrunptr)
		{
			m_pFriendList->SetChildVisible(retrunptr,true) ; 
			
			BOOL isInsertNode = TRUE ; 
			std::vector<FriendListItemInfo>::iterator itBegin  = friends_.begin();
			while( itBegin != friends_.end())
			{
				if( itBegin->numid == friendInfo.numid)
				{
					itBegin->Lastword = friendInfo.Lastword ; 
					isInsertNode = FALSE ; 
				}
				itBegin++ ;
			}
			if(isInsertNode)
				friends_.push_back(friendInfo) ;
		}
	}
#endif 
}
///////////////////////////////////////////////////////////////////////
//点击列表，弹出与该好友聊天 
///////////////////////////////////////////////////////////////////////
void CMsgNotifyDialog::OnListItemClick(TNotifyUI& msg)
{
	// 获取好友信息 
	if( _tcsicmp(msg.pSender->GetClass() ,_T("ListContainerElementUI") ) == 0  )
	{
		Node* pNode = (Node*)(msg.pSender->GetTag()) ; 
		CDuiString strName =  pNode->data().id ; 
		
		// 将Node节点从m_pFriendList列表中删除 
		Node* pSelectNode = m_pFriendList->SelectNodeByID(strName.GetData()); 
		if( !pSelectNode)
			return ; 
		CDuiString strNumID = pNode->data().numid ; 
// 		DeleteFriend(strNumID) ; 
// 		g_pFrame->DeleteFriend(strNumID) ; 
		//隐藏自己窗口 
		this->ShowWindow(false) ; 
		
		//m_vecFriend
		std::vector<FriendListItemInfo>::iterator itBegin = m_vecFriend.begin() ; 
		while( itBegin != m_vecFriend.end())
		{
			if( itBegin->numid == pNode->data().numid )
			{
				break; 
			}
			itBegin++ ; 
		}
		// 让主窗口打开对话框 
		g_pFrame->OpenChatDiolog(strName ,itBegin) ;
		m_pFriendList->RemoveNode(pSelectNode) ; 
		// 托盘图标停止闪动 
		if( m_vecFriend.size() == 0 )
			g_pFrame->StopNotifyiconFlash() ; 
		DeleteFriend(strNumID) ; 
		g_pFrame->DeleteFriend(strNumID) ; 

	}
 
}