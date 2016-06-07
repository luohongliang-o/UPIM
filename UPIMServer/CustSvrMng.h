#pragma once
class CCustSvrMng
{
public:
	CCustSvrMng();
	~CCustSvrMng();

	// ≥ı ºªØ
	BOOL Init(CADODatabase* pAdoDb);

	long LoadEmpInfo(CADODatabase* pAdoDb);
	long GetEmpInfo(long nBegin, LPSPUPIMID pUPID, long nSize);

protected:
	vector<SPUPIMID>	m_vtEmpInfo;
	CCriticalSection	m_csEmp;
};

