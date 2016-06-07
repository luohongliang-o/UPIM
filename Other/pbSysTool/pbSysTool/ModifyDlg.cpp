// ModifyDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "pbSysTool.h"
#include "ModifyDlg.h"
#include "afxwin.h"
#include "../NewCellTypes/GridCellCheck.h"

// CModifyDlg 对话框

IMPLEMENT_DYNAMIC(CModifyDlg, CDialog)

CModifyDlg::CModifyDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CModifyDlg::IDD, pParent)
{
	
}

CModifyDlg::~CModifyDlg()
{
	DeleteAllCtrl();
}

void CModifyDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}	


BEGIN_MESSAGE_MAP(CModifyDlg, CDialog)
	ON_CONTROL(CBN_SELCHANGE,IDC_COMBOX1,OnComboxSelChange)
	ON_BN_CLICKED(IDOK, &CModifyDlg::OnBnClickedOk)
END_MESSAGE_MAP()

BOOL CModifyDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	SetWindowText(m_TitleStr);
	CreateCtrl();
	return TRUE;
}
void CModifyDlg::CreateCtrl()
{
	CRect rcClient;
	GetClientRect(&rcClient);
	int top = rcClient.top + TOPINCREMENT;
	int Left = rcClient.left + LEFTINCREMENT;
	CRect CTRRECT;

	if (PUBLIC == m_tableType || INCODE == m_tableType){
		for (int i = 0;i < m_vHead.size();i++){
			CStatic *Static = new CStatic;
			CTRRECT = SetCtrlPos(Left,top,88,CTRLHEIGHT);
			Static->Create(m_vHead[i],WS_CHILD|WS_VISIBLE,CTRRECT,this);
			Static->SetFont(GetFont());
			Left += 88 + HINCREMENT;
			CEdit *Edit = new CEdit;
			CTRRECT = SetCtrlPos(Left,top - 3,380,EDITHEIGHT);
			Edit->Create(WS_CHILD|WS_VISIBLE|WS_TABSTOP|WS_BORDER|ES_MULTILINE|ES_AUTOVSCROLL,CTRRECT,this,IDC_MODIFY_EDIT);
			Edit->SetFont(GetFont());
			Edit->SetWindowText(m_vData[i]);
			top += VINCREMENT + CTRLHEIGHT ;
			Left = rcClient.left + LEFTINCREMENT;

			if (i == 0){
				Static->EnableWindow(FALSE);
				Edit->EnableWindow(FALSE);
			}
			if (i == m_vHead.size()-1 && INCODE == m_tableType)
			{
				CStatic *Static1 = new CStatic;
				CTRRECT = SetCtrlPos(Left,top,450,CTRLHEIGHT);
				Static1->Create("调度规则 1:最大空闲时间调度 2:最短会话时间调度 3:最高优先级调度",WS_CHILD|WS_VISIBLE,CTRRECT,this);
				Static1->SetFont(GetFont());
				m_StaticObject.push_back(Static1);
			}
			m_StaticObject.push_back(Static);
			m_EidtObject.push_back(Edit);
		}
	}
	if (TP == m_tableType) SetTPRuleCtrl();
}
void CModifyDlg::SetCtrlData(vString Head,vString Data)
{
	for (int i = 0;i < Head.size(); i++){
		m_vHead.push_back(Head[i]);
	}
	for (int i = 0;i < Data.size(); i++){
		m_vData.push_back(Data[i]);
	}
}

void CModifyDlg::DeleteAllCtrl()
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
}

void CModifyDlg::SetTPRuleCtrl()
{
	CRect rcClient;
	GetClientRect(&rcClient);
	int top = rcClient.top + TOPINCREMENT;
	int Left = rcClient.left + LEFTINCREMENT;

	CStatic *Static0= new CStatic;
	CRect CTRRECT = SetCtrlPos(Left,top ,88,CTRLHEIGHT);
	Static0->Create("路由规则ID",WS_CHILD|WS_VISIBLE,CTRRECT,this);
	Static0->EnableWindow(FALSE);
	Static0->SetFont(GetFont());
	Left += 88 + HINCREMENT;
	CEdit *Edit0 = new CEdit;
	CTRRECT = SetCtrlPos(Left,top - 3,380,EDITHEIGHT);
	Edit0->Create(WS_CHILD|WS_VISIBLE|WS_TABSTOP|WS_BORDER|ES_MULTILINE|ES_AUTOVSCROLL,CTRRECT,this,IDC_EDIT1);
	Edit0->EnableWindow(FALSE);
	Edit0->SetFont(GetFont());
	Edit0->SetWindowText(m_vData[0]);
	top += CTRLHEIGHT + VINCREMENT;
	Left = rcClient.left + LEFTINCREMENT;

	CStatic *Static1= new CStatic;
	CTRRECT = SetCtrlPos(Left,top ,88,CTRLHEIGHT);
	Static1->Create("路由名称",WS_CHILD|WS_VISIBLE,CTRRECT,this);
	Static1->SetFont(GetFont());
	Left += 88 + HINCREMENT;
	CEdit *Edit1 = new CEdit;
	CTRRECT = SetCtrlPos(Left,top - 3,380,EDITHEIGHT);
	Edit1->Create(WS_CHILD|WS_VISIBLE|WS_TABSTOP|WS_BORDER|ES_MULTILINE|ES_AUTOVSCROLL,CTRRECT,this,IDC_EDIT1);
	Edit1->SetFont(GetFont());
	Edit1->SetWindowText(m_vData[2]);
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
	int cursel = UnTransToTpRule(m_vData[1]) - 1;
	Combo1->SetCurSel(cursel);
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
	if(m_vData[3].IsEmpty())m_vData[3] ="0:0:0";
	COleVariant vtime(m_vData[3]);
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
	Edit2->SetWindowText(m_vData[4]);

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
	Edit3->SetWindowText(m_vData[5]);

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
	Edit4->SetWindowText(m_vData[6]);

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
	Edit5->SetWindowText(m_vData[7]);

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

	m_StaticObject.push_back(Static0);
	m_EidtObject.push_back(Edit0);

	IsShowCtrl();

}


void CModifyDlg::IsShowTimeTP(BOOL bFlag)
{
	m_StaticObject[3]->EnableWindow(bFlag);
	m_EidtObject[1]->EnableWindow(bFlag);
	m_DatetimeObject[0]->EnableWindow(bFlag);
}
void CModifyDlg::IsShowProblemTP(BOOL bFlag)
{
	m_StaticObject[4]->EnableWindow(bFlag);
	m_EidtObject[2]->EnableWindow(bFlag);
}
void CModifyDlg::IsShowInterfaceTP(BOOL bFlag)
{
	m_StaticObject[5]->EnableWindow(bFlag);
	m_EidtObject[3]->EnableWindow(bFlag);
}
void CModifyDlg::IsShowDirectCustomerTP(BOOL bFlag)
{
	m_StaticObject[6]->EnableWindow(bFlag);
	m_EidtObject[4]->EnableWindow(bFlag);
}

BOOL CModifyDlg::ReSetTPRuleParam()
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

void CModifyDlg::IsShowCtrl()
{
	CComboBox* combo = (CComboBox*)GetDlgItem(IDC_COMBOX1);
	int selItem = combo->GetCurSel()+1;
	switch(selItem)
	{
	case TP_TIME:
		IsShowTimeTP(TRUE);
		IsShowProblemTP(FALSE);
		IsShowInterfaceTP(FALSE);
		IsShowDirectCustomerTP(FALSE);
		//IsShowExTP(FALSE);
		break;
	case TP_PROBLEM:
		IsShowTimeTP(FALSE);
		IsShowProblemTP(TRUE);
		IsShowInterfaceTP(FALSE);
		IsShowDirectCustomerTP(FALSE);
		//IsShowExTP(FALSE);
		break;
	case TP_INTERFACE:
		IsShowTimeTP(FALSE);
		IsShowProblemTP(FALSE);
		IsShowInterfaceTP(TRUE);
		IsShowDirectCustomerTP(FALSE);
		//IsShowExTP(FALSE);
		break;
	case TP_CUSTOMERSERVICE:
		IsShowTimeTP(FALSE);
		IsShowProblemTP(FALSE);
		IsShowInterfaceTP(FALSE);
		IsShowDirectCustomerTP(TRUE);
		//IsShowExTP(FALSE);
		break;
	case TP_OTHER:
		IsShowTimeTP(FALSE);
		IsShowProblemTP(FALSE);
		IsShowInterfaceTP(FALSE);
		IsShowDirectCustomerTP(FALSE);
		//IsShowExTP(TRUE);
		break;
	default:
		break;
	}
}

LONG CModifyDlg::ModifyProc(CADODatabase* pDb,int tabletype,int &ErrorNum,CString &ErrorMsg)
{
	LONG lRet = eGCError_Success;
	if (PUBLIC == tabletype){
		if (m_EidtObject.size()){
			CString id,name,szComment,szWelcomComment;
			m_EidtObject[0]->GetWindowText(id);
			m_EidtObject[1]->GetWindowText(name);
			m_EidtObject[2]->GetWindowText(szComment);
			m_EidtObject[3]->GetWindowText(szWelcomComment);
			lRet = ModifyPublic(pDb,id,name,szComment,szWelcomComment,ErrorNum,ErrorMsg);
		}	
	}
	if (INCODE == tabletype){
		if (m_EidtObject.size()){
			CString szID,szName,szDisRule;
			m_EidtObject[0]->GetWindowText(szID);
			m_EidtObject[1]->GetWindowText(szName);
			m_EidtObject[2]->GetWindowText(szDisRule);
			lRet = ModifyIncode(pDb,szID,szName,szDisRule,ErrorNum,ErrorMsg);
		}
	}
	if (TP == tabletype) {
		if (m_EidtObject.size()){
			if (!ReSetTPRuleParam()) {lRet = eGCError_Param;ErrorMsg = "存储过程参数错误";goto errorRet;}
			CString szid,sztpytpe,szname,szstarttime,sztimeinc,szptype,sziname,szcser;
			m_EidtObject[5]->GetWindowTextA(szid);
			sztpytpe.Format("%d",m_ComboObject[0]->GetCurSel()+1);
			m_EidtObject[0]->GetWindowText(szname);
			COleDateTime oleTime;
			m_DatetimeObject[0]->GetTime(oleTime);
			szstarttime = oleTime.Format("%H%M%S");
			m_EidtObject[1]->GetWindowTextA(sztimeinc);
			m_EidtObject[2]->GetWindowTextA(szptype);
			if (szptype.IsEmpty()){szptype.Format("%d",0);}
			m_EidtObject[3]->GetWindowTextA(sziname);
			m_EidtObject[4]->GetWindowTextA(szcser);
			lRet = ModifyTPRule(pDb,szid,szname,sztpytpe,szstarttime,sztimeinc,szptype,sziname,szcser,ErrorNum,ErrorMsg);
		}
	}
	return lRet;
errorRet:
	return lRet;
}

LONG CModifyDlg::ModifyPublic(CADODatabase* pDb,CString id,CString name,CString comment,
	CString welcontent,int &ErrorNum,CString &ErrorMsg)
{
	LONG lRet = eGCError_Success;
	if (!pDb) return eGCError_DbCon;
	if (!pDb->IsOpen()) return eGCError_DbClose;
	CADORecordset adoRs(pDb);
	CADOCommand adoCmd(pDb,"public_mod");
	CADOParameter pParamIn_id(CADORecordset::typeBigInt); 
	pParamIn_id.SetValue(atol((LPCSTR)id));
	adoCmd.AddParameter(&pParamIn_id);
	CADOParameter pParamIn_name(CADORecordset::typeVarChar,32); 
	pParamIn_name.SetValue(name);
	adoCmd.AddParameter(&pParamIn_name);
	CADOParameter pParamIn_comment(CADORecordset::typeVarChar,127); 
	pParamIn_comment.SetValue(comment);
	adoCmd.AddParameter(&pParamIn_comment);
	CADOParameter pParamIn_welcontent(CADORecordset::typeVarChar,127); 
	pParamIn_welcontent.SetValue(welcontent);
	adoCmd.AddParameter(&pParamIn_welcontent);
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


LONG CModifyDlg::ModifyIncode(CADODatabase* pDb,CString id,CString name,CString disRule,int &ErrorNum,CString &ErrorMsg)
{
	LONG lRet = eGCError_Success;
	if (!pDb) return eGCError_DbCon;
	if (!pDb->IsOpen()) return eGCError_DbClose;
	CADORecordset adoRs(pDb);
	CADOCommand adoCmd(pDb,"public_mod_incode");
	CADOParameter pParamIn_id(CADORecordset::typeBigInt); 
	pParamIn_id.SetValue(atol(id));
	adoCmd.AddParameter(&pParamIn_id);
	CADOParameter pParamIn_name(CADORecordset::typeVarChar,50); 
	pParamIn_name.SetValue(name);
	adoCmd.AddParameter(&pParamIn_name);
	CADOParameter pParamIn_disRule(CADORecordset::typeBigInt); 
	pParamIn_disRule.SetValue(atol(disRule));
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

LONG CModifyDlg::ModifyTPRule(CADODatabase* pDb,CString id,CString tpname,CString tptype,
	CString starttime, CString timeincre,CString probtype,CString iname,CString csser
	,int &ErrorNum,CString &ErrorMsg)
{
	LONG lRet = eGCError_Success;
	if (!pDb) return eGCError_DbCon;
	if (!pDb->IsOpen()) return eGCError_DbClose;
	CADORecordset adoRs(pDb);
	CADOCommand adoCmd(pDb,"public_mod_tprule");
	CADOParameter pParamIn_id(CADORecordset::typeBigInt); 
	pParamIn_id.SetValue(atol(id));
	adoCmd.AddParameter(&pParamIn_id);

	CADOParameter pParamIn_tpname(CADORecordset::typeVarChar,50); 
	pParamIn_tpname.SetValue(tpname);
	adoCmd.AddParameter(&pParamIn_tpname);

	CADOParameter pParamIn_tptype(CADORecordset::typeBigInt); 
	pParamIn_tptype.SetValue(atol(tptype));
	adoCmd.AddParameter(&pParamIn_tptype);

	CADOParameter pParamIn_starttime(CADORecordset::typeBigInt); 
	pParamIn_starttime.SetValue(atol(starttime));
	adoCmd.AddParameter(&pParamIn_starttime);

	CADOParameter pParamIn_timeincre(CADORecordset::typeBigInt); 
	pParamIn_timeincre.SetValue(atol(timeincre));
	adoCmd.AddParameter(&pParamIn_timeincre);

	CADOParameter pParamIn_probtype(CADORecordset::typeSmallInt); 
	pParamIn_probtype.SetValue(StrToInt(probtype));
	adoCmd.AddParameter(&pParamIn_probtype);

	CADOParameter pParamIn_iname(CADORecordset::typeVarChar,50); 
	pParamIn_iname.SetValue(iname);
	adoCmd.AddParameter(&pParamIn_iname);

	CADOParameter pParamIn_csser(CADORecordset::typeVarChar,32); 
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

// CModifyDlg 消息处理程序
void CModifyDlg::OnComboxSelChange()
{
	ReSetTPRuleParam();
	IsShowCtrl();	
}



void CModifyDlg::OnBnClickedOk()
{
	CADODatabase* pDB = new CADODatabase;
	if(!ConnectDataBase(pDB)) {
		AfxMessageBox("数据库连接失败!");
		return;
	}
	if (PUBLIC == m_tableType){
		CString id,name,szComment,szWelcomComment;
		m_EidtObject[0]->GetWindowTextA(id);
		m_EidtObject[1]->GetWindowTextA(name);
		m_EidtObject[2]->GetWindowTextA(szComment);
		m_EidtObject[3]->GetWindowTextA(szWelcomComment);
		if (id.IsEmpty()){
			AfxMessageBox("请输入公众号ID！");
			return;
		}
		if (name.IsEmpty()){
			AfxMessageBox("请输入公众号名称！");
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
	
	int ErrorNum;
	CString ErrorMsg;
	LONG lRet = ModifyProc(pDB,m_tableType,ErrorNum,ErrorMsg);
	if (lRet != eGCError_Success){
		AfxMessageBox(ErrorMsg);
		return ;
	}
	else AfxMessageBox(GetErrorStr(lRet));
	delete pDB;
	pDB = NULL;	

	CDialog::OnOK();
}
