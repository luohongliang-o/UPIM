/////////////////////////////////////////////////////////////////////////////
/**
* 文件名  ：pattern.cpp
* 版本号  ：V1.0
* 项目名称：CPH
* 版权声明：Copyright 2002-2003 . All Rights Reserved.
* 模块简单描述：卡通等贴图类的头文件,实现将动物等图片贴到当前画布上，
*                并可以实现缩放、旋转、镜像等操作   
*======================================================
* 版本变更记录:
*      v1.0  2002-11-7   15:30  
*
* 
*/

/////////////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "..\Paint\PaintDlg.h"
#include "..\\INCLUDE\\pattern.h"
#include "..\\INCLUDE\\DIBitmap.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// Cpattern

Cpattern::Cpattern()
{
	//图片以定制大小显示在画布中心
     m_nType=BITMAP_CUSTOMIZE;
}

Cpattern::~Cpattern()
{
}


BEGIN_MESSAGE_MAP(Cpattern, CWnd)
	//{{AFX_MSG_MAP(Cpattern)
	ON_WM_CREATE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// Cpattern message handlers


//初始化DIB对象,成功返回TRUE，失败返回FALSE
BOOL Cpattern::InitDib()
{
	return TRUE;
}

//更新当前DIB对象,成功返回TRUE，失败返回FALSE
BOOL Cpattern::UpDataDib()
{
	return TRUE;

}

//显示当前DIB对象
Cpattern::ShowDib()
{
}

//隐藏当前DIB对象
Cpattern::HideDib()
{
}

//改变状态
Cpattern::ChangeDibSize()
{
}

//改变大小(缩放)
Cpattern::ChangeDibZoom()
{
}

//得到状态,返回值对应整数1-10,分别表示不同的位图状态
INT Cpattern::GetStatus()
{
	return 0;
}


//得到大小
CRect Cpattern::GetDibSize()
{
   return CRect(0,0,0,0);
}

//得到关联页面指针
HANDLE Cpattern::GetDibPage()
{
	HBITMAP bitmap=NULL;
	return   bitmap;
}


//改变关联页面指针,返回改变后的页面句柄
HANDLE Cpattern::ChangeDibPage()
{
	HBITMAP bitmap=NULL;
	return   bitmap;
}



/*
*--------------------------------------------------------------------------------
*  成员函数名   :Create
*  功能描述     :当创建图片窗口时，修改窗口的类型
*  输入参数描述	:
*  返回变量说明 :
*  算法描述		:
*  附加说明:
*--------------------------------------------------------------------------------
*/
BOOL Cpattern::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CWnd::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
}


/*
*--------------------------------------------------------------------------------
*  成员函数名   :OnCreate
*  功能描述     :初始化类成员变量的值
*  输入参数描述	:
*  返回变量说明 :
*  算法描述		:
*  附加说明:
*--------------------------------------------------------------------------------
*/
int Cpattern::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	
	return 0;
}


/*
*--------------------------------------------------------------------------------
*  成员函数名   :OnLButtonDown
*  功能描述     :鼠标左击图案引发的操作：光标变为移动光标，并开始记录起始点位置
*  输入参数描述	:
*  返回变量说明 :
*  算法描述		:
*  附加说明:
*--------------------------------------------------------------------------------
*/
void Cpattern::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
	CWnd::OnLButtonDown(nFlags, point);
}


/*
*--------------------------------------------------------------------------------
*  成员函数名   :OnLButtonUp
*  功能描述     :鼠标左击图案后Up引发的操作：结束移动托放，记录最后点的位置
*  输入参数描述	:
*  返回变量说明 :
*  算法描述		:
*  附加说明:
*--------------------------------------------------------------------------------
*/
void Cpattern::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
	CWnd::OnLButtonUp(nFlags, point);
}


/*
*--------------------------------------------------------------------------------
*  成员函数名   :OnMouseMove
*  功能描述     :鼠标经过图案引发的操作：当光标在窗体边
*  输入参数描述	:
*  返回变量说明 :
*  算法描述		:
*  附加说明：当光标在窗体边界时，设置光标样式为变化尺寸大小，当光标位于按纽上时，光标变为手状
*            当不
*--------------------------------------------------------------------------------
*/
void Cpattern::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
	CWnd::OnMouseMove(nFlags, point);
}
