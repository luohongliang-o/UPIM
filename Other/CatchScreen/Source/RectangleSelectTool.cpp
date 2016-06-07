/**
*   RectangleSelectTool.cpp	
*   Version 1.0
*   CPH	绘画乐园
*   Copyright 2002 . All Rights Reserved.
*   定义矩形选取工具
*======================================================
*   版本
*   Revision 1.0	2002/11/14     	  实现矩形选取工具
*/

#include "stdafx.h"
#include "..\INCLUDE\RectangleSelectTool.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRectangleSelectTool::CRectangleSelectTool(HWND hWnd) : CSelectTool(hWnd)
{

}

CRectangleSelectTool::~CRectangleSelectTool()
{

}

/*
*--------------------------------------------------------------------------------
*  函数名: DrawTrack
*  功能	 : 画出选取时的虚线框
*  参数	 : CPoint& ptStart	-	起始选取点
*		   CPoint& ptEnd	-	终止选取点
*  算法  :	重载函数
*--------------------------------------------------------------------------------
*/
void CRectangleSelectTool::DrawTrack(const CPoint& ptStart, const CPoint& ptEnd) const
{
	Rectangle(m_hDC, ptStart.x, ptStart.y, ptEnd.x, ptEnd.y);
}
