#include "stdafx.h"
#include "../Func.h"
#include "HistoryMsg.h"
#include "ChatDialog.h"
#include "../globalvar.h"

const TCHAR* const kTitleControlName			= _T("historymsg_title");
const TCHAR* const kBackgroundControlName		= _T("historymsg_bg");
const TCHAR* const kMinButtonControlName		= _T("historymsg_minbtn");
const TCHAR* const kMaxButtonControlName		= _T("historymsg_maxbtn");
const TCHAR* const kRestoreButtonControlName	= _T("historymsg_restorebtn");
const TCHAR* const kCloseButtonControlName		= _T("historymsg_closebtn");
const TCHAR* const kListControlName				= _T("historymsg_list");
const TCHAR* const kRichEditControlName			= _T("historymsg_view_richedit");

CHistoryMsgDialog::CHistoryMsgDialog(const CDuiString& bgimage, DWORD bkcolor)
	: bgimage_(bgimage)
	, bkcolor_(bkcolor)
{
	pGroupsList = NULL;
	m_pHisMsgEdit = NULL;
}

CHistoryMsgDialog::~CHistoryMsgDialog()
{
	return;
}

LPCTSTR CHistoryMsgDialog::GetWindowClassName() const
{
	return _T("UPGuiFoundation");
}

CControlUI* CHistoryMsgDialog::CreateControl(LPCTSTR pstrClass)
{
	if (_tcsicmp(pstrClass, _T("MemberList")) == 0)
	{
		return new CGroupsUI(m_PaintManager, eUI_Member);
	}
	return NULL;
}

void CHistoryMsgDialog::InitWindow()
{
	pGroupsList = static_cast<CGroupsUI*>(m_PaintManager.FindControl(kListControlName));
	m_pHisMsgEdit = static_cast<CRichEditUI*>(m_PaintManager.FindControl(kRichEditControlName));
	LoadMemList();

	m_FaceList.LoadConfigFile(g_config.szFaceConfig);
}

void CHistoryMsgDialog::CreateOtherNode()
{
	if (!pGroupsList)
		return;
	// 添加一个根结点“其他”
	FriendListItemInfo item_node;
	CDuiString mGroupID = _T("");
	mGroupID.Format("DEFAULT$%d", 2);
	item_node.id = mGroupID;
	item_node.folder = true;
	item_node.empty = false;
	item_node.nick_name = _T("其他");
	item_node.showname = _T("其他");
	Node* myGroupptr = NULL;
	myGroupptr = pGroupsList->AddNode(item_node, NULL);

	//
	return;
}

void CHistoryMsgDialog::CreateRoomNode()
{
	if (!pGroupsList)
		return;
	// 添加一个根结点“我的群组”
	FriendListItemInfo item_node;
	CDuiString mGroupID = _T("");
	mGroupID.Format("DEFAULT$%d", 1);
	item_node.id = mGroupID;
	item_node.folder = true;
	item_node.empty = false;
	item_node.nick_name = _T("我的群组");
	item_node.showname = _T("我的群组");
	Node* myGroupptr = NULL;
	myGroupptr = pGroupsList->AddNode(item_node, NULL);
	
	int n = m_mapChatRoom.size();
	if (n > 0)
	{
		GCR_ITER iterator;
		for (iterator = m_mapChatRoom.begin(); iterator != m_mapChatRoom.end() ; iterator++)
		{
			FriendListItemInfo item;
			item.folder = false;
			item.logo = _T("groups.png");
			item.nodetype = eNode_Group;

			CDuiString mRoomID = _T("");
			mRoomID.Format("%d", iterator->second.m_nRoomId);
			item.id = mRoomID;
			item.numid = IntToStr(iterator->second.m_nRoomId);
			
			item.nick_name = iterator->second.m_szRoomName;
			item.showname =  iterator->second.m_szRoomName;
			item.groupnote = iterator->second.m_szNote;
			item.roomtype = iterator->second.m_nRoomType;
			
			if (myGroupptr)
				pGroupsList->AddNode(item, myGroupptr);
		}
	}
}

void CHistoryMsgDialog::CreateGroupNode()
{
	if (!pGroupsList)
		return;
	FriendListItemInfo item;
	int n = m_vtUserGroup.size();
	for (std::vector<USER_GROUP_INFO>::iterator iterator = m_vtUserGroup.begin(); iterator != m_vtUserGroup.end() ; ++iterator)
	{
		CDuiString mGroupID = _T("");
		mGroupID.Format("GROUP$%d",iterator->m_nGroupID);

		item.numid = IntToStr(iterator->m_nGroupID);
		item.id = mGroupID;
		item.folder = true;
		item.empty = false;
		Node* retrunptr = NULL;
		item.nick_name = iterator->m_szGroupName;
		item.showname =  iterator->m_szGroupName;
		if (iterator->m_nParentID == 0)
		{
			retrunptr = pGroupsList->AddNode(item, NULL);
		}
		else
		{
			CString mPreGroupID = _T("");
			mPreGroupID.Format("GROUP$%d",iterator->m_nParentID);

			Node* m_mode = pGroupsList->SelectNodeByID(mPreGroupID);
			if (m_mode)
				retrunptr = pGroupsList->AddNode(item, m_mode);
		}
	}
}

void CHistoryMsgDialog::CreateDeptNode()
{
	if (!pGroupsList)
		return;
	FriendListItemInfo item;
	int n = m_vtUserGroup.size();
	for (std::vector<USER_GROUP_INFO>::iterator iterator = m_vtDeptInfo.begin(); iterator != m_vtDeptInfo.end(); ++iterator)
	{
		CDuiString mGroupID = _T("");
		mGroupID.Format("DEPT$%d",iterator->m_nGroupID);

		item.numid = IntToStr(iterator->m_nGroupID);
		item.id = mGroupID;
		item.folder = true;
		item.empty = false;
		Node* retrunptr = NULL;
		item.nick_name = iterator->m_szGroupName;
		item.showname =  iterator->m_szGroupName;
		if (iterator->m_nParentID == 0)
		{
			retrunptr = pGroupsList->AddNode(item, NULL);
		}
		else
		{
			CString mPreGroupID = _T("");
			mPreGroupID.Format("DEPT$%d",iterator->m_nParentID);
			Node* m_mode = pGroupsList->SelectNodeByID(mPreGroupID);
			if (m_mode)
				retrunptr = pGroupsList->AddNode(item, m_mode);
		}
	}
}

void CHistoryMsgDialog::CreateFriendNode()
{
	if (!pGroupsList)
		return;
	for (int idex=0; idex<(int)m_vtFriendInfo.size(); idex++)
	{
		CString mGroupID = _T("");
		FriendListItemInfo item;
		Node* retrunptr = NULL;

		if (m_vtFriendInfo[idex].type == 4)
			mGroupID.Format("DEPT$%d", m_vtFriendInfo[idex].relation);
		else
			mGroupID.Format("GROUP$%d", m_vtFriendInfo[idex].relation);

		Node* m_mode = pGroupsList->SelectNodeByID(mGroupID);
		if (m_mode)
		{
			item.folder = false;
			item.logo = _T("default.png");

			item.nodetype = eNode_Single;
			item.id = m_vtFriendInfo[idex].userid;
			item.numid = m_vtFriendInfo[idex].userid;
			item.Parentid = mGroupID;

			item.nick_name = m_vtFriendInfo[idex].nickname;
			item.showname.Format("%s(%s)", m_vtFriendInfo[idex].nickname, m_vtFriendInfo[idex].userid);
			retrunptr = pGroupsList->AddNode(item, m_mode);
		}
	}
}


// 分析师登录之后看见的房间节点
void CHistoryMsgDialog::CreateTgRoomNode()
{
	if (pGroupsList != NULL)
	{
		FriendListItemInfo item;
		int m_nNodeID = 0;
		CDuiString mRoomID = _T("");
		CDuiString m_szGroupName = _T("");
		if (g_MyClient.m_nUserType == eUser_UPAnalyst)
			m_szGroupName = _T("分析师互动");

		mRoomID.Format("ANALYST$%d", m_nNodeID);
		Node * hasNode = pGroupsList->SelectNodeByID(mRoomID.GetData());
		if (hasNode == NULL)
		{
			item.numid = IntToStr(m_nNodeID);
			item.id = mRoomID;
			item.folder = true;
			item.empty = false;
			item.nick_name = m_szGroupName;
			item.showname =  m_szGroupName;
			item.nodetype = eNode_Analyst;
			item.type = eNode_Analyst;

			Node* retrunptr = NULL;
			retrunptr = pGroupsList->AddNode(item, NULL);
			if (retrunptr)
				pGroupsList->SetChildVisible(retrunptr, true);

			USER_LOG("CreateANALYST[%d] NAME:%s GroupID:%s", m_nNodeID, m_szGroupName.GetData(), mRoomID.GetData());
		}
	}
}

//
void CHistoryMsgDialog::AddTgRoomAccNode()
{
	if (pGroupsList != NULL)
	{
		CString mRoomID = _T("ANALYST$0");
		FriendListItemInfo item;
		Node* retrunptr = NULL;

		Node* m_mode = pGroupsList->SelectNodeByID(mRoomID);
		if (m_mode)
		{
			int nTgRoomNum = m_mapTgRoomInfo.size();
			TR_ITER iterator;
			for (iterator = m_mapTgRoomInfo.begin(); iterator != m_mapTgRoomInfo.end(); iterator++)
			{
				USER_LOG("CreateTgRoomAccNode PID:%d PNAME:%s PNOTE:%s", iterator->second.nRoomID, iterator->second.szName, iterator->second.szInfo);

				CDuiString mTgRoomID = _T("");
				mTgRoomID.Format("ANALYST$%d", iterator->second.nRoomID);

				item.folder = true;
				item.empty = false;
				item.nodetype = eNode_Analyst;
				item.type = eNode_Analyst;
				item.id = mTgRoomID;
				item.numid = IntToStr(iterator->second.nRoomID);
				item.nick_name = iterator->second.szName;
				item.showname = iterator->second.szName;

				item.description = iterator->second.szInfo;
				retrunptr = pGroupsList->AddNode(item, m_mode);
				if (retrunptr)
					pGroupsList->SetChildVisible(retrunptr, true);

				//////////////////////////////////////////////////////////////////////////
				// 每个分析师房间下面显示历史联系人
				long Roomid = iterator->second.nRoomID;
				AddTgRoomClientNode(Roomid, retrunptr);
			}
		}
	}
}

void CHistoryMsgDialog::AddTgRoomClientNode(long nRoomID, Node* nodeptr /* = NULL */)
{
	if (nodeptr == NULL)
		return;

	std::vector<CString> vtUsers;
	if(!g_SqliteOp.QueryAllAnalystUser(vtUsers, eRecord_TgRoom, nRoomID))
		return ;

	for(int index=0; index < (int)vtUsers.size(); ++index)
	{
		CString strUser = vtUsers[index];
		if (strUser.CompareNoCase(g_MyClient.m_strUserid) == 0)
			;
		else
		{
			CDuiString mTgRoomID = _T("");
			mTgRoomID.Format("ANALYST$%d$%s", nRoomID, strUser);

			FriendListItemInfo item;
			item.logo = _T("default.png");
			item.nodetype = eNode_Analyst;
			item.type = eNode_Analyst;
			item.id = mTgRoomID;
			item.Parentid = IntToStr(nRoomID);
			item.numid = strUser;
			item.nick_name = strUser;

#ifdef NOT_DISPLAY_ID_LIKE_MOBLIE_NUM
			strUser = ConvertIdToNoPhonrNum(strUser);
#endif // NOT_DISPLAY_ID_LIKE_MOBLIE_NUM
			item.showname = strUser;

			pGroupsList->AddNode(item, nodeptr);
		}
	}
}

void CHistoryMsgDialog::LoadMemList()
{
	if (pGroupsList)
	{
		//////////////////////////////////////////////////////////////////////////
		// 分析师IM看见的历史消息
		if (g_MyClient.m_nUserType == eUser_UPAnalyst)
		{
			CreateTgRoomNode();
			AddTgRoomAccNode();
		}
		else
		{
			CreateGroupNode();
			CreateDeptNode();
			CreateFriendNode();

			CreateRoomNode();
			CreateOtherNode();
		}
	}
}

void CHistoryMsgDialog::OnFinalMessage(HWND hWnd)
{
	RemoveObserver();
	WindowImplBase::OnFinalMessage(hWnd);	
	delete this;
}

BOOL CHistoryMsgDialog::Receive(SkinChangedParam param)
{
	bgimage_ = param.bgimage;
	bkcolor_ = param.bkcolor;
	CControlUI* background = m_PaintManager.FindControl(kBackgroundControlName);
	if (background != NULL)
	{
		if (!param.bgimage.IsEmpty())
		{
			TCHAR szBuf[MAX_PATH] = {0};
			_stprintf_s(szBuf, MAX_PATH - 1, _T("file='%s' corner='600,200,1,1'"), param.bgimage);
			background->SetBkImage(szBuf);
		}
		else
			background->SetBkImage(_T(""));

		background->SetBkColor(param.bkcolor);
	}

	return TRUE;
}

CDuiString CHistoryMsgDialog::GetSkinFile()
{
	return _T("dlg_HistoryMsg.xml");
}

CDuiString CHistoryMsgDialog::GetSkinFolder()
{
	return CDuiString(CPaintManagerUI::GetInstancePath()) + _T("skin\\");
}

LRESULT CHistoryMsgDialog::OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	bHandled = FALSE;
	m_FaceList.Reset();
	return 0;
}

LRESULT CHistoryMsgDialog::OnSetFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	bHandled = FALSE;
	return 0;
}

LRESULT CHistoryMsgDialog::OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	BOOL bZoomed = ::IsZoomed(m_hWnd);
	LRESULT lRes = CWindowWnd::HandleMessage(uMsg, wParam, lParam);
	if (::IsZoomed(m_hWnd) != bZoomed)
	{
		if (!bZoomed)
		{
			CControlUI* pControl = static_cast<CControlUI*>(m_PaintManager.FindControl(kMaxButtonControlName));
			if( pControl ) pControl->SetVisible(false);
			pControl = static_cast<CControlUI*>(m_PaintManager.FindControl(kRestoreButtonControlName));
			if( pControl ) pControl->SetVisible(true);
		}
		else 
		{
			CControlUI* pControl = static_cast<CControlUI*>(m_PaintManager.FindControl(kMaxButtonControlName));
			if( pControl ) pControl->SetVisible(true);
			pControl = static_cast<CControlUI*>(m_PaintManager.FindControl(kRestoreButtonControlName));
			if( pControl ) pControl->SetVisible(false);
		}
	}
	return 0;
}

LRESULT CHistoryMsgDialog::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	return __super::HandleMessage(uMsg, wParam, lParam);
}

LRESULT CHistoryMsgDialog::HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	bHandled = FALSE;
	return 0;
}

LRESULT CHistoryMsgDialog::ResponseDefaultKeyEvent(WPARAM wParam)
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

void CHistoryMsgDialog::OnTimer(TNotifyUI& msg)
{

}

void CHistoryMsgDialog::OnExit()
{
	Close();
}

void CHistoryMsgDialog::OnPrepare(TNotifyUI& msg)
{
	TCHAR szBuf[MAX_PATH] = {0};

	CControlUI* background = m_PaintManager.FindControl(kBackgroundControlName);
	if (background != NULL)
	{
		_stprintf_s(szBuf, MAX_PATH - 1, _T("file='%s' corner='600,200,1,1'"), bgimage_);
		background->SetBkImage(szBuf);
		background->SetBkColor(bkcolor_);
	}
}

// List单击消息
void CHistoryMsgDialog::OnListItemClick(TNotifyUI& msg)
{
	if ((pGroupsList != NULL) &&  pGroupsList->GetItemIndex(msg.pSender) != -1)
	{
		if (_tcsicmp(msg.pSender->GetClass(), _T("ListContainerElementUI")) == 0)
		{
			Node* node = (Node*)msg.pSender->GetTag();

			if (pGroupsList->CanExpand(node)) // 如果是节点
			{
				pGroupsList->SetChildVisible(node, !node->data().child_visible_);
			}
			else // 如果是好友
			{
				m_pHisMsgEdit->SetText(_T(""));

				std::vector<CHAT_RECORD2> vtRecordset;
				int nStart = 0;
				int nEnd = 1000;
				CString m_strfromID;
				CString m_strToID;

				// 个人节点
				if (node->data().nodetype == eNode_Single)
				{
					m_strfromID.Format("%s", node->data().id.GetData());
					m_strToID.Format("%s", node->data().id.GetData());

					if(!g_SqliteOp.QueryOnePageRecord(vtRecordset, eRecord_Single, nStart, nEnd, m_strfromID, m_strToID))
						return ;

					if (vtRecordset.size() > 0)
						InsertHistoryMSG(vtRecordset, node->data().nickname.GetData(), node->data().id.GetData());
				}
				// 群组节点
				else if (node->data().nodetype == eNode_Group)
				{
					m_strToID.Format("%s", node->data().id.GetData());

					if(!g_SqliteOp.QueryOnePageRecord(vtRecordset, eRecord_Group, nStart, nEnd, m_strfromID, m_strToID))
						return ;

					if (vtRecordset.size() > 0)
						InsertHistoryMSG(vtRecordset, node->data().nickname.GetData(), node->data().id.GetData());
				}
				// 分析师节点
				else if (node->data().nodetype == eNode_Analyst)
				{
					m_strToID.Format("%s", node->data().numid.GetData());
					m_strfromID = g_MyClient.m_strUserid;
					long nRoomid = atol(node->data().Parentid);

					if(!g_SqliteOp.QueryOnePageRecord(vtRecordset, eRecord_TgRoom, nStart, nEnd, m_strfromID, m_strToID, FALSE, 0, nRoomid))
						return ;

					if (vtRecordset.size() > 0)
						InsertHistoryMSG(vtRecordset, node->data().nickname.GetData(), node->data().nickname.GetData());
				}
			}
		}
	}
}

void CHistoryMsgDialog::Notify(TNotifyUI& msg)
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
		if (_tcsicmp(msg.pSender->GetName(), kCloseButtonControlName) == 0)
			OnBtn_Close(msg);
		else if (_tcsicmp(msg.pSender->GetName(), kMinButtonControlName) == 0)
			OnBtn_Min(msg);
		else if (_tcsicmp(msg.pSender->GetName(), kMaxButtonControlName) == 0)
			OnBtn_Max(msg);
		else if (_tcsicmp(msg.pSender->GetName(), kRestoreButtonControlName) == 0)
			OnBtn_Restore(msg);
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
	}
	else if (_tcsicmp(msg.sType, _T("itemselect")) == 0)
	{
	}
	//CUIList双击消息
	else if (_tcsicmp(msg.sType, _T("itemactivate")) == 0)
	{
	}
	//CUIList单击消息
	else if (_tcsicmp(msg.sType, _T("itemclick")) == 0)
	{
		OnListItemClick(msg);
	}
}


//最小化按钮
void CHistoryMsgDialog::OnBtn_Min(TNotifyUI& msg)
{
	SendMessage(WM_SYSCOMMAND, SC_MINIMIZE, 0);
}

//最大化按钮
void CHistoryMsgDialog::OnBtn_Max(TNotifyUI& msg)
{
	SendMessage(WM_SYSCOMMAND, SC_MAXIMIZE, 0);
}

//还原窗口按钮
void CHistoryMsgDialog::OnBtn_Restore(TNotifyUI& msg)
{
	SendMessage(WM_SYSCOMMAND, SC_RESTORE, 0);
}

//关闭窗口按钮
void CHistoryMsgDialog::OnBtn_Close(TNotifyUI& msg)
{
	Close();
}

BOOL CHistoryMsgDialog::InsertHistoryMSG(std::vector<CHAT_RECORD2>& vtRecord, LPCTSTR szSendName, LPCTSTR szSendId)
{
	for(int index=0; index < (int)vtRecord.size(); ++index)
	{
		long nseq = vtRecord[index].seq;
		long senddate = vtRecord[index].senddate;
		long sendtime = vtRecord[index].sendtime;

		CDuiString strTime = "";
		CDuiString strspName = "";
		strTime.Format("%04d-%02d-%02d %02d:%02d:%02d", 
			senddate/10000, (senddate%10000)/100, senddate%100, sendtime/10000, (sendtime%10000)/100, sendtime%100);

		if (vtRecord[index].format == eMsgFormat_System)
			strspName = _T("系统消息");
		else
			strspName = szSendName;

		if (vtRecord[index].format == eMsgFormat_Def)
		{
			TiXmlDocument xmlDoc;
			xmlDoc.Parse(vtRecord[index].msg);

			if(xmlDoc.Error()) 
			{
				printf( "Error in %s: %s\n", xmlDoc.Value(), xmlDoc.ErrorDesc() );
				return FALSE;
			}

			CString strTmpFmt,strOld;
			TiXmlNode* pRootNode = xmlDoc.FirstChild("msg");
			if(!pRootNode) 	return FALSE;
			TiXmlElement* pRootElm =pRootNode->ToElement();
			if(!pRootElm) 	return FALSE;

			//////////////////////////////////////////////////////////////////////////
			CFontInfo m_ufontInfo;
			LPCSTR pszValue = "";
			pszValue = pRootElm->Attribute("f");				// 字体
			m_ufontInfo.m_strName = pszValue;
			pszValue = pRootElm->Attribute("b");				// 粗细
			m_ufontInfo.m_bBold = atoi(pszValue);
			pszValue = pRootElm->Attribute("i");				// 斜体
			m_ufontInfo.m_bItalic = atoi(pszValue);
			pszValue = pRootElm->Attribute("u");				// 下划线
			m_ufontInfo.m_bUnderLine = atoi(pszValue);
			pszValue = pRootElm->Attribute("s");				// 字体大小
			m_ufontInfo.m_nSize = atoi(pszValue);
			pszValue = pRootElm->Attribute("c");				// 颜色
			m_ufontInfo.m_clrText = atoi(pszValue);

			//////////////////////////////////////////////////////////////////////////

			TiXmlNode* pItemNode = pRootNode->FirstChild("text"); 
			if(!pItemNode) return FALSE;
			TiXmlNode* pTextNode = pItemNode->FirstChild();
			if(!pTextNode) return FALSE;

			CString strText = pTextNode->Value();

			if ( strcmp(g_MyClient.m_strUserid, vtRecord[index].fromid) == 0)
			{
				AddMsgToHisMSGEdit(strText, m_ufontInfo, true, (CDuiString)g_MyClient.m_strUserid, (CDuiString)g_MyClient.m_strUserName, strTime);
			}
			else
			{
				if (vtRecord[index].type == eRecord_Single)
					AddMsgToHisMSGEdit(strText, m_ufontInfo, false, szSendId, strspName, strTime);
				else if (vtRecord[index].type == eRecord_Group)
				{
					CDuiString strSendID = _T("");
					CDuiString strSendName = _T("");
					strSendID = vtRecord[index].fromid;
					strSendName = alluserNickname[strSendID];
					if (strSendName.IsEmpty())
						strSendName = strSendID;

					AddMsgToHisMSGEdit(strText, m_ufontInfo, false, strSendID.GetData(), strSendName.GetData(), strTime);
				}
				else if (vtRecord[index].type == eRecord_TgRoom)
				{
					CDuiString strSendID = _T("");
					CDuiString strSendName = _T("");
					strSendID = vtRecord[index].fromid;
					strSendName = alluserNickname[strSendID];
					if (strSendName.IsEmpty())
						strSendName = strSendID;

#ifdef NOT_DISPLAY_ID_LIKE_MOBLIE_NUM
					CString _strSendName = strSendName.GetData();
					_strSendName = ConvertIdToNoPhonrNum(_strSendName);
					strSendName = _strSendName;

					CString _strSendID = strSendID.GetData();
					_strSendID = ConvertIdToNoPhonrNum(_strSendID);
					strSendID = _strSendID;
#endif // NOT_DISPLAY_ID_LIKE_MOBLIE_NUM

					AddMsgToHisMSGEdit(strText, m_ufontInfo, false, strSendID.GetData(), strSendName.GetData(), strTime);
				}
			}
		}
	}
	return TRUE;
}

BOOL CHistoryMsgDialog::HandleSysFaceId_More(CRichEditUI * pRichEdit, LPCTSTR& p, tstring& strText, CFontInfo mfontInfo)
{
	CHAR *cStart = _T("[\"");
	CHAR *cEnd = _T("\"]");
	int nFaceId = GetBetweenInt(p+2, cStart, cEnd, -1);

	CFaceInfo * lpFaceInfo = m_FaceList.GetFaceInfoById(nFaceId);
	if (lpFaceInfo != NULL)
	{
		if (!strText.empty())
		{
			_RichEdit_ReplaceSel_More(pRichEdit, strText.c_str(), mfontInfo); 
			strText = _T("");
		}

		_RichEdit_InsertFace(pRichEdit, lpFaceInfo->m_strFileName.c_str(), 
			lpFaceInfo->m_nId, lpFaceInfo->m_nIndex);

		p = _tcsstr(p+2, _T("\"]"));
		p++;
		return TRUE;
	}
	return FALSE;
}

void CHistoryMsgDialog::_RichEdit_ReplaceSel_More(CRichEditUI * pRichEdit, LPCTSTR lpszNewText, CFontInfo mfontInfo)
{
	ITextServices * pTextServices = pRichEdit->GetTextServices();
	if (pRichEdit == m_pHisMsgEdit)
	{
		RichEdit_ReplaceSel(pTextServices, lpszNewText, 
			mfontInfo.m_strName.c_str(), mfontInfo.m_nSize, 
			mfontInfo.m_clrText, mfontInfo.m_bBold, mfontInfo.m_bItalic, 
			mfontInfo.m_bUnderLine, FALSE, 300);
	}
	else
	{
		RichEdit_ReplaceSel(pTextServices, lpszNewText);
	}
	pTextServices->Release();
}

BOOL CHistoryMsgDialog::_RichEdit_InsertFace(CRichEditUI * pRichEdit, LPCTSTR lpszFileName, int nFaceId, int nFaceIndex)
{
	BOOL bRet = FALSE;

	if (NULL == pRichEdit || NULL == lpszFileName || NULL == *lpszFileName)
		return FALSE;

	ITextServices * pTextServices = pRichEdit->GetTextServices();
	ITextHost * pTextHost = pRichEdit->GetTextHost();
	if (pTextServices != NULL && pTextHost != NULL)
	{
		if (pRichEdit == m_pHisMsgEdit)
			RichEdit_SetStartIndent(pTextServices, 300);
		bRet = RichEdit_InsertFace(pTextServices, pTextHost, 
			lpszFileName, nFaceId, nFaceIndex, RGB(255,255,255), TRUE, 40);
	}

	if (pTextServices != NULL)
		pTextServices->Release();
	if (pTextHost != NULL)
		pTextHost->Release();

	return bRet;
}

// "/f["系统表情id"]/s["系统表情index"]/c["自定义图片路径"]"
void CHistoryMsgDialog::AddMsg_More(CRichEditUI * pRichEdit, LPCTSTR lpText, CFontInfo mfontInfo)
{
	if (NULL == pRichEdit || NULL == lpText || NULL == *lpText)
		return;

	tstring strText;

	for (LPCTSTR p = lpText; *p != _T('\0'); p++)
	{
		if (*p == _T('/'))
		{
			if (*(p+1) == _T('/'))
			{
				strText += *p;
				p++;
				continue;
			}
			else if (*(p+1) == _T('f'))
			{
				if (HandleSysFaceId_More(pRichEdit, p, strText, mfontInfo))
					continue;
			}
			else if (*(p+1) == _T('s'))
			{
				if (HandleSysFaceIndex_More(pRichEdit, p, strText, mfontInfo))
					continue;
			}
			else if (*(p+1) == _T('c'))
			{
				if (HandleCustomPic_More(pRichEdit, p, strText, mfontInfo))
					continue;
			}
		}
		strText += *p;
	}

	if (!strText.empty())
		_RichEdit_ReplaceSel_More(pRichEdit, strText.c_str(), mfontInfo);
}


BOOL CHistoryMsgDialog::HandleCustomPic_More(CRichEditUI * pRichEdit, LPCTSTR& p, tstring& strText, CFontInfo mfontInfo)
{
	CHAR *cStart = _T("[\"");
	CHAR *cEnd = _T("\"]");
	tstring strFileName = GetBetweenString(p+2, cStart, cEnd);

	if (!strFileName.empty())
	{
		if (!strText.empty())
		{
			_RichEdit_ReplaceSel_More(pRichEdit, strText.c_str(), mfontInfo); 
			strText = _T("");
		}

		CString strAllFile = "";
		strAllFile.Format("%s\\%s", g_config.szUserImage, strFileName.c_str());
		_RichEdit_InsertFace(pRichEdit, strAllFile, -1, -1);
		//_RichEdit_InsertFace(pRichEdit, strFileName.c_str(), -1, -1);

		p = _tcsstr(p+2, _T("\"]"));
		p++;
		return TRUE;
	}
	return FALSE;
}

BOOL CHistoryMsgDialog::HandleSysFaceIndex_More(CRichEditUI * pRichEdit, LPCTSTR& p, tstring& strText, CFontInfo mfontInfo)
{
	CHAR *cStart = _T("[\"");
	CHAR *cEnd = _T("\"]");
	int nFaceIndex = GetBetweenInt(p+2, cStart, cEnd, -1);

	CFaceInfo * lpFaceInfo = m_FaceList.GetFaceInfoByIndex(nFaceIndex);
	if (lpFaceInfo != NULL)
	{
		if (!strText.empty())
		{
			_RichEdit_ReplaceSel_More(pRichEdit, strText.c_str(), mfontInfo); 
			strText = _T("");
		}

		_RichEdit_InsertFace(pRichEdit, lpFaceInfo->m_strFileName.c_str(), 
			lpFaceInfo->m_nId, lpFaceInfo->m_nIndex);

		p = _tcsstr(p+2, _T("\"]"));
		p++;
		return TRUE;
	}
	return FALSE;
}

void CHistoryMsgDialog::AddMsgToHisMSGEdit(LPCTSTR lpText, CFontInfo mfontInfo, bool bisselfmsg/* =false */, CDuiString sid/* ="" */, CDuiString sname/* ="" */, CDuiString strSendTime/* ="" */)
{
	if (NULL == lpText || NULL == *lpText)
		return;

	m_pHisMsgEdit->SetAutoURLDetect(true);

	SYSTEMTIME st = {0};
	GetLocalTime(&st);
	CDuiString strTime = "";
	strTime.Format("%04d-%02d-%02d %02d:%02d:%02d", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);

	ITextServices * pTextServices = m_pHisMsgEdit->GetTextServices();
	
	RichEdit_SetSel(pTextServices, -1, -1);
	
	TCHAR cText[2048] = {0};
	if (bisselfmsg)
	{
		sprintf(cText, _T("%s("), sname);
		RichEdit_ReplaceSel(pTextServices, cText, STR_DEFAULT_FONT, 9, RGB(0, 128, 64), FALSE, FALSE, FALSE, FALSE, 0);
		sprintf(cText, _T("%s"), sid);
		RichEdit_ReplaceSel(pTextServices, cText, STR_DEFAULT_FONT, 9, RGB(0, 114, 193), FALSE, FALSE, TRUE, TRUE, 0);
		sprintf(cText, _T(") %s\r\n"), strSendTime.GetData());
		RichEdit_ReplaceSel(pTextServices, cText, STR_DEFAULT_FONT, 9, RGB(0, 128, 64), FALSE, FALSE, FALSE, FALSE, 0);
		AddMsg_More(m_pHisMsgEdit, lpText, mfontInfo);
	}
	else
	{
		sprintf(cText, _T("%s("), sname);
		RichEdit_ReplaceSel(pTextServices, cText, STR_DEFAULT_FONT, 9, RGB(0, 0, 255), FALSE, FALSE, FALSE, FALSE, 0);
		sprintf(cText, _T("%s"), sid);
		RichEdit_ReplaceSel(pTextServices, cText, STR_DEFAULT_FONT, 9, RGB(0, 114, 193), FALSE, FALSE, TRUE, TRUE, 0);
		sprintf(cText, _T(") %s\r\n"), strSendTime.GetData());
		RichEdit_ReplaceSel(pTextServices, cText, STR_DEFAULT_FONT, 9, RGB(0, 0, 255), FALSE, FALSE, FALSE, FALSE, 0);
		AddMsg_More(m_pHisMsgEdit, lpText, mfontInfo);
	}

	RichEdit_ReplaceSel(pTextServices, _T("\r\n"));
	RichEdit_SetStartIndent(pTextServices, 0);
	m_pHisMsgEdit->EndDown();
	
	pTextServices->Release();
}
