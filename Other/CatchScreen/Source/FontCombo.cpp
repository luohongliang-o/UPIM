/**
*	File Name			:	FontCombo.cpp
*	Version				:	1.0
*	Project Information	:	绘画乐园(1003-2002)
*	Copyright			:	Copyright 2002 . All Rights Reserved.
*	Brief Description   :	CFontCombo类的函数实现
*====================================================================================
*   版本
*   Revision 1.0	2002/11/11		CFontCombo类的函数实现
*/

#include "stdafx.h"
#include "..\Include\FontCombo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CFontCombo

CFontCombo::CFontCombo()
{
	// 默认所有字体集
	required_charset = DEFAULT_CHARSET;
	m_sSelection = "8";
}

CFontCombo::~CFontCombo()
{
}


BEGIN_MESSAGE_MAP(CFontCombo, CComboBox)
	//{{AFX_MSG_MAP(CFontCombo)
	ON_CONTROL_REFLECT(CBN_SELCHANGE, OnSelchange)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFontCombo message handlers

/*
*--------------------------------------------------------------------------------
*  Member Name:					ProcessFonts
*  Function Description:		初始化喷枪工具
*  Parameter Specification:	    无
*  Return Specification:		无 
*   Algorithm:					无		
*   Addtion:				    
        前置条件：				ComboBox控件对象已被创建
		后置条件：				得到系统所有已安装的字体，并将其储存在ComboBox下拉菜单中
*--------------------------------------------------------------------------------
*/

void CFontCombo::ProcessFonts(void)
{
	this->ResetContent();
	CClientDC dc(this);

	// Determine the size required by the font comboboxes
	// We will use the DEFAULT_GUI_FONT
	HGDIOBJ hFont = GetStockObject( DEFAULT_GUI_FONT );
	CFont font;
	font.Attach( hFont );
	dc.SelectObject( font );

	TEXTMETRIC tm;
	dc.GetTextMetrics( &tm );
	int cxChar = tm.tmAveCharWidth;
	int cyChar = tm.tmHeight + tm.tmExternalLeading;

	SetFont(&font);
	::EnumFontFamilies( dc.m_hDC, NULL, (FONTENUMPROC)EnumFontFamProc, 
				(LPARAM) this );
}

/*
*--------------------------------------------------------------------------------
*  Member Name:					EnumFontFamProc
*  Function Description:		得到字体
*  Parameter Specification:	
        ENUMLOGFONTEX *lpelfe	-逻辑字体数据
		NEWTEXTMETRICEX *lpntme -物理字体数据
		int	FontType			-字体类型
		LPARAM lParam           -EnumFontFamiliesEx 函数自定义数据
*  Return Specification:		无 
*   Algorithm:					无		
*   Addtion:				    
        前置条件：				EnumFontFamiliesEx函数被调用
		后置条件：				将所有字体存储于链表中
*--------------------------------------------------------------------------------
*/

int CALLBACK CFontCombo::EnumFontFamProc(ENUMLOGFONT *lpelf, NEWTEXTMETRIC *lpntm,
					int nFontType, LPARAM lParam)
{
	CFontCombo* pWnd = (CFontCombo*)lParam;

	// Add the font name to the combo
	pWnd->AddString(lpelf->elfLogFont.lfFaceName);

	return 1;		// 1 to continue enumeration
}

void CFontCombo::OnSelchange() 
{
	GetWindowText(m_sSelection);
	int cursel = SelectString(0,m_sSelection);
	SetCurSel(cursel);
    CRect wRect;
	GetWindowRect(wRect);
	AfxGetMainWnd()->InvalidateRect(wRect);	
	
}

CString CFontCombo::GetFontType()
{
	return m_sSelection;
}




