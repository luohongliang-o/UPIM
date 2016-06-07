
// PaintDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "..\Include\PaintDlg.h"
#include "..\Include\BitmapDialog.h"
#include "..\CatchScreenDlg.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CPaintDlg 对话框
IniPaintSet g_stuPaintSet;
extern Canvas *m_DrawRect;
CString g_strAppPath; 
const int DlgHeight = 37;
const int DlgWidth  = 247;
CPaintDlg::CPaintDlg(CWnd* pParent /*=NULL*/)
	: CBitmapDialog(CPaintDlg::IDD, pParent)
{
	m_hbrush.CreateSolidBrush(0x335B91);
	
}

void CPaintDlg::DoDataExchange(CDataExchange* pDX)
{
	CBitmapDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CPaintDlg, CBitmapDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_CTLCOLOR()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_CBN_SELCHANGE(IDC_COMBOFONT,&CPaintDlg::OnCbnSelchangeComboFont)
	ON_MESSAGE(UWM_SHOWPENCOLOR1, &CPaintDlg::OnUwmShowpencolor1)
END_MESSAGE_MAP()


// CPaintDlg 消息处理程序

BOOL CPaintDlg::OnInitDialog()
{
	CBitmapDialog::OnInitDialog();
	//CString faceFile = _T("E:\\U\\PaintDlg\\Paint\\res\\face_dlg.bmp");
	if(!SetBitmap(IDB_FACE_DLG))
		return FALSE;
	if (m_DlgType == 0 ||m_DlgType == 1||m_DlgType == 2||m_DlgType == 3)
	{
		for (int j = 0;j<3;j++)
		{
			CRect penrect(2+j+24*j,6,2+j+24*(j+1),6+24);
			m_penrect[j].CopyRect(penrect);
			m_pen[j].Create(NULL,WS_CHILD|WS_VISIBLE,penrect,this,IDC_PEN1);
		}
		m_pen[0].SetBitmap(IDB_PEN1_1);//点击后的状态图
		m_pen[1].SetBitmap(IDB_PEN2_0);
		m_pen[2].SetBitmap(IDB_PEN3_0);
		m_pen[0].SetState(1);
		m_pen[1].SetState(0);
		m_pen[2].SetState(0);
		if (m_DlgType == 2){
			m_pen[0].SetBitmap(IDB_PEN1_0);//点击后的状态图
			m_pen[1].SetBitmap(IDB_PEN2_1);
			m_pen[2].SetBitmap(IDB_PEN3_0);		
			m_pen[0].SetState(0);
			m_pen[1].SetState(1);
			m_pen[2].SetState(0);
		}
		if (m_DlgType == 3){
			m_pen[0].SetBitmap(IDB_PEN1_0);//点击后的状态图
			m_pen[1].SetBitmap(IDB_PEN2_0);
			m_pen[2].SetBitmap(IDB_PEN3_1);		
			m_pen[0].SetState(0);
			m_pen[1].SetState(0);
			m_pen[2].SetState(1);
		}
	}
	else if (m_DlgType = 4)
	{
		//faceFile = _T("E:\\U\\PaintDlg\\Paint\\res\\font_face.bmp");
		if(!SetBitmap(IDB_FONT_FACE))
			return FALSE;
		m_fontCombo.Create(WS_CHILD |CBS_DROPDOWNLIST | CBS_SORT | WS_VSCROLL|WS_VISIBLE,CRect(30,7,76,27),this,IDC_COMBOFONT);
		for (int i = 8;i<23;i++)
		{
			CString fontSize;
			fontSize.Format("%d",i);
			m_fontCombo.InsertString(i-8,fontSize);
		}
		m_fontCombo.SetCurSel(4);

	}

	CPoint cirPoint[16];
	CPoint ColorPoint(120,4);
	CRect  ColorCurSel(89,5,89+26,5+26);
	m_preSelColor.Create(NULL,WS_CHILD|WS_VISIBLE,ColorCurSel,this,IDC_PRECOLOR);

	for (int i = 0;i<16;i++)
	{
		cirPoint[i].x = ColorPoint.x + i%8*12 + i%8*3 + 6;
		cirPoint[i].y = ColorPoint.y + i/8*12 + i/8*3 + 6;
		m_rctColor[i].SetRect(cirPoint[i].x-7,cirPoint[i].y-7,cirPoint[i].x+8,cirPoint[i].y+8);
		m_SelColor[i].Create(NULL,NULL,WS_CHILD|WS_VISIBLE,m_rctColor[i],this,sel[i]);
	}

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CPaintDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
	}
	else
	{
		CBitmapDialog::OnPaint();
	}
}
void CPaintDlg::SetDlgRect(CRect rc)
{
	m_rctDlg.CopyRect(rc);
	m_rctDlg.bottom = m_rctDlg.top + DlgHeight;
	m_rctDlg.right  = m_rctDlg.left+ DlgWidth;
}
BOOL CPaintDlg::GetFontIsBold()
{
	return m_bFontBold;
}

void CPaintDlg::SetPenCol(COLORREF pencolor)
{
	CRect rect;
	m_preSelColor.GetClientRect(rect);
	CWnd * colwnd=GetDlgItem(IDC_PRECOLOR);
	CClientDC dc(colwnd);
	dc.FillSolidRect(rect,  pencolor); 
	InvalidateRect(rect, TRUE);
	//SetCanvansToolStyle();

}

void CPaintDlg::SetDlgType(int type)
{
	m_DlgType = type;
}

afx_msg LRESULT CPaintDlg::OnUwmShowpencolor1(WPARAM wParam, LPARAM lParam)
{
	CRect rect,rect1;
	m_preSelColor.GetClientRect(rect);
	rect1.CopyRect(rect);
	rect.InflateRect(-1,-1);
	CColorPen * pCurrentPen = (CColorPen*) wParam ;
	pCurrentPen->m_bSelect=FALSE; 
	CWnd * colwnd=GetDlgItem(IDC_PRECOLOR);
	CClientDC dc(colwnd);
	dc.FillSolidRect(rect1,RGB(255,255,255));
	dc.FillSolidRect(rect,  pCurrentPen->m_pencolor); 
	InvalidateRect(rect, TRUE);
	m_pencolor = pCurrentPen->m_pencolor;
	m_DrawRect->SetPenColor(m_pencolor );
	return 0;
}


HBRUSH CPaintDlg::OnCtlColor(CDC* pDC, CWnd* pWnd,UINT nCtlColor)
{

	pDC->SetBkMode(TRANSPARENT);
	return (HBRUSH)m_hbrush;
}

void CPaintDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	if (PtInRect(m_penrect[0],point))
	{
		m_pen[0].SetBitmap(IDB_PEN1_1);
		m_pen[1].SetBitmap(IDB_PEN2_1);
		m_pen[2].SetBitmap(IDB_PEN3_0);
		m_pen[0].SetState(1);
		m_pen[1].SetState(0);
		m_pen[2].SetState(0);
		m_DrawRect->SetPenSize(2);
	}
	if (PtInRect(m_penrect[1],point))
	{
		m_pen[0].SetBitmap(IDB_PEN1_0);
		m_pen[1].SetBitmap(IDB_PEN2_1);
		m_pen[2].SetBitmap(IDB_PEN3_0);
		m_pen[0].SetState(0);
		m_pen[1].SetState(1);
		m_pen[2].SetState(0);
		m_DrawRect->SetPenSize(5);
	}
	if (PtInRect(m_penrect[2],point))
	{
		m_pen[0].SetBitmap(IDB_PEN1_0);
		m_pen[1].SetBitmap(IDB_PEN2_0);
		m_pen[2].SetBitmap(IDB_PEN3_1);
		m_pen[0].SetState(0);
		m_pen[1].SetState(0);
		m_pen[2].SetState(1);
		m_DrawRect->SetPenSize(10);
	}
	for (int i = 0;i<3;i++)
	{
		InvalidateRect(m_penrect[i]);
	}

	return CBitmapDialog::OnLButtonDown(nFlags,point);
}

void CPaintDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	int penid1[3]={IDB_PEN1_1,IDB_PEN2_1,IDB_PEN3_1};
	int penid2[3]={IDB_PEN1_2,IDB_PEN2_2,IDB_PEN3_2};
	int penid0[3]={IDB_PEN1_0,IDB_PEN2_0,IDB_PEN3_0};
	for (int i = 0;i<3;i++)
	{
		//CString resName;
		if (m_pen[i].GetState()) m_pen[i].SetBitmap(penid1[i]);//resName.Format(_T("E:\\U\\PaintDlg\\Paint\\res\\pen%d_1.bmp"),i+1);
		else if (PtInRect(m_penrect[i],point)) m_pen[i].SetBitmap(penid2[i]);//resName.Format(_T("E:\\U\\PaintDlg\\Paint\\res\\pen%d_2.bmp"),i+1);
		else m_pen[i].SetBitmap(penid0[i]);//resName.Format(_T("E:\\U\\PaintDlg\\Paint\\res\\pen%d_0.bmp"),i+1);
		//m_pen[i].SetBitmap(resName);
		InvalidateRect(m_penrect[i]);
	}

	return CBitmapDialog::OnMouseMove(nFlags,point);

}

void CPaintDlg::OnCbnSelchangeComboFont()
{
	m_DrawRect->SetPenSize(m_fontCombo.GetCurSel()+8);

}