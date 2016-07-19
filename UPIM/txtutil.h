
#pragma once
#include <vector>
#include <string> 

#define UTF2WS(x) epius::txtutil::convert_utf8_to_wcs(x)
#define WS2UTF(x) epius::txtutil::convert_wcs_to_utf8(x)

namespace epius
{
	namespace txtutil
	{
		// 字符编码转换
		std::string convert_wcs_to_utf8( const std::wstring& wcs );
		std::wstring convert_utf8_to_wcs( const std::string& str );
		std::string convert_from_base64( const std::string& base64 );
		std::string convert_from_base64_wcs( const std::wstring& base64 );
		std::string convert_to_base64( const std::string& buf );
		std::wstring convert_to_base64_wcs( const std::string& buf );

		// 字符串常用操作
		void wcs_make_lower(std::wstring& wcs);
		void wcs_make_upper(std::wstring& wcs);
		void wcs_trim_left(std::wstring& wcs, const wchar_t* str = L" \t");
		void wcs_trim_right(std::wstring& wcs, const wchar_t* str = L" \t");
		void wcs_trim(std::wstring& wcs, const wchar_t* str = L" \t");
		bool wcs_split(const std::wstring& wcs, const wchar_t* str, std::vector<std::wstring>& arr);
		std::wstring convert_ansi_to_wcs( const std::string& buffer );
		std::string convert_wcs_to_ansi( const std::wstring& wcs );

		// 字节流与 HEX 字符串转换
		enum hex_case
		{
			HEX_CASE_UPPER,
			HEX_CASE_LOWER
		};
		std::wstring convert_to_hex_wcs( const std::string& buf, hex_case e = HEX_CASE_LOWER );
		std::string convert_from_hex_wcs( const std::wstring& hex );

		std::wstring url_encode( const std::wstring& wcs );
		std::wstring url_decode( const std::wstring& wcs );
	}
}
namespace txtconv = epius::txtutil;