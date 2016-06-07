#pragma once

#include "log4cxx/logger.h"
#include "log4cxx/propertyconfigurator.h"
using namespace log4cxx;

#ifdef _DEBUG
#	pragma comment(lib, "log4cxx/lib/log4cxxD.lib")
#else
#	pragma comment(lib, "log4cxx/lib/log4cxx.lib")
#endif

class CServerlog
{
public:
	CServerlog(void);
	~CServerlog(void);
public:
	//infologger
	void WriteFormatDebugLog(char* lpszFormat, ...);			//LOG4CXX_FATAL
	void WriteFormatInfoLog(char* lpszFormat, ...);				//LOG4CXX_INFO
	void WriteFormatTraceLog(char* lpszFormat, ...);			//LOG4CXX_TRACE

	//errorlogger
	void WriteFormatWarnLog(char* lpszFormat, ...);				//LOG4CXX_WARN
	void WriteFormatFatalLog(char* lpszFormat, ...);			//LOG4CXX_FATAL
	void WriteFormatErrorLog(char* lpszFormat, ...);			//LOG4CXX_ERROR

	//more
	void WriteFormatInfoLog_DjInfo(char* lpszFormat, ...);		//LOG4CXX_INFO
	void WriteFormatInfoLog_MenTable(char* lpszFormat, ...);	//LOG4CXX_INFO
	void WriteFormatInfoLog_MySQL(char* lpszFormat, ...);		//LOG4CXX_INFO
	
protected:
	LoggerPtr infologger;
	LoggerPtr errorlogger;

	LoggerPtr djinfologger;
	LoggerPtr mysqllogger;
	LoggerPtr memtablelogger;
};

