// FloatDibWnd.cpp: implementation of the CFloatDibWnd class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "..\Paint\resource.h"
#include "..\INCLUDE\FloatDibWnd.h"
#include "..\INCLUDE\DefineMessage.h"
#include "..\include\canvas.h"
#include <math.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

extern  CString g_strAppPath;        
extern  IniPaintSet g_stuPaintSet;
extern  BOOL g_createBtn;

//const COLORREF TRANSPARENT_COLOR = RGB(255, 0, 255);

//形变按钮大小常量
const int BUTTONSIZE = 16;

//定义图象宽度或高度小于下面的尺寸时,不创建四个按纽
const int MINSIZE = 24;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

/*
*--------------------------------------------------------------------------------
*  函数名	: CFloatDibWnd
*  功能		: 构造函数
*  参数		: HWND hParentWnd			- 父窗口句柄
*			  const CString strFileName - 卡通贴图文件名
*  说明		: 为卡通贴图功能而设 
*--------------------------------------------------------------------------------
*/
CFloatDibWnd::CFloatDibWnd(HWND hParentWnd, const CString strFileName,INT curDrawSize/*=55*/)
{
	m_curDrawSize = curDrawSize;
	m_hParentWnd = hParentWnd; 
	m_ChangeType = CT_MOVE;
	m_pDib = new CDib();
	m_pSrcDib = new CDib();
	m_pHistoryDib = NULL;
	// 标识为卡通贴图窗口
	m_bCartoonWnd = TRUE;
	m_dblPreRotateAngle = 0.0;

	//设置卡通图片并创建窗口
   // CheckShowBtn();
	SetCartoonDib(strFileName);


}


/*
*--------------------------------------------------------------------------------
*  函数名	: CFloatDibWnd
*  功能		: 构造函数
*  参数		: HWND hParentWnd			- 父窗口句柄
*			  const CRect& rect			- 被选取区域，相对于父窗口坐标
*  算法		: 将父窗口指定区域内的图像复制到浮动窗口中
*  说明		: 为选取区域浮动窗口而设
*--------------------------------------------------------------------------------
*/
CFloatDibWnd::CFloatDibWnd(HWND hParentWnd, const CRect& rect,INT curDrawSize/*=55*/)
{
	m_curDrawSize = curDrawSize;
	m_hParentWnd = hParentWnd; 
	m_ChangeType = CT_MOVE;
	m_bCartoonWnd = FALSE;
	m_pHistoryDib = NULL;
	m_dblPreRotateAngle = 0.0;
	// 调整坐标，以确定左上角和右下角坐标正确	
	m_rcWndRect.left = rect.left <= rect.right ? rect.left : rect.right;
	m_rcWndRect.right = rect.left <= rect.right ? rect.right : rect.left;
	m_rcWndRect.top = rect.top <= rect.bottom ? rect.top : rect.bottom;
	m_rcWndRect.bottom = rect.top <= rect.bottom ? rect.bottom : rect.top;	

	// 从指定窗口上抓取指定区域的图像到窗口图像中
	m_pDib = new CDib();	
	m_pDib->Create(m_hParentWnd, m_rcWndRect);

	// 保存原始图像
	m_pSrcDib = m_pDib->Clone();
	
    CheckShowBtn();
	// 创建窗口
	CreateWnd();
}


/*
*--------------------------------------------------------------------------------
*  函数名	: CFloatDibWnd
*  功能		: 构造函数
*  参数		: HWND hParentWnd	- 父窗口句柄
*			  CDib* pDib		- DIB图像指针
*  算法		: 新建浮动窗口，并居中显示，窗口内容为指定的DIB图像
*  说明     : 为粘贴图像功能而设
*--------------------------------------------------------------------------------
*/
CFloatDibWnd::CFloatDibWnd(HWND hParentWnd, CDib* pDib,INT curDrawSize/*=55*/)
{
	ASSERT(pDib);
	m_curDrawSize = curDrawSize;
	m_hParentWnd = hParentWnd; 
	m_ChangeType = CT_MOVE;
	m_pHistoryDib = NULL;
	m_bCartoonWnd = FALSE;
	m_dblPreRotateAngle = 0.0;
	m_pDib = pDib->Clone();
	ASSERT(m_pDib);

	// 保存原始图像
	m_pSrcDib = m_pDib->Clone();
	ASSERT(m_pSrcDib);


	// 将窗口位置设为居中
	long nWidth = m_pDib->GetWidth();
	long nHeight = m_pDib->GetHeight();

	CRect rcWnd;
	::GetWindowRect(m_hParentWnd, &rcWnd);
	
	m_rcWndRect.left = rcWnd.Width() / 2 - nWidth / 2;
	m_rcWndRect.right = rcWnd.Width() / 2 + nWidth / 2;
	m_rcWndRect.top = rcWnd.Height() / 2 - nHeight / 2;
	m_rcWndRect.bottom = rcWnd.Height() / 2 + nHeight / 2;

    CheckShowBtn();

	//创建窗口
	CreateWnd();
}


CFloatDibWnd::~CFloatDibWnd()
{
	DestroyWindow();

	if(m_pDib)
	{
	   delete m_pDib;
	    m_pDib = NULL;
	}
	if(m_pHistoryDib)
	{
	    delete m_pHistoryDib;
	    m_pHistoryDib = NULL;
	}
	if (m_pSrcDib)
		delete m_pSrcDib;
}


BEGIN_MESSAGE_MAP(CFloatDibWnd, CWnd)
	//{{AFX_MSG_MAP(CFloatDibWnd)
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_MOVE()
	ON_WM_SETCURSOR()
	ON_WM_NCHITTEST()
	ON_WM_SIZE()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_MESSAGE(WM_OWNDOWNMESSAGE, OnBeginChange)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/*
*--------------------------------------------------------------------------------
*  函数名	: OnCreate
*  功能     : 重载创建窗口消息
*  说明     : 创建窗口时创建形变按钮
*--------------------------------------------------------------------------------
*/
int CFloatDibWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	CreateButton();

	return 0;
}


/*
*--------------------------------------------------------------------------------
*  函数名	: CreateButton
*  功能		: 创建浮动窗口四周的形变按钮
*  说明		: 按钮位于图片区内，每个按钮为 16 像素大小
*--------------------------------------------------------------------------------
*/
BOOL CFloatDibWnd::CreateButton()
{	
    if(!g_createBtn)
		return FALSE;


	// 创建图片四周的形变按钮
	static CString strfilename[] = {
				_T("BtnRes\\i1-2.bmp"),  _T("BtnRes\\i1-1.bmp"),
				_T("BtnRes\\i2-2.bmp"),  _T("BtnRes\\i2-1.bmp"),
				_T("BtnRes\\i3-2.bmp"),  _T("BtnRes\\i3-1.bmp"),
				_T("BtnRes\\i4-2.bmp"),  _T("BtnRes\\i4-1.bmp"),
				_T("BtnRes\\i5-2.bmp"),  _T("BtnRes\\i5-1.bmp"),
				_T("BtnRes\\i6-2.bmp"),  _T("BtnRes\\i6-1.bmp"),
				_T("BtnRes\\i7-2.bmp"),  _T("BtnRes\\i7-1.bmp"),
				_T("BtnRes\\i8-2.bmp"),  _T("BtnRes\\i8-1.bmp")
				};

	CRect rect;
	GetClientRect(&rect);

	HCURSOR hCursor = AfxGetApp()->LoadCursor(IDC_CURHAND);
//	hCursor = LoadCursor(NULL, IDC_HELP);
 //    hCursor = AfxGetApp()->LoadCursor(IDC_HELP);

		// 调整大小，右上角
		m_btnResize.Create("", WS_CHILD|WS_VISIBLE, CRect(0,0,0,0), this, IDC_VERTOPSIZE); 
		m_btnResize.SetBitmaps(strfilename[4], g_stuPaintSet.tranCol, strfilename[5], g_stuPaintSet.tranCol); 
		m_btnResize.SetCursor(hCursor); 
		m_btnResize.SetLbuttonSendMsgStyle(1);

		// 旋转按钮，右下角
		m_btnRotate.Create("", WS_CHILD|WS_VISIBLE, CRect(0,0,0,0), this, IDC_ROTATE); 
		m_btnRotate.SetBitmaps(strfilename[6], g_stuPaintSet.tranCol, strfilename[7], g_stuPaintSet.tranCol); 
		m_btnRotate.SetCursor(hCursor); 
		m_btnRotate.SetLbuttonSendMsgStyle(1);

		// 镜像按钮，左下角
		m_btnHorMirror.Create("", WS_CHILD|WS_VISIBLE, CRect(0,0,0,0), this, IDC_HORMIRROR); 
		m_btnHorMirror.SetBitmaps(strfilename[8], g_stuPaintSet.tranCol, strfilename[9], g_stuPaintSet.tranCol); 
		m_btnHorMirror.SetCursor(hCursor); 
		m_btnHorMirror.SetLbuttonSendMsgStyle(1);

		// 翻转按钮，左上角
		m_btnVerMirror.Create("", WS_CHILD|WS_VISIBLE, CRect(0,0,0,0), this, IDC_VERMIRROR); 
  		m_btnVerMirror.SetBitmaps(strfilename[10], g_stuPaintSet.tranCol, strfilename[11], g_stuPaintSet.tranCol); 
		m_btnVerMirror.SetCursor(hCursor); 
		m_btnVerMirror.SetLbuttonSendMsgStyle(1);

     if(!g_createBtn)
	 {
     	m_btnRotate.ShowWindow(SW_HIDE); 
		m_btnVerMirror.ShowWindow(SW_HIDE); 
		m_btnHorMirror.ShowWindow(SW_HIDE); 
		m_btnResize.ShowWindow(SW_HIDE); 
	 }

	return TRUE;
}


/*
*--------------------------------------------------------------------------------
*  成员函数名   : OnPaint() 
*  功能描述     : 绘制窗体或窗体尺寸发生改变时重画窗体
*  算法			: 透明显示位图，透明色为 g_stuPaintSet.tranCol
*  附加说明		: 窗口缩放时图像并不改变，而是在显示时改变复制位图，然后缩放显示
*--------------------------------------------------------------------------------
*/
void CFloatDibWnd::OnPaint() 
{
	ASSERT(m_pDib);

	CRect rect;
	GetClientRect(&rect);

	// 用当前图像创建一个临时位图，对它进行缩放显示，以防失真
	CDib *pTmpDib = m_pDib->Clone();
	pTmpDib->ChangeImageSize(rect.Width(), rect.Height()); 
	pTmpDib->DisplayTransparent(GetDC(), 0, 0, g_stuPaintSet.tranCol);
	delete pTmpDib;

	CWnd::OnPaint();
}


/*
*--------------------------------------------------------------------------------
*  成员函数名   : OnNcHitTest
*  功能描述     : 点击测试，返回标题栏，以拖动窗口
*--------------------------------------------------------------------------------
*/
UINT CFloatDibWnd::OnNcHitTest(CPoint point) 
{
	return HTCAPTION;
}


/*
*--------------------------------------------------------------------------------
*  函数名: OnMove
*  功能  : 处理窗口移动消息，让窗口刷新
*  算法  : 如果是选择区域窗口移动时需清除掉原来的画布区域
*--------------------------------------------------------------------------------
*/
void CFloatDibWnd::OnMove(int x, int y) 
{
	Invalidate(TRUE);
}


/*
*--------------------------------------------------------------------------------
*  函数名	: SetCartoonDib
*  功能		: 设置卡通窗口的 Dib 图像
*  参数		: CString strDibFileName -	加载位图的文件名
*  算法		:
*  前置条件 : m_pDib 已初始化
*  后置条件 : 删除现存的位图，赋于新的位图，销毁现存窗口，新建一个窗口并居中显示
*--------------------------------------------------------------------------------
*/
void CFloatDibWnd::SetCartoonDib(CString strDibFileName)
{
    if(!FileExist(strDibFileName))
		return;

	DestroyWindow();

	if (m_pDib)
	{
		m_pDib->Destroy();
		delete m_pDib;
	}

//	Canvas * pWnd = (Canvas *)GetParent();
//	CRect wndRect = pWnd->GetCanvasWndRect();

	// 从文件中导入图片
	m_pDib = new CDib();
	m_pDib->Load(strDibFileName);
//	m_pDib->ChangeImageSize(nWidth, nHeight);

	CRect rcWnd;
	::GetWindowRect(m_hParentWnd, &rcWnd);

	// 初始化时的窗口大小
	long nWidth = m_pDib->GetWidth();
	long nHeight = m_pDib->GetHeight();

    double scaleXY = nWidth/nHeight;
    double scaleYX = nHeight/nWidth;
    double scaleDef = rcWnd.Width()/rcWnd.Height();

	if(scaleXY <= scaleDef)
	{
		if(nWidth > rcWnd.Width()-4)
		{

     			nWidth = rcWnd.Width()-20;
				nHeight = nWidth * scaleYX;
		}
	}
   else
   {
      if(nHeight > rcWnd.Height() - 4)
	  {
		  nHeight = rcWnd.Height() -4;
		  nWidth = nHeight * scaleXY;
	  }

   }
   
   //根据主窗口上滑块的位置计算图象加载的大小
    if(g_stuPaintSet.bCartoonOrgSize)
	{
		m_rcWndRect.left = rcWnd.Width() / 2 - nWidth / 2;
		m_rcWndRect.right = rcWnd.Width() / 2 + nWidth / 2;
		m_rcWndRect.top = rcWnd.Height() / 2 - nHeight / 2;
		m_rcWndRect.bottom = rcWnd.Height() / 2 + nHeight / 2;
	}
	else
	{
        DOUBLE scale =  m_curDrawSize/110.0;
	    m_rcWndRect.left = rcWnd.Width() / 2 - DOUBLE(nWidth / 2.0)*scale;
		m_rcWndRect.right = rcWnd.Width() / 2 + DOUBLE(nWidth / 2.0)*scale;
		m_rcWndRect.top = rcWnd.Height() / 2 - DOUBLE(nHeight / 2.0)*scale;
		m_rcWndRect.bottom = rcWnd.Height() / 2 + DOUBLE(nHeight / 2.0)*scale;      

	}

	// 保存原始图像
	if (m_pSrcDib)
		delete m_pSrcDib;
	m_pSrcDib = m_pDib->Clone();

   CheckShowBtn();

	// 创建窗口
	CreateWnd();
}


/*
*--------------------------------------------------------------------------------
*  函数名	: CreateWnd
*  功能		: 创建浮动窗口
*--------------------------------------------------------------------------------
*/
BOOL CFloatDibWnd::CreateWnd()
{	
	DestroyWindow();
	// 创建窗口
	return Create(NULL,
			"卡通窗体",
			WS_VISIBLE | WS_CHILD | WS_CLIPSIBLINGS, 
		    m_rcWndRect, 
			CWnd::FromHandle(m_hParentWnd), 
			0x0844 );
}	


/*
*--------------------------------------------------------------------------------
*  函数名	: CenterDib
*  功能		: 将浮动窗口置于父窗口的正中间
*  算法		: 窗口大小与图像同样大小
*  前置条件 : m_pDib 已初始化
*  说明		: 得出的矩形座标是相对父窗口的坐标
*--------------------------------------------------------------------------------
*/
//DEL void CFloatDibWnd::CenterWnd()
//DEL {
//DEL 	long nWidth = m_pDib->GetWidth();
//DEL 	long nHeight = m_pDib->GetHeight();
//DEL 
//DEL 	CRect rcWnd;
//DEL 	::GetWindowRect(m_hParentWnd, &rcWnd);
//DEL 	
//DEL 	m_rcWndRect.left = rcWnd.Width() / 2 - nWidth / 2;
//DEL 	m_rcWndRect.right = rcWnd.Width() / 2 + nWidth / 2;
//DEL 	m_rcWndRect.top = rcWnd.Height() / 2 - nHeight / 2;
//DEL 	m_rcWndRect.bottom = rcWnd.Height() / 2 + nHeight / 2;
//DEL }


/*
*--------------------------------------------------------------------------------
*  函数名	: MergeDib
*  功能		: 将窗口内的图像与指定图像融合
*  参数		: CDib* pDib	-	画布图像，融合于此DIB之上
*  算法		: 先隐藏按钮，然后将窗口内的图像抓取下来，再将此DIB与参数DIB融合
*--------------------------------------------------------------------------------
*/
BOOL CFloatDibWnd::MergeDib(CDib * pDib)
{	
	CDib* newDib = new CDib();
	
	// 隐藏全部形变按钮
	if(g_createBtn)
    	HideAllButton();

	CRect rcSelf, rcParent;
	GetWindowRect(&rcSelf);
	::GetWindowRect(m_hParentWnd, &rcParent);

	// 抓取浮动窗口客户区图像
	newDib->Create(GetSafeHwnd(), CRect(0, 0, rcSelf.Width(), rcSelf.Height()) );

	// 计算起点座标
	int xStart, yStart;

	// 减 2 是因为边框占两个像素
	xStart = rcSelf.left - rcParent.left - 2;
	yStart = rcSelf.top - rcParent.top - 2;

	pDib->MergeDib(newDib->GetHandle(), CPoint(xStart, yStart));

	delete newDib;
	return TRUE;
}


/*
*--------------------------------------------------------------------------------
*  成员函数名   :OnLButtonUp
*  功能描述     :鼠标左键按起时的消息
*  输入参数描述	:
*    UINT nType：按键类型
*    CPoint point ：当前鼠标坐标点的位置
*  返回变量说明 :
*  算法描述		:
*  附加说明     :用于取消左键按下的标志，并计算出要旋转的角度，画出旋转后的位图
*--------------------------------------------------------------------------------
*/
void CFloatDibWnd::OnLButtonUp(UINT nFlags, CPoint point) 
{       
	//结束形变操作
//	SetCursor(LoadCursor(NULL, IDC_ARROW));
	SetCursor(AfxGetApp()->LoadCursor(IDC_ARROW));
	switch (m_ChangeType)
	{
	case CT_ROTATE:
		{
		// 旋转窗口内的图像以原始图像为操作对象，此次旋转要加上最后的旋转角度
		double dblAngle = GetRotateAngle(PointToParentClient(point));

		// 旋转
		RotateAngle(dblAngle);

		UpdateWindow();
		break;
		}

	case CT_RESIZE:
		{
		// 缩放图像时实际上只是改变窗口大小，并不改变图像的实际尺寸
		// 改变图像及窗口大小
		CRect rect = RectToParentClient(point);
		::MoveWindow(GetSafeHwnd(), rect.left, rect.top, rect.Width(), rect.Height(), TRUE);
		break;
		}
	
	default:
		return;
	}
	m_ChangeType = CT_MOVE;

	::SendMessage(m_hParentWnd, WM_PAINT, 0, 0);
	InvalidateRect(NULL, TRUE);

	ReleaseCapture();

//	CWnd::OnLButtonUp(nFlags, point);
}


/*
*--------------------------------------------------------------------------------
*  成员函数名   :OnMouseMove
*  功能描述     :当鼠标在窗体移动消息
*  输入参数描述	:
*    UINT nType ：  按键类型
*    CPoint point ：当前鼠标坐标点的位置
*  返回变量说明 :
*  算法描述		:以矩形区中心点坐标为基点进行旋转
*  附加说明     :主要用于任意角度旋转图形时，得到旋转的角度
*--------------------------------------------------------------------------------
*/
void CFloatDibWnd::OnMouseMove(UINT nFlags, CPoint point) 
{
    //画一虚线框，当旋转图片时先只旋转这个框，当MouseUp时才正式旋转图形

	HDC hDC = ::GetDC(m_hParentWnd);
	switch (m_ChangeType)
	{
	case CT_ROTATE:
		{	    	
		double dblAngle;
		dblAngle = GetRotateAngle(PointToParentClient(m_ptLast));
		DrawRotateBorder(dblAngle);

		dblAngle = GetRotateAngle(PointToParentClient(point));
        DrawRotateBorder(dblAngle);
		SetCursor(AfxGetApp()->LoadCursor(IDC_CURROTATE));	
		
		break;
		}

	case CT_MOVE:
		break;

	case CT_RESIZE:
		// 调整大小	
		{
			CRect rcBorder = RectToParentClient(m_ptLast);
			DrawResizeBorder(rcBorder);				

			rcBorder = RectToParentClient(point);
			DrawResizeBorder(rcBorder);

			break;
		}

	default:
		break;
	}

	m_ptLast = point;
	CWnd::OnMouseMove(nFlags, point);
}


/*
*--------------------------------------------------------------------------------
*  成员函数名   :OnSize
*  功能描述     :窗口尺寸大小发生变化时
*  输入参数描述	:
*    UINT nType：窗体尺寸变化类型
*    int cx    ：窗体新客户区的宽度
*    int cy    ：窗体新客户区的高度
*  返回变量说明 :
*  算法描述		:
*  附加说明     :
*--------------------------------------------------------------------------------
*/
void CFloatDibWnd::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);
	
	CRect rect;
	GetClientRect(&rect);

	//更新形变按钮的位置
    if(g_createBtn)
	{
		m_btnResize.MoveWindow(rect.Width() - BUTTONSIZE, 0, BUTTONSIZE, BUTTONSIZE);
		m_btnRotate.MoveWindow(rect.Width() - BUTTONSIZE, rect.bottom - BUTTONSIZE, BUTTONSIZE, BUTTONSIZE);
		m_btnHorMirror.MoveWindow(0, rect.bottom - BUTTONSIZE, BUTTONSIZE, BUTTONSIZE);
		m_btnVerMirror.MoveWindow(0, 0, BUTTONSIZE, BUTTONSIZE);
	}
	Invalidate(TRUE);
}


/*
*--------------------------------------------------------------------------------
*  成员函数名   :OnSetCursor
*  功能描述     :设置光标样式
*  输入参数描述	:
*		CWnd* pWnd   ：
*		UINT nHitTest：
*		UINT message ：
*  返回变量说明 :
*  算法描述		:
*  附加说明     :
*--------------------------------------------------------------------------------
*/
BOOL CFloatDibWnd::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	SetCursor(LoadCursor(NULL, IDC_SIZEALL));
  //  SetCursor(AfxGetApp()->LoadCursor(IDC_SIZEALL));
	return TRUE;
}


/*
*--------------------------------------------------------------------------------
*  函数名	: DrawRotateBorder
*  功能		: 画出在旋转过程中的虚边框
*  参数		: DOUBLE angle - 旋转角度
*  算法
*--------------------------------------------------------------------------------
*/
void CFloatDibWnd::DrawRotateBorder(double angle)
{
	angle = -angle;
	CRect rect = RectToParent();
	CPoint ptCenter = rect.CenterPoint();
	double dwRadius = sqrt(pow((ptCenter.x - rect.left), 2) + pow((ptCenter.y - rect.top), 2));

	double dwStartAngle[4];
	double disX = (double)(rect.Width() / 2);
	double disY = (double) (rect.Height() / 2);	
	double dwBaseAngle = atan(disX / disY) * (180 / 3.1415926545);

	dwStartAngle[0] = 90 + dwBaseAngle;
	dwStartAngle[1] = 270 - dwBaseAngle;
	dwStartAngle[2] = 270 + dwBaseAngle;
	dwStartAngle[3] = 90 - dwBaseAngle;

	CPoint ptVertex[4];	
	ptVertex[0] = GetArcEndPoint(ptCenter, dwRadius, dwStartAngle[0], angle);
	ptVertex[1] = GetArcEndPoint(ptCenter, dwRadius, dwStartAngle[1], angle);
	ptVertex[2] = GetArcEndPoint(ptCenter, dwRadius, dwStartAngle[2], angle);
	ptVertex[3] = GetArcEndPoint(ptCenter, dwRadius, dwStartAngle[3], angle);


	HDC hDC = ::GetDC(m_hParentWnd);

	HBRUSH hOldBrush = (HBRUSH) SelectObject(hDC, GetStockObject(NULL_BRUSH));
	HPEN hPen = CreatePen(PS_DASH, 1, RGB(0x0, 0x0, 0x0));
	HPEN hOldPen = (HPEN) SelectObject(hDC, hPen);
	int nOldRop = SetROP2(hDC, R2_NOTXORPEN);

	::Polygon(hDC, ptVertex, 4);

	SetROP2(hDC, nOldRop);
	SelectObject(hDC, hOldPen);
	SelectObject(hDC, hOldBrush);

	DeleteObject(hOldBrush);
	DeleteObject(hOldPen);
	DeleteObject(hPen);
}


/*
*--------------------------------------------------------------------------------
*  成员函数名   : GetRotateAngle
*  功能描述     : 根据三条边长计算旋转的角度(角度)，扫描角，顺时针为正角
*  输入参数描述	: CPoint ptEnd		-	旋转终止点终止
*  返回变量说明 :
*  算法描述		: 以浮动窗口最右下角点为基准点，计算终止点与基准点边线与水平线之间的角度
*  附加说明     : 坐标是相对于父窗口客户区的坐标，顺时针旋转为正角
*--------------------------------------------------------------------------------
*/
DOUBLE CFloatDibWnd::GetRotateAngle(CPoint ptEnd)
{
	// 弧度转角度常量
	const double pitoa = 180 / 3.1415926535l;

	// 窗口矩形
	CRect rect;
	rect = RectToParentClient();
	
	// 最右下角点为基准点
	CPoint ptCenter, ptBottomRight;


	// 以图像中心为角度计算基准点
	ptCenter = rect.CenterPoint();
	ptBottomRight = rect.BottomRight();

	double nx, ny, angle, dblStartAngle;

	// 计算起始旋转角
	nx = abs(ptBottomRight.x - ptCenter.x);
	ny = abs(ptBottomRight.y - ptCenter.y);
	dblStartAngle = atan(ny / nx) * pitoa;

	// 计算旋转过后的右下角点与中心点边线与水平线的夹角
	nx = abs(ptEnd.x - ptCenter.x);
	ny = abs(ptEnd.y - ptCenter.y);
	angle = atan(ny / nx) * pitoa;

	if (ptEnd.x > ptCenter.x && ptEnd.y > ptCenter.y)
	{
		// 四象限，为正角
		angle = angle;
	}
	else if (ptEnd.x > ptCenter.x && ptEnd.y < ptCenter.y)
	{
		// 一象限，为负角
		angle = -angle;
	}
	else if (ptEnd.x < ptCenter.x && ptEnd.y > ptCenter.y)
	{
		// 三象限
		angle = 180 - angle;
	}
	else if (ptEnd.x < ptCenter.x && ptEnd.y < ptCenter.y)
	{
		// 二象限
		angle = 180 + angle;
	}
	else if (ptEnd.x == ptCenter.x && ptEnd.y < ptCenter.y)
	{
		// 垂直线正轴
		angle = 270;
	}
	else if (ptEnd.x == ptCenter.x && ptEnd.y > ptCenter.y)
	{
		// 垂直线负轴
		angle = 90;
	}
	else if (ptEnd.y == ptCenter.y && ptEnd.x > ptCenter.x)
	{
		// 水平线正轴
		angle = 0;
	}
	else if (ptEnd.y == ptCenter.y && ptEnd.x < ptCenter.x)
	{
		// 水平线负轴
		angle = 180;
	}
	else if (ptEnd.y == ptCenter.y && ptEnd.x == ptCenter.x)
	{
		// 原点
		angle = 0;
	}	

	// 图片旋转角度
	angle -= dblStartAngle;

	return angle;
}


/*
*--------------------------------------------------------------------------------
*  成员函数名   :HideAllButton()
*  功能描述     :隐藏所有的按纽，便于在融合位图时将按纽控件也融入画布区图形中
*  输入参数描述	:
*  返回变量说明 :
*  算法描述		:
*  附加说明     :
*--------------------------------------------------------------------------------
*/
void CFloatDibWnd::HideAllButton()
{	

	m_btnRotate.ShowWindow(SW_HIDE); 
	m_btnVerMirror.ShowWindow(SW_HIDE); 
	m_btnHorMirror.ShowWindow(SW_HIDE); 
	m_btnResize.ShowWindow(SW_HIDE); 

	//先刷新父窗口再刷新自身窗口
	::SendMessage(m_hParentWnd, WM_PAINT, 0, 0);
	UpdateWindow();
}


/*
*--------------------------------------------------------------------------------
*  函数名	: OnBeginChange
*  功能		: 开始对图片进行形变操作
*  参数     :
*		WPARAM wParam	-	形变按钮控件ID号
*		LPARAM lParam	-	无用
*--------------------------------------------------------------------------------
*/
void CFloatDibWnd::OnBeginChange(WPARAM wParam, LPARAM lParam)
{
	CRect rcSelf, rect;
	::GetWindowRect(GetSafeHwnd(), &rcSelf);
	rect = RectToParentClient();
//	::GetWindowRect(&rcParent);

	// 控件ID
	WORD wCtlID = LOWORD(wParam);
	switch (wCtlID)
	{
	case IDC_ROTATE:
		m_ChangeType = CT_ROTATE;
		m_ptLast.x = rect.Width();
		m_ptLast.y = rect.Height();
//		::SetCursorPos(rcSelf.right, rcSelf.bottom);
		SetCursor(AfxGetApp()->LoadCursor(IDC_CURROTATE));
		break;

	case IDC_VERMIRROR:
		// 翻转，原始图像和当前图像都翻转，因为翻转不会引起失真
		SetCursor(AfxGetApp()->LoadCursor(IDC_CURVERMIRRION));
		m_pSrcDib->FlipVert();
		m_pDib->FlipVert();
		::InvalidateRect(m_hParentWnd, NULL, FALSE);
		return;
		break;

	case IDC_HORMIRROR:
		// 镜像，原始图像和当前图像都翻转，因为镜像不会引起失真
		SetCursor(AfxGetApp()->LoadCursor(IDC_CURHORMIRRION));
		m_pSrcDib->FlipHorz();
		m_pDib->FlipHorz();
		::InvalidateRect(m_hParentWnd, NULL, FALSE);
		return;
		break;		

	default:
		// 缩放，点坐标是相对于浮动窗口的坐标，而非父窗口
		m_ChangeType = CT_RESIZE;

		// 将鼠标位置定于右上角
		m_ptLast.x = rcSelf.Width();
		m_ptLast.y = 0;
		::SetCursorPos(rcSelf.right, rcSelf.top);
		
		DrawResizeBorder(rect);	
		break;
	}
	
	// 捕获鼠标消息
	if (SetCapture() != NULL)
	{
		ReleaseCapture();
		SetCapture();
	}

//	SetCursor(LoadCursor(NULL, IDC_ARROW));
     SetCursor(AfxGetApp()->LoadCursor(IDC_ARROW));
}


/*
*--------------------------------------------------------------------------------
*  函数名	: DrawResizeBorder
*  功能		: 画出在缩放过程中的虚边框
*  参数		: DOUBLE angle - 旋转角度
*  算法
*--------------------------------------------------------------------------------
*/
void CFloatDibWnd::DrawResizeBorder(const CRect &rect) const
{
	HDC hDC = ::GetDC(m_hParentWnd);

	HBRUSH hOldBrush = (HBRUSH) SelectObject(hDC, GetStockObject(NULL_BRUSH));
	HPEN hPen = CreatePen(PS_DASH, 1, RGB(0x0, 0x0, 0x0));
	HPEN hOldPen = (HPEN) SelectObject(hDC, hPen);

	int nOldRop = SetROP2(hDC, R2_NOTXORPEN);
	Rectangle(hDC, rect.left, rect.top, rect.right, rect.bottom);
	SetROP2(hDC, nOldRop);

	SelectObject(hDC, hOldPen);
	SelectObject(hDC, hOldBrush);

	DeleteObject(hOldBrush);
	DeleteObject(hOldPen);
	DeleteObject(hPen);
}


/*
*--------------------------------------------------------------------------------
*  函数名	: RectToParent
*  功能		: 得到相对父窗口坐标的窗口边框矩形区域
*  返回值	: CRect	- 子窗口相对父窗口的坐标
*  说明		: 是相对父窗口而非父窗口客户区，因为父窗口有两个像素的边框
*--------------------------------------------------------------------------------
*/
CRect CFloatDibWnd::RectToParent()
{
	CRect rcParent, rcSelf, rect;
	::GetWindowRect(GetSafeHwnd(), &rcSelf);
	::GetWindowRect(m_hParentWnd, &rcParent);

	// 相对父窗口左上角而非父窗口客户区左上角的座标
	rect.left = rcSelf.left - rcParent.left;
	rect.top = rcSelf.top - rcParent.top;
	rect.bottom = rcSelf.top - rcParent.top + rcSelf.Height();
	rect.right = rect.left + rcSelf.Width();
	
	return rect;
}


/*
*--------------------------------------------------------------------------------
*  函数名	: RectToParent
*  功能		: 得到相对父窗口坐标的窗口边框矩形区域
*  参数		: CPoint& point	-	窗口右上角移动后的坐标
*--------------------------------------------------------------------------------
*/
CRect CFloatDibWnd::RectToParent(const CPoint& point)
{
	CRect rcSelf, rcParent, rect;
	::GetWindowRect(GetSafeHwnd(), &rcSelf);
	::GetWindowRect(m_hParentWnd, &rcParent);
	
	int nX, nY;
	nX = point.x - rcSelf.Width();
	nY = - point.y;

	// 底部和左部坐标不变
	if (point.x >= 0 && point.y <= rcSelf.Height())
	{
		// point 点在第一象限
		rect.bottom = rcSelf.top - rcParent.top + rcSelf.Height() ;
		rect.left = rcSelf.left - rcParent.left;
		rect.top = rcSelf.top - rcParent.top - nY;
		rect.right = rect.left + rcSelf.Width() + nX;
	}
	else if (point.x >= 0 && point.y > rcSelf.Height())
	{	
		// point 点在第四象限
		rect.bottom = rcSelf.top - rcParent.top + rcSelf.Height() + (point.y - rcSelf.Height());
		rect.left = rcSelf.left - rcParent.left;
		rect.top = rcSelf.top - rcParent.top + rcSelf.Height();
		rect.right = rect.left + rcSelf.Width() + nX;
	}
	else if (point.x < 0 && point.y <= rcSelf.Height())
	{
		// 点在第二象限，左边变为右边，底边不动
		rect.bottom = rcSelf.top - rcParent.top + rcSelf.Height() ;
		rect.left = rcSelf.left - rcParent.left - abs(point.x);
		rect.top = rcSelf.top - rcParent.top - nY;
		rect.right = rcSelf.left - rcParent.left ;
	}
	else if (point.x < 0 && point.y > rcSelf.Height())
	{
		// 点在第三象限，上边变下边
		rect.left = rcSelf.left - rcParent.left - abs(point.x);
		rect.right = rcSelf.left - rcParent.left;
		rect.top = rcSelf.top - rcParent.top + rcSelf.Height();
		rect.bottom = rcSelf.top - rcParent.top + rcSelf.Height() + (point.y - rcSelf.Height());		
	}	

	return rect;
}


/*
*--------------------------------------------------------------------------------
*  GetArcEndPoint
*  功能	- 计算一段弧的终点座标
*  参数
*	[in]  CPoint ptCenter		- 弧所在圆的圆心座标
*	[in]  DWORD dwRadius		- 弧所在圆的半径
*	[in]  FLOAT eStartAngle		- 弧的起始角度
*	[in]  FLOAT eSweepAngle		- 弧的扫描角度
*  返回值
*		 CPoint	 -	弧的终点座标
*  算法
*		逆时针旋转
*--------------------------------------------------------------------------------
*/
CPoint CFloatDibWnd::GetArcEndPoint(const CPoint &ptCenter, double dwRadius, double eStartAngle, double eSweepAngle) const
{
	// 角度转弧度常数
	const FLOAT piover180 = (FLOAT) 3.141592653586 / 180;

	// 确保最多只画一次圆
	if (eSweepAngle >= 360)
		eSweepAngle = 360;
	else if (eSweepAngle <= -360)
		eSweepAngle = -360;

	// 计算起始角、终止角度的弧度
	double eEndAngle = (eStartAngle + eSweepAngle) * piover180;

	// 计算出弧起始、终止点的座标
	int nXEnd, nYEnd;

	nXEnd = ptCenter.x + (int) (dwRadius * cos(eEndAngle));
	nYEnd = ptCenter.y - (int) (dwRadius * sin(eEndAngle));

	return CPoint(nXEnd, nYEnd);
}


/*
*--------------------------------------------------------------------------------
*  函数名	: PointToParentClient
*  功能		: 得到相对父窗口客户区坐标的窗口边框矩形区域
*  参数		: CPoint& point	-	窗口右上角移动后的坐标
*--------------------------------------------------------------------------------
*/
CPoint CFloatDibWnd::PointToParentClient(const CPoint &point)
{
	CRect rcSelf, rcParent;
	::GetWindowRect(GetSafeHwnd(), &rcSelf);
	::GetWindowRect(m_hParentWnd, &rcParent);

	CPoint pt;
	pt.x = rcSelf.left - rcParent.left + point.x - 2;
	pt.y = rcSelf.top - rcParent.top + point.y - 2;

	return pt;
}


//DEL BOOL CFloatDibWnd::SaveOperate()
//DEL {
//DEL 	if (m_pHistoryDib)
//DEL 		delete m_pHistoryDib;
//DEL 
//DEL 	m_pHistoryDib = m_pDib->Clone();	
//DEL 	m_HistoryChangeType = m_ChangeType;
//DEL 	m_rcHistoryWndRect = RectToParent();
//DEL 
//DEL 	return m_pHistoryDib ? TRUE: FALSE ;
//DEL } 


//DEL BOOL CFloatDibWnd::Undo()
//DEL {
//DEL 	if (!m_pHistoryDib)	
//DEL 		return FALSE;
//DEL 
//DEL 	// 将当前DIB与历史DIB互换，以支持 Redo
//DEL 	CDib* pTmpDib = m_pDib;
//DEL 	m_pDib = m_pHistoryDib;
//DEL 	m_pHistoryDib = pTmpDib;
//DEL 
//DEL 	if (m_HistoryChangeType == CT_RESIZE)
//DEL 	{
//DEL 		::MoveWindow(GetSafeHwnd(), 
//DEL 			m_rcHistoryWndRect.left,
//DEL 			m_rcHistoryWndRect.top,
//DEL 			m_rcHistoryWndRect.Width(), 
//DEL 			m_rcHistoryWndRect.Height(), 
//DEL 			FALSE);
//DEL 	}
//DEL 
//DEL 	return TRUE;
//DEL }


//DEL BOOL CFloatDibWnd::Redo()
//DEL {
//DEL 	if (!m_pHistoryDib)	
//DEL 		return FALSE;
//DEL 
//DEL 	// 将当前DIB与历史DIB互换，以支持 Redo
//DEL 	CDib* pTmpDib = m_pDib;
//DEL 	m_pDib = m_pHistoryDib;
//DEL 	m_pHistoryDib = pTmpDib;
//DEL 
//DEL 	if (m_HistoryChangeType == CT_RESIZE)
//DEL 	{
//DEL 		::MoveWindow(GetSafeHwnd(), 
//DEL 			m_rcHistoryWndRect.left,
//DEL 			m_rcHistoryWndRect.top,
//DEL 			m_rcHistoryWndRect.Width(), 
//DEL 			m_rcHistoryWndRect.Height(), 
//DEL 			FALSE);
//DEL 	}
//DEL 
//DEL 	return TRUE;
//DEL }


/*
*--------------------------------------------------------------------------------
*  函数名	: RectToParentClient
*  功能		: 得到相对父窗口客户区坐标的窗口边框矩形区域
*  返回值	: CRect	- 子窗口相对父窗口客户区的坐标
*  说明		: 是相对于父窗口的客户区，因为父窗口有两个像素的边框
*--------------------------------------------------------------------------------
*/
CRect CFloatDibWnd::RectToParentClient()
{
	CRect rect = RectToParent();

	// 因父窗口框占两个像素，故向左上平移两个像素
	rect.OffsetRect(-2, -2);
	//rect.InflateRect(-2, -2, -2, -2);

	return rect;
}


/*
*--------------------------------------------------------------------------------
*  函数名	: RectToParentClient
*  功能		: 得到相对父窗口客户区坐标的窗口边框矩形区域
*  参数		: 窗口右上角的点座标
*  返回值	: CRect	- 子窗口相对父窗口客户区的坐标
*  说明		: 是相对于父窗口的客户区，因为父窗口有两个像素的边框
*--------------------------------------------------------------------------------
*/
CRect CFloatDibWnd::RectToParentClient(const CPoint& point)
{
	CRect rect = RectToParent(point);
	rect.OffsetRect(-2, -2);

	return rect;
}


/*
*--------------------------------------------------------------------------------
*  函数名: Cut
*  功能  : 剪切浮动窗口中的图像，保存到剪贴板上
*  返回值: CDib*	-   返回一个DIB图像的指针
*  算法  : 剪切窗口中的DIB图像然后返回
*  后置条件: 销毁浮动窗口
*--------------------------------------------------------------------------------
*/
CDib* CFloatDibWnd::Cut()
{
	CDib* pRet = m_pDib->Clone();
	delete this;
	return pRet;
}


/*
*--------------------------------------------------------------------------------
*  函数名: Copy
*  功能  : 复制浮动窗口中的图像，保存到剪贴板上
*  返回值: CDib*	-   返回一个DIB图像的指针
*  算法  : 复制窗口中的DIB图像然后返回
*  后置条件: 并不销毁浮动窗口
*--------------------------------------------------------------------------------
*/
CDib* CFloatDibWnd::Copy()
{
	return m_pDib->Clone();	
}


/*
*--------------------------------------------------------------------------------
*  函数名	: RotateAngle
*  功能		: 旋转浮动窗口
*  参数		: double angle	-	旋转的角度
*--------------------------------------------------------------------------------
*/
BOOL CFloatDibWnd::RotateAngle(double angle)
{
	// 因为是对源图像进行旋转操作，所以要把上一次旋转角度与此次旋转角度叠加
	m_dblPreRotateAngle += angle;
	if (m_dblPreRotateAngle > 360)
		m_dblPreRotateAngle -= 360;
	angle = m_dblPreRotateAngle;


	//对pDib操作实际上就是对m_pDib操作
	HBITMAP *m_hBitmap ;
	CDib * pDib; 
	pDib = m_pSrcDib->Clone();
		
	CPoint cirpoint;
	CRect rect;
	GetClientRect(&rect);
	
	
	//使源DIB缩放成当前画布区大小
	pDib->ChangeImageSize(rect.Width(), rect.Height()); 
	m_pDib->ChangeImageSize(rect.Width(), rect.Height()); 
	
	//得到图形矩形区的中心点坐标值
	cirpoint.x = rect.Width()/2;
	cirpoint.y = rect.Height()/2; 
	
	
	CClientDC dc(this);
	ASSERT_VALID(&dc);	
	   
	//以下生成一相要操作的源内存设备上下文，它同目的内存上下文数据一致
    BITMAP  srcbm;
	HBITMAP * hBitmap;
	CDC  srcDC; 
	CBitmap pBitmap;
	   
	// 产生源内存设备情境对象（为避免失真总是选用最初的源DIB）
	
	srcDC.CreateCompatibleDC(NULL);  
	hBitmap=(HBITMAP *)pDib->GetBitmap()->GetSafeHandle(); 
	srcDC.SetBkMode(TRANSPARENT); 
	   
	// 将位图选入源内存设备情境对象	   
	srcDC.SelectObject(hBitmap);
	GetObject(hBitmap, sizeof(srcbm),&srcbm);
	   
	//为使旋转的源和目的设备上下文一致，必须使目的DIB使用m_pDib
	   
	CDC *  MemDC = m_pDib->BeginPaint(&dc);
	MemDC->SetBkMode(TRANSPARENT); 
	   
	int i,j;
	BITMAP m_Bm ;
	   
	m_hBitmap = (HBITMAP *)m_pDib->GetBitmap()->GetSafeHandle();  
	   
	// 将目的位图选入内存设备情境对象(此时源及目的位图数据内容一致)
	MemDC->SelectObject(m_hBitmap);
	   
	GetObject(m_hBitmap, sizeof(m_Bm),&m_Bm);
	   
	//将目的内存设备上下文件全部填充为粉红色
	MemDC->FillSolidRect(CRect(0, 0, m_pDib->GetWidth(), m_pDib->GetHeight()),g_stuPaintSet.tranCol);
	   
	CPoint point1, point2;
	   
	point1.x = m_pDib->GetWidth()/ 2;
	point1.y = m_pDib->GetHeight()/ 2; 
	   
	//开始对内存DC进行旋转操作
	   
	for (i = (int) -0.4 * m_Bm.bmWidth; i < (int) 1.4 * m_Bm.bmWidth; i++) 
		for(j = (int) -0.4 * m_Bm.bmHeight; j < (int) 1.4 * m_Bm.bmHeight; j++)
		{
			point2.x = i;
			point2.y = j;
			point2 = CountPoint(point1, point2, angle);
			if(srcDC.GetPixel(point2) != g_stuPaintSet.tranCol)
				MemDC->StretchBlt(
				cirpoint.x + i-point1.x, cirpoint.y + j-point1.y, 
				1, 1,
				&srcDC,
				point2.x, point2.y,
				1,1,
				SRCCOPY
				);
		}		
	m_pDib->EndPaint(); 

	if (pDib)
		delete pDib;
	// 缩放窗口，以与旋转过后的图像大小相匹配
/*	int nWidth, nHeight, left, top;
	nWidth = m_pDib->GetWidth();
	nHeight = m_pDib->GetHeight();
	CPoint ptCenter = RectToParentClient().CenterPoint();
	left = ptCenter.x - nWidth / 2;
	top = ptCenter.y - nHeight / 2;

	::MoveWindow(GetSafeHwnd(), left, top, nWidth, nHeight, TRUE);
*/	
		
	return TRUE;  
}


/*
*--------------------------------------------------------------------------------
*  成员函数名   :CountPoint
*  功能描述     :计算一个点pointSide相对于中心点pointCenter旋转angle角度后的点的坐标值
*  输入参数描述	:
*  CPoint pointCenter：相对于中心点坐标值
*  CPoint pointSide：  要计算的点的坐标值
*  angle：             要旋转的角度   
*  返回变量说明 :经计算后的点的坐标值
*  算法描述		:
*  附加说明     :
*--------------------------------------------------------------------------------
*/
CPoint CFloatDibWnd::CountPoint(CPoint pointCenter, CPoint pointSide, double angle)
{
	const double PAI = 3.1415926;
	double dR, dTemp;
	double dAngle;
	int iQuadrant;
	CPoint point;

	//若中心点与边框点重合则返回它本身
	if(pointCenter == pointSide)
		return pointCenter;

	//以点 pointCenter 为中心构建一个坐标系,坐标系中心就是多边形中心	
	pointSide.x = pointSide.x -pointCenter.x;
	pointSide.y = pointCenter.y - pointSide.y;
	//计算点 pointSide 与 pointCenter 所构成的直线角度
	dR = sqrt(pow(pointSide.x, 2) + pow(pointSide.y, 2));
	dAngle = atan((double) pointSide.y / pointSide.x);
	if(pointSide.x >= 0 && pointSide.y >= 0)  iQuadrant = 1;
	if(pointSide.x < 0 && pointSide.y > 0)  iQuadrant = 2;
	if(pointSide.x <= 0 && pointSide.y <= 0)  iQuadrant = 3;
	if(pointSide.x > 0 && pointSide.y < 0)  iQuadrant = 4;
	if(iQuadrant == 2 || iQuadrant == 3)  dAngle = dAngle + PAI;
	if(iQuadrant == 3 && pointSide.x == 0) dAngle = dAngle + PAI;
	if(iQuadrant == 4) 	dAngle = dAngle + 2 * PAI;
	dAngle = dAngle + angle * PAI / 180;
	if(dAngle > 2 * PAI) dAngle = dAngle - 2 * PAI;

    //取得旋转 PAI / numberSide 弧度后的那个点的坐标
	dTemp = (double)pointCenter.x + (double)(cos(dAngle) * dR);
	point.x = (long)(dTemp + 0.5);
	dTemp = (double)pointCenter.y - (double)(sin(dAngle) * dR);
	point.y = (long)(dTemp + 0.5);
    return point;  

}


/*
*--------------------------------------------------------------------------------
*  函数名	: IsCartoonWnd
*  功能		: 判断一个浮动窗口是否卡通贴图
*  返回值	: BOOL	-	是卡通贴图窗口返回 TRUE, 否则返回 FALSE
*--------------------------------------------------------------------------------
*/
BOOL CFloatDibWnd::IsCartoonWnd()
{
	return m_bCartoonWnd;
}


/*
*--------------------------------------------------------------------------------
*  函数名	: SetCreateBtn
*  功能		: 设置创建浮动窗口时是否创建四个按纽
*  返回值	: 
*--------------------------------------------------------------------------------
*/
void CFloatDibWnd::SetCreateBtn(BOOL bCreateBtn)
{ 
	if(!bCreateBtn &&  g_createBtn)
	{
      //先隐藏已经显示了的四个按纽
     	m_btnRotate.ShowWindow(SW_HIDE);    
		m_btnVerMirror.ShowWindow(SW_HIDE); 
		m_btnHorMirror.ShowWindow(SW_HIDE); 
		m_btnResize.ShowWindow(SW_HIDE);   
        SendMessage(WM_SIZE,0,0);
	}
	else if(bCreateBtn &&  !g_createBtn)
	{
       //先显示已经隐藏了的四个按纽
     	m_btnRotate.ShowWindow(SW_SHOW);    
		m_btnVerMirror.ShowWindow(SW_SHOW);
		m_btnHorMirror.ShowWindow(SW_SHOW); 
		m_btnResize.ShowWindow(SW_SHOW);   
        SendMessage(WM_SIZE,0,0);
	}
   g_createBtn = bCreateBtn;
    Invalidate(); 

}


//测试是否有必要显示按纽(当图象太小时)
void CFloatDibWnd::CheckShowBtn()
{
    if(m_rcWndRect.Width() <= MINSIZE || m_rcWndRect.Height() <= MINSIZE)
		g_createBtn = FALSE;
}
