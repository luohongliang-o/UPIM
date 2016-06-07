/**
*   Polygon.h		
*   Version 1.0
*   CPH	绘画乐园
*   Copyright 2002 . All Rights Reserved.
*   定义六边形轨迹
*======================================================
*   版本
*   Revision 1.0	2002/11/23    	 建立 CPolygon 类
*/
#if !defined(AFX_POLYGONTRACK_H__3C0A9CEB_C301_44FF_B85C_FD0932F78C39__INCLUDED_)
#define AFX_POLYGONTRACK_H__3C0A9CEB_C301_44FF_B85C_FD0932F78C39__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Track.h"


/*
*类名: CPolygonTrack
*说明：定义六边形轨迹类
*版本
*	Revision 1.0	2002/11/23     	 建立 CPolygonTrack 类
*/
class CPolygonTrack : public CTrack  
{
public:
	CPolygonTrack(HDC hDC, COLORREF crTrackColor);
	virtual ~CPolygonTrack();


protected:
	virtual BOOL CreatePos();
	void Draw(const CPoint& ptStart, const CPoint& ptEnd) const;
};

#endif // !defined(AFX_POLYGONTRACK_H__3C0A9CEB_C301_44FF_B85C_FD0932F78C39__INCLUDED_)
