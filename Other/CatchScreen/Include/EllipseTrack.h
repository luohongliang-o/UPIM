/**
*   EllipseTrack.h		
*   Version 1.0
*   CPH	绘画乐园
*   Copyright 2002p. All Rights Reserved.
*   定义椭圆形轨迹
*======================================================
*   版本
*   Revision 1.0	2002/11/11  	 建立 CEllipseTrack 类
*/
#if !defined(AFX_ELLIPSETRACK_H__8E8BA802_F6E1_11D6_95DB_000795CFF65D__INCLUDED_)
#define AFX_ELLIPSETRACK_H__8E8BA802_F6E1_11D6_95DB_000795CFF65D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Track.h"

/*
*类名: CEllipseTrack
*说明：定义各种椭圆轨迹类
*版本
*	Revision 1.0	2002/11/11      建立 CEllipseTrack 类
*/
class CEllipseTrack : public CTrack  
{
public:
	CEllipseTrack(HDC hDC, COLORREF crTrackColor);
	virtual ~CEllipseTrack();


protected:
	void CalEllipsePath(int xCenter, int yCenter, int Rx, int Ry);
	// 根据轨迹创建点的集合
	virtual BOOL CreatePos();
	// 画矩形
	virtual void Draw(const CPoint& ptStart, const CPoint& ptEnd) const;
};

#endif // !defined(AFX_ELLIPSETRACK_H__8E8BA802_F6E1_11D6_95DB_000795CFF65D__INCLUDED_)
