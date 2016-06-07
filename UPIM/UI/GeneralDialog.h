#ifndef GENERAL_HPP
#define GENERAL_HPP

#include "SkinChangeEvent.h"


class CGeneralDialog : public WindowImplBase, public SkinChangedReceiver
{
public:
	CGeneralDialog(const CDuiString& bgimage, DWORD bkcolor, ECREATEDIALOGTYPE m_CreateType, CDuiString strTitle_=_T(""));
	~CGeneralDialog();

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

protected:	
	void Notify(TNotifyUI& msg);
	void OnPrepare(TNotifyUI& msg);
	void OnTimer(TNotifyUI& msg);
	void OnListItemClick(TNotifyUI& msg);
	void OnOKBtnClick(TNotifyUI& msg);
	void OnCloseBtnClick(TNotifyUI& msg);
public:
	void OnExit();
	void UISetInitSize();									// 外部父窗口大小改变时，调用此方法自适应大小
	CTextUI *m_pDialog_name, *m_pDialog_Errorstr;
private:
	CDuiString	m_strDialogName;
	ECREATEDIALOGTYPE CreateType;
	CDuiString bgimage_;
	DWORD bkcolor_;

	CEditUI *m_pGroupName;
	CComboUI *m_pGroupType, *m_pLoginType;
	CButtonUI *m_pCloseBtn, *m_pOKBtn;
	CContainerUI *m_pPannel_GreateGroup, *m_pPannel_LoginIN;
	CEditUI *m_pLoginName, *m_pLoginPSW;
	CRichEditUI* m_pGroupNote;
};

#endif // GENERAL_HPP