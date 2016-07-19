#include "StdAfx.h"
#include "txtutil.h"
#include <locale>
#include "utf8.h"
#include <memory>
#include <sstream>
#include <algorithm>
using namespace std;

namespace epius
{
	namespace txtutil
	{
		namespace inner
		{
			static const std::string base64_chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZ" "abcdefghijklmnopqrstuvwxyz" "0123456789+/";
			inline bool is_base64( unsigned char c ) 
			{
				return (isalnum(c) || (c == '+') || (c == '/'));
			}
		}

		std::string convert_wchar_to_utf8( wchar_t wch )
		{
			std::string ret;
			utf8::append( wch, back_inserter( ret ) );
			return ret;
		}

		std::string convert_wcs_to_utf8( const std::wstring& wcs )
		{
			std::string ret;
			utf8::utf16to8( wcs.begin(), wcs.end(), back_inserter( ret ) );
			return ret;
		}

		std::wstring convert_utf8_to_wcs( const std::string& str )
		{
			std::wstring wcs;
			try {
				utf8::utf8to16( str.begin(), str.end(), back_inserter( wcs ) );
			}catch(...){return L"";}
			return wcs;
		}

		std::string convert_wcs_to_ansi( const std::wstring& wcs )
		{
			setlocale(LC_ALL,"chs");
			int len=wcslen(wcs.c_str())*2;
			shared_ptr<char> pansi(new char[len+1]);
			int ansiLen = wcstombs(pansi.get(),wcs.c_str(),len+1);
			if(ansiLen>=0)return std::string(pansi.get(),ansiLen);
			else return std::string();
		}

		std::wstring convert_ansi_to_wcs( const std::string& buffer )
		{
			setlocale(LC_ALL,"chs");
			int len = strlen(buffer.c_str());
			shared_ptr<wchar_t> pwide(new wchar_t[len+1]);
			int wlen = mbstowcs(pwide.get(),buffer.c_str(),len);
			if(wlen>=0)return wstring(pwide.get(),wlen);
			else return wstring();
		}
		std::string convert_from_base64( const std::string& base64 )
		{
			int in_len = base64.size();
			int i = 0;
			int j = 0;
			int in_ = 0;
			unsigned char char_array_4[4], char_array_3[3];
			std::string ret;

			while (in_len-- && ( base64[in_] != '=') && inner::is_base64(base64[in_])) {
				char_array_4[i++] = base64[in_]; in_++;
				if (i ==4) {
					for (i = 0; i <4; i++)
						char_array_4[i] = inner::base64_chars.find(char_array_4[i]);

					char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
					char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
					char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

					for (i = 0; (i < 3); i++)
						ret += char_array_3[i];
					i = 0;
				}
			}

			if (i) {
				for (j = i; j <4; j++)
					char_array_4[j] = 0;

				for (j = 0; j <4; j++)
					char_array_4[j] = inner::base64_chars.find(char_array_4[j]);

				char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
				char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
				char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

				for (j = 0; (j < i - 1); j++) ret += char_array_3[j];
			}

			return ret;
		}

		std::string convert_from_base64_wcs( const std::wstring& base64 )
		{
			std::string utf8 = convert_wcs_to_utf8( base64 );
			return convert_from_base64( utf8 );
		}

		std::string convert_to_base64( const std::string& buf )
		{
			std::string ret;
			const unsigned char* bytes_to_encode = reinterpret_cast< const unsigned char* >( buf.data() );
			int in_len = buf.size();
			int i = 0;
			int j = 0;
			unsigned char char_array_3[3];
			unsigned char char_array_4[4];

			while (in_len--) {
				char_array_3[i++] = *(bytes_to_encode++);
				if (i == 3) {
					char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
					char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
					char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
					char_array_4[3] = char_array_3[2] & 0x3f;

					for(i = 0; (i <4) ; i++)
						ret += inner::base64_chars[char_array_4[i]];
					i = 0;
				}
			}

			if (i)
			{
				for(j = i; j < 3; j++)
					char_array_3[j] = '\0';

				char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
				char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
				char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
				char_array_4[3] = char_array_3[2] & 0x3f;

				for (j = 0; (j < i + 1); j++)
					ret += inner::base64_chars[char_array_4[j]];

				while((i++ < 3))
					ret += '=';

			}
			return ret;
		}

		std::wstring convert_to_base64_wcs( const std::string& buf )
		{
			std::string ret = convert_to_base64( buf );
			return convert_utf8_to_wcs( ret );
		}

		static wchar_t wcs_to_lower(wchar_t c)
		{
			// use_facet doesn't work here well (doesn't convert all the local characters properly)
			return std::tolower(c, std::locale());
		}

		static wchar_t wcs_to_upper(wchar_t c)
		{
			// use_facet doesn't work here well (doesn't convert all the local characters properly)
			return std::toupper(c, std::locale());
		}

		void wcs_make_lower(std::wstring& wcs)
		{
			std::transform(wcs.begin(),wcs.end(),wcs.begin(),wcs_to_lower);
		}

		void wcs_make_upper(std::wstring& wcs)
		{
			std::transform(wcs.begin(),wcs.end(),wcs.begin(),wcs_to_upper);
		}

		void wcs_trim_left(std::wstring& wcs, const wchar_t* str /* = _T(" \t") */)
		{
			int pos = wcs.find_first_not_of(str);
			if (pos == wcs.npos)
				wcs.erase();
			else
				wcs.erase(0, pos);
		}

		void wcs_trim_right(std::wstring& wcs, const wchar_t* str /* = _T(" \t") */)
		{
			int pos = wcs.find_last_not_of(str);
			if (pos == wcs.npos)
				wcs.erase();
			else
				wcs.erase(++pos);
		}

		void wcs_trim(std::wstring& wcs, const wchar_t* str /* = _T(" \t") */)
		{
			wcs_trim_left(wcs, str);
			wcs_trim_right(wcs, str);
		}

		bool wcs_split( const std::wstring& wcs, const wchar_t* str, std::vector<std::wstring>& arr)
		{
			int start = 0, pos = wcs.find(str);
			for (; pos != wcs.npos; pos = wcs.find(str, start))
			{
				arr.insert(arr.end(), wcs.substr(start, pos - start));
				start = pos + wcslen(str);
			}
			arr.insert(arr.end(), wcs.substr(start));

			return start != 0;
		}

		std::wstring convert_to_hex_wcs( char ch )
		{
			std::wstring out;
			int h = 0, l = 0;
			out.resize( 3 , L'\0' );
			h = ch & 0xf0;
			h >>= 4;
			l = ch & 0x0f;
			out[0] = ( h >= 0x0 && h <= 0x9 ) ? ( h + 0x30 ) : ( h + 0x57 );
			out[1] = ( l >= 0x0 && l <= 0x9 ) ? ( l + 0x30 ) : ( l + 0x57 );
			wcs_make_upper( out );
			return out.c_str();
		}

		std::wstring convert_to_hex_wcs( const std::string& buf, hex_case e )
		{
			std::wstring out;
			unsigned int inlen = buf.size();
			const char* in = buf.data();
			int h = 0, l = 0;
			out.resize( inlen * 2 + 1 , L'\0' );
			unsigned int i = 0;
			for ( ; i < inlen; i++ )
			{
				h = in[i] & 0xf0;
				h >>= 4;
				l = in[i] & 0x0f;
				out[i * 2] = ( h >= 0x0 && h <= 0x9 ) ? ( h + 0x30 ) : ( h + 0x57 );
				out[i * 2 + 1] = ( l >= 0x0 && l <= 0x9 ) ? ( l + 0x30 ) : ( l + 0x57 );
			}
			out[i * 2] = L'\0';
			if ( e == HEX_CASE_UPPER )
				wcs_make_upper( out );
			return out.c_str();
		}

		std::string convert_from_hex_wcs( const std::wstring& hex )
		{
			const wchar_t* in = hex.data();
			unsigned int inlen = hex.size();
			int o = 0 , h = 0, l = 0 ;
			if ( inlen == 0 || ( inlen / 2 * 2 ) != inlen )
				return std::string();
			char *out = new char[ inlen / 2 ];
			for ( unsigned int i = 0 ; i < inlen; i += 2, o++ )
			{
				h = ( in[i] >= 0x30 && in[i] <= 0x39 ) ? ( in[i] - 0x30 ) : ( in[i] >= 0x41 && in[i] <= 0x46 ) ? ( in[i] - 0x36 ) : ( in[i] >= 0x61 && in[i] <= 0x66 ) ? ( in[i] - 0x61 + 0x0a ) : -1;
				l = ( in[i + 1] >= 0x30 && in[i + 1] <= 0x39 ) ? ( in[i + 1] - 0x30 ) : ( in[i + 1] >= 0x41 && in[i + 1] <= 0x46 ) ? ( in[i + 1] - 0x36 ) : ( in[i + 1] >= 0x61 && in[i + 1] <= 0x66 ) ? ( in[i + 1] - 0x61 + 0x0a ) : -1;
				if ( h < 0 || l < 0 )
					return std::string();
				out[o] = ( h << 4 ) + l;
			}
			std::string ret;
			ret.append( out, inlen / 2 );
			delete[] out;
			return ret;
		}

		std::wstring url_encode( const std::wstring& wcs )
		{
			std::wstring ret;
			for ( std::wstring::const_iterator iter = wcs.begin(); iter != wcs.end(); iter++ )
			{
				wchar_t ch = ( *iter );
				if ( ::iswalnum( ch ) )
					ret += ch;
				else if ( ::iswspace( ch ) )
					ret += L'+';
				else
				{
					std::string raw = convert_wchar_to_utf8( ch );
					for ( std::string::const_iterator rawiter = raw.begin(); rawiter != raw.end(); rawiter++ )
					{
						ret += L'%';
						ret += convert_to_hex_wcs( *rawiter );
					}
				}
			}
			return ret;
		}

		std::wstring url_decode( const std::wstring& wcs )
		{
			std::wstring ret;
			for ( std::wstring::const_iterator iter = wcs.begin(); iter != wcs.end(); iter++ )
			{
				wchar_t ch = ( *iter );
				if ( ch == L'+' )
				{
					ret += L' ';
					break;
				}
				if ( ch != L'%' )
					ret += ch;
				else
				{
					std::wstring single_hex;
					for ( ; *iter == L'%'; iter++ )
					{
						single_hex.assign( ++iter, ++iter );
						std::string raw = convert_from_hex_wcs( single_hex );
						ret += convert_utf8_to_wcs( raw );
					}
				}
			}
			return ret;
		}
	}
}
