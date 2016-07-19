#include "stdafx.h"
#include "char2wchar.h"
CUser::CUser()
:m_char(NULL)
, m_wchar(NULL)
{
}
CUser::~CUser()
{
	Release();
}
char* CUser::WcharToChar(const wchar_t* wp)
{
	Release();
	int len = WideCharToMultiByte(CP_ACP, 0, wp, wcslen(wp), NULL, 0, NULL, NULL);
	m_char = new char[len + 1];
	WideCharToMultiByte(CP_ACP, 0, wp, wcslen(wp), m_char, len, NULL, NULL);
	m_char[len] = '\0';
	return m_char;
}
wchar_t* CUser::CharToWchar(const char* c)
{
	Release();
	int len = MultiByteToWideChar(CP_ACP, 0, c, strlen(c), NULL, 0);
	m_wchar = new wchar_t[len + 1];
	MultiByteToWideChar(CP_ACP, 0, c, strlen(c), m_wchar, len);
	m_wchar[len] = '\0';
	return m_wchar;
}
void CUser::Release()
{
	if (m_char)
	{
		delete m_char;
		m_char = NULL;
	}
	if (m_wchar)
	{
		delete m_wchar;
		m_wchar = NULL;
	}
}
char* CUser::StringToChar(const string& s)
{
	return const_cast<char*>(s.c_str());
}
char* CUser::WstringToChar(const std::wstring &ws)
{
	const wchar_t* wp = ws.c_str();
	return WcharToChar(wp);
}
wchar_t* CUser::WstringToWchar(const std::wstring &ws)
{
	return const_cast<wchar_t*>(ws.c_str());
}
wchar_t* CUser::StringToWchar(const string& s)
{
	const char* p = s.c_str();
	return CharToWchar(p);
}
string CUser::WstringToString(const std::wstring &ws)
{
	string s;
	char* p = WstringToChar(ws);
	s.append(p);
	return s;
}
wstring CUser::StringToWstring(const std::string &s)
{
	wstring ws;
	wchar_t* wp = StringToWchar(s);
	ws.append(wp);
	return ws;
}