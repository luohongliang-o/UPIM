#include "stdafx.h"
#include "../Func.h"
#include "AddMember.h"
#include "../globalvar.h"

const TCHAR* const kTitleControlName		= _T("addmember_title");
const TCHAR* const kCloseControlName		= _T("addmember_closebtn");
const TCHAR* const kBackgroundControlName	= _T("addmember_bg");
const TCHAR* const kRichEditControlName		= _T("addmember_search");
const TCHAR* const kListControlName			= _T("addmember_list");
const TCHAR* const kListControlName2		= _T("addmember_list2");
const TCHAR* const kNumberControlName		= _T("addmember_num");
const TCHAR* const kOKButtonControlName		= _T("addmember_ok");
const TCHAR* const kCloseButtonControlName	= _T("addmember_close");

AddMemberDialog::AddMemberDialog(const CDuiString& bgimage, DWORD bkcolor, ROOMID RoomID_ /* = 0 */, short RoomType_ /* = 0 */ )
	: bgimage_(bgimage)
	, bkcolor_(bkcolor)
	, nRoomID(RoomID_)
	, nRoomType(RoomType_)
{
	pGroupsList = NULL;
	pGroupsList2 = NULL;
	pMemNumText = NULL;
}

AddMemberDialog::~AddMemberDialog()
{
	return;
}

LPCTSTR AddMemberDialog::GetWindowClassName() const
{
	return _T("UPGuiFoundation");
}

CControlUI* AddMemberDialog::CreateControl(LPCTSTR pstrClass)
{
	if (_tcsicmp(pstrClass, _T("MemberList")) == 0)
	{
		return new CGroupsUI(m_PaintManager, eUI_Member);
	}
	return NULL;
}

void AddMemberDialog::InitWindow()
{
	pGroupsList = static_cast<CGroupsUI*>(m_PaintManager.FindControl(kListControlName));
	pGroupsList2 = static_cast<CGroupsUI*>(m_PaintManager.FindControl(kListControlName2));
	pMemNumText = static_cast<CTextUI*>(m_PaintManager.FindControl(kNumberControlName));
	LoadMemList();
}

void AddMemberDialog::CreateGroupNode()
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

void AddMemberDialog::CreateDeptNode()
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

void AddMemberDialog::CreateFriendNode()
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
			if (m_vtFriendInfo[idex].online == 1)
				item.logo = _T("default.png");
			else
				item.logo = _T("default_offline.png");

			item.nodetype = eNode_Single;
			item.type = m_vtFriendInfo[idex].type;
			item.id = m_vtFriendInfo[idex].userid;
			item.numid = m_vtFriendInfo[idex].userid;
			item.Parentid = mGroupID;
			
			item.nick_name = m_vtFriendInfo[idex].nickname;
			item.showname.Format("%s(%s)", m_vtFriendInfo[idex].nickname, m_vtFriendInfo[idex].userid);
			retrunptr = pGroupsList->AddNode(item, m_mode);
		}
	}
}

void AddMemberDialog::LoadMemList()
{
	if (pGroupsList)
	{
		//////////////////////////////////////////////////////////////////////////
		if (nRoomType == eCR_Group)
		{
			CreateDeptNode();
		}
		else if (nRoomType == eCR_Group_Client)
		{
			CreateGroupNode();
		}
		CreateFriendNode();
	}
	if (pGroupsList2)
	{
		if (nRoomID > 0)
		{
			GCR_ITER iter_ = m_mapChatRoom.find(nRoomID);
			std::vector<GUSER_INFO>::iterator iter_info;
			for (iter_info = iter_->second.pVtUser->begin(); iter_info != iter_->second.pVtUser->end(); iter_info++)
			{
				FriendListItemInfo item;
				item.nodetype = eNode_Single;

				item.folder = false;
				item.type = eAdd_Exist;
				item.id = iter_info->m_szIMID;
				item.logo = _T("default.png");

				item.nick_name = alluserNickname[item.id];
				if (item.nick_name.IsEmpty())
					item.showname = iter_info->m_szIMID;
				else
					item.showname.Format("%s(%s)", item.nick_name.GetData(), iter_info->m_szIMID);

				Node* retrunptr = pGroupsList2->AddNode(item, NULL);
				if (retrunptr)
				{
					COLORREF huicor = RGB(80, 80, 80);
					pGroupsList2->SetNickNameColor(retrunptr, huicor);
				}
				
				//////////////////////////////////////////////////////////////////////////
				// 将原有的用户ID存储起来，便于匹配
				VUPID oldid = {0};
				oldid.m_nUserType = (BYTE)iter_info->m_nUserType;
				strcpy(oldid.m_szIMID, iter_info->m_szIMID);
				vtExistID.push_back(oldid);

				// 更新文字
				nFristTotleNum = (int)vtExistID.size();
				CDuiString strNum = _T("");
				strNum.Format("已选联系人 (%d/50)", nFristTotleNum);
				pMemNumText->SetText(strNum.GetData());
			}
		}
	}
}

void AddMemberDialog::OnFinalMessage(HWND hWnd)
{
	RemoveObserver();
	WindowImplBase::OnFinalMessage(hWnd);	
	delete this;
}

BOOL AddMemberDialog::Receive(SkinChangedParam param)
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

CDuiString AddMemberDialog::GetSkinFile()
{
	return _T("dlg_AddMember.xml");
}

CDuiString AddMemberDialog::GetSkinFolder()
{
	return CDuiString(CPaintManagerUI::GetInstancePath()) + _T("skin\\");
}

LRESULT AddMemberDialog::OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	bHandled = FALSE;
	return 0;
}

LRESULT AddMemberDialog::OnSetFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	bHandled = FALSE;
	return 0;
}

LRESULT AddMemberDialog::OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	BOOL bZoomed = ::IsZoomed(m_hWnd);
	LRESULT lRes = CWindowWnd::HandleMessage(uMsg, wParam, lParam);
	
	return 0;
}

LRESULT AddMemberDialog::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	// 屏蔽标题栏双击消息
	if(WM_NCLBUTTONDBLCLK != uMsg)
		return __super::HandleMessage(uMsg, wParam, lParam);
	return 0;
}

LRESULT AddMemberDialog::HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	bHandled = FALSE;
	return 0;
}

LRESULT AddMemberDialog::ResponseDefaultKeyEvent(WPARAM wParam)
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

void AddMemberDialog::OnTimer(TNotifyUI& msg)
{

}

void AddMemberDialog::OnExit()
{
	Close();
}

void AddMemberDialog::OnPrepare(TNotifyUI& msg)
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

BOOL AddMemberDialog::IsUserInList(VUPID *thisID)
{
	for (int nindex=0; nindex<(int)vtExistID.size(); nindex++)
	{
		if (thisID->m_nUserType == vtExistID[nindex].m_nUserType
			&& strcmp(thisID->m_szIMID, vtExistID[nindex].m_szIMID) == 0)
		{
			return TRUE;
		}
	}
	for (int nindex=0; nindex<(int)vtSelectID.size(); nindex++)
	{
		if (thisID->m_nUserType == vtSelectID[nindex].m_nUserType
			&& strcmp(thisID->m_szIMID, vtSelectID[nindex].m_szIMID) == 0)
		{
			return TRUE;
		}
	}
	return FALSE;
}

BOOL AddMemberDialog::RemoveUser(VUPID *thisID)
{
	for (int nindex=0; nindex<(int)vtSelectID.size(); nindex++)
	{
		if (thisID->m_nUserType == vtSelectID[nindex].m_nUserType
			&& strcmp(thisID->m_szIMID, vtSelectID[nindex].m_szIMID) == 0)
		{
			vtSelectID.erase(vtSelectID.begin()+nindex);
		}
	}
	return TRUE;
}

void AddMemberDialog::OnListItemActivate(TNotifyUI& msg)
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
				VUPID thisid = {0};
				if (node->data().type == eList_Dept)
					thisid.m_nUserType = eUser_Broker;
				else
					thisid.m_nUserType = eUser_Client;
				strcpy(thisid.m_szIMID, node->data().id);

				//////////////////////////////////////////////////////////////////////////
				// 判断右边列表中是否已经有这个客户
				BOOL bIsExist = IsUserInList(&thisid);
				// 没有就加入列表
				if (!bIsExist)
				{
					vtSelectID.push_back(thisid);

					//////////////////////////////////////////////////////////////////////////
					FriendListItemInfo item;
					item.nodetype = eNode_Single;

					item.folder = false;
					item.type = eAdd_New;
					item.rusertype = thisid.m_nUserType;
					item.id = thisid.m_szIMID;
					item.logo = _T("default.png");

					item.nick_name = alluserNickname[item.id];
					if (item.nick_name.IsEmpty())
						item.showname = item.id;
					else
						item.showname.Format("%s(%s)", item.nick_name.GetData(), thisid.m_szIMID);

					Node* retrunptr = pGroupsList2->AddNode(item, NULL);
					if (retrunptr)
					{
						COLORREF huicor = RGB(80, 80, 255);
						pGroupsList2->SetNickNameColor(retrunptr, huicor);
					}
					//////////////////////////////////////////////////////////////////////////
					// 更新文字
					CDuiString strNum = _T("");
					strNum.Format("已选联系人 (%d/50)", nFristTotleNum + (int)vtSelectID.size());
					pMemNumText->SetText(strNum.GetData());
				}
			}
		}
	}

	if ((pGroupsList2 != NULL) &&  pGroupsList2->GetItemIndex(msg.pSender) != -1)
	{
		if (_tcsicmp(msg.pSender->GetClass(), _T("ListContainerElementUI")) == 0)
		{
			Node* node = (Node*)msg.pSender->GetTag();

			if (pGroupsList2->CanExpand(node)) // 如果是节点
			{
				pGroupsList2->SetChildVisible(node, !node->data().child_visible_);
			}
			else // 如果是好友
			{
				// 已经存在的成员
				if (node->data().type == eAdd_New)
				{
					VUPID thisid = {0};
					thisid.m_nUserType = node->data().rusertype;
					strcpy(thisid.m_szIMID, node->data().id);

					if (RemoveUser(&thisid))
					{
						// 刚添加过来的，双击就删除了
						pGroupsList2->RemoveNode(node);

						//////////////////////////////////////////////////////////////////////////
						// 更新文字
						CDuiString strNum = _T("");
						strNum.Format("已选联系人 (%d/50)", nFristTotleNum + (int)vtSelectID.size());
						pMemNumText->SetText(strNum.GetData());
					}
				}
			}
		}
	}
	return;
}

// List单击消息
void AddMemberDialog::OnListItemClick(TNotifyUI& msg)
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
			}
		}
	}

	if ((pGroupsList2 != NULL) &&  pGroupsList2->GetItemIndex(msg.pSender) != -1)
	{
		if (_tcsicmp(msg.pSender->GetClass(), _T("ListContainerElementUI")) == 0)
		{
			Node* node = (Node*)msg.pSender->GetTag();

			if (pGroupsList2->CanExpand(node)) // 如果是节点
			{
				pGroupsList2->SetChildVisible(node, !node->data().child_visible_);
			}
			else // 如果是好友
			{
			}
		}
	}
}

void AddMemberDialog::OnOKBtnDown(TNotifyUI& msg)
{
	int nvtSelectCount = (int)vtSelectID.size();
	if (nvtSelectCount>0)
	{
		GROUP_CHAT_ADD_STU mtu = {0};
		mtu.roomtype = nRoomType;
		mtu.roomid = nRoomID;
		mtu.count = nvtSelectCount;
		for (int nIndex_=0; nIndex_<nvtSelectCount; nIndex_++)
		{
			mtu.agUpid[nIndex_].m_nUserType = (EUSERTYPE)vtSelectID[nIndex_].m_nUserType;
			COPYSTRARRAY(mtu.agUpid[nIndex_].m_szUSERID, vtSelectID[nIndex_].m_szIMID);
		}
		// 发送添加消息
		g_MyClient.SendGcAddReq(mtu);
	}
	OnExit();
}

void AddMemberDialog::Notify(TNotifyUI& msg)
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
		if (_tcsicmp(msg.pSender->GetName(), kCloseControlName) == 0)
		{
			OnExit();
		}
		else if (_tcsicmp(msg.pSender->GetName(), kCloseButtonControlName) == 0)
        {
            OnExit();
		}
		else if (_tcsicmp(msg.pSender->GetName(), kOKButtonControlName) == 0)
		{
			OnOKBtnDown(msg);
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
	}
	else if (_tcsicmp(msg.sType, _T("itemselect")) == 0)
	{
	}
	//CUIList双击消息
	else if (_tcsicmp(msg.sType, _T("itemactivate")) == 0)
	{
		OnListItemActivate(msg);
	}
	//CUIList单击消息
	else if (_tcsicmp(msg.sType, _T("itemclick")) == 0)
	{
		OnListItemClick(msg);
	}
}
