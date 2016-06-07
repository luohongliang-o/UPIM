/**
*   ViewBig.h		
*   Version 1.0
*   CPH	绘画乐园
*   Copyright 2002 . All Rights Reserved.
*   定义放大相册内图片查看窗口
*======================================================
*   版本
*   Revision 1.0	2002/11/25    	  定义放大查看窗口类
*/

#if !defined(AFX_VIEWBIG_H__66EEADA8_5E99_423F_8A9F_7EBDF5EAEBFC__INCLUDED_)
#define AFX_VIEWBIG_H__66EEADA8_5E99_423F_8A9F_7EBDF5EAEBFC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ViewBig.h : header file
//

#include "DIB.H"

// 定义图片显示区域常量
const CRect CRectRECT_IMG0 =  CRect(29, 94, 29+160, 94+120);
const CRect CRectRECT_IMG1 =  CRect(214, 94, 214+160, 94+120);
const CRect CRectRECT_IMG2 =  CRect(399, 94, 399+160, 94+120);
const CRect CRectRECT_IMG3 =  CRect(29, 239, 29+160, 239+120);
const CRect CRectRECT_IMG4 =  CRect(214, 239, 214+160, 239+120);
const CRect CRectRECT_IMG5 =  CRect(399, 239, 399+160, 239+120);
const CRect CRectRECT_IMG6 =  CRect(29, 384, 29+160, 384+120);
const CRect CRectRECT_IMG7 =  CRect(214, 384, 214+160, 384+120);
const CRect CRectRECT_IMG8 =  CRect(399, 384, 399+160, 384+120);


/*
*类名: CViewBig
*说明：定义图片放大查看窗口类
*版本
*	Revision 1.0	2002/11/25    	 完成 CViewBig 类的基本功能
*/
class CViewBig : public CWnd
{
public:
	BOOL m_bFirstView;
	CDib * m_srcDib;
	CDib * m_BigFrameDib;
	CDib * m_pDib;

protected:
	BOOL m_bBig;

private:
	//要画边框的图片位置索引
	INT m_nIndex;  
	//是否得到焦点，是否画边框
	BOOL m_bFocus;    
	//是否已画边框
	BOOL m_bDrawedFrm; 
	CString m_strViewFileName;
	CPoint m_point[5];

// Construction
public:
	CViewBig();
	virtual ~CViewBig();

// Implementation
public:
	void ShowWnd();
	void HideWnd();
	void DrawSmall();
	void DrawBig();
	void DrawFrame(BOOL isDraw);
	BOOL GetPreFocus();
	INT GetPrePos();
	void SetPreFocus(BOOL bFocus);
	void SetPrePos(INT index);
	void SetBig(BOOL isBig);
	void SetViewBitmap(CString strfile);

	// Generated message map functions
protected:
	//{{AFX_MSG(CViewBig)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VIEWBIG_H__66EEADA8_5E99_423F_8A9F_7EBDF5EAEBFC__INCLUDED_)
