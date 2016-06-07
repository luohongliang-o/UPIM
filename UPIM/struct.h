#ifndef STRUCT_H
#define STRUCT_H
//UPIM用到的STRUCT
//单字节对齐
#pragma pack(push,1)

// 字体信息
class CFontInfo
{
public:
	CFontInfo(void)
	{
		m_nSize = 10;
		m_clrText = RGB(0,0,0);
		m_strName = STR_DEFAULT_FONT;
		m_bBold = FALSE;
		m_bItalic = FALSE;
		m_bUnderLine = FALSE;
	}
	~CFontInfo(void){}

public:
	int			m_nSize;				// 字体大小
	COLORREF	m_clrText;				// 字体颜色
	string		m_strName;				// 字体名称
	BOOL		m_bBold;				// 是否加粗
	BOOL		m_bItalic;				// 是否倾斜
	BOOL		m_bUnderLine;			// 是否带下划线
};

// 数据库配置
typedef struct tagBKINFO
{
	int		m_nBkType;
	int		m_nBkIndex;
	DWORD	dwBackColor;
}BKINFO,*LPBKINFO;

// 数据库配置
typedef struct tagHOSTCFG
{
	CHAR	szSVRIP[MAX_PATH];			// 侦听地址
	DWORD	dwSVRPORT;					// 侦听端口
}HOSTCFG,*LPHOSTCFG;

// 服务器配置
struct CLIENTCONFIG
{
	HOSTCFG aHostCfg[MAX_HOSTCOUNT];	// 服务器配置
	DWORD	nMainHost;					// 默认服务器

	CHAR	szHomePath[MAX_PATH];		// 程序路径
	CHAR	szConfigPath[MAX_PATH];		// 配置文件
	CHAR	szFaceConfig[MAX_PATH];		// 表情配置文件
	CHAR	szUserConfig[MAX_PATH];		// 用户自定义配置
	CHAR	szUserDir[MAX_PATH];		// 用户文件夹
	CHAR	szUserImage[MAX_PATH];		// 用户接收图片
	CHAR	szUserFile[MAX_PATH];		// 用户接收文件
	CHAR	szUserHead[MAX_PATH];		// 用户头像图片
	CHAR	szUserDisplay[MAX_PATH];	// 系统图标目录
	CHAR	szLogPath[MAX_PATH];		// 日志文件

	BOOL	m_bClientGrp;				// 我的客户
	BOOL	m_bDefGroup;				// 自定义分组
	BOOL	m_bDept;					// 部门
	BOOL	m_bPublic;					// 公众号权限
	BOOL	m_bEmpProduct;				// 产品
	BOOL    m_bDefGroup2;				// 自定义分组2(客户分组)
	BOOL	m_bHisMsg;					// sqlite的历史消息

	BOOL	m_bWriteLog;
	CHAR	szLogFile[MAX_PATH];		// 日志文件
	DWORD	dwBuildNo;					// 编译版本号
	CHAR	szBuildDateTime[32];		// 编译时间
	CHAR	szRunTime[MAX_PATH];		// 运行时间

	CHAR	szLoginName[MAX_PATH];		// 基于登陆文件的用户名
	DWORD	dwMarket;					// 市场
};

// 用户个人配置
struct USERCONFIG
{
	BOOL	m_bMsgSound;				// 来消息声音
	BOOL	m_bNotifySound;				// 通知消息时声音
	BOOL	m_bOnlineSound;				// 好友上线时声音
	BOOL	m_bShakeSound;				// 窗口抖动时声音

	int		m_nNickType;				// 好友列表中显示的类型(NICKTYPE)
	BKINFO	m_bkInfo;					// 保存的背景图片序号/背景颜色
	CFontInfo m_fontInfo;				// 全局字体信息

	BOOL	m_bSendImage;			// 是否允许发送图片,当此项为FALSE时,不能发送图片
	BOOL	m_bSendOffImage;		// 是否发送离线图片,m_bSendImage为TRUE时有效,为TRUE时，不发送中转图片，图片先上传到服务器
};

// 当前选中节点信息
struct SELECTNODE
{
	char		m_szID[USERID_SIZE];	// 用户列表中被选中的人的id或者群id
	EPANELTYPE	m_nPanelType;			// 选中面板类型，表示是个人聊天/组聊天/最近联系人
	ENODETYPE	m_nNodeType;			// 选中节点类型，表示是节点，还是用户
	EUSERTYPE	m_nSelUserType;			// 选中人的类型，m_nNodeType=0有效，个人面板中，标注用户类别
	E_GROUPCHAT_TYPE m_nGroupType;		// 如果选中的是群组，保存群组类型
	LPVOID		lpNode;					// 选中节点的Node指针
};

// 消息包发送结构
typedef struct tagMSG_PKG_INFO
{
	DWORD	dwMsgSeq;					// 包序号
	char	guid[GUID_SIZE];			// 包的唯一标识
	char	userid[USERID_SIZE];		// 包的接收方,如果是群组，则是发送方ID
	ROOMID  nRoomID;					// 群组ID
	BYTE	ucRoomType;					// 群组类型 E_GROUPCHAT_TYPE
	BYTE	ucMsgType;					// 消息类型 E_MSGSEND_TYPE 
	char	szInnerID[USERID_SIZE];		// 内部ID
	time_t	tmSend;						// 发送时间
	BYTE	nSendCount;					// 重试次数 最大重试次数 SEND_MAXCOUNT
	BYTE	ucFormat;					// 消息格式 EMSGTEXTFORMAT 
	int		nPublicID;					// 公众号ID
	int		nRouteID;					// 路由序号
	LPVOID  lpData;						// 包的自定义结构 COMPLEX_MSG_DATA*
}MSG_PKG_INFO,*LPMSG_PKG_INFO;


enum EIMAGETYPE
{
	eImgType_SysFace=0,		// 系统表情
	eImgType_Image,			// 图片
};

//richedit中的图片信息
struct PIC_OBJECT_INFO
{
	BYTE	nType;						// 图片类型
	int		nTextPos;					// 在文本的起始位置
	BYTE	ucState;					// 状态
	char	szFile[48];					// 文件名
	int		nTotalLen;					// 总长度
	int		nLen;						// 当前长度
	char*	pBuf;						// 具体的数据
};

// 大型包的包头
struct PACKAGE_HEADER
{
	WORD	protocolid;			// 协议编号
	DWORD	packagelen;			// 包长(不包括包头);
	DWORD   dwReserved;			// 
	PACKAGE_HEADER()
	{
		packagelen = 0;
		dwReserved = 0;
	}
};

struct COMPLEX_MSG_DATA
{
	int nTextLen;						// 文本长度
	char* pBuf;							// 文本内容
	short sSend;						// 发送部分，-1,准备发送文本，0--nObjCount-1，发送图片序号
	int nObjCount;						// 图片对象个数
	PIC_OBJECT_INFO* pObjectInfo;
};

struct GUPID
{
	char		m_szUSERID[USERID_SIZE];
	EUSERTYPE	m_nUserType;
};

typedef struct tagCGPARAM
{
	ROOMID	m_nRoomID;
	int		m_nRoomType;
	char	m_szMasterID[USERID_SIZE];
	char	m_szRoomName[ROOMNAME_SIZE];
	std::vector<GUPID>	m_vtUpID;
}CGPARAM,*LPCGPARAM;

// 添加成员
struct GROUP_CHAT_ADD_STU
{
	ROOMID	roomid;						// 聊天室id
	int		roomtype;					// 聊天室类型
	// attach
	int		count;
	GUPID	agUpid[REFRESH_GUSER_COUNT];
};

// 消息请求包
struct  MSG_PKG_TRANSFER_REQ
{
	char guid[GUID_SIZE];			// 消息guid
	short sSend;					// 图片序号
	char filename[48];				// 文件名
	bool request;					// 是请求包还是回应包
	int totallength;				// 总长度
	int length;						// 数据长度
	char data[1];					// 用户发送的消息
};

// 消息应答包
struct MSG_PKG_TRANSFER_ANS
{
	char	guid[GUID_SIZE];		// GUID
	DWORD	dwSeq;					// 包序列号
	char	filename[48];
	int		length;
};

// 群组发包
struct GROUP_CHAT_CLIENT_SEND_PKG_REQ
{
	DWORD	seq;				// 包序列号
	char	guid[17];			// guid
	char	filename[44];		// 文件名
	ROOMID	roomid;				// 聊天室id
	int		roomtype;			// 聊天室类型
	char	tqid[USERID_SIZE];	// 发送方ID
	int		totallen;			// 总长度
	int		original_len;		// 原始长度
	int		pos;				// 位置
	WORD	msglen;				// 长度
	char	msg[1];				// 内容
};

// 群组收包
struct GROUP_CHAT_CLIENT_RECV_PKG_ANS
{
	char	guid[GUID_SIZE];	// guid
	char	filename[44];		// 文件名
	ROOMID	roomid;				// 聊天室id
	int		roomtype;			// 聊天室类型
	char	tqid[USERID_SIZE];	// 发送方ID
	int		totallen;			// 总长度
	int		original_len;		// 原始长度
	DWORD	pos;				// 位置
	WORD	msglen;				// 长度
	char	*msg;				// 内容
};

// 消息结构2.0
struct CHAT_RECORD2
{
	int		seq;						// seq序列主键
	char	fromid[USERID_SIZE];		// 发送userid
	char	toid[USERID_SIZE];			// 接收userid
	int		roomid;						// 房间ID
	int		senddate;					// 日期 YYYYMMDD
	int		sendtime;					// 时间 HHMMSS
	int		type;						// 类型 E_CHATRECORD_TYPE
	int		format;						// 格式 EMSGTEXTFORMAT
	int		msglen;						// 消息长
	char*	msg;						// 消息内容
};

// 发送消息后收到的服务器应答
typedef struct tagSENDMSG_ANS_
{
	char	m_szDesID[USERID_SIZE];
	long	m_nState;
	DWORD	m_dwSeq;
	long	m_tmSend;
	char	m_szInnerID[USERID_SIZE];
}SENDMSG_ANS_,*LPSENDMSG_ANS_;

// 接收到的好友消息
struct RECEIVED_MSG
{
	char	imid[USERID_SIZE];			// 发送者ID
	char	nickname[S_NICKNAME_LEN];	// 昵称
	long	senddate;					// 发送日期
	long	sendtime;					// 发送时间
	BYTE	msgtype;					// 消息类型,是否转接消息
	char	innerID[USERID_SIZE];		// 内部ID
	BYTE	format;						// 消息内容格式,EMSGTEXTFORMAT
	int		msglen;						// 消息长度
	char*	msg;						// 内容
	RECEIVED_MSG()
	{
		memset(this,0,sizeof(RECEIVED_MSG));
	}
};
typedef std::vector<RECEIVED_MSG> VECTOR_RECEIVED_MSG;

// 接收到的好友信息2
// 如果是收到的中转消息，则m_szUSERID是消息的最原始发送方(src),m_szInnerID是消息的中转方(mid),登录用户为消息的中转目标方.
typedef struct tagRECVMSG2
{
	DWORD	m_dwSeq;					// 序列
	char	m_szUSERID[USERID_SIZE];	// 发包人id;
	BYTE	m_ucUserType;				// 用户类型
	BYTE	m_ucMsgType;				// 消息类型
	char	m_szInnerID[USERID_SIZE];	// 内部ID
	long	m_nSendDate;				// 发送日期
	long	m_nSendTime;				// 发送时间
	BYTE	m_ucFormat;					// 消息的格式 EMSGTEXTFORMAT
	int		nPublicID;					// 公众号ID
	WORD	m_nMsglen;					// 消息长,仅对复文本有意义
	char	m_szMsg[MAX_TEXT_LEN+1];	// 用户发送的消息
}RECVMSG2,*LPRECVMSG2;

// 用户分组信息
struct USER_GROUP_INFO
{
	BYTE	m_ucGroupType;				// EGROUPLISTTYPE 分组类型
	DWORD	m_nGroupID;
	char	m_szGroupName[GROUPNAME_SIZE];
	DWORD	m_nParentID;
	bool	m_bLoad;					// 是否加载
	bool	m_bCreateNode;				// 是否创建节点
	short	m_nFailCount;
};

// 用户群组信息
struct USER_ROOM_INFO
{
	BYTE	m_ucGroupType;				// EGROUPLISTTYPE 分组类型
	DWORD	m_nGroupID;
	char	m_szGroupName[GROUPNAME_SIZE];
	DWORD	m_nParentID;
	bool	m_bLoad;					// 是否加载
	bool	m_bCreateNode;				// 是否创建节点
	short	m_nFailCount;
};

// 分析师房间用户信息
struct USER_ROOMCLIENT_INFO
{
	char	m_szUPID[USERID_SIZE];				// UP用户ID
	char	m_szImgURL[MAX_PATH];				// 用户头像url(通过网页接口取得)
	char	m_szImgName[IMG_NAME_SIZE];			// 用户头像文件名
	char	m_szLNomalImgName[IMG_NAME_SIZE];	// 处理之后的本地文件名
	char	m_szLGrayImgName[IMG_NAME_SIZE];	// 处理之后的本地文件名
	char	m_szNodeName[NODE_NAME_SIZE];		// 用户节点名，跟列表树中的一致
	char	m_szLastWords[LAST_WORD_SIZE];		// 最后交谈内容

	BOOL	m_bIsImgDownLoad;
	BOOL	m_bIsOnLine;
	BOOL	m_bLoad;							// 是否加载
	BOOL	m_bGroupLoad;						// 自定义的分组是否添加
	int		m_nGroupID;							// 分组序号
	BOOL	m_bCreateNode;						// 是否创建节点
	short	m_nFailCount;						// 失败次数
	USER_ROOMCLIENT_INFO()
	{
		COPY_USERID(m_szUPID,0) ; 

	}
	// UP的用户名唯一  当用户名相同时 认为两个用户是同一个人 
	BOOL operator==( const USER_ROOMCLIENT_INFO& other )
	{
		if( strlen(m_szUPID) > 0 )
		{
			if( strcmp(m_szUPID ,other.m_szUPID) == 0 )
				return TRUE ; 
			else
				return FALSE ; 
		}
		else
			return FALSE ; 
	}
};

// bLoad 
// bUsed 在数据m_groupInfoStc中，当前节点是否被使用
// bGetUserInfo 是否加载该分组的客户 
struct ANALYSTGROUP_INFO
{
	bool        bLoad ;                         // 是否显示按钮 
	bool		bUsed;							// 是否使用 
	bool		bGetUserInfo;					// 是否已经加载了分组下的用户信息
	bool		bCreateNode;					// 是否创建节点
	int			nGroupID;						// 分组序号，与后台数据库数据一致
	CString		GroupName;						// 分组名称
};

// 分析师房间用户信息
struct ROOMANALYS_INFO
{
	int		m_nAnalysID;						// 分析师ID
	char	m_szAnalysName[USERID_SIZE];		// 分析师名称

	char	m_szGood[MAX_PATH];					// 分析师擅长领域
	char	m_szImgURL[MAX_PATH];				// 用户头像url(通过网页接口取得)
	char	m_szImgName[IMG_NAME_SIZE];			// 用户头像文件名
	char	m_szLocalImgName[IMG_NAME_SIZE];	// 处理之后的本地文件名

	char	m_szNodeName[NODE_NAME_SIZE];		// 用户节点名，跟列表树中的一致
	char	m_szLastWords[LAST_WORD_SIZE];		// 最后交谈内容
	BOOL	m_bIsOnLine;
	bool	m_bLoad;							// 是否加载
	bool	m_bCreateNode;						// 是否创建节点
	short	m_nFailCount;						// 失败次数
};

// 所有房间信息
struct ALLCLIENT_ROOM_INFO
{
	int		m_nRoomID;							// 房间ID
	char	m_szRoomName[USERID_SIZE];			// 房间名称
	int		m_nOnLineAnalysNum;					// 在线的分析师数量
	int		m_nAnalysNum;						// 分析师数量
	int		m_nVipRight;						// 权限串
	int		m_nIsVip;							// 是否是VIP房间
	bool	m_bLoad;							// 是否加载
	bool	m_bCreateNode;						// 是否创建节点
	short	m_nFailCount;						// 失败次数
	vector<ROOMANALYS_INFO>* pVtAnalys;			// 房间下的分析师集合
};
typedef std::vector<ROOMANALYS_INFO> VECTOR_ROOMANALYS_INFO;

struct MODIFY_USERINFO_STU
{
	char	userid[S_USERID_LEN];
	char	nickname[S_NICKNAME_LEN];
	char	sex;
	long	birthday;
	char	postalcode[7];
	char	telenum[41];
	char	mobilenum[S_MOBILE_LEN];
	char	email[S_EMAIL_LEN];
	char	department[41];
	char	sign[S_SIGNATURE_LEN];
};

// 与登录用户有关的人的信息的结构
typedef struct tagFRIEND_INFO
{
	char userid[S_USERID_LEN];			// userid
	char nickname[S_NICKNAME_LEN];		// 昵称
	char truename[S_USERNAME_LEN];		// 真实姓名
	long relation;						// 分组关系
	BYTE type;							// 分组类型 EGROUPLISTTYPE
	BYTE sex;							// 性别,EGENDER
	BYTE iconindex;						// 图标序号
	BYTE online;						// 是否在线
	char mobile[S_MOBILE_LEN];			// 移动电话
	char email[S_EMAIL_LEN];			// 电子邮箱	
	char sign[S_SIGNATURE_LEN];			// 签名
	DWORD reserved;						// 保留
	bool m_bLoad;						// 是否加载
}FRIEND_INFO,*LPFRIEND_INFO;

// 问题路由的信息结构
struct ROUTE_INFO
{
	int		nID;						// ID
	char	m_RName[USERID_SIZE];		// 问题路由名称
	int		nProblemtype;				// Problemtype
};
typedef std::vector<ROUTE_INFO> VECTOR_ROUTE_INFO;

// 公众号的信息结构
typedef struct tagPUBLIC_INFO
{
	int		nPublicID;					// 公众号ID
	char	szName[S_NICKNAME_LEN];		// 公众号昵称(32)
	char	szInfo[S_SIGNATURE_LEN];	// 公众号说明(128)
	char	szWelcome[S_SIGNATURE_LEN];	// 公众号欢迎语(128)
	bool	m_bLoad;					// 是否加载
	vector<ROUTE_INFO>* pVtRoute;		// 问题路由
}PUBLIC_INFO,*LPPUBLIC_INFO;

// 投顾聊天室的信息结构
typedef struct tagTGROOM_INFO
{
	int		nRoomID;					// 聊天室ID
	char	szName[S_NICKNAME_LEN];		// 聊天室昵称(32)
	char	szInfo[S_SIGNATURE_LEN];	// 聊天室说明(128)
	bool	m_bLoad;					// 是否加载
}TGROOM_INFO,*LPTGROOM_INFO;

struct STATE_INFO
{
	char	userid[S_USERID_LEN];		// id
	short	nUserstate;					// 用户状态
};

struct IMAGE_INFO
{
	int nFaceNo;
	char filename[44];
	BOOL bFinish;		//图片是否已装载
	BOOL bSend;			//对于该图片是否为发送方
	time_t tmR;			//接收方最初收到此图时间
	IMAGE_INFO()
	{
		memset(this,0,sizeof(IMAGE_INFO));
		nFaceNo = -1;
	}
};

struct AnalystInfo
{
	DWORD		AnalystId;		// 分析师ID 
	CString		AnalystName;	// 代码
	CString		sDescription;	// 简介
	CString		sImg;			// 头像URL
	CString		sDownImg;		// 下载原始头像
	CString		sLocalImg;		// 下载下来的本地头像路径
	CString		sLocalGrayImg;	// 下载的头像本地处理
	BOOL		bIsImgDownLoad;	// 头像是否下载完毕
	BOOL		bIsShow;		// 是否显示
	int			bIsOnLine;		// 是否状态
	int			nControlIndex;	// 顶部控件序号
};


#pragma pack(pop,1)
#endif