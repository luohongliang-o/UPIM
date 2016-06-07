/**
*  ShowWnd.cpp
*  CPH 1.0 版
*  显示具有透明度的动态提示框
*  Copyright 2002-2003p. All Rights Reserved.
*  显示动态提示框
*====================================================
*  版本变更记录
*  CPH 1.0 2002/11/13  创建
*
*/
#include "stdafx.h"
#include "..\\include\\ShowWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CShowWnd

/*
*--------------------------------------------------------------------------------
*  CShowWnd()
*  类的构造函数
*  Parameter :
*     无
*  Return value:
*     无
*====================================================
*  版本变更记录
*  CPH 1.0 2002/11/13  完成数据的初始化
*
*/
CShowWnd::CShowWnd()
{
	m_nIndex = -1;
	//初始化数组
    m_hBitmap[1] = (HBITMAP) ::LoadImage (AfxGetInstanceHandle(),
										   "help\\help1.bmp",IMAGE_BITMAP,
										   0,0,
										   LR_LOADFROMFILE
										   );

    m_hBitmap[2] = (HBITMAP) ::LoadImage (AfxGetInstanceHandle(),
										   "help\\help2.bmp",IMAGE_BITMAP,
										   0,0,
										   LR_LOADFROMFILE
										   );

	m_hBitmap[3] = (HBITMAP) ::LoadImage (AfxGetInstanceHandle(),
										   "help\\help3.bmp",IMAGE_BITMAP,
										   0,0,
										   LR_LOADFROMFILE
										   );

	m_hBitmap[4] = (HBITMAP) ::LoadImage (AfxGetInstanceHandle(),
										   "help\\help4.bmp",IMAGE_BITMAP,
										   0,0,
										   LR_LOADFROMFILE
										   );

    m_hBitmap[5] = (HBITMAP) ::LoadImage (AfxGetInstanceHandle(),
										   "help\\help5.bmp",IMAGE_BITMAP,
										   0,0,
										   LR_LOADFROMFILE
										   );
    m_hBitmap[6] = (HBITMAP) ::LoadImage (AfxGetInstanceHandle(),
										   "help\\help6.bmp",IMAGE_BITMAP,
										   0,0,
										   LR_LOADFROMFILE
										   );
    m_hBitmap[7] = (HBITMAP) ::LoadImage (AfxGetInstanceHandle(),
										   "help\\help7.bmp",IMAGE_BITMAP,
										   0,0,
										   LR_LOADFROMFILE
										   );
    m_hBitmap[8] = (HBITMAP) ::LoadImage (AfxGetInstanceHandle(),
										   "help\\help8.bmp",IMAGE_BITMAP,
										   0,0,
										   LR_LOADFROMFILE
										   );
    m_hBitmap[9] = (HBITMAP) ::LoadImage (AfxGetInstanceHandle(),
										   "help\\help9.bmp",IMAGE_BITMAP,
										   0,0,
										   LR_LOADFROMFILE
										   );
    m_hBitmap[10] = (HBITMAP) ::LoadImage (AfxGetInstanceHandle(),
										   "help\\help10.bmp",IMAGE_BITMAP,
										   0,0,
										   LR_LOADFROMFILE
										   );
    m_hBitmap[11] = (HBITMAP) ::LoadImage (AfxGetInstanceHandle(),
										   "help\\help11.bmp",IMAGE_BITMAP,
										   0,0,
										   LR_LOADFROMFILE
										   );
    m_hBitmap[12] = (HBITMAP) ::LoadImage (AfxGetInstanceHandle(),
										   "help\\help12.bmp",IMAGE_BITMAP,
										   0,0,
										   LR_LOADFROMFILE
										   );
    m_hBitmap[13] = (HBITMAP) ::LoadImage (AfxGetInstanceHandle(),
										   "help\\help13.bmp",IMAGE_BITMAP,
										   0,0,
										   LR_LOADFROMFILE
										   );
    m_hBitmap[14] = (HBITMAP) ::LoadImage (AfxGetInstanceHandle(),
										   "help\\help14.bmp",IMAGE_BITMAP,
										   0,0,
										   LR_LOADFROMFILE
										   );
    m_hBitmap[15] = (HBITMAP) ::LoadImage (AfxGetInstanceHandle(),
										   "help\\help15.bmp",IMAGE_BITMAP,
										   0,0,
										   LR_LOADFROMFILE
										   );
    m_hBitmap[16] = (HBITMAP) ::LoadImage (AfxGetInstanceHandle(),
										   "help\\help16.bmp",IMAGE_BITMAP,
										   0,0,
										   LR_LOADFROMFILE
										   );
    m_hBitmap[17] = (HBITMAP) ::LoadImage (AfxGetInstanceHandle(),
										   "help\\help17.bmp",IMAGE_BITMAP,
										   0,0,
										   LR_LOADFROMFILE
										   );
    m_hBitmap[18] = (HBITMAP) ::LoadImage (AfxGetInstanceHandle(),
										   "help\\help18.bmp",IMAGE_BITMAP,
										   0,0,
										   LR_LOADFROMFILE
										   );
    m_hBitmap[19]= (HBITMAP) ::LoadImage (AfxGetInstanceHandle(),
    									  "help\\help19.bmp",IMAGE_BITMAP,
										  0,0,
										  LR_LOADFROMFILE
										  );
    m_hBitmap[20] = (HBITMAP) ::LoadImage (AfxGetInstanceHandle(),
										   "help\\help20.bmp",IMAGE_BITMAP,
										   0,0,
										   LR_LOADFROMFILE
										   );
    m_hBitmap[21] = (HBITMAP) ::LoadImage (AfxGetInstanceHandle(),
										   "help\\help21.bmp",IMAGE_BITMAP,
										   0,0,
										   LR_LOADFROMFILE
										   );

}

CShowWnd::~CShowWnd()
{
	
}


BEGIN_MESSAGE_MAP(CShowWnd, CWnd)
	//{{AFX_MSG_MAP(CShowWnd)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/*
*--------------------------------------------------------------------------------
*  SetIndex()
*  选择特殊画笔 
*  Parameter :  
*              [IN]    nIndex
*                 序号
*  Return value:
*                 无
*====================================================
*  版本变更记录
*  CPH 1.0 2002/11/13  设置要显示的动态提示框
*/

void CShowWnd::Show (BOOL isVisible, int nIndex)
{
	if(isVisible == TRUE)
	{
		m_nIndex = nIndex;
		OnPaint();
	    CShowWnd::ShowWindow(SW_SHOW);
	}
	else
	{
		m_nIndex = -1;
	    CShowWnd::ShowWindow(SW_HIDE);
	}
}

void CShowWnd::OnPaint() 
{
	if(m_nIndex == -1) return;
	CDC m_MemDC;
	m_MemDC.CreateCompatibleDC(NULL); 

	if(m_hBitmap[m_nIndex] == NULL) return;
	m_MemDC.SelectObject (m_hBitmap[m_nIndex]);
	::GetObject (m_hBitmap[m_nIndex], sizeof (m_bitmap), &m_bitmap);

	CPaintDC dc(this); // device context for painting
    BLENDFUNCTION blend = { AC_SRC_OVER, 0, 170, 0};
	AlphaBlend (dc.GetSafeHdc(),
				0, 0,
				m_bitmap.bmWidth, m_bitmap.bmHeight,
				m_MemDC.GetSafeHdc(),
				0, 0,
				m_bitmap.bmWidth, m_bitmap.bmHeight,
				blend
				);

	m_MemDC.DeleteDC(); }

