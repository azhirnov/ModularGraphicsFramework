// Copyright © 2014-2017  Zhirnov Andrey. All rights reserved.

#include "Engine/STL/Engine.STL.h"

using namespace GX_STL;
using namespace GX_STL::GXTypes;
using namespace GX_STL::GXMath;

/*
void ExtString_Test0()
{
	ext_string	s;

	s.Parse( "#[align:top]fdgdfg#[spacing:1.5]12345#[color:1F2E3D4C]6789" );
	
	ext_string::TReader	reader = s.GetReader();
	
	reader.Move( 3 );
	s.Erase( reader );

	reader.MoveToEnd();
	s.Insert( reader, (StringCRef) "aaaa" );
	
	//reader.MoveToEnd();

	s.InsertColor( reader, 0xFF1F2D3D );
	s.InsertOffsetX( reader, 4.6f );

	String str;
	s.UnParse( str );

	WARNING( str.cstr() );
}


void ExtString_Test1()
{
	ext_string	s, a;

	s.Parse( "#[spacing:1.4]22#[spacing:1.4]11#[spacing:1.4]1#[spacing:1.4]" );
	
	ext_string::TReader	reader = s.GetReader();

	if ( reader.Find( ext_string::SPACING ) )
		a.AppendChunk( reader );
	
	if ( reader.Find( ext_string::STRING ) )
		a.AppendChunk( reader );

	if ( reader.Find( ext_string::STRING ) )
		a.AppendChunk( reader );

	String str;
	a.UnParse( str );

	WARNING( str.cstr() );
}


void ExtString_Test2()
{
	ext_string	s;

	s.Parse( "#[spacing:1.4]#[spacing:1.1]11112222#[spacing:1.3]" );
	
	ext_string::TReader	reader = s.GetReader();

	reader.Move( 2 );

	s.DivideString( reader, 4 );
	
	//reader.MoveFromStart( 3 );
	s.InsertAlign( reader, ext_string::EAlign::CENTER );

	String str;
	s.UnParse( str );

	WARNING( str.cstr() );
}

*/
extern void Test_Types_ExtString ()
{
	//ExtString_Test0();
	//ExtString_Test1();
	//ExtString_Test2();
}
