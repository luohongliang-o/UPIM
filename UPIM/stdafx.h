#pragma once

//////////////////////////////////////////////////////////////////////////
// 不同版本版本
//#define VER_UPIM_NORMAL		// 普通版
//#define VER_UPIM_DLLMODE		// DLL版，投顾一期中使用的客户端，是封装成DLL调用，作为一种特殊的特例处理
//#define VER_UPIM_ANALYS		// 投顾版
//#define VER_UPIM_ANALYS2		// 投顾版2(需求文档1117中的投顾端登陆的版本，通过登录框直接打开登陆。)
//#define VER_UPIM_CLIENT		// 普通版2(需求文档1117中的客户端登陆的版本，只能通过命令行传参调用，直接打开无法打开。)
//#define IM_EXE_TO_UP          // 宏定义：客户版本提供exe给UP客户端使用 此宏和VER_UPIM_CLIENT一起开启或关闭 
#define VER_UPIM_RONGYUN      // 融云客服系统
//////////////////////////////////////////////////////////////////////////
// 版本数字化
#ifdef VER_UPIM_NORMAL
	#define VER_UPIM_NUMS 0
#endif
#ifdef VER_UPIM_DLLMODE
	#define VER_UPIM_NUMS 1
#endif
#ifdef VER_UPIM_ANALYS
	#define VER_UPIM_NUMS 2
#endif
#ifdef VER_UPIM_ANALYS2
	#define VER_UPIM_NUMS 3
#endif
#ifdef VER_UPIM_CLIENT
	#define VER_UPIM_NUMS 4
#endif
#ifdef VER_UPIM_RONGYUN
	#define VER_UPIM_NUMS 5
#endif

//////////////////////////////////////////////////////////////////////////
// 只有VER_UPIM_DLLMODE模式下，才是封装成dll使用，其余的全是EXE调用模式
#ifdef VER_UPIM_DLLMODE
	#define _DLL_MODE		// 封装成DLL接口调用
#else
	#define _EXE_MODE		// 开发时用于显示化
#endif

// 导出成DLL
#define IM_EXPORT

#define __NEW_MODE		// 用于解决网络环境差时候的分包	
//#define _NEW_HEADER	// 使用UP完整的协议包头

#define NOT_DISPLAY_ID_LIKE_MOBLIE_NUM // 不显示ID疑似手机号码的

//////////////////////////////////////////////////////////////////////////

#define _USE_MFC
#define _RIBBONS_SUPPORT
//#define _DETECT_MEMORY_LEAK

#define _HTTPAPI1(api) "http://121.14.69.23:8004/API/"##api
#define _HTTPAPI2(api) "http://113.108.146.107:855/UserInfoService.svc/"##api
#define HTTPAPIUSERMANAGE(api)  _HTTPAPI1(api)
#define HTTPAPIUSERINFO(api)  _HTTPAPI2(api)

#include "../Socket/Common/Src/GeneralHelper.h"
#include "../UiLib/UIlib.h"
#include "../UiLib/Utils/WinImplBase.h"

#include "../Common/macro.h"
#include <afxcontrolbars.h>
#include "CBase64Coding.h"
// 引入XML支持
#include "TinyXML/tinyxml.h"
// 引入sqlite支持
#include "sqlite/DbSqlite.h"
// 引入md5支持
#include "md5.h"
// 引入json支持
#include "../Common/json/json.h"
using namespace Json;    //json的命名空间

#ifndef NO_USING_DUILIB_NAMESPACE
using namespace DuiLib;
using namespace std;
#endif

#ifdef _DEBUG
#pragma comment(lib, "..\\UiLib\\lib\\DuiLib_d.lib")
#pragma comment(lib, "sqlite\\sqlite3d.lib")
#else
#pragma comment(lib, "..\\UiLib\\lib\\DuiLib.lib")
#pragma comment(lib, "sqlite\\sqlite3.lib")
#endif
#include "rcsdk.h"
#include "globalvar.h"
#include "Func.h"
#include "mmsystem.h"					//播放声音相关
#include "txtutil.h"
using namespace epius;
#pragma comment(lib, "winmm.lib")

