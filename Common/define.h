#ifndef DEFINE_H_
#define DEFINE_H_

#pragma once
//////////////////////////////////////////////////////////////////////////
// UPIM

#define UPIM_SOFT_NAME              _T("UPIM 2015")         // 软件运行时的名字 
#define UPIM_REQ_COUNT              46
#define UPIM_REQ_BASE				5000					// 请求基准
#define UPIM_REQ_CHECKPWD			UPIM_REQ_BASE+1			// 用户登录 100
#define UPIM_REQ_GUSER				UPIM_REQ_BASE+2			// 服务人员获取分组好友(我的客户/部门) 5830
#define UPIM_REQ_GUSER2				UPIM_REQ_BASE+3			// 服务人员获取分组好友(我的客户/部门)_兼容模式Json
#define UPIM_REQ_LIST				UPIM_REQ_BASE+4			// 客户得到服务人员列表 5720
#define UPIM_REQ_LIST2				UPIM_REQ_BASE+5			// 客户得到服务人员列表_兼容模式Json
#define UPIM_REQ_DEPT				UPIM_REQ_BASE+6			// 服务人员获取部门列表 5718
#define UPIM_REQ_GCINFO				UPIM_REQ_BASE+7			// 得到固定聊天室 5782
#define UPIM_REQ_GCINFO2			UPIM_REQ_BASE+8			// 得到固定聊天室_兼容模式Json
#define UPIM_REQ_GLIST				UPIM_REQ_BASE+9			// 客户得到服务分组列表 5728
#define UPIM_REQ_SENDMSG			UPIM_REQ_BASE+10		// 消息发送(客户发送消息到服务器-send) 5734
#define UPIM_REQ_S_SENDMSG			UPIM_REQ_BASE+11		// 服务器消息(服务器推送来消息-recive) 5736
#define UPIM_REQ_GCGETLIST			UPIM_REQ_BASE+12		// 群组聊天,得到联系人列表 5788
#define UPIM_REQ_GCGETLIST2			UPIM_REQ_BASE+13		// 群组聊天,得到联系人列表_兼容模式Json
#define UPIM_REQ_GCSENDMSG			UPIM_REQ_BASE+14		// 群组发送消息 5796
#define UPIM_REQ_GCRECVMSG			UPIM_REQ_BASE+15		// 群组接收消息 5798
#define UPIM_REQ_GCCREATE			UPIM_REQ_BASE+16		// 群组聊天,创建 5784
#define UPIM_REQ_GCADD				UPIM_REQ_BASE+17		// 群组聊天,添加成员 5786
#define UPIM_REQ_GCKICK				UPIM_REQ_BASE+18		// 群组聊天,踢人 5794
#define UPIM_REQ_GCLEAVE			UPIM_REQ_BASE+19		// 群组聊天,离开 5790
#define UPIM_REQ_GCANNOUNCE			UPIM_REQ_BASE+20		// 群公告发布
#define UPIM_REQ_M_USERSTATE		UPIM_REQ_BASE+21		// 更改用户状态
#define UPIM_REQ_GPUBLIC            UPIM_REQ_BASE+22		// 获取公众号
#define UPIM_REQ_PUBLICINFO			UPIM_REQ_BASE+23		// 获取公众号信息，如果是问题路由，则在主界面显示
#define UPIM_REQ_PUBCONCLOSE		UPIM_REQ_BASE+24		// 客户跟此公众号断开
#define UPIM_REQ_CONFIG				UPIM_REQ_BASE+25		// 从服务器获取UPIM关键配置开关
#define UPIM_REQ_USERINFO			UPIM_REQ_BASE+26		// 得到用户资料
#define UPIM_REQ_M_USERINFO			UPIM_REQ_BASE+27		// 更改用户资料
#define UPIM_REQ_RECONNECT			UPIM_REQ_BASE+28		// 用户重连接，需要发包通知服务器
#define UPIM_REQ_S_GCADD			UPIM_REQ_BASE+29		// 群组聊天,添加成员后通知此用户(在线)
#define UPIM_REQ_S_GCKICK			UPIM_REQ_BASE+30		// 群组聊天,踢人之后通知此用户(在线)
#define UPIM_REQ_OLNOTIFY			UPIM_REQ_BASE+31		// 用户在线通知 5754
#define UPIM_REQ_ONLINE				UPIM_REQ_BASE+32		// 查询联系人状态 5756
#define UPIM_REQ_S_ONLINE			UPIM_REQ_BASE+33		// 用户上线状态的服务器推送 5758
#define UPIM_REQ_OFFMSG				UPIM_REQ_BASE+34		// 离线消息 5748
#define UPIM_REQ_KICKOFF			UPIM_REQ_BASE+35		// 同一ID异地登录之后，服务器向前一个ID发消息，表示被踢
#define UPIM_REQ_SVROFF				UPIM_REQ_BASE+36		// 服务器异常关闭，向每一个在线ID发消息，通知客户重连
#define UPIM_REQ_PKG				UPIM_REQ_BASE+37		// 发送包
#define UPIM_REQ_S_PKG				UPIM_REQ_BASE+38		// 服务器中转包(服务器推)
#define UPIM_REQ_CHECK_USERSTATE    UPIM_REQ_BASE+39		// 服务器发送客户端确认其状态(服务器推)
#define UPIM_REQ_ALLTGROOM			UPIM_REQ_BASE+40		// 取得所有的投顾聊天室
#define UPIM_REQ_ALLTGROOMEX		UPIM_REQ_BASE+41		// im2.0取得所有的房间号对应的分析师列表
#define UPIM_REQ_ALLROOMTOUSER		UPIM_REQ_BASE+42		// im2.0取得所有的房间号对应的用户列表
#define UPIM_REQ_GETGROUP			UPIM_REQ_BASE+43		// im2.0取得所有的自定义分组信息
#define UPIM_REQ_GETGUSER			UPIM_REQ_BASE+44		// im2.0取得分组下对应的用户列表
#define UPIM_REQ_ADDGROUP			UPIM_REQ_BASE+45		// im2.0添加/修改自定义分组信息
#define UPIM_REQ_MODGUSER			UPIM_REQ_BASE+46		// im2.0添加/修改用户的分组

//////////////////////////////////////////////////////////////////////////
// UM_MSG
#define UM_USER_LOGIN				WM_USER + 1				// 用户登录成功
#define UM_USER_CLOSE				WM_USER + 2				// 主界面关闭
#define UM_USER_ADDGROUP			WM_USER + 3				// 添加组
#define UM_USER_SETSELFINFO			WM_USER + 4				// 显示个人信息
#define UM_USER_ADDFRIEND			WM_USER + 5				// 添加好友列表
#define UM_USER_OPENWND				WM_USER + 6				// 打开聊天窗口
#define UM_USER_ADDDEPT				WM_USER + 7				// 添加部门
#define	UM_SOUND_PLAY				WM_USER + 8				// 播放声音消息
#define UM_USER_ADDROOM				WM_USER + 9				// 添加聊天室/群/讨论组
#define	UM_USER_ADDROOMMEMBER		WM_USER + 10			// 为聊天室添加成员
#define	UM_USER_LEAVEROOM			WM_USER + 11			// 退出聊天室
#define	UM_USER_KICKOK				WM_USER + 12			// 踢人成功
#define	UM_USER_ADDPUBLIC			WM_USER + 13			// 添加公众号
#define UM_USER_OFFLINE				WM_USER + 14			// 用户离线
#define UM_USER_ONLINE				WM_USER + 15			// 用户上线
#define UM_USER_RECVMSG				WM_USER + 16			// 打开的窗口收到消息
#define	UM_USER_PUBLICINFO			WM_USER + 17			// 收到后台传来的公众号信息
#define UM_USER_USERSTATE			WM_USER + 18			// 用户状态改变
#define	UM_USER_ADDTGROOM			WM_USER + 19			// 添加投顾端房间列表
#define	UM_USER_STATECHANGE			WM_USER + 20			// 好友状态改变
#define UM_USER_HISMSG				WM_USER + 21			// 打开的窗口收到消息
#define UM_USER_HISANDRECVMSG		WM_USER + 22			// 打开的窗口收到消息
#define UM_USER_SHOWRECORD			WM_USER + 23			// 历史消息
#define UM_USER_UISETINITSIZE		WM_USER + 24			// 改变UI大小
#define	UM_USER_ADDTGROOM2			WM_USER + 25			// im2.0添加投顾端房间列表
#define	UM_USER_ADDROOMCLIENT		WM_USER + 26			// im2.0添加投顾端房间下面的客户
#define	UM_USER_ADDROOMANALYS		WM_USER + 27			// im2.0添加客户端房间下面的投顾
#define	UM_USER_GETUSERPIC			WM_USER + 28			// im2.0服务器得到用户头像
#define	UM_USER_ADDSELFGROUP		WM_USER + 29			// im2.0加载自定义分组
#define	UM_USER_GETSELFPIC			WM_USER + 30			// im2.0服务器得到自己的头像
#define	UM_USER_OPENRIGHTS			WM_USER + 31			// im2.0打开开通权限窗口
#define UM_USER_RIGHTHISMSG			WM_USER + 32			// im2.0右侧窗口填充历史消息
#define UM_USER_ADDHISTORYCLENT     WM_USER + 33            // im2.0加载最近联系人 
#define WM_USER_FINDFRIEND          WM_USER + 34            // im2.0 查找联系人  wuchao add at 2016-04-05 
#define	UM_USER_ADDNEWPUBLIC		WM_USER + 35			// im3.0 新IM系统添加公众号
#define UM_USER_ADDNEWLASTUSER      WM_USER + 36
#define WM_IM_USERID                WM_USER + 4317          // up投资卫士终端  点击分析师头像  打开与分析师对话框  

#define UM_UI_MESSAGELOOP			WM_USER + 100			// duilib绘图
#define UM_UI_TERM					WM_USER + 101			// duilib关闭绘图

#define	UM_FEEDBACKMSG				WM_USER + 0x987			// 关闭截图消息
#define	UM_IMFEEDBACKMSG			WM_USER + 0x988			// 外部调用消息
// WM_MSG
#define WM_NOTIFYICON				WM_USER + 1009			// 托盘图标消息
#define WM_MENUCLICK				WM_USER + 1010			// 用来接收按钮单击的消息

#define WM_CHOOSEDATETIMEINHISTORYRECORD WM_USER + 1011      // 按照日期筛选聊天记录消息 


//////////////////////////////////////////////////////////////////////////
#define NOW_CVERSION		1.107			// 客户端版本号
#define NOW_VERSION			12				// 协议版本号
#define SEPARATOR_NUM		7654321
#define	VER_STARTYEAR		2015			// BuildNo起始计算年份
#define GROUPNAME_SIZE		31				// 分组名称大小
#define COMM_GROUP_COUNT	5				// 一般分组个数(普通版本,类似qq好友分组)
#define REQBUFFER_LEN		4*1024			// 4K,请求包缓冲区大小(20150116修改)
#define ANSBUFFER_LEN		32*1024			// 32K,应答包缓冲区大小
#define PROXYBUFFER_LEN		4*1024			// 4K,代理缓冲区长度
#define RICHEDIT_MAXLEN		1024			// 输入区最大长度1024

#define MAX_BUF_SEND_LEN	8*1024			// 发送包最大缓存
#define MIN_BUF_SEND_LEN	4*1024			// 发送包最小缓存

#define MAX_LINK_NUM		30				// 数据库连接池最大连接数
#define MAX_HOSTCOUNT		10				// 服务器最大个数
#define MAX_DBCOUNT			5				// 数据库最大个数
#define MAX_IPWHITELIST     256             // IP白名单
#define	MAX_SERVER_NUM		20				// 后台最大数量
#define MAX_MSG_LEN			2*1024			// 广播消息最大长度
#define MAX_EMP_NUM			20000			// 员工最多个数
#define ONESECOND			1000			// 1秒的毫秒数，用于简化显示
#define MAX_PIC_PIX			800				// 传送图片的最大像素
#define MAX_PIC_SIZE		800*1024		// 传送图片的最大大小

#define GCNT_MAX_COUNT		100				// 聊天记录条数
#define USER_MAX_ROOM_COUNT 100				// 每个用户所拥有的最大群组数
#define MAX_ROOM_COUNT		100000			// 总的群组数
#define REFRESH_GUSER_COUNT	50				// 一次刷新群组用户个数
#define REFRESH_TGUSER_COUNT 100			// 一次刷新投顾用户个数
#define REFRESH_GROOM_COUNT	10				// 一次刷新群组个数
#define GPUBLIC_COUNT	    10				// 一次获取公众号个数

#define S_USERID_LEN		32
#define S_NICKNAME_LEN		32
#define S_USERNAME_LEN		32
#define S_USERPWD_LEN		48
#define S_MOBILE_LEN		16
#define S_EMAIL_LEN			16
#define S_SIGNATURE_LEN		128
#define S_FUNCCTRL_LEN		64
#define S_YYB_LEN			12
#define S_GROUPGG_LEN		128
#define S_ERRSTR_LEN		64
#define S_IDVEC_LEN			64

#define USERID_SIZE			32				// USERID大小
#define GUID_SIZE			16				// GUID长度
#define SEND_MAXCOUNT		3				// 最大发送次数
#define SEND_TIMEOUT		30				// 发送超时
#define MAX_TEXT_LEN		2*1024			// 2KB,发送文本的最大长度(在线消息文本)
#define COMM_TEXT_LEN		400				// 普通消息长度
#define GROUPNAME_SIZE		31				// 分组名称大小
#define ROOMNAME_SIZE		31				// 群组名称大小
#define NOTE_SIZE			101				// 群公告
#define TG_LAOSHINUM		10				// 分析师互动里面，单个聊天室最多老师数
#define TG_RECORD_NUM		20				// 投顾端打开的时候最多看见50条历史消息
#define TG_DISAPLY_NUM      20              // 聊天记录中每页显示的条数 
#define IMG_NAME_SIZE		360				// 用户头像文件名长度
#define NODE_NAME_SIZE		64				// 用户节点名长度
#define LAST_WORD_SIZE		128				// 最近聊天记录展示最大长度
#define USERMRG_FENZU		10				// IM2.0 客户管理器 最大分组个数

#define MAX_STATE			10000			// 状态队列
#define MAX_GMSG			1//100			// 分组消息队列
#define MAX_PMSG			10//1000		// 个人消息队列
#define MAX_RMSG			500				// 群组消息
#define MAX_HISMSG			1500			// 历史消息队列
#define MAX_OFFMSG			500				// 离线消息队列
#define MAX_PRDTMSG			500				// 产品消息
#define MAX_ROOMOFFMSG		500             // 群组离线消息队列
#define SHAKE_TIME_SPAN		25				// 窗口抖动sleep毫秒
#define WLEN_TO_PIX			8				// 字符数转像素长度

//////////////////////////////////////////////////////////////////////////
#define AP_SIZE				64				// 64字节的AP信息
#define UORG_DEFID			0				// 用户机构默认ID
#define UORG_SIZE			41				// 用户机构最大长度
#define UID_SIZE			41				// 用户帐号最大长度
#define UNAME_SIZE			41				// 用户名称最大长度
#define UID_GSIZE			13				// 用户帐号最大长度

#define	STR_SERVICE			_T("我的服务")
#define	STR_CLIENT			_T("我的客户")
#define	STR_PRODUCT			_T("我的产品")
#define	STR_FRIEND			_T("我的好友")
#define	STR_STRANGER		_T("陌生人")
#define	STR_BLACKLIST		_T("黑名单")
#define	STR_NOTIFY			_T("通知消息")
#define	STR_PUBLIC			_T("公众号")
#define	STR_DEFAULT_FONT	_T("微软雅黑")
#define STR_NOCHECKPWD		_T("UPouter@nnn027")// 外部登录标志,不校验密码
#define DEFAULT_NOTE		_T("暂时没有发布公告")

#define UPIM_CLIENT_MUTEX   "UP_IM_MUTEX"
typedef long	IMGROUPID;			// 用户分组id
typedef long	ROOMID;				// 群组聊天室id
typedef int		PUBLICID;			// 公众号id
typedef int		TGROOMID;			// 投顾房间id
typedef LPSTR	LPUSERID;
typedef LPCSTR	LPCUSERID;

const int	MAX_SEND_BUF_LEN = 6*1024;		// 发送最大缓冲区
const int	REFRESH_USER_COUNT = 50;		// 一次刷新用户在线状态的用户个数
const int	USER_SIZE = REFRESH_USER_COUNT*(USERID_SIZE+1)+100;	// 用户长度
const int	GROUPUSER_COUNT = 500;			// 群组最大人数
const int	DATA_TRANSFER_UNIT = 3*1024;	// 图片或文件传送单元
const int	IMAGE_TIMEOUT = 50;				// 图片传输超时(秒)

// 全局错误类型
enum UPIM_ERRORCODE
{
	erError_None		= 0,								// 无错误
	//SERVER
	erServer_Error		= UPIM_REQ_BASE*100 + 1,			// 服务器异常
	erServer_DBError	= UPIM_REQ_BASE*100 + 2,			// 数据库异常
	//UPIM_REQ_CHECKPWD
	erLogin_IDNotExist	= UPIM_REQ_CHECKPWD*100 + 2,		// 用户名不存在
	erLogin_PswError	= UPIM_REQ_CHECKPWD*100 + 3,		// 密码不正确
	erLogin_IsLogined	= UPIM_REQ_CHECKPWD*100 + 4,		// 用户已经登录
	erLogin_IPNotAllow	= UPIM_REQ_CHECKPWD*100 + 5,		// 此IP不允许登录
	//UPIM_REQ_S_SENDMSG
	erLogin_SendMsgE	= UPIM_REQ_SENDMSG*100 + 1,			// 往服务器发送消息失败
	//UPIM_REQ_S_SENDMSG
	erLogin_S_SendMsgE	= UPIM_REQ_S_SENDMSG*100 + 1,		// 服务器推送消息失败
	//UPIM_REQ_RECONNECT
	erLogin_RC_IDNULL	= UPIM_REQ_RECONNECT*100 + 1,		// ID为空
	erLogin_RC_IPNotAllow	= UPIM_REQ_RECONNECT*100 + 2,	// IP不允许登录
};

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
// 枚举
enum EUSERSTATE
{
	eUSER_ONLINE = 0,
	eUSER_OFFLINE,
	eUSER_LIKAI,
};

// 定时器
enum ETIMER
{
	T_FLASHICON = 10,				// 图标闪烁
	T_DOCK,							// 窗口停靠
	T_OFFMSG,						// 离线消息
	T_REFRESHLIST,					// 刷新联系人列表
	T_REFRESHSTATUS,				// 刷新在线状态
	T_REFRESHTRANSFER,				// 刷新文件传输
	T_QUERYTIMEOUT,					// 查询超时
	T_FILTERWORDS,					// 过滤关键字
	T_MSG_TIMEOUT,					// 消息超时
	T_FILESVR_CONN,					// 文件服务器

	T_FLASHITEM,					// 好友图标闪烁
	T_SETSELFINFO,					// 自我信息更新
	T_USERSTATUS,					// 用户状态
	T_CHATBOX,						// 聊天窗口计时
	T_REFRESH,						// 聊天窗口图片刷新
	T_NOTIFYICON,					// 托盘图标闪烁
	T_CONNECT,						// 分析师，连接检测
	T_RECORDSHOW,					// 客户，历史消息延时

	T_TRAY_HOVER_TIMER_ID ,         // 鼠标悬停在托盘上面      
	T_TRAY_LEAVE_TIMER_ID ,         // 鼠标离开托盘

	T_SHOW_MSGDIALOG,               // 鼠标离开托盘 鼠标处于消息提示框之内时，一直显示提示框 
};

// 定时器间隔
enum ETIMER_ELAPSE
{
	eTE_FlashIcon		= 500,
	eTE_Dock			= 100,
	eTE_OffMsg			= 2500,
	eTE_RefreshStatus	= 20*1000,
	eTE_RefreshTransfer	= 1000,
	eTE_QueryTimeout	= 3000,
	eTE_MsgTimeout		= 5000,
	eTE_FileSvr			= 2*60*1000,

	eTE_FlashItem		= 150,
	eTE_Setselfinfo		= 1000,
	eTE_UserStatus		= 1000,
	eTE_Chatbox			= 500,
	eTE_Refresh			= 1500,
	eTE_NotifyIcon		= 500,
	eTE_Connect			= 20*1000,
	eTE_RecordShow		= 300,
};

enum EVTTYPE
{
	eVT_Friend = 0,					// 好友集合
	eVT_Group,						// 群组集合
	eVT_Recent,						// 最近联系人集合
};

enum EUITYPE
{
	eUI_Group = 0,					// Group
	eUI_Member = 1,					// Member
	eUI_Recent=2,					// Recent
};

enum NICKTYPE
{
	eNICK_nameandid=0,				// 昵称(ID)
	eNICK_id = 1,					// ID
	eNICK_name = 2,					// 昵称	
};

enum EDIALOGTYPE
{
	eDIALOG_Single = 0,				// 单人聊天
	eDIALOG_Group = 1,				// 群组聊天
	eDIALOG_Team = 2,				// 讨论组
	eDIALOG_Public = 3,				// 公众号
	eDIALOG_GroupSend = 4,			// 消息群发
	eDIALOG_ChatTG = 5,				// 投顾端-客户聊天界面
	eDIALOG_Analys = 6,				// 投顾端-分析师聊天界面
	eDIALOG_RYPublic = 7,				// 新IM系统-客户聊天界面
};

enum EMSGOPENTYPE
{
	eMSG_Single = 0,				// 单人聊天
	eMSG_Group = 1,					// 群组聊天
};

enum NEWIMCATEGORYID
{
	newMSG_PRIVATE  = 1    ,   // 单人
	newMSG_DISCUSSION      ,   // 讨论组
	newMSG_GROUP           ,   // 群组
	newMSG_CHATROOM        ,   // 聊天室
	newMSG_CUSTOMERSERVICE ,   // 客服
	newMSG_SYSTEM          ,   // 系统消息
	newMSG_MC              ,   // MC消息应用公众服务
	newMSG_MP              ,   // MP消息公众服务平台
};
enum NEWIMTRANSFERTYPE
{
	newMSG_S = 1,  // 消息数据服务器不缓存，不保证信息到达接收方
	newMSG_N    ,  // 消息数据服务器缓存，交互方会收到通知消息
	newMSG_P    ,  // 消息数据服务器缓存，交互方不在线会收到PUSH消息

};
// 枚举
enum ECREATEDIALOGTYPE
{
	eCreate_LoginIn = 0,			// 创建登录框
	eCreate_Group,					// 创建群组
	eCreate_Team,					// 创建讨论组
	eCreate_Node,					// 创建分组
	eCreate_Public,					// 创建公众号
	eCreate_TG1,					// TG1
	eCreate_TG2,					// TG2
};

// 登录类型
enum ELOGINTYPE
{
	eLogin_Emp = 0,					// 员工登录
	eLogin_Client = 1,				// 客户登录
	eLogin_EmpSpecial=2,			// 员工特殊登录(不校验密码)
	eLogin_ClientWeb = 3,			// 客户web登录
	eLogin_ClientNeedPwd=4,			// 非行情交互版本登录，需要输入密码进行校验 
	eLogin_Comm = 8,				// 普通登录
	eLogin_FileSvr = 10,			// 文件服务器
	eLogin_UPClient = 11,			// up客户登录,不校验密码
	eLogin_Analyst = 12,			// 分析师登录
	eLogin_KF = 13,					// 融云新客户端登陆
};

// 用户类型
enum EUSERTYPE
{
	eUser_All=-1,
	eUser_Client=0,					// 客户
	eUser_Broker=1,					// 投顾
	eUser_Agent=2,					// 坐席

	eUser_UPClient=3,				// 嵌在客户端中的投顾界面
	eUser_UPAnalyst=4,				// 分析师登录

	eUser_UPClient2=5,				// im2.0独立出来的用户界面
	eUser_UPAnalyst2=6,				// im2.0分析师登录

	eUser_Product=7,				// 产品
	eUser_Comm=8,
	eUser_UPExe = 9 ,               // im2.0 给UP投资卫视使用  提供EXE格式  不显示托盘图标 
	eUser_Unknow=10,
	eUser_UPRYExe = 11 ,               // im3.0 给UP  提供EXE格式  融云整合
};


//服务器类型
enum enumQuoterType
{
	Quoter_NULL=0,
	Quoter_TP=100,					//如果模块消息服务器下被引用，该服务提供本机的用户连接信息，上传到远端的状态服务器
	Quoter_TS=101,					//如果模块消息服务器下被引用，该服务提供完整的用户连接信息，数据从远端的状态服务器
};

// 用户状态
enum MYSTATE 
{
	MYSTATE_OFFLINE,				// 离线
	MYSTATE_NORMAL ,				// 在线
	MYSTATE_HIDE,					// 隐身
	MYSTATE_LEAVE,					// 离开
};

// 分组类型
enum EGROUPTYPE
{
	eList_Friend		= 0,		// 一般好友分组(通用版本)

	eList_TopGroup		= 1,		// 置顶组(客户版本)
	eList_YybGroup		= 2,		// 营业部分组(客户版本)
	eList_SrvGroup		= 3,		// 服务分组(客户版本的服务分组)

	eList_Dept			= 4,		// 部门员工分组(服务版本有此组,此组好友都是员工)
	eList_ClientDefGrp	= 5,		// 客户自定义组(服务版本有此组,此组好友都是客户)
	eList_ClientGrp		= 6,		// 客户分组(服务版本：直接从对应关系表取，二维表,此组好友都是客户)
	eList_ClientDefGrp2 = 9,		// 客户自定义组(服务版本：直接从对应关系表取，二维表,此组好友都是客户,relation2字段显示客户的分组ID)

	eList_ProductGrp	= 7,		// 产品分组
	eList_Agent			= 8,		// 坐席版本，取坐席好友
	eList_Public		= 10,		// 公众号
};

// 分组ID
enum GROUP_INDEX 
{
	GROUP_FRIEND = 1,				// 我的好友
	GROUP_STRANGER = 2,				// 陌生人
	GROUP_BLACKLIST = 3,			// 黑名单
	GROUPID_BEGIN = 3,
	GROUPID_END = GROUPID_BEGIN+COMM_GROUP_COUNT,

	GROUPID_CLIENT = 9,				// 我的客户 9
	GROUPID_SERVICE = 10,			// 我的服务
	GROUPID_AGENT = 11,				// 坐席分组
	GROUPID_PRODUCT = 20,			// 我的产品 20
};


// 群组类型
enum E_GROUPCHAT_TYPE
{
	eCR_None = 0,					// 非群组类型,个人
	eCR_Group = 1,					// 内部群组(员工群组),和QQ群组一样
	eCR_Group_Client = 2,			// 外部群组(客户群组)
};

// 消息发送类型
enum E_MSGSEND_TYPE
{
	eMsgSend_Normal=0,				// 普通发送
	eMsgSend_Shift=1,				// 消息转移( A-->B,B(A)-->C ,A向B发送消息，B将A的消息转给C)
	eMsgSend_Public=2,				// 公众号消息
	eMsgSend_Prdt=7,				// 产品消息
	eMsgSend_PrdtFile=8,			// 产品文件
	eMsgSend_Analyst=9,				// 分析师消息，与房间相关
	eMsgSend_NewPublic=10
};

//通知消息的类型
enum ENOTIFY_TYPE
{
	eMsg_System_Notify=0,			// 系统通知
	eMsg_PDb,						// 个人数据库消息
	eMsg_GDb,						// 组数据库消息
};

// 消息文本格式
enum EMSGTEXTFORMAT
{
	eMsgFormat_Txt=0,				// 纯文本txt
	eMsgFormat_Def=2,				// 自定义格式(自定义xml)
	eMsgFormat_File=3,				// 文件消息
	eMsgFormat_DbMsg=4,				// 数据库消息
	eMsgFormat_OutRoom=5,			// 外部群组消息
	eMsgFormat_System=6,			// 系统消息(显示在个人聊天对话框里面)
	eMsgFormat_Shake=7,				// 窗口抖动
	eMsgFormat_PrdtFile=8,			// 产品文件
	eMsgFormat_OffimageReceipt=9,   // 贴图回执(用于放入离线消息库,使得能够查看离线贴图)
	eMsgFormat_Prdt=10,				// 产品消息
	eMsgFormat_AutoReply=11,        // 离开状态的自动回复
	eMsgFormat_FileBtSend=12,		// 产品经理群发产品文件		
};

enum E_CHATRECORD_TYPE
{
	eRecord_Single=0,				// 个人消息
	eRecord_Group,					// 群组消息
	eRecord_Sys,					// 系统消息
	eRecord_GS,						// 公式
	eRecord_BM,						// 版面
	eRecord_GPC,					// 股票池
	eRecord_JYXH,					// 交易信号
	eRecord_Public,					// 公众号消息
	eRecord_TgRoom,					// 投顾聊天室消息
	eRecord_NewPublic,					// 公众号消息
};

// 面板类型
enum EPANELTYPE
{
	ePanel_Single=0,				// 客户
	ePanel_Group=1,					// 群组
	ePanel_Recent=2,				// 最近联系人
};

// 节点类型
enum ENODETYPE
{
	eNode_Node=0,					// 节点
	eNode_None,						// 非节点

	eNode_Single,					// 个人
	eNode_Group,					// 群组
	eNode_Recent,					// 最近联系人
	eNode_Public,					// 公众号
	eNode_Analyst,					// 分析师房间
	eNode_RYPublic,					// 融云公众号
};

// 节点类型
enum EADDIDTYPE
{
	eAdd_No=0,						// 
	eAdd_Exist,						// 已存在的
	eAdd_New,						// 新加入的
};

enum EMSGDATAERROR
{
	eMsgData_Suc = 0,		
	eMsgData_Null = 1,				// 为空
	eMsgData_TextMax = 2,			// 文本太长
	eMsgData_ImgMax = 3,			// 图片数量过多
	eMsgData_Xml=4,					// 生成xml出错
	eMsgData_OfflineImg=5,			// 离线不能发送图片
};


enum E_GROUPCHAT_ERROR
{
	eGCError_NoDefine = -9999,		// 未定义的错误类型
	eGCError_Success = 0,			// 成功
	eGCError_DbCon = -1,			// 没有可用的数据库连接	 
	eGCError_DbExe = -2,			// 数据库执行失败
	eGCError_DbOp = -3,				// 数据库操作失败
	eGCError_RoomType = -4,			// 无此群组类型
	eGCError_UpUserMax = -5,		// 群人数已满
	eGCError_Delete = -6,			// 该群已被删除
	eGCError_RsClose = -7,			// 记录集已关闭
	eGCError_RsNULL = -8,			// 记录集为空
	eGCError_DbClose = -9           // 数据库关闭
};


enum  UPIM_PROTOCOL
{
	UPIM_FILE_TRANSFER = 2,			// 客户请求发送文件
	UPIM_FILE_TRANSFER_DATA = 3,	// 传送文件内容
	UPIM_FILE_TRANSFER_CANCEL = 4,	// 取消文件传送
	UPIM_FILE_QUERY = 5,			// 查询传输情况

	UPIM_MSG_PKG_TRANSFER = 9,		// 复杂消息传送协议(传送图像)
	UPIM_TRADE_TRANS = 10,			// 交易查询请求(经纪人查询客户持仓,自选股等)
	UPIM_TRADE_TRANS_DATA = 11,		// 交易查询数据
	UPIM_TRADE_REJECT = 13,			// 交易查询拒绝

	UPIM_BROKER_SPEC_DATA = 12,		// 经纪人特殊数据传递(经纪人向客户发送股票池,交易计划等)

	//个人形象相关协议
	UPIM_PICTURE_CHANGE = 30,		// 通知联系人改变了个人形象
	UPIM_CHECK_PICTURE = 31,		// 查询联系人的个人形象是否更新了
	UPIM_SEND_MYPICTURE = 32,		// 发送我的个人形象给联系人

	//////////////////////////////////////////////////////////////////////////
	// 直接与服务器交互
	UPIM_OFFFILE = 61,				// 离线文件请求
	UPIM_OFFFILE_DATA = 62,			// 离线文件数据请求
	UPIM_OFFFILE_CANCEL = 63,			// 离线文件取消
	UPIM_OFFFILE_DEL = 64,			// 离线文件删除
};

#endif