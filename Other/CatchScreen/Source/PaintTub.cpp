/**
*   PaintTub.cpp		
*   Version 1.0
*   CPH	绘画乐园
*   Copyright 2002 . All Rights Reserved.
*   实现油漆桶类
*======================================================
*   版本
*	Revision 1.0	2002/11/12    	 实现 CPaintTub 类
*/

#include "stdafx.h"
#include "..\\include\\PaintTub.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPaintTub::CPaintTub(HDC hDC, int nSize, COLORREF crColor) : CAbstractPen(hDC, nSize, crColor)
{
	m_strPenName = _T("油漆桶");
//	m_hCursor = LoadCursor(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDC_PAINTTUB));
    m_hCursor = AfxGetApp()->LoadCursor(IDC_PAINTTUB);
}


CPaintTub::~CPaintTub()
{

}


/*
*--------------------------------------------------------------------------------
*  函数名:	BeginDraw
*  功能	 :	用当前画刷颜色填充封闭区域
*  参数	 :	CPoint ptPoint	 -   当前的坐标点
*  算法  :	重载函数，调用 API 函数进行填充
*--------------------------------------------------------------------------------
*/
void CPaintTub::BeginDraw(const CPoint& ptPoint)
{
	HDC hDC = m_hDC;

	// 得到当前坐标点的颜色
	COLORREF crCurPos = GetPixel(hDC, ptPoint.x, ptPoint.y);

	HBRUSH hBrush = CreateSolidBrush(m_crPenColor);
	HBRUSH hOldBrush = (HBRUSH) SelectObject(hDC, hBrush);

	// 填充颜色, Windows API
	ExtFloodFill(hDC, ptPoint.x, ptPoint.y, crCurPos, FLOODFILLSURFACE);

	SelectObject(hDC, hOldBrush);
	DeleteObject(hOldBrush);
	DeleteObject(hBrush);
}


/*
*--------------------------------------------------------------------------------
*  函数名:	DrawPoint
*  功能	 :	什么事情都不做
*  参数	 :	CPoint ptPoint	 -   当前的坐标点
*  算法  :	重载函数，因为是在按下鼠标时进行填充，故鼠标移动时什么事不做
*--------------------------------------------------------------------------------
*/
void CPaintTub::DrawPoint(const CPoint& ptPoint)
{	
}


/*
*--------------------------------------------------------------------------------
*  函数名:	EndDraw
*  功能	 :	什么事情都不做
*  参数	 :	CPoint ptPoint	 -   当前的坐标点
*  算法  :	重载函数，因为是在按下鼠标时进行填充，故松开鼠标时什么事不做
*--------------------------------------------------------------------------------
*/
void CPaintTub::EndDraw(const CPoint& ptPoint)
{	
}
