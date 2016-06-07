
// pbSysToolDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "pbSysTool.h"
#include "pbSysToolDlg.h"
#include "afxwin.h"
#include "AddDlg.h"
#include "ModifyDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define FORMATTITLE(a) (#a)

const int PUBLIC_TABLEHEAD_LEN = 10;
const int INCODE_TABLEHEAD_LEN = 3;
const int TPRULE_TABLEHEAD_LEN = 10;
const int PUBLIC_TP_RELATION_LEN = 9;
const int TPRELATION_TABLEHEAD_LEN = 5;
const int DISPATCHRELATION_TABLEHEAD_LEN=7;

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框
const char* m_szPublicTbHead[PUBLIC_TABLEHEAD_LEN] ={
	"公众号ID","公众号名称","创建时间","删除时间"
	,"修改时间","是否有效","可见用户范围","创建者账号",
	"公众号标签","公众号欢迎内容"};

const char* m_szIncodeTbHead[INCODE_TABLEHEAD_LEN] = {
	"接入码ID","接入码名称","调度规则"};;

const char* m_szTpRuleTbHead[TPRULE_TABLEHEAD_LEN] = {
	"路由规则ID","路由规则类型","路由名称","起始时间",
	"时间间隔(秒)","问题类型优先级","接口路由名称",
	"客服工号","有效标识","扩展规则"};

const char* m_szPublicTPRelationHead[PUBLIC_TP_RELATION_LEN] = {
	"公众号ID","公众号名称","路由规则ID","路由规则类型",
	"路由名称","起始时间","时间间隔(秒)","问题类型优先级",
	"接口路由名称"};

//路由规则对应接入码 1:n 关系
const char* m_szTpRelationTbHead[TPRELATION_TABLEHEAD_LEN] = {
	"路由规则ID","路由规则类型","路由名称","接入码ID","接入码名称"};

//接入码对应客服 1:n 关系
const char* m_szDispatchRelationTbHead[DISPATCHRELATION_TABLEHEAD_LEN] ={
	"接入码ID","接入码名称","调度规则","客服工号",
	"客服ID","客服名称","客服优先级"};

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()

CpbSysToolDlg::CpbSysToolDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CpbSysToolDlg::IDD, pParent)
	,m_curComboSel(1)
{
	m_Grid = new CGridCtrl;
}

void CpbSysToolDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, m_ComboTable);
}

BEGIN_MESSAGE_MAP(CpbSysToolDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON2, &CpbSysToolDlg::OnBnClickedButtonAdd)
	ON_CBN_SELCHANGE(IDC_COMBO1, &CpbSysToolDlg::OnCbnSelchangeComboSelectTable)
	ON_BN_CLICKED(IDC_BUTTON1, &CpbSysToolDlg::OnBnClickedButtonModify)
	ON_BN_CLICKED(IDC_BUTTON3, &CpbSysToolDlg::OnBnClickedButtonDelte)
	ON_BN_CLICKED(IDOK, &CpbSysToolDlg::OnBnClickedOk)
	ON_NOTIFY(NM_CLICK, IDC_GRID, OnGridClick)
	ON_WM_SIZE()
	ON_WM_CANCELMODE()
END_MESSAGE_MAP()


// CpbSysToolDlg 消息处理程序

BOOL CpbSysToolDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标
	m_pDB = new CADODatabase;
	if (!ConnectDataBase(m_pDB))
	{
		AfxMessageBox("数据库连接失败！");
		return 0;
	}
 	
 	m_ComboTable.InsertString(PUBLIC-1,"公众号");
	m_ComboTable.InsertString(INCODE-1,"接入码");
	m_ComboTable.InsertString(TP-1,"路由规则");
	m_ComboTable.InsertString(PUBLIC_TP_RELATION-1,"公司号路由关系表");
	m_ComboTable.InsertString(TP_INCODE_RELATION-1,"路由关系表");
	m_ComboTable.InsertString(DISPATCH_RELATION-1,"调度关系表");
	m_ComboTable.SetCurSel(PUBLIC-1);
	CRect rc1;
	GetClientRect(&rc1);
	CRect rc2(rc1.left,rc1.top+35,rc1.right,rc1.bottom);
	m_Grid->Create(rc2,this,IDC_GRID,WS_CHILD|WS_VISIBLE);
	m_Grid->SetListMode(TRUE);
	m_Grid->SetSingleRowSelection(TRUE);
	m_Grid->SetFont(GetFont());
	vDataContainer Data;
	vGridHead      Head;
	GetDbData(PUBLIC,Data,Head);
	UpdateGridData(Data,Head);
	return TRUE;
}
void CpbSysToolDlg::OnPaint()
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
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

HCURSOR CpbSysToolDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CpbSysToolDlg::UpdateGridData(vDataContainer Data,vGridHead Head)
{
	int nRow = Data.size()+1;
	int nCol = Head.size();
	m_Grid->DeleteAllItems();
	for (int col = 0;col < nCol;col++){
		m_Grid->DeleteColumn(col);
	}
	m_Grid->SetRowCount(nRow);
	m_Grid->SetColumnCount(nCol);
	m_Grid->SetFixedRowCount(1);
	m_Grid->SetRowHeight(0,GRIDHEIGHT);
	m_Grid->SetBkColor(RGB(240,240,240));

	for (int col = 0;col < nCol;col++)
	{
		m_Grid->SetItemText(0,col,Head[col]);
	}
	for (int row = 1;row<nRow;row++)
	{
		for (int col = 0;col<nCol;col++)
		{
			CString strData = Data[row-1][col];
			if(INCODE == m_curComboSel){
				if (col == 2)strData = TransToDispatch(StrToInt(strData));
			}
			if(TP == m_curComboSel){
				if (col == 1) strData = TransToTpRule(StrToInt(strData));
				if (col == 3) strData = TransIntToStrTime(StrToInt(strData));
			}
			if(PUBLIC_TP_RELATION == m_curComboSel){
				if (col == 3) strData = TransToTpRule(StrToInt(strData));
				if (col == 5) strData = TransIntToStrTime(StrToInt(strData));
			}
			if (TP_INCODE_RELATION == m_curComboSel){
				if(col == 1) strData = TransToTpRule(StrToInt(strData));
			}
			if (DISPATCH_RELATION == m_curComboSel){
				if (col == 2) strData = TransToDispatch(StrToInt(strData));
			}
			m_Grid->SetItemText(row,col,strData);
			m_Grid->SetItemState(row,col,GVIS_READONLY);
		}
	}
	m_Grid->SetCallbackFunc(GridCallback,(LPARAM)this);
	m_Grid->Refresh();
}

CString CpbSysToolDlg::GetDlgTitle(int btnop)
{
	CString tempstr=_T("");
	int curSel = m_curComboSel;
	switch(curSel<<btnop)
	{
	case PUBLIC_ADD: 
		tempstr = FORMATTITLE(PUBLIC_ADD);
		break;
	case PUBLIC_ADD_INCODE:
		tempstr = FORMATTITLE(PUBLIC_ADD_INCODE);
		break;
	case PUBLIC_ADD_TP:
		tempstr = FORMATTITLE(PUBLIC_ADD_TP);
		break;
	case PUBLIC_ADD_RELATION:
		tempstr = FORMATTITLE(PUBLIC_ADD_RELATION);
		break;
	case PUBLIC_ADD_TP_INCODE_RELATION:
		tempstr = FORMATTITLE(PUBLIC_ADD_TP_INCODE_RELATION);
		break;
	case PUBLIC_ADD_DISPATCH_RELATION:
		tempstr = FORMATTITLE(PUBLIC_ADD_DISPATCH_RELATION);
		break;
	case PUBLIC_MODIFY:
		tempstr = FORMATTITLE(PUBLIC_MODIFY);
		break;
	case PUBLIC_MODIFY_INCODE:
		tempstr = FORMATTITLE(PUBLIC_MODIFY_INCODE);
		break;
	case PUBLIC_MODIFY_TP:
		tempstr = FORMATTITLE(PUBLIC_MODIFY_TP);
		break;
	case PUBLIC_MODIFY_RELATION:
		tempstr = FORMATTITLE(PUBLIC_MODIFY_RELATION);
		break;
	case PUBLIC_MODIFY_TP_INCODE_RELATION:
		tempstr = FORMATTITLE(PUBLIC_MODIFY_TP_INCODE_RELATION);
		break;
	case PUBLIC_MODIFY_DISPATCH_RELATION:
		tempstr = FORMATTITLE(PUBLIC_MODIFY_DISPATCH_RELATION);
		break;
	case PUBLIC_DELETE:
		tempstr = FORMATTITLE(PUBLIC_DELETE);
		break;
	case PUBLIC_DELETE_INCODE:
		tempstr = FORMATTITLE(PUBLIC_DELETE_INCODE);
		break;
	case PUBLIC_DELETE_TP:
		tempstr = FORMATTITLE(PUBLIC_DELETE_TP);
		break;
	case PUBLIC_DELETE_TP_INCODE_RELATION:
		tempstr = FORMATTITLE(PUBLIC_DELETE_TP_INCODE_RELATION);
		break;
	case PUBLIC_DELETE_DISPATCH_RELATION:
		tempstr = FORMATTITLE(PUBLIC_DELETE_DISPATCH_RELATION);
		break;
	default:
		break;
	}
	return tempstr;
};

LONG CpbSysToolDlg::GetPublicInfo(vDataContainer& vpbInfo)
{
	LONG lRet = eGCError_Success;
	if (!m_pDB) return eGCError_DbCon;
	if (!m_pDB->IsOpen()) return eGCError_DbClose;
	BOOL bRecord = FALSE;
	CADORecordset adoRs(m_pDB);
	CADOCommand adoCmd(m_pDB,"select * from publicinfo where delflag = 0;",CADOCommand::typeCmdText);
	do 
	{
		if (!adoRs.Execute(&adoCmd)) { lRet = eGCError_DbExe;break;}
		if(!adoRs.IsOpen()) break;
		long nRecordCount = adoRs.GetRecordCount();
		for (long nIdx=0;nIdx<nRecordCount;nIdx++,adoRs.MoveNext())
		{
			bRecord = TRUE;
			long fieldCount = adoRs.GetFieldCount();
			CString strValue = _T("");
			std::vector<CString > vtRecord;
			for (long fieldIdx = 0;fieldIdx<fieldCount;fieldIdx++)
			{
				adoRs.GetFieldValue(fieldIdx,strValue);
				vtRecord.push_back(strValue);
			}
			vpbInfo.push_back(vtRecord);
		}

	}while(FALSE);
	if (!bRecord)
		lRet = eGCError_RsNULL;
	return lRet;
}
LONG CpbSysToolDlg::GetIncode(vDataContainer& vincode)
{
	//m_vIncode.clear();
	LONG lRet = eGCError_Success;
	if (!m_pDB) return eGCError_DbCon;
	if (!m_pDB->IsOpen()) return eGCError_DbClose;
	BOOL bRecord = FALSE;
	CADORecordset adoRs(m_pDB);
	CADOCommand adoCmd(m_pDB,"select id,name,dispatchrule from public_incode where delflag = 0;",adCmdText);
	do 
	{
		if (!adoRs.Execute(&adoCmd)) { lRet = eGCError_DbExe;break;}
		if(!adoRs.IsOpen()) break;
		long nRecordCount = adoRs.GetRecordCount();
		for (long nIdx=0;nIdx<nRecordCount;nIdx++,adoRs.MoveNext())
		{
			bRecord = TRUE;
			long fieldCount = adoRs.GetFieldCount();
			CString strValue = _T("");
			std::vector<CString > vtRecord;
			for (long fieldIdx = 0;fieldIdx<fieldCount;fieldIdx++)
			{
				adoRs.GetFieldValue(fieldIdx,strValue);
				vtRecord.push_back(strValue);
			}
			vincode.push_back(vtRecord);
		}

	}while(FALSE);
	if (!bRecord)
		lRet = eGCError_RsNULL;
	return lRet;
}

LONG CpbSysToolDlg::GetTpRule(vDataContainer& vtprule)
{
	LONG lRet = eGCError_Success;
	if (!m_pDB) return eGCError_DbCon;
	if (!m_pDB->IsOpen()) return eGCError_DbClose;
	BOOL bRecord = FALSE;
	CString selectStr = _T("");
	CADORecordset adoRs(m_pDB);
	CADOCommand adoCmd(m_pDB,"select * from public_tprule where delflag = 0;",adCmdText);

	do 
	{
		if (!adoRs.Execute(&adoCmd)) { lRet = eGCError_DbExe;break;}
		if(!adoRs.IsOpen()) break;
		long nRecordCount = adoRs.GetRecordCount();
		for (long nIdx=0;nIdx<nRecordCount;nIdx++,adoRs.MoveNext())
		{
			bRecord = TRUE;
			long fieldCount = adoRs.GetFieldCount();
			CString strValue = _T("");
			std::vector<CString > vtRecord;
			for (long fieldIdx = 0;fieldIdx<fieldCount;fieldIdx++)
			{
				adoRs.GetFieldValue(fieldIdx,strValue);
				vtRecord.push_back(strValue);
			}
			vtprule.push_back(vtRecord);
		}

	}while(FALSE);
	if (!bRecord)
		lRet = eGCError_RsNULL;
	return lRet;
}

LONG CpbSysToolDlg::GetPublicTpRelation(vDataContainer& Data)
{
	LONG lRet = eGCError_Success;
	if (!m_pDB) return eGCError_DbCon;
	if (!m_pDB->IsOpen()) return eGCError_DbClose;
	BOOL bRecord = FALSE;
	CString selectStr = _T("");
	CADORecordset adoRs(m_pDB);
	CADOCommand adoCmd(m_pDB,"public_get_relation");
	do 
	{
		if (!adoRs.Execute(&adoCmd)) { lRet = eGCError_DbExe;break;}
		if(!adoRs.IsOpen()) break;
		long nRecordCount = adoRs.GetRecordCount();
		for (long nIdx=0;nIdx<nRecordCount;nIdx++,adoRs.MoveNext())
		{
			bRecord = TRUE;
			long fieldCount = adoRs.GetFieldCount();
			CString strValue = _T("");
			std::vector<CString > vtRecord;
			for (long fieldIdx = 0;fieldIdx<fieldCount;fieldIdx++)
			{
				adoRs.GetFieldValue(fieldIdx,strValue);
				vtRecord.push_back(strValue);
			}
			Data.push_back(vtRecord);
		}
	}while(FALSE);
	if (!bRecord)
		lRet = eGCError_RsNULL;
	return lRet;
}

LONG CpbSysToolDlg::GetTpRelation(vDataContainer& vtprelation)
{
	LONG lRet = eGCError_Success;
	if (!m_pDB) return eGCError_DbCon;
	if (!m_pDB->IsOpen()) return eGCError_DbClose;
	BOOL bRecord = FALSE;
	CString selectStr = _T("");
	CADORecordset adoRs(m_pDB);
	CADOCommand adoCmd(m_pDB,"public_get_alltpinfo");
	do 
	{
		if (!adoRs.Execute(&adoCmd)) { lRet = eGCError_DbExe;break;}
		if(!adoRs.IsOpen()) break;
		long nRecordCount = adoRs.GetRecordCount();
		for (long nIdx=0;nIdx<nRecordCount;nIdx++,adoRs.MoveNext())
		{
			bRecord = TRUE;
			long fieldCount = adoRs.GetFieldCount();
			CString strValue = _T("");
			std::vector<CString > vtRecord;
			for (long fieldIdx = 0;fieldIdx<fieldCount;fieldIdx++)
			{
				adoRs.GetFieldValue(fieldIdx,strValue);
				vtRecord.push_back(strValue);
			}
			vtprelation.push_back(vtRecord);
		}
	}while(FALSE);
	if (!bRecord)
		lRet = eGCError_RsNULL;
	return lRet;
}

LONG CpbSysToolDlg::GetDispatchRelation(vDataContainer& vdisrelation)
{
	LONG lRet = eGCError_Success;
	if (!m_pDB) return eGCError_DbCon;
	if (!m_pDB->IsOpen()) return eGCError_DbClose;
	BOOL bRecord = FALSE;
	CString selectStr = _T("");
	CADORecordset adoRs(m_pDB);
	CADOCommand adoCmd(m_pDB,"public_get_alldispatchrelation");
	do 
	{
		if (!adoRs.Execute(&adoCmd)) { lRet = eGCError_DbExe;break;}
		if(!adoRs.IsOpen()) break;
		long nRecordCount = adoRs.GetRecordCount();
		for (long nIdx=0;nIdx<nRecordCount;nIdx++,adoRs.MoveNext())
		{
			bRecord = TRUE;
			long fieldCount = adoRs.GetFieldCount();
			CString strValue = _T("");
			std::vector<CString > vtRecord;
			for (long fieldIdx = 0;fieldIdx<fieldCount;fieldIdx++)
			{
				adoRs.GetFieldValue(fieldIdx,strValue);
				vtRecord.push_back(strValue.Trim());
			}
			vdisrelation.push_back(vtRecord);
		}
	}while(FALSE);
	if (!bRecord)
		lRet = eGCError_RsNULL;
	return lRet;
}


LONG CpbSysToolDlg::GetCustomerInfo(vDataContainer& vcustomer)
{
	LONG lRet = eGCError_Success;
	if (!m_pDB) return eGCError_DbCon;
	if (!m_pDB->IsOpen()) return eGCError_DbClose;
	BOOL bRecord = FALSE;
	CString selectStr = _T("");
	CADORecordset adoRs(m_pDB);
	CADOCommand adoCmd(m_pDB,"public_get_CustomerInfo");
	do 
	{
		if (!adoRs.Execute(&adoCmd)) { lRet = eGCError_DbExe;break;}
		if(!adoRs.IsOpen()) break;
		long nRecordCount = adoRs.GetRecordCount();
		for (long nIdx=0;nIdx<nRecordCount;nIdx++,adoRs.MoveNext())
		{
			bRecord = TRUE;
			long fieldCount = adoRs.GetFieldCount();
			CString strValue = _T("");
			std::vector<CString > vtRecord;
			for (long fieldIdx = 0;fieldIdx<fieldCount;fieldIdx++)
			{
				adoRs.GetFieldValue(fieldIdx,strValue);
				vtRecord.push_back(strValue.Trim());
			}
			vcustomer.push_back(vtRecord);
		}
	}while(FALSE);
	if (!bRecord)
		lRet = eGCError_RsNULL;
	return lRet;
}

LONG CpbSysToolDlg::GetDbData(int type,vDataContainer &Data,vGridHead& Head)
{
	if (!m_vCurTPData.empty()){
		m_vCurTPData.clear();
	}
	if(!m_vCurIncodeData.empty()){
		m_vCurIncodeData.clear();
	}
	if(!m_vCurCustData.empty()){
		m_vCurCustData.clear();
	}
	if(!m_vCurPublicInfoData.empty()){
		m_vCurPublicInfoData.clear();
	}
	LONG lRet = eGCError_Success;
	if (PUBLIC == type)
	{
		for (int i = 0;i<PUBLIC_TABLEHEAD_LEN;i++){
			Head.push_back(m_szPublicTbHead[i]);
		}
		lRet = GetPublicInfo(Data);
		GetTpRule(m_vCurTPData);
	}
	if (INCODE == type)
	{
		for (int i = 0;i<INCODE_TABLEHEAD_LEN;i++){
			Head.push_back(m_szIncodeTbHead[i]);
		}
		lRet = GetIncode(Data);
	}
	if (TP == type)
	{
		for (int i = 0;i<TPRULE_TABLEHEAD_LEN ;i++){
			Head.push_back(m_szTpRuleTbHead[i]);
		}
		lRet = GetTpRule(Data);
	}
	if (PUBLIC_TP_RELATION == type)
	{
		for(int i = 0;i<PUBLIC_TP_RELATION_LEN;i++){
			Head.push_back(m_szPublicTPRelationHead[i]);
		}
		GetPublicInfo(m_vCurPublicInfoData);
		GetTpRule(m_vCurTPData);
		lRet = GetPublicTpRelation(Data);
	}
	if (TP_INCODE_RELATION == type)
	{
		for (int i = 0;i<TPRELATION_TABLEHEAD_LEN ;i++){
			Head.push_back(m_szTpRelationTbHead[i]);
		}
		GetTpRule(m_vCurTPData);
		GetIncode(m_vCurIncodeData);
		lRet = GetTpRelation(Data);
	}
	if (DISPATCH_RELATION == type)
	{
		for (int i = 0;i<DISPATCHRELATION_TABLEHEAD_LEN;i++){
			Head.push_back(m_szDispatchRelationTbHead[i]);
		}
		GetIncode(m_vCurIncodeData);
		GetCustomerInfo(m_vCurCustData);
		lRet = GetDispatchRelation(Data);
	}
	return lRet;
}

LONG CpbSysToolDlg::DelPulbic(CString pa,int &ErrorNum,CString &ErrorMsg)
{
	LONG lRet = eGCError_Success;
	if (!m_pDB) return eGCError_DbCon;
	if (!m_pDB->IsOpen()) return eGCError_DbClose;
	CADORecordset adoRs(m_pDB);
	CADOCommand adoCmd(m_pDB,"public_del");
	CADOParameter pParamIn_pa(CADORecordset::typeBigInt); 
	pParamIn_pa.SetValue(atol(pa));
	adoCmd.AddParameter(&pParamIn_pa);
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

LONG CpbSysToolDlg::DelIncode(CString pa,int &ErrorNum,CString &ErrorMsg)
{
	LONG lRet = eGCError_Success;
	if (!m_pDB) return eGCError_DbCon;
	if (!m_pDB->IsOpen()) return eGCError_DbClose;
	CADORecordset adoRs(m_pDB);
	CADOCommand adoCmd(m_pDB,"public_del_incode");
	CADOParameter pParamIn_pa(CADORecordset::typeBigInt); 
	pParamIn_pa.SetValue(atol(pa));
	adoCmd.AddParameter(&pParamIn_pa);
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

LONG CpbSysToolDlg::DelTpRule(CString pa,int &ErrorNum,CString &ErrorMsg)
{
	LONG lRet = eGCError_Success;
	if (!m_pDB) return eGCError_DbCon;
	if (!m_pDB->IsOpen()) return eGCError_DbClose;
	CADORecordset adoRs(m_pDB);
	CADOCommand adoCmd(m_pDB,"public_del_tprule");
	CADOParameter pParamIn_pa(CADORecordset::typeBigInt); 
	pParamIn_pa.SetValue(atol(pa));
	adoCmd.AddParameter(&pParamIn_pa);
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

LONG CpbSysToolDlg::DelPublicRelation(CString pa1,CString pa2,int &ErrorNum,CString &ErrorMsg)
{
	LONG lRet = eGCError_Success;
	if (!m_pDB) return eGCError_DbCon;
	if (!m_pDB->IsOpen()) return eGCError_DbClose;
	CADORecordset adoRs(m_pDB);
	CADOCommand adoCmd(m_pDB,"public_del_publicrelation");
	CADOParameter pParamIn_pa1(CADORecordset::typeBigInt); 
	pParamIn_pa1.SetValue(atol(pa1));
	adoCmd.AddParameter(&pParamIn_pa1);
	CADOParameter pParamIn_pa2(CADORecordset::typeBigInt); 
	pParamIn_pa2.SetValue(atol(pa2));
	adoCmd.AddParameter(&pParamIn_pa2);
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

LONG CpbSysToolDlg::DelTpRelation(CString pa1,CString pa2,int &ErrorNum,CString &ErrorMsg)
{
	LONG lRet = eGCError_Success;
	if (!m_pDB) return eGCError_DbCon;
	if (!m_pDB->IsOpen()) return eGCError_DbClose;
	CADORecordset adoRs(m_pDB);
	CADOCommand adoCmd(m_pDB,"public_del_tpincoderelation");
	CADOParameter pParamIn_pa1(CADORecordset::typeBigInt); 
	pParamIn_pa1.SetValue(atol(pa1));
	adoCmd.AddParameter(&pParamIn_pa1);
	CADOParameter pParamIn_pa2(CADORecordset::typeBigInt); 
	pParamIn_pa2.SetValue(atol(pa2));
	adoCmd.AddParameter(&pParamIn_pa2);
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
LONG CpbSysToolDlg::DelDispatchRelation(CString pa1,CString pa2,int &ErrorNum,CString &ErrorMsg)
{
	LONG lRet = eGCError_Success;
	if (!m_pDB) return eGCError_DbCon;
	if (!m_pDB->IsOpen()) return eGCError_DbClose;
	CADORecordset adoRs(m_pDB);
	CADOCommand adoCmd(m_pDB,"public_del_dispatchrelation");
	CADOParameter pParamIn_pa1(CADORecordset::typeBigInt); 
	pParamIn_pa1.SetValue(atol(pa1));
	adoCmd.AddParameter(&pParamIn_pa1);
	CADOParameter pParamIn_pa2(CADORecordset::typeBigInt); 
	pParamIn_pa2.SetValue(atol(pa2));
	adoCmd.AddParameter(&pParamIn_pa2);
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

//消息映射
void CpbSysToolDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}



void CpbSysToolDlg::OnBnClickedButtonAdd()
{
	CAddDlg adddlg;
	adddlg.SetTableType(m_curComboSel);
	adddlg.SetDlgTitle(GetDlgTitle(BTN_ADD));
	if (PUBLIC == m_curComboSel)
	{
		adddlg.SetTPRule(m_vCurTPData);
	}
	if (PUBLIC_TP_RELATION == m_curComboSel)
	{
		adddlg.SetPublicInfo(m_vCurPublicInfoData);
		adddlg.SetTPRule(m_vCurTPData);
	}
	if (TP_INCODE_RELATION == m_curComboSel)
	{
		adddlg.SetTPRule(m_vCurTPData);
		adddlg.SetIncode( m_vCurIncodeData);
	}
	if (DISPATCH_RELATION == m_curComboSel)
	{
		adddlg.SetIncode( m_vCurIncodeData);
		adddlg.SetCustomer(m_vCurCustData);
	}
	INT_PTR dlgRet = adddlg.DoModal();
	if (IDOK == dlgRet)
	{
		vDataContainer Data;
		vGridHead      Head;
		GetDbData(m_curComboSel,Data,Head);
		UpdateGridData(Data,Head);
	}
	if (IDCANCEL == dlgRet)
	{
		//AfxMessageBox("取消");
		
	}
}

void CpbSysToolDlg::OnBnClickedButtonModify()
{
	CModifyDlg modifydlg;
	modifydlg.SetTableType(m_curComboSel);
	modifydlg.SetDlgTitle(GetDlgTitle(BTN_MODIFY*2));
	modifydlg.SetCtrlData(m_vModifyHead,m_vModifyData);
	CGridCtrl* pGrid = (CGridCtrl*)GetDlgItem(IDC_GRID);
	int nSelRow = pGrid->GetSelectedCount();
	if(nSelRow == 0) {
		AfxMessageBox("请选择要修改的数据项");
		return;
	}
	INT_PTR dlgRet = modifydlg.DoModal();
	if (IDOK == dlgRet)
	{
		vDataContainer Data;
		vGridHead      Head;
		GetDbData(m_curComboSel,Data,Head);
		UpdateGridData(Data,Head);
	}
	if (IDCANCEL == dlgRet)
	{
		//AfxMessageBox("取消");
	}
}

void CpbSysToolDlg::OnBnClickedButtonDelte()
{
	LONG lRet;
	CGridCtrl* pGrid = (CGridCtrl*)GetDlgItem(IDC_GRID);
	CString param1,param2;
	int ErrorNum;
	CString ErrorMsg;
	if (PUBLIC == m_curComboSel){
		param1 = pGrid->GetItemText(m_iCurRow,0);
		lRet = DelPulbic(param1,ErrorNum,ErrorMsg);
	}

	if (INCODE == m_curComboSel){
		param1 = pGrid->GetItemText(m_iCurRow,0);
		lRet = DelIncode(param1,ErrorNum,ErrorMsg);
	}
	if (TP == m_curComboSel){	
		param1 = pGrid->GetItemText(m_iCurRow,0);
		lRet = DelTpRule(param1,ErrorNum,ErrorMsg);
	}
	if (PUBLIC_TP_RELATION == m_curComboSel){
		param1 = pGrid->GetItemText(m_iCurRow,0);
		param2 = pGrid->GetItemText(m_iCurRow,2);
		lRet = DelPublicRelation(param1,param2,ErrorNum,ErrorMsg);
	}

	if (TP_INCODE_RELATION == m_curComboSel){
		param1 = pGrid->GetItemText(m_iCurRow,0);
		param2 = pGrid->GetItemText(m_iCurRow,3);
		lRet = DelTpRelation(param1,param2,ErrorNum,ErrorMsg);
	}

	if (DISPATCH_RELATION== m_curComboSel){
		param1 = pGrid->GetItemText(m_iCurRow,0);
		param2 = pGrid->GetItemText(m_iCurRow,4);
		lRet = DelDispatchRelation(param1,param2,ErrorNum,ErrorMsg);
	}

	vDataContainer Data;
	vGridHead      Head;
	if (lRet == eGCError_Success){
		AfxMessageBox(GetErrorStr(lRet));
		GetDbData(m_curComboSel,Data,Head);
		UpdateGridData(Data,Head);
	}
	else AfxMessageBox(ErrorMsg);
}

void CpbSysToolDlg::OnCbnSelchangeComboSelectTable()
{
	m_curComboSel = m_ComboTable.GetCurSel()+1;
	CButton* Btn = (CButton*)GetDlgItem(IDC_BUTTON1);
	if (m_curComboSel > TP) Btn->EnableWindow(FALSE);
	else Btn->EnableWindow(TRUE);
	vDataContainer Data;
	vGridHead      Head;
	LONG lRet = GetDbData(m_curComboSel,Data,Head);
	if (eGCError_Success == lRet){
		UpdateGridData(Data,Head);
	}
	else AfxMessageBox(GetErrorStr(lRet));
}

void CpbSysToolDlg::OnSize(UINT nType, int cx, int cy)
{
	if (m_Grid->GetSafeHwnd()){
		CRect rc1;
		GetClientRect(&rc1);
		CRect rc2(rc1.left,rc1.top+35,rc1.right,rc1.bottom);
		m_Grid->MoveWindow(&rc2);
	}
}

void CpbSysToolDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码

}

void CpbSysToolDlg::OnCancel()
{
	CDialog::OnCancel();
	delete m_pDB;
	delete m_Grid;
	m_Grid = NULL;
	m_pDB = NULL;
}

BOOL CALLBACK CpbSysToolDlg::GridCallback(GV_DISPINFO *pDispInfo, LPARAM lParam)
{
	CpbSysToolDlg* pDlg = (CpbSysToolDlg*)lParam;
	if(INCODE == pDlg->m_curComboSel){
		if (pDispInfo->item.col== 2){
			TransToDispatch(StrToInt(pDispInfo->item.strText));
		}
	}
	return TRUE;
}

void CpbSysToolDlg::OnGridClick(NMHDR *pNotifyStruct, LRESULT* pResult)
{
	if (!m_vModifyHead.empty()) m_vModifyHead.clear();
	if (!m_vModifyData.empty()) m_vModifyData.clear();
	NM_GRIDVIEW* pItem = (NM_GRIDVIEW*) pNotifyStruct;
	m_iCurRow = pItem->iRow;
	CGridCtrl* Grid = (CGridCtrl*)GetDlgItem(IDC_GRID);
	int nCol = Grid->GetColumnCount();
	if(m_curComboSel == PUBLIC){
		for (int col = 0;col < nCol; col++){
			if(col < 2 || col > 7 ) {
				CString szHead = Grid->GetItemText(0,col);
				CString szData = Grid->GetItemText(pItem->iRow,col);
				m_vModifyHead.push_back(szHead);
				m_vModifyData.push_back(szData);
			}
		}
	}
	if(m_curComboSel == INCODE){
		for (int col = 0; col < nCol;col++){
			CString szHead = Grid->GetItemText(0,col);
			CString szData = Grid->GetItemText(pItem->iRow,col);
			if(col == 2) szData.Format(_T("%d"),UnTransToDispatch(szData));
			m_vModifyHead.push_back(szHead);
			m_vModifyData.push_back(szData);
		}
	}
	if(m_curComboSel == TP){
		for (int col = 0; col < nCol; col++)
		{
			if(col < 8){
				CString szHead = Grid->GetItemText(0,col);
				CString szData = Grid->GetItemText(pItem->iRow,col);
				m_vModifyHead.push_back(szHead);
				m_vModifyData.push_back(szData);
			}
		}
	}
}