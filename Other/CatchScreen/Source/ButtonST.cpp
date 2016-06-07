/////////////////////////////////////////////////////////////////////////////
/**
* 文件名  ：ButtonST.cpp
* 版本号  : V1.0
* 项目名称：CPH
* 版权声明：Copyright 2002-2003 . All Rights Reserved.
* 模块简单描述：图标(位图)按纽控件类
*======================================================
* 版本变更记录:
*
*   v1.0  2001	              Davide Calabro'		davide_calabro@yahoo.com
*   v1.1  2002-10-30   20:30  罗伟  
*
* 
*/

/////////////////////////////////////////////////////////////////////////////



#include "stdafx.h"
//#include "..\PaintDlg.h"
#include "..\\INCLUDE\\ButtonST.h"
#include "..\\INCLUDE\\DefineMessage.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//应用程序运行路径,如"c:\cph\"
extern  CString g_strAppPath;        
extern  IniPaintSet g_stuPaintSet;

/////////////////////////////////////////////////////////////////////////////
/*
*--------------------------------------------------------------------------------
*  成员函数名   :CBmpPalette
*  功能描述     :
*  输入参数描述	:
*  返回变量说明 :
*  算法描述		:
*  附加说明     : 
*--------------------------------------------------------------------------------
*/
CButtonST::CButtonST()
{
	m_bMouseOnButton = FALSE;

	FreeResources(FALSE);

	// 默认为“平面”类型按纽
	m_bIsFlat = TRUE; 
  
	// 默认为“不画按纽边框”类型按纽
	m_bDrawBorder = FALSE; 
  
	// 默认图标同文字标签在同一行上
	m_nAlign = ST_ALIGN_HORIZ; 
  
	// 默认显示文本标签
	m_bShowText = TRUE; 
  
	//默认为平面按纽时,不画焦点矩形边框线
	m_bDrawFlatFocus = FALSE;

	//默认按纽不是窗体上的默认按纽
	m_bIsDefault = FALSE;

	//按纽不是CheckBox类型
	m_bIsCheckBox = FALSE;
	m_nCheck = 0;

	// 按纽默认颜色
	SetDefaultColors(FALSE);

	// 按纽工具提示为NULL
	m_ToolTip.m_hWnd = NULL;

	//默认按纽为不透明的
	m_bDrawTransparent =TRUE;
	m_pbmpOldBk = NULL;

	// 无 URL被定义
	::ZeroMemory(&m_szURL, sizeof(m_szURL));

	// 按纽上光标 cursor没有 定义
	m_hCursor = NULL;	// 

	//不自动重复消息
	m_bAutoRepeat = FALSE;
	m_hWndAutoRepeat = NULL;
	m_nMsgAutoRepeat = WM_APP;
	m_dwPeriodAutoRepeat = 100;

   //默认为单击消息
    m_downmessage = LCLICK;

	//默认按纽窗体作用点在客户区
	m_HitTestStyle = HTCLIENT;

	m_isFirst = TRUE;
	m_nIndex = -1;


} // End of CButtonST

/*
*--------------------------------------------------------------------------------
*  成员函数名   :~CButtonST()
*  功能描述     :析构函数: 释放句柄
*  输入参数描述	:
*  返回变量说明 :
*  算法描述		:
*  附加说明     : 
*--------------------------------------------------------------------------------
*/
CButtonST::~CButtonST()
{
	// 恢复旧位图 
	if (m_dcBk.m_hDC != NULL && m_pbmpOldBk != NULL)
	{
		m_dcBk.SelectObject(m_pbmpOldBk);
	}

	FreeResources();

	// 释放光标句柄
	if (m_hCursor != NULL) ::DestroyCursor(m_hCursor);
} // End of ~CButtonST


BEGIN_MESSAGE_MAP(CButtonST, CButton)
    //{{AFX_MSG_MAP(CButtonST)
	ON_WM_CAPTURECHANGED()
	ON_WM_SETCURSOR()
	ON_WM_KILLFOCUS()
	ON_WM_MOUSEMOVE()
	ON_WM_SYSCOLORCHANGE()
	ON_CONTROL_REFLECT_EX(BN_CLICKED, OnClicked)
	ON_WM_ACTIVATE()
	ON_WM_ENABLE()
	ON_WM_CANCELMODE()
	ON_WM_LBUTTONDOWN()
	ON_WM_NCHITTEST()
	ON_WM_SIZE()
	ON_WM_MOVING()
	ON_WM_TIMER()
	ON_WM_NCLBUTTONDBLCLK()
	ON_WM_RBUTTONDOWN()
	ON_WM_CTLCOLOR_REFLECT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/*
*--------------------------------------------------------------------------------
*  成员函数名   :SetIcon
*  功能描述     :为按纽设置两态图标
*  输入参数描述	:
*int nIconInId ：鼠标掠过或压下时按纽显示的图标的资源ID号
*int nIconOutId：鼠标离开时常态按纽显示的图标的资源ID号
*  返回变量说明 :
*  算法描述		:
*  附加说明     : 
*--------------------------------------------------------------------------------
*/
DWORD CButtonST::SetIcon(int nIconInId, int nIconOutId)
{
	HICON		hIconIn;
	HICON		hIconOut;
	HINSTANCE	hInstResource;

	// 查找图标资源
	hInstResource = AfxFindResourceHandle(MAKEINTRESOURCE(nIconInId), RT_GROUP_ICON);

	// 设置当掠过或压下时按纽显示的图标
	hIconIn = (HICON)::LoadImage(hInstResource, MAKEINTRESOURCE(nIconInId), IMAGE_ICON, 0, 0, 0);
  	//设置鼠标离开时常态按纽显示的图标
	hIconOut = (HICON)::LoadImage(hInstResource, MAKEINTRESOURCE(nIconOutId), IMAGE_ICON, 0, 0, 0);

	return SetIcon(hIconIn, hIconOut);
} // End of SetIcon


/*
*--------------------------------------------------------------------------------
*  成员函数名   :SetIcon
*  功能描述     :为按纽设置两态图标
*  输入参数描述	:
*HICON nIconInId ：鼠标掠过或压下时按纽显示的图标句柄
*HICON nIconOutId：鼠标离开时常态按纽显示的图标句柄
*  返回变量说明 :
*  算法描述		:
*  附加说明     : 
*--------------------------------------------------------------------------------
*/
DWORD CButtonST::SetIcon(HICON hIconIn, HICON hIconOut)
{
	BOOL		bRetValue;
	ICONINFO	ii;

	//释放所有导入的资源
	FreeResources();

	if (hIconIn != NULL)
	{
		m_csIcons[0].hIcon = hIconIn;

		//得到图标尺寸
		ZeroMemory(&ii, sizeof(ICONINFO));
		bRetValue = ::GetIconInfo(hIconIn, &ii);
		if (bRetValue == FALSE)
		{
			FreeResources();
			return BTNST_INVALIDRESOURCE;
		} // end if

		m_csIcons[0].dwWidth	= (DWORD)(ii.xHotspot * 2);
		m_csIcons[0].dwHeight	= (DWORD)(ii.yHotspot * 2);
		::DeleteObject(ii.hbmMask);
		::DeleteObject(ii.hbmColor);

		if (hIconOut != NULL)
		{
			m_csIcons[1].hIcon = hIconOut;

			// 得到图标尺寸
			ZeroMemory(&ii, sizeof(ICONINFO));
			bRetValue = ::GetIconInfo(hIconOut, &ii);
			if (bRetValue == FALSE)
			{
				FreeResources();
				return BTNST_INVALIDRESOURCE;
			} //end if

			m_csIcons[1].dwWidth	= (DWORD)(ii.xHotspot * 2);
			m_csIcons[1].dwHeight	= (DWORD)(ii.yHotspot * 2);
			::DeleteObject(ii.hbmMask);
			::DeleteObject(ii.hbmColor);
		} //end  if
	} // end if

	RedrawWindow();

	return BTNST_OK;
} // End of SetIcon


/*
*--------------------------------------------------------------------------------
*  成员函数名   :SetBtnCursor
*  功能描述     :当鼠标经过按纽时为按纽设置光标样式
*  输入参数描述	:
*int nCursorId  :光标资源的ID号
*  返回变量说明 :返回TRUE表不成功设置。
*  算法描述		:
*  附加说明     : 
*--------------------------------------------------------------------------------
*/
BOOL CButtonST::SetBtnCursor(int nCursorId)
{
	HINSTANCE hInstResource;
	// 释放以前的光标
	if (m_hCursor != NULL) ::DestroyCursor(m_hCursor);
	m_hCursor = NULL;

	if (nCursorId != NULL)
	{
		hInstResource = AfxFindResourceHandle(MAKEINTRESOURCE(nCursorId),
											RT_GROUP_CURSOR);
		// 导入ICON资源
		m_hCursor = (HCURSOR)::LoadImage(hInstResource/*AfxGetApp()->m_hInstance*/, MAKEINTRESOURCE(nCursorId), IMAGE_CURSOR, 0, 0, 0);
		//若出错返回FALSE
		if (m_hCursor == NULL) return FALSE;
	}

	return TRUE;
} // End of SetBtnCursor


/*
*--------------------------------------------------------------------------------
*  成员函数名   :SetFlat
*  功能描述     :设置按纽是否为平面类型
*  输入参数描述	:
*   BOOL bState：为TRUE表示要设置按纽为平面类型
*  返回变量说明 :
*  算法描述		:
*  附加说明     : 
*--------------------------------------------------------------------------------
*/
void CButtonST::SetFlat(BOOL bState)
{
	m_bIsFlat = bState;
	Invalidate();
} // End of SetFlat

/*
*--------------------------------------------------------------------------------
*  成员函数名   :GetFlat
*  功能描述     :得到按纽是否是平面类型
*  输入参数描述	:
*  返回变量说明 :返回TRUE表示按纽为平面类型
*  算法描述		:
*  附加说明     : 
*--------------------------------------------------------------------------------
*/
BOOL CButtonST::GetFlat()
{
	return m_bIsFlat;
} // End of GetFlat



/*
*--------------------------------------------------------------------------------
*  成员函数名   :SetAlign
*  功能描述     :设置按纽上文本同图标的对齐方式
*  输入参数描述	:
*    int nAlign ：对齐方式
*  返回变量说明 :
*  算法描述		:
*  附加说明     : 
*--------------------------------------------------------------------------------
*/
void CButtonST::SetAlign(int nAlign)
{
	switch (nAlign)
	{    
		case ST_ALIGN_HORIZ:
		case ST_ALIGN_HORIZ_RIGHT:
		case ST_ALIGN_VERT:
			m_nAlign = nAlign;
			break;
	}
	Invalidate();
} // End of SetAlign


/*
*--------------------------------------------------------------------------------
*  成员函数名   :GetAlign()
*  功能描述     :得到按纽上文本同图标的对齐方式
*  输入参数描述	:
*  返回变量说明 :
*  算法描述		:
*  附加说明     : 
*--------------------------------------------------------------------------------
*/
int CButtonST::GetAlign()
{
	return m_nAlign;
} // End of GetAlign


/*
*--------------------------------------------------------------------------------
*  成员函数名   :DrawBorder
*  功能描述     :设置按纽得到焦点时是否画一边框
*  输入参数描述	:
*  BOOL bEnable   ：为TRUE表示按纽要设置焦点边框
*  返回变量说明 :
*  算法描述		:
*  附加说明     : 
*--------------------------------------------------------------------------------
*/
void CButtonST::DrawBorder(BOOL bEnable)
{
	m_bDrawBorder = bEnable;
} // End of DrawBorder

/*
*--------------------------------------------------------------------------------
*  成员函数名   :SetShowText
*  功能描述     :设置图标按纽是否显示文本标签
*  输入参数描述	:
*  BOOL bShow     ：为TRUE表示按纽图标与文本标签都显示
*  返回变量说明 :
*  算法描述		:
*  附加说明     : 
*--------------------------------------------------------------------------------
*/
void CButtonST::SetShowText(BOOL bShow)
{
	m_bShowText = bShow;
	Invalidate();
} // End of SetShowText


/*
*--------------------------------------------------------------------------------
*  成员函数名   :GetShowText()
*  功能描述     :得到图标按纽是否显示文本标签
*  输入参数描述	:
*  返回变量说明 :
*  返回TRUE表示按纽图标与文本标签都显示
*  算法描述		:
*  附加说明     : 
*--------------------------------------------------------------------------------
*/
BOOL CButtonST::GetShowText()
{
	return m_bShowText;
} // End of GetShowText


/*
*--------------------------------------------------------------------------------
*  成员函数名   :OnMouseMove
*  功能描述     :鼠标在按纽表面移动时
*  输入参数描述	:
*   UINT nFlags: 其它不同的按键压下情况
*   CPoint point:光标在窗体上的当前坐标点
*  返回变量说明 :
*  算法描述		:
*  附加说明     : 
*--------------------------------------------------------------------------------
*/
void CButtonST::OnMouseMove(UINT nFlags, CPoint point)
{	
	//按纽窗体指针
	CWnd* pWnd;  
    //父窗体指针
	CWnd* pParent;

	CButton::OnMouseMove(nFlags, point);
    
   
	// 若鼠标掠过按纽表面并左键压下不作任何处理
	if (nFlags & MK_LBUTTON && m_bMouseOnButton == FALSE) return;

	CString strIndex;
	strIndex.Format("%d",m_nIndex); 
	//CCPHDlg *pParentWnd = (CCPHDlg *)GetParent(); 
	//pParentWnd->m_showHelp.ShowWindow(SW_SHOW);

	//如果第一次则创建一个动态提示窗体
	if(m_isFirst == TRUE && m_nIndex != -1)
	{
		m_nCount = 0;
		m_isFirst = FALSE;
		m_mousePoint = point;
		SetTimer(1,1000,NULL);
	}

	// 或按纽是平面类型返回
	if (m_bIsFlat == FALSE) return;

	pWnd = GetActiveWindow();
	pParent = GetOwner();

	if ((GetCapture() != this) && (
		 #ifndef ST_LIKEIE
			pWnd != NULL && 
		 #endif
		pParent != NULL)) 
	{
		m_bMouseOnButton = TRUE;
		//SetFocus();	
		SetCapture();
		Invalidate();
	}
	else
	{
		POINT p2 = point;
		ClientToScreen(&p2);
		CWnd* wndUnderMouse = WindowFromPoint(p2);
//		if (wndUnderMouse != this)
		if (wndUnderMouse && wndUnderMouse->m_hWnd != this->m_hWnd)
		{
			//当鼠标离开时重画按纽
			if (m_bMouseOnButton == TRUE)
			{

				//鼠标从按纽上移开时关闭动态提示框
				m_isFirst = TRUE;
				if(m_nIndex != -1 && m_nCount != -1)
                {
					if(pParentWnd->GetTextStyle() == TRUE)
						pParentWnd->m_showHelp.Show(FALSE, m_nIndex);
					if(pParentWnd->GetVoiceStyle() == TRUE)
						StopPlayTipSound();
					m_nCount = -1;
					KillTimer(1);
				}
				m_bMouseOnButton = FALSE;
				Invalidate();
			}
			//若没有压下鼠标左键释放!
			if (!(nFlags & MK_LBUTTON)) ReleaseCapture();
		}


	}
} // End of OnMouseMove


/*
*--------------------------------------------------------------------------------
*  成员函数名   :OnKillFocus
*  功能描述     :按纽失去焦点时
*  输入参数描述	:
*  CWnd * pNewWnd：按纽控件窗体指针
*  返回变量说明 :
*  算法描述		:
*  附加说明     : 
*--------------------------------------------------------------------------------
*/
void CButtonST::OnKillFocus(CWnd * pNewWnd)
{
	CButton::OnKillFocus(pNewWnd);

	CancelHover();
} // End of OnKillFocus

/*
*--------------------------------------------------------------------------------
*  成员函数名   :OnLButtonDown
*  功能描述     :鼠标在按纽上压下时
*  输入参数描述	:
*   UINT nFlags: 其它不同的按键压下情况
*   CPoint point:光标在窗体上的当前坐标点
*  返回变量说明 :
*  算法描述		:
*  附加说明     : 
*--------------------------------------------------------------------------------
*/
void CButtonST::OnLButtonDown(UINT nFlags, CPoint point) 
{

CButton::OnLButtonDown(nFlags, point);

	MSG		csMsg;
	int		nButtonID;
	HWND	hWndParent;
	nButtonID = GetDlgCtrlID();
	hWndParent = GetParent()->GetSafeHwnd();

	//关闭动态提示
	if(m_nIndex != -1 && m_nCount != -1)
    {
    	CCPHDlg *pParentWnd = (CCPHDlg *)GetParent(); 
        if(pParentWnd->GetTextStyle() == TRUE)
		    pParentWnd->m_showHelp.Show(FALSE, m_nIndex);
		if(pParentWnd->GetVoiceStyle() == TRUE)
			StopPlayTipSound();
		m_nCount = -1;
		KillTimer(1);
	}

    switch(m_downmessage)
	{
	case LCLICK:
		{			
			if (m_bAutoRepeat == TRUE)
			{
				while(PeekMessage(&csMsg, m_hWnd, WM_LBUTTONUP, WM_LBUTTONUP, PM_REMOVE) == FALSE)
				{
					::SendMessage(hWndParent, WM_COMMAND, MAKEWPARAM((WORD)nButtonID, BN_CLICKED), (LPARAM)m_hWnd);
					::Sleep(m_dwPeriodAutoRepeat);
				}
				SendMessage(WM_LBUTTONUP);
			} // if
		}
	case LDOWN:		
		::SendMessage(hWndParent, WM_OWNDOWNMESSAGE, MAKEWPARAM((WORD)nButtonID, 0), (LPARAM)m_hWnd);
		break;
		//显示上拉菜单
    case LPOPMENU:
			::SendMessage(hWndParent,UWM_RBUTTONDOWN,(long)&point,GetDlgCtrlID());
			break;
	}

} // End of OnLButtonDown


/*
*--------------------------------------------------------------------------------
*  成员函数名   :OnActivate
*  功能描述     :按纽为活动状态时
*  输入参数描述	:
*  UINT nState:
*  CWnd* pWndOther:
*  BOOL bMinimized:
*  返回变量说明 :
*  算法描述		:
*  附加说明     : 
*--------------------------------------------------------------------------------
*/
void CButtonST::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized) 
{
	CButton::OnActivate(nState, pWndOther, bMinimized);
	
	if (nState == WA_INACTIVE)	CancelHover();
} // End of OnActivate

/*
*--------------------------------------------------------------------------------
*  成员函数名   :OnEnable
*  功能描述     :设置按纽有效
*  输入参数描述	:
*   BOOL bEnable：为TRUE时表示按纽有效，可以接收消息
*  返回变量说明 :
*  算法描述		:
*  附加说明     : 
*--------------------------------------------------------------------------------
*/
void CButtonST::OnEnable(BOOL bEnable) 
{
	CButton::OnEnable(bEnable);
	
	if (bEnable == FALSE)	CancelHover();
} // End of OnEnable

/*
*--------------------------------------------------------------------------------
*  成员函数名   :OnCancelMode() 
*  功能描述     :设置按纽为鼠标捕获CancelMode模式
*  输入参数描述	:
*  返回变量说明 :
*  算法描述		:
*  附加说明     : 
*--------------------------------------------------------------------------------
*/
void CButtonST::OnCancelMode() 
{
	CButton::OnCancelMode();
		
	CancelHover();
} // End of OnCancelMode


/*
*--------------------------------------------------------------------------------
*  成员函数名   :OnCaptureChanged
*  功能描述     :鼠标捕获CancelMode模式失效
*  输入参数描述	:
*  CWnd *pWnd   ：鼠标捕获CancelMode模式指向的窗口指针
*  返回变量说明 :
*  算法描述		:
*  附加说明     : 
*--------------------------------------------------------------------------------
*/
void CButtonST::OnCaptureChanged(CWnd *pWnd) 
{
	if (m_bMouseOnButton == TRUE)
	{
		ReleaseCapture();
		Invalidate();
	}

	CButton::OnCaptureChanged(pWnd);
} // End of OnCaptureChanged


/*
*--------------------------------------------------------------------------------
*  成员函数名   :DrawItem
*  功能描述     :自画按纽窗体
*  输入参数描述	:
*  LPDRAWITEMSTRUCT lpDIS：DRAWITEMSTRUCT 结构体指明自画按纽的类型
*  返回变量说明 :
*  算法描述		:
*  附加说明     : 
*--------------------------------------------------------------------------------
*/
void CButtonST::DrawItem(LPDRAWITEMSTRUCT lpDIS)
{
	CDC* pDC = CDC::FromHandle(lpDIS->hDC);

	CPen *pOldPen;
	BOOL	bIsPressed;

	// 是Checkbox或Radiobutton类型 ?
	if (m_bIsCheckBox == TRUE)
	{
		bIsPressed  =  (lpDIS->itemState & ODS_SELECTED)
					|| (m_nCheck != 0);
		//bIsPressed = TRUE;
	}
	// 常规按纽或其它类型按纽类型 ...
	else
	{
		bIsPressed = (lpDIS->itemState & ODS_SELECTED);
	}

//	BOOL bIsPressed  = (lpDIS->itemState & ODS_SELECTED);
	BOOL bIsFocused  = (lpDIS->itemState & ODS_FOCUS);
	BOOL bIsDisabled = (lpDIS->itemState & ODS_DISABLED);

	CRect itemRect = lpDIS->rcItem;

	pDC->SetBkMode(TRANSPARENT);

	if (m_bIsFlat == FALSE)
	{
		if (bIsFocused || (GetDefault() == TRUE))
		{
			//此语句造成黑色成为透明色，暂改为
			//原CBrush br(RGB(0,0,0));  
			CBrush br(RGB(0,0,0));
			pDC->FrameRect(&itemRect, &br);
			itemRect.DeflateRect(1, 1);
		}
	}

	// 准备画... 用背景色画按纽面
	COLORREF bgColor;
	if ((m_bMouseOnButton == TRUE) || (bIsPressed))
		bgColor = m_crColors[BTNST_COLOR_BK_IN]; 
	else
		bgColor = m_crColors[BTNST_COLOR_BK_OUT]; 

	CBrush br(bgColor);
	// 按纽是否透明?
	if (m_bDrawTransparent == TRUE)
	{
		PaintBk(pDC);
	}
	else
	{
		pDC->FillRect(&itemRect, &br);
	}

	//CBrush br(GetSysColor(COLOR_BTNFACE));  
	//pDC->FillRect(&itemRect, &br);

	// 画下压式按纽
	if (bIsPressed)
	{
		if (m_bIsFlat == TRUE)
		{
			if (m_bDrawBorder == TRUE)
			{
				pDC->Draw3dRect(itemRect, ::GetSysColor(COLOR_BTNSHADOW), ::GetSysColor(COLOR_BTNHILIGHT));
			}
		}
		else    
		{
			CBrush brBtnShadow(GetSysColor(COLOR_BTNSHADOW));
			pDC->FrameRect(&itemRect, &brBtnShadow);
		}
	}
	else // ...else画非下压式按纽
	{
		CPen penBtnHiLight(PS_SOLID, 0, GetSysColor(COLOR_BTNHILIGHT)); // White
		CPen pen3DLight(PS_SOLID, 0, GetSysColor(COLOR_3DLIGHT));       // Light gray
		CPen penBtnShadow(PS_SOLID, 0, GetSysColor(COLOR_BTNSHADOW));   // Dark gray
		CPen pen3DDKShadow(PS_SOLID, 0, GetSysColor(COLOR_3DDKSHADOW)); // Black

		if (m_bIsFlat == TRUE)
		{
			if (m_bMouseOnButton == TRUE && m_bDrawBorder == TRUE)
			{
				pDC->Draw3dRect(itemRect, ::GetSysColor(COLOR_BTNHILIGHT), ::GetSysColor(COLOR_BTNSHADOW));
			}
		}
		else
		{
			//白色线条
			pOldPen = pDC->SelectObject(&penBtnHiLight);
			pDC->MoveTo(itemRect.left, itemRect.bottom-1);
			pDC->LineTo(itemRect.left, itemRect.top);
			pDC->LineTo(itemRect.right, itemRect.top);
			// 亮灰线条
			pDC->SelectObject(pen3DLight);
			pDC->MoveTo(itemRect.left+1, itemRect.bottom-1);
			pDC->LineTo(itemRect.left+1, itemRect.top+1);
			pDC->LineTo(itemRect.right, itemRect.top+1);
			// 黑线条
			pDC->SelectObject(pen3DDKShadow);
			pDC->MoveTo(itemRect.left, itemRect.bottom-1);
			pDC->LineTo(itemRect.right-1, itemRect.bottom-1);
			pDC->LineTo(itemRect.right-1, itemRect.top-1);
			//暗灰线条
			pDC->SelectObject(penBtnShadow);
			pDC->MoveTo(itemRect.left+1, itemRect.bottom-2);
			pDC->LineTo(itemRect.right-2, itemRect.bottom-2);
			pDC->LineTo(itemRect.right-2, itemRect.top);
			//
			pDC->SelectObject(pOldPen);
		}
	}

	//得到按纽窗体标题
	CString sTitle;
	GetWindowText(sTitle);

	//若不显示标题
	if (m_bShowText == FALSE) sTitle.Empty();

	CRect captionRect = lpDIS->rcItem;

	// 画图标
	if (m_csIcons[0].hIcon != NULL)
	{
		DrawTheIcon(pDC, !sTitle.IsEmpty(), &lpDIS->rcItem, &captionRect, bIsPressed, bIsDisabled);
	}

	if (m_csBitmaps[0].hBitmap != NULL)
	{
		//此语名造成黑色成为透明色，暂改为黑色背景
	    //原pDC->SetBkColor(RGB(255,255,255));
		pDC->SetBkColor(RGB(0,0,0));
		DrawTheBitmap(pDC, !sTitle.IsEmpty(), &lpDIS->rcItem, &captionRect, bIsPressed, bIsDisabled);
	} // if

	//画按纽窗体标题
	if (sTitle.IsEmpty() == FALSE)
	{
		//若按纽为下压式按纽
		if (bIsPressed && m_bIsCheckBox == FALSE)
			captionRect.OffsetRect(1, 1);
    
		// 以下仅用于DEBUG模式 
		//CBrush brBtnShadow(RGB(255, 0, 0));
		//pDC->FrameRect(&captionRect, &brBtnShadow);

		/*
		if ((m_bMouseOnButton == TRUE) || (bIsPressed)) 
		{
			pDC->SetTextColor(GetActiveFgColor());
			pDC->SetBkColor(GetActiveBgColor());
		} 
		else 
		{
			pDC->SetTextColor(GetInactiveFgColor());
			pDC->SetBkColor(GetInactiveBgColor());
		}
		*/
		// 文本居中
		CRect centerRect = captionRect;
		pDC->DrawText(sTitle, -1, captionRect, DT_WORDBREAK | DT_CENTER | DT_CALCRECT);
		captionRect.OffsetRect((centerRect.Width() - captionRect.Width())/2, (centerRect.Height() - captionRect.Height())/2);
		/* RFU
		captionRect.OffsetRect(0, (centerRect.Height() - captionRect.Height())/2);
		captionRect.OffsetRect((centerRect.Width() - captionRect.Width())-4, (centerRect.Height() - captionRect.Height())/2);
		*/

		pDC->SetBkMode(TRANSPARENT);
		/*
		pDC->DrawState(captionRect.TopLeft(), captionRect.Size(), (LPCTSTR)sTitle, (bIsDisabled ? DSS_DISABLED : DSS_NORMAL), 
						TRUE, 0, (CBrush*)NULL);
		*/
		if (bIsDisabled)
		{
			captionRect.OffsetRect(1, 1);
			pDC->SetTextColor(::GetSysColor(COLOR_3DHILIGHT));
			pDC->DrawText(sTitle, -1, captionRect, DT_WORDBREAK | DT_CENTER);
			captionRect.OffsetRect(-1, -1);
			pDC->SetTextColor(::GetSysColor(COLOR_3DSHADOW));
			pDC->DrawText(sTitle, -1, captionRect, DT_WORDBREAK | DT_CENTER);
		}
		else
		{
			if ((m_bMouseOnButton == TRUE) ||(bIsPressed)) 
			{
				pDC->SetTextColor(m_crColors[BTNST_COLOR_FG_IN]);
				pDC->SetBkColor(m_crColors[BTNST_COLOR_BK_IN]);
			} 
			else 
			{
				pDC->SetTextColor(m_crColors[BTNST_COLOR_FG_OUT]);
				pDC->SetBkColor(m_crColors[BTNST_COLOR_BK_OUT]);
			}
			pDC->DrawText(sTitle, -1, captionRect, DT_WORDBREAK | DT_CENTER);
		}
	}

	if (m_bIsFlat == FALSE || (m_bIsFlat == TRUE && m_bDrawFlatFocus == TRUE))
	{
		// 画焦点矩形框
		if (bIsFocused)
		{
			CRect focusRect = itemRect;
			focusRect.DeflateRect(3, 3);
			pDC->DrawFocusRect(&focusRect);
		}
	}
} // End of DrawItem


/*
*--------------------------------------------------------------------------------
*  成员函数名   :DrawTheIcon
*  功能描述     :画图标
*  输入参数描述	:
*  CDC* pDC：      设备上下文
*  BOOL bHasTitle：是否有标签文本
*  RECT* rpItem：窗体矩形大小
*  CRect* rpTitle：标签文本矩形大小
*  BOOL bIsPressed：是否是下压式按纽
*  BOOL bIsDisabled：是否失效
*  返回变量说明 :
*  算法描述		:
*  附加说明     : 
*--------------------------------------------------------------------------------
*/
void CButtonST::DrawTheIcon(CDC* pDC, BOOL bHasTitle, RECT* rpItem, CRect* rpTitle, BOOL bIsPressed, BOOL bIsDisabled)
{
	BYTE		byIndex		= 0;

	// 选择要使用的图标
	if (m_bIsCheckBox == TRUE)
	{
		if (bIsPressed == TRUE)
		{
			byIndex = 0;
		} // if
		else
		{
			if (m_csIcons[1].hIcon != NULL)
				byIndex = 1;
			else
				byIndex = 0;		// 无鼠标离开图标可用
		} // else
	} // if
	else
	{
		if (m_bMouseOnButton == TRUE || bIsPressed == TRUE)
		{
			byIndex = 0;
		} // if
		else
		{
			if (m_csIcons[1].hIcon != NULL)
				byIndex = 1;
			else
				byIndex = 0;		// 无鼠标离开图标可用
		} // else
	} // else

	CRect	rImage;
	PrepareImageRect(bHasTitle, rpItem, rpTitle, bIsPressed, m_csIcons[byIndex].dwWidth, m_csIcons[byIndex].dwHeight, &rImage);

	// Ole'!
	pDC->DrawState(	rImage.TopLeft(),
					rImage.Size(), 
					m_csIcons[byIndex].hIcon,
					(bIsDisabled ? DSS_DISABLED : DSS_NORMAL), 
					(CBrush*)NULL);
} // End of DrawTheIcon


/*
*--------------------------------------------------------------------------------
*  成员函数名   :PreSubclassWindow() 
*  功能描述     :当前按纽窗体类型状态
*  输入参数描述	:
*  返回变量说明 :
*  算法描述		:
*  附加说明     : 
*--------------------------------------------------------------------------------
*/
void CButtonST::PreSubclassWindow() 
{
	UINT nBS;

	nBS = GetButtonStyle();

	// 检查是否是窗体中的默认按纽
	if (nBS & BS_DEFPUSHBUTTON) m_bIsDefault = TRUE;

	// 检查是否是checkbox类型按纽
	if (nBS & BS_CHECKBOX) m_bIsCheckBox = TRUE;

	// 加入自画类型 BS_OWNERDRAW 
	SetButtonStyle(nBS | BS_OWNERDRAW);

	CButton::PreSubclassWindow();
} // End of PreSubclassWindow


/*
*--------------------------------------------------------------------------------
*  成员函数名   :PreTranslateMessage
*  功能描述     :
*  输入参数描述	:
*  MSG* pMsg：   消息指针
*  返回变量说明 :若消息可以被分派，返回0,否则返加非0

*  算法描述		:
*  附加说明     : 
*--------------------------------------------------------------------------------
*/
BOOL CButtonST::PreTranslateMessage(MSG* pMsg) 
{
	InitToolTip();
	m_ToolTip.RelayEvent(pMsg);
	
	return CButton::PreTranslateMessage(pMsg);
} // End of PreTranslateMessage


/*
*--------------------------------------------------------------------------------
*  成员函数名   :DefWindowProc
*  功能描述     :
*  输入参数描述	:
*   UINT message:指定要WINDOWS 处理的消息
*   WPARAM wParam:消息中额外的参数
*   LPARAM lParam:消息中额外的参数
*  返回变量说明 :
*  算法描述		:
*  附加说明     : 
*--------------------------------------------------------------------------------
*/
LRESULT CButtonST::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	if (message == WM_LBUTTONDBLCLK)
	{
		message = WM_LBUTTONDOWN;
	}
	return CButton::DefWindowProc(message, wParam, lParam);
} // End of DefWindowProc

/*
*--------------------------------------------------------------------------------
*  成员函数名   :SetFlatFocus
*  功能描述     :设置平面类型按纽的焦点
*  输入参数描述	:
*  BOOL bDrawFlatFocus：是否画平面按纽
*  BOOL bRepaint      ：是否重画
*  返回变量说明 :
*  算法描述		:
*  附加说明     : 
*--------------------------------------------------------------------------------
*/
void CButtonST::SetFlatFocus(BOOL bDrawFlatFocus, BOOL bRepaint)
{
	m_bDrawFlatFocus = bDrawFlatFocus;
	
	// 重画按纽窗体
	if (bRepaint == TRUE) Invalidate();
} // End of SetFlatFocus


/*
*--------------------------------------------------------------------------------
*  成员函数名   :GetFlatFocus()
*  功能描述     :得到平面按纽焦点
*  输入参数描述	:
*  返回变量说明 :返回TRUE表示按纽有平面焦点
*  算法描述		:
*  附加说明     : 
*--------------------------------------------------------------------------------
*/
BOOL CButtonST::GetFlatFocus()
{
	return m_bDrawFlatFocus;
} // End of GetFlatFocus


/*
*--------------------------------------------------------------------------------
*  成员函数名   :SetBtnCursor
*  功能描述     :为按纽设置鼠标光标
*  输入参数描述	:
*  HCURSOR hCursor  ：要设置的光标句柄
*  返回变量说明 :返回TRUE表示成功
*  算法描述		:
*  附加说明     : 
*--------------------------------------------------------------------------------
*/
BOOL CButtonST::SetBtnCursor(HCURSOR hCursor)
{
	// 释放以前的光标
	if (m_hCursor != NULL) 
		::DestroyCursor(m_hCursor);
	m_hCursor = NULL;
	if (hCursor != NULL)
	{
		// 导入ICON资源
		m_hCursor = hCursor;
		//若出错返回FALSE
		if (m_hCursor == NULL) 
			return FALSE;
	}

	return TRUE;
}


/*
*--------------------------------------------------------------------------------
*  成员函数名   :OnSetCursor
*  功能描述     :为按纽设置鼠标光标
*  输入参数描述	:
*  CWnd* pWnd   ：
*  UINT nHitTest：
*  UINT message ：
*  返回变量说明 :
*  算法描述		:
*  附加说明     : 
*--------------------------------------------------------------------------------
*/
BOOL CButtonST::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	// 若光标被指定使用它
	if (m_hCursor != NULL)
	{
		::SetCursor(m_hCursor);
		return TRUE;
	}

	return CButton::OnSetCursor(pWnd, nHitTest, message);
} // End of OnSetCursor

/*
*--------------------------------------------------------------------------------
*  成员函数名   :SetTooltipText
*  功能描述     :为按纽设置提示信息文本
*  输入参数描述	:
*   LPCTSTR lpszText：提示信息文本内容
*   BOOL bActivate  ：是否显示提示信息
*  返回变量说明 :
*  算法描述		:
*  附加说明     : 
*--------------------------------------------------------------------------------
*/
void CButtonST::SetTooltipText(LPCTSTR lpszText, BOOL bActivate)
{
	//不接受空文本
	if (lpszText == NULL) return;

	// 初始化ToolTip
	InitToolTip();

	//若没有定义tooltip增加它的定义
	if (m_ToolTip.GetToolCount() == 0)
	{
		CRect rectBtn; 
		GetClientRect(rectBtn);
		m_ToolTip.AddTool(this, lpszText, rectBtn, 1);
	}

	// 为tooltip加入文本
	m_ToolTip.UpdateTipText(lpszText, this, 1);
	m_ToolTip.Activate(bActivate);
} // End of SetTooltipText


/*
*--------------------------------------------------------------------------------
*  成员函数名   :SetTooltipText
*  功能描述     :为按纽设置提示信息文本
*  输入参数描述	:
*  int nId      ：提示文本字符串资源ID号
*  BOOL bActivate：提示信息文本是不有效
*  返回变量说明 :
*  算法描述		:
*  附加说明     : 
*--------------------------------------------------------------------------------
*/
void CButtonST::SetTooltipText(int nId, BOOL bActivate)
{
	CString sText;

	// 导入提示文本资源
	sText.LoadString(nId);
	// 若字符串资源不为空
	if (sText.IsEmpty() == FALSE) SetTooltipText((LPCTSTR)sText, bActivate);
} // End of SetTooltipText


/*
*--------------------------------------------------------------------------------
*  成员函数名   :ActivateTooltip
*  功能描述     :设置按纽提示文本有效
*  输入参数描述	:
*  BOOL bActivate：按纽提示信息是否有效
*  返回变量说明 :
*  算法描述		:
*  附加说明     : 
*--------------------------------------------------------------------------------
*/
void CButtonST::ActivateTooltip(BOOL bActivate)
{
	// 若无tooltip返回
	if (m_ToolTip.GetToolCount() == 0) return;

	//使提示文本有效
	m_ToolTip.Activate(bActivate);
} // End of EnableTooltip


/*
*--------------------------------------------------------------------------------
*  成员函数名   :GetDefault()
*  功能描述     :得到按纽是否是窗体上的默认按纽
*  输入参数描述	:
*  返回变量说明 :返回TRUE表不按纽为窗体上的默认按纽
*  算法描述		:
*  附加说明     : 
*--------------------------------------------------------------------------------
*/
BOOL CButtonST::GetDefault()
{
	return m_bIsDefault;
} // End of GetDefault


/*
*--------------------------------------------------------------------------------
*  成员函数名   :DrawTransparent
*  功能描述     :画透明按纽
*  输入参数描述	:
*  BOOL bRepaint：按纽是否透明
*  返回变量说明 :
*  算法描述		:
*  附加说明     : 
*--------------------------------------------------------------------------------
*/
void CButtonST::DrawTransparent(BOOL bRepaint)
{
	m_bDrawTransparent = TRUE;

	// 恢复旧位图
	if (m_dcBk.m_hDC != NULL && m_pbmpOldBk != NULL)
	{
		m_dcBk.SelectObject(m_pbmpOldBk);
	} // if

	m_bmpBk.DeleteObject();
	m_dcBk.DeleteDC();

	// 重画按纽
	if (bRepaint == TRUE) Invalidate();
} // End of DrawTransparent


/*
*--------------------------------------------------------------------------------
*  成员函数名   :InitToolTip()
*  功能描述     :初始化按纽提示信息
*  输入参数描述	:
*  返回变量说明 :
*  算法描述		:
*  附加说明     : 
*--------------------------------------------------------------------------------
*/
void CButtonST::InitToolTip()
{
	if (m_ToolTip.m_hWnd == NULL)
	{
		// 创建ToolTip控件
		m_ToolTip.Create(this);
		// 非活动的
		m_ToolTip.Activate(FALSE);
		// 多行有效
		m_ToolTip.SendMessage(TTM_SETMAXTIPWIDTH, 0, 400);
	} // if
} // End of InitToolTip


/*
*--------------------------------------------------------------------------------
*  成员函数名   :PaintBk
*  功能描述     :画按纽背景
*  输入参数描述	:
*   CDC* pDC ： 绘画设备上下文
*  返回变量说明 :
*  算法描述		:
*  附加说明     : 
*--------------------------------------------------------------------------------
*/
void CButtonST::PaintBk(CDC* pDC)
{
	CClientDC clDC(GetParent());
	CRect rect;
	CRect rect1;

	GetClientRect(rect);

	GetWindowRect(rect1);
	GetParent()->ScreenToClient(rect1);

	if (m_dcBk.m_hDC == NULL)
	{
		m_dcBk.CreateCompatibleDC(&clDC);
		m_bmpBk.CreateCompatibleBitmap(&clDC, rect.Width(), rect.Height());
		m_pbmpOldBk = m_dcBk.SelectObject(&m_bmpBk);
		m_dcBk.BitBlt(0, 0, rect.Width(), rect.Height(), &clDC, rect1.left, rect1.top, SRCCOPY);
	} // if

	pDC->BitBlt(0, 0, rect.Width(), rect.Height(), &m_dcBk, 0, 0, SRCCOPY);
} // End of PaintBk

/*
*--------------------------------------------------------------------------------
*  成员函数名   :CtlColor
*  功能描述     :
*  输入参数描述	:
*  CDC* pDC：
*  UINT nCtlColor：
*  返回变量说明 :
*  算法描述		:
*  附加说明     : 
*--------------------------------------------------------------------------------
*/
HBRUSH CButtonST::CtlColor(CDC* pDC, UINT nCtlColor) 
{
	return (HBRUSH)::GetStockObject(NULL_BRUSH); 
} // End of CtlColor

/*
*--------------------------------------------------------------------------------
*  成员函数名   :OnSysColorChange
*  功能描述     :当系统色彩变化时删除CBitmap对象
*  输入参数描述	:
*  返回变量说明 :
*  算法描述		:
*  附加说明     : 
*--------------------------------------------------------------------------------
*/
void CButtonST::OnSysColorChange() 
{
	CButton::OnSysColorChange();

	m_dcBk.DeleteDC();
	m_bmpBk.DeleteObject();	
} // End of OnSysColorChange

/*
*--------------------------------------------------------------------------------
*  成员函数名   :OnClicked()
*  功能描述     :单击按纽时
*  输入参数描述	:
*  返回变量说明 :
*  算法描述		:
*  附加说明     : 
*--------------------------------------------------------------------------------
*/
BOOL CButtonST::OnClicked() 
{	
	if (m_bIsCheckBox == TRUE)
	{
		m_nCheck = !m_nCheck;
		Invalidate();
	} // if
	else
	{
		// Handle the URL (if any)
		if (::lstrlen(m_szURL) > 0)
			::ShellExecute(NULL, _T("open"), m_szURL, NULL,NULL, SW_SHOWMAXIMIZED);
	} // else

	return FALSE;
} // End of OnClicked


/*
*--------------------------------------------------------------------------------
*  成员函数名   :SetCheck
*  功能描述     :按纽为CheckBox类型时设置当前显示状态
*  输入参数描述	:
*  int nCheck：   是否选中
*  BOOL bRepaint：是否重画
*  返回变量说明 :
*  算法描述		:
*  附加说明     : 
*--------------------------------------------------------------------------------
*/
void CButtonST::SetCheck(int nCheck, BOOL bRepaint)
{
	if (m_bIsCheckBox == TRUE)
	{
		if (nCheck == 0) m_nCheck = 0;
		else m_nCheck = 1;

		if (bRepaint == TRUE) Invalidate();
	} // if
} // End of SetCheck


/*
*--------------------------------------------------------------------------------
*  成员函数名   :GetCheck()
*  功能描述     :按纽为CheckBox类型时得到当前按纽的CHECK状态
*  输入参数描述	:
*  返回变量说明 :
*  返回状态值
*  算法描述		:
*  附加说明     : 
*--------------------------------------------------------------------------------
*/
int CButtonST::GetCheck()
{
	return m_nCheck;
} // End of GetCheck


/*
*--------------------------------------------------------------------------------
*  成员函数名   :FreeResources
*  功能描述     :释放所有资源
*  输入参数描述	：
*  BOOL bCheckForNULL：
*  返回变量说明 :
*  算法描述		:
*  附加说明     : 
*--------------------------------------------------------------------------------
*/
void CButtonST::FreeResources(BOOL bCheckForNULL)
{
	if (bCheckForNULL == TRUE)
	{
		// 释放光标资源
		
		if (m_csIcons[0].hIcon != NULL)	::DeleteObject(m_csIcons[0].hIcon);
		if (m_csIcons[1].hIcon != NULL)	::DeleteObject(m_csIcons[1].hIcon);

		//  释放位图资源
		if (m_csBitmaps[0].hBitmap != NULL)	::DeleteObject(m_csBitmaps[0].hBitmap);
		if (m_csBitmaps[1].hBitmap != NULL)	::DeleteObject(m_csBitmaps[1].hBitmap);

		// 释放mask 位图
		if (m_csBitmaps[0].hMask != NULL)	::DeleteObject(m_csBitmaps[0].hMask);
		if (m_csBitmaps[1].hMask != NULL)	::DeleteObject(m_csBitmaps[1].hMask);
	} // end if

	::ZeroMemory(&m_csIcons, sizeof(m_csIcons));
	::ZeroMemory(&m_csBitmaps, sizeof(m_csBitmaps));
} // End of FreeResources


/*
*--------------------------------------------------------------------------------
*  成员函数名   :SetBitmaps
*  功能描述     :使用位图资源句柄设置按纽的两态图
*  输入参数描述	:
*  HBITMAP hBitmapIn：       当鼠标在按纽上时显示的位图句柄指针
*  COLORREF crTransColorIn： 当鼠标在按纽上时显示的位图的透明色
*  HBITMAP hBitmapOut：      当鼠标离开按纽时显示的位图句柄指针
*  COLORREF crTransColorOut：当鼠标离开按纽时显示的位图的透明色
*  返回变量说明 :
*  算法描述		:
*  附加说明     : 
*--------------------------------------------------------------------------------
*/
DWORD CButtonST::SetBitmaps(HBITMAP hBitmapIn, COLORREF crTransColorIn, HBITMAP hBitmapOut, COLORREF crTransColorOut)
{
	int		nRetValue;
	BITMAP	csBitmapSize;

	//释放所有导入的资源
	FreeResources();

	if (hBitmapIn != NULL)
	{
		m_csBitmaps[0].hBitmap = hBitmapIn;
		m_csBitmaps[0].crTransparent = crTransColorIn;
		//得到位图指针
		nRetValue = ::GetObject(hBitmapIn, sizeof(csBitmapSize), &csBitmapSize);
		if (nRetValue == 0)
		{
			FreeResources();
			return BTNST_INVALIDRESOURCE;
		} //end  if
		m_csBitmaps[0].dwWidth = (DWORD)csBitmapSize.bmWidth;
		m_csBitmaps[0].dwHeight = (DWORD)csBitmapSize.bmHeight;

		// 为当鼠标在按纽上时创建掩饰位图
		m_csBitmaps[0].hMask = CreateBitmapMask(hBitmapIn, m_csBitmaps[0].dwWidth, m_csBitmaps[0].dwHeight, crTransColorIn);
		if (m_csBitmaps[0].hMask == NULL)
		{
			FreeResources();
			return BTNST_FAILEDMASK;
		} // end if

		if (hBitmapOut != NULL)
		{
			m_csBitmaps[1].hBitmap = hBitmapOut;
			m_csBitmaps[1].crTransparent = crTransColorOut;
			// 得到位图尺寸
			nRetValue = ::GetObject(hBitmapOut, sizeof(csBitmapSize), &csBitmapSize);
			if (nRetValue == 0)
			{
				FreeResources();
				return BTNST_INVALIDRESOURCE;
			} // if
			m_csBitmaps[1].dwWidth = (DWORD)csBitmapSize.bmWidth;
			m_csBitmaps[1].dwHeight = (DWORD)csBitmapSize.bmHeight;

			// 为当鼠标离开按纽时创建掩饰位图
			m_csBitmaps[1].hMask = CreateBitmapMask(hBitmapOut, m_csBitmaps[1].dwWidth, m_csBitmaps[1].dwHeight, crTransColorOut);
			if (m_csBitmaps[1].hMask == NULL)
			{
				FreeResources();
				return BTNST_FAILEDMASK;
			} //end  if
		} // end if
	} // end if

	RedrawWindow();

	return BTNST_OK;
} // End of SetBitmaps


/*
*--------------------------------------------------------------------------------
*  成员函数名   :SetBitmaps
*  功能描述     :使用位图资源设置按纽的两态图
*  输入参数描述	:
*  int nBitmapIn：           当鼠标在按纽上时显示的位图资源ID号
*  COLORREF crTransColorIn： 当鼠标在按纽上时显示的位图的透明色
*  int nBitmapOut：          当鼠标离开按纽时显示的位图资源ID号
*  COLORREF crTransColorOut：当鼠标离开按纽时显示的位图的透明色
*  返回变量说明 :
*  算法描述		:
*  附加说明     : 
*--------------------------------------------------------------------------------
*/
DWORD CButtonST::SetBitmaps(int nBitmapIn, COLORREF crTransColorIn, int nBitmapOut, COLORREF crTransColorOut)
{
	HBITMAP		hBitmapIn	= NULL;
	HBITMAP		hBitmapOut	= NULL;
	HINSTANCE hInstResource	= NULL;
	
	//查找当前资源
	hInstResource = AfxFindResourceHandle(MAKEINTRESOURCE(nBitmapIn), RT_BITMAP);

	//导入当鼠标在按纽上时显示的位图资源
	hBitmapIn = (HBITMAP)::LoadImage(hInstResource, MAKEINTRESOURCE(nBitmapIn), IMAGE_BITMAP, 0, 0, 0);

	// 导入当鼠标离开按纽时显示的位图资源
	hBitmapOut = (HBITMAP)::LoadImage(hInstResource, MAKEINTRESOURCE(nBitmapOut), IMAGE_BITMAP, 0, 0, 0);

	return SetBitmaps(hBitmapIn, crTransColorIn, hBitmapOut, crTransColorOut);
} // End of SetBitmaps


/*
*--------------------------------------------------------------------------------
*  成员函数名   :DrawTheBitmap
*  功能描述     :画位图
*  输入参数描述	:
*  CDC* pDC：      绘画设备上下文
*  BOOL bHasTitle：是否有文本标签
*  RECT* rItem：    绘画矩形区
*  CRect *rCaption：文本矩形区
*  BOOL bIsPressed：是否是下压按纽
*  BOOL bIsDisabled：是否失效
*  返回变量说明 :
*  算法描述		:
*  附加说明     : 
*--------------------------------------------------------------------------------
*/
void CButtonST::DrawTheBitmap(CDC* pDC, BOOL bHasTitle, RECT* rItem, CRect *rCaption, BOOL bIsPressed, BOOL bIsDisabled)
{
	HDC			hdcBmpMem	= NULL;
	HBITMAP		hbmOldBmp	= NULL;
	HDC			hdcMem		= NULL;
	HBITMAP		hbmT		= NULL;

	BYTE		byIndex		= 0;

	// 选取要使用的位图
	if (m_bIsCheckBox == TRUE)
	{
		if (bIsPressed == TRUE)
		{
			byIndex = 0;
		} // if
		else
		{
			if (m_csBitmaps[1].hBitmap != NULL)
				byIndex = 1;
			else
				byIndex = 0;		//无位图可用
		} // else
	} // if
	else
	{
		if (m_bMouseOnButton == TRUE || bIsPressed == TRUE)
		{
			byIndex = 0;
		} // if
		else
		{
			if (m_csBitmaps[1].hBitmap != NULL)
				byIndex = 1;
			else
				byIndex = 0;		//无位图可用
		} // else
	} // else

	CRect	rImage;
	PrepareImageRect(bHasTitle, rItem, rCaption, bIsPressed, m_csBitmaps[byIndex].dwWidth, m_csBitmaps[byIndex].dwHeight, &rImage);

	hdcBmpMem = ::CreateCompatibleDC(pDC->m_hDC);

	hbmOldBmp = (HBITMAP)::SelectObject(hdcBmpMem, m_csBitmaps[byIndex].hBitmap);

	hdcMem = ::CreateCompatibleDC(NULL);

	hbmT = (HBITMAP)::SelectObject(hdcMem, m_csBitmaps[byIndex].hMask);

	::BitBlt(pDC->m_hDC, rImage.left, rImage.top, m_csBitmaps[byIndex].dwWidth, m_csBitmaps[byIndex].dwHeight, hdcMem, 0, 0, SRCAND);

	::BitBlt(pDC->m_hDC, rImage.left, rImage.top, m_csBitmaps[byIndex].dwWidth, m_csBitmaps[byIndex].dwHeight, hdcBmpMem, 0, 0, SRCPAINT);

	::SelectObject(hdcMem, hbmT);
	::DeleteDC(hdcMem);

	::SelectObject(hdcBmpMem, hbmOldBmp);
	::DeleteDC(hdcBmpMem);
} // End of DrawTheBitmap


/*
*--------------------------------------------------------------------------------
*  成员函数名   :CreateBitmapMask
*  功能描述     :创建掩饰位图
*  输入参数描述	:
*   HBITMAP hSourceBitmap：资源位图句柄
*   DWORD dwWidth：位图宽
*   DWORD dwHeight：位图高
*   COLORREF crTransColor：透明色
*  返回变量说明 :返回创建成功的掩饰位图句柄指针
*  算法描述		:
*  附加说明     : 
*--------------------------------------------------------------------------------
*/
HBITMAP CButtonST::CreateBitmapMask(HBITMAP hSourceBitmap, DWORD dwWidth, DWORD dwHeight, COLORREF crTransColor)
{
	HBITMAP		hMask		= NULL;
	HDC			hdcSrc		= NULL;
	HDC			hdcDest		= NULL;
	HBITMAP		hbmSrcT		= NULL;
	HBITMAP		hbmDestT	= NULL;
	COLORREF	crSaveBk;
	COLORREF	crSaveDestText;

	hMask = ::CreateBitmap(dwWidth, dwHeight, 1, 1, NULL);
	if (hMask == NULL)	return NULL;

	hdcSrc	= ::CreateCompatibleDC(NULL);
	hdcDest	= ::CreateCompatibleDC(NULL);

	hbmSrcT = (HBITMAP)::SelectObject(hdcSrc, hSourceBitmap);
	hbmDestT = (HBITMAP)::SelectObject(hdcDest, hMask);

	crSaveBk = ::SetBkColor(hdcSrc, crTransColor);

	::BitBlt(hdcDest, 0, 0, dwWidth, dwHeight, hdcSrc, 0, 0, SRCCOPY);

	crSaveDestText = ::SetTextColor(hdcSrc, RGB(255, 255, 255));
	::SetBkColor(hdcSrc,RGB(0, 0, 0));

	::BitBlt(hdcSrc, 0, 0, dwWidth, dwHeight, hdcDest, 0, 0, SRCAND);

	SetTextColor(hdcDest, crSaveDestText);

	::SetBkColor(hdcSrc, crSaveBk);
	::SelectObject(hdcSrc, hbmSrcT);
	::SelectObject(hdcDest, hbmDestT);

	::DeleteDC(hdcSrc);
	::DeleteDC(hdcDest);

	return hMask;
} // End of CreateBitmapMask

/*
*--------------------------------------------------------------------------------
*  成员函数名   :PrepareImageRect
*  功能描述     :准备画图形区矩形
*  输入参数描述	:
*   BOOL bHasTitle：是否有文本标签
*   RECT* rpItem：窗体矩形区
*   CRect* rpTitle：标签矩形区
*   BOOL bIsPressed：是否是下压按纽
*   DWORD dwWidth：图象宽度
*   DWORD dwHeight：图象高度
*   CRect* rpImage：图象矩形区
*  返回变量说明 :
*  算法描述		:
*  附加说明     : 
*--------------------------------------------------------------------------------
*/
void CButtonST::PrepareImageRect(BOOL bHasTitle, RECT* rpItem, CRect* rpTitle, BOOL bIsPressed, DWORD dwWidth, DWORD dwHeight, CRect* rpImage)
{
	CRect rBtn;

	rpImage->CopyRect(rpItem);

	switch (m_nAlign)
	{
		case ST_ALIGN_HORIZ:
			if (bHasTitle == FALSE /*spTitle->IsEmpty()*/)
			{
				// 图象水平居中
				rpImage->left += ((rpImage->Width() - dwWidth)/2);
			}
			else
			{
				//图象必须放置在焦点矩形区内
				rpImage->left += 3;  
				rpTitle->left += dwWidth + 3;
			}
			// 图象垂直居中
			rpImage->top += ((rpImage->Height() - dwHeight)/2);
			break;

		case ST_ALIGN_HORIZ_RIGHT:
			GetClientRect(&rBtn);
			if (bHasTitle == FALSE /*spTitle->IsEmpty()*/)
			{
				// 图象水平居中
				rpImage->left += ((rpImage->Width() - dwWidth)/2);
			}
			else
			{
				//图象必须放置在焦点矩形区内
				rpTitle->right = rpTitle->Width() - dwWidth - 3;
				rpTitle->left = 3;
				rpImage->left = rBtn.right - dwWidth - 3;
				// 图象垂直居中
				rpImage->top += ((rpImage->Height() - dwHeight)/2);
			}
			break;
		
		case ST_ALIGN_VERT:
			// 图象水平居中
			rpImage->left += ((rpImage->Width() - dwWidth)/2);
			if (bHasTitle == FALSE /*spTitle->IsEmpty()*/)
			{
					// 图象垂直居中
				rpImage->top += ((rpImage->Height() - dwHeight)/2);           
			}
			else
			{
				rpImage->top = 3;
				rpTitle->top += dwHeight;
			}
			break;
	}
    
	//若按纽被压下图象也显示相应效果
	if (bIsPressed == TRUE && m_bIsCheckBox == FALSE)
		rpImage->OffsetRect(1, 1);
} // End of PrepareImageRect

/*
*--------------------------------------------------------------------------------
*  成员函数名   :SetDefaultColors
*  功能描述     :设置绘画用的默认颜色
*  输入参数描述	:
*   BOOL bRepaint:是否重画
*  返回变量说明 :返回BTNST_OK
*
*  算法描述		:
*  附加说明     : 
*--------------------------------------------------------------------------------
*/
DWORD CButtonST::SetDefaultColors(BOOL bRepaint)
{
	m_crColors[BTNST_COLOR_BK_IN]	= ::GetSysColor(COLOR_BTNFACE);
	m_crColors[BTNST_COLOR_FG_IN]	= ::GetSysColor(COLOR_BTNTEXT);
	m_crColors[BTNST_COLOR_BK_OUT]	= ::GetSysColor(COLOR_BTNFACE);
	m_crColors[BTNST_COLOR_FG_OUT]	= ::GetSysColor(COLOR_BTNTEXT);

	if (bRepaint == TRUE)	Invalidate();

	return BTNST_OK;
} // End of SetDefaultColors



/*
*--------------------------------------------------------------------------------
*  成员函数名   :SetColor
*  功能描述     :设置按纽颜色
*  输入参数描述	:
*  BYTE byColorIndex：要设置颜色的索引号
*  COLORREF crColor： 设置的新颜色
*   BOOL bRepaint：   是否重画
*  返回变量说明 :
*    BTNST_INVALIDINDEX：失败
*    BTNST_OK：          成功
*  算法描述		:
*  附加说明     : 
*--------------------------------------------------------------------------------
*/
DWORD CButtonST::SetColor(BYTE byColorIndex, COLORREF crColor, BOOL bRepaint)
{
	if (byColorIndex >= BTNST_MAX_COLORS)	return BTNST_INVALIDINDEX;

	// 设置新颜色
	m_crColors[byColorIndex] = crColor;

	if (bRepaint == TRUE)	Invalidate();

	return BTNST_OK;
} // End of SetColor

//
// Parameters:
//		[IN]	byColorIndex
//				Index of the color to get. This index is zero-based.
//		[OUT]	crpColor
//				A pointer to a COLORREF that will receive the color.
//
// Return value:
//		BTNST_OK
//			Function executed successfully.
//		BTNST_INVALIDINDEX
//			Invalid color index.
//
/*
*--------------------------------------------------------------------------------
*  成员函数名   :GetColor
*  功能描述     :得到指定的颜色
*  输入参数描述	:
*  BYTE byColorIndex：要得到颜色的索引号
*  COLORREF* crpColor：颜色值
*  返回变量说明 :
*    BTNST_INVALIDINDEX：失败
*    BTNST_OK：          成功
*  算法描述		:
*  附加说明     : 
*--------------------------------------------------------------------------------
*/
DWORD CButtonST::GetColor(BYTE byColorIndex, COLORREF* crpColor)
{
	if (byColorIndex >= BTNST_MAX_COLORS)	return BTNST_INVALIDINDEX;

	// 得到颜色
	*crpColor = m_crColors[byColorIndex];

	return BTNST_OK;
} // End of GetColor

//
// Parameters:
//		[IN]	lpszURL
//				Pointer to a null-terminated string that contains the URL.
//
// Return value:
//		BTNST_OK
//			Function executed successfully.
//
/*
*--------------------------------------------------------------------------------
*  成员函数名   :SetURL
*  功能描述     :超联接按纽
*  输入参数描述	:
*  LPCTSTR lpszURL：超联接文本字符串
*  返回变量说明 :
*    BTNST_INVALIDINDEX：失败
*    BTNST_OK：          成功
*  算法描述		:
*  附加说明     : 
*--------------------------------------------------------------------------------
*/
DWORD CButtonST::SetURL(LPCTSTR lpszURL)
{
	if (lpszURL != NULL)
	{
		// 保存 URL
		::lstrcpyn(m_szURL, lpszURL, _MAX_PATH);
	} // if
	else
	{
		// 转移URL
		::ZeroMemory(&m_szURL, sizeof(m_szURL));
	} // else

	return BTNST_OK;
} // End of SetURL


/*
*--------------------------------------------------------------------------------
*  成员函数名   :CancelHover()
*  功能描述     :失效回旋功能
*  输入参数描述	:
*  返回变量说明 :
*  算法描述		:
*  附加说明     : 
*--------------------------------------------------------------------------------
*/
void CButtonST::CancelHover()
{
	// 不是平面按纽返回
	if (m_bIsFlat == FALSE) return;

	if (m_bMouseOnButton == TRUE)
	{
		m_bMouseOnButton = FALSE;
		Invalidate();
	} // if
} // End of CancelHover



/*
*--------------------------------------------------------------------------------
*  成员函数名   :SetAutoRepeat
*  功能描述     :设置按纽是否有自动重复按下的功能
*  输入参数描述	:
*  BOOL bSet           ：纽是否有自动重现按下的功能
*  DWORD dwMilliseconds：按纽双击间的时间间隔
*  返回变量说明 :
*  算法描述		:
*  附加说明     : 
*--------------------------------------------------------------------------------
*/
DWORD CButtonST::SetAutoRepeat(BOOL bSet, DWORD dwMilliseconds)
{
	m_bAutoRepeat = bSet;
	m_dwPeriodAutoRepeat = dwMilliseconds;

	return BTNST_OK;
} // End of SetAutoRepeat


/*
*--------------------------------------------------------------------------------
*  成员函数名   :SetIcon
*  功能描述     ：使用外部图标文件为按纽设置两态图标
*  输入参数描述	:
*   LPCSTR strIconIn ：当鼠标在按纽上时要显示的图标的文件名
*   LPCSTR strIconOut：当鼠标离按纽时要显示的图标的文件名
*  返回变量说明 :
*  算法描述		:
*  附加说明     : 
*--------------------------------------------------------------------------------
*/
DWORD CButtonST::SetIcon(LPCSTR strIconIn, LPCSTR strIconOut)
{
	BOOL		bRetValue;
	ICONINFO	ii;
   
	HICON hIconIn = (HICON) LoadIcon(NULL, strIconIn);
	HICON hIconOut = (HICON) LoadIcon(NULL, strIconOut);

//	hIconIn=AfxGetApp()->LoadIcon(strIconIn);
//  hIconOut=AfxGetApp()->LoadIcon(strIconOut);
	// 释放所有资源
	FreeResources();

	if (hIconIn != NULL)
	{
		m_csIcons[0].hIcon = hIconIn;

		// 得到图标尺寸
		ZeroMemory(&ii, sizeof(ICONINFO));
		bRetValue = ::GetIconInfo(hIconIn, &ii);
		if (bRetValue == FALSE)
		{
			FreeResources();
			return BTNST_INVALIDRESOURCE;
		} // if

		m_csIcons[0].dwWidth	= (DWORD)(ii.xHotspot * 2);
		m_csIcons[0].dwHeight	= (DWORD)(ii.yHotspot * 2);
		::DeleteObject(ii.hbmMask);
		::DeleteObject(ii.hbmColor);

		if (hIconOut != NULL)
		{
			m_csIcons[1].hIcon = hIconOut;

			//得到图标尺寸
			ZeroMemory(&ii, sizeof(ICONINFO));
			bRetValue = ::GetIconInfo(hIconOut, &ii);
			if (bRetValue == FALSE)
			{
				FreeResources();
				return BTNST_INVALIDRESOURCE;
			} //end  if

			m_csIcons[1].dwWidth	= (DWORD)(ii.xHotspot * 2);
			m_csIcons[1].dwHeight	= (DWORD)(ii.yHotspot * 2);
			::DeleteObject(ii.hbmMask);
			::DeleteObject(ii.hbmColor);
		} //end  if
	} //end if

	RedrawWindow();

    //DeleteObject(hIconIn);
    //DeleteObject(hIconOut);

	return BTNST_OK;

}


/*
*--------------------------------------------------------------------------------
*  成员函数名   :SetBitmaps
*  功能描述     ：使用外部位图文件为按纽设置两态图标
*  输入参数描述	:
*   LPCSTR strIconIn ：当鼠标在按纽上时要显示的图标的.bmp文件名
*   LPCSTR strIconOut：当鼠标离按纽时要显示的图标的.bmp文件名
*  返回变量说明 :
*  算法描述		:
*  附加说明     : 
*--------------------------------------------------------------------------------
*/
DWORD CButtonST::SetBitmaps(LPCSTR strBitmapIn, COLORREF crTransColorIn, LPCSTR strBitmapOut, COLORREF crTransColorOut)
{
	int		nRetValue;

	HBITMAP hBitmapIn = (HBITMAP) LoadImage(NULL, strBitmapIn, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION | LR_DEFAULTSIZE);

	HBITMAP hBitmapOut=(HBITMAP) LoadImage(NULL, strBitmapOut, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION | LR_DEFAULTSIZE);

	BITMAP	csBitmapSize;

	// 释放所有资源
	FreeResources();

	if (hBitmapIn != NULL)
	{
		m_csBitmaps[0].hBitmap = hBitmapIn;
		m_csBitmaps[0].crTransparent = crTransColorIn;
		//得到位图尺寸
		nRetValue = ::GetObject(hBitmapIn, sizeof(csBitmapSize), &csBitmapSize);
		if (nRetValue == 0)
		{
			FreeResources();
			return BTNST_INVALIDRESOURCE;
		} // if
		m_csBitmaps[0].dwWidth = (DWORD)csBitmapSize.bmWidth;
		m_csBitmaps[0].dwHeight = (DWORD)csBitmapSize.bmHeight;

		//创建鼠标在按纽上时的掩饰位图
		m_csBitmaps[0].hMask = CreateBitmapMask(hBitmapIn, m_csBitmaps[0].dwWidth, m_csBitmaps[0].dwHeight, crTransColorIn);
		if (m_csBitmaps[0].hMask == NULL)
		{
			FreeResources();
			return BTNST_FAILEDMASK;
		} // if

		if (hBitmapOut != NULL)
		{
			m_csBitmaps[1].hBitmap = hBitmapOut;
			m_csBitmaps[1].crTransparent = crTransColorOut;
			// 得到位图尺寸
			nRetValue = ::GetObject(hBitmapOut, sizeof(csBitmapSize), &csBitmapSize);
			if (nRetValue == 0)
			{
				FreeResources();
				return BTNST_INVALIDRESOURCE;
			} // if
			m_csBitmaps[1].dwWidth = (DWORD)csBitmapSize.bmWidth;
			m_csBitmaps[1].dwHeight = (DWORD)csBitmapSize.bmHeight;

		     //创建鼠标离开按纽时的掩饰位图
			m_csBitmaps[1].hMask = CreateBitmapMask(hBitmapOut, m_csBitmaps[1].dwWidth, m_csBitmaps[1].dwHeight, crTransColorOut);
			if (m_csBitmaps[1].hMask == NULL)
			{
				FreeResources();
				return BTNST_FAILEDMASK;
			} //end if
		} // end if
	} // end if

	RedrawWindow();
 	RedrawWindow();   
	//DeleteObject(hBitmapIn);
	//DeleteObject(hBitmapOut);
	return BTNST_OK;

}



/*
*--------------------------------------------------------------------------------
*  成员函数名   :SetLbuttonSendMsgStyle
*  功能描述     ：设置按纽返回父窗体的左键的单击消息类型
*  输入参数描述	:
*     INT msgstyle：消息类型
                    LDOWN：左键按下
					LCLICK：单击事件
*  返回变量说明 :
*  算法描述		:
*  附加说明     : 
*--------------------------------------------------------------------------------
*/
void CButtonST::SetLbuttonSendMsgStyle(INT msgstyle)
{
  if(msgstyle == 0)
      m_downmessage = LCLICK;
  else if(msgstyle == 1)
	  m_downmessage = LDOWN;
  else if(msgstyle==2)
	  m_downmessage = LPOPMENU;
}

/*
*--------------------------------------------------------------------------------
*  成员函数名   :OnNcHitTest
*  功能描述     ：设置按纽返回父窗体的左键的单击消息类型
*  输入参数描述	:
*   CPoint point:
*  返回变量说明 :
*  算法描述		:
*  附加说明     : 
*--------------------------------------------------------------------------------
*/

UINT CButtonST::OnNcHitTest(CPoint point) 
{
	 return m_HitTestStyle;

//	return CButton::OnNcHitTest(point);
}


/*
*--------------------------------------------------------------------------------
*  成员函数名   :SetHitTestStyle
*  功能描述     ：设置按纽返回父窗体的左键的单击消息类型
*  输入参数描述	:
*   LRESULT style:要设置的鼠标在窗体上的模式类型 
*  返回变量说明 :
*  算法描述		:
*  附加说明     : 
*--------------------------------------------------------------------------------
*/

void CButtonST::SetHitTestStyle(LRESULT style)
{
	m_HitTestStyle = style;
    //处理滑块不响应双击
	if(m_HitTestStyle == HTCAPTION) 
	{

   	    SetAutoRepeat(FALSE,5000) ;
	
	}


}


/*
*--------------------------------------------------------------------------------
*  成员函数名   :OnSize
*  功能描述     ：设置按纽尺寸调整时其父视窗体也要相应调整
*  输入参数描述	:
*  UINT nType:
*  int x:
*  int cy:
*  返回变量说明 :
*  算法描述		:
*  附加说明     : 
*--------------------------------------------------------------------------------
*/
void CButtonST::OnSize(UINT nType, int cx, int cy) 
{
	CButton::OnSize(nType, cx, cy);
	if(m_HitTestStyle != HTCLIENT)
	{
		CRect crect;
		GetClientRect(&crect);
		CRect prect;

		int		nButtonID;
		HWND	hWndParent;
		nButtonID = GetDlgCtrlID();
		hWndParent = GetParent()->GetSafeHwnd();
		::SendMessage(hWndParent, WM_SIZE, 0, 0);
		
		MoveWindow(CRect(0,0,16,16),TRUE);
	}
	
	
}


/*
*--------------------------------------------------------------------------------
*  成员函数名   :OnMoving
*  功能描述     ：设置按纽尺寸调整时其父视窗体也要相应调整
*  输入参数描述	:
*  UINT fwSide:窗体移动时的边缘
*  LPRECT pRect:窗体移动时正处的矩形区大小指针
*  返回变量说明 :
*  算法描述		:
*  附加说明     : 
*--------------------------------------------------------------------------------
*/
void CButtonST::OnMoving(UINT fwSide, LPRECT pRect) 
{
   CRect frect;
   CCPHDlg * maindlg = (CCPHDlg *)GetParent();  
   maindlg->GetWindowRect(&frect);
   /*
   if(frect.left < 680)
         frect.left = 680;
   if(frect.left > 790)
         frect.left = 790;
   MoveWindow(frect.left, 399, frect.left + 10, 399 + 16);
   */
    pRect->top = 394 + frect.top;
	pRect->bottom = pRect->top + 16;
	if(pRect->left < (680 + frect.left))
		pRect->left= 680 + frect.left;
	else if(pRect->left > (790 + frect.left))
		pRect->left = 790 + frect.left;
	
//	CButton::OnMoving(fwSide, pRect);
   // CCPHDlg *pParent = (CCPHDlg *)GetParent();
   //pParent->MoveSizeScroll(pRect); 
	::PostMessage(GetParent()->GetSafeHwnd(), WM_MOVESIZEBTN, pRect->left - frect.left, 0);
	
}


/*
*--------------------------------------------------------------------------------
*  成员函数名   :SetShowHelp
*  功能描述     ：设置显示的文字图象窗口
*  输入参数描述	:
*    int nIndex :要采用的帮助图片的索引号
*    CRect rect :图片显示的矩形区
*  返回变量说明 :
*  算法描述		:
*  附加说明     : 
*--------------------------------------------------------------------------------
* 版本变更记录:
*
*   v1.1  2002-11-24  9:30     
*/
void CButtonST::SetShowHelp(int nIndex, CRect rect)
{
	const int helpWidth = 100, helpHeight = 70;
	m_nIndex = nIndex;
    if(nIndex < 11)
	{
		m_rectHelp.left  = rect.right - helpWidth / 3;
		m_rectHelp.right = rect.right + helpWidth * 2 / 3;
		m_rectHelp.top   = rect.bottom - helpHeight / 4;
		m_rectHelp.bottom = rect.bottom + helpHeight * 3 / 4;
	}
	if(nIndex >= 11 && m_nIndex <17) 
	{
		m_rectHelp.left  = rect.right - helpWidth  / 4;
		m_rectHelp.right = rect.right + helpWidth * 3 / 4;
		m_rectHelp.top   = rect.top - helpHeight;
		m_rectHelp.bottom = rect.top;
	}
	if(m_nIndex >= 17) 
	{
		m_rectHelp.left  = rect.right - helpWidth / 2;
		m_rectHelp.right = rect.right + helpWidth / 2;
		m_rectHelp.top   = rect.top - helpHeight * 2 / 3;
		m_rectHelp.bottom = rect.top + helpHeight / 3;
	}


}


/*
*--------------------------------------------------------------------------------
*  成员函数名   :OnTimer
*  功能描述     ：控制显示文字图象窗口的开始及结束时间
*  输入参数描述	:
*    UINT nIDEvent：时间事件号
*  返回变量说明 :
*  算法描述		:
*  附加说明     : 
*--------------------------------------------------------------------------------
* 版本变更记录:
*
*   v1.1  2002-11-24  9:30     
*/
void CButtonST::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	CString mp3File;
	if(nIDEvent != 1 || m_nCount == -1) return;
    m_nCount ++;
	CCPHDlg *pParent = (CCPHDlg *)GetParent(); 
    if(m_nCount == 1)
	{
        if(pParent->GetTextStyle() == TRUE)
		{
			pParent->m_showHelp.MoveWindow(m_rectHelp,TRUE); 
		/*	CRect rect;
			rect.left = m_mousePoint.x;
			rect.right = m_mousePoint.x + m_rectHelp.Width();
			rect.top = m_mousePoint.y + m_rectHelp.Width();
	         pParent->m_showHelp.MoveWindow(rect,TRUE);*/
			pParent->m_showHelp.Show(TRUE,m_nIndex);
		}
		if(pParent->GetVoiceStyle() == TRUE)
		{
			CString str;
			if (m_nIndex > 9)
				str.Format("%2d", m_nIndex);
			else 
				str.Format("%1d", m_nIndex);
			str += _T(".mp3");
			TRACE("%s",str);
            mp3File = g_strAppPath + "\\Sounds\\" + str;
			PlayTipSound(mp3File);	
		}
		
	}
	if(m_nCount == 5)
	{
		m_nCount = -1;
		//关闭计数器
		KillTimer(1);
		//关闭动态提示框
        if(pParent->GetTextStyle() == TRUE)
		    pParent->m_showHelp.Show(FALSE, m_nIndex);
		if(pParent->GetVoiceStyle() == TRUE)
			StopPlayTipSound();
	}		

}

/*
*--------------------------------------------------------------------------------
*  成员函数名   :OnNcLButtonDblClk
*  功能描述     ：对滑块屏蔽双击按纽消息
*  输入参数描述	:
*   UINT nHitTest  :其它按键情况
*   CPoint point：双击点的坐标
*  返回变量说明 :
*  算法描述		:
*  附加说明     : 
*--------------------------------------------------------------------------------
*/
void CButtonST::OnNcLButtonDblClk(UINT nHitTest, CPoint point) 
{
	if(m_HitTestStyle == HTCAPTION)
		return;	

}
/////////////////完///////////////////
//当鼠标在按纽上停留2秒时播放一提示声音
void CButtonST::PlayTipSound(CString fileName)
{

}
// 停止播放一提示声音
void CButtonST::StopPlayTipSound()
{

}

//显示一上拉菜单
void CButtonST::OnRButtonDown(UINT nFlags, CPoint point) 
{
	INT id = GetDlgCtrlID();

	::SendMessage(GetParent()->GetSafeHwnd(),UWM_RBUTTONDOWN,(long)&point,id);
	CButton::OnRButtonDown(nFlags, point);
}

//用于得到当前控件的ID号
INT CButtonST::GetBtnID()
{
  return GetDlgCtrlID();
}



#undef ST_LIKEIE












