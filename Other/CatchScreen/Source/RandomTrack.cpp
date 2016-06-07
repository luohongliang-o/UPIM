/**
*  RandomTrack.cpp
*   Version 1.0
*   CPH	绘画乐园
*   Copyright 2000-2001 . All Rights Reserved.
*   实现任意轨迹类
*======================================================
*   版本
*   Revision 1.0	2002/11/15     	 实现 CRandomTrack 类
*/
#include "stdafx.h"
#include "..\Include\RandomTrack.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRandomTrack::CRandomTrack(HDC hDC, COLORREF crTrackColor) : CTrack(hDC, crTrackColor)
{
	m_strTrackName = _T("任意");
}


CRandomTrack::~CRandomTrack()
{

}


/*
*--------------------------------------------------------------------------------
*  函数名	: Draw
*  功能		: 画出椭圆轨迹
*  参数		: CPoint& ptStart	-	椭圆外围矩形左上角座标
*			  CPoint& ptEnd	-	椭圆外围矩形右下角座标
*  算法		: 任意轨迹不画轨迹点，直接用笔画点
*--------------------------------------------------------------------------------
*/
void CRandomTrack::Draw(const CPoint &ptStart, const CPoint &ptEnd) const
{
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
BOOL CRandomTrack::CreatePos()
{
	// 两次鼠标移动事件有时间间隔
	LineDDA(m_ptTrackStartPoint.x, m_ptTrackStartPoint.y, m_ptTrackEndPoint.x, m_ptTrackEndPoint.y, CTrack::AddPosToList, (LPARAM) this);

	return TRUE;
}