/////////////////////////////////////////////////////////////////////////////
/**
* 文件名  ：pattern.h
* 版本号  ：V1.0
* 项目名称：CPH
* 版权声明：Copyright 2002-2003 . All Rights Reserved.
* 模块简单描述：卡通等贴图类的头文件,实现将动物等图片贴到当前画布上，
*                并可以实现缩放、旋转、镜像等操作   
*======================================================
* 版本变更记录:
*      v1.0  2002-11-7   15:30  罗伟 
*
* 
*/

/////////////////////////////////////////////////////////////////////////////


#if !defined(AFX_PATTERN_H__F534F582_F320_11D6_95DA_000795D2C55B__INCLUDED_)
#define AFX_PATTERN_H__F534F582_F320_11D6_95DA_000795D2C55B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "..\\INCLUDE\\DIBitmap.h"   //加入对DIB 图像的支持

/////////////////////////////////////////////////////////////////////////////
/*
*类名:Cpattern
*说明：应用程序类
*变更记录：
*
*/

class Cpattern : public CWnd
{
// Construction
public:
	Cpattern();

// Attributes
public:
	//名称
	CString m_strDibName;

	//初始化DIB对象,成功返回TRUE，失败返回FALSE
	BOOL InitDib();
	//更新当前DIB对象,成功返回TRUE，失败返回FALSE
	BOOL UpDataDib();
	//显示当前DIB对象
	ShowDib();
	//隐藏当前DIB对象
	HideDib();
	//改变状态
	ChangeDibSize();
	//改变大小(缩放)
	ChangeDibZoom();
	//得到状态,返回值对应整数1-10,分别表示不同的位图状态
	INT GetStatus();
	//得到大小
	CRect GetDibSize();
	//得到关联页面指针
	HANDLE GetDibPage();
	//改变关联页面指针,返回改变后的页面句柄
	HANDLE ChangeDibPage();

protected:
     //图案所在文件名
	CString m_DibFileName;
	//文件中的顶点数组
	CPoint m_arrayFileTopPoint;
	//文件中的大小
	CSize m_DibSize;
	//顶点
	CPoint m_TopPoint;
	//大小
	CSize m_size;
	//图案状态,对应图案当前的状态（0-9:10种）
	INT m_DibStatus;
	//图案的页面数组
	HBITMAP m_arrayPages;
	//关联页面指针
	HBITMAP m_pPage;
	//显示/隐藏标识
	BOOL m_bShowHide;
	//当前的图案页面指针
	HBITMAP m_pPreDibPage;


public:
	enum {		
		BITMAP_CUSTOMIZE=0,         //位图以指定大小100*100显示在画布中心
		BITMAP_STRETCH,		       // 位图以拉伸方式填充对话框
		BITMAP_TILE,		       // 位图以自身尺寸填满对话框
		BITMAP_CENTER			   //位图居中显示在对话框中
	};

protected:
	CDIBitmap	m_picture;         //用DIB位图画在画布中心
	int			m_nType;	      //DIB图片在画布上的显示方式
	CBrush		m_brush;         //DIB画刷
	
protected:
	//{{AFX_MSG(Canvas)

	//}}AFX_MSG

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(Cpattern)
	public:
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~Cpattern();

	// Generated message map functions
protected:
	//{{AFX_MSG(Cpattern)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PATTERN_H__F534F582_F320_11D6_95DA_000795D2C55B__INCLUDED_)
