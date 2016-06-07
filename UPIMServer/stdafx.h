
// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently,
// but are changed infrequently

#pragma once

#ifndef _WIN32_WINNT
#define _WIN32_WINNT _WIN32_WINNT_WS03//_WIN32_WINNT_WINXP
#endif

#ifndef _SECURE_ATL
#define _SECURE_ATL 1
#endif

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // 从 Windows 头中排除极少使用的资料
#endif

#define _USE_MFC
#include "targetver.h"
#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // 某些 CString 构造函数将是显式的

// 关闭 MFC 对某些常见但经常可放心忽略的警告消息的隐藏
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC 核心组件和标准组件
#include <afxext.h>         // MFC 扩展

#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // MFC 对 Internet Explorer 4 公共控件的支持
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>             // MFC 对 Windows 公共控件的支持
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxcontrolbars.h>     // 功能区和控件条的 MFC 支持
	
#define _RIBBONS_SUPPORT
#define _DETECT_MEMORY_LEAK
#define _CRT_SECURE_NO_DEPRECATE 

#define LOG_FILTER      // 发送协议号日志控制宏
#define IP_WHITELIST    // 是否开启白名单保护
#define KICKMAN			// 是否单点登录
#define __NEW_MODE		// 用于解决网络环境差时候的分包	
//#define _NEW_HEADER	// 使用UP完整的协议包头

// 引入LOG支持 
#define _USE_LOG_
#include "Serverlog.h"
#include "CBase64Coding.h"

// 引入socket支持
#include "../Socket/Common/Src/GeneralHelper.h"
#include "ado2.h"
#include "IniFile.h"

// 引入json支持
#include "../Common/json/json.h"
using namespace Json;    //json的命名空间

#include "../Common/macro.h"
#include "struct.h"

#include "../Common/protocol.h"
#include "TabCtrl/TabCtrl.h"
#include "Func.h"

#ifdef _DEBUG
	#define _HOST(s)         "http://upia."##s##"/api/live/"
	#define HOST(s)          _HOST(s)
#else
	#define _HOST(s)         "http://upia."##s##"/api/live/"
	#define HOST(s)          _HOST(s)
#endif

#pragma warning(disable : 4786)	// too long identifiers.
#pragma warning(disable : 4996)