
// pbSysTool.cpp : 定义应用程序的类行为。
//

#include "stdafx.h"
#include "pbSysTool.h"
#include "pbSysToolDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CpbSysToolApp

BEGIN_MESSAGE_MAP(CpbSysToolApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CpbSysToolApp 构造

CpbSysToolApp::CpbSysToolApp()
{
	// 支持重新启动管理器
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;

	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的一个 CpbSysToolApp 对象

CpbSysToolApp theApp;


// CpbSysToolApp 初始化

BOOL CpbSysToolApp::InitInstance()
{
	// 如果一个运行在 Windows XP 上的应用程序清单指定要
	// 使用 ComCtl32.dll 版本 6 或更高版本来启用可视化方式，
	//则需要 InitCommonControlsEx()。否则，将无法创建窗口。
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// 将它设置为包括所有要在应用程序中使用的
	// 公共控件类。
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();


	// 创建 shell 管理器，以防对话框包含
	// 任何 shell 树视图控件或 shell 列表视图控件。
	//CShellManager *pShellManager = new CShellManager;

	// 标准初始化
	// 如果未使用这些功能并希望减小
	// 最终可执行文件的大小，则应移除下列
	// 不需要的特定初始化例程
	// 更改用于存储设置的注册表项
	// TODO: 应适当修改该字符串，
	// 例如修改为公司或组织名
	SetRegistryKey(_T("应用程序向导生成的本地应用程序"));

	CpbSysToolDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: 在此放置处理何时用
		//  “确定”来关闭对话框的代码
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: 在此放置处理何时用
		//  “取消”来关闭对话框的代码
	}

	// 删除上面创建的 shell 管理器。
// 	if (pShellManager != NULL)
// 	{
// 		delete pShellManager;
// 	}

	// 由于对话框已关闭，所以将返回 FALSE 以便退出应用程序，
	//  而不是启动应用程序的消息泵。
	return FALSE;
}



CString IntToStr(int nVal)
{
	CString strRet;
	char buff[10];

	_itoa(nVal, buff, 10);
	strRet = buff;
	return strRet;
}

CString LongToStr(long lVal)
{
	CString strRet;
	char buff[20];

	_ltoa(lVal, buff, 10);
	strRet = buff;
	return strRet;
}

CString ULongToStr(unsigned long ulVal)
{
	CString strRet;
	char buff[20];

	_ultoa(ulVal, buff, 10);
	strRet = buff;
	return strRet;

}

CString DblToStr(double dblVal, int ndigits)
{
	CString strRet;
	char buff[50];

	_gcvt(dblVal, ndigits, buff);
	strRet = buff;
	return strRet;
}

double StrToOleDateTime(CString str)
{
	COleDateTime time;
	CString syear,smonth,sday,shour,sminute,ssecond;
	INT year,month,day,hour,minute,second;
	if(str.GetLength() == 19)
	{
		syear = str.Mid(0,4);  year = StrToInt(syear);
		smonth = str.Mid(5,2); month = StrToInt(smonth);
		sday = str.Mid(8,2);   day = StrToInt(sday);
		shour = str.Mid(11,2); hour = StrToInt(shour);
		sminute = str.Mid(14,2); minute =StrToInt(sminute);
		ssecond = str.Mid(17,2); second = StrToInt(ssecond);
		time.SetDateTime(year,month,day,hour,minute,second);
	}
	return time.m_dt;
}


CString OleDateTimetoStr(double mdt,const char* pFormatstr)
{
	CString str=_T("");
	if (mdt>0)
	{
		COleDateTime time;
		time.m_dt = mdt;
		str = time.Format(pFormatstr);
	}
	return str;

}


CString TransToDispatch(int distype)
{
	CString str = (_T(""));
	switch (distype)
	{
	case DISPATCH_MAX_IDLETIME:
		str.Format(_T("最大空闲时间调度"));
		break;
	case DISPATCH_MIN_SESSIONTIME:
		str.Format(_T("最短会话时间调度"));
		break;
	case DISPATCH_MAX_PRIORITY:
		str.Format(_T("最大优先级调度"));
		break;
	default:
		break;
	}
	return str;
}

int UnTransToDispatch(CString str)
{
	if(0 == strcmp(str,"最大空闲时间调度")) return DISPATCH_MAX_IDLETIME;
	if(0 == strcmp(str,"最短会话时间调度")) return DISPATCH_MIN_SESSIONTIME;
	if(0 == strcmp(str,"最大优先级调度")) return DISPATCH_MAX_PRIORITY;
}

CString TransToTpRule(int distype)
{
	CString str = (_T(""));
	switch (distype)
	{
	case TP_TIME:
		str.Format(_T("时间路由"));
		break;
	case TP_PROBLEM:
		str.Format(_T("问题路由"));
		break;
	case TP_INTERFACE:
		str.Format(_T("接口路由"));
		break;
	case TP_CUSTOMERSERVICE:
		str.Format(_T("指定客服路由"));
		break;
	case TP_OTHER:
		str.Format(_T("扩展路由"));
		break;
	default:
		break;
	}
	return str;
}

int UnTransToTpRule(CString str)
{
	if (0 == strcmp("时间路由",str)) return TP_TIME;
	if (0 == strcmp("问题路由",str)) return TP_PROBLEM;
	if (0 == strcmp("接口路由",str)) return TP_INTERFACE;
	if (0 == strcmp("指定客服路由",str)) return TP_CUSTOMERSERVICE;
	if (0 == strcmp("扩展路由",str)) return TP_OTHER;
}

CString TransIntToStrTime(int ntime)
{
	CString strText=_T("");
	if (ntime>0)
	{
		int hour=0,minute=0,second=0;
		hour = ntime/10000;
		minute = (ntime/100)%100;
		second = ntime%100;
		strText.Format(_T("%02d:%02d:%02d"),hour ,minute,minute);
	}
	return strText;
}
const char* GetErrorStr(int nError)
{
	const char* error = NULL;
	switch(nError)
	{
	case eGCError_Success:
		error = "成功";
		break;
	case eGCError_DbCon:
		error = "没有可用的数据库连接";
		break;
	case eGCError_DbExe:
		error = "数据库执行失败";
		break;
	case eGCError_DbOp:
		error = "数据库操作失败";
		break;
	case eGCError_Param:
		error = "存储过程参数错误";
		break;
	case eGCError_RsClose:
		error = "记录集已关闭";
		break;
	case eGCError_RsNULL:
		error = "记录集为空";
		break;
	case eGCError_DbClose:
		error = "数据库关闭";
		break;
	default:
		error = "未定义的错误类型";
		break;
	}
	return error;
}

CRect SetCtrlPos(int x,int y,int cx,int cy)
{
	CRect rc;
	POINT p;
	SIZE s;
	p.x = x;
	p.y = y;
	s.cx = cx;
	s.cy = cy;
	rc = CRect(p,s);
	return rc;
}

BOOL ConnectDataBase(CADODatabase *Db)
{
	LPDBCONFIG lpdbConfig = new DBCONFIG;	
	CHAR szHomePath[MAX_PATH],szCfgPath[MAX_PATH];
	GetModuleFileName( NULL, szHomePath, sizeof(szHomePath) );
	long nLen = strlen( szHomePath );
	for ( long loop=(nLen-1); loop>=0; loop-- )
	{
		if ( szHomePath[loop] == '\\' )
		{
			szHomePath[loop] = 0;
			break;
		}
	}
	nsprintf( szCfgPath, sizeof(szCfgPath), "%s\\pbSys.ini", szHomePath);
	lpdbConfig->dwConnTimeOut = GetPrivateProfileInt("dbconfig", "Conntimeout", 0, szCfgPath);
	GetPrivateProfileString("dbconfig", "DataSource", "",lpdbConfig->szDataSource,sizeof(lpdbConfig->szDataSource), szCfgPath);
	GetPrivateProfileString("dbconfig", "DataBase", "",lpdbConfig->szDataBase,sizeof(lpdbConfig->szDataBase), szCfgPath);
	GetPrivateProfileString("dbconfig", "UserName", "",lpdbConfig->szUserName,sizeof(lpdbConfig->szUserName), szCfgPath);
	GetPrivateProfileString("dbconfig", "Password", "",lpdbConfig->szPassword,sizeof(lpdbConfig->szPassword), szCfgPath);

	if(Db==NULL) return FALSE;
	Db->Close();
	CString	strConnect;
	strConnect.Format( "Provider=sqloledb;Data Source=%s;Network Library=DBMSSOCN;Initial Catalog=%s;",lpdbConfig->szDataSource, lpdbConfig->szDataBase);
	Db->SetConnectionTimeout(lpdbConfig->dwConnTimeOut);
	Db->Open( strConnect, lpdbConfig->szUserName, lpdbConfig->szPassword);
	delete lpdbConfig;
	return Db->IsOpen();
}