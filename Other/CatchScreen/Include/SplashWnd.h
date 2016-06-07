/////////////////////////////////////////////////////////////////////////////
/**
* 文件名  ：Splash.h
* 版本号  ：V1.0
* 项目名称：
* 版权声明：Copyright 2002-2003 . All Rights Reserved.
* 模块简单描述：启动Splash封面头文件
*======================================================
* 版本变更记录:
*      v1.0  2002-11-24   9：00   
*
* 
*/
/*使用方法:
1.在应用种序入口模块中加入代码，以实现启动Splash动画
  由于在Spalsh类中封装了动画文件名，需要更改相应类，实现动态指定文件
  //安装时将swflash.ocx文件拷贝到windows\system路径下
	TCHAR szDir [MAX_PATH];
	::GetSystemDirectory (szDir, MAX_PATH);
	//TRACE("Windows 系统目录为%s\n", szDir);
    CString syspath = szDir;
    syspath = syspath + _T("\\swflash.ocx");
    
	//注册flash的控件
	HMODULE	hmod=LoadLibrary (syspath);
	FARPROC  p=GetProcAddress (hmod,"DllRegisterServer");
	(*p)();

//	OnRegocx("swflash.ocx") ;
	{
		CCommandLineInfo cmdInfo;
		ParseCommandLine(cmdInfo);

		CSplashWnd::EnableSplashScreen(cmdInfo.m_bShowSplash);
	}

2. 在要使用Splash动画的类的函数中只加入下面一句
        //参数：播放的第一个Splash文件名,接着播放的第二个Splash文件名,在哪个TIME线程号中打开flash文件1,
       //       播放完FLASH后是否关闭主窗体,Splash启动窗体的父窗体
    CString swfFile = g_strAppPath + "\\UseRes\\startup.swf";
	CSplashWnd::ShowSplashScreen(swfFile, _T(""), 1, TRUE, FALSE, this);	

3.在2中可能要加入一句用于设置动画的总帖数，以便到最后上帖时自动关闭窗体










*/
/////////////////////////////////////////////////////////////////////////////

#ifndef _SPLASH_SCRN_
#define _SPLASH_SCRN_

#include "shockwaveflash.h"
/////////////////////////////////////////////////////////////////////////////
/*
*类名:CShockwaveFlash
*说明：显示Splash动画文件窗体类
*变更记录：
*
*/

class CSplashWnd : public CWnd
{
// Construction
protected:
	CSplashWnd();

// Attributes:
public:

   //Flash控件窗体对象
   CShockwaveFlash  m_startflash;

// Operations
public:
	static void EnableSplashScreen(BOOL bEnable = TRUE);
	static void ShowSplashScreen(CString flashfile1, CString flashfile2 = _T(""), 
		                         INT timeID = 1, BOOL isKeyControl = TRUE, BOOL isCloseMainFrm = FALSE, 
								 CWnd* pParentWnd = NULL);
	static BOOL PreTranslateAppMessage(MSG* pMsg);
	void SetFlashFileName(CString filename1, CString filename2 = _T(""));

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSplashWnd)
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
public:

	~CSplashWnd();
	virtual void PostNcDestroy();

protected:
	BOOL Create(CWnd* pParentWnd = NULL);
	void HideSplashScreen();
	static BOOL c_bShowSplashWnd;
	static CSplashWnd* c_pSplashWnd;

// Generated message map functions
protected:
	//{{AFX_MSG(CSplashWnd)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
 	CString m_flashFile1;
 	CString m_flashFile2;
	INT m_timeID;
	BOOL m_bKeyControl;
	BOOL m_isCloseMainWnd;
};


#endif
