// AddDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "pbSysTool.h"
#include "AddDlg.h"
#include "afxwin.h"
#include "../NewCellTypes/GridCellCheck.h"

// CAddDlg 对话框

IMPLEMENT_DYNAMIC(CAddDlg, CDialog)



CAddDlg::CAddDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAddDlg::IDD, pParent)
	,m_TitleStr("")
{
}

CAddDlg::~CAddDlg()
{
	DeleteAllCtrl();
}

void CAddDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON_TPRULE, m_BtnTpRule);
	DDX_Control(pDX, IDC_BUTTON_INCODE, m_BtnIncode);
	DDX_Control(pDX, IDC_BUTTON_CUSTOMER, m_BtnCustomer);
	DDX_Control(pDX, IDC_BUTTON_PUBLIC, m_BtnPblicInfo);
}


BEGIN_MESSAGE_MAP(CAddDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_TPRULE, &CAddDlg::OnBnClickedGetTpRule)
	ON_BN_CLICKED(IDOK, &CAddDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CAddDlg::OnBnClickedCancel)
	ON_NOTIFY(NM_CLICK, IDC_LIST_INCODE, OnGridClick1)
	ON_NOTIFY(NM_CLICK, IDC_LIST_TPRULE, OnGridClick2)
	ON_NOTIFY(NM_CLICK, IDC_GRID_PUBLIC, OnGridClick3)
	ON_CONTROL(CBN_SELCHANGE,IDC_COMBOX1,OnComboxSelChange)
	ON_BN_CLICKED(IDC_BUTTON_INCODE, &CAddDlg::OnBnClickedButtonGetIncode)
	ON_BN_CLICKED(IDC_BUTTON_CUSTOMER, &CAddDlg::OnBnClickedButtonGetCustomer)
	ON_BN_CLICKED(IDC_BUTTON_PUBLIC, &CAddDlg::OnBnClickedButtonGetPublic)
END_MESSAGE_MAP()


// CAddDlg 消息处理程序

BOOL CAddDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	SetWindowText(m_TitleStr);
	m_BtnTpRule.ShowWindow(SW_HIDE);
	m_BtnIncode.ShowWindow(SW_HIDE);
	m_BtnCustomer.ShowWindow(SW_HIDE);
	m_BtnPblicInfo.ShowWindow(SW_HIDE);
	CreateCtrl();
	return TRUE;
}


void CAddDlg::CreateCtrl()
{
	switch(m_tableType)
	{
	case PUBLIC:
		PublicAddCtrl();
		break;
	case INCODE:
		InCodeAddCtrl();
		break;
	case TP:
		TpAddCtrl();
		break;
	case PUBLIC_TP_RELATION:
		PublicRelationAddCtrl();
		break;
	case TP_INCODE_RELATION:
		TpIncodeAddCtrl();
		break;
	case DISPATCH_RELATION:
		InCodeUserAddCtrl();
		break;

	}
}

void CAddDlg::PublicAddCtrl()
{
	CRect rcClient;
	GetClientRect(&rcClient);
	int top = rcClient.top + TOPINCREMENT;
	int Left = rcClient.left + LEFTINCREMENT;

	CStatic *Static1 = new CStatic;
	CRect CTRRECT = SetCtrlPos(Left,top,88,CTRLHEIGHT);
	Static1->Create("公众号名称",WS_CHILD|WS_VISIBLE,CTRRECT,this);
	Static1->SetFont(GetFont());
	Left += 88 + HINCREMENT;
	CEdit *Edit1 = new CEdit;
	CTRRECT = SetCtrlPos(Left,top - 3,380,EDITHEIGHT);
	Edit1->Create(WS_CHILD|WS_VISIBLE|WS_TABSTOP|WS_BORDER|ES_MULTILINE|ES_AUTOVSCROLL,CTRRECT,this,IDC_EDIT1);
	Edit1->SetFont(GetFont());
	top += VINCREMENT + CTRLHEIGHT ;
	Left = rcClient.left + LEFTINCREMENT;

	CStatic *Static2 = new CStatic;
	CTRRECT = SetCtrlPos(Left,top,88,CTRLHEIGHT);
	Static2->Create("创建者",WS_CHILD|WS_VISIBLE,CTRRECT,this);
	Static2->SetFont(GetFont());
	Left += 88 + HINCREMENT;
	CEdit *Edit2 = new CEdit;
	CTRRECT = SetCtrlPos(Left,top - 3,380,EDITHEIGHT);
	Edit2->Create(WS_CHILD|WS_VISIBLE|WS_TABSTOP|WS_BORDER|ES_MULTILINE|ES_AUTOVSCROLL,CTRRECT,this,IDC_EDIT2);
	Edit2->SetFont(GetFont());
	top += VINCREMENT + CTRLHEIGHT ;
	Left = rcClient.left + LEFTINCREMENT;

	CStatic *Static3= new CStatic;
	CTRRECT = SetCtrlPos(Left,top,88,CTRLHEIGHT);
	Static3->Create("公众号签名",WS_CHILD|WS_VISIBLE,CTRRECT,this);
	Static3->SetFont(GetFont());
	Left += 88 + HINCREMENT;
	CEdit *Edit3 = new CEdit;
	CTRRECT = SetCtrlPos(Left,top - 3,380,EDITHEIGHT);
	Edit3->Create(WS_CHILD|WS_VISIBLE|WS_TABSTOP|WS_BORDER|ES_MULTILINE|ES_AUTOVSCROLL,CTRRECT,this,IDC_EDIT3);
	Edit3->SetFont(GetFont());
	top += VINCREMENT + CTRLHEIGHT ;
	Left = rcClient.left + LEFTINCREMENT;

	CStatic *Static4 = new CStatic;
	CTRRECT = SetCtrlPos(Left,top,88,CTRLHEIGHT);
	Static4->Create("公众号欢迎内容",WS_CHILD|WS_VISIBLE,CTRRECT,this);
	Static4->SetFont(GetFont());
	Left += 88 + HINCREMENT;
	CEdit *Edit4 = new CEdit;
	CTRRECT = SetCtrlPos(Left,top - 3,380,EDITHEIGHT);
	Edit4->Create(WS_CHILD|WS_VISIBLE|WS_TABSTOP|WS_BORDER|ES_MULTILINE|ES_AUTOVSCROLL,CTRRECT,this,IDC_EDIT4);
	Edit4->SetFont(GetFont());
	top += VINCREMENT + CTRLHEIGHT ;
	Left = rcClient.left + LEFTINCREMENT;

	CTRRECT = SetCtrlPos(Left,top,88,CTRLHEIGHT);
	m_BtnTpRule.MoveWindow(CTRRECT);
	m_BtnTpRule.ShowWindow(SW_SHOW);
	top += VINCREMENT + CTRLHEIGHT ;

	CGridCtrl *Grid1 = new CGridCtrl;
	CTRRECT = SetCtrlPos(Left,top,482,195);
	Grid1->Create(CTRRECT,this,IDC_LIST_TPRULE);
	Grid1->SetFont(GetFont());
	Grid1->SetFixedRowCount(1);
	Grid1->SetRowHeight(0,GRIDHEIGHT);
	Grid1->SetColumnCount(9);
	Grid1->SetListMode(TRUE);
	Grid1->SetSingleRowSelection(TRUE);
	char* HeadName[9]={"路由规则ID","路由规则类型","路由名称","起始时间","时间间隔","问题路由类型","接口路由名称","客服工号","是否删除"};
	
	for (int ncol = 0;ncol < 9;ncol++)
	{
		Grid1->SetItemText(0,ncol,HeadName[ncol]);	
	}

	m_StaticObject.push_back(Static1);
	m_StaticObject.push_back(Static2);
	m_StaticObject.push_back(Static3);
	m_StaticObject.push_back(Static4);
	m_GridObject.push_back(Grid1);
	m_EidtObject.push_back(Edit1);
	m_EidtObject.push_back(Edit2);
	m_EidtObject.push_back(Edit3);
	m_EidtObject.push_back(Edit4);
	
}

void CAddDlg::InCodeAddCtrl()
{
	CRect rcClient;
	GetClientRect(&rcClient);
	int top = rcClient.top + TOPINCREMENT;
	int Left = rcClient.left + LEFTINCREMENT;

	CStatic *Static1= new CStatic;
	CRect CTRRECT = SetCtrlPos(Left,top,88,CTRLHEIGHT);
	Static1->Create("接入码名称",WS_CHILD|WS_VISIBLE,CTRRECT,this);
	Static1->SetFont(GetFont());
	Left += 88 + HINCREMENT;
	CEdit *Edit1 = new CEdit;
	CTRRECT = SetCtrlPos(Left,top- 3,380,EDITHEIGHT);
	Edit1->Create(WS_CHILD|WS_VISIBLE|WS_TABSTOP|WS_BORDER|ES_MULTILINE|ES_AUTOVSCROLL,CTRRECT,this,IDC_EDIT1);
	Edit1->SetFont(GetFont());
	top += VINCREMENT + CTRLHEIGHT ;
	Left = rcClient.left + LEFTINCREMENT;

	CStatic *Static2 = new CStatic;
	CTRRECT = SetCtrlPos(Left,top,88,CTRLHEIGHT);
	Static2->Create("调度规则ID",WS_CHILD|WS_VISIBLE,CTRRECT,this);
	Static2->SetFont(GetFont());
	Left += 88 + HINCREMENT;
	CEdit *Edit2 = new CEdit;
	CTRRECT = SetCtrlPos(Left,top - 3,380,EDITHEIGHT);
	Edit2->Create(WS_CHILD|WS_VISIBLE|WS_TABSTOP|WS_BORDER|ES_MULTILINE|ES_AUTOVSCROLL,CTRRECT,this,IDC_EDIT2);
	Edit2->SetFont(GetFont());
	top += VINCREMENT + CTRLHEIGHT;
	Left = rcClient.left + LEFTINCREMENT;

	CStatic *Static3 = new CStatic;
	CTRRECT = SetCtrlPos(Left,top,450,CTRLHEIGHT);
	Static3->Create("调度规则 1:最大空闲时间调度 2:最短会话时间调度 3:最高优先级调度",WS_CHILD|WS_VISIBLE,CTRRECT,this);
	Static3->SetFont(GetFont());

	m_StaticObject.push_back(Static1);
	m_StaticObject.push_back(Static2);
	m_StaticObject.push_back(Static3);
	m_EidtObject.push_back(Edit1);
	m_EidtObject.push_back(Edit2);
}

void CAddDlg::TpAddCtrl()
{
	CRect rcClient;
	GetClientRect(&rcClient);
	int top = rcClient.top + TOPINCREMENT;
	int Left = rcClient.left + LEFTINCREMENT;
	CStatic *Static1= new CStatic;
	CRect CTRRECT = SetCtrlPos(Left,top ,88,CTRLHEIGHT);
	Static1->Create("路由名称",WS_CHILD|WS_VISIBLE,CTRRECT,this);
	Static1->SetFont(GetFont());
	Left += 88 + HINCREMENT;
	CEdit *Edit1 = new CEdit;
	CTRRECT = SetCtrlPos(Left,top - 3,380,EDITHEIGHT);
	Edit1->Create(WS_CHILD|WS_VISIBLE|WS_TABSTOP|WS_BORDER|ES_MULTILINE|ES_AUTOVSCROLL,CTRRECT,this,IDC_EDIT1);
	Edit1->SetFont(GetFont());
	top += CTRLHEIGHT + VINCREMENT;

	Left = rcClient.left + LEFTINCREMENT;
	CStatic *Static2 = new CStatic;
	CTRRECT = SetCtrlPos(Left ,top ,88,CTRLHEIGHT);
	Static2->Create("路由类型",WS_CHILD|WS_VISIBLE,CTRRECT,this);
	Static2->SetFont(GetFont());
	Left+=88 + HINCREMENT;
	CComboBox *Combo1 = new CComboBox;
	CTRRECT = SetCtrlPos(Left,top,380,EDITHEIGHT*5);
	Combo1->Create(WS_CHILD|WS_VISIBLE|WS_VSCROLL|WS_TABSTOP|CBS_DROPDOWNLIST,CTRRECT,this,IDC_COMBOX1);
	Combo1->SetFont(GetFont());
	Combo1->AddString("时间路由");
	Combo1->AddString("问题路由");
	Combo1->AddString("接口路由");
	Combo1->AddString("直接客服");
	//Combo1->AddString("扩展路由");
	Combo1->SetCurSel(0);
	top += CTRLHEIGHT + VINCREMENT;
	
	CStatic *Static3 = new CStatic();
	Left = rcClient.left + LEFTINCREMENT;
	CTRRECT = SetCtrlPos(Left,top ,88,CTRLHEIGHT);
	Static3->Create("起始时间",WS_CHILD|WS_VISIBLE,CTRRECT,this);
	Static3->SetFont(GetFont());
	Left += 88 + HINCREMENT;
	CDateTimeCtrl* Datetime1 = new CDateTimeCtrl;
	CTRRECT = SetCtrlPos(Left,top-3 ,90,EDITHEIGHT);
	Datetime1->Create(WS_CHILD|WS_VISIBLE|DTS_TIMEFORMAT,CTRRECT,this,IDC_DATETIMEPICKER1);
	COleVariant vtime("0:00:00");
	vtime.ChangeType(VT_DATE);
	COleDateTime oletime = vtime;
	Datetime1->SetTime(oletime);
	Left += 90 + HINCREMENT;

	CStatic *Static4 = new CStatic();
	CTRRECT = SetCtrlPos(Left,top ,55,CTRLHEIGHT);
	Static4->Create("时间间隔",WS_CHILD|WS_VISIBLE,CTRRECT,this);
	Static4->SetFont(GetFont());
	Left += 55 + HINCREMENT;
	CEdit *Edit2 = new CEdit;
	CTRRECT = SetCtrlPos(Left,top - 3,215,EDITHEIGHT);
	Edit2->Create(WS_CHILD|WS_VISIBLE|WS_TABSTOP|WS_BORDER|ES_MULTILINE|ES_AUTOVSCROLL,CTRRECT,this,IDC_EDIT1);
	Edit2->SetFont(GetFont());
	
	top += CTRLHEIGHT + VINCREMENT;
	Left = rcClient.left + LEFTINCREMENT;
	CStatic *Static5 = new CStatic();
	CTRRECT = SetCtrlPos(Left,top ,88,CTRLHEIGHT);
	Static5->Create("问题路由类型",WS_CHILD|WS_VISIBLE,CTRRECT,this);
	Static5->SetFont(GetFont());
	Left += 88 + HINCREMENT;
	CEdit *Edit3 = new CEdit;
	CTRRECT = SetCtrlPos(Left,top - 3,380,EDITHEIGHT);
	Edit3->Create(WS_CHILD|WS_VISIBLE|WS_TABSTOP|WS_BORDER|ES_MULTILINE|ES_AUTOVSCROLL,CTRRECT,this,IDC_EDIT1);
	Edit3->SetFont(GetFont());

	top += CTRLHEIGHT + VINCREMENT;
	Left = rcClient.left + LEFTINCREMENT;
	CStatic *Static6 = new CStatic();
	CTRRECT = SetCtrlPos(Left,top ,88,CTRLHEIGHT);
	Static6->Create("接口名称",WS_CHILD|WS_VISIBLE,CTRRECT,this);
	Static6->SetFont(GetFont());
	Left += 88 + HINCREMENT;
	CEdit *Edit4 = new CEdit;
	CTRRECT = SetCtrlPos(Left,top - 3,380,EDITHEIGHT);
	Edit4->Create(WS_CHILD|WS_VISIBLE|WS_TABSTOP|WS_BORDER|ES_MULTILINE|ES_AUTOVSCROLL,CTRRECT,this,IDC_EDIT1);
	Edit4->SetFont(GetFont());

	top += CTRLHEIGHT + VINCREMENT;
	Left = rcClient.left + LEFTINCREMENT;
	CStatic *Static7 = new CStatic();
	CTRRECT = SetCtrlPos(Left,top ,88,CTRLHEIGHT);
	Static7->Create("客服工号",WS_CHILD|WS_VISIBLE,CTRRECT,this);
	Static7->SetFont(GetFont());
	Left += 88 + HINCREMENT;
	CEdit *Edit5 = new CEdit;
	CTRRECT = SetCtrlPos(Left,top - 3,380,EDITHEIGHT);
	Edit5->Create(WS_CHILD|WS_VISIBLE|WS_TABSTOP|WS_BORDER|ES_MULTILINE|ES_AUTOVSCROLL,CTRRECT,this,IDC_EDIT1);
	Edit5->SetFont(GetFont());

	top += CTRLHEIGHT + VINCREMENT;
	Left = rcClient.left + LEFTINCREMENT;
	CStatic *Static8 = new CStatic();
	CTRRECT = SetCtrlPos(Left,top ,88,CTRLHEIGHT);
	Static8->Create("扩展路由",WS_CHILD|WS_VISIBLE,CTRRECT,this);
	Static8->SetFont(GetFont());
	Left += 88 + HINCREMENT;
	CEdit *Edit6 = new CEdit;
	CTRRECT = SetCtrlPos(Left,top - 3,380,EDITHEIGHT);
	Edit6->Create(WS_CHILD|WS_VISIBLE|WS_TABSTOP|WS_BORDER|ES_MULTILINE|ES_AUTOVSCROLL,CTRRECT,this,IDC_EDIT1);
	Edit6->SetFont(GetFont());

	m_StaticObject.push_back(Static1);
	m_StaticObject.push_back(Static2);
	m_StaticObject.push_back(Static3);
	m_EidtObject.push_back(Edit1);
	m_ComboObject.push_back(Combo1);

	m_StaticObject.push_back(Static4);//时间路由
	m_EidtObject.push_back(Edit2);    
	m_DatetimeObject.push_back(Datetime1);

	m_StaticObject.push_back(Static5);//问题路由
	m_EidtObject.push_back(Edit3);

	m_StaticObject.push_back(Static6); //接口路由
	m_EidtObject.push_back(Edit4);

	m_StaticObject.push_back(Static7);//直接客服路由
	m_EidtObject.push_back(Edit5);
	
	m_StaticObject.push_back(Static8);//扩展路由
	m_EidtObject.push_back(Edit6);
	
	IsShowTimeTP(TRUE);
	IsShowProblemTP(FALSE);
	IsShowInterfaceTP(FALSE);
	IsShowDirectCustomerTP(FALSE);
	IsShowExTP(FALSE);

}
void CAddDlg::PublicRelationAddCtrl()
{
	CRect rcClient;
	GetClientRect(&rcClient);
	int top = rcClient.top + TOPINCREMENT;
	int Left = rcClient.left + LEFTINCREMENT;
	CRect CTRRECT = SetCtrlPos(Left,top,88,CTRLHEIGHT);
	m_BtnPblicInfo.MoveWindow(CTRRECT);
	m_BtnPblicInfo.ShowWindow(SW_SHOW);
	top += CTRLHEIGHT + HINCREMENT;

	CGridCtrl *Grid1 = new CGridCtrl;
	CTRRECT = SetCtrlPos(Left,top,482,120);
	Grid1->Create(CTRRECT,this,IDC_GRID_PUBLIC);
	Grid1->SetFont(GetFont());
	Grid1->SetFixedRowCount(1);
	Grid1->SetRowHeight(0,GRIDHEIGHT);
	Grid1->SetColumnCount(10);
	Grid1->SetListMode(TRUE);
	char* HeadName[10]={
		"公众号ID","公众号名称","创建时间","删除时间"
		,"修改时间","是否有效","可见用户范围","创建者账号",
		"公众号标签","公众号欢迎内容"};
	for (int ncol = 0;ncol < 10;ncol++)
	{
		Grid1->SetItemText(0,ncol,HeadName[ncol]);
	}
	top += 120 + HINCREMENT;
	CTRRECT = SetCtrlPos(Left,top,88,CTRLHEIGHT);
	m_BtnTpRule.MoveWindow(CTRRECT);
	m_BtnTpRule.ShowWindow(SW_SHOW);
	top += CTRLHEIGHT + HINCREMENT;

	CGridCtrl *Grid2 = new CGridCtrl;
	CTRRECT = SetCtrlPos(Left,top,482,120);
	Grid2->Create(CTRRECT,this,IDC_LIST_TPRULE);
	Grid2->SetFixedRowCount(1);
	Grid2->SetRowHeight(0,GRIDHEIGHT);
	Grid2->SetFont(GetFont());
	Grid2->SetColumnCount(9);
	Grid2->SetListMode(TRUE);

	char* IncodeHeadName[9]={
		"路由规则ID","路由规则类型","路由名称","起始时间","时间间隔"
		,"问题路由类型","接口路由名称","客服工号","是否删除"};
	for (int ncol = 0;ncol < 9;ncol++)
	{
		Grid2->SetItemText(0,ncol,IncodeHeadName[ncol]);
	}
	m_GridObject.push_back(Grid1);
	m_GridObject.push_back(Grid2);

}

void CAddDlg::TpIncodeAddCtrl()
{
	CRect rcClient;
	GetClientRect(&rcClient);
	int top = rcClient.top + TOPINCREMENT;
	int Left = rcClient.left + LEFTINCREMENT;
	CRect CTRRECT = SetCtrlPos(Left,top,88,CTRLHEIGHT);
	m_BtnTpRule.MoveWindow(CTRRECT);
	m_BtnTpRule.ShowWindow(SW_SHOW);
	top += CTRLHEIGHT + HINCREMENT;

	CGridCtrl *Grid1 = new CGridCtrl;
	CTRRECT = SetCtrlPos(Left,top,482,120);
	Grid1->Create(CTRRECT,this,IDC_LIST_TPRULE);
	Grid1->SetFixedRowCount(1);
	Grid1->SetRowHeight(0,GRIDHEIGHT);
	Grid1->SetFont(GetFont());
	Grid1->SetColumnCount(9);
	Grid1->SetListMode(TRUE);
	char* HeadName[9]={"路由规则ID","路由规则类型","路由名称","起始时间","时间间隔","问题路由类型","接口路由名称","客服工号","是否删除"};
	for (int ncol = 0;ncol < 9;ncol++)
	{
		Grid1->SetItemText(0,ncol,HeadName[ncol]);
	}
	top += 120 + HINCREMENT;
	CTRRECT = SetCtrlPos(Left,top,88,CTRLHEIGHT);
	m_BtnIncode.MoveWindow(CTRRECT);
	m_BtnIncode.ShowWindow(SW_SHOW);
	top += CTRLHEIGHT + HINCREMENT;
	
	CGridCtrl *Grid2 = new CGridCtrl;
	CTRRECT = SetCtrlPos(Left,top,482,120);
	Grid2->Create(CTRRECT,this,IDC_LIST_INCODE);
	Grid2->SetFixedRowCount(1);
	Grid2->SetRowHeight(0,GRIDHEIGHT);
	Grid2->SetFont(GetFont());
	Grid2->SetColumnCount(3);
	Grid2->SetListMode(TRUE);

	char* IncodeHeadName[3]={"接入码ID","接入码名称","调度规则"};
	for (int ncol = 0;ncol < 3;ncol++)
	{
		Grid2->SetItemText(0,ncol,IncodeHeadName[ncol]);
	}
	m_GridObject.push_back(Grid1);
	m_GridObject.push_back(Grid2);
}

void CAddDlg::InCodeUserAddCtrl()
{
	CRect rcClient;
	GetClientRect(&rcClient);
	int top = rcClient.top + TOPINCREMENT;
	int Left = rcClient.left + LEFTINCREMENT;
	CRect CTRRECT = SetCtrlPos(Left,top,88,CTRLHEIGHT);
	m_BtnIncode.MoveWindow(CTRRECT);
	m_BtnIncode.ShowWindow(SW_SHOW);
	top += CTRLHEIGHT + HINCREMENT;

	CGridCtrl *Grid1 = new CGridCtrl;
	CTRRECT = SetCtrlPos(Left,top,482,120);
	Grid1->Create(CTRRECT,this,IDC_LIST_INCODE);
	Grid1->SetFixedRowCount(1);
	Grid1->SetRowHeight(0,GRIDHEIGHT);
	Grid1->SetFont(GetFont());
	Grid1->SetColumnCount(3);
	Grid1->SetListMode(TRUE);

	char* HeadName[3]={"接入码ID","接入码名称","调度规则"};
	for (int ncol = 0;ncol < 3;ncol++)
	{
		Grid1->SetItemText(0,ncol,HeadName[ncol]);
	}
	top += 120 + HINCREMENT;
	CTRRECT = SetCtrlPos(Left,top,88,CTRLHEIGHT);
	m_BtnCustomer.MoveWindow(CTRRECT);
	m_BtnCustomer.ShowWindow(SW_SHOW);
	top += CTRLHEIGHT + HINCREMENT;
	
	CTRRECT = SetCtrlPos(Left,top,482,120);

	CGridCtrl *Grid2 = new CGridCtrl;
	Grid2->Create(CTRRECT,this,IDC_LIST_CUSTERMER);
	Grid2->SetColumnCount(4);
	Grid2->SetFixedRowCount(1);
	Grid2->SetRowHeight(0,GRIDHEIGHT);
	Grid2->SetFont(GetFont());
	Grid2->SetListMode(TRUE);
	int nFixedHead = Grid2->GetColumnCount();
	char* CustomerHeadName[4]={"客服工号","登陆账号","客服名称","客服优先级"};
	for (int ncol = 0;ncol < nFixedHead;ncol++)
	{
		Grid2->SetItemText(0,ncol,CustomerHeadName[ncol]);
	}
	m_GridObject.push_back(Grid1);
	m_GridObject.push_back(Grid2);

}

void CAddDlg::DeleteAllCtrl()
{
	for (int i =0;i<m_StaticObject.size();i++)
	{
		if (m_StaticObject[i]) delete m_StaticObject[i];
	}
	for (int i =0;i<m_EidtObject.size();i++)
	{
		if (m_EidtObject[i]) delete m_EidtObject[i];
	}
	for (int i =0;i<m_ComboObject.size();i++)
	{
		if (m_ComboObject[i]) delete m_ComboObject[i];
	}
	for (int i = 0;i<m_DatetimeObject.size();i++)
	{
		if (m_DatetimeObject.size()) delete m_DatetimeObject[i];
	}
	for (int i = 0;i<m_GridObject.size();i++)
	{
		if (m_GridObject[i]->GetSafeHwnd()) { 
			delete m_GridObject[i];
		}
	}
}

void CAddDlg::OnBnClickedOk()
{
	CADODatabase* pDB = new CADODatabase;
	if(!ConnectDataBase(pDB)) {
		AfxMessageBox("数据库连接失败!");
		return;
	}

	if (m_tableType == PUBLIC)
	{
		CGridCtrl* Grid = (CGridCtrl*)GetDlgItem(IDC_LIST_TPRULE);
		int nCount = Grid->GetRowCount()-1;
		if (nCount<=0)
		{
			AfxMessageBox("公众号没有关联相应路由，创建失败！");
			return ;
		}

		CString szPublicName,szCreateName,szComment,szWelcomComment;
		m_EidtObject[0]->GetWindowTextA(szPublicName);
		m_EidtObject[1]->GetWindowTextA(szCreateName);
		m_EidtObject[2]->GetWindowTextA(szComment);
		m_EidtObject[3]->GetWindowTextA(szWelcomComment);
		if (szPublicName.IsEmpty()){
			AfxMessageBox("请输入公众号名称！");
			return;
		}
		if (szCreateName.IsEmpty()){
			AfxMessageBox("请输入创建者名称！");
			return;
		}
		if (szComment.IsEmpty()){
			AfxMessageBox("请输入公众号标签！");
			return;
		}
		if (szWelcomComment.IsEmpty()){
			AfxMessageBox("请输入公众号欢迎内容！");
			return;
		}
	}
	if (INCODE == m_tableType)
	{
		CString szName,szDisRule;
		m_EidtObject[0]->GetWindowText(szName);
		m_EidtObject[1]->GetWindowText(szDisRule);
		if (szName.IsEmpty())
		{
			AfxMessageBox("请输入接入码名称！");
			return;
		}
		if (szDisRule.IsEmpty())
		{
			AfxMessageBox("请输入调度规则！");
			return;
		}
	}

	int ErrorNum;
	CString ErrorMsg;
	LONG lRet = AddProc(pDB,m_tableType,ErrorNum,ErrorMsg);
	delete pDB;
	if (lRet!=eGCError_Success){AfxMessageBox(ErrorMsg);return;}
	else AfxMessageBox(GetErrorStr(lRet));
	CDialog::OnOK();
}


void CAddDlg::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码

	CDialog::OnCancel();
}

void CAddDlg::OnComboxSelChange()
{
	CComboBox* combo = (CComboBox*)GetDlgItem(IDC_COMBOX1);
	int selItem = combo->GetCurSel()+1;
	ReSetTPRuleParam();
	switch(selItem)
	{
	case TP_TIME:
		IsShowTimeTP(TRUE);
		IsShowProblemTP(FALSE);
		IsShowInterfaceTP(FALSE);
		IsShowDirectCustomerTP(FALSE);
		IsShowExTP(FALSE);
		break;
	case TP_PROBLEM:
		IsShowTimeTP(FALSE);
		IsShowProblemTP(TRUE);
		IsShowInterfaceTP(FALSE);
		IsShowDirectCustomerTP(FALSE);
		IsShowExTP(FALSE);
		break;
	case TP_INTERFACE:
		IsShowTimeTP(FALSE);
		IsShowProblemTP(FALSE);
		IsShowInterfaceTP(TRUE);
		IsShowDirectCustomerTP(FALSE);
		IsShowExTP(FALSE);
		break;
	case TP_CUSTOMERSERVICE:
		IsShowTimeTP(FALSE);
		IsShowProblemTP(FALSE);
		IsShowInterfaceTP(FALSE);
		IsShowDirectCustomerTP(TRUE);
		IsShowExTP(FALSE);
		break;
	case TP_OTHER:
		IsShowTimeTP(FALSE);
		IsShowProblemTP(FALSE);
		IsShowInterfaceTP(FALSE);
		IsShowDirectCustomerTP(FALSE);
		IsShowExTP(TRUE);
		break;
	default:
		break;
	}
}

void CAddDlg::IsShowTimeTP(BOOL bFlag)
{
	m_StaticObject[3]->EnableWindow(bFlag);
	m_EidtObject[1]->EnableWindow(bFlag);
	m_DatetimeObject[0]->EnableWindow(bFlag);
}
void CAddDlg::IsShowProblemTP(BOOL bFlag)
{
	m_StaticObject[4]->EnableWindow(bFlag);
	m_EidtObject[2]->EnableWindow(bFlag);
}
void CAddDlg::IsShowInterfaceTP(BOOL bFlag)
{
	m_StaticObject[5]->EnableWindow(bFlag);
	m_EidtObject[3]->EnableWindow(bFlag);
}
void CAddDlg::IsShowDirectCustomerTP(BOOL bFlag)
{
	m_StaticObject[6]->EnableWindow(bFlag);
	m_EidtObject[4]->EnableWindow(bFlag);
}
void CAddDlg::IsShowExTP(BOOL bFlag)
{
	m_StaticObject[7]->EnableWindow(bFlag);
	m_EidtObject[5]->EnableWindow(bFlag);
}

void CAddDlg::OnBnClickedGetTpRule()
{
	CString strText;
	int nRow = m_vtprule.size() + 1;
	CGridCtrl* Grid = (CGridCtrl*)GetDlgItem(IDC_LIST_TPRULE);
	int nCol = Grid->GetColumnCount();
	Grid->SetRowCount(nRow);
	for (int row = 1;row < nRow;row++)
	{
		for (int col = 0;col<nCol;col++){
			CString dataText;
			dataText = m_vtprule[row-1][col];
			if(col == 1) dataText = TransToTpRule(StrToInt(m_vtprule[row-1][col]));
			if (col == 3) dataText = TransIntToStrTime(StrToInt(m_vtprule[row-1][col]));
			Grid->SetItemText(row,col,dataText);
			if (col !=0) Grid->SetItemState(row,col,GVIS_READONLY);
		}
		Grid->SetCellType(row,0, RUNTIME_CLASS(CGridCellCheck));
	}
}

void CAddDlg::OnBnClickedButtonGetIncode()
{
	CString strText;
	int nRow = m_vIncode.size() + 1;
	CGridCtrl* Grid = (CGridCtrl*)GetDlgItem(IDC_LIST_INCODE);
	int nCol = Grid->GetColumnCount();
	Grid->SetRowCount(nRow);
	for (int row = 1;row < nRow;row++)
	{
		for (int col = 0;col<nCol;col++){
			CString dataText;
			dataText = m_vIncode[row-1][col];
			if (col == 2) dataText = TransToDispatch(StrToInt(m_vIncode[row-1][col]));
			Grid->SetItemText(row,col,dataText);
		}
		Grid->SetCellType(row,0, RUNTIME_CLASS(CGridCellCheck));
		Grid->SetItemState(row,1, Grid->GetItemState(row,0) | GVIS_READONLY);
		Grid->SetItemState(row,2, Grid->GetItemState(row,0) | GVIS_READONLY);
	}
}

void CAddDlg::OnBnClickedButtonGetPublic()
{
	CString strText;
	int nRow = m_vPublicInfo.size() + 1;
	CGridCtrl* Grid = (CGridCtrl*)GetDlgItem(IDC_GRID_PUBLIC);
	int nCol = Grid->GetColumnCount();
	Grid->SetRowCount(nRow);
	for (int row = 1;row < nRow;row++)
	{
		for (int col = 0;col<nCol;col++){
			CString dataText;
			dataText = m_vPublicInfo[row-1][col];
			//if (col == 2) dataText = TransToDispatch(StrToInt(m_vIncode[row-1][col]));
			Grid->SetItemText(row,col,dataText);
			if(col != 0)Grid->SetItemState(row,col, GVIS_READONLY);
		}
		Grid->SetCellType(row,0, RUNTIME_CLASS(CGridCellCheck));
	}
}

void CAddDlg::OnBnClickedButtonGetCustomer()
{
	CString strText;
	int nRow = m_vCustomer.size() + 1;
	CGridCtrl* Grid = (CGridCtrl*)GetDlgItem(IDC_LIST_CUSTERMER);
	int nCol = Grid->GetColumnCount();
	Grid->SetRowCount(nRow);

	for (int row = 1;row < nRow;row++)
	{
		for(int col = 0;col<nCol;col++){
			Grid->SetItemText(row,col,m_vCustomer[row-1][col]);
		}
		Grid->SetCellType(row,0, RUNTIME_CLASS(CGridCellCheck));
		Grid->SetItemState(row,1, Grid->GetItemState(row,0) | GVIS_READONLY);
		Grid->SetItemState(row,2, Grid->GetItemState(row,0) | GVIS_READONLY);
	}
}

LONG CAddDlg::AddProc(CADODatabase* pDb,int tabletype,int &ErrorNum,CString &ErrorMsg)
{
	LONG lRet = eGCError_Success;
	if(PUBLIC == tabletype){
		if (m_EidtObject.size()){
			CString szPublicName,szCreateName,szComment,szWelcomComment;
			m_EidtObject[0]->GetWindowText(szPublicName);
			m_EidtObject[1]->GetWindowText(szCreateName);
			m_EidtObject[2]->GetWindowText(szComment);
			m_EidtObject[3]->GetWindowText(szWelcomComment);
			CString pubID;
			lRet = AddPublicProc(pDb,szPublicName,szCreateName,szComment,szWelcomComment,pubID,ErrorNum,ErrorMsg);
			if (lRet == eGCError_Success){
				CGridCtrl* Grid = (CGridCtrl*)GetDlgItem(IDC_LIST_TPRULE);
				int nCount = Grid->GetRowCount();
				for(int row = 1 ; row <  nCount ; row++){
					CGridCellCheck* pCell = (CGridCellCheck*)Grid->GetCell(row,0);
					BOOL bChecked = pCell->GetCheck();
					if( bChecked == TRUE ){ 
						CString szTpID = Grid->GetItemText(row, 0);
						lRet = SetTpToPublic(pDb,pubID.GetBuffer(0),szTpID.GetBuffer(0),ErrorNum,ErrorMsg);
					}
				}
			}
		}
	}
	if (INCODE == tabletype){
		if (m_EidtObject.size()){
			CString szName,szDisRule;
			m_EidtObject[0]->GetWindowText(szName);
			m_EidtObject[1]->GetWindowText(szDisRule);
			lRet = AddIncode(pDb,szName,StrToInt((LPCSTR)szDisRule),ErrorNum,ErrorMsg);
		}
	}
	if (TP == tabletype){
		if (m_EidtObject.size()){
			if (!ReSetTPRuleParam()) {lRet = eGCError_Param;ErrorMsg = "存储过程参数错误";goto errorRet;}
			CString sztpytpe,szname,szstarttime,sztimeinc,szptype,sziname,szcser;
			sztpytpe.Format("%d",m_ComboObject[0]->GetCurSel()+1);
			m_EidtObject[0]->GetWindowText(szname);
			COleDateTime oleTime;
			m_DatetimeObject[0]->GetTime(oleTime);
			szstarttime = oleTime.Format("%H%M%S");
			m_EidtObject[1]->GetWindowText(sztimeinc);
			m_EidtObject[2]->GetWindowTextA(szptype);
			if (szptype.IsEmpty()){szptype.Format("%d",0);}
			m_EidtObject[3]->GetWindowTextA(sziname);
			m_EidtObject[4]->GetWindowTextA(szcser);
			lRet = AddTPRule(pDb,sztpytpe,szname,szstarttime,sztimeinc,szptype,sziname,szcser,ErrorNum,ErrorMsg);
		}
	}
	if(PUBLIC_TP_RELATION == tabletype){
		CGridCtrl* Grid1 = (CGridCtrl*)GetDlgItem(IDC_GRID_PUBLIC); 
		CGridCtrl* Grid2 = (CGridCtrl*)GetDlgItem(IDC_LIST_TPRULE); 
		int row;
		CString szPublicID,szTpID;
		std::vector< std::vector<CString> > vPara;
		for(row = 1 ; row <  Grid1->GetRowCount(); row++){
			CGridCellCheck* pCell = (CGridCellCheck*)Grid1->GetCell(row,0);
			BOOL bChecked = pCell->GetCheck();
			if( bChecked == TRUE ){ 
				szPublicID = Grid1->GetItemText(row, 0);
				break;
			}
		}
		if (szPublicID.IsEmpty()) {
			lRet = eGCError_Param;
			ErrorMsg = "请勾选公众号ID";
			goto errorRet;
		}
		for(int row = 1;row<Grid2->GetRowCount();row++){
			CGridCellCheck* pCell = (CGridCellCheck*)Grid2->GetCell(row,0);
			BOOL bChecked = pCell->GetCheck();
			if( bChecked == TRUE ){
				std::vector<CString> szParam;
				szTpID = Grid2->GetItemText(row,0);
				szParam.push_back(szPublicID);
				szParam.push_back(szTpID );
				vPara.push_back(szParam);
			}
		}
		int nParamSize = vPara.size();
		if (0 == nParamSize){
			lRet = eGCError_Param;
			ErrorMsg = "请勾选路由ID";
		}
		for(int i = 0;i<vPara.size();i++){
			lRet = SetTpToPublic(pDb,vPara[i][0].GetBuffer(0),vPara[i][1].GetBuffer(0),ErrorNum,ErrorMsg);
			if (lRet!=eGCError_Success)break;
		}
	}
	if(TP_INCODE_RELATION == tabletype){
		CGridCtrl* Grid1 = (CGridCtrl*)GetDlgItem(IDC_LIST_TPRULE); 
		CGridCtrl* Grid2 = (CGridCtrl*)GetDlgItem(IDC_LIST_INCODE); 
		int row;
		CString sztpid;
		std::vector< std::vector<CString> > vPara;
		for(row = 1 ; row <  Grid1->GetRowCount(); row++){
			CGridCellCheck* pCell = (CGridCellCheck*)Grid1->GetCell(row,0);
			BOOL bChecked = pCell->GetCheck();
			if( bChecked == TRUE ){ 
				sztpid = Grid1->GetItemText(row, 0);
				break;
			}
		}
		if (sztpid.IsEmpty()){
			lRet = eGCError_Param;
			ErrorMsg = "请勾选路由ID";
			goto errorRet;
		}
		for(row = 1 ; row <  Grid2->GetRowCount(); row++){
			CString incodeid;
			CGridCellCheck* pCell = (CGridCellCheck*)Grid2->GetCell(row,0);
			BOOL bChecked = pCell->GetCheck();
			if (bChecked == TRUE){
				incodeid = Grid2->GetItemText(row,0);
				std::vector<CString> szParam;
				szParam.push_back(sztpid);
				szParam.push_back(incodeid);
				vPara.push_back(szParam);
			}
		}
		int nParamSize = vPara.size();
		if (0 == nParamSize){
			lRet = eGCError_Param;
			ErrorMsg = "请勾选接入码ID";
		}
		for(int i = 0;i<vPara.size();i++){
			lRet = AddTpRelation(pDb,vPara[i][0] ,vPara[i][1],ErrorNum,ErrorMsg);
			if (lRet!=eGCError_Success)break;
		}
	}
	if (DISPATCH_RELATION == tabletype){
		CGridCtrl* Grid1 = (CGridCtrl*)GetDlgItem(IDC_LIST_INCODE);
		CGridCtrl* Grid2 = (CGridCtrl*)GetDlgItem(IDC_LIST_CUSTERMER);
		int nRow;
		CString szIncodeID,szdisType;
		std::vector< std::vector<CString> > vPara;
		nRow = Grid1->GetRowCount();
		for(int row = 1;row<nRow;row++){
			CGridCellCheck* pCell = (CGridCellCheck*)Grid1->GetCell(row,0);
			BOOL bCheck = pCell->GetCheck();
			if (TRUE == bCheck){
				szIncodeID = Grid1->GetItemText(row,0);
				szdisType = Grid1->GetItemText(row,2);
				break;
			}
		}
		if (szIncodeID.IsEmpty()){
			lRet = eGCError_Param;
			ErrorMsg = "请勾选接入码ID";
			goto errorRet;
		}
		nRow = Grid2->GetRowCount();
		for (int row = 0;row<nRow;row++){
			CGridCellCheck* pCell = (CGridCellCheck*)Grid2->GetCell(row,0);
			BOOL bCheck = pCell->GetCheck();
			CString id,loginname,priority;
			if (TRUE == bCheck){
				id = Grid2->GetItemText(row,0);
				loginname = Grid2->GetItemText(row,1);
				priority = Grid2->GetItemText(row,3);
				std::vector<CString> szParam;
				szParam.push_back(szIncodeID);
				szParam.push_back(id);
				szParam.push_back(loginname);
				szParam.push_back(priority);
				vPara.push_back(szParam);
				if (DISPATCH_MAX_PRIORITY == UnTransToDispatch(szdisType) 
					&& 0 == StrToInt((LPCSTR)priority)){
					CString ErrorStr = _T("");
					ErrorStr.Format("请修改客服工号为%s的优先级后再提交！",id);
					AfxMessageBox(ErrorStr);
					vPara.clear();
					lRet = eGCError_Param;
					break;
				}
			}
		}
		int nParamSize = vPara.size();
		if (0 == nParamSize){
			lRet = eGCError_Param;
			ErrorMsg = "请勾选客服ID";
		}
		for (int i =0;i<vPara.size();i++){
			lRet = AddDisRelation(pDb,vPara[i][0],vPara[i][1],vPara[i][2],vPara[i][3],ErrorNum,ErrorMsg);
			if (lRet != eGCError_Success) break;
		}
	}
	return lRet;
errorRet:
	return lRet;
}

LONG CAddDlg::AddPublicProc(CADODatabase* pDb,CString publicname,CString createname,
							CString comment,CString welcomcomment,CString& curPubID
							,int &ErrorNum,CString &ErrorMsg)
{
	LONG lRet = eGCError_Success;
	if (!pDb) return eGCError_DbCon;
	if (!pDb->IsOpen()) return eGCError_DbClose;
	CADORecordset adoRs(pDb);
	CADOCommand adoCmd(pDb,"public_add");
	CADOParameter pParamIn_publicname(CADORecordset::typeVarChar,50); 
	pParamIn_publicname.SetValue(publicname);
	adoCmd.AddParameter(&pParamIn_publicname);
	CADOParameter pParamIn_createname(CADORecordset::typeVarChar,32); 
	pParamIn_createname.SetValue(createname);
	adoCmd.AddParameter(&pParamIn_createname);
	CADOParameter pParamIn_comment(CADORecordset::typeVarChar,127); 
	pParamIn_comment.SetValue(comment);
	adoCmd.AddParameter(&pParamIn_comment);
	CADOParameter pParamIn_welcomcomment(CADORecordset::typeVarChar,127); 
	pParamIn_welcomcomment.SetValue(welcomcomment);
	adoCmd.AddParameter(&pParamIn_welcomcomment);
	do 
	{
		if (!adoRs.Execute(&adoCmd)) { lRet = eGCError_DbExe;break;}
		if(!adoRs.IsOpen()) break;
		adoRs.GetFieldValue("curpublicid",curPubID);
		adoRs.GetFieldValue("ErrorNum",ErrorNum);
		adoRs.GetFieldValue("ErroMsg",ErrorMsg);
		if (ErrorNum!=1) lRet = eGCError_DbOp;
	}while(FALSE);
	return lRet;
}

LONG CAddDlg::SetTpToPublic(CADODatabase* pDb,char* publicid,char* strtpid,int &ErrorNum,CString &ErrorMsg)
{
	LONG lRet = eGCError_Success;
	if (!pDb) return eGCError_DbCon;
	if (!pDb->IsOpen()) return eGCError_DbClose;
	CADORecordset adoRs(pDb);
	CADOCommand adoCmd(pDb,"public_add_tprelation");
	CADOParameter pParamIn_publicid(CADORecordset::typeBigInt); 
	pParamIn_publicid.SetValue(atol(publicid));
	adoCmd.AddParameter(&pParamIn_publicid);
	CADOParameter pParamIn_strtpid(CADORecordset::typeBigInt); 
	pParamIn_strtpid.SetValue(atol(strtpid));
	adoCmd.AddParameter(&pParamIn_strtpid);
	do 
	{
		if (!adoRs.Execute(&adoCmd)) { lRet = eGCError_DbExe;break;}
		if(!adoRs.IsOpen()) break;
		adoRs.GetFieldValue("ErrorNum",ErrorNum);
		adoRs.GetFieldValue("ErroMsg",ErrorMsg);
		if (ErrorNum!=1) lRet = eGCError_DbOp;
	}while(FALSE);
	return lRet;
}

void CAddDlg::SetTPRule(vContainerData Data)
{
	for (int i = 0;i<Data.size();i++){
		m_vtprule.push_back(Data[i]);
	}
}

void CAddDlg::SetIncode(vContainerData Data)
{
	for (int i = 0;i<Data.size();i++){
		m_vIncode.push_back(Data[i]);
	}
}

void CAddDlg::SetCustomer(vContainerData Data)
{
	for (int i = 0;i<Data.size();i++){
		m_vCustomer.push_back(Data[i]);
	}
}

void CAddDlg::SetPublicInfo(vContainerData Data)
{
	for (int i = 0;i<Data.size();i++){
		m_vPublicInfo.push_back(Data[i]);
	}
}

LONG CAddDlg::AddIncode(CADODatabase* pDb,CString name,int disRule,int &ErrorNum,CString &ErrorMsg)
{
	LONG lRet = eGCError_Success;
	if (!pDb) return eGCError_DbCon;
	if (!pDb->IsOpen()) return eGCError_DbClose;
	CADORecordset adoRs(pDb);
	CADOCommand adoCmd(pDb,"public_add_incode");
	CADOParameter pParamIn_name(CADORecordset::typeVarChar,50); 
	pParamIn_name.SetValue(name);
	adoCmd.AddParameter(&pParamIn_name);
	CADOParameter pParamIn_disRule(CADORecordset::typeBigInt); 
	pParamIn_disRule.SetValue(disRule);
	adoCmd.AddParameter(&pParamIn_disRule);
	do 
	{
		if (!adoRs.Execute(&adoCmd)) { lRet = eGCError_DbExe;break;}
		if(!adoRs.IsOpen()) break;
		adoRs.GetFieldValue("ErrorNum",ErrorNum);
		adoRs.GetFieldValue("ErroMsg",ErrorMsg);
		if (ErrorNum!=1) lRet = eGCError_DbOp;
	}while(FALSE);
	return lRet;
}

long CAddDlg::AddTPRule(CADODatabase* pDb,CString tptype,CString tpname,
						CString starttime, CString timeincre,CString probtype,CString iname,CString csser
						,int &ErrorNum,CString &ErrorMsg)
{
	LONG lRet = eGCError_Success;
	if (!pDb) return eGCError_DbCon;
	if (!pDb->IsOpen()) return eGCError_DbClose;
	CADORecordset adoRs(pDb);
	CADOCommand adoCmd(pDb,"public_add_tprule");
	CADOParameter pParamIn_tptype(CADORecordset::typeBigInt); 
	pParamIn_tptype.SetValue(atol(tptype));
	adoCmd.AddParameter(&pParamIn_tptype);
	CADOParameter pParamIn_tpname(CADORecordset::typeVarChar,50); 
	pParamIn_tpname.SetValue(tpname);
	adoCmd.AddParameter(&pParamIn_tpname);
	CADOParameter pParamIn_starttime(CADORecordset::typeVarChar,20); 
	pParamIn_starttime.SetValue(starttime);
	adoCmd.AddParameter(&pParamIn_starttime);
	CADOParameter pParamIn_timeincre(CADORecordset::typeVarChar,20); 
	pParamIn_timeincre.SetValue(timeincre);
	adoCmd.AddParameter(&pParamIn_timeincre);
	CADOParameter pParamIn_probtype(CADORecordset::typeSmallInt); 
	pParamIn_probtype.SetValue(StrToInt(probtype));
	adoCmd.AddParameter(&pParamIn_probtype);
	CADOParameter pParamIn_iname(CADORecordset::typeVarChar,50); 
	pParamIn_iname.SetValue(iname);
	adoCmd.AddParameter(&pParamIn_iname);
	CADOParameter pParamIn_csser(CADORecordset::typeVarChar,50); 
	pParamIn_csser.SetValue(csser);
	adoCmd.AddParameter(&pParamIn_csser);
	do 
	{
		if (!adoRs.Execute(&adoCmd)) { lRet = eGCError_DbExe;break;}
		if(!adoRs.IsOpen()) break;
		adoRs.GetFieldValue("ErrorNum",ErrorNum);
		adoRs.GetFieldValue("ErroMsg",ErrorMsg);
		if (ErrorNum!=1) lRet = eGCError_DbOp;
	}while(FALSE);
	return lRet;

}

BOOL CAddDlg::ReSetTPRuleParam()
{
	BOOL bRet = FALSE;
	CComboBox* combo = (CComboBox*)GetDlgItem(IDC_COMBOX1);
	int selItem = combo->GetCurSel()+1;
	CString strData;
	if(selItem == TP_TIME){
		m_EidtObject[1]->GetWindowTextA(strData);
		m_EidtObject[2]->SetWindowText("");
		m_EidtObject[3]->SetWindowText("");
		m_EidtObject[4]->SetWindowText("");
	}
	else if(selItem == TP_PROBLEM){
		COleVariant vtime("0:00:00");
		vtime.ChangeType(VT_DATE);
		COleDateTime oletime = vtime;
		m_DatetimeObject[0]->SetTime(oletime);
		m_EidtObject[1]->SetWindowText("");
		m_EidtObject[2]->GetWindowTextA(strData);
		m_EidtObject[3]->SetWindowText("");
		m_EidtObject[4]->SetWindowText("");
	}
	else if (selItem == TP_INTERFACE){
		COleVariant vtime("0:00:00");
		vtime.ChangeType(VT_DATE);
		COleDateTime oletime = vtime;
		m_DatetimeObject[0]->SetTime(oletime);
		m_EidtObject[1]->SetWindowText("");
		m_EidtObject[2]->SetWindowText("");
		m_EidtObject[3]->GetWindowTextA(strData);
		m_EidtObject[4]->SetWindowText("");
	}
	else if (selItem == TP_CUSTOMERSERVICE){
		COleVariant vtime("0:00:00");
		vtime.ChangeType(VT_DATE);
		COleDateTime oletime = vtime;
		m_DatetimeObject[0]->SetTime(oletime);
		m_EidtObject[1]->SetWindowText("");
		m_EidtObject[2]->SetWindowText("");
		m_EidtObject[3]->SetWindowText("");
		m_EidtObject[4]->GetWindowTextA(strData);
	}
	else 
		m_EidtObject[5]->EnableWindow(FALSE);
	if (!strData.IsEmpty())
	{
		bRet = TRUE;
	}
	return bRet;
}

LONG CAddDlg::AddTpRelation(CADODatabase* pDb,CString tpid,CString incodeid,int &ErrorNum,CString &ErrorMsg)
{
	LONG lRet = eGCError_Success;
	if (!pDb) return eGCError_DbCon;
	if (!pDb->IsOpen()) return eGCError_DbClose;
	CADORecordset adoRs(pDb);
	CADOCommand adoCmd(pDb,"public_add_tpincoderelation");
	CADOParameter pParamIn_tpid(CADORecordset::typeBigInt); 
	pParamIn_tpid.SetValue(atol(tpid));
	adoCmd.AddParameter(&pParamIn_tpid);
	CADOParameter pParamIn_incodeid(CADORecordset::typeBigInt); 
	pParamIn_incodeid.SetValue(atol(incodeid));
	adoCmd.AddParameter(&pParamIn_incodeid);
	do 
	{
		if (!adoRs.Execute(&adoCmd)) { lRet = eGCError_DbExe;break;}
		if(!adoRs.IsOpen()) break;
		adoRs.GetFieldValue("ErrorNum",ErrorNum);
		adoRs.GetFieldValue("ErroMsg",ErrorMsg);
		if (ErrorNum!=1) lRet = eGCError_DbOp;
	}while(FALSE);
	return lRet;
}

void CAddDlg::OnGridClick1(NMHDR *pNotifyStruct, LRESULT* /*pResult*/)
{
	if (DISPATCH_RELATION == m_tableType){
		NM_GRIDVIEW* pItem = (NM_GRIDVIEW*) pNotifyStruct;
		CGridCtrl* Grid = (CGridCtrl*)GetDlgItem(IDC_LIST_INCODE);
		CGridCellCheck* pCurCell = (CGridCellCheck*)Grid->GetCell(pItem->iRow,pItem->iColumn);
		BOOL bCheck = FALSE;
		if (pCurCell) bCheck = pCurCell->GetCheck();
		int nCurcol = pItem->iColumn;
		int nRow = Grid->GetRowCount();
		for (int row = 1;row < nRow; row++)
		{
			if (row!=pItem->iRow && bCheck && pItem->iRow>=1)
			{
				CGridCellCheck* pCell = (CGridCellCheck*)Grid->GetCell(row,nCurcol);
				pCell->SetCheck(!bCheck);
			}
		}
	}	
}

void CAddDlg::OnGridClick2(NMHDR *pNotifyStruct, LRESULT* /*pResult*/)
{
	if (TP_INCODE_RELATION == m_tableType){
		NM_GRIDVIEW* pItem = (NM_GRIDVIEW*) pNotifyStruct;
		CGridCtrl* Grid = (CGridCtrl*)GetDlgItem(IDC_LIST_TPRULE);
		CGridCellCheck* pCurCell = (CGridCellCheck*)Grid->GetCell(pItem->iRow,pItem->iColumn);
		BOOL bCheck = FALSE;
		if (pCurCell) bCheck = pCurCell->GetCheck();
		int nCurcol = pItem->iColumn;
		int nRow = Grid->GetRowCount();
		for (int row = 1;row < nRow; row++)
		{
			if (row!=pItem->iRow && bCheck && pItem->iRow>=1)
			{
				CGridCellCheck* pCell = (CGridCellCheck*)Grid->GetCell(row,nCurcol);
				pCell->SetCheck(!bCheck);
			}
		}
	}
}

void CAddDlg::OnGridClick3(NMHDR *pNotifyStruct, LRESULT* /*pResult*/)
{
	if(PUBLIC_TP_RELATION == m_tableType){
		NM_GRIDVIEW* pItem = (NM_GRIDVIEW*) pNotifyStruct;
		CGridCtrl* Grid = (CGridCtrl*)GetDlgItem(IDC_GRID_PUBLIC);
		if(Grid->GetFocus()){
			CGridCellCheck* pCurCell = (CGridCellCheck*)Grid->GetCell(pItem->iRow,pItem->iColumn);
			BOOL bCheck = FALSE;
			if (pCurCell) bCheck = pCurCell->GetCheck();
			int nCurcol = pItem->iColumn;
			int nRow = Grid->GetRowCount();
			for (int row = 1;row < nRow; row++)
			{
				if (row!=pItem->iRow && bCheck && pItem->iRow>=1)
				{
					CGridCellCheck* pCell = (CGridCellCheck*)Grid->GetCell(row,nCurcol);
					pCell->SetCheck(!bCheck);
				}
			}
		}
	}
}

LONG CAddDlg::AddDisRelation(CADODatabase* pDb,CString incodeid,CString id,CString loginname,CString priority,int &ErrorNum,CString &ErrorMsg)
{
	LONG lRet = eGCError_Success;
	if (!pDb) return eGCError_DbCon;
	if (!pDb->IsOpen()) return eGCError_DbClose;
	CADORecordset adoRs(pDb);
	CADOCommand adoCmd(pDb,"public_add_dispatchrelation");
	CADOParameter pParamIn_incodeid(CADORecordset::typeBigInt); 
	pParamIn_incodeid.SetValue(atol(incodeid));
	adoCmd.AddParameter(&pParamIn_incodeid);
	CADOParameter pParamIn_id(CADORecordset::typeBigInt); 
	pParamIn_id.SetValue(atol(id));
	adoCmd.AddParameter(&pParamIn_id);
	CADOParameter pParamIn_loginname(CADORecordset::typeVarChar,50); 
	pParamIn_loginname.SetValue(loginname);
	adoCmd.AddParameter(&pParamIn_loginname);
	CADOParameter pParamIn_priority(CADORecordset::typeBigInt); 
	pParamIn_priority.SetValue(atol(priority));
	adoCmd.AddParameter(&pParamIn_priority);
	do 
	{
		if (!adoRs.Execute(&adoCmd)) { lRet = eGCError_DbExe;break;}
		if(!adoRs.IsOpen()) break;
		adoRs.GetFieldValue("ErrorNum",ErrorNum);
		adoRs.GetFieldValue("ErroMsg",ErrorMsg);
		if (ErrorNum!=1) lRet = eGCError_DbOp;		
	}while(FALSE);
	return lRet;
}

