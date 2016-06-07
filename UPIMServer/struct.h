#ifndef STRUCT_H
#define STRUCT_H

//UPIMSERVER用到的STRUCT
#include "../Common/define.h"
//////////////////////////////////////////////////////////////////////////
#pragma pack(push,1)
//单字节对齐
//////////////////////////////////////////////////////////////////////////
// 互斥区
class CLock
{
public:
	CLock()	{ InitializeCriticalSection(&m_cs); }
	~CLock() { DeleteCriticalSection(&m_cs); };
	VOID Lock() { EnterCriticalSection(&m_cs); };
	VOID Unlock() { LeaveCriticalSection(&m_cs); };
private:
	CRITICAL_SECTION m_cs;
};

//////////////////////////////////////////////////////////////////////////
// 自动锁
class CAutoLock
{
public:
	CAutoLock(CLock* pLock){m_pLock = pLock;pLock->Lock();}
	~CAutoLock() {m_pLock->Unlock();}
protected:
	CLock* m_pLock;
};

struct UserNode 
{
	DWORD	m_dwSessionID;			//sessionID
	CHAR	m_szUID[UID_SIZE];		//用户ID		
	DWORD	m_dwUserStatus;			//UserStatus	
	CHAR	m_szInternetIp[24];		//客户端公网IP	
	DWORD	m_Port;					//客户端Port	

	void Copy(const UserNode& other) 
	{
		m_dwSessionID = other.m_dwSessionID;
		COPYSTRARRAY(m_szUID,other.m_szUID);
		m_dwUserStatus = other.m_dwUserStatus;
		COPYSTRARRAY(m_szInternetIp,other.m_szInternetIp);
		m_Port = other.m_Port;		
	}

	BOOL operator ==(const UserNode& other) 
	{
		if(m_dwSessionID == other.m_dwSessionID &&
			0 == strcmp(m_szUID,other.m_szUID) &&
			0 == strcmp(m_szInternetIp,other.m_szInternetIp) &&
			m_Port == other.m_Port &&
			m_dwUserStatus == other.m_dwUserStatus )    
			return TRUE;
		else
			return FALSE;
	}
};

// 连接信息
class CADODatabase;
typedef struct tagOPLINK
{
	WORD	wServerIndex;				// 后台序号
	DWORD	dwBranchID;					// 营业部代码
	BOOL	bBusy;						// 是否使用中
	short	nSel;						// 选择哪个数据库
	BOOL	bConnected;					// 是否连接
	long	nBusyTime;					// 开始忙时间点
	CADODatabase* pAdodb;
} OPLINK, *LPOPLINK;

// 数据库配置
typedef struct tagDBCONFIG
{
	CHAR	szDataSource[32];			// 数据源
	CHAR	szDataBase[32];				// 数据库
	CHAR	szUserName[32];				// 数据库用户
	CHAR	szPassword[32];				// 数据库密码
}DBCONFIG,*LPDBCONFIG;

// User状态
struct USERSTATE
{
	char	m_szToUser[USER_SIZE];		// 接收者ID集合		
	char	m_szUSERID[USERID_SIZE];	// id
	BYTE	m_ucState;					// id的状态
};
//白名单
struct IPInfo
{
	int    id;
	CHAR   IP[40];
};
//ip
struct IPWHITELIST
{
	int     num;
	IPInfo  ipinfo[MAX_IPWHITELIST];
};
// 服务器配置
struct SVRCONFIG
{
	DWORD	dwSVRID;					// 服务器ID
	CHAR	szSVRIP[MAX_PATH];			// 侦听地址
	DWORD	dwSVRPORT;					// 侦听端口

	DWORD	dwVersion;					// 调用代码版本
	DWORD	dwMaxAnsBufLen;				// 最大应答缓冲区长度
	DWORD	dwMaxSafeBufLen;			// 最大安全缓冲区长度
	DWORD	dwThreadNum;				// 全部运行线程数
	DWORD	dwBranchNum;				// 营业部数

	DWORD	dwBuildNo;					// 编译版本号
	CHAR	szBuildDateTime[32];		// 编译时间

	CHAR	szHomePath[MAX_PATH];		// 程序路径
	CHAR	szConfigPath[MAX_PATH];		// 配置文件
	CHAR	szLogFile[MAX_PATH];		// 日志文件路径

	short	nInfoDBCount;				// 消息数据库个数
	short	nInfoSel;					// 消息数据库默认设置
	DBCONFIG aDBInfoConfig[MAX_DBCOUNT];// 消息数据库配置

	DWORD	dwConnTimeOut;				// 数据库连接超时
	DWORD	dwCommTimeOut;				// 数据库执行超时
	BOOL	bHttp;						// 是否支持HTTP连接

	BOOL	bPushMsg;					// 后台是否推送消息
	BOOL	bPushPrdtMsg;				// 后台是否推送产品消息

	BOOL	bWriteLog;					// 是否记录调试日志

	DWORD	dwMsgInterval;				// 数据库扫描时间间隔,单位:秒
	DWORD	dwMsgBeginTime;				// 数据库消息开始扫描时间,单位:秒(HH*3600+MM*60+SS)
	DWORD	dwMsgEndTime;				// 数据库消息结束扫描时间,单位:秒(HH*3600+MM*60+SS)
	__int64	nSeqGID;					// 组消息序列ID
	__int64	nSeqPID;					// 个人消息序列ID

	DWORD	dwSynBeginTime;				// 员工信息同步开始时间(一天同步一次)
	DWORD	dwSynEndTime;				// 员工信息同步结束时间
	DWORD	dwSynLastDate;				// 上一次同步的日期	
};

// 线程信息
typedef struct tagTHREADINFO
{
	HANDLE	hUserState;							// 状态线程,推送用户状态更新
	HANDLE	hOnTimer;							// 定时器线程
	HANDLE	hDBMsgSend;							// 数据库消息推送线程
	HANDLE	hMsgSend;							// 消息推送线程,推送消息
	HANDLE	hRoomUpdate;						// 群组信息更新线程
	HANDLE	hHisMsg;							// 历史消息
	HANDLE	hOffMsg;							// 离线消息
	HANDLE	hInitSvr;							// 初始化服务线程
	HANDLE	hWriteLog;							// 写日志

	HANDLE	hEventExit;							// 退出事件
	BOOL	bStartExit;							// 开始退出
	HANDLE  hEventLog;							// 日志事件
	HANDLE	hTsWorkSvr;							// 服务器取数据
}THREADINFO,*LPTHREADINFO;

// 分组消息用户
typedef struct tagMSGGUSER
{
	__int64	m_nSeqId;
	char	m_szUSERID[USERID_SIZE];
}MSGGUSER,*LPMSGGUSER;

// 外部数据库组消息
typedef struct tagDBGMSG
{
	__int64	m_nID;						// 消息ID
	__int64	m_nUserSeqId;				// 用户SeqId
	BYTE	m_ucGroupType;				// 分组类型
	char	m_szGroupID[21];			// 分组ID
	BYTE	m_ucAttr;					// 属性
	long	m_nEndDate;					// 截止日期
	long	m_nEndTime;					// 截止日期
	char	m_szSrcID[USERID_SIZE];		// 发送者ID
	char	m_szText[MAX_MSG_LEN+1];	// 消息内容
}DBGMSG,*LPDBGMSG;

// 外部数据库个人消息
typedef struct tagDBPMSG
{
	__int64	m_nID;						// 消息ID
	char	m_szUSERID[USERID_SIZE];	// 目标ID
	char	m_szYyb[5];					// 营业部
	BYTE	m_ucAttr;					// 属性
	BYTE	m_ucFlag;					// 标记
	long	m_nEndDate;					// 截止日期
	long	m_nEndTime;					// 截止时间
	char	m_szSrcID[USERID_SIZE];		// 发送者ID
	char	m_szText[MAX_MSG_LEN+1];	// 消息内容
}DBPMSG,*LPDBPMSG;

// 历史消息
typedef struct tagHISMSG
{
	char	m_szSrcID[USERID_SIZE];		// 发送者ID,客户ID
	char	m_szDesID[USERID_SIZE];		// 接收者ID,用户ID
	BYTE	m_ucMsgType;				// 消息类型,0表示个人消息,1表示群组消息
	BYTE	m_ucFormat;					// 消息格式
	long	m_nSendDate;				// 发送日期
	long	m_nSendTime;				// 发送时间
	char	m_szMsg[MAX_TEXT_LEN+1];	// 消息内容
}HISMSG,*LPHISMSG;

// 历史消息2(入库队列)
typedef struct tagHISMSG2
{
	char	m_szSrcID[USERID_SIZE];		// 发送者ID,客户ID
	char	m_szDesID[USERID_SIZE];		// 接收者ID,用户ID
	BYTE	m_ucMsgType;				// 消息类型,0表示个人消息,>0表示群组消息
	BYTE	m_ucFormat;					// 消息格式
	long	m_nSendDate;				// 发送日期
	long	m_nSendTime;				// 发送时间
	long	m_nMsgLen;					// 消息长度
	char	m_szMsg[COMM_TEXT_LEN+1];	// 消息内容
	char*	m_pMsgExt;					// 消息附加内容
}HISMSG2,*LPHISMSG2;

typedef HISMSG	OFFMSG,*LPOFFMSG;
typedef HISMSG2 OFFMSG2,*LPOFFMSG2;

// 群组离线消息
typedef struct tagRoomOffMsg
{
	char	m_szSrcID[USERID_SIZE];		// 发送者ID,客户ID
	char	m_szDesID[USERID_SIZE];		// 接收者ID,用户ID
	char    m_szSrcName[S_NICKNAME_LEN]; // 发送者姓名
	ROOMID  m_nRoomid;                  // 群组id
	BYTE    m_ucRoomType;               // 群组类型
	BYTE	m_ucFormat;					// 消息格式
	long	m_nSendDate;				// 发送日期
	long	m_nSendTime;				// 发送时间
	char	m_szMsg[COMM_TEXT_LEN+1];	// 消息内容
}ROOMOFFMSG,*LPROOMOFFMSG;


// 群组消息
typedef struct tagROOMMSG
{
	ROOMID	m_nRoomID;					// 分组ID
	BYTE	m_ucRoomType;				// 分组类型
	BYTE	m_ucFormat;					// 消息格式
	time_t	m_tmSend;
	BYTE	m_ucUserType;				// m_szSrcID的类型,0客户,1员工		
	char	m_szSrcID[USERID_SIZE];		// 用户ID
	char	m_szMsg[MAX_TEXT_LEN+1];
}ROOMMSG,*LPROOMMSG;

// 产品消息
typedef struct tagPRDTMSG
{
	char	m_szSrcID[USERID_SIZE];		// 发送者
	char	m_szPrdtID[USERID_SIZE];	// 接收者
	BYTE	m_ucFormat;					// 消息格式
	time_t	m_tmSend;					// 发送时间
	char	m_szMsg[MAX_TEXT_LEN+1];	// 发送消息
}PRDTMSG,*LPPRDTMSG;

// 图像信息
typedef struct tagPICINFO
{
	char	m_szPicName[40];
	char	m_szPicAttr[40];
	long	m_nPicLen;
}PICINFO,*LPPICINFO;

// 上传文件请求
// 旧版本
struct UPLOAD_FILE_OLD_REQ
{
	char guid[GUID_SIZE];			// 文件的唯一标识
	char name[128];
	long filelen;
};
// 新版本
struct UPLOAD_FILE_REQ
{
	char guid[GUID_SIZE];				// 文件的唯一标识
	char name[128];						// 文件名
	long filelen;						// 文件长度
	char innerstr[33];					// 文件内部标记串,2012.3.23 新加结构
};

#pragma pack(pop,1)
#endif