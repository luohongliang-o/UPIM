#ifndef UIMESSAGES_HPP
#define UIMESSAGES_HPP

//////////////////////////////////////////////////////////////////////////
// CMessagesUI主要用于 历史联系人列表以及新消息提醒列表
// CListUI::SortItems 排序排序
// SetItemIndex 可以用来排序
#include "UIListCommonDefine.h"

namespace DuiLib
{

class CMessagesUI : public CListUI
{
public:
	enum {SCROLL_TIMERID = 10};

	CMessagesUI(CPaintManagerUI& paint_manager, EUITYPE m_uiType);
	~CMessagesUI();

	bool Add(CControlUI* pControl);
	bool AddAt(CControlUI* pControl, int iIndex);
	bool Remove(CControlUI* pControl);
	bool RemoveAt(int iIndex);
	void RemoveAll();
	void DoEvent(TEventUI& event);
	Node* GetRoot();
	Node* AddNode(const FriendListItemInfo& item, Node* parent = NULL);
	bool RemoveNode(Node* node);
	void SetChildVisible(Node* node, bool visible);
	bool CanExpand(Node* node) const;
	bool SelectItem(int iIndex, bool bTakeFocus = false);
	Node* SelectNodeByID(CString strUserID);
	bool SetNickNameColor(Node* mNode, COLORREF textcolor);
	bool RemoveNodeMapInfo(Node* node);

private:
	EUITYPE uiType;
	Node*	root_node_;
	LONG	delay_deltaY_;
	DWORD	delay_number_;
	DWORD	delay_left_;
	CDuiRect	text_padding_;
	int level_text_start_pos_;
	CDuiString level_expand_image_;
	CDuiString level_collapse_image_;
	CPaintManagerUI& paint_manager_;

    CDialogBuilder m_dlgBuilder;
	std::map<CString, Node*> allNodeInfo;			//将NODE指针存储起来，以便根据item的Id来查找节点
	typedef map<CString, Node*>::iterator NODE_ITER;
};

} // DuiLib

#endif // UIMESSAGES_HPP