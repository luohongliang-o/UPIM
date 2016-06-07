#include "StdAfx.h"
#include "MongoDBMgr.h"
/*
uri_string的规则为：
mongodb://                                <1>
[username:password@]                      <2>
host1                                     <3>
[:port1]                                  <4>
[,host2[:port2],...[,hostN[:portN]]]      <5>
[/[database]                              <6>
[?options]]                               <7>
（1）mongodb是MongoDB协议的说明符（specifier）
（2）可选的用户名和密码
（3）这是uri唯一必须的部分，它将指定主机名、IP地址或者UNIX域套接字中的任意一个
（4）可选的端口号，默认的是27017
（5）额外的可选主机和端口，可以制定多个主机
（6）如果连接字符串包含认证的证书，这里要写上要认证的数据库名称，如果/database没有具体制定并且连接字符串有认证要求，默认连接“admin”数据库
（7）连接的特定选择。如果/database没有通过，需要在上一个主机和选择字符串（the string of options）开始的问号那里加上一个斜线
*/

CMongoDBMgr::CMongoDBMgr()
{

}

CMongoDBMgr::~CMongoDBMgr()
{
	UnInit();
}

BOOL CMongoDBMgr::InitConnect()
{
    //只能调用一次mongoc_init
	mongoc_init();

	const char *uristr = "mongodb://127.0.0.1:27017/";

	//connection to MongoDB.
	m_client = mongoc_client_new (uristr);

	if (!m_client) 
	{
		OP_LOG_INFO("MongoDB 连接失败 [uri:%s]", uristr);
		return FALSE;
	}

	// 启动之后清空数据
	mongoc_collection_t * m_pCollection = mongoc_client_get_collection(m_client, "upim", "users");
	mongoc_collection_drop(m_pCollection, NULL);
	mongoc_collection_destroy (m_pCollection);
	return TRUE;
}

BOOL CMongoDBMgr::UnInit()
{
	mongoc_client_destroy (m_client);
	mongoc_cleanup ();
	return TRUE;
}

// 用户
void CMongoDBMgr::UserLoginIn(DWORD dwConnID, LPCTSTR pszUserID, BOOL nbLogin, LPCTSTR pszIp/* ="" */, DWORD nPort/* =0 */)
{
	m_csFreeItem.Lock();
	mongoc_collection_t * m_pCollection = mongoc_client_get_collection(m_client, "upim", "users");
	bson_error_t error;
	
	if (nbLogin)
	{
		// 登录,表中添加新数据
		bson_t doc;
		bson_init(&doc);
		uint32_t tmLogin = (uint32_t)time(NULL);
		BSON_APPEND_UTF8(&doc, "uid", pszUserID);
		BSON_APPEND_INT32(&doc, "conn", dwConnID );
		BSON_APPEND_INT32(&doc, "sta", 1 );
		BSON_APPEND_UTF8(&doc, "ip", pszIp );
		BSON_APPEND_INT32(&doc, "port", nPort );
		bool r = mongoc_collection_insert(m_pCollection, MONGOC_INSERT_NONE, &doc, NULL, &error);
		if (!r)  OP_LOG_INFO("MongoDB 插入失败 [%s]", error.message);
		bson_destroy(&doc);
	}
	else
	{
		// 离线，从表中删除数据
		bson_t query;
		bson_init(&query);
		BSON_APPEND_INT32(&query, "conn", dwConnID);
		bool r = mongoc_collection_delete(m_pCollection, MONGOC_DELETE_NONE, &query, NULL, &error);
		if (!r)  OP_LOG_INFO("MongoDB 删除失败 [%s]", error.message);
		bson_destroy(&query);
	}

	mongoc_collection_destroy (m_pCollection);
	m_csFreeItem.Unlock();
	return;
}

// 得到某个用户的在线状态
BOOL CMongoDBMgr::GetUserState(LPCTSTR pszUserID, MYSTATE &m_state, DWORD &dwConnID)
{
	m_csFreeItem.Lock();
	mongoc_collection_t * m_pCollection = mongoc_client_get_collection(m_client, "upim", "users");
	
	//表示查询 uid=xxxx的记录 可以指定多个条件
	bson_t query;
	bson_init(&query);
	BSON_APPEND_UTF8(&query, "uid", pszUserID);
	mongoc_cursor_t *m_pCursor;
	m_pCursor = mongoc_collection_find(m_pCollection, MONGOC_QUERY_NONE, 0, 0, 0, &query, NULL, NULL);
	bson_destroy(&query);

	const bson_t *doc;
	bson_error_t error;
	while (mongoc_cursor_more (m_pCursor) && mongoc_cursor_next (m_pCursor, &doc)) 
	{
		if (!mongoc_cursor_error (m_pCursor, &error))
		{
			char* str;
			str = bson_as_json (doc, NULL);
			//TRACE ("%s\n", str);
			bson_free (str);

			bson_iter_t iter;
			bson_iter_init(&iter, doc);
			if (bson_iter_find(&iter, "conn"))
			{
				int nDf2 = bson_iter_int32(&iter);
				dwConnID = nDf2;
			}
			if (bson_iter_find(&iter, "sta"))
			{
				int nDf1 = bson_iter_int32(&iter);
				m_state = (MYSTATE)nDf1;
			}
		}
		else
		{
			OP_LOG_ERROR("MongoDB1:An error occurred: %s\n", error.message);
			mongoc_cursor_destroy(m_pCursor);
			mongoc_collection_destroy (m_pCollection);
			return FALSE;
		}
	}

	mongoc_cursor_destroy(m_pCursor);
	mongoc_collection_destroy (m_pCollection);
	m_csFreeItem.Unlock();
	return TRUE;
}

// 遍历用户表
BOOL CMongoDBMgr::GetAllOnlineUser(vector<UserNode> &users)
{
	m_csFreeItem.Lock();
	mongoc_collection_t * m_pCollection = mongoc_client_get_collection(m_client, "upim", "users");
	
	bson_t query;
	bson_init(&query);
	BSON_APPEND_INT32(&query, "sta", 1);
	mongoc_cursor_t *m_pCursor;
	m_pCursor = mongoc_collection_find(m_pCollection, MONGOC_QUERY_NONE, 0, 0, 0, &query, NULL, NULL);
	bson_destroy(&query);

	const bson_t *doc;
	bson_error_t error;
	while (mongoc_cursor_more (m_pCursor) && mongoc_cursor_next (m_pCursor, &doc)) 
	{
		UserNode thisnode = {0};
		if (!mongoc_cursor_error (m_pCursor, &error)) 
		{
			bson_iter_t iter;
			bson_iter_init(&iter, doc);
			if (bson_iter_find(&iter, "uid"))
				COPYSTRARRAY(thisnode.m_szUID ,bson_iter_utf8(&iter, NULL));
			if (bson_iter_find(&iter, "conn"))
				thisnode.m_dwSessionID = bson_iter_int32(&iter);
			if (bson_iter_find(&iter, "sta"))
				thisnode.m_dwUserStatus = bson_iter_int32(&iter);
			if (bson_iter_find(&iter, "ip"))
				COPYSTRARRAY(thisnode.m_szInternetIp, bson_iter_utf8(&iter, NULL));
			if (bson_iter_find(&iter, "port"))
				thisnode.m_Port = bson_iter_int32(&iter);
		}
		else
		{
			OP_LOG_ERROR("MongoDB2:An error occurred: %s\n", error.message);
			mongoc_cursor_destroy(m_pCursor);
			mongoc_collection_destroy (m_pCollection);
			return FALSE;
		}
		if (thisnode.m_dwSessionID > 0)
			users.push_back(thisnode);
	}
	
	mongoc_cursor_destroy(m_pCursor);
	mongoc_collection_destroy (m_pCollection);
	m_csFreeItem.Unlock();
	return TRUE;
}

BOOL CMongoDBMgr::UpdateUserTime(DWORD dwConnID,uint32_t time)
{
	m_csFreeItem.Lock();
	BOOL bRet = TRUE;

	mongoc_collection_t * m_pCollection = mongoc_client_get_collection(m_client, "upim", "users");
	bson_error_t error;
	bson_t query;
	bson_t* update;
	bson_t reply;
	char* str;

	bson_init(&query);
	BSON_APPEND_INT32(&query, "conn", dwConnID);

	update = BCON_NEW ("$set", "{", "time", BCON_TIMESTAMP(time,0) ,"updated", BCON_BOOL (true),"}");

	if(!mongoc_collection_find_and_modify (m_pCollection, &query, NULL, update, NULL, false, false, true, &reply, &error))
	{
		bRet = FALSE;
		OP_LOG_ERROR ("find_and_modify() failure: %s\n", error.message);
	}

	str = bson_as_json (&reply, NULL);
	//TRACE ("%s\n", str);
	bson_free (str);

	bson_destroy(&query);
	bson_destroy(update);
	bson_destroy(&reply);
	mongoc_collection_destroy(m_pCollection);

	m_csFreeItem.Unlock();
	return bRet;
}

BOOL CMongoDBMgr::QueryTailTimeUser(uint32_t time,vector<LPCUSERID> &vUsers,vector<DWORD> &vDwConnID)
{
	m_csFreeItem.Lock();
	BOOL bRet = TRUE;
	
	mongoc_collection_t * m_pCollection = mongoc_client_get_collection(m_client, "upim", "users");
	const bson_t *doc;
	bson_error_t error;
	mongoc_cursor_t *cursor;
	bson_t query;
	
	bson_t lt;
	char* str;

	bson_init(&query);
	bson_append_document_begin(&query, "time", -1, &lt);
	bson_append_timestamp(&lt, "$lt", 3, time, 0);
	bson_append_document_end(&query, &lt);

	str = bson_as_json (&query, NULL);
	//TRACE ("query:  %s\n", str);
	bson_free (str);

	cursor = mongoc_collection_find(m_pCollection, MONGOC_QUERY_NONE, 0, 0, 0, &query, NULL, NULL);
	bson_destroy(&query);

	while (mongoc_cursor_more (cursor) && mongoc_cursor_next (cursor, &doc)) 
	{
		if (!mongoc_cursor_error (cursor, &error)) 
		{
			bson_iter_t iter;
			bson_iter_init(&iter, doc);
			if (bson_iter_find(&iter, "uid"))
				vUsers.push_back(bson_iter_utf8(&iter, NULL));
			if (bson_iter_find(&iter, "conn"))
				vDwConnID.push_back(bson_iter_int32(&iter));
		}
		else
		{
			OP_LOG_ERROR("MongoDB2:An error occurred: %s\n", error.message);
			bRet = FALSE;
		}
	}
	mongoc_cursor_destroy(cursor);
	
	mongoc_collection_destroy(m_pCollection);
	m_csFreeItem.Unlock();

	return bRet;
}