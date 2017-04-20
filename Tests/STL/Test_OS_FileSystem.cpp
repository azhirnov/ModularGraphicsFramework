// Copyright © 2014-2017  Zhirnov Andrey. All rights reserved.

#include "Engine/STL/Engine.STL.h"

using namespace GX_STL;
using namespace GX_STL::GXTypes;


static void TestGetAllFiles ()
{
	Array<String>	dirs;

	ASSERT( OS::FileSystem::GetAllDirsInPath( "..", dirs ) );

	ASSERT( not dirs.Empty() );
}


static void TestFolderSearch ()
{
	ASSERT( OS::FileSystem::FindAndSetCurrentDir( "Tests/Engine/Compute", 5 ) );

	ASSERT( OS::FileSystem::IsFileExist( "Main.cpp" ) );
}


static void TestRecursiveFileSearch ()
{
	ASSERT( OS::FileSystem::FindAndSetCurrentDir( "Tests/STL", 5 ) );

	Array<String>	file_names;
	auto			filter = LAMBDA()(StringCRef fname) -> bool
	{{
		StringCRef ext	= FileAddress::GetExtension( fname );
		StringCRef name	= FileAddress::GetName( fname );

		return ext.EqualsIC( "cpp" ) and name.StartsWithIC( "Test_" );
	}};

	OS::FileSystem::RecursiveFindFiles( "", filter, 1, OUT file_names );

	usize	idx;
	ASSERT( file_names.Find( OUT idx, String("Test_OS_FileSystem.cpp"), 0 ) );
}


extern void Test_OS_FileSystem ()
{
	TestGetAllFiles();
	TestFolderSearch();
	TestRecursiveFileSearch();
}
