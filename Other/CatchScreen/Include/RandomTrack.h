/**
*  RandomTrack.h		
*   Version 1.0
*   CPH	绘画乐园
*   Copyright 2000-2001 . All Rights Reserved.
*   定义任意轨迹类
*======================================================
*   版本
*   Revision 1.0	2002/11/15     	 建立 CRandomTrack 类
*/

#if !defined(AFX_RANDOMTrack_H__AC1D78AA_7752_4B41_B980_83A89DAB6846__INCLUDED_)
#define AFX_RANDOMTrack_H__AC1D78AA_7752_4B41_B980_83A89DAB6846__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Track.h"


/*
*类名: CRandomTrack
*说明：定义任意轨迹类
*版本
*	Revision 1.0	2002/11/15     	 建立 CRandomTrack 类
*/
class CRandomTrack : public CTrack  
{
public:
	CRandomTrack(HDC hDC, COLORREF crTrackColor);
	virtual ~CRandomTrack();


protected:
	// 根据轨迹创建点的集合
	virtual BOOL CreatePos();

	// 画直线
	virtual void Draw(const CPoint& ptStart, const CPoint& ptEnd) const;
};

#endif // !defined(AFX_RANDOMTrack_H__AC1D78AA_7752_4B41_B980_83A89DAB6846__INCLUDED_)
