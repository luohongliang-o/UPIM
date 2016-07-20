#include "StdAfx.h"
#include "globalvar.h"

CMyClient	g_MyClient;
CSqliteOp	g_SqliteOp;
CCharConv	g_CharConv;

MainFrame*	g_pFrame = NULL;
long		g_nMsgSeq = 0;
std::map<CDuiString, LPVOID>	allopenwnd;
std::map<CDuiString, CDuiString>	alluserNickname;
std::map<CDuiString, int>	allUserState;
HINSTANCE g_hInstance;
ChatDialog* g_TGChatDialog = NULL;
CGeneralDialog* g_pCreateTG1 = NULL;
CGeneralDialog* g_pCreateTG2 = NULL;
CWinThread* g_pCreateThread = NULL;
BOOL g_bUserKickOff = FALSE;
BOOL g_UIMessageLoopStart = FALSE;

long g_nUserType = 0;
long g_bIndepend = 0;
HWND g_hVessel = NULL;
UINT g_hFeedBackMsg = 0;
long g_nHQ = FALSE;

long g_nLoginMode = eIMLogin_Default;
BOOL g_bLogining = FALSE;
BOOL g_bLogined = FALSE;
long g_nLoginSucNum = 0;
BOOL g_bExiting = FALSE;

CLIENTCONFIG	g_config = {0};
USERCONFIG		g_userconfig = {0};
char			g_loginname[51];
char            g_khID[51] ; 
int				g_nMarket = 0;
char			g_analysheadurl[256];
// 在线状态处理
std::vector<STATE_INFO>	m_vtStateInfo;
// flashnode
std::vector<Node*>	m_vtFlashNodeInfo;
std::vector<Node*>	m_vtFlashNodeInfo2;
std::vector<Node*>	m_vtFlashNodeInfo3;
// department
std::vector<USER_GROUP_INFO>	m_vtDeptInfo;
int		m_nDeptIdx = 0;		
// group
std::vector<USER_GROUP_INFO>	m_vtUserGroup;
int		m_nGroupIdx = 0;
// friend
std::vector<FRIEND_INFO>	m_vtFriendInfo;
int		m_nFriendIdx = 0;
// roomclient
std::vector<USER_ROOMCLIENT_INFO>	m_vtRoomClient;
int		m_noomClientIdx = 0;


std::map<CString,NEWPUBLIC_INFO>	g_mapNewPublicInfo;
////////////////////////////////////////////////////////////////
// wuchao add at 2016-03-29
// 分析师中 当前在线人员的详细信息和离线人员
////////////////////////////////////////////////////////////////
std::vector<USER_ROOMCLIENT_INFO>	m_vtOnlineClient ;
std::vector<USER_ROOMCLIENT_INFO>	m_vtOfflineClient;

// room
std::map<ROOMID, GROUP_CHAT_INFO>	m_mapChatRoom;
// public
std::map<PUBLICID, PUBLIC_INFO>		m_mapPublicInfo;
// tgroom
std::map<TGROOMID, TGROOM_INFO>		m_mapTgRoomInfo;
//im2.0 AllRoom
std::map<TGROOMID, ALLCLIENT_ROOM_INFO>	m_mapAllClientRoom;
// rights
std::map<int, int>	m_mapCilentRights;
int m_nCilentRightsIdx = 0;
std::map<int, CString>	m_mapSelfGroups;

ANALYSTGROUP_INFO m_groupInfoStc[USERMRG_FENZU+1] = {0};
Gdiplus::GdiplusStartupInput g_gdiplusStartupInput;
ULONG_PTR	g_gdiplusToken;
HMODULE		g_hRichEditDll;
FriendListItemInfo g_selfunit;

char g_szOpenUID[S_USERID_LEN] = {0};
CCriSec			g_fileLock;
CFile           g_fileUserLog;

char		g_chBuffer[MAX_BUF_SEND_LEN];
DWORD g_dwRoomID = 0;
PKGRECVINFO g_strpkgrecvinfo = {0};

// wuchao add at 20160421
PKGRECVINFO g_pack_info = {0} ; 
int iBufferIndex = 0 ; 
int	g_nRoomID_im2_0 = 0;
CString g_strAddGroupName = _T("");
CString g_strDelGroupName = _T("") ;
BOOL  g_downPicLock = FALSE ; 

char g_jtcode[2] = "";
char g_regcampaignid[10] = "";
char g_usernc[S_NICKNAME_LEN]="";
char g_userHeadUrl[256] = "";
char g_userHeadIMG[256] = "";
char g_strToken[128] = "";