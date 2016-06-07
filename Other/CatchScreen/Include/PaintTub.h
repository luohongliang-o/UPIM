/**
*   PaintTub.h		
*   Version 1.0
*   CPH	绘画乐园
*   Copyright 2002 . All Rights Reserved.
*   实现油漆桶类
*======================================================
*   版本
*	Revision 1.0	2002/11/12   	 建立 CPaintTub 类
*/

#if !defined(AFX_PAINTTUB_H__688A1FEA_B218_427A_9520_FC543CDCDDF4__INCLUDED_)
#define AFX_PAINTTUB_H__688A1FEA_B218_427A_9520_FC543CDCDDF4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "AbstractPen.h"


/*
*类名: CPaintTub
*说明：定义油漆桶类
*版本
*	Revision 1.0	2002/11/12     完成 CPaintTub 的基本功能
*/
class CPaintTub : public CAbstractPen
{
public:
	CPaintTub(HDC hDC, int nSize = 12, COLORREF crColor = RGB(0, 0, 0));
	virtual ~CPaintTub();

public:
	virtual void BeginDraw(const CPoint& ptPoint);
	virtual void DrawPoint(const CPoint& ptPoint);
	virtual void EndDraw(const CPoint& ptPoint);

};

#endif // !defined(AFX_PAINTTUB_H__688A1FEA_B218_427A_9520_FC543CDCDDF4__INCLUDED_)
