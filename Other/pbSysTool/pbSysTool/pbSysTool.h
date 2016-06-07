
// pbSysTool.h : PROJECT_NAME 应用程序的主头文件
//

#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"		// 主符号
#include <vector>

// CpbSysToolApp:
// 有关此类的实现，请参阅 pbSysTool.cpp
//


extern CString	DblToStr(double dblVal, int ndigits = 20);
extern CString	IntToStr(int nVal);
extern CString	LongToStr(long lVal);
extern CString	ULongToStr(unsigned long ulVal);
extern double   StrToOleDateTime(CString str);
extern CString  OleDateTimetoStr(double mdt,const char* pFormatstr);
extern CString  TransToDispatch(int distype);
extern int      UnTransToDispatch(CString);
extern CString  TransToTpRule(int distype);
extern int      UnTransToTpRule(CString);
extern CString  TransIntToStrTime(int ntime);
extern const char* GetErrorStr(int nError);
extern CRect    SetCtrlPos(int x,int y,int cx,int cy);
extern BOOL     ConnectDataBase(CADODatabase *Db);

const int COLLUMSIZE = 20;
const int USERID_SIZE = 32;
const int CTRLHEIGHT = 25;
const int EDITHEIGHT = 20;
const int LEFTINCREMENT = 30;
const int TOPINCREMENT  = 20;
const int VINCREMENT = 5;     //上下间隔
const int HINCREMENT = 10;    //左右间隔
const int GRIDHEIGHT = 25;

typedef struct tagDBCONFIG
{
	DWORD	dwConnTimeOut;				// 数据库连接超时
	CHAR	szDataSource[32];			// 数据源
	CHAR	szDataBase[32];				// 数据库
	CHAR	szUserName[32];				// 数据库用户
	CHAR	szPassword[32];				// 数据库密码
}DBCONFIG,*LPDBCONFIG;


enum{
	TP_TIME = 1,
	TP_PROBLEM,
	TP_INTERFACE,
	TP_CUSTOMERSERVICE,
	TP_OTHER
};
enum
{
	DISPATCH_MAX_IDLETIME = 1,
	DISPATCH_MIN_SESSIONTIME,
	DISPATCH_MAX_PRIORITY
};


enum E_GROUPCHAT_ERROR
{
	eGCError_NoDefine = -9999,		// 未定义的错误类型
	eGCError_Success = 0,			// 成功
	eGCError_DbCon = -1,			// 没有可用的数据库连接	 
	eGCError_DbExe = -2,			// 数据库执行失败
	eGCError_DbOp = -3,				// 数据库操作失败
	eGCError_Param = -4,            // 存储过程参数错误
	eGCError_RsClose = -7,			// 记录集已关闭
	eGCError_RsNULL = -8,			// 记录集为空
	eGCError_DbClose = -9           // 数据库关闭
};

enum combo_operation{
	PUBLIC = 1
	,INCODE
	,TP
	,PUBLIC_TP_RELATION
	,TP_INCODE_RELATION
	,DISPATCH_RELATION
};

enum btn_operation{
	BTN_ADD = 1
	,BTN_MODIFY
	,BTN_DELETE
};
enum proctype{
	PUBLIC_ADD                          = PUBLIC<<BTN_ADD
	,PUBLIC_ADD_INCODE                  = INCODE<<BTN_ADD
	,PUBLIC_ADD_TP                      = TP<<BTN_ADD
	,PUBLIC_ADD_RELATION                = PUBLIC_TP_RELATION<<BTN_ADD
	,PUBLIC_ADD_TP_INCODE_RELATION      = TP_INCODE_RELATION<<BTN_ADD
	,PUBLIC_ADD_DISPATCH_RELATION       = DISPATCH_RELATION<<BTN_ADD
	,PUBLIC_MODIFY                      = PUBLIC<<(BTN_MODIFY*2)
	,PUBLIC_MODIFY_INCODE				= INCODE<<(BTN_MODIFY*2)
	,PUBLIC_MODIFY_TP					= TP<<(BTN_MODIFY*2)
	,PUBLIC_MODIFY_RELATION             = PUBLIC_TP_RELATION<<(BTN_MODIFY*2)
	,PUBLIC_MODIFY_TP_INCODE_RELATION	= TP_INCODE_RELATION<<(BTN_MODIFY*2)
	,PUBLIC_MODIFY_DISPATCH_RELATION	= DISPATCH_RELATION<<(BTN_MODIFY*2)
	,PUBLIC_DELETE                      = PUBLIC<<(BTN_DELETE*3)
	,PUBLIC_DELETE_INCODE				= INCODE<<(BTN_DELETE*3)
	,PUBLIC_DELETE_TP					= TP<<(BTN_DELETE*3)
	,PUBLIC_DELETE_TP_INCODE_RELATION	= TP_INCODE_RELATION<<(BTN_DELETE*3)
	,PUBLIC_DELETE_DISPATCH_RELATION	= DISPATCH_RELATION<<(BTN_DELETE*3)
};


class CpbSysToolApp : public CWinApp
{
public:
	CpbSysToolApp();

// 重写
public:
	virtual BOOL InitInstance();

// 实现

	DECLARE_MESSAGE_MAP()
};

extern CpbSysToolApp theApp;