//Download by http://www.NewXing.com
// CatchScreenDlg.h : header file
//

#if !defined(AFX_CATCHSCREENDLG_H__536FDBC8_7DB2_4BEF_8943_70DBE8AD845F__INCLUDED_)
#define AFX_CATCHSCREENDLG_H__536FDBC8_7DB2_4BEF_8943_70DBE8AD845F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "Resource.h"
// 
// #ifndef MYTRACKER_
// #include "MyTracker.h"
// #endif

#include "Include\PngImage.h"
#include "Include\PaintDlg.h"
#include "afxwin.h"
#include "Include\canvas.h"
/////////////////////////////////////////////////////////////////////////////
// CCatchScreenDlg dialog

void WriteDebugMessage
	(
	LPCSTR p_lpcszFileNamePrifix,	// [in]文件名前缀(填空则默认为"DB"--Debug)
	LPCSTR p_lpcszFormat,			// [in]输出格式
	...							// [in]输出内容
	);
#ifndef WM_SENDTOEXEEXIT
#define WM_SENDTOEXEEXIT  (WM_APP + 0x994)
#endif

class CCatchScreenDlg : public CDialog
{
public:
	HBITMAP CopyScreenToBitmap(LPRECT lpRect,BOOL bSave=FALSE);   //考贝桌面到位图
	void DrawTip();                            //显示操作提示信息
	void DrawMessage(CRect &inRect,CDC * pDC);       //显示截取矩形信息
	//void PaintWindow();               //重画窗口

//**********************************************************************
// Construction
public:
	void DrawImage();
	void saveToFile(CString strSaveFile="");
	CCatchScreenDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CCatchScreenDlg)
	enum { IDD = IDD_CATCHSCREEN_DIALOG };
	//CMyEdit	m_tipEdit;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCatchScreenDlg)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CCatchScreenDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnClose();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
protected:
	void DrawToolBar(CDC &dc,CRect rc);
	void SetCorrectRect(CRect &rc);
	void DrawDot(CDC &dc);
protected:
	CPngImage2	m_pngMask;
	CPngImage2	m_pngDot;
	CPngImage2	m_pngAction;
	CRect       m_ToolBarRect;
	CRect       m_rcAction[9];
	LONG64      m_iActionMask;
	CPaintDlg   m_PaintDlg[5];
	int         m_DlgShowIndex;
	BOOL        m_bLButtonUp;
	int			m_iCapture;
	BOOL		m_bLeftDown;
	CPoint		m_ptLButtonDown;
	BOOL        m_bTopDownFlag;
	UINT		m_curSel[9];
	BOOL		m_bSelect;
	BOOL		m_bSelected;
	int         m_xScreen;
	int         m_yScreen;
	CBitmap   * m_pBitmap;            //背景位图
	HCURSOR     m_hCursor;//光标
//	Canvas      m_DrawRect;
	BOOL        m_bPanitCanvas;
public:
	CRect		m_rcSel[9];
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CATCHSCREENDLG_H__536FDBC8_7DB2_4BEF_8943_70DBE8AD845F__INCLUDED_)
