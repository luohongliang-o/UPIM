// App.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "../resource.h"
#include "EmotionDialog.h" 
#include "EmotionButton.h"

EmotionDialog::EmotionDialog()
{   
}

EmotionDialog::~EmotionDialog()
{ 
	PostQuitMessage(0);
}

LPCTSTR EmotionDialog::GetWindowClassName() const 
{ 
	return _T("QSBFaceMain"); 
}

CDuiString EmotionDialog::GetSkinFile()
{
	return _T("QSBFace.xml");
}

void EmotionDialog::InitWindow()
{

}

CDuiString EmotionDialog::GetSkinFolder()
{
	return CDuiString(CPaintManagerUI::GetInstancePath()) + _T("skin\\");
}

void EmotionDialog::OnPrepare(TNotifyUI& msg)
{
	return;
}  

void EmotionDialog::OnExit(TNotifyUI& msg)
{
	Close();
}

LRESULT EmotionDialog::HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	return 0;
}
 
void EmotionDialog::Notify(TNotifyUI& msg)
{
	if (_tcsicmp(msg.sType, _T("windowinit")) == 0)
	{
		OnPrepare(msg);
	}
	else if (_tcsicmp(msg.sType, _T("click")) == 0)
	{
		if( _tcsicmp(msg.pSender->GetName(),_T("closebtn")) == 0) 
		{
			PostQuitMessage(0);
			return; 
		}
		else if( _tcsicmp( msg.pSender->GetName() , _T("minbtn")) == 0 ) 
		{ 
			SendMessage(WM_SYSCOMMAND, SC_MINIMIZE, 0);
			return; 
		}
		else if( _tcsicmp(msg.pSender->GetName() ,_T("maxbtn")) ==0 ) 
		{ 
			SendMessage(WM_SYSCOMMAND, SC_MAXIMIZE, 0); return; 
		}
		else if( _tcsicmp(msg.pSender->GetName() , _T("restorebtn")) ==0 ) 
		{ 
			SendMessage(WM_SYSCOMMAND, SC_RESTORE, 0); return; 
		} 
		else if( _tcsicmp(msg.pSender->GetName() , _T("prev_page")) ==0 )  //上一页
		{
			CEmotionButton *pEmotionButton = static_cast<CEmotionButton*>(m_PaintManager.FindControl(_T("emotion_button")));
			if(pEmotionButton!=NULL && pEmotionButton->GetCurrentPage()> 0)
			{							
				pEmotionButton->SetPage(pEmotionButton->GetCurrentPage()-1); 
			}
		}
		else if( _tcsicmp(msg.pSender->GetName() , _T("next_page")) ==0 )  //下一页
		{
			CEmotionButton *pEmotionButton = static_cast<CEmotionButton*>(m_PaintManager.FindControl(_T("emotion_button")));

			if(pEmotionButton!=NULL && pEmotionButton->GetCurrentPage() < (pEmotionButton->GetPageCount()-1))
			{ 
				pEmotionButton->SetPage(pEmotionButton->GetCurrentPage()+1); 
			}
		} 
	}
	return;
}

LRESULT EmotionDialog::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	return __super::HandleMessage(uMsg, wParam, lParam);
}
	 
LRESULT EmotionDialog::OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    // 有时会在收到WM_NCDESTROY后收到wParam为SC_CLOSE的WM_SYSCOMMAND
    if( wParam == SC_CLOSE ) 
	{
        ::PostQuitMessage(0L);
        bHandled = TRUE;
        return 0;
    }
    BOOL bZoomed = ::IsZoomed(*this);
    LRESULT lRes = CWindowWnd::HandleMessage(uMsg, wParam, lParam);
    if( ::IsZoomed(*this) != bZoomed ) {
        if( !bZoomed ) {
            CControlUI* pControl = static_cast<CControlUI*>(m_PaintManager.FindControl(_T("maxbtn")));
            if( pControl ) pControl->SetVisible(false);
            pControl = static_cast<CControlUI*>(m_PaintManager.FindControl(_T("restorebtn")));
            if( pControl ) pControl->SetVisible(true);
        }
        else {
            CControlUI* pControl = static_cast<CControlUI*>(m_PaintManager.FindControl(_T("maxbtn")));
            if( pControl ) pControl->SetVisible(true);
            pControl = static_cast<CControlUI*>(m_PaintManager.FindControl(_T("restorebtn")));
            if( pControl ) pControl->SetVisible(false);
        }
    }
    return lRes;
}
 
LRESULT EmotionDialog::ResponseDefaultKeyEvent(WPARAM wParam)
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


CControlUI* EmotionDialog::CreateControl(LPCTSTR pstrClass)
{
//  	if( _tcscmp(pstrClass, _T("EmotionButton")) == 0 ) 
//  		return new CEmotionButton;
	return NULL;
}

LRESULT EmotionDialog::OnKillFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	Close();
	return 0;
}
