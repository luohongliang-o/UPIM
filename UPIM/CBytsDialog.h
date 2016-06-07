#ifndef _USECBYTSDIALOG_
#define _USECBYTSDIALOG_


/////////////////////////////////////////////////////////////////////////////////////
//
// 表扬投诉分析对话框  

/////////////////////////////////////////////////////////////////////
// 客户投诉OR表扬 结构体 
/////////////////////////////////////////////////////////////////////
struct ClientComment
{
	int nType ; // 投诉为1  表扬为2 
	CString strPhoneNum ; // 电话号码 
	CString strBehavior ; // 举报分析师的行为  
	CString strMsg ;      // 补充说明 

	ClientComment()
	{
		nType = 0 ; 
		strPhoneNum = strBehavior = strMsg = _T(" ") ; 
	}
};


class CBytsDialog : public WindowImplBase
{
public:
	CBytsDialog(CDuiString strMyselfName ,CDuiString strMyselfID ,CDuiString strAnayName ,CDuiString strAnayID);
	CBytsDialog(LPCTSTR lpMyselfName ,LPCTSTR lpMyselfID ,LPCTSTR lpAnayName ,LPCTSTR lpAnayID);
	~CBytsDialog();

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

	void OnBtnSubmit(TNotifyUI& msg) ; 
	BOOL CheckPhoneNum() ;

	// 显示Web页面 
	CWebBrowserUI* m_pActiveX_Web ;

protected:	
	void Notify(TNotifyUI& msg);
	void OnPrepare(TNotifyUI& msg);
	void OnTimer(TNotifyUI& msg);
	void OnListItemClick(TNotifyUI& msg);
	void OnCloseBtnClick(TNotifyUI& msg);
	void OnOpenBtnClick(TNotifyUI& msg);
	void OnSelectCheckBox(TNotifyUI& msg) ;
	void SwitchInterface(int nSwitch) ; 
	void OnBtn_Min(TNotifyUI& msg) ;
	void OnBtn_Max(TNotifyUI& msg) ;

public:
	void OnExit();
private:
	CButtonUI * m_pMinSysBtn, * m_pMaxSysBtn, * m_pRestoreSysBtn, * m_pCloseSysBtn;
	CButtonUI * m_pSubmitBtn;


	CRichEditUI *m_pBCSMEdit ; // 表扬补充说明Edit 
	CRichEditUI *m_pTSBCSMEdit ; // 投诉补充说明Edit

	CEditUI *m_pPhoneNumEdit ; 

	CCheckBoxUI *m_pTSCheckBox;  // 投诉分析师   
	CCheckBoxUI *m_pBYCheckBox;  // 表扬分析师 

	CComboUI* m_pFxsBehaviorList; // 举报分析师行为列表 

	CTabLayoutUI* m_pControlSwitch; // 切换界面控件 
	CDuiString m_strMyselfName ;
	CDuiString m_strMyselfID ;
	CDuiString m_strAnayName ;
	CDuiString m_strAnayID ; 

	
	LPCTSTR m_lpMyselfName ;
	LPCTSTR m_lpMyselfID ;
	LPCTSTR m_lpAnayName ;
	LPCTSTR m_lpAnayID ;
	 
private:
	int m_nSwitch ; // 显示投诉分析师界面还是表扬分析师界面控制变量  默认为0 显示投诉分析师界面 
};

#endif // _USECBYTSDIALOG_
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////