#ifndef GLOBALVAR_H
#define GLOBALVAR_H

#include "IMExport.h"
#include "../Common/define.h"
#include "MyClient.h"
#include "UI/MainFrame.h"
#include "UI/ChatDialog.h"
#include "sqlite/SqliteOp.h"
#include "MyThread.h"

class CMyClient;
extern CMyClient g_MyClient;	// 连接封装类，发送消息用
extern MainFrame* g_pFrame;		// 主窗口指针 
extern CSqliteOp g_SqliteOp;	// sqlite操作对象
extern CCharConv g_CharConv;	// 字符转换操作对象
extern ChatDialog* g_TGChatDialog;
extern CGeneralDialog* g_pCreateTG1;
extern CGeneralDialog* g_pCreateTG2;
extern CWinThread* g_pCreateThread;
extern BOOL g_UIMessageLoopStart;

extern HINSTANCE g_hInstance;	// 编译成DLL，资源需记录下来
extern long		g_nUserType;	// 用户类型 
extern long		g_bIndepend;	// 是否独立版本
extern HWND		g_hVessel;		// DLL调用模式的父窗口指针
extern UINT		g_hFeedBackMsg;	// DLL调用传出的，给父窗口处理的消息
extern long		g_nHQ;			// 是否和行情集成

extern long		g_nLoginMode;	// 登录模式
extern BOOL		g_bLogining;	// 是否正在登录
extern BOOL		g_bLogined;		// 登录是否成功
extern long		g_nLoginSucNum;	// 登录成功次数
extern BOOL		g_bExiting;		// 是否正在退出
extern BOOL		g_bUserKickOff;	// 是否被踢掉

extern long		g_nMsgSeq;		// 全局消息序列标记
extern std::map<CDuiString, LPVOID>		allopenwnd;
extern std::map<CDuiString, CDuiString>	alluserNickname;
extern std::map<CDuiString, int>		allUserState;
typedef map<CDuiString, int>::iterator UST_ITER;

extern CLIENTCONFIG	g_config;
extern USERCONFIG	g_userconfig;
extern char			g_loginname[51];
extern char         g_khID[51] ;   // wuchao add at 20160425 客户登录时的客户ID  IM集成到UP中 UP传过来的参数 
extern int			g_nMarket;
extern char			g_analysheadurl[256];
// 在线状态处理
extern std::vector<STATE_INFO>	m_vtStateInfo;
typedef vector<STATE_INFO>::iterator StateInfo_ITER;
// flashnode
extern std::vector<Node*>	m_vtFlashNodeInfo;
extern std::vector<Node*>	m_vtFlashNodeInfo2;
typedef vector<Node*>::iterator FlashNode_ITER;
// department
extern std::vector<USER_GROUP_INFO>	m_vtDeptInfo;
extern int		m_nDeptIdx;		
// group
extern std::vector<USER_GROUP_INFO>	m_vtUserGroup;
extern int		m_nGroupIdx;
// friend
extern std::vector<FRIEND_INFO>	m_vtFriendInfo;
extern int		m_nFriendIdx;
// roomclient
extern std::vector<USER_ROOMCLIENT_INFO>	m_vtRoomClient;
extern int		m_noomClientIdx;

////////////////////////////////////////////////////////////////
// wuchao add at 2016-03-29
// 分析师中 当前在线人员的详细信息和离线人员
////////////////////////////////////////////////////////////////
extern std::vector<USER_ROOMCLIENT_INFO>	m_vtOnlineClient;
extern std::vector<USER_ROOMCLIENT_INFO>	m_vtOfflineClient;

// room
extern std::map<ROOMID, GROUP_CHAT_INFO>	m_mapChatRoom;
typedef map<ROOMID, GROUP_CHAT_INFO>::iterator GCR_ITER;
// rights
extern std::map<int, int>	m_mapCilentRights;
extern int		m_nCilentRightsIdx;

// public
// 对于客户来说，存储的是他可见的公众号
// 对于服务人员来说，存储的是他提供服务的公众号
extern std::map<PUBLICID, PUBLIC_INFO>		m_mapPublicInfo;
typedef map<PUBLICID, PUBLIC_INFO>::iterator PUB_ITER;

// Analyst
// 对于分析师来说，存储的是他提供服务的房间号
extern std::map<TGROOMID, TGROOM_INFO>		m_mapTgRoomInfo;
typedef map<TGROOMID, TGROOM_INFO>::iterator TR_ITER;

//im2.0中，客户取得的所有分析师信息
extern std::map<TGROOMID, ALLCLIENT_ROOM_INFO>		m_mapAllClientRoom;
typedef map<TGROOMID, ALLCLIENT_ROOM_INFO>::iterator ACRI_ITER;
extern ANALYSTGROUP_INFO m_groupInfoStc[USERMRG_FENZU+1];
extern std::map<int, CString>	m_mapSelfGroups;

extern Gdiplus::GdiplusStartupInput g_gdiplusStartupInput;
extern ULONG_PTR	g_gdiplusToken;
extern HMODULE		g_hRichEditDll;
extern FriendListItemInfo g_selfunit;

extern char g_szOpenUID[S_USERID_LEN]; //打开窗口
extern CCriSec g_fileLock;
extern CFile  g_fileUserLog;

extern char g_chBuffer[MAX_BUF_SEND_LEN];
extern DWORD g_dwRoomID;

extern PKGRECVINFO g_strpkgrecvinfo;

extern PKGRECVINFO g_pack_info ; 
extern int iBufferIndex ; 
extern int		g_nRoomID_im2_0;	// 假设单个分析师只有一个房间的权限 
extern CString g_strAddGroupName;   // 增加的分组 
extern CString g_strDelGroupName ;  // 要删除的分组 
extern BOOL g_downPicLock  ;        // 记录是否下载图片  线程锁 

#endif