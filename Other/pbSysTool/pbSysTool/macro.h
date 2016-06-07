#ifndef MACRO_H
#define MACRO_H

#pragma pack(push,1)

//////////////////////////////////////////////////////////////////////////
#ifndef TDEL
#define TDEL(a)				if(a!=NULL) { delete a; a=NULL; }
#endif

#ifndef TDELARRAY
#define TDELARRAY(a)		if(a!=NULL) { delete[] a; a=NULL; }
#endif

#ifndef TDELWND
#define TDELWND(a)			if(a){if(IsWindow(a->GetSafeHwnd())) a->DestroyWindow(); TDEL(a);}
#endif

#ifndef CLOSEHANDLE
#define CLOSEHANDLE(a)		if(a!=NULL) { CloseHandle(a); a=NULL; }
#endif

#ifndef INITCOM_EX_MUTI
#define INITCOM_EX_MUTI()	::CoInitializeEx(NULL, COINIT_MULTITHREADED)
#endif

#ifndef INITCOM_EX_APAR
#define INITCOM_EX_APAR()	::CoInitializeEx(NULL, COINIT_APARTMENTTHREADED)
#endif

#ifndef INITCOM
#define INITCOM()			::CoInitialize(NULL)
#endif

#ifndef UNINITCOM
#define UNINITCOM()			::CoUninitialize()
#endif

#ifndef EQUAL_USERID
#define EQUAL_USERID(a,b)	(strcmp(a,b)==0)
#endif

#ifndef COPY_USERID
#define COPY_USERID(a,b)	{ if(b){strncpy(a,b,USERID_SIZE);a[USERID_SIZE-1]=0;}else a[0]=0;}
#endif

#ifndef COPY_NICKNAME
#define COPY_NICKNAME(a,b)	{ if(b){strncpy(a,b,S_NICKNAME_LEN);a[S_NICKNAME_LEN-1]=0;}else a[0]=0;}
#endif

#ifndef USERID_ISNOTNULL
#define USERID_ISNOTNULL(a)	(strlen(a)>0)
#endif

// 时间定义
#ifndef SYSTM_T
#define SYSTM_T
typedef __int64 systm_t;
#endif

// 断言
#ifndef CLIBVERIFY
#define CLIBVERIFY(expr) ((void)0)
#endif

#ifndef CLIBASSERT
#define CLIBASSERT(f)    ((void)0)
#endif

#ifndef offset_of
#define offset_of(s,m)	(size_t)&(((s*)0)->m)
#endif

#ifndef ex_offset_of
#define ex_offset_of(s,m)	((size_t)&(((s*)0x8)->m)-0x8)
#endif

#ifndef min_of
#define min_of(a,b) ((a)<(b)?(a):(b))
#endif

#ifndef MIN
#define MIN(a,b) ((a)<(b)?(a):(b))
#endif

#ifndef min
#define min MIN
#endif

#ifndef max_of
#define max_of(a,b) ((a)>(b)?(a):(b))
#endif

#ifndef MAX
#define MAX(a,b) ((a)>(b)?(a):(b))
#endif

#ifndef max
#define max MAX
#endif

#ifndef size_of
#define size_of(a)			((DWORD)sizeof(a))
#endif

#ifndef ARRAYSIZE
#define SIZEOF(a)			((DWORD)sizeof(a))
#define ARRAYSIZE(a)		((DWORD)((sizeof(a)/sizeof(a[0]))))
#endif


// 字符串复制
#ifndef COPYSTRARRAY
#define COPYSTRARRAY(a,b)	nstrcpy((char*)(a),(b),ARRAYSIZE(a))
#endif

#ifndef CATSTRARRAY
#define CATSTRARRAY(a,b)	nstrcat((char*)(a),(b),ARRAYSIZE(a))
#endif

// 缓冲区处理
#ifndef COPYBINARRAY
#define COPYBINARRAY(a,b)	memcpy((a),(b),sizeof(a))
#endif

#ifndef COPYBINARRAY2
#define COPYBINARRAY2(a,b,c)	if(min(sizeof(a),(c))>0) { memcpy((a),(b),min(sizeof(a),(c))); }
#endif

// X32和X64处理
#ifndef strlen32
#define strlen32(p)	((unsigned int)strlen(p))
#endif

// MAC地址定义
#ifndef MAC_DEFINED
#define MAC_DEFINED
typedef struct tagMAC
{
	// 地址数据
	BYTE m_acMacData[6];
	// 赋值和特殊构造
	static inline tagMAC FromBytes(LPBYTE pBytes) { tagMAC Mac; memcpy(Mac.m_acMacData,pBytes,size_of(Mac.m_acMacData)); return Mac; }
} MAC;
static const MAC EMPTY_MAC={{0,0,0,0,0,0}};		// 空MAC地址
#endif

// IPV4地址定义
typedef struct tagIPV4
{	
	BYTE m_acIpData[4];
} IPV4;		

// IPV6地址定义
typedef struct tagIPV6
{	
	BYTE m_acIpData[16];
} IPV6;		

typedef IPV4 IP;

#ifndef nsprintf
inline int __nsprintf(char* lpszString,long nSize,const char* lpszFmt,...)
{	
	if(nSize<=0) return 0;
	va_list args;
	va_start(args,lpszFmt);
	int nRet=_vsnprintf(lpszString,nSize,lpszFmt,args);
	if(nRet<0||nRet==nSize)
	{	
		lpszString[nSize-1]=0;
		nRet=nSize-1;
	}
	va_end(args);
	return nRet;
}
#define nsprintf __nsprintf
#endif

#ifndef nvsprintf
inline int __nvsprintf(char* lpszString,long nSize,const char* lpszFmt,va_list ap)
{	
	if(nSize<=0) return 0;
	int nRet=_vsnprintf(lpszString,nSize,lpszFmt,ap);
	if(nRet<0||nRet==nSize)
	{	
		lpszString[nSize-1]=0;
		nRet=nSize-1;
	}
	return nRet;
}
#define nvsprintf __nvsprintf
#endif

#ifndef nscatprintf
inline int __nscatprintf(char* lpszString,long nSize,const char* lpszFmt,...)
{	
	if(nSize<=0) return 0;
	va_list args;
	va_start(args,lpszFmt);
	long nOldLen=strlen(lpszString);
	if(nOldLen+1>=nSize) return 0;
	int nRet=_vsnprintf(lpszString+nOldLen,nSize-nOldLen,lpszFmt,args);
	if(nRet<0||nRet==nSize-nOldLen)
	{	
		lpszString[nSize-1]=0;
		nRet=nSize-1-nOldLen;
	}
	va_end(args);
	return nRet;
}
#define nscatprintf __nscatprintf
#endif

#ifndef nvscatprintf
inline int __nvscatprintf(char* lpszString,long nSize,const char* lpszFmt,va_list ap)
{	
	if(nSize<=0) return 0;
	long nOldLen=strlen(lpszString);
	if(nOldLen+1>=nSize) return 0;
	int nRet=_vsnprintf(lpszString+nOldLen,nSize-nOldLen,lpszFmt,ap);
	if(nRet<0||nRet==nSize-nOldLen)
	{	
		lpszString[nSize-1]=0;
		nRet=nSize-1-nOldLen;
	}
	return nRet;
}
#define nvscatprintf __nvscatprintf
#endif

#ifndef nstrcpy
inline void __nstrcpy(char* lpszString,const char* lpszSrc,long nSize)
{	
	if(nSize<=0) return;
	if(lpszSrc!=NULL)
	{	
		strncpy(lpszString,lpszSrc,nSize);
		lpszString[nSize-1]=0;
	}
	else
	{	
		lpszString[0]=0;
	}
}
#define nstrcpy __nstrcpy
#endif

#ifndef nstrcat
inline void __nstrcat(char* lpszFront,const char* lpszBack,long nSize)
{	
	strncat(lpszFront,lpszBack,nSize);
	lpszFront[nSize-1]=0;
}
#define nstrcat __nstrcat
#endif



#pragma pack(pop,1)
#endif