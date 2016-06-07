#include "StdAfx.h"
#include "Serverlog.h"

CServerlog::CServerlog()
{
	PropertyConfigurator::configure("log4cxx.properties");
	infologger		= Logger::getLogger("server");
	errorlogger		= Logger::getLogger("server.error");
	djinfologger	= Logger::getLogger("DjInfo");
	mysqllogger		= Logger::getLogger("MySQL");
	memtablelogger	= Logger::getLogger("MemTable");
}

CServerlog::~CServerlog()
{
}

void CServerlog::WriteFormatDebugLog(char* lpszFormat, ...)
{
#ifdef _USE_LOG_
	va_list args;
	va_start(args, lpszFormat);
	CHAR szBuffer[1024] = {0};
	vsprintf(szBuffer, lpszFormat, args);
	va_end(args);
	infologger->debug(szBuffer);
#endif // _USE_LOG_
}

void CServerlog::WriteFormatInfoLog(char* lpszFormat, ...)
{
#ifdef _USE_LOG_
	va_list args;
	va_start(args, lpszFormat);
	CHAR szBuffer[1024];
	vsprintf(szBuffer, lpszFormat, args);
	va_end(args);
	infologger->info(szBuffer);
#endif // _USE_LOG_
}

void CServerlog::WriteFormatTraceLog(char* lpszFormat, ...)
{
#ifdef _USE_LOG_
	va_list args;
	va_start(args, lpszFormat);
	CHAR szBuffer[1024];
	vsprintf(szBuffer, lpszFormat, args);
	va_end(args);
	infologger->trace(szBuffer);
#endif // _USE_LOG_
}

void CServerlog::WriteFormatWarnLog(char* lpszFormat, ...)
{
#ifdef _USE_LOG_
	va_list args;
	va_start(args, lpszFormat);
	CHAR szBuffer[1024];
	vsprintf(szBuffer, lpszFormat, args);
	va_end(args);
	errorlogger->warn(szBuffer);
#endif // _USE_LOG_
}

void CServerlog::WriteFormatErrorLog(char* lpszFormat, ...)
{
#ifdef _USE_LOG_
	va_list args;
	va_start(args, lpszFormat);
	CHAR szBuffer[1024];
	vsprintf(szBuffer, lpszFormat, args);
	va_end(args);
	errorlogger->error(szBuffer);
#endif // _USE_LOG_
}

void CServerlog::WriteFormatFatalLog(char* lpszFormat, ...)
{
#ifdef _USE_LOG_
	va_list args;
	va_start(args, lpszFormat);
	CHAR szBuffer[1024];
	vsprintf(szBuffer, lpszFormat, args);
	va_end(args);
	errorlogger->fatal(szBuffer);
#endif // _USE_LOG_
}

void CServerlog::WriteFormatInfoLog_DjInfo(char* lpszFormat, ...)
{
#ifdef _USE_LOG_
	va_list args;
	va_start(args, lpszFormat);
	CHAR szBuffer[1024];
	vsprintf(szBuffer, lpszFormat, args);
	va_end(args);
	djinfologger->info(szBuffer);
#endif // _USE_LOG_
}

void CServerlog::WriteFormatInfoLog_MenTable(char* lpszFormat, ...)
{
#ifdef _USE_LOG_
	va_list args;
	va_start(args, lpszFormat);
	CHAR szBuffer[1024];
	vsprintf(szBuffer, lpszFormat, args);
	va_end(args);
	memtablelogger->info(szBuffer);
#endif // _USE_LOG_
}

void CServerlog::WriteFormatInfoLog_MySQL(char* lpszFormat, ...)
{
#ifdef _USE_LOG_
	va_list args;
	va_start(args, lpszFormat);
	CHAR szBuffer[1024];
	vsprintf(szBuffer, lpszFormat, args);
	va_end(args);
	mysqllogger->info(szBuffer);
#endif // _USE_LOG_
}