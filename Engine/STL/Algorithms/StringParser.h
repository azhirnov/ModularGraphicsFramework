// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/STL/Containers/String.h"
#include "Engine/STL/Containers/Array.h"
#include "Engine/STL/Containers/StringRef.h"

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

		static bool IsBeginOfLine	(StringCRef str, usize pos);
		static bool IsEndOfLine		(StringCRef str, usize pos);

		static usize CalculateNumberOfLines	(StringCRef str);

		static void  ReadCurrLine (StringCRef str, INOUT usize &pos, OUT StringCRef &result);
		static void  ReadLineToEnd (StringCRef str, INOUT usize &pos, OUT StringCRef &result);

		static bool  MoveToLine (StringCRef str, INOUT usize &pos, usize lineNumber);


		// Indent //
		static void IncreaceIndent	(INOUT String &str, StringCRef indent = "\t");
		static void DecreaceIndent	(INOUT String &str, StringCRef indent = "\t");


		// utils //
		static ubyte CharToNumber (char c);


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

		//static bool ReadToEndBracket (StringCRef str, StringCRef openBracket, StringCRef closeBracket, INOUT usize &pos, OUT StringCRef &result);

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
