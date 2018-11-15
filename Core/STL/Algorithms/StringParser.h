// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Core/STL/Containers/String.h"
#include "Core/STL/Containers/Array.h"
#include "Core/STL/Containers/StringRef.h"

namespace GX_STL
{
namespace GXTypes
{

	//
	// String Parser
	//

	struct StringParser final : Noninstancable
	{
	public:
		// move cursor //
			static void ToEndOfLine		(StringCRef str, INOUT usize &pos);
			static void ToBeginOfLine	(StringCRef str, INOUT usize &pos);
			static void ToNextLine		(StringCRef str, INOUT usize &pos);
			static void ToPrevLine		(StringCRef str, INOUT usize &pos);

		ND_ static bool IsBeginOfLine	(StringCRef str, usize pos);
		ND_ static bool IsEndOfLine		(StringCRef str, usize pos);

		ND_ static usize CalculateNumberOfLines (StringCRef str);

			static void  ReadCurrLine (StringCRef str, INOUT usize &pos, OUT StringCRef &result);
			static void  ReadLineToEnd (StringCRef str, INOUT usize &pos, OUT StringCRef &result);

			static bool  MoveToLine (StringCRef str, INOUT usize &pos, usize lineNumber);


		// Indent //
			static void IncreaceIndent (INOUT String &str, StringCRef indent = "\t");
			static void DecreaceIndent (INOUT String &str, StringCRef indent = "\t");

		ND_ static String  IncreaceIndent (String &&str, StringCRef indent = "\t");
		ND_ static String  DecreaceIndent (String &&str, StringCRef indent = "\t");


		// utils //
		ND_ static ubyte CharToNumber (char c);


		// parser //
			static void Tokenize (StringCRef str, char divisor, OUT Array< StringCRef > &tokens);

			template <typename TParser>
			static bool DivideString (StringCRef str, TParser parser, OUT Array< StringCRef > &tokens);

			static bool DivideString_CPP (StringCRef str, OUT Array< StringCRef > &tokens);
			static bool DivideString_Words (StringCRef str, OUT Array< StringCRef > &tokens);

			static void DivideLines (StringCRef str, OUT Array< StringCRef > &lines);

		// "..."
			static bool ReadString (StringCRef str, INOUT usize &pos, OUT StringCRef &result);
			static bool ReadStringCStyle (StringCRef str, INOUT usize &pos, OUT String &result);

			static void ParseCStyleString (StringCRef str, OUT String &result);

			static bool ReadTo (StringCRef str, StringCRef endSymbol, INOUT usize &pos, OUT StringCRef &result);

		ND_ static float CompareBySymbols (StringCRef left, StringCRef right);

		ND_ static float CompareByWords (StringCRef left, StringCRef right);

		ND_ static bool CompareLineByLine (StringCRef left, StringCRef right,
										   OUT StringCRef &leftLine, OUT StringCRef &rightLine,
										   OUT usize &leftLineNumber, OUT usize &rightLineNumber);


	// types
	private:
		struct _CStyleParser;
		struct _WordParser;
	};
	
	

/*
=================================================
	DivideString
=================================================
*/
	template <typename TParser>
	inline bool StringParser::DivideString (StringCRef str, TParser parser, OUT Array< StringCRef > &tokens)
	{
		bool	word = false;
		usize	begin = 0;

		tokens.Clear();

		for (usize i = 0; i < str.Length(); ++i)
		{
			const char	c = str[i];

			if ( word )
			{
				if ( parser.IsEnd( c ) )
				{
					word = false;
					tokens.PushBack( StringCRef( str.ptr() + begin, i - begin ) );
					--i;
				}
				continue;
			}

			if ( parser.IsUnused( c ) )
				continue;

			if ( parser.IsBegin( c ) )
			{
				word = true;
				begin = i;
				continue;
			}
		
			if ( not parser.OnUnknown( c ) ) {
				RETURN_ERR( "invalid char '" << c << "'" );
			}
		}	

		if ( word )
			tokens.PushBack( StringCRef( str.ptr() + begin, str.Length() - begin ) );

		return true;
	}


}	// GXTypes
}	// GX_STL
