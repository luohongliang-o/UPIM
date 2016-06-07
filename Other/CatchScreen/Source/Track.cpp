/**
*   Track.cpp		
*   Version 1.0
*   CPH	绘画乐园
*   Copyright 2002. All Rights Reserved.
*   轨迹类的实现文件
*======================================================
*   版本
*   Revision 1.0	2002/11/11   	 建立 CTrack 类的实现
*/


#include "stdafx.h"
#include "..\Include\Track.h"
#include <math.h>

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
*  参数
*	HDC hDC	 - 轨迹绘画的窗口设备描述表
*--------------------------------------------------------------------------------
*/
CTrack::CTrack(HDC hDC, COLORREF crTrackColor)
{
	m_hDC = hDC;
	m_crTrackColor = crTrackColor;
}


CTrack::~CTrack()
{
	m_listTrackPoints.RemoveAll();
}

	
/*
*--------------------------------------------------------------------------------
*  函数名: BeginTrack
*  功能	 : 开始记录轨迹
*  参数  : CPoint& ptPoint	 -  轨迹的起始坐标点
*  后置条件: 将轨迹起点和终点皆设为当前座标点，并画出一个点
*--------------------------------------------------------------------------------
*/
void CTrack::BeginTrack(const CPoint& ptPoint)
{
	m_ptTrackStartPoint.x = ptPoint.x;
	m_ptTrackStartPoint.y = ptPoint.y;
	m_ptTrackEndPoint.x = ptPoint.x;
	m_ptTrackEndPoint.y = ptPoint.y;

	// 画出新的轨迹
	PreDraw(ptPoint, ptPoint);
}


/*
*--------------------------------------------------------------------------------
*  函数名: DrawTrack
*  功能	 : 绘出轨迹形状
*  参数  : CPoint ptPoint - 当前的坐标点
*  算法  : 先设置DC，再调用画图函数PreDraw
*  前置条件:	必须先调用 BeginDraw
*--------------------------------------------------------------------------------
*/
void CTrack::DrawTrack(const CPoint &ptPoint)
{
	// 擦除上一次画的轨迹
	PreDraw(m_ptTrackStartPoint, m_ptTrackEndPoint);
	
	// 画出新的轨迹
	PreDraw(m_ptTrackStartPoint, ptPoint);
	
	// 保存座标点
	m_ptTrackEndPoint = ptPoint;	
}


/*
*--------------------------------------------------------------------------------
*  函数名: EndTrack
*  功能	 : 结束记录轨迹
*  参数  : CPoint& ptPoint	 - 当前的坐标点
*  后置条件: 保存当前坐标为轨迹终止点，同时创建轨迹点的集合
*--------------------------------------------------------------------------------
*/
void CTrack::EndTrack(const CPoint& ptPoint)
{
	// 擦除前次画出的轨迹
	PreDraw(m_ptTrackStartPoint, ptPoint);

	m_ptTrackEndPoint = ptPoint;

 	//计算轨迹点
 	PreCreatePos();
}


/*
*--------------------------------------------------------------------------------
*  函数名: PreDraw
*  功能	 : 绘出轨迹形状
*  参数  : CPoint ptPoint	 - 当前的坐标点
*  算法  : 对一些特殊情况进行判断，作出统一处理，再调用派生类的绘轨迹函数 Draw
*			采用 Template Method 模式
*  前置条件:	必须先调用 BeginDraw
*--------------------------------------------------------------------------------
*/
void CTrack::PreDraw(const CPoint &ptStart, const CPoint &ptEnd) const
{
	HDC hDC = m_hDC;
	CBrush brush(m_crTrackColor);
	HBRUSH hOldBrush = (HBRUSH)SelectObject(m_hDC, GetStockObject(NULL_BRUSH));
	HPEN hPen = CreatePen(PS_SOLID, 1, m_crTrackColor);
	HPEN hOldPen = (HPEN) SelectObject(m_hDC, hPen);

	int nOldRop = SetROP2(m_hDC, R2_NOTXORPEN);

	// 调用派生类的画轨迹函数
	Draw(ptStart, ptEnd);


	SetROP2(m_hDC, nOldRop);
	
	SelectObject(m_hDC, hOldPen);
	SelectObject(m_hDC, hOldBrush);	
	DeleteObject(hPen);
	DeleteObject(hOldPen);
	DeleteObject(hOldBrush);
}


/*
*--------------------------------------------------------------------------------
*  函数名: PreCreatePos
*  功能	 : 对一起特殊情况下的轨迹点集合作出处理
*  算法  : 通过 LineDDA 函数得出所有的轨迹点座标
*  前置条件: 必须已经设定了轨迹起始点座标和终止点座标
*  后置条件: 在 m_listTrackPoints 列表中保存所有轨迹点的座标
*--------------------------------------------------------------------------------
*/
BOOL CTrack::PreCreatePos()
{
	m_listTrackPoints.RemoveAll();

	if (m_ptTrackStartPoint.x == m_ptTrackEndPoint.x && m_ptTrackStartPoint.y == m_ptTrackEndPoint.y)
	{
		// 起点和终点相同，轨迹就一个点
		AddPoint(m_ptTrackStartPoint.x, m_ptTrackStartPoint.y);
	}
	else
	{
		// 正常的轨迹形状
		CreatePos();
	}

	return TRUE;
}


/*
*--------------------------------------------------------------------------------
*  函数名: GetIntersectPoint
*  功能	 : 求两条直线的交点座标
*  参数  :
*	[in]	CPoint &pt1Begin	- 直线一的起始点座标
*	[in]	CPoint &pt1End		- 直线一的终点座标
*	[in]	CPoint &pt2Begin	- 直线二的起始点座标
*	[in]	CPoint &pt2End		- 直线二的终点座标
*	[out]	CPoint & ptIntersect	-	交点座标
*  算法:	直线方程	y = k * x + b
*		交点处(x0, y0)
*					y0 = k1 * x + b1
*					y0 = k2 * x + b2
*		求出两直线的方程常量代入上等式即可
*--------------------------------------------------------------------------------
*/
void CTrack::GetIntersectPoint(const CPoint &pt1Begin, const CPoint &pt1End, const CPoint &pt2Begin, const CPoint &pt2End, CPoint& ptIntersect) const
{
	double k1, b1, k2, b2;
	
	BOOL bVer1, bVer2;
	bVer1 = CalLineEquation(pt1Begin, pt1End, &k1, &b1);
	bVer2 = CalLineEquation(pt2Begin, pt2End, &k2, &b2);

	double x, y;

	// 两条线中有一条是直线
	if (!bVer1 || !bVer2)	
	{
		if (!bVer1)
		{
			// 第一条直线是垂直线
			x = pt1Begin.x;
			y =  k2 * (double) x + b2;
		}
		else
		{
			// 第二条线是垂直线
			x = pt2Begin.x;
			y = k1 * (double) x + b1;
		}
	}
	else
	{
		x = (b2 - b1) / (k1 - k2);
		y = k1 * (double) x + b1;
	}
	
	ptIntersect.x = (int) x;
	ptIntersect.y = (int) y;
}


/*
*--------------------------------------------------------------------------------
*  函数名: CalLineEquation
*  功能	 : 求直线的方程式 y = k * x + b  中的 k 和 b
*  参数  :
*	[in]	CPoint &pt1Begin	- 直线一的起始点座标
*	[in]	CPoint &pt1End		- 直线一的终点座标
*	[out]	double * k			- k 值
*	[out]	double * b			- b 值
*  返回值:  BOOL	-	如果直线是垂直线则返回 FALSE， 否则返回 TRUE
*  算法  :	k = (y2 - y1) / (x2 - x1)
*			b = y1 - k * x1
*--------------------------------------------------------------------------------
*/
BOOL CTrack::CalLineEquation(const CPoint &ptBegin, const CPoint &ptEnd, double * k, double * b) const
{
	if (ptBegin.x == ptEnd.x)
	{
		// 垂直线
		return FALSE;
	}
	else if (ptBegin.y == ptEnd.y)
	{
		//水平线
		*k = 0;
	}
	else
	{
		// 斜线
		*k = (double) ((ptBegin.y - ptEnd.y)) / (double) ((ptBegin.x - ptEnd.x));
	}
	*b = ptBegin.y - (*k) * (double) ptBegin.x;

	return TRUE;
}



/*
*--------------------------------------------------------------------------------
*  函数名: SplitCircle
*  功能	 : 将圆等分成指定份数，求出每个点的座标
*  参数  :
*	[in]  CPoint ptCenter		- 圆心座标
*	[in]  CPoint ptBegin		- 指定一个起始顶点座标
*	[in]  int nCount			- 等分份数
*	[out] CPoint *ptVertex	- 返回顶点座标数组
*  算法  : 将圆周分成 nCount 个弧，由弧的起点和扫描角度计算出终点即顶点座标
*--------------------------------------------------------------------------------
*/
BOOL CTrack::SplitCircle(const CPoint &ptCenter, const CPoint &ptBegin, int nCount, CPoint *ptVertex) const
{
	ASSERT(nCount > 2);

	// 角度转弧度常数
	const FLOAT piover180 = (FLOAT) 3.141592653586 / 180;

	CPoint point[4];
	point[0] = ptBegin;
	point[3] = ptBegin;

	int cx, cy, r;
	double a;

	cx = abs(ptBegin.x - ptCenter.x);
	cy = abs(ptBegin.y - ptCenter.y);
	
	// 圆的半径，为保证精度，下面的计算公式里直接用的是公式
	r = int(sqrt((float)cx * cx + (float)cy * cy));

	if (!r)
		return FALSE;

	// 求出弧的起始角度 a
	if (ptBegin.x > ptCenter.x && ptBegin.y < ptCenter.y)
	{	
		// 第一象限
		a = acos(double(cx) / double(sqrt((float)cx * cx + (float)cy * cy))) / piover180;
	}
	else if (ptBegin.x < ptCenter.x && ptBegin.y < ptCenter.y)
	{
		// 第二象限
		a = asin(double(cx) / double(sqrt((float)cx * cx + (float)cy * cy))) / piover180;
		a = 90 + a;
	}
	else if (ptBegin.x < ptCenter.x && ptBegin.y > ptCenter.y)
	{
		// 第三象限
		a = acos(double(cx) / double(sqrt((float)cx * cx + (float)cy * cy))) / piover180;
		a = 180 + a;
	}
	else if (ptBegin.x > ptCenter.x && ptBegin.y > ptCenter.y)
	{
		// 第四象限
		a = asin(double(cx) / double(sqrt((float)cx * cx + (float)cy * cy))) / piover180;
		a = 270 + a;
	}
	else if (ptBegin.x == ptCenter.x && ptBegin.y < ptCenter.y)
	{
		// 位于正Y轴
		a = 90;
	}
	else if (ptBegin.x == ptCenter.x && ptBegin.y > ptCenter.y)
	{
		// 位于 -Y 轴
		a = 270;
	}
	else if (ptBegin.y == ptCenter.y && ptBegin.x > ptCenter.x)
	{
		// 位于正X轴
		a = 0;
	}
	else if (ptBegin.y == ptCenter.y && ptBegin.x < ptCenter.x)
	{
		// 位于负X轴
		a = 180;
	}


	// 每段弧的旋转角度
	double fltArcAngle = 360.0 / (float) nCount;

	// 求出顶点座标数组
	for (int i = 0; i < nCount; i++)
	{
		ptVertex[i] = GetArcEndPoint(ptCenter, sqrt((float)cx * cx + (float)cy * cy), a, fltArcAngle * i);
	}
	
	return TRUE;
}


/*
*--------------------------------------------------------------------------------
*  函数名: GetArcEndPoint
*  功能  :计算一段弧的终点座标
*  参数  :
*	[in]  CPoint ptCenter		- 弧所在圆的圆心座标
*	[in]  DWORD dwRadius		- 弧所在圆的半径
*	[in]  FLOAT eStartAngle		- 弧的起始角度
*	[in]  FLOAT eSweepAngle		- 弧的扫描角度
*  返回值 :	 CPoint	 -	弧的终点座标
*  算法
*		
*--------------------------------------------------------------------------------
*/
CPoint CTrack::GetArcEndPoint(const CPoint &ptCenter, double dwRadius, double eStartAngle, double eSweepAngle) const
{
	// 角度转弧度常数
	const FLOAT piover180 = (FLOAT) 3.141592653586 / 180;

	// 确保最多只画一次圆
	if (eSweepAngle >= 360)
		eSweepAngle = 360;
	else if (eSweepAngle <= -360)
		eSweepAngle = -360;

	// 计算起始角、终止角度的弧度
	double eEndAngle = (eStartAngle + eSweepAngle) * piover180;

	// 计算出弧起始、终止点的座标
	int nXEnd, nYEnd;

	nXEnd = ptCenter.x + (int) (dwRadius * cos(eEndAngle));
	nYEnd = ptCenter.y - (int) (dwRadius * sin(eEndAngle));

	return CPoint(nXEnd, nYEnd);
}

/*
*--------------------------------------------------------------------------------
*  函数名: AddPosToList
*  功能	 : 往轨迹点列表中加入一个点
*  参数  :
*	[in]  int x				- 点的 X 座标
*	[in]  int y				- 点的 Y 座标
*	[in]  LPARAM lpData		- 轨迹对象的指针
*  算法  : 回调函数，用于计算两点中所有点的 LineDDA 函数调用。
*--------------------------------------------------------------------------------
*/
void CALLBACK CTrack::AddPosToList(int x, int y, LPARAM lpData)
{
	CTrack * pTrack = (CTrack*) lpData;
	
	pTrack->AddPoint(x, y);
}


/*
*--------------------------------------------------------------------------------
*  函数名: AddPoint
*  功能	 : 往轨迹点列表中加入一个点
*  参数  :
*	[in]  int x				- 点的 X 座标
*	[in]  int y				- 点的 Y 座标
*  算法  : 真正用于加入点到轨迹链表中
*--------------------------------------------------------------------------------
*/
void CTrack::AddPoint(int x, int y)
{
	// 先判断是否超出范围
	if (x < 0 || y < 0)
		return;

	m_listTrackPoints.AddTail(CPoint(x, y));
}


/*
*--------------------------------------------------------------------------------
*  函数名: GetHeadPosition
*  功能	 : 得到第一个点在轨迹点列表中的位置
*  返回值: POSITION	 -	轨迹第一个点在列表中的位置，用于取下一个点
*--------------------------------------------------------------------------------
*/
POSITION CTrack::GetHeadPosition()
{
	return m_listTrackPoints.GetHeadPosition();
}


/*
*--------------------------------------------------------------------------------
*  函数名: GetNextPoint
*  功能	 : 得到下一个轨迹上的点
*  参数  : [in, out]  POSITION& rPos	-  所欲取点在列表中的位置
*  返回值: CPoint&	 -	轨迹上点的引用
*  后置条件:	rPos = rPos + 1
*		如果已到列表尾，则 rPos = NULL
*--------------------------------------------------------------------------------
*/
CPoint& CTrack::GetNextPoint(POSITION& rPos)
{
	return m_listTrackPoints.GetNext(rPos);	
}


/*
*--------------------------------------------------------------------------------
*  函数名	: SetTrackColor
*  功能		: 设置轨迹的颜色
*  参数		: [in]  COLORREF crTrackColor - 轨迹颜色
*  后置条件 : 设置轨迹颜色
*--------------------------------------------------------------------------------
*/
void CTrack::SetTrackColor(COLORREF crTrackColor)
{
	m_crTrackColor = crTrackColor;
}
