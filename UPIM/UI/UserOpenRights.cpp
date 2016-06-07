#include "stdafx.h"
#include "../Func.h"
#include "../MainPort.h"
#include "UserOpenRights.h"
#include "RichEditUtil.h"

const TCHAR* const kCloseControlName	= _T("general_closebtn");
const TCHAR* const kOpenControlName		= _T("general_open");

CUserOpenRights::CUserOpenRights()
{
	m_pMinSysBtn = m_pMaxSysBtn = m_pRestoreSysBtn = m_pCloseSysBtn = NULL;
	m_pCloseBtn = m_pOpenBtn = NULL;
}

CUserOpenRights::~CUserOpenRights()
{
	return;
}

LPCTSTR CUserOpenRights::GetWindowClassName() const
{
	return _T("UPGuiFoundation");
}

CControlUI* CUserOpenRights::CreateControl(LPCTSTR pstrClass)
{
	return NULL;
}

void CUserOpenRights::InitWindow()
{
	//////////////////////////////////////////////////////////////////////////
	// 共用
	m_pCloseBtn = static_cast<CButtonUI*>(m_PaintManager.FindControl(kCloseControlName));
	m_pOpenBtn = static_cast<CButtonUI*>(m_PaintManager.FindControl(kOpenControlName));
}

void CUserOpenRights::OnFinalMessage(HWND hWnd)
{
	WindowImplBase::OnFinalMessage(hWnd);	
	delete this;
}

CDuiString CUserOpenRights::GetSkinFile()
{
	return _T("dlg_OpenRights.xml");
}

CDuiString CUserOpenRights::GetSkinFolder()
{
	return CDuiString(CPaintManagerUI::GetInstancePath()) + _T("skin\\");
}

LRESULT CUserOpenRights::OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	bHandled = FALSE;
	return 0;
}

LRESULT CUserOpenRights::OnSetFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	bHandled = FALSE;
	return 0;
}

LRESULT CUserOpenRights::OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	BOOL bZoomed = ::IsZoomed(m_hWnd);
	LRESULT lRes = CWindowWnd::HandleMessage(uMsg, wParam, lParam);
	return 0;
}

LRESULT CUserOpenRights::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	// 屏蔽标题栏双击消息
	if(WM_NCLBUTTONDBLCLK != uMsg)
		return __super::HandleMessage(uMsg, wParam, lParam);
	return 0;
}

LRESULT CUserOpenRights::HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	bHandled = FALSE;
	return 0;
}

LRESULT CUserOpenRights::ResponseDefaultKeyEvent(WPARAM wParam)
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

void CUserOpenRights::OnTimer(TNotifyUI& msg)
{
}

void CUserOpenRights::OnExit()
{
	Close();
}

void CUserOpenRights::OnPrepare(TNotifyUI& msg)
{
	TCHAR szBuf[MAX_PATH] = {0};
}

void CUserOpenRights::Notify(TNotifyUI& msg)
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
		else if (_tcsicmp(msg.pSender->GetName(), kOpenControlName) == 0)
        {
            OnOpenBtnClick(msg);
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

void CUserOpenRights::OnCloseBtnClick(TNotifyUI& msg)
{
	OnExit();
	return;
}

void CUserOpenRights::OnOpenBtnClick(TNotifyUI& msg)
{
	ShellExecute(NULL,"open","http://www.upchina.com",NULL,NULL,SW_SHOWNORMAL);
	OnExit();
	return;
}