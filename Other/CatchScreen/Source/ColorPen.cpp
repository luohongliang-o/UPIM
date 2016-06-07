/////////////////////////////////////////////////////////////////////////////
/**
* 文件名  ：ColorPen.cpp 
* 版本号  ：V1.0
* 项目名称：CPH
* 版权声明：Copyright 2002-2003 . All Rights Reserved.
* 模块简单描述：用于调色板上的取色控件的模块文件
*======================================================
* 版本变更记录:
*      v1.0  2002-11-5   11:00  罗伟  
*
* 
*/

/////////////////////////////////////////////////////////////////////////////


#include "stdafx.h"
#include "..\Include\ColorPen.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CColorPen
/*
*--------------------------------------------------------------------------------
*  成员函数名   :CColorPen()    ~CColorPen()
*  功能描述     :构造函数       析构函数
*  输入参数描述	:
*  返回变量说明 :
*  算法描述		:
*  附加说明:
*--------------------------------------------------------------------------------
*/

CColorPen::CColorPen()
{
	m_bsuckepen=TRUE;
    m_bSelect=FALSE;
	m_ID=0;
}

CColorPen::~CColorPen()
{
}


BEGIN_MESSAGE_MAP(CColorPen, CWnd)
	//{{AFX_MSG_MAP(CColorPen)
	ON_WM_LBUTTONDOWN()
	ON_WM_SETCURSOR()
	ON_WM_MOUSEMOVE()
	ON_WM_PAINT()
	ON_MESSAGE(WM_MOUSEHOVER,OnMouseHover)
	ON_MESSAGE(WM_MOUSELEAVE,OnMouseLeave)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CColorPen message handlers
//////////////////////////////////////////////////////////////////////////////

/*
*--------------------------------------------------------------------------------
*  成员函数名   :OnLButtonDown
*  功能描述     :当鼠标在控件上按下时
*  输入参数描述	:
*   UINT nFlags：
*   CPoint point：
*  返回变量说明 :
*  算法描述		:
*  附加说明:
*--------------------------------------------------------------------------------
*/
void CColorPen::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
    //得到当前吸管色
     if (m_bsuckepen)
			{
				CRect rc;
				GetClientRect(&rc);
				rc.InflateRect(-2,-2);
				if (PtInRect(&rc,point))
				{
					CClientDC dc(this);
					m_pencolor = dc.GetPixel(point);
					m_bSelect=TRUE;
					::PostMessage(GetParent()->GetSafeHwnd(), UWM_SHOWPENCOLOR1, (long)this,0);
				}
				 
	 }//end if
	CWnd::OnLButtonDown(nFlags, point);
}


/*
*--------------------------------------------------------------------------------
*  成员函数名   :OnSetCursor
*  功能描述     :当鼠标在控件上时显示的光标样式
*  输入参数描述	:
*     CWnd* pWnd:
*     UINT nHitTest:
*     UINT message:
*  返回变量说明 :
*  算法描述		:
*  附加说明:
*--------------------------------------------------------------------------------
*/
BOOL CColorPen::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	// TODO: Add your message handler code here and/or call default
	/*
	if(m_bsuckepen)
	{
		SetCursor(AfxGetApp()->LoadCursor(IDC_CURPICKER));
    	return TRUE;
	}
	else 
	{
     //	SetCursor(LoadCursor(NULL,IDC_ARROW));   //设置鼠标光标为箭头
	SetCursor(AfxGetApp()->LoadCursor(IDC_ARROW));   //设置鼠标光标为箭头
	}
	*/
	return CWnd::OnSetCursor(pWnd, nHitTest, message);
}

LRESULT CColorPen::OnMouseHover(WPARAM wParam,  LPARAM)
{
	CRect rc,rc1,rc2;
	GetClientRect(rc);
	rc1.CopyRect(rc);
	rc2.CopyRect(rc);
	rc1.InflateRect(-1,-1);
	rc2.InflateRect(-2,-2);
	CClientDC dc(this);
	
	CPoint point(rc2.left+3,rc2.top+3);
	m_pencolor = dc.GetPixel(point);
	dc.FillSolidRect(rc,RGB(51,91,145));
	dc.FillSolidRect(rc1,RGB(255,255,255));
	dc.FillSolidRect(rc2,m_pencolor);
	return 0;
}

LRESULT CColorPen::OnMouseLeave(WPARAM wParam,  LPARAM)
{
	CRect rc,rc1,rc2;
	GetClientRect(rc);
	rc1.CopyRect(rc);
	rc2.CopyRect(rc);
	rc1.InflateRect(-1,-1);
	rc2.InflateRect(-2,-2);
	CClientDC dc(this);
	dc.FillSolidRect(rc,RGB(222,238,255));
	dc.FillSolidRect(rc1,RGB(51,91,145));
	dc.FillSolidRect(rc2,m_pencolor);
	return 0;
}

void CColorPen::OnMouseMove(UINT nFlags, CPoint point)
{
	TRACKMOUSEEVENT csTME;
	csTME.cbSize = sizeof(csTME);
	csTME.dwFlags = TME_HOVER|TME_LEAVE;
	csTME.hwndTrack = this->m_hWnd;
	csTME.dwHoverTime = 10;
	::_TrackMouseEvent(&csTME);
}

void CColorPen::OnPaint()
{
	CWnd::OnPaint();
}



void CColorPen::SetColor(COLORREF c1,COLORREF c2,COLORREF c3)
{
	m_OutSide = c1;
	m_MiddleSide = c2;
	m_InSide = c3;
}
