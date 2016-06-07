/**
*   TriangleTrack.h		
*   Version 1.0
*   CPH	绘画乐园
*   Copyright 2002 . All Rights Reserved.
*   定义三角形轨迹形状工具，起始点 m_ptTractStartPoint 为三角形的中心点
*======================================================
*   版本
*   Revision 1.0	2002/11/11    	 建立 CTriangleTrack 类
*/

#if !defined(AFX_TRIANGLETrack_H__0F4DFF3C_FCBD_443F_9630_74E1623DB6D7__INCLUDED_)
#define AFX_TRIANGLETrack_H__0F4DFF3C_FCBD_443F_9630_74E1623DB6D7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Track.h"


/*
*类名: CTriangleTrack
*说明：定义三角形轨迹类
*版本
*	Revision 1.0	2002/11/11     	 建立 CTriangleTrack 类
*/
class CTriangleTrack : public CTrack  
{
public:
	CTriangleTrack(HDC hDC, COLORREF crTrackColor);
	virtual ~CTriangleTrack();


protected:
	// 根据轨迹创建点的集合
	virtual BOOL CreatePos();

	// 画三角形
	virtual void Draw(const CPoint& ptStart, const CPoint& ptEnd) const;
};

#endif // !defined(AFX_TRIANGLETrack_H__0F4DFF3C_FCBD_443F_9630_74E1623DB6D7__INCLUDED_)
