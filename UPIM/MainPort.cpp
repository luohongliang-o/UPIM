#include "StdAfx.h"
#include "MyClient.h"
#include "UI/MainFrame.h"
#include "globalvar.h"
#include "Func.h"
#include "MainPort.h"
#include "UI/Utils.h"
#include "LoadDllFunc.h"
#include "ProcessAns.h"


//////////////////////////////////////////////////////////////////////////



// 123 obaby
//D68r3cXNJdPlfh+M6ZPP2JT7jnHE1PkWabGAI6ucwXMdSkCkEROlK+uzUIJg7eldTQWTUC7Cz3QquJIcbJf2eA==

// 123456 lhl
//SZyqhQM1O/w2QWCZhVwD+ZT7jnHE1PkWabGAI6ucwXMdSkCkEROlK2fa/qIUfCWE7ZMKLlfpXe67XJ7NU3FRPQ==
// 初始连接
VOID InitConnect()
{
#ifdef VER_UPIM_RONGYUN
	CString strUserFolder = "";
	strUserFolder.Format("%s\\UPIMUser\\", g_config.szHomePath);
	CreateDirectory(strUserFolder, NULL);
	CHAR szUserDir[MAX_PATH] = {0};
	nsprintf(szUserDir, sizeof(g_config.szUserDir), "%s\\UPIMUser", g_config.szHomePath);
	WCHAR * IMPath = Utf8ToUnicode(szUserDir);
	g_RongCloudDll.m_pInitClient("pgyu6atqyliru", "IM.WINFORM", "deviceId",IMPath,IMPath); //vnroth0krxiwo
	delete IMPath;
	g_RongCloudDll.m_pSetDeviceInfo("Apple", "iPhone 6 Simulator", "8.1.1", "WIFI", "");
	//char szToken[] = {"SZyqhQM1O/w2QWCZhVwD+ZT7jnHE1PkWabGAI6ucwXMdSkCkEROlK2fa/qIUfCWE7ZMKLlfpXe67XJ7NU3FRPQ==" };
	g_RongCloudDll.m_pConnect(g_strToken, (ConnectAckListenerCallback)&connectCallback,false);//g_strToken

	g_RongCloudDll.m_pRegisterMessageType("RC:TxtMsg", 3);
	//图片消息
	g_RongCloudDll.m_pRegisterMessageType("RC:ImgMsg", 3);
	//图文消息
	g_RongCloudDll.m_pRegisterMessageType("RC:VcMsg", 3);
	//位置消息
	g_RongCloudDll.m_pRegisterMessageType("RC:LBSMsg", 3);
	//添加联系人消息
	g_RongCloudDll.m_pRegisterMessageType("RC:ContactNtf", 3);
	//提示条（小灰条）通知消息
	g_RongCloudDll.m_pRegisterMessageType("RC:InfoNtf", 3);
	//资料通知消息
	g_RongCloudDll.m_pRegisterMessageType("RC:ProfileNtf", 3);
	//通用命令通知消息
	g_RongCloudDll.m_pRegisterMessageType("RC:CmdNtf", 3);

	//设置消息监听
	g_RongCloudDll.m_pSetMessageListener(message_callback);

	//设置网络异常监听
	g_RongCloudDll.m_pSetExceptionListener(exception_callback);

#else
	if (g_MyClient.m_bConnect)			//如果已连接，跳过
		return;
	g_MyClient.InitClient();
	int nMainSvr = g_config.nMainHost;
	CString strAddress = g_config.aHostCfg[nMainSvr].szSVRIP;
	USHORT usPort = (USHORT)g_config.aHostCfg[nMainSvr].dwSVRPORT;

	BOOL bConnect = g_MyClient.StartClient(strAddress, usPort, FALSE);
	if (bConnect)
	{
		g_MyClient.m_bConnect = TRUE;
		USER_LOG("Socket Connect OK! [ConnID:%d]", g_MyClient.m_Client.GetConnectionID());
	}
	
#endif
	return;
}

// 重连接
VOID ReConnect()
{
	if (!g_MyClient.m_bMainWndClose)
	{
		// 如果登录成功过，才让重登录
		if (g_MyClient.m_bLoginSuccess)
		{
			g_MyClient.InitClient();
			int nMainSvr = g_config.nMainHost;
			CString strAddress = g_config.aHostCfg[nMainSvr].szSVRIP;
			USHORT usPort = (USHORT)g_config.aHostCfg[nMainSvr].dwSVRPORT;

			if (g_MyClient.m_strUserid.IsEmpty())
			{
				::MessageBox(NULL, _T("IM组件重连失败，请你重新登录客户端！"), _T("提示"), MB_OK);
				return;
			}

			BOOL bConnect = g_MyClient.StartClient(strAddress, usPort, FALSE);
			if (bConnect)
			{
				USER_LOG("Socket ReConnect OK:%d", g_MyClient.m_Client.GetConnectionID());
				g_MyClient.SendReconnentReq();
				g_MyClient.m_bConnect = TRUE;
			//	g_pFrame->PostMessage(UM_USER_ONLINE, 0, 0);
			}
		}
	}
	return;
}

// 加载richedit的gif支持
VOID InitRichEditCom()
{
	USER_LOG("InitRichEditCom Start!");

	CString strFileName = "";
	strFileName.Format("%s\\IMGOle.dll", g_config.szHomePath);
	BOOL bRet = DllRegisterServer(strFileName);	// 注册COM组件
	if (!bRet)
	{
		USER_LOG("IMGOle ERROR - path [%s]", strFileName);
		::MessageBox(NULL, _T("COM组件注册失败，应用程序无法完成初始化操作！"), _T("提示"), MB_OK);
		return;
	}

	BOOL bAfxInitRichEdit = ::AfxInitRichEdit();
	HRESULT hrCoInitialize = ::CoInitialize(NULL);
	if (FAILED(hrCoInitialize)) 
	{
		USER_LOG("CoInitialize ERROR - [%s]", HrToMessage(hrCoInitialize));
		::MessageBox(NULL, _T("CoInitialize失败，应用程序无法完成初始化操作！"), _T("提示"), MB_OK);
		return;
	}

	HRESULT hrOleInitialize = ::OleInitialize(NULL);
	if (FAILED(hrOleInitialize))
	{
		USER_LOG("OleInitialize ERROR - [%s]", HrToMessage(hrOleInitialize));
		::MessageBox(NULL, _T("OleInitialize失败，应用程序无法完成初始化操作！"), _T("提示"), MB_OK);
		return;
	}
	Gdiplus::Status gdistatus = GdiplusStartup(&g_gdiplusToken, &g_gdiplusStartupInput, NULL);	// 初始化GDI+
	HMODULE g_hRichEditDll = ::LoadLibrary(_T("Riched20.dll"));		// 加载RichEdit控件DLL

	return;
}

// 卸载
VOID UnInitRichEditCom()
{
	::CoUninitialize();
	::OleUninitialize();

 	if (g_hRichEditDll != NULL)					// 卸载RichEdit控件DLL
 		::FreeLibrary(g_hRichEditDll);
 
 	Gdiplus::GdiplusShutdown(g_gdiplusToken);	// 反初始化GDI+
}

// 登录
VOID LoginIN()
{
	InitConnect();				// 连接服务器
	g_MyClient.SetClientInfo(eUser_Broker);
	g_MyClient.SendCheckPwdReq("3150", "21218cca77804d2ba1922c33e0151105", eLogin_Emp);
	
//	g_MyClient.SetClientInfo(eUser_Client);
//	g_MyClient.SendCheckPwdReq("069000000412", STR_NOCHECKPWD, eLogin_Client);
}

// 停止
VOID StopConnect()
{
#ifndef VER_UPIM_RONGYUN
	g_MyClient.m_bMainWndClose = TRUE;
	g_MyClient.StopClient();
#endif
	return;
}

// 初始化窗口，主窗口隐藏
VOID InitMainFrame()
{
	if (g_pFrame)
		return;
	g_pFrame = new MainFrame();
	if( g_pFrame == NULL ) return;
	// 客户 
	if(VER_UPIM_NUMS == 4 )
	{
		g_pFrame->Create(NULL,_T("UPIM 2015"), WS_EX_APPWINDOW, WS_EX_TOOLWINDOW, 0, 0, 600, 800);
	}
	else if (5 == VER_UPIM_NUMS)
		g_pFrame->Create(NULL,_T("UPIM 2016"), WS_EX_APPWINDOW, WS_EX_TOOLWINDOW, 0, 0, 600, 800);
	else 
	{
		// 分析师和其他版本 
		g_pFrame->Create(NULL,_T("UPIM_FXS 2015"), WS_EX_APPWINDOW, WS_EX_TOOLWINDOW, 0, 0, 600, 800);
	}
 	g_pFrame->CenterWindow();

// 	//////////////////////////////////////////////////////////////////////////
// 	if (g_pFrame)
// 	{
// #ifdef VER_UPIM_ANALYS2
// 		g_pFrame->UpdateMyselfInfo(&g_selfunit, "秘书");
// 		g_pFrame->PostMessage(UM_USER_SETSELFINFO, 0, 0);
// #elif defined VER_UPIM_RONGYUN
// 		g_pFrame->UpdateMyselfInfo(&g_selfunit, "优信");
// 		g_pFrame->PostMessage(UM_USER_SETSELFINFO, 0, 0);
// #else
// 		g_pFrame->UpdateMyselfInfo(&g_selfunit, "UPIM");
// 		g_pFrame->PostMessage(UM_USER_SETSELFINFO, 0, 0);
// #endif // VER_UPIM_ANALYS2
// 	}

	g_pFrame->StartLogin();

	return;
}

// 打开跟某人的会话窗口
VOID OpenChatWnd(CString strUID, int nType)
{
	if (!g_pFrame)
		InitMainFrame();
	memset(g_szOpenUID, 0, sizeof(g_szOpenUID));
	strncpy(g_szOpenUID, strUID, strUID.GetLength());
	g_pFrame->PostMessage(UM_USER_OPENWND, (WPARAM)nType, (LPARAM)0);
	return;
}