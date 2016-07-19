#include "StdAfx.h"
#include "SqliteOp.h"
#include "../Func.h"

CSqliteOp::CSqliteOp(void)
{
}

CSqliteOp::~CSqliteOp(void)
{
}

BOOL CSqliteOp::DBOpen()
{
	CString strTmp = _T(""),strUTF8 = _T("");
	strTmp.Format("%s\\upmsg.db", g_config.szUserDir);

	// SQLite(_sqlite3_open)传入的路径是UTF-8编码。所以路径中如果有中文的话，会引起BUG！
	// 此处进行编码转换
	strUTF8 = UTF8Convert(strTmp, CP_ACP, CP_UTF8);

	return m_sqlite.Open(strUTF8);
	return TRUE;
}

BOOL CSqliteOp::DBClose()
{
	//m_sqlite.Close();
	return TRUE;
}

BOOL CSqliteOp::DBExec(const char* sql)
{
	return m_sqlite.DirectStatement(sql);
}

BOOL CSqliteOp::DBExec_Rs(const char* sql, CDbSQLite::ResultTable& res)
{
	bool brtn = m_sqlite.SelectStatement(sql, res);
	return brtn;
}

BOOL CSqliteOp::DBCreate()
{
	CString temp;
	temp.Format("%s\\upmsg.db", g_config.szUserDir);
	if(TestFileExist(temp)==false)
	{
		DBOpen();
		DBExec("create table chatrecord("
				"seq integer primary key,"
				"fromid char(20),"
				"toid char(20),"
				"roomid integer,"
				"senddate integer,"
				"sendtime integer,"
				"type integer,"
				"format integer,"
				"msglen integer,"
				"msg blob);");
		DBExec("create index cr_id on chatrecord(toid, fromid, type, senddate);");
		DBClose();
	}
	return TRUE;
}

BOOL CSqliteOp::DBInsertRecordRaw(LPCSTR fromid, LPCSTR toid, int nroomid, long senddate, long  sendtime, int type, int format, const char* msg)
{
	if(!g_config.m_bHisMsg) 
		return false;
	int msglen = 0;
	CString strMsg;
	if (msg)
	{
		msglen = strlen(msg);
		strMsg.Format("%s",msg);
	}

	CString sql;
	strMsg.Replace("\'","\'\'");
	sql.Format("insert into chatrecord \
			   (seq,fromid,toid,roomid,senddate,sendtime,type,format,msglen,msg)\
			   values(NULL,\'%s\',\'%s\',%d,%d,%d,%d,%d,%d,\'%s\');",
			   fromid, toid, nroomid, senddate, sendtime, type, format, msglen, strMsg);
	return DBExec(sql);
}

BOOL CSqliteOp::DBInsertRecord(LPCSTR fromid, LPCSTR toid, int nroomid, int type, int format, const char* msg)
{
	long nDate=0,nTime=0;
	ConvertTimeToLong(time(NULL),nDate,nTime);
	return DBInsertRecordRaw(fromid, toid, nroomid, nDate, nTime, type, format, msg);
}

BOOL CSqliteOp::DBInsertRecord(LPCSTR fromid, LPCSTR toid, int nroomid, long sendtime, int type, int format, const char* msg)
{
	if(!g_config.m_bHisMsg) 
		return false;
	long nDate=0,nTime=0;
	ConvertTimeToLong(sendtime, nDate, nTime);
	return DBInsertRecordRaw(fromid, toid, nroomid, nDate, nTime, type, format, msg);
}

BOOL CSqliteOp::DBInsertGRecord(LPCSTR fromid, long roomid, int sendtime, const char* msg)
{
	char szToID[USERID_SIZE] = {0};
	sprintf(szToID, "%d", roomid);
	return DBInsertRecord(fromid, szToID, 0, sendtime, eRecord_Group, eMsgFormat_Def, msg);
}

BOOL CSqliteOp::GetRecord(const char *sql, std::vector<CHAT_RECORD2> &vt)
{
	CDbSQLite::ResultTable res;
	DBExec_Rs(sql, res);
	int nRecord = res.m_records.size();

	for (int nindex = 0; nindex <nRecord; nindex++)
	{
		CDbSQLite::ResultRecord *record = res.next();
		if (record)
		{
			CHAT_RECORD2 rcd;
			rcd.seq	= atol(record->m_fields[0]);
			COPY_USERID(rcd.fromid, record->m_fields[1]);
			COPY_USERID(rcd.toid, record->m_fields[2]);
			rcd.roomid = atol(record->m_fields[3]);
			rcd.senddate = atol(record->m_fields[4]);
			rcd.sendtime = atol(record->m_fields[5]);
			rcd.type = atol(record->m_fields[6]);
			rcd.format = atol(record->m_fields[7]);
			rcd.msglen = atol(record->m_fields[8]);
			int len = strlen(record->m_fields[9]);
			rcd.msg = new char[len+1];
			strncpy(rcd.msg, record->m_fields[9], len);
			rcd.msg[len]=0;

			vt.push_back(rcd);
		}
	}
	return TRUE;
}

BOOL CSqliteOp::GetRecordsetCount(const char *sql, long &lrecordcount)
{
	CDbSQLite::ResultTable res;
	DBExec_Rs(sql, res);
	lrecordcount = res.m_records.size();
	return TRUE;
}

BOOL CSqliteOp::GetAllDistinctDate(const char *sql, std::vector<long>& vtDate)
{
	CDbSQLite::ResultTable res;
	DBExec_Rs(sql, res);
	int nRecord = res.m_records.size();

	for (int nindex = 0; nindex <nRecord; nindex++)
	{
		CDbSQLite::ResultRecord *record = res.next();
		if (record)
		{
			long lDate = atol(record->m_fields[0]);
			vtDate.push_back(lDate);
		}
	}
	return TRUE;
}

BOOL CSqliteOp::GetAllAnalystUser(const char *sql, std::vector<CString>& vtUser)
{
	CDbSQLite::ResultTable res;
	DBExec_Rs(sql, res);
	int nRecord = res.m_records.size();

	for (int nindex = 0; nindex <nRecord; nindex++)
	{
		CDbSQLite::ResultRecord *record = res.next();
		if (record)
		{
			CString strUser = record->m_fields[0];
			vtUser.push_back(strUser);
		}
	}
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////////////////////////
//查询某个时间段里面的聊天记录数据 
// vtRecord 返回查询的集合 
// recordType 聊天类型 
// fromid 
// toid 
// datetimeBegin 起始时间 
// datetimeEnd   结束时间 
// roomID        房间ID  
//////////////////////////////////////////////////////////////////////////////////////////////
BOOL CSqliteOp::QueryTimeQuantumRecord(std::vector<CHAT_RECORD2> &vtRecord ,E_CHATRECORD_TYPE recordType ,LPCTSTR fromid /*= NULL*/, LPCTSTR toid /*= NULL*/, long datetimeBegin /*= 0*/, long datetimeEnd, long roomid /*= 0*/)
{
	CString strSql;
	switch(recordType)
	{
	case eRecord_Group:
		{
			strSql.Format("select * from chatrecord where (senddate between %d and %d) and (( fromid='%s' and toid='%s') or (fromid='%s' and toid='%s')) and type=%d and roomid=%d order by seq asc ",
				datetimeEnd,datetimeBegin, fromid, toid, toid, fromid, eRecord_Group, roomid);
		}
		break;
	case eRecord_Sys:
		{
			strSql.Format("select * from chatrecord where (senddate between %d and %d) and (( fromid='%s' and toid='%s') or (fromid='%s' and toid='%s')) and type=%d and roomid=%d order by seq asc ",
				datetimeEnd,datetimeBegin, fromid, toid, toid, fromid, eRecord_Sys, roomid);
		}
		break;
	case eRecord_Single:
		{
			strSql.Format("select * from chatrecord where (senddate between %d and %d) and (( fromid='%s' and toid='%s') or (fromid='%s' and toid='%s')) and type=%d and roomid=%d order by seq asc ",
				datetimeEnd,datetimeBegin, fromid, toid, toid, fromid, eRecord_Single, roomid);
		}
		break;
	case eRecord_TgRoom:
		{
			strSql.Format("select * from chatrecord where (senddate between %d and %d) and (( fromid='%s' and toid='%s') or (fromid='%s' and toid='%s')) and type=%d and roomid=%d order by seq asc ",
				datetimeEnd,datetimeBegin, fromid, toid, toid, fromid, eRecord_TgRoom, roomid);
		}
		break;
	default:
		return FALSE;
	}

	GetRecord(strSql, vtRecord);
	return TRUE;
}

// 在sqlite库中查询会话历史消息，每个ID取最新一条
BOOL CSqliteOp::QueryLastRecordByEachID(std::vector<CHAT_RECORD2> &vtRecord ,E_CHATRECORD_TYPE recordType ,LPCTSTR fromid /* = NULL */, LPCTSTR toid /* = NULL */, long datetimeBegin /* = 0 */, long datetimeEnd /* = 0 */, long roomid /* = 0 */)
{
	CString strSql;
	switch(recordType)
	{
	case eRecord_TgRoom:
		{
			strSql.Format("select * from chatrecord where (senddate between %d and %d) and (( fromid='%s' and toid='%s') or (fromid='%s' and toid='%s')) and type=%d and roomid=%d order by seq DESC ",
				datetimeEnd,datetimeBegin, fromid, toid, toid, fromid, eRecord_TgRoom, roomid);
		}
		break;
	default:
		return FALSE;
	}

	GetRecord(strSql, vtRecord);
	return TRUE;
}

// 按条件分页查询复合条件的记录
BOOL CSqliteOp::QueryOnePageRecord(std::vector<CHAT_RECORD2>& vtRecord, E_CHATRECORD_TYPE recordType, int nStart, int nEnd, LPCTSTR fromid /* = NULL */, LPCTSTR toid /* = NULL */, BOOL bLimitDate /* = FALSE */, long datetime /* = 0 */, long roomid /* = 0 */)
{
	CString strSql;
	switch(recordType)
	{
	case eRecord_Group:
		{
			if(!bLimitDate)
				strSql.Format("select * from chatrecord where toid='%s' and type=%d order by seq asc limit %d,%d",
				toid, eRecord_Group, nStart, nEnd-nStart);
			else
				strSql.Format("select * from chatrecord where senddate=%d and toid='%s' and type=%d order by seq asc limit %d,%d",
				datetime, toid, eRecord_Group, nStart, nEnd-nStart);
		}
		break;
	case eRecord_Sys:
		{
			if(!bLimitDate)
				strSql.Format("select * from chatrecord where toid='%s' and type=%d order by seq asc limit %d,%d",
				toid, eRecord_Sys, nStart, nEnd-nStart);
			else
				strSql.Format("select * from chatrecord where senddate=%d and toid='%s' and type=%d order by seq asc limit %d,%d",
				datetime, toid, eRecord_Sys, nStart, nEnd-nStart);
		}
		break;
	case eRecord_Single:
		{
			if(!bLimitDate)
				strSql.Format("select * from chatrecord where ( fromid='%s' or toid='%s' ) and type=%d order by seq asc limit %d,%d",
				fromid, toid, eRecord_Single, nStart, nEnd-nStart);
			else
				strSql.Format("select * from chatrecord where senddate=%d and ( fromid='%s' or toid='%s' ) and type=%d order by seq asc limit %d,%d",
				datetime, fromid, toid, eRecord_Single, nStart, nEnd-nStart);
		}
		break;
	case eRecord_TgRoom:
		{
			if(!bLimitDate)
				strSql.Format("select * from chatrecord where (( fromid='%s' and toid='%s') or (fromid='%s' and toid='%s')) and type=%d and roomid=%d order by seq asc limit %d,%d",
				fromid, toid, toid, fromid, eRecord_TgRoom, roomid, nStart, nEnd-nStart);
			else
				strSql.Format("select * from chatrecord where senddate=%d and (( fromid='%s' and toid='%s') or (fromid='%s' and toid='%s')) and type=%d and roomid=%d order by seq asc limit %d,%d",
				datetime, fromid, toid, toid, fromid, eRecord_TgRoom, roomid, nStart, nEnd-nStart);
		}
		break;
	default:
		return FALSE;
	}

	GetRecord(strSql, vtRecord);
	return TRUE;
}

// 查询总聊天记录的条数
long CSqliteOp::QueryRecordsetCount(E_CHATRECORD_TYPE recordType, LPCTSTR fromid /* = NULL */, LPCTSTR toid /* = NULL */, BOOL bLimitDate /* = FALSE */, long datetime /* = 0 */, long roomid /* = 0 */)
{
	CString strSql = _T("");
	long  nRecordsetCount = 0;
	switch(recordType)
	{
	case eRecord_Group:    
		{
			if(!bLimitDate)
				strSql.Format("select count(*) from chatrecord where toid='%s' and type=%d order by seq asc", toid, eRecord_Group);
			else
				strSql.Format("select count(*) from chatrecord where senddate=%d and toid='%s' and type=%d order by seq asc", datetime, toid, eRecord_Group);
		}
		break;
	case eRecord_Sys:
		{
			if(!bLimitDate)
				strSql.Format("select count(*) from chatrecord where toid='%s' and type=%d order by seq asc", toid, eRecord_Sys);
			else
				strSql.Format("select count(*) from chatrecord where senddate=%d and toid='%s' and type=%d order by seq asc", datetime, toid, eRecord_Sys);
		}
		break;
	case eRecord_Single:
		{
			if(!bLimitDate)
				strSql.Format("select count(*) from chatrecord where ( fromid='%s' or toid='%s' ) and type=%d order by seq asc",
				fromid, toid, eRecord_Single);
			else
				strSql.Format("select count(*) from chatrecord where senddate=%d and ( fromid='%s' or toid='%s' ) and type=%d order by seq asc",
				datetime, fromid, toid, eRecord_Single);
		}
		break;
	case eRecord_TgRoom:
		{
			if(!bLimitDate)
				strSql.Format("select * from chatrecord where (( fromid='%s' and toid='%s') or (fromid='%s' and toid='%s')) and type=%d and roomid=%d order by seq asc",
				fromid, toid, toid, fromid, eRecord_TgRoom, roomid);
			else
				strSql.Format("select * from chatrecord where senddate=%d and (( fromid='%s' and toid='%s') or (fromid='%s' and toid='%s')) and type=%d and roomid=%d order by seq asc",
				datetime, fromid, toid, toid, fromid, eRecord_TgRoom, roomid);
		}
		break;
	default:
		return -1;
	}

	GetRecordsetCount(strSql, nRecordsetCount);
	return nRecordsetCount;
}

// 查询有聊天记录的日期
BOOL CSqliteOp::QueryAllDistinctDate(std::vector<long>& vtDate, E_CHATRECORD_TYPE recordType, LPCTSTR fromid/* =NULL */, LPCTSTR toid/* =NULL */)
{
	CString strSql = _T("");
	switch(recordType)
	{
	case eRecord_Group:
		{
			strSql.Format("select distinct senddate from chatrecord where toid='%s' and type=%d order by seq asc", toid, eRecord_Group);
		}
		break;
	case eRecord_Sys:
		{
			strSql.Format("select distinct senddate from chatrecord where toid='%s' and type=%d order by seq asc", toid, eRecord_Sys);
		}
		break;
	case eRecord_Single:
		{
			strSql.Format("select distinct senddate from chatrecord where ( fromid='%s' or toid='%s' ) and type=%d order by seq asc", fromid, toid, eRecord_Single);
		}
		break;
	default:
		return FALSE;
	}

	GetAllDistinctDate(strSql ,vtDate);
	return TRUE;
}


// 查询分析师房间下与其交流过的用户
BOOL CSqliteOp::QueryAllAnalystUser(std::vector<CString>& vtUser, E_CHATRECORD_TYPE recordType, long roomid /* = 0 */, LPCTSTR fromid /* = NULL */, LPCTSTR toid /* = NULL */)
{
	CString strSql = _T("");
	switch(recordType)
	{
	case eRecord_TgRoom:
		{
			if (roomid == 0)
			{
				strSql.Format("select distinct fromid from chatrecord where type=%d order by seq asc limit 0,30", eRecord_TgRoom);
			}
			else
			{
				strSql.Format("select distinct fromid from chatrecord where roomid=%d and type=%d order by seq desc", roomid, eRecord_TgRoom);
			}
		}
		break;
	case eRecord_NewPublic:
		{
			if (roomid == 0)
			{
				strSql.Format("select distinct fromid from chatrecord where type=%d order by seq asc limit 0,30", eRecord_NewPublic);
			}
			else
			{
				strSql.Format("select distinct fromid from chatrecord where roomid=%d and type=%d order by seq desc", roomid, eRecord_NewPublic);
			}
		}
		break;

	default:
		return FALSE;
	}

	GetAllAnalystUser(strSql ,vtUser);
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
CCharConv::CCharConv(void)
	:m_char(NULL)  
	,m_wchar(NULL) 
{
}

CCharConv::~CCharConv(void)
{
	Release();
}

char* CCharConv::WcharToChar(const wchar_t* wp)  
{
	Release();
	int len = WideCharToMultiByte(CP_ACP, 0, wp, wcslen(wp), NULL, 0, NULL, NULL);  
	m_char = new char[len+1];  
	WideCharToMultiByte(CP_ACP, 0, wp, wcslen(wp), m_char, len, NULL, NULL);  
	m_char[len] = '\0';  
	return m_char;  
}

wchar_t* CCharConv::CharToWchar(const char* c)  
{
	Release();
	int len = MultiByteToWideChar(CP_ACP, 0 , c, strlen(c), NULL, 0);  
	m_wchar = new wchar_t[len+1];  
	MultiByteToWideChar(CP_ACP, 0, c,strlen(c), m_wchar, len);  
	m_wchar[len] = '\0';  
	return m_wchar;  
}  

char* CCharConv::StringToChar(const string& s)  
{  
	return const_cast<char*>(s.c_str());  
}

wchar_t* CCharConv::StringToWchar(const string& s)  
{
	const char* p = s.c_str();  
	return CharToWchar(p);  
}

wchar_t* CCharConv::WstringToWchar(const std::wstring &ws)  
{  
	return const_cast<wchar_t*>(ws.c_str());  
}   

char* CCharConv::WstringToChar(const std::wstring &ws)  
{  
	const wchar_t* wp = ws.c_str();  
	return WcharToChar(wp);  
}

string CCharConv::WstringToString(const std::wstring &ws)  
{  
	string s;  
	char* p = WstringToChar(ws);  
	s.append(p);  
	return s;  
}

wstring CCharConv::StringToWstring(const std::string &s)  
{  
	wstring ws;  
	wchar_t* wp = StringToWchar(s);  
	ws.append(wp);  
	return ws;  
}

void CCharConv::Release()
{  
	if(m_char)  
	{  
		delete m_char;  
		m_char=NULL;  
	}  
	if(m_wchar)  
	{  
		delete m_wchar;  
		m_wchar=NULL;  
	}  
}  