#ifndef USEROPENRIGHTS_HPP
#define USEROPENRIGHTS_HPP

class CUserOpenRights : public WindowImplBase
{
public:
	CUserOpenRights();
	~CUserOpenRights();

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
	void OnCloseBtnClick(TNotifyUI& msg);
	void OnOpenBtnClick(TNotifyUI& msg);
public:
	void OnExit();
private:
	CButtonUI * m_pMinSysBtn, * m_pMaxSysBtn, * m_pRestoreSysBtn, * m_pCloseSysBtn;
	CButtonUI * m_pCloseBtn, * m_pOpenBtn;
};

#endif // USEROPENRIGHTS_HPP