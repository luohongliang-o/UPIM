/**
*  TetragonTrack.h		
*   Version 1.0
*   CPH	绘画乐园
*   Copyright 2000-2001 . All Rights Reserved.
*   定义四角星轨迹类
*======================================================
*   版本
*   Revision 1.0	2002/11/12     	 建立 CTetragonTrack 类
*/
#if !defined(AFX_TETRAGONTrack_H__0192398C_2E22_4218_A64E_0E97D78B1DFA__INCLUDED_)
#define AFX_TETRAGONTrack_H__0192398C_2E22_4218_A64E_0E97D78B1DFA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Track.h"


/*
*类名: CRectangleTrack
*说明：定义四角星轨迹类
*版本
*	Revision 1.0	2002/11/12     	 建立 CTetragonTrack 类
*/
class CTetragonTrack : public CTrack  
{
public:
	CTetragonTrack(HDC hDC, COLORREF crTrackColor);
	virtual ~CTetragonTrack();


protected:
	// 根据轨迹创建点的集合
	virtual BOOL CreatePos();
	// 画四角星
	virtual void Draw(const CPoint& ptStart, const CPoint& ptEnd) const;
};

#endif // !defined(AFX_TETRAGONTrack_H__0192398C_2E22_4218_A64E_0E97D78B1DFA__INCLUDED_)
