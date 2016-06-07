#include "stdafx.h"
#include "../Func.h"
#include "../MainPort.h"
#include "GeneralDialog.h"
#include "RichEditUtil.h"

const TCHAR* const kTitleControlName		= _T("general_title");
const TCHAR* const kCloseControlName		= _T("general_closebtn");
const TCHAR* const kBackgroundControlName	= _T("general_bg");
const TCHAR* const kOKButtonControlName		= _T("general_ok");
const TCHAR* const kCloseButtonControlName	= _T("general_close");

CGeneralDialog::CGeneralDialog(const CDuiString& bgimage, DWORD bkcolor, ECREATEDIALOGTYPE m_CreateType, CDuiString strTitle_/* =_T("") */)
	: bgimage_(bgimage)
	, bkcolor_(bkcolor)
	, CreateType(m_CreateType)
	, m_strDialogName(strTitle_)
{
	m_pDialog_Errorstr = m_pDialog_name = NULL;
	m_pGroupName = NULL;
	m_pGroupType = m_pLoginType = NULL;
	m_pCloseBtn = m_pOKBtn = NULL;
	m_pPannel_GreateGroup = m_pPannel_LoginIN = NULL;
	m_pLoginName = m_pLoginPSW = NULL;
	m_pGroupNote = NULL;
}

CGeneralDialog::~CGeneralDialog()
{
	return;
}

LPCTSTR CGeneralDialog::GetWindowClassName() const
{
	return _T("UPGuiFoundation");
}

CControlUI* CGeneralDialog::CreateControl(LPCTSTR pstrClass)
{
	return NULL;
}

void CGeneralDialog::InitWindow()
{
	//////////////////////////////////////////////////////////////////////////
	// 共用
	m_pDialog_name =  static_cast<CTextUI*>(m_PaintManager.FindControl(kTitleControlName));
	m_pDialog_Errorstr =  static_cast<CTextUI*>(m_PaintManager.FindControl(_T("general_errorstr")));
	m_pOKBtn = static_cast<CButtonUI*>(m_PaintManager.FindControl(kOKButtonControlName));
	m_pCloseBtn = static_cast<CButtonUI*>(m_PaintManager.FindControl(kCloseButtonControlName));

	// 如果传进来名称不为空
	if (!m_strDialogName.IsEmpty() && m_pDialog_name)
		m_pDialog_name->SetText(m_strDialogName.GetData());

	// creategroup
	if (CreateType == eCreate_Group)
	{
		m_pPannel_GreateGroup = static_cast<CContainerUI*>(m_PaintManager.FindControl(_T("general_creategroup")));
		m_pGroupType = static_cast<CComboUI*>(m_PaintManager.FindControl(_T("general_creategroup_type")));
		m_pGroupName = static_cast<CEditUI*>(m_PaintManager.FindControl(_T("general_creategroup_name")));
		m_pGroupNote = static_cast<CRichEditUI*>(m_PaintManager.FindControl(_T("general_creategroup_note")));
	}
	// login
	else if (CreateType == eCreate_LoginIn)
	{
		m_pPannel_LoginIN = static_cast<CContainerUI*>(m_PaintManager.FindControl(_T("general_login")));
		m_pLoginName = static_cast<CEditUI*>(m_PaintManager.FindControl(_T("general_login_upid")));
		m_pLoginPSW = static_cast<CEditUI*>(m_PaintManager.FindControl(_T("general_login_password")));
		m_pLoginType = static_cast<CComboUI*>(m_PaintManager.FindControl(_T("general_login_type")));
		if (m_pLoginName)
			m_pLoginName->SetFocus();
		if (m_pOKBtn)
		{
			m_pOKBtn->SetText("登录");
			m_pOKBtn->SetToolTip("登录");
		}
		m_pLoginName->SetText("1142");
		m_pLoginPSW->SetText("888888");
	}
}

void CGeneralDialog::OnFinalMessage(HWND hWnd)
{
	RemoveObserver();
	WindowImplBase::OnFinalMessage(hWnd);	
	delete this;
}

BOOL CGeneralDialog::Receive(SkinChangedParam param)
{
	bgimage_ = param.bgimage;
	bkcolor_ = param.bkcolor;
	CControlUI* background = m_PaintManager.FindControl(kBackgroundControlName);
	if (background != NULL)
	{
		if (!param.bgimage.IsEmpty())
		{
			TCHAR szBuf[MAX_PATH] = {0};
			_stprintf_s(szBuf, MAX_PATH - 1, _T("file='%s' corner='600,200,1,1'"), param.bgimage);
			background->SetBkImage(szBuf);
		}
		else
			background->SetBkImage(_T(""));

		background->SetBkColor(param.bkcolor);
	}

	return TRUE;
}

CDuiString CGeneralDialog::GetSkinFile()
{
	if (CreateType == eCreate_Group)
	{
		return _T("dlg_General.xml");
	}
	else if (CreateType == eCreate_LoginIn)
	{
		return _T("dlg_General_Login.xml");
	}
	else if (CreateType == eCreate_TG1)
	{
		return _T("dlg_TG1.xml");
	}
	else if (CreateType == eCreate_TG2)
	{
		return _T("dlg_TG2.xml");
	}

	return _T("dlg_General.xml");
}

CDuiString CGeneralDialog::GetSkinFolder()
{
	return CDuiString(CPaintManagerUI::GetInstancePath()) + _T("skin\\");
}

LRESULT CGeneralDialog::OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	bHandled = FALSE;
	return 0;
}

LRESULT CGeneralDialog::OnSetFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	bHandled = FALSE;
	return 0;
}

LRESULT CGeneralDialog::OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	BOOL bZoomed = ::IsZoomed(m_hWnd);
	LRESULT lRes = CWindowWnd::HandleMessage(uMsg, wParam, lParam);
	return 0;
}

LRESULT CGeneralDialog::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	// 屏蔽标题栏双击消息
	if(WM_NCLBUTTONDBLCLK != uMsg)
		return __super::HandleMessage(uMsg, wParam, lParam);
	return 0;
}

LRESULT CGeneralDialog::HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	bHandled = FALSE;
	return 0;
}

LRESULT CGeneralDialog::ResponseDefaultKeyEvent(WPARAM wParam)
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

void CGeneralDialog::OnTimer(TNotifyUI& msg)
{
}

void CGeneralDialog::OnExit()
{
	Close();
}

void CGeneralDialog::UISetInitSize()
{
	CRect rectInit(0, 0, 0, 0);
	if(g_hVessel)	
		::GetClientRect(g_hVessel, &rectInit);

	CRect rectInit2(0, 0, 0, 0);
	if (CreateType == eCreate_TG1)
	{
		rectInit2.left = rectInit.left;
		rectInit2.right = rectInit.right;
		rectInit2.bottom = rectInit.bottom;
		rectInit2.top = rectInit.top+78;

		if (m_PaintManager.GetPaintWindow() != NULL)
		{
			m_PaintManager.GetRoot()->SetPos(rectInit);
			::MoveWindow (m_hWnd, rectInit2.left, rectInit2.top, rectInit2.right-rectInit2.left, rectInit2.bottom-rectInit2.top, TRUE);
		}
	}
	else if (CreateType == eCreate_TG2)
	{
		rectInit2.left = rectInit.left;
		rectInit2.right = rectInit.right;
		rectInit2.bottom = rectInit.bottom-132;
		rectInit2.top = rectInit.top;

		if (m_PaintManager.GetPaintWindow() != NULL)
		{
			m_PaintManager.GetRoot()->SetPos(rectInit2);
			::MoveWindow (m_hWnd, rectInit2.left, rectInit2.top, rectInit2.right-rectInit2.left, rectInit2.bottom-rectInit2.top, TRUE);
		}
	}
}

void CGeneralDialog::OnPrepare(TNotifyUI& msg)
{
	TCHAR szBuf[MAX_PATH] = {0};

	if (CreateType == eCreate_TG1 || CreateType == eCreate_TG2)
		return;
	
	CControlUI* background = m_PaintManager.FindControl(kBackgroundControlName);
	if (background != NULL)
	{
		_stprintf_s(szBuf, MAX_PATH - 1, _T("file='%s' corner='600,200,1,1'"), bgimage_);
		background->SetBkImage(szBuf);
		background->SetBkColor(bkcolor_);
	}
}

void CGeneralDialog::Notify(TNotifyUI& msg)
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
			g_pFrame->PostMessage(UM_USER_CLOSE, 0, 0);
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

void CGeneralDialog::OnCloseBtnClick(TNotifyUI& msg)
{
	if (CreateType == eCreate_Group)
	{
		OnExit();
	}
	else if (CreateType == eCreate_LoginIn)
	{
		OnExit();
		g_pFrame->PostMessage(UM_USER_CLOSE, 0, 0);
	}
	return;
}

void CGeneralDialog::OnOKBtnClick(TNotifyUI& msg)
{
	//检测参数
	if (CreateType == eCreate_Group)
	{
		CDuiString strType = (TCHAR *)m_pGroupType->GetText().GetData();
		short nGroupType = 0;
		if (strcmp(strType.GetData(),_T("内部群组")) == 0 )
			nGroupType = eCR_Group;
		else nGroupType = eCR_Group_Client;

		CDuiString strText = m_pGroupName->GetText();
		if (strText.GetLength() <= 0)
		{
			m_pDialog_Errorstr->SetText("错误信息：群组名称不能为空");
			return;
		}
		else
		{
			if (strText.GetLength() > 30)
			{
				m_pDialog_Errorstr->SetText("错误信息：群组名称太长");
				return;
			}
			g_MyClient.SendCreateGroupReq((LPCTSTR)strText.GetData(), nGroupType);
		}
		OnExit();
	}

	else if (CreateType == eCreate_LoginIn)
	{
		CDuiString strText_name = m_pLoginName->GetText();
		if (strText_name.IsEmpty())
		{
			m_pDialog_Errorstr->SetText("错误信息：请您输入用户名");
			return;
		}
		CDuiString strText_psw = m_pLoginPSW->GetText();
		if (strText_psw.IsEmpty())
		{
			m_pDialog_Errorstr->SetText("错误信息：请您输入密码");
			return;
		}

		short nCheckType = 0;
		if (m_pLoginType)
		{
			CDuiString strType = (TCHAR *)m_pLoginType->GetText().GetData();
			// 密码校验类型(登录类型)
			if (strcmp(strType.GetData(),_T("管理登录")) == 0 )
				nCheckType = eLogin_Emp;
			else if (strcmp(strType.GetData(),_T("客户登录")) == 0 )
				nCheckType = eLogin_Client;
			else if (strcmp(strType.GetData(),_T("分析师登录")) == 0 )
				nCheckType = eLogin_Analyst;
		}
		else
		{
			if (VER_UPIM_NUMS == 2 || VER_UPIM_NUMS == 3)
			{
				nCheckType = eLogin_Analyst;
			}
		}
		
		// 连接服务器
		InitConnect();
		if (nCheckType == eLogin_Emp)
		{
			char szPassWordMD5[33] = {0};
			MD5StringTo32((char *)strText_psw.GetData(), szPassWordMD5);
			g_MyClient.SetClientInfo(eUser_Broker);
			g_MyClient.SendCheckPwdReq(strText_name.GetData(), szPassWordMD5, nCheckType);
		}
		else if (nCheckType == eLogin_Client)
		{
			g_MyClient.SetClientInfo(eUser_Client);
			g_MyClient.SendCheckPwdReq(strText_name.GetData(), STR_NOCHECKPWD, nCheckType);
		}
		else if (nCheckType == eLogin_Analyst)
		{
			char szPassWordMD5[33] = {0};
			MD5StringTo32((char *)strText_psw.GetData(), szPassWordMD5);
			if (VER_UPIM_NUMS == 2)
				g_MyClient.SetClientInfo(eUser_UPAnalyst);
			if (VER_UPIM_NUMS == 3)
				g_MyClient.SetClientInfo(eUser_UPAnalyst2);

			g_MyClient.SendCheckPwdReq(strText_name.GetData(), szPassWordMD5, nCheckType);
		}
	}
	return;
}