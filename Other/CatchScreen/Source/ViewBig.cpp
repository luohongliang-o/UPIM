/**
*   ViewBig.cpp		
*   Version 1.0
*   CPH	绘画乐园
*   Copyright 2002 . All Rights Reserved.
*   实现放大相册内图片查看窗口
*======================================================
*   版本
*   Revision 1.0	2002/11/25    	  实现放大查看窗口类
*/

#include "stdafx.h"
#include "..\\PaintDlg.h"
#include "..\\INCLUDE\\ViewBig.h"
#include "..\\INCLUDE\\GALLERYDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//应用程序运行路径,如"c:\cph\"
extern  CString g_strAppPath;        
extern  IniPaintSet g_stuPaintSet;

/////////////////////////////////////////////////////////////////////////////
// CViewBig

CViewBig::CViewBig()
{
	m_pDib = new CDib();
	m_srcDib = new CDib();
	m_BigFrameDib = new CDib();

	m_strViewFileName = _T("");
	m_bBig = FALSE;

	//默认为不画边框
	m_bFocus = FALSE;

	//默认没有窗体具有焦点 
	INT m_nIndex = 0;     
	
	// 未画边框
	m_bDrawedFrm = FALSE;
	//第一次显示
	m_bFirstView = TRUE;  
	
}

CViewBig::~CViewBig()
{
	if(m_pDib)
	{
		delete m_pDib;
		m_pDib = NULL;
	}

	if(m_srcDib)
	{
		delete m_srcDib;
		m_srcDib = NULL;
	}

	if(m_BigFrameDib)
	{
		delete m_BigFrameDib;
		m_BigFrameDib = NULL;
	}
}


BEGIN_MESSAGE_MAP(CViewBig, CWnd)
	//{{AFX_MSG_MAP(CViewBig)
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CViewBig message handlers

int CViewBig::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO: Add your specialized creation code here
	
	return 0;
}


/*
*--------------------------------------------------------------------------------
*  成员函数名  	:	OnPaint
*  功能描述		:	窗体刷新处理函数
*  输入参数描述	:	
*  返回变量说明 :	
*  算法描述		:	
*  附加说明 	:   
*--------------------------------------------------------------------------------
*/
void CViewBig::OnPaint() 
{
	CPaintDC dc(this); 
	
	CRect rect;
	GetClientRect(&rect);

	CGALLERYDlg * pdlg = (CGALLERYDlg *)GetParent(); 


	m_srcDib->Load(m_strViewFileName); 
	if(m_strViewFileName != "")
	{  
		if(m_pDib)
			delete m_pDib;
		m_pDib = m_srcDib->Clone();
	}
	else
		return;

	//第一次显示全部9幅图像 
	if(m_bFirstView)
	{
		m_pDib->ChangeImageSize(160,120);
		m_pDib->Display(&dc, 0, 0); 
        m_bFirstView = FALSE;
		m_bBig = FALSE;
		return;
	}
	if(m_bBig && m_bFocus)
	{
		rect.SetRect(0, 0, 592, 458);

	    CString faceFile = g_strAppPath + "\\UseRes\\face_frm.bmp";
		m_BigFrameDib->Load(faceFile); 
		CPoint point1;
		point1.x = 36;
		point1.y = 36;
		m_BigFrameDib->MergeDib(m_srcDib->GetHandle(), point1);
		 
		//显示之前先隐藏其它8个窗体
	    ::PostMessage(GetParent()->GetSafeHwnd(), UWM_GETPREPICTURE, (long)this, 2);	
		m_BigFrameDib->Display(&dc, 0, 0); 
        this->MoveWindow(16, 58, 592 + 16, 458 +58,TRUE);
        m_bBig = TRUE;
	  //MoveWindow(&rect, TRUE);
 	}
	//重新显示图形
	else
	{   m_bBig = FALSE;
		m_pDib->ChangeImageSize(160,120);

			 TRACE("当前选择%d\n", m_nIndex);	
	  switch(m_nIndex)
		 {	

		  case 0:
			  {
				if(pdlg->GetPageNum() > 0)
					SetWindowPos(NULL, CRectRECT_IMG0.left ,CRectRECT_IMG0.top,160,120, SWP_SHOWWINDOW);
				break;
			  }
		  case 1:
			  {	
				 if(pdlg->GetPageNum() > 1)
					SetWindowPos(NULL, CRectRECT_IMG1.left ,CRectRECT_IMG1.top,160,120, SWP_SHOWWINDOW);
				break;
			  }
		  case 2:
			  {
				 if(pdlg->GetPageNum() > 2)
					SetWindowPos(NULL, CRectRECT_IMG2.left ,CRectRECT_IMG2.top,160,120, SWP_SHOWWINDOW);
				break;
			  }
		  case 3:
			  {
				 if(pdlg->GetPageNum() > 3)
					SetWindowPos(NULL, CRectRECT_IMG3.left ,CRectRECT_IMG3.top,160,120, SWP_SHOWWINDOW);
				break;
			  }
		  case 4:
			  {
				 if(pdlg->GetPageNum() > 4)
					SetWindowPos(NULL, CRectRECT_IMG4.left ,CRectRECT_IMG4.top,160,120, SWP_SHOWWINDOW);
				break;
			  }
		  case 5:
			  {
				if(pdlg->GetPageNum() > 5)
					SetWindowPos(NULL, CRectRECT_IMG5.left ,CRectRECT_IMG5.top,160,120, SWP_SHOWWINDOW);
				break;
			  }
		  case 6:
			  {
				if(pdlg->GetPageNum() > 6)
					SetWindowPos(NULL, CRectRECT_IMG6.left ,CRectRECT_IMG6.top,160,120, SWP_SHOWWINDOW);
				break;
			  }
		  case 7:
			  {
				if(pdlg->GetPageNum() > 7)
					SetWindowPos(NULL, CRectRECT_IMG7.left ,CRectRECT_IMG7.top,160,120, SWP_SHOWWINDOW);
				break;
			  }
		  case 8:
			  {
				if(pdlg->GetPageNum() > 8)
					SetWindowPos(NULL, CRectRECT_IMG8.left ,CRectRECT_IMG8.top,160,120, SWP_SHOWWINDOW);
				break;
			  }

		  default:
			  break;
		 }//end switch

         m_pDib->Display(&dc, 0, 0); 
	}
     
	//发送消息到父窗体中通知移动窗体到指定位置
    // ::PostMessage(GetParent()->GetSafeHwnd(), UWM_GETPREPICTURE, (long)this, 1);		
	// Do not call CWnd::OnPaint() for painting messages
}


/*
*--------------------------------------------------------------------------------
*  成员函数名  	:	SetViewBitmap
*  功能描述		:	设置当前窗体要显示的图像
*  输入参数描述	:	CString strfile	 -  图像文件名
*  返回变量说明 :	
*  算法描述		:	
*  附加说明 	:   
*--------------------------------------------------------------------------------
*/
void CViewBig::SetViewBitmap(CString strfile)
{
   m_strViewFileName = strfile;
}


/*
*--------------------------------------------------------------------------------
*  成员函数名  	:	OnLButtonDown
*  功能描述		:	鼠标左键单击消息，在此画当前窗口的边框
*  输入参数描述	:	UINT nFlags
*					CPoint point	-	点前坐标点
*  返回变量说明 :	void
*  算法描述		:	单击图片时在图片周围画上边框
*  附加说明 	:   使被单击的图片具有焦点
*--------------------------------------------------------------------------------
*/
void CViewBig::OnLButtonDown(UINT nFlags, CPoint point) 
{
	//对单击的图片周围画边框
    m_bFocus = TRUE;
	if(!m_bBig)
    DrawFrame(TRUE);

	//发送消息到父对话框通知取消其它8个窗体的焦点状态
    ::PostMessage(GetParent()->GetSafeHwnd(), UWM_GETPREPICTURE, (long)this, 0);

	CWnd::OnLButtonDown(nFlags, point);
}


/*
*--------------------------------------------------------------------------------
*  成员函数名  	:	SetBig
*  功能描述		:	设置当前窗体是否要最大化显示
*  输入参数描述	:	BOOL isBig	-	是否最大化显示图片
*  返回变量说明 :	void
*  算法描述		:	
*  附加说明 	: 
*--------------------------------------------------------------------------------
*/
void CViewBig::SetBig(BOOL isBig)
{
  m_bBig = isBig;

}


/*
*--------------------------------------------------------------------------------
*  成员函数名  	:	SetPrePos
*  功能描述		:	设置当前窗体的索引号
*  输入参数描述	:	INT index	-	当前窗体的索引号
*  返回变量说明 :	void
*  算法描述		:	
*  附加说明 	: 
*--------------------------------------------------------------------------------
*/
void CViewBig::SetPrePos(INT index)
{
	m_nIndex = index;
}


/*
*--------------------------------------------------------------------------------
*  成员函数名  	:	DrawFrame
*  功能描述		:	为当前窗体画边框
*  输入参数描述	:	BOOL isDraw - 画边框还是擦除边框的标志
*  返回变量说明 :	void
*  算法描述		:	
*  附加说明 	: 
*--------------------------------------------------------------------------------
*/
void CViewBig::DrawFrame(BOOL isDraw)
{
    CDC * pDC = GetDC();
	CPen pen;
	pen.CreatePen(PS_SOLID, 3, g_stuPaintSet.tranCol); 
	CPen * OldPen = pDC->SelectObject(&pen);		
     
	m_point[0].x = 1;
	m_point[0].y = 1;	
	m_point[1].x = 159;
	m_point[1].y = 1;	
	m_point[2].x = 159;
	m_point[2].y = 119;	
	m_point[3].x = 1;
	m_point[3].y = 119;	
	m_point[4].x = 1;
	m_point[4].y = 1;	
	pDC->SetBkMode(TRANSPARENT); 
	//画边框
	if(isDraw && !m_bBig)
	{
		pDC->Polyline(m_point, 5); 
        m_bDrawedFrm = TRUE;
	}
	//设置为异或模式重画一遍以取消已画的矩形
	else if(!isDraw && m_bDrawedFrm && !m_bBig)
	{
		pDC->SetROP2(R2_NOTXORPEN); 
		pDC->Polyline(m_point, 5); 
        m_bDrawedFrm = FALSE;
	}

}


/*
*--------------------------------------------------------------------------------
*  成员函数名  	:	GetPrePos
*  功能描述		:	得到当前窗体的索引号
*  输入参数描述	:	
*  返回变量说明 :	INT	- 当前窗体的索引号
*  算法描述		:	
*  附加说明 	: 
*--------------------------------------------------------------------------------
*/
INT CViewBig::GetPrePos()
{
    return m_nIndex;
}


/*
*--------------------------------------------------------------------------------
*  成员函数名  	:	SetPreFocus
*  功能描述		:	设置当前窗体
*  输入参数描述	:	BOOL bFocus - 是否具有焦点
*  返回变量说明 :	
*  算法描述		:	
*  附加说明 	:	具有焦点窗体四周画出边框
*--------------------------------------------------------------------------------
*/
void CViewBig::SetPreFocus(BOOL bFocus)
{
	m_bFocus = bFocus;
}


/*
*--------------------------------------------------------------------------------
*  成员函数名  	:	GetPreFocus
*  功能描述		:	得到当前焦点窗体的索引号
*  输入参数描述	:	
*  返回变量说明 :	BOOL -	窗口具有焦点返回 TRUE, 否则返回 FALSE
*  算法描述		:	
*  附加说明 	:	
*--------------------------------------------------------------------------------
*/
BOOL CViewBig::GetPreFocus()
{
	return m_bFocus;
}


/*
*--------------------------------------------------------------------------------
*  成员函数名  	:	DrawBig
*  功能描述		:	在当前窗口内以大图方式显示图片
*  输入参数描述	:	
*  返回变量说明 :	
*  算法描述		:	
*  附加说明 	:	
*--------------------------------------------------------------------------------
*/
void CViewBig::DrawBig()
{
	m_bBig = TRUE;
	GetParent()->Invalidate(); 
    Invalidate(TRUE);
}

/*
*--------------------------------------------------------------------------------
*  成员函数名  	:	DrawSmall
*  功能描述		:	在当前窗口内以小图方式显示图片
*  输入参数描述	:	
*  返回变量说明 :	
*  算法描述		:	
*  附加说明 	:	
*--------------------------------------------------------------------------------
*/
void CViewBig::DrawSmall()
{
	m_bBig = FALSE;
	GetParent()->Invalidate(); 
    Invalidate(TRUE);
}


/*
*--------------------------------------------------------------------------------
*  成员函数名  	:	HideWnd
*  功能描述		:	隐藏当前窗体
*  输入参数描述	:	
*  返回变量说明 :	
*  算法描述		:	
*  附加说明 	:	
*--------------------------------------------------------------------------------
*/
void CViewBig::HideWnd()
{
   this->ShowWindow(SW_HIDE); 
}


/*
*--------------------------------------------------------------------------------
*  成员函数名  	:	ShjowWnd
*  功能描述		:	显示当前窗体
*  输入参数描述	:	
*  返回变量说明 :	
*  算法描述		:	
*  附加说明 	:	
*--------------------------------------------------------------------------------
*/
void CViewBig::ShowWnd()
{
   this->ShowWindow(SW_SHOW); 
}
