#pragma once

class CIniFile  
{
public:
	CIniFile();
	CIniFile(CString strFileName);
	virtual ~CIniFile();

	void PutString(LPCSTR pdata, LPCSTR pKeyName, LPCSTR pAppName=NULL);
	void GetString(CString &strData, LPCSTR pKeyName, LPCSTR def="", LPCSTR pAppName=NULL);
	void GetString(LPSTR pszData, int nSize, LPCSTR pKeyName, LPCSTR def="", LPCSTR pAppName=NULL);
 	void PutInt(int data, LPCSTR pKeyName, LPCSTR pAppName=NULL);
	int GetInt(LPCSTR pKeyName, int def=0, LPCSTR pAppName=NULL);
	void SetAppName(CString strAppName);
	void SetFileName(CString strFileName);
private:
	CString m_strFileName;
	CString m_strAppName;
};
