/**
*   EllipseTrack.cpp		
*   Version 1.0
*   CPH	绘画乐园
*   Copyright 2002 . All Rights Reserved.
*   实现各种椭圆轨迹类
*======================================================
*   版本
*   Revision 1.0	2002/11/11   	 实现 CEllipseTrack 类
*/
#include "stdafx.h"
#include "..\Include\EllipseTrack.h"
#include <math.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define ROUND(a)	((int) (a + 0.5))

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CEllipseTrack::CEllipseTrack(HDC hDC, COLORREF crTrackColor) : CTrack(hDC, crTrackColor)
{
	m_strTrackName = _T("椭圆形");
}

CEllipseTrack::~CEllipseTrack()
{

}


/*
*--------------------------------------------------------------------------------
*  函数名	: Draw
*  功能		: 画出椭圆轨迹
*  参数		:
*		CPoint& ptStart	-	椭圆外围矩形左上角座标
*		CPoint& ptEnd	-	椭圆外围矩形右下角座标
*--------------------------------------------------------------------------------
*/
void CEllipseTrack::Draw(const CPoint &ptStart, const CPoint &ptEnd) const
{
	HDC hDC = m_hDC;

	if (ptStart.y == ptEnd.y || ptStart.x == ptEnd.x)
	{
		// 水平线或是垂直线
		MoveToEx(hDC, ptStart.x, ptEnd.y, NULL);
		LineTo(hDC, ptEnd.x, ptEnd.y);
	}
	else
	{
		Ellipse(hDC, ptStart.x, ptStart.y, ptEnd.x, ptEnd.y);
	}
}


/*
*--------------------------------------------------------------------------------
*  函数名: CreatePos
*  功能	 : 计算出椭圆轨迹上所有点的集合，保存到列表中去
*  算法  : 根据椭圆方程求椭圆上的每一个点
*  前置条件: 必须已经设定了轨迹起始点座标和终止点座标
*  后置条件: 在 m_listTrackPoints 列表中保存所有轨迹点的座标
*--------------------------------------------------------------------------------
*/
BOOL CEllipseTrack::CreatePos()
{
	if (m_ptTrackStartPoint.y == m_ptTrackEndPoint.y && m_ptTrackStartPoint.x == m_ptTrackEndPoint.x)
	{
		// 水平线或是垂直线
		LineDDA(m_ptTrackStartPoint.x, m_ptTrackStartPoint.y, m_ptTrackEndPoint.x, m_ptTrackEndPoint.y, CTrack::AddPosToList, (LPARAM) this);
	}
	else
	{
		int xCenter, yCenter;
		double a, b;
		
		xCenter = (m_ptTrackEndPoint.x + m_ptTrackStartPoint.x) / 2;
		yCenter = (m_ptTrackEndPoint.y + m_ptTrackStartPoint.y) / 2;
		
		// 椭圆 X 轴长度
		a = abs(m_ptTrackEndPoint.x - m_ptTrackStartPoint.x) / 2;
		// 椭圆 Y 轴长度
		b = abs(m_ptTrackEndPoint.y - m_ptTrackStartPoint.y) / 2;
				
		double b2 = b * b;
		double a2 = a * a;
	
		// 中心点为(xCenter, yCenter)，扫描 X 轴求 Y 值
		int x, y;		
		for (x = (int) -a; x <= (int) a; x++)
		{
			y = (int) sqrt(b2 - (b2 / a2) * (x * x));
			AddPoint(x + xCenter, y + yCenter);
		}
		// 求对称轴，求两次是为了让坐标是连贯的
		for (x = (int) -a; x <= (int) a; x++)
		{
			y = (int) sqrt(b2 - (b2 / a2) * (x * x));
			AddPoint(x + xCenter, yCenter - y);
		}


		// 中心点为(xCenter, yCenter)，扫描 Y 轴求 X 值
		for (y = (int) b; y >= (int) -b; y--)
		{
			x = (int) sqrt(a2 - (a2 / b2) * (y * y));
			if ( y != (int) sqrt(b2 - (b2 / a2) * (x * x)) )
			{
				AddPoint(xCenter - x, yCenter + y);
			}
		}
		// 求对称轴，求两次是为了让坐标是连贯的
		for (y = (int) -b; y <= (int) b; y++)
		{
			x = (int) sqrt(a2 - (a2 / b2) * (y * y));
			if ( y != (int) sqrt(b2 - (b2 / a2) * (x * x)) )
			{
				AddPoint(x + xCenter, y + yCenter);
			}
		}
	}

	return TRUE;
}


void CEllipseTrack::CalEllipsePath(int xCenter, int yCenter, int Rx, int Ry)
{
//	int Rx, Ry, xCenter, yCenter;
	int Rx2, Ry2, twoRx2, twoRy2;


	Rx = abs(m_ptTrackEndPoint.x - xCenter);
	Ry = abs(m_ptTrackEndPoint.y - yCenter);

	Rx2 = Rx * Rx;
	Ry2 = Ry * Ry;
	twoRx2 = 2 * Rx2;
	twoRy2 = 2 * Ry2;
	int p;
	int x = 0;
	int y = Ry;
	int px = 0;
	int py = twoRx2 * y;

	void ellipseGetPoints(int, int, int, int);

	AddPoint(x, y);;

	p = ROUND(Ry2 - (Rx2 + Ry) + (0.25 * Rx2));
	while (px < py)
	{
		x++;
		px += twoRy2;
		if(p<0)
			p += Ry2 + px;
		else
		{
			y--;
			py -= twoRx2;
			p += Ry2 + px - py;
		}
		AddPoint(x, y);;
	}

	p = ROUND(Ry2 * (x + 0.5) * (x + 0.5) + Rx2 * (y - 1) * (y -1)
		      - Rx2 * Ry2);
	while (y > 0)
	{
		y--;
		py -= twoRx2;
		if (p > 0)
			p += Rx2 - py;
		else
		{
			x++;
			px += twoRy2;
			p += Rx2 - py + px;
		}
		AddPoint(x, y);
	}
}

void ellipseGetPoints(int xCenter, int yCenter, int x, int y)
{
	static CList<CPoint, CPoint> MyPoint;
	MyPoint.AddTail(CPoint(xCenter + x, yCenter + y));
	MyPoint.AddTail(CPoint(xCenter - x, yCenter + y));
	MyPoint.AddTail(CPoint(xCenter - x, yCenter - y));
	MyPoint.AddTail(CPoint(xCenter + x, yCenter - y));
}
