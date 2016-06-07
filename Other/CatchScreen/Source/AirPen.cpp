/**
*	File Name			:	AirPen.cpp
*	Version				:	1.0
*	Project Information	:	绘画乐园(1003-2002)
*	Copyright			:	Copyright 2002 . All Rights Reserved.
*	Brief Description   :	CAirPen类的函数实现
*====================================================================================
*   版本
*   Revision 1.0	2002/11/06		CAirPen类的函数实现
*
*	Revision 2.0	2002/11/13	
*			修改构造函数参数，修改需用到窗口句柄的相应函数
*/

#include "stdafx.h"
#include "..\\include\\AirPen.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

// 初始化喷枪颜色为黑色
COLORREF CAirPen::g_crPenColor = RGB(0, 0, 0);
// 初始化喷枪的大小为18个像素
int CAirPen::g_nPenSize = 18;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

/*
*--------------------------------------------------------------------------------
*  CAirPen
*  功能：构造函数
*  参数
*		HWND hWnd - 绘图区窗口句柄
*  说明
*		因喷枪用到定时器，需要窗口句柄作为参数
*		设备描述表可由此窗口句柄而得		    
*--------------------------------------------------------------------------------
*/
CAirPen::CAirPen(HWND hWnd, int nSize, COLORREF crColor) : CAbstractPen(GetDC(hWnd), nSize, crColor)
{
	m_hWnd = hWnd;
	m_strPenName = _T("喷枪");
	g_nPenSize = nSize;
	g_crPenColor = crColor;
//	m_hCursor = LoadCursor(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDC_AIRPEN));
    m_hCursor = AfxGetApp()->LoadCursor(IDC_AIRPEN);
}


CAirPen::~CAirPen()
{

}


/*
*--------------------------------------------------------------------------------
*  Member Name:					SetPenColor
*  Function Description:		设置喷枪的颜色
*  Parameter Specification:	
        COLORREF	crColor		-要设定的颜色
*  Return Specification:		无 
*   Algorithm:					无		
*   Addtion:				    
        前置条件：				无
		后置条件：				喷枪被设定为指定颜色
*--------------------------------------------------------------------------------
*/

void CAirPen::SetPenColor(COLORREF crColor)
{
	g_crPenColor = crColor;
}

/*
*--------------------------------------------------------------------------------
*  Member Name:					SetPenStyle
*  Function Description:		无功能
*  Parameter Specification:	
        PEN_STYLE   PenStyle	-被忽略
*  Return Specification:		无 
*   Algorithm:					无		
*   Addtion:				    
        前置条件：				无
		后置条件：				无
		由于喷枪在固定圆形区域中产生离散的点，所以该函数无作为
*--------------------------------------------------------------------------------
*/

//DEL void CAirPen::SetPenStyle(PEN_STYLE PenStyle)
//DEL {
//DEL }

/*
*--------------------------------------------------------------------------------
*  Member Name:					SetPenSize
*  Function Description:		设置喷枪的喷洒范围
*  Parameter Specification:	
          int  nSize			-喷枪喷洒的圆形区域的半径
*  Return Specification:		无 
*   Algorithm:					无		
*   Addtion:				    
        前置条件：				喷枪已被初始化
		后置条件：				喷绘区域变为指定大小的圆形
*--------------------------------------------------------------------------------
*/

void CAirPen::SetPenSize(int nSize)
{
	CAirPen::g_nPenSize = nSize;
}

/*
*--------------------------------------------------------------------------------
*  Member Name:					BiginDraw
*  Function Description:		开始喷绘
*  Parameter Specification:	
        HDC hDC					-当前绘图区的设备上下文句柄 
		CPoint &ptPoint         -当前鼠标所在位置的逻辑坐标
*  Return Specification:		无 
*   Algorithm:					无		
*   Addtion:				    
        前置条件：				绘图区已被创建，喷枪类已被创建
		后置条件：				在屏幕指定区域显示点
*--------------------------------------------------------------------------------
*/
void CAirPen::BeginDraw(const CPoint &ptPoint)
{
	::SetTimer(m_hWnd, 1, 1, (TIMERPROC) & TimerProc);
}


/*
*--------------------------------------------------------------------------------
*  Member Name:					TimerProc
*  Function Description:		回调函数处理点的绘制
*  Parameter Specification:	
        HWND     hWnd		    -窗口的句柄 
		UINT     uMsg           -WM_TIMER消息
        UINT_PTR idEvent		-计时器号
		DWORD    dwTime			-当前系统时间
*  Return Specification:		无 
*   Algorithm:					随机算法		
*   Addtion:				    
        前置条件：				计时器成功初始化
		后置条件：				以每55毫秒30个的速率在制定区域产生随机点
*--------------------------------------------------------------------------------
*/
VOID CALLBACK CAirPen::TimerProc(HWND hWnd, UINT uMsg, UINT_PTR idEvent, 
		                           DWORD dwTime)
{
	CPoint CurPoint;
	::GetCursorPos(&CurPoint);
	::ScreenToClient(hWnd, &CurPoint);
    
    int x, y, ratius = g_nPenSize;
	ratius = g_nPenSize;
	TRACE("size = %d\t", g_nPenSize);
	HDC hDC = ::GetDC(hWnd);

    for(int i = 0; i < 30; i++)
	{
		int irand = rand();
	    float Temp = (float)ratius * irand * 2 / RAND_MAX;
	    x = (int)(Temp - ratius +0.5);
	
		irand = rand();
        Temp = (float)ratius * irand * 2 / RAND_MAX;
	    y = (int)(Temp - ratius + 0.5);
	
    
        // 如果点在圆形区域内，则画点
	    if (((x * x) + (y * y)) < (ratius * ratius))
			::SetPixel(hDC, (CurPoint.x + x), (CurPoint.y + y), g_crPenColor);
	}
}


/*
*--------------------------------------------------------------------------------
*  Member Name:					DrawPoint
*  Function Description:		无作为
*  Parameter Specification:	
        HDC hDC					-当前绘图区的设备上下文句柄 
		CPoint &ptPoint         -当前鼠标所在位置的逻辑坐标
*  Return Specification:		无 
*   Algorithm:					无		
*   Addtion:				    
        前置条件：				画笔对象已被创建并正确初始化
		后置条件：				无
*--------------------------------------------------------------------------------
*/
void CAirPen::DrawPoint(const CPoint &ptPoint)
{
}


/*
*--------------------------------------------------------------------------------
*  Member Name:					EndDraw
*  Function Description:		结束喷绘
*  Parameter Specification:	
        HDC hDC					-当前绘图区的设备上下文句柄 
		CPoint &ptPoint         -当前鼠标所在位置的逻辑坐标
*  Return Specification:		无 
*   Algorithm:					无		
*   Addtion:				    
        前置条件：				计时器已被创建并开始工作
		后置条件：				终止计时器计时
*--------------------------------------------------------------------------------
*/
void CAirPen::EndDraw(const CPoint &ptPoint)
{	
	::KillTimer(m_hWnd, 1);
}