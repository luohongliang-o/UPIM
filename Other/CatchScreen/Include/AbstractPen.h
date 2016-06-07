/**
*   AbstractPen.h		
*   Version 2.0
*   CPH	绘画乐园
*   Copyright 2002 . All Rights Reserved.
*   定义各种画笔的抽象基类，抽取取公共接口
*======================================================
*   版本
*   Revision 1.0	2002/11/1     	 建立 CAbstractPen 类，定义 PEN_STYLE 枚举类型
*	
*	Revision 2.0	2002/11/12    	 定义 PEN_TRACK 枚举类型，给画笔加入轨迹功能
*/

#if !defined(AFX_PEN_H__AC1A111F_077C_4E6E_B998_90E7FDEDC4FD__INCLUDED_)
#define AFX_PEN_H__AC1A111F_077C_4E6E_B998_90E7FDEDC4FD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\resource.h"
#include "ENUM.h"


class CTrack;
class CPenNeb;


/*
*类名: CAbstractPen
*说明：定义各种画笔的抽象基类，抽取取公共接口
*版本
*	Revision 1.0	2002/11/1     	 完成 CAbstractPen 的基本功能
*
*	Revision 2.0	2002/11/12   	 定义 PEN_TRACK 枚举类型，给画笔加入轨迹功能
*/
class CAbstractPen  
{
protected:
	PEN_TRACK   m_PenTrack;
	CPenNeb   * m_pPenNeb;
	CTrack    * m_pPenTrack;
	BOOL        m_bAlpha;
	// 用笔绘画的设备描述表句柄
	HDC         m_hDC;

	CString     m_strPenName;
	COLORREF    m_crPenColor;
	// 笔的大小
	int         m_nPenSize;

	// 笔的光标图形
	HCURSOR     m_hCursor;


public:
	CAbstractPen(HDC hDC, int nSize = 12, COLORREF crColor = RGB(0, 0, 0));
	virtual ~CAbstractPen();


public:		
	//设置笔属性函数
	virtual BOOL SetPenTrack(PEN_TRACK PenTrack);
	virtual void SetPenColor(COLORREF crColor);
	virtual void SetPenStyle(PEN_STYLE PenStyle);
	virtual void SetPenSize(int nSize);

	// 绘画函数
	virtual void BeginDraw(const CPoint& ptPoint);
	virtual void DrawPoint(const CPoint& ptPoint);
	virtual void EndDraw(const CPoint& ptPoint);

	// 返回笔的光标句柄
	HCURSOR GetCursor() const;


protected:
	// 填充轨迹上的点
	void FillTrack();
	// 在指定座标上绘点
	virtual void Apply(const CPoint& ptPoint) const;
};

#endif // !defined(AFX_PEN_H__AC1A111F_077C_4E6E_B998_90E7FDEDC4FD__INCLUDED_)
