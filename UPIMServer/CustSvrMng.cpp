#include "StdAfx.h"
#include "globalvar.h"
#include "CustSvrMng.h"


CCustSvrMng::CCustSvrMng(void)
{
	m_vtEmpInfo.clear();
}


CCustSvrMng::~CCustSvrMng(void)
{
}


BOOL CCustSvrMng::Init(CADODatabase* pAdoDb)
{
	if(!pAdoDb) return FALSE;

	CADOCommand adoCmd(pAdoDb,"icq_get_allemps");
	CADORecordset adoRs(pAdoDb);
	if(!adoRs.Execute(&adoCmd)) 
		return 0;
	long nRowCount = adoRs.GetRecordCount();
	if(nRowCount>MAX_EMP_NUM) nRowCount = MAX_EMP_NUM;
	if(nRowCount<=0) return 0;
	SPUPIMID empInfo = {0};
	CString strValue;

	m_csEmp.Lock();
	m_vtEmpInfo.clear();
	m_vtEmpInfo.resize(nRowCount);
	for(long nIdx=0; nIdx<nRowCount; nIdx++, adoRs.MoveNext())	
	{
		adoRs.GetFieldValue("loginname", strValue);
		strValue.Trim();
		COPYSTRARRAY(empInfo.m_szUID, strValue);
		m_vtEmpInfo[nIdx] = empInfo;
	}
	m_csEmp.Unlock();

	SYSTEMTIME st;
	GetLocalTime(&st);
	g_config.dwSynLastDate = st.wYear*10000 + st.wMonth*100 + st.wDay;

	return TRUE;
}

long  CCustSvrMng::LoadEmpInfo(CADODatabase* pAdoDb)
{
	if (!pAdoDb) return 0;
	return 1;
}

long CCustSvrMng::GetEmpInfo(long nBegin, LPSPUPIMID pUPID, long nSize)
{
	long nRet = 0;
	m_csEmp.Lock();
	for(; nBegin<m_vtEmpInfo.size(); nBegin++)
	{
		if(nRet>=nSize) break;
		COPYSTRARRAY(pUPID[nRet].m_szUID, m_vtEmpInfo[nBegin].m_szUID);
		nRet++;
	}
	m_csEmp.Unlock();
	return nRet;
}