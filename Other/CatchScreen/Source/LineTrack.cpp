/**
*   EllipseTrack.h		
*   Version 1.0
*   CPH	绘画乐园
*   Copyright 2002 . All Rights Reserved.
*   实现椭圆形轨迹
*======================================================
*   版本
*   Revision 1.0	2002/11/11    	 建立 CLineTrack 类
*/
#include "stdafx.h"
#include "..\Include\LineTrack.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CLineTrack::CLineTrack(HDC hDC, COLORREF crTrackColor) : CTrack(hDC, crTrackColor)
{
	m_strTrackName = _T("直线");
}

CLineTrack::~CLineTrack()
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
void CLineTrack::Draw(const CPoint &ptStart, const CPoint &ptEnd) const
{
	HDC hDC = m_hDC;

	if (ptStart.x == ptEnd.x && ptStart.y == ptEnd.y)
	{
		// 起点和终点相同，就画一个点
		SetPixel(hDC, ptStart.x, ptEnd.y, TRACK_PEN_COLOR);
		return;
	}

	MoveToEx(hDC, ptStart.x, ptStart.y, NULL);
	LineTo(hDC, ptEnd.x, ptEnd.y);
}


/*
*--------------------------------------------------------------------------------
*  函数名	: CreatePos
*  功能		: 计算出轨迹上所有点的集合，保存到列表中去
*  参数		: VOID
*  算法		: 通过 LineDDA 函数得出所有的轨迹点座标
*  前置条件 : 必须已经设定了轨迹起始点座标和终止点座标
*  后置条件 : 在 m_listTrackPoints 列表中保存所有轨迹点的座标
*--------------------------------------------------------------------------------
*/
BOOL CLineTrack::CreatePos()
{
	// 斜线	
	LineDDA(m_ptTrackStartPoint.x, m_ptTrackStartPoint.y, m_ptTrackEndPoint.x, m_ptTrackEndPoint.y, CTrack::AddPosToList, (LPARAM) this);

	return TRUE;
}