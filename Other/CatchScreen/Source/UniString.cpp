/////////////////////////////////////////////////////////////////////////////
/**
* 文件名  ：UniString.cpp
* 版本号  ： V1.0
* 项目名称： 双字节处理字符串的类
* 版权声明： Copyright 2005 Luowei. All Rights Reserved.
* 模块简单描述：
*======================================================
* 版本变更记录:
*      
*      2005-3-19  v1.0 罗伟
*/
/////////////////////////////////////////////////////////////////////////////

//若是MFC支持，请使用下面的文件包含
#include "stdafx.h"

#include "../include/UniString.h"
#include <stdlib.h>
////////////////////////////////////////////////////////////////////////
//静态成员值
LPWSTR CUniString::NullString = (LPWSTR) "";


////////////////////////////////////////////////////////////////////////
// 构造函数

CUniString::CUniString( const CUniString & string )
{
    AllocBuffer( wcslen( string.m_pchData ) );
    
	//wcscpy( m_pchData, string.m_pchData );
	if(wcslen(string.m_pchData) > 0)
		wcscpy( m_pchData, string.m_pchData );
	else {
		if(m_pchData != NullString)
			m_pchData[0] = 0;
	}

#ifdef _MFC_SUPPORT
	m_pchDebugData = m_pchData;
#else 
	m_pchDebugData = LPCSTR(m_pchData);
#endif
}

CUniString::CUniString( const LPWSTR string )
{
    if( string == NULL )      // Use "" if string is NULL
    {
        Init();	// ???
        m_pchData[ 0 ] = '\0';
    }
    else
    {
        AllocBuffer( wcslen( string ) );
        wcscpy( m_pchData, string );
    }

#ifdef _MFC_SUPPORT
	m_pchDebugData = m_pchData;
#else 
	m_pchDebugData = LPCSTR(m_pchData);
#endif
}

CUniString::CUniString( const LPCSTR string )
{
    if( string == NULL )      // Use "" if string is NULL
    {
        Init();	// ???
        m_pchData[ 0 ] = '\0';
    }
    else
    {
		WCHAR *sUni;
		int nLen = strlen(string);

#ifdef _UNICODE
		sUni = new WCHAR[nLen+1];
		wcscpy(sUni, string);
#else
		nLen = MultiByteToWideChar(CP_ACP, 0, string, -1, NULL, NULL);
		sUni = new WCHAR[nLen];
		MultiByteToWideChar(CP_ACP, 0, string, -1, (LPWSTR)sUni, nLen);
#endif

        AllocBuffer( wcslen( sUni ) );
        wcscpy( m_pchData, sUni );

		delete sUni;
    }

#ifdef _MFC_SUPPORT
	m_pchDebugData = m_pchData;
#else 
	m_pchDebugData = LPCSTR(m_pchData);
#endif
}


////////////////////////////////////////////////////////////////////////
// Initialize functions

void 
CUniString::Init()
{
	m_pchData = NullString;
	m_nBufferLen = -1;

#ifdef _MFC_SUPPORT
	m_pchDebugData = m_pchData;
#else 
	m_pchDebugData = LPCSTR(m_pchData);
#endif
}


////////////////////////////////////////////////////////////////////////
// Member functions

// Return buffer pointer
LPWSTR CUniString::GetBuffer(int nMinBufLength)
{
#ifdef  _MFC_SUPPORT
	ASSERT(nMinBufLength >= 0);
	// return a pointer to the character storage for this string
	ASSERT(m_pchData != NULL);
#endif
	return m_pchData;
}

// Set buffer length.
// Allocate new memory if it is needed.
LPWSTR CUniString::GetBufferSetLength(int nNewLength)
{
#ifdef  _MFC_SUPPORT
	ASSERT(nNewLength >= 0);
#endif
	if(GetLength()<nNewLength) {
		AllocBuffer(nNewLength);
		m_pchData[nNewLength] = '\0';
	}
	else {
		;
	}
	return m_pchData;
}

// Empty the buffer and set buffer NullString
void 
CUniString::Empty()
{
	if(m_nBufferLen != -1)
		delete [] m_pchData;

	m_pchData = NullString;
	m_nBufferLen = -1;

#ifdef _MFC_SUPPORT
	m_pchDebugData = m_pchData;
#endif
}

// Check the StringLength is less than or equal 0
BOOL 
CUniString::IsEmpty() const
{
	if(GetLength()<=0)
		return TRUE;

	return FALSE;
}

// Always allocate one extra character for '\0' termination
// Assumes [optimistically] that data length will equal allocation length
void CUniString::AllocBuffer(int nLen)
{
#ifdef  _MFC_SUPPORT
	ASSERT(nLen >= 0);
	ASSERT(nLen <= INT_MAX-1);    // max size (enough room for 1 extra)
#endif
	if (nLen == 0)
		Init();
	else
	{
		m_pchData = (LPWSTR)new BYTE[(nLen+1)*sizeof(WCHAR)];
		m_pchData[nLen] = '\0';
		m_nBufferLen = nLen;
	}

#ifdef _MFC_SUPPORT
	m_pchDebugData = m_pchData;
#endif
}

// Release the buffer
void 
CUniString::Release()
{
	if (m_nBufferLen != -1)
	{
		delete [] m_pchData;
		Init();
		//AllocBuffer(INIT_BUFFER_LEN);
	}
}


////////////////////////////////////////////////////////////////////////
// CString like method: Left, Mid, Right

CUniString CUniString::Mid(int nFirst) const
{
	return Mid(nFirst, GetLength() - nFirst);
}

CUniString CUniString::Mid(int nFirst, int nCount) const
{
	// out-of-bounds requests return sensible things
	if (nFirst < 0)
		nFirst = 0;
	if (nCount < 0)
		nCount = 0;

	if (nFirst + nCount > GetLength())
		nCount = GetLength() - nFirst;
	if (nFirst > GetLength())
		nCount = 0;

	CUniString dest;
	AllocCopy(dest, nCount, nFirst, 0);
	
#ifdef _MFC_SUPPORT
	dest.m_pchDebugData = dest.m_pchData;
#endif
	return dest;
}

CUniString 
CUniString::Right(int nCount) const
{
	if (nCount < 0)
		nCount = 0;
	else if (nCount > GetLength())
		nCount = GetLength();

	CUniString dest;
	AllocCopy(dest, nCount, GetLength()-nCount, 0);

#ifdef _MFC_SUPPORT
	dest.m_pchDebugData = dest.m_pchData;
#endif
	return dest;
}

CUniString 
CUniString::Left(int nCount) const
{
	if (nCount < 0)
		nCount = 0;
	else if (nCount > GetLength())
		nCount = GetLength();

	CUniString dest;
	AllocCopy(dest, nCount, 0, 0);

#ifdef _MFC_SUPPORT
	dest.m_pchDebugData = dest.m_pchData;
#endif
	return dest;
}

//#ifdef  _MFC_SUPPORT
int PASCAL CUniString::SafeStrlen(LPWSTR lpsz)
{
	return wcslen( lpsz );
}
//#endif

////////////////////////////////////////////////////////////////////////
// 操作符重载
#ifdef  _MFC_SUPPORT

const CUniString & CUniString::operator=(const CString& string)
		{
			WCHAR *sUni;
			int nLen;
			nLen = string.GetLength();

		#ifdef _UNICODE
			sUni = new WCHAR[nLen+1];
			wcscpy(sUni, string);
		#else
			nLen = MultiByteToWideChar(CP_ACP, 0, string, -1, NULL, NULL);
			sUni = new WCHAR[nLen];
			MultiByteToWideChar(CP_ACP, 0, string, -1, (LPWSTR)sUni, nLen);
		#endif

			const int Len = wcslen( sUni );

			if( Len >= m_nBufferLen )
			{
				if( m_nBufferLen != -1 )
					delete [] m_pchData;
				AllocBuffer( Len );
			}
			wcscpy( m_pchData, sUni );

			delete [] sUni;

		#ifdef _MFC_SUPPORT
			m_pchDebugData = m_pchData;	
		#endif

			return *this;
		}

#endif


const CUniString & CUniString::operator=(const LPCSTR Rhs)
{
	WCHAR *sUni;
	int nLen;
	nLen = strlen(Rhs);
	
	if(nLen == 0) {
		Empty();
		return *this;
	}

	nLen = MultiByteToWideChar(CP_ACP, 0, Rhs, -1, NULL, NULL);
	sUni = new WCHAR[nLen];
	MultiByteToWideChar(CP_ACP, 0, Rhs, -1, (LPWSTR)sUni, nLen);

    const int Len = wcslen( sUni );

    if( Len >= m_nBufferLen )
    {
        if( m_nBufferLen != -1 )
            delete [] m_pchData;
        AllocBuffer( Len );
    }
    wcscpy( m_pchData, sUni );

	delete [] sUni;

#ifdef _MFC_SUPPORT
	m_pchDebugData = m_pchData;	
#endif

	return *this;
}



const CUniString & CUniString::operator=( const CUniString & Rhs )
{
    const int Len = wcslen( Rhs.m_pchData );

    if( this != &Rhs )       // Don't copy to yourself
    {
        if( Len >= m_nBufferLen )
        {
            if( m_nBufferLen != -1 )
                delete [] m_pchData;
            AllocBuffer( Len );
        }
		//wcscpy( m_pchData, Rhs.m_pchData );
		if(Len > 0)
			wcscpy( m_pchData, Rhs.m_pchData );
		else {
			if(m_pchData != NullString)
				m_pchData[0] = 0;
		}
    }

#ifdef _MFC_SUPPORT
	m_pchDebugData = m_pchData;
#endif

    return *this;
}

const CUniString &
CUniString::operator=( LPWSTR Rhs )
{
    if( Rhs == NULL )
        Rhs = L"";

    const int Len = wcslen( Rhs );

    if( Len >= m_nBufferLen )
    {
        if( m_nBufferLen != -1 )
            delete [] m_pchData;
        AllocBuffer( Len );
    }
    wcscpy( m_pchData, Rhs );

#ifdef _MFC_SUPPORT
	m_pchDebugData = m_pchData;
#endif

    return *this;
}

const CUniString &
CUniString::operator=( const WCHAR Rhs )
{
    const int Len = 2;

    if( Len >= m_nBufferLen )
    {
        if( m_nBufferLen != -1 )
            delete [] m_pchData;
        AllocBuffer( Len );
    }
    m_pchData[0] = Rhs;
	m_pchData[1] = 0;

#ifdef _MFC_SUPPORT
	m_pchDebugData = m_pchData;
#endif

    return *this;
}

WCHAR & CUniString::operator[ ]( int Index )
{
#ifdef  _MFC_SUPPORT
    ASSERT(!(Index < 0 || (size_t)Index > wcslen( m_pchData )));
#endif
    return m_pchData[ Index ];
}

WCHAR CUniString::operator[ ]( int Index ) const
{
#ifdef  _MFC_SUPPORT
    ASSERT(!(Index < 0 || (size_t)Index > wcslen( m_pchData )));
#endif
    return m_pchData[ Index ];
}

void CUniString::AllocCopy(CUniString& dest, int nCopyLen, int nCopyIndex, int nExtraLen) const
{
	// will clone the data attached to this string
	// allocating 'nExtraLen' characters
	// Places results in uninitialized string 'dest'
	// Will copy the part or all of original data to start of new string

	int nNewLen = nCopyLen + nExtraLen;
	if (nNewLen == 0)
	{
		dest.Init();
	}
	else
	{
		dest.AllocBuffer(nNewLen);
		memcpy(dest.m_pchData, m_pchData+nCopyIndex, nCopyLen*sizeof(WCHAR));
	}
}

void CUniString::ConcatCopy(int nSrc1Len, LPWSTR lpszSrc1Data,
	                        int nSrc2Len, LPWSTR lpszSrc2Data)
{
	// -- master concatenation routine
	// Concatenate two sources
	// -- assume that 'this' is a new CString object

	int nNewLen = nSrc1Len + nSrc2Len;
	if (nNewLen != 0)
	{
		AllocBuffer(nNewLen);
		memcpy(m_pchData, lpszSrc1Data, nSrc1Len*sizeof(WCHAR));
		memcpy(m_pchData+nSrc1Len, lpszSrc2Data, nSrc2Len*sizeof(WCHAR));
	}
}

CUniString AFXAPI operator+(const CUniString& string1, const CUniString& string2)
{
	CUniString s;
	s.ConcatCopy(string1.GetLength(), string1.m_pchData,
		string2.GetLength(), string2.m_pchData);
	return s;
}

void CUniString::ConcatInPlace(int nSrcLen, const LPWSTR lpszSrcData)
{
	//  -- the main routine for += operators

	// concatenating an empty string is a no-op!
	if (nSrcLen == 0)
		return;

	// if the buffer is too small, or we have a width mis-match, just
	//   allocate a new buffer (slow but sure)
	if (GetLength() + nSrcLen > m_nBufferLen)
	{
		// we have to grow the buffer, use the ConcatCopy routine
		LPWSTR pOldData = m_pchData;
		ConcatCopy(GetLength(), m_pchData, nSrcLen, lpszSrcData);
#ifdef  _MFC_SUPPORT
		ASSERT(pOldData != NULL);
#endif
		
		if(pOldData != NullString)
			delete [] pOldData;
	}
	else
	{
		// fast concatenation when buffer is big enough
		memcpy(m_pchData+GetLength(), lpszSrcData, nSrcLen*sizeof(WCHAR));
#ifdef  _MFC_SUPPORT
		ASSERT(GetLength() <= m_nBufferLen);
#endif
		m_pchData[GetLength()] = '\0';
	}
}

const CUniString & 
CUniString::operator+=(const WCHAR Rhs)
{
	ConcatInPlace(1, (LPWSTR)(&Rhs));

#ifdef _MFC_SUPPORT
	m_pchDebugData = m_pchData;	
#endif
	return *this;
}

const CUniString & CUniString::operator+=(const LPWSTR lpsz)
{
#ifdef  _MFC_SUPPORT
	ASSERT(lpsz == NULL || AfxIsValidString(lpsz, FALSE));
#endif

	ConcatInPlace(SafeStrlen(lpsz), lpsz);

#ifdef _MFC_SUPPORT
	m_pchDebugData = m_pchData;	
#endif
	return *this;
}

const CUniString & 
CUniString::operator+=(const CUniString& string)
{
	ConcatInPlace(string.GetLength(), string.m_pchData);

#ifdef _MFC_SUPPORT
	m_pchDebugData = m_pchData;	
#endif
	return *this;
}

#ifdef  _MFC_SUPPORT
	const CUniString & CUniString::operator+=(const CString& string)
	{
		WCHAR *sUni;
		int nLen;
		nLen = string.GetLength();

	#ifdef _UNICODE
		sUni = new WCHAR[nLen+1];
		wcscpy(sUni, string);
	#else
		nLen = MultiByteToWideChar(CP_ACP, 0, string, -1, NULL, NULL);
		sUni = new WCHAR[nLen];
		MultiByteToWideChar(CP_ACP, 0, string, -1, (LPWSTR)sUni, nLen);
	#endif
		
		ConcatInPlace(nLen, sUni);

	#ifdef _MFC_SUPPORT
		m_pchDebugData = m_pchData;	
	#endif

		delete [] sUni;
		return *this;
	}

#endif

CUniString::operator LPWSTR() const           // as a C string
{
	return (LPWSTR)m_pchData;
}



//////////////////////////////////////////////////////////////////////////////
// CUniString支持流函数
#ifdef  _MFC_SUPPORT

CArchive& AFXAPI operator<<(CArchive& ar, const CUniString& string)
{
	// special signature to recognize unicode strings
#ifdef _UNICODE
	ar << (BYTE)0xff;
	ar << (WORD)0xfffe;
#endif

	if (string.GetLength() < 255)
	{
		ar << (BYTE)string.GetLength();
	}
	else if (string.GetLength() < 0xfffe)
	{
		ar << (BYTE)0xff;
		ar << (WORD)string.GetLength();
	}
	else
	{
		ar << (BYTE)0xff;
		ar << (WORD)0xffff;
		ar << (DWORD)string.GetLength();
	}
	ar.Write(string.m_pchData, string.GetLength()*sizeof(WCHAR));
	return ar;
}

// return string length or -1 if UNICODE string is found in the archive
static UINT AFXAPI ReadStringLength(CArchive& ar)
{
	DWORD nNewLen;

	// attempt BYTE length first
	BYTE bLen;
	ar >> bLen;

	if (bLen < 0xff)
		return bLen;

	// attempt WORD length
	WORD wLen;
	ar >> wLen;
	if (wLen == 0xfffe)
	{
		// UNICODE string prefix (length will follow)
		return (UINT)-1;
	}
	else if (wLen == 0xffff)
	{
		// read DWORD of length
		ar >> nNewLen;
		return (UINT)nNewLen;
	}
	else
		return wLen;
}

CArchive& AFXAPI operator>>(CArchive& ar, CUniString& string)
{
	int nConvert = 0;   // if we get UNICODE, convert

	UINT nNewLen = ReadStringLength(ar);
	if (nNewLen == (UINT)-1)
	{
		nConvert = 1 - nConvert;
		nNewLen = ReadStringLength(ar);
#ifdef  _MFC_SUPPORT
		ASSERT(nNewLen != -1);
#endif
	}

	// set length of string to new length
	UINT nByteLen = nNewLen * sizeof(WCHAR);

	nByteLen += nByteLen * nConvert;    // bytes to read
	if (nNewLen == 0)
		string.GetBufferSetLength(0);
	else
		string.GetBufferSetLength((int)nNewLen+nConvert);

	// read in the characters
	if (nNewLen != 0)
	{
#ifdef  _MFC_SUPPORT
		ASSERT(nByteLen != 0);
#endif

		// read new data
		if (ar.Read(string.m_pchData, nByteLen) != nByteLen)
			AfxThrowArchiveException(CArchiveException::endOfFile);

		// convert the data if as necessary
		if (nConvert != 0)
		{
			LPWSTR pOldData = string.m_pchData;
			LPWSTR lpsz = (LPWSTR)string.m_pchData;

			lpsz[nNewLen] = '\0';    // must be NUL terminated
			string.Init();   // don't delete the old data
			string = lpsz;   // convert with operator=(LPWCSTR)
			delete [] (BYTE*)pOldData;
		}
	}

#ifdef _MFC_SUPPORT
	string.m_pchDebugData = string.m_pchData;
#endif

	return ar;
}

#endif


//在字符串指定的区间发现第一次出现findStr字符的位置
int CUniString::Find(int nFirst, int nEnd, CUniString findStr)
{
  CUniString str;
   int count = GetLength();
   int fCount = findStr.GetLength(); 
   int pos = -1;
   int i = 0;
   if(count >0 && (nFirst < count && nEnd < count) && (nFirst >= 0 && nEnd >= 0 && nFirst <= nEnd) && fCount > 0)
   {
     for(i = 0; i < nEnd; i++)
	 {
       str = Mid(nFirst + i, fCount);
	   if(str == findStr)
	   {  
		   i = i + nFirst;
		   pos = i;
		   break;
	   }
	}

   }
   return pos;
}

//得到双字节对应的缓冲区中的字符串内容
long CUniString::GetBufferStr(LPTSTR & bufStr, long bufSize)
{
    LPTSTR buf = new CHAR[bufSize];
  *(buf+0) = '\0';

  int nLen =WideCharToMultiByte(CP_ACP,0,m_pchData,-1,NULL,NULL,"",false);
  WideCharToMultiByte(CP_ACP, 0, m_pchData, -1, buf, nLen,"",false);

  strcpy(bufStr,buf);
  delete buf;
  return strlen(bufStr);
}

//重载类型转换函数LPTSTR
CUniString::operator LPTSTR() const          
{ 
  LPTSTR lpsz = new CHAR[1028];
  int nLen =WideCharToMultiByte(CP_ACP,0,m_pchData,-1,NULL,NULL,"",false);

  WideCharToMultiByte(CP_ACP, 0, m_pchData, -1, lpsz, nLen,"",false);

  return lpsz;
}


//类型转换
int	CUniString::UniStrToInt(CUniString str)
{
	int ri = 0;
	if(str.GetLength()>0)
	{
		LPTSTR pStr = LPTSTR(str);
		ri = atoi(pStr);
	}
   return ri;
}

CUniString  CUniString::IntToUniStr(int intV)
{


    CUniString str;
	char ch[10];
	LPTSTR pStr = ch;
	_itoa(intV,pStr,10);
	str = CUniString(pStr);
	return str;
}

long	CUniString::UniStrToLong(CUniString str)
{
	long ri = 0;
	if(str.GetLength()>0)
	{
		LPTSTR pStr = LPTSTR(str);
		ri = atol(pStr);
	}
   return ri;
}

CUniString  CUniString::LongToUniStr(long longV)
{

    CUniString str;
	char ch[10];
	LPTSTR pStr = ch;
	_ltoa(longV,pStr,10);
	str = CUniString(pStr);
	return str;
}
double	CUniString::UniStrToDouble(CUniString str)
{
	double rf = 0;
	if(str.GetLength()>0)
	{
		LPTSTR pStr = LPTSTR(str);
		rf = atof(pStr);
	}
   return rf;
}

CUniString  CUniString::DoubleToUniStr(double doubleV,int defPoint/*=6*/)
{
    CUniString str;
    int  decimal, sign;

	char ch[64];
	LPTSTR pStr = ch; 


	pStr = _fcvt(doubleV,defPoint,&decimal, &sign);

	str = CUniString(pStr);
	return str;

}

