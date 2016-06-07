#ifndef ADDMEMBER_HPP
#define ADDMEMBER_HPP

#include "SkinChangeEvent.h"
#include "UIGroups.h"

class AddMemberDialog : public WindowImplBase, public SkinChangedReceiver
{
public:
	AddMemberDialog(const CDuiString& bgimage, DWORD bkcolor, ROOMID RoomID_ = 0, short RoomType_ = 0 );
	~AddMemberDialog();

public:
	LPCTSTR GetWindowClassName() const;	
	virtual void OnFinalMessage(HWND hWnd);
	virtual void InitWindow();
	virtual LRESULT ResponseDefaultKeyEvent(WPARAM wParam);
	virtual CDuiString GetSkinFile();
	virtual CDuiString GetSkinFolder();
	virtual CControlUI* CreateControl(LPCTSTR pstrClass);
	virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
	virtual LRESULT HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual LRESULT OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual LRESULT OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual LRESULT OnSetFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual BOOL Receive(SkinChangedParam param);

	void LoadMemList();
	void CreateGroupNode();
	void CreateDeptNode();
	void CreateFriendNode();
	BOOL IsUserInList(VUPID *thisID);
	BOOL RemoveUser(VUPID *thisID);
protected:	
	void Notify(TNotifyUI& msg);
	void OnPrepare(TNotifyUI& msg);
	void OnExit();
	void OnTimer(TNotifyUI& msg);
	void OnListItemClick(TNotifyUI& msg);
	void OnListItemActivate(TNotifyUI& msg);
	void OnOKBtnDown(TNotifyUI& msg);
private:	
	CDuiString bgimage_;
	DWORD bkcolor_;
	ROOMID nRoomID;
	short nRoomType;
	CGroupsUI* pGroupsList;
	CGroupsUI* pGroupsList2;
	CTextUI* pMemNumText;
	
public:
	vector<VUPID> vtSelectID;		// 选择的准备加入ID
	vector<VUPID> vtExistID;		// 初始化时群里原有的ID
	int nFristTotleNum;
};

#endif // CHARTDIALOG_HPP