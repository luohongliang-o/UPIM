/**
*   SelectTool.cpp		
*   Version 1.0
*   CPH	绘画乐园
*   Copyright 2002 . All Rights Reserved.
*   定义选取工具的抽象基类
*======================================================
*   版本
*   Revision 1.0	2002/11/14    	  实现选取工具的抽象基类
*/
#include "stdafx.h"
#include "..\INCLUDE\SelectTool.h"
#include "..\INCLUDE\FloatDibWnd.h"
#include "..\Include\DefineMessage.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSelectTool::CSelectTool(HWND hWnd)
{
	// 所选取客户区窗口句柄
	m_hWnd = hWnd;
	m_hDC = GetDC(hWnd);
}

CSelectTool::~CSelectTool()
{
}


/*
*--------------------------------------------------------------------------------
*  函数名: BeginSelect
*  功能	 : 开始进行区域选取
*  参数  : CPoint ptPoint  - 选取区域的起始点坐标
*  说明  : 选取时先画虚线框
*--------------------------------------------------------------------------------
*/
void CSelectTool::BeginSelect(const CPoint& ptPoint)
{
	m_ptSelectStartPoint = ptPoint;
	m_ptSelectEndPoint = ptPoint;

	// 画出起始的选取框
	DrawRubber(m_ptSelectEndPoint);
}


/*
*--------------------------------------------------------------------------------
*  函数名: ChangeSelect
*  功能	 : 移动鼠标改变选取区域的范围
*  参数  : CPoint ptPoint  - 选取区域的临时结束点坐标
*  说明  : 在此函数中先清除上一次选取时画出的虚框
*--------------------------------------------------------------------------------
*/
void CSelectTool::ChangeSelect(const CPoint& ptPoint)
{
	CPoint pt = ptPoint;

	// 限制选择范围只在画布区内
	CRect rcParent;
	GetClientRect(m_hWnd, &rcParent);

	if (pt.x <= 0)
		pt.x =0;
	if (pt.x >= rcParent.Width())
		pt.x = rcParent.Width();
	if (pt.y <= 0)
		pt.y =0;
	if (pt.y >= rcParent.Height())
		pt.y = rcParent.Height();

	// 擦除前一次画出的选取框
	DrawRubber(m_ptSelectEndPoint);

	// 画出此次的选取框
	DrawRubber(pt);

	m_ptSelectEndPoint = pt;
}


/*
*--------------------------------------------------------------------------------
*  函数名: EndSelect
*  功能	 : 结束区域选取
*  参数  : CPoint ptPoint  - 选取区域的结束点坐标
*  返回值: CFloatDibWnd	 -  选取区域浮动窗口指针，由调用函数负责释放此窗口
*--------------------------------------------------------------------------------
*/
CFloatDibWnd * CSelectTool::EndSelect(const CPoint& ptPoint)
{
	// 擦除前一次画出的选取框
	DrawRubber(m_ptSelectEndPoint);

	// 刷新画布，以防虚框还未擦除就把画布抓下来了
	::SendMessage(m_hWnd, WM_PAINT, 0, 0);

	CFloatDibWnd * pFloatWnd = NULL;

	if (m_ptSelectStartPoint != m_ptSelectEndPoint)
	{
		// 终点不等于起点
		CRect rect(m_ptSelectStartPoint, m_ptSelectEndPoint);

		pFloatWnd = new CFloatDibWnd(m_hWnd, rect);
		pFloatWnd->CreateWnd();

		CRect* rc = new CRect(rect);
		// 发送消息给画布，让其自己清除区域
		::SendMessage(m_hWnd, WM_CUTCAVASDIBRECT, WPARAM(rc), 0);
		delete rc;

		//未选区域变暗
//		::SendMessage(m_hWnd, WM_DARKUNSELECTEDRECT, 0, 0);
	}

	return pFloatWnd;
}


/*
*--------------------------------------------------------------------------------
*  函数名: DrawRubber
*  功能	 : 画出选取区域的虚框，以作为提示
*  参数  : CPoint ptPoint  - 选取区域的结束点坐标
*  返回值: CFloatDibWnd	 -  选取区域浮动窗口指针，由调用函数负责释放此窗口
*--------------------------------------------------------------------------------
*/
void CSelectTool::DrawRubber(const CPoint& ptPoint) const
{
	HBRUSH hOldBrush = (HBRUSH)SelectObject(m_hDC, GetStockObject(NULL_BRUSH));
	HPEN hPen = CreatePen(PS_DASH, 1, RGB(0x0, 0x0, 0x0));
	HPEN hOldPen = (HPEN) SelectObject(m_hDC, hPen);

	int nOldRop = SetROP2(m_hDC, R2_NOTXORPEN);

	// 画出轨迹
	DrawTrack(m_ptSelectStartPoint, ptPoint);



	SetROP2(m_hDC, nOldRop);

	SelectObject(m_hDC, hOldPen);
	SelectObject(m_hDC, hOldBrush);	
	DeleteObject(hOldPen);
	DeleteObject(hPen);
	DeleteObject(hOldBrush);
}


/*
*--------------------------------------------------------------------------------
*  函数名: DrawUnselectedRgn
*  功能	 : 使未选取区域变暗
*  算法  :
*--------------------------------------------------------------------------------
*/
void CSelectTool::DarkUnselectedRgn()
{
	
 	CRect rect;
 	GetWindowRect(m_hWnd, &rect);
 
 	HDC hDarkMemDC = CreateCompatibleDC(m_hDC);
 	HBITMAP hDarkBitmap = CreateCompatibleBitmap(m_hDC, rect.Width(), rect.Height());
 	HBITMAP hOldBitmap = (HBITMAP) SelectObject(hDarkMemDC, hDarkBitmap);
 	HBRUSH hOldBrush = (HBRUSH) SelectObject(hDarkMemDC, GetStockObject(NULL_BRUSH));
 	
 	Rectangle(hDarkMemDC, 0, 0, rect.Width(), rect.Height());
 

	BLENDFUNCTION blend = { AC_SRC_OVER, 0, 100, 0 };
 
 	AlphaBlend( m_hDC, 
 				0, 
 				0, 
 				rect.Width(),
 				rect.Height(),
 				hDarkMemDC, 
 				0, 
 				0, 
 				100, 
 				100,
 				blend);
 
 	DeleteObject(hOldBrush);
 	DeleteObject(hOldBitmap);
 	DeleteObject(hDarkBitmap);
 	DeleteObject(hDarkMemDC);
}
