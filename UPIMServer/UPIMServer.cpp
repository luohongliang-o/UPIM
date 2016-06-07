
// UPIMServer.cpp : 定义应用程序的类行为。
//

#include "stdafx.h"
#include "afxwinappex.h"
#include "afxdialogex.h"
#include "UPIMServer.h"
#include "MainFrm.h"

#include "UPIMServerDoc.h"
#include "UPIMServerView.h"

// CUPIMServerApp
BEGIN_MESSAGE_MAP(CUPIMServerApp, CWinAppEx)
	ON_COMMAND(ID_APP_ABOUT, &CUPIMServerApp::OnAppAbout)
	// 基于文件的标准文档命令
END_MESSAGE_MAP()


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
		SYSTEMTIME st = {0};
		GetLocalTime(&st);
		CString strTime = _T("");
		strTime.Format("%04d_%02d_%02d_%02d%02d%02d%03d", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);
		strDumpFileName.Format("\\log\\%s_Dump.dmp",strTime);
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
// CUPIMServerApp 构造


CUPIMServerApp::CUPIMServerApp()
{
	m_bHiColorIcons = TRUE;

	// 支持重新启动管理器
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;
#ifdef _MANAGED
	// 如果应用程序是利用公共语言运行时支持(/clr)构建的，则:
	//     1) 必须有此附加设置，“重新启动管理器”支持才能正常工作。
	//     2) 在您的项目中，您必须按照生成顺序向 System.Windows.Forms 添加引用。
	System::Windows::Forms::Application::SetUnhandledExceptionMode(System::Windows::Forms::UnhandledExceptionMode::ThrowException);
#endif

	// TODO: 将以下应用程序 ID 字符串替换为唯一的 ID 字符串；建议的字符串格式
	//为 CompanyName.ProductName.SubProduct.VersionInformation
	SetAppID(_T("UPIMServer.AppID.NoVersion"));

	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}

// 唯一的一个 CUPIMServerApp 对象

CUPIMServerApp theApp;


// CUPIMServerApp 初始化

BOOL CUPIMServerApp::InitInstance()
{
	CWinAppEx::InitInstance();


	SetErrorMode(1|2);

	LPTOP_LEVEL_EXCEPTION_FILTER pPrevFilter =  ::SetUnhandledExceptionFilter(MyExptFilter);  
	if(pPrevFilter != NULL)  
		_tprintf(_T("Previous exception filter exists.\n"));  
	else  
		_tprintf(_T("No Previous exception filter.\n")); 

	EnableTaskbarInteraction(FALSE);

	// 使用 RichEdit 控件需要  AfxInitRichEdit2()	
	// AfxInitRichEdit2();

	// 标准初始化
	// 如果未使用这些功能并希望减小
	// 最终可执行文件的大小，则应移除下列
	// 不需要的特定初始化例程
	// 更改用于存储设置的注册表项
	// TODO: 应适当修改该字符串，
	// 例如修改为公司或组织名
	SetRegistryKey(_T("UPIMSEVER_1.0.3.1_1"));
	LoadStdProfileSettings(4);  // 加载标准 INI 文件选项(包括 MRU)


	InitContextMenuManager();

	InitKeyboardManager();

	InitTooltipManager();
	CMFCToolTipInfo ttParams;
	ttParams.m_bVislManagerTheme = TRUE;
	theApp.GetTooltipManager()->SetTooltipParams(AFX_TOOLTIP_TYPE_ALL,
		RUNTIME_CLASS(CMFCToolTipCtrl), &ttParams);

	// 注册应用程序的文档模板。文档模板
	// 将用作文档、框架窗口和视图之间的连接
	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MYMENU,
		RUNTIME_CLASS(CUPIMServerDoc),
		RUNTIME_CLASS(CMainFrame),       // 主 SDI 框架窗口
		RUNTIME_CLASS(CUPIMServerView));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);


	// 分析标准 shell 命令、DDE、打开文件操作的命令行
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);



	// 调度在命令行中指定的命令。如果
	// 用 /RegServer、/Register、/Unregserver 或 /Unregister 启动应用程序，则返回 FALSE。
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// 唯一的一个窗口已初始化，因此显示它并对其进行更新
	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();
	
	// 仅当具有后缀时才调用 DragAcceptFiles
	//  在 SDI 应用程序中，这应在 ProcessShellCommand 之后发生
	return TRUE;
}

// CUPIMServerApp 消息处理程序


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()

// 用于运行对话框的应用程序命令
void CUPIMServerApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

// CUPIMServerApp 自定义加载/保存方法

void CUPIMServerApp::PreLoadState()
{
	BOOL bNameValid;
	CString strName;
	bNameValid = strName.LoadString(IDS_EDIT_MENU);
	ASSERT(bNameValid);
	GetContextMenuManager()->AddMenu(strName, IDR_POPUP_EDIT);
}

void CUPIMServerApp::LoadCustomState()
{
}

void CUPIMServerApp::SaveCustomState()
{
}


// CUPIMServerApp 消息处理程序
int CUPIMServerApp::ExitInstance()
{
	return CWinAppEx::ExitInstance();
}