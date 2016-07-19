#include "stdafx.h"
#include "LoadDllFunc.h"
#include "globalvar.h"
IUPIM_PupupDialog	IUPIM_PupupDialog_ = NULL;
HMODULE	II_mod = NULL;
IMRongCloudDLL g_RongCloudDll = {0};
BOOL LoadDLLFunc()
{
	char dllpath[MAX_PATH],filename_ImageTool[MAX_PATH];
#ifdef _DEBUG
	sprintf(dllpath,"%s\\", g_config.szHomePath);
	sprintf(filename_ImageTool,"%s\\CatchScreend.dll", g_config.szHomePath);
#else
	sprintf(dllpath,"%s\\", g_config.szHomePath);
	sprintf(filename_ImageTool,"%s\\CatchScreen.dll", dllpath);
#endif
#ifdef VER_UPIM_RONGYUN
	char filename_Rcsdk[MAX_PATH];
	sprintf(filename_Rcsdk,"%s\\rcsdk.dll", g_config.szHomePath);
	if (!g_RongCloudDll.m_hModule)
	{
		char PathPreRcsdk[MAX_PATH] = {0};
		GetCurrentDirectory(MAX_PATH, PathPreRcsdk);
		SetCurrentDirectory(dllpath);
		g_RongCloudDll.m_hModule = AfxLoadLibrary(filename_Rcsdk);
		SetCurrentDirectory(PathPreRcsdk);
		if (g_RongCloudDll.m_hModule == NULL)
		{
			AfxFreeLibrary(g_RongCloudDll.m_hModule);
			AfxMessageBox(_T("LoadLibrary rcsdk.dll failed"));
			return FALSE;
		}
	}
	g_RongCloudDll.m_pInitClient                =  (IMInitClient)GetProcAddress(g_RongCloudDll.m_hModule, "InitClient" );
	g_RongCloudDll.m_pSetDeviceInfo             =  (IMSetDeviceInfo)GetProcAddress(g_RongCloudDll.m_hModule, "SetDeviceInfo" );
	g_RongCloudDll.m_pConnect                   =  (IMConnect)GetProcAddress(g_RongCloudDll.m_hModule, "Connect" );
	g_RongCloudDll.m_pRegisterMessageType       =  (IMRegisterMessageType)GetProcAddress(g_RongCloudDll.m_hModule, "RegisterMessageType" );
	g_RongCloudDll.m_pSetMessageListener        =  (IMSetMessageListener)GetProcAddress(g_RongCloudDll.m_hModule, "SetMessageListener" );
	g_RongCloudDll.m_pSetExceptionListener      =  (IMSetExceptionListener)GetProcAddress(g_RongCloudDll.m_hModule, "SetExceptionListener" );
	g_RongCloudDll.m_pDisconnect                =  (IMDisconnect)GetProcAddress(g_RongCloudDll.m_hModule, "Disconnect" );
	g_RongCloudDll.m_pSaveMessage               =  (IMSaveMessage)GetProcAddress(g_RongCloudDll.m_hModule, "SaveMessage" );
	g_RongCloudDll.m_psendMessage               =  (IMsendMessage)GetProcAddress(g_RongCloudDll.m_hModule, "sendMessage" );
	g_RongCloudDll.m_pGetUserInfo               =  (IMGetUserInfo)GetProcAddress(g_RongCloudDll.m_hModule, "GetUserInfo" );
	g_RongCloudDll.m_pGetUnreadCount            =  (IMGetUnreadCount)GetProcAddress(g_RongCloudDll.m_hModule, "GetUnreadCount" );
	g_RongCloudDll.m_pClearMessagesUnreadStatus =  (IMClearMessagesUnreadStatus)GetProcAddress(g_RongCloudDll.m_hModule, "ClearMessagesUnreadStatus" );
	g_RongCloudDll.m_pUploadUserInfo            =  (IMUploadUserInfo)GetProcAddress(g_RongCloudDll.m_hModule, "UploadUserInfo" );

#endif
	if(!II_mod)
	{
		char PathPre[MAX_PATH] = {0};
		GetCurrentDirectory(MAX_PATH, PathPre);

		SetCurrentDirectory(dllpath);
		II_mod = AfxLoadLibrary(filename_ImageTool);

		// 如果是被UP客户端调用的，需要把路径设置回去，否则会影响UP客户端的功能
		SetCurrentDirectory(PathPre);
		if(!II_mod)	
			return FALSE;

		IUPIM_PupupDialog_ = ( IUPIM_PupupDialog ) ::GetProcAddress(II_mod, "PupupDialog");
	}
	if( !IUPIM_PupupDialog_ )
		return FALSE;

	return TRUE;
}

void UnLoadDLLFunc()
{
	if(II_mod)
	{
		AfxFreeLibrary(II_mod);
		II_mod = NULL;
	}
	if (g_RongCloudDll.m_hModule)
	{
		AfxFreeLibrary(g_RongCloudDll.m_hModule);
		g_RongCloudDll.m_hModule = NULL;
	}
	
}

void sendMessageCallback(const wchar_t* json_str) 
{
	Json::Reader reader;
	Json::Value jobj;
	std::string str = txtutil::convert_wcs_to_utf8(json_str);
	if (reader.parse(str, jobj))  // reader将Json字符串解析到jobj将包含Json里所有子元素  
	{

	}
};
