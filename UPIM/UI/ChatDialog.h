#ifndef CHATDIALOG_HPP
#define CHATDIALOG_HPP

#include "SkinChangeEvent.h"
#include "UIGroups.h"
#include "FaceSelDlg.h"
#include "FaceList.h"
#include "IImageOle.h"
#include "FileDialogEx.h"
#include "../MsgQueueSendMng.h"
#include "../resource.h"
#include "RichEditUtil.h"
#include "CustomWebEventHandler.h"
#include "CalendarDlg.h"
#include "CBytsDialog.h"

class ChatDialog : public WindowImplBase, public SkinChangedReceiver
{
public:
	ChatDialog(const CDuiString& bgimage, DWORD bkcolor, const FriendListItemInfo& myselft_info, const FriendListItemInfo& friend_info, EDIALOGTYPE eDialogType_=eDIALOG_Single);
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
	virtual LRESULT HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual LRESULT OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual LRESULT OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual BOOL Receive(SkinChangedParam param);

	void OnMenuClick(WPARAM wParam , LPARAM lParam);
	void OnUserState(WPARAM wParam , LPARAM lParam);
    void SendMsg();
	void SendMsg_More();
	COMPLEX_MSG_DATA* GetMsgData(BOOL bOnLine=TRUE, CString strFileInfo=_T(""), CString strProductName=_T(""));
	COMPLEX_MSG_DATA* GetMsgData_More(tstring strText, BOOL bOnLine=TRUE, CString strFileInfo=_T(""), CString strProductName=_T(""));
	void DeleteMsgData(COMPLEX_MSG_DATA*& pMsgData);
	void InsertChatHistory(RECEIVED_MSG *pMsg, LPCTSTR szSendName);
	void InsertChatHistory_More(RECEIVED_MSG *pMsg, LPCTSTR szSendName, LPCTSTR szSendId);
	BOOL ReadXmlMsgData(LPCSTR pszBuf, CRichEditUI* pRichEdit);
	void ShakeWindow();
	void RecvPublicInfo();
	void InsertRecvMsg();				// 显示已收到的消息
	void InsertHisMsg();				// 显示已收到的历史消息
	void ShowHisRecord();				// 显示聊天记录
	void ShowRightHisRecord();			// 聊天界面右侧显示聊天记录
	void ShowHisRecordPageByPage( int nStart ,int nEnd ) ;  // 聊天记录分页显示
	long  GetHisRecordCount() ;          // 获取聊天记录的总数目  

	void TestSqlite();
	void OnExit();
	void DeleteMember(LPCTSTR szID, short nUserType);
	void LoadTeamMember();
	void OnRichEditLink(CDuiString strLink);
	void AddTimer(UINT nTimerID, UINT uElapse);
	void KillTimer(UINT nTimerID);
	BOOL CheckTextInput();
	void GetRootRect(CRect &rectInit);
	
protected:	
	void Notify(TNotifyUI& msg);
	void OnPrepare(TNotifyUI& msg);
	void OnTimer(TNotifyUI& msg);
	void RoomMember(TNotifyUI& msg);
	void RoomMemberClick(TNotifyUI& msg);
	void RoomMemberRClick(TNotifyUI& msg);
	void RoomMember_ChooseChat(TNotifyUI& msg);
	void RoomMember_Add(TNotifyUI& msg);
	void RoomMember_Del(TNotifyUI& msg);
	//////////////////////////////////////////////////////////////////////////
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
	void OnBtn_Shake(TNotifyUI& msg);
	void OnBtn_Screenshots(TNotifyUI& msg);
	void OnBtn_Send(TNotifyUI& msg);
	
	//IM2.0
	void OnBtn_WenZhang(TNotifyUI& msg);
	void OnBtn_PinZhong(TNotifyUI& msg);
	void OnBtn_HisRecord(TNotifyUI& msg);
	void OnBtn_Right_SearchBar(TNotifyUI& msg);
	void OnBtn_Right_RiLi(TNotifyUI& msg);
	void OnBtn_Right_PageBegin(TNotifyUI& msg);
	void OnBtn_Right_PageUp(TNotifyUI& msg);
	void OnBtn_Right_PageDown(TNotifyUI& msg);
	void OnBtn_Right_PageEnd(TNotifyUI& msg);
	void OnBtn_Right_SearchOK(TNotifyUI& msg);
	void OnBtn_Right_Close(TNotifyUI& msg);
	void OnBtn_BYTSAnalyst(TNotifyUI& msg) ; 
	void OnBtn_ShouCangAnalyst(TNotifyUI& msg) ; 

	//////////////////////////////////////////////////////////////////////////
	void OnMenu_Cut(TNotifyUI& msg);			// “剪切”菜单
	void OnMenu_Copy_Recv();					// “复制”菜单
	void OnMenu_Copy_Send();					// “复制”菜单
	void OnMenu_Paste(TNotifyUI& msg);			// “粘贴”菜单
	void OnMenu_SelAll_Recv();					// “全部选择”菜单
	void OnMenu_SelAll_Send();					// “全部选择”菜单
	void OnMenu_Clear(TNotifyUI& msg);			// “清屏”菜单
	void OnMenu_ZoomRatio(LPCTSTR lpZoom);		// “显示比例”菜单
	void OnMenu_SaveAs(int nType);				// “另存为”菜单
	//////////////////////////////////////////////////////////////////////////
	LRESULT OnLoadRoomList(WPARAM wParam, LPARAM lParam);
	LRESULT OnMouseMove(UINT uMsg, WPARAM wParam, LPARAM lParam);
	LRESULT OnRButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam);
	LRESULT OnLButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam);
	LRESULT OnLButtonDblClk(UINT uMsg, WPARAM wParam, LPARAM lParam);
	LRESULT OnFaceCtrlSel(UINT uMsg, WPARAM wParam, LPARAM lParam);	// “表情”控件选取消息
	LRESULT OnChooseHisRecordByDateTime( UINT uMsg, WPARAM wParam, LPARAM lParam ) ;  // 按照日期筛选聊天记录
	LRESULT OnKeyUp(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled); 
	void _RichEdit_ReplaceSel(CRichEditUI * pRichEdit, LPCTSTR lpszNewText);
	void _RichEdit_ReplaceSel_More(CRichEditUI * pRichEdit, LPCTSTR lpszNewText, CFontInfo mfontInfo);
	BOOL _RichEdit_InsertFace(CRichEditUI * pRichEdit, LPCTSTR lpszFileName, int nFaceId, int nFaceIndex, IMAGE_INFO *pII = NULL);
	BOOL HandleSysFaceId(CRichEditUI * pRichEdit, LPCTSTR& p, tstring& strText);
	BOOL HandleSysFaceIndex(CRichEditUI * pRichEdit, LPCTSTR& p, tstring& strText);
	BOOL HandleCustomPic(CRichEditUI * pRichEdit, LPCTSTR& p, tstring& strText, BOOL bPaste = FALSE);
	BOOL HandleSysFaceId_More(CRichEditUI * pRichEdit, LPCTSTR& p, tstring& strText, CFontInfo mfontInfo);
	BOOL HandleSysFaceIndex_More(CRichEditUI * pRichEdit, LPCTSTR& p, tstring& strText, CFontInfo mfontInfo);
	BOOL HandleCustomPic_More(CRichEditUI * pRichEdit, LPCTSTR& p, tstring& strText, CFontInfo mfontInfo);
	BOOL HandleCustomPic_More2(CRichEditUI * pRichEdit, tstring strFileName, BOOL bSend = FALSE, BOOL bTimeout = FALSE);
	void AddMsg(CRichEditUI * pRichEdit, LPCTSTR lpText, BOOL bPaste = FALSE);
	void AddMsg_More(CRichEditUI * pRichEdit, LPCTSTR lpText, CFontInfo mfontInfo);
	void AddMsgToSendEdit(LPCTSTR lpText, BOOL bPaste = FALSE);
	void AddMsgToRecvEdit(LPCTSTR lpText);
	void AddMsgToRecvEdit_Text(LPCTSTR lpText);
	void AddMsgToRichEdit_More(CRichEditUI* m_pEdit, LPCTSTR lpText, CFontInfo mfontInfo, bool bisselfmsg=false, bool bshowid=true, CDuiString sid=_T(""), CDuiString sname=_T(""), CDuiString strSendTime=_T(""), bool bFromXML=false);
	void AddMsgToRecvEdit_Link(LPCTSTR lpText);

	BOOL GetObjectInfoPic(LPCTSTR& p, tstring& strFileName);
	void HandleObjectInfo(tstring strText, COMPLEX_MSG_DATA* pData);
	LPCTSTR  GetLastErrorStr();
	void OnRefreshPic();
	void SetRichEditCallback(CRichEditUI* m_pEdit);

public:
	//群发消息的时候，储存分组每个用户的ID;讨论组中，储存讨论组成员ID
	std::vector<UPIMID> m_vtUPIMID;
	FriendList_ITER friend_iterator;
	int m_nErrorCode;
	bool bIsInitWindow;
private:
	COptionUI* m_pTgHead[TG_LAOSHINUM];		// 聊天室中老师的头像
	CControlUI* m_pBackground;
	CButtonUI * m_pMinSysBtn, * m_pMaxSysBtn, * m_pRestoreSysBtn, * m_pCloseSysBtn;
	CButtonUI * m_pFontBtn, * m_pFaceBtn, * m_pImageBtn, * m_pScreenshotsBtn, * m_pShakeBtn;
	CButtonUI * m_pPinZhongBtn, * m_pWenZhangBtn, * m_pHisRecordBtn;	//IM2.0
	CButtonUI * m_pRight_search, * m_pRight_close, * m_pRight_rili, * m_pRight_begin, * m_pRight_up, * m_pRight_down, * m_pRight_end, * m_pRight_searchok; //IM2.0
	CComboUI * m_pFontNameCombo, * m_pFontSizeCombo;
	COptionUI * m_pBoldBtn, * m_pItalicBtn, * m_pUnderLineBtn;
	CButtonUI * m_pColorBtn;
	CButtonUI * m_pCloseBtn, * m_pSendBtn;
	CRichEditUI* m_pSendEdit, * m_pRecvEdit;

	// wuchao add at 2016-04-08 表扬投诉分析师和收藏分析师按钮 
	CButtonUI * m_pBYTSBtn;     //  表扬投诉分析师
	CButtonUI * m_pShouCangBtn; //  收藏分析师

	//right_part
	CTextUI * m_pRoom_note, * m_pRoom_member;
	CRichEditUI* m_pRoomNoteEdit;
	CGroupsUI* pGroupsList;

	// IM2.0
	CComboUI * m_pSearchTimeCombo;
	CTabLayoutUI* m_pControlSwitch;
	CRichEditUI* m_pHisRecordEdit;
	CEditUI * m_pSearchContent;
	CWebBrowserUI* m_pActiveX_Web;
	CCustomWebEventHandler *pWebHandle;

	CLabelUI *m_pDateTimeLabel ; 

	// 日历控件 
	CCalendarDlg m_pCalendaryDlg ;

	CFaceSelDlg m_FaceSelDlg;
	CFaceList m_FaceList;
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

	EDIALOGTYPE eDialogType;
	CDuiString bgimage_;
	DWORD bkcolor_;
	FriendListItemInfo myselft_;
	FriendListItemInfo friend_;
	SELECTNODE m_selectNode;
	CDuiString m_PublicSveID;		//公众号中跟我会话的服务人员ID
	int nRouteID;					//路由ID
	CDuiString m_pIncodeID;
	int nPublicID;					//公众号ID
	time_t	tmStart_;				//开始计时(difftime算时间间隔)
	BOOL	bsendPubconClose;		//发送断开消息
	int nRightSwitchType;			// 右侧边栏类型计数

	map<CDuiString,bool> m_MenuCheckInfo;	//保存菜单的设置信息,应该为每个菜单都声明一个map,我这里只做了循环方式菜单的功能
	map<CDuiString,bool> m_MenuLoopCheckInfo;
	CArray<IMAGE_INFO*,IMAGE_INFO*> m_arrII;
	BOOL m_bRefreshing;
	
	//////////////////////////////////////////////////////////////////////////
	//投顾端分析师处理
public:
	void OnUserStateChange(WPARAM wParam, LPARAM lParam);	// 用户状态改变消息
	void UISetInitSize();									// 外部父窗口大小改变时，调用此方法自适应大小
	void ChangeChatRoom(DWORD rid);							// 改变聊天室
	void ChangeLaoshi(int nindex);							// 切换分析师
	void ShowFXS(BOOL bShow = TRUE, int bChange = 0, int bNoOneOnline = 0);						// 显示分析师
	BOOL ShowPic(int nType, int nValue = 1);				// 显示图片
	BOOL LoginSuccess();									// for test
protected:
	void GetAllFXSInfo(DWORD rid);							// 获取某聊天室的所有分析师信息
	void ShowRecord();										// 获取聊天记录显示
	void GetAllFXSState();									// 获取某聊天室的所有分析师在线状态
	BOOL Parser(CString strJson, vector <AnalystInfo> &m_vtAnalystInfo);	//初始化JOSN字段
	void InsertChatHistory_Formdb(CRichEditUI* m_pEdit, CHAT_RECORD2 *pRec, LPCTSTR szSendName, LPCTSTR szSendId);
	BOOL DownLoadAllFXSPic();								// 分析师头像下载
	BOOL DownLoadPic(CString strUrl, CString strFile);		// 分析师头像下载
	void SetUIInitSize();
public:
	DWORD m_roomid;
	DWORD m_analystid;
	vector <AnalystInfo> m_vtInfo;
	map <DWORD, DWORD> m_mapRoomAnalys;		// 保存房间中选择过的投顾
	std::vector<RECEIVED_MSG> *pMsg;		// 投顾的客户端特殊处理,消息集合
	//CLock m_csLock;							// pMsg锁
	int nAnalystCtrIndex;
	CString strCurrentID;
	CString strCurrentName;
	BOOL bControlImg;
	BOOL bControlImg2;
	BOOL bselectFXS;
	BOOL bFristReturn;

	// wuchao add 
	// 聊天记录显示的起始和结束位置
	// 标识上一页下一页的起始位置 
	int m_nBeginDisplay ; 
	int m_nEndDisplay ; 


};

#endif // CHARTDIALOG_HPP