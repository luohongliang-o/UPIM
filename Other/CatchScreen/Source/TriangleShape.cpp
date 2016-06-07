// TriangleShape.cpp: implementation of the CTriangleShape class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "..\Paint\PaintDlg.h"
#include "..\\INCLUDE\\TriangleShape.h"

#include <math.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTriangleShape::CTriangleShape()
{

}

CTriangleShape::~CTriangleShape()
{

}

/*
*--------------------------------------------------------------------------------
*  BeginDraw
*  功能	- 开始进入绘画状态
*  参数
*	hDC hDC			 - 作图的设备描述表
*	CPoint ptPoint	 - 当前的坐标点
*  后置条件
*		在当前坐标点画出笔的图形，设定上一次绘画坐标点。
*--------------------------------------------------------------------------------
*/
void CTriangleShape::BeginDraw(HDC hDC, const CPoint& ptPoint)
{
	m_ptCenter = ptPoint;
	m_ptLast = ptPoint;
}


/*
*--------------------------------------------------------------------------------
*  DrawPoint
*  功能	- 绘图
*  参数
*	hDC hDC			 - 作图的设备描述表
*	CPoint ptPoint	 - 当前的坐标点
*  算法
*		用当前笔尖图形在 m_ptLast 与 ptPoint 之间直线上的每一个点上绘此图形
*  前置条件
*		必须先调用 BeginDraw
*--------------------------------------------------------------------------------
*/
void CTriangleShape::DrawPoint(HDC hDC, const CPoint& ptPoint)
{
	HBRUSH hBrush = (HBRUSH)SelectObject(hDC, GetStockObject(NULL_BRUSH));
	HPEN hPen = CreatePen(PS_DOT, 1, m_crPenColor);
	SelectObject(hDC, hPen);

	int nOldRop = SetROP2(hDC, R2_NOTXORPEN);

	// 擦除上一次画的线条
	DrawTriangle(hDC, m_ptLast);

	// 画出新的边框线条
	DrawTriangle(hDC, ptPoint);
	
	SetROP2(hDC, nOldRop);

	m_ptLast = ptPoint;
	
	SelectObject(hDC, hBrush);
	DeleteObject(hBrush);
}

void CTriangleShape::EndDraw(HDC hpDC, const CPoint& ptBegin)
{
}


void CTriangleShape::DrawTriangle(HDC hDC, const CPoint &ptBegin)
{

	CPoint point[4];
	point[0] = ptBegin;
	point[3] = ptBegin;
//	point[1] = GetNextPoint(ptBegin);
//	point[2] = GetNextPoint(point[1]);
/*
	const double PI = 3.1415926;
	int cx, cy, x, y, s, r;
	double a, b;

	cx = abs(ptBegin.x - m_ptCenter.x);
	cy = abs(ptBegin.y - m_ptCenter.y);
		
	r = sqrt(cx * cx + cy * cy);
	s = 1.732 * r;
	if (!r)
		return;

	point[0].x = 50;
	point[0].y = 10;
	cx = 20;
	cy = 0;
	r = 20;
	s = 1.732 * r;


	a = acos(double(cx) / double(r)) / PI * 180.0;
	if (cos(a) == 0)
		a = 0;


	b = 90 - a - 30;



	x = double(s) * sin(double(b) / 180.0 * PI);
	y = double(s) * cos(double(b) / 180.0 * PI);

TRACE("cx =%d  r = %d  a = %f b = %f x = %d, y = %d \n", cx, r, a, b, x, y);
	
	point[1].x = ptBegin.x - x;
	point[1].y = ptBegin.y + y;

	point[2].x = point[1].x - x ;
	point[2].y = point[1].y - y;




	PolylineTo(hDC, point, 4);
*/

	const double PI = 3.1415926;
	int cx, cy, x, y, s, r;
	double a, b;

	cx = abs(ptBegin.x - m_ptCenter.x);
	cy = abs(ptBegin.y - m_ptCenter.y);
		
	r = sqrt(cx * cx + cy * cy);

	if (!r)
		return;

a = acos(double(cx) / double(r)) / PI * 180.0;
	
	if (ptBegin.x < m_ptCenter.x && ptBegin.y < m_ptCenter.y)
	{
		// 第二象限
		a = asin(double(cx) / double(r)) / PI * 180.0;
		a = 90 + a;
	}
	else if (ptBegin.x > m_ptCenter.x && ptBegin.y > m_ptCenter.y)
	{
		// 第四象限
		a = asin(double(cx) / double(r)) / PI * 180.0;
		a = 270 + a;
	}
	else if (ptBegin.x < m_ptCenter.x && ptBegin.y > m_ptCenter.y)
	{
		// 第三象限
		a = 180 + a;
	}

	if (cx == r)
		a = 0;
	else if (cx == 0)
		a = 90;	

	SelectObject(hDC, GetStockObject(NULL_PEN));

	AngleArcTo(hDC, m_ptCenter.x, m_ptCenter.y, r, a, 120);
	GetCurrentPositionEx(hDC, point + 1);

//	AngleArc(hDC, m_ptCenter.x, m_ptCenter.y, r, 90, 240);

	AngleArcTo(hDC, m_ptCenter.x, m_ptCenter.y, r, a, 240);
	GetCurrentPositionEx(hDC, point + 2);

	AngleArcTo(hDC, m_ptCenter.x, m_ptCenter.y, r, a, 360);
//	GetCurrentPositionEx(hDC, point + 3);
	
	SelectObject(hDC, GetStockObject(BLACK_PEN));

	MoveToEx(hDC, ptBegin.x, ptBegin.y, NULL);
	PolylineTo(hDC, point, 4);

}

CPoint CTriangleShape::GetNextPoint(const CPoint &ptBegin)
{
	const double PI = 3.1415926;
	int cx, cy, x, y, s, r;
	double a, b;

	cx = abs(ptBegin.x - m_ptCenter.x);
	cy = abs(ptBegin.y - m_ptCenter.y);
		
	r = sqrt(cx * cx + cy * cy);
	s = 1.732 * r;
	if (!r)
		return ptBegin;

	a = acos(double(cx) / double(r)) / PI * 180.0;
	if (cos(a) == 0)
		a = 0;
	b = 90 - a - 30;
	
	x = double(s) * sin(double(b) / 180.0 * PI);
	y = double(s) * cos(double(b) / 180.0 * PI);

TRACE("cx =%d  r = %d  a = %f b = %f x = %d, y = %d \n", cx, r, a, b, x, y);
	
	return CPoint(ptBegin.x - x, ptBegin.y + y);
}


BOOL CTriangleShape::AngleArcTo(HDC hDC, int X, int Y, DWORD dwRadius, FLOAT eStartAngle, FLOAT eSweepAngle)
{
	const FLOAT piover180 = (FLOAT) 3.141592653586 / 180;

	if (eSweepAngle >= 360)
		eSweepAngle = 360;
	else if (eSweepAngle <= -360)
		eSweepAngle = -360;

	FLOAT eEndAngle = (eStartAngle + eSweepAngle) * piover180;
	eStartAngle = eStartAngle * piover180;

	int dir;
	if (eSweepAngle > 0)
		dir = SetArcDirection(hDC, AD_COUNTERCLOCKWISE);
	else
		dir = SetArcDirection(hDC, AD_CLOCKWISE);

	BOOL rslt = ArcTo(hDC, X - dwRadius, Y - dwRadius,
				X + dwRadius, Y + dwRadius,
				X + (int) (dwRadius * 10 * cos(eStartAngle)),
				Y - (int) (dwRadius * 10 * sin(eStartAngle)),
				X + (int) (dwRadius * 10 * cos(eEndAngle)),
				Y - (int) (dwRadius * 10 * sin(eEndAngle)));

	SetArcDirection(hDC, dir);
	return rslt;
}
