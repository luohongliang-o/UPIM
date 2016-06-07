/////////////////////////////////////////////////////////////////////////////
/**
* 文件名  ： BitmapDialog.cpp
* 版本号  ： V1.0
* 项目名称： CPH
* 版权声明： Copyright 2002-2003 . All Rights Reserved.
* 模块简单描述：使用真彩色作为对话框的背景图，并可将对话框最大化，占据整个屏幕
*======================================================
* 版本变更记录:
*      v1.0  2002-10-30   20:30  罗伟  根据一些参考资料完成本类
*
* 
*/

/////////////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "..\Include\BitmapDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////


/*
*--------------------------------------------------------------------------------
*  成员函数名   :CBitmapDialog
*  功能描述     :构造函数
*  输入参数描述	:
*  返回变量说明 :
*  算法描述		:
*  附加说明:
*--------------------------------------------------------------------------------
*/
CBitmapDialog::CBitmapDialog()
{
	CommonConstruct();
	Init(); 
}

CBitmapDialog::CBitmapDialog(UINT uResource, CWnd* pParent /*=NULL*/)
	: CDialog(uResource, pParent)
{
	CommonConstruct();
	Init(); 
}

CBitmapDialog::CBitmapDialog(LPCTSTR pszResource, CWnd* pParent /*=NULL*/)
	: CDialog(pszResource, pParent)
{
	CommonConstruct();
	Init(); 
}

/*
*--------------------------------------------------------------------------------
*  成员函数名   :CommonConstruct()
*  功能描述     :公用的构造，用于一些变量的初始化
*  输入参数描述	:
*  返回变量说明 :
*  算法描述		:
*  附加说明:
*--------------------------------------------------------------------------------
*/
void CBitmapDialog::CommonConstruct()
{
	//默认为拉伸类型
	m_nType = BITMAP_STRETCH;
	VERIFY(m_HollowBrush.CreateStockObject(HOLLOW_BRUSH));

	//{{AFX_DATA_INIT(CBitmapDialog)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

/*
*--------------------------------------------------------------------------------
*  成员函数名   :SetBitmap
*  功能描述     :为对话框设置位图底图
*  输入参数描述	:
*   UINT uResource：位图资源的ID号
*   int nType：      位图在对话框中的显示方式，默认为拉伸
*  返回变量说明 :设置成功返回TRUE
*  算法描述		:
*  附加说明:
*--------------------------------------------------------------------------------
*/
BOOL CBitmapDialog :: SetBitmap(UINT uResource, int nType /*BITMAP_STRETCH*/) {
	m_nType = nType;
	ASSERT(m_nType == BITMAP_TILE || m_nType == BITMAP_STRETCH || m_nType == BITMAP_CENTER);
	return m_bmpBackground.LoadResource(uResource);
}

/*
*--------------------------------------------------------------------------------
*  成员函数名   :SetBitmap
*  功能描述     :为对话框设置位图底图
*  输入参数描述	:
*   LPCSTR lpszBitmap：外部位图文件指针
*   int nType：        位图在对话框中的显示方式，默认为拉伸
*  返回变量说明 :
*  算法描述		:
*  附加说明:如调用说明，前置条件，后置条件。
*--------------------------------------------------------------------------------
*/
BOOL CBitmapDialog :: SetBitmap(LPCSTR lpszBitmap, int nType /*BITMAP_STRETCH*/) {
	m_nType = nType;
	ASSERT(m_nType == BITMAP_TILE || m_nType == BITMAP_STRETCH || m_nType == BITMAP_CENTER);
	return m_bmpBackground.LoadBitmap((LPCTSTR)lpszBitmap);
}

/*
*--------------------------------------------------------------------------------
*  成员函数名   :DoDataExchange
*  功能描述     :
*  输入参数描述	:
*  返回变量说明 :
*  算法描述		:
*  附加说明:
*--------------------------------------------------------------------------------
*/
void CBitmapDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBitmapDialog)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBitmapDialog, CDialog)
	//{{AFX_MSG_MAP(CBitmapDialog)
	ON_WM_ERASEBKGND()
	ON_WM_CTLCOLOR()
	ON_WM_QUERYNEWPALETTE()
	ON_WM_PALETTECHANGED()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBitmapDialog 类消息句柄
/////////////////////////////////////////////////////////////////////////////
/*
*--------------------------------------------------------------------------------
*  成员函数名   :OnEraseBkgnd
*  功能描述     :为对话框设置背景色
*  输入参数描述	:
*  CDC* pDC  ：绘画设备上下文
*  返回变量说明 :设置成功返回TRUE
*  算法描述		:
*  附加说明:如调用说明，前置条件，后置条件。
*--------------------------------------------------------------------------------
*/
BOOL CBitmapDialog::OnEraseBkgnd(CDC* pDC) 
{
	if(m_bmpBackground.GetPixelPtr() != 0) {
		ASSERT(m_nType == BITMAP_TILE || m_nType == BITMAP_STRETCH || m_nType == BITMAP_CENTER);
		
		CRect rc;
		GetClientRect(rc);
		int x = 0, y = 0;

		switch(m_nType) {
			case BITMAP_CENTER:   //位图居中显示
				CDialog::OnEraseBkgnd(pDC);
				x = (rc.Width() - m_bmpBackground.GetWidth()) / 2;
				y = (rc.Height() - m_bmpBackground.GetHeight()) / 2;
				m_bmpBackground.DrawDIB(pDC, x, y);
				break;

			case BITMAP_STRETCH:    //位图拉伸显示以便它填充整个对话框
				m_bmpBackground.DrawDIB(pDC, 0, 0, rc.Width(), rc.Height());
				break;

			default:	// 填补满对话框
			
				while(y < rc.Height()) {
					while(x < rc.Width()) {
						m_bmpBackground.DrawDIB(pDC, x, y);
						x += m_bmpBackground.GetWidth();
					}
					x = 0;
					y += m_bmpBackground.GetHeight();
				}
				break;
		}
	} else
		// 无位图时显示常规对话框
		return CDialog::OnEraseBkgnd(pDC);

	return TRUE;
}

/*
*--------------------------------------------------------------------------------
*  成员函数名   :OnCtlColor
*  功能描述     :
*  输入参数描述	:
*      CDC* pDC：
*      CWnd* pWnd：
*      UINT nCtlColor：
*  返回变量说明 :
*  算法描述		:
*  附加说明:
*--------------------------------------------------------------------------------
*/
HBRUSH CBitmapDialog::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	if(m_bmpBackground.GetPixelPtr() != 0) {
		switch(nCtlColor) {
			case CTLCOLOR_STATIC:
				TCHAR lpszClassName[255];
				GetClassName(pWnd->m_hWnd, lpszClassName, 255);
				if(_tcscmp(lpszClassName, TRACKBAR_CLASS) == 0)
					return CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

			case CTLCOLOR_BTN:
				pDC->SetBkMode(TRANSPARENT);
				return HBRUSH(m_HollowBrush);

			default:
				break;
		}
	}
	
	//提示：若我们按这条线延伸，可以使用画刷等以实现更强大的功能
	return CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
}

/*
*--------------------------------------------------------------------------------
*  成员函数名   :OnQueryNewPalette
*  功能描述     :查询新调色板
*  输入参数描述	:
*  返回变量说明 :成功返回TRUE
*  算法描述		:
*  附加说明:
*--------------------------------------------------------------------------------
*/
BOOL CBitmapDialog::OnQueryNewPalette() 
{
	CPalette * pPal = m_bmpBackground.GetPalette();
	if( pPal != 0 && GetSafeHwnd() != 0 ) {
		CClientDC dc(this);
		CPalette * pOldPalette = dc.SelectPalette(pPal, FALSE);
		UINT nChanged = dc.RealizePalette();
		dc.SelectPalette(pOldPalette, TRUE);

		if (nChanged == 0)
			return FALSE;

		Invalidate();
		return TRUE;
	}
	return CDialog::OnQueryNewPalette();
}

/*
*--------------------------------------------------------------------------------
*  成员函数名   :
*  功能描述     :当调色板变化时
*  输入参数描述	:
*   CWnd* pFocusWnd：当前焦点窗口的句柄
*  返回变量说明 :
*  算法描述		:
*  附加说明:
*--------------------------------------------------------------------------------
*/
void CBitmapDialog::OnPaletteChanged(CWnd* pFocusWnd) 
{
	CPalette * pPal = m_bmpBackground.GetPalette();
	if( pPal != 0 && GetSafeHwnd() != 0 && pFocusWnd != this && ! IsChild(pFocusWnd) ) {
		CClientDC dc(this);
		CPalette * pOldPalette = dc.SelectPalette(pPal, TRUE);
		UINT nChanged = dc.RealizePalette();
		dc.SelectPalette(pOldPalette, TRUE);

		if( nChanged )
			Invalidate();
	} else
		CDialog::OnPaletteChanged(pFocusWnd);
}



/////////////////////////////////////////////////////////////////////////////////
//以下消息函数用于实现全屏幕显示
////////////////////////////////////////////////////////////////////////////////
/*
*--------------------------------------------------------------------------------
*  成员函数名   :OnDestroy()
*  功能描述     :对话框释放时
*  输入参数描述	:
*  返回变量说明 :
*  算法描述		:
*  附加说明:
*--------------------------------------------------------------------------------
*/
void CBitmapDialog::OnDestroy() 
{
	//用于刷新显示底图
	ShowWindow(SW_SHOW);
	ShowWindow(SW_HIDE);

	CDialog::OnDestroy();
}

/*
*--------------------------------------------------------------------------------
*  成员函数名   :ThrowOpenDlg()
*  功能描述     :使对话框最大化，并隐藏windows桌面上的状态条
*  输入参数描述	:
*  返回变量说明 :
*  算法描述		:
*  附加说明:
*--------------------------------------------------------------------------------
*/
void CBitmapDialog::ThrowOpenDlg()
{
	ZeroMemory(&m_WorkArea,sizeof(m_WorkArea));

	if(::SystemParametersInfo(SPI_GETWORKAREA,
							  0,
							  &m_WorkArea,
							  0))
	{
		::SystemParametersInfo(SPI_SETWORKAREA,
							  0,
							  NULL,
							  SPIF_SENDCHANGE);
	}

	if(!m_bIsToolBarHidden)
	{
		CWnd* pTaskBar = CWnd::FindWindow(_T("Shell_TrayWnd"),_T(""));
		if(pTaskBar && ::IsWindow(*pTaskBar))
		{
			m_bWasToolBarVisible = pTaskBar->IsWindowVisible();
			pTaskBar->ShowWindow(SW_HIDE);
			m_bIsToolBarHidden = TRUE;
		}
	}

	if(!m_bAreDisplaySettingsChanged)
	{
		DWORD dmPelsWidth = m_DlgRect.Width();
		DWORD dmPelsHeight = m_DlgRect.Height();

		m_bAreDisplaySettingsSaved = ::EnumDisplaySettings(NULL,
														   ENUM_REGISTRY_SETTINGS,
														   &m_SaveDevMode);

		BOOL bIsMaxColorDepth = FALSE;
		DEVMODE MaxColorDepthDevMode;
		ZeroMemory(&MaxColorDepthDevMode,sizeof(MaxColorDepthDevMode));
		MaxColorDepthDevMode.dmSize = sizeof(MaxColorDepthDevMode);

		BOOL bIsTheSameColorDepth = FALSE;
		DEVMODE TheSameColorDepth;
		ZeroMemory(&TheSameColorDepth,sizeof(TheSameColorDepth));
		TheSameColorDepth.dmSize = sizeof(TheSameColorDepth);

		DEVMODE TempDevMode;
		DWORD dwModeNum;

		for(dwModeNum=0;;dwModeNum++)
		{
			ZeroMemory(&TempDevMode,sizeof(TempDevMode));
			TempDevMode.dmSize = sizeof(TempDevMode);

			if(!::EnumDisplaySettings(NULL,
									  dwModeNum,
									  &TempDevMode))
				break;

			if(TempDevMode.dmPelsWidth != dmPelsWidth ||
			   TempDevMode.dmPelsHeight != dmPelsHeight)
				continue;

			if(TempDevMode.dmBitsPerPel > MaxColorDepthDevMode.dmBitsPerPel)
			{
				MaxColorDepthDevMode = TempDevMode;
				bIsMaxColorDepth = TRUE;
			}

		   if(!bIsTheSameColorDepth)
		   {
			   TheSameColorDepth = TempDevMode;
			   bIsTheSameColorDepth = TRUE;
		   }
		}

		m_bAreDisplaySettingsChanged = bIsMaxColorDepth &&
									   ::ChangeDisplaySettings(&MaxColorDepthDevMode,0) == DISP_CHANGE_SUCCESSFUL ||
									   bIsTheSameColorDepth &&
									   ::ChangeDisplaySettings(&TheSameColorDepth,0) == DISP_CHANGE_SUCCESSFUL;
	}

	ShowWindow(SW_SHOWNORMAL);

	CenterWindow();
}


/*
*--------------------------------------------------------------------------------
*  成员函数名   :WrapUpDlg()
*  功能描述     :将已经最大化全桌面的对话框回原，同时显示被隐藏的windows桌面状态栏
*  输入参数描述	:
*  返回变量说明 :
*  算法描述		:
*  附加说明:
*--------------------------------------------------------------------------------
*/
void CBitmapDialog::WrapUpDlg()
{
	if(m_bAreDisplaySettingsChanged && m_bAreDisplaySettingsSaved)
		::ChangeDisplaySettings(&m_SaveDevMode,0);

	m_bAreDisplaySettingsChanged = m_bAreDisplaySettingsSaved = FALSE;

	if(m_bIsToolBarHidden)
	{
		CWnd* pTaskBar = CWnd::FindWindow(_T("Shell_TrayWnd"),_T(""));
		if(pTaskBar && 
		   ::IsWindow(*pTaskBar) &&
		   m_bWasToolBarVisible)
			pTaskBar->ShowWindow(SW_SHOW);
	}

	if(!CRect(m_WorkArea).IsRectEmpty())
		::SystemParametersInfo(SPI_SETWORKAREA,
							  0,
							  &m_WorkArea,
							  SPIF_SENDCHANGE);

	m_bIsToolBarHidden = m_bWasToolBarVisible = FALSE;

    ShowWindow(SW_SHOWMAXIMIZED);
 
}


/*
*--------------------------------------------------------------------------------
*  成员函数名   :OnInitDialog() 
*  功能描述     :初始化对话框
*  输入参数描述	:
*  返回变量说明 :初始化成功返加TRUE
*  算法描述		:
*  附加说明:
*--------------------------------------------------------------------------------
*/
BOOL CBitmapDialog::OnInitDialog() 
{
	ModifyStyle(WS_THICKFRAME |
				WS_BORDER     |
				WS_CAPTION,
				WS_SYSMENU);

	GetClientRect(m_DlgRect);

	CDialog::OnInitDialog();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


/*
*--------------------------------------------------------------------------------
*  成员函数名   :Init()
*  功能描述     :自定义的额外初始化函数
*  输入参数描述	:
*  返回变量说明 :
*  算法描述		:
*  附加说明:
*--------------------------------------------------------------------------------
*/
void CBitmapDialog::Init()
{
	m_bAreDisplaySettingsSaved = FALSE;
	m_bAreDisplaySettingsChanged = FALSE;
	m_bIsToolBarHidden = FALSE;
	m_bWasToolBarVisible = FALSE;

	ZeroMemory(&m_SaveDevMode,sizeof(m_SaveDevMode));
	m_SaveDevMode.dmSize = sizeof(m_SaveDevMode);

	ZeroMemory(&m_WorkArea,sizeof(m_WorkArea));
}


/*
*--------------------------------------------------------------------------------
*  成员函数名   :OnActivateApp
*  功能描述     :控制桌面的窗口的活动情况
*  输入参数描述	:
*    BOOL bActive：窗口是否活动
*    HTASK hTask： 桌面句柄
*  返回变量说明 :
*  算法描述		:
*  附加说明:
*--------------------------------------------------------------------------------
*/
void CBitmapDialog::OnActivateApp(BOOL bActive, HTASK hTask) 
{
	if(bActive)
		ThrowOpenDlg();
	else
		WrapUpDlg();

	CDialog::OnActivateApp(bActive, (DWORD)hTask);	
}
