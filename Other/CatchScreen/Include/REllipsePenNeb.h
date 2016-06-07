/**
*   REllipsePenneb.h		
*   Version 1.0
*   CPH	绘画乐园
*   Copyright 2002 . All Rights Reserved.
*   定义上部右倾45度椭圆形笔尖工具
*======================================================
*   版本
*   Revision 1.0	2002/11/14     建立 CREllipsePenNeb 类
*/
#if !defined(AFX_RELLIPSEPENNEB_H__7DED83E6_F715_11D6_95DB_000795CFF65D__INCLUDED_)
#define AFX_RELLIPSEPENNEB_H__7DED83E6_F715_11D6_95DB_000795CFF65D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "PenNeb.h"

/*
*类名: CREllipsePenNeb
*说明：定义上部右倾45度椭圆形笔尖工具类
*版本
*	Revision 1.0	2002/11/14   	 创建 CREllipsePebNeb 类
*/
class CREllipsePenNeb : public CPenNeb  
{
public:
	CREllipsePenNeb(HDC hDC, int nSize, COLORREF crColor, BOOL bAlpha = FALSE);
	virtual ~CREllipsePenNeb();


public:
	virtual void NormalApply(const CPoint& ptPoint) const;
	virtual void SetSize(int nSize);


protected:
	virtual void SetAlpha(BYTE * pPixel) ;
	virtual void CreatePenNeb();
};

#endif // !defined(AFX_RELLIPSEPENNEB_H__7DED83E6_F715_11D6_95DB_000795CFF65D__INCLUDED_)
