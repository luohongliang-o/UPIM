// PaintDraw.cpp : implementation file
//

#include "stdafx.h"
#include "..\Paint\PaintDlg.h"
#include "..\\INCLUDE\\PaintDraw.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPaintDraw

CPaintDraw::CPaintDraw()
{
}

CPaintDraw::~CPaintDraw()
{
}


BEGIN_MESSAGE_MAP(CPaintDraw, CWnd)
	//{{AFX_MSG_MAP(CPaintDraw)
	ON_WM_PAINT()
	ON_WM_MOUSEMOVE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CPaintDraw message handlers

void CPaintDraw::OnPaint() 
{
	   CPaintDC dc(this); // device context for painting
//////////////////////////////////////////////////////
   HBITMAP bitmap;
 // MoveWindow(95,43,615,443,TRUE); 
   CRect rect;
   GetClientRect(&rect);
///////读取位图文件DrawRect.BMP作为绘画区底图
   bitmap=(HBITMAP)LoadImage(AfxGetInstanceHandle(),"DrawRect.bmp",IMAGE_BITMAP,0,0,LR_LOADFROMFILE|LR_CREATEDIBSECTION);
   ASSERT(bitmap);
   HBITMAP OldBitmap;
   CDC MemDC;
   MemDC.CreateCompatibleDC(&dc);
   OldBitmap=(HBITMAP)MemDC.SelectObject(bitmap);
   ///显示它
   dc.BitBlt(0,0,520,390,&MemDC,0,0,SRCCOPY);
   MemDC.SelectObject(OldBitmap);

////////////////////////////////////////////////////////////////////////////////////


	// Do not call CWnd::OnPaint() for painting messages
}

void CPaintDraw::OnMouseMove(UINT nFlags, CPoint point) 
{   

	//UpdateWindow(); 

	CWnd::OnMouseMove(nFlags, point);

}
