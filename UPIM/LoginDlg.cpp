// LoginDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "UPIM.h"
#include "LoginDlg.h"
#include "afxdialogex.h"
#include "UI/MainFrame.h"


// CLoginDlg 对话框

IMPLEMENT_DYNAMIC(CLoginDlg, CDialogEx)

CLoginDlg::CLoginDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CLoginDlg::IDD, pParent)
{

}

CLoginDlg::~CLoginDlg()
{
}

void CLoginDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CLoginDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CLoginDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CLoginDlg 消息处理程序


BOOL CLoginDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

// 	CLoginFrameWnd* pLoginFrame = new CLoginFrameWnd();
// 	pLoginFrame->Create(m_hWnd, _T(""), UI_WNDSTYLE_DIALOG, 0, 0, 0, 0, 0, NULL);
// 	pLoginFrame->CenterWindow();
// 	pLoginFrame->ShowModal();

// 	this->MoveWindow(0, 0, 800, 572);
// 	::SetWindowLongPtr(this->GetSafeHwnd(), GWLP_USERDATA, reinterpret_cast<LPARAM>(&m_dlgWnd));
// 	m_dlgWnd.Subclass(this->GetSafeHwnd());
// 	m_dlgWnd.LoadSkin();

// 	MainFrame* pFrame = new MainFrame();
// 	if( pFrame == NULL ) return 0;
// 	pFrame->Create(m_hWnd, _T("UPDemo"), UI_WNDSTYLE_FRAME, WS_EX_STATICEDGE | WS_EX_APPWINDOW, 0, 0, 600, 800);
// 	pFrame->CenterWindow();
// 	::ShowWindow(*pFrame, SW_SHOW);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CLoginDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnOK();
}
