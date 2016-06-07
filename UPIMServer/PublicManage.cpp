#include "StdAfx.h"
#include "PublicManage.h"
#include "Func.h"
#include "MainFrm.h"
#include "ThreadFunc.h"

LONG g_MainFrmThreadState;

CPublicManage::CPublicManage()
{
	m_hThread = NULL;
}

CPublicManage::~CPublicManage()
{
	std::list<CUser*>::iterator itpUser = m_listUser.begin();
	for (;itpUser != m_listUser.end();){
		delete (*itpUser);
		m_listUser.erase(itpUser++);
	}

	std::list<CCustomer*>::iterator itpCust = m_listCustomer.begin();
	for (;itpCust != m_listCustomer.end();)
	{
 		delete (*itpCust);
		m_listCustomer.erase(itpCust++);
	}
}

LONG CPublicManage::Init(CADODatabase* pAdoDb)
{
	LONG lRet = eGCError_Success;

	if(eGCError_Success != (lRet = GetPublicInfo(pAdoDb))) {
		printf("--------公众号获取失败！\n");
		return lRet;
	}
	if(eGCError_Success != (lRet = GetIncode(pAdoDb))) {
		printf("--------接入码获取失败！\n");
		return lRet;
	}
	if(eGCError_Success != (lRet = GetTpRule(pAdoDb))) {
		printf("--------路由规则表获取失败！\n");
		return lRet;
	}
	if(eGCError_Success != (lRet = GetTpRelation(pAdoDb))) {
		printf("--------公众号对应路由规则关系表获取失败！\n");
		return lRet;
	}
	if(eGCError_Success != (lRet = GetTpInCodeRelation(pAdoDb))) {
		printf("--------路由规则对应接入码关系表获取失败！\n");
		return lRet;
	}
	if(eGCError_Success != (lRet = GetDispatchRelation(pAdoDb))) {
		printf("--------调度关系表获取失败！\n");
		return lRet;
	}

	return lRet;
}

LONG CPublicManage::GetPublicInfo(CADODatabase* pAdoDb)
{
	LONG lRet = eGCError_Success;
	if (!pAdoDb) return eGCError_DbCon;
	if (!pAdoDb->IsOpen()) return eGCError_DbClose;
	BOOL bRecord = FALSE;
	CString selectStr = _T("");
	CADORecordset adoRs(pAdoDb);
	CADOCommand adoCmd(pAdoDb,"select * from publicinfo where delflag = 0;",adCmdText);
	do {
		if (!adoRs.Execute(&adoCmd)) { lRet = eGCError_DbExe;break;}
		if(!adoRs.IsOpen()) break;
		long nRecordCount = adoRs.GetRecordCount();
		for (long nIdx=0;nIdx<nRecordCount;nIdx++,adoRs.MoveNext()){
			bRecord = TRUE;
			long fieldCount = adoRs.GetFieldCount();
			CString strValue = _T("");
			std::vector<CString > vtRecord;
			for (long fieldIdx = 0;fieldIdx<fieldCount;fieldIdx++){
				adoRs.GetFieldValue(fieldIdx,strValue);
				vtRecord.push_back(strValue);
			}
			PublicInfo publicinfo;
			publicinfo.id              = StrToInt((LPCSTR)vtRecord[0]);           //  公众号ID
			COPYSTRARRAY(publicinfo.name,(LPCSTR)vtRecord[1]);					  //  公众号名称
			publicinfo.createtime      = StrToOleDateTime((LPCSTR)vtRecord[2]);   //  创建时间
			publicinfo.deletetime      = StrToOleDateTime((LPCSTR)vtRecord[3]);   //  删除时间
			publicinfo.modifytime      = StrToOleDateTime((LPCSTR)vtRecord[4]);   //  修改时间
			publicinfo.usertype        = StrToInt((LPCSTR)vtRecord[5]);		      //  可见用户范围(默认1:所有用户)
			COPYSTRARRAY(publicinfo.createuserid ,(LPCSTR)vtRecord[6]);		      //  创建者账号
			COPYSTRARRAY(publicinfo.comment,(LPCSTR)vtRecord[7]);				  //  公众号标签
			COPYSTRARRAY(publicinfo.wellcomcontent,(LPCSTR)vtRecord[8]);	      //  公众号欢迎内容
			m_vPublicInfo.push_back(publicinfo);
		}
	}while(FALSE);
	if (!bRecord)
		lRet = eGCError_RsNULL;
	return lRet;
}

LONG CPublicManage::GetIncode(CADODatabase* pAdoDb)
{
	LONG lRet = eGCError_Success;
	if (!pAdoDb) return eGCError_DbCon;
	if (!pAdoDb->IsOpen()) return eGCError_DbClose;
	BOOL bRecord = FALSE;
	CADORecordset adoRs(pAdoDb);
	CADOCommand adoCmd(pAdoDb,"select * from public_incode where delflag = 0;",adCmdText);
	do {
		if (!adoRs.Execute(&adoCmd)) { lRet = eGCError_DbExe;break;}
		if(!adoRs.IsOpen()) break;
		long nRecordCount = adoRs.GetRecordCount();
		for (long nIdx=0;nIdx<nRecordCount;nIdx++,adoRs.MoveNext()){
			bRecord = TRUE;
			long fieldCount = adoRs.GetFieldCount();
			CString strValue = _T("");
			std::vector<CString > vtRecord;
			for (long fieldIdx = 0;fieldIdx<fieldCount;fieldIdx++){
				adoRs.GetFieldValue(fieldIdx,strValue);
				vtRecord.push_back(strValue);
			}
			Incode incode;
			incode.id                  = StrToInt((LPCSTR)vtRecord[0]);          //  接入码
			COPYSTRARRAY(incode.name,(LPCSTR)vtRecord[1]);					     //  接入码名称(接入问题)
			incode.delflag             = StrToInt((LPCSTR)vtRecord[2]);          //  有效标识
			incode.dispatchtype        = StrToInt((LPCSTR)vtRecord[3]);          //  调试规则类型
			m_vIncode.push_back(incode);
		}

	}while(FALSE);
	if (!bRecord)
		lRet = eGCError_RsNULL;
	return lRet;
}

LONG CPublicManage::GetTpRule(CADODatabase* pAdoDb)
{
	LONG lRet = eGCError_Success;
	if (!pAdoDb) return eGCError_DbCon;
	if (!pAdoDb->IsOpen()) return eGCError_DbClose;
	BOOL bRecord = FALSE;
	CString selectStr = _T("");
	CADORecordset adoRs(pAdoDb);
	CADOCommand adoCmd(pAdoDb,"select * from public_tprule where delflag = 0;",adCmdText);
	do {
		if (!adoRs.Execute(&adoCmd)) { lRet = eGCError_DbExe;break;}
		if(!adoRs.IsOpen()) break;
		long nRecordCount = adoRs.GetRecordCount();
		for (long nIdx=0;nIdx<nRecordCount;nIdx++,adoRs.MoveNext()){
			bRecord = TRUE;
			long fieldCount = adoRs.GetFieldCount();
			CString strValue = _T("");
			std::vector<CString > vtRecord;
			for (long fieldIdx = 0;fieldIdx<fieldCount;fieldIdx++){
				adoRs.GetFieldValue(fieldIdx,strValue);
				vtRecord.push_back(strValue);
			}
			TpRule temptprule;			
			temptprule.id                  = StrToInt((LPCSTR)vtRecord[0]);         //  key
			temptprule.typtype             = StrToInt((LPCSTR)vtRecord[1]);		    //  路由规则类型 
			COPYSTRARRAY(temptprule.typname,vtRecord[2].GetBuffer(0));          	//  路由名称
			temptprule.starttime           = StrToInt((LPCSTR)vtRecord[3]);		    //  起始时间点
			temptprule.timeincreament      = StrToInt((LPCSTR)vtRecord[4]);		    //  时间间隔
			temptprule.problemtype         = StrToInt((LPCSTR)vtRecord[5]);		    //  问题类型优先
			COPYSTRARRAY(temptprule.interfacename,(LPCSTR)vtRecord[6]);			    //  指定接口名路由
			COPYSTRARRAY(temptprule.customerservice,(LPCSTR)vtRecord[7]);					//  指定客服路由
			temptprule.delflag             = StrToInt((LPCSTR)vtRecord[8]);		    //  有效标识
			strncpy(temptprule.ruleex,(LPCSTR)vtRecord[9],vtRecord[9].GetLength()); //  扩展规则
			m_vTpRule.push_back(temptprule);
		}
	}while(FALSE);
	if (!bRecord)
		lRet = eGCError_RsNULL;
	return lRet;
}

LONG CPublicManage::GetTpRelation(CADODatabase* pAdoDb)
{
	LONG lRet = eGCError_Success;
	if (!pAdoDb) return eGCError_DbCon;
	if (!pAdoDb->IsOpen()) return eGCError_DbClose;
	BOOL bRecord = FALSE;
	CString selectStr = _T("");
	CADORecordset adoRs(pAdoDb);
	CADOCommand adoCmd(pAdoDb,"select * from public_tprelation ;",adCmdText);
	do {
		if (!adoRs.Execute(&adoCmd)) { lRet = eGCError_DbExe;break;}
		if(!adoRs.IsOpen()) break;
		long nRecordCount = adoRs.GetRecordCount();
		for (long nIdx=0;nIdx<nRecordCount;nIdx++,adoRs.MoveNext()){
			bRecord = TRUE;
			long fieldCount = adoRs.GetFieldCount();
			CString strValue = _T("");
			std::vector<CString > vtRecord;
			for (long fieldIdx = 0;fieldIdx<fieldCount;fieldIdx++){
				adoRs.GetFieldValue(fieldIdx,strValue);
				vtRecord.push_back(strValue);
			}
			TpRelation tprelation;
			tprelation.publicid      = StrToInt((LPCSTR)vtRecord[0]);            //  公众号ID
			tprelation.tpid          = StrToInt((LPCSTR)vtRecord[1]);            //  路由规则id
			m_vTpRelation.push_back(tprelation);
		}
	}while(FALSE);
	if (!bRecord)
		lRet = eGCError_RsNULL;
	return lRet;
}

LONG CPublicManage::GetTpInCodeRelation(CADODatabase* pAdoDb)
{
	LONG lRet = eGCError_Success;
	if (!pAdoDb) return eGCError_DbCon;
	if (!pAdoDb->IsOpen()) return eGCError_DbClose;
	BOOL bRecord = FALSE;
	CString selectStr = _T("");
	CADORecordset adoRs(pAdoDb);
	CADOCommand adoCmd(pAdoDb,"select * from public_tpincoderelation ;",adCmdText);
	do {
		if (!adoRs.Execute(&adoCmd)) { lRet = eGCError_DbExe;break;}
		if(!adoRs.IsOpen()) break;
		long nRecordCount = adoRs.GetRecordCount();
		for (long nIdx=0;nIdx<nRecordCount;nIdx++,adoRs.MoveNext()){
			bRecord = TRUE;
			long fieldCount = adoRs.GetFieldCount();
			CString strValue = _T("");
			std::vector<CString > vtRecord;
			for (long fieldIdx = 0;fieldIdx<fieldCount;fieldIdx++){
				adoRs.GetFieldValue(fieldIdx,strValue);
				vtRecord.push_back(strValue);
			}
			TpInCodeRelation tpincoderelation;
			tpincoderelation.id          = StrToInt((LPCSTR)vtRecord[0]);            //  路由规则id
			tpincoderelation.incode      = StrToInt((LPCSTR)vtRecord[1]);              //  接入码
			m_vTpIncodeRelation.push_back(tpincoderelation);
		}
	}while(FALSE);
	if (!bRecord) lRet = eGCError_RsNULL;
	return lRet;
}

LONG CPublicManage::GetDispatchRelation(CADODatabase* pAdoDb)
{
	LONG lRet = eGCError_Success;
	if (!pAdoDb) return eGCError_DbCon;
	if (!pAdoDb->IsOpen()) return eGCError_DbClose;
	BOOL bRecord = FALSE;
	CString selectStr = _T("");
	CADORecordset adoRs(pAdoDb);

	CADOCommand adoCmd(pAdoDb,"select * from public_dispatchrelation ;",adCmdText);
	do {
		if (!adoRs.Execute(&adoCmd)) { lRet = eGCError_DbExe;break;}
		if(!adoRs.IsOpen()) break;
		long nRecordCount = adoRs.GetRecordCount();
		for (long nIdx=0;nIdx<nRecordCount;nIdx++,adoRs.MoveNext()){
			bRecord = TRUE;
			long fieldCount = adoRs.GetFieldCount();
			CString strValue = _T("");
			std::vector<CString > vtRecord;
			for (long fieldIdx = 0;fieldIdx<fieldCount;fieldIdx++){
				adoRs.GetFieldValue(fieldIdx,strValue);
				vtRecord.push_back(strValue);
			}
			DispatchRelation disprelation;
			disprelation.incodeid        = StrToInt((LPCSTR)vtRecord[0]);              //  接入码
			disprelation.customerserviceid = StrToInt((LPCSTR)vtRecord[1]);            //  客服id
			COPYSTRARRAY(disprelation.customerservice,(LPCSTR)vtRecord[2].Trim());     //  客服工号
			disprelation.priority        = StrToInt((LPCSTR)vtRecord[3]);              //  客户服优先级
			disprelation.delflag         = StrToInt((LPCSTR)vtRecord[4]);              //  删除标识
			m_vDispatchRelation.push_back(disprelation);
			std::vector<Incode>::iterator id_Incode 
				= find_if(m_vIncode.begin(),
				m_vIncode.end(),find_id<Incode>(disprelation.incodeid));
			CCustomer* custObj = new CCustomer(disprelation.customerservice);
			custObj->SetCustID(disprelation.customerserviceid);
			custObj->SetPriority(disprelation.priority);
			custObj->SetDisType(id_Incode->dispatchtype);
			m_listCustomer.push_back(custObj );
		}
	}while(FALSE);
	if (!bRecord)
		lRet = eGCError_RsNULL;
	return lRet;
}

VOID CPublicManage::Uninit()
{
	std::list<CUser*>::iterator itpUser = m_listUser.begin();
	for (;itpUser != m_listUser.end();){
		delete (*itpUser);
		m_listUser.erase(itpUser++);
	}

	std::list<CCustomer*>::iterator itpCust = m_listCustomer.begin();
	for (;itpCust != m_listCustomer.end();)
	{
		delete (*itpCust);
		m_listCustomer.erase(itpCust++);
	}

	StopIdleThread();
	return;
}

char* CPublicManage::GetServiceID(int nPublicID, char* szUserID, int tpid /* = 0 */)
{
	CLock m_csLock;
	CAutoLock autolock(&m_csLock);
	m_iCurPublic = nPublicID;
	COPYSTRARRAY(m_szCurUser,szUserID);
	m_iCurTp = tpid > 0?tpid:-1;

	BOOL bRet = FALSE;
	TpRelation tprelation;
	COPYSTRARRAY(m_pstrActiveID,"");
	for (int i = 0;i<m_vTpRelation.size();i++){//遍历公众号对应路由关系表{
		tprelation = m_vTpRelation[i];
		if (nPublicID == tprelation.publicid){
			if (tpid == -1) m_iCurTp = tprelation.tpid;
			std::vector<TpRule>::iterator it_tprule =
				find_if(m_vTpRule.begin(),m_vTpRule.end(),find_id<TpRule>(m_iCurTp));
			if (it_tprule != m_vTpRule.end()){
				switch(it_tprule->typtype){
				case TP_TIME:
					bRet = TimeTP(&(*it_tprule));
					break;
				case TP_PROBLEM:
					bRet = ProblemTP(m_iCurTp);
					break;
				case TP_INTERFACE:
					bRet = InterfaceTP(&(*it_tprule));
					break;
				case TP_CUSTOMERSERVICE:
					bRet = DirectCustomerService(&(*it_tprule));
					break;
				case TP_OTHER:
				default:
					bRet = FALSE;
					break;
				}
			}
		}
		if(bRet) break;
	}

	if (bRet)
	{
		if(m_listUser.size()){
			CUser *pCurUser = m_listUser.back();
			if(!strcmp(pCurUser->GetName(),szUserID)) 
				return pCurUser->GetCurCustomer()->GetName();
		}
		
		CUser *pUser = new CUser(szUserID);
		pUser->Attach(m_pCurCustomer);
		pUser->SetCurPublic(nPublicID);
		pUser->SetCurTp(m_iCurTp);
		m_listUser.push_back(pUser);	

		COPYSTRARRAY(m_pstrActiveID,m_pCurCustomer->GetName());
		
	}
	return m_pstrActiveID;
}

BOOL CPublicManage::TimeTP(TpRule* ptprule)
{	
	BOOL bRet = FALSE;
	
	COleDateTime dtime = COleDateTime::GetCurrentTime();
	int curtime =  dtime.GetHour()*60*60 + dtime.GetMinute()*60 + dtime.GetSecond();
	int _starttime = ptprule->starttime/10000*60*60 + 
		(ptprule->starttime/100)%100 * 60 + ptprule->starttime%100;
	int endtime = ptprule->starttime/10000*60*60 + 
		(ptprule->starttime/100)%100 * 60 + ptprule->starttime%100 + ptprule->timeincreament;
	if (curtime <endtime && curtime >= _starttime) {
		for (int i = 0;i<m_vTpIncodeRelation.size();i++){//遍历路由对应接入码关系表
			TpInCodeRelation tpincoderelation = m_vTpIncodeRelation[i];
			if (ptprule->id ==  tpincoderelation.id){
				std::vector<Incode>::iterator vIncode = 
					find_if(m_vIncode.begin(),m_vIncode.end(),find_id<Incode>(tpincoderelation.incode));
				bRet = DispatchToCustomerService(&(*vIncode));//统一调度入口
			}
			if (bRet) break;
		}
	}
	return bRet ;
}

BOOL CPublicManage::ProblemTP(int tpid)
{
	BOOL bRet = FALSE;
	for(int i = 0;i<m_vTpIncodeRelation.size();i++){
		if (m_vTpIncodeRelation[i].id == tpid){
			std::vector<Incode>::iterator vIncode = 
				find_if(m_vIncode.begin(),m_vIncode.end(),find_id<Incode>(m_vTpIncodeRelation[i].incode));
			bRet = DispatchToCustomerService(&(*vIncode));//统一调度入口
		}
		if (bRet) break;
	}
	return bRet ;
}

BOOL CPublicManage::InterfaceTP(TpRule* ptprule)
{
	BOOL bRet = FALSE;

	return bRet ;
}

BOOL CPublicManage::DirectCustomerService(TpRule* ptprule)
{
	BOOL bRet = FALSE;
	if (NULL != ptprule->customerservice)
	{
		COPYSTRARRAY(m_pstrActiveID,ptprule->customerservice);
		bRet = TRUE;
	}
	return bRet;
}

BOOL CPublicManage::DispatchToCustomerService(Incode* pstincode)
{
	BOOL bRet = FALSE;
	switch (pstincode->dispatchtype){
	case DISPATCH_MAX_IDLETIME:
		bRet = IdleMaxDisptach(pstincode->id);
		break;
	case DISPATCH_MIN_SESSIONTIME:
		bRet = SessionMinDispatch(pstincode->id);
		break;
	case DISPATCH_MAX_PRIORITY:
		bRet = PriorityMaxDispatch(pstincode->id);
		break;
	default:break;
	}
	return bRet;
}

BOOL CPublicManage::IdleMaxDisptach(int id)
{
	BOOL bRet = FALSE;
	CCustomer* pMax_IDLE = GetCurMaxDispatchCustomer(id,DISPATCH_MAX_IDLETIME);
	if (pMax_IDLE != NULL && !pMax_IDLE->GetSessionState() ){
		pMax_IDLE->ReSetIdleTime();
		pMax_IDLE->SetSessionState(TRUE);
		pMax_IDLE->SetClientNum(pMax_IDLE->GetClientNum()+1);
		bRet = TRUE;
		m_pCurCustomer = pMax_IDLE;
		m_pCurCustomer->SetCurIncode(id);
		m_pCurCustomer->SetDisType(DISPATCH_MAX_IDLETIME);
	}
	return bRet;
}

BOOL CPublicManage::SessionMinDispatch(int id)
{
	BOOL bRet = FALSE;
	CCustomer* pMin_SessionTime = GetCurMaxDispatchCustomer(id,DISPATCH_MIN_SESSIONTIME);
	if (pMin_SessionTime != NULL ){
		pMin_SessionTime->ReSetIdleTime();
		pMin_SessionTime->SetSessionState(TRUE);
		pMin_SessionTime->SetClientNum(pMin_SessionTime->GetClientNum()+1);
		bRet = TRUE;
		m_pCurCustomer = pMin_SessionTime;
		m_pCurCustomer->SetCurIncode(id);
		m_pCurCustomer->SetDisType(DISPATCH_MIN_SESSIONTIME);
	}
	return bRet;
}

BOOL CPublicManage::PriorityMaxDispatch(int id)
{
	BOOL bRet = FALSE;
	CCustomer* pMax_Priority = GetCurMaxDispatchCustomer(id,DISPATCH_MAX_PRIORITY);
	if (pMax_Priority != NULL ){
		pMax_Priority->SetSessionState(TRUE);
		pMax_Priority->SetClientNum(pMax_Priority->GetClientNum()+1);
		pMax_Priority->ReSetIdleTime();
		bRet = TRUE;
		m_pCurCustomer = pMax_Priority;
		m_pCurCustomer->SetCurIncode(id);
		m_pCurCustomer->SetDisType(DISPATCH_MAX_PRIORITY);
	}
	return bRet;
}


UINT WINAPI CPublicManage::StatCusTime(LPVOID pv)
{
	CPublicManage* pbM = (CPublicManage*)pv;
	pbM->RestCustomerServiceIDLE();
	while (g_MainFrmThreadState){
		std::list<CCustomer*>::iterator itpCust = pbM->m_listCustomer.begin();
		for (;itpCust !=pbM->m_listCustomer.end();itpCust++){
			
			if (!(*itpCust)->GetSessionState() && 
				DISPATCH_MAX_IDLETIME == (*itpCust)->GetDisType() &&
				MYSTATE_NORMAL == (*itpCust)->GetCustState()){
				(*itpCust)->AddIdleTime();
			}
			if ((*itpCust)->GetSessionState() &&
				DISPATCH_MIN_SESSIONTIME == (*itpCust)->GetDisType() &&
				(*itpCust)->GetClientNum()> 0)
				(*itpCust)->AddSessionTime();
		}
		Sleep(1000);
	}
	return 0;
}

void CPublicManage::RestCustomerServiceIDLE()
{
	CLock m_csLock;
	CAutoLock autolock(&m_csLock);
	std::list<CCustomer*>::iterator itpCust = m_listCustomer.begin();
	for (;itpCust!=m_listCustomer.end();itpCust++){
		(*itpCust)->ReSetIdleTime();
	}
}

void CPublicManage::StopIdleThread()
{
	WaitForSingleObject(m_hThread,INFINITE);	
	CloseHandle(m_hThread);
	m_hThread = NULL;
}

HANDLE CPublicManage::StartIdleThread()
{
	HANDLE hThread = (HANDLE)_beginthreadex(nullptr, 0, StatCusTime, (LPVOID)this, 0, nullptr);
	m_hThread = hThread;
	return hThread;
}

void CPublicManage::GetCustomerState(const char* CusID,MYSTATE &state)
{
	DWORD dwConnectID = 0; 
	g_MongoDBMgr.GetUserState(CusID, state, dwConnectID);
	std::list<CCustomer*>::iterator it_CustObj = m_listCustomer.begin();
	for (;it_CustObj != m_listCustomer.end();it_CustObj ++){
		if (!strcmp(CusID,(*it_CustObj)->GetName())){
			(*it_CustObj)->SetCustState(state);
			if (state != MYSTATE_NORMAL)
			{
				(*it_CustObj)->ReSetIdleTime();
				(*it_CustObj)->ReSetSessionTime();
				(*it_CustObj)->SetClientNum(0);
				(*it_CustObj)->SetSessionState(FALSE);
			}
			break;
		}
	}
}

CCustomer* CPublicManage::GetCurMaxDispatchCustomer(int id,int disPatchType)
{
	if (m_listUser.size())
	{
		CUser* pCurUser = m_listUser.back();
		CCustomer* pCurCustomer = pCurUser->GetCurCustomer();
		if (pCurUser && pCurUser->GetState() && 
			m_iCurPublic == pCurUser->GetCurPublic() &&
			!strcmp(m_szCurUser,pCurUser->GetName()) &&
			m_iCurTp     == pCurUser->GetCurTp() &&
			pCurCustomer->GetCurIncode() == id &&
			pCurCustomer->GetDisType() == disPatchType
			){
			pCurCustomer->SetSessionState(FALSE);
			pCurCustomer->SetClientNum(pCurCustomer->GetClientNum()-1);
			return pCurCustomer;
		}
	}

	std::vector<char*> vCustName;
	for (int i = 0;i<m_vDispatchRelation.size();i++){
		if (m_vDispatchRelation[i].incodeid == id){
			vCustName.push_back(m_vDispatchRelation[i].customerservice);
		}
	}
	std::list<CCustomer*> list_tempCust; 
	std::vector<char*>::iterator itCustName = vCustName.begin();
	while (itCustName!=vCustName.end()){
		MYSTATE myState = (MYSTATE)0;
		std::list<CCustomer*>::iterator it_CustObj = m_listCustomer.begin();
		for (;it_CustObj!=m_listCustomer.end();it_CustObj++)
		{
			if(!strcmp((*it_CustObj)->GetName(),*itCustName))
				break;
		}
		GetCustomerState((*it_CustObj)->GetName(),myState);
		if (MYSTATE_NORMAL == myState &&
			(*it_CustObj)->GetDisType() == disPatchType && 
			(*it_CustObj)->GetClientNum() < MAX_CLIENTNUM){
				list_tempCust.push_back(*it_CustObj);
		}
		itCustName++;
	}
	
	std::list<CCustomer*>::iterator itmax_index = list_tempCust.begin();
	if (itmax_index== list_tempCust.end()) return NULL;
	std::list<CCustomer*>::iterator itmax_temp = list_tempCust.begin();
	if (list_tempCust.size() > 1) ++itmax_temp;
	for (;itmax_index != list_tempCust.end();itmax_index++){
		if((*itmax_index)->CmpByDisType(*itmax_temp,disPatchType) > 0 ){
			itmax_temp = itmax_index;
		}
	}
	return *itmax_temp;
}

void CPublicManage::ConnectClose(char* userID,char* Customer)
{
	CLock m_csLock;
	CAutoLock autolock(&m_csLock);
	std::list<CUser*>::iterator itpUser = m_listUser.begin();
	CCustomer pCust(Customer);
	while(strcmp(Customer,"") && itpUser != m_listUser.end()){
		if (!strcmp(userID,(*itpUser)->GetName())){
			(*itpUser)->SetState(FALSE);
			(*itpUser)->Notify();
			(*itpUser)->Detach(&pCust);
			delete (*itpUser);
			m_listUser.erase(itpUser++);
		}
		else ++itpUser;
	}
}

void CPublicManage::SetCustomerState(char* Name,MYSTATE eMystate)
{
	CLock m_csLock;
	CAutoLock autolock(&m_csLock);
	std::list<CCustomer*>::iterator itpCust = m_listCustomer.begin();
	while(itpCust != m_listCustomer.end()){
		if(!strcmp((*itpCust)->GetName(),Name)) {
			(*itpCust)->SetSessionState(eMystate);
			break;
		}
		itpCust++;
	}
}

// CCustomer

void CCustomer::Update(CUser* pUser)
{
	CLock m_csLock;
	CAutoLock autolock(&m_csLock);
	if(!pUser->GetState()){
		m_iClientnum -= 1;
		if (m_iClientnum == 0)
		{
			SetSessionState(FALSE);
			ReSetSessionTime();
		}
	}
}

//User

void CUser::Attach(CCustomer* pCust)
{
	assert(pCust->GetName());
	pCust->SetLastUserName(m_pUser);
	if (!IsExistCustomer(pCust->GetName()))
	{
		m_ListCust.push_back(pCust);
	}
}

void CUser::Detach(CCustomer* pCust)
{
	std::list<CCustomer* >::iterator list_itCust = m_ListCust.begin();
	while (list_itCust != m_ListCust.end()){
		if (!strcmp((*list_itCust)->GetName() ,pCust->GetName()))
			m_ListCust.erase(list_itCust++);
		else 
			++list_itCust;
	}
}

void CUser::Notify()
{
	std::list<CCustomer* >::iterator list_itCust = m_ListCust.begin();
	while (list_itCust != m_ListCust.end()) {
		(*list_itCust)->Update(this);
		list_itCust++;
	}
}

BOOL CUser::GetState()
{
	return m_bUserState;
}

void CUser::SetState(BOOL bState)
{
	m_bUserState = bState;
}

BOOL CUser::IsExistCustomer(char* szCust)
{
	BOOL bRet = FALSE;
	std::list<CCustomer*>::iterator itpCust = m_ListCust.begin();
	while(itpCust != m_ListCust.end() ){
		if(strcmp((*itpCust)->GetName(),szCust)) {
			bRet = TRUE;
			break;
		}
		itpCust++;
	}
	return bRet;
}