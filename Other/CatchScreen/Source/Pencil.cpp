/**
*   Pencil.cpp		
*   Version 1.0
*   CPH	绘画乐园
*   Copyright 2002 . All Rights Reserved.
*   实现铅笔类
*======================================================
*   版本
*	Revision 1.0	2002/11/12   	 建立 CPencil 类
*/

#include "stdafx.h"
#include "..\Include\Pencil.h"

#include <math.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPencil::CPencil(HDC hDC, int nSize, COLORREF crColor) : CAbstractPen(hDC, nSize, crColor)
{
	m_strPenName = _T("铅笔");
//	m_hCursor = LoadCursor(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDC_PENCIL)); //本行代码在DLL 中无效，应用下面的一行代码
   //m_hCursor = AfxGetApp()->LoadCursor(IDC_PENCIL);
}


CPencil::~CPencil()
{

}