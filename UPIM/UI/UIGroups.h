#ifndef UIGROUPS_HPP
#define UIGROUPS_HPP

#include "UIListCommonDefine.h"

namespace DuiLib
{

class CGroupsUI : public CListUI
{
public:
	enum {SCROLL_TIMERID = 10};

	CGroupsUI(CPaintManagerUI& paint_manager, EUITYPE m_uiType);
	~CGroupsUI();

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
	void SetNodeFlash(Node* node, bool nFlash, int nDeirction = 0);
	void SetNodeLastWord(Node* node, CString strLastWord, int nTime);

private:
	int kGroupListItemNormalHeight;
	int kGroupListItemSelectedHeight;

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

#endif // UIGROUPS_HPP