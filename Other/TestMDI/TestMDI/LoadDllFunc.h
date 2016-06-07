#pragma once

#define IMFLAG_CHILD	0X00000000L		// 子窗口
#define IMFLAG_INDEPENT 0X00000001L		// 独立窗口
#define IMFLAG_NOHQ		0X00000000L		// 没有行情交互
#define IMFLAG_HQ		0X00000010L		// 有行情交互(被TdxW调用)

#define	UM_IMFEEDBACKMSG			WM_USER + 0x988			// 外部调用消息
// 登陆模式
enum EIMLOGINMODE
{
	eIMLogin_Default	= 0,			// 默认,类似qq2010登录模式
	eIMLogin_Classic	= 1,			// 经典模式,弹出对话框输入用户名和密码,类似qq经典模式登录
	eIMLogin_NoCheckPwd = 2,			// 不校验密码,使用客户号登录,外部已经登录
	eIMLogin_ClientWeb	= 3,			// web客户端
};

typedef BOOL (*UEXP_Init)(const char *PubDir, const char *PriDir, const char *CfgFile);
typedef BOOL (*UEXP_SetLoginType)(long nUserType, HWND parent, long lFeedBackMsg, long nFlag , long Reserved);
typedef BOOL (*UEXP_Login)(long nMode, char Khh[51], char YYBInfo[51], char SSOCode[255], long Reserved);
typedef BOOL (*UEXP_Resize)();
typedef BOOL (*UEXP_Uninit)();
typedef BOOL (*UEXP_ChangeChat)(DWORD roomid);

extern CString g_strHomePath;
typedef struct tagIMDLL
{
	HMODULE					m_hModule;
	UEXP_Init				m_pUEXP_Init;
	UEXP_SetLoginType		m_pUEXP_SetLoginType;
	UEXP_Login				m_pUEXP_Login;
	UEXP_Resize				m_pUEXP_Resize;
	UEXP_Uninit				m_pUEXP_Uninit;
	UEXP_ChangeChat			m_pUEXP_ChangeChat;
}IMDLL;
extern IMDLL g_infoDll;

extern BOOL LoadDLLFunc();
extern void UnLoadDLLFunc();