
// UPIM.h : PROJECT_NAME 应用程序的主头文件
//

#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"		// 主符号

// CUPIMApp:
// 有关此类的实现，请参阅 UPIM.cpp
//

#ifdef _EXE_MODE

class CUPIMApp : public CWinApp
{
public:
	CUPIMApp();

// 重写
public:
	virtual BOOL InitInstance();

// 实现

	DECLARE_MESSAGE_MAP()
	virtual int ExitInstance();

public:
	HANDLE    m_hMutex ;     // 判断客户版本是否只运行一个实例
};

extern CUPIMApp theApp;

#endif