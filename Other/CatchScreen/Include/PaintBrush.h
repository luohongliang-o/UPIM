/*
*  class PaintBrush
*  说明：画刷类的实现
*  Copyright 2002-2003. All Rights Reserved.
*====================================================
*  版本变更记录
*  Revision 1.0 2002/11/6 
*
*  Revision 2.0	2002/11/12    	 调整类层次，将笔效果放入笔尖内中
*/

#if !defined(AFX_PAINTBRUSH_H__8584D307_6BE7_4707_848F_B1001F880E9A__INCLUDED_)
#define AFX_PAINTBRUSH_H__8584D307_6BE7_4707_848F_B1001F880E9A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "AbstractPen.h"


/*
*类名: CPaintBrush
*说明：定义画刷类
*版本
*  Revision 1.0 2002/11/6 
*
*  Revision 2.0	2002/11/12    	 调整类层次，将笔效果放入笔尖内中
*
*  Revision 2.1	2002/11/27    	 使画刷大小比别的笔大两倍，重载 SetPenSize 函数
*/
class CPaintBrush : public CAbstractPen
{
public:
	CPaintBrush(HDC hDC, int nSize = 12, COLORREF crColor = RGB(0, 0, 0));
	virtual ~CPaintBrush();
	virtual void SetPenSize(int nSize);
};

#endif // !defined(AFX_PAINTBRUSH_H__8584D307_6BE7_4707_848F_B1001F880E9A__INCLUDED_)
