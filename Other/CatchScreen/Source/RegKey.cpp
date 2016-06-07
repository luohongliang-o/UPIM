/////////////////////////////////////////////////////////////////////////////
/**
* 文件名  ：RegKey.cpp
* 版本号  ：V1.0
* 项目名称：
* 版权声明：Copyright 2002-2003 LuoWei. All Rights Reserved.
* 模块简单描述： 读写注册表类模块文件
*======================================================
* 版本变更记录:
*     
*
* 
*/

/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "../include/RegKey.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


LPBYTE CString_To_LPBYTE(CString str)
{
	LPBYTE lpb=new BYTE[str.GetLength()+1];
	for(int i=0;i<str.GetLength();i++)
		lpb[i]=str[i];
	lpb[str.GetLength()]=0;
	return lpb;
}

CRegKey::CRegKey()
{

}

CRegKey::~CRegKey()
{

}

//新建或重写指定的键值
BOOL CRegKey::WriteRegValue(UINT mainKeyName, CString SubKeyName,  CString ValueName, CString value)
{
  return WriteRegValue(GetKeyName(mainKeyName), SubKeyName, ValueName, value);
}

BOOL CRegKey::WriteRegValue(UINT mainKeyName, CString SubKeyName,  CString ValueName, DOUBLE value)
{
  return WriteRegValue(GetKeyName(mainKeyName), SubKeyName, ValueName, value);
}
BOOL CRegKey::WriteRegValue(UINT mainKeyName, CString SubKeyName,  CString ValueName, LONG value)
{
  return WriteRegValue(GetKeyName(mainKeyName), SubKeyName, ValueName, value);
}
BOOL CRegKey::WriteRegValue(UINT mainKeyName, CString SubKeyName,  CString ValueName, INT value)
{
  return WriteRegValue(GetKeyName(mainKeyName), SubKeyName, ValueName, value);
}


/*
BOOL CRegKey::WriteRegValue(HKEY mainKeyName,  CString SubKeyName, CString ValueName, BOOL value)
{ 

	CString str;
	if(value)
		str = "1";
	else
		str = "0";
    return  WriteRegValue(mainKeyName, SubKeyName,  ValueName, str);
}
*/

BOOL CRegKey::WriteRegValue(HKEY mainKeyName,  CString SubKeyName, CString ValueName, INT value)
{ 
	CString str = "0";
	str.Format("%d",value);
    return  WriteRegValue(mainKeyName, SubKeyName,  ValueName, str);
}

BOOL CRegKey::WriteRegValue(HKEY mainKeyName,  CString SubKeyName, CString ValueName, DOUBLE value)
{     	
	CString str = "0.00";
	str.Format("%f",value);
    return  WriteRegValue(mainKeyName, SubKeyName,  ValueName, str);
}

BOOL CRegKey::WriteRegValue(HKEY mainKeyName,  CString SubKeyName, CString ValueName, LONG value)
{ 
	CString str = "0";
	str.Format("%ld",value);
    return  WriteRegValue(mainKeyName, SubKeyName,  ValueName, str);
}

BOOL CRegKey::WriteRegValue(HKEY mainKeyName, CString SubKeyName,  CString ValueName, CString value)
{ 

	   HKEY hKEY;//定义有关的hKEY,在查询结束时要关闭

	  //打开与路径 pKeyName相关的hKEY
	  LPCTSTR pKeyName = SubKeyName;

	//访问注册表，hKEY则保存此函数所打开的键的句柄
	long ret0=(::RegOpenKeyEx(mainKeyName,pKeyName,0,KEY_WRITE,&hKEY));


	if(ret0!=ERROR_SUCCESS)//如果无法打开hKEY,则中止程序的执行
	{
		TRACE0("错误：无法打开有关的hKEY");
		return FALSE;
	}

	//设置有关的数据
	LPBYTE ValueStr = CString_To_LPBYTE(value);  //要写的字符串值

    //数据类型:这个函数肯定为字符串类型
// unsigned long type = REG_SZ;

	unsigned long strLen = value.GetLength() + 1;//定义数据长度


	long ret1=::RegSetValueEx(hKEY, ValueName, NULL, REG_SZ, ValueStr, strLen);

	if(ret1!=ERROR_SUCCESS)
	{
		TRACE0("错误：无法设置有关的注册表信息");
		return FALSE;
	}

	::RegCloseKey(hKEY);

	if(ValueStr)
        delete ValueStr;    //@@@@@@@@@这一行是自加的,因为函数返回的值是NEW的，需要释放,否则会内存泄漏

    return TRUE;
}



BOOL CRegKey::ReadRegValue(HKEY mainKeyName, CString SubKeyName, CString ValueName, CString &value)
{
	HKEY hKEY;//定义有关的hKEY,在查询结束时要关闭

	//打开与路径 data_Set相关的hKEY
	LPCTSTR data_Set = SubKeyName;
	//访问注册表，hKEY则保存此函数所打开的键的句柄
	long ret0=(::RegOpenKeyEx(mainKeyName,data_Set,0,KEY_READ,&hKEY));
	if(ret0!=ERROR_SUCCESS)//如果无法打开hKEY,则中止程序的执行
	{
		TRACE0("错误：无法打开有关的hKEY\n");
		return FALSE;
	}

	//查询有关的数据
	LPBYTE pValue = new BYTE[1028];  //定义得到值的指针
	unsigned long Vtype = REG_SZ;            //定义数据类型
	unsigned long strLen = 1028;               //定义数据长度

	long ret1=::RegQueryValueEx(hKEY, ValueName, NULL, &Vtype, pValue, &strLen);
	if(ret1!=ERROR_SUCCESS)
	{
		TRACE0("错误：无法查询有关的注册表信息\n");
		return FALSE;
	}

	//显示信息
	value = CString(pValue);
	delete[] pValue;

	//程序结束，关闭打开的hKEY
	::RegCloseKey(hKEY);

    return TRUE;
}



BOOL CRegKey::ReadRegValue(HKEY mainKeyName, CString SubKeyName, CString ValueName, DOUBLE &value)
{  
	CString str;
	ReadRegValue(mainKeyName, SubKeyName, ValueName, str);
    try{ value = atof(str);}
	catch(...)
	{value = 0.00;}
    return TRUE;
}

BOOL CRegKey::ReadRegValue(HKEY mainKeyName, CString SubKeyName, CString ValueName, LONG &value)
{
 	CString str;
	ReadRegValue(mainKeyName, SubKeyName, ValueName, str);
    try{ value = atol(str);}
	catch(...)
	{value = 0;}
    return TRUE;
}

BOOL CRegKey::ReadRegValue(HKEY mainKeyName, CString SubKeyName, CString ValueName, INT &value)
{  
  	CString str;
	ReadRegValue(mainKeyName, SubKeyName, ValueName, str);
    try{ value = atoi(str);}
	catch(...)
	{value = 0;}
    return TRUE;

}



BOOL CRegKey::ReadRegValue(UINT mainKeyName, CString SubKeyName, CString ValueName, CString &value)
{
  return ReadRegValue(GetKeyName(mainKeyName), SubKeyName, ValueName, value);
}
BOOL CRegKey::ReadRegValue(UINT mainKeyName, CString SubKeyName, CString ValueName, DOUBLE &value)
{
  return ReadRegValue(GetKeyName(mainKeyName), SubKeyName, ValueName, value);
}
BOOL CRegKey::ReadRegValue(UINT mainKeyName, CString SubKeyName, CString ValueName, LONG &value)
{
  return ReadRegValue(GetKeyName(mainKeyName), SubKeyName, ValueName, value);
}
BOOL CRegKey::ReadRegValue(UINT mainKeyName, CString SubKeyName, CString ValueName, INT &value)
{
  return ReadRegValue(GetKeyName(mainKeyName), SubKeyName, ValueName, value);
}








HKEY CRegKey::GetKeyName(UINT mainKeyName)
{
     HKEY mainKEY;//定义有关的hKEY,在查询结束时要关闭
	switch(mainKeyName)
	{
	case _HKEY_CLASSES_BOOT:
        mainKEY = HKEY_CLASSES_ROOT;
		break;
	case _HKEY_CURRENT_USER:
        mainKEY =HKEY_CURRENT_USER;
		break;

	case _HKEY_LOCAL_MACHINE:
        mainKEY =HKEY_LOCAL_MACHINE;
		break;

	case _HKEY_USERS:
        mainKEY = HKEY_USERS;
		break;

	case _HKEY_CURRENT_CONFIG:
        mainKEY = HKEY_CURRENT_CONFIG;
		break;

	case _HKEY_DYN_DATA:
        mainKEY = HKEY_DYN_DATA;
		break;
    default:
       mainKEY =NULL;

	}
	return mainKEY;
}
