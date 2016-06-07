#include "stdafx.h"
#include <windows.h>
#include <shellapi.h>

#include "ChatDialog.h"
#include "ColorPicker.h"
#include "../globalvar.h"
#include "../../UiLib/Control/UIRichEdit.h"
#include "../LoadDllFunc.h"

#pragma  comment (lib,"oledlg.lib")

CFontInfo g_buddyFontInfo;

CFontInfo::CFontInfo(void)
{
	m_nSize = 10;
	m_clrText = RGB(0,0,0);
	m_strName = _T("微软雅黑");
	m_bBold = FALSE;
	m_bItalic = FALSE;
	m_bUnderLine = FALSE;
}

CFontInfo::~CFontInfo(void)
{

}

//主面板
const TCHAR* const kTitleControlName			= _T("apptitle");
const TCHAR* const kCloseButtonControlName		= _T("closebtn");
const TCHAR* const kMinButtonControlName		= _T("minbtn");
const TCHAR* const kMaxButtonControlName		= _T("maxbtn");
const TCHAR* const kRestoreButtonControlName	= _T("restorebtn");
const TCHAR* const kSelfNameControlName			= _T("mainselfname");
const TCHAR* const kBackgroundControlName		= _T("bg");
//联系人
const TCHAR* const kLogoButtonControlName		= _T("logo");
const TCHAR* const kNickNameControlName			= _T("nickname");
const TCHAR* const kDescriptionControlName		= _T("description");
const TCHAR* const kFontButtonControlName		= _T("fontbtn");
const TCHAR* const kScreenShotsControlName		= _T("screenshotsbtn");
const TCHAR* const kImageBtnControlName			= _T("imagebtn");

const TCHAR* const kFontbarControlName			= _T("fontbar");
const TCHAR* const kFontTypeControlName			= _T("font_type");
const TCHAR* const kFontSizeControlName			= _T("font_size");
const TCHAR* const kBoldButtonControlName		= _T("boldbtn");
const TCHAR* const kItalicButtonControlName		= _T("italicbtn");
const TCHAR* const KUnderlineButtonControlName	= _T("underlinebtn");
const TCHAR* const kColorButtonControlName		= _T("colorbtn");
const TCHAR* const kInputRichEditControlName	= _T("input_richedit");
const TCHAR* const kViewRichEditControlName		= _T("view_richedit");
const TCHAR* const kEmotionButtonControlName	= _T("emotionbtn");
const TCHAR* const kSendButtonControlName		= _T("sendbtn");

const int kEmotionRefreshTimerId = 1001;
const int kEmotionRefreshInterval = 150;

ChatDialog::ChatDialog(const CDuiString& bgimage, DWORD bkcolor, const FriendListItemInfo& myselft_info, const FriendListItemInfo& friend_info, bool bGroupSend/* =false */)
: bgimage_(bgimage)
, bkcolor_(bkcolor)
, myselft_(myselft_info)
, friend_(friend_info)
, emotion_timer_start_(false)
, text_color_(0xFF000000)
, bold_(false)
, italic_(false)
, underline_(false)
, font_size_(12)
, font_face_name_(_T("微软雅黑"))
{
	bGroupSend_ = bGroupSend;

	m_pMinSysBtn = m_pMaxSysBtn = m_pRestoreSysBtn = m_pCloseSysBtn = NULL;
	m_pFontBtn = m_pFaceBtn = m_pImageBtn = NULL;
	m_pFontNameCombo = m_pFontSizeCombo = NULL;
	m_pBoldBtn = m_pItalicBtn = m_pUnderLineBtn = NULL;
	m_pColorBtn = NULL;
	m_pCloseBtn = m_pSendBtn = NULL;
	m_pSendEdit = m_pRecvEdit = NULL;

	memset(&m_ptRBtnDown, 0, sizeof(m_ptRBtnDown));
	m_strCheckMenu = _T("Menu_ZoomRatio_100");
	m_pLastImageOle = NULL;
	m_cxPicBarDlg = m_cyPicBarDlg = 0;
}

ChatDialog::~ChatDialog()
{
}

LPCTSTR ChatDialog::GetWindowClassName() const
{
	return _T("ChatDialog");
}


void ChatDialog::InitWindow()
{
	m_pMinSysBtn = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("minbtn")));
	m_pMaxSysBtn = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("maxbtn")));
	m_pRestoreSysBtn = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("restorebtn")));
	m_pCloseSysBtn = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("closebtn")));
	m_pFontBtn = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("fontbtn")));
	m_pFaceBtn = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("emotionbtn")));
	m_pImageBtn = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("imagebtn")));
	m_pFontNameCombo = static_cast<CComboUI*>(m_PaintManager.FindControl(_T("font_type")));
	m_pFontSizeCombo = static_cast<CComboUI*>(m_PaintManager.FindControl(_T("font_size")));
	m_pBoldBtn = static_cast<COptionUI*>(m_PaintManager.FindControl(_T("boldbtn")));
	m_pItalicBtn = static_cast<COptionUI*>(m_PaintManager.FindControl(_T("italicbtn")));
	m_pUnderLineBtn = static_cast<COptionUI*>(m_PaintManager.FindControl(_T("underlinebtn")));
	m_pColorBtn = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("colorbtn")));

	m_pSendBtn = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("sendbtn")));
	m_pSendEdit = static_cast<CRichEditUI*>(m_PaintManager.FindControl(_T("input_richedit")));
	m_pRecvEdit = static_cast<CRichEditUI*>(m_PaintManager.FindControl(_T("view_richedit")));

	
	//////////////////////////////////////////////////////////////////////////
	IRichEditOleCallback2* pRichEditOleCallback2 = NULL;
	HRESULT hr = ::CoCreateInstance(CLSID_ImageOle, NULL, CLSCTX_INPROC_SERVER,
		__uuidof(IRichEditOleCallback2), (void**)&pRichEditOleCallback2);
	if (SUCCEEDED(hr))
	{
		pRichEditOleCallback2->SetNotifyHwnd(m_hWnd);
		ITextServices * pTextServices = m_pRecvEdit->GetTextServices();
		pRichEditOleCallback2->SetTextServices(pTextServices);
		pTextServices->Release();
		m_pRecvEdit->SetOleCallback(pRichEditOleCallback2);
		pRichEditOleCallback2->Release();
	}

	pRichEditOleCallback2 = NULL;
	hr = ::CoCreateInstance(CLSID_ImageOle, NULL, CLSCTX_INPROC_SERVER,
		__uuidof(IRichEditOleCallback2), (void**)&pRichEditOleCallback2);
	if (SUCCEEDED(hr))
	{
		pRichEditOleCallback2->SetNotifyHwnd(m_hWnd);
		ITextServices * pTextServices = m_pSendEdit->GetTextServices();
		pRichEditOleCallback2->SetTextServices(pTextServices);
		pTextServices->Release();
		m_pSendEdit->SetOleCallback(pRichEditOleCallback2);
		pRichEditOleCallback2->Release();
	}

	IDropTarget *pdt = m_pSendEdit->GetTxDropTarget();
	hr = ::RegisterDragDrop(m_hWnd, pdt);
	pdt->Release();

	//////////////////////////////////////////////////////////////////////////
	/*sqlite测试
	CString strSql;
	strSql.Format("select * from chatrecord where totqid='069000000412' and type=0 order by seq asc limit 0,100");
	CDbSQLite::ResultTable res;
	BOOL brtn = g_SqliteOp.m_sqlite.SelectStatement(strSql, res);
	int nRecord = res.m_records.size();

	for (int nindex = 0; nindex <nRecord; nindex++)
	{
		CDbSQLite::ResultRecord *record = res.next();
		if (record)
		{
			CString strseq = record->m_fields[0];
			CString strseq2 = record->m_fields[1];
			CString strseq3 = record->m_fields[2];
			CString strseq4 = record->m_fields[3];
			CString strseq5 = record->m_fields[4];
			CString strseq6 = record->m_fields[5];
			CString strseq7 = record->m_fields[6];
			CString strseq8 = record->m_fields[7];
			CString strseq9 = record->m_fields[8];
		}
	}
	*/
}

void ChatDialog::OnTimer(TNotifyUI& msg)
{

}


BOOL ChatDialog::Receive(SkinChangedParam param)
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

CDuiString ChatDialog::GetSkinFile()
{
	return _T("chatbox.xml");
}

CDuiString ChatDialog::GetSkinFolder()
{
	return CDuiString(CPaintManagerUI::GetInstancePath()) + _T("skin\\");
}



LRESULT ChatDialog::OnSetFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	bHandled = FALSE;
	return 0;
}

CControlUI* ChatDialog::CreateControl(LPCTSTR pstrClass)
{
	return NULL;
}

LRESULT ChatDialog::ResponseDefaultKeyEvent(WPARAM wParam)
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

LRESULT ChatDialog::OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	BOOL bZoomed = ::IsZoomed(m_hWnd);
	LRESULT lRes = CWindowWnd::HandleMessage(uMsg, wParam, lParam);
	if (::IsZoomed(m_hWnd) != bZoomed)
	{
		if (!bZoomed)
		{
			if (m_pMaxSysBtn != NULL)
				m_pMaxSysBtn->SetVisible(false);
			if (m_pRestoreSysBtn != NULL)
				m_pRestoreSysBtn->SetVisible(true);
		}
		else 
		{
			if (m_pMaxSysBtn != NULL)
				m_pMaxSysBtn->SetVisible(true);
			if (m_pRestoreSysBtn != NULL)
				m_pRestoreSysBtn->SetVisible(false);
		}
	}
	return 0;
}

LRESULT ChatDialog::OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	::RevokeDragDrop(m_hWnd);
	m_FaceList.Reset();
	bHandled = FALSE;
	return 0;
}

void ChatDialog::OnFinalMessage(HWND hWnd)
{
	RemoveObserver();
	WindowImplBase::OnFinalMessage(hWnd);	
	delete this;
}

LRESULT ChatDialog::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (uMsg == WM_MOUSEMOVE)
		OnMouseMove(uMsg, wParam, lParam);

	if (uMsg == WM_RBUTTONDOWN)
	{
		LRESULT lRes = __super::HandleMessage(uMsg, wParam, lParam);
		OnRButtonDown(uMsg, wParam, lParam);
		return lRes;
	}

	if (uMsg == WM_LBUTTONDBLCLK)
		OnLButtonDblClk(uMsg, wParam, lParam);

	if ((m_pSendEdit != NULL) && m_pSendEdit->IsFocused() 
		&& (uMsg == WM_KEYDOWN) && (wParam == 'V') && (lParam & VK_CONTROL))	// 发送消息框的Ctrl+V消息
	{
		m_pSendEdit->PasteSpecial(CF_TEXT);
		return TRUE;
	}

	if (uMsg == WM_NOTIFY && EN_PASTE == ((LPNMHDR)lParam)->code)
	{
		ITextServices * pTextServices = m_pSendEdit->GetTextServices();
		if ((UINT)pTextServices == ((LPNMHDR)lParam)->idFrom)
		{
			LPWSTR lpzData = ((NMRICHEDITOLECALLBACK *)lParam)->lpszText;
			LPCTSTR lpText = UnicodeToAnsi(lpzData);
			AddMsgToSendEdit(lpText);
			delete lpText;
		}
		if (pTextServices != NULL)
			pTextServices->Release();
	}

	if (uMsg == WM_MENU)
	{
		CControlUI * pControl = (CControlUI *)lParam;
		if (pControl != NULL)
			this->m_PaintManager.SendNotify(pControl, _T("menu_msg"), wParam, lParam);
	}

	if (uMsg == FACE_CTRL_SEL)
		return OnFaceCtrlSel(uMsg, wParam, lParam);

	return __super::HandleMessage(uMsg, wParam, lParam);
}

LRESULT ChatDialog::HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	bHandled = FALSE;
	return 0;
}


void ChatDialog::OnExit(TNotifyUI& msg)
{
 	allopenwnd[friend_.id] = NULL;			//窗口关闭，将保存的全局用户id-窗口句柄对应关系清除
 	Close();
}


void ChatDialog::OnPrepare(TNotifyUI& msg)
{
	TCHAR szBuf[MAX_PATH] = {0};

	CControlUI* background = m_PaintManager.FindControl(kBackgroundControlName);
	if (background != NULL)
	{
		_stprintf_s(szBuf, MAX_PATH - 1, _T("file='%s' corner='600,200,1,1'"), bgimage_);
		background->SetBkImage(szBuf);
		background->SetBkColor(bkcolor_);
	}

	CButtonUI* log_button = static_cast<CButtonUI*>(m_PaintManager.FindControl(kLogoButtonControlName));
	if (log_button != NULL)
	{
		_stprintf_s(szBuf, MAX_PATH - 1, _T("%s"), friend_.logo);
		log_button->SetNormalImage(szBuf);
	}

	CControlUI* nick_name = m_PaintManager.FindControl(kNickNameControlName);
	if (nick_name != NULL)
		nick_name->SetText(friend_.nick_name);

	CControlUI* desciption = m_PaintManager.FindControl(kDescriptionControlName);
	if (desciption != NULL)
		desciption->SetText(friend_.description);

	CContainerUI* pFontbar = static_cast<CContainerUI*>(m_PaintManager.FindControl(kFontbarControlName));
	if (pFontbar != NULL)
		pFontbar->SetVisible(!pFontbar->IsVisible());

	//此处加载表情列表配置文件
	m_FaceList.LoadConfigFile(g_config.szFaceConfig);

}

CDuiString GetCurrentTimeString()
{
	SYSTEMTIME time = {0};
	TCHAR szTime[MAX_PATH] = {0};
	::GetLocalTime( &time );
	_stprintf_s( szTime, MAX_PATH, _T("%04d年%02d月%02d日 %02d:%02d:%02d"), time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute, time.wSecond);
	return szTime;
}

//消息循环
void ChatDialog::Notify(TNotifyUI& msg)
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
		if (msg.pSender == m_pMinSysBtn)
			OnBtn_Min(msg);
		else if (msg.pSender == m_pMaxSysBtn)
			OnBtn_Max(msg);
		else if (msg.pSender == m_pRestoreSysBtn)
			OnBtn_Restore(msg);
		else if (msg.pSender == m_pCloseSysBtn)
			OnBtn_Close(msg);
		else if (msg.pSender == m_pCloseBtn)
			OnBtn_Close(msg);
		else if (msg.pSender == m_pBoldBtn)
			OnBtn_Bold(msg);
		else if (msg.pSender == m_pItalicBtn)
			OnBtn_Italic(msg);
		else if (msg.pSender == m_pUnderLineBtn)
			OnBtn_UnderLine(msg);
 		else if (msg.pSender == m_pColorBtn)
 			OnBtn_Color(msg);
		else if (msg.pSender == m_pSendBtn)
			SendMsg_More();
		//	OnBtn_Send(msg);
		else if (msg.pSender == m_pFontBtn)
			OnBtn_Font(msg);
		else if (msg.pSender == m_pFaceBtn)
			OnBtn_Face(msg);
		else if (msg.pSender == m_pImageBtn)
			OnBtn_Image(msg);

 		if (_tcsicmp(msg.pSender->GetName(), kCloseButtonControlName) == 0)
 		{
			OnExit(msg);
 		}
// 		else if (_tcsicmp(msg.pSender->GetName(), kMinButtonControlName) == 0)
// 		{
// 			SendMessage(WM_SYSCOMMAND, SC_MINIMIZE, 0);
// 		}
// 		else if (_tcsicmp(msg.pSender->GetName(), kMaxButtonControlName) == 0)
// 		{
// 			SendMessage(WM_SYSCOMMAND, SC_MAXIMIZE, 0);
// 		}
// 		else if (_tcsicmp(msg.pSender->GetName(), kRestoreButtonControlName) == 0)
// 		{
// 			SendMessage(WM_SYSCOMMAND, SC_RESTORE, 0);
// 		}
// 		else if (_tcsicmp(msg.pSender->GetName(), kFontButtonControlName) == 0)
// 		{
// 			CContainerUI* pFontbar = static_cast<CContainerUI*>(m_PaintManager.FindControl(kFontbarControlName));
// 			if (pFontbar != NULL)
// 				pFontbar->SetVisible(!pFontbar->IsVisible());
// 		}
		// 屏幕截图
		else if (_tcsicmp(msg.pSender->GetName(), kScreenShotsControlName) == 0)
		{
			IUPIM_PupupDialog_();
		}
// 		// 表情
// 		else if (_tcsicmp(msg.pSender->GetName(), kEmotionButtonControlName) == 0)
// 		{
// 			OnBtn_Face(msg);
// 		}
// 		// 插入图片
// 		else if (_tcsicmp(msg.pSender->GetName(), kImageBtnControlName) == 0)
// 		{
// 			OnBtn_Image(msg);
// 		}
		
// 		else if (_tcsicmp(msg.pSender->GetName(), kSendButtonControlName) == 0)
//         {
//             SendMsg();
// 		}
	}
    else if( _tcsicmp(msg.sType, _T("return")) == 0 ) 
    {
        if (_tcsicmp(msg.pSender->GetName(), kInputRichEditControlName) == 0)
        {
            SendMsg_More();
        }
    }
	else if (msg.sType == _T("itemselect"))
	{
		if (msg.pSender == m_pFontNameCombo)
			OnCbo_SelChange_FontName(msg);
		else if (msg.pSender == m_pFontSizeCombo)
			OnCbo_SelChange_FontSize(msg);
	}
	else if (msg.sType == _T("timer"))
		OnTimer(msg);
	else if (msg.sType == _T("menu_msg"))
	{
		CControlUI * pControl = (CControlUI *)msg.lParam;
		tstring strMenuName = (TCHAR *)(LPCTSTR)pControl->GetUserData();
		if (strMenuName == _T("Menu_Cut"))
			OnMenu_Cut(msg);
		else if (strMenuName == _T("Menu_Copy"))
			OnMenu_Copy(msg);
		else if (strMenuName == _T("Menu_Paste"))
			OnMenu_Paste(msg);
		else if (strMenuName == _T("Menu_SelAll"))
			OnMenu_SelAll(msg);
		else if (strMenuName == _T("Menu_Clear"))
			OnMenu_Clear(msg);
		else if (strMenuName == _T("Menu_ZoomRatio_400")
			|| strMenuName == _T("Menu_ZoomRatio_200")
			|| strMenuName == _T("Menu_ZoomRatio_150")
			|| strMenuName == _T("Menu_ZoomRatio_125")
			|| strMenuName == _T("Menu_ZoomRatio_100")
			|| strMenuName == _T("Menu_ZoomRatio_75")
			|| strMenuName == _T("Menu_ZoomRatio_50"))
			OnMenu_ZoomRatio(msg);
		else if (strMenuName == _T("Menu_SaveAs"))
			OnMenu_SaveAs(msg);
	}
	/*
	else if (_tcsicmp(msg.sType, _T("selectchanged")) == 0)
	{
		if (_tcsicmp(msg.pSender->GetName(), kColorButtonControlName) == 0)
		{
			CContainerUI* pFontbar = static_cast<CContainerUI*>(m_PaintManager.FindControl(kFontbarControlName));
			if (pFontbar != NULL)
			{
				POINT pt = {0};
				CDuiRect rcFontbar = pFontbar->GetPos();
				CDuiRect rcColorBtn = msg.pSender->GetPos();
				CDuiRect rcWindow;
				GetWindowRect(m_hWnd, &rcWindow);

				pt.y = rcWindow.top + rcFontbar.top;
				pt.x = rcWindow.left + rcColorBtn.left + static_cast<LONG>(rcColorBtn.right - rcColorBtn.left / 2);
				new CColorPicker(this, pt);
			}
		}
		else if (_tcsicmp(msg.pSender->GetName(), kBoldButtonControlName) == 0)
		{
			COptionUI* bold_button = static_cast<COptionUI*>(msg.pSender);
			if (bold_button != NULL)
			{
				bold_ = bold_button->IsSelected();
				FontStyleChanged();
			}
		}
		else if (_tcsicmp(msg.pSender->GetName(), kItalicButtonControlName) == 0)
		{
			COptionUI* italic_button = static_cast<COptionUI*>(msg.pSender);
			if (italic_button != NULL)
			{
				italic_ = italic_button->IsSelected();
				FontStyleChanged();
			}
		}
		else if (_tcsicmp(msg.pSender->GetName(), KUnderlineButtonControlName) == 0)
		{
			COptionUI* underline_button = static_cast<COptionUI*>(msg.pSender);
			if (underline_button != NULL)
			{
				underline_ = underline_button->IsSelected();
				FontStyleChanged();
			}
		}
	}
	else if (_tcsicmp(msg.sType, _T("itemselect")) == 0)
	{
		if (_tcsicmp(msg.pSender->GetName(), kFontTypeControlName) == 0)
		{
			CComboUI* font_type = static_cast<CComboUI*>(msg.pSender);
			if (font_type != NULL)
			{
				font_face_name_ = font_type->GetText();
				FontStyleChanged();
			}
		}
		else if (_tcsicmp(msg.pSender->GetName(), kFontSizeControlName) == 0)
		{
			CComboUI* font_size = static_cast<CComboUI*>(msg.pSender);
			if (font_size != NULL)
			{
				font_size_ = _ttoi(font_size->GetText());
				FontStyleChanged();
			}
		}
	}*/
}



void ChatDialog::OnBtn_Min(TNotifyUI& msg)
{
	SendMessage(WM_SYSCOMMAND, SC_MINIMIZE, 0);
}

void ChatDialog::OnBtn_Max(TNotifyUI& msg)
{
	SendMessage(WM_SYSCOMMAND, SC_MAXIMIZE, 0);
}

void ChatDialog::OnBtn_Restore(TNotifyUI& msg)
{
	SendMessage(WM_SYSCOMMAND, SC_RESTORE, 0);
}

void ChatDialog::OnBtn_Close(TNotifyUI& msg)
{
	allopenwnd[friend_.id] = NULL;			//窗口关闭，将保存的全局用户id-窗口句柄对应关系清除
	Close();
}


void ChatDialog::OnCbo_SelChange_FontName(TNotifyUI& msg)
{
	tstring strText = (TCHAR *)m_pFontNameCombo->GetText().GetData();
	if (m_fontInfo.m_strName != strText)
	{
		m_fontInfo.m_strName = strText;
		ITextServices * pTextServices = m_pSendEdit->GetTextServices();
		RichEdit_SetDefFont(pTextServices, m_fontInfo.m_strName.c_str(),
			m_fontInfo.m_nSize, m_fontInfo.m_clrText, m_fontInfo.m_bBold,
			m_fontInfo.m_bItalic, m_fontInfo.m_bUnderLine, FALSE);
		pTextServices->Release();
		g_buddyFontInfo.m_strName = m_fontInfo.m_strName;
	}
}

void ChatDialog::OnCbo_SelChange_FontSize(TNotifyUI& msg)
{
	tstring strText = (TCHAR *)m_pFontSizeCombo->GetText().GetData();

	int nSize = _tcstol(strText.c_str(), NULL, 10);
	if (m_fontInfo.m_nSize != nSize)
	{
		m_fontInfo.m_nSize = nSize;
		ITextServices * pTextServices = m_pSendEdit->GetTextServices();
		RichEdit_SetDefFont(pTextServices, m_fontInfo.m_strName.c_str(),
			m_fontInfo.m_nSize, m_fontInfo.m_clrText, m_fontInfo.m_bBold,
			m_fontInfo.m_bItalic, m_fontInfo.m_bUnderLine, FALSE);
		pTextServices->Release();
		g_buddyFontInfo.m_nSize = m_fontInfo.m_nSize;
	}
}

void ChatDialog::OnBtn_Bold(TNotifyUI& msg)
{
	BOOL bBold = !m_pBoldBtn->IsSelected();
	if (m_fontInfo.m_bBold != bBold)
	{
		m_fontInfo.m_bBold = bBold;
		ITextServices * pTextServices = m_pSendEdit->GetTextServices();
		RichEdit_SetDefFont(pTextServices, m_fontInfo.m_strName.c_str(),
			m_fontInfo.m_nSize, m_fontInfo.m_clrText, m_fontInfo.m_bBold,
			m_fontInfo.m_bItalic, m_fontInfo.m_bUnderLine, FALSE);
		pTextServices->Release();
		g_buddyFontInfo.m_bBold = m_fontInfo.m_bBold;
	}
}

void ChatDialog::OnBtn_Italic(TNotifyUI& msg)
{
	BOOL bItalic = !m_pItalicBtn->IsSelected();
	if (m_fontInfo.m_bItalic != bItalic)
	{
		m_fontInfo.m_bItalic = bItalic;
		ITextServices * pTextServices = m_pSendEdit->GetTextServices();
		RichEdit_SetDefFont(pTextServices, m_fontInfo.m_strName.c_str(),
			m_fontInfo.m_nSize, m_fontInfo.m_clrText, m_fontInfo.m_bBold,
			m_fontInfo.m_bItalic, m_fontInfo.m_bUnderLine, FALSE);
		pTextServices->Release();
		g_buddyFontInfo.m_bItalic = m_fontInfo.m_bItalic;
	}
}

void ChatDialog::OnBtn_UnderLine(TNotifyUI& msg)
{
	BOOL bUnderLine = !m_pUnderLineBtn->IsSelected();
	if (m_fontInfo.m_bUnderLine != bUnderLine)
	{
		m_fontInfo.m_bUnderLine = bUnderLine;
		ITextServices * pTextServices = m_pSendEdit->GetTextServices();
		RichEdit_SetDefFont(pTextServices, m_fontInfo.m_strName.c_str(),
			m_fontInfo.m_nSize, m_fontInfo.m_clrText, m_fontInfo.m_bBold,
			m_fontInfo.m_bItalic, m_fontInfo.m_bUnderLine, FALSE);
		pTextServices->Release();
		g_buddyFontInfo.m_bUnderLine = m_fontInfo.m_bUnderLine;
	}
}

void ChatDialog::OnBtn_Color(TNotifyUI& msg)
{
	static COLORREF rgbCustomColors[16] =
	{
		RGB(255, 255, 255), RGB(255, 255, 255), 
		RGB(255, 255, 255), RGB(255, 255, 255), 
		RGB(255, 255, 255), RGB(255, 255, 255), 
		RGB(255, 255, 255), RGB(255, 255, 255), 
		RGB(255, 255, 255), RGB(255, 255, 255), 
		RGB(255, 255, 255), RGB(255, 255, 255), 
		RGB(255, 255, 255), RGB(255, 255, 255), 
		RGB(255, 255, 255), RGB(255, 255, 255), 
	};

	CHOOSECOLOR cc = {0};
	cc.lStructSize = sizeof(cc);
	cc.lpCustColors = rgbCustomColors;
	cc.hwndOwner = m_hWnd;
	cc.Flags = CC_RGBINIT;

	BOOL bRet = ::ChooseColor(&cc);
	if (bRet)
	{
		COLORREF clrText = cc.rgbResult;
		if (m_fontInfo.m_clrText != clrText)
		{
			m_fontInfo.m_clrText = clrText;
			ITextServices * pTextServices = m_pSendEdit->GetTextServices();
			RichEdit_SetDefFont(pTextServices, m_fontInfo.m_strName.c_str(),
				m_fontInfo.m_nSize, m_fontInfo.m_clrText, m_fontInfo.m_bBold,
				m_fontInfo.m_bItalic, m_fontInfo.m_bUnderLine, FALSE);
			pTextServices->Release();
			g_buddyFontInfo.m_clrText = m_fontInfo.m_clrText;
		}
	}
}

void ChatDialog::OnBtn_Font(TNotifyUI& msg)
{
	CContainerUI* pFontbar = static_cast<CContainerUI*>(m_PaintManager.FindControl(kFontbarControlName));
	if (pFontbar != NULL)
		pFontbar->SetVisible(!pFontbar->IsVisible());
}

void ChatDialog::OnBtn_Face(TNotifyUI& msg)
{
	m_FaceSelDlg.SetFaceList(&m_FaceList);
	if (!::IsWindow((HWND)m_FaceSelDlg))
	{
		m_FaceSelDlg.Create(m_hWnd, NULL, WS_CHILD|WS_POPUP, WS_EX_TOOLWINDOW);

		RECT rcBtn = m_pFaceBtn->GetPos();
		::ClientToScreen(m_hWnd, (LPPOINT)&rcBtn);

		int cx = 432;
		int cy = 236;
		int x = rcBtn.left - cx / 2;
		int y = rcBtn.top - cy;

		::SetWindowPos((HWND)m_FaceSelDlg, NULL, x, y, cx, cy, NULL);
		::ShowWindow((HWND)m_FaceSelDlg, SW_SHOW);
	}
}

void ChatDialog::OnBtn_Image(TNotifyUI& msg)
{
	if (NULL == m_pSendEdit)
		return;

	LPCTSTR lpszFilter = _T("图像文件(*.bmp;*.jpg;*.jpeg;*.gif;*.png)\0*.bmp;*.jpg;*.jpeg;*.gif;*.png\0\0");;

	CFileDialogEx fileDlg;
	fileDlg.SetFilter(lpszFilter);
	fileDlg.SetParentWnd(m_hWnd);
	if (fileDlg.ShowOpenFileDlg())
	{
 		_RichEdit_InsertFace(m_pSendEdit, fileDlg.GetPathName().c_str(), -1, -1);
 		m_pSendEdit->SetFocus();
	}
}

// “剪切”菜单
void ChatDialog::OnMenu_Cut(TNotifyUI& msg)
{
	m_pSendEdit->Cut();
}

// “复制”菜单
void ChatDialog::OnMenu_Copy(TNotifyUI& msg)
{
	if (msg.pSender == m_pSendEdit)
		m_pSendEdit->Copy();
	else if (msg.pSender == m_pRecvEdit)
		m_pRecvEdit->Copy();
}

// “粘贴”菜单
void ChatDialog::OnMenu_Paste(TNotifyUI& msg)
{
	m_pSendEdit->PasteSpecial(CF_TEXT);
}

// “全部选择”菜单
void ChatDialog::OnMenu_SelAll(TNotifyUI& msg)
{
	if (msg.pSender == m_pSendEdit)
		m_pSendEdit->SetSel(0, -1);
	else if (msg.pSender == m_pRecvEdit)
		m_pRecvEdit->SetSel(0, -1);
}

// “清屏”菜单
void ChatDialog::OnMenu_Clear(TNotifyUI& msg)
{
	m_pRecvEdit->SetText(_T(""));
}

// “显示比例”菜单
void ChatDialog::OnMenu_ZoomRatio(TNotifyUI& msg)
{
	CControlUI * pControl = (CControlUI *)msg.lParam;
	tstring strMenuName = (TCHAR *)(LPCTSTR)pControl->GetUserData();
	if (strMenuName == _T("Menu_ZoomRatio_400"))
		m_pRecvEdit->SetZoom(16, 4);
	else if (strMenuName == _T("Menu_ZoomRatio_200"))
		m_pRecvEdit->SetZoom(8, 4);
	else if (strMenuName == _T("Menu_ZoomRatio_150"))
		m_pRecvEdit->SetZoom(6, 4);
	else if (strMenuName == _T("Menu_ZoomRatio_125"))
		m_pRecvEdit->SetZoom(5, 4);
	else if (strMenuName == _T("Menu_ZoomRatio_100"))
		m_pRecvEdit->SetZoom(0, 0);
	else if (strMenuName == _T("Menu_ZoomRatio_75"))
		m_pRecvEdit->SetZoom(3, 4);
	else if (strMenuName == _T("Menu_ZoomRatio_50"))
		m_pRecvEdit->SetZoom(1, 2);
	else
		return;

	m_strCheckMenu = strMenuName;
}

// “另存为”菜单
void ChatDialog::OnMenu_SaveAs(TNotifyUI& msg)
{
	if (msg.pSender != m_pSendEdit && msg.pSender != m_pRecvEdit)
		return;

	ITextServices * pTextServices = ((CRichEditUI *)msg.pSender)->GetTextServices();
	IImageOle * pImageOle = NULL;
	BOOL bRet = RichEdit_GetImageOle(pTextServices, m_ptRBtnDown, &pImageOle);
	pTextServices->Release();
	if (!bRet || NULL == pImageOle)
		return;

	tstring strFileName;

	BSTR bstrFileName = NULL;
	HRESULT hr = pImageOle->GetFileName(&bstrFileName);
	if (SUCCEEDED(hr))
		strFileName = COLE2T(bstrFileName);
	if (bstrFileName != NULL)
		::SysFreeString(bstrFileName);

	LPCTSTR lpszDefExt;
	LPCTSTR lpszFileName = _T("未命名");
	DWORD dwFlags = OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT|OFN_NOCHANGEDIR|OFN_EXTENSIONDIFFERENT;
	LPCTSTR lpszFilter;
	HWND hWndParent = m_hWnd;

	GUID guid = {0};
	hr = pImageOle->GetRawFormat(&guid);

	if (InlineIsEqualGUID(guid, Gdiplus::ImageFormatJPEG))
	{
		lpszDefExt = _T(".jpg");
		lpszFilter = _T("图像文件(*.jpg)\0*.jpg\0图像文件(*.bmp)\0*.bmp\0\0");
	}
	else if (InlineIsEqualGUID(guid, Gdiplus::ImageFormatPNG))
	{
		lpszDefExt = _T(".png");
		lpszFilter = _T("图像文件(*.png)\0*.png\0\0");
	}
	else if (InlineIsEqualGUID(guid, Gdiplus::ImageFormatGIF))
	{
		lpszDefExt = _T(".gif");
		lpszFilter = _T("图像文件(*.gif)\0*.gif\0图像文件(*.jpg)\0*.jpg\0图像文件(*.bmp)\0*.bmp\0\0");
	}
	else
	{
		lpszDefExt = _T(".jpg");
		lpszFilter = _T("图像文件(*.jpg)\0*.jpg\0图像文件(*.bmp)\0*.bmp\0\0");
	}

	CFileDialogEx fileDlg;
	fileDlg.SetDefExt(lpszDefExt);
	fileDlg.SetFileName(lpszFileName);
	fileDlg.SetFlags(dwFlags);
	fileDlg.SetFilter(lpszFilter);
	fileDlg.SetParentWnd(m_hWnd);
	fileDlg.SetTitle(_T("保存图片"));
	if (fileDlg.ShowSaveFileDlg())
	{
		tstring strSavePath = fileDlg.GetPathName();
		tstring strExtName = _T(".") + ZYM::CPath::GetExtension(strSavePath.c_str());
		GUID guid2 = GetFileTypeGuidByExtension(strExtName.c_str());

		if (InlineIsEqualGUID(guid, guid2))
		{
			CopyFile(strFileName.c_str(), strSavePath.c_str(), FALSE);
		}
		else
		{
			BSTR bstrSavePath = ::SysAllocString(CT2OLE(strSavePath.c_str()));
			if (bstrSavePath != NULL)
			{
				pImageOle->SaveAsFile(bstrSavePath);
				::SysFreeString(bstrSavePath);
			}
		}
	}

	if (pImageOle != NULL)
		pImageOle->Release();
}

LRESULT ChatDialog::OnMouseMove(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	IImageOle * pNewImageOle = NULL;
	RECT rc = {0};

	POINT pt = {LOWORD(lParam), HIWORD(lParam)};
	CControlUI * pControl = this->m_PaintManager.FindControl(pt);
	if (pControl != NULL)
	{
		if (pControl == m_pSendEdit || pControl == m_pRecvEdit)
		{
			ITextServices * pTextServices = ((CRichEditUI *)pControl)->GetTextServices();

			IImageOle * pImageOle = NULL;
			BOOL bRet = RichEdit_GetImageOle(pTextServices, pt, &pImageOle);
			if (bRet && pImageOle != NULL)
			{
				pNewImageOle = pImageOle;
				pImageOle->GetObjectRect(&rc);
			}
			if (pImageOle != NULL)
				pImageOle->Release();

			pTextServices->Release();
		}
	}

	if (m_pLastImageOle != pNewImageOle)
	{
		m_pLastImageOle = pNewImageOle;
		if (m_pLastImageOle != NULL)
		{
			m_ptRBtnDown = pt;

			if (!::IsWindow((HWND)m_PicBarDlg))
			{
				m_PicBarDlg.Create(m_hWnd, NULL, WS_CHILD|WS_POPUP, WS_EX_TOOLWINDOW);

				RECT rcPos = {0};
				::GetClientRect((HWND)m_PicBarDlg, &rcPos);
				m_cxPicBarDlg = rcPos.right-rcPos.left;
				m_cyPicBarDlg = rcPos.bottom-rcPos.top;
			}

			m_PicBarDlg.SetCallBackCtrl(pControl);

			RECT rc2 = pControl->GetPos();
			POINT pt = {rc.right, rc.bottom-m_cyPicBarDlg};
			if (pt.x < rc2.left)
				pt.x = rc2.left;
			if (pt.x > rc2.right)
				pt.x = rc2.right;
			if (pt.y > rc2.bottom-m_cyPicBarDlg)
				pt.y = rc2.bottom-m_cyPicBarDlg;
			::ClientToScreen(m_hWnd, &pt);

			::SetWindowPos((HWND)m_PicBarDlg, NULL, pt.x, pt.y, m_cxPicBarDlg, m_cyPicBarDlg, SWP_NOACTIVATE|SWP_SHOWWINDOW);
		}
		else
		{
			::ShowWindow((HWND)m_PicBarDlg, SW_HIDE);
		}
	}

	return 0;
}

LRESULT ChatDialog::OnRButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	POINT pt = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };
	CControlUI * pControl = this->m_PaintManager.FindControl(pt);

	CMenuWnd* pMenu = new CMenuWnd(m_hWnd);
	::ClientToScreen(m_hWnd, &pt);
	pMenu->Init(NULL, _T("RecvEditMenu.xml"), _T("xml"), pt, pControl);
	pMenu->ShowWindow();

	/*
	
	if (pControl != NULL)
	{	
		if (pControl == m_pRecvEdit)
		{
			m_ptRBtnDown = pt;

			CMenuWnd* pMenu = new CMenuWnd(m_hWnd);
			::ClientToScreen(m_hWnd, &pt);
			pMenu->Init(NULL, _T("RecvEditMenu.xml"), _T("xml"), pt, pControl);

			BOOL bSel = (m_pRecvEdit->GetSelectionType() != SEL_EMPTY);
			pMenu->EnableMenuItem(_T("Menu_Copy"), bSel);

			pMenu->CheckMenuItem(m_strCheckMenu.c_str(), TRUE);

			ITextServices * pTextServices = m_pRecvEdit->GetTextServices();

			IImageOle * pImageOle = NULL;
			BOOL bRet = RichEdit_GetImageOle(pTextServices, m_ptRBtnDown, &pImageOle);
			BOOL bSaveAs = (bRet && pImageOle != NULL);
			pMenu->EnableMenuItem(_T("Menu_SaveAs"), bSaveAs);
			if (pImageOle != NULL)
				pImageOle->Release();

			pTextServices->Release();
			
		}
		else if (pControl == m_pSendEdit)
		{
			m_ptRBtnDown = pt;

			CMenuWnd* pMenu = new CMenuWnd(m_hWnd);
			::ClientToScreen(m_hWnd, &pt);
			pMenu->Init(NULL, _T("SendEditMenu.xml"), _T("xml"), pt, pControl);

			BOOL bSel = (m_pSendEdit->GetSelectionType() != SEL_EMPTY);
			pMenu->EnableMenuItem(_T("Menu_Cut"), bSel);
			pMenu->EnableMenuItem(_T("Menu_Copy"), bSel);

			BOOL bPaste = m_pSendEdit->CanPaste();
			pMenu->EnableMenuItem(_T("Menu_Paste"), bPaste);

			ITextServices * pTextServices = m_pSendEdit->GetTextServices();

			IImageOle * pImageOle = NULL;
			BOOL bRet = RichEdit_GetImageOle(pTextServices, m_ptRBtnDown, &pImageOle);
			BOOL bSaveAs = (bRet && pImageOle != NULL);
			pMenu->EnableMenuItem(_T("Menu_SaveAs"), bSaveAs);
			if (pImageOle != NULL)
				pImageOle->Release();

			pTextServices->Release();
		}
	}*/
	
	return 0;
}

LRESULT ChatDialog::OnLButtonDblClk(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	POINT pt = {GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)};
	CControlUI * pControl = this->m_PaintManager.FindControl(pt);
	if (pControl != NULL)
	{
		if (pControl == m_pSendEdit || pControl == m_pRecvEdit)
		{
			ITextServices * pTextServices = ((CRichEditUI *)pControl)->GetTextServices();

			IImageOle * pImageOle = NULL;
			BOOL bRet = RichEdit_GetImageOle(pTextServices, pt, &pImageOle);
			if (bRet && pImageOle != NULL)
			{
				BSTR bstrFileName = NULL;
				HRESULT hr = pImageOle->GetFileName(&bstrFileName);
				if (SUCCEEDED(hr))
					::ShellExecute(NULL, _T("open"), COLE2T(bstrFileName), NULL, NULL, SW_SHOWNORMAL);
				if (bstrFileName != NULL)
					::SysFreeString(bstrFileName);
			}
			if (pImageOle != NULL)
				pImageOle->Release();

			pTextServices->Release();
		}
	}
	return 0;
}

// “表情”控件选取消息
LRESULT ChatDialog::OnFaceCtrlSel(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	int nFaceId = m_FaceSelDlg.GetSelFaceId();
	int nFaceIndex = m_FaceSelDlg.GetSelFaceIndex();
	tstring strFileName = m_FaceSelDlg.GetSelFaceFileName();
	if (!strFileName.empty())
	{
		_RichEdit_InsertFace(m_pSendEdit, strFileName.c_str(), nFaceId, nFaceIndex);
		m_pSendEdit->SetFocus();
	}
	((COptionUI *)m_pFaceBtn)->Selected(false);
	return 0;
}


void ChatDialog::_RichEdit_ReplaceSel(CRichEditUI * pRichEdit, LPCTSTR lpszNewText)
{
	ITextServices * pTextServices = pRichEdit->GetTextServices();
	if (pRichEdit == m_pRecvEdit)
	{
		RichEdit_ReplaceSel(pTextServices, lpszNewText, 
			m_fontInfo.m_strName.c_str(), m_fontInfo.m_nSize, 
			m_fontInfo.m_clrText, m_fontInfo.m_bBold, m_fontInfo.m_bItalic, 
			m_fontInfo.m_bUnderLine, FALSE, 300);
	}
	else
	{
		RichEdit_ReplaceSel(pTextServices, lpszNewText);
	}
	pTextServices->Release();
}


void ChatDialog::_RichEdit_ReplaceSel_More(CRichEditUI * pRichEdit, LPCTSTR lpszNewText, CFontInfo mfontInfo)
{
	ITextServices * pTextServices = pRichEdit->GetTextServices();
	if (pRichEdit == m_pRecvEdit)
	{
		RichEdit_ReplaceSel(pTextServices, lpszNewText, 
			mfontInfo.m_strName.c_str(), mfontInfo.m_nSize, 
			mfontInfo.m_clrText, mfontInfo.m_bBold, mfontInfo.m_bItalic, 
			mfontInfo.m_bUnderLine, FALSE, 300);
	}
	else
	{
		RichEdit_ReplaceSel(pTextServices, lpszNewText);
	}
	pTextServices->Release();
}

BOOL ChatDialog::_RichEdit_InsertFace(CRichEditUI * pRichEdit, LPCTSTR lpszFileName, int nFaceId, int nFaceIndex)
{
	BOOL bRet = FALSE;

	if (NULL == pRichEdit || NULL == lpszFileName || NULL == *lpszFileName)
		return FALSE;

	ITextServices * pTextServices = pRichEdit->GetTextServices();
	ITextHost * pTextHost = pRichEdit->GetTextHost();
	if (pTextServices != NULL && pTextHost != NULL)
	{
		if (pRichEdit == m_pRecvEdit)
			RichEdit_SetStartIndent(pTextServices, 300);
		bRet = RichEdit_InsertFace(pTextServices, pTextHost, 
			lpszFileName, nFaceId, nFaceIndex, RGB(255,255,255), TRUE, 40);
	}

	if (pTextServices != NULL)
		pTextServices->Release();
	if (pTextHost != NULL)
		pTextHost->Release();

	return bRet;
}

BOOL ChatDialog::HandleSysFaceId(CRichEditUI * pRichEdit, LPCTSTR& p, tstring& strText)
{
	CHAR *cStart = _T("[\"");
	CHAR *cEnd = _T("\"]");
	int nFaceId = GetBetweenInt(p+2, cStart, cEnd, -1);

	CFaceInfo * lpFaceInfo = m_FaceList.GetFaceInfoById(nFaceId);
	if (lpFaceInfo != NULL)
	{
		if (!strText.empty())
		{
			_RichEdit_ReplaceSel(pRichEdit, strText.c_str()); 
			strText = _T("");
		}

		_RichEdit_InsertFace(pRichEdit, lpFaceInfo->m_strFileName.c_str(), 
			lpFaceInfo->m_nId, lpFaceInfo->m_nIndex);

		p = _tcsstr(p+2, _T("\"]"));
		p++;
		return TRUE;
	}
	return FALSE;
}


BOOL ChatDialog::HandleSysFaceId_More(CRichEditUI * pRichEdit, LPCTSTR& p, tstring& strText, CFontInfo mfontInfo)
{
	CHAR *cStart = _T("[\"");
	CHAR *cEnd = _T("\"]");
	int nFaceId = GetBetweenInt(p+2, cStart, cEnd, -1);

	CFaceInfo * lpFaceInfo = m_FaceList.GetFaceInfoById(nFaceId);
	if (lpFaceInfo != NULL)
	{
		if (!strText.empty())
		{
			_RichEdit_ReplaceSel_More(pRichEdit, strText.c_str(), mfontInfo); 
			strText = _T("");
		}

		_RichEdit_InsertFace(pRichEdit, lpFaceInfo->m_strFileName.c_str(), 
			lpFaceInfo->m_nId, lpFaceInfo->m_nIndex);

		p = _tcsstr(p+2, _T("\"]"));
		p++;
		return TRUE;
	}
	return FALSE;
}

BOOL ChatDialog::HandleSysFaceIndex(CRichEditUI * pRichEdit, LPCTSTR& p, tstring& strText)
{
	CHAR *cStart = _T("[\"");
	CHAR *cEnd = _T("\"]");
	int nFaceIndex = GetBetweenInt(p+2, cStart, cEnd, -1);

	CFaceInfo * lpFaceInfo = m_FaceList.GetFaceInfoByIndex(nFaceIndex);
	if (lpFaceInfo != NULL)
	{
		if (!strText.empty())
		{
			_RichEdit_ReplaceSel(pRichEdit, strText.c_str()); 
			strText = _T("");
		}

		_RichEdit_InsertFace(pRichEdit, lpFaceInfo->m_strFileName.c_str(), 
			lpFaceInfo->m_nId, lpFaceInfo->m_nIndex);

		p = _tcsstr(p+2, _T("\"]"));
		p++;
		return TRUE;
	}
	return FALSE;
}


BOOL ChatDialog::HandleSysFaceIndex_More(CRichEditUI * pRichEdit, LPCTSTR& p, tstring& strText, CFontInfo mfontInfo)
{
	CHAR *cStart = _T("[\"");
	CHAR *cEnd = _T("\"]");
	int nFaceIndex = GetBetweenInt(p+2, cStart, cEnd, -1);

	CFaceInfo * lpFaceInfo = m_FaceList.GetFaceInfoByIndex(nFaceIndex);
	if (lpFaceInfo != NULL)
	{
		if (!strText.empty())
		{
			_RichEdit_ReplaceSel_More(pRichEdit, strText.c_str(), mfontInfo); 
			strText = _T("");
		}

		_RichEdit_InsertFace(pRichEdit, lpFaceInfo->m_strFileName.c_str(), 
			lpFaceInfo->m_nId, lpFaceInfo->m_nIndex);

		p = _tcsstr(p+2, _T("\"]"));
		p++;
		return TRUE;
	}
	return FALSE;
}

BOOL ChatDialog::HandleCustomPic(CRichEditUI * pRichEdit, LPCTSTR& p, tstring& strText)
{
	CHAR *cStart = _T("[\"");
	CHAR *cEnd = _T("\"]");
	tstring strFileName = GetBetweenString(p+2, cStart, cEnd);

	if (!strFileName.empty())
	{
		if (!strText.empty())
		{
			_RichEdit_ReplaceSel(pRichEdit, strText.c_str()); 
			strText = _T("");
		}

		_RichEdit_InsertFace(pRichEdit, strFileName.c_str(), -1, -1);

		p = _tcsstr(p+2, _T("\"]"));
		p++;
		return TRUE;
	}
	return FALSE;
}


BOOL ChatDialog::HandleCustomPic_More(CRichEditUI * pRichEdit, LPCTSTR& p, tstring& strText, CFontInfo mfontInfo)
{
	CHAR *cStart = _T("[\"");
	CHAR *cEnd = _T("\"]");
	tstring strFileName = GetBetweenString(p+2, cStart, cEnd);

	if (!strFileName.empty())
	{
		if (!strText.empty())
		{
			_RichEdit_ReplaceSel_More(pRichEdit, strText.c_str(), mfontInfo); 
			strText = _T("");
		}

		_RichEdit_InsertFace(pRichEdit, strFileName.c_str(), -1, -1);

		p = _tcsstr(p+2, _T("\"]"));
		p++;
		return TRUE;
	}
	return FALSE;
}


// "/f["系统表情id"]/s["系统表情index"]/c["自定义图片路径"]"
void ChatDialog::AddMsg_More(CRichEditUI * pRichEdit, LPCTSTR lpText, CFontInfo mfontInfo)
{
	if (NULL == pRichEdit || NULL == lpText || NULL == *lpText)
		return;

	tstring strText;

	for (LPCTSTR p = lpText; *p != _T('\0'); p++)
	{
		if (*p == _T('/'))
		{
			if (*(p+1) == _T('/'))
			{
				strText += *p;
				p++;
				continue;
			}
			else if (*(p+1) == _T('f'))
			{
				if (HandleSysFaceId_More(pRichEdit, p, strText, mfontInfo))
					continue;
			}
			else if (*(p+1) == _T('s'))
			{
				if (HandleSysFaceIndex_More(pRichEdit, p, strText, mfontInfo))
					continue;
			}
			else if (*(p+1) == _T('c'))
			{
				if (HandleCustomPic_More(pRichEdit, p, strText, mfontInfo))
					continue;
			}
		}
		strText += *p;
	}

	if (!strText.empty())
		_RichEdit_ReplaceSel_More(pRichEdit, strText.c_str(), mfontInfo);
}

// "/f["系统表情id"]/s["系统表情index"]/c["自定义图片路径"]"
void ChatDialog::AddMsg(CRichEditUI * pRichEdit, LPCTSTR lpText)
{
	if (NULL == pRichEdit || NULL == lpText || NULL == *lpText)
		return;

	tstring strText;

	for (LPCTSTR p = lpText; *p != _T('\0'); p++)
	{
		if (*p == _T('/'))
		{
			if (*(p+1) == _T('/'))
			{
				strText += *p;
				p++;
				continue;
			}
			else if (*(p+1) == _T('f'))
			{
				if (HandleSysFaceId(pRichEdit, p, strText))
					continue;
			}
			else if (*(p+1) == _T('s'))
			{
				if (HandleSysFaceIndex(pRichEdit, p, strText))
					continue;
			}
			else if (*(p+1) == _T('c'))
			{
				if (HandleCustomPic(pRichEdit, p, strText))
					continue;
			}
		}
		strText += *p;
	}

	if (!strText.empty())
		_RichEdit_ReplaceSel(pRichEdit, strText.c_str());
}

void ChatDialog::AddMsgToSendEdit(LPCTSTR lpText)
{
	AddMsg(m_pSendEdit, lpText);
	m_pSendEdit->EndDown();
}


void ChatDialog::SetTextColor(DWORD dwColor)
{
	COptionUI* color_button = static_cast<COptionUI*>(m_PaintManager.FindControl(kColorButtonControlName));
	if (color_button != NULL)
	{
		color_button->Selected(false);
		if (dwColor != 0)
		{
			text_color_ = dwColor;
			FontStyleChanged();
		}
	}
}

void ChatDialog::FontStyleChanged()
{
	CRichEditUI* pRichEdit = static_cast<CRichEditUI*>(m_PaintManager.FindControl(kInputRichEditControlName));
	if( pRichEdit == NULL ) return;
	pRichEdit->SetFont(font_face_name_, font_size_, bold_, underline_, italic_);

	return;
}

void ChatDialog::AddMsgToRecvEdit(LPCTSTR lpText)
{
	if (NULL == lpText || NULL == *lpText)
		return;

	m_pRecvEdit->SetAutoURLDetect(true);

	tstring strTime;
	strTime = FormatTime(time(NULL), _T("%H:%M:%S"));

	ITextServices * pTextServices = m_pRecvEdit->GetTextServices();
	RichEdit_SetSel(pTextServices, -1, -1);

	TCHAR cText[2048] = {0};
	wsprintf(cText, _T("%s("), _T("王彭东"));
	RichEdit_ReplaceSel(pTextServices, cText, _T("宋体"), 9, RGB(0, 0, 255), FALSE, FALSE, FALSE, FALSE, 0);

	wsprintf(cText, _T("%u"), 23774312);
	RichEdit_ReplaceSel(pTextServices, cText, _T("宋体"), 9, RGB(0, 114, 193), FALSE, FALSE, TRUE, TRUE, 0);

	wsprintf(cText, _T(")  %s\r\n"), strTime.c_str());
	RichEdit_ReplaceSel(pTextServices, cText, _T("宋体"), 9, RGB(0, 0, 255), FALSE, FALSE, FALSE, FALSE, 0);

	AddMsg(m_pRecvEdit, lpText);

	RichEdit_ReplaceSel(pTextServices, _T("\r\n"));
	RichEdit_SetStartIndent(pTextServices, 0);
	m_pRecvEdit->EndDown();

	pTextServices->Release();
}


void ChatDialog::AddMsgToRecvEdit_More(LPCTSTR lpText, CFontInfo mfontInfo, bool bisselfmsg/* =false */, CDuiString sid/* ="" */, CDuiString sname/* ="" */, CDuiString strSendTime/* ="" */)
{
	if (NULL == lpText || NULL == *lpText)
		return;

	m_pRecvEdit->SetAutoURLDetect(true);

	SYSTEMTIME st = {0};
	GetLocalTime(&st);
	CDuiString strTime = "";
	strTime.Format("%04d-%02d-%02d %02d:%02d:%02d", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);

	ITextServices * pTextServices = m_pRecvEdit->GetTextServices();
	RichEdit_SetSel(pTextServices, -1, -1);

	TCHAR cText[2048] = {0};

	if (bisselfmsg)
	{
		sprintf(cText, _T("%s("), myselft_.nick_name);
		RichEdit_ReplaceSel(pTextServices, cText, _T("微软雅黑"), 9, RGB(0, 128, 64), FALSE, FALSE, FALSE, FALSE, 0);
		sprintf(cText, _T("%s"), myselft_.id);
		RichEdit_ReplaceSel(pTextServices, cText, _T("微软雅黑"), 9, RGB(0, 114, 193), FALSE, FALSE, TRUE, TRUE, 0);
		sprintf(cText, _T(") %s\r\n"), strTime.GetData());
		RichEdit_ReplaceSel(pTextServices, cText, _T("微软雅黑"), 9, RGB(0, 128, 64), FALSE, FALSE, FALSE, FALSE, 0);
		AddMsg(m_pRecvEdit, lpText);
	}
	else
	{
		sprintf(cText, _T("%s("), sname);
		RichEdit_ReplaceSel(pTextServices, cText, _T("微软雅黑"), 9, RGB(0, 0, 255), FALSE, FALSE, FALSE, FALSE, 0);
		sprintf(cText, _T("%s"), sid);
		RichEdit_ReplaceSel(pTextServices, cText, _T("微软雅黑"), 9, RGB(0, 114, 193), FALSE, FALSE, TRUE, TRUE, 0);
		sprintf(cText, _T(") %s\r\n"), strSendTime.GetData());
		RichEdit_ReplaceSel(pTextServices, cText, _T("微软雅黑"), 9, RGB(0, 0, 255), FALSE, FALSE, FALSE, FALSE, 0);
		AddMsg_More(m_pRecvEdit, lpText, mfontInfo);
	}

	RichEdit_ReplaceSel(pTextServices, _T("\r\n"));
	RichEdit_SetStartIndent(pTextServices, 0);
	m_pRecvEdit->EndDown();

	pTextServices->Release();
}

void ChatDialog::OnBtn_Send(TNotifyUI& msg)
{
	ITextServices * pTextServices = m_pSendEdit->GetTextServices();

	long lenth = m_pSendEdit->GetTextLength();
	tstring strText;
	RichEdit_GetText(pTextServices, strText);
	pTextServices->Release();

	if (strText.size() <= 0)
		return;

	m_pSendEdit->SetText(_T(""));
	m_pSendEdit->SetFocus();

	AddMsgToRecvEdit(strText.c_str());
}

void ChatDialog::SendMsg_More()
{
	if( m_pSendEdit == NULL ) return;
	ITextServices * pTextServices = m_pSendEdit->GetTextServices();

	long lenth = m_pSendEdit->GetTextLength();
	tstring strText;
	RichEdit_GetText(pTextServices, strText);
	pTextServices->Release();

	if (strText.size() <= 0)
		return;

	long mTextLen = m_pSendEdit->GetTextLength();
	if (mTextLen > MAX_TEXT_LEN)
	{
		AfxMessageBox("您输入的文本太长，无法发送！");
		return;
	}

	//群聊，批量途径发送
	if (bGroupSend_)
	{
		COMPLEX_MSG_DATA* pData = GetMsgData_More();	
		g_MyClient.m_MsgQueueSendMng.PushMsg(pData, eMsgFormat_Def, m_vtUPIMID, 0);
	}
	//个人聊天，原始途径发送
	else
	{
		COMPLEX_MSG_DATA* pData = GetMsgData_More();	
		g_MyClient.m_BigPackageSend.CreateMsg(friend_.id, pData);
	}

	m_pSendEdit->SetText(_T(""));
	m_pSendEdit->SetFocus();

	AddMsgToRecvEdit_More(strText.c_str(), m_fontInfo, true);
}

//发送消息，在这里处理
void ChatDialog::SendMsg()
{
	CRichEditUI* pRichEdit = static_cast<CRichEditUI*>(m_PaintManager.FindControl(kInputRichEditControlName));
	if( pRichEdit == NULL ) return;
	pRichEdit->SetFocus();
	CDuiString sText = pRichEdit->GetTextRange(0, pRichEdit->GetTextLength());
	if( sText.IsEmpty() ) return;

	long mTextLen = pRichEdit->GetTextLength();
	if (mTextLen > MAX_TEXT_LEN)
	{
		AfxMessageBox("您输入的文本太长，无法发送！");
		return;
	}

	//群聊，批量途径发送
	if (bGroupSend_)
	{
		g_MyClient.m_MsgQueueSendMng.PushMsg("sdsadasd", eMsgFormat_Txt, m_vtUPIMID, 0);
	}
	//个人聊天，原始途径发送
	else
	{
		COMPLEX_MSG_DATA* pData = GetMsgData();	
		g_MyClient.m_BigPackageSend.CreateMsg(friend_.id, pData);
	}
	pRichEdit->SetText(_T(""));

	//////////////////////////////////////////////////////////////////////////
	pRichEdit = static_cast<CRichEditUI*>(m_PaintManager.FindControl(kViewRichEditControlName));
	if( pRichEdit == NULL ) return;
	long lSelBegin = 0, lSelEnd = 0;

	
	lSelEnd = lSelBegin = pRichEdit->GetTextLength();
	pRichEdit->SetSel(lSelEnd, lSelEnd);
	SYSTEMTIME st = {0};
	GetLocalTime(&st);
	CDuiString strTime = "";
	strTime.Format("%s %04d-%02d-%02d %02d:%02d:%02d\n", myselft_.nick_name.GetData(), st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);
	pRichEdit->ReplaceSel(strTime, false);

	CHARFORMAT2 cf;
	ZeroMemory(&cf, sizeof(CHARFORMAT2));
	cf.cbSize = sizeof(cf);
	cf.dwReserved = 0;
	cf.dwMask = CFM_COLOR;
	cf.crTextColor = RGB(0, 128, 64);
	cf.dwEffects = 0;

	lSelEnd = pRichEdit->GetTextLength();
	pRichEdit->SetSel(lSelBegin, lSelEnd);
	pRichEdit->SetSelectionCharFormat(cf);

	PARAFORMAT2 pf;
	ZeroMemory(&pf, sizeof(PARAFORMAT2));
	pf.cbSize = sizeof(pf);
	pf.dwMask = PFM_STARTINDENT;
	pf.dxStartIndent = 0;
	pRichEdit->SetParaFormat(pf);

	lSelEnd = lSelBegin = pRichEdit->GetTextLength();

	pRichEdit->SetSel(-1, -1);
	pRichEdit->ReplaceSel(sText.GetData(), false);

	pRichEdit->SetSel(-1, -1);
	pRichEdit->ReplaceSel(_T("\n"), false);

	cf.crTextColor = RGB(0, 0, 0);
	lSelEnd = pRichEdit->GetTextLength();
	pRichEdit->SetSel(lSelBegin, lSelEnd);
	pRichEdit->SetSelectionCharFormat(cf);

	ZeroMemory(&pf, sizeof(PARAFORMAT2));
	pf.cbSize = sizeof(pf);
	pf.dwMask = PFM_STARTINDENT;
	pf.dxStartIndent = 220;
	pRichEdit->SetParaFormat(pf);

	pRichEdit->EndDown();

}

/*
* 用XML来存储复杂消息内容，包括，自定义的字体类型，文本，图片信息，表情等
*/
COMPLEX_MSG_DATA* ChatDialog::GetMsgData(BOOL bOnLine/* =TRUE */,CString strFileInfo/* ="" */,CString strProductName/* ="" */)
{
	int m_nErrorCode = 0;
	CRichEditUI* pRichEdit = static_cast<CRichEditUI*>(m_PaintManager.FindControl(kInputRichEditControlName));
	if( pRichEdit == NULL ) return NULL;
	pRichEdit->SetFocus();

	CDuiString sText = pRichEdit->GetTextRange(0, pRichEdit->GetTextLength());
	if( sText.IsEmpty() ) return NULL;
	long nLen = sText.GetLength();

	if(nLen<=0) 
	{
		m_nErrorCode = eMsgData_Null;
		return NULL;
	}
	if(nLen>=MAX_TEXT_LEN)
	{
		m_nErrorCode = eMsgData_TextMax;
		return NULL;
	}

	COMPLEX_MSG_DATA* pData = new COMPLEX_MSG_DATA;
	if(!pData) return NULL;
	memset(pData,0,sizeof(COMPLEX_MSG_DATA));

	TiXmlDocument xmlDoc;
	TiXmlElement rootEle("msg");
	TiXmlNode* pNode = xmlDoc.InsertEndChild(rootEle);
	TiXmlElement* pRootElm = pNode->ToElement();

	TiXmlElement textElm("text");
	TiXmlText xmlText(sText.GetData());
	xmlText.SetCDATA(true);
	TiXmlNode *pTextNode = pRootElm->InsertEndChild(textElm);
	pTextNode->InsertEndChild(xmlText);
	TiXmlElement iamgeElm("image");
	TiXmlElement* pChildeElm = NULL;

	TiXmlPrinter printer;
	printer.SetIndent("");
	printer.SetLineBreak("");
	xmlDoc.Accept( &printer );
	LPCTSTR pBuf = printer.CStr();
	nLen = printer.Size();
	if(nLen <= 0) 
	{
		m_nErrorCode = eMsgData_Xml;
		DeleteMsgData(pData);
		return NULL;
	}
	if(nLen >= MAX_TEXT_LEN)
	{
		m_nErrorCode = eMsgData_TextMax;
		DeleteMsgData(pData);
		return NULL;
	}
	pData->pBuf = new char[nLen+1];
	strncpy(pData->pBuf, pBuf, nLen);
	pData->pBuf[nLen] = 0;
	pData->nTextLen = nLen;
	pData->sSend = -1;

	return pData;
}

/*
* 用XML来存储复杂消息内容，包括，自定义的字体类型，文本，图片信息，表情等
*/
COMPLEX_MSG_DATA* ChatDialog::GetMsgData_More(BOOL bOnLine/* =TRUE */,CString strFileInfo/* ="" */,CString strProductName/* ="" */)
{
	int m_nErrorCode = 0;

	if( m_pSendEdit == NULL ) return NULL;
	ITextServices * pTextServices = m_pSendEdit->GetTextServices();
	tstring strText;
	RichEdit_GetText(pTextServices, strText);
	pTextServices->Release();

	CDuiString sText = strText.c_str();
	if( sText.IsEmpty() ) return NULL;
	long nLen = sText.GetLength();

	if(nLen<=0) 
	{
		m_nErrorCode = eMsgData_Null;
		return NULL;
	}
	if(nLen>=MAX_TEXT_LEN)
	{
		m_nErrorCode = eMsgData_TextMax;
		return NULL;
	}

	COMPLEX_MSG_DATA* pData = new COMPLEX_MSG_DATA;
	if(!pData) return NULL;
	memset(pData,0,sizeof(COMPLEX_MSG_DATA));

	TiXmlDocument xmlDoc;
	TiXmlElement rootEle("msg");
	TiXmlNode* pNode = xmlDoc.InsertEndChild(rootEle);
	TiXmlElement* pRootElm = pNode->ToElement();

	//////////////////////////////////////////////////////////////////////////
	pRootElm->SetAttribute("f", m_fontInfo.m_strName.c_str());		// 字体
	pRootElm->SetAttribute("b", m_fontInfo.m_bBold);				// 粗细
	pRootElm->SetAttribute("i", m_fontInfo.m_bItalic);				// 斜体
	pRootElm->SetAttribute("u", m_fontInfo.m_bUnderLine);			// 下划线
	pRootElm->SetAttribute("s", m_fontInfo.m_nSize);				// 字体大小
	CString strTmpFmt = "";
	strTmpFmt.Format("%i", m_fontInfo.m_clrText);
	pRootElm->SetAttribute("c",strTmpFmt);							// 颜色
	//////////////////////////////////////////////////////////////////////////
	TiXmlElement textElm("text");
	TiXmlText xmlText(sText.GetData());
	xmlText.SetCDATA(true);
	TiXmlNode *pTextNode = pRootElm->InsertEndChild(textElm);
	pTextNode->InsertEndChild(xmlText);
	TiXmlElement iamgeElm("image");
	TiXmlElement* pChildeElm = NULL;

	TiXmlPrinter printer;
	printer.SetIndent("");
	printer.SetLineBreak("");
	xmlDoc.Accept( &printer );
	LPCTSTR pBuf = printer.CStr();
	nLen = printer.Size();
	if(nLen <= 0) 
	{
		m_nErrorCode = eMsgData_Xml;
		DeleteMsgData(pData);
		return NULL;
	}
	if(nLen >= MAX_TEXT_LEN)
	{
		m_nErrorCode = eMsgData_TextMax;
		DeleteMsgData(pData);
		return NULL;
	}
	pData->pBuf = new char[nLen+1];
	strncpy(pData->pBuf, pBuf, nLen);
	pData->pBuf[nLen] = 0;
	pData->nTextLen = nLen;
	pData->sSend = -1;

	return pData;
}


void ChatDialog::DeleteMsgData(COMPLEX_MSG_DATA*& pMsgData)
{
	if(!pMsgData) return;
	if(pMsgData->pObjectInfo)
	{
		for(int j=0; j<pMsgData->nObjCount; j++)
			TDEL(pMsgData->pObjectInfo[j].pBuf);
		TDELARRAY(pMsgData->pObjectInfo);
	}
	TDEL(pMsgData->pBuf);
	TDEL(pMsgData);
}

BOOL ChatDialog::ReadXmlMsgData(LPCSTR pszBuf, CRichEditUI* pRichEdit)
{
	TiXmlDocument xmlDoc;
	xmlDoc.Parse(pszBuf);

	if(xmlDoc.Error()) 
	{
		printf( "Error in %s: %s\n", xmlDoc.Value(), xmlDoc.ErrorDesc() );
		return FALSE;
	}

	CString strTmpFmt,strOld;
	TiXmlNode* pRootNode = xmlDoc.FirstChild("msg");
	if(!pRootNode) 	return FALSE;
	TiXmlElement* pRootElm =pRootNode->ToElement();
	if(!pRootElm) 	return FALSE;
	TiXmlNode* pItemNode = pRootNode->FirstChild("text"); 
	if(!pItemNode) return FALSE;
	TiXmlNode* pTextNode = pItemNode->FirstChild();
	if(!pTextNode) return FALSE;

	CString strText = pTextNode->Value();
	strText += "\r\n";

	//////////////////////////////////////////////////////////////////////////
	PARAFORMAT2 pf;
	ZeroMemory(&pf, sizeof(PARAFORMAT2));
	pf.cbSize = sizeof(pf);
	pf.dwMask = PFM_STARTINDENT;
	pf.dxStartIndent = 0;
	pRichEdit->SetParaFormat(pf);

	long lSelBegin = 0, lSelEnd = 0;
	lSelEnd = lSelBegin = pRichEdit->GetTextLength();

	pRichEdit->SetSel(-1, -1);
	pRichEdit->ReplaceSel(strText, false);
	pRichEdit->SetSel(-1, -1);

	CHARFORMAT2 cf;
	ZeroMemory(&cf, sizeof(CHARFORMAT2));
	cf.cbSize = sizeof(cf);
	cf.dwReserved = 0;
	cf.dwMask = CFM_COLOR;
	cf.crTextColor = RGB(0, 0, 0);
	cf.dwEffects = 0;

	lSelEnd = pRichEdit->GetTextLength();
	pRichEdit->SetSel(lSelBegin, lSelEnd);
	pRichEdit->SetSelectionCharFormat(cf);

	ZeroMemory(&pf, sizeof(PARAFORMAT2));
	pf.cbSize = sizeof(pf);
	pf.dwMask = PFM_STARTINDENT;
	pf.dxStartIndent = 220;
	pRichEdit->SetParaFormat(pf);

	return TRUE;
}

void ChatDialog::InsertChatHistory_More(RECEIVED_MSG *pMsg, LPCTSTR szSendName, LPCTSTR szSendId)
{
	CDuiString strTime = "";
	CDuiString strspName = "";
	strTime.Format("%04d-%02d-%02d %02d:%02d:%02d", 
		pMsg->senddate/10000, (pMsg->senddate%10000)/100, pMsg->senddate%100, pMsg->sendtime/10000, (pMsg->sendtime%10000)/100, pMsg->sendtime%100);

	if (pMsg->format == eMsgFormat_System)
		strspName = _T("系统消息");
	else
		strspName = szSendName;

	if (pMsg->format == eMsgFormat_Txt || pMsg->format == eMsgFormat_AutoReply)
	{
		AddMsgToRecvEdit_More(pMsg->msg, m_fontInfo, false, szSendId, strspName, strTime);
	}
	if (pMsg->format == eMsgFormat_Def)
	{
		TiXmlDocument xmlDoc;
		xmlDoc.Parse(pMsg->msg);

		if(xmlDoc.Error()) 
		{
			printf( "Error in %s: %s\n", xmlDoc.Value(), xmlDoc.ErrorDesc() );
			return;
		}

		CString strTmpFmt,strOld;
		TiXmlNode* pRootNode = xmlDoc.FirstChild("msg");
		if(!pRootNode) 	return;
		TiXmlElement* pRootElm =pRootNode->ToElement();
		if(!pRootElm) 	return;

		//////////////////////////////////////////////////////////////////////////
		CFontInfo m_ufontInfo;
		LPCSTR pszValue = "";
		pszValue = pRootElm->Attribute("f");				// 字体
		m_ufontInfo.m_strName = pszValue;
		pszValue = pRootElm->Attribute("b");				// 粗细
		m_ufontInfo.m_bBold = atoi(pszValue);
		pszValue = pRootElm->Attribute("i");				// 斜体
		m_ufontInfo.m_bItalic = atoi(pszValue);
		pszValue = pRootElm->Attribute("u");				// 下划线
		m_ufontInfo.m_bUnderLine = atoi(pszValue);
		pszValue = pRootElm->Attribute("s");				// 字体大小
		m_ufontInfo.m_nSize = atoi(pszValue);
		pszValue = pRootElm->Attribute("c");				// 颜色
		m_ufontInfo.m_clrText = atoi(pszValue);

		//////////////////////////////////////////////////////////////////////////

		TiXmlNode* pItemNode = pRootNode->FirstChild("text"); 
		if(!pItemNode) return;
		TiXmlNode* pTextNode = pItemNode->FirstChild();
		if(!pTextNode) return;

		CString strText = pTextNode->Value();
		AddMsgToRecvEdit_More(strText, m_ufontInfo, false, szSendId, strspName, strTime);
	}
}

void ChatDialog::InsertChatHistory(RECEIVED_MSG *pMsg, LPCTSTR szSendName)
{
	CRichEditUI* pRichEdit = static_cast<CRichEditUI*>(m_PaintManager.FindControl(kViewRichEditControlName));
	if( pRichEdit == NULL ) return;
	long lSelBegin = 0, lSelEnd = 0;

	SYSTEMTIME st = {0};
	GetLocalTime(&st);
	lSelEnd = lSelBegin = pRichEdit->GetTextLength();
	pRichEdit->SetSel(lSelEnd, lSelEnd);

	CDuiString strTime = "";
	CDuiString strTimeAndName = "";
	strTime.Format("%04d-%02d-%02d %02d:%02d:%02d\n", 
		pMsg->senddate/10000, (pMsg->senddate%10000)/100, pMsg->senddate%100, pMsg->sendtime/10000, (pMsg->sendtime%10000)/100, pMsg->sendtime%100);
	if (pMsg->format == eMsgFormat_System)
		strTimeAndName.Format("系统消息 %s",strTime);
	else
		strTimeAndName.Format("%s %s", szSendName, strTime);

	pRichEdit->ReplaceSel(strTimeAndName, false);

	CHARFORMAT2 cf;
	ZeroMemory(&cf, sizeof(CHARFORMAT2));
	cf.cbSize = sizeof(cf);
	cf.dwReserved = 0;
	cf.dwMask = CFM_COLOR;
	cf.crTextColor = RGB(0, 0, 255);
	cf.dwEffects = 0;

	lSelEnd = pRichEdit->GetTextLength();
	pRichEdit->SetSel(lSelBegin, lSelEnd);
	pRichEdit->SetSelectionCharFormat(cf);
	pRichEdit->SetSel(-1, -1);

	if (pMsg->format == eMsgFormat_Txt || pMsg->format == eMsgFormat_AutoReply)
	{
		CString strMsg = "";
		strMsg.Format("%s\n",pMsg->msg);
		pRichEdit->ReplaceSel(strMsg, false);
	}
	if (pMsg->format == eMsgFormat_Def)
	{
		ReadXmlMsgData(pMsg->msg, pRichEdit);
	}

	pRichEdit->EndDown();
}