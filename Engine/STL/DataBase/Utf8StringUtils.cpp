// Copyright © 2014-2017  Zhirnov Andrey. All rights reserved.

#include "Utf8StringUtils.h"

#ifdef GX_SQLITE_DATABASE_ENABLED

namespace GX_STL
{
namespace GXDataBase
{
	
	int convert_utf8_to_windows1251 (const char* utf8, String &windows1251, size_t n);

/*
=================================================
	EncodeToWindows1251
=================================================
*/
	void Utf8StringUtils::EncodeToWindows1251 (StringCRef src, OUT String &dst)
	{
		const usize	len = Length( src.cstr() );

		dst.Clear();
		dst.Reserve( len );

		convert_utf8_to_windows1251( src.cstr(), dst, 0x7FFFFFFF );
		dst.CalculateLength();
	}

	
/*
=================================================
	convert_utf8_to_windows1251
----
	from http://code.google.com/p/convert-utf8-to-cp1251/
=================================================
*/
#pragma warning (push)
#pragma warning (disable: 4309)		// 'conversion' : truncation of constant value

	typedef struct ConvLetter {
		unsigned char	win1251;
		int				unicode;
	} Letter;

	static Letter g_letters[] = {
		{0x82, 0x201A}, // SINGLE LOW-9 QUOTATION MARK
		{0x83, 0x0453}, // CYRILLIC SMALL LETTER GJE
		{0x84, 0x201E}, // DOUBLE LOW-9 QUOTATION MARK
		{0x85, 0x2026}, // HORIZONTAL ELLIPSIS
		{0x86, 0x2020}, // DAGGER
		{0x87, 0x2021}, // DOUBLE DAGGER
		{0x88, 0x20AC}, // EURO SIGN
		{0x89, 0x2030}, // PER MILLE SIGN
		{0x8A, 0x0409}, // CYRILLIC CAPITAL LETTER LJE
		{0x8B, 0x2039}, // SINGLE LEFT-POINTING ANGLE QUOTATION MARK
		{0x8C, 0x040A}, // CYRILLIC CAPITAL LETTER NJE
		{0x8D, 0x040C}, // CYRILLIC CAPITAL LETTER KJE
		{0x8E, 0x040B}, // CYRILLIC CAPITAL LETTER TSHE
		{0x8F, 0x040F}, // CYRILLIC CAPITAL LETTER DZHE
		{0x90, 0x0452}, // CYRILLIC SMALL LETTER DJE
		{0x91, 0x2018}, // LEFT SINGLE QUOTATION MARK
		{0x92, 0x2019}, // RIGHT SINGLE QUOTATION MARK
		{0x93, 0x201C}, // LEFT DOUBLE QUOTATION MARK
		{0x94, 0x201D}, // RIGHT DOUBLE QUOTATION MARK
		{0x95, 0x2022}, // BULLET
		{0x96, 0x2013}, // EN DASH
		{0x97, 0x2014}, // EM DASH
		{0x99, 0x2122}, // TRADE MARK SIGN
		{0x9A, 0x0459}, // CYRILLIC SMALL LETTER LJE
		{0x9B, 0x203A}, // SINGLE RIGHT-POINTING ANGLE QUOTATION MARK
		{0x9C, 0x045A}, // CYRILLIC SMALL LETTER NJE
		{0x9D, 0x045C}, // CYRILLIC SMALL LETTER KJE
		{0x9E, 0x045B}, // CYRILLIC SMALL LETTER TSHE
		{0x9F, 0x045F}, // CYRILLIC SMALL LETTER DZHE
		{0xA0, 0x00A0}, // NO-BREAK SPACE
		{0xA1, 0x040E}, // CYRILLIC CAPITAL LETTER SHORT U
		{0xA2, 0x045E}, // CYRILLIC SMALL LETTER SHORT U
		{0xA3, 0x0408}, // CYRILLIC CAPITAL LETTER JE
		{0xA4, 0x00A4}, // CURRENCY SIGN
		{0xA5, 0x0490}, // CYRILLIC CAPITAL LETTER GHE WITH UPTURN
		{0xA6, 0x00A6}, // BROKEN BAR
		{0xA7, 0x00A7}, // SECTION SIGN
		{0xA8, 0x0401}, // CYRILLIC CAPITAL LETTER IO
		{0xA9, 0x00A9}, // COPYRIGHT SIGN
		{0xAA, 0x0404}, // CYRILLIC CAPITAL LETTER UKRAINIAN IE
		{0xAB, 0x00AB}, // LEFT-POINTING DOUBLE ANGLE QUOTATION MARK
		{0xAC, 0x00AC}, // NOT SIGN
		{0xAD, 0x00AD}, // SOFT HYPHEN
		{0xAE, 0x00AE}, // REGISTERED SIGN
		{0xAF, 0x0407}, // CYRILLIC CAPITAL LETTER YI
		{0xB0, 0x00B0}, // DEGREE SIGN
		{0xB1, 0x00B1}, // PLUS-MINUS SIGN
		{0xB2, 0x0406}, // CYRILLIC CAPITAL LETTER BYELORUSSIAN-UKRAINIAN I
		{0xB3, 0x0456}, // CYRILLIC SMALL LETTER BYELORUSSIAN-UKRAINIAN I
		{0xB4, 0x0491}, // CYRILLIC SMALL LETTER GHE WITH UPTURN
		{0xB5, 0x00B5}, // MICRO SIGN
		{0xB6, 0x00B6}, // PILCROW SIGN
		{0xB7, 0x00B7}, // MIDDLE DOT
		{0xB8, 0x0451}, // CYRILLIC SMALL LETTER IO
		{0xB9, 0x2116}, // NUMERO SIGN
		{0xBA, 0x0454}, // CYRILLIC SMALL LETTER UKRAINIAN IE
		{0xBB, 0x00BB}, // RIGHT-POINTING DOUBLE ANGLE QUOTATION MARK
		{0xBC, 0x0458}, // CYRILLIC SMALL LETTER JE
		{0xBD, 0x0405}, // CYRILLIC CAPITAL LETTER DZE
		{0xBE, 0x0455}, // CYRILLIC SMALL LETTER DZE
		{0xBF, 0x0457} // CYRILLIC SMALL LETTER YI
	};

#pragma warning (pop)


	int convert_utf8_to_windows1251 (const char* utf8, String &windows1251, size_t n)
	{
		int i = 0;
		int j = 0;
		for(; i < (int)n && utf8[i] != 0; ++i) {
			char prefix = utf8[i];
			char suffix = utf8[i+1];
			if ((prefix & 0x80) == 0) {
				windows1251 << (char)prefix;
				++j;
			} else if ((~prefix) & 0x20) {
				int first5bit = prefix & 0x1F;
				first5bit <<= 6;
				int sec6bit = suffix & 0x3F;
				int unicode_char = first5bit + sec6bit;

				if ( unicode_char >= 0x410 && unicode_char <= 0x44F ) {
					windows1251 << (char)(unicode_char - 0x350);
				} else if (unicode_char >= 0x80 && unicode_char <= 0xFF) {
					windows1251 << (char)(unicode_char);
				} else if (unicode_char >= 0x402 && unicode_char <= 0x403) {
					windows1251 << (char)(unicode_char - 0x382);
				} else {
					int count = sizeof(g_letters) / sizeof(Letter);
					for (int k = 0; k < count; ++k) {
						if (unicode_char == g_letters[k].unicode) {
							windows1251 << g_letters[k].win1251;
							goto NEXT_LETTER;
						}
					}
					// can't convert this char
					return 0;
				}
	NEXT_LETTER:
				++i;
				++j;
			} else {
				// can't convert this chars
				return 0;
			}
		}
		return 1;
	}

	
//-------------------------------------------------------------------

}	// GXDataBase
}	// GX_STL

#endif	// GX_SQLITE_DATABASE_ENABLED