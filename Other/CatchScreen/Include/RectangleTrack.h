/**
*  RectangleTrack.h		
*   Version 1.0
*   CPH	绘画乐园
*   Copyright 2000-2001 . All Rights Reserved.
*   定义矩形迹类
*======================================================
*   版本
*   Revision 1.0	2002/11/12     	 建立 CRectangleTrack 类
*/
#if !defined(AFX_RETANGETrack_H__BF4480B8_16B2_49A2_B730_2F351A0ECB0A__INCLUDED_)
#define AFX_RETANGETrack_H__BF4480B8_16B2_49A2_B730_2F351A0ECB0A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Track.h"


/*
*类名: CRectangleTrack
*说明：定义矩形轨迹类
*版本
*	Revision 1.0	2002/11/12     	 建立 CRectangleTrack 类
*/
class CRectangleTrack : public CTrack  
{
public:
	CRectangleTrack(HDC hDC, COLORREF crTrackColor);
	virtual ~CRectangleTrack();


protected:
	// 根据轨迹创建点的集合
	virtual BOOL CreatePos();

	// 画矩形
	virtual void Draw(const CPoint& ptStart, const CPoint& ptEnd) const;
};

#endif // !defined(AFX_RETANGETrack_H__BF4480B8_16B2_49A2_B730_2F351A0ECB0A__INCLUDED_)
