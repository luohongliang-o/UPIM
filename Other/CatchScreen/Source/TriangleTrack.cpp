/**
*   TriangleTrack.h		
*   Version 1.0
*   CPH	绘画乐园
*   Copyright 2002 . All Rights Reserved.
*   定义三角形轨迹形状工具，起始点 m_ptTractStartPoint 为三角形的中心点
*======================================================
*   版本
*   Revision 1.0	2002/11/11   	 实现 CTriangleTrack 类
*/

#include "stdafx.h"
#include "..\include\TriangleTrack.h"

#include <math.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTriangleTrack::CTriangleTrack(HDC hDC, COLORREF crTrackColor) : CTrack(hDC, crTrackColor)
{
	m_strTrackName = _T("三角形");
}

CTriangleTrack::~CTriangleTrack()
{

}


/*
*--------------------------------------------------------------------------------
*  函数名	: Draw
*  功能		: 给定三角形的中心和一个点，画出三角形
*  参数		:	CPoint &ptCenter	- 三角形的中心点座标	
*				CPoint &ptBegin		- 三角形的一个顶点座标
*  算法		: 三等分一个圆，得到三个顶点，依次连线即可
*--------------------------------------------------------------------------------
*/
void CTriangleTrack::Draw(const CPoint &ptStart, const CPoint &ptEnd) const
{
	HDC hDC = m_hDC;
	if (ptStart.x == ptEnd.x && ptStart.y == ptEnd.y)
	{
		// 起点和终点相同，就画一个点
		SetPixel(hDC, ptStart.x, ptEnd.y, TRACK_PEN_COLOR);
		return;
	}

	CPoint ptVertex[4];

	// 三等分圆，得到三个顶点座标
	SplitCircle(ptStart, ptEnd, 3, ptVertex);
	ptVertex[3] = ptVertex[0];

	// 画三角形
	MoveToEx(hDC, ptVertex[0].x, ptVertex[0].y, NULL);
	PolylineTo(hDC, ptVertex, 4);
}


/*
*--------------------------------------------------------------------------------
*  函数名	: CreatePos
*  功能		: 计算出轨迹上所有点的集合，保存到列表中去
*  参数		: VOID
*  算法		: 由三角形的三个顶点连线，能过 LineDDA 函数得出所有的轨迹点座标
*  前置条件 : 必须已经设定了轨迹起始点座标和终止点座标
*  后置条件 : 在 m_listTrackPoints 列表中保存所有轨迹点的座标
*--------------------------------------------------------------------------------
*/
BOOL CTriangleTrack::CreatePos()
{
	CPoint ptVertex[3];

	// 三等分圆，得到三个顶点座标
	SplitCircle(m_ptTrackStartPoint, m_ptTrackEndPoint, 3, ptVertex);
	
	// 三角形的三个顶点依次连线，调用 LineDDA 得到每一个轨迹点

	// 第一条边
	if (ptVertex[0] == ptVertex[1])
	{
		AddPoint(ptVertex[0].x, ptVertex[0].y);
	}
	else
	{
		LineDDA(ptVertex[0].x, ptVertex[0].y, ptVertex[1].x, ptVertex[1].y, CTrack::AddPosToList, (LPARAM) this);		
	}

	// 第二条边
	if (ptVertex[1] == ptVertex[2])
	{
		AddPoint(ptVertex[1].x, ptVertex[1].y);
	}
	else
	{
		LineDDA(ptVertex[1].x, ptVertex[1].y, ptVertex[2].x, ptVertex[2].y, CTrack::AddPosToList, (LPARAM) this);
	}

	// 第三条边
	if (ptVertex[2] == ptVertex[0])
	{
		AddPoint(ptVertex[2].x, ptVertex[2].y);
	}
	else
	{
		LineDDA(ptVertex[2].x, ptVertex[2].y, ptVertex[0].x, ptVertex[0].y, CTrack::AddPosToList, (LPARAM) this);
	}
	
	return TRUE;
}
