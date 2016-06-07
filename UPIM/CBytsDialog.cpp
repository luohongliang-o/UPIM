#include "stdafx.h"
#include "CBytsDialog.h"
#include <sstream>

using namespace std ; 

//CBytsDialog(LPCTSTR lpMyselfName ,LPCTSTR lpMyselfID ,LPCTSTR lpAnayName ,LPCTSTR lpAnayID);

CBytsDialog::CBytsDialog(/*CDuiString strMyselfName ,CDuiString strMyselfID ,CDuiString strAnayName ,CDuiString strAnayID*/LPCTSTR lpMyselfName ,LPCTSTR lpMyselfID ,LPCTSTR lpAnayName ,LPCTSTR lpAnayID)
{
	m_pMinSysBtn = m_pMaxSysBtn  = m_pCloseSysBtn = NULL;
	m_pSubmitBtn = NULL ; 

	m_pBCSMEdit = NULL ; 
	m_pTSBCSMEdit = NULL ; 
	m_pPhoneNumEdit = NULL ; 
	//m_pTSPhoneNumEdit = NULL ; 
	m_pBYCheckBox = NULL ; 
	m_pTSCheckBox = NULL ; 
	m_pFxsBehaviorList = NULL ; 
	m_pControlSwitch = NULL ; 
	m_pBCSMEdit = NULL ; 
	
	m_nSwitch = 0 ;  // 默认显示投诉分析师界面 

/*	m_strAnayName = strAnayName ; 
	m_strAnayID   = strAnayID ; 
	m_strMyselfName = strMyselfName ; 
	m_strMyselfID = strMyselfID ;*/ 
	
	m_lpAnayName = m_lpAnayID = m_lpMyselfName=m_lpMyselfID = NULL ; 

	//memcpy((void*)m_lpAnayName ,(void*)lpAnayName ,strlen(lpAnayName) ) ;
	//memcpy((void*)m_lpAnayID ,(void*)lpAnayID ,strlen(lpAnayID) ) ;
	//memcpy((void*)m_lpMyselfName ,(void*)lpMyselfName ,strlen(lpMyselfName) ) ;
	//memcpy((void*)m_lpMyselfID ,(void*)lpMyselfID ,strlen(lpMyselfID) ) ;

	m_lpAnayName = lpAnayName ; 
	m_lpAnayID   = lpAnayID ; 
	m_lpMyselfName = lpMyselfName ; 
	m_lpMyselfID   = lpMyselfID ; 

}

CBytsDialog::~CBytsDialog()
{
	return;
}


const TCHAR* const kCloseControlName	= _T("btn_closebtn");
const TCHAR* const kMinControlName      = _T("btn_minbtn") ; 
const TCHAR* const kMaxControlName      = _T("btn_maxbtn") ; 
const TCHAR* const kSubmitControlName      = _T("btn_submit") ;  


LPCTSTR CBytsDialog::GetWindowClassName() const
{
	return _T("UPGuiFoundation");
}

CControlUI* CBytsDialog::CreateControl(LPCTSTR pstrClass)
{
	return NULL;
}

void CBytsDialog::InitWindow()
{
	//////////////////////////////////////////////////////////////////////////
	// 共用
	m_pMinSysBtn = static_cast<CButtonUI*>(m_PaintManager.FindControl(kMinControlName));
	m_pMaxSysBtn = static_cast<CButtonUI*>(m_PaintManager.FindControl(kMaxControlName));
	m_pCloseSysBtn = static_cast<CButtonUI*>(m_PaintManager.FindControl(kCloseControlName));
	m_pSubmitBtn = static_cast<CButtonUI*>(m_PaintManager.FindControl(kSubmitControlName));


	m_pTSCheckBox = static_cast<CCheckBoxUI*>(m_PaintManager.FindControl(_T("checkbox_tousu")));
	m_pBYCheckBox = static_cast<CCheckBoxUI*>(m_PaintManager.FindControl(_T("checkbox_biaoyang")));

	m_pBCSMEdit = static_cast<CRichEditUI*>(m_PaintManager.FindControl(_T("buchongshuoming_edit"))); ; 
	m_pTSBCSMEdit = static_cast<CRichEditUI*>(m_PaintManager.FindControl(_T("buchongshuoming_edit"))); 
	m_pPhoneNumEdit = static_cast<CEditUI*>(m_PaintManager.FindControl(_T("phone_edit")));
	m_pControlSwitch = static_cast<CTabLayoutUI*>(m_PaintManager.FindControl(_T(" switch_face")));

	
	// 调用浏览器接口 
	m_pActiveX_Web = static_cast<CWebBrowserUI*>(m_PaintManager.FindControl(_T("tousubiaoyang_web"))) ; 
	if( m_pActiveX_Web ) 
	{
		IWebBrowser2* pWebBrowser = NULL;
		m_pActiveX_Web->GetControl(IID_IWebBrowser2, (void**)&pWebBrowser);
		m_pActiveX_Web->Navigate2("about:blank");

		if( pWebBrowser != NULL ) 
		{
			TCHAR str_path_name[1024] ={0} ;
			sprintf( str_path_name ,"upia.upchinafund.com\\Home\\im\\opinion.html?userId=%s&userName=%s&analystId=%s&analystName=" ,m_lpMyselfID,m_lpMyselfName,m_lpAnayID) ; 
			
			TCHAR str_analys_name[100] = {0} ; // 分析师的名字  
			sprintf(str_analys_name ,"%s" ,m_lpAnayName) ;
			string str = _EncodeURL(str_analys_name).c_str() ; 


			CString name = _T("") ; 
			name.Format("%s%s" ,str_path_name,str) ; 
			m_pActiveX_Web->Navigate2(name);
			//BSTR bsName = name.AllocSysString();
			//pWebBrowser->Navigate( bsName, NULL, NULL, NULL, NULL); 
			//pWebBrowser->Release();
			//SysFreeString(bsName);
		}
	}

}

void CBytsDialog::OnFinalMessage(HWND hWnd)
{
	WindowImplBase::OnFinalMessage(hWnd);	
	delete this;
}

CDuiString CBytsDialog::GetSkinFile()
{
	// return _T("dlg_biaoyangtousu.xml");
	return _T("dlg_byts_web.xml") ; 
}

CDuiString CBytsDialog::GetSkinFolder()
{
	return CDuiString(CPaintManagerUI::GetInstancePath()) + _T("skin\\");
}

LRESULT CBytsDialog::OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	bHandled = FALSE;
	return 0;
}

LRESULT CBytsDialog::OnSetFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	bHandled = FALSE;
	return 0;
}

LRESULT CBytsDialog::OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	BOOL bZoomed = ::IsZoomed(m_hWnd);
	LRESULT lRes = CWindowWnd::HandleMessage(uMsg, wParam, lParam);
	return 0;
}

LRESULT CBytsDialog::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	// 屏蔽标题栏双击消息
	if(WM_NCLBUTTONDBLCLK != uMsg)
		return __super::HandleMessage(uMsg, wParam, lParam);
	return 0;
}

LRESULT CBytsDialog::HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	bHandled = FALSE;
	return 0;
}

LRESULT CBytsDialog::ResponseDefaultKeyEvent(WPARAM wParam)
{
	//if (wParam == VK_RETURN)
	//{
	//	return FALSE;
	//}
	//else if (wParam == VK_ESCAPE)
	//{
	//	return TRUE;
	//}
	return FALSE;
}

void CBytsDialog::OnTimer(TNotifyUI& msg)
{
}

void CBytsDialog::OnExit()
{
	Close();
}

void CBytsDialog::OnPrepare(TNotifyUI& msg)
{
	TCHAR szBuf[MAX_PATH] = {0};
}

void CBytsDialog::Notify(TNotifyUI& msg)
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
		//else if(_tcsicmp(msg.pSender->GetName(), kSubmitControlName) == 0 )
		//{
		//	OnBtnSubmit(msg) ; 
		//}
		//else if( _tcsicmp(msg.pSender->GetName() ,kMinControlName) == 0  )
		//{
		//	OnBtn_Min(msg) ; 
		//}
		//else if( _tcsicmp(msg.pSender->GetName() ,kMaxControlName) == 0 )
		//{
		//	OnBtn_Max(msg) ; 
		//}
		//else if( msg.pSender == m_pTSCheckBox) 
		//{
		//	m_nSwitch = 0 ; 
		//	SwitchInterface(m_nSwitch) ;
		//}
		//else if( msg.pSender == m_pBYCheckBox )
		//{
		//	m_nSwitch = 1 ; 
		//	SwitchInterface(m_nSwitch) ; 
		//}
	}
	else if( _tcsicmp(msg.sType, _T("return")) == 0 ) 
	{
	}
	else if (_tcsicmp(msg.sType, _T("timer")) == 0)
	{
		//return OnTimer(msg);
	}
	else if (_tcsicmp(msg.sType, _T("selectchanged")) == 0)
	{
		//OnSelectCheckBox(msg) ; 
	}
}


BOOL CBytsDialog::CheckPhoneNum()
{
	if( m_pPhoneNumEdit)
	{	
		CString strPhoneNum = m_pPhoneNumEdit->GetText().GetData()  ; 
		if( strPhoneNum.GetLength() == 11 )
		{
			if( strPhoneNum == strPhoneNum.SpanIncluding(_T("0123456789")))
			{
				return TRUE ; 
			}
		}
	}
	return FALSE ; 
}
void CBytsDialog::SwitchInterface(int nSwitch)
{
	if( !m_pControlSwitch)
		return ; 
	if( nSwitch == 0 )
	{
		if( m_pBYCheckBox)
			m_pBYCheckBox->SetCheck(FALSE) ; // 这样保证不能同时选择两个 
		m_pControlSwitch->SelectItem(0);
	}
	else if( nSwitch == 1 )
	{
		if( m_pTSCheckBox)
			m_pTSCheckBox->SetCheck(FALSE) ; //  
		m_pControlSwitch->SelectItem(1);
	}
	/////////////////////////////////////////////////////////
	// 控件内容清空  
	//m_pBCSMEdit->SetText(_T("")) ; 
	//m_pPhoneNumEdit->SetText(_T("用于客服与您联系核实") ) ; 

}

void CBytsDialog::OnSelectCheckBox(TNotifyUI& msg)
{
	//if( msg.pSender == m_pTSCheckBox)
	//{
	//	if(m_pBYCheckBox)
	//		m_pBYCheckBox->SetCheck(FALSE) ; 
	//
	//}
	//else if(msg.pSender == m_pBYCheckBox)
	//{
	//	if(m_pTSCheckBox)
	//		m_pTSCheckBox->SetCheck(FALSE) ; 
	//}
}

void CBytsDialog::OnBtnSubmit(TNotifyUI& msg)
{
	if( m_pTSCheckBox || m_pBYCheckBox)
	{
		if( ( m_pBYCheckBox->GetCheck() == false ) && (m_pTSCheckBox->GetCheck() == false ))
		{
			MessageBox(NULL,_T("请选择投诉分析师或者表扬分析师") ,_T("警告") ,MB_OK) ; 
			return ;
		}
	}

	ClientComment coment ; 
	coment.nType = m_nSwitch ; 
	// 补充说明 
	if( m_pBCSMEdit)
	{
		coment.strMsg = m_pBCSMEdit->GetText() ; 
	}

	// 电话号码  
	if(m_pPhoneNumEdit)
	{
		coment.strPhoneNum = m_pPhoneNumEdit->GetText() ; 
	}

	if( m_pPhoneNumEdit ) 
	{
		USER_LOG("手机号码：%s" ,m_pPhoneNumEdit->GetText()) ; 
		CDuiString strName = m_pPhoneNumEdit->GetText() ; 

		// 手机号码不为空 下一步还要验证手机号码的正确性 
		if(strName.IsEmpty() == FALSE)
		{
			if(!CheckPhoneNum())
			{
				MessageBox(NULL,_T("您提交的手机号码有误,请输入正确的号码") ,_T("警告") ,MB_OK) ;
				return ;
			}
			MessageBox(NULL,_T("已提交,谢谢反馈！") ,_T("提示") ,MB_OK) ;
			OnExit();
			USER_LOG("退出成功！！！") ;
		}
		else
		{
			MessageBox(NULL,_T("请提交您的手机号码，方便我们与您联系！") ,_T("警告") ,MB_OK) ;
		}
	}
}
void CBytsDialog::OnCloseBtnClick(TNotifyUI& msg)
{
	OnExit();
	return;
}


//最小化按钮
void CBytsDialog::OnBtn_Min(TNotifyUI& msg)
{
	SendMessage(WM_SYSCOMMAND, SC_MINIMIZE, 0);
}

//最大化按钮
void CBytsDialog::OnBtn_Max(TNotifyUI& msg)
{
	SendMessage(WM_SYSCOMMAND, SC_MAXIMIZE, 0);
}


