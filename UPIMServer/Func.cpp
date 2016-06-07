#include "StdAfx.h"
#include "Func.h"
#include "globalvar.h"

BOOL InitAnsBufHead(IMANSBUFFER& m_ansBuf, long req, long PacketLen /* = 0 */, long RawLen /* = 0 */)
{
	m_ansBuf.AnsHeader.Separator	= SEPARATOR_NUM;
	m_ansBuf.AnsHeader.Info.Version	= NOW_VERSION;
	m_ansBuf.AnsHeader.Priority		= 1;
	m_ansBuf.AnsHeader.req			= req;
	m_ansBuf.AnsHeader.PacketLen	= PacketLen;
	m_ansBuf.AnsHeader.RawLen		= RawLen;
	return TRUE;
}

VOID LoadConfig()
{
	g_config.dwBuildNo = GetBuildNo(g_config.szBuildDateTime, sizeof(g_config.szBuildDateTime));

	CHAR szFormat[MAX_PATH] = {0}, szData[MAX_PATH] = {0};
	LONG nData = 0;
	GetModuleFileName( NULL, g_config.szHomePath, sizeof(g_config.szHomePath) );
	long nLen = strlen( g_config.szHomePath );
	for ( long loop=(nLen-1); loop>=0; loop-- )
	{
		if ( g_config.szHomePath[loop] == '\\' )
		{
			g_config.szHomePath[loop] = 0;
			break;
		}
	}
	nsprintf( g_config.szConfigPath, sizeof(g_config.szConfigPath), "%s\\UPIMServer.ini", g_config.szHomePath );
	nsprintf( g_config.szLogFile, sizeof(g_config.szLogFile), "%s\\UPIMServer", g_config.szHomePath );

	CIniFile m_IniFile;
	m_IniFile.SetFileName(g_config.szConfigPath);
	CString strTmp = "";
	//////////////////////////////////////////////////////////////////////////
	//服务器配置
	m_IniFile.SetAppName("SERVICE");
	g_config.dwSVRID = m_IniFile.GetInt("SERVID",0);
	m_IniFile.GetString(strTmp,"SERVICE_ADDR","");
	strncpy(g_config.szSVRIP, strTmp, sizeof(g_config.szSVRIP));
	g_config.dwSVRPORT =  m_IniFile.GetInt("SERVICE_PORT",0);

	//////////////////////////////////////////////////////////////////////////
	//数据库配置
	m_IniFile.SetAppName("dbconfig");
	int nDBCount =  m_IniFile.GetInt( "num",0);
	nDBCount = min(nDBCount, MAX_DBCOUNT);
	char szKey[64] = {0};
	for(int i=0;i<nDBCount;i++)
	{
		// 数据源
		sprintf(szKey,"DataSource_%d", i+1);
		m_IniFile.GetString(strTmp, szKey, "");
		strTmp.Trim();
		SafeStrcpy(g_config.aDBInfoConfig[i].szDataSource, strTmp, sizeof(g_config.aDBInfoConfig[i].szDataSource) );
		// 数据库
		sprintf(szKey,"DataBase_%d", i+1);
		m_IniFile.GetString(strTmp, szKey, "");
		strTmp.Trim();
		SafeStrcpy(g_config.aDBInfoConfig[i].szDataBase, strTmp, sizeof(g_config.aDBInfoConfig[i].szDataBase) );
		// 数据库用户
		sprintf(szKey,"UserName_%d",i+1);
		m_IniFile.GetString(strTmp, szKey, "");
		strTmp.Trim();
		SafeStrcpy(g_config.aDBInfoConfig[i].szUserName, strTmp, sizeof(g_config.aDBInfoConfig[i].szUserName) );
		// 数据库密码
		sprintf(szKey,"Password_%d",i+1);
		m_IniFile.GetString(strTmp, szKey, "");
		strTmp.Trim();
		SafeStrcpy(g_config.aDBInfoConfig[i].szPassword, strTmp, sizeof(g_config.aDBInfoConfig[i].szPassword) );
	}

	g_config.nInfoDBCount = nDBCount;
	g_config.nInfoSel = m_IniFile.GetInt( "sel",0);
	if(g_config.nInfoSel<0 || g_config.nInfoSel>=MAX_DBCOUNT) 
		g_config.nInfoSel = 0;


	//////////////////////////////////////////////////////////////////////////
	m_IniFile.SetAppName("serverbmd");
	int nIPCount =  m_IniFile.GetInt( "num",0);
	nIPCount = min(nIPCount, MAX_IPWHITELIST);
	g_IPWhiteList.num = nIPCount;
	for (int j = 0;j<nIPCount;j++)
	{
		//IP白名单
		g_IPWhiteList.ipinfo[j].id = j;
		sprintf(szKey,"IP_%d", j+1);
		m_IniFile.GetString(strTmp, szKey, "");
		strTmp.Trim();
		SafeStrcpy(g_IPWhiteList.ipinfo[j].IP, strTmp, sizeof(g_IPWhiteList.ipinfo[j].IP));
	}
	//////////////////////////////////////////////////////////////////////////
	return;
}

VOID SaveConfig()
{
	return;
}

BOOL IsWhiteIP(CHAR* strIP)
{
	BOOL bRet = FALSE;
	CString tempstrIP;
	for(int i = 0;i<g_IPWhiteList.num;i++)
	{
		tempstrIP.Format(g_IPWhiteList.ipinfo[i].IP);
		if (!tempstrIP.Compare(strIP))
		{
			bRet = TRUE;
			break;
		}
	}
	return bRet;
}
// 获取编译序列号
WORD GetBuildNo( LPSTR lpszDateTime, const WORD wSize )
{
	static CHAR *s_MonthStr[12] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
	static CHAR	s_szBuildDateTime[32] = "";
	static WORD	s_nBuildNo = 0;

	if ( s_nBuildNo == 0 )
	{
		CHAR szTemp[16] = {0};
		LONG nYear, nMonth, nDay, nHour, nMin,nSec;
		// 日期
		strncpy( szTemp, __DATE__, 3 );
		szTemp[3] = '\0';
		for ( nMonth=1; nMonth<=12; nMonth++ )
		{
			if ( _stricmp( szTemp, s_MonthStr[nMonth-1] ) == 0 )
			{
				break;
			}
		}
		strncpy( szTemp, __DATE__+4, 2 );
		szTemp[2] = '\0';
		nDay = atoi( szTemp );
		strncpy( szTemp, __DATE__+7, 4 );
		szTemp[4] = '\0';
		nYear = atoi( szTemp );
		// 时间
		strncpy( szTemp, __TIME__, 2 );
		szTemp[2] = '\0';
		nHour = atoi( szTemp );
		strncpy( szTemp, __TIME__+3, 2 );
		szTemp[2] = '\0';
		nMin = atoi( szTemp );
		strncpy( szTemp, __TIME__+6, 2 );
		szTemp[2] = '\0';
		nSec = atoi( szTemp );
		// 形成输出格式
		if ((nHour >= 9) && (nHour <= 16))
		{
			s_nBuildNo = (WORD)( nHour - 8 );
		}
		else if (nHour < 9)
		{
			s_nBuildNo = 0;
		}
		else
		{
			s_nBuildNo = 9;
		}
		s_nBuildNo += (WORD)( ( ( (nYear - VER_STARTYEAR) * 12 + nMonth) * 31 + nDay) * 10 );
		s_nBuildNo %= 10000;
		_snprintf( s_szBuildDateTime, 32, "%04d/%02d/%02d %02d:%02d:%02d", nYear, nMonth, nDay, nHour, nMin,nSec);
	}

	strncpy( lpszDateTime, s_szBuildDateTime, wSize );
	return s_nBuildNo;
}

CString IntToStr(int nVal)
{
	CString strRet;
	char buff[10];

	_itoa(nVal, buff, 10);
	strRet = buff;
	return strRet;
}

CString LongToStr(long lVal)
{
	CString strRet;
	char buff[20];

	_ltoa(lVal, buff, 10);
	strRet = buff;
	return strRet;
}

CString ULongToStr(unsigned long ulVal)
{
	CString strRet;
	char buff[20];

	_ultoa(ulVal, buff, 10);
	strRet = buff;
	return strRet;

}

CString DblToStr(double dblVal, int ndigits)
{
	CString strRet;
	char buff[50];

	_gcvt(dblVal, ndigits, buff);
	strRet = buff;
	return strRet;
}

CString DblToStr(float fltVal)
{
	CString strRet;
	char buff[50];

	_gcvt(fltVal, 10, buff);
	strRet = buff;
	return strRet;
}

// 连接数据库
BOOL ConnectDataBase(LPDBCONFIG lpdbConfig, CADODatabase* pAdoDb)
{
	if(pAdoDb==NULL) return FALSE;
	pAdoDb->Close();
	CString	strConnect;
	strConnect.Format( "Provider=sqloledb;Data Source=%s;Network Library=DBMSSOCN;Initial Catalog=%s;",lpdbConfig->szDataSource, lpdbConfig->szDataBase);
	pAdoDb->SetConnectionTimeout(g_config.dwConnTimeOut);
	if(pAdoDb->Open( strConnect, lpdbConfig->szUserName, lpdbConfig->szPassword))
	{
	//	pAdoDb->SetConnectionTimeout(g_config.dwConnTimeOut);
	}
	return pAdoDb->IsOpen();
}

// 断开数据库
BOOL DisConnectDataBase( CADODatabase* pAdoDb )
{
	if (pAdoDb==NULL) return FALSE;
	pAdoDb->Close();
	return TRUE;
}

BOOL IsDBDisConn(CString strError)
{
	if(strError.Find("80004005",0) != -1
		||strError.Find("TIMEOUT",0) != -1
		||strError.Find("CONNECTIONWRITE",0) != -1
		||strError.Find("CONNECTIONREAD",0) != -1
		||strError.Find("COMMUNICATION",0) != -1
		||strError.Find("连接失败",0) != -1
		||strError.Find("链接失败",0) != -1) 
		return TRUE;
	return FALSE;
}

LPSTR SafeStrcpy( LPSTR lpszDest, LPCSTR lpcszSrc, const WORD wDestSize )
{
	if(!lpszDest) return NULL;
	strncpy( lpszDest, lpcszSrc, wDestSize );
	lpszDest[wDestSize-1] = '\0';
	return lpszDest;
}

// 字符串规整
LPCSTR AllTrim( LPSTR lpszString )
{
	LONG i = 0;
	if ( !(*lpszString) ) return lpszString;
	while ( lpszString[i] == ' ' ) i++;
	if ( i ) memmove( lpszString, lpszString+i, strlen(lpszString)+1-i );
	i = strlen(lpszString) - 1;
	while ( (i) && (lpszString[i] == ' ') ) lpszString[i--] = 0;
	return lpszString;
}

CString	GetNowTime()
{
	SYSTEMTIME st = {0};
	GetLocalTime(&st);
	CString strTime = _T("");
	strTime.Format("%04d-%02d-%02d %02d:%02d:%02d.%03d", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);
	return strTime;
}

double StrToOleDateTime(CString str)
{
	COleDateTime time;
	CString syear,smonth,sday,shour,sminute,ssecond;
	INT year,month,day,hour,minute,second;
	if(str.GetLength() == 19)
	{
		syear = str.Mid(0,4);  year = StrToInt(syear);
		smonth = str.Mid(5,2); month = StrToInt(smonth);
		sday = str.Mid(8,2);   day = StrToInt(sday);
		shour = str.Mid(11,2); hour = StrToInt(shour);
		sminute = str.Mid(14,2); minute =StrToInt(sminute);
		ssecond = str.Mid(17,2); second = StrToInt(ssecond);
		time.SetDateTime(year,month,day,hour,minute,second);
	}
	return time.m_dt;
}

LPCSTR OleDateTimetoStr(double mdt,const char* pFormatstr)
{
	COleDateTime time;
	time.m_dt = mdt;
	return time.Format(pFormatstr);
}

void ConvertTimeToLong(time_t tmTime, long& nRetDate, long& nRetTime)
{
	struct tm *ptm = localtime(&tmTime);
	if(!ptm)
	{
		time_t thisTime = time(NULL);
		ptm = localtime(&thisTime);
	}
	nRetDate = (ptm->tm_year+1900)*10000+(ptm->tm_mon+1)*100+ptm->tm_mday;
	nRetTime = ptm->tm_hour*10000+ptm->tm_min*100+ptm->tm_sec;
}

LONG ParseJsonFields(LPSTR lpszString, STATE_REFLASH_& mstu, LONG nMaxField)
{
	char szJson[MAX_SEND_BUF_LEN] = {0};
	strcpy(szJson, lpszString);
	//////////////////////////////////////////////////////////////////////////
	Value value;  
	Reader read;  
	if ( !read.parse( szJson,value ) )  
	{  
		return -1;  
	}
	
	int nItenNum = value["ct"].asInt();
	Value val_array = value["it"];  
	int iSize = val_array.size();
	if (nItenNum == iSize && nItenNum>0)
	{
		mstu.nCount = nItenNum;
		for (int nIndex=0; nIndex<iSize; nIndex++)
		{
			CString strTmp = val_array[nIndex].asCString();
			strncpy(mstu.mIDVT[nIndex].m_szUID, strTmp, strTmp.GetLength());
		}
	}
	return nItenNum;
}


long Base64_Encode(BYTE *pRawData, long nRawDataLen, BYTE *pResultBuf, long nMaxResultBufLen, BOOL bEncode)
{
	if(nMaxResultBufLen < 0)
		return -1;
	memset(pResultBuf, 0, nMaxResultBufLen);
	int i;
	if(bEncode)
	{
		CByteArray array, res_array;
		for (i=0; i<nRawDataLen; i++)
			array.Add(pRawData[i]);

		CBase64Coding base64coding;
		if (!base64coding.Encode(array, res_array))
			return -1;
		int nRet = res_array.GetSize();
		if(nMaxResultBufLen < nRet)
			return -1;
		for (i=0; i<nRet; i++)
			pResultBuf[i] = res_array.GetAt(i);
		return nRet;
	}
	else
	{
		CByteArray array, res_array;
		for (i=0; i<nRawDataLen; i++)
			array.Add(pRawData[i]);

		CBase64Coding base64coding;
		if (!base64coding.Decode(array, res_array))
			return -1;
		int nRet = res_array.GetSize();
		if(nMaxResultBufLen<nRet)
			return -1;
		for (i=0; i<nRet; i++)
			pResultBuf[i] = res_array.GetAt(i);
		return nRet;
	}
	return -1;
}