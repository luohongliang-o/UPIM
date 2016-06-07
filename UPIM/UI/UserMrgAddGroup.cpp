#include "stdafx.h"
#include "../Func.h"
#include "../MainPort.h"
#include "UserMrgAddGroup.h"
#include "RichEditUtil.h"

const TCHAR* const kCloseControlName		= _T("general_closebtn");
const TCHAR* const kOKButtonControlName		= _T("usermrg_add_ok");
const TCHAR* const kCloseButtonControlName	= _T("usermrg_add_close");

CUserMrgAddGroup::CUserMrgAddGroup()
{
	m_pMinSysBtn = m_pMaxSysBtn = m_pRestoreSysBtn = m_pCloseSysBtn = NULL;
	m_pCloseBtn = m_pOKBtn = NULL;
	m_pGroupEdit = NULL;
}

CUserMrgAddGroup::~CUserMrgAddGroup()
{
	return;
}

LPCTSTR CUserMrgAddGroup::GetWindowClassName() const
{
	return _T("UPGuiFoundation");
}

CControlUI* CUserMrgAddGroup::CreateControl(LPCTSTR pstrClass)
{
	return NULL;
}

void CUserMrgAddGroup::InitWindow()
{
	//////////////////////////////////////////////////////////////////////////
	// 共用
	m_pOKBtn = static_cast<CButtonUI*>(m_PaintManager.FindControl(kOKButtonControlName));
	m_pCloseBtn = static_cast<CButtonUI*>(m_PaintManager.FindControl(kCloseButtonControlName));
	m_pGroupEdit = static_cast<CEditUI*>(m_PaintManager.FindControl(_T("usermrg_add_name")));
	
	if (m_pGroupEdit)
		m_pGroupEdit->SetFocus();
	g_strAddGroupName = _T("");
	
}

void CUserMrgAddGroup::OnFinalMessage(HWND hWnd)
{
	WindowImplBase::OnFinalMessage(hWnd);	
	delete this;
}

CDuiString CUserMrgAddGroup::GetSkinFile()
{
	return _T("dlg_UserMrgAddGroup.xml");
}

CDuiString CUserMrgAddGroup::GetSkinFolder()
{
	return CDuiString(CPaintManagerUI::GetInstancePath()) + _T("skin\\");
}

LRESULT CUserMrgAddGroup::OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	bHandled = FALSE;
	return 0;
}

LRESULT CUserMrgAddGroup::OnSetFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	bHandled = FALSE;
	return 0;
}

LRESULT CUserMrgAddGroup::OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	BOOL bZoomed = ::IsZoomed(m_hWnd);
	LRESULT lRes = CWindowWnd::HandleMessage(uMsg, wParam, lParam);
	return 0;
}

LRESULT CUserMrgAddGroup::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	// 屏蔽标题栏双击消息
	if(WM_NCLBUTTONDBLCLK != uMsg)
		return __super::HandleMessage(uMsg, wParam, lParam);
	return 0;
}

LRESULT CUserMrgAddGroup::HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	bHandled = FALSE;
	return 0;
}

LRESULT CUserMrgAddGroup::ResponseDefaultKeyEvent(WPARAM wParam)
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

void CUserMrgAddGroup::OnTimer(TNotifyUI& msg)
{
}

void CUserMrgAddGroup::OnExit()
{
	Close();
}

void CUserMrgAddGroup::OnPrepare(TNotifyUI& msg)
{
	TCHAR szBuf[MAX_PATH] = {0};
}

void CUserMrgAddGroup::Notify(TNotifyUI& msg)
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
		if (_tcsicmp(msg.pSender->GetName(), kCloseControlName) == 0)
		{
			OnExit();
		}
		else if (_tcsicmp(msg.pSender->GetName(), kCloseButtonControlName) == 0)
        {
            OnCloseBtnClick(msg);
		}
		else if (_tcsicmp(msg.pSender->GetName(), kOKButtonControlName) == 0)
		{
			OnOKBtnClick(msg);
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
}

void CUserMrgAddGroup::OnCloseBtnClick(TNotifyUI& msg)
{
	OnExit();
	return;
}

void CUserMrgAddGroup::OnOKBtnClick(TNotifyUI& msg)
{
	CDuiString strText_groupname = m_pGroupEdit->GetText();
	if (!strText_groupname.IsEmpty())
	{
		g_strAddGroupName = strText_groupname.GetData();
		g_strAddGroupName.Trim();
	}
	OnExit();
	return;
}