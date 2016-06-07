#include "StdAfx.h"
#include "globalvar.h"

CInfoList*		g_pList = NULL;
SVRCONFIG		g_config = {0};
THREADINFO		g_threadInfo = {0};
long			g_nInitSuc;

CMyServer       g_MySever;
CServerlog		g_serverlog;
std::map<LONG, LPPKGRECVINFO>	g_mapPKGRECVINFO;
IPWHITELIST     g_IPWhiteList = {0};