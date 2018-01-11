// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Tests/STL/Common.h"

using namespace GX_STL;
using namespace GX_STL::GXTypes;
using namespace GX_STL::GXMath;


static void StringParser_ToEndOfLine ()
{
	usize	pos = 7;
	StringParser::ToEndOfLine( "1111\n2222\n3333333", pos );
	TEST( pos == 9 );

	pos = 0;
	StringParser::ToEndOfLine( "1111", pos );
	TEST( pos == 4 );
	
	pos = 0;
	StringParser::ToEndOfLine( "1111\r\n2222", pos );
	TEST( pos == 4 );
	
	pos = 0;
	StringParser::ToEndOfLine( "1111\r\n", pos );
	TEST( pos == 4 );
}


static void StringParser_ToBeginOfLine ()
{
	usize	pos = 7;
	StringParser::ToBeginOfLine( "1111\n2222\n3333333", pos );
	TEST( pos == 5 );
	
	pos = 6;
	StringParser::ToBeginOfLine( "11\r\n222\r\n33", pos );
	TEST( pos == 4 );
}


static void StringParser_ToNextLine ()
{
	usize	pos = 7;
	StringParser::ToNextLine( "1111\n2222\n3333333", pos );
	TEST( pos == 10 );
	
	pos = 7;
	StringParser::ToNextLine( "1111\n2222\r\n\r\n333", pos );
	TEST( pos == 11 );
}


static void StringParser_ToPrevLine ()
{
	usize	pos = 7;
	StringParser::ToPrevLine( "1111\n2222\n3333333", pos );
	TEST( pos == 4 );
}


static void StringParser_Tokenize_1 ()
{
	Array< StringCRef >	tokens;
	StringParser::Tokenize( "11,22,33,44,55", ',', tokens );

	TEST( tokens.Count() == 5 );
	TEST( tokens[0] == "11" );
	TEST( tokens[1] == "22" );
	TEST( tokens[2] == "33" );
	TEST( tokens[3] == "44" );
	TEST( tokens[4] == "55" );
}


static void StringParser_Tokenize_2 ()
{
	Array< StringCRef >	tokens;
	StringParser::Tokenize( "1111,2,,4,", ',', tokens );

	TEST( tokens.Count() == 5 );
	TEST( tokens[0] == "1111" );
	TEST( tokens[1] == "2" );
	TEST( tokens[2] == "" );
	TEST( tokens[3] == "4" );
	TEST( tokens[4] == "" );
}


static void StringParser_CStyleDivideString ()
{
	Array< StringCRef >	tokens;
	StringParser::DivideString_CPP( "a=122; _bc+=5/ 7*34\",,\"--->", tokens );

	TEST( tokens.Count() == 17 );
	TEST( tokens[0] == "a" );
	TEST( tokens[1] == "=" );
	TEST( tokens[2] == "122" );
	TEST( tokens[3] == ";" );
	TEST( tokens[4] == "_bc" );
	TEST( tokens[5] == "+=" );
	TEST( tokens[6] == "5" );
	TEST( tokens[7] == "/" );
	TEST( tokens[8] == "7" );
	TEST( tokens[9] == "*" );
	TEST( tokens[10] == "34" );
	TEST( tokens[11] == "\"" );
	TEST( tokens[12] == "," );
	TEST( tokens[13] == "," );
	TEST( tokens[14] == "\"" );
	TEST( tokens[15] == "--" );
	TEST( tokens[16] == "->" );
}


static void StringParser_ReadLine ()
{
	StringCRef	str = "01234\r\n5678";
	StringCRef	line;
	usize			pos = 0;

	StringParser::ReadLineToEnd( str, pos, line );

	TEST( line == "01234" );
	TEST( pos == 7 );
}


static void StringParser_ReadString ()
{
	usize			pos = 0;
	StringCRef	result;
	StringParser::ReadString( "include \"123456\" ; ", pos, result );

	TEST( pos == 16 );
	TEST( result == "123456" );
}


static void StringParser_ParseCStyleString ()
{
	String	result;

	StringParser::ParseCStyleString( TOSTRING( test\n And as\t\t34 \x0D\x0A asd ), result );

	TEST( result == "test\n And as\t\t34 \x0D\x0A asd" );
}


static void StringParser_CalculateNumberOfLines ()
{
	usize	lines = StringParser::CalculateNumberOfLines( "1\n2\n3\r\n4\r\n5\n6\n7\r8\n9\n10" );
	TEST( lines == 10 );

	lines = StringParser::CalculateNumberOfLines( "1" );
	TEST( lines == 1 );

	lines = StringParser::CalculateNumberOfLines( "1\n2\n" );
	TEST( lines == 2 );
}


static void StringParser_IncreaceIndent ()
{
	const String	src = "1111\n2222\n3333\n\t4444\n5555";
	const String	cmp = "\t1111\n\t2222\n\t3333\n\t\t4444\n\t5555";
	String			dst = src;

	StringParser::IncreaceIndent( dst, "\t" );
	TEST( dst == cmp );
}


static void StringParser_DecreaceIndent ()
{
	const String	cmp  = "1111\n2222\n3333\n\t4444\n5555";
	const String	cmp2 = "1111\n2222\n3333\n4444\n5555";
	const String	src  = "\t1111\n\t2222\n\t3333\n\t\t4444\n\t5555";
	String			dst  = src;

	StringParser::DecreaceIndent( dst, "\t" );
	TEST( dst == cmp );
	
	StringParser::DecreaceIndent( dst, "\t" );
	TEST( dst == cmp2 );
}


static void StringParser_MoveToLine ()
{
	usize pos = 0;
	StringParser::MoveToLine( "1\n2\n3\r\n4\r\n5\n6\n7\r8\n9\n10", OUT pos, 0 );
	TEST( pos == 0 );
	
	pos = 0;
	StringParser::MoveToLine( "1\n2\n3\r\n4\r\n5\n6\n7\r8\n9\n10", OUT pos, 1 );
	TEST( pos == 2 );
	
	pos = 0;
	StringParser::MoveToLine( "1\n2\n3\r\n4\r\n5\n6\n7\r8\n9\n10", OUT pos, 9 );
	TEST( pos == 20 );
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
	StringParser_MoveToLine();
}
