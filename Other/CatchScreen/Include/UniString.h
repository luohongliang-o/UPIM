/////////////////////////////////////////////////////////////////////////////
/**
* 文件名  ：UniString.h
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

#ifndef __UNISTRING_H__
#define __UNISTRING_H__

#include <iostream.h>
#include <string.h>
#include <wtypes.h>

#ifndef PASCAL
#define PASCAL      __stdcall
#endif

#define INIT_BUFFER_LEN 100

#ifndef INT_MAX
#define INT_MAX       2147483647 
#endif

#define INIT_BUFFER_LEN 100

#define _UNISTRING
#ifndef AFXAPI
	#define AFXAPI __stdcall
#endif

//若是MFC支持，请去除下面的定义前的注释

#define _MFC_SUPPORT


class CUniString
{
public:

#ifdef _MFC_SUPPORT
	  CString m_pchDebugData;

#else
	  LPCTSTR m_pchDebugData;
#endif

   long GetBufferStr(LPTSTR & bufStr, long bufSize);    //增加：将双字节类型转化为系统默认类型
   int Find(int nFirst, int nEnd, CUniString findStr);  //增加：在字符串中查找findStr字符串

     // 构造函数
    CUniString( ) : m_pchData( NullString ), m_nBufferLen( -1 ) { }
    CUniString( const LPWSTR Value );
    CUniString( const LPCSTR Value );
    CUniString( const CUniString & Value );

    // 析构函数
    ~CUniString( ) { if( m_nBufferLen != -1 ) delete [ ] m_pchData; }

    //类似MFC中的 CString 函数和方法
    int	 GetLength( ) const { return wcslen( m_pchData ); }
	BOOL IsEmpty() const;
	void Empty();                       // free up the data
    LPWSTR GetBuffer( int MaxStrLen );
	LPWSTR GetBufferSetLength(int nNewLength);

	CUniString Mid(int nFirst, int nCount) const;
	CUniString Mid(int nFirst) const;
	CUniString Left(int nCount) const;
	CUniString Right(int nCount) const;

//类型转换
	int			UniStrToInt(CUniString str);
	CUniString  IntToUniStr(int intV);
	long		UniStrToLong(CUniString str);
	CUniString  LongToUniStr(long longV);
	double		UniStrToDouble(CUniString str);
	CUniString  DoubleToUniStr(double doubleV, int defPoint=6);	



	// 操作符重载
    const CUniString & operator=( const CUniString & Rhs );

    const CUniString & operator=( LPWSTR Rhs );
#ifdef  _MFC_SUPPORT
	const CUniString & operator=( const CString & string );
#endif
	const CUniString & operator=( const LPCSTR Rhs );


	operator LPWSTR() const;          

     //增加类型转换
	operator LPTSTR() const;         
	

    const CUniString & operator=( const WCHAR Rhs );
    const CUniString & operator+=( const WCHAR Rhs );
	const CUniString & operator+=( const LPWSTR lpsz );
	const CUniString & operator+=( const CUniString& string );

     #ifdef _MFC_SUPPORT
		const CUniString & operator+=( const CString& string );
    #endif

    WCHAR operator[ ]( int Index ) const;
    WCHAR & operator[ ]( int Index );

    // 比照友元函数
    friend int operator ==
        ( const CUniString & Lhs, const CUniString & Rhs );
    friend int operator !=
        ( const CUniString & Lhs, const CUniString & Rhs );
    friend int operator <
        ( const CUniString & Lhs, const CUniString & Rhs );
    friend int operator >
        ( const CUniString & Lhs, const CUniString & Rhs );
    friend int operator <=
        ( const CUniString & Lhs, const CUniString & Rhs );
    friend int operator >=
        ( const CUniString & Lhs, const CUniString & Rhs );

    friend CUniString AFXAPI operator+(const CUniString& string1, const CUniString& string2);
	#ifdef _MFC_SUPPORT
		
		friend CArchive& AFXAPI operator<<(CArchive& ar, const CUniString& string);
		friend CArchive& AFXAPI operator>>(CArchive& ar, CUniString& string);
    #endif

private:
    int		m_nBufferLen;       // 缓冲区最大长度
    static	LPWSTR NullString;  // 示初始化情况的成员：空双字节字符
	void	Release();

//	#ifdef _MFC_SUPPORT
		static int PASCAL SafeStrlen(const LPWSTR lpsz);
 //    #endif

	// 初始化函数
	void Init();
	void AllocBuffer(int nLen);
	void AllocCopy(CUniString& dest, int nCopyLen, int nCopyIndex, int nExtraLen) const;
	void ConcatInPlace(int nSrcLen, const LPWSTR lpszSrcData);

public:


	void ConcatCopy(int nSrc1Len, LPWSTR lpszSrc1Data,	int nSrc2Len, LPWSTR lpszSrc2Data);
    LPWSTR	m_pchData;			// 缓冲区的字符串内容存在此成员变量中
};


istream & operator>>( istream & In, CUniString & Value );


inline int
operator==( const CUniString & Lhs, const CUniString & Rhs )
{
    return wcscmp( Lhs.m_pchData, Rhs.m_pchData ) == 0;
}

inline int
operator!=( const CUniString & Lhs, const CUniString & Rhs )
{
    return wcscmp( Lhs.m_pchData, Rhs.m_pchData ) != 0;
}

inline int
operator<( const CUniString & Lhs, const CUniString & Rhs )
{
    return wcscmp( Lhs.m_pchData, Rhs.m_pchData ) < 0;
}

inline int
operator>( const CUniString & Lhs, const CUniString & Rhs )
{
    return wcscmp( Lhs.m_pchData, Rhs.m_pchData ) > 0;
}

inline int
operator<=( const CUniString & Lhs, const CUniString & Rhs )
{
    return wcscmp( Lhs.m_pchData, Rhs.m_pchData ) <= 0;
}

inline int
operator>=( const CUniString & Lhs, const CUniString & Rhs )
{
    return wcscmp( Lhs.m_pchData, Rhs.m_pchData ) >= 0;
}

#endif // __UNISTRING_H__
