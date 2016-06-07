/**
*   Pencil.h		
*   Version 1.0
*   CPH	绘画乐园
*   Copyright 2002  All Rights Reserved.
*   实现铅笔类
*======================================================
*   版本
*	Revision 1.0	2002/11/12   	 建立 CPencil 类
*/
#if !defined(AFX_PENCIL_H__47D10E43_66DD_4899_AAA2_E50DD37847D6__INCLUDED_)
#define AFX_PENCIL_H__47D10E43_66DD_4899_AAA2_E50DD37847D6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "AbstractPen.h"


/*
*类名: CPencil
*说明：定义铅笔类
*版本
*	Revision 1.0	2002/11/12     	 完成 CPencil 的基本功能
*/
class CPencil : public CAbstractPen  
{
public:
	CPencil(HDC hDC, int nSize = 12, COLORREF crColor = RGB(0, 0, 0));
	virtual ~CPencil();
};

#endif // !defined(AFX_PENCIL_H__47D10E43_66DD_4899_AAA2_E50DD37847D6__INCLUDED_)
