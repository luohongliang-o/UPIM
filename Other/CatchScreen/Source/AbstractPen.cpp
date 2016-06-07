/**
*   AbstractPen.cpp		
*   Version 2.0
*   CPH	绘画乐园
*   Copyright 2002 . All Rights Reserved.
*   实现笔的抽象类
*======================================================
*   版本
*   Revision 1.0	2002/11/1     	 实现 CAbstractPen 类
*	
*	Revision 2.0	2002/11/12    	 给画笔加入轨迹功能
*/


#include "stdafx.h"
#include <math.h>

#include "..\Include\PenNeb.h"
#include "..\Include\CirclePenNeb.h"
#include "..\Include\HEllipsePenNeb.h"
#include "..\Include\VEllipsePenNeb.h"
#include "..\Include\REllipsePenNeb.h"
#include "..\Include\LEllipsePenNeb.h"

#include "..\Include\AbstractPen.h"
#include "..\Include\Track.h"
#include "..\Include\RandomTrack.h"
#include "..\Include\LineTrack.h"
//#include "PentagonTrack.h"
#include "..\Include\RectangleTrack.h"
//#include "TriangleTrack.h"
//#include "TetragonTrack.h"
#include "..\Include\EllipseTrack.h"
//#include "FiveSideTrack.h"
//#include "PolygonTrack.h"
#include "..\Include\ArrowTrack.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
/*
*--------------------------------------------------------------------------------
*  功能: 设置笔属性的缺省值
*  参数: HDC hDC  -	用笔绘画的设备描述表句柄
*		 int nSize - 笔的大小
*		 COLORREF crColor - 笔的颜色
*  缺省值:	笔宽		-   12
*			笔尖形状	-	圆形
*			笔的轨迹	-	任意轨迹
*			笔的颜色	-	黑色
*--------------------------------------------------------------------------------
*/
CAbstractPen::CAbstractPen(HDC hDC, int nSize, COLORREF crColor)
	: m_hDC(hDC), m_nPenSize(nSize), m_crPenColor(crColor)
{
	m_pPenNeb = NULL;
	m_bAlpha = FALSE;
	// 需先初始化 m_bAlpha
	SetPenStyle(PS_CIRCLE);

	m_pPenTrack = NULL;
	SetPenTrack(PT_RANDOM);
}


CAbstractPen::~CAbstractPen()
{
	if (m_pPenTrack)
		delete m_pPenTrack;
	m_pPenTrack = NULL;

	if (m_pPenNeb)
		delete m_pPenNeb;
	m_pPenNeb = NULL;
}


/*
*--------------------------------------------------------------------------------
*  函数名: BeginDraw
*  功能	 : 开始进入绘画状态
*  参数  : CPoint ptPoint  - 当前的坐标点
*  后置条件:	委托给轨迹对象进行相应处理
*--------------------------------------------------------------------------------
*/
void CAbstractPen::BeginDraw(const CPoint& ptPoint)
{	
	ASSERT(m_pPenTrack);

	if (m_PenTrack == PT_RANDOM)
	{
		//任意轨迹，每次鼠标移动时即结束一次轨迹并开始新的一次轨迹
		m_pPenTrack->BeginTrack(ptPoint);
		Apply(ptPoint);
	}
	else
	{
		// 具有形状轨迹
		m_pPenTrack->BeginTrack(ptPoint);
	}
}


/*
*--------------------------------------------------------------------------------
*  函数名: DrawPoint
*  功能	 : 绘点
*  参数  : CPoint ptPoint - 当前的坐标点
*  算法  : 委托给轨迹对象进行画点处理
*  前置条件: 必须先调用 BeginDraw
*--------------------------------------------------------------------------------
*/
void CAbstractPen::DrawPoint(const CPoint& ptPoint)
{
	ASSERT(m_pPenTrack);

	if (m_PenTrack == PT_RANDOM)
	{		
		// 任意轨迹，在每次鼠标移动时建立一次轨迹

		//结束上一次的轨迹
		m_pPenTrack->EndTrack(ptPoint);
		FillTrack();

		//开始新一次轨迹
		m_pPenTrack->BeginTrack(ptPoint);
	}
	else
	{
		m_pPenTrack->DrawTrack(ptPoint);
	}
}


/*
*--------------------------------------------------------------------------------
*  函数名: EndDraw
*  功能	 : 结束绘画
*  参数  : CPoint ptPoint  - 当前的坐标点
*  算法  : 委托给轨迹对象进行处理
*--------------------------------------------------------------------------------
*/
void CAbstractPen::EndDraw(const CPoint& ptPoint)
{
	ASSERT(m_pPenTrack);

	m_pPenTrack->EndTrack(ptPoint);

	//填充轨迹上的点集合
	FillTrack();
}


/*
*--------------------------------------------------------------------------------
*  函数名: SetPenColor
*  功能	 : 设置笔的颜色
*  参数  : COLORREF crColor	-  笔的颜色
*  算法	 : 委托给笔尖对象设置笔尖颜色，同时保存颜色值
*--------------------------------------------------------------------------------
*/
void CAbstractPen::SetPenColor(COLORREF crColor)
{
	ASSERT(m_pPenNeb);

	m_crPenColor = crColor ;
	m_pPenNeb->SetColor(crColor) ;
	m_pPenTrack->SetTrackColor(crColor);
}


/*
*--------------------------------------------------------------------------------
*  函数名: SetPenSize
*  功能	 : 设置笔的大小
*  参数  : int  nSize	-	笔的大小
*  算法	 : 委托给笔尖对象设置笔尖大小，同时保存笔大小值
*--------------------------------------------------------------------------------
*/
void CAbstractPen::SetPenSize(int nSize)
{
	ASSERT(m_pPenNeb);

	m_nPenSize = nSize;
	m_pPenNeb->SetSize(nSize);
}


/*
*--------------------------------------------------------------------------------
*  函数名: SetPenStyle
*  功能	 : 设置笔的大小
*  参数  : PEN_STYLE PenStyle	-	笔尖形状
*  算法	 : 删除旧的笔尖对象，新建新笔尖对象
*--------------------------------------------------------------------------------
*/
void CAbstractPen::SetPenStyle(PEN_STYLE PenStyle)
{
	if (m_pPenNeb)
		delete m_pPenNeb;

	switch (PenStyle)
	{
	case PS_CIRCLE:
		m_pPenNeb = new CCirclePenNeb(m_hDC, m_nPenSize, m_crPenColor, m_bAlpha);
		break;

	case PS_HELLIPSE:
		m_pPenNeb = new CHEllipsePenNeb(m_hDC, m_nPenSize, m_crPenColor, m_bAlpha);
		break;

	case PS_VELLIPSE:
		m_pPenNeb = new CVEllipsePenNeb(m_hDC, m_nPenSize, m_crPenColor, m_bAlpha);
		break;

	case PS_RELLIPSE:
		m_pPenNeb = new CREllipsePenNeb(m_hDC, m_nPenSize, m_crPenColor, m_bAlpha);
		break;

	case PS_LELLIPSE:
		m_pPenNeb = new CLEllipsePenNeb(m_hDC, m_nPenSize, m_crPenColor, m_bAlpha);
		break;

	default:
		ASSERT(0);
	}	
	m_pPenNeb->InitPenNeb();
}


/*
*--------------------------------------------------------------------------------
*  函数名: SetPenTrack
*  功能	 : 设置笔的轨迹
*  参数  : [in]  PEN_TRACK PenTrack	- 笔的轨迹形状
*  返回值: 如果轨迹类型合法返回 TRUE，否则返回 FALSE
*  算法  : 删除旧的笔轨迹对象，新建新的笔轨迹对象
*--------------------------------------------------------------------------------
*/
BOOL CAbstractPen::SetPenTrack(PEN_TRACK PenTrack)
{	
	if (m_pPenTrack)
		delete m_pPenTrack;
	m_pPenTrack = NULL;

	m_PenTrack = PenTrack;

	switch (PenTrack)
	{
	case PT_RANDOM:
		// 任意轨迹
		m_pPenTrack = new CRandomTrack(m_hDC, m_crPenColor);
		break;

	case PT_LINE:
		// 直线
		m_pPenTrack = new CLineTrack(m_hDC, m_crPenColor);
		break;

	case PT_ELLIPSE:
		// 椭圆
		m_pPenTrack = new CEllipseTrack(m_hDC, m_crPenColor);
		break;
		
// 	case PT_TRIANGLE:
// 		// 三角形
// 		m_pPenTrack = new CTriangleTrack(m_hDC, m_crPenColor);
// 		break;

	case PT_RECTANGLE:
		// 矩形
		m_pPenTrack = new CRectangleTrack(m_hDC, m_crPenColor);
		break;

// 	case PT_TETRAGON:
// 		// 四角星	
// 		m_pPenTrack = new CTetragonTrack(m_hDC, m_crPenColor);
// 		break;

// 	case PT_PENTAGON:
// 		// 五角星
// 		m_pPenTrack = new CPentagonTrack(m_hDC, m_crPenColor);
// 		break;

// 	case PT_FIVESIDE:
// 		// 五边形
// 		m_pPenTrack = new CFiveSideTrack(m_hDC, m_crPenColor);
// 		break;

// 	case PT_POLYGON:
// 		// 多边形
// 		m_pPenTrack = new CPolygonTrack(m_hDC, m_crPenColor);
// 		break;
	case PT_ARROW:
		m_pPenTrack = new CArrowTrack(m_hDC, m_crPenColor);
		break;

	default:
		;
	}

	return m_pPenTrack ? TRUE : FALSE;
}


/*
*--------------------------------------------------------------------------------
*  函数名: GetCursor
*  功能	 : 显示此种笔的光标
*  返回值: 返回此种笔所对应的光标句柄
*--------------------------------------------------------------------------------
*/
HCURSOR CAbstractPen::GetCursor() const
{	
	return m_hCursor;
}


/*
*--------------------------------------------------------------------------------
*  函数名: FillTrack
*  功能	 : 用笔填充轨迹上的每一个点
*  算法	 : 扫描轨迹点集合，在每一个点上都用当前笔画点
*  说明	 : 为提高填充速度，加入优化。当两个相邻点之间距离小于指定
*			大小时跳过此点，直到大于此距离之后才填充点
*--------------------------------------------------------------------------------
*/
void CAbstractPen::FillTrack()
{
	ASSERT(m_pPenTrack);

	POSITION pos;
	pos = m_pPenTrack->GetHeadPosition();

	// 最小点距
	int nInterval = m_nPenSize / 10;
	CPoint ptLast(0,0);
	// 第一次画点不算距离
	BOOL	bFirst = TRUE;

	while (pos)
	{	
		CPoint& point = m_pPenTrack->GetNextPoint(pos);
		int x = point.x - ptLast.x;
		int y = point.y - ptLast.y;
		int dis = (int) (sqrt((float)x * x + y * y));						

		if (dis > nInterval || bFirst)
		{
			Apply(point);
			ptLast = point;
			bFirst = FALSE;
		}
	}
}


/*
*--------------------------------------------------------------------------------
*  函数名: Apply
*  功能	 : 在指定座标点上以当前笔效画点
*  参数  : [in] CPoint& ptPoint		-	指定点的座标
*  算法  : 委托给笔尖形状对象
*--------------------------------------------------------------------------------
*/
void CAbstractPen::Apply(const CPoint& ptPoint) const
{
	m_pPenNeb->Apply(ptPoint);
}
