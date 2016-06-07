#ifndef GLOBALVAR_H
#define GLOBALVAR_H

#include "../Common/protocol.h"
#include "struct.h"

#include "InfoList.h"
extern CInfoList*		g_pList;
extern SVRCONFIG		g_config;
extern THREADINFO		g_threadInfo;
extern long				g_nInitSuc;

#include "MyServer.h"
extern CMyServer        g_MySever;

#include "Serverlog.h"
extern CServerlog		g_serverlog;

extern std::map<LONG, LPPKGRECVINFO>	g_mapPKGRECVINFO;
extern IPWHITELIST     g_IPWhiteList;
#endif