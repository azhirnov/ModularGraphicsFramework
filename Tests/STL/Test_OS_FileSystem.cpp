// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Tests/STL/Common.h"

using namespace GX_STL;
using namespace GX_STL::GXTypes;


static void TestGetAllFiles ()
{
	Array<String>	dirs;

	TEST( OS::FileSystem::GetAllDirsInPath( "..", dirs ) );

	TEST( not dirs.Empty() );
}


static void TestFolderSearch ()
{
	TEST( OS::FileSystem::FindAndSetCurrentDir( "Tests/Engine.Base", 4 ) );

	TEST( OS::FileSystem::IsFileExist( "Main.cpp" ) );
}


static void TestRecursiveFileSearch ()
{
	TEST( OS::FileSystem::FindAndSetCurrentDir( "Tests/STL", 4 ) );

	Array<String>	file_names;
	auto			filter = LAMBDA()(StringCRef fname) -> bool
	{{
		StringCRef ext	= FileAddress::GetExtension( fname );
		StringCRef name	= FileAddress::GetName( fname );

		return ext.EqualsIC( "cpp" ) and name.StartsWithIC( "Test_" );
	}};

	OS::FileSystem::RecursiveFindFiles( "", filter, 1, OUT file_names );

	usize	idx;
	TEST( file_names.Find( OUT idx, String("Test_OS_FileSystem.cpp"), 0 ) );
}


extern void Test_OS_FileSystem ()
{
	TestGetAllFiles();
	TestFolderSearch();
	TestRecursiveFileSearch();
}
