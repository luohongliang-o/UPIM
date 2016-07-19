#ifndef CUSERMRGDIALOG_HPP
#define CUSERMRGDIALOG_HPP

#include "ListEx.h"
#include "../../Common/json/json.h"  //新建用户分组中


class CUserMrgDialog : public WindowImplBase
{
public:
	CUserMrgDialog(ECREATEDIALOGTYPE m_CreateType);
	~CUserMrgDialog();

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

protected:	
	void Notify(TNotifyUI& msg);
	void OnPrepare(TNotifyUI& msg);
	void OnTimer(TNotifyUI& msg);
	void OnListItemClick(TNotifyUI& msg);
	void OnOKBtnClick(TNotifyUI& msg);
	void OnCloseBtnClick(TNotifyUI& msg);

	void OnBtn_Min(TNotifyUI& msg);
	void OnBtn_Max(TNotifyUI& msg);
	void OnBtn_Restore(TNotifyUI& msg);
	void OnBtn_Close(TNotifyUI& msg);
	void OnBtn_AddGroup(TNotifyUI& msg);
	void OnBtn_DelGroup(TNotifyUI& msg) ; 
	void OnBtn_AllClient(TNotifyUI& msg);
	void OnCbo_SelChange_Group(TNotifyUI& msg);
	void OnBtnFenzuList(TNotifyUI& msg, CString strName) ; 
	int AddGroupInfo(CString strGroupName, int nNums);
	void AddList(CString strUpid, CString strGroupName);
	void ChangeFenZu(int nIndex);
	void ShowAllClient();
	void OnCheckBoxSelected();
	void LoadSelfGroup();
	void RemoveCustomFromGroup(std::vector<int> vec,int len) ; 
	void DelGroupInfo(CString strGroupName, int nNums) ;

public:
	void OnExit();
	void AddGroup(CString strGroupName ,CString strGroupID ) ; 
	void DelGroup(CString strGroupName ,CString strGroupID ) ;
	int  ConvertGroupIDToControl(int nGroupID) ; //好友管理器中客户分组中，根据分组ID得到控件btnID
	// nType表示向分组增加还是移除成员
	// 1表示增加成员
	// 0 表示移除成员 
	void UpdateListBtnName(int nBtnID,int iCustomCount,int nType) ;

	// 删除分组Btn
	bool DeleteFenZuBtn( int btnID ) ;

public:
	COptionUI* m_pFenZu[USERMRG_FENZU+1];	// 客户分组 + 一个固定分组
private:
	ECREATEDIALOGTYPE CreateType;
	CButtonUI * m_pMinSysBtn, * m_pMaxSysBtn, * m_pRestoreSysBtn, * m_pCloseSysBtn, *m_pUserMrgAddBtn;
	CButtonUI * m_pUserMrgDelBtn ; 
	CButtonUI * m_pMoveto ;   // 移动到
	COptionUI* m_pAllClient;				// 全部客户
	CControlUI* m_pFenZuList;
	CComboUI* m_pGroupList;
	CListUIEx *m_pList;
	CCheckBoxUI *m_pChenkBox;
	int nGroupIndex;
	int nChooseGroup;

	std::map<int, int>	m_mapGroupIdToControls;
	
};

#endif // CUSERMRGDIALOG_HPP