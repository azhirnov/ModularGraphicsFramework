// Copyright © 2014-2017  Zhirnov Andrey. All rights reserved.

#include "StringParser.h"

namespace GX_STL
{
namespace GXTypes
{

	//
	// C-Style Parser
	//

	struct StringParser::_CStyleParser
	{
	private:
		enum EMode {
			NONE,
			WORD,
			NUMBER,
			OPERATOR,
		};

		uint	_size;
		char	_prev;
		char	_prevPrev;
		EMode	_mode;

	public:
		_CStyleParser ();
		bool IsBegin (char c);
		bool IsEnd (char c);
		bool IsUnused (char c);
		bool OnUnknown (char)		{ return false; }	// return true to continue parsing, false - to exit

	private:
		static bool _IsWordBegin (char c);
		static bool _IsWord (char c);
		static bool _IsNumberBegin (char c);
		static bool _IsNumber (char c);
		static bool _IsOperator (char c);
		static bool _IsDoubleOperator (char p, char c);
		static bool _IsTripleOperator (char pp, char p, char c);
	};


	//
	// Word Parser
	//

	struct StringParser::_WordParser
	{
	private:
		enum EMode {
			NONE,
			WORD,
			NUMBER,
		};

		EMode	_mode;

	public:
		_WordParser ();
		bool IsBegin (char c);
		bool IsEnd (char c);
		bool IsUnused (char)		{ return false; }
		bool OnUnknown (char)		{ return false; }	// return true to continue parsing, false - to exit
	};



//------------------------------ _CStyleParser ------------------------------//
/*
=================================================
	constructor
=================================================
*/
	inline StringParser::_CStyleParser::_CStyleParser () :
		_mode(NONE), _size(0), _prev(' ')
	{}

/*
=================================================
	IsBegin
=================================================
*/
	inline bool StringParser::_CStyleParser::IsBegin (char c)
	{
		_mode		= NONE;
		_size		= 0;
		_prevPrev	= _prev;
		_prev		= c;

		if ( _IsWordBegin( c ) )	{ _mode = WORD;		return true; }
		if ( _IsNumberBegin( c ) )	{ _mode = NUMBER;	return true; }
		if ( _IsOperator( c ) )		{ _mode = OPERATOR;	return true; }

		return false;
	}
	
/*
=================================================
	IsEnd
=================================================
*/
	inline bool StringParser::_CStyleParser::IsEnd (char c)
	{
		++_size;

		switch ( _mode )
		{
			case WORD :
			{
				return not _IsWord( c );
			}
			case NUMBER :
			{
				return not _IsNumber( c );
			}
			case OPERATOR :
			{
				if ( _size == 1 )
					return not _IsDoubleOperator( _prev, c );
				else
				if ( _size == 2 )
					return not _IsTripleOperator( _prevPrev, _prev, c );
				else
					return true;
			}
			case NONE :
				return true;
		}

		_prevPrev	= _prev;
		_prev		= c;

		return true;
	}

/*
=================================================
	IsUnused
=================================================
*/
	inline bool StringParser::_CStyleParser::IsUnused (char c)
	{
		return	( (c == ' ') | (c == '\t') | /*(c == '\\') |*/ (c == '\n') | (c == '\r') );
	}
	
/*
=================================================
	_IsWordBegin
=================================================
*/
	inline bool StringParser::_CStyleParser::_IsWordBegin (char c)
	{
		return	bool( (c >= 'A') & (c <= 'Z') ) |
				bool( (c >= 'a') & (c <= 'z') ) |
					  (c == '_');
	}
	
/*
=================================================
	_IsWord
=================================================
*/
	inline bool StringParser::_CStyleParser::_IsWord (char c)
	{
		return	_IsWordBegin( c ) | _IsNumberBegin( c );
	}
	
/*
=================================================
	_IsNumberBegin
=================================================
*/
	inline bool StringParser::_CStyleParser::_IsNumberBegin (char c)
	{
		return	( (c >= '0') & (c <= '9') );
	}
	
/*
=================================================
	_IsNumber
=================================================
*/
	inline bool StringParser::_CStyleParser::_IsNumber (char c)
	{
		return	bool( (c >= '0') & (c <= '9') ) |
				bool( (c >= 'A') & (c <= 'F') ) |
				bool( (c >= 'a') & (c <= 'f') ) |
					  (c == '.') | (c == 'x') |
					  (c == 'X');
	}

/*
=================================================
	_IsOperator
=================================================
*/
	inline bool StringParser::_CStyleParser::_IsOperator (char c)
	{
		// TODO: use Set
		return	(	(c == '-') | (c == '+') | (c == '*') | (c == '/') |
					(c == '|') | (c == '&') | (c == '^') | (c == '<') |
					(c == '>') | (c == '?') | (c == ':') | (c == ';') |
					(c == ',') | (c == '.') | (c == '!') | (c == '~') |
					(c == '[') | (c == ']') | (c == '(') | (c == ')') |
					(c == '=') | (c == '%') | (c == '"') | (c == '#') |
					(c == '{') | (c == '}') | (c == '\'') );
	}
	
/*
=================================================
	_IsDoubleOperator
=================================================
*/
	inline bool StringParser::_CStyleParser::_IsDoubleOperator (char p, char c)
	{
		// TODO: use Set
#		define PAIR_CMP( _pair_ )	(bool( (p == _pair_[0]) & (c == _pair_[1]) ))

		return	PAIR_CMP( "//" ) | PAIR_CMP( "/*" ) | PAIR_CMP( "*/" ) |
				PAIR_CMP( "&&" ) | PAIR_CMP( "||" ) | PAIR_CMP( "^^" ) |
				PAIR_CMP( "::" ) | PAIR_CMP( "==" ) | PAIR_CMP( "!=" ) |
				PAIR_CMP( ">=" ) | PAIR_CMP( "<=" ) | PAIR_CMP( "##" ) |
				PAIR_CMP( ">>" ) | PAIR_CMP( "<<" ) | PAIR_CMP( "|=" ) |
				PAIR_CMP( "&=" ) | PAIR_CMP( "^=" ) | PAIR_CMP( "+=" ) |
				PAIR_CMP( "-=" ) | PAIR_CMP( "*=" ) | PAIR_CMP( "/=" ) |
				PAIR_CMP( "%=" ) | PAIR_CMP( "--" ) | PAIR_CMP( "++" ) |
				PAIR_CMP( "->" );

#		undef PAIR_CMP
	}
	
/*
=================================================
	_IsTripleOperator
=================================================
*/
	inline bool StringParser::_CStyleParser::_IsTripleOperator (char pp, char p, char c)
	{
#		define TRIPLE_CMP( _triple_ )	(bool( (pp == _triple_[0]) & (p == _triple_[1]) & (c == _triple_[2]) ))

		return	TRIPLE_CMP( ">>=" ) | TRIPLE_CMP( "<<=" );

#		undef TRIPLE_CMP
	}



//------------------------------ _WordParser --------------------------------//
/*
=================================================
	constructor
=================================================
*/
	inline StringParser::_WordParser::_WordParser () : _mode(NONE)
	{
	}
	
/*
=================================================
	IsBegin
=================================================
*/
	inline bool StringParser::_WordParser::IsBegin (char c)
	{
		if ( (c >= 'a' and c <= 'z') or (c >= 'A' and c <= 'Z') )
			_mode = WORD;
		else
		if ( c >= '0' and c <= '9' )
			_mode = NUMBER;
		else
			_mode = NONE;

		return true;
	}
	
/*
=================================================
	IsEnd
=================================================
*/
	inline bool StringParser::_WordParser::IsEnd (char c)
	{
		switch ( _mode )
		{
			case WORD :
				return not ((c >= 'a' and c <= 'z') or (c >= 'A' and c <= 'Z') or (c >= '0' and c <= '9'));

			case NUMBER :
				return not (c >= '0' and c <= '9');
		}

		return true;
	}
	


//------------------------------ StringParser -------------------------------//
/*
=================================================
	DivideString_CPP
----
	used characters from C++, strings and comments parsed incorrectly
=================================================
*/
	bool StringParser::DivideString_CPP (StringCRef str, OUT Array< StringCRef > &tokens)
	{
		return DivideString( str, _CStyleParser(), tokens );
	}
	
/*
=================================================
	DivideString_Words
=================================================
*/
	bool StringParser::DivideString_Words (StringCRef str, OUT Array< StringCRef > &tokens)
	{
		return DivideString( str, _WordParser(), tokens );
	}

/*
=================================================
	ToEndOfLine
=================================================
*/
	void StringParser::ToEndOfLine (StringCRef str, INOUT usize &pos)
	{
		if ( str[pos] == '\n' or str[pos] == '\r' )
			return;

		while ( pos < str.Length() )
		{
			const char	n = (pos+1) >= str.Length() ? 0 : str[pos+1];
				
			++pos;

			if ( n == '\n' or n == '\r' )
				return;
		}
	}
	
/*
=================================================
	ToBeginOfLine
=================================================
*/
	void StringParser::ToBeginOfLine (StringCRef str, INOUT usize &pos)
	{
		while ( pos < str.Length() )
		{
			//const char	c = str[pos];
			const char	p = (pos-1) >= str.Length() ? '\0' : str[pos-1];
				
			if ( p == '\n' or p == '\r' or p == '\0' )
				return;

			--pos;
		}
		pos = 0;
	}
	
/*
=================================================
	IsBeginOfLine
=================================================
*/
	bool StringParser::IsBeginOfLine (StringCRef str, const usize pos)
	{
		usize	p = pos;
		ToBeginOfLine( str, INOUT p );
		return p == pos;
	}
	
/*
=================================================
	IsEndOfLine
=================================================
*/
	bool StringParser::IsEndOfLine (StringCRef str, const usize pos)
	{
		usize	p = pos;
		ToEndOfLine( str, INOUT p );
		return p == pos;
	}

/*
=================================================
	ToNextLine
=================================================
*/
	void StringParser::ToNextLine (StringCRef str, INOUT usize &pos)
	{
		while ( pos < str.Length() )
		{
			const char	c = str[pos];
			const char	n = (pos+1) >= str.Length() ? 0 : str[pos+1];
			
			++pos;

			// windows style "\r\n"
			if ( c == '\r' and n == '\n' )
			{
				++pos;
				return;
			}

			// linux style "\n" (or mac style "\r")
			if ( c == '\n' or c == '\r' )
				return;
		}
	}
	
/*
=================================================
	ToPrevLine
=================================================
*/
	void StringParser::ToPrevLine (StringCRef str, INOUT usize &pos)
	{
		while ( pos < str.Length() )
		{
			const char	c = str[pos];
			const char	p = (pos-1) >= str.Length() ? 0 : str[pos-1];
			
			--pos;

			// windows style "\r\n"
			if ( p == '\r' and c == '\n' )
			{
				--pos;
				return;
			}

			// linux style "\n" (or mac style "\r")
			if ( p == '\n' or p == '\r' )
				return;
		}
	}
	
/*
=================================================
	CalculateNumberOfLines
=================================================
*/
	usize StringParser::CalculateNumberOfLines (StringCRef str)
	{
		usize lines = 0;

		for (usize pos = 0; pos < str.Length(); ++lines)
		{
			ToNextLine( str, INOUT pos );
		}
		return lines;
	}
	
/*
=================================================
	MoveToLine
=================================================
*/
	bool StringParser::MoveToLine (StringCRef str, INOUT usize &pos, usize lineNumber)
	{
		usize lines = 0;

		for (; pos < str.Length() and lines < lineNumber; ++lines)
		{
			ToNextLine( str, INOUT pos );
		}
		return lines == lineNumber;
	}

/*
=================================================
	ReadCurrLine
---
	Read line from begin of line to end of line
	and move to next line.
=================================================
*/
	void StringParser::ReadCurrLine (StringCRef str, INOUT usize &pos, OUT StringCRef &result)
	{
		ToBeginOfLine( str, INOUT pos );

		ReadLineToEnd( str, INOUT pos, OUT result );
	}
	
/*
=================================================
	ReadLineToEnd
----
	Read line from current position to end of line
	and move to next line.
=================================================
*/
	void StringParser::ReadLineToEnd (StringCRef str, INOUT usize &pos, OUT StringCRef &result)
	{
		const usize	prev_pos = pos;

		ToEndOfLine( str, INOUT pos );

		result = str.SubString( prev_pos, pos - prev_pos );

		ToNextLine( str, INOUT pos );
	}

/*
=================================================
	ReadString
----
	read string from " to "
=================================================
*/
	bool StringParser::ReadString (StringCRef str, INOUT usize &pos, OUT StringCRef &result)
	{
		result = StringCRef();

		for (; pos < str.Length(); ++pos)
		{
			if ( str[pos] == '"' )
				break;
		}

		CHECK_ERR( str[pos] == '"' );

		const usize	begin = ++pos;

		for (; pos < str.Length(); ++pos)
		{
			const char	c = str[pos];

			if ( c == '"' )
			{
				result = StringCRef( str.cstr() + begin, pos - begin );
				++pos;
				return true;
			}
		}
	
		RETURN_ERR( "no pair for bracket \"" );
	}
	
/*
=================================================
	ReadTo
----
	read from 'pos' to symbol 'endSymbol'
=================================================
*/
	bool StringParser::ReadTo (StringCRef str, StringCRef endSymbol, INOUT usize &pos, OUT StringCRef &result)
	{
		result = StringCRef();

		usize	start = pos;

		if ( not str.Find( endSymbol, pos, start ) )
			RETURN_ERR( "end symbol not found" );

		result = str.SubString( start, pos );
		return true;
	}
	
/*
=================================================
	ReadToEndBracket
----
	read from 'openBracket' to 'closeBracket' for
	first 'openBracket'
=================================================
*
	bool StringParser::ReadToEndBracket (StringCRef str, StringCRef openBracket, StringCRef closeBracket,
												INOUT usize &pos, OUT StringCRef &result)
	{
		result = StringCRef();

		str.Find( openBracket, pos, pos );
		
		const usize	start = pos;

		usize	prev = pos + openBracket.Length();

		str.Find( closeBracket, pos, pos );


	}

/*
=================================================
	CharToNumber
=================================================
*/
	ubyte StringParser::CharToNumber (char c)
	{
		if ( c >= '0' and c <= '9' )	return c - '0';
		if ( c >= 'a' and c <= 'f' )	return c - 'a' + char(10);
		if ( c >= 'A' and c <= 'F' )	return c - 'A' + char(10);
		
		WARNING( "incorrect char" );
		return 0;
	}

/*
=================================================
	ReadStringCStyle
----
	read string from " to ", ignore \",
	convert \X and \xXX to char
=================================================
*/
	bool StringParser::ReadStringCStyle (StringCRef str, INOUT usize &pos, OUT String &result)
	{
		result.Clear();

		for (; pos < str.Length(); ++pos)
		{
			if ( str[pos] == '"' )
				break;
		}

		CHECK_ERR( str[pos] == '"' );
		
		const usize	begin = ++pos;

		for (; pos < str.Length(); ++pos)
		{
			const char	c = str[ pos ];
	
			if ( c == '"' )
			{
				ParseCStyleString( StringCRef( str.cstr() + begin, pos - begin ), OUT result );
				++pos;
				return true;
			}

			if ( c == '\\' )
			{
				const char	n = str[ ++pos ];

				if ( n == '"' )
					continue;
			}
		}

		RETURN_ERR( "no pair for bracket \"" );
	}
		
/*
=================================================
	ParseCStyleString
----
	convert \X and \xXX to char
=================================================
*/
	void StringParser::ParseCStyleString (StringCRef str, OUT String &result)
	{
		for (usize i = 0; i < str.Length(); ++i)
		{
			const char	c = str[i];
	
			if ( c == '\\' )
			{
				const char	n = str[ ++i ];

				switch ( n )
				{
					case 'N'  :	
					case 'n'  :		result << '\n';		break;
					case 'T'  :
					case 't'  :		result << '\t';		break;
					case 'R'  :
					case 'r'  :		result << '\r';		break;
					case 'X'  :
					case 'x'  :		result << char( ( CharToNumber( str[i+1] ) << 4 ) + CharToNumber( str[i+2] ) );  i += 2;  break;
					case '\\' :		result << '\\';		break;
					case '"'  :		result << '"';		break;
					case '\'' :		result << '\'';		break;
					default   :		result << "<unknown char after '\\': " << CharToNumber( n ) << ">";	WARNING( "unknown symbol" );
				}
			}
			else
				result << c;
		}
	}

/*
=================================================
	Tokenize
=================================================
*/
	void StringParser::Tokenize (StringCRef str, const char divisor, OUT Array< StringCRef > &tokens)
	{
		usize	begin = 0;

		tokens.Clear();

		for (usize i = 0; i < str.Length(); ++i)
		{
			const char	c = str[i];

			if ( c == divisor )
			{
				tokens.PushBack( StringCRef( str.cstr() + begin, i - begin ) );
				begin = i+1;
			}
		}

		tokens.PushBack( StringCRef( str.cstr() + begin, str.Length() - begin ) );
	}
	
/*
=================================================
	IncreaceIndent
=================================================
*/
	void StringParser::IncreaceIndent (INOUT String &str, StringCRef indent)
	{
		ASSERT( not indent.Empty() );

		usize	pos = 0;

		while ( pos < str.Length() )
		{
			str.Insert( indent, pos );

			pos += indent.Length();

			ToNextLine( str, pos );
		}
	}
	
/*
=================================================
	DecreaceIndent
=================================================
*/
	void StringParser::DecreaceIndent (INOUT String &str, StringCRef indent)
	{
		ASSERT( not indent.Empty() );

		usize	pos = 0;

		while ( pos < str.Length() )
		{
			if ( str.SubString( pos ).StartsWith( indent ) )
			{
				str.Erase( pos, indent.Length() );
				//pos -= indent.Length();
			}

			ToNextLine( str, pos );
		}
	}
	
/*
=================================================
	DivideLines
=================================================
*/
	void StringParser::DivideLines (StringCRef str, OUT Array< StringCRef > &lines)
	{
		lines.Clear();

		usize	pos = 0;
		usize	prev = 0;

		while ( pos < str.Length() )
		{
			ToEndOfLine( str, INOUT pos );

			if ( pos != prev ) {
				lines.PushBack( str.SubString( prev, pos-prev ) );
			}

			ToNextLine( str, INOUT pos );

			prev = pos;
		}
	}


}	// GXTypes
}	// GX_STL
