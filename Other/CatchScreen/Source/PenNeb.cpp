/**
*   PenNeb.cpp
*   Version 1.0
*   CPH	绘画乐园
*   Copyright 2002 . All Rights Reserved.
*   定义各种笔尖的抽象基类，抽取取公共接口
*======================================================
*   版本
*   Revision 1.0	2002/11/12     实现 CPenNeb 类
*/

#include "stdafx.h"
#include "..\Include\PenNeb.h"
#include <math.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


// 定义 AlphaBlend 中的Alpha混合常量
#define AC_SRC_ALPHA	0x01

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

/*
*--------------------------------------------------------------------------------
*  功能: 构造函数
*  参数: HDC hDC  -	用笔尖绘画的设备描述表句柄
*		 COLORREF crColor - 笔尖的颜色
*		 BOOL	bAlpha - 笔尖是否使用 Alpha 混合效果
*--------------------------------------------------------------------------------
*/
CPenNeb::CPenNeb(HDC hDC, COLORREF crColor, BOOL bAlpha)
{
	m_hDC = hDC;
	m_crPenNeb = crColor;
	m_bAlpha = bAlpha;
	m_hMemDC = NULL;
}

CPenNeb::~CPenNeb()
{
	if (m_bAlpha)
		Release();
}


/*
*--------------------------------------------------------------------------------
*  函数名: InitPenNeb
*  功能  : 初始化笔尖对象
*  算法  : 为创建 Alpha 笔而设，因为要用到虚拟函数，故不能用构造函数替代
*--------------------------------------------------------------------------------
*/
void CPenNeb::InitPenNeb()
{
	if (m_bAlpha)
		CreateAlphaPen();
}


/*
*--------------------------------------------------------------------------------
*  函数名: Apply
*  功能	 : 在指定座标点上以当前笔效画点
*  参数  : CPoint& ptPoint	-	指定点的座标
*  算法  : 笔效分普通效果和 Alpha 混合特效
*--------------------------------------------------------------------------------
*/
void CPenNeb::Apply(const CPoint &ptPoint) const
{
	if (m_bAlpha)
	{
		// Alpha 混合特效
		AlphaApply(ptPoint);
	}
	else
	{
		// 为防失真，小于2的笔宽直接画像素点，均为一像素大
		if (m_PenNebSize.cx <= 2 || m_PenNebSize.cy <= 2)
		{
			SetPixel(m_hDC, ptPoint.x, ptPoint.y, m_crPenNeb);
		}
		else
			NormalApply(ptPoint);
	}
}


/*
*--------------------------------------------------------------------------------
*  函数名: NormalApply
*  功能	 : 在指定座标点上以普通笔效画点
*  参数  : CPoint& ptPoint	-	指定点的座标
*  算法  : 基类缺省行为是在点上以当前笔色画出笔宽大小的椭圆
*--------------------------------------------------------------------------------
*/
void CPenNeb::NormalApply(const CPoint &ptPoint) const
{
	HBRUSH hBrush = CreateSolidBrush(m_crPenNeb);
	HBRUSH hOldBrush = (HBRUSH) SelectObject(m_hDC, hBrush);
	HPEN hOldPen = (HPEN) SelectObject(m_hDC, GetStockObject(NULL_PEN));

	{
		int nX, nY;
		nX = m_PenNebSize.cx / 2;
		nY = m_PenNebSize.cy / 2;
		if (m_PenNebSize.cx == 3)
		{
			nX = 2;
		}
		if (m_PenNebSize.cy == 3)
		{
			nY = 2;
		}
		Ellipse(m_hDC, 
			ptPoint.x - nX, 
			ptPoint.y - nY, 
			ptPoint.x + nX, 
			ptPoint.y + nY);
	}

	SelectObject(m_hDC, hOldPen);
	SelectObject(m_hDC, hOldBrush);
	DeleteObject(hOldPen);
	DeleteObject(hOldBrush);
	DeleteObject(hBrush);
}


/*
*--------------------------------------------------------------------------------
*  函数名: AlphaApply
*  功能	 : 在指定坐标点的设备上用 Alpha 笔效绘点
*  参数	 : CPoint& ptPoint	-	指定点的座标
*  算法	 : 调用 Alpha 混合函数在指定点上作点
*  前置条件: 已创建笔尖的 32 位 DIB 图像，即要先调用 InitPen 函数
*--------------------------------------------------------------------------------
*/
void CPenNeb::AlphaApply(const CPoint &ptPoint) const
{
	BLENDFUNCTION blend = { AC_SRC_OVER, 0, 255, AC_SRC_ALPHA };

	AlphaBlend( m_hDC, 
				ptPoint.x - m_PenNebSize.cx / 2, 
				ptPoint.y - m_PenNebSize.cy / 2, 
				m_PenNebSize.cx,
				m_PenNebSize.cy,
				m_hMemDC, 
				0, 
				0, 
				m_PenNebSize.cx, 
				m_PenNebSize.cy,
				blend);
}


/*
*--------------------------------------------------------------------------------
*  函数名: CreateAlphaPen
*  功能	 : 创建具有 Alpha 效果的笔尖形状DIB位图
*  算法  : 创建一个指定大小和颜色的笔尖形状的 DIB 图像，并设置透明度
*  说明  : 采用了 Template Method 模式
*  前置条件: 已设置了笔的 大小、色、笔尖形状
*  后置条件: 创建一个所需要的笔，并将以前的笔删除，将成员变量 m_hPenBrush 设为建立的 DIB 图像的句柄
*--------------------------------------------------------------------------------
*/
void CPenNeb::CreateAlphaPen()
{
	Release();
	
	BYTE * pBits;
	BITMAPINFO Bmi = { { sizeof(BITMAPINFOHEADER), 
						m_PenNebSize.cx, m_PenNebSize.cy,
						1, 32, BI_RGB } };

	// 创建笔的 DIB 图像
	m_hPenBrush = CreateDIBSection(m_hDC, &Bmi, 
								DIB_RGB_COLORS,
								(void **) &pBits, 
								NULL, NULL);

	m_hMemDC = CreateCompatibleDC(NULL);
	m_hOldBrush = (HBITMAP) SelectObject(m_hMemDC, m_hPenBrush);
	

	// 在已创建的 DIB 图像上作出笔尖形状的图形
	{
	PatBlt(m_hMemDC, 0, 0, m_PenNebSize.cx, m_PenNebSize.cy, WHITENESS);

	HBRUSH hBrush = CreateSolidBrush(m_crPenNeb);
	HPEN hPen = CreatePen(1, 1, m_crPenNeb);
	HBRUSH hOldBrush = (HBRUSH) SelectObject(m_hMemDC, hBrush);
	HPEN hOldPen = (HPEN) SelectObject(m_hMemDC, GetStockObject(NULL_PEN));

	// 画出笔尖图像，调用派生类的重载函数
	CreatePenNeb();

	SelectObject(m_hMemDC, hOldPen);
	SelectObject(m_hMemDC, hOldBrush);
	DeleteObject(hPen);
	DeleteObject(hOldPen);
	DeleteObject(hOldBrush);
	DeleteObject(hBrush);
	}

	// 设置笔尖图像每个像素点的 Alpha 值，虚函数
	SetAlpha(pBits);
}


/*
*--------------------------------------------------------------------------------
*  函数名: Release
*  功能	 : 释放笔所占用的资源，重新初始化成员变量
*  后置条件: 删除已创建的笔，释放各种资源
*--------------------------------------------------------------------------------
*/
void CPenNeb::Release()
{
	// 未曾创建笔尖
	if (m_hMemDC == NULL)
		return;

	SelectObject(m_hMemDC, m_hOldBrush);
	DeleteObject(m_hOldBrush);
	DeleteObject(m_hMemDC);
	DeleteObject(m_hPenBrush);
	
	m_hOldBrush = NULL;
	m_hMemDC = NULL;
	m_hPenBrush = NULL;
}


/*
*--------------------------------------------------------------------------------
*  函数名: CreatePenNeb
*  功能	 : 创建笔时在笔的位图上画出笔尖形状
*  后置条件: 基类的缺省行为是画出一个椭圆
*--------------------------------------------------------------------------------
*/
void CPenNeb::CreatePenNeb()
{
	Ellipse(m_hMemDC, 0, 0, m_PenNebSize.cx + 1, m_PenNebSize.cy + 1);
}


/*
*--------------------------------------------------------------------------------
*  函数名: SetColor
*  功能	 : 设置笔尖的颜色
*  参数  : COLORREF crColor	-  笔尖颜色
*  后置条件: 设置笔色，如果是 Alpha 笔则重新创建笔尖图形
*--------------------------------------------------------------------------------
*/
void CPenNeb::SetColor(COLORREF crColor)
{
	m_crPenNeb = crColor;

	if (m_bAlpha)
		CreateAlphaPen();
}



//-----------------------------------------------------------------------------



/*
*--------------------------------------------------------------------------------
*  函数名: DrawRotateEllipse
*  功能	 : 画出一个旋转指定角度的椭圆
*  参数  : HDC hDC			- 设备描述表
*			CRect& rect		- 椭圆的外矩形
*			double radians	- 旋转的角度
*  算法  : 通过四条贝兹曲线来模拟画椭圆。先计算出四条 Bezier 曲线的
*		13个控制点，再将这些控制点旋转指定角度，来出贝兹线，最后
*		填充此椭圆内部区域。
*--------------------------------------------------------------------------------
*/
void CPenNeb::DrawRotateEllipse(HDC hDC, const CRect& rect, double radians) const
{
	CPoint pt[13] ;		
	
	// 计算出椭圆上的控制点数组
	EllipseToBezier(rect, pt);

	// 顺时针旋转 45 度
	RotateEllipse(radians, rect.CenterPoint(), pt, 13);
	
	BeginPath(hDC);
	PolyBezier(hDC, pt, 13);
	EndPath(hDC);
	FillPath(hDC);
}


/*
*--------------------------------------------------------------------------------
*  函数名: EllipseToBezier
*  功能	 : 从给定矩形得到椭圆转换成贝兹曲线后的控制点数组
*  参数  : CRect & rect		- 椭圆的外矩形
*		   CPoint * cCtlPt	- 经转换后的控制点数组指针	
*--------------------------------------------------------------------------------
*/
void CPenNeb::EllipseToBezier(const CRect & rect, CPoint * cCtlPt) const
{
	// MAGICAL CONSTANT to map ellipse to beziers
    //  			2/3*(sqrt(2)-1) 
    const double EToBConst = 0.2761423749154; 
	
    CSize offset((int)(rect.Width() * EToBConst), (int)(rect.Height() * EToBConst));
	//  Use the following line instead for mapping systems where +ve Y is upwards
	//  CSize offset((int)(r.Width() * EToBConst), -(int)(r.Height() * EToBConst));
	
    CPoint centre((rect.left + rect.right) / 2, (rect.top + rect.bottom) / 2);
	
    cCtlPt[0].x  = rect.left;                 //------------------------/
	cCtlPt[1].x  = rect.left;                 //                        /
	cCtlPt[11].x = rect.left;                 //        2___3___4       /
	cCtlPt[12].x = rect.left;         
											  //     1             5    /
    cCtlPt[5].x  = rect.right;                //     |             |    /
	cCtlPt[6].x  = rect.right;                //     |             |    /
	cCtlPt[7].x  = rect.right;                //     0,12          6    /

    cCtlPt[2].x  = centre.x - offset.cx;      //     |             |    /
	cCtlPt[10].x = centre.x - offset.cx;      //     |             |    /

    cCtlPt[4].x  = centre.x + offset.cx;      //    11             7    /
	cCtlPt[8].x  = centre.x + offset.cx;      //       10___9___8       /

    cCtlPt[3].x  = centre.x;                  //                        /
	cCtlPt[9].x  = centre.x;                  //------------------------*
	
    cCtlPt[2].y  = rect.top;
	cCtlPt[3].y  = rect.top;
	cCtlPt[4].y  = rect.top;

    cCtlPt[8].y  = rect.bottom;
	cCtlPt[9].y  = rect.bottom;
	cCtlPt[10].y = rect.bottom;

    cCtlPt[7].y  = centre.y + offset.cy;
	cCtlPt[11].y = centre.y + offset.cy;

    cCtlPt[1].y  = centre.y - offset.cy;
	cCtlPt[5].y  = centre.y - offset.cy;

    cCtlPt[0].y  = centre.y;
	cCtlPt[12].y = centre.y;
	cCtlPt[6].y  = centre.y;
}


/*
*--------------------------------------------------------------------------------
*  函数名: RotateEllipse
*  功能	 : 画出一个旋转指定角度的椭圆
*  参数  : double radians	- 椭圆的旋转角度
*		   CPoint & c		- 椭圆的中心点
*		   CPoint *vCtlPt	- 椭圆的贝兹控制点数组
*		   unsigned int Cnt	- 控制点个数
*--------------------------------------------------------------------------------
*/
void CPenNeb::RotateEllipse(double radians, const CPoint &c, CPoint *vCtlPt, unsigned int Cnt) const
{
    for (int i = Cnt-1; i>=0; --i)
    {
		RotatePoint(radians, c, vCtlPt[i]);
	}
}


/*
*--------------------------------------------------------------------------------
*  函数名: RotatePoint
*  功能	 : 计算一个点绕中心点旋转指定角度后的坐标
*  参数  : double radians	- 旋转角度，单位为角度
*		   CPoint & c		- 旋转中心点
*		   CPoint & point	- 旋转的点
*--------------------------------------------------------------------------------
*/
void CPenNeb::RotatePoint(double radians, const CPoint& c, CPoint &point) const
{
    double sinAng           = sin(radians);
    double cosAng           = cos(radians);
	double xOffset, yOffset, x, y;
	xOffset = c.x - c.x * cosAng - c.y * sinAng;
	yOffset = c.y + c.x * sinAng - c.y * cosAng;


	x = point.x * cosAng + point.y * sinAng + xOffset;
	y = -point.x * sinAng + point.y * cosAng + yOffset;

	point.x = int (x + 0.5);
	point.y = int (y + 0.5);
}
