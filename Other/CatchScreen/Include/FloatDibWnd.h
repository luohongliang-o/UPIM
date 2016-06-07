/**
*   FloatDibWnd.h		
*   Version 1.0
*   CPH	绘画乐园
*   Copyright 2002 . All Rights Reserved.
*   定义选取工具的抽象基类
*======================================================
*   版本
*   Revision 1.0	2002/11/14    	  定义浮动窗口类
*/

#if !defined(AFX_FLOATDIBWND_H__4C7B2E0E_42D4_4BB6_A954_E66A15C65264__INCLUDED_)
#define AFX_FLOATDIBWND_H__4C7B2E0E_42D4_4BB6_A954_E66A15C65264__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include  "Dib.h"
#include  "ENUM.h"
#include  "ButtonST.h"


typedef enum enuCHAGNE_TYPE
{
	CT_MOVE,			// 移动
	CT_ROTATE,			// 旋转
	CT_HORMIRROR,		// 水平镜像
	CT_VERMIRROR,		// 垂直镜像,即翻转
	CT_RESIZE				// 缩放
}CHANGE_TYPE;



/*
*类名: CFloatDibWnd
*说明：定义浮动窗口类
*版本
*	Revision 1.0	2002/11/14     	 完成 CFloatDibWnd 的基本功能
*/
class CFloatDibWnd : public CWnd  
{	
//	CRect     m_preRect;     //当前客户区矩形区尺寸


public:
	// 四个形变按钮
	CButtonST  m_btnRotate;	
	CButtonST  m_btnVerMirror;
	CButtonST  m_btnHorMirror;
	CButtonST  m_btnResize;


protected:
	CDib * m_pDib;			// 当前绘图DIB
	CDib * m_pSrcDib;		// 原始的DIB图像，以便多次变换时失真最小
	CDib * m_pHistoryDib;   // 保存操作前的图像，以便Undo
	BOOL m_bCartoonWnd;
	
	// 浮动窗口相对父窗口的屏幕位置
	CRect m_rcWndRect;

	HWND m_hParentWnd;
	CHANGE_TYPE	m_ChangeType;
	CPoint m_ptLast;

	// 支持 undo 和 redo 的数据成员
	CRect m_rcHistoryWndRect;
	CHANGE_TYPE m_HistoryChangeType;


public:
	void CheckShowBtn();
	void SetCreateBtn(BOOL bCreateBtn = TRUE);
	BOOL IsCartoonWnd();
	CFloatDibWnd(HWND hParentWnd, const CString strFileName,INT curDrawSize=55);
	CFloatDibWnd(HWND hParentWnd, const CRect& rect,INT curDrawSize=55);
	CFloatDibWnd(HWND hParentWnd, CDib* pDib,INT curDrawSize=55);
	virtual ~CFloatDibWnd();
	BOOL CreateWnd();	

	void SetCartoonDib(CString strDibFileName);


	void DrawRotateBorder(double angle = 0);
	BOOL MergeDib(CDib * pDib);

	void HideAllButton();

	CDib* Copy();
	CDib* Cut();


protected:
	BOOL CreateButton();
	DOUBLE GetRotateAngle(CPoint ptEnd);
	CPoint PointToParentClient(const CPoint& point);
	CPoint GetArcEndPoint(const CPoint &ptCenter, double dwRadius, double eStartAngle, double eSweepAngle) const;
	CRect RectToParent();
	CRect RectToParent(const CPoint& point);
	CRect RectToParentClient();
	CRect RectToParentClient(const CPoint& point);
	void DrawResizeBorder(const CRect& rect) const;


protected:
	double m_dblPreRotateAngle;
	CPoint CountPoint(CPoint pointCenter, CPoint pointSide, double angle);
	BOOL RotateAngle(double angle);
	//{{AFX_MSG(CCartoon)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
	afx_msg void OnMove(int x, int y);
	afx_msg UINT OnNcHitTest(CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnBeginChange(WPARAM wParam, LPARAM lParam);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
   INT 	m_curDrawSize; //1-110
};

#endif // !defined(AFX_FLOATDIBWND_H__4C7B2E0E_42D4_4BB6_A954_E66A15C65264__INCLUDED_)
