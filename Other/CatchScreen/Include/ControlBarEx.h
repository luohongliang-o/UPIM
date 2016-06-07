/////////////////////////////////////////////////////////////////////////////
/**本类没有被应用程序使用，功能没有得到扩展，只为今后提供扩充之用，若决定不用，可以删除该类
* 文件名  ：ControlBarEx.h
* 版本号  ：V1.0
* 项目名称：CPH
* 版权声明：Copyright 2002-2003 . All Rights Reserved.
* 模块简单描述：浮动工具控件面板控件类头文件
*======================================================
* 版本变更记录:
*      v1.0  2002-10-31   16：00  
*
* 
*/

/////////////////////////////////////////////////////////////////////////////


#if !defined(AFX_CONTROLBAREX_H__7E293642_EDB2_11D6_95DA_000795D2C55B__INCLUDED_)
#define AFX_CONTROLBAREX_H__7E293642_EDB2_11D6_95DA_000795D2C55B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ControlBarEx.h : header file
//

#include "BitmapDialog.h"
#include "Canvas.h"
#include "ButtonST.h"
/////////////////////////////////////////////////////////////////////////////
/*
*类名    : CControlBarEx
*说明    : 浮动工具栏类
*变更记录:（暂不用）
*
*/

class CControlBarEx : public CBitmapDialog
{
// Construction
public:
	CControlBarEx(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CControlBarEx)
	enum { IDD = IDD_CONTROLBAR };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CControlBarEx)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CControlBarEx)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CONTROLBAREX_H__7E293642_EDB2_11D6_95DA_000795D2C55B__INCLUDED_)
