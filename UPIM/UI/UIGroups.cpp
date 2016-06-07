#include "stdafx.h"
#include "UIGroups.h"

namespace DuiLib
{
CGroupsUI::CGroupsUI(CPaintManagerUI& paint_manager, EUITYPE m_uiType)
	: root_node_(NULL)
	, delay_deltaY_(0)
	, delay_number_(0)
	, delay_left_(0)
	, level_expand_image_(_T("<i list_icon_b.png>"))
	, level_collapse_image_(_T("<i list_icon_a.png>"))
	, level_text_start_pos_(10)
	, text_padding_(0, 0, 0, 0)
	, paint_manager_(paint_manager)
{
	SetItemShowHtml(true);

	uiType = m_uiType;
	root_node_ = new Node;
	root_node_->data().level_ = -1;
	root_node_->data().child_visible_ = true;
	root_node_->data().has_child_ = true;
	root_node_->data().list_elment_ = NULL;

	if (uiType == eUI_Group)
	{
		kGroupListItemNormalHeight = 26;
		kGroupListItemSelectedHeight = 44;
	}
	else if (uiType == eUI_Member)
	{
		kGroupListItemNormalHeight = 26;
		kGroupListItemSelectedHeight = 44;
	}
	else if (uiType == eUI_Recent)
	{
		kGroupListItemNormalHeight = 56;
		kGroupListItemSelectedHeight = 56;
	}
	else
	{
		kGroupListItemNormalHeight = 26;
		kGroupListItemSelectedHeight = 44;
	}
}

CGroupsUI::~CGroupsUI()
{
	if (root_node_)
		delete root_node_;

	root_node_ = NULL;
}

bool CGroupsUI::Add(CControlUI* pControl)
{
	if (!pControl)
		return false;

	if (_tcsicmp(pControl->GetClass(), _T("ListContainerElementUI")) != 0)
		return false;

	return CListUI::Add(pControl);
}

bool CGroupsUI::AddAt(CControlUI* pControl, int iIndex)
{
	if (!pControl)
		return false;

	if (_tcsicmp(pControl->GetClass(), _T("ListContainerElementUI")) != 0)
		return false;

	return CListUI::AddAt(pControl, iIndex);
}

bool CGroupsUI::Remove(CControlUI* pControl)
{
	if (!pControl)
		return false;

	if (_tcsicmp(pControl->GetClass(), _T("ListContainerElementUI")) != 0)
		return false;

	if (reinterpret_cast<Node*>(static_cast<CListContainerElementUI*>(pControl->GetInterface(_T("ListContainerElement")))->GetTag()) == NULL)
		return CListUI::Remove(pControl);
	else
		return RemoveNode(reinterpret_cast<Node*>(static_cast<CListContainerElementUI*>(pControl->GetInterface(_T("ListContainerElement")))->GetTag()));
}

bool CGroupsUI::RemoveAt(int iIndex)
{
	CControlUI* pControl = GetItemAt(iIndex);
	if (!pControl)
		return false;

	if (_tcsicmp(pControl->GetClass(), _T("ListContainerElementUI")) != 0)
		return false;

	if (reinterpret_cast<Node*>(static_cast<CListContainerElementUI*>(pControl->GetInterface(_T("ListContainerElement")))->GetTag()) == NULL)
		return CListUI::RemoveAt(iIndex);
	else
		return RemoveNode(reinterpret_cast<Node*>(static_cast<CListContainerElementUI*>(pControl->GetInterface(_T("ListContainerElement")))->GetTag()));
}

void CGroupsUI::RemoveAll()
{
	CListUI::RemoveAll();
	for (int i = 0; i < root_node_->num_children(); ++i)
	{
		Node* child = root_node_->child(i);
		RemoveNode(child);
	}
	delete root_node_;

	root_node_ = new Node;
	root_node_->data().level_ = -1;
	root_node_->data().child_visible_ = true;
	root_node_->data().has_child_ = true;
	root_node_->data().list_elment_ = NULL;
}

void CGroupsUI::DoEvent(TEventUI& event) 
{
	if (!IsMouseEnabled() && event.Type > UIEVENT__MOUSEBEGIN && event.Type < UIEVENT__MOUSEEND)
	{
		if (m_pParent != NULL)
			m_pParent->DoEvent(event);
		else
			CVerticalLayoutUI::DoEvent(event);
		return;
	}

	if (event.Type == UIEVENT_TIMER && event.wParam == SCROLL_TIMERID)
	{
		if (delay_left_ > 0)
		{
			--delay_left_;
			SIZE sz = GetScrollPos();
			LONG lDeltaY =  (LONG)(CalculateDelay((double)delay_left_ / delay_number_) * delay_deltaY_);
			if ((lDeltaY > 0 && sz.cy != 0)  || (lDeltaY < 0 && sz.cy != GetScrollRange().cy ))
			{
				sz.cy -= lDeltaY;
				SetScrollPos(sz);
				return;
			}
		}
		delay_deltaY_ = 0;
		delay_number_ = 0;
		delay_left_ = 0;
		m_pManager->KillTimer(this, SCROLL_TIMERID);
		return;
	}
	if (event.Type == UIEVENT_SCROLLWHEEL)
	{
		LONG lDeltaY = 0;
		if (delay_number_ > 0)
			lDeltaY =  (LONG)(CalculateDelay((double)delay_left_ / delay_number_) * delay_deltaY_);
		switch (LOWORD(event.wParam))
		{
		case SB_LINEUP:
			if (delay_deltaY_ >= 0)
				delay_deltaY_ = lDeltaY + 8;
			else
				delay_deltaY_ = lDeltaY + 12;
			break;
		case SB_LINEDOWN:
			if (delay_deltaY_ <= 0)
				delay_deltaY_ = lDeltaY - 8;
			else
				delay_deltaY_ = lDeltaY - 12;
			break;
		}
		if
			(delay_deltaY_ > 100) delay_deltaY_ = 100;
		else if
			(delay_deltaY_ < -100) delay_deltaY_ = -100;

		delay_number_ = (DWORD)sqrt((double)abs(delay_deltaY_)) * 5;
		delay_left_ = delay_number_;
		m_pManager->SetTimer(this, SCROLL_TIMERID, 20U);
		return;
	}

	CListUI::DoEvent(event);
}

Node* CGroupsUI::GetRoot()
{
	return root_node_;
}

const TCHAR* const kLogoButtonControlName = _T("logo");
const TCHAR* const kLogoContainerControlName = _T("logo_container");
const TCHAR* const kNickNameControlName = _T("nickname");
const TCHAR* const kDescriptionControlName = _T("description");
const TCHAR* const kOperatorPannelControlName = _T("operation");
const TCHAR* const kTimeControlName = _T("time");
const TCHAR* const kLastWordControlName = _T("lastword");

static bool OnLogoButtonEvent(void* event) 
{
    if( ((TEventUI*)event)->Type == UIEVENT_BUTTONDOWN ) 
	{
        CControlUI* pButton = ((TEventUI*)event)->pSender;
        if( pButton != NULL ) 
		{
            CListContainerElementUI* pListElement = (CListContainerElementUI*)(pButton->GetTag());
            if( pListElement != NULL ) pListElement->DoEvent(*(TEventUI*)event);
        }
    }
    return true;
}

Node* CGroupsUI::AddNode(const FriendListItemInfo& item, Node* parent)
{
	if (!parent)
		parent = root_node_;

	TCHAR szBuf[MAX_PATH] = {0};

    CListContainerElementUI* pListElement = NULL;
    if( !m_dlgBuilder.GetMarkup()->IsValid() ) 
	{
		if (uiType == eUI_Group)
		{
			pListElement = static_cast<CListContainerElementUI*>(m_dlgBuilder.Create(_T("item_group_list.xml"), (UINT)0, NULL, &paint_manager_));
			kGroupListItemNormalHeight = 26;
			kGroupListItemSelectedHeight = 44;
		}
		else if (uiType == eUI_Member)
		{
			pListElement = static_cast<CListContainerElementUI*>(m_dlgBuilder.Create(_T("item_member_list.xml"), (UINT)0, NULL, &paint_manager_));
			kGroupListItemNormalHeight = 26;
			kGroupListItemSelectedHeight = 44;
		}
		else if (uiType == eUI_Recent)
		{
			pListElement = static_cast<CListContainerElementUI*>(m_dlgBuilder.Create(_T("item_recent_list.xml"), (UINT)0, NULL, &paint_manager_));
			kGroupListItemNormalHeight = 56;
			kGroupListItemSelectedHeight = 56;
		}
    }
    else 
	{
        pListElement = static_cast<CListContainerElementUI*>(m_dlgBuilder.Create((UINT)0, &paint_manager_));
    }
    if (pListElement == NULL)
        return NULL;

	Node* node = new Node;

	node->data().level_ = parent->data().level_ + 1;
	if (item.folder)
		node->data().has_child_ = !item.empty;
	else
		node->data().has_child_ = false;

	node->data().folder_ = item.folder;
	node->data().child_visible_ = (node->data().level_ == 0);

	node->data().nodetype = item.nodetype;
	node->data().type = item.type;
	node->data().rusertype = item.rusertype;
	node->data().text_ = item.showname;
	node->data().value = item.id;
	node->data().id = item.id;
	node->data().numid = item.numid;
	node->data().Parentid = item.Parentid;
	node->data().nickname = item.nick_name;
	node->data().list_elment_ = pListElement;

	//IM2.0
	node->data().Lastword = item.Lastword;
	node->data().GoodAt = item.GoodAt;
	node->data().lastwordtime = item.lastwordtime;
	node->data().isvip = item.isvip;
	node->data().hasright = item.hasright;
	node->data().totlenum = item.totlenum;
	node->data().onlinenum = item.onlinenum;

	if (!parent->data().child_visible_)
		pListElement->SetVisible(false);

	if (parent != root_node_ && !parent->data().list_elment_->IsVisible())
		pListElement->SetVisible(false);

	CDuiRect rcPadding = text_padding_;
	for (int i = 0; i < node->data().level_; ++i)
	{
		rcPadding.left += level_text_start_pos_;		
	}
	pListElement->SetPadding(rcPadding);

	CButtonUI* log_button = static_cast<CButtonUI*>(paint_manager_.FindSubControlByName(pListElement, kLogoButtonControlName));
	if (log_button != NULL)
	{
		if (!item.folder && !item.logo.IsEmpty())
		{
			_stprintf_s(szBuf, MAX_PATH - 1, _T("%s"), item.logo);
			log_button->SetNormalImage(szBuf);
		}
		else
		{
			CContainerUI* logo_container = static_cast<CContainerUI*>(paint_manager_.FindSubControlByName(pListElement, kLogoContainerControlName));
			if (logo_container != NULL)
				logo_container->SetVisible(false);
		}
        log_button->SetTag((UINT_PTR)pListElement);
        log_button->OnEvent += MakeDelegate(&OnLogoButtonEvent);
	}

	CDuiString html_text;
	if (node->data().has_child_)
	{
		if (node->data().child_visible_)
			html_text += level_expand_image_;
		else
			html_text += level_collapse_image_;

		_stprintf_s(szBuf, MAX_PATH - 1, _T("<x %d>"), level_text_start_pos_);
		html_text += szBuf;
	}

	if (item.folder)
	{
		html_text += node->data().text_;
	}
	else
	{
		_stprintf_s(szBuf, MAX_PATH - 1, _T("%s"), node->data().text_);
		html_text += szBuf;
	}

	CLabelUI* nick_name = static_cast<CLabelUI*>(paint_manager_.FindSubControlByName(pListElement, kNickNameControlName));
	if (nick_name != NULL)
	{
		if (item.folder)
			nick_name->SetFixedWidth(0);

		nick_name->SetShowHtml(true);
		nick_name->SetText(html_text);
	}

	//////////////////////////////////////////////////////////////////////////
	//IM2.0
	CLabelUI* l_time = static_cast<CLabelUI*>(paint_manager_.FindSubControlByName(pListElement, kTimeControlName));
	if (l_time != NULL)
	{
		CDuiString lastwordtime = _T("");
		long time = item.lastwordtime ; 
		CTime tempTime(time) ; 
		CTime curTime = CTime::GetCurrentTime() ; 

		// 昨天 
		if( ( curTime.GetYear() == tempTime.GetYear() && curTime.GetMonth() == tempTime.GetMonth() ) && ( curTime.GetDay() == tempTime.GetDay() + 1 ) )
		{
			l_time->SetText(_T("昨天")) ; 
		}
		else if( ( curTime.GetYear() == tempTime.GetYear() && curTime.GetMonth() == tempTime.GetMonth() ) && ( curTime.GetDay() == tempTime.GetDay() + 2 ) ) 
		{
			l_time->SetText(_T("前天")) ;
		}
		else if( ( curTime.GetYear() == tempTime.GetYear() && curTime.GetMonth() == tempTime.GetMonth() ) && ( curTime.GetDay() == tempTime.GetDay()) )
		{
			// 当天 
			CString strText = _T(" ") ;
			strText.Format("%d:%d" ,tempTime.GetHour(),tempTime.GetMinute()) ; 
			l_time->SetText(strText.GetBuffer()) ;
		}
		else 
		{
			CString strText = _T(" ") ;
			strText.Format("%d-%d" ,tempTime.GetMonth(),tempTime.GetDay()) ; 
			l_time->SetText(strText.GetBuffer()) ;
		}
		//lastwordtime.Format("%02d:%02d", item.lastwordtime/100, item.lastwordtime%100);
		//l_time->SetText(lastwordtime);
	}
	CLabelUI* l_lastword = static_cast<CLabelUI*>(paint_manager_.FindSubControlByName(pListElement, kLastWordControlName));
	if (l_lastword != NULL)
		l_lastword->SetText(item.Lastword);
	//////////////////////////////////////////////////////////////////////////

	if (!item.folder && !item.description.IsEmpty())
	{
		CLabelUI* description = static_cast<CLabelUI*>(paint_manager_.FindSubControlByName(pListElement, kDescriptionControlName));
		if (description != NULL)
		{
			_stprintf_s(szBuf, MAX_PATH - 1, _T("<x 8><c #808080>%s</c>"), item.description);
			description->SetShowHtml(true);
			description->SetText(szBuf);
		}
	}

	pListElement->SetFixedHeight(kGroupListItemNormalHeight);
	pListElement->SetTag((UINT_PTR)node);
	int index = 0;
	if (parent->has_children())
	{
		Node* prev = parent->get_last_child();
		index = prev->data().list_elment_->GetIndex() + 1;
	}
	else 
	{
		if (parent == root_node_)
			index = 0;
		else
			index = parent->data().list_elment_->GetIndex() + 1;
	}
	if (!CListUI::AddAt(pListElement, index))
	{
		delete pListElement;
		delete node;
		node = NULL;
	}

	parent->add_child(node);

	CString strID = item.id.GetData();
	allNodeInfo[strID] = node;
	return node;
}

bool CGroupsUI::RemoveNodeMapInfo(Node* node)
{
	// 删除保存的Node信息
	NODE_ITER iter_node_;
	for (iter_node_ = allNodeInfo.begin(); iter_node_ != allNodeInfo.end() ; iter_node_++)
	{
		if ((Node*)iter_node_->second == node)
		{
			allNodeInfo.erase(iter_node_);
			break;
		}
	}
	return true;
}

bool CGroupsUI::RemoveNode(Node* node)
{
	if (!node || node == root_node_) return false;

	for (int i = 0; i < node->num_children(); ++i)
	{
		Node* child = node->child(i);
		RemoveNode(child);
	}

	CListUI::Remove(node->data().list_elment_);
	node->parent()->remove_child(node);
	delete node;

	return true;
}

void CGroupsUI::SetChildVisible(Node* node, bool visible)
{
	if (!node || node == root_node_)
		return;

	if (node->data().child_visible_ == visible)
		return;

	node->data().child_visible_ = visible;

	TCHAR szBuf[MAX_PATH] = {0};
	CDuiString html_text;
	if (node->data().has_child_)
	{
		if (node->data().child_visible_)
			html_text += level_expand_image_;
		else
			html_text += level_collapse_image_;

		_stprintf_s(szBuf, MAX_PATH - 1, _T("<x %d>"), level_text_start_pos_);
		html_text += szBuf;

		html_text += node->data().text_;

		CLabelUI* nick_name = static_cast<CLabelUI*>(paint_manager_.FindSubControlByName(node->data().list_elment_, kNickNameControlName));
		if (nick_name != NULL)
		{
			nick_name->SetShowHtml(true);
			nick_name->SetText(html_text);
		}
	}

	if (!node->data().list_elment_->IsVisible())
		return;

	if (!node->has_children())
		return;

	Node* begin = node->child(0);
	Node* end = node->get_last_child();
	for (int i = begin->data().list_elment_->GetIndex(); i <= end->data().list_elment_->GetIndex(); ++i)
	{
		CControlUI* control = GetItemAt(i);
		if (_tcsicmp(control->GetClass(), _T("ListContainerElementUI")) == 0)
		{
			if (visible) 
			{
				Node* local_parent = ((Node*)control->GetTag())->parent();
				if (local_parent->data().child_visible_ && local_parent->data().list_elment_->IsVisible())
				{
					control->SetVisible(true);
				}
			}
			else
			{
				control->SetVisible(false);
			}
		}
	}
}

bool CGroupsUI::CanExpand(Node* node) const
{
	if (!node || node == root_node_) return false;

	return node->data().has_child_;
}

Node* CGroupsUI::SelectNodeByID(CString strUserID)
{
	if (strUserID.IsEmpty())
		return NULL;
	else
		return allNodeInfo[strUserID];
	return NULL;
}


bool CGroupsUI::SelectItem(int iIndex, bool bTakeFocus)
{
	if( iIndex == m_iCurSel ) return true;

	// We should first unselect the currently selected item
	if( m_iCurSel >= 0 ) {
		CControlUI* pControl = GetItemAt(m_iCurSel);
		if( pControl != NULL) {
			IListItemUI* pListItem = static_cast<IListItemUI*>(pControl->GetInterface(_T("ListItem")));
			if( pListItem != NULL )
			{
				CListContainerElementUI* pFriendListItem = static_cast<CListContainerElementUI*>(pControl);
				Node* node = (Node*)pControl->GetTag();
				if ((pFriendListItem != NULL) && (node != NULL) && !node->folder())
				{
// 					pFriendListItem->SetFixedHeight(kFriendListItemNormalHeight);
// 					CContainerUI* pOperatorPannel = static_cast<CContainerUI*>(paint_manager_.FindSubControlByName(pFriendListItem, kOperatorPannelControlName));
// 					if (pOperatorPannel != NULL)
// 					{
// 						pOperatorPannel->SetVisible(false);
// 					}
				}
				pListItem->Select(false);
			}
		}

		m_iCurSel = -1;
	}

	if( iIndex < 0 )
		return false;

	if (!__super::SelectItem(iIndex, bTakeFocus))
		return false;

	CControlUI* pControl = GetItemAt(m_iCurSel);
	if( pControl != NULL) {
		CListContainerElementUI* pFriendListItem = static_cast<CListContainerElementUI*>(pControl);
		Node* node = (Node*)pControl->GetTag();
		if ((pFriendListItem != NULL) && (node != NULL) && !node->folder())
		{
// 			pFriendListItem->SetFixedHeight(kFriendListItemSelectedHeight);
// 			CContainerUI* pOperatorPannel = static_cast<CContainerUI*>(paint_manager_.FindSubControlByName(pFriendListItem, kOperatorPannelControlName));
// 			if (pOperatorPannel != NULL)
// 			{
// 				pOperatorPannel->SetVisible(true);
// 			}
			//pFriendListItem->SetBkImage()
		}
	}
	return true;
}

bool CGroupsUI::SetNickNameColor(Node* mNode, COLORREF textcolor)
{
	if (!mNode->folder())
	{
		CLabelUI* nick_name = static_cast<CLabelUI*>(paint_manager_.FindSubControlByName(mNode->data().list_elment_, kNickNameControlName));
		if (nick_name != NULL)
		{
			nick_name->SetTextColor(textcolor);
		}
	}
	return true;
}

void CGroupsUI::SetNodeLastWord(Node* node, CString strLastWord, int nTime)
{
	if (node)
	{
		CLabelUI* l_time = static_cast<CLabelUI*>(paint_manager_.FindSubControlByName(node->data().list_elment_, kTimeControlName));
		CLabelUI* l_lastword = static_cast<CLabelUI*>(paint_manager_.FindSubControlByName(node->data().list_elment_, kLastWordControlName));
		if (l_time != NULL)
		{
			CDuiString lastwordtime = _T("");
			lastwordtime.Format("%02d:%02d", nTime/100, nTime%100);
			l_time->SetText(lastwordtime);
		}
		
		if (l_lastword != NULL)
			l_lastword->SetText(strLastWord);
	}
}

void CGroupsUI::SetNodeFlash(Node* node, bool nFlash, int nDeirction /* = 0 */)
{
	CButtonUI* pic_btn = static_cast<CButtonUI*>(paint_manager_.FindSubControlByName(node->data().list_elment_, kLogoButtonControlName));
	CContainerUI* logo_container = static_cast<CContainerUI*>(paint_manager_.FindSubControlByName(node->data().list_elment_, kLogoContainerControlName));
	if (pic_btn != NULL)
	{
		// 头像闪动时，以下方Container的坐标为基准，以免pos改变之后，下次基准失效
		RECT mRect = {0};
		RECT mRect2 = {0};
		RECT mRect3 = logo_container->GetPos();
		mRect.top = mRect3.top;
		mRect.bottom = mRect.top + 28;
		mRect.left = mRect3.left;
		mRect.right = mRect.left + 28;

		if (nFlash == true)
		{
			switch(nDeirction)
			{
			case 0:
				{
					mRect2.left = mRect.left - 1;
					mRect2.right = mRect.right - 1;
					mRect2.bottom = mRect.bottom;
					mRect2.top = mRect.top;
					pic_btn->SetPos(mRect2);
				}
				break;
			case 1:
				{
					mRect2.left = mRect.left;
					mRect2.right = mRect.right;
					mRect2.bottom = mRect.bottom - 1;
					mRect2.top = mRect.top - 1;
					pic_btn->SetPos(mRect2);
				}
				break;
			case 2:
				{
					mRect2.left = mRect.left + 1;
					mRect2.right = mRect.right + 1;
					mRect2.bottom = mRect.bottom;
					mRect2.top = mRect.top;
					pic_btn->SetPos(mRect2);
				}
				break;
			case 3:
				{
					mRect2.left = mRect.left;
					mRect2.right = mRect.right;
					mRect2.bottom = mRect.bottom - 1;
					mRect2.top = mRect.top - 1;
					pic_btn->SetPos(mRect2);
				}
				break;
			default:
				{
					mRect2.left = mRect.left;
					mRect2.right = mRect.right;
					mRect2.bottom = mRect.bottom;
					mRect2.top = mRect.top;
					pic_btn->SetPos(mRect2);
				}
				break;
			}
		}
		// 停止闪烁
		else
		{
			pic_btn->SetPos(mRect);
		}
	}
}

} // namespace DuiLib