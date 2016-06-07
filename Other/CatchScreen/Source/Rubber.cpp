/**
*   Rubber.h		
*   Version 1.0
*   CPH	绘画乐园
*	Copyright 2002 . All Rights Reserved.
*   定义橡皮类
*======================================================
*   版本
*   Revision 1.0	2002/11/4    	 实现 CRubber 类
*/
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "..\include\Rubber.h"
#include "..\include\Track.h"

#include <math.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRubber::CRubber(HDC hDC, int nSize, COLORREF crColor) : CAbstractPen(hDC, nSize, crColor)
{
	m_strPenName = _T("橡皮擦");
//	m_hCursor = LoadCursor(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDC_RUBBER));
    m_hCursor = AfxGetApp()->LoadCursor(IDC_RUBBER);
	//橡皮颜色固定为背景色
	m_crPenColor = RGB(0xFF, 0xFF, 0xFF);
	m_pPenTrack->SetTrackColor(crColor);
}

CRubber::~CRubber()
{

}


/*
*--------------------------------------------------------------------------------
*  函数名: SetPenColor
*  功能	 : 设置橡皮的颜色
*  参数  : COLORREF crColor	-  笔的颜色
*  算法	 : 橡皮颜色固定为背景色，故直接返回
*--------------------------------------------------------------------------------
*/
void CRubber::SetPenColor(COLORREF crColor)
{
	m_crTrackColor = crColor;
	m_pPenTrack->SetTrackColor(crColor);
	return;
}


/*
*--------------------------------------------------------------------------------
*  函数名: SetPenTrack
*  功能	 : 设置笔的轨迹
*  参数  : [in]  PEN_TRACK PenTrack	- 笔的轨迹形状
*  返回值: 如果轨迹类型合法返回 TRUE，否则返回 FALSE
*  算法  : 橡皮的笔色始终为背景色，而轨迹色要变动，所以重载此函数
*			在此只是暂时替换笔色为轨迹，完了再换回来
*--------------------------------------------------------------------------------
*/
BOOL CRubber::SetPenTrack(PEN_TRACK PenTrack)
{	
	COLORREF crTemp = m_crPenColor;
	m_crPenColor = m_crTrackColor;
	
	CAbstractPen::SetPenTrack(PenTrack);

	m_crPenColor = crTemp;

	return m_pPenTrack ? TRUE : FALSE;
}