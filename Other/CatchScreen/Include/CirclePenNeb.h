/**
*   CirclePenneb.h		
*   Version 1.0
*   CPH	绘画乐园
*   Copyright 2002 . All Rights Reserved.
*   定义圆形笔尖工具
*======================================================
*   版本
*   Revision 1.0	2002/11/14   	 建立 CCirclePenNeb 类
*/
#if !defined(AFX_CIRCLEPENNEB_H__7DED83E3_F715_11D6_95DB_000795CFF65D__INCLUDED_)
#define AFX_CIRCLEPENNEB_H__7DED83E3_F715_11D6_95DB_000795CFF65D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "PenNeb.h"

/*
*类名: CCirclePenNeb
*说明：定义圆形笔尖效果类
*版本
*	Revision 1.0	2002/11/14    	 创建 CCirclePebNeb 类
*/
class CCirclePenNeb : public CPenNeb  
{
public:
	CCirclePenNeb(HDC hDC, int nSize, COLORREF crColor, BOOL bAlpha = FALSE);
	virtual ~CCirclePenNeb();


public:
	virtual void SetSize(int nSize);


protected:
	virtual void SetAlpha(BYTE * pPixel) ;
};

#endif // !defined(AFX_CIRCLEPENNEB_H__7DED83E3_F715_11D6_95DB_000795CFF65D__INCLUDED_)
