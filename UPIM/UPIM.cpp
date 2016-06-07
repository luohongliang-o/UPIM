
// UPIM.cpp : 定义应用程序的类行为。
//

#include "stdafx.h"
#include "UPIM.h"
#include "MyClient.h"
#include "LoginDlg.h"
#include "UI/MainFrame.h"
#include "MainPort.h"
#include "Func.h"
#include "LoadDllFunc.h"

#ifdef _EXE_MODE
BEGIN_MESSAGE_MAP(CUPIMApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()

CUPIMApp::CUPIMApp()
{
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;
	m_hMutex = NULL ; 
}

CUPIMApp theApp;

//////////////////////////////////////////////////////////////////////////
// Create Dump File
#include <dbghelp.h>  
#pragma comment(lib,  "dbghelp.lib")

LONG WINAPI MyExptFilter(EXCEPTION_POINTERS *pExptInfo)  
{  
	LONG ret = EXCEPTION_CONTINUE_SEARCH;  
	TCHAR szExePath[MAX_PATH] = {0};  
	if(::GetModuleFileName(NULL, szExePath, MAX_PATH) > 0)  
	{  
		int ch = _T('\\');  
		*_tcsrchr(szExePath, ch) = _T('\0');  

		CString strDumpFileName ;
		strDumpFileName.Format("\\log\\%s_Dump.dmp", g_config.szRunTime);
		_tcscat(szExePath, strDumpFileName);  
	}  

	// 程序崩溃时，将写入程序目录下的MyDump.dmp文件  
	HANDLE hFile = ::CreateFile(szExePath, GENERIC_WRITE,   
		FILE_SHARE_WRITE, NULL, CREATE_NEW,   
		FILE_ATTRIBUTE_NORMAL, NULL );  
	if(hFile != INVALID_HANDLE_VALUE)  
	{  
		MINIDUMP_EXCEPTION_INFORMATION exptInfo;  
		exptInfo.ThreadId = ::GetCurrentThreadId();  
		exptInfo.ExceptionPointers = pExptInfo;  

		BOOL bOK = ::MiniDumpWriteDump(::GetCurrentProcess(),  
			::GetCurrentProcessId(),   
			hFile, MiniDumpNormal,  
			&exptInfo, NULL, NULL);  
		if(bOK)  
			ret = EXCEPTION_EXECUTE_HANDLER;  
	}  

	return ret;  
} 

//////////////////////////////////////////////////////////////////////////

BOOL CUPIMApp::InitInstance()
{
	CWinApp::InitInstance();

	HINSTANCE hInstance = GetModuleHandle(NULL);
	CPaintManagerUI::SetInstance(hInstance);
	CPaintManagerUI::SetResourcePath(CPaintManagerUI::GetInstancePath());

	LoadConfig();				// 加载配置文件

#ifdef IM_EXE_TO_UP
	// 客户版继承到安全卫士第三方软件调用时，要求只能启动IM一次
	// 并隐藏主窗口 
	if( 4 == VER_UPIM_NUMS )
	{
		// 系统运行的exe目录所在
		CString strHomePath = _T("") ;
		strHomePath.Format("%s" ,g_config.szHomePath) ; 
		if( strHomePath.Right(1) != "\\")
			strHomePath+="\\" ;

		CString tempMutexStr  = _T("") ;
		tempMutexStr = strHomePath + "UPIM_kh.exe" ; 
		tempMutexStr.Replace("\\" ," ") ; 
		// 客户版继承到安全卫士第三方软件调用时，要求只能启动IM一次
		// 并隐藏主窗口 
		m_hMutex = CreateMutex(NULL ,FALSE ,UPIM_CLIENT_MUTEX) ; 
		DWORD iRet = GetLastError() ; 
		if( m_hMutex == NULL ||iRet == ERROR_ALREADY_EXISTS)
		{
			//::MessageBox(NULL ,_T("一个目录里面只能运行一个该程序"),_T("提示") ,MB_OK|MB_ICONERROR) ; 
			// 系统存在实例 
			CloseHandle(m_hMutex) ; 
			m_hMutex = NULL ;
			return FALSE ; 
		}
	}
#endif 
	// 如果是命令行启动exe的模式，则需要判断传入参数，直接打开无法启动
#ifdef VER_UPIM_CLIENT
	//////////////////////////////////////////////////////////////////////////
	// 天贵：金牡丹、金蝴蝶、百川	
	// 广贵：环球投资
	// 对于客户exe调用模式，需要入参传给我 【用户名/市场】
	//CCommandLineInfo cmdInfo;//定义命令行
	//ParseCommandLine(cmdInfo);//解析命令行
	// 调度在命令行中指定的命令。如果
	// 用 /RegServer、/Register、/Unregserver 或 /Unregister 启动应用程序，则返回 FALSE。
	//if (!ProcessShellCommand(cmdInfo)) //程序启动时创建新文档
	//	return FALSE;
#ifndef IM_EXE_TO_UP
	COPYSTRARRAY(g_loginname, g_config.szLoginName/*"laifuwei"*/); 
	g_nMarket = g_config.dwMarket;
#else 
	DWORD t1 = GetTickCount() ; 
	USER_LOG("UP调用入口时间:%d" ,t1 );
	// wuchao add at 20160322  UP调用IM过程： UP将用户名传过来，根据用户名登录  
	// 从命令行中读取UP传过来的用户名
	// 传过来的参数格式为： "dir"username#nMarketID  wuchao modify at 20160418 
	CString strCommand = ::GetCommandLine() ; 
	USER_LOG("UP传过来的启动IM参数%s" ,strCommand) ;

	int index = strCommand.ReverseFind('\"') ;
	if(index == -1 )
	{
		USER_LOG("解析UP传过来的用户名错误") ; 
		return FALSE ; 
	}
	USER_LOG("\"出现的位置：%d" ,index);

	// 将用户名和市场ID解析出来username#nMarketID
	int strCommandLength = strCommand.GetLength() ; 
	CString strNameMarket = strCommand.Right(strCommandLength - index - 1 ) ;
	strNameMarket.Trim() ; 

	CString strUserName ;  // 用户名 
	CString strUserID ;    // 用户ID  
	CString strMarketID ;  // 市场代码 
	
	USER_LOG("strNameMarket：%s" ,strNameMarket) ; 
	// 解析#号出现的位置 
	int nNetIndex = strNameMarket.Find('#') ; // #出现的位置
	if( nNetIndex == -1 )
	{
		USER_LOG("没有市场代码 请检查UP安全卫士：InvestAdviserView.cpp 1282行") ;
		return FALSE ; 
	}
	USER_LOG("#出现的位置:%d" ,nNetIndex) ; 

	AfxExtractSubString(strUserName,(LPCTSTR)strNameMarket ,0,'#') ;
	AfxExtractSubString(strUserID,(LPCTSTR)strNameMarket ,1,'#') ;
	AfxExtractSubString(strMarketID,(LPCTSTR)strNameMarket ,2,'#') ; 

	if( !strUserName.IsEmpty())
		USER_LOG("用户名%s",strUserName) ;
	else
	{
		USER_LOG("无法获取用户名") ;
		return FALSE ; 
	}
	if( !strUserID.IsEmpty())
	{
		USER_LOG("用户ID %s" ,strUserID) ; 
	}
	else
	{
		USER_LOG("无法获取用户名") ;
		return FALSE ; 
	}
	if( !strMarketID.IsEmpty())
		USER_LOG("市场代码%s",strMarketID) ;
	else
	{
		USER_LOG("无法获取市场代码") ;
		return FALSE ; 
	}

	
	COPYSTRARRAY(g_loginname, strUserName.GetBuffer());
	COPYSTRARRAY(g_khID, strUserID.GetBuffer()) ; 

	USER_LOG("g_loginname = %s " ,g_loginname) ;
	USER_LOG("g_khID = %s " ,g_khID) ;

	// wuchao add at 20160418  
	// 解析UP传过来的用户名和市场ID  
	int nMarkeID = atoi(strMarketID.GetBuffer()) ;

	if( nMarkeID == 3 )
		g_nMarket = 3 ; 
	else 
		g_nMarket = g_config.dwMarket; 
#endif 
	
#endif // VER_UPIM_CLIENT

	//////////////////////////////////////////////////////////////////////////
	LPTOP_LEVEL_EXCEPTION_FILTER pPrevFilter =  ::SetUnhandledExceptionFilter(MyExptFilter);  
	if(pPrevFilter != NULL)  
		_tprintf(_T("Previous exception filter exists.\n"));  
	else  
		_tprintf(_T("No Previous exception filter.\n")); 
	//////////////////////////////////////////////////////////////////////////

	InitRichEditCom();			// COM加载
	LoadDLLFunc();				// 加载DLL

	CShellManager *pShellManager = new CShellManager;
	SetRegistryKey(_T("UPIM"));
	InitMainFrame();

	CPaintManagerUI::MessageLoop();
	CPaintManagerUI::Term();
	
 	if (pShellManager != NULL){	delete pShellManager;}


	return FALSE;
}

int CUPIMApp::ExitInstance()
{
	::CoUninitialize();
	::OleUninitialize();
	if( m_hMutex )
	{
		ReleaseMutex(m_hMutex) ; 
		CloseHandle(m_hMutex) ; 
		m_hMutex = NULL ; 
	}
	UnLoadDLLFunc();		// 卸载DLL
	UnInitRichEditCom();	// 卸载COM
	StopConnect();			// 断开连接
	
	return CWinApp::ExitInstance();
}

#endif // _EXE_MODE