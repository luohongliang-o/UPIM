/**
*   EllipseTrack.h		
*   Version 1.0
*   CPH	绘画乐园
*   Copyright 2002 . All Rights Reserved.
*   定义直线轨迹
*======================================================
*   版本
*   Revision 1.0	2002/11/11   	 建立 CLineTrack 类
*/
#if !defined(AFX_LINETrack_H__212CA942_F6EF_11D6_95DB_000795CFF65D__INCLUDED_)
#define AFX_LINETrack_H__212CA942_F6EF_11D6_95DB_000795CFF65D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Track.h"

/*
*类名: CLineTrack
*说明：定义直线轨迹类
*版本
*	Revision 1.0	2002/11/11    	 建立 CLineTrack 类
*/
class CLineTrack : public CTrack  
{
public:
	CLineTrack(HDC hDC, COLORREF crTrackColor);
	virtual ~CLineTrack();


protected:
	// 根据轨迹创建点的集合
	virtual BOOL CreatePos();

	// 画直线
	virtual void Draw(const CPoint& ptStart, const CPoint& ptEnd) const;

};

#endif // !defined(AFX_LINETrack_H__212CA942_F6EF_11D6_95DB_000795CFF65D__INCLUDED_)
