/**
*   Rubber.h		
*   Version 1.0
*   CPH	绘画乐园
*	Copyright 2002. All Rights Reserved.
*   定义橡皮类
*======================================================
*   版本
*   Revision 1.0	2002/11/4     	 建立 CRubber 类
*/
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RUBBER_H__1F010B34_FF30_4D42_8211_145B0448F956__INCLUDED_)
#define AFX_RUBBER_H__1F010B34_FF30_4D42_8211_145B0448F956__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "AbstractPen.h"


/*
*类名: CRubber
*说明：定义橡皮类
*版本
*	Revision 1.0	2002/11/4    	 完成 CRubber 的基本功能
*/
class CRubber : public CAbstractPen  
{
protected:
	COLORREF m_crTrackColor;

public:
	CRubber(HDC hDC, int nSize = 12, COLORREF crColor = RGB(0, 0, 0));
	virtual ~CRubber();

public:
	virtual void SetPenColor(COLORREF crColor);
	virtual BOOL SetPenTrack(PEN_TRACK PenTrack);
};

#endif // !defined(AFX_RUBBER_H__1F010B34_FF30_4D42_8211_145B0448F956__INCLUDED_)
