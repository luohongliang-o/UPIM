#include "stdafx.h"
#include "../Func.h"
#include "../MainPort.h"
#include "UserMrgDialog.h"
#include "RichEditUtil.h"
#include "UserMrgAddGroup.h"




const TCHAR* const kTitleControlName		= _T("general_title");
const TCHAR* const kCloseControlName		= _T("general_closebtn");
const TCHAR* const kBackgroundControlName	= _T("general_bg");
const TCHAR* const kOKButtonControlName		= _T("general_ok");
const TCHAR* const kCloseButtonControlName	= _T("general_close");

/*
 分组情况：
	全部客户 fz_all
		未分组客户 fz_0
		自定义分组1 ~ 自定义分组10 fz_1~fz_10
*/

CUserMrgDialog::CUserMrgDialog(ECREATEDIALOGTYPE m_CreateType)
	:CreateType(m_CreateType)
{
	m_pMinSysBtn = m_pMaxSysBtn = m_pRestoreSysBtn = m_pCloseSysBtn = m_pUserMrgAddBtn = NULL;
	for (int nIndex = 0; nIndex <= USERMRG_FENZU; nIndex++)
		m_pFenZu[nIndex]  = NULL;
	m_pAllClient = NULL;
	m_pFenZuList = NULL;
	nGroupIndex = 0;
	nChooseGroup = -1;
	m_pGroupList = NULL;
	m_pList = NULL;
	m_pChenkBox = NULL;
	m_pUserMrgDelBtn = NULL;
}

CUserMrgDialog::~CUserMrgDialog()
{
	return;
}

LPCTSTR CUserMrgDialog::GetWindowClassName() const
{
	return _T("UPGuiFoundation");
}

CControlUI* CUserMrgDialog::CreateControl(LPCTSTR pstrClass)
{
	return NULL;
}

void CUserMrgDialog::InitWindow()
{
	//////////////////////////////////////////////////////////////////////////
	// 共用
	m_pMinSysBtn = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("general_minbtn")));
	m_pMaxSysBtn = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("general_maxbtn")));
	m_pRestoreSysBtn = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("general_restorebtn")));
	m_pCloseSysBtn = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("general_closebtn")));
	m_pUserMrgAddBtn = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("usermrg_addbtn")));
	m_pFenZuList = static_cast<CControlUI*>(m_PaintManager.FindControl(_T("usermrg_fenzu")));
	m_pGroupList = static_cast<CComboUI*>(m_PaintManager.FindControl(_T("usermrg_group")));
	m_pList = static_cast<CListUIEx*>(m_PaintManager.FindControl(_T("listex")));
	m_pChenkBox = static_cast<CCheckBoxUI*>(m_PaintManager.FindControl(_T("selall")));
	m_pUserMrgDelBtn = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T("usermrg_delbtn")));

	for (int nIndex = 0; nIndex <= USERMRG_FENZU; nIndex++)
	{
		CString strOptionName = "";
		strOptionName.Format("fz_%d", nIndex);
		m_pFenZu[nIndex]  = static_cast<COptionUI*>(m_PaintManager.FindControl(strOptionName));
	}
	m_pAllClient = static_cast<COptionUI*>(m_PaintManager.FindControl(_T("fz_all")));

	//////////////////////////////////////////////////////////////////////////
	// 分析师版本的才有客户管理器以及客户分组
	if (m_pAllClient)
	{
		CString strAllUserNum = "";
		strAllUserNum.Format("全部客户(%d)", m_vtRoomClient.size());
		m_pAllClient->SetText(strAllUserNum);
	}

	//////////////////////////////////////////////////////////////////////////
	// 得到服务器存储的分组信息
	// 加载列表
	LoadSelfGroup();
	ShowAllClient();
}

void CUserMrgDialog::AddList(CString strUpid, CString strGroupName)
{
	//////////////////////////////////////////////////////////////////////////
	CDialogBuilder builder;
	CListContainerElementUI* pLine = (CListContainerElementUI*)(builder.Create(_T("item_sigle_list_column.xml"),(UINT)0));
	// 按照xml中配置的list列表形式，设置成自己需要的值，再插入

	CTextUI * pText_ID = (CTextUI *)pLine->FindSubControl(_T("list_1"));
	CTextUI * pText_Group = (CTextUI *)pLine->FindSubControl(_T("list_2"));
	if (pText_ID && pText_Group)
	{
		pText_ID->SetText(strUpid);
		pText_Group->SetText(strGroupName);

		if( pLine != NULL ) 
			m_pList->InsertItem(m_pList->GetCount(), 30, pLine);
	}
	return;
}

int CUserMrgDialog::AddGroupInfo(CString strGroupName, int nNums)
{
	// m_pFenZu : 左边分组列表的标题  
	// nGroupIndex 分组列表的索引 
	// m_mapGroupIdToControls nGroupIndex 与 数据库中分组表中的分组ID的对应关系 
	// m_groupInfoStc    分析师分组 
	// m_mapSelfGroups   自定义分组  nGroupIndex 与 分组名字的对应 
	int nReturnID = -1;
	if (nGroupIndex <= USERMRG_FENZU)
	{
		CString strAllUserNum = "";
		strAllUserNum.Format("%s(%d)", strGroupName, nNums);
		int nBtnLen = strAllUserNum.GetLength() * 7;

		m_pFenZu[nGroupIndex]->SetText(strAllUserNum);
		m_pFenZu[nGroupIndex]->SetVisible(true);
		m_pFenZu[nGroupIndex]->SetMinWidth(nBtnLen);
		nReturnID = nGroupIndex;
		//基准值35，每个按钮25像素
		int nFixHight = 35 + 25*(nGroupIndex+1);
		m_pFenZuList->SetAttribute("height", IntToStr(nFixHight));

		if (nGroupIndex > 0)
		{
			CListLabelElementUI *pLabel = new CListLabelElementUI;
			pLabel->SetAttribute(_T("text"), strGroupName);
			pLabel->SetAttribute(_T("height"), _T("22"));
			// 下拉列表 
			m_pGroupList->Add(pLabel);
		}

		nGroupIndex ++;
	}
	else
	{
		::MessageBox(NULL, _T("自定义客户分组不能超过10个！"), _T("提示"), MB_OK);
	}
	return nReturnID;
}

void CUserMrgDialog::ShowAllClient()
{
	m_pList->RemoveAll();
	for (int nIndex = 0; nIndex< (int)m_vtRoomClient.size(); nIndex++)
	{
		CString strName,strGroup;
		strName.Format("%s", m_vtRoomClient[nIndex].m_szUPID);
		strGroup = m_mapSelfGroups[m_vtRoomClient[nIndex].m_nGroupID];

		AddList(strName, strGroup);
	}
	return;
}

void CUserMrgDialog::OnBtn_AllClient(TNotifyUI& msg)
{
	if (nChooseGroup == -1)
		return;
	nChooseGroup = -1;

	m_pChenkBox->SetCheck(false);
	ShowAllClient();
	return;
}

// 单击全部客户分组list  消息响应  
void CUserMrgDialog::ChangeFenZu(int nIndex)
{
	if (nChooseGroup == nIndex)
		return;

	// 鼠标点击的分组ID  
	nChooseGroup = nIndex;
	
	m_pChenkBox->SetCheck(false);

	m_pList->RemoveAll();
	int nGroupID = m_mapGroupIdToControls[nIndex];
	for (int nIndex = 0; nIndex< (int)m_vtRoomClient.size(); nIndex++)
	{
		if (m_vtRoomClient[nIndex].m_nGroupID == nGroupID)
		{
			// 分组下面的好友  
			CString strName,strGroup;
			strName.Format("%s", m_vtRoomClient[nIndex].m_szUPID);
			strGroup = m_mapSelfGroups[m_vtRoomClient[nIndex].m_nGroupID];

			AddList(strName, strGroup);
		}
	}
	return;
}

void CUserMrgDialog::LoadSelfGroup()
{
	if (IsAllGroupLoad())
	{
		for (int n=0; n<=USERMRG_FENZU; n++)
		{
			if (m_groupInfoStc[n].bLoad == false && m_groupInfoStc[n].bUsed == true && m_groupInfoStc[n].bGetUserInfo == true)
			{
				int nThisGroupNum = GetGroupClientNums(m_groupInfoStc[n].nGroupID);
				int nControlID = AddGroupInfo(m_groupInfoStc[n].GroupName, nThisGroupNum);
				m_mapGroupIdToControls[nControlID] = m_groupInfoStc[n].nGroupID;
				m_groupInfoStc[n].bLoad = true;
			}
		}
	}
	return;
}

void CUserMrgDialog::AddGroup(CString strGroupName ,CString strGroupID )
{
	m_mapSelfGroups.insert(map<int,CString>::value_type(atoi(strGroupID.GetBuffer()),strGroupName)) ;	
	m_groupInfoStc[nGroupIndex].bUsed = 1;
	m_groupInfoStc[nGroupIndex].nGroupID = atoi(strGroupID.GetBuffer());
	m_groupInfoStc[nGroupIndex].GroupName = strGroupName;
	m_groupInfoStc[nGroupIndex].bGetUserInfo = TRUE ; 

	int nThisGroupNum = GetGroupClientNums(m_groupInfoStc[nGroupIndex].nGroupID);
	int nControlID = AddGroupInfo(m_groupInfoStc[nGroupIndex].GroupName, nThisGroupNum);
	m_mapGroupIdToControls[nControlID] = m_groupInfoStc[nGroupIndex-1].nGroupID;
}
void CUserMrgDialog::DelGroup(CString strGroupName ,CString strGroupID ) 
{
	int groupID = atoi(strGroupID.GetBuffer()) ;
	strGroupID.ReleaseBuffer() ; 

	// 在 m_mapSelfGroups中找到strGroupID 并删除 
	std::map<int ,CString>::iterator itBegin,itEnd  ; 
	itBegin  = m_mapSelfGroups.begin() ; 
	while( itBegin !=  m_mapSelfGroups.end() )
	{
		if( itBegin->first == groupID )
		{
			// 删除该项 
			m_mapSelfGroups.erase(itBegin) ;
			break ; 
		}
		else
			itBegin++ ; 
	}

	// 在m_groupInfoStc 中找到strGroupName 并删除 
	for (int n=0; n<=USERMRG_FENZU; n++)
	{
		if ( m_groupInfoStc[n].bUsed == true )
		{
			if( m_groupInfoStc[n].nGroupID == groupID )
			{
				m_groupInfoStc[n].bLoad = false ;
				m_groupInfoStc[n].bUsed = false ;
				m_groupInfoStc[n].bGetUserInfo = false ; 
				m_groupInfoStc[n].bCreateNode = false ; 
				m_groupInfoStc[n].GroupName = _T("") ; 
				break ; 
			}
		}
	}
	// 处理m_mapGroupIdToControls  维护GroupID与后台数据库组ID之间的关系  也要处理
	std::map<int, int>::iterator itMapBegin ,itMapEnd ; 
	itMapBegin =  m_mapGroupIdToControls.begin() ; 
	
	while( itMapBegin !=  m_mapGroupIdToControls.end() )
	{
		if( itMapBegin->second == groupID )
		{
			m_mapGroupIdToControls.erase(itMapBegin) ; 
			break; 
		}
		else
		{
			itMapBegin++ ;
		}
	}

	DelGroupInfo(strGroupName,0) ; 

}
void CUserMrgDialog::DelGroupInfo(CString strGroupName, int nNums) 
{
	// 更新下拉列表
	//CDuiString str = m_pGroupList->GetText() ; 
	m_pGroupList->RemoveAll() ;
	int n = 0; 
	while( n < USERMRG_FENZU)
	{
		if( m_groupInfoStc[n].bUsed )
		{
			CString strGroupName = m_groupInfoStc[n].GroupName ; 
			CListLabelElementUI *pLabel = new CListLabelElementUI;
			pLabel->SetAttribute(_T("text"), strGroupName);
			pLabel->SetAttribute(_T("height"), _T("22"));
			// 下拉列表 
			m_pGroupList->Add(pLabel);
		}
		n++;
	}
	m_pGroupList->SetText(_T("未分组客户")) ;
}

void CUserMrgDialog::OnCheckBoxSelected()
{
	bool bSelect = m_pChenkBox->GetCheck();
	int nAllClomum = m_pList->GetCount();
	if (bSelect)
	{
		for (int nListIndex = 0; nListIndex < nAllClomum; nListIndex ++)
		{
			CListContainerElementUI* pLine = m_pList->GetListItem(nListIndex);
			if (pLine)
			{
				CCheckBoxUI * pCheckBox = (CCheckBoxUI *)pLine->FindSubControl(_T("selectme"));
				CTextUI * pText_ID = (CTextUI *)pLine->FindSubControl(_T("list_1"));
				CTextUI * pText_Group = (CTextUI *)pLine->FindSubControl(_T("list_2"));
				if (pCheckBox)
				{
					pCheckBox->SetCheck(true);
				}
			}
		}
	}
	else
	{
		for (int nListIndex = 0; nListIndex < nAllClomum; nListIndex ++)
		{
			CListContainerElementUI* pLine = m_pList->GetListItem(nListIndex);
			if (pLine)
			{
				CCheckBoxUI * pCheckBox = (CCheckBoxUI *)pLine->FindSubControl(_T("selectme"));
				CTextUI * pText_ID = (CTextUI *)pLine->FindSubControl(_T("list_1"));
				CTextUI * pText_Group = (CTextUI *)pLine->FindSubControl(_T("list_2"));
				if (pCheckBox)
				{
					pCheckBox->SetCheck(false);
				}
			}
		}
	}
}

void CUserMrgDialog::OnBtn_AddGroup(TNotifyUI& msg)
{
	
	// 如果分组大于10  提示消息 
	if( nGroupIndex >= 10 )
	{
		::MessageBox(NULL, _T("自定义客户分组不能超过10个！"), _T("提示"), MB_OK);
		return ;
	}

	CUserMrgAddGroup* pAddGroupDialog = new CUserMrgAddGroup();
	if( pAddGroupDialog == NULL )
		return;

	pAddGroupDialog->Create(m_hWnd, "添加联系人", UI_WNDSTYLE_FRAME | WS_POPUP, NULL, 0, 0, 600, 800);
	pAddGroupDialog->CenterWindow();
	pAddGroupDialog->ShowModal();

	
	//////////////////////////////////////////////////////////////////////////
	if (!g_strAddGroupName.IsEmpty())
	{
		//////////////////////////////////////////////////////////////////////////
		WORD iGID = 0 ; // 组ID 
		CString strJson = _T("") ; 
		strJson.Format("{\"type\"\:\"1\" ,\"name\"\:\"\%s\",\"gID\"\:\"%d\"}",g_strAddGroupName,iGID) ; 
		string strAddGroup(strJson.GetBuffer()) ;
		strJson.ReleaseBuffer() ; 
		g_MyClient.SendSetSelfGroupReq(strAddGroup) ;
	}
	return;
}

bool CUserMrgDialog::DeleteFenZuBtn( int btnID ) 
{
	int i = nGroupIndex ; 

	ANALYSTGROUP_INFO temp_groupInfoStc[USERMRG_FENZU+1]; 
	//temp_groupInfoStc = m_groupInfoStc ; 
	for (int n=0; n<=USERMRG_FENZU; n++)
	{
		if (m_groupInfoStc[n].bUsed == true)
		{
			CString name = m_groupInfoStc[n].GroupName ; 

			m_groupInfoStc[n].bLoad = false ;
			m_groupInfoStc[n].bUsed = true ;
			//temp_groupInfoStc[n].GroupName = m_groupInfoStc[n].GroupName ;
			m_groupInfoStc[n].bCreateNode = true ;
			//temp_groupInfoStc[n].nGroupID = m_groupInfoStc[n].nGroupID ;
			
			m_mapGroupIdToControls[n] = m_groupInfoStc[n].nGroupID;
		}
	}
	std::map<int, int>	tempGroupIdToControls = m_mapGroupIdToControls ; 

	m_mapGroupIdToControls.clear() ; 
	//memset(m_groupInfoStc ,0,sizeof(ANALYSTGROUP_INFO)) ; 

	LoadSelfGroup() ; 
	std::map<int,int>::iterator itBegin ,itEnd ; 
	itBegin = m_mapGroupIdToControls.find(btnID) ; 
	itEnd = m_mapGroupIdToControls.end()  ; 
	if( itBegin != itEnd )
	{
		m_mapGroupIdToControls.erase(btnID) ; 

		m_groupInfoStc[btnID].bUsed = 0;
		m_groupInfoStc[btnID].bGetUserInfo = 0;
		return true ; 
	}
	return false ; 
}
// 删除分组 
void CUserMrgDialog::OnBtn_DelGroup(TNotifyUI& msg)
{
	// 获取要删除的GroupID和name  
	// (1) GroupID 
	int nDelGroupID = m_mapGroupIdToControls[nChooseGroup]; 
	// (2) 
	
	// 未定义分组 和 全部客户 不能删除 
	if( nDelGroupID == 0 || nDelGroupID == -1 )
		return ; 

	// 获取分组列表名称 
	string strName = m_pFenZu[nChooseGroup]->GetText().GetData() ;
	int strlength = strName.length() ; 

	CString strJson = _T("") ; 
	strJson.Format("{\"type\"\:\"0\" ,\"name\"\:\"\%s\",\"gID\"\:\"%d\"}",strName.c_str(),nDelGroupID) ; 
	string strDelGroup(strJson.GetBuffer()) ;
	strJson.ReleaseBuffer() ; 
	if(!strName.empty())
	{
		// 去掉括号
		int indexStart = strName.find("(") ;
		int indexEnd   = strName.find(")") ;
		string strNumInName = strName.substr(indexStart+1 ,indexEnd - indexStart-1) ; 
		int nNumInName = atoi(strNumInName.c_str()) ; 
		if( nNumInName == 0 )
		{
			// 直接删除btn 
			m_pFenZu[nChooseGroup]->SetVisible(FALSE) ; 
		}
		nGroupIndex = nGroupIndex - 1 ; 
	}

	Reader reader ;
	Value  data_add_value ; 
	Value  data_del_value ; 
	Value  item ; 
	Value  array ; 
	FastWriter writer ; 
	string strdata ;

	// json包 { "groupID":"XXX" ,"nType":"XXX" ,"item":[{"clientID":"XXX"},...,{"clientID":"XXX"}]}
	data_add_value["groupID"] = nDelGroupID ; 
	data_add_value["nType"]   = 0 ; 

	int delNum = 0 ; 
	// 如果分组下面有人，将全部的人移到未分组下面 并将新分组信息发送到服务器 
	for (int nIndex = 0; nIndex< (int)m_vtRoomClient.size(); nIndex++)
	{
		if (m_vtRoomClient[nIndex].m_nGroupID == nDelGroupID)
		{
			delNum++ ;
			// 分组下面的好友  
			CString strName,strGroup;
			strName.Format("%s", m_vtRoomClient[nIndex].m_szUPID);
			strGroup = m_mapSelfGroups[m_vtRoomClient[nIndex].m_nGroupID];
			AddList(strName, _T("未分组客户"));
		}
	}
	m_pFenZu[nChooseGroup]->SetVisible(FALSE) ;
	
	// 获取未分组客户  ID  
	int nContorlID = ConvertGroupIDToControl(0) ; 
	UpdateListBtnName(nContorlID,delNum,1) ;
	
	//DeleteFenZuBtn(nChooseGroup) ; 
	g_MyClient.SendSetSelfGroupReq(strDelGroup) ;
}

void CUserMrgDialog::OnCbo_SelChange_Group(TNotifyUI& msg)
{
	tstring strText = (TCHAR *)m_pGroupList->GetText().GetData();

	// 获取选中分组的ID 
	int iGroupID = GetGroupIDByName(m_pGroupList->GetText().GetData()) ; 
	
	// nChooseGroup是左边的分组列表中当前选中的列表ID，也就是将nChooseGroup分组中选中的用户移动到
	// IGroupID去，最后nChooseGroup和IGroupID分组的标题都要更新。

	//////////////////////////////////////////////////////////////////////////
	// 检测那些条目是选中的
	int nAllClomum = m_pList->GetCount();

	// 选择的项 
	std::vector<int> chooseItemRemove(nAllClomum,-1) ; 
	int removeCount = 0 ; 

	int nListIndex = 0 ; 

	Reader reader ;
	Value  data_add_value ; 
	Value  data_del_value ; 
	Value  item ; 
	Value  array ; 
	FastWriter writer ; 
	string strdata ; 
	int nIndex = 0 ;

	// 为了避免溢出，当有50条数据时就发送socket给服务器 
	int iMaxSendMsgCount = 0 ; 

	for (; nListIndex < nAllClomum; nListIndex ++)
	{
		CListContainerElementUI* pLine = m_pList->GetListItem(nListIndex);
		if (pLine)
		{
			CCheckBoxUI * pCheckBox = (CCheckBoxUI *)pLine->FindSubControl(_T("selectme"));
			CTextUI * pText_ID = (CTextUI *)pLine->FindSubControl(_T("list_1"));
			CTextUI * pText_Group = (CTextUI *)pLine->FindSubControl(_T("list_2"));
			//            源 组ID                操作类型           移除的成员 
			//   操作类型：
			//   0 表示 将客户成员从组ID中移出 
			//   1 表示 将客户成员添加到组ID中
			// json包 { "groupID":"XXX" ,"nType":"XXX" ,"item":[{"clientID":"XXX"},...,{"clientID":"XXX"}]}
			data_add_value["groupID"] = m_mapGroupIdToControls[nChooseGroup] ; 
			data_add_value["nType"]   = 0 ; 

			data_del_value["groupID"]   = iGroupID ; 
			data_del_value["nType"]   = 1 ;

			if (pCheckBox->GetCheck())
			{
				pText_Group->SetText(strText.c_str()) ; 
				CString strCustomID = pText_ID->GetText() ;  // 客户ID 
				CString strCustomGroupName = pText_Group->GetText() ;  // 组名称
				// 在m_vtRoomClient找到该用户，并且将原来所在的list中删除该项  
				int iSize = m_vtRoomClient.size() ;
				
 
				for( int i = 0 ; i < iSize ; i++)
				{
					if( strcmp(m_vtRoomClient[i].m_szUPID ,strCustomID.GetBuffer()) == 0 )
					{
						m_vtRoomClient[i].m_nGroupID = iGroupID ;

						item["clientID"] = (LPCSTR)(strCustomID) ; 
						array.append(item) ;

						data_add_value["item"] = array ; 
						data_del_value["item"] = array ; 

						iMaxSendMsgCount++;
						// 删除该项
						// (1) 如果在全部客户下面，则不删除用户，但是将分组号设置为实际分组 
						if( nGroupIndex == -1)
						{
							pText_ID->SetText(m_pGroupList->GetText()) ;
						}
						// (2) 否则 删除这个分组里面的客户
						else  
						{
							chooseItemRemove.push_back(nListIndex) ;
							removeCount++ ; 
						}

						if( iMaxSendMsgCount >= 50 )
						{
							strdata = writer.write(data_add_value) ; 
							g_MyClient.SendSetSelfGroupUserReq(strdata) ; 

							strdata.clear() ; 
							strdata = writer.write(data_del_value) ;
							g_MyClient.SendSetSelfGroupUserReq(strdata) ; 

							iMaxSendMsgCount = 0 ;
							array.clear() ;

						}

					}
				}
				pCheckBox->SetCheck(false);
			}
			else
			{
				//::MessageBox(NULL, _T("请选择要移动的客户"), _T("提示"), MB_OK);
				//return ; 
			}
		}
	}

	// 发送小于50项的数据  
	if ( iMaxSendMsgCount > 0 )
	{
		strdata = writer.write(data_add_value) ; 
		g_MyClient.SendSetSelfGroupUserReq(strdata) ; 

		strdata.clear() ; 
		strdata = writer.write(data_del_value) ;
		g_MyClient.SendSetSelfGroupUserReq(strdata) ; 
	}

	if( nGroupIndex != -1)
	{
		// 从未分组中移除
		RemoveCustomFromGroup(chooseItemRemove,removeCount) ; 
		// 将分组ID 转化为 控件的ID  
		int nContorlID = ConvertGroupIDToControl(iGroupID) ; 
		
		// 更新左边对应分组的名称： XXXX(iCustomCount) 
		if( nContorlID != -1 )
			UpdateListBtnName(nContorlID,removeCount,1) ; 
		if( nChooseGroup != -1)
			UpdateListBtnName(nChooseGroup,removeCount,0) ; 
	}
	m_pChenkBox->SetCheck(false);
	return;
}
// nType表示向分组增加还是移除成员
// 1 表示增加成员
// 0 表示移除成员 
void CUserMrgDialog::UpdateListBtnName(int nBtnID,int iCustomCount,int nType)
{
	string strName = m_pFenZu[nBtnID]->GetText().GetData() ;
	int strlength = strName.length() ; 

	if(!strName.empty())
	{
		// 去掉括号
		int indexStart = strName.find("(") ;
		int indexEnd   = strName.find(")") ;

		// 没找到 
		if( ( indexEnd == -1 )|| ( indexStart == -1 ))
			return ; 
		
		int iCustomerInFenzu = atoi( (strName.substr(indexStart+1,indexEnd-indexStart-1)).c_str()) ; 
		string name = strName.substr(0,indexStart ) ;
		CString updateName=_T("");
		if( nType == 1 )
		{
			iCustomerInFenzu = iCustomCount + iCustomerInFenzu ; 
		}
		else if (nType == 0 )
		{
			iCustomerInFenzu = iCustomerInFenzu - iCustomCount ;
			if( iCustomerInFenzu < 0 )
				iCustomerInFenzu = 0 ; 
		}
		updateName.Format("%s(%d)",name.c_str(),iCustomerInFenzu) ; 
		m_pFenZu[nBtnID]->SetText(updateName) ; 
	}
}
void  CUserMrgDialog::RemoveCustomFromGroup(std::vector<int> vec ,int len ) 
{
	int iCount = vec.size() ; 

	for( int i = 0 ; i < iCount ;i++)
	{
		if( vec[i] != -1 )
		{
			m_pList->RemoveAt(vec[i]) ;
		}
	}
}

//好友管理器中客户分组中，根据分组ID得到控件btnID
int  CUserMrgDialog::ConvertGroupIDToControl(int nGroupID) 
{
	int nControlID = 0 ; 
	std::map<int,int>::iterator itBegin ,itEnd ; 

	itBegin = m_mapGroupIdToControls.begin() ;
	itEnd   = m_mapGroupIdToControls.end() ;
	while(itBegin!=itEnd)
	{
		if( (itBegin)->second == nGroupID )
		{
			nControlID = itBegin->first ;
			return nControlID ; 
		}
		itBegin++ ; 
	}
	return nControlID ; 
}

void CUserMrgDialog::OnFinalMessage(HWND hWnd)
{
	WindowImplBase::OnFinalMessage(hWnd);	
	delete this;
}

CDuiString CUserMrgDialog::GetSkinFile()
{
	return _T("dlg_UserMrg.xml");
}

CDuiString CUserMrgDialog::GetSkinFolder()
{
	return CDuiString(CPaintManagerUI::GetInstancePath()) + _T("skin\\");
}

LRESULT CUserMrgDialog::OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	bHandled = FALSE;
	for (int n=0; n<=USERMRG_FENZU; n++)
	{
		m_groupInfoStc[n].bLoad = false;
	}
	return 0;
}

LRESULT CUserMrgDialog::OnSetFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	bHandled = FALSE;
	return 0;
}

LRESULT CUserMrgDialog::OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	BOOL bZoomed = ::IsZoomed(m_hWnd);
	LRESULT lRes = CWindowWnd::HandleMessage(uMsg, wParam, lParam);
	if (::IsZoomed(m_hWnd) != bZoomed)
	{
		if (!bZoomed)
		{
			if (m_pMaxSysBtn != NULL)
				m_pMaxSysBtn->SetVisible(false);
			if (m_pRestoreSysBtn != NULL)
				m_pRestoreSysBtn->SetVisible(true);
		}
		else 
		{
			if (m_pMaxSysBtn != NULL)
				m_pMaxSysBtn->SetVisible(true);
			if (m_pRestoreSysBtn != NULL)
				m_pRestoreSysBtn->SetVisible(false);
		}
	}
	return 0;
}

LRESULT CUserMrgDialog::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	// 屏蔽标题栏双击消息
	if(WM_NCLBUTTONDBLCLK != uMsg)
		return __super::HandleMessage(uMsg, wParam, lParam);
	return 0;
}

LRESULT CUserMrgDialog::HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	bHandled = FALSE;
	return 0;
}

LRESULT CUserMrgDialog::ResponseDefaultKeyEvent(WPARAM wParam)
{
	if (wParam == VK_RETURN)
	{
		return FALSE;
	}
	else if (wParam == VK_ESCAPE)
	{
		return TRUE;
	}
	return FALSE;
}

void CUserMrgDialog::OnTimer(TNotifyUI& msg)
{
}

void CUserMrgDialog::OnExit()
{
	Close();
}

void CUserMrgDialog::OnPrepare(TNotifyUI& msg)
{
	TCHAR szBuf[MAX_PATH] = {0};

	if (CreateType == eCreate_TG1 || CreateType == eCreate_TG2)
		return;
}

void CUserMrgDialog::Notify(TNotifyUI& msg)
{
	if (_tcsicmp(msg.sType, _T("windowinit")) == 0)
	{
		OnPrepare(msg);
	}
	else if (_tcsicmp(msg.sType, _T("killfocus")) == 0)
	{
	}
	else if (_tcsicmp(msg.sType, _T("click")) == 0)
	{
		CControlUI *pOne = static_cast<CControlUI*>(m_PaintManager.FindControl(msg.ptMouse)) ; 
		if (msg.pSender == m_pMinSysBtn)
			OnBtn_Min(msg);
		else if (msg.pSender == m_pMaxSysBtn)
			OnBtn_Max(msg);
		else if (msg.pSender == m_pRestoreSysBtn)
			OnBtn_Restore(msg);
		else if (msg.pSender == m_pCloseSysBtn)
			OnBtn_Close(msg);
		else if (msg.pSender == m_pUserMrgAddBtn)
			OnBtn_AddGroup(msg);
		else if( msg.pSender == m_pUserMrgDelBtn)
			OnBtn_DelGroup(msg) ; 
		else if (msg.pSender == m_pAllClient)
			OnBtn_AllClient(msg);

		for (int nIndex = 0; nIndex<=USERMRG_FENZU; nIndex++)
		{
			// 如果是分组
			if (msg.pSender == m_pFenZu[nIndex])
				ChangeFenZu(nIndex);
		}

		if (_tcsicmp(msg.pSender->GetName(), kCloseControlName) == 0)
		{
			OnExit();
		}
		else if (_tcsicmp(msg.pSender->GetName(), kCloseButtonControlName) == 0)
        {
            OnCloseBtnClick(msg);
		}
		else if (_tcsicmp(msg.pSender->GetName(), kOKButtonControlName) == 0)
		{
			OnOKBtnClick(msg);
		}
	}
    else if( _tcsicmp(msg.sType, _T("return")) == 0 ) 
    {
    }
	else if (_tcsicmp(msg.sType, _T("timer")) == 0)
	{
		return OnTimer(msg);
	}
	else if (_tcsicmp(msg.sType, _T("selectchanged")) == 0)
	{
		if (msg.pSender == m_pChenkBox)
		{
			// checkbox选择改变
			OnCheckBoxSelected();
		}
	}
	else if (_tcsicmp(msg.sType, _T("itemselect")) == 0)
	{
		if (msg.pSender == m_pGroupList)
			OnCbo_SelChange_Group(msg);
	}
}

void CUserMrgDialog::OnBtnFenzuList(TNotifyUI& msg, CString strName)
{
	COptionUI* m_btnFenZu = static_cast<COptionUI*>(m_PaintManager.FindControl(strName));
	if( m_btnFenZu)
	{
		g_strDelGroupName = m_btnFenZu->GetText() ;
		// 
	}
}
//最小化按钮
void CUserMrgDialog::OnBtn_Min(TNotifyUI& msg)
{
	SendMessage(WM_SYSCOMMAND, SC_MINIMIZE, 0);
}

//最大化按钮
void CUserMrgDialog::OnBtn_Max(TNotifyUI& msg)
{
	SendMessage(WM_SYSCOMMAND, SC_MAXIMIZE, 0);
}

//还原窗口按钮
void CUserMrgDialog::OnBtn_Restore(TNotifyUI& msg)
{
	SendMessage(WM_SYSCOMMAND, SC_RESTORE, 0);
}

//关闭窗口按钮
void CUserMrgDialog::OnBtn_Close(TNotifyUI& msg)
{
	Close();
}

void CUserMrgDialog::OnCloseBtnClick(TNotifyUI& msg)
{
	OnExit();
	return;
}

void CUserMrgDialog::OnOKBtnClick(TNotifyUI& msg)
{
	return;
}
