/**
*   LEllipsePenneb.h		
*   Version 1.0
*   CPH	绘画乐园
*   Copyright 2002 . All Rights Reserved.
*   定义上部左倾45度椭圆形笔尖工具
*======================================================
*   版本
*   Revision 1.0	2002/11/14    	 建立 CLEllipsePenNeb 类
*/
#if !defined(AFX_LELLIPSEPENNEB_H__7DED83E7_F715_11D6_95DB_000795CFF65D__INCLUDED_)
#define AFX_LELLIPSEPENNEB_H__7DED83E7_F715_11D6_95DB_000795CFF65D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "PenNeb.h"

/*
*类名: CLEllipsePenNeb
*说明：定义上部左倾45度椭圆形笔尖工具类
*版本
*	Revision 1.0	2002/11/14    	 创建 CLEllipsePebNeb 类
*/
class CLEllipsePenNeb : public CPenNeb  
{
public:
	CLEllipsePenNeb(HDC hDC, int nSize, COLORREF crColor, BOOL bAlpha = FALSE);
	virtual ~CLEllipsePenNeb();


public:
	virtual void NormalApply(const CPoint& ptPoint) const;
	virtual void SetSize(int nSize);


protected:
	virtual void SetAlpha(BYTE * pPixel) ;
	virtual void CreatePenNeb();
};

#endif // !defined(AFX_LELLIPSEPENNEB_H__7DED83E7_F715_11D6_95DB_000795CFF65D__INCLUDED_)
