#include "stdafx.h"
#include "UIFriends.h"

namespace DuiLib
{
	//////////////////////////////////////////////////////////////////////////
	// 普通版
#ifdef VER_UPIM_ANALYS
	const int kFriendListItemNormalHeight = 28;
	const int kFriendListItemSelectedHeight = 28;
#endif
#ifdef VER_UPIM_ANALYS2
	const int kFriendListItemNormalHeight = 56;
	const int kFriendListItemSelectedHeight = 56;
#endif
#ifdef VER_UPIM_CLIENT
	const int kFriendListItemNormalHeight = 56;
	const int kFriendListItemSelectedHeight = 56;
#endif

#ifdef VER_UPIM_NORMAL
	const int kFriendListItemNormalHeight = 26;
	const int kFriendListItemSelectedHeight = 44;
#endif

#ifdef VER_UPIM_RONGYUN
	const int kFriendListItemNormalHeight = 100;
	const int kFriendListItemSelectedHeight = 100;
#endif

	CFriendsUI::CFriendsUI(CPaintManagerUI& paint_manager)
		: root_node_(NULL)
		, delay_deltaY_(0)
		, delay_number_(0)
		, delay_left_(0)
		, level_expand_image_(_T("<i list_icon_b.png>"))
		, level_collapse_image_(_T("<i list_icon_a.png>"))
		, level_text_start_pos_(1)
		, text_padding_(1, 0, 0, 0)
		, paint_manager_(paint_manager)
	{
		SetItemShowHtml(true);

		root_node_ = new Node;
		root_node_->data().level_ = -1;
		root_node_->data().child_visible_ = true;
		root_node_->data().has_child_ = true;
		root_node_->data().list_elment_ = NULL;
	}

	CFriendsUI::~CFriendsUI()
	{
		if (root_node_)
			delete root_node_;

		root_node_ = NULL;
	}

	bool CFriendsUI::Add(CControlUI* pControl)
	{
		if (!pControl)
			return false;

		if (_tcsicmp(pControl->GetClass(), _T("ListContainerElementUI")) != 0)
			return false;

		return CListUI::Add(pControl);
	}

	bool CFriendsUI::AddAt(CControlUI* pControl, int iIndex)
	{
		if (!pControl)
			return false;

		if (_tcsicmp(pControl->GetClass(), _T("ListContainerElementUI")) != 0)
			return false;

		return CListUI::AddAt(pControl, iIndex);
	}

	bool CFriendsUI::Remove(CControlUI* pControl)
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

	bool CFriendsUI::RemoveAt(int iIndex)
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

	void CFriendsUI::RemoveAll()
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
	Node* CFriendsUI::GetNodeItem(CDuiString strID )
	{
		Node* findNode = allNodeInfo[(strID.GetData())] ; 
		if( findNode )
			return findNode ; 
		else 
			return NULL ; 

	}

	void CFriendsUI::DoEvent(TEventUI& event) 
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

	Node* CFriendsUI::GetRoot()
	{
		return root_node_;
	}

	const TCHAR* const kLogoButtonControlName = _T("logo");
	const TCHAR* const kLogoContainerControlName = _T("logo_container");
	const TCHAR* const kLastwordContainerControlName = _T("lastword_container");
	const TCHAR* const kNickNameControlName = _T("nickname");
	const TCHAR* const kDescriptionControlName = _T("description");
	const TCHAR* const kDescriptionControlName2 = _T("description2");
	const TCHAR* const kOperatorPannelControlName = _T("operation");
	const TCHAR* const kTimeControlName = _T("time");
	const TCHAR* const kLastWordControlName = _T("lastword");
	const TCHAR* const kNumsControlName = _T("nums");
	const TCHAR* const kGoodATControlName = _T("goodat");

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


	// 在parent节点后面添加一个子节点child
	void CFriendsUI::AddNode(FriendListItemInfo  item ,Node *child ,Node* parent )
	{
		CListContainerElementUI* pListElement = NULL;
		if( !m_dlgBuilder.GetMarkup()->IsValid() ) 
		{
#ifndef VER_UPIM_RONGYUN
			pListElement = static_cast<CListContainerElementUI*>(m_dlgBuilder.Create(_T("item_friend_list.xml"), (UINT)0, NULL, &paint_manager_));
#else
			pListElement = static_cast<CListContainerElementUI*>(m_dlgBuilder.Create(_T("item_friend_list_new_im.xml"), (UINT)0, NULL, &paint_manager_));
#endif
			
		}
		else 
		{
			pListElement = static_cast<CListContainerElementUI*>(m_dlgBuilder.Create((UINT)0, &paint_manager_));
		}
		if (pListElement == NULL)
			return;
		if( !parent->data().child_visible_)
			pListElement->SetVisible(false) ; 

		CDuiRect rcPadding = text_padding_ ; 
		for( int i = 0 ; i < parent->data().level_ + 1 ; ++i)
		{
			rcPadding.left += level_text_start_pos_ ; 
		}
		pListElement->SetPadding(rcPadding) ;

		CButtonUI* log_button = static_cast<CButtonUI*>(paint_manager_.FindSubControlByName(pListElement, kLogoButtonControlName));
		if( log_button != NULL)
		{
			log_button->SetNormalImage(item.logo.GetData()) ; 
			log_button->SetTag((UINT_PTR)pListElement) ;
			log_button->OnEvent += MakeDelegate(&OnLogoButtonEvent) ; 
		}
		pListElement->SetFixedHeight(kFriendListItemNormalHeight) ; 
		pListElement->SetTag( (UINT_PTR)child) ;

		// 分组节点孩子的最后一个节点 
		Node* pLastChildNode =  parent->get_last_child() ; 
		if( !pLastChildNode)
			return ; 

		int index = parent->data().list_elment_->GetIndex() + 1 ; 
		if( !CListUI::AddAt(pListElement, index) )
			delete pListElement ; 

		parent->add_child(child) ; 
	}

	// 托盘提示消息框 专用  CMsgNotifyDialog中
	Node* CFriendsUI::AddNode(const FriendListItemInfo& item, Node* parent /*= NULL*/ ,int itemHeight /*= 0*/ )
	{
		if (!parent)
			parent = root_node_;

		TCHAR szBuf[MAX_PATH] = {0};

		CListContainerElementUI* pListElement = NULL;
		if( !m_dlgBuilder.GetMarkup()->IsValid() ) 
		{
#ifndef VER_UPIM_RONGYUN
			pListElement = static_cast<CListContainerElementUI*>(m_dlgBuilder.Create(_T("item_friend_list.xml"), (UINT)0, NULL, &paint_manager_));
#else
			pListElement = static_cast<CListContainerElementUI*>(m_dlgBuilder.Create(_T("item_friend_list_new_im.xml"), (UINT)0, NULL, &paint_manager_));
#endif

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
		node->data().child_visible_ = false;

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

		//////////////////////////////////////////////////////////////////////////
		// 如果是IM2.0版本
		if (item.folder)
		{
			if (VER_UPIM_NUMS == 3 || VER_UPIM_NUMS == 4)
			{
				CDuiString strDispNums = _T("");

				strDispNums.Format(" %d/%d", item.onlinenum, item.totlenum);
				_stprintf_s(szBuf, MAX_PATH - 1, _T("%s"), strDispNums);
				html_text += szBuf;
			}
			if (VER_UPIM_NUMS == 4)
			{
				if (node->data().isvip == 1)
				{
					CDuiString strNoRightPic = _T("<x 5><i vipicon.png>");
					html_text += strNoRightPic;
				}
				if (node->data().hasright == 0)
				{
					CDuiString strNoRightPic = _T("<x 5><i noright.png>");
					html_text += strNoRightPic;
				}
			}
		}
		CLabelUI* nick_name = static_cast<CLabelUI*>(paint_manager_.FindSubControlByName(pListElement, kNickNameControlName));
		if (nick_name != NULL)
		{
			if (item.folder)
			{
				nick_name->SetFixedWidth(500);
				//nick_name->SetFixedWidth(node->data().text_.GetLength() * WLEN_TO_PIX);
				nick_name->SetShowHtml(true);
				nick_name->SetText(html_text);
			}
			else
			{
				nick_name->SetFixedWidth(node->data().text_.GetLength() * WLEN_TO_PIX);
				nick_name->SetShowHtml(true);
				nick_name->SetText(html_text);
			}
		}
		// 最后一条信息 
		CLabelUI* l_lastword = static_cast<CLabelUI*>(paint_manager_.FindSubControlByName(pListElement, kLastWordControlName)); 
		if(l_lastword)
			l_lastword->SetText(item.Lastword) ; 
		//////////////////////////////////////////////////////////////////////////
		// 节点 展示在线信息
		if (!item.folder && !item.GoodAt.IsEmpty())
		{
			CLabelUI* l_goodat = static_cast<CLabelUI*>(paint_manager_.FindSubControlByName(node->data().list_elment_, kGoodATControlName));
			if (l_goodat != NULL)
				l_goodat->SetText(item.GoodAt);
		}

		if (!item.folder && !item.description.IsEmpty())
		{
			// 		CLabelUI* description = static_cast<CLabelUI*>(paint_manager_.FindSubControlByName(pListElement, kDescriptionControlName));
			// 		if (description != NULL)
			// 		{
			// 			_stprintf_s(szBuf, MAX_PATH - 1, _T("<x 8><c #808080>%s</c>"), item.description);
			// 			description->SetShowHtml(true);
			// 			description->SetText(szBuf);
			// 		}
		}
		if (!item.folder)
			pListElement->SetFixedHeight(itemHeight);
		else
			pListElement->SetFixedHeight(30);

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


	Node* CFriendsUI::AddNode(const FriendListItemInfo& item, Node* parent)
	{
		if (!parent)
			parent = root_node_;

		TCHAR szBuf[MAX_PATH] = {0};

		CListContainerElementUI* pListElement = NULL;
		if( !m_dlgBuilder.GetMarkup()->IsValid() ) 
		{
#ifndef VER_UPIM_RONGYUN
			pListElement = static_cast<CListContainerElementUI*>(m_dlgBuilder.Create(_T("item_friend_list.xml"), (UINT)0, NULL, &paint_manager_));
#else
			pListElement = static_cast<CListContainerElementUI*>(m_dlgBuilder.Create(_T("item_friend_list_new_im.xml"), (UINT)0, NULL, &paint_manager_));
#endif

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
		node->data().child_visible_ = false;

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
			// 如果是列表中的分组名称
			else
			{
				CContainerUI* logo_container = static_cast<CContainerUI*>(paint_manager_.FindSubControlByName(pListElement, kLogoContainerControlName));
				if (logo_container != NULL)
					logo_container->SetVisible(false);

				CContainerUI* lastword_container = static_cast<CContainerUI*>(paint_manager_.FindSubControlByName(pListElement, kLastwordContainerControlName));
				if (lastword_container != NULL)
					lastword_container->SetVisible(false);

				CContainerUI* logo_img_container = static_cast<CContainerUI*>(paint_manager_.FindSubControlByName(pListElement, "logo_img"));
				//if (logo_img_container != NULL)
					//logo_img_container->SetAttribute("height", "30");					
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

		//////////////////////////////////////////////////////////////////////////
		// 如果是IM2.0版本
		if (item.folder)
		{
			if (VER_UPIM_NUMS == 3 || VER_UPIM_NUMS == 4)
			{
				CDuiString strDispNums = _T("");

				strDispNums.Format(" %d/%d", item.onlinenum, item.totlenum);
				_stprintf_s(szBuf, MAX_PATH - 1, _T("%s"), strDispNums);
				html_text += szBuf;
			}
			if (VER_UPIM_NUMS == 4)
			{
				if (node->data().isvip == 1)
				{
					CDuiString strNoRightPic = _T("<x 5><i vipicon.png>");
					html_text += strNoRightPic;
				}
				if (node->data().hasright == 0)
				{
					CDuiString strNoRightPic = _T("<x 5><i noright.png>");
					html_text += strNoRightPic;
				}
			}
		}
		CLabelUI* nick_name = static_cast<CLabelUI*>(paint_manager_.FindSubControlByName(pListElement, kNickNameControlName));
		if (nick_name != NULL)
		{
			if (item.folder)
			{
				nick_name->SetFixedWidth(500);
				//nick_name->SetFixedWidth(node->data().text_.GetLength() * WLEN_TO_PIX);
				nick_name->SetShowHtml(true);
				nick_name->SetText(html_text);
			}
			else
			{
				nick_name->SetFixedWidth(node->data().text_.GetLength() * WLEN_TO_PIX);
				nick_name->SetShowHtml(true);
				nick_name->SetText(html_text);
			}
		}

		//////////////////////////////////////////////////////////////////////////
		// 节点 展示在线信息
		if (!item.folder && !item.GoodAt.IsEmpty())
		{
			CLabelUI* l_goodat = static_cast<CLabelUI*>(paint_manager_.FindSubControlByName(node->data().list_elment_, kGoodATControlName));
			if (l_goodat != NULL)
				l_goodat->SetText(item.GoodAt);
		}

		if (!item.folder && !item.description.IsEmpty())
		{
			// 		CLabelUI* description = static_cast<CLabelUI*>(paint_manager_.FindSubControlByName(pListElement, kDescriptionControlName));
			// 		if (description != NULL)
			// 		{
			// 			_stprintf_s(szBuf, MAX_PATH - 1, _T("<x 8><c #808080>%s</c>"), item.description);
			// 			description->SetShowHtml(true);
			// 			description->SetText(szBuf);
			// 		}
		}
		if (!item.folder)
			pListElement->SetFixedHeight(kFriendListItemNormalHeight);
		else
			pListElement->SetFixedHeight(30);

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

	bool CFriendsUI::RemoveNodeMapInfo(Node* node)
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

	bool CFriendsUI::RemoveNode(Node* node)
	{
		if (!node || node == root_node_) return false;
		int iSize = node->num_children() ; 

		for (int i = 0; i < iSize; ++i)
		{
			Node* child = node->child(i);
			RemoveNode(child);
		}
		if( node->data().list_elment_)
			CListUI::Remove(node->data().list_elment_);
		node->parent()->remove_child(node);
		delete node;

		return true;
	}

	void CFriendsUI::SetChildVisible(Node* node, bool visible)
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

			//////////////////////////////////////////////////////////////////////////
			//
			if (VER_UPIM_NUMS == 3 || VER_UPIM_NUMS == 4)
			{
				CDuiString strDispNums = _T("");
				strDispNums.Format(" %d/%d", node->data().onlinenum, node->data().totlenum);
				_stprintf_s(szBuf, MAX_PATH - 1, _T("%s"), strDispNums);
				html_text += szBuf;
			}
			if (VER_UPIM_NUMS == 4)
			{
				if (node->data().isvip == 1)
				{
					CDuiString strNoRightPic = _T("<x 5><i vipicon.png>");
					html_text += strNoRightPic;
				}
				if (node->data().hasright == 0)
				{
					CDuiString strNoRightPic = _T("<x 5><i noright.png>");
					html_text += strNoRightPic;
				}
			}

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
		int nBeginIndex =  begin->data().list_elment_->GetIndex() ; // 起始位置 
		int nEndIndex   =  end->data().list_elment_->GetIndex() ;   // 结束位置
#if 0
		for (int i = nBeginIndex; i <= nEndIndex; ++i)
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
#endif 

#if 1 
		int iChildIndex = 0 ;

		while( begin != end )
		{
			int index = begin->data().list_elment_->GetIndex() ; 
			CControlUI* control = GetItemAt(index);
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
			iChildIndex++ ; 
			begin =  node->child(iChildIndex) ; 
		} 
		// 最后一个的处理
		if( begin == end )
		{
			int index = begin->data().list_elment_->GetIndex() ; 
			CControlUI* control = GetItemAt(index);
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
#endif 
	}

	bool CFriendsUI::CanExpand(Node* node) const
	{
		if (!node || node == root_node_)
			return false;

		return node->data().has_child_;
	}

	bool CFriendsUI::SelectItem(int iIndex, bool bTakeFocus)
	{
		if( iIndex == m_iCurSel ) return true;

		// We should first unselect the currently selected item
		if( m_iCurSel >= 0 ) 
		{
			CControlUI* pControl = GetItemAt(m_iCurSel);
			if( pControl != NULL) 
			{
				IListItemUI* pListItem = static_cast<IListItemUI*>(pControl->GetInterface(_T("ListItem")));
				if( pListItem != NULL )
				{
					CListContainerElementUI* pFriendListItem = static_cast<CListContainerElementUI*>(pControl);
					Node* node = (Node*)pControl->GetTag();
					if ((pFriendListItem != NULL) && (node != NULL) && !node->folder())
					{
						pFriendListItem->SetFixedHeight(kFriendListItemNormalHeight);
						CContainerUI* pOperatorPannel = static_cast<CContainerUI*>(paint_manager_.FindSubControlByName(pFriendListItem, kOperatorPannelControlName));
						if (pOperatorPannel != NULL)
						{
							pOperatorPannel->SetVisible(false);

							// 						CContainerUI* plogocontainer = static_cast<CContainerUI*>(paint_manager_.FindSubControlByName(pFriendListItem, _T("logo_container")));
							// 						CButtonUI * m_pLogoBtn = static_cast<CButtonUI*>(plogocontainer->FindSubControl("logo"));
							// 						m_pLogoBtn->SetAttribute("width", "24");
							// 						m_pLogoBtn->SetAttribute("height", "24");
						}
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
		if( pControl != NULL) 
		{
			CListContainerElementUI* pFriendListItem = static_cast<CListContainerElementUI*>(pControl);
			Node* node = (Node*)pControl->GetTag();
			if ((pFriendListItem != NULL) && (node != NULL) && !node->folder())
			{
				pFriendListItem->SetFixedHeight(kFriendListItemSelectedHeight);
				CContainerUI* pOperatorPannel = static_cast<CContainerUI*>(paint_manager_.FindSubControlByName(pFriendListItem, kOperatorPannelControlName));
				if (pOperatorPannel != NULL)
				{
					pOperatorPannel->SetVisible(true);
					// 				CContainerUI* plogoimg = static_cast<CContainerUI*>(paint_manager_.FindSubControlByName(pFriendListItem, _T("logo_img")));
					// 				plogoimg->SetAttribute("height", "32");
					// 
					// 				CContainerUI* plogocontainer = static_cast<CContainerUI*>(paint_manager_.FindSubControlByName(pFriendListItem, _T("logo_container")));
					// 				CButtonUI * m_pLogoBtn = static_cast<CButtonUI*>(plogocontainer->FindSubControl("logo"));
					//  				m_pLogoBtn->SetAttribute("width", "32");
					//  				m_pLogoBtn->SetAttribute("height", "32");
				}
				//pFriendListItem->SetBkImage()
			}
		}
		return true;
	}

	Node* CFriendsUI::SelectNodeByID(CString strUserID)
	{
		if (strUserID.IsEmpty())
			return NULL;
		else
			return allNodeInfo[strUserID];
		return NULL;
	}

	bool CFriendsUI::SetNickNameType(int nType)
	{
		NODE_ITER iter_node_;
		for (iter_node_ = allNodeInfo.begin(); iter_node_ != allNodeInfo.end() ; iter_node_++)
		{
			Node * mNode = (Node*)iter_node_->second;
			if (!mNode->folder())
			{
				CLabelUI* nick_name = static_cast<CLabelUI*>(paint_manager_.FindSubControlByName(mNode->data().list_elment_, kNickNameControlName));
				if (nick_name != NULL)
				{
					CString nametext;
					if (nType == 0)
					{
						//nickname(id)
						nametext.Format("%s(%s)", mNode->data().nickname.GetData(), mNode->data().id.GetData());
					}
					else if (nType == 1)
					{
						nametext.Format("%s", mNode->data().id.GetData());
					}
					else if (nType == 2)
					{
						nametext.Format("%s", mNode->data().nickname.GetData());
					}
					nick_name->SetFixedWidth(nametext.GetLength() * WLEN_TO_PIX);
					nick_name->SetText(nametext);
				}
			}
		}
		return true;
	}

	void CFriendsUI::SetNodeState(Node* node, int nstate)
	{
		CButtonUI* pic_btn = static_cast<CButtonUI*>(paint_manager_.FindSubControlByName(node->data().list_elment_, kLogoButtonControlName));
		USER_ROOMCLIENT_INFO *pNode  = NULL ; 
		if( VER_UPIM_NUMS == 4)
		{
			ALLCLIENT_ROOM_INFO itemNode ; 
			int nRoomId = atoi( node->parent()->data().numid ) ;
			// 查找分析师所在的房间 
			itemNode = m_mapAllClientRoom[nRoomId] ; 

			// 找到分析师 并将是否在线设置为nState 
			vector<ROOMANALYS_INFO>* pVtAnalys = itemNode.pVtAnalys ; 
			int iSize = pVtAnalys->size() ; 
			for( int i = 0 ; i < iSize ; i++)
			{
				ROOMANALYS_INFO info = (*pVtAnalys).at(i) ;
				int nAnalysID = atoi((node->data().numid)); 
				if( info.m_nAnalysID ==  nAnalysID)
				{
					//info.m_bIsOnLine = nstate ; 
					m_mapAllClientRoom[nRoomId].pVtAnalys->at(i).m_bIsOnLine = nstate ; 
					CString strHead = "";		// 在线的正常头像
					CString strHead2 = "";		// 离线时的灰化头像
					strHead.Format("%s\\%d_n.jpg", g_config.szUserHead,  info.m_nAnalysID);
					strHead2.Format("%s\\%d_g.jpg", g_config.szUserHead,  info.m_nAnalysID);

					// 在硬盘中找到了图片 
					if(GetFileAttributes(strHead) != -1 && ( GetFileAttributes(strHead2) != -1 ) )
					{
						if (nstate == 1)
						{
							pic_btn->SetNormalImage(strHead);
						}
						else
						{
							pic_btn->SetNormalImage(strHead2);
						}
					}
					else
					{
						if (nstate == 1)
						{
							pic_btn->SetNormalImage("public.png");
						}
						else
						{
							pic_btn->SetNormalImage("public_g.png");
						}
					}
					break ; // 找到了就退出  
				}
			}
		}
		else 
		{
			pNode = FindClientInRoom((node->data().numid).GetData()) ;
			if( !pNode)
				return ;
			if (nstate == 0)
			{
				if( strlen(pNode->m_szLGrayImgName) == 0 )
					pic_btn->SetNormalImage("pubic_g.png") ;
				else
					pic_btn->SetNormalImage(pNode->m_szLGrayImgName);
			}
			else
			{
				if( strlen(pNode->m_szLNomalImgName) == 0 )
					pic_btn->SetNormalImage("pubic.png") ;
				else
					pic_btn->SetNormalImage(pNode->m_szLNomalImgName);
			}
		}


	}

	void CFriendsUI::SetNodeName(Node* node ,int onlineNums /*= 0*/,int totalNums /*= 0*/ )
	{
		// node 不为空  且 是文件夹（包含其他节点）
		if( (node != NULL ) && ( node->folder() ) )
		{
			CLabelUI *pNickName = static_cast<CButtonUI*>(paint_manager_.FindSubControlByName(node->data().list_elment_ ,kNickNameControlName)) ; 
			if( pNickName)
			{
				CListContainerElementUI *pListElement = NULL ; 
				if( !m_dlgBuilder.GetMarkup()->IsValid() ) 
				{
#ifndef VER_UPIM_RONGYUN
					pListElement = static_cast<CListContainerElementUI*>(m_dlgBuilder.Create(_T("item_friend_list.xml"), (UINT)0, NULL, &paint_manager_));
#else
					pListElement = static_cast<CListContainerElementUI*>(m_dlgBuilder.Create(_T("item_friend_list_new_im.xml"), (UINT)0, NULL, &paint_manager_));
#endif

				}
				else 
				{
					pListElement = static_cast<CListContainerElementUI*>(m_dlgBuilder.Create((UINT)0, &paint_manager_));
				}
				if (pListElement == NULL)
					return ;
				CContainerUI* logo_img_container = static_cast<CContainerUI*>(paint_manager_.FindSubControlByName(pListElement, "logo_img"));
				if (logo_img_container != NULL)
					logo_img_container->SetAttribute("height", "30");
				TCHAR szBuf[MAX_PATH] = {0};
				CDuiString html_text  ; 

				if (node->data().has_child_)
				{
					if (node->data().child_visible_)
						html_text += level_expand_image_;
					else
						html_text += level_collapse_image_;

					_stprintf_s(szBuf, MAX_PATH - 1, _T("<x %d>"), level_text_start_pos_);
					html_text += szBuf;
				}
				html_text += node->data().text_;
				if (VER_UPIM_NUMS == 3 || VER_UPIM_NUMS == 4)
				{
					CDuiString strDispNums = _T("");

					strDispNums.Format(" %d/%d", onlineNums, totalNums);
					_stprintf_s(szBuf, MAX_PATH - 1, _T("%s"), strDispNums);
					html_text += szBuf;
				}
				if (VER_UPIM_NUMS == 4)
				{
					if (node->data().isvip == 1)
					{
						CDuiString strNoRightPic = _T("<x 5><i vipicon.png>");
						html_text += strNoRightPic;
					}
					if (node->data().hasright == 0)
					{
						CDuiString strNoRightPic = _T("<x 5><i noright.png>");
						html_text += strNoRightPic;
					}
				}
				node->data().onlinenum = onlineNums ; 
				node->data().totlenum  = totalNums ; 
				pNickName->SetFixedWidth(500);
				pNickName->SetShowHtml(true);
				pNickName->SetText(html_text);
			}
		}


	}
	void CFriendsUI::SetNodePic(Node* node, CString HeadImg)
	{
		if ((node != NULL) && !node->folder())
		{
			CButtonUI* pic_btn = static_cast<CButtonUI*>(paint_manager_.FindSubControlByName(node->data().list_elment_, kLogoButtonControlName));
			if( pic_btn)
				pic_btn->SetNormalImage(HeadImg);
		}
	}

	void CFriendsUI::SetNodeLastWord(Node* node, CString strLastWord, int nTime)
	{
		if ((node != NULL) && !node->folder())
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

	// 修改在线用户信息，直接修改fold的nickname字段，用html方式
	void CFriendsUI::SetNodeNums(Node* node, int nTotleNum, int nOnlineNum)
	{
		if (node)
		{
			// 		CLabelUI* l_nums = static_cast<CLabelUI*>(paint_manager_.FindSubControlByName(node->data().list_elment_, kNumsControlName));
			// 		if (l_nums != NULL)
			// 		{
			// 			CDuiString strDispNums = _T("");
			// 			strDispNums.Format("%d:%d", nOnlineNum, nTotleNum);
			// 			l_nums->SetText(strDispNums);
			// 		}
		}
	}

	void CFriendsUI::SetNodeFlash(Node* node, bool nFlash, int nDeirction /* = 0 */)
	{
		CButtonUI* pic_btn = static_cast<CButtonUI*>(paint_manager_.FindSubControlByName(node->data().list_elment_, kLogoButtonControlName));
		CContainerUI* logo_container = static_cast<CContainerUI*>(paint_manager_.FindSubControlByName(node->data().list_elment_, kLogoContainerControlName));
		if (pic_btn != NULL)
		{
			// 头像闪动时，以下方Container的坐标为基准，以免pos改变之后，下次基准失效
			RECT mRect = {0};
			RECT mRect2 = {0};
			RECT mRect3 = logo_container->GetPos();
			// 		mRect.top = mRect3.top;
			// 		mRect.bottom = mRect.top + 20;
			// 		mRect.left = mRect3.left;
			// 		mRect.right = mRect.left + 20;
			mRect.top = mRect3.top + 8;
			mRect.bottom = mRect.top + 40;
			mRect.left = mRect3.left +10;
			mRect.right = mRect.left + 40;

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


	int CALLBACK CFriendsUI::MyCompareProc(UINT_PTR lParam1, UINT_PTR lParam2, UINT_PTR lParamSort )
	{
		CControlUI *pControl1 = reinterpret_cast<CControlUI*>(lParam1) ;  
		CControlUI *pControl2 = reinterpret_cast<CControlUI*>(lParam2) ; 
		if( pControl2 || pControl1)
		{
			Node *pNode1 = reinterpret_cast<Node*>(static_cast<CListContainerElementUI*>(pControl1->GetInterface(_T("ListContainerElement")))->GetTag()) ; 
			Node *pNode2 = reinterpret_cast<Node*>(static_cast<CListContainerElementUI*>(pControl2->GetInterface(_T("ListContainerElement")))->GetTag() ); 
			if( pNode1->num_children() > 0 || pNode2->num_children() > 0 )
				return 0 ; 
			else
			{
				bool bOnline1 = FALSE,bOnline2 = FALSE; 
				CString strName1 = _T(" ") ,strName2 = _T(" ") ;

				USER_ROOMCLIENT_INFO *pClient1 = FindClientInRoom(pNode1->data().numid.GetData()) ;
				USER_ROOMCLIENT_INFO *pClient2 = FindClientInRoom(pNode2->data().numid.GetData()) ;
				if( pClient1)
				{
					bOnline1 = pClient1->m_bIsOnLine ; 
					strName1.Format("%s" ,pClient1->m_szUPID) ; 
				}
				if( pClient2)
				{
					bOnline2 = pClient2->m_bIsOnLine ;
					strName2.Format("%s" ,pClient2->m_szUPID) ; 
				}
				if( bOnline1 != bOnline2 )
				{
					if( bOnline1 > bOnline2 )
						return 0; 
					else 
						return 1;
				}
				else
				{
					if( strcmp(strName1.GetBuffer() ,strName2.GetBuffer() ) <= 0 )
						return 0 ; 
					else
						return 1 ; 
				}
			}
		}
		//return TRUE ; 
	}

	CRecordCallBack::CRecordCallBack()
	{

	}
	CRecordCallBack::~CRecordCallBack()
	{

	}

	LPCSTR CRecordCallBack::GetItemText(CControlUI* pList, int iItem, int iSubItem)
	{
		CDuiString str =_T("") ; 
		str = pList->GetText()  ; 
		if (m_vtRoomClient[iItem].m_bIsOnLine)
			return "1" ; 
		else
			return "0" ; 
		//return NULL;
	}

	std::vector<USER_ROOMCLIENT_INFO> CRecordCallBack::GetClientRoom()
	{
		return m_ClientRoom ; 
	}
	void CRecordCallBack::SetClientRoom(std::vector<USER_ROOMCLIENT_INFO> vec)
	{
		this->m_ClientRoom = vec ; 
	}


	bool CFriendsUI::SortList(Node *pNode )
	{

		// 获取该节点的孩子节点列表 
		std::vector<Node*> childList = pNode->GetChildList() ; 
		int beginIndex = childList.at(0)->data().list_elment_->GetIndex() ; 
		int lastIndex = childList.at(childList.size() - 1)->data().list_elment_->GetIndex() ; 
		vector<CListContainerElementUI *> listElements; 

		int iCount = this->GetCount() ; 

		for(int i= beginIndex ; ((i<= lastIndex) && (lastIndex < iCount)) ; i++)
		{
			CControlUI* pControl = this->GetItemAt(i);
			if( pControl != NULL)
			{
				CListContainerElementUI* pListElement = static_cast<CListContainerElementUI*>(pControl);
				listElements.push_back(pListElement);
			}
		}
		sort(listElements.begin() ,listElements.end(),CompareWithOnlineAndName);

		vector<CListContainerElementUI *>::iterator listElementIter;
		int index =beginIndex;
		for(listElementIter = listElements.begin();listElementIter!=listElements.end();listElementIter++)
		{
			CListContainerElementUI *pListElement = *listElementIter;
			SetItemIndex(pListElement,index);
			index++;
		}
		return true ; 
	}
	//////////////////////////////////////////////////////////////
	// 排序 改排序只能完成分析师界面的排序 客户界面的排序要对改函数简
	// 单的更改
	// 将pInsertNode插入到合适的位置就OK
	// wuchao  add at 2016-03-29 
	//////////////////////////////////////////////////////////////
	bool CFriendsUI::SortList()
	{
		//////////////////////////////////////////////////////////////////////
		// wuchao modify at 20160412 
		// 先将每个节点的对应的底层控件获取  然后排序这些控件  
		vector<CListContainerElementUI *> listElements; 

		int iCount = this->GetCount() ; 

		for(int i= 0 ; i< iCount; i++)
		{
			CControlUI* pControl = this->GetItemAt(i);
			if( pControl != NULL)
			{
				CListContainerElementUI* pListElement = static_cast<CListContainerElementUI*>(pControl);
				listElements.push_back(pListElement);
			}
		}

		vector<CListContainerElementUI*>::iterator itBegin = listElements.begin() ; 
		if( itBegin != listElements.end())
			itBegin++ ; // 第一个不参加排序
		else
			return FALSE ;

		if( itBegin != listElements.end() )
			sort(itBegin ,listElements.end(),CompareWithOnlineAndName);
		else
			return FALSE ; 

		vector<CListContainerElementUI *>::iterator listElementIter;
		int index =0;
		for(listElementIter = listElements.begin();listElementIter!=listElements.end();listElementIter++)
		{
			CListContainerElementUI *pListElement = *listElementIter;
			SetItemIndex(pListElement,index);
			index++;
		}
		return true ; 
	}
	void CRecordCallBack::SortClientRoom()
	{
		sort(m_ClientRoom.begin() ,m_ClientRoom.end() ,SortByNameAndOnlineClient) ; 
	}

	bool CompareWithOnlineAndName(CListContainerElementUI * item1,CListContainerElementUI * item2)
	{
		CLabelUI *pLabelItem1 = (CLabelUI *)item1->FindSubControl(_T("nickname")) ; 
		CLabelUI *pLabelItem2 = (CLabelUI *)item2->FindSubControl(_T("nickname")) ; 
		int tempIndex ;

		if(!pLabelItem1 || !pLabelItem2)
			return false ; 
		 
		CDuiString strName1 = pLabelItem1->GetText() ; 
		CDuiString strName2 = pLabelItem2->GetText() ; 

		USER_ROOMCLIENT_INFO client1 ,client2 ; 
		memset(&client1 ,0,sizeof(USER_ROOMCLIENT_INFO)) ;
		COPY_USERID(client1.m_szUPID ,strName1.GetData()) ; 
		memset(&client2 ,0,sizeof(USER_ROOMCLIENT_INFO)) ;
		COPY_USERID(client2.m_szUPID ,strName2.GetData()) ;

		std::vector<USER_ROOMCLIENT_INFO>::iterator itFindClient1 = find( m_vtRoomClient.begin() ,m_vtRoomClient.end() ,client1) ;
		std::vector<USER_ROOMCLIENT_INFO>::iterator itFindClient2 = find( m_vtRoomClient.begin() ,m_vtRoomClient.end() ,client2) ;
		
		int isClient1Online = 0 ; 
		int isClient2Online = 0 ;

		if( itFindClient1 != m_vtRoomClient.end() )
		{
			if(itFindClient1->m_bIsOnLine)
				isClient1Online = 1 ; 
		}

		if( itFindClient2 != m_vtRoomClient.end() )
		{
			if(itFindClient2->m_bIsOnLine)
				isClient2Online = 1 ;  
		}

		if( isClient1Online != isClient2Online)
		{
			if( isClient1Online > isClient2Online )
			{
/*				tempIndex = item1->GetIndex(); 
				item1->SetIndex(item2->GetIndex()) ; 
				item2->SetIndex(tempIndex) ;*/ 
				return TRUE ; 
			}
			else
				return FALSE ; 
		}
		else
		{
			//return strcmp(strName1,strName2) < 0 ; 
			if( strcmp(strName1,strName2) < 0 )
			{
				//tempIndex = item1->GetIndex(); 
				//item1->SetIndex(item2->GetIndex()) ; 
				//item2->SetIndex(tempIndex) ; 
				return TRUE ;
			}
			else
				return FALSE ; 
		}

		//return true ; 
	}
} // namespace DuiLib