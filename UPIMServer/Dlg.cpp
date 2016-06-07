#include "stdafx.h"
#include "MainFrm.h"
#include "resource.h"
#include "Dlg.h"
#include "globalvar.h"

BEGIN_MESSAGE_MAP(CStatisticsDlg, CDialog)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_PAINT()
END_MESSAGE_MAP()

typedef enum LISTCTRL_e_ {
	LC_NUM
	,LC_SESSIONID          //sessionID
	, LC_UID			   //用户ID		
	, LC_USERSTATUS		   //UserStatus	
	, LC_INTERNETIP		   //客户端公网IP	
	, LC_PORT			   //客户端Port		
} LISTCTRL_e;			   	
const int COLLUMSIZE = 6;
int ROWNUM = 300;

CStatisticsDlg::CStatisticsDlg()
{
	
}

int CStatisticsDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{	
	if(CDialog::OnCreate(lpCreateStruct)==-1) return -1;
	//ModifyStyle(0,WS_CLIPCHILDREN);	// to avoid flicks of dialog child controls.
		// 
	return 0;
}
/////////////////////////////////////////////////////////////////////////////
// 
BOOL CStatisticsDlg::OnInitDialog()
{	
	CDialog::OnInitDialog();

	const int ColLen = 60;
	const int dwColLen = 120;
	TCHAR* HeadName[COLLUMSIZE] = {"序号", "连接ID", "用户ID", "用户状态", "连接IP", "连接PORT" };
	CListCtrl* ListInfo = (CListCtrl*)GetDlgItem(IDC_LIST_CONNECTINFO);
	ListInfo->SetExtendedStyle(LVS_EX_GRIDLINES|LVS_EX_SIMPLESELECT|LVS_EX_FULLROWSELECT);
	int i = 0;

	for (i= 0;i<COLLUMSIZE;i++)
	{
		ListInfo->InsertColumn(i,HeadName[i],LVCFMT_LEFT,dwColLen);	
	}

	for (int j = 0;j<ROWNUM;j++)
	{
		ListInfo->InsertItem(LVIF_TEXT|LVIF_STATE, j, "", 0, LVIS_SELECTED, 0, 0);
	}
	
	CRect rc1,rc2;
	GetDlgItem(IDC_STATIC_DATASTAT)->GetClientRect(&rc1);
	this->GetClientRect(&rc2);
	CRect rc3(rc2.left+rc1.Width()+3,rc1.top,rc2.right,rc1.bottom);
	ListInfo->MoveWindow(&rc3);
	return TRUE;
}

void CStatisticsDlg::OnOK()
{

}

void CStatisticsDlg::OnCancel()
{

}

void CStatisticsDlg::UpdateData(PSDATA SData )
{
	CString temp;
	temp.Format(_T("%ldB"),SData->SendFlow);
	GetDlgItem(IDC_STATIC_SENDFLOW)->SetWindowText(temp);
	
	temp.Format(_T("%ldB"),SData->SendPeak);
	GetDlgItem(IDC_STATIC_SENDPEAK)->SetWindowText(temp);

	temp.Format(_T("%ld"),SData->SendPack);
	GetDlgItem(IDC_STATIC_SENDPACK)->SetWindowText(temp);

	temp.Format(_T("%ld"),SData->SendPackPeak);
	GetDlgItem(IDC_STATIC_SENDPACKPEAK)->SetWindowText(temp);

	temp.Format(_T("%ldB"),SData->ReceiveFlow);
	GetDlgItem(IDC_STATIC_RECIEVEFLOW)->SetWindowText(temp);
	
	temp.Format(_T("%ldB"),SData->RecievePeak);
	GetDlgItem(IDC_STATIC_RECIEVEPEAK)->SetWindowText(temp);

	temp.Format(_T("%ld"),SData->ReceivePack);
	GetDlgItem(IDC_STATIC_RECIEVEPACK)->SetWindowText(temp);

	temp.Format(_T("%ld"),SData->ReceivePackPeak);
	GetDlgItem(IDC_STATIC_RECIEVEPACKPEAK)->SetWindowText(temp);

	temp.Format(_T("%ld"),SData->CurConnectNum);
	GetDlgItem(IDC_STATIC_CURCONNECT)->SetWindowText(temp);
	
	temp.Format(_T("%ld"),SData->ConnectPeak);
	GetDlgItem(IDC_STATIC_CONNECTPEAK)->SetWindowText(temp);
	CMainFrame* pMainFrame = (CMainFrame*)AfxGetApp()->m_pMainWnd;

	if (pMainFrame->m_bOnLineOrOffLine)
	{
		LoadUserOnLineInfo();
		InterlockedExchange(&pMainFrame->m_bOnLineOrOffLine,FALSE);
	}
	UpdateWindow();
}

void CStatisticsDlg::OnSize(UINT nType, int cx, int cy)
{
	CListCtrl* ListInfo = (CListCtrl*)GetDlgItem(IDC_LIST_CONNECTINFO);
	if (ListInfo->GetSafeHwnd())
	{
		CRect rc1,rc2;
		GetDlgItem(IDC_STATIC_DATASTAT)->GetClientRect(&rc1);
		this->GetClientRect(&rc2);
		CRect rc3(rc2.left+rc1.Width()+3,rc1.top,rc2.right,rc2.bottom);
		ListInfo->MoveWindow(&rc3);
	}
}

void CStatisticsDlg::OnPaint()
{
	Invalidate();
	__super::OnPaint();
}

void CStatisticsDlg::LoadUserOnLineInfo()
{
	CListCtrl* ListInfo = (CListCtrl*)GetDlgItem(IDC_LIST_CONNECTINFO);
	UpdateListData();
	vector <UserNode> allNode;
	if (g_MongoDBMgr.GetAllOnlineUser(allNode))
	{
		if (allNode.size() > 0)
		{
			for (int nIndex_=0; nIndex_< allNode.size(); nIndex_++)
			{
				AddListItem(allNode[nIndex_],nIndex_);
			}
		}
	}
}

BOOL CStatisticsDlg::AddListItem(const UserNode& userNode,int nCurItem)
{
	BOOL bRet = TRUE;
	CString strText;
	UserNode tempUserNode;
	tempUserNode.Copy(userNode);
	CLock cslock;
	CAutoLock autolock(&cslock);
	CListCtrl* ListInfo = (CListCtrl*)GetDlgItem(IDC_LIST_CONNECTINFO);

	strText.Format(_T("%d"), nCurItem+1);
	if ((nCurItem + 1)> ROWNUM) {
		ROWNUM = nCurItem + 1;
		ListInfo->InsertItem(LVIF_TEXT|LVIF_STATE, nCurItem + 1, strText, 0, LVIS_SELECTED, 0, 0);
	}
	else 
		ListInfo->SetItemText(nCurItem, LC_NUM, strText);
	
	strText.Format(_T("%u"), tempUserNode.m_dwSessionID);
	ListInfo->SetItemText(nCurItem, LC_SESSIONID, strText);

	strText.Format(_T("%s"), tempUserNode.m_szUID);
	ListInfo->SetItemText(nCurItem, LC_UID, strText);

	strText.Format(_T("%d"), tempUserNode.m_dwUserStatus);
	ListInfo->SetItemText(nCurItem, LC_USERSTATUS, strText);

	strText.Format(_T("%s"), tempUserNode.m_szInternetIp);
	ListInfo->SetItemText(nCurItem, LC_INTERNETIP, strText);

	strText.Format(_T("%d"), tempUserNode.m_Port);
	ListInfo->SetItemText(nCurItem, LC_PORT, strText);

	return bRet;
}

void CStatisticsDlg::UpdateListData()
{
	CListCtrl* ListInfo = (CListCtrl*)GetDlgItem(IDC_LIST_CONNECTINFO);
	int nCount = ListInfo->GetItemCount();
	for (int i = 0; i < nCount; i++)
	{
		ListInfo->SetItemText(i, LC_NUM, "");
		ListInfo->SetItemText(i, LC_SESSIONID, "");
		ListInfo->SetItemText(i, LC_UID, "");
		ListInfo->SetItemText(i, LC_USERSTATUS, "");
		ListInfo->SetItemText(i, LC_INTERNETIP, "");
		ListInfo->SetItemText(i, LC_PORT, "");
	}
}