/**
*	File Name			:	AirPen.h
*	Version				:	1.0
*	Project Information	:	绘画乐园(1003-2002)
*	Copyright			:	Copyright 2002 . All Rights Reserved.
*	Brief Description   :	实现喷枪工具的类
*====================================================================================
*   版本
*   Revision 1.0	2002/11/6		建立CAirPen类
*/

#if !defined(AFX_AIRPEN_H__704E2958_F195_11D6_95DA_000795CFF5EB__INCLUDED_)
#define AFX_AIRPEN_H__704E2958_F195_11D6_95DA_000795CFF5EB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "AbstractPen.h"



/*
*类名：CAirPen
*说明：用于实现喷枪的画笔效果
*版本：
*Revision     1.0     2002/11/06        完成喷枪的绘图功能设计
*
*Revision     2.0     2002/11/13       修改代码以适应基类的调整
*/


class CAirPen : public CAbstractPen  
{
private:
	// 绘图区窗口句柄，用于定时器中
    HWND m_hWnd;

	static int g_nPenSize;
	static COLORREF g_crPenColor;


public:
	CAirPen(HWND hWnd, int nSize = 12, COLORREF crColor = RGB(0, 0, 0));
	virtual ~CAirPen();
    
	static VOID CALLBACK TimerProc(HWND hWnd, UINT uMsg, UINT_PTR idEvent, 
		                           DWORD dwTime);

	virtual void SetPenColor(COLORREF crColor);
	virtual void SetPenSize(int nSize);
	virtual void BeginDraw(const CPoint & ptPoint);
	virtual void DrawPoint(const CPoint & ptPoint);
	virtual void EndDraw(const CPoint & ptPoint);
};

#endif // !defined(AFX_AIRPEN_H__704E2958_F195_11D6_95DA_000795CFF5EB__INCLUDED_)
