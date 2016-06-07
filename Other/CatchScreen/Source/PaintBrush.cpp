/*
*  class PaintBrush
*  说明：画刷类的实现
*  Copyright 2002 . All Rights Reserved.
*====================================================
*  版本变更记录
*  Revision 1.0 2002/11/6  创建
*
*  Revision 2.0	2002/11/12   	 调整类层次，将笔效果放入笔尖内中
*
*  Revision 2.1	2002/11/27   	 使画刷大小比别的笔大两倍，重载 SetPenSize 函数
*/

#include "stdafx.h"
#include "..\Include\PaintBrush.h"

#include <math.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPaintBrush::CPaintBrush(HDC hDC, int nSize, COLORREF crColor) : CAbstractPen(hDC, nSize, crColor)
{
	m_strPenName = _T("画刷");
//	m_hCursor = LoadCursor(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDC_PAINTBRUSH));
    //m_hCursor = AfxGetApp()->LoadCursor(IDC_PAINTBRUSH);
	// 画刷使用 Alpha 笔尖效果
	m_bAlpha = TRUE;

	// 画刷笔大小是其他笔的两倍
	m_nPenSize = nSize * 2;

	SetPenStyle(PS_CIRCLE);
}


CPaintBrush::~CPaintBrush()
{

}


/*
*--------------------------------------------------------------------------------
*  函数名: SetPenSize
*  功能	 : 设置画刷的大小
*  参数  : int  nSize	-	笔的大小
*  算法	 : 画刷大小是其他笔大小的两倍，在此放大
*--------------------------------------------------------------------------------
*/
void CPaintBrush::SetPenSize(int nSize)
{
	if (nSize <= 1)
		nSize = 2;

	m_nPenSize = (int)(nSize * 2);
	
	CAbstractPen::SetPenSize(m_nPenSize);
}
