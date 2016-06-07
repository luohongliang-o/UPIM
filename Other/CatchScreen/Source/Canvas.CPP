/////////////////////////////////////////////////////////////////////////////
/**
* 文件名  ：Canvas.cpp 
* 版本号  ：V1.0
* 项目名称：CPH
* 版权声明：Copyright 2002-2003 . All Rights Reserved.
* 模块简单描述：绘画区画布类模块文件
*======================================================
* 版本变更记录:
*      v1.0  2002-11-4   8:30  罗伟
*      v1.1  2002-11-21 10:30   添加对文本编辑的支持      
* 
*/

/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "..\CatchScreenDlg.h"
#include "..\Include\Canvas.h"
#include "..\Include\DefineMessage.h"
#include "..\Include\includeres.h"
#include "..\Include\AbstractPen.h"
#include "..\Include\Pencil.h"
//#include "Rubber.h"
//#include "PaintTub.h"
#include "..\Include\Track.h"
//#include "AirPen.h"
#include "..\Include\PaintBrush.h"

//#include "SelectTool.h"
//#include "RectangleSelectTool.h"
//#include "FloatDibWnd.h"
#include <math.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//应用程序运行路径,如"c:\cph\"
extern  CString g_strAppPath;        
extern IniPaintSet g_stuPaintSet;

/////////////////////////////////////////////////////////////////////////////
/*
*--------------------------------------------------------------------------------
*  成员函数名   :Canvas()    
*  功能描述     :构造函数     
*  输入参数描述	:
*  返回变量说明 :
*  算法描述		:
*  附加说明     :
*--------------------------------------------------------------------------------
*/
Canvas::Canvas():m_vDib()
{
	m_pDib = new CDib();
	m_pDibBk = new CDib();
	m_tempDib = NULL;

	m_bSetBkBitmap = TRUE;
    m_nSelectColorMode = PP_PEN_COLOR;
    m_bLdown = FALSE;
    m_DrawPenStyle = 0;
    m_bchanged = FALSE;

	//为TRUE表示可以撤消
    m_bUnDo = TRUE; 
    //为TRUE表示可以重作
    m_bReDo = FALSE;
	//背景图默认为拉伸
	m_nType = BITMAP_STRETCH; 

	m_paintDC = NULL;

	// 缺省时使用铅笔工具
// 	m_pFloatDibWnd = NULL;
// 	m_pSelectTool = NULL;
	m_pPen = NULL;

	m_nFontSize = 0;
	// 字体缺省为普通宋体
	m_bBold = FALSE;
	m_bItalic = FALSE;
	m_bUnderline = FALSE;
	m_stSelection = "宋体";
	
	m_presize = 12;
	m_nPenSize =12;
	m_basepen = PEN;
	m_PenTrack = PT_RANDOM;
	m_PenStyle = PS_CIRCLE;
    m_bchanged = FALSE;
	m_bDrawingRubber = FALSE;
	m_pFont = new CFont();

}

/*
*--------------------------------------------------------------------------------
*  成员函数名   :~Canvas()     
*  功能描述     :析构函数  
*  输入参数描述	:
*  返回变量说明 :
*  算法描述		:
*  附加说明     :
*--------------------------------------------------------------------------------
*/
Canvas::~Canvas()
{
	delete m_pDib;
    m_pDib = NULL;

	delete m_pDibBk;
    m_pDibBk = NULL;

	if (m_tempDib)
		delete m_tempDib;
	if(memDC.GetSafeHdc() != NULL)
	{
		memDC.SelectObject(pOldBitmap);
		if(pBitmap != NULL)
			delete pBitmap;
	}
	
// 	if (m_pSelectTool)
// 		delete m_pSelectTool;

	if (m_pPen)
		delete m_pPen;

	if (m_pFont != NULL)
	{
		delete m_pFont;
	    m_pFont = NULL;
	}
	
// 	if (m_pFloatDibWnd)
// 		delete m_pFloatDibWnd;
}


BEGIN_MESSAGE_MAP(Canvas, CWnd)
	//{{AFX_MSG_MAP(Canvas)
	ON_WM_PAINT()
	ON_WM_MOUSEMOVE()
	ON_WM_SETCURSOR()
	ON_WM_CREATE()
	ON_WM_LBUTTONDOWN()
	ON_WM_HSCROLL()
	ON_WM_VSCROLL()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDBLCLK()
	//ON_MESSAGE(WM_CUTCAVASDIBRECT , OnCutCanvasDibRect)
	ON_MESSAGE(UWM_REDRAWCANVAS , OnReDraw)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
//Canvas message handlers
///////////////////////////////////////////////////////////////////////////
/*
*--------------------------------------------------------------------------------
*  成员函数名   :OnPaint() 
*  功能描述     :当窗体尺寸变化时重绘窗体（重载）
*  输入参数描述	:
*  返回变量说明 :
*  算法描述		:
*  附加说明     :本函数只执行一次，完成背景图的设置等初画布初始化的绘图工作
*--------------------------------------------------------------------------------
*/
void Canvas::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	// MoveWindow(96,43,616,433,TRUE); 
	/*以下是不使用DIB时导入图象到画布区，可以删除
		  CRect rect;
		  GetClientRect(&rect);
		  //读取位图文件DrawRect.BMP作为绘画区底图显示它
		  if(m_paint)
		  if(memDC.GetSafeHdc() != NULL)
		  {
		  dc.BitBlt(0, 0, rcClient.Width(), rcClient.Height(), &memDC, 0, 0, SRCCOPY);
		  m_paint=FALSE;
		  }
	*/
	////////////////////////////////////////////////////////////////////////////////		  
	
	CRect rect;
	GetClientRect(&rect);
	ClientToScreen(&rect); 
	CClientDC pDC(this);
	ASSERT_VALID(&pDC);

  //  dc.FillSolidRect(rect,COLOR_WHITE);

	if (m_pDib)
		m_pDib->DisplayTransparent(&pDC, 0, 0, g_stuPaintSet.tranCol);
    
	if (::IsWindow(m_EditText.m_hWnd))
	{
		CRect rc;
		m_EditText.GetWindowRect(&rc);
		ScreenToClient(&rc);
		
		
		rc.InflateRect(2,2);
		
		//CPen pen1(PS_SOLID,1,RGB(255,255,255));
		CPen pen2(PS_DOT,1,RGB(0,0,0));
		CBrush *pOldBrush = pDC.SelectObject(CBrush::FromHandle((HBRUSH)::GetStockObject(HOLLOW_BRUSH)));
		//CPen *pOldPen = pDC.SelectObject(&pen1);
		//pDC.Rectangle(&rc);
		CPen *pOldPen = pDC.SelectObject(&pen2);
		pDC.Rectangle(&rc);
		pDC.SelectObject(pOldBrush);
		pDC.SelectObject(pOldPen);
    } 

  
}





/*
*--------------------------------------------------------------------------------
*  成员函数名   :OnMouseMove
*  功能描述     :鼠标在画布窗体上移动时的操作（重载）
*  输入参数描述	:
*               UINT nFlags  ：按键标记
                CPoint point ：当前点坐标
*
*  返回变量说明 :
*  算法描述		:
*  附加说明     :
*--------------------------------------------------------------------------------
*/
void Canvas::OnMouseMove(UINT nFlags, CPoint point) 
{   

	/////////////////////////////////////////////////////////////////////////
	//针对当前选择的不同笔进行不同的操作
	//当前绘画类型为基本画笔
	if (m_bLdown)
	{
		if (m_toolbarstyle == PBASEPEN)
		{
			switch(m_basepen)
			{	
				//是吸管:不作任何操作
			case SUCKERPEN:
				break;

			//选择区域工具
// 			case SELRGN:
// 				ASSERT(m_pSelectTool);
// 				m_pSelectTool->ChangeSelect(point);
// 				break;
				
			// 各种笔			
			case PEN:
			{
				CClientDC cdc(this);
				ASSERT_VALID(&cdc);	  
				m_paintDC = m_pDib->BeginPaint(&cdc); 
	
				m_pPen->DrawPoint(point);
                 
				m_pDib->EndPaint(); 
				SetModify();
				m_strPoint= point;
				
				break;
			}  // end pencil
			
                //其它画笔
				//画特殊笔
			/*
			case SPECIA:
				{
					switch(m_speciapenType)
					{
						//画水泡
					case BLEB:
						{
                            if(sqrt(pow((point.x - pointOld.x),2) + pow((point.y - pointOld.y),2)) < m_SpeciaPen.GetWidth() / 2) 
									return;
									//	srand( (unsigned)time(NULL));
							int iTemp = rand() % g_stuPaintSet.Speci4Num + 1;
							m_SpeciaPen.chooseSpecialPen (3,iTemp); 

							m_SpeciaPen.Draw(GetDC()->GetSafeHdc(), point) ;
							SetModify();
							pointOld = point;
							break;
						}
						//画木栅栏
					case WOODBAR:
						{
							 CClientDC cdc(this);
							m_SpeciaPen.chooseSpecialPen(2,1); 
							if(sqrt(pow((point.x - pointOld.x),2) + pow((point.y - pointOld.y),2)) > m_SpeciaPen.GetDibWidth()/2) 
						//	m_SpeciaPen.Draw(GetDC()->GetSafeHdc(), point);
							{
								m_SpeciaPen.DrawDIB(&cdc, point, 2, 1, m_presize, TRUE);  //移动鼠标时画序号为1的图象
								SetModify();
							    pointOld = point;
							}
							break;

						}
						//画树叶
					case LEAF:
						{   CClientDC cdc(this);							 
							//	m_SpeciaPen.chooseSpecialPen(0, m_leafIndex); 
							if(sqrt(pow((point.x - pointOld.x),2) + pow((point.y - pointOld.y),2)) > m_SpeciaPen.GetDibWidth() / 2) 
							//	m_SpeciaPen.Draw(GetDC()->GetSafeHdc(), point);
	                       	{
								m_SpeciaPen.DrawDIB(&cdc, point, 0, m_leafIndex, m_presize, FALSE); 
								SetModify();
								pointOld = point;
							}
							break;
						}
						//画箭头:6-上 7-下  8-左  9-右
					case ARROW:
						{   
					//		INT arrow = 6;
					//		CClientDC cdc(this);
					//		m_SpeciaPen.chooseSpecialPen(1, 0); //  暂为0应根据拖动方向选取箭头图形
							if(sqrt(pow((point.x - pointOld.x),2) + pow((point.y - pointOld.y),2)) < m_SpeciaPen.GetWidth() / 2) 
								return;
							if(point.x >= pointOld.x)
							{
								if((point.x - pointOld.x) > fabs(point.y - pointOld.y))
									arrow = 9;  //右
								else 
								{
									if(point.y < pointOld.y)
										arrow = 6;   //上
									else
										arrow = 7;  //下
								}
							}

							else
							{
								if((pointOld.x - point.x) > fabs(point.y - pointOld.y))
									arrow = 8;  //左
								else 
								{
									if(point.y < pointOld.y)
										arrow = 6;   //上
									else
										arrow = 7;  //下
								}
							}

							m_SpeciaPen.DrawDIB(&cdc, point, 1,arrow, m_presize, TRUE);
							pointOld = point;
							break;
						}
						
					default:
						AfxMessageBox("请先选择画笔样式...");
						//m_speciapenType = BLEB;
					}//end switch
					
					break;
				}*/
				
				
				//画文字
			case TEXT:
				{
					if (m_bDrawingRubber)
					{
						DrawRubber(point);
						SetModify();
					}
					break;
				}
				
				
			default:
				ASSERT(0);
				
			}//end switch
		} //end if基本笔
	}//end if
	
	
	/////////////////////////////////////////////////////////////////////
	
	UpdateWindow(); 
	CWnd::OnMouseMove(nFlags, point);
}

/*--------------------------------------------------------------------------------
*  成员函数名   :OnSetCursor
*  功能描述     :为画布窗体设置鼠标光标样式(重载)
*  输入参数描述	:CWnd* pWnd   ：包含鼠标光标样式的窗口句柄  
*                UINT nHitTest：指定这个hit-test 面积码
*                UINT message ：鼠标消息值
*  返回变量说明 :
*  算法描述		:
*  附加说明     :
*--------------------------------------------------------------------------------
*/
BOOL Canvas::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
//		HCURSOR curs = LoadCursor(NULL,IDC_ARROW);   //开始设置鼠标光标为箭头
//		SetCursor(curs);
        SetCursor(AfxGetApp()->LoadCursor(IDC_ARROW)); 
	/*
	switch(m_basepen)
	{
	case SUCKERPEN :
		{
			m_hCursorPicker = AfxGetApp()->LoadCursor(IDC_CURPICKER);
			SetCursor(m_hCursorPicker);
			return TRUE;
			break;
		}
	// 各种笔
	case PEN:
		ASSERT(m_pPen);
		SetCursor(m_pPen->GetCursor());
		return TRUE;
		break;

	default:
		{
			HCURSOR curs = LoadCursor(NULL, IDC_ARROW);   //设置鼠标光标为箭头
	       SetCursor(curs);
   //       SetCursor(AfxGetApp()->LoadCursor(IDC_ARROW));
		
		}
		return TRUE;
	}  //end switch
	*/
	return CWnd::OnSetCursor(pWnd, nHitTest, message);
}


/*
*--------------------------------------------------------------------------------
*  成员函数名   :OnCreate(重载)
*  功能描述     :当创建画布窗体时执行一些诸如窗体初始化的操作
*  输入参数描述	:
*               LPCREATESTRUCT lpCreateStruct: 
*  返回变量说明 :
*  算法描述		:
*  附加说明     :
*--------------------------------------------------------------------------------
*/

int Canvas::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
//	HCURSOR curs = LoadCursor(NULL, IDC_ARROW);   //开始设置鼠标光标为箭头
   	HCURSOR  curs= AfxGetApp()->LoadCursor(IDC_ARROW);
	SetCursor(curs);
    
	    //导入背景图(如果需要)
       //m_pDibBk->Load("drawrect.bmp"); 
    	//初始化绘图DIB对象同背景DIB或画一真充白色矩形
       //m_pDib = m_pDibBk->Clone();
	
	  //导入背景图......(如果需要)
	/*
	  CClientDC dc(this);
      CDC * pDC = m_pDib->BeginPaint(&dc);
	  pDC->FillSolidRect(CRect(0,0,524,394), COLOR_WHITE));
	  m_pDib->EndPaint(); 
  */
	//创建一白色画布区
     CreatNewCanvas();
	 // 创建缺省的笔
	 CClientDC dc(this);
     CDC * pDC = m_pDib->BeginPaint(&dc);
	 m_pPen = new CPencil(GetDC()->GetSafeHdc(), GetPenSize(), m_pencolor);
	 SetPenColor(RGB(255,0,0));
	 m_pDib->EndPaint();
     
	return 0;
}

void Canvas::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	::PostMessage(GetDlgItem(IDD_CATCHSCREEN_DIALOG)->GetSafeHwnd(), WM_LBUTTONDBLCLK, (long)this,0);
	CWnd::OnLButtonDblClk(nFlags,point);
}
/*
*--------------------------------------------------------------------------------
*  成员函数名   :OnLButtonDown(重载)
*  功能描述     :当创建画布窗体时执行一些诸如窗体初始化的操作
*  输入参数描述	:
*   UINT nFlags ：
*   CPoint point：
*
*  返回变量说明 :
*  算法描述		:
*  附加说明     :
*--------------------------------------------------------------------------------
*/

void Canvas::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default

	CClientDC dc(this);
    m_strPoint = point;
	m_bLdown = TRUE;
	pointOld = point;
	// 融合画布上的浮动窗口
// 	if (m_pFloatDibWnd)
// 	{
// 		m_pFloatDibWnd->MergeDib(m_pDib);
// 		SetModify();
// 		delete m_pFloatDibWnd;
// 		m_pFloatDibWnd = NULL;
// 	}


	//针对当前选择的不同笔进行不同的操作
	//当前绘画类型为基本画笔
	if(m_toolbarstyle == PBASEPEN)
	{
		switch(m_basepen)
		{	
			//是吸管:得到当前吸管色
		case SUCKERPEN:
			{

				if(::IsWindow(m_EditText.m_hWnd))
	                m_EditText.DestroyWindow();
				COLORREF crColor = dc.GetPixel(point);
				if (m_nSelectColorMode == PP_FILL_COLOR)
				{
					m_crFillColor = crColor;
					ShowFillColor();
				}
				else if (m_nSelectColorMode == PP_PEN_COLOR)
				{
					m_pencolor = crColor;
					ShowPenColor();					
				}
				break;
			}

		// 选择工具
// 		case SELRGN:
// 			// 支持 Undo
// 			SaveBak();
// 
// 			ASSERT(m_pSelectTool);
// 			m_pSelectTool->BeginSelect(point);
// 			if(::IsWindow(m_EditText.m_hWnd))
// 		          m_EditText.DestroyWindow();
// 			SetModify();
// 			break;
			
		// 各种笔			
		case PEN:
		{
		   if(::IsWindow(m_EditText.m_hWnd))
	               m_EditText.DestroyWindow();
			// 支持 Undo
			SaveBak();
			SaveNBak();
			ASSERT(m_pPen);

			m_pPen->BeginDraw(point);
            SetModify();
			break;				
		}

		case TEXT: 
			{    
				SaveBak();
				SaveNBak();
				if (PointInDib(point))
				{
					MergeText();
		            StartDrawRubber(point);
					SetModify();
		            m_bDrawingRubber = TRUE;
				}
				else
					MergeText();
				    m_pTemp = point;
				break;
			}
	/*
		//特殊笔
		case SPECIA:
       {
            m_SpeciaPen.SetTransparence(g_stuPaintSet.Transparence);
		   if(::IsWindow(m_EditText.m_hWnd))
		          m_EditText.DestroyWindow();

		   SaveBak();
		   switch(m_speciapenType)
		   {	 
			   //树叶
			   case LEAF:
				   {
					  //m_SpeciaPen.chooseSpecialPen(0, m_leafIndex);
					  //DDB:m_SpeciaPen.Draw(dc.GetSafeHdc() , point); 

					    CClientDC cdc(this);
                        m_SpeciaPen.SetPenType(LEAF);
			            m_SpeciaPen.DrawDIB(GetDC(), point,0,m_leafIndex, m_presize, TRUE); 
                        SetModify();
					  break;
				   }
			  //箭头
			   case ARROW:
				   {
                     // m_SpeciaPen.chooseSpecialPen(1, 0);
					 // m_SpeciaPen.Draw(dc.GetSafeHdc() , point); 
					  //画虚框
					  //m_SpeciaPen.DrawDIB(&dc, point, 1,0, m_presize, TRUE); 
                      m_SpeciaPen.SetPenType(ARROW);
					  break;
				   }   
				  //木栅栏
			   case WOODBAR:
				   {
                     // m_SpeciaPen.chooseSpecialPen(2, 0);
					 // m_SpeciaPen.Draw(GetDC()->GetSafeHdc() , point); 
                       m_SpeciaPen.SetPenType(WOODBAR);
					   m_SpeciaPen.DrawDIB(GetDC(), point, 2, 0, m_presize, TRUE); 
					   SetModify();
					  break;
				   }			   

              //水泡
			   case BLEB:
				   {

						int iTemp = rand() % g_stuPaintSet.Speci4Num;
						m_SpeciaPen.chooseSpecialPen (3,iTemp);                      
						m_SpeciaPen.SetPenType(BLEB);
						m_SpeciaPen.Draw(GetDC()->GetSafeHdc(),point); 
                        SetModify();
					  break;
				   }
	
   
           } //end switch 
	   } //end case特殊笔
	   */

		default:
			break;	
			
		
		} //end switch

		SetCapture();
	} //end if(m_toolbarstyle == PBASEPEN)



//......................................

	CWnd::OnLButtonDown(nFlags, point);
}


/*
*--------------------------------------------------------------------------------
*  成员函数名   :ShowPenColor()
*  功能描述     :得到当前画笔及填充色，并在窗体中的控件中显示出来
*  输入参数描述	:
*
*  返回变量说明 :
*  算法描述		:
*  附加说明     :
*--------------------------------------------------------------------------------
*/
void Canvas::ShowPenColor()
{
	//发送消息通知主对话框更新当前选择画笔颜色
   ::PostMessage(GetParent()->GetSafeHwnd(), UWM_SHOWPENCOLOR2, (long)this, 0);
}
/*
*--------------------------------------------------------------------------------
*  成员函数名   :ShowFillColor()
*  功能描述     :得到当前画笔及填充色，并在窗体中的控件中显示出来
*  输入参数描述	:
*
*  返回变量说明 :
*  算法描述		:
*  附加说明     :
*--------------------------------------------------------------------------------
*/
void Canvas::ShowFillColor()
{

}


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
/*
*--------------------------------------------------------------------------------
*  成员函数名   :SetBitmap
*  功能描述     :为画布设置一张位图底图
*  输入参数描述	:
*   HBITMAP hBmp：要导入的位图句柄
*  返回变量说明 :
*  算法描述		:
*  附加说明     :
*--------------------------------------------------------------------------------
*/
void Canvas::SetBitmap(HBITMAP hBmp)
{
	if (SHOWSCROLL)
	{
		//重新设置偏移
		x_offset = 0;
		y_offset = 0;

		//再设置滚动条
		SCROLLINFO si;
		si.fMask = SIF_PAGE | SIF_RANGE;
		si.nMin = 0;
		si.nMax = 0;
		si.nPage = 0;
		SetScrollInfo(SB_HORZ, &si, TRUE);
		SetScrollInfo(SB_VERT, &si, TRUE);
		
		//重画背景图
		GetClientRect(&rcClient);
		//memDC.FillSolidRect(&rcClient,COLOR_WHITE);
			
		//得到位图句柄
		hBitmap = hBmp;

		if (hBitmap == 0)
		{
			memDC.TextOut(0,0,"文件没有发现.");
			Invalidate();
			return;
		}
		
		//得到位图尺寸
		CPoint point(x_offset,y_offset);
		BITMAP bmpInfo;
		CBitmap::FromHandle(hBitmap)->GetBitmap(&bmpInfo);
		image_size.cx = bmpInfo.bmWidth;
		image_size.cy = bmpInfo.bmHeight;
		
		//检查位图尺寸
		BOOL x_fit;
		BOOL y_fit;
		x_fit = (bmpInfo.bmWidth <= rcClient.Width());
		if (!x_fit)
			rcClient.bottom -= scrollbar_width;
		y_fit = (bmpInfo.bmHeight <= rcClient.Height());
		if (!y_fit)
		{
			rcClient.right -= scrollbar_width;
			x_fit = (bmpInfo.bmWidth <= rcClient.Width());
		}
		if (!x_fit)
		{
			// 显示水平滚动条
			ShowScrollBar(SB_HORZ);

			//CRect scroll_rect;
			//GetScrollBarCtrl(SB_HORZ)->GetWindowRect(&scroll_rect);
			//char sz[256];
			//sprintf(sz,"%d",scroll_rect.left - scroll_rect.right);
			//MessageBox(sz);

			// 更新滚动条
			SCROLLINFO si;
			si.fMask = SIF_PAGE | SIF_RANGE;
			si.nMin = 0;
			si.nMax = bmpInfo.bmWidth-1;
			si.nPage = rcClient.Width();
			SetScrollInfo(SB_HORZ, &si, TRUE);
		}
		if (!y_fit)
		{
			//显示竖向滚动条
			ShowScrollBar(SB_VERT);
			// 更新滚动条
			SCROLLINFO si;
			si.fMask = SIF_PAGE | SIF_RANGE;
			si.nMin = 0;
			si.nMax = bmpInfo.bmHeight-1;
			si.nPage = rcClient.Height();
			SetScrollInfo(SB_VERT, &si, TRUE);
		}
    }//end if 
	//重画位图
	DrawBitmap();
}


/*
*--------------------------------------------------------------------------------
*  成员函数名   :DrawBitmap()
*  功能描述     :用当前位图句柄重画画布上的位图
*  输入参数描述	:
*
*  返回变量说明 :
*  算法描述		:
*  附加说明     :
*--------------------------------------------------------------------------------
*/
void Canvas::DrawBitmap()
{

	CPoint point(-x_offset, -y_offset);
	memDC.DrawState(point, image_size, hBitmap, DST_BITMAP|DSS_NORMAL);
	//重画位图区域
	CRect ReRect(0, 0, image_size.cx, image_size.cy);
	
	InvalidateRect(ReRect, FALSE);
}


/*
*--------------------------------------------------------------------------------
*  成员函数名   :Create
*  功能描述     :当创建画布窗体时初始化窗口的类型等
*  输入参数描述	:
*
*  返回变量说明 :
*  算法描述		:
*  附加说明     :
*--------------------------------------------------------------------------------
*/
BOOL Canvas::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) 
{
	m_pDib->Create(rect); 
	m_wndRect = rect;
    m_clientRect.left = 0;
	m_clientRect.top = 0;
	m_clientRect.right = m_wndRect.Width();
	m_clientRect.bottom = m_wndRect.Height();

	scrollbar_width = GetSystemMetrics(SM_CXVSCROLL);
	
	// 创建窗口
	BOOL ret;
	static CString className = AfxRegisterWndClass(CS_HREDRAW | CS_VREDRAW,AfxGetApp()->LoadStandardCursor(IDC_ARROW));

	ret = CWnd::CreateEx(NULL,className,NULL,dwStyle, 
		rect.left,rect.top,rect.right - rect.left,rect.bottom - rect.top,
		pParentWnd->GetSafeHwnd(),0);

   //m_SpeciaPen.SetMainWnd((CCPHDlg *)GetParent()->GetOwner());

	//得到客户区屏幕
	GetClientRect(rcClient);
	CClientDC dc(this);
	if(memDC.GetSafeHdc() == NULL)
	{
		memDC.CreateCompatibleDC(&dc);

		pBitmap= NULL;
		if((pBitmap = new CBitmap()) == NULL) 
			return FALSE;
		pBitmap->CreateCompatibleBitmap(&dc, rcClient.Width(), rcClient.Height());
		pOldBitmap = memDC.SelectObject(pBitmap);
		memDC.SelectStockObject(ANSI_VAR_FONT);
		memDC.SetBkMode(TRANSPARENT);
	}

	x_offset = 0;
	y_offset = 0;

	//memDC.FillSolidRect(&rcClient,COLOR_WHITE);

	//MoveCanvas();

   
    //m_SpeciaPen.InitBmpFile(g_stuPaintSet.curType);  //必须初始化特殊画笔的类型，以便正确加载文件


    

	/////////////////////////////////////////////////////////////////////////
	return ret;



//	return CWnd::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
}


/*
*--------------------------------------------------------------------------------
*  成员函数名   :LoadFile
*  功能描述     :
*  输入参数描述	:
*CString filename:从外部文件中导入位图
*  返回变量说明 :
*  算法描述		:
*  附加说明     :
*--------------------------------------------------------------------------------
*/
void Canvas::LoadFile(CString filename)
{

   SaveBak();
   
   CClientDC cdc(this);
   CDC * pDC = m_pDib->BeginPaint(&cdc); 
   m_pDib->Load(filename); 
   CRect rect;
   CRect wRect;
   if (!m_pDib->IsEmpty())
   {  
	   //使画布尺寸适合当前加载的位图
        rect.SetRect(0,0,m_pDib->GetWidth(),m_pDib->GetHeight());
		wRect.SetRect(0,0,rect.Width()+4,rect.Height()+4);
        SetNewWndRect(wRect);
		m_pDib->Display(&cdc,0, 0);
   }
   else
   {
       rect.SetRect(0,0,0,0);
       SetNewWndRect(rect);
   }

    m_pDib->EndPaint(); 
    Invalidate(FALSE); 
	
}


/*
*--------------------------------------------------------------------------------
*  成员函数名   :OnHScroll
*  功能描述     :当位图宽度大于画布窗口尺寸时显示水平滚动条（该功能暂保留，本程序不用）
*  输入参数描述	:
*  UINT nSBCode：
*  UINT nPos   ：
*  CScrollBar* pScrollBar：
*  返回变量说明 :
*  算法描述		:
*  附加说明     :
*--------------------------------------------------------------------------------
*/
void Canvas::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	if(SHOWSCROLL)
	{
		int nSmall = 1;
		int nLarge = 20;

		if(pScrollBar == GetScrollBarCtrl(SB_HORZ))
		{
			if(nSBCode == SB_LEFT || nSBCode == SB_LINELEFT)
				x_offset -= nSmall;
			if(nSBCode == SB_PAGELEFT)
				x_offset -= nLarge;
			if(nSBCode == SB_RIGHT || nSBCode == SB_LINERIGHT)
				x_offset += nSmall;
			if(nSBCode == SB_PAGERIGHT)
				x_offset += nLarge;
			if(nSBCode == SB_THUMBPOSITION)
				x_offset = (int)nPos;
			if(nSBCode == SB_THUMBTRACK)
				x_offset = (int)nPos;
			if (x_offset<0)
				x_offset = 0;
			if (x_offset>image_size.cx - rcClient.Width())
				x_offset = image_size.cx - rcClient.Width();
			SetScrollPos(SB_HORZ, x_offset, TRUE);
			DrawBitmap();
		}	
	} //end if
	CWnd::OnHScroll(nSBCode, nPos, pScrollBar);
}


/*
*--------------------------------------------------------------------------------
*  成员函数名   :OnVScroll
*  功能描述     :当位图高度大于画布窗口尺寸时显示垂直滚动条（该功能暂保留，本程序不用）
*  输入参数描述	:
*
*  返回变量说明 :
*  算法描述		:
*  附加说明     :
*--------------------------------------------------------------------------------
*/
void Canvas::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	if(SHOWSCROLL)
	{
		int nSmall = 1;
		int nLarge = 20;

		if(pScrollBar == GetScrollBarCtrl(SB_VERT))
		{
			if(nSBCode == SB_TOP || nSBCode == SB_LINEUP)
				y_offset -= nSmall;
			if(nSBCode == SB_PAGEUP)
				y_offset -= nLarge;
			if(nSBCode == SB_BOTTOM || nSBCode == SB_LINEDOWN)
				y_offset += nSmall;
			if(nSBCode == SB_PAGEDOWN)
				y_offset += nLarge;
			if(nSBCode == SB_THUMBPOSITION)
				y_offset = (int)nPos;
			if(nSBCode == SB_THUMBTRACK)
				y_offset = (int)nPos;
			if (y_offset<0)
				y_offset = 0;
			if (y_offset>image_size.cy - rcClient.Height())
				y_offset = image_size.cy - rcClient.Height();
			SetScrollPos(SB_VERT, y_offset, TRUE);
			DrawBitmap();
		}	
	}//end  if
	CWnd::OnVScroll(nSBCode, nPos, pScrollBar);
}


/*
*--------------------------------------------------------------------------------
*  成员函数名   :Serialize
*  功能描述     :用于
*  输入参数描述	:
*  
*  返回变量说明 :
*  算法描述		:
*  附加说明     :
*--------------------------------------------------------------------------------
*/

void Canvas::Serialize(CArchive& ar) 
{
	//读写DIB文件中数来初始化DIB对象
	m_pDib->Serialize(ar);
	if (ar.IsStoring())
	{	// storing code
	}
	else
	{	// loading code
	}
}


/*
*--------------------------------------------------------------------------------
*  成员函数名   :OnReDraw
*  功能描述     :用于重画画布
*  输入参数描述	:
*  
*  返回变量说明 :
*  算法描述		:
*  附加说明     :
*--------------------------------------------------------------------------------
*/

LRESULT Canvas::OnReDraw(WPARAM ,  LPARAM)
{
   //AfxMessageBox("在画布类中得到来自卡通类发送的消息");

   return 0;
}



/*
*--------------------------------------------------------------------------------
*  成员函数名   :MergeCartoonDib()
*  功能描述     :用于将m_cartoon上的图片对象融入画布中
*  输入参数描述	:
*  
*  返回变量说明 :
*  算法描述		:
*  附加说明     :
*--------------------------------------------------------------------------------
*/

//DEL void Canvas::MergeCartoonDib()
//DEL { 	
//DEL     
//DEL 	if (m_cartoon && m_cartoon->GetSafeHwnd())
//DEL 	{
//DEL 		SaveBak();
//DEL 	    CRect rc;
//DEL 		m_cartoon->GetWindowRect(&rc);
//DEL 		CPoint point = rc.TopLeft();
//DEL 		ScreenToClient(&point);
//DEL 
//DEL 		//处理m_cartoon对象，防止它的粉红色底色融入底图中应行隐去按纽控件的窗体
//DEL      	m_cartoon->HideAllButton();	
//DEL 
//DEL 		SendMessage(WM_PAINT, 0, 0);
//DEL         ::UpdateWindow(m_cartoon->GetSafeHwnd());
//DEL 
//DEL 		CRect rect;
//DEL 		m_cartoon->GetClientRect(&rect);
//DEL 		CPoint cpoint = rect.TopLeft();
//DEL         CDib * cdib =new CDib;
//DEL    
//DEL         //Invalidate(TRUE); 
//DEL 		cdib->Create(m_cartoon->GetSafeHwnd(), 
//DEL 			        CRect(0, 0, rect.Width(), rect.Height())); 
//DEL         HDIB   dib= CopyHandle(cdib->GetHandle()); 
//DEL         m_pDib->MergeDib(dib, point);
//DEL 
//DEL 		delete cdib;
//DEL 
//DEL 	   //设置修改标记
//DEL 	    SetModify(TRUE);
//DEL 	    DeleteCartoonWnd();
//DEL 	}  //end if
//DEL 
//DEL }


/*
*--------------------------------------------------------------------------------
*  成员函数名   :CreateCartoonWnd
*  功能描述     :创建卡通图片窗体m_cartoon
*  输入参数描述	:
*  
*  返回变量说明 :
*  算法描述		:
*  附加说明     :
*--------------------------------------------------------------------------------
*/

//DEL void Canvas::CreateCartoonWnd(HDIB hDib, CPoint ptTopLeft, CString strFileName)
//DEL {
//DEL //	MergeCartoonDib();
//DEL 	//得到DIB的宽度和高度
//DEL     LPBYTE lpDIB = (LPBYTE)GlobalLock(hDib); 
//DEL 	if (! lpDIB)
//DEL 	{
//DEL 		GlobalUnlock(hDib);
//DEL 		return;
//DEL 	}
//DEL 	int nWidth  = (int)DIBWidth(lpDIB);
//DEL 	int nHeight = (int)DIBHeight(lpDIB);
//DEL 	GlobalUnlock(hDib);
//DEL 
//DEL 	// 创建卡通窗体
//DEL 	CRect rc(ptTopLeft.x, ptTopLeft.y, ptTopLeft.x+nWidth, ptTopLeft.y+nHeight);
//DEL 	SaveBak();
//DEL     //m_cartoon	= new CCartoon(hDib, rc, this);
//DEL 
//DEL }


/*
*--------------------------------------------------------------------------------
*  成员函数名   :DeleteCartoonWnd()
*  功能描述     :删除卡通图片窗体m_cartoon
*  输入参数描述	:
*  
*  返回变量说明 :
*  算法描述		:
*  附加说明     :
*--------------------------------------------------------------------------------
*/
//DEL void Canvas::DeleteCartoonWnd()
//DEL {
//DEL 	if (m_cartoon)
//DEL 	{ 
//DEL 		SaveBak();
//DEL 		delete m_cartoon;
//DEL 		m_cartoon = NULL;
//DEL 	}
//DEL  
//DEL }


/*
*--------------------------------------------------------------------------------
*  成员函数名   : CreeateNewCartoon
*  功能描述     : 新建一卡通窗体
*  输入参数描述	:
*		 CString strFile：窗体所有显示的图片文件名
*  返回变量说明 :创建成功返回TRUE
*  算法描述		: 新建一个浮动窗口，将指定图片文件导入到窗口中
*  附加说明     : 如果当前存在有浮动窗口，则替换此窗口内容
*--------------------------------------------------------------------------------
*/
BOOL Canvas::CreateNewCartoon(CString strFile)
{
	// 支持Undo操作
	SaveBak();

	if(::IsWindow(m_EditText.m_hWnd))
	      m_EditText.DestroyWindow();
/*
	// 存在浮动窗口
	if (m_pFloatDibWnd && m_pFloatDibWnd->IsCartoonWnd())
	{
		// 此浮动窗口为卡通窗口
//		m_pFloatDibWnd->SetCartoonDib(strFile);
		if (m_pFloatDibWnd)
		{
			m_pFloatDibWnd->MergeDib(m_pDib);
			delete m_pFloatDibWnd;
		}	
		m_pFloatDibWnd = new CFloatDibWnd(GetSafeHwnd(), strFile, m_orgPenSize);

	}
	else
	{
		if (m_pFloatDibWnd)
		{
			m_pFloatDibWnd->MergeDib(m_pDib);
			delete m_pFloatDibWnd;
		}	
		m_pFloatDibWnd = new CFloatDibWnd(GetSafeHwnd(), strFile,m_orgPenSize);
	}

	return m_pFloatDibWnd ? TRUE : FALSE;
	*/
	return FALSE;
}


/*
*--------------------------------------------------------------------------------
*  成员函数名   :SaveBak()
*  功能描述     :将当前窗体DIB备份起来,每次对m_pDib对象操作后都应调用本函数作一备份以便撤消操作
*  输入参数描述	:
*  
*  返回变量说明 :
*  算法描述		:
*  附加说明     :
*--------------------------------------------------------------------------------
*/
void Canvas::SaveBak()
{
	if (m_tempDib)
		delete m_tempDib;
	m_tempDib = m_pDib->Clone(); 
	m_bUnDo = TRUE;
	m_bReDo = FALSE;
}

void Canvas::SaveNBak()
{
	if (m_tempDib)
		delete m_tempDib;
	m_tempDib = m_pDib->Clone(); 
	CDib * tempDib = m_pDib->Clone(); 
	m_vDib.push_back(tempDib);
}

/*
*--------------------------------------------------------------------------------
*  成员函数名   :UnDo()
*  功能描述     :将当前窗体DIB备份恢复,用于撤消
*  输入参数描述	:
*  
*  返回变量说明 :
*  算法描述		:
*  附加说明     : 撤销操作分两种情况，一种是普通作图，另一种情况是有浮动窗口时
*--------------------------------------------------------------------------------
*/
void Canvas::UnDo()
{
	if(m_bUnDo)
	{	
		CDib * dib ;
		//dib = new CDib;
		dib = m_pDib->Clone();
		delete m_pDib;
		m_pDib = m_tempDib->Clone() ;
        delete m_tempDib;   
		m_tempDib = dib->Clone();
		delete dib;
        dib = NULL;
		Invalidate(TRUE); 
		m_bUnDo = FALSE;
		m_bReDo = TRUE;
	}
 }

BOOL Canvas::UnNDo()
{
	if(m_vDib.empty())
	{
		return FALSE;
	}
	else
	{
		CDib *dib;
		dib = m_vDib.back()->Clone();
		delete m_pDib;
		m_pDib = dib->Clone() ;
		m_vDib.pop_back();
		delete dib;
		Invalidate(TRUE); 
		return TRUE;

	}		
}


/*
*--------------------------------------------------------------------------------
*  成员函数名   :ReDo()
*  功能描述     :将当前窗体DIB备份恢复用于重作
*  输入参数描述	:
*  
*  返回变量说明 :
*  算法描述		:
*  附加说明     :
*--------------------------------------------------------------------------------
*/
void Canvas::ReDo()
{
	if(m_bReDo)
	{
		CDib * dib ;
		//dib = new CDib;
		dib = m_pDib->Clone();
		delete m_pDib;
		m_pDib = m_tempDib->Clone() ;
		delete m_tempDib;
		m_tempDib = dib->Clone();
		delete dib;
		dib = NULL;
		Invalidate(TRUE); 
		m_bReDo = FALSE;
		m_bUnDo = TRUE;
	}
 }


/*
*--------------------------------------------------------------------------------
*  成员函数名   :CreatNewCanvas
*  功能描述     :新建画布
*  输入参数描述	:
*   CString strfile:画布背景图(初始用图)
*   CRect rect     :画布矩形区大小
*  返回变量说明 :
*  算法描述		:
*  附加说明     :
*--------------------------------------------------------------------------------
*/
void Canvas::CreatNewCanvas(CString strfile, INT width/*=0*/,INT height/*=0 */)
{
    CRect clRect;
// 	clRect.left = 0;
// 	clRect.top  = 0;
// 	//默认画布矩形区CRect(0,0,524,394)
//     //默认画布窗体CRect(94, 39, 618, 433)
// 	  if(width == 0)  
// 	  {
// 		  clRect.right = m_wndRect.Width() - 4;
// 	  }
// 	  else
// 	  {
// 	     m_wndRect.right = m_wndRect.left + width;
// 	     clRect.right = m_wndRect.Width() - 4;
// 	  }
// 
// 
// 	  if(height == 0)
// 	  {
// 		  clRect.bottom = m_wndRect.Height() - 4;
// 	  }
//       else
// 	  {
// 	      m_wndRect.bottom = m_wndRect.top + height;
// 		  clRect.bottom = m_wndRect.Height() - 4;
// 	  }
	
	  GetClientRect(m_clientRect);
	  clRect = m_clientRect ;
	  strfile = _T("");
	  CClientDC dc(this);

	  if(strfile != "")  //暂不支持
	  {
		  InitImage(strfile);
	  }

	  else
	  {
		  CDC * pDC = m_pDib->BeginPaint(&dc);

		  CRect cRect;
		  this->GetClientRect(cRect);
          //pDC->FillSolidRect(cRect, COLOR_WHITE);
		  CString s;
		  s.Format("left=%d,top=%d,width=%d,height=%d",clRect.left,clRect.top,clRect.Width(),clRect.Height());
		  TRACE(s);
       //   pDC->FillSolidRect(clRect, COLOR_WHITE);
        //  pDC->FillSolidRect(m_clientRect, COLOR_WHITE);
	      m_pDib->EndPaint();
		  
		  //初始化时使备份DIB同源DIB一致
		  if(m_tempDib)
				delete m_tempDib;
          m_tempDib = m_pDib->Clone(); 

		  //新建画布时删除浮动窗口
// 		  if (m_pFloatDibWnd)
// 		  {
// 			  delete m_pFloatDibWnd;
// 			  m_pFloatDibWnd = NULL;
// 		  }
		  //MoveCanvas();
          Invalidate(TRUE);
          SetModify(FALSE);


	  }
    
}


/*
*--------------------------------------------------------------------------------
*  成员函数名   :SetModify
*  功能描述     :设置修改标记
*  输入参数描述	:
*    BOOL isChanged：画布上是否有新的操作
*  返回变量说明 :
*  算法描述		:
*  附加说明     :
*--------------------------------------------------------------------------------
*/
void Canvas::SetModify(BOOL isChanged)
{
      m_bchanged = isChanged;
	  CBitmap *hBitmap = m_pDib->GetBitmap();
	  if(isChanged && g_stuPaintSet.bAutoSave)
	  {
		  ;//::SendMessage(AfxGetApp()->GetMainWnd()->GetSafeHwnd(),WM_SENDIMAGETOEXE,WPARAM(hBitmap),LPARAM(m_pDib));
	  }
}


/*
*--------------------------------------------------------------------------------
*  成员函数名   :OnLButtonUp
*  功能描述     :设置修改标记
*  输入参数描述	:
*    UINT nFlags:其它按键情况
*    CPoint point:鼠标弹起时的点的坐标
*  返回变量说明 :
*  算法描述		:
*  附加说明     :
*--------------------------------------------------------------------------------
*/
void Canvas::OnLButtonUp(UINT nFlags, CPoint point) 
{
    
	m_endPoint = point;

	if (m_bLdown && m_toolbarstyle == PBASEPEN)
	{
		switch (m_basepen)
		{
		//区域选取工具
// 		case SELRGN:
// 			ASSERT(m_pSelectTool);
// 			m_pFloatDibWnd = m_pSelectTool->EndSelect(point);
// 
// 			break;

		// 各种笔
		case PEN:
			ASSERT(m_pPen);
			m_pPen->EndDraw(point);
            SetModify();
			Invalidate(FALSE); 
			//保存当前屏幕到DIB对象中
		   //CopyClientRectToDib();
			break;
		//文字
		case TEXT:
			if (m_bDrawingRubber && (m_pTemp != point))
				{
					StopDrawRubber();
					SetModify();
					m_bDrawingRubber = FALSE;
				}
			break;
		/*
		//特殊笔 
		case SPECIA:
				{
					switch(m_speciapenType)
					{
					//画木栅栏
					case WOODBAR:
						{
							if(point.x > pointOld.x)
						//	m_SpeciaPen.chooseSpecialPen(2,0); 
							point.x = point.x + m_SpeciaPen.GetWidth()/2 ; 
						//	m_SpeciaPen.Draw(GetDC()->GetSafeHdc(), point) ;
			               m_SpeciaPen.DrawDIB(GetDC(), point, 2, 0, m_presize, TRUE); 
						   SetModify();
							//specialPen.Delete(); 
                           break;
						}
                       	//画箭头:0-上 1-下  2-左  3-右
					case ARROW:
						{   
							INT arrow = 0;
							CClientDC cdc(this);
							m_SpeciaPen.chooseSpecialPen(1, 0); //  暂为0应根据拖动方向选取箭头图形
						//	if(sqrt(pow((point.x - pointOld.x),2) + pow((point.y - pointOld.y),2)) < m_SpeciaPen.GetWidth() / 2) 
						//		return;
							if(point.x >= pointOld.x)
							{
								if((point.x - pointOld.x) > fabs(point.y - pointOld.y))
									arrow = 3;  //右
								else 
								{
									if(point.y < pointOld.y)
										arrow = 0;   //上
									else
										arrow = 1;  //下
								}
							}

							else
							{
								if((pointOld.x - point.x) > fabs(point.y - pointOld.y))
									arrow = 2;  //左
								else 
								{
									if(point.y < pointOld.y)
										arrow = 0;   //上
									else
										arrow = 1;  //下
								}
							}

							m_SpeciaPen.DrawDIB(&cdc, point, 1,arrow, m_presize, TRUE);
							SetModify();
							pointOld = point;
							break;
						}
					
					default:
						break;
					}//end switch
             } //end case特殊笔	*/
		   default:
			  break;
		}
        
		ReleaseCapture();
	}

    CopyClientRectToDib();
	//SetTimer (1, 0, NULL);
    m_bLdown = FALSE;
	CWnd::OnLButtonUp(nFlags, point);

}


/*
*--------------------------------------------------------------------------------
*  成员函数名   :CopyClientRectToDib()
*  功能描述     :拷贝当前客户区屏幕区图象到m_pDib对象中
*  输入参数描述	:
*		
*  返回变量说明 :
*  算法描述		:
*  附加说明     :在对屏幕进行了GDI操作完后，在刷新前必须调用本函数
*--------------------------------------------------------------------------------
*/
BOOL Canvas::CopyClientRectToDib()
{
	CRect rect;
	GetClientRect(&rect);
    CDib * cdib = new CDib;
	cdib->Create(this->GetSafeHwnd(), CRect(0, 0, rect.Width(), rect.Height())); 

	if (m_pDib)
		delete m_pDib;
	m_pDib = cdib->Clone(); 

	delete cdib;
	cdib = NULL;
	return TRUE;
}


/*
*--------------------------------------------------------------------------------
*  成员函数名   :GetPenColor()
*  功能描述     :得以当前画笔颜色
*  输入参数描述	:
*		
*  返回变量说明 :
*  算法描述		:
*  附加说明     :
*--------------------------------------------------------------------------------
*/
COLORREF Canvas::GetPenColor()
{
   return m_pencolor;
}


/*
*--------------------------------------------------------------------------------
*  成员函数名   :GetFillColor()
*  功能描述     :得到当前画刷填充色(暂没有使用)
*  输入参数描述	:
*		
*  返回变量说明 :
*  算法描述		:
*  附加说明     :
*--------------------------------------------------------------------------------
*/
COLORREF Canvas::GetFillColor()
{
	return m_crFillColor;
}


/*
*--------------------------------------------------------------------------------
*  成员函数名   :GetToolBarStyle()
*  功能描述     :得到当前选中的工具栏(详枚举)
*  输入参数描述	:
*		
*  返回变量说明 :
*  算法描述		:
*  附加说明     :
*--------------------------------------------------------------------------------
*/
PRETOOLBAR Canvas::GetToolBarStyle()
{
	return m_toolbarstyle;
}


/*
*--------------------------------------------------------------------------------
*  成员函数名   :GetBasePenStyle()
*  功能描述     :得到当前基本画笔工具(详枚举)
*  输入参数描述	:
*		
*  返回变量说明 :
*  算法描述		:
*  附加说明     :
*--------------------------------------------------------------------------------
*/
BASEPEN Canvas::GetBasePenStyle()
{
	return m_basepen;
}


/*
*--------------------------------------------------------------------------------
*  成员函数名   :GetFontStyle()
*  功能描述     :得到当前字体类型(详枚举)
*  输入参数描述	:
*		
*  返回变量说明 :
*  算法描述		:
*  附加说明     :
*--------------------------------------------------------------------------------
*/
FONTSTYLE Canvas::GetFontStyle()
{
	return m_fontstyle;
}


/*
*--------------------------------------------------------------------------------
*  成员函数名   :GetPictureStyle()
*  功能描述     :得到当前图片类别(详枚举)
*  输入参数描述	:
*		
*  返回变量说明 :
*  算法描述		:
*  附加说明     :
*--------------------------------------------------------------------------------
*/
PICTURES Canvas::GetPictureStyle()
{
	return m_picture;
}


/*
*--------------------------------------------------------------------------------
*  成员函数名   :GetSpeciaPenStyle()
*  功能描述     :得到当前特殊笔类型(详枚举)
*  输入参数描述	:
*		
*  返回变量说明 :
*  算法描述		:
*  附加说明     :
*--------------------------------------------------------------------------------
*/
/*
SPECIAPEN Canvas::GetSpeciaPenStyle()
{
	return m_speciapenType;
}
*/

/*
*--------------------------------------------------------------------------------
*  成员函数名   :GetPenSize()
*  功能描述     :得到当前画笔尺寸(1-110)
*  输入参数描述	:
*		
*  返回变量说明 :
*  算法描述		:
*  附加说明     :
*--------------------------------------------------------------------------------
*/
INT Canvas::GetPenSize()
{
	return  m_presize;
}

//////////////////////////////////////////////////////////
/*
*--------------------------------------------------------------------------------
*  成员函数名   :SetPenColor()
*  功能描述     :设置当前画笔颜色
*  输入参数描述	:
*	COLORREF pencolor:画笔颜色	
*  返回变量说明 :
*  算法描述		:
*  附加说明     :
*--------------------------------------------------------------------------------
*/
void Canvas::SetPenColor(COLORREF pencolor)
{
	m_pencolor = pencolor;
	if (m_pPen)
		m_pPen->SetPenColor(pencolor);
}


/*
*--------------------------------------------------------------------------------
*  成员函数名   :SetFillColor()
*  功能描述     :设置当前画刷填充色(暂没有使用)
*  输入参数描述	:
*	COLORREF fillcolor:画刷的填充颜色	
*  返回变量说明 :
*  算法描述		:
*  附加说明     :
*--------------------------------------------------------------------------------
*/
void Canvas::SetFillColor(COLORREF fillcolor)
{
	m_crFillColor = fillcolor;
}


/*
*--------------------------------------------------------------------------------
*  成员函数名   :SetToolBarStyle()
*  功能描述     :设置当前选中的工具栏(详枚举)
*  输入参数描述	:
*PRETOOLBAR	toolbarstyle:当前选中的工具栏类型	
*  返回变量说明 :
*  算法描述		:
*  附加说明     :
*--------------------------------------------------------------------------------
*/
void Canvas::SetToolBarStyle(PRETOOLBAR toolbarstyle)
{
	m_toolbarstyle = toolbarstyle;
}


/*
*--------------------------------------------------------------------------------
*  成员函数名   SetBasePenStyle()
*  功能描述     :设置当前基本画笔工具(详枚举)
*  参数描述	: 	BASEPEN basepen	:基本画笔工具类型
*  返回变量说明 :
*  算法描述		:
*  附加说明     :
*--------------------------------------------------------------------------------
*/
void Canvas::SetBasePenStyle(BASEPEN basepen)
{
	 m_basepen = basepen;

	switch (m_basepen)
	{
// 	case SELRGN:
// 		//圈选工具
// 		if (m_pSelectTool)
// 			delete m_pSelectTool;
// 		m_pSelectTool = new CRectangleSelectTool(GetSafeHwnd());
// 		break;

	// 各种笔
	case PEN:
		break;

	case SUCKERPEN:
		//吸管
		break;
	
	case SPECIA:
		//特殊笔
		break;

	case TEXT:
		//文字
		break;

	default:
		ASSERT(0);
	}
}


/*
*--------------------------------------------------------------------------------
*  成员函数名   :SetFontStyle()
*  功能描述     :设置当前字体类型(详枚举)
*  输入参数描述	:
*	FONTSTYLE fontstyle	:字体类型
*  返回变量说明 :
*  算法描述		:
*  附加说明     :
*--------------------------------------------------------------------------------
*/
void Canvas::SetFontStyle(FONTSTYLE fontstyle)
{
	 m_fontstyle = fontstyle;
}


/*
*--------------------------------------------------------------------------------
*  成员函数名   :SetPenPointStyle()
*  功能描述     :设置当前笔触类型(详枚举)
*  输入参数描述	:
*	PENPOINTSTYLE penpoint:笔触类型
*  返回变量说明 :
*  算法描述		:
*  附加说明     :
*--------------------------------------------------------------------------------
*/
void Canvas::SetPenStyle(PEN_STYLE PenStyle)
{
	m_PenStyle = PenStyle;
	if (m_pPen)
		m_pPen->SetPenStyle(PenStyle);
}


/*
*--------------------------------------------------------------------------------
*  成员函数名   :SetShapePenStyle()
*  功能描述     :设置当前画笔的轨迹(详枚举)
*  输入参数描述	:
*		PEN_TRACK shapepen	:	画笔轨迹
*  返回变量说明 :
*  算法描述		:
*  附加说明     :
*--------------------------------------------------------------------------------
*/
void Canvas::SetPenTrack(PEN_TRACK PenTrack)
{
	m_PenTrack = PenTrack;
	if (m_pPen)
		m_pPen->SetPenTrack(PenTrack);
}

/*
*--------------------------------------------------------------------------------
*  成员函数名   :SetPictureStyle()
*  功能描述     :设置当前图片类别(详枚举)
*  输入参数描述	:
*	PICTURES picture:图片类别	
*  返回变量说明 :
*  算法描述		:
*  附加说明     :
*--------------------------------------------------------------------------------
*/
void Canvas::SetPictureStyle(PICTURES picture)
{
	 m_picture = picture;
}


/*
*--------------------------------------------------------------------------------
*  成员函数名   :SetSpeciaPenStyle()
*  功能描述     :设置当前特殊笔类型(详枚举)
*  输入参数描述	:
*	SPECIAPEN speciapen	:特殊笔类型
*  返回变量说明 :
*  算法描述		:
*  附加说明     :
*--------------------------------------------------------------------------------
*/
void Canvas::SetSpeciaPenStyle(SPECIAPEN speciapen)
{
// 	m_speciapenType = speciapen;
// 
// 	m_SpeciaPen.SetPenType(m_speciapenType);

}


/*
*--------------------------------------------------------------------------------
*  成员函数名   :SetPenSize()
*  功能描述     :设置当前画笔尺寸(1-110)
*  输入参数描述	:
*	INT presize	:画笔尺寸
*  返回变量说明 :
*  算法描述		:
*  附加说明     : 滚动条范围区间为 1-110，笔大小范围为 1-40，需进行转换
*--------------------------------------------------------------------------------
*/
void Canvas::SetPenSize(INT nSize)
{
    m_orgPenSize = nSize;
	 m_presize = nSize;

	//进行滚动条区间范围到笔大小范围的转换
	const int MAXSIZE = 40;
// 	m_nPenSize = (int) (nSize * (double(MAXSIZE) / 100.0));
// 	m_nPenSize = m_nPenSize < 1 ? 1 : m_nPenSize;
// 	m_nPenSize = m_nPenSize > MAXSIZE ? MAXSIZE : m_nPenSize;
	m_nPenSize = nSize;
	if (m_pPen)
		m_pPen->SetPenSize(m_nPenSize);
}


/*
*--------------------------------------------------------------------------------
*  成员函数名   : OnCutCanvasDibRect
*  功能描述     : 剪切画布上的指定区域，响应自定义消息
*  输入参数描述	:
*		WPARAM	wParam  - 剪切的矩形区域
*		LPARAM	lParam	- 无用
*  算法			: 在画布上指定区域以白色背景画图
*--------------------------------------------------------------------------------
*/
// void Canvas::OnCutCanvasDibRect(WPARAM wParam, LPARAM lParam)
// {
// 	CRect * rcClip = (CRect*) wParam;
// 
// 	CDC * pDC = GetDC();
// 	pDC->SelectObject(GetStockObject(WHITE_BRUSH));
// 	pDC->SelectObject(GetStockObject(NULL_PEN));
// 	pDC->Rectangle(rcClip);
// 	//m_pDib->CutRect(*rcClip);	
// }


/*
*--------------------------------------------------------------------------------
*  成员函数名   : Cut
*  功能描述     : 剪切当前的被选取区域
*  返回值		: 如果不存在选取区域，返回 FALSE
*  算法			: 把选取窗口内的图像保存到剪贴板上，并销毁窗口
*--------------------------------------------------------------------------------
*/
BOOL Canvas::Cut()
{
/*
	if (!m_pFloatDibWnd)
	{
		AfxMessageBox(_T("尚未选取任何区域，不能进行剪切。"), MB_OK);
		return FALSE;
	}

	m_ClipBoard.PutDibToClipBoard(m_pFloatDibWnd->Cut());
	m_pFloatDibWnd = NULL;
	*/
	return TRUE;
}


/*
*--------------------------------------------------------------------------------
*  成员函数名   : Copy
*  功能描述     : 复制当前的被选取区域
*  返回值		: 如果不存在选取区域，返回 FALSE
*  算法			: 把选取窗口内的图像保存到剪贴板上
*--------------------------------------------------------------------------------
*/
BOOL Canvas::Copy()
{
/*	if (!m_pFloatDibWnd)
	{
		AfxMessageBox(_T("尚未选取任何区域，不能进行剪切。"), MB_OK);
		return FALSE;
	}

	m_ClipBoard.PutDibToClipBoard(m_pFloatDibWnd->Copy());

	
	*/
	m_ClipBoard.PutDibToClipBoard(m_pDib);
	return TRUE;
}


/*
*--------------------------------------------------------------------------------
*  成员函数名   : Paste
*  功能描述     : 将当前剪贴板上的图像粘帖到画布中央
*  返回值		: 如果粘贴失败，返回 FALSE
*  算法			: 在画布上创建一个浮动窗口，位于画布中央
*--------------------------------------------------------------------------------
*/
BOOL Canvas::Paste()
{
	if (m_ClipBoard.IsEmpty())
	{
		AfxMessageBox(_T("剪贴板内无任何内容，不能粘贴。"), MB_OK);
		return FALSE;
	}
/*
	if (m_pFloatDibWnd)
	{
		// 如果已经存在一个浮动窗口，先将其与画布融合
		m_pFloatDibWnd->MergeDib(m_pDib);
		delete m_pFloatDibWnd;
		m_pFloatDibWnd = NULL;
	}

	m_pFloatDibWnd = new CFloatDibWnd(GetSafeHwnd(), m_ClipBoard.GetDibFromClipBoard(),m_orgPenSize);

	return m_pFloatDibWnd ? TRUE : FALSE;
	*/
	return FALSE;
}


/*
*--------------------------------------------------------------------------------
*  成员函数名   : SetPenType
*  功能描述     : 将当前剪贴板上的图像粘帖到画布中央
*  返回值		: 如果粘贴失败，返回 FALSE
*  算法			: 在画布上创建一个浮动窗口，位于画布中央
*--------------------------------------------------------------------------------
*/
void Canvas::SetPenType(PEN_TYPE PenType)
{
	m_basepen = PEN;
	m_toolbarstyle = PBASEPEN;
	if(::IsWindow(m_EditText.m_hWnd))
		m_EditText.DestroyWindow();

	if (m_pPen)
		delete m_pPen;

	switch (PenType)
	{
	case PTP_PENCIL:
		//铅笔
		m_pPen = new CPencil(GetDC()->GetSafeHdc());
		break;

	case PTP_PAINTBRUSH:
		//画刷
		m_pPen = new CPaintBrush(GetDC()->GetSafeHdc());
		break;

// 	case PTP_AIRPEN:
// 		//喷枪
// 		m_pPen = new CAirPen(GetSafeHwnd());
// 		break;
		
// 	case PTP_PAINTTUB:
// 		//油漆桶
// 		m_pPen = new CPaintTub(GetDC()->GetSafeHdc());
// 		break;

// 	case PTP_RUBBER:
// 		//橡皮
// 		m_pPen = new CRubber(GetDC()->GetSafeHdc());
// 		break;

	default:
		break;
	}
	m_pPen->SetPenColor(m_pencolor);
	m_pPen->SetPenSize(m_nPenSize);
	m_pPen->SetPenTrack(m_PenTrack);
	m_pPen->SetPenStyle(m_PenStyle);
}


/*
*--------------------------------------------------------------------------------
*  成员函数名   : SelectToolAct
*  功能描述     : 选取工具子功能动作
*  返回值		: 如果粘贴失败，返回 FALSE
*  算法			: 在画布上创建一个浮动窗口，位于画布中央
*--------------------------------------------------------------------------------
*/
BOOL Canvas::SelectToolAct(SELECT_TOOL st)
{
	/*
	switch (st)
	{
	case ST_ALL:
		{
			// 选取全部
			if (m_pFloatDibWnd)
			{
				m_pFloatDibWnd->MergeDib(m_pDib);
				delete m_pFloatDibWnd;
				m_pFloatDibWnd = NULL;
			}
			CRect rc;
			GetClientRect(&rc);
			m_pFloatDibWnd = new CFloatDibWnd(GetSafeHwnd(), rc,m_orgPenSize);
			break;
		}

	case ST_RECTANGLE:
		// 矩形选取工具
		if (m_pFloatDibWnd)
		{
			m_pFloatDibWnd->MergeDib(m_pDib);
			delete m_pFloatDibWnd;
			m_pFloatDibWnd = NULL;
		}
		if (m_pSelectTool)
			delete m_pSelectTool;
		m_pSelectTool = new CRectangleSelectTool(GetSafeHwnd());
		break;

	case ST_CUT:
		// 剪切
		Cut();
		break;

	case ST_COPY:
		// 复制
		Copy();
		break;

	case ST_PASTE:
		// 粘贴
		Paste();
		break;

	default:
		ASSERT(0);
	}
	*/
	return TRUE;
}

/*
*--------------------------------------------------------------------------------
*  成员函数名   :PointInDib
*  功能描述     :判断鼠标是否点击在位图区域
*  输入参数描述	:
*        CPoint point   - 鼠标点击位置的坐标  
*  返回变量说明 :如果在位图区域中，返回TRUE，否则返回FALSE
*  算法描述		: 无
*  附加说明     : 无
*--------------------------------------------------------------------------------
*/

BOOL Canvas::PointInDib(CPoint point)
{
	if (m_pDib->IsEmpty())
		return FALSE;

	CRect rcDib(0, 0, m_pDib->GetWidth(), m_pDib->GetWidth());
	return rcDib.PtInRect(point);

}

/*
*--------------------------------------------------------------------------------
*  成员函数名   :DrawRubber
*  功能描述     :画选择区域的矩形虚框
*  输入参数描述	:
*        CPoint point   -   矩形框的右下角坐标
*  返回变量说明 :无
*  算法描述		: 无
*  附加说明     : 
*         前置条件：矩形框的左上角坐标已得到
*         后置条件：矩形虚框被绘制，如果在这之前已存在虚框，则该虚框被清除
*--------------------------------------------------------------------------------
*/

void Canvas::DrawRubber(CPoint point)
{
	CClientDC dc(this);

	int			nPenStyle;
	int			nPenWidth;
	COLORREF	color;
	int nOldRop = dc.SetROP2(R2_NOTXORPEN);
	
	nPenStyle = PS_DOT;
	nPenWidth = 1;
	color = RGB(0, 0, 0);

	CPen pen(nPenStyle, nPenWidth, color);
	CPen* pOldPen = dc.SelectObject(&pen);

	//  如果矩形已存在，清除它
	DoDrawRubber(&dc, m_rcRubber);

   	CPoint ptStart(m_ptStart);
	
	// 设置新的矩形
	if (point.x < ptStart.x)
	{
		m_rcRubber.left = point.x;
		m_rcRubber.right = ptStart.x;
	}
	else
	{
		m_rcRubber.left = ptStart.x;
		m_rcRubber.right = point.x;
	}
	if (point.y < ptStart.y)
	{
		m_rcRubber.top = point.y;
		m_rcRubber.bottom = ptStart.y;
	}
	else
	{
		m_rcRubber.top = ptStart.y;
		m_rcRubber.bottom = point.y;
	}
	m_rcRubber.NormalizeRect();

	// 画新矩形
	DoDrawRubber(&dc, m_rcRubber);

	// 恢复设备上下文设定
	dc.SelectObject(pOldPen);
	dc.SetROP2(nOldRop);
}

/*
*--------------------------------------------------------------------------------
*  成员函数名   :MergeText
*  功能描述     :融合文字到画布
*  输入参数描述	:无
*  返回变量说明 :如果调用成功返回TRUE,否则返回FALSE
*  算法描述		: 无
*  附加说明     : 
*         前置条件：文本编辑窗口已成功创建
*         后置条件：文本被绘制到画布区
*--------------------------------------------------------------------------------
*/

BOOL Canvas::MergeText()
{
	if (! ::IsWindow(m_EditText.m_hWnd))
		return FALSE;

	CString s;
	m_EditText.GetWindowText(s);

	CRect rc;
	m_EditText.GetWindowRect(&rc);
	ScreenToClient(&rc);
	CRect rcClear = rc;
	rcClear.InflateRect(3, 3);
	m_EditText.DestroyWindow();
	InvalidateRect(&rcClear);

	// 融合文本到DIB中
	if (! s.IsEmpty())
	{
		CClientDC dc(this);

		CDC * pDibDC = m_pDib->BeginPaint(&dc);
		int nOldBkMode = pDibDC->SetBkMode(TRANSPARENT);
		COLORREF crOldTextColor = pDibDC->SetTextColor(m_pencolor);
		CFont * pOldFont = pDibDC->SelectObject(m_pFont);
		

		// 确定矩形区域
		if (rc.bottom > m_pDib->GetHeight())
			rc.bottom = m_pDib->GetHeight();
		if (rc.right > m_pDib->GetWidth())
            rc.right = m_pDib->GetWidth();
        
		pDibDC->DrawText(s, &rc, DT_LEFT);
		pDibDC->SetBkMode(nOldBkMode);
		pDibDC->SetTextColor(m_pencolor);
		pDibDC->SelectObject(pOldFont);
		m_pDib->EndPaint();
	}

	return TRUE;
}

/*
*--------------------------------------------------------------------------------
*  成员函数名   :StartDrawRubber
*  功能描述     :开始绘制选择区域的矩形虚框
*  输入参数描述	:
*        CPoint point   -   矩形框的左下角坐标
*  返回变量说明 : 无
*  算法描述		: 无
*  附加说明     : 
*         前置条件：鼠标被左键被点下
*         后置条件：保存存储虚框左上角坐标，并清空以前的选取框矩形
*--------------------------------------------------------------------------------
*/


void Canvas::StartDrawRubber(CPoint point)
{
	// 保存当前鼠标位置
	m_ptStart = point;

	// 清空当前矩形
	
	m_rcRubber.SetRectEmpty();

	// 捕获鼠标
	SetCapture();
}

/*
*--------------------------------------------------------------------------------
*  成员函数名   :StopDrawRubber
*  功能描述     :停止绘制矩形并创建文本编辑窗口
*  输入参数描述	:无
*  返回变量说明 :无
*  算法描述		:无
*  附加说明     : 
*         前置条件：鼠标被左键被释放
*         后置条件：清空矩形框，并生成文本编辑窗口
*--------------------------------------------------------------------------------
*/

void Canvas::StopDrawRubber()
{
	CRect rc = m_rcRubber;
    	
	CClientDC dc(this);

	int OldRop = dc.SetROP2(R2_NOTXORPEN);

	CPen pen(PS_DOT, 1, RGB(0, 0, 0));
	CPen* pOldpen = dc.SelectObject(&pen);

	DoDrawRubber(&dc, rc);

	dc.SetROP2(OldRop);
	dc.SelectObject(pOldpen);

	if(m_pFont)
		m_pFont->DeleteObject();
	m_nFontSize = m_presize + 10;
	CCatchScreenDlg * dlg = (CCatchScreenDlg*)GetParent();
	//m_bBold = dlg->GetFontIsBold();
// 	m_bItalic = dlg->GetFontIsIncline();
// 	m_bUnderline = dlg->GetFontIsUnderLine();
	//m_stSelection = dlg->GetSelection();

	m_pFont->CreateFont(m_nFontSize, 0, 0, 0, m_bBold, m_bItalic, m_bUnderline,
		FALSE, DEFAULT_CHARSET, OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, PROOF_QUALITY, 
		VARIABLE_PITCH, m_stSelection);

	CFont * pOldFont = dc.SelectObject(m_pFont);
	
	CRect rcLetter(0, 0, 1, 1);
	int nHeight = dc.DrawText(_T("中"), &rcLetter, DT_CALCRECT);
	int nWidth = 4 * rcLetter.Width();
	dc.SelectObject(pOldFont);
    
	if (rc.Height() < nHeight)
		rc.bottom = rc.top + nHeight;
	if (rc.Width() < nWidth)
		rc.right = rc.left + nWidth;

	if (rc.bottom > m_pDib->GetHeight())
		rc.bottom = m_pDib->GetHeight();
    if (rc.right > m_pDib->GetWidth())
		rc.right = m_pDib->GetWidth();
    
	DWORD style = ES_LEFT;
	m_EditText.Create(style | WS_CHILD | WS_VISIBLE | ES_MULTILINE,
		             rc, this, IDC_EDITFONT);
	rc.InflateRect(2, 2);
	InvalidateRect(&rc);
    m_EditText.SetFont(m_pFont);
    m_EditText.SetFocus();
    
	// 释放鼠标
	ReleaseCapture();

}

/*
*--------------------------------------------------------------------------------
*  成员函数名   :DoDrawRubber
*  功能描述     :设置绘制选择区域的矩形虚框的标识符
*  输入参数描述	: 无
*  返回变量说明 : 无
*  算法描述		: 无
*  附加说明     : 无
*--------------------------------------------------------------------------------
*/

void Canvas::DoDrawRubber(CDC *pDC, CRect rc)
{
	if(rc.IsRectEmpty())
		return;

	pDC->Rectangle(&rc);
}


/*
*--------------------------------------------------------------------------------
*  成员函数名   :SetLeafIndex()
*  功能描述     :设置当前采用的树叶图形的索引号
*  输入参数描述	:
*	INT index	:树叶图形的索引号
*  返回变量说明 :
*  算法描述		:
*  附加说明     :
*--------------------------------------------------------------------------------
*/
void Canvas::SetLeafIndex(INT index)
{
    m_leafIndex = index;
}


/*--------------------------------------------------------------------------------
*  成员函数名   :OnTimer
*  功能描述     :为主对话框设置一定时器并实现以下功能:
                 1-在鼠标UP后100毫秒后才进行画布区抓图工作
				 
*  输入参数描述	:
*    
*  返回变量说明 :
*  算法描述		:
*  附加说明:
*-----------------------------------------------------------------------------------
*/
void Canvas::OnTimer(UINT nIDEvent) 
{
	 if (nIDEvent ==1)
  {	
	  CopyClientRectToDib();
	  KillTimer (1);
  }
	
	CWnd::OnTimer(nIDEvent);
}


/*
*--------------------------------------------------------------------------------
*  成员函数名   :GetModify
*  功能描述     :得到修改标记
*  输入参数描述	:
*  返回变量说明 :画布上是否有最新的操作
*  算法描述		:
*  附加说明     :
*--------------------------------------------------------------------------------
*/
BOOL Canvas::GetModify()
{
   return  m_bchanged;

}

////////////////+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//得到画布的尺寸
CRect Canvas::GetCanvasWndRect()
{
  return m_wndRect;
}
CRect Canvas::GetCanvasClientRect()
{
	return m_clientRect;
}

//在创建新画布时，根据指定的画布尺寸重新计算画布中父窗体中的位置
//最大画布矩形区CRect(0,0,524,394)
//控件最初创建于 CRect(94, 39, 618, 433), 
void Canvas::MoveCanvas()
{
  CWnd *pWnd = GetParent();
  CRect wRect,cRect;
  pWnd->GetWindowRect(wRect);  //0,0,800,600
  GetClientRect(cRect);
/*
 if(m_wndRect.Width()>524)
  { 
	  TRACE("可画的图象最大宽度不能超过520像素\n");
	  m_wndRect.right = m_wndRect.left + 524;
	  m_clientRect.right = m_clientRect.left + 520;
  }
  if(m_wndRect.Height() > 394)
  {
	  TRACE("可画的图象最大高度不能超过390像素\n");
	  m_wndRect.bottom = m_wndRect.top + 394;
	  m_clientRect.bottom = m_clientRect.top +390;
  }
  */
//   CPoint centerP;
//   centerP.x = 94+524/2;
//   centerP.y = 39+394/2;
  
//   CRect rect;
//   rect = m_wndRect;
//   m_wndRect.left = centerP.x - m_wndRect.Width()/2;
//   m_wndRect.right = m_wndRect.left + rect.Width();
//   m_wndRect.top = centerP.y - m_wndRect.Height()/2;
//   m_wndRect.bottom = m_wndRect.top + rect.Height();

  //通知父窗体重画客户区
//  ::SendMessage(pWnd->GetSafeHwnd(),WM_PAINT,0,0);
     MoveWindow(m_wndRect);

	 //@@@@@@@@@新加,解决画布重新新建一大过上一画布尺寸时部份底色为灰色的问题
	 /*
	 CRect lpRect;
	 GetClientRect(lpRect);
	 CPaintDC dc(this); 
     dc.FillSolidRect(CRect(0,0,520,390),COLOR_WHITE);
	 */
}

//得到当前笔的原始尺寸值
INT Canvas::GetOrgPenSize()
{
 return m_orgPenSize;
}

//得到当前显示的位图句柄
HBITMAP Canvas::GetHbitmap()
{
  if(!hBitmap)
	  hBitmap = m_pDib->GetHBitmap();
  return hBitmap;
}

//设置新画布的尺寸
void Canvas::SetNewWndRect(CRect  wndRect)
{
  m_wndRect = wndRect;
   //MoveCanvas();
}


//使用外部文件初始化画布上的图
void Canvas::InitImage(CString fileName)
{/*
    CClientDC cdc(this);
	CRect rect;
	GetClientRect(&rect);
    CDib * cdib = new CDib();
	if(cdib->Load(fileName))
	{
		if(m_pDib)
			delete m_pDib;
		m_pDib = new CDib();
		m_pDib->Create(cdib->GetHBitmap());
	    CDC * pDC = m_pDib->BeginPaint(&cdc); 
	    if (!m_pDib->IsEmpty())
				m_pDib->Display(&cdc,0, 0);  
		m_pDib->EndPaint(); 
		
	}
		delete cdib;
		cdib = NULL;
*/
	LoadFile(fileName);

}
//使用外部位图指针初始化画布上的图
void Canvas::InitImage(HBITMAP hBitmap)
{
    if(!hBitmap)
		return;
	CRect rect;
	GetClientRect(&rect);
    CDib * cdib = new CDib;
	cdib->Create(hBitmap);

    if(m_pDib)
			delete m_pDib;
	m_pDib = new CDib();
	m_pDib->Create(hBitmap);


	delete cdib;
	cdib = NULL;


   CClientDC cdc(this);
   CDC * pDC = m_pDib->BeginPaint(&cdc); 
   if (!m_pDib->IsEmpty())
			m_pDib->Display(&cdc,0, 0);
    m_pDib->EndPaint(); 
    Invalidate(FALSE); 

}

//更新EXE主窗体中的位图
void Canvas::UpdateMainWndImage()
{
  	  CBitmap *hBitmap = m_pDib->GetBitmap();

      ::SendMessage(AfxGetApp()->GetMainWnd()->GetSafeHwnd(),WM_SENDIMAGETOEXE,WPARAM(hBitmap),LPARAM(m_pDib));
      //保存到临时文件中
	//  m_pDib->Save(m_curTmpFile);
}


//设置画板上的浮动窗体是否显示按纽
void Canvas::SetFloatWndShowBtn(BOOL bCreateBtn)
{
//   if(m_pFloatDibWnd->IsCartoonWnd())
	/*if(m_pFloatDibWnd)
    	m_pFloatDibWnd->SetCreateBtn(bCreateBtn);
		*/
	Invalidate();
}
