// ColorStatic.cpp : implementation file
//

#include "stdafx.h"
//#include "..\erecord.h"
#include "..\Include\ColorStatic.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CColorStatic

CColorStatic::CColorStatic()
{
	m_TextColor=RGB(0,0,0);
	m_uFormat = DT_LEFT;
}

CColorStatic::~CColorStatic()
{
}


BEGIN_MESSAGE_MAP(CColorStatic, CStatic)
	//{{AFX_MSG_MAP(CColorStatic)
	ON_WM_CREATE()
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CColorStatic message handlers



int CColorStatic::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CStatic::OnCreate(lpCreateStruct) == -1)
		return -1;

	return 0;
}

void CColorStatic::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	CRect rect;
	GetClientRect(&rect);
	dc.SetBkColor(m_BackColor);
	dc.SetBkMode(TRANSPARENT);
	CFont *pFont=GetParent()->GetFont();
	CFont *pOldFont;
	pOldFont=dc.SelectObject(pFont);
	dc.SetTextColor(m_TextColor);
	dc.DrawText(m_strCaption,&rect,m_uFormat);
	dc.SelectObject(pOldFont);

	// Do not call CStatic::OnPaint() for painting messages
}

// void CColorStatic::Create(CString strCaption, COLORREF BackColor)
// {
// 	
// 	
// }

void CColorStatic::SetCaption(CString strCaption)
{
	m_strCaption=strCaption;
}

void CColorStatic::SetBackColor(COLORREF BackColor)
{
	m_BackColor=BackColor;
}

void CColorStatic::SetTextColor(COLORREF TextColor)
{
	m_TextColor=TextColor;
}

void CColorStatic::SetText(CString text, COLORREF  fonCol, COLORREF bkCol, UINT uFormat)
{
	SetCaption(text);
	SetTextColor(fonCol); 
	SetBackColor(bkCol);
    m_uFormat = uFormat;
}


void CColorStatic::SetColor(COLORREF FonCol, COLORREF bkCol, UINT uFormat)
{
	SetTextColor(FonCol); 
	SetBackColor(bkCol);
    m_uFormat = uFormat;
}

void CColorStatic::PreSubclassWindow() 
{
		
   GetWindowText(m_strCaption); 
	CStatic::PreSubclassWindow();
}
