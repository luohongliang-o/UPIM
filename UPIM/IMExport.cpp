#include "StdAfx.h"
#include "IMExport.h"
#include "globalvar.h"
#include "UPIM.h"
#include "LoadDllFunc.h"
#include "MainPort.h"
#include "MyThread.h"

#ifdef _DLL_MODE

// 调用IM窗口之前 需要INIT
BOOL UEXP_Init(const char *PubDir, const char *PriDir, const char *CfgFile)
{
	OutputDebugString("UEXP_Init begin\r\n");
	AFX_MANAGE_STATE(AfxGetStaticModuleState()); 
	//////////////////////////////////////////////////////////////////////////

	// 保存dll的全局资源，供使用，假如使用GetModuleHandle(NULL)，得到的是调用此DLL的exe的资源
	g_hInstance = AfxGetInstanceHandle();
	CPaintManagerUI::SetInstance(g_hInstance);
	CPaintManagerUI::SetResourcePath(CPaintManagerUI::GetInstancePath());

	LoadConfig();				// 加载配置文件

	//winxp中，在此处InitRichEditCom的话，在后面使用com组件，会报0x800401f0(CoInitialize has not been called)
	//InitRichEditCom();			// COM加载
	
	LoadDLLFunc();				// 加载DLL
	InitConnect();				// 连接服务器

	// 为了避免duilib中的界面库GetMessage获取主线程中的消息，开启新的线程来进行界面绘制
 	g_pCreateThread = AfxBeginThread(RUNTIME_CLASS(CMyThread), THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED, NULL);
 	g_pCreateThread->m_bAutoDelete = FALSE;
	g_pCreateThread->ResumeThread();

	return TRUE;
}

// 外部调用的
// 关闭
BOOL UEXP_Uninit()
{
	OutputDebugString("UEXP_Uninit begin\r\n");
	AFX_MANAGE_STATE(AfxGetStaticModuleState()); 
	//////////////////////////////////////////////////////////////////////////

	UnLoadDLLFunc();		// 卸载DLL
	UnInitRichEditCom();	// 卸载COM
	StopConnect();			// 断开连接

	if (g_TGChatDialog)
	{
		g_TGChatDialog->OnExit();
		g_TGChatDialog = NULL;
	}
	if (g_pCreateTG1)
	{
		g_pCreateTG1->OnExit();
		g_pCreateTG1 = NULL;
	}
	if (g_pCreateTG2)
	{
		g_pCreateTG2->OnExit();
		g_pCreateTG2 = NULL;
	}

	if (g_pFrame)
	{
		if (g_pFrame->pLoginDialog)
		{
			if (!g_pFrame->bLoginDialogClosed)
				g_pFrame->pLoginDialog->OnExit();
		}
		g_pFrame->OnExit();
		g_pFrame = NULL;
	}

	// 线程退出
	if((NULL != g_pCreateThread) && (NULL != g_pCreateThread->m_hThread))
	{
		BOOL bPost = ::PostThreadMessage(g_pCreateThread->m_nThreadID, WM_MYTHREAD_DELALL, 0, 0);
		if(::WaitForSingleObject(g_pCreateThread->m_hThread, 500) == WAIT_OBJECT_0)
		{
			delete g_pCreateThread;
			g_pCreateThread = NULL;
		}
// 		g_pCreateThread->PostThreadMessage(WM_QUIT, 0, 0);
// 		WaitForSingleObject(g_pCreateThread->m_hThread, INFINITE);
// 		delete g_pCreateThread;
// 		g_pCreateThread = NULL;
	}

	CPaintManagerUI::SetCloseFlag(true);
	CPaintManagerUI::Term();

	return TRUE;
}

// 设置登录参数
void UEXP_SetLoginType(long nUserType, HWND parent, long lFeedBackMsg, long nFlag /* = IMFLAG_INDEPENT|IMFLAG_NOHQ */, long Reserved /* = 0 */)
{
	OutputDebugString("UEXP_SetLoginType begin\r\n");
	AFX_MANAGE_STATE(AfxGetStaticModuleState()); 
	//////////////////////////////////////////////////////////////////////////
	USER_LOG("UEXP_SetLoginType %d,%p,%d,%d", nUserType, parent, lFeedBackMsg, nFlag);

	//g_nUserType:enum EUSERTYPE
	g_nUserType = nUserType;
	g_hVessel = parent;
	g_hFeedBackMsg = lFeedBackMsg;
	g_bIndepend = (nFlag & IMFLAG_INDEPENT) ? 1 : 0;
	g_nHQ = (nFlag & IMFLAG_HQ) ? 1 : 0;

	// 先绘制MIAN，并开始消息循环
 //	::PostThreadMessage(g_pCreateThread->m_nThreadID, WM_MYTHREAD_INITMAIN, 0, 0);
}

// 登录接口 
BOOL UEXP_Login(long nMode, char Khh[51], char YYBInfo[51], char SSOCode[255], long Reserved /* = 0 */)
{
	OutputDebugString("UEXP_Login begin\r\n");
	AFX_MANAGE_STATE(AfxGetStaticModuleState()); 
	
	//////////////////////////////////////////////////////////////////////////
	USER_LOG("UEXP_Login %d,%s,%s",nMode, Khh, YYBInfo);
	if(!g_bIndepend && !g_hVessel) return FALSE;

	strncpy(g_loginname, Khh, strlen(Khh));
	g_MyClient.m_strUserid = g_loginname;

	//g_nLoginMode:enum EIMLOGINMODE
	g_nLoginMode = nMode;

	// 通知线程绘制
	::PostThreadMessage(g_pCreateThread->m_nThreadID, WM_MYTHREAD_CREATE, 0, 0);

	return TRUE;
}

// 重新组织窗口大小
void UEXP_Resize()
{
	OutputDebugString("UEXP_Resize begin\r\n");
	AFX_MANAGE_STATE(AfxGetStaticModuleState()); 
	//////////////////////////////////////////////////////////////////////////
	if (g_UIMessageLoopStart == FALSE)
	{
		OutputDebugString("UEXP_Resize ERROR !!!!!!\r\n");
		return;
	}

	if (g_TGChatDialog)
	{
  		g_TGChatDialog->UISetInitSize();
		g_TGChatDialog->PostMessage(UM_USER_UISETINITSIZE, 0, 0);
	}

	return;
}

// 更改聊天室
void UEXP_ChangeChat(DWORD roomid)
{
	OutputDebugString("UEXP_ChangeChat begin\r\n");
	AFX_MANAGE_STATE(AfxGetStaticModuleState()); 
	//////////////////////////////////////////////////////////////////////////
	if (g_TGChatDialog)
		g_TGChatDialog->ChangeChatRoom(roomid);
	else
		g_dwRoomID = roomid;
}

#endif