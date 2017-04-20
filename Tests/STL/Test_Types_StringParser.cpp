// Copyright © 2014-2017  Zhirnov Andrey. All rights reserved.

#include "Engine/STL/Engine.STL.h"

using namespace GX_STL;
using namespace GX_STL::GXTypes;
using namespace GX_STL::GXMath;


static void StringParser_ToEndOfLine ()
{
	usize	pos = 7;
	StringParser::ToEndOfLine( "1111\n2222\n3333333", pos );
	ASSERT( pos == 9 );

	pos = 0;
	StringParser::ToEndOfLine( "1111", pos );
	ASSERT( pos == 4 );
	
	pos = 0;
	StringParser::ToEndOfLine( "1111\r\n2222", pos );
	ASSERT( pos == 4 );
	
	pos = 0;
	StringParser::ToEndOfLine( "1111\r\n", pos );
	ASSERT( pos == 4 );
}


static void StringParser_ToBeginOfLine ()
{
	usize	pos = 7;
	StringParser::ToBeginOfLine( "1111\n2222\n3333333", pos );
	ASSERT( pos == 5 );
	
	pos = 6;
	StringParser::ToBeginOfLine( "11\r\n222\r\n33", pos );
	ASSERT( pos == 4 );
}


static void StringParser_ToNextLine ()
{
	usize	pos = 7;
	StringParser::ToNextLine( "1111\n2222\n3333333", pos );
	ASSERT( pos == 10 );
	
	pos = 7;
	StringParser::ToNextLine( "1111\n2222\r\n\r\n333", pos );
	ASSERT( pos == 11 );
}


static void StringParser_ToPrevLine ()
{
	usize	pos = 7;
	StringParser::ToPrevLine( "1111\n2222\n3333333", pos );
	ASSERT( pos == 4 );
}


static void StringParser_Tokenize_1 ()
{
	Array< StringCRef >	tokens;
	StringParser::Tokenize( "11,22,33,44,55", ',', tokens );

	ASSERT( tokens.Count() == 5 );
	ASSERT( tokens[0] == "11" );
	ASSERT( tokens[1] == "22" );
	ASSERT( tokens[2] == "33" );
	ASSERT( tokens[3] == "44" );
	ASSERT( tokens[4] == "55" );
}


static void StringParser_Tokenize_2 ()
{
	Array< StringCRef >	tokens;
	StringParser::Tokenize( "1111,2,,4,", ',', tokens );

	ASSERT( tokens.Count() == 5 );
	ASSERT( tokens[0] == "1111" );
	ASSERT( tokens[1] == "2" );
	ASSERT( tokens[2] == "" );
	ASSERT( tokens[3] == "4" );
	ASSERT( tokens[4] == "" );
}


static void StringParser_CStyleDivideString ()
{
	Array< StringCRef >	tokens;
	StringParser::DivideString_CPP( "a=122; _bc+=5/ 7*34\",,\"--->", tokens );

	ASSERT( tokens.Count() == 17 );
	ASSERT( tokens[0] == "a" );
	ASSERT( tokens[1] == "=" );
	ASSERT( tokens[2] == "122" );
	ASSERT( tokens[3] == ";" );
	ASSERT( tokens[4] == "_bc" );
	ASSERT( tokens[5] == "+=" );
	ASSERT( tokens[6] == "5" );
	ASSERT( tokens[7] == "/" );
	ASSERT( tokens[8] == "7" );
	ASSERT( tokens[9] == "*" );
	ASSERT( tokens[10] == "34" );
	ASSERT( tokens[11] == "\"" );
	ASSERT( tokens[12] == "," );
	ASSERT( tokens[13] == "," );
	ASSERT( tokens[14] == "\"" );
	ASSERT( tokens[15] == "--" );
	ASSERT( tokens[16] == "->" );
}


static void StringParser_ReadLine ()
{
	StringCRef	str = "01234\r\n5678";
	StringCRef	line;
	usize			pos = 0;

	StringParser::ReadLineToEnd( str, pos, line );

	ASSERT( line == "01234" );
	ASSERT( pos == 7 );
}


static void StringParser_ReadString ()
{
	usize			pos = 0;
	StringCRef	result;
	StringParser::ReadString( "include \"123456\" ; ", pos, result );

	ASSERT( pos == 16 );
	ASSERT( result == "123456" );
}


static void StringParser_ParseCStyleString ()
{
	String	result;

	StringParser::ParseCStyleString( TOSTRING( test\n And as\t\t34 \x0D\x0A asd ), result );

	ASSERT( result == "test\n And as\t\t34 \x0D\x0A asd" );
}


static void StringParser_CalculateNumberOfLines ()
{
	usize	lines = StringParser::CalculateNumberOfLines( "1\n2\n3\r\n4\r\n5\n6\n7\r8\n9\n10" );
	ASSERT( lines == 10 );
}


static void StringParser_IncreaceIndent ()
{
	const String	src = "1111\n2222\n3333\n\t4444\n5555";
	const String	cmp = "\t1111\n\t2222\n\t3333\n\t\t4444\n\t5555";
	String			dst = src;

	StringParser::IncreaceIndent( dst, "\t" );
	ASSERT( dst == cmp );
}


static void StringParser_DecreaceIndent ()
{
	const String	cmp  = "1111\n2222\n3333\n\t4444\n5555";
	const String	cmp2 = "1111\n2222\n3333\n4444\n5555";
	const String	src  = "\t1111\n\t2222\n\t3333\n\t\t4444\n\t5555";
	String			dst  = src;

	StringParser::DecreaceIndent( dst, "\t" );
	ASSERT( dst == cmp );
	
	StringParser::DecreaceIndent( dst, "\t" );
	ASSERT( dst == cmp2 );
}


extern void Test_Types_StringParser ()
{
	StringParser_ToEndOfLine();
	StringParser_ToBeginOfLine();
	StringParser_ToNextLine();
	StringParser_ToPrevLine();
	StringParser_Tokenize_1();
	StringParser_Tokenize_2();
	StringParser_CStyleDivideString();
	StringParser_ReadLine();
	StringParser_ReadString();
	StringParser_ParseCStyleString();
	StringParser_CalculateNumberOfLines();
	StringParser_IncreaceIndent();
	StringParser_DecreaceIndent();
}
