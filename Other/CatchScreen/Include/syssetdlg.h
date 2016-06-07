/////////////////////////////////////////////////////////////////////////////
/**
* 文件名  ：SysSetDlg.h
* 版本号  ：V1.0
* 项目名称：CPH
* 版权声明：Copyright 2002-2003 . All Rights Reserved.
* 模块简单描述：系统设置对话框头文件
*======================================================
* 版本变更记录:
*      v1.0  2002-11-18   14：00   罗伟
*
* 
*/

/////////////////////////////////////////////////////////////////////////////


#if !defined(AFX_SYSSETDLG_H__E71C6803_FAED_11D6_95DA_000795D2C55B__INCLUDED_)
#define AFX_SYSSETDLG_H__E71C6803_FAED_11D6_95DA_000795D2C55B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "BitmapDialog.h"
#include "ButtonST.h"

#define BKMUSICON CRect(158, 111, 167, 120)
#define BKMUSICOFF CRect(158, 141, 167, 150)
#define MUSIC0 CRect(235, 78, 244, 87)
#define MUSIC1 CRect(235, 103, 244, 112)
#define MUSIC2 CRect(235, 128, 244, 137)
#define MUSIC3 CRect(235, 153, 244, 162)
#define MUSIC4 CRect(235, 178, 244, 187)
#define VOICEON CRect(158, 241, 167, 250)
#define VOICEOFF CRect(158, 271, 167, 280)
#define TXTON CRect(332, 241, 341, 250)
#define TXTOFF CRect(332, 271, 341, 280)
#define SETCANCEL   CRect(230, 20, 230 + 65, 20 + 34)
#define SETOK   CRect(310, 20, 310 + 65, 20 + 34)


/////////////////////////////////////////////////////////////////////////////
// CSysSetDlg dialog
class CCPHDlg;
class CSysSetDlg : public CBitmapDialog
{
// Construction
public:
	
	void InitButton();
	CSysSetDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSysSetDlg)
	enum { IDD = IDD_SYSSET_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSysSetDlg)
	public:
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSysSetDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:


	CButtonST m_btnBkMusic;
	CButtonST m_btnMusic;
	CButtonST m_btnVoice;
	CButtonST m_btnTxt;
	CButtonST m_btnOk;
	CButtonST m_btnCancel;

   CCPHDlg *m_pMainWnd;

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SYSSETDLG_H__E71C6803_FAED_11D6_95DA_000795D2C55B__INCLUDED_)
