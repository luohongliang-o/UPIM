/////////////////////////////////////////////////////////////////////////////
/**
* 文件名  ：IniFile.h
* 版本号  ：V1.0
* 项目名称：
* 版权声明：Copyright 2007 . All Rights Reserved.
* 模块简单描述：读写.ini文件的类的头文件(API方式)
*======================================================
* 版本变更记录:
*     
*
* 
*/
/////////////////////////////////////////////////////////////////////////////
#ifndef _READWRETE_INIFILE_H_
#define _READWRETE_INIFILE_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


///////////////////////////////////////////////////////////////////////
/*
*类名    : CIniFile 
*说明    : 读写INI文件的类
*变更记录:
*
*/
class CIniFile  
{
public:
	BOOL WriteIni(CString SectionTxt, CString ItemTxt, COleDateTime dateValue);
	BOOL WriteIni(CString SectionTxt, CString ItemTxt, COLORREF colValue);
	BOOL WriteIni(CString SectionTxt, CString ItemTxt, DOUBLE doubleValue,INT dec=3);
	BOOL WriteIni(CString SectionTxt, CString ItemTxt, INT intValue);
	BOOL WriteIni(CString SectionTxt, CString ItemTxt, LONG longValue);
	BOOL WriteIni(CString SectionTxt, CString ItemTxt, CString strValue);

	BOOL ReadIni(CString SectionTxt, CString ItemTxt, COleDateTime &dateValue);
	BOOL ReadIni(CString SectionTxt, CString ItemTxt, COLORREF &colValue);
	BOOL ReadIni(CString SectionTxt, CString ItemTxt, DOUBLE &doubleValue);
	BOOL ReadIni(CString SectionTxt, CString ItemTxt, LONG &longValue);
	BOOL ReadIni(CString SectionTxt, CString ItemTxt, INT &intValue);
	BOOL ReadIni(CString SectionTxt,CString ItemTxt,CString &strValue);

	void SetIniFile(CString iniFileName);
	CIniFile();	//默认构造函数
	CIniFile(CString iniFileName);

	void checkIniFile();
    CString GetAppPath();  //得到应用程序安装路径
	BOOL FileExist(LPCTSTR szFindPath); //文件是否存在
protected:
	BOOL isNumber(CString str);
	INT StrToInt(CString str);
	CString IntToStr(INT n);
	LONG StrToLong(CString str);
	CString LongToStr(LONG n);
	DOUBLE StrToDouble(CString str);
	CString DoubleToStr(DOUBLE d,INT decimal=3);

	CString OleDateTimeToStr(COleDateTime dt,BOOL bHasTime = TRUE);
	COleDateTime StrToOleDateTime(CString str);

    CString TimeToStr(CTime time,BOOL bHasTime = TRUE);
	CTime StrToTime(CString str);

    BOOL ShowOpenFileDialog(CString &filePathName, CString DefPath = "", CString Filter = "所有文件(*.*)|*.*|", CString Title = "打开文件");
	BOOL ShowSaveFileDialog(CString &filePathName, CString DefPath = "", CString Filter = "所有文件(*.*)|*.*|", CString Title = "保存文件");



private:
	CString m_iniFileName;
	BOOL  m_bCheck;
};

#endif