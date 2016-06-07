/**
*   Polygon.cpp		
*   Version 1.0
*   CPH	绘画乐园
*   Copyright 2002 . All Rights Reserved.
*   六边形轨迹
*======================================================
*   版本
*   Revision 1.0	2002/11/23    	 实现 CPolygon 类
*/

#include "stdafx.h"
#include "..\INCLUDE\PolygonTrack.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPolygonTrack::CPolygonTrack(HDC hDC, COLORREF crTrackColor) : CTrack(hDC, crTrackColor)
{
}

CPolygonTrack::~CPolygonTrack()
{

}


/*
*--------------------------------------------------------------------------------
*  函数名	: CreatePos
*  功能		: 计算出轨迹上所有点的集合，保存到列表中去
*  算法		: 由六边形的六个顶点依次连线，通过 LineDDA 函数得出所有的轨迹点座标
*  前置条件 : 必须已经设定了轨迹起始点座标和终止点座标
*  后置条件 : 在 m_listTrackPoints 列表中保存所有轨迹点的座标
*--------------------------------------------------------------------------------
*/
BOOL CPolygonTrack::CreatePos()
{
	CPoint ptOutVertex[6];

	// 六等分圆，得到六个顶点座标
	SplitCircle(m_ptTrackStartPoint, m_ptTrackEndPoint, 6, ptOutVertex);

	// 依次连接六边形的六个顶点用LineDDA求出每个点
	LineDDA(ptOutVertex[0].x, ptOutVertex[0].y, ptOutVertex[1].x, ptOutVertex[1].y, CTrack::AddPosToList, (LPARAM) this);
	LineDDA(ptOutVertex[1].x, ptOutVertex[1].y, ptOutVertex[2].x, ptOutVertex[2].y, CTrack::AddPosToList, (LPARAM) this);
	LineDDA(ptOutVertex[2].x, ptOutVertex[2].y, ptOutVertex[3].x, ptOutVertex[3].y, CTrack::AddPosToList, (LPARAM) this);
	LineDDA(ptOutVertex[3].x, ptOutVertex[3].y, ptOutVertex[4].x, ptOutVertex[4].y, CTrack::AddPosToList, (LPARAM) this);
	LineDDA(ptOutVertex[4].x, ptOutVertex[4].y, ptOutVertex[5].x, ptOutVertex[5].y, CTrack::AddPosToList, (LPARAM) this);
	LineDDA(ptOutVertex[5].x, ptOutVertex[5].y, ptOutVertex[0].x, ptOutVertex[0].y, CTrack::AddPosToList, (LPARAM) this);

	return TRUE;
}


/*
*--------------------------------------------------------------------------------
*  函数名	: Draw
*  功能		: 给定六边形的中心和一个顶点，画出六边形
*  参数		:
*		CPoint &ptCenter	- 六角形的中心点座标
*		CPoint &ptBegin		- 六角形的一个顶点座标
*  算法		: 六等分圆得到六个顶点，再依次连接此六个点
*--------------------------------------------------------------------------------
*/
void CPolygonTrack::Draw(const CPoint &ptStart, const CPoint &ptEnd) const
{
	HDC hDC = m_hDC;

	if (ptStart.x == ptEnd.x && ptStart.y == ptEnd.y)
	{
		// 起点和终点相同，就画一个点
		SetPixel(hDC, ptStart.x, ptEnd.y, TRACK_PEN_COLOR);
		return;
	}

	CPoint ptOutVertex[6];

	// 六等分圆，得到六个顶点座标
	SplitCircle(ptStart, ptEnd, 6, ptOutVertex);
	
	// 画六边形
	MoveToEx(hDC, ptOutVertex[0].x, ptOutVertex[0].y, NULL);
	LineTo(hDC, ptOutVertex[1].x, ptOutVertex[1].y);
	LineTo(hDC, ptOutVertex[2].x, ptOutVertex[2].y);
	LineTo(hDC, ptOutVertex[3].x, ptOutVertex[3].y);
	LineTo(hDC, ptOutVertex[4].x, ptOutVertex[4].y);
	LineTo(hDC, ptOutVertex[5].x, ptOutVertex[5].y);
	LineTo(hDC, ptOutVertex[0].x, ptOutVertex[0].y);
}