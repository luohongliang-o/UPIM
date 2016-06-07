#ifndef DLL_CONTROLS_CCOLORSTATIC
#define DLL_CONTROLS_CCOLORSTATIC

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ColorStatic.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CColorStatic window

class  CColorStatic : public CStatic
{
// Construction
public:
	CColorStatic();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CColorStatic)
	protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

// Implementation
public:
	void SetColor(COLORREF FonCol=RGB(0,0,0), COLORREF bkCol=RGB(222,223,222), UINT uFormat =DT_LEFT);
	void SetText(CString text, COLORREF  fonCol=RGB(0,0,0), COLORREF bkCol=RGB(222,223,222), UINT uFormat=DT_LEFT);
	void SetTextColor(COLORREF TextColor);
	COLORREF m_TextColor;
	void SetBackColor(COLORREF BackColor);
	void SetCaption(CString strCaption);
	//void Create(CString strCaption,COLORREF BackColor);
	COLORREF m_BackColor;
	CString m_strCaption;

	virtual ~CColorStatic();

	// Generated message map functions
protected:
	//{{AFX_MSG(CColorStatic)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
	//}}AFX_MSG
   
	DECLARE_MESSAGE_MAP()
private:
	UINT m_uFormat; 
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined   DLL_CONTROLS_CCOLORSTATIC
