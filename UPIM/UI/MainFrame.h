#ifndef MAINFRAME_HPP
#define MAINFRAME_HPP

#include "SkinChangeEvent.h"
#include "UIFriends.h"
#include "UIGroups.h"
#include "GeneralDialog.h"
#include "UserMrgDialog.h"
#include <GdiplusHeaders.h>
#include "MsgNotifyDialog.h"
#include <CommCtrl.h> 

#pragma   comment(lib,   "comctl32.lib")



class MainFrame : public WindowImplBase
{
public:
	MainFrame();
	~MainFrame();

public:
	typedef enum _TrayIconMessage  // 鼠标在托盘图标上面的不同状态 
	{
		WM_MOUSE_ICON_ENTRY = 0x00F0 ,  // 进入 
		WM_MOUSE_ICON_HOVER ,           // 悬停 
		WM_MOUSE_ICON_LEAVE,             // 离开 
	}TrayIconMessage;

public:
	LPCTSTR GetWindowClassName() const;	
	virtual void OnFinalMessage(HWND hWnd);
	virtual void InitWindow();
	virtual LRESULT ResponseDefaultKeyEvent(WPARAM wParam);
	virtual CDuiString GetSkinFile();
	virtual CDuiString GetSkinFolder();
	virtual UILIB_RESOURCETYPE GetResourceType() const;
	virtual CControlUI* CreateControl(LPCTSTR pstrClass);
	virtual LRESULT OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
	virtual LRESULT HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual LRESULT OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);  
	virtual LPCTSTR GetResourceID() const;
	LRESULT MessageNotify(TrayIconMessage uMsg) ;  // 处理鼠标在托盘上面的事件
	LRESULT OnRButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam);
	void NotifyMessage(DWORD dwMessage, HICON hIcon, LPCSTR szTips);
    void SetBkColor(DWORD dwBackColor);
	void SetBkInfo();
	void SetNotifyIcon();
	void SetNotifyIcon( EUSERSTATE state ) ;  // 根据不同的状态  设置不同的图标  
	void StartLogin();
	void CreatePublicUserNode(int nPID, LPCSTR szUID);	// 服务人员收到公众号消息的时候，临时创建节点
	void CreateTgRoomUserNode(int nRID, LPCSTR szUID);	// 分析师收到客户互动的时候，临时创建节点
	void OnNotifyIcon(WPARAM wParam , LPARAM lParam);
	void OnMenuClick(WPARAM wParam , LPARAM lParam);
	CDuiString	GetMainBkImage();
	DWORD GetMainBkColor();
	BOOL GetIteratorOfID(FriendList_ITER &friend_iterator, CDuiString strID, EVTTYPE eType, BOOL id = TRUE);
	BOOL SetIteratorState(CDuiString strID, EVTTYPE eType, int nState, BOOL id = TRUE);
	void OnExit();
	void CheckConnectStatus();
	int GetClientOnlineCount() ;
	std::vector<USER_ROOMCLIENT_INFO>* GetOnlineClientInfo() ; 
	void DeleteFriend(CDuiString strName) ; 
	void DeleGroup(CString strGroupName ,CString strGroupID) ;

protected:	
	void FriendNodeClick(TNotifyUI& msg);	// 好友列表双击事件
	void GroupNodeClick(TNotifyUI& msg);	// 群组/自定义分组双击事件
	void RecentNodeClick(TNotifyUI& msg);	// 最近联系人分组双击事件
	void FindFriendNodeClick(TNotifyUI& msg ) ;  // wuchao add at 2016-04-06 
	void Notify(TNotifyUI& msg);
	void OnPrepare(TNotifyUI& msg);
	void OnTimer(TNotifyUI& msg);
	void OnUserListRClick(TNotifyUI& msg);
	void OnMainMenuClick(TNotifyUI& msg);
	void OnListItemClick(TNotifyUI& msg);
	void OnListItemActivate(TNotifyUI& msg);
	void OnStateMenuClick(TNotifyUI& msg);
	//////////////////////////////////////////////////////////////////////////
	// btn响应
	void OnBtn_Min(TNotifyUI& msg);					// 最小化按钮
	void OnBtn_Max(TNotifyUI& msg);					// 最大化按钮
	void OnBtn_Restore(TNotifyUI& msg);				// 还原按钮
	void OnBtn_Close(TNotifyUI& msg);				// 关闭按钮
	void OnBtn_ChangeBkSkin(TNotifyUI& msg);		// 更换背景按钮
	void OnBtn_UserMrg(TNotifyUI& msg);				// 好友管理器
	void OnBtn_SearchFriend(TNotifyUI &msg) ;       // 搜索好友 
	// menu响应
	void OnMenu_FriendOpen();
	void OnMenu_FriendInfo();
	void OnMenu_FriendHistroy();
	void OnMenu_CreateGroup();
	void OnMenu_UserOnline();						// 用户上线
	void OnMenu_UserOffline();						// 用户离线
	void OnMenu_UserLiKai();						// 用户离开
	void OnMenu_GroupOpen();
	void OnMenu_GroupQuit();
	void OnMenu_CreateSubGroup();					// 创建分组
	void OnMenu_ModifySubGroup();					// 修改分组
	void OnMenu_DeleteSubGroup();					// 删除分组
	       
	// 好友来消息时，鼠标移动到托盘图标时，显示来消息好友的信息 类似QQ功能 
	void OnShowNotifyDialog() ; 
	void OnCloseNotifyDialog() ;

	// 客户上线  交换两个节点的位置 
	void ExchangeNode(CFriendsUI* friendList ,Node* firstNode ,Node* secondNode) ;
	int FindInsertNode(std::vector<USER_ROOMCLIENT_INFO> clientList,Node *pInsertNode) ; // 在好友列表中找到满足条件的位置 
public:
	void UpdateFriendsList();
	void UpdateGroupsList();
	void UpdateRecentList();
	void CreateMsgNotifyDlg();
	void KillMyTimer() ; 
	void AddMyTimer() ; 
	// im2.0 当分析师好友登录时，更新好友列表  分组  最近联系人  
	void UpdateFriendsListInFxs(CDuiString strID, int nstate);
	void UpdateGroupsListInFxs(Node* pGroupNode ,CDuiString strID, int nstate);
	void UpdateRecentListInFxs(CDuiString strID, int nstate);
	void ReloadFriendList(Node *pParendNode) ; 
	void InsertFriendsIntoVector(FriendListItemInfo* m_unit);
	void UpdateMyselfInfo(FriendListItemInfo* m_unit, CDuiString AppTitle);
	void AddStdGroupNode();
	void CreateGroupNode();
	void CreatePublicNode();
	void AddPublicAccNode();
	void CreateTgRoomNode();
	void CreateTgRoomNode2();
	void CreateRoomClientNode();
	void CreateRoomAnalysNode();
	void AddTgRoomAccNode();
	void CreateDeptNode();
	void CreateFriendNode();
	void CreateRoomNode();
	void CreateTgHisNode();
	void CreatSelfGroup();
	void AddSelfGroupClientNode(int nGroupID, Node* nodeptr = NULL);
	void AddSelfGroupClientNode( int nGroupID ,CString strClientID) ; 
	void DelSelfGroupClientNode( int nGroupID ,CString strClientID) ; 
	void CreateTgRoomHisNode();
	void ModifyDisRecentInfo(CDuiString m_strUserID, CDuiString m_strUserName, CDuiString m_LastWord, int n_Lastwordtime, int nRoomID);
	void CreateRecentNode(CDuiString m_strNodeID, CDuiString m_strUserName, CDuiString m_LastWord, long n_Lastwordtime);
	void AddTgRoomClientNode(long nRoomID, Node* nodeptr = NULL);
	BOOL OpenChatDiolog(CDuiString m_strUserID, FriendList_ITER iterator);
	BOOL OpenGroupChatDiolog(CDuiString m_strRoomID, FriendList_ITER iterator);
	//BOOL OpenChatDialog() ;

	// 将jpg转化成为HICON
	void ConvertImageToHicon(CString strFileName) ; 

	void SetAppTitle(CDuiString m_strApptitle);
	void SetSelfInfo();
	void ModifySign(CDuiString m_strSign);
	
	BOOL ProcessRecvMsg(LPRECVMSG2 lpRecvMsg, BOOL bOffMsg = FALSE);			// 收到消息后进行处理
	BOOL ProcessRecvMsg(LPNEWRECVMSGPUBLIC lpRecvMsg);			// 收到消息后进行处理
	BOOL ProcessRecvGroupMsg(GROUP_CHAT_HIS_MSG* lpHisMsg);						// 收到历史群组消息后进行处理
	BOOL ProcessRecvGroupMsg(gcrecvmsg_ans* lpRecvMsg, BOOL bOffMsg = FALSE);	// 收到群组消息后进行处理
	BOOL GCGetList(ROOMID mRoomID);
	BOOL GetPublicRoute(PUBLICID mPublicID);
	void DeleteRoomMember(ROOMID mRoomID, LPCTSTR szID, short nUserType);
	void SetControlVisable();
	BOOL SetMainFace(EUSERSTATE nstate, LPCSTR szFile = "");							//设置自己头像
	BOOL SetMainFace2(EUSERSTATE nstate, LPCSTR szFile1 = "", LPCSTR szFile2 = "");		//设置自己头像  
	BOOL SetAnalysNodeHead(EUSERSTATE nstate, int nRoomID, int nAnalysID, LPCSTR szFile1 = "", LPCSTR szFile2 = "");	//设置好友头像 客户版本  设置分析师的头像 
	BOOL SetClientNodeHead(EUSERSTATE nstate, CString strClientID, LPCSTR szFile1 = "", LPCSTR szFile2 = "") ;//设置好友头像 分析师界面  设置客户的头像
	BOOL SetGroupClientNodeHead(EUSERSTATE nstate, USER_ROOMCLIENT_INFO userClientInfo, LPCSTR szFile1, LPCSTR szFile2 ) ; // 设置分组里面好友的头像 
	BOOL SetAnalysNodeNickName(int nRoomID ,int onlineNums = 0 ,int totalNums = 0) ; 
	BOOL SetGroupNodeNickName( int nGroupID ,int onlineNums = 0 , int totalNums = 0 ) ; 
	void ReleaseUserData();
	void FlashAllItem();
	void StopFlashItem(CDuiString strID, EPANELTYPE e_pType);
	void InsertIntoFlashVt(CDuiString strID, EPANELTYPE e_pType);
	BOOL SendStateNotify(int nstate);
	void UserStateChange(CDuiString strID, int nstate); // 分析师登录 客户上线提醒 
	void AnalystStateChange(CDuiString strID, int nstate) ; // 客户登录 分析师上线提醒 
	void AddTimer(UINT nTimerID, UINT uElapse);
	void ImSvrOff();											// 服务器关闭
	void UserKickOff();											// 异地登录
	CString GetNotifyiconToolTip();
	void NotifyiconFlash();
	void StopNotifyiconFlash();
	void SetNotifyiconFlash();
	void GetClientRights(CString strUpID);
	CString GetUserPicURL(CString strUpID);
	BOOL DownLoadPic(CString strUrl, CString strFile);
	void OpenRightsDlg();
	void CloseWhoSendMsg() ; // 清空消息发送方 
	void GetAllFXSState() ; 

	void SwitchTable() ; 
	void ClearFindFriendList() ; 

	LRESULT OnUIMessageLoop(WPARAM wParam, LPARAM lParam);		// UI_MessageLoop
	LRESULT OnKeyUp(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled); 
	LRESULT OnEnterUp() ;
	LRESULT OnUserColse(WPARAM wParam, LPARAM lParam);			// 关闭	
	LRESULT OnUserLoginIN(WPARAM wParam, LPARAM lParam);		// 登录
	LRESULT OnUserOffLine(WPARAM wParam, LPARAM lParam);		// 离线
	LRESULT OnUserOnLine(WPARAM wParam, LPARAM lParam);			// 上线
	LRESULT OnUserADDGroup(WPARAM, LPARAM);
	LRESULT OnSetSelfInfo(WPARAM, LPARAM);
	LRESULT OnAddFriendInfo(WPARAM, LPARAM);
	LRESULT OnUserADDPublic(WPARAM, LPARAM);
	LRESULT OnUserADDTgRoom(WPARAM, LPARAM);
	LRESULT OnUserADDTgRoom2(WPARAM, LPARAM);
	LRESULT OnADDRoomClient(WPARAM, LPARAM);
	LRESULT OnADDRoomAnalys(WPARAM, LPARAM);
	LRESULT OnUserStateChange(WPARAM wParam, LPARAM lParam);
	LRESULT OnOpenWND(WPARAM wParam, LPARAM lParam);
	LRESULT OnSoundPlay(WPARAM wParam, LPARAM lParam);
	LRESULT OnAddRoom(WPARAM wParam, LPARAM lParam);
	LRESULT OnLeaveRoom(WPARAM wParam, LPARAM lParam);
	LRESULT OnGetUserPic(WPARAM wParam, LPARAM lParam);
	LRESULT OnCreatSelfGroup(WPARAM wParam, LPARAM lParam);
	LRESULT OnGetSelfPic(WPARAM wParam, LPARAM lParam);
	LRESULT OnOpenRight(WPARAM wParam, LPARAM lParam);
	LRESULT OnLoadRecentClientNode(WPARAM wParam, LPARAM lParam) ;  // 加载最近联系人 
	LRESULT OnOpenChatDialog(WPARAM wParam, LPARAM lParam) ; 
	LRESULT OnFindFriends(WPARAM wParam, LPARAM lParam) ;    // wuchao add at 2016-04-05 查找好友 
	LRESULT OnSetFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled); 
	LRESULT OnAddNewPublic(WPARAM wParam, LPARAM lParam);
	LRESULT OnAddNewLastUser(WPARAM wParam, LPARAM lParam);
public:
	SkinChangedObserver					skin_changed_observer_;
	CGeneralDialog*						pLoginDialog;

	BOOL								bLoginDialogClosed;
	map<CDuiString,bool> m_MenuCheckInfo;	//保存菜单的设置信息,应该为每个菜单都声明一个map,我这里只做了循环方式菜单的功能
	map<CDuiString,bool> m_MenuLoopCheckInfo;
	 

	bool        m_isEnterLeave ; 
private:
	bool        m_bIsMsgDlgShow ;   // 是否显示消息提示框 
 	bool        m_bMouseEnter ;     // 标记鼠标是否进入托盘图标  
	bool		m_bIsIcon;

	int         m_nSwitchTab ;      // 记录Tab切换的页面 

	EUSERSTATE  m_eUserState ;        // 记录当前启动用户的状态
	int			nDeirction;
	int			bk_image_index_;
	void *		m_pWave[4];	//装载了wave资源,按顺序分别是系统消息,收到信息,用户上线
	bool								m_bSetSelfInfo;
	FriendListItemInfo					myself_info_;
	std::vector<FriendListItemInfo>     m_vecWhoSendMsg ; // wuchao add 记录消息发送方  
	vector <AnalystInfo> m_vtInfo;      // 客户的分析师列表  里面存分析师的名字 


	CDuiString							m_AppTitle;
	std::vector<FriendListItemInfo>		friends_;
	std::vector<FriendListItemInfo>		Recentfriends_;
	std::vector<FriendListItemInfo>		groups_;
	CMsgNotifyDialog m_NotifyDlg ;      // 消息提示框   
	BOOL                                m_bRecvFriendsMsg ; // wuchao add 是否是收到了好友发送的消息，标志是显示MainFrame还是ChatDialog
	HICON								m_hIcon ;           // 在线  图标 
	HICON                               m_hOffLineIcon ;    // 离线  图标 
	// 用于用户头像jgp与HICON之间的转换
	Gdiplus::Bitmap   *m_hUserBitMap;

	// 托盘中  新来消息时  好友图标闪动 
	HICON m_hUesrIcon;
	NOTIFYICONDATA						m_noti;        // 托盘  
	SELECTNODE							m_MainSelectNode;
	CCriticalSection					m_csNodeFlash;
	CCriticalSection					m_csNodeFlash2;
	CCriticalSection					m_csNodeFlash3;
	CCriticalSection					m_csStateInfo;
private:
	CButtonUI * m_pMinSysBtn, * m_pMaxSysBtn, * m_pRestoreSysBtn, * m_pCloseSysBtn ,*m_pSearchBtn ,*m_pSearchCloseBtn;
	CTabLayoutUI* m_pControlSwitch;  // wuchao add at 2016-04-06  
	CRichEditUI* m_pSearch_edit ; 
	CRichEditUI* m_pSearch_edit_friend ; 

	CFriendsUI* m_pFriendsList;   // 好友列表 
	CFriendsUI* m_pSelfGroup;     // 自定义分组 
	CFriendsUI* m_pFindFriendList ;  //wuchao add at 2016-04-06 搜索好友 
	CGroupsUI* m_pGroupsList;     
	CGroupsUI* m_pRecentLinkList; // 最近联系人  
	CTabLayoutUI* m_pTabControl;
	CControlUI* m_pBackground;
	CRect     m_rtMsgDlg ; 
	//TrayIconMessage trayIconMessage ; 
	POINT m_ptMouseHover ;  //鼠标悬停的位置

public:
	// 客户管理窗口 
	CUserMrgDialog *m_pUserMrgDialog  ; 
};



#endif // MAINFRAME_HPP

// 从web上面获取客户的头像 
UINT DownUserPicFromWeb(PVOID pData ) ; 
// 从web上面获取分析师的头像 
UINT DownAnalysPicFromWeb(PVOID pData) ; 