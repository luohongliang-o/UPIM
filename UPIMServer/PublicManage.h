#pragma once
#include <list>
const int MAX_CLIENTNUM = 5;

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

struct PublicInfo
{
	int    id;                  //  公众号ID
	char   name[50];            //  公众号名称
	double createtime;          //  创建时间
	double deletetime;          //  删除时间
	double modifytime;          //  修改时间
	int    usertype;            //  可见用户范围(默认1:所有用户)
	char   createuserid[USERID_SIZE]; //  创建者账号
	char   comment[CHAR_MAX];   //  公众号标签
	char   wellcomcontent[CHAR_MAX];   //  公众号欢迎内容
};

struct Incode
{
	int   id;                  //  接入码
	char  name[32];            //  接入码名称(接入问题)
	BOOL  delflag;             //  有效标识
	int   dispatchtype;        //  调度规则    1:最大空闲时间调度 2:最短会话时间调度 3:最高优先级调度
};


struct TpRule
{
	int    id;                  //  key
	int    typtype;             //  路由规则类型 1:时间路由 2:问题类型路由 3:接口路由 4:直接客服路由 5:扩展路由 
	char   typname[50];         //  路由名称
	int    starttime;           //  起始时间点
	int    timeincreament;      //  时间间隔
	short  problemtype;         //  问题类型优先
	char   interfacename[50];   //  指定接口名路由
	char   customerservice[USERID_SIZE]; //  指定客服路由
	BOOL   delflag;             //  有效标识
	char   ruleex[CHAR_MAX];    //  扩展规则
};

struct TpRelation
{
	int   publicid;             //  公众号ID
	int   tpid;                 //  路由规则id
};

struct TpInCodeRelation
{
	int   id;                 //  路由规则id
	int   incode;               //  接入码
};

struct DispatchRelation
{
	int   incodeid;                      //  接入码
	long  customerserviceid;             //  客服id
	char  customerservice[USERID_SIZE];   //客服工号
	int   priority;                      //  客户服优先级
	int   delflag;                       //  删除标识
};

struct CustomerServiceInfo
{
	long id;
	char customerservice[USERID_SIZE];
	long sessiontime;
	long idletime;
	int  priority;
	int  clientnum;             //  一个客服同时服务客户数
	BOOL bstate;                //  客服状态0:空闲状态 1:会话状态
	char username[USERID_SIZE]; //  最近一次服务过的客户
	int  disType;               //  调度类型
	MYSTATE cusState;           //  客服上下线状态
	CustomerServiceInfo()
	{
		id = 0;
		COPYSTRARRAY(customerservice,"");
		sessiontime = 0;
		idletime = 0;
		priority = 0;
		clientnum =0;
		bstate = 0;
		COPYSTRARRAY(username,"");
		disType = 0;
		cusState = (MYSTATE)0;
	}
	int CmpByDisType(CustomerServiceInfo & other,int type)
	{
		if (type == DISPATCH_MAX_IDLETIME)
			return idletime - other.idletime;
		if (type == DISPATCH_MAX_PRIORITY)
			return priority - other.priority;
		if (type == DISPATCH_MIN_SESSIONTIME)
			return -(sessiontime - other.sessiontime);
	}
};
class CPublicManage;

// class CPublic
// {
// public:
// 	CPublic(){};
// 	CPublic(CPublicManage* pbManage,int nPublic,int nTp):
// 	m_nPublic(nPublic),m_nTp(nTp),m_pPbManage(pbManage){};
// 	~CPublic();
// 	int GetIncode(int& nDis);
// 
// private:
// 	int   m_nPublic;
// 	int   m_nTp;
// 	CPublicManage* m_pPbManage;
// };

// class CIncode
// {
// public:
// 	CIncode();
// 	CIncode(int nIncode,int nDis){m_nIncode = nIncode,m_nDis = nDis};
// 	~CIncode();
// 	int GetCustomer();
// 	
// private:
// 	int m_nIncode;
// 	int m_nDis;
// };
class CUser;

class CCustomer
{
public:
	enum {
		TIME_DECREASE = -1
		,TIME_INIT
		,TIME_ADD
	};
	CCustomer(){};
	CCustomer(char* szCust):
	m_lid(0)
	,m_lSessionTime(0)
	,m_lIdleTime(0)
	,m_iPriority(0)
	,m_iClientnum(0)
	,m_bState(FALSE)
	,m_iDisType(0)
	,m_eCusState((MYSTATE)0)
	{
		strcpy(m_szUserName,"");
		if (szCust == NULL){
			m_pCust = 0;
		}
		else{
			m_pCust = new char[strlen(szCust)+1];
			strcpy(m_pCust,szCust);
		}	
	};
	~CCustomer()
	{
		delete [] m_pCust;
		m_pCust = NULL;
	};

	void ReSetIdleTime()           { m_lIdleTime = TIME_INIT ;};
	void ReSetSessionTime()        { m_lSessionTime = TIME_INIT;};
	void Update(CUser* pUser);
	
	void SetCustID(long ID)        { m_lid = ID ;};
	long GetCustID()               { return m_lid; }; 

	void AddIdleTime()             { m_lIdleTime += TIME_ADD;};
	void DecreaseIdleTime()        { m_lIdleTime += TIME_DECREASE;}

	void AddSessionTime()          { m_lSessionTime += TIME_ADD;};
	void DecreaseSessionTime()     { m_lSessionTime += TIME_DECREASE;}

	void SetPriority(int priority) { m_iPriority = priority;};
	
	void SetClientNum(int nClient) { m_iClientnum = nClient;};
	int  GetClientNum()            { return m_iClientnum;};

	void SetSessionState(BOOL bState) { m_bState = bState;};
	BOOL GetSessionState()         { return m_bState;};

	void SetLastUserName(char* userName){COPYSTRARRAY(m_szUserName,userName);};
	char* GetLastUserName()        { return m_szUserName;}

	void SetDisType(int disType)   { m_iDisType = disType;};
	int  GetDisType()              { return m_iDisType;};

	void SetCustState(MYSTATE myState) { m_eCusState = myState;};
	MYSTATE GetCustState()         {return m_eCusState;};
	
	void SetCurIncode(int incode)  { m_iCurIncodeID = incode;};
	int  GetCurIncode()            { return m_iCurIncodeID;};

	char* GetName(){return m_pCust;};
	BOOL operator ==(CCustomer &ohter)
	{
		if (this == &ohter) return TRUE;
		else return FALSE;
	};

	int CmpByDisType(CCustomer* other,int type)
	{
		if (type == DISPATCH_MAX_IDLETIME)
			return m_lIdleTime - other->m_lIdleTime;
		if (type == DISPATCH_MAX_PRIORITY)
			return m_iPriority - other->m_iPriority;
		if (type == DISPATCH_MIN_SESSIONTIME)
			return other->m_lSessionTime - m_lSessionTime ;
		return 0;
	};

private:
	CPublicManage*       m_pPbManage;
	char*                m_pCust;

	long                 m_lid;
	long                 m_lSessionTime;
	long                 m_lIdleTime;
	int                  m_iPriority;
	int                  m_iClientnum;               //  一个客服同时服务客户数
	BOOL                 m_bState;                   //  客服状态0:空闲状态 1:会话状态
	char                 m_szUserName[USERID_SIZE];  //  最近一次服务过的客户
	int                  m_iDisType;                 //  调度类型
	MYSTATE              m_eCusState;                //  客服上下线状态
	int                  m_iCurIncodeID;
};

class CUser
{
public:
	CUser(){ m_bUserState = TRUE; };
	CUser(char* strUser)
	{
		if (strUser == NULL){
			m_pUser = 0;
		}
		else{
			m_pUser = new char[strlen(strUser)+1];
			strcpy(m_pUser,strUser);
		}
	};
	~CUser()
	{
		delete [] m_pUser;
		m_pUser = NULL;

	};
	void Attach(CCustomer*);
	void Detach(CCustomer*);
	void Notify();
	BOOL GetState();
	void SetState(BOOL bState);
	char* GetName(){return m_pUser;};
	BOOL IsExistCustomer(char* szCust);
	
	void SetCurPublic(int PublicID) { m_iCurPublicID = PublicID; };
	int  GetCurPublic()             { return m_iCurPublicID;};

	void SetCurTp( int TpId)        { m_iCurTPID = TpId; };
	int  GetCurTp()                 { return m_iCurTPID;};

	CCustomer* GetCurCustomer()     { return m_ListCust.back();};
private:
	std::list<CCustomer*> m_ListCust;
	BOOL                  m_bUserState;
	char*                 m_pUser;
	int                   m_iCurPublicID;
	int                   m_iCurTPID;
};

template<typename ST>
class find_id
{
public:
	find_id(const int &id):__id(id){}
	BOOL operator()(const ST &st) const 
	{return st.id == __id;}
private:
	int __id;
};

class CPublicManage
{
	//friend CPublic;
	friend CCustomer;
public:
	CPublicManage();
	~CPublicManage();
	
	LONG Init(CADODatabase* pAdoDb);			// 初始化，将数据库里面的各种信息载入到服务器
	VOID Uninit();

	char* GetServiceID(int nPublicID, char* szUserID,int tpid = 0);			// 传入公众号ID，匹配一个 可用的客服抛出
	void  SetCurUserID(const char* userID);
	void  GetCustomerState(const char* CusID,MYSTATE &state);
	typedef BOOL (*ApiInvoke)(int nIncodeID,va_list args);
	ApiInvoke m_pInvokeApi;
	void      StopIdleThread();
	HANDLE    StartIdleThread();
	void  ConnectClose(char* userID,char* Customer);
	void  SetCustomerState(char* Name,MYSTATE eMystate);

protected:
	//获取基础数据
	LONG GetPublicInfo(CADODatabase* pAdoDb);
	LONG GetIncode(CADODatabase* pAdoDb);
	LONG GetTpRule(CADODatabase* pAdoDb);
	LONG GetTpRelation(CADODatabase* pAdoDb);
	LONG GetTpInCodeRelation(CADODatabase* pAdoDb);
	LONG GetDispatchRelation(CADODatabase* pAdoDb);
	// 路由过程
	BOOL TimeTP(TpRule* ptprule);
	BOOL ProblemTP(int tpid);
	BOOL InterfaceTP(TpRule* ptprule);
	BOOL DirectCustomerService(TpRule* ptprule);
	// 调度过程
	BOOL DispatchToCustomerService(Incode* pstincode);
	BOOL IdleMaxDisptach(int id);
	BOOL SessionMinDispatch(int id);
	BOOL PriorityMaxDispatch(int id );
	
	BOOL IsCustomerServiceIdle(char* strcustomerservice);

	void RestCustomerServiceIDLE();
	
	CCustomer* GetCurMaxDispatchCustomer(int id,int disPatchType);

private: 
	static UINT WINAPI StatCusTime(LPVOID pv);
protected:

	std::vector<PublicInfo>       m_vPublicInfo;
	std::vector<Incode>           m_vIncode;
	std::vector<TpRule>           m_vTpRule;
	std::vector<TpRelation>       m_vTpRelation;
	std::vector<TpInCodeRelation> m_vTpIncodeRelation;
	std::vector<DispatchRelation> m_vDispatchRelation;
	
	int                           m_nCurProblemtype;
	char                          m_pstrActiveID[USERID_SIZE];
	HANDLE                        m_hThread ;
	std::list<CCustomer*>         m_listCustomer;
	std::list<CUser*>             m_listUser;
	CCustomer*                    m_pCurCustomer;

	int                           m_iCurPublic;
	int                           m_iCurTp;
	char                          m_szCurUser[USERID_SIZE];
};