
// TestMDIView.cpp : CTestMDIView 类的实现
//

#include "stdafx.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "TestMDI.h"
#endif

#include "TestMDIDoc.h"
#include "TestMDIView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CTestMDIView

IMPLEMENT_DYNCREATE(CTestMDIView, CView)

BEGIN_MESSAGE_MAP(CTestMDIView, CView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CTestMDIView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_SIZE()
	ON_WM_CLOSE()
	ON_WM_DESTROY()
END_MESSAGE_MAP()

// CTestMDIView 构造/析构

CTestMDIView::CTestMDIView()
{
	// TODO: 在此处添加构造代码

}

CTestMDIView::~CTestMDIView()
{
}

BOOL CTestMDIView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// CTestMDIView 绘制

void CTestMDIView::OnDraw(CDC* /*pDC*/)
{
	CTestMDIDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 在此处为本机数据添加绘制代码
}


// CTestMDIView 打印


void CTestMDIView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

void CTestMDIView::OnInitialUpdate()
{
	if (g_infoDll.m_pUEXP_Init("", "", ""))
	{
		g_infoDll.m_pUEXP_SetLoginType(3, m_hWnd, UM_IMFEEDBACKMSG, IMFLAG_CHILD|IMFLAG_NOHQ, 0);
		char szKhh[51]={0}, szYyb[51]={0}, szSSOCode[255]={0};
		strcpy_s(szKhh, "069000003496");
		g_infoDll.m_pUEXP_Login(2, szKhh, szYyb, szSSOCode, 0);

		g_infoDll.m_pUEXP_ChangeChat(1143);
	}

	return;
}

BOOL CTestMDIView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CTestMDIView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void CTestMDIView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清理过程
}

void CTestMDIView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CTestMDIView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CTestMDIView 诊断

#ifdef _DEBUG
void CTestMDIView::AssertValid() const
{
	CView::AssertValid();
}

void CTestMDIView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CTestMDIDoc* CTestMDIView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CTestMDIDoc)));
	return (CTestMDIDoc*)m_pDocument;
}
#endif //_DEBUG


// CTestMDIView 消息处理程序


void CTestMDIView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);
	g_infoDll.m_pUEXP_Resize();
	
}


void CTestMDIView::OnClose()
{
	g_infoDll.m_pUEXP_Uninit();
	CView::OnClose();
}


void CTestMDIView::OnDestroy()
{
	CView::OnDestroy();

	g_infoDll.m_pUEXP_Uninit();
}
