#pragma once
#include "globalvar.h"

//引入mongo支持
#include "mongoc.h"
#pragma comment(lib,"ws2_32.lib")
//#define USE_STATIC_LIBS_ 0

#ifdef _DEBUG
	#ifdef USE_STATIC_LIBS_
		#pragma comment(lib, "bson-static-1.0_d.lib")
		#pragma comment(lib, "mongoc-static-1.0_d.lib")
	#else
		#pragma comment(lib, "bson-1.0_d.lib")
		#pragma comment(lib, "mongoc-1.0_d.lib")
	#endif
#else
	#ifdef USE_STATIC_LIBS_
		#pragma comment(lib, "bson-static-1.0.lib")
		#pragma comment(lib, "mongoc-static-1.0.lib")
	#else
		#pragma comment(lib, "bson-1.0.lib")
		#pragma comment(lib, "mongoc-1.0.lib")
	#endif
#endif

class CMongoDBMgr
{
public:
	CMongoDBMgr();
	~CMongoDBMgr();

	BOOL InitConnect();
	BOOL UnInit();

	// 用户登录/下线，根据ID记录用户信息
	void UserLoginIn(DWORD dwConnID, LPCTSTR pszUserID, BOOL nbLogin, LPCTSTR pszIp="", DWORD nPort=0);
	// 根据用户ID查询服务器上的在线信息
	BOOL GetUserState(LPCTSTR pszUserID, MYSTATE &m_state, DWORD &dwConnID);
	// 外部传入，遍历在线用户
	BOOL GetAllOnlineUser(vector<UserNode> &users);
	// 更新用户状态，用以确认是否在线
	BOOL UpdateUserTime(DWORD dwConnID,uint32_t time);
	// 获取时间条件的用户
	BOOL QueryTailTimeUser(uint32_t time, vector<LPCUSERID> &vUsers, vector<DWORD> &vDwConnID);

public:	
	mongoc_client_t * m_client;		//mongodb连接管理
	CCriSec			m_csFreeItem;
};

