/////////////////////////////////////////////////////////////////////////////
/**
* 文件名  ：BitmapDialog.h
* 版本号  ：v1.0
* 项目名称：CPH
* 版权声明：Copyright 2002-2003 . All Rights Reserved.
* 模块简单描述：创建一个用位图作为对话框背景的对话框类的头文件
*                这个类为绘画乐园所有的彩色图形对话框的基类
*======================================================
* 版本变更记录:
*      v1.0  2002-10-30   20:30   根据一些参考资料完成本类
*
* 
*/

/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_BITMAPDIALOG_H__55BE67E3_17E6_11D2_9AB8_0060B0CDC13E__INCLUDED_)
#define AFX_BITMAPDIALOG_H__55BE67E3_17E6_11D2_9AB8_0060B0CDC13E__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "DIBitmap.h"	// Added by ClassView

/////////////////////////////////////////////////////////////////////////////
/*
*类名    : CBitmapDialog
*说明    : 用位图作为对话框的背景的类(支持底图拉伸、并列、居中显示，支持对话框最大化)
*变更记录:
*
*/

class  CBitmapDialog : public CDialog
{
// Construction
public:
	CBitmapDialog();         
	CBitmapDialog(LPCTSTR lpszTemplateName, CWnd* pParentWnd = NULL);  //++++                    
   CBitmapDialog(UINT nIDTemplate, CWnd* pParentWnd = NULL);     //++++
	                          
protected:  
	RECT m_WorkArea;
	virtual void WrapUpDlg();
	virtual void ThrowOpenDlg();

	BOOL m_bAreDisplaySettingsChanged;
	BOOL m_bAreDisplaySettingsSaved;
	DEVMODE m_SaveDevMode;

	BOOL m_bIsToolBarHidden;
	BOOL m_bWasToolBarVisible;

	CRect m_DlgRect;

	void Init();
public:
	enum {		
		BITMAP_STRETCH=0,		// 位图以拉伸方式填充对话框
		BITMAP_TILE,		   // 位图以自身尺寸填满对话框
		BITMAP_CENTER			//位图居中显示在对话框中
	};


// Attributes
public:
	BOOL	SetBitmap(UINT uResource, int Type = CBitmapDialog::BITMAP_TILE);
	BOOL	SetBitmap(LPCSTR lpszBitmap, int Type = CBitmapDialog::BITMAP_TILE);

// Dialog Data
protected:
	//{{AFX_DATA(CBitmapDialog)
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
protected:
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBitmapDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CDIBitmap	m_bmpBackground;
	int			m_nType;		
	CBrush		m_HollowBrush;

	// Generated message map functions
	//{{AFX_MSG(CBitmapDialog)
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg BOOL OnQueryNewPalette();
	afx_msg void OnPaletteChanged(CWnd* pFocusWnd);

	//以下三函数用于对话框最大化窗口
	afx_msg void OnDestroy();
	virtual BOOL OnInitDialog();
	afx_msg void OnActivateApp(BOOL bActive, HTASK hTask);

	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	void	CommonConstruct();
};
//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BITMAPDIALOG_H__55BE67E3_17E6_11D2_9AB8_0060B0CDC13E__INCLUDED_)
