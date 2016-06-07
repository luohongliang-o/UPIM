
/*
*  类名：class CShowWnd
*  说明：具有透明度的动态提示框
*  版本变更记录
*  CPH 1.0 2002/11/13    创建
*
*/

#if !defined(AFX_SHOWWND_H__D3114AD0_F87B_11D6_95DC_000795CD4EC5__INCLUDED_)
#define AFX_SHOWWND_H__D3114AD0_F87B_11D6_95DC_000795CD4EC5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CShowWnd window

class CShowWnd : public CWnd
{
// Construction
public:
	CShowWnd();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CShowWnd)
	//}}AFX_VIRTUAL

// Implementation
public:
	void     Show (BOOL isVisible, int nIndex);
	virtual  ~CShowWnd();
private:
	BITMAP   m_bitmap;
	HBITMAP  m_hBitmap[22];
	int m_nIndex;
	// Generated message map functions
protected:
	//{{AFX_MSG(CShowWnd)
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SHOWWND_H__D3114AD0_F87B_11D6_95DC_000795CD4EC5__INCLUDED_)
