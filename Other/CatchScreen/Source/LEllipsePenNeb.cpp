/**
*   LEllipsePenneb.h		
*   Version 1.0
*   CPH	绘画乐园
*   Copyright 2002 . All Rights Reserved.
*   实现上部左倾45度椭圆形笔尖工具
*======================================================
*   版本
*   Revision 1.0	2002/11/14   	 实现 CLEllipsePenNeb 类
*/
#include "stdafx.h"
#include "..\Include\LEllipsePenNeb.h"
#include <math.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CLEllipsePenNeb::CLEllipsePenNeb(HDC hDC, int nSize, COLORREF crColor, BOOL bAlpha) 
		: CPenNeb(hDC, crColor, bAlpha)
{
	if (bAlpha)
	{
		// 对于Alpha效果旋转过后的矩形框为一正方形
		m_PenNebSize.cx = nSize;
		m_PenNebSize.cy = nSize;
	}
	else
	{
		// 对于普通效果高为宽的两倍
		m_PenNebSize.cx = nSize / 2;
		m_PenNebSize.cy = nSize;
	}
}

CLEllipsePenNeb::~CLEllipsePenNeb()
{

}


/*
*--------------------------------------------------------------------------------
*  函数名	: NormalApply
*  功能		: 在指定座标的点上用普通笔效作图
*  参数		: CPoint& ptPoint	-	作图的点
*  算法		: 直接画椭圆
*--------------------------------------------------------------------------------
*/
void CLEllipsePenNeb::NormalApply(const CPoint& ptPoint) const
{
	HBRUSH hBrush = CreateSolidBrush(m_crPenNeb);
	HBRUSH hOldBrush = (HBRUSH) SelectObject(m_hDC, hBrush);
	HPEN hOldPen = (HPEN) SelectObject(m_hDC, GetStockObject(NULL_PEN));

	DrawRotateEllipse(m_hDC, 
		CRect(ptPoint.x - m_PenNebSize.cx / 2,
		ptPoint.y - m_PenNebSize.cy / 2, 
		ptPoint.x + m_PenNebSize.cx / 2, 
		ptPoint.y + m_PenNebSize.cy / 2),
		45);
	
	SelectObject(m_hDC, hOldPen);
	SelectObject(m_hDC, hOldBrush);
	DeleteObject(hOldPen);
	DeleteObject(hOldBrush);
	DeleteObject(hBrush);
}


/*
*--------------------------------------------------------------------------------
*  函数名: SetAlpha
*  功能	 : 设置笔尖图像上每个像素点的Alpha值
*  参数  : BYTE* pPixel - 笔尖DIB图像的数据指针
*  算法  : 左倾椭圆笔尖，不透明度中间为最高150，向四周过渡到完全透明
*  说明  : 采用 Alpha 混合每个像素点必须为 32 位
*--------------------------------------------------------------------------------
*/
void CLEllipsePenNeb::SetAlpha(BYTE *pPixel)
{
	int nWidth = m_PenNebSize.cx;
	int nHeight = m_PenNebSize.cy;
	
	for (int y = 0; y < nHeight; y++)
		for (int x = 0; x < nWidth; x++, pPixel += 4)
		{
			BYTE alpha = 255;
			int dis;

			// 旋转后的点
			CPoint pt(x, y);
			RotatePoint(45, CPoint(m_PenNebSize.cx / 2, m_PenNebSize.cy / 2), pt);
			pt.x -= nWidth / 4;
			dis = (int) ( sqrt( (float)(pt.x*2-nWidth/2) * (pt.x*2-nWidth/2)
				+ (pt.y-nHeight/2) * (pt.y-nHeight/2)) 
				* 150 / (max(nWidth, nHeight)/2) );
			
			alpha = (BYTE) max(min(150 - dis, 150), 0);


			pPixel[0] = pPixel[0] * alpha / 255;
			pPixel[1] = pPixel[1] * alpha / 255;
			pPixel[2] = pPixel[2] * alpha / 255;
			pPixel[3] = alpha;
		}
}


/*
*--------------------------------------------------------------------------------
*  函数名	: SetSize
*  功能		: 设置笔尖的大小
*  参数		: int nSize	-	笔尖大小
*  算法		: 左倾45度椭圆笔尖的宽度与高度相等
*--------------------------------------------------------------------------------
*/
void CLEllipsePenNeb::SetSize(int nSize)
{
	if (m_bAlpha)
	{
		// 对于Alpha效果旋转过后的矩形框为一正方形
		m_PenNebSize.cx = nSize;
		m_PenNebSize.cy = nSize;
		CreateAlphaPen();
	}
	else
	{
		// 对于普通效果高为宽的两倍
		m_PenNebSize.cx = nSize / 2;
		m_PenNebSize.cy = nSize;
	}
}


/*
*--------------------------------------------------------------------------------
*  函数名	: CreatePenNeb
*  功能		: 创建笔时在笔的位图上画出笔尖形状
*  算法		: 直接画椭圆
*--------------------------------------------------------------------------------
*/
void CLEllipsePenNeb::CreatePenNeb()
{
	// 现在是画椭圆，高为宽的两倍
	CRect rect(m_PenNebSize.cx / 4, 0, 3 * m_PenNebSize.cx / 4, m_PenNebSize.cy);
	CPoint ellipsePts[13];
	EllipseToBezier(rect, ellipsePts);

	// Rotate
	RotateEllipse(45, CPoint(m_PenNebSize.cx / 2, m_PenNebSize.cy / 2) , ellipsePts, 13);
	
	BeginPath(m_hMemDC);		
	PolyBezier(m_hMemDC, ellipsePts, 13);
	EndPath(m_hMemDC);
	FillPath(m_hMemDC);
}