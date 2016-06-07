
// UPIMServerView.cpp : CUPIMServerView 类的实现
//

#include "stdafx.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "UPIMServer.h"
#endif

#include "UPIMServerDoc.h"
#include "UPIMServerView.h"
#include "Func.h"
#include "globalvar.h"

// CUPIMServerView

IMPLEMENT_DYNCREATE(CUPIMServerView, CFormView)

BEGIN_MESSAGE_MAP(CUPIMServerView, CFormView)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_SIZE()
END_MESSAGE_MAP()

// CUPIMServerView 构造/析构

CUPIMServerView::CUPIMServerView()
	: CFormView(CUPIMServerView::IDD)
{
	// TODO: 在此处添加构造代码

}

CUPIMServerView::~CUPIMServerView()
{
}

void CUPIMServerView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}

BOOL CUPIMServerView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CFormView::PreCreateWindow(cs);
}

void CUPIMServerView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	GetParentFrame()->RecalcLayout();
	ResizeParentToFit();
	CRect rc;
	this->GetClientRect(&rc);
	m_TabCtrl.Create(this,WS_CHILD | WS_VISIBLE, rc,  IDC_TAB_STATISTICS);
	m_TabCtrl.SetLayout(TAB_LAYOUT_BOTTOM);

	m_DlgStatistics.Create(IDD_DLG_STATISTICS,&m_TabCtrl);
	m_DlgStatistics.SetDlgCtrlID(IDD_DLG_STATISTICS);
	m_TabCtrl.Add(m_DlgStatistics,_T("统计"),0);
	m_DlgStatistics.MoveWindow(CRect(rc.left,rc.top,rc.right-50,rc.bottom-20));

	m_TabCtrl.SetBehavior(TAB_BEHAVIOR_SCROLL);
	m_TabCtrl.ShowScrollButtons( true );

	CImageList imagelistSys;
	CBitmap bmpSys;
	imagelistSys.Create(14,14,ILC_COLOR24 | ILC_MASK,4,0);
	bmpSys.LoadBitmap(IDB_BTM_SYS);
	imagelistSys.Add(&bmpSys,RGB(255,0,255));
	// 
	m_TabCtrl.SetSystemImageList(&imagelistSys);
	m_TabCtrl.ShowMenuButton(FALSE);
	m_TabCtrl.ShowCloseButton(FALSE);
	m_TabCtrl.Update();

	InitChildCtrlPos();

}

void CUPIMServerView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CUPIMServerView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CUPIMServerView 诊断

#ifdef _DEBUG
void CUPIMServerView::AssertValid() const
{
	CFormView::AssertValid();
}

void CUPIMServerView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

CUPIMServerDoc* CUPIMServerView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CUPIMServerDoc)));
	return (CUPIMServerDoc*)m_pDocument;
}
#endif //_DEBUG



void CUPIMServerView::OnSize(UINT nType, int cx, int cy)
{
	MoveWnd(cx,cy);
}

// CUPIMServerView 消息处理程序

void CUPIMServerView::InitChildCtrlPos()
{
	//开始初始化控件大小
	m_IsInitialed = false;
	CRect m_ClientRect;
	this->GetClientRect(&m_ClientRect);
	ScreenToClient(m_ClientRect);
	CSize m_Forsize;
	m_Forsize = GetTotalSize();//在资源编辑器中定好大小后，程序运行时大小（不管最大化和最小化，该大小均为同一个值），客户区大于或等于显示的大小
	double m_x = (double)m_ClientRect.Width() / m_Forsize.cx;//宽度方向发大倍数
	double m_y = (double)m_ClientRect.Height() / m_Forsize.cy;//高度方向发大倍数
	//调整控件的大小
	CWnd *pWnd = NULL; 
	pWnd = GetWindow(GW_CHILD);
	while(pWnd)//判断是否为空，因为对话框创建时会调用此函数，而当时控件还未创建
	{
		CRect rect;   //获取控件变化前大小
		pWnd->GetWindowRect(&rect);
		ScreenToClient(&rect);//将控件大小转换为在对话框中的区域坐标
		m_ControlRect.insert(pair<int, CRect>(pWnd->GetDlgCtrlID(), rect));//保存控件的初始大小，以便在OnSize函数中继续使用
		int width = rect.Width();
		int height = rect.Height();
		CHAR szBuf[256];
		GetClassName(pWnd->m_hWnd,szBuf,256);         
		if( _tcsicmp(szBuf,_T("Static")) == 0||_tcsicmp(szBuf,_T("edit")) == 0)   
		{ 
			//Edit只是位置变化，大小没有变
			rect.top = m_y * rect.top;
			rect.left = m_x * rect.left;
			rect.bottom = rect.top + height;
			rect.right = rect.left + width;
		}
		// 		else if (wcsstr(szBuf,_T("Afx:TabWnd")))
		// 		{
		// 			rect.top = m_y * rect.top - 20;
		// 			rect.left = m_x * rect.left;
		// 			rect.bottom = m_y * rect.bottom;
		// 			rect.right = m_x * rect.right;
		// 		}
		else
		{
			//其它控件位置和大小均变化
			rect.top = m_y * rect.top;
			rect.left = m_x * rect.left;
			rect.bottom = m_y * rect.bottom;
			rect.right = m_x * rect.right;
		}
		pWnd->MoveWindow(&rect);//设置控件大小
		pWnd = pWnd->GetWindow(GW_HWNDNEXT);
	}

	//控件初始化结束
	m_IsInitialed = true;

}

void CUPIMServerView::MoveWnd(int cx,int cy)
{
	// TODO: 在此处添加消息处理程序代码
	CFormView::ShowScrollBar(SB_BOTH, false);//设置没有滚动条，视情况而定。
	//在界面不是最小化并且已经初始化完毕
	if (!IsIconic() && m_IsInitialed)
	{
		CSize m_Forsize;
		m_Forsize = GetTotalSize();
		double m_x = (double)cx / m_Forsize.cx;
		double m_y = (double)cy / m_Forsize.cy;
		//读取控件的初始大小
		map<int, CRect>::iterator pos = m_ControlRect.begin();
		for (; pos != m_ControlRect.end(); ++pos)
		{
			CRect rect = pos->second;
			int width = rect.Width();
			int height = rect.Height();
			CHAR szBuf[256];
			GetClassName(GetDlgItem(pos->first)->m_hWnd,szBuf,256);     				
			if( _tcsicmp(szBuf,_T("Static")) == 0||_tcsicmp(szBuf,_T("edit")) == 0)   
			{ 
				rect.top = m_y * rect.top;
				rect.left = m_x * rect.left;
				rect.bottom = rect.top + height;
				rect.right = rect.left + width;
			}
			// 			else if (wcsstr(szBuf,_T("Afx:TabWnd")) )
			// 			{
			// 				rect.top = m_y * rect.top -20;
			// 				rect.left = m_x * rect.left;
			// 				rect.bottom = m_y * rect.bottom;
			// 				rect.right = m_x * rect.right;
			// 
			// 			}
			else
			{
				rect.top = m_y * rect.top;
				rect.left = m_x * rect.left;
				rect.bottom = m_y * rect.bottom;
				rect.right = m_x * rect.right;
			}
			GetDlgItem(pos->first)->MoveWindow(rect);
		}
	}
}
