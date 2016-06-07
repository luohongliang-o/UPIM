/**
*  TetragonTrack.cpp		
*   Version 1.0
*   CPH	绘画乐园
*   Copyright 2000-2001 . All Rights Reserved.
*   实现四角星轨迹类
*======================================================
*   版本
*   Revision 1.0	2002/11/12   	 实现 CTetragonTrack 类
*/
#include "stdafx.h"
#include "..\include\TetragonTrack.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTetragonTrack::CTetragonTrack(HDC hDC, COLORREF crTrackColor) : CTrack(hDC, crTrackColor)
{
	m_strTrackName = _T("四角星");
}


CTetragonTrack::~CTetragonTrack()
{

}


/*
*--------------------------------------------------------------------------------
*  函数名	: Draw
*  功能		: 给定四角星的中心和一个顶点，画出四角星
*  参数		:	CPoint &ptCenter	- 四角星的中心点座标	
*				CPoint &ptBegin		- 四角星的一个顶点座标
*  算法		: 将圆等分为12份，其中四个顶点相互相线得到四个相交点即内部顶点
*--------------------------------------------------------------------------------
*/
void CTetragonTrack::Draw(const CPoint &ptStart, const CPoint &ptEnd) const
{
	HDC hDC = m_hDC;
	if (ptStart.x == ptEnd.x && ptStart.y == ptEnd.y)
	{
		// 起点和终点相同，就画一个点
		SetPixel(hDC, ptStart.x, ptEnd.y, TRACK_PEN_COLOR);
		return;
	}

	CPoint ptOutVertex[12], ptInnerVertex[4];

	// 12 等分圆，得到 12 个顶点座标
	SplitCircle(ptStart, ptEnd, 12, ptOutVertex);
	
	// 得到内部四个顶点
	GetIntersectPoint(ptOutVertex[0], ptOutVertex[5], ptOutVertex[3], ptOutVertex[10], ptInnerVertex[0]);
	GetIntersectPoint(ptOutVertex[3], ptOutVertex[8], ptOutVertex[6], ptOutVertex[1], ptInnerVertex[1]);
	GetIntersectPoint(ptOutVertex[6], ptOutVertex[11], ptOutVertex[9], ptOutVertex[4], ptInnerVertex[2]);
	GetIntersectPoint(ptOutVertex[9], ptOutVertex[2], ptOutVertex[0], ptOutVertex[7], ptInnerVertex[3]);

	// 画四角星
	MoveToEx(hDC, ptOutVertex[0].x, ptOutVertex[0].y, NULL);
	LineTo(hDC, ptInnerVertex[0].x, ptInnerVertex[0].y);
	LineTo(hDC, ptOutVertex[3].x, ptOutVertex[3].y);
	LineTo(hDC, ptInnerVertex[1].x, ptInnerVertex[1].y);
	LineTo(hDC, ptOutVertex[6].x, ptOutVertex[6].y);
	LineTo(hDC, ptInnerVertex[2].x, ptInnerVertex[2].y);
	LineTo(hDC, ptOutVertex[9].x, ptOutVertex[9].y);
	LineTo(hDC, ptInnerVertex[3].x, ptInnerVertex[3].y);
	LineTo(hDC, ptOutVertex[0].x, ptOutVertex[0].y);
}


/*
*--------------------------------------------------------------------------------
*  函数名	: CreatePos
*  功能		: 计算出轨迹上所有点的集合，保存到列表中去
*  参数		: VOID
*  算法		: 由四角星的外部四个顶点和内部四个顶点依次连线，
*				通过 LineDDA 函数得出所有的轨迹点座标
*  前置条件 : 必须已经设定了轨迹起始点座标和终止点座标
*  后置条件 : 在 m_listTrackPoints 列表中保存所有轨迹点的座标
*--------------------------------------------------------------------------------
*/
BOOL CTetragonTrack::CreatePos()
{
	CPoint ptOutVertex[12], ptInnerVertex[4];

	// 12等分圆，得到12个顶点座标
	SplitCircle(m_ptTrackStartPoint, m_ptTrackEndPoint, 12, ptOutVertex);

	// 得到内圆的四个顶点
	GetIntersectPoint(ptOutVertex[0], ptOutVertex[5], ptOutVertex[3], ptOutVertex[10], ptInnerVertex[0]);
	GetIntersectPoint(ptOutVertex[3], ptOutVertex[8], ptOutVertex[6], ptOutVertex[1], ptInnerVertex[1]);
	GetIntersectPoint(ptOutVertex[6], ptOutVertex[11], ptOutVertex[9], ptOutVertex[4], ptInnerVertex[2]);
	GetIntersectPoint(ptOutVertex[9], ptOutVertex[2], ptOutVertex[0], ptOutVertex[7], ptInnerVertex[3]);

	// 将四角星的外部四顶点和内部四顶点依次连线用 LineDDA 得到轨迹点
	LineDDA(ptOutVertex[0].x, ptOutVertex[0].y, ptInnerVertex[0].x, ptInnerVertex[0].y, CTrack::AddPosToList, (LPARAM) this);
	LineDDA(ptInnerVertex[0].x, ptInnerVertex[0].y, ptOutVertex[3].x, ptOutVertex[3].y, CTrack::AddPosToList, (LPARAM) this);

	LineDDA(ptOutVertex[3].x, ptOutVertex[3].y, ptInnerVertex[1].x, ptInnerVertex[1].y, CTrack::AddPosToList, (LPARAM) this);
	LineDDA(ptInnerVertex[1].x, ptInnerVertex[1].y, ptOutVertex[6].x, ptOutVertex[6].y, CTrack::AddPosToList, (LPARAM) this);

	LineDDA(ptOutVertex[6].x, ptOutVertex[6].y, ptInnerVertex[2].x, ptInnerVertex[2].y, CTrack::AddPosToList, (LPARAM) this);
	LineDDA(ptInnerVertex[2].x, ptInnerVertex[2].y, ptOutVertex[9].x, ptOutVertex[9].y, CTrack::AddPosToList, (LPARAM) this);

	LineDDA(ptOutVertex[9].x, ptOutVertex[9].y, ptInnerVertex[3].x, ptInnerVertex[3].y, CTrack::AddPosToList, (LPARAM) this);
	LineDDA(ptInnerVertex[3].x, ptInnerVertex[3].y, ptOutVertex[0].x, ptOutVertex[0].y, CTrack::AddPosToList, (LPARAM) this);

	return TRUE;
}