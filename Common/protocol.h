#ifndef PROTOCOL_H
#define PROTOCOL_H

#include "define.h"
//////////////////////////////////////////////////////////////////////////
#pragma pack(push,1)
//单字节对齐
typedef struct tagIMReqHeader
{
	struct
	{
		unsigned char Version;		//:4;
		unsigned char Compressed;	//:1;
		unsigned char Encrypted;	//:1;
		unsigned char Reserved;		//:2;
	}	Info;
	unsigned long  PacketLen;
	unsigned long  RawLen;
	char  Priority;
} IMREQHEADER;

typedef struct tagIMAnsHeader
{
	long		   Separator;
	unsigned long  PacketLen;
	unsigned long  RawLen;
	struct
	{
		unsigned char Version;		//:4;
		unsigned char Compressed;	//:1;
		unsigned char Encrypted;	//:1;
		unsigned char Reserved;		//:2;
	}	Info;
	char	Priority;
	long	ErrorCode;				// 错误信息
	long	req;
} IMANSHEADER;

#ifdef _NEW_HEADER
typedef struct tagIMReqHeader
{
	union
	{
		unsigned long dwcrc;
		char newversion[4];
	};
	struct
	{
		unsigned char Version;		//:4;
		unsigned char Compressed;	//:1;
		unsigned char Encrypted;	//:1;
		unsigned char Reserved;		//:2;
	}	Info;
	DWORD	cookie;
	long	MainID;
	long	AssisID;
	unsigned long  PacketLen;
	unsigned long  RawLen;
	char  Priority;
} IMREQHEADER;

typedef struct tagIMAnsHeader
{
	union
	{
		unsigned long dwcrc;
		char newversion[4];
	};
	long		   Separator;
	unsigned long  PacketLen;
	unsigned long  RawLen;
	struct
	{
		unsigned char Version;		//:4;
		unsigned char Compressed;	//:1;
		unsigned char Encrypted;	//:1;
		unsigned char Reserved;		//:2;
	}	Info;
	DWORD	cookie;
	long	MainID;
	long	AssisID;
	char	Priority;
	long	req;
} IMANSHEADER;
#endif // _NEW_HEADER

typedef struct 
{
	IMREQHEADER		ReqHeader;
	char			ReqBuffer[REQBUFFER_LEN];
} IMREQBUFFER;

typedef struct 
{
	IMANSHEADER		AnsHeader;
	char			AnsBuffer[ANSBUFFER_LEN];
} IMANSBUFFER;

//////////////////////////////////////////////////////////////////////////
//用户登录(100)_req
//UPIM_REQ_CHECKPWD
struct login_req
{
	short	req;
	short	usertype;						//用户类型
	char	userid[S_USERID_LEN];			//登录账号/客户号
	char	userpwd[S_USERPWD_LEN];			//登录密码
};

//用户登录(100)_ans
//UPIM_REQ_CHECKPWD
struct login_ans
{
	char	userid[S_USERID_LEN];			// 用户账号
	char	username[S_USERNAME_LEN];		// 用户姓名
	short	role;							// 角色
	char	useryyb[S_YYB_LEN];				// 营业部
	short	sex;							// 性别，1男2女0未知
	char	yyb[S_YYB_LEN];					// 真实营业部
	short	deptid;							// 部门id	
	char	funcctrl[S_FUNCCTRL_LEN];		// 服务器对客户的功能控制
	char	signature[S_SIGNATURE_LEN];		// 个性签名
};

//分组人员(5830)_req
//UPIM_REQ_GUSER
struct guser_req
{
	short	req;
	char	userid[S_USERID_LEN];			// 登录账号/客户号
	short   ntype;							// 类型
	short	ngroupid;						// 分组ID
	short	nLevel;							// **待删除
	short	ndeptid;						// **待删除
};

//好友结构
struct guser_unit
{
	char	userid[S_USERID_LEN];			// 用户账号
	char	username[S_USERNAME_LEN];		// 用户姓名
	short	sex;							// 性别，1男2女0未知
	short	state;							// 在线状态
	char	mobile[S_MOBILE_LEN];			// 手机
	char	signature[S_SIGNATURE_LEN];		// 个性签名
};

//分组人员(5830)_ans
//UPIM_REQ_GUSER
struct guser_ans
{
	short	rtnnum;							// 返回个数
	short   ntype;							// 类型
	short	ngroupid;						// 分组ID
	struct	guser_unit headp[1];			// 内容数组头	
};

//分组人员(5830)_ans
//UPIM_REQ_GUSER2
struct guser_ans2
{
	short	rtnnum;							// 返回个数
	short   ntype;							// 类型
	short	ngroupid;						// 分组ID
	int		nlen;							// 后面char长度
	char	headp[1];						// 内容json	
};

//消息发送(5734)_req
//UPIM_REQ_SENDMSG
struct sendmsg_req
{
	short	req;
	char	cFromuserid[S_USERID_LEN];		// 发送者账号
	DWORD	dwMsgSeq;						// 消息序列
	short	nUsertype;						// 用户类型
	char	cTouserid[S_USERID_LEN];		// 接收者账号
	time_t	tmSend;							// 消息发送时间
	BYTE	ucMsgType;						// 消息类型
	char	innerID[S_USERID_LEN];			// 内部ID
	int		nPublicID;						// 公众号ID
	int		nRouteID;						// 路由序号
	int		format;							// 消息的格式 EMSGTEXTFORMAT
	int		msglen;							// 消息长,仅对复文本有意义
	char	msg[1];							// 用户发送的消息	
};

//数据包发送（5766）_req
//UPIM_REQ_PKG
struct send_pkg_req
{
	short	req;
	char	cFromuserid[S_USERID_LEN];		// 发送者账号
	short	nUsertype;						// 用户类型
	char	cTouserid[S_USERID_LEN];		// 接收者账号
	int		nrequest;						// 是请求包还是回应包
	int		pkglen;							// 消息长,仅对复文本有意义
	char	pkg[1];							// 用户发送的消息
};

struct s_send_pkg_ans
{
	char	cFromuserid[S_USERID_LEN];		// 发送者账号
	short	nUsertype;						// 用户类型
	int		nrequest;						// 是请求包还是回应包
	int		pkglen;							// 消息长,仅对复文本有意义
	char	pkg[1];							// 用户发送的消息
};

// 广播请求
struct broadcast_req
{
	int	type;								// 广播类型
	char nickname[S_NICKNAME_LEN];			// 发送者昵称
	char toid[64];							// 接受者
	long msglen;							// 消息长度
	char msg[1];							// 广播内容
};

struct sendmsg_ans
{
	char	cTouserid[S_USERID_LEN];		// 接收者账号
	int		nUserState;						// 用户在线状态
	DWORD	dwMsgSeq;						// 消息序列
	time_t	tmSend;							// 消息发送时间
};


//服务器消息发送(5737)_req
//UPIM_REQ_S_SENDMSG
struct s_sendmsg_ans
{
	char	cFromuserid[S_USERID_LEN];		// 发送者账号
	DWORD	dwMsgSeq;						// 消息序列
	BYTE	ucMsgType;						// 消息类型
	char	innerID[S_USERID_LEN];			// 内部ID
	int		format;							// 消息的格式 EMSGTEXTFORMAT
	time_t	tmSend;							// 消息发送时间
	int		nPublicID;						// 公众号ID
	int		msglen;							// 消息长,仅对复文本有意义
	char	msg[1];							// 用户发送的消息
};

//分组人员(5827)_req
//UPIM_REQ_GLIST
struct glist_req
{
	short	req;
	char	userid[S_USERID_LEN];			// 登录账号/客户号
	short	ngrouptype;						// 分组ID
};

// 分组
struct  glist_unit
{
	DWORD	m_nGroupID;
	char	m_szGroupName[GROUPNAME_SIZE];
	BYTE	m_ucGroupType;
};

//分组人员(5827)_ans
//UPIM_REQ_GLIST
struct glist_ans
{
	short	rtnnum;							// 返回个数
	struct	glist_unit headp[1];			// 内容数组头	
};

//得到联系人列表(5720)_req
//UPIM_REQ_LIST
struct list_req
{
	short	req;
	char	userid[S_USERID_LEN];			// 登录账号/客户号
	short	ngrouptype;						// 分组ID
};

//好友结构
struct list_unit
{
	char	userid[S_USERID_LEN];			// 用户账号
	char	username[S_USERNAME_LEN];		// 用户姓名
	short	sex;							// 性别，1男2女0未知
	short	relation;						// 关系代码
	short	state;							// 在线状态
	char	signature[S_SIGNATURE_LEN];		// 个性签名
};

//得到联系人列表(5720)_ans
//UPIM_REQ_LIST
struct list_ans
{
	short	rtnnum;							// 返回个数
	struct	list_unit headp[1];				// 内容数组头	
};

struct list_ans2
{
	int  len;
	char headp[1];							// 内容json	
};

//得到分组列表(5718)_req
// UPIM_REQ_DEPT
struct dept_req
{
	short	req;
	char	userid[S_USERID_LEN];			// 登录账号/客户号
	short	ndeptid;						// 分组ID
};

//部门结构
struct dept_unit
{
	short	deptid;							// deptid
	char	deptname[S_USERID_LEN];			// 部门名称
	short	predept;						// 父部门
};

//得到联系人列表(5718)_ans
//UPIM_REQ_DEPT
struct dept_ans
{
	short	rtnnum;							// 返回个数
	struct	dept_unit headp[1];				// 内容数组头	
};

//得到群组列表(5782)_req
// UPIM_REQ_GCINFO
struct gcinfo_req
{
	short	req;
	char	userid[S_USERID_LEN];			// 登录账号/客户号
	short	type;							// 是否是客户？
	long	RoomId;							//
	short	Roomtype;						// 
};

//群组结构
struct gcinfo_unit
{
	long	RoomId;							// 群组id
	short	Roomtype;						// 群组类型
	char	masterid[S_USERID_LEN];			// 创建者账号/客户号
	char	Roomname[S_USERID_LEN];			// 群组名称
	char	RoomNote[NOTE_SIZE];			// 群公告(101以内)
	short	delflag;						// 标记(删除)
};

//得到群组列表(5782)_ans
// UPIM_REQ_GCINFO
struct gcinfo_ans
{
	short	rtnnum;							// 返回个数
	struct	gcinfo_unit headp[1];			// 内容数组头	
};

//得到群组列表(5782)_ans_JSON
// UPIM_REQ_GCINFO2
struct gcinfo_ans2
{
	short	rtnnum;							// 返回个数
	int		nlen;							// 后面char长度
	char	headp[1];						// 内容json	
};

// 被添加入群组之后的操作
// UPIM_REQ_S_GCADD
struct gc_s_add_ans
{
	int		nlen;							// 后面char长度
	char	headp[1];						// 内容json	
};


//得到群组列表(5788)_req
// UPIM_REQ_GCGETLIST
struct gcgetlist_req
{
	short	req;
	long	RoomId;							// 群组ID
	short	Roomtype;						// 群组类型
	int     nStartPos;						// 开始序号，以免一次取不完整
};

//群组成员结构
struct gcgetlist_unit
{
	char	userid[S_USERID_LEN];			// 群成员账号/客户号
	short	usertype;						// 用户分类
	short	state;							// 用户在线状态
};

//得到群组列表(5788)_ans
// UPIM_REQ_GCINFO
struct gcgetlist_ans
{
	long	RoomId;							// 群组id
	int		nStartPos;						// 开始序号
	short	rtnnum;							// 返回个数
	struct	gcgetlist_unit headp[1];		// 内容数组头	
};


// 群组发送消息(文本)(5796)_req
// UPIM_REQ_GCSENDMSG
struct gcsendmsg_req
{
	short	req;
	DWORD	dwMsgSeq;						// 包序列号
	char	cFromuserid[S_USERID_LEN];		// 发送者userid
	short	nUsertype;						// 用户类型
	char	nickname[S_NICKNAME_LEN];		// 发送者昵称
	ROOMID	roomid;							// 聊天室id
	int		roomtype;						// 聊天室类型
	time_t	tmSend;							// 发送时间
	int		format;							// 消息格式
	int		msglen;							// 长度
	char	msg[1];							// 内容
};

// 群组接收消息(文本):服务器推送
struct GROUP_CHAT_HIS_MSG
{
	char	cFromuserid[S_USERID_LEN];		// 发送者userid
	char	nickname[S_NICKNAME_LEN];		// 发送者昵称
	BYTE	usertype;						// 用户类型
	ROOMID	roomid;							// 聊天室id
	int		roomtype;						// 聊天室类型
	time_t	tmSend;							// 发送时间
	int		format;							// 消息格式
	int		msglen;							// 长度
	char	*msg;							// 内容
};

// 群组发送消息(文本)(5796)_ans
// UPIM_REQ_GCSENDMSG
struct gcsendmsg_ans
{
	long	RoomId;							// 群组id
	short	Roomtype;						// 群组类型
	DWORD	dwMsgSeq;						// 消息序列
	time_t	tmSend;							// 消息发送时间
};

// 群组发送消息(文本)(5798)_ans
// UPIM_REQ_GCRECVMSG
struct gcrecvmsg_ans
{
	char	cFromuserid[S_USERID_LEN];		// 发送者userid
	char	nickname[S_NICKNAME_LEN];		// 发送者昵称
	short	nUsertype;						// 用户类型
	ROOMID	roomid;							// 聊天室id
	short	roomtype;						// 聊天室类型
	time_t	tmSend;							// 发送时间
	int		format;							// 消息格式
	int		msglen;							// 长度
	char	msg[1];							// 内容
};

// 聊天室创建(5784)_req
//UPIM_REQ_GCCREATE
struct gccreate_req
{
	short	req;
	char	createuserid[S_USERID_LEN];		// 创建者id
	char	roomname[S_NICKNAME_LEN];		// 聊天室名称
	short	roomtype;						// 聊天室类型
	int		nroomParam;						// 房间好友列表参数
};

// 聊天室创建(5784)_ans
//UPIM_REQ_GCCREATE
struct gccreate_ans
{
	bool	bSussess;						// 是否成功
	ROOMID	nRoomID;						// 创建成功的房间id
	int		nroomParam;						// 房间好友列表参数
	char	ErrStr[S_ERRSTR_LEN];			// 错误信息
};

// 添加聊天室成员(5786)_req
// UPIM_REQ_GCADD
struct gcadd_req
{
	short	req;
	ROOMID	nRoomID;						// 房间id
	short	roomtype;						// 聊天室类型

	int		szlen;							// 长度
	char	szUpID[1];						// 成员ID序列
};

// 添加聊天室成员(5786)_ans
// UPIM_REQ_GCADD
struct gcadd_ans
{
	ROOMID	nRoomID;						// 房间id
	short	roomtype;						// 聊天室类型
};

// UPIM_REQ_GCKICK
// 踢出聊天室成员(5794)_req
// UPIM_REQ_GCLEAVE
// 主动退出聊天室(5790)_req
struct gcleave_req
{
	short	req;
	ROOMID	nRoomID;						// 房间id
	short	roomtype;						// 聊天室类型
	char	userid[S_USERID_LEN];			// id
	short	nUsertype;						// 用户类型
};

// UPIM_REQ_GCKICK
// 踢出聊天室成员(5794)_ans
// UPIM_REQ_GCLEAVE
// 主动退出聊天室(5790)_ans
struct gcleave_ans
{
	bool	bSussess;						// 是否成功
	ROOMID	nRoomID;						// 房间id
	char	userid[S_USERID_LEN];			// id
	short	nUsertype;						// 用户类型
	char	ErrStr[S_ERRSTR_LEN];			// 错误信息
};

// UPIM_REQ_S_GCKICK
// 被踢出聊天室_ans
struct gckick_s_ans
{
	ROOMID	nRoomID;						// 房间id
	short	roomtype;						// 聊天室类型
	char	KickId[S_USERID_LEN];			// KickId
};

// UPIM_REQ_GPUBLIC
// 获取公众号
struct public_req
{
	short   req;
	char	userid[S_USERID_LEN];			// id
	short	nUsertype;						// 用户类型
};

struct public_ans
{
	int	 rtnnum;							// 返回个数
	int  len;								// 包长
	char headp[1];							// 内容json	
};

// UPIM_REQ_PUBLICINFO
// 获取公众号信息，如果是问题路由，则在主界面显示
struct publicinfo_req
{
	short   req;
	int		publicID;						// publicID
};

struct publicinfo_ans
{
	int		publicID;
	int		rtnnum;							// 返回个数
	int		len;							// 包长
	char	headp[1];						// 内容json	
};

// UPIM_REQ_PUBCONCLOSE
// 客户断开与公众号的对话
struct pubconclose_req
{
	short   req;
	int		publicID;						// 公众号ID
	int		routeID;						// 路由ID
	char	userid[S_USERID_LEN];			// 自己的ID
	char	SevcId[S_USERID_LEN];			// 客服人员ID
	int		closeType;						// 关闭类型(0:超时断开；1:主动关闭；2:其余情况)
};

// UPIM_REQ_M_USERINFO
// 修改用户资料
struct m_userinfo_req
{
	short   req;
	int		len;							// 包长
	char	headp[1];						// 内容json	
};

// UPIM_REQ_RECONNECT
// 用户重新连接服务器，需发包通知
struct reconnect_req
{
	short   req;
	char	userid[S_USERID_LEN];			// id
	short	nUsertype;						// 用户类型
};

// 用户重新连接服务器，返回包
struct reconnect_ans
{
	int		nResult;						// 重登录返回结果
	int		nErrorCode;						// 用户错误类型
};

// UPIM_REQ_S_ONLINE
// 服务器推送的用户在线信息 
struct online_s_ans
{
	short   req;
	char	userid[S_USERID_LEN];			// id
	short	nUserstate;						// 用户状态
};

// UPIM_REQ_OLNOTIFY
// 修改用户状态改变推送
struct olnotify_req
{
	short   req;
	char	userid[S_USERID_LEN];			// id
	short	nUserstate;						// 用户状态
	int		len;							// 包长
	char	headp[1];						// 内容json	
};

// UPIM_REQ_OFFMSG
// 用户登录之后取得离线消息
struct offmsg_req
{
	short   req;
	char	userid[S_USERID_LEN];			// id
	short	nUsertype;						// 用户类型
};

// json格式的离线消息
struct offmsg_ans
{
	int		rtnnum;							// 返回个数
	int		len;							// 包长
	char	headp[1];						// 内容json	
};

// UPIM_REQ_KICKOFF
// 同一ID异地登录之后，服务器向前一个ID发消息，表示被踢
struct kickoff_msg
{
	int		nMsg;							// 踢人类型
};

// UPIM_REQ_SVROFF
// 服务器异常关闭，向每一个在线ID发消息，通知服务器关闭
struct svroff_msg
{
	int		nMsg;							// 关闭类型
};

// UPIM_REQ_ALLTGROOM
// 获取投顾聊天室
struct alltgroom_req
{
	short   req;
	char	userid[S_USERID_LEN];			// id
	short	nUsertype;						// 用户类型
};

// UPIM_REQ_ALLTGROOMEX
// 客户的IM，需要当前版本的所有房间所有分析师列表
struct alltgroomEx_req
{
	short   req;
	short   nMarket;
	char	userid[S_USERID_LEN];			// id
	short	nUsertype;						// 用户类型
};

// UPIM_REQ_ALLROOMTOUSER
// 投顾的IM，需要当前有权限的所有用户列表
struct tggetuserbyroomid_req
{
	short   req;
	char	userid[S_USERID_LEN];			// id
	short	nUsertype;						// 用户类型
	int     roomid;                         // 客户房间号
};

// UPIM_REQ_ALLTGROOMEX/UPIM_REQ_ALLROOMTOUSER 共用返回json格式
// json格式的投顾聊天室信息
struct alltgroom_ans
{
	int		rtnnum;							// 返回个数
	int		len;							// 包长
	char	headp[1];						// 内容json	
};

// UPIM_REQ_ONLINE
// 查询好友状态
struct userstate_req
{
	short   req;
	char	userid[S_USERID_LEN];			// id
	short	nUsertype;						// 用户类型
	int		flag;							// 标志位
	int		len;							// 内容长度
	char	headp[1];						// 内容	
};

// json格式的好友状态信息
struct userstate_ans
{
	int		flag;							// 标志位
	int		rtnnum;							// 返回个数
	int		len;							// 包长
	char	headp[1];						// 内容json	
};

// UPIM_REQ_GETGROUP
// im2.0取得所有的自定义分组信息
struct getgroup_req
{
	short   req;
	char	userid[S_USERID_LEN];			// id
	short	nUsertype;						// 用户类型
};

struct getgroup_ans
{
	int		flag;							// 标志位
	int		rtnnum;							// 返回个数
	int		len;							// 包长
	char	headp[1];						// 内容json	
};

// UPIM_REQ_GETGUSER
// im2.0取得分组下对应的用户列表
struct getguser_req
{
	short   req;
	char	userid[S_USERID_LEN];			// id
	short	nUsertype;						// 用户类型
	int		nGroupID;						// 自定义的分组ID
};

struct getguser_ans
{
	int		nGroupID;						// 自定义的分组ID
	int		flag;							// 标志位
	int		rtnnum;							// 返回个数
	int		len;							// 包长
	char	headp[1];						// 内容json	
};

// UPIM_REQ_ADDGROUP
// im2.0添加/修改自定义分组信息
struct addgroup_req
{
	short   req;
	char	userid[S_USERID_LEN];			// id
	int		len;							// 包长
	char	headp[1];						// 内容json	
};
// 
struct addgroup_ans
{
	int		flag;							// 标志位
	int		rtnnum;							// 返回个数
	int		len;							// 包长
	char	headp[1];						// 内容json	
};


// UPIM_REQ_MODGUSER

// im2.0添加/修改用户的分组
struct modguser_req
{
	short   req;
	char	userid[S_USERID_LEN];			// id
	int		len;							// 包长
	char	headp[1];						// 内容json	
};

struct modguser_ans
{
	int		flag;							// 标志位
	int		rtnnum;							// 返回个数
	int		len;							// 包长
	char	headp[1];						// 内容json	
};


struct json_ans
{
	int		flag;							// 标志位
	int		rtnnum;							// 返回个数
	int		len;							// 包长
	char	headp[1];						// 内容json	
};

#pragma pack(pop,1)
#endif