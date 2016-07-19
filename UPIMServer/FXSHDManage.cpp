#include "StdAfx.h"
#include "FXSHDManage.h"
#include "globalvar.h"
#include "MainFrm.h"
#include <WinCrypt.h>
#include <atlenc.h>

#include "stdafx.h"

typedef struct _PLAINTEXTBLOB
{
	BLOBHEADER Blob;
	DWORD      dwKeyLen;
	CHAR       bKey[8];
} PLAINTEXTBLOB;


NTSTATUS
CreateSymmetricKey(HCRYPTPROV  hProv,
				   IN ALG_ID     Algid,
				   IN DWORD       cbKey,
				   IN UCHAR      *pbKey,
				   IN UCHAR      *pbIV,
				   OUT HCRYPTKEY *phKey
				   )
{
	PLAINTEXTBLOB PlainBlob = {0};



	if (cbKey > 8)
	{
		goto CleanUp;
	}


	PlainBlob.Blob.bType = PLAINTEXTKEYBLOB;
	PlainBlob.Blob.bVersion = CUR_BLOB_VERSION;
	PlainBlob.Blob.reserved = 0;
	PlainBlob.Blob.aiKeyAlg = Algid;
	memcpy(PlainBlob.bKey, pbKey, cbKey);
	PlainBlob.dwKeyLen = cbKey;


	// import thw simpleblob to get a handle to the symmetric key
	if (!CryptImportKey(hProv,
		(BYTE *)&PlainBlob,
		sizeof(PlainBlob),
		0,
		0,
		phKey))
	{
		goto CleanUp;
	}


	if ((Algid == CALG_DES) || (Algid == CALG_3DES_112))
	{

		if (!pbIV)
		{
		}
		else
		{
			if (!CryptSetKeyParam(*phKey, KP_IV, pbIV, 0))
			{
				goto CleanUp;
			}
			// set the IV
			/*
			DWORD dwMode = CRYPT_MODE_CBC;
			if(!CryptSetKeyParam(*phKey, KP_MODE, (const BYTE*)&dwMode, 0)) {
				goto CleanUp;
			}


			dwMode = PKCS5_PADDING;
				if(!CryptSetKeyParam(*phKey, KP_PADDING, (const BYTE*)&dwMode, 0)) {
					goto CleanUp;
				}*/

		}

	}

CleanUp:

	return 0;
}

int Encrypt(BYTE  Data3[],int leng,BOOL b,BYTE** pRet)
{
	//--------------------------------------------------------------------
	// Declare variables.
	//
	// hProv:           Handle to a cryptographic service provider (CSP). 
	//                  This example retrieves the default provider for  
	//                  the PROV_RSA_FULL provider type.  
	// hHash:           Handle to the hash object needed to create a hash.
	// hKey:            Handle to a symmetric key. This example creates a 
	//                  key for the RC4 algorithm.
	// hHmacHash:       Handle to an HMAC hash.
	// pbHash:          Pointer to the hash.
	// dwDataLen:       Length,in bytes, of the hash.
	// IV:           Password string used to create a symmetric key.
	// Keys:           Message string to be hashed.
	// HmacInfo:        Instance of an HMAC_INFO structure that contains 
	//                  information about the HMAC hash.
	// 
	HCRYPTPROV  hProv       = NULL;
	HCRYPTHASH  hHash       = NULL;
	HCRYPTKEY   hKey        = NULL;
	HCRYPTHASH  hHmacHash   = NULL;
	PBYTE       pbHash      = NULL;
	DWORD       dwDataLen   = 0;
	BYTE        IV[]     = {'u','p','c','h','i','n','a','1'};
	//BYTE        Keys[]     = {'u','p','c','h','i','n','a','8'};
	BYTE        Keys[]     = {'1','6','2','2','a','9','2','d'};// website
	//BYTE        Keys[]     = {'c','2','6','8','a','2','c','d'}; //product
	//BYTE        Data3[]     = {'u','p','c','h','i','n','a','1',0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

	//--------------------------------------------------------------------
	// Acquire a handle to the default RSA cryptographic service provider.

	if (!CryptAcquireContext(
		&hProv,                   // handle of the CSP
		NULL,                     // key container name
		NULL,                     // CSP name
		PROV_RSA_FULL,            // provider type
		CRYPT_VERIFYCONTEXT))     // no key access is requested
	{
		printf(" Error in AcquireContext 0x%08x \n",
			GetLastError());
		goto ErrorExit;
	}


	DWORD cb2=leng;
	DWORD cb=cb2+8;
	DWORD dwErr;

	CreateSymmetricKey(hProv,CALG_DES, 8, Keys, IV, &hKey);
	BYTE* pOut=(BYTE*)malloc(cb);
	memset(pOut,0,cb);
	memcpy(pOut,Data3,cb2);
	if (b)
	{
		CryptEncrypt(hKey, NULL, TRUE, 0, pOut, &cb2, cb);
		//CryptDecrypt(hKey, NULL, TRUE, 0, pOut, &cb2);
	}
	else
		CryptDecrypt(hKey, NULL, TRUE, 0, pOut, &cb2);

	*pRet = pOut;

	dwErr = GetLastError();       


	// Free resources.
ErrorExit:
	if(hKey)
		CryptDestroyKey(hKey);
	if(hProv)
		CryptReleaseContext(hProv, 0);
	if(pbHash)
		free(pbHash);
	return cb2;
}

BYTE * MacHash(BYTE        Keys[],int leng,int* pMd5Out)
{
	//--------------------------------------------------------------------
	// Declare variables.
	//
	// hProv:           Handle to a cryptographic service provider (CSP). 
	//                  This example retrieves the default provider for  
	//                  the PROV_RSA_FULL provider type.  
	// hHash:           Handle to the hash object needed to create a hash.
	// hKey:            Handle to a symmetric key. This example creates a 
	//                  key for the RC4 algorithm.
	// hHmacHash:       Handle to an HMAC hash.
	// pbHash:          Pointer to the hash.
	// dwDataLen:       Length,in bytes, of the hash.
	// IV:           Password string used to create a symmetric key.
	// Keys:           Message string to be hashed.
	// HmacInfo:        Instance of an HMAC_INFO structure that contains 
	//                  information about the HMAC hash.
	// 
	HCRYPTPROV  hProv       = NULL;
	HCRYPTHASH  hHash       = NULL;
	HCRYPTKEY   hKey        = NULL;
	HCRYPTHASH  hHmacHash   = NULL;
	PBYTE       pbHash      = NULL;
	DWORD       dwDataLen   = 0;
	//BYTE        IV[]     = {1,2,3,4,5,6,7,8};
	//BYTE        Keyss[]     = {(BYTE)'u',(BYTE)'p',(BYTE)'c','h','i','n','a','8'};
	//BYTE        Keyss[]      = {'c','2','6','8','a','2','c','d'};// product 
	BYTE        Keyss[]      = {'1','6','2','2','a','9','2','d'};//website
	HMAC_INFO   HmacInfo;
	CString strError =_T("");

	//--------------------------------------------------------------------
	// Zero the HMAC_INFO structure and use the SHA1 algorithm for
	// hashing.

	ZeroMemory(&HmacInfo, sizeof(HmacInfo));
	HmacInfo.HashAlgid = CALG_MD5;

	//--------------------------------------------------------------------
	// Acquire a handle to the default RSA cryptographic service provider.

	if (!CryptAcquireContext(
		&hProv,                   // handle of the CSP
		NULL,                     // key container name
		NULL,                     // CSP name
		PROV_RSA_FULL,            // provider type
		CRYPT_VERIFYCONTEXT))     // no key access is requested
	{
		printf(" Error in AcquireContext 0x%08x \n",
			GetLastError());
		strError.Format(_T("Error in AcquireContext 0x%08x"),GetLastError());
		
		goto ErrorExit;
	}



	CreateSymmetricKey(hProv,CALG_RC4, 8, Keyss, NULL, &hKey);

	//--------------------------------------------------------------------
	// Create an HMAC by performing the following steps:
	//    1. Call CryptCreateHash to create a hash object and retrieve 
	//       a handle to it.
	//    2. Call CryptSetHashParam to set the instance of the HMAC_INFO 
	//       structure into the hash object.
	//    3. Call CryptHashData to compute a hash of the message.
	//    4. Call CryptGetHashParam to retrieve the size, in bytes, of
	//       the hash.
	//    5. Call malloc to allocate memory for the hash.
	//    6. Call CryptGetHashParam again to retrieve the HMAC hash.

	if (!CryptCreateHash(
		hProv,                    // handle of the CSP.
		CALG_HMAC,                // HMAC hash algorithm ID
		hKey,                     // key for the hash (see above)
		0,                        // reserved
		&hHmacHash))              // address of the hash handle
	{
		printf("Error in CryptCreateHash 0x%08x \n", 
			GetLastError());
		strError.Format(_T("Error in CryptCreateHash 0x%08x"),GetLastError());
		
		goto ErrorExit;
	}

	if (!CryptSetHashParam(
		hHmacHash,                // handle of the HMAC hash object
		HP_HMAC_INFO,             // setting an HMAC_INFO object
		(BYTE*)&HmacInfo,         // the HMAC_INFO object
		0))                       // reserved
	{
		printf("Error in CryptSetHashParam 0x%08x \n", 
			GetLastError());
		strError.Format(_T("Error in CryptSetHashParam 0x%08x"),GetLastError());
		goto ErrorExit;
	}

	if (!CryptHashData(
		hHmacHash,                // handle of the HMAC hash object
		Keys,                    // message to hash
		leng,//sizeof(Keys),            // number of bytes of data to add
		0))                       // flags
	{
		printf("Error in CryptHashData 0x%08x \n", 
			GetLastError());
		strError.Format(_T("Error in CryptHashData 0x%08x"),GetLastError());
		
		goto ErrorExit;
	}

	//--------------------------------------------------------------------
	// Call CryptGetHashParam twice. Call it the first time to retrieve
	// the size, in bytes, of the hash. Allocate memory. Then call 
	// CryptGetHashParam again to retrieve the hash value.

	if (!CryptGetHashParam(
		hHmacHash,                // handle of the HMAC hash object
		HP_HASHVAL,               // query on the hash value
		NULL,                     // filled on second call
		&dwDataLen,               // length, in bytes,of the hash
		0))
	{
		printf("Error in CryptGetHashParam 0x%08x \n", 
			GetLastError());
		strError.Format(_T("Error in CryptGetHashParam 0x%08x"),GetLastError());
		
		goto ErrorExit;
	}

	pbHash = (BYTE*)malloc(dwDataLen);
	*pMd5Out = dwDataLen;
	if(NULL == pbHash) 
	{
		printf("unable to allocate memory\n");
		strError.Format(_T("unable to allocate memory"));
		
		goto ErrorExit;
	}

	if (!CryptGetHashParam(
		hHmacHash,                 // handle of the HMAC hash object
		HP_HASHVAL,                // query on the hash value
		pbHash,                    // pointer to the HMAC hash value
		&dwDataLen,                // length,in bytes, of the hash
		0))
	{
		printf("Error in CryptGetHashParam 0x%08x \n", GetLastError());
		strError.Format(_T("Error in CryptGetHashParam 0x%08x"),GetLastError());
		
		goto ErrorExit;
	}

	// Print the hash to the console.


	// Free resources.
ErrorExit:
	if(hHmacHash)
		CryptDestroyHash(hHmacHash);
	if(hKey)
		CryptDestroyKey(hKey);
	if(hHash)
		CryptDestroyHash(hHash);    
	if(hProv)
		CryptReleaseContext(hProv, 0);
	return pbHash;
}



CFXSHDManage::CFXSHDManage(void):
	m_vFXSData(vector<FXSData>(0)),
	m_strFXSJson("")
{
}

CFXSHDManage::~CFXSHDManage(void)
{
}



int CFXSHDManage::ParseJson(CString strJson,int type,FXSHouseInfo* pFxsHouseInfo /*= NULL*/,int nMarket /*= 0*/)
{
	int itemLength = 0;
	Reader _read;
	Value  _value;
	if (strJson.GetLength() == 0) return itemLength;
	if ( !_read.parse( (LPCTSTR)strJson,_value ) )  return -1;  

	if (FXSINFO == type)
	{
		Value  _array = _value["Msg"];
		itemLength = _array.size();
		if (itemLength  < 1) return -1;
		for (int i = 0;i<itemLength; i++){
			FXSData tempFxs;
			tempFxs.AnalystId = _array[i]["AnalystId"].asString().c_str();
			tempFxs.AnalystName = _array[i]["AnalystName"].asString().c_str();
			m_vFXSData.push_back(tempFxs);
		}
		UpdateFxsDB();
	}
	if (FXS_HOUSEINFO == type)
	{
		Value  _array = _value["Msg"];
		itemLength = _array.size();
		if (itemLength  < 1) return -1;
		for (int i = 0;i<itemLength; i++){
			FXSHouseInfo tempFxsHouse ;
			tempFxsHouse.RoomId = _array[i]["RoomId"].asString().c_str();
			tempFxsHouse.RoomName = _array[i]["RoomName"].asString().c_str();
			tempFxsHouse.Description = _array[i]["Description"].asString().c_str();
			m_vFXSHouse.push_back(tempFxsHouse);
		}
	}
	if (FXS_TO_HOUSEINFO == type)
	{
		Value  _array = _value["Msg"];
		itemLength = _array.size();
		for (int i = 0;i<itemLength; i++){
			CString csAnalystId(_array[i]["AnalystId"].asString().c_str());
			CString csImageUrl(_array[i]["AnalystImg"].asString().c_str());
			pFxsHouseInfo->ImageAddr = csImageUrl;
			m_mapFxsToHouseInfo[csAnalystId].push_back(*pFxsHouseInfo);
		}
	}
	
	return itemLength;
}

int CFXSHDManage::ParseJsonEx(CString strJson,int type,FXSVipHouseInfo* pFxsHouseInfo /* = NULL */,int nMarket /* = 0 */)
{
	int itemLength = 0;
	Reader _read;
	Value  _value;
	if ( !_read.parse( (LPCTSTR)strJson,_value ) )  return -1;  
	if (ROOM_TO_FXSINFO == type)
	{
		Value  _array = _value["Msg"];
		itemLength = _array.size();
		vector<FXSDataEx> CurFxsInfo;

		vector<FXSVipHouseInfo> tempVipHouseInfo = m_mapVipHouseInfo[nMarket];
 		vector<FXSVipHouseInfo>::iterator it_vTempVipHouseInfo = find(tempVipHouseInfo.begin(),
 			tempVipHouseInfo.end(),*pFxsHouseInfo);
		for (int i = 0;i<itemLength; i++){
			FXSDataEx tempFxs;
			CString csAnalystId(_array[i]["AnalystId"].asString().c_str());
			CString csAnalystName(_array[i]["AnalystName"].asString().c_str());
			CString csImageUrl(_array[i]["AnalystImg"].asString().c_str());
			CString SpecialtyArea(_array[i]["SpecialtyArea"].asString().c_str());
			tempFxs.SpecialtyArea = SpecialtyArea;
			tempFxs.AnalystId = csAnalystId;
			tempFxs.AnalystName = csAnalystName;
			tempFxs.AnalystImg = csImageUrl;
			CurFxsInfo.push_back(tempFxs);
		}
		m_mapRoomIDToFxsInfo[pFxsHouseInfo->RoomId].FxsData = CurFxsInfo;
		m_mapRoomIDToFxsInfo[pFxsHouseInfo->RoomId].VipHouse = *it_vTempVipHouseInfo;
	}
	if (FXS_VIP_HOUSEINFO == type)
	{
		Value  _array = _value["Msg"];
		itemLength = _array.size();
		for (int i = 0;i<itemLength; i++){
			FXSVipHouseInfo tempVipHouse;
			tempVipHouse.RoomId = _array[i]["RoomId"].asString().c_str();
			tempVipHouse.RoomName = _array[i]["RoomName"].asString().c_str();
			tempVipHouse.Description = "";
			tempVipHouse.Img = _array[i]["Img"].asString().c_str();
			tempVipHouse.Notice = "";
			tempVipHouse.VipType = _array[i]["VipType"].asString().c_str();
			tempVipHouse.IconState = _array[i]["IconState"].asInt();
			m_mapVipHouseInfo[nMarket].push_back(tempVipHouse);
		}

	}
	if (ROOMRIGHT_TO_USER == type)
	{
		CString tempJsonStr = _value["retMessage"].asString().c_str();

		Value _value1;
		_read.parse((LPCTSTR)tempJsonStr,_value1);
		itemLength = _value1.size();
		MapMarketToVipHouseinfo::iterator it_mapVipHouse = m_mapVipHouseInfo.begin();
		for (;it_mapVipHouse != m_mapVipHouseInfo.end();it_mapVipHouse++)
		{
			for (int i = 0;i<itemLength; i++){
				CString UserName = _value1[i]["Username"].asString().c_str();
				int     RoomRight = atoi(_value1[i]["RightId"].asString().c_str());
				CString RoomId;
				for (int j = 0;j<it_mapVipHouse->second.size();j++)
				{
					if (atoi(it_mapVipHouse->second[j].VipType.GetBuffer(0))== RoomRight)
					{
						RoomId = it_mapVipHouse->second[j].RoomId;
						m_mapRoomIDUser[atoi(RoomId.GetBuffer(0))].push_back(UserName);
						break;
					}
				}
			}
		}
	}
	return itemLength;
}

void CFXSHDManage::DeleteFxsToHousInfo()
{
	m_csFreeItem.Lock();
	if(m_vFXSData.size()>0)
		m_vFXSData.clear();
	if (m_vFXSHouse.size()>0)
		m_vFXSHouse.clear();
	if (m_mapRoomIDToFxsInfo.size()>0)
		m_mapRoomIDToFxsInfo.clear();
	if (m_mapFxsToHouseInfo.size()>0){
		m_mapFxsToHouseInfo.clear();
	}
	if (m_mapVipHouseInfo.size()>0){
		m_mapVipHouseInfo.clear();
	}
	if (m_mapRoomIDUser.size()>0)
	{
		m_mapRoomIDUser.clear();
	}
	m_csFreeItem.Unlock();
}

MapFxsToHouseInfo* CFXSHDManage::GetFxsToHouseInfo()
{
	return &m_mapFxsToHouseInfo;
}

LONG CFXSHDManage::UpdateFxsDB()
{
	LONG lRet = eGCError_Success;
	LPOPLINK pLinkInfo = NULL;
	char szMessage[128] = {0};
	pLinkInfo = g_LinkMangaer.GetLink(szMessage,sizeof(szMessage),TRUE);
	if (pLinkInfo)
	{
		CADODatabase* pAdoDb = pLinkInfo->pAdodb;
		if (!pAdoDb) return eGCError_DbCon;
		if (!pAdoDb->IsOpen()) return eGCError_DbClose;
		try
		{
			for (int i = 0;i<m_vFXSData.size() ;i++)
			{
				CADOCommand adoCmd(pAdoDb,"fxshd_add");
				adoCmd.AddInputParameter("@loginname",m_vFXSData[i].AnalystId.GetLength(),m_vFXSData[i].AnalystId);
				adoCmd.AddInputParameter("@truename",m_vFXSData[i].AnalystName.GetLength(),m_vFXSData[i].AnalystName);
				adoCmd.Execute();
			}
		}
		catch (...)
		{
			OP_LOG_DEGUB("%s","数据库消息同步发生异常");
			//g_LinkMangaer.DisConnect(pLinkInfo);
		}

	}
	g_LinkMangaer.FreeLink(pLinkInfo);
	return lRet;
}

BOOL CFXSHDManage::Request(LPCTSTR szURL, LPCTSTR* aresponse, int type,void* pFxsHouseInfo, int nMarket,int nMethod,LPCTSTR szAgent )
{
	BOOL bRet = m_httpRep.Request(szURL,nMethod,szAgent);
	CString csResponse = (CString)m_httpRep.QueryHTTPResponse();
	wchar_t*  uresponse = NULL;
 	if (ROOMRIGHT_TO_USER == type)
 	{
		int  destlen = 1024*100;
		Base64Decode(csResponse,csResponse.GetLength(),NULL,&destlen);
		destlen++;
		LPBYTE pDest = new BYTE[destlen];
		memset(pDest,0,destlen);
		Base64Decode(csResponse,csResponse.GetLength(),pDest,&destlen);

		BYTE* pOutByte = NULL;
		int retLen = Encrypt(pDest,destlen,FALSE,&pOutByte);
		pOutByte[retLen] = '\0';
		uresponse = m_httpRep.UTF8ToUnicode( (const char*)pOutByte, CP_UTF8);
		*aresponse = m_httpRep.UnicodeToACP( uresponse, CP_ACP);
 	}
	else
	{
		wchar_t*  uresponse = m_httpRep.UTF8ToUnicode( (const char*)csResponse, CP_UTF8);
		*aresponse = m_httpRep.UnicodeToACP( uresponse, CP_ACP);
	}

	TDEL(uresponse );
	if (ROOM_TO_FXSINFO == type || FXS_VIP_HOUSEINFO == type || ROOMRIGHT_TO_USER == type)
		ParseJsonEx(*aresponse,type,(FXSVipHouseInfo*)pFxsHouseInfo,nMarket);
	else 
		ParseJson(*aresponse,type,(FXSHouseInfo*)pFxsHouseInfo,nMarket);
	TDEL(*aresponse);
	return bRet;
}

VHouseInfo* CFXSHDManage::GetHouseInfo()
{
	return &m_vFXSHouse;
}

MapRoomIDToFxsInfo* CFXSHDManage::GetRoomIDToFxsDataEx()
{
	return &m_mapRoomIDToFxsInfo;
}

MapMarketToVipHouseinfo* CFXSHDManage::GetVipHouseInfo()
{
	return &m_mapVipHouseInfo;
}

CString CFXSHDManage::GetUserByRoomRight()
{	
	CString urlstr = "http://121.14.69.17:801/RoomService.svc/GetRoomRights?";

	FastWriter writer;
	Value data_value;
	string contentValue;
	data_value["RoomRight"] = Value("834");
	contentValue = writer.write(data_value);

	//CString contentValue = "{\"RoomRight\":\"834\"}";
	wchar_t* uStr = m_httpRep.UTF8ToUnicode(contentValue.c_str(),CP_ACP);
	char* aStr = m_httpRep.UnicodeToACP(uStr,CP_UTF8);
	TDEL(uStr);
	CString toencrypt = aStr;
	TDEL(aStr);
	int strlen = toencrypt.GetLength();
// 	int spacecount = (8-strlen%8)%8;
// 	for (int i = 0;i<spacecount;i++)
// 	{
// 		toencrypt+=" ";
// 	}
// 	strlen = (strlen+7)/8*8;
	BYTE* pByte = (BYTE*)toencrypt.GetBuffer(0);
	char lpDestStr[4096];
	int destLen = 4096;
	BYTE* pOutByte = NULL;
	int retlen = Encrypt(pByte,strlen,TRUE,&pOutByte);
	if (retlen>0)
	{
		Base64Encode(pOutByte,retlen,lpDestStr,&destLen);
		if(destLen > 0)
			lpDestStr[destLen] = '\0';
		if(pOutByte)
			free(pOutByte);
	}
	CString DestStr = lpDestStr;
	DestStr.Remove('\r');
	DestStr.Remove('\n');

	int len = DestStr.GetLength();
	const char* pMd5 = DestStr;
	int md5Len = 0;
	BYTE* md5 = MacHash((BYTE*)pMd5,DestStr.GetLength(),&md5Len);
	char lpMd5Str[4096] = "";
	int md5RetLen = 4096;
	Base64Encode(md5,md5Len,lpMd5Str,&md5RetLen);
	lpMd5Str[md5RetLen]=0;
	CString sign = lpMd5Str;
	DestStr = DestStr.TrimRight();
	DestStr = urlstr+"content="+URLEncode(DestStr)+"&clientid=UPWEBSITE"+"&sign="+URLEncode(sign);
	//DestStr = urlstr+"content="+DestStr+"&clientid=UPWEBSITE"+"&sign="+sign;
	if (md5)
	{
		free(md5);
	}
	
	return DestStr;
}

CString CFXSHDManage::URLEncode(const char* str)
{
	CString strTemp = "";
	size_t length = strlen(str);
	for (size_t i = 0; i < length; i++)
	{
		if (isalnum((unsigned char)str[i]) || 
			(str[i] == '-') ||
			(str[i] == '_') || 
			(str[i] == '.') || 
			(str[i] == '~'))
			strTemp += str[i];
		else if (str[i] == ' ')
			strTemp += "+";
		else
		{
			strTemp += '%';
			strTemp += ToHex((unsigned char)str[i] >> 4);
			strTemp += ToHex((unsigned char)str[i] % 16);
		}
	}
	return strTemp;
}

unsigned char CFXSHDManage::ToHex(unsigned char x)
{
	return  x > 9 ? x + 55 : x + 48;
}

MapRoomIDToUser* CFXSHDManage::GetRoomUser()
{
	return &m_mapRoomIDUser;
}