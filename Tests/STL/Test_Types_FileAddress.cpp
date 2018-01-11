// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Tests/STL/Common.h"

using namespace GX_STL;
using namespace GX_STL::GXTypes;
using namespace GX_STL::GXMath;


extern void Test_Types_FileAddress ()
{
	const String filename = "../11/../22/33\\44\\55.ab";

	const String ext = FileAddress::GetExtension( filename );			TEST( ext == "ab" );
	const String name = FileAddress::GetName( filename );				TEST( name == "55" );
	const String path = FileAddress::GetPath( filename );				TEST( path == "../11/../22/33\\44" );
	const String name_ext = FileAddress::GetNameAndExt( filename );		TEST( name_ext == "55.ab" );

	Array<StringCRef> path_arr;
	FileAddress::DividePath( filename, path_arr );
	TEST( path_arr.Count() == 7 );
	TEST( path_arr[0] == ".." );
	TEST( path_arr[1] == "11" );
	TEST( path_arr[2] == ".." );
	TEST( path_arr[3] == "22" );
	TEST( path_arr[4] == "33" );
	TEST( path_arr[5] == "44" );
	TEST( path_arr[6] == "55.ab" );

	String	without_name = filename;
	FileAddress::RemoveName( without_name );
	TEST( without_name == "../11/../22/33\\44" );
	
	String	without_ext = filename;
	FileAddress::RemoveExtension( without_ext );
	TEST( without_ext == "../11/../22/33\\44\\55" );

	String formated = filename;
	FileAddress::FormatPath( formated );
	TEST( formated == "../22/33/44/55.ab" );
	
	formated = "aa/bb/../../../aa/bb/cc/dd";
	FileAddress::FormatPath( formated );
	TEST( formated == "../aa/bb/cc/dd" );

	String left = filename;
	FileAddress::RemoveDirectoriesFromLeft( left, 2 );
	TEST( left == "../11" );

	String right = filename;
	FileAddress::RemoveDirectoriesFromRight( right, 2 );
	TEST( right == "../11/../22/33" );

	String dir_path = "path";
	FileAddress::AddDirectoryToPath( dir_path, "dir" );
	TEST( dir_path == "path/dir" );
	
	dir_path = "/path/";
	FileAddress::AddDirectoryToPath( dir_path, "/dir1/" );
	FileAddress::AddDirectoryToPath( dir_path, "/dir2/" );
	TEST( dir_path == "/path/dir1/dir2/" );
	
	dir_path = "path";
	FileAddress::AddBaseDirectoryToPath( dir_path, "dir" );
	TEST( dir_path == "dir/path" );
	
	dir_path = "/path/";
	FileAddress::AddBaseDirectoryToPath( dir_path, "/dir1/" );
	FileAddress::AddBaseDirectoryToPath( dir_path, "/dir2/" );
	TEST( dir_path == "/dir2/dir1/path/" );

	String name1 = name;
	FileAddress::AddExtensionToName( name1, ".ext" );		TEST( name1 == name + ".ext" );
	
	name1 = name + ".";
	FileAddress::AddExtensionToName( name1, ".ext" );		TEST( name1 == name + ".ext" );
	
	name1 = name;
	FileAddress::AddExtensionToName( name1, "ext" );		TEST( name1 == name + ".ext" );
	
	name1 = name + ".";
	FileAddress::AddExtensionToName( name1, "ext" );		TEST( name1 == name + ".ext" );

	String	name2 = FileAddress::GetName( "../out/common" );	TEST( name2 == "common" );
	String	name3 = FileAddress::GetName( "../out/common" );	TEST( name3 == "common" );
}
