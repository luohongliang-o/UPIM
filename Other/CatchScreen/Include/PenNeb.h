/**
*   Penneb.h		
*   Version 1.0
*   CPH	绘画乐园
*   Copyright 2002. All Rights Reserved.
*   定义各种笔尖的抽象基类，抽取取公共接口
*======================================================
*   版本
*   Revision 1.0	2002/11/12 	 建立 CPenNeb 类
*/

#if !defined(AFX_PENNEB_H__7DED83E2_F715_11D6_95DB_000795CFF65D__INCLUDED_)
#define AFX_PENNEB_H__7DED83E2_F715_11D6_95DB_000795CFF65D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


/*
*类名: CPenNeb
*说明：定义各种笔尖的抽象基类，抽取取公共接口
*版本
*	Revision 1.0	2002/11/12    	 创建 CPebNeb 类
*/
class CPenNeb  
{
protected:
	HDC m_hDC;
	BOOL m_bAlpha;
	COLORREF m_crPenNeb;
	CSize m_PenNebSize;
	HBITMAP m_hOldBrush;
	// 保存笔尖图像的句柄
	HBITMAP m_hPenBrush;
	// 保存笔尖图像所在内存设备描述表的句柄
	HDC m_hMemDC;


public:
	CPenNeb(HDC hDC, COLORREF crColor, BOOL bAlpha = FALSE);
	virtual ~CPenNeb();


public:
	void InitPenNeb();
	void Apply(const CPoint& pt) const;
	virtual void SetSize(int nSize) = 0;
	virtual void SetColor(COLORREF crColor);


protected:
	virtual void CreatePenNeb();
	void CreateAlphaPen();
	void Release();
	virtual void SetAlpha(BYTE * pPixel) = 0;
	void AlphaApply(const CPoint& pt) const;
	virtual void NormalApply(const CPoint& pt) const;

protected:
	void RotatePoint(double radians, const CPoint &c, CPoint& point) const;
	// 三个用于画旋转椭圆的函数
	void DrawRotateEllipse(HDC hDC, const CRect& rect, double radians) const;
	void EllipseToBezier(const CRect& rect, CPoint* cCtlPt) const;
	void RotateEllipse(double radians, const CPoint& c, CPoint* vCtlPt, unsigned Cnt) const;

};

#endif // !defined(AFX_PENNEB_H__7DED83E2_F715_11D6_95DB_000795CFF65D__INCLUDED_)
