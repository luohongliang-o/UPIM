#pragma once

#ifdef IM_EXPORT
#define IM_FUNC_PREFIX extern "C" _declspec(dllexport)
#else
#define IM_FUNC_PREFIX extern "C" _declspec(dllimport)
#endif

#define IMFLAG_CHILD	0X00000000L		// 子窗口
#define IMFLAG_INDEPENT 0X00000001L		// 独立窗口
#define IMFLAG_NOHQ		0X00000000L		// 没有行情交互
#define IMFLAG_HQ		0X00000010L		// 有行情交互

// 员工端登录模式
enum EIMLOGINMODE
{
	eIMLogin_Default	= 0,			// 默认,类似qq2010登录模式
	eIMLogin_Classic	= 1,			// 经典模式,弹出对话框输入用户名和密码,类似qq经典模式登录
	eIMLogin_NoCheckPwd = 2,			// 不校验密码,使用客户号登录,外部已经登录
	eIMLogin_ClientWeb	= 3,			// web客户端
};

#ifdef _DLL_MODE

// 初始化
IM_FUNC_PREFIX BOOL UEXP_Init(const char *PubDir, const char *PriDir, const char *CfgFile);

// 退出
IM_FUNC_PREFIX BOOL	UEXP_Uninit();

// 设置登录类型
IM_FUNC_PREFIX void UEXP_SetLoginType(long nUserType, HWND parent, long lFeedBackMsg, long nFlag = IMFLAG_INDEPENT|IMFLAG_NOHQ, long Reserved = 0);

// 登录
IM_FUNC_PREFIX BOOL UEXP_Login(long nMode, char Khh[51], char YYBInfo[51], char SSOCode[255], long Reserved = 0 );

// 重新组织窗口大小
IM_FUNC_PREFIX void UEXP_Resize();

// 更改聊天室
IM_FUNC_PREFIX void UEXP_ChangeChat(DWORD roomid);

// 显示主窗口
IM_FUNC_PREFIX void UEXP_ShowMainWnd(); 

// 消息传递
IM_FUNC_PREFIX BOOL	UEXP_PreTransMsg(MSG* pMsg);

// 行情交互数据
IM_FUNC_PREFIX BOOL UEXP_TransSpecialData(DWORD nGUID, long nReserved = 0);

#endif