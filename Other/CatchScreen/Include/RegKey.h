/////////////////////////////////////////////////////////////////////////////
/**
* 文件名  ：RegKey.h
* 版本号  ：V1.0
* 项目名称：
* 版权声明：Copyright 2002-2003 LuoWei. All Rights Reserved.
* 模块简单描述： 读写注册表类头文件
*======================================================
* 版本变更记录:
*     
*
* 
*/

/////////////////////////////////////////////////////////////////////////////



#if !defined(_AFX_REGKEY_H_)
#define _AFX_REGKEY_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/*
#if !defined _WINDEF_
   #include <WinDef.h>
#endif
*/

#if !defined _WINREG_
   #include <WinReg.h>
#endif
/*下面的宏在winreg.h中已定义
#define HKEY_CLASSES_ROOT           (( HKEY ) 0x80000000 )
#define HKEY_CURRENT_USER           (( HKEY ) 0x80000001 )
#define HKEY_LOCAL_MACHINE          (( HKEY ) 0x80000002 )
#define HKEY_USERS                  (( HKEY ) 0x80000003 )
#define HKEY_PERFORMANCE_DATA       (( HKEY ) 0x80000004 )
#if(WINVER >= 0x0400)
#define HKEY_CURRENT_CONFIG         (( HKEY ) 0x80000005 )
#define HKEY_DYN_DATA               (( HKEY ) 0x80000006 )
*/

//******************************************************************
#if !defined _WINNT_
   #include <winnt.h>
#endif
/*下面的宏在winnt.h中已定义
#define REG_NONE                    ( 0 )   // No value type
#define REG_SZ                      ( 1 )   // Unicode nul terminated string
#define REG_EXPAND_SZ               ( 2 )   // Unicode nul terminated string
                                            // (with environment variable references)
#define REG_BINARY                  ( 3 )   // Free form binary
#define REG_DWORD                   ( 4 )   // 32-bit number
#define REG_DWORD_LITTLE_ENDIAN     ( 4 )   // 32-bit number (same as REG_DWORD)
#define REG_DWORD_BIG_ENDIAN        ( 5 )   // 32-bit number
#define REG_LINK                    ( 6 )   // Symbolic Link (unicode)
#define REG_MULTI_SZ                ( 7 )   // Multiple Unicode strings
#define REG_RESOURCE_LIST           ( 8 )   // Resource list in the resource map
#define REG_FULL_RESOURCE_DESCRIPTOR ( 9 )  // Resource list in the hardware description
#define REG_RESOURCE_REQUIREMENTS_LIST ( 10 )
*/
/////////////////////////////////////////////////////////////////////////////


// 以下几个常数已经在系统文件<WINREG.H>中定义了
//下面重定义枚举:键名，以便使用(最前面加了一下划线)
	typedef enum KEYNAME
	{
		_HKEY_CLASSES_BOOT = 0,
		_HKEY_CURRENT_USER,
		_HKEY_LOCAL_MACHINE,
		_HKEY_USERS,
		_HKEY_CURRENT_CONFIG,
		_HKEY_DYN_DATA
	};


//读写注册表的类
class CRegKey
{

  public:
	CRegKey();	
   ~CRegKey();

 
  private:


  protected:
    DWORD m_szStrLen;   //字符串类型值长度(一般为CString.GetLength()+1) ,用于定注册表用
	HKEY GetKeyName(UINT mainKeyName);
  public:

	  BOOL ReadRegValue(UINT mainKeyName, CString SubKeyName, CString ValueName, CString &value);
	  BOOL ReadRegValue(UINT mainKeyName, CString SubKeyName, CString ValueName, DOUBLE &value);
	  BOOL ReadRegValue(UINT mainKeyName, CString SubKeyName, CString ValueName, LONG &value);
	  BOOL ReadRegValue(UINT mainKeyName, CString SubKeyName, CString ValueName, INT &value);

	  BOOL ReadRegValue(HKEY mainKeyName, CString SubKeyName, CString ValueName, INT &value);
	  BOOL ReadRegValue(HKEY mainKeyName, CString SubKeyName, CString ValueName, LONG &value);
	  BOOL ReadRegValue(HKEY mainKeyName, CString SubKeyName, CString ValueName, DOUBLE &value);
	  BOOL ReadRegValue(HKEY mainKeyName,  CString SubKeyName, CString ValueName, CString &value);

	  BOOL WriteRegValue(HKEY mainKeyName, CString SubKeyName,  CString ValueName, CString value);
	  BOOL WriteRegValue(HKEY mainKeyName,  CString SubKeyName, CString VlaueName, LONG value);
	  BOOL WriteRegValue(HKEY mainKeyName,  CString SubKeyName, CString VlaueName, DOUBLE value);
	  BOOL WriteRegValue(HKEY mainKeyName,  CString SubKeyName, CString VlaueName, INT value);

	  BOOL WriteRegValue(UINT mainKeyName,  CString SubKeyName, CString VlaueName, CString value);
	  BOOL WriteRegValue(UINT mainKeyName,  CString SubKeyName, CString VlaueName, INT value);
	  BOOL WriteRegValue(UINT mainKeyName,  CString SubKeyName, CString VlaueName, DOUBLE value);
	  BOOL WriteRegValue(UINT mainKeyName,  CString SubKeyName, CString VlaueName, LONG value);





};

#endif