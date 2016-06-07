/**
*   PentagonTrack.cpp	
*   Version 1.0
*   CPH	绘画乐园
*   Copyright 2002 . All Rights Reserved.
*   实现五角星轨迹类
*======================================================
*   版本
*   Revision 1.0	2002/11/11  	 实现 CPentagonTrack 类
*/
#include "stdafx.h"
#include "..\include\PentagonTrack.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPentagonTrack::CPentagonTrack(HDC hDC, COLORREF crTrackColor) : CTrack(hDC, crTrackColor)
{
	m_strTrackName = _T("五角星");
}

CPentagonTrack::~CPentagonTrack()
{

}


/*
*--------------------------------------------------------------------------------
*  函数名	: Draw
*  功能		: 画出椭圆轨迹
*  参数		:
*		CPoint& ptStart	-	椭圆外围矩形左上角座标
*		CPoint& ptEnd	-	椭圆外围矩形右下角座标
*  算法		: 五等分圆得到五个顶点，再由此五点的交叉连线得到五个内部顶点
*--------------------------------------------------------------------------------
*/
void CPentagonTrack::Draw(const CPoint &ptStart, const CPoint &ptEnd) const
{
	HDC hDC = m_hDC;

	CPoint ptOutVertex[5], ptInnerVertex[5];

	if (ptStart.x == ptEnd.x && ptStart.y == ptEnd.y)
	{
		// 起点和终点相同，就画一个点
		SetPixel(hDC, ptStart.x, ptEnd.y, TRACK_PEN_COLOR);
		return;
	}

	// 五等分圆，得到五个顶点座标
	SplitCircle(ptStart, ptEnd, 5, ptOutVertex);
	
	// 得到内圆的五个顶点
	GetIntersectPoint(ptOutVertex[0], ptOutVertex[2], ptOutVertex[1], ptOutVertex[4], ptInnerVertex[0]);
	GetIntersectPoint(ptOutVertex[0], ptOutVertex[2], ptOutVertex[1], ptOutVertex[3], ptInnerVertex[1]);
	GetIntersectPoint(ptOutVertex[1], ptOutVertex[3], ptOutVertex[2], ptOutVertex[4], ptInnerVertex[2]);
	GetIntersectPoint(ptOutVertex[0], ptOutVertex[3], ptOutVertex[2], ptOutVertex[4], ptInnerVertex[3]);
	GetIntersectPoint(ptOutVertex[0], ptOutVertex[3], ptOutVertex[1], ptOutVertex[4], ptInnerVertex[4]);
	
	// 画五角星
	MoveToEx(hDC, ptOutVertex[0].x, ptOutVertex[0].y, NULL);
	LineTo(hDC, ptInnerVertex[0].x, ptInnerVertex[0].y);
	LineTo(hDC, ptOutVertex[1].x, ptOutVertex[1].y);
	LineTo(hDC, ptInnerVertex[1].x, ptInnerVertex[1].y);
	LineTo(hDC, ptOutVertex[2].x, ptOutVertex[2].y);
	LineTo(hDC, ptInnerVertex[2].x, ptInnerVertex[2].y);
	LineTo(hDC, ptOutVertex[3].x, ptOutVertex[3].y);
	LineTo(hDC, ptInnerVertex[3].x, ptInnerVertex[3].y);
	LineTo(hDC, ptOutVertex[4].x, ptOutVertex[4].y);
	LineTo(hDC, ptInnerVertex[4].x, ptInnerVertex[4].y);
	LineTo(hDC, ptOutVertex[0].x, ptOutVertex[0].y);

}


/*
*--------------------------------------------------------------------------------
*  函数名	: CreatePos
*  功能		: 计算出轨迹上所有点的集合，保存到列表中去
*  算法		: 由五角星的外部五个顶点和内部五个顶点依次连线，
*				能过 LineDDA 函数得出所有的轨迹点座标
*  前置条件 : 必须已经设定了轨迹起始点座标和终止点座标
*  后置条件 : 在 m_listTrackPoints 列表中保存所有轨迹点的座标
*--------------------------------------------------------------------------------
*/
BOOL CPentagonTrack::CreatePos()
{
	CPoint ptOutVertex[5], ptInnerVertex[5];

	// 五等分圆，得到五个顶点座标
	SplitCircle(m_ptTrackStartPoint, m_ptTrackEndPoint, 5, ptOutVertex);

	// 得到内圆的五个顶点
	GetIntersectPoint(ptOutVertex[0], ptOutVertex[2], ptOutVertex[1], ptOutVertex[4], ptInnerVertex[0]);
	GetIntersectPoint(ptOutVertex[0], ptOutVertex[2], ptOutVertex[1], ptOutVertex[3], ptInnerVertex[1]);
	GetIntersectPoint(ptOutVertex[1], ptOutVertex[3], ptOutVertex[2], ptOutVertex[4], ptInnerVertex[2]);
	GetIntersectPoint(ptOutVertex[0], ptOutVertex[3], ptOutVertex[2], ptOutVertex[4], ptInnerVertex[3]);
	GetIntersectPoint(ptOutVertex[0], ptOutVertex[3], ptOutVertex[1], ptOutVertex[4], ptInnerVertex[4]);

	// 将五角星的外部五顶点和内部五顶点依次连线用 LineDDA 得到轨迹点
	LineDDA(ptOutVertex[0].x, ptOutVertex[0].y, ptInnerVertex[0].x, ptInnerVertex[0].y, CTrack::AddPosToList, (LPARAM) this);
	LineDDA(ptInnerVertex[0].x, ptInnerVertex[0].y, ptOutVertex[1].x, ptOutVertex[1].y, CTrack::AddPosToList, (LPARAM) this);

	LineDDA(ptOutVertex[1].x, ptOutVertex[1].y, ptInnerVertex[1].x, ptInnerVertex[1].y, CTrack::AddPosToList, (LPARAM) this);
	LineDDA(ptInnerVertex[1].x, ptInnerVertex[1].y, ptOutVertex[2].x, ptOutVertex[2].y, CTrack::AddPosToList, (LPARAM) this);

	LineDDA(ptOutVertex[2].x, ptOutVertex[2].y, ptInnerVertex[2].x, ptInnerVertex[2].y, CTrack::AddPosToList, (LPARAM) this);
	LineDDA(ptInnerVertex[2].x, ptInnerVertex[2].y, ptOutVertex[3].x, ptOutVertex[3].y, CTrack::AddPosToList, (LPARAM) this);

	LineDDA(ptOutVertex[3].x, ptOutVertex[3].y, ptInnerVertex[3].x, ptInnerVertex[3].y, CTrack::AddPosToList, (LPARAM) this);
	LineDDA(ptInnerVertex[3].x, ptInnerVertex[3].y, ptOutVertex[4].x, ptOutVertex[4].y, CTrack::AddPosToList, (LPARAM) this);

	LineDDA(ptOutVertex[4].x, ptOutVertex[4].y, ptInnerVertex[4].x, ptInnerVertex[4].y, CTrack::AddPosToList, (LPARAM) this);
	LineDDA(ptInnerVertex[4].x, ptInnerVertex[4].y, ptOutVertex[0].x, ptOutVertex[0].y, CTrack::AddPosToList, (LPARAM) this);
	
	return TRUE;
}