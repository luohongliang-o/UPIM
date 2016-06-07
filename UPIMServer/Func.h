#pragma once
#include "stdafx.h"

//日志类
#define OP_LOG_DEGUB(pszFormat, ...)    g_serverlog.WriteFormatDebugLog( pszFormat, __VA_ARGS__)
#define OP_LOG_INFO(pszFormat, ...)		g_serverlog.WriteFormatInfoLog(pszFormat, __VA_ARGS__)
#define OP_LOG_TRACE(pszFormat, ...)    g_serverlog.WriteFormatTraceLog( pszFormat, __VA_ARGS__)
#define OP_LOG_WARN(pszFormat, ...)		g_serverlog.WriteFormatWarnLog( pszFormat, __VA_ARGS__)
#define OP_LOG_ERROR(pszFormat, ...)    g_serverlog.WriteFormatErrorLog( pszFormat, __VA_ARGS__)
#define OP_LOG_FATAL(pszFormat, ...)    g_serverlog.WriteFormatFatalLog( pszFormat, __VA_ARGS__)

// 填充返回buf结构
extern BOOL InitAnsBufHead(IMANSBUFFER& m_ansBuf, long req, long PacketLen = 0, long RawLen = 0);
extern VOID		LoadConfig();															// 加载服务器配置文件	
extern VOID		SaveConfig();
extern BOOL	    IsWhiteIP(CHAR* strIP);
extern WORD		GetBuildNo( LPSTR lpszDateTime, const WORD wSize );						// 获取编译序列号
extern CString	IntToStr(int nVal);
extern CString	LongToStr(long lVal);
extern CString	ULongToStr(unsigned long ulVal);
extern CString	DblToStr(double dblVal, int ndigits = 20);
extern CString	DblToStr(float fltVal);
extern LPCSTR	AllTrim( LPSTR lpszString );
extern LPSTR	SafeStrcpy( LPSTR lpszDest, LPCSTR lpcszSrc, const WORD wDestSize );
extern BOOL		ConnectDataBase( LPDBCONFIG lpdbConfig, CADODatabase* pAdoDb);			// 连接数据库
extern BOOL		DisConnectDataBase( CADODatabase* pAdoDb );								// 断开数据库
extern BOOL		IsDBDisConn(CString strError);											// 数据库状态
extern CString	GetNowTime();
extern double   StrToOleDateTime(CString str);
extern LPCSTR   OleDateTimetoStr(double mdt,const char* pFormatstr);
extern void ConvertTimeToLong(time_t tmTime, long& nRetDate, long& nRetTime);
extern LONG ParseJsonFields(LPSTR lpszString, STATE_REFLASH_& mstu, LONG nMaxField);

//编码解码API 将二进制流编码成字符串流 或将字符串流解码为原始的二进制流
//编码或解码后的数据放在pResultBuf,注意nMaxResultBufLen的长度至少应为nRawDataLen的2倍以上
//bEncode为TRUE,表示此函数用于编码,否则用于解码
//如果编码或解码失败,返回-1,成功则返回编码或解码后的长度
extern long Base64_Encode(BYTE *pRawData, long nRawDataLen, BYTE *pResultBuf, long nMaxResultBufLen, BOOL bEncode);