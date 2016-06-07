#ifndef CHATDIALOG_HPP
#define CHATDIALOG_HPP

#include "SkinChangeEvent.h"
#include "UIFriends.h"
#include "FaceSelDlg.h"
#include "FaceList.h"
#include "IImageOle.h"
#include "FileDialogEx.h"
#include "UIMenu.h"
#include "PicBarDlg.h"
#include "../MsgQueueSendMng.h"
#include "RichEditUtil.h"

class CFontInfo				// 字体信息
{
public:
	CFontInfo(void);
	~CFontInfo(void);

public:
	int m_nSize;			// 字体大小
	COLORREF m_clrText;		// 字体颜色
	tstring m_strName;		// 字体名称
	BOOL m_bBold;			// 是否加粗
	BOOL m_bItalic;			// 是否倾斜
	BOOL m_bUnderLine;		// 是否带下划线
};

class ChatDialog : public WindowImplBase, public SkinChangedReceiver
{
public:
	ChatDialog(const CDuiString& bgimage, DWORD bkcolor, const FriendListItemInfo& myselft_info, const FriendListItemInfo& friend_info, bool bGroupSend=false);
	~ChatDialog();

public:
	LPCTSTR GetWindowClassName() const;	
	virtual void OnFinalMessage(HWND hWnd);
	virtual void InitWindow();
	virtual LRESULT ResponseDefaultKeyEvent(WPARAM wParam);
	virtual CDuiString GetSkinFile();
	virtual CDuiString GetSkinFolder();
	virtual CControlUI* CreateControl(LPCTSTR pstrClass);
	virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
	virtual LRESULT OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual LRESULT OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual LRESULT OnSetFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual BOOL Receive(SkinChangedParam param);
	virtual LRESULT HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	void SetTextColor(DWORD dwColor);

    void SendMsg();
	void SendMsg_More();
	COMPLEX_MSG_DATA* GetMsgData(BOOL bOnLine=TRUE,CString strFileInfo="",CString strProductName="");
	COMPLEX_MSG_DATA* GetMsgData_More(BOOL bOnLine=TRUE,CString strFileInfo="",CString strProductName="");
	void DeleteMsgData(COMPLEX_MSG_DATA*& pMsgData);
	void InsertChatHistory(RECEIVED_MSG *pMsg, LPCTSTR szSendName);
	void InsertChatHistory_More(RECEIVED_MSG *pMsg, LPCTSTR szSendName, LPCTSTR szSendId);
	BOOL ReadXmlMsgData(LPCSTR pszBuf, CRichEditUI* pRichEdit);

protected:	
	void Notify(TNotifyUI& msg);
	void OnPrepare(TNotifyUI& msg);
	void OnExit(TNotifyUI& msg);
	void OnTimer(TNotifyUI& msg);

	void OnBtn_Min(TNotifyUI& msg);
	void OnBtn_Max(TNotifyUI& msg);
	void OnBtn_Restore(TNotifyUI& msg);
	void OnBtn_Close(TNotifyUI& msg);
	void OnCbo_SelChange_FontName(TNotifyUI& msg);
	void OnCbo_SelChange_FontSize(TNotifyUI& msg);
	void OnBtn_Bold(TNotifyUI& msg);
	void OnBtn_Italic(TNotifyUI& msg);
	void OnBtn_UnderLine(TNotifyUI& msg);
	void OnBtn_Color(TNotifyUI& msg);
	void OnBtn_Font(TNotifyUI& msg);
	void OnBtn_Face(TNotifyUI& msg);
	void OnBtn_Image(TNotifyUI& msg);
	void OnBtn_Send(TNotifyUI& msg);

	void OnMenu_Cut(TNotifyUI& msg);		// “剪切”菜单
	void OnMenu_Copy(TNotifyUI& msg);		// “复制”菜单
	void OnMenu_Paste(TNotifyUI& msg);		// “粘贴”菜单
	void OnMenu_SelAll(TNotifyUI& msg);		// “全部选择”菜单
	void OnMenu_Clear(TNotifyUI& msg);		// “清屏”菜单
	void OnMenu_ZoomRatio(TNotifyUI& msg);	// “显示比例”菜单
	void OnMenu_SaveAs(TNotifyUI& msg);		// “另存为”菜单
	LRESULT OnMouseMove(UINT uMsg, WPARAM wParam, LPARAM lParam);
	LRESULT OnRButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam);
	LRESULT OnLButtonDblClk(UINT uMsg, WPARAM wParam, LPARAM lParam);
	LRESULT OnFaceCtrlSel(UINT uMsg, WPARAM wParam, LPARAM lParam);	// “表情”控件选取消息

	void _RichEdit_ReplaceSel(CRichEditUI * pRichEdit, LPCTSTR lpszNewText);
	void _RichEdit_ReplaceSel_More(CRichEditUI * pRichEdit, LPCTSTR lpszNewText, CFontInfo mfontInfo);
	BOOL _RichEdit_InsertFace(CRichEditUI * pRichEdit, LPCTSTR lpszFileName, int nFaceId, int nFaceIndex);
	BOOL HandleSysFaceId(CRichEditUI * pRichEdit, LPCTSTR& p, tstring& strText);
	BOOL HandleSysFaceIndex(CRichEditUI * pRichEdit, LPCTSTR& p, tstring& strText);
	BOOL HandleCustomPic(CRichEditUI * pRichEdit, LPCTSTR& p, tstring& strText);
	BOOL HandleSysFaceId_More(CRichEditUI * pRichEdit, LPCTSTR& p, tstring& strText, CFontInfo mfontInfo);
	BOOL HandleSysFaceIndex_More(CRichEditUI * pRichEdit, LPCTSTR& p, tstring& strText, CFontInfo mfontInfo);
	BOOL HandleCustomPic_More(CRichEditUI * pRichEdit, LPCTSTR& p, tstring& strText, CFontInfo mfontInfo);
	void AddMsg(CRichEditUI * pRichEdit, LPCTSTR lpText);
	void AddMsg_More(CRichEditUI * pRichEdit, LPCTSTR lpText, CFontInfo mfontInfo);
	void AddMsgToSendEdit(LPCTSTR lpText);
	void AddMsgToRecvEdit(LPCTSTR lpText);
	void AddMsgToRecvEdit_More(LPCTSTR lpText, CFontInfo mfontInfo, bool bisselfmsg=false, CDuiString sid="", CDuiString sname="", CDuiString strSendTime="");

private:
	void FontStyleChanged();
private:	
	CButtonUI * m_pMinSysBtn, * m_pMaxSysBtn, * m_pRestoreSysBtn, * m_pCloseSysBtn;
	CButtonUI * m_pFontBtn, * m_pFaceBtn, * m_pImageBtn;
	CComboUI * m_pFontNameCombo, * m_pFontSizeCombo;
	COptionUI * m_pBoldBtn, * m_pItalicBtn, * m_pUnderLineBtn;
	CButtonUI * m_pColorBtn;
	CButtonUI * m_pCloseBtn, * m_pSendBtn;
	CRichEditUI* m_pSendEdit, * m_pRecvEdit;

	CFaceSelDlg m_FaceSelDlg;
	CPicBarDlg m_PicBarDlg;

	CFaceList m_FaceList;
	CFontInfo m_fontInfo;
	POINT m_ptRBtnDown;
	tstring m_strCheckMenu;
	IImageOle * m_pLastImageOle;
	int m_cxPicBarDlg, m_cyPicBarDlg;

	bool emotion_timer_start_;

	bool bold_;
	bool italic_;
	bool underline_;
	DWORD text_color_;
	DWORD font_size_;
	CDuiString font_face_name_;

	CDuiString bgimage_;
	DWORD bkcolor_;
	FriendListItemInfo myselft_;
	FriendListItemInfo friend_;
	bool bGroupSend_;
public:
	std::vector<UPIMID> m_vtUPIMID;
};

#endif // CHARTDIALOG_HPP