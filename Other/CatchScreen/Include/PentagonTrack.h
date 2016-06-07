/**
*   PentagonTrack.h		
*   Version 1.0
*   CPH	绘画乐园
*   Copyright 2002. All Rights Reserved.
*   定义五角星轨迹类
*======================================================
*   版本
*   Revision 1.0	2002/11/11   	 建立 CPentagonTrack 类
*/
#if !defined(AFX_PENTAGON_H__DB58ED1A_EAA6_4785_B056_DC5064407996__INCLUDED_)
#define AFX_PENTAGON_H__DB58ED1A_EAA6_4785_B056_DC5064407996__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Track.h"


/*
*类名: CPentagonTrack
*说明：定义各种椭圆轨迹类
*版本
*	Revision 1.0	2002/11/11     	 建立 CPentagonTrack 类
*/
class CPentagonTrack : public CTrack  
{
public:
	CPentagonTrack(HDC hDC, COLORREF crTrackColor);
	virtual ~CPentagonTrack();


protected:
	// 根据轨迹创建点的集合
	virtual BOOL CreatePos();
	
	// 画出五角星
	void Draw(const CPoint& ptStart, const CPoint& ptEnd) const;
};

#endif // !defined(AFX_PENTAGON_H__DB58ED1A_EAA6_4785_B056_DC5064407996__INCLUDED_)
