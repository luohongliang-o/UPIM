/**
*  SpecialPen.h
*  CPH 1.0 版
*  完成特殊笔的功能（树叶，箭头，栅栏，水泡)
*  Copyright 2002-2003 . All Rights Reserved.
*  实现拾色功能
*====================================================
*  版本变更记录
*  CPH 1.0 2002/11/13 
*
*  CPH 1.0 2002/11/14  解决了内存泄露问题
*
*/

#if !defined(AFX_SPECIALPEN_H__DA17A4C2_F6E3_11D6_95DC_000795CD4EC5__INCLUDED_)
#define AFX_SPECIALPEN_H__DA17A4C2_F6E3_11D6_95DC_000795CD4EC5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "dib.h"
#include "ENUM.h"
/*
*类名    : class CSpecialPen  
*说明    : 封装了特殊笔的绘画功能
*变更记录:
*          2002/11/14  创建
*
*/
class CCPHDlg;
const INT MAX_SPECIALPEN = 100;
class CSpecialPen 
{
public:
	void SetPenType(INT penType);
	void SetMainWnd(CCPHDlg *pWnd);
	CString GetBtnFile(INT index);
	void InitBmpFile(INT type);
	void DrawDIB(CDC * pDC, CPoint point, INT ID, INT index,INT size, BOOL isChange = TRUE);
	HBITMAP ChangeBitmapSize(HBITMAP hBitmap, INT ID, INT index, CRect newRect = CRect(0,0,0,0));
	CSpecialPen(CCPHDlg * pMainWnd=NULL);
	virtual ~CSpecialPen();
	int  GetWidth();
	int  GetHeight();
	int  GetDibWidth();
	int  GetDibHeight();


	void SetTransparence (int iTransparence);
	BOOL chooseSpecialPen (int nStyle, int nIndex);
	void Draw(HDC hDC, CPoint point);

	CDib * m_psDib;
    //当前特殊笔类型(详枚举)
	SPECIAPEN     m_speciapen; 
private:
	int     m_nStyle;
	int     m_nIndex;
	BITMAP  m_bitmap;

	HBITMAP m_hBitmap[4][MAX_SPECIALPEN];  //每个子项最大支持20个特殊笔类型
    CString  m_bmpFile[4][MAX_SPECIALPEN];  //对庆按纽上的图象文件
    CString  m_btn1File[MAX_SPECIALPEN];  //在按纽控件上的缩略图(只对第一种类型的笔)

	CDC     m_memDC;
	int     m_iTransparence;

    CCPHDlg *m_pMainWnd;

};

#endif // !defined(AFX_SPECIALPEN_H__DA17A4C2_F6E3_11D6_95DC_000795CD4EC5__INCLUDED_)
