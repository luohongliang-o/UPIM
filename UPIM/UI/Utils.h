#pragma once

#include <vector>
#include <string>
#include <time.h>
#include <gdiplus.h>
#pragma comment(lib, "gdiplus.lib")
using namespace Gdiplus;

#if defined(UNICODE) || defined(_UNICODE)
	typedef std::wstring tstring;
#else
	typedef std::string tstring;
#endif

CHAR * UnicodeToUtf8(const WCHAR * lpszStr);
WCHAR * AnsiToUnicode(const CHAR * lpszStr);
WCHAR * Utf8ToUnicode(const CHAR * lpszStr);
CHAR * UnicodeToAnsi(const WCHAR * lpszStr);

HGLOBAL GlobalAllocByString(const CHAR * lpszText, long nLen = -1);
HGLOBAL GlobalAllocByString(const WCHAR * lpszText, long nLen = -1);

BOOL EnumSysFont(std::vector<tstring> * arrSysFont);	// 枚举系统字体
BOOL FlashWindowEx(HWND hWnd, int nCount);				// 闪烁窗口标题栏
BOOL GetTrayWndRect(RECT * lpRect);						// 获取系统任务栏区域
const TCHAR * FormatTime(time_t lTime, LPCTSTR lpFmt);

BOOL IsDigit(const CHAR * lpStr);
/*void Replace(std::wstring& strText, const WCHAR * lpOldStr, const WCHAR * lpNewStr);*/
void Replace(std::string& strText, const CHAR * lpOldStr, const CHAR * lpNewStr);
WCHAR ToLower(WCHAR c);
void ToLower(WCHAR * lpText);
void ToLower(std::wstring& strText);
WCHAR ToUpper(WCHAR c);
void ToUpper(WCHAR * lpText);
void ToUpper(std::wstring& strText);

tstring GetBetweenString(const CHAR * pStr, CHAR * cStart, CHAR * cEnd);
int GetBetweenInt(const CHAR * pStr, CHAR * cStart, CHAR * cEnd, int nDefValue = 0);

BOOL DllRegisterServer(LPCTSTR lpszFileName);
BOOL DllUnregisterServer(LPCTSTR lpszFileName);

GUID GetFileTypeGuidByExtension(const WCHAR * lpExtension);
GUID GetFileTypeGuidByExtension(const CHAR * lpExtension);
CLSID GetEncoderClsidByExtension(const WCHAR * lpExtension);
CLSID GetEncoderClsidByFileType(REFGUID guidFileType);
CLSID GetEncoderClsidByMimeType(const WCHAR * lpMineType);