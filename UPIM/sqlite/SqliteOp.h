#pragma once

class CSqliteOp
{
public:
	CSqliteOp(void);
	~CSqliteOp(void);

	BOOL DBOpen();
	BOOL DBClose();
	BOOL DBExec(const char* sql);
	BOOL DBExec_Rs(const char* sql, CDbSQLite::ResultTable& res);
	BOOL DBCreate();
	BOOL DBInsertRecordRaw(LPCSTR fromid, LPCSTR toid, int nroomid, long senddate, long sendtime, int type, int format, const char* msg);
	BOOL DBInsertRecord(LPCSTR fromid, LPCSTR toid, int nroomid, int type, int format, const char* msg);
	BOOL DBInsertRecord(LPCSTR fromid, LPCSTR toid, int nroomid, long sendtime, int type, int format, const char* msg);
	BOOL DBInsertGRecord(LPCSTR fromid, long roomid, int sendtime, const char* msg);

	BOOL GetRecord(const char *sql, std::vector<CHAT_RECORD2> &vt);
	BOOL GetRecordsetCount(const char *sql, long &lrecordcount);
	BOOL GetAllDistinctDate(const char *sql, std::vector<long>& vtDate);
	BOOL GetAllAnalystUser(const char *sql, std::vector<CString>& vtUser);


	//////////////////////////////////////////////////////////////////////////
	// 查询本地数据库
	// 查询与某个用户，群组的聊天记录的总个数
	long QueryRecordsetCount(E_CHATRECORD_TYPE recordType, LPCTSTR fromid = NULL, LPCTSTR toid = NULL, BOOL bLimitDate = FALSE, long datetime = 0, long roomid = 0); 
	// 从本地数据库中查询一页的数据到vtRecord中
	BOOL QueryOnePageRecord(std::vector<CHAT_RECORD2>& vtRecord, E_CHATRECORD_TYPE recordType, int nStart, int nEnd, LPCTSTR fromid = NULL, LPCTSTR toid = NULL, BOOL bLimitDate = FALSE, long datetime = 0, long roomid = 0);    
	// 查询与某个人，群组聊天的全部不同的日期(YYYYMMDD)
	BOOL QueryAllDistinctDate(std::vector<long>& vtDate, E_CHATRECORD_TYPE recordType, LPCTSTR fromid = NULL, LPCTSTR toid = NULL); 
	// 查询分析师房间下与其交流过的用户
	BOOL QueryAllAnalystUser(std::vector<CString>& vtUser, E_CHATRECORD_TYPE recordType, long roomid = 0, LPCTSTR fromid = NULL, LPCTSTR toid = NULL);
	// 查询某个时间段的数据到vtRecord中 
	BOOL QueryTimeQuantumRecord(std::vector<CHAT_RECORD2> &vtRecord ,E_CHATRECORD_TYPE recordType ,LPCTSTR fromid = NULL, LPCTSTR toid = NULL, long datetimeBegin = 0, long datetimeEnd = 0, long roomid = 0) ; 
	// 在sqlite库中查询会话历史消息，每个ID取最新一条
	BOOL QueryLastRecordByEachID(std::vector<CHAT_RECORD2> &vtRecord ,E_CHATRECORD_TYPE recordType ,LPCTSTR fromid = NULL, LPCTSTR toid = NULL, long datetimeBegin = 0, long datetimeEnd = 0, long roomid = 0) ; 
public:
	CDbSQLite m_sqlite;

};

//////////////////////////////////////////////////////////////////////////
class CCharConv
{
public:
	CCharConv(void);
	~CCharConv(void);

	char*	WcharToChar(const wchar_t* wp);  
	char*	StringToChar(const string& s);  
	char*	WstringToChar(const wstring& ws);  
	wchar_t* CharToWchar(const char* c);  
	wchar_t* WstringToWchar(const wstring& ws);  
	wchar_t* StringToWchar(const string& s);  
	wstring	StringToWstring(const string& s);  
	string WstringToString(const wstring& ws);  
	void Release();  
private:  
	char* m_char;  
	wchar_t* m_wchar;  
};
