#include "StdAfx.h"
#include "Dlg.h"
#include "pbSysTool.h"

CDlg::CDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(-1,pParent)
{
	
}


CDlg::~CDlg(void)
{
}

void CDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}	

BEGIN_MESSAGE_MAP(CDlg, CDialogEx)
END_MESSAGE_MAP()

BOOL CDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	SetWindowText(m_TitleStr);
	return TRUE;
}
