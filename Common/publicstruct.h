#ifndef PUBLICSTRUCT_H
#define PUBLICSTRUCT_H
//UPIM/SERVER用到的共用STRUCT
//单字节对齐
#include "../Common/define.h"
#include "../Common/protocol.h"

#pragma pack(push,1)

//////////////////////////////////////////////////////////////////////////
// 服务器使用的群组
// UPIMID
typedef struct tagSPUPIMID
{
	char	m_szUID[USERID_SIZE];
}SPUPIMID,*LPSPUPIMID;

typedef struct tagVUPID
{
	char	m_szIMID[USERID_SIZE];
	BYTE	m_nUserType;
}VUPID,*LPVUPID;

struct STATE_REFLASH_
{
	int			nCount;
	SPUPIMID	mIDVT[REFRESH_USER_COUNT];
};

// 群组
struct GROUP_CHAT_ROOM
{
	ROOMID	m_nRoomId;					// 群组id
	int		m_nType;					// 群组类型 E_CHATROOM_TYPE
	char	m_szMasterID[USERID_SIZE];	// 管理员id
	char	m_szRoomName[ROOMNAME_SIZE];// 群组名称
	char	m_szNote[NOTE_SIZE];		// 群组公告
	bool	m_bDelFlag;					// 删除标志
	bool	m_bUserFlag;				// 用户加载标记
	long	m_nBusy;					// 使用状态

	vector<VUPID> m_vtUser;				// 当前群组用户
	CRITICAL_SECTION m_csRoom;
};

// 群组基本信息
struct GROUP_CHAT_BASE
{
	ROOMID	m_nRoomId;					// 群组id
	int		m_nType;					// 群组类型 E_CHATROOM_TYPE
	char	m_szMasterID[USERID_SIZE];	// 管理员id
	char	m_szRoomName[ROOMNAME_SIZE];// 群组名称
	char	m_szNote[NOTE_SIZE];		// 群组公告
	bool	m_bDelFlag;
};

//////////////////////////////////////////////////////////////////////////
// 客户端使用的群组
// 群聊的相关信息
// 群组用户信息
struct GUSER_INFO
{
	char	m_szIMID[USERID_SIZE];		// IMIDID
	short	m_nUserType;				// 用户类型
	char	nickname[S_NICKNAME_LEN];	// 名称
	bool	bload;						// 用户是否在列表中
	BYTE	iconindex;					// 图标
	short	userstate;					// 用户在线状态
	bool	brefresh;					// 用户信息是否刷新
};

struct GROUP_CHAT_INFO
{							
	ROOMID	m_nRoomId;					// 聊天室id
	int		m_nRoomType;				// 聊天室类型，1：固定聊天室；2：临时聊天室
	char	m_szMasterID[USERID_SIZE];	// 聊天室管理员id
	char	m_szRoomName[ROOMNAME_SIZE];// 聊天室名字
	char	m_szNote[NOTE_SIZE];		// 公告
	bool	m_bDelFlag;					// 删除标志
	bool	m_bLoadFlag;				// 用户加载标记
	vector<GUSER_INFO>* pVtUser;		// 用户集合
};

typedef std::vector<GUSER_INFO> VECTOR_GUSER_INFO;

// 接收缓冲，避免分包的时候
typedef struct tagPKGRECVINFO
{
	BOOL	bNeedRecvMore;		// 需要再次接收
	int		nRecvLen;			// 接收长度
	int		nNeedRecvLen;		// 需要接收长度
	BYTE	m_btInfo[16*1024];	// 接收缓冲区16K
}PKGRECVINFO,*LPPKGRECVINFO;

typedef std::map<LONG, LPPKGRECVINFO>::iterator PKGRECV_ITER;

#pragma pack(pop,1)
#endif