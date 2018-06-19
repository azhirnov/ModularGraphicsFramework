// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "PipelineBuilding.h"

extern CMakeBuilderPtr  GenCore (Ptr<CMakeBuilder> compilers)
{
#ifdef ENABLE_STL

	CMakeBuilderPtr		builder{ new CMakeBuilder{ "Core", "Core" } };
	
	builder->InheritCompilers( compilers );
	builder->SetVersion( 1, 0 );
	builder->SetSecondary();
	
	builder->Projects_IncludeDirectory( ".." )
			->Projects_IncludeDirectory( "../External" )
			->Projects_IncludeDirectory( "${EXTERNALS_PATH}" );

	const String	core_path	= "";
	const String	tests_path	= "../CoreTests/";
	

	// STL //
	auto	core_config = builder->AddLibrary( "Core.Config", core_path + "Config" );
	{
		core_config->AddFoldersRecursive( "" );
		core_config->ProjFolder( "Core" );
	}

	auto	core_stl = builder->AddLibrary( "Core.STL", "STL" );
	{
		core_stl->AddFoldersRecursive( "" );
		core_stl->ProjFolder( "Core" );
		core_stl->LinkLibrary( "SDL2", "ENABLE_SDL" )
				->LinkLibrary( "LZ4", "ENABLE_LZ4" )
				->LinkLibrary( "MiniZ", "ENABLE_MINIZ" )
				->LinkLibrary( "dl;pthread", "UNIX" );
	}
		
	auto	test_core_stl = builder->AddExecutable( "CoreTests.STL", tests_path + "STL" );
	{
		test_core_stl->AddFoldersRecursive( "" );
		test_core_stl->ProjFolder( "CoreTests" );
		test_core_stl->LinkLibrary( core_stl );
	}

	auto	core_physics = builder->AddLibrary( "Core.Physics", core_path + "Physics" );
	{
		core_physics->AddFoldersRecursive( "" );
		core_physics->ProjFolder( "Core" );
		core_physics->LinkLibrary( core_stl );
	}

	auto	test_core_physics = builder->AddExecutable( "CoreTests.Physics", tests_path + "Physics" );
	{
		test_core_physics->AddFoldersRecursive( "" );
		test_core_physics->ProjFolder( "CoreTests" );
		test_core_physics->LinkLibrary( core_physics );
	}
	//----------------------------------------------------------------------------


	// External //
#	ifdef ENABLE_EXTERNALS
	auto	core_script = builder->AddLibrary( "Core.Script", core_path + "Script" );
	{
		core_script->AddFoldersRecursive( "" );
		core_script->ProjFolder( "Core" );
		core_script->LinkLibrary( core_stl )->LinkLibrary( "AngelScript" );
	}

	auto	test_script = builder->AddExecutable( "CoreTests.Scipt", tests_path + "Script" );
	{
		test_script->AddFoldersRecursive( "" );
		test_script->ProjFolder( "CoreTests" );
		test_script->LinkLibrary( core_script );
	}
#	endif	// ENABLE_EXTERNALS
	//----------------------------------------------------------------------------
		
	
	// SCU //
#	ifdef ENABLE_SCU
	builder->AddExecutable( "Tests.Core.STL.SCU" )->ProjFolder("SCU")->LinkLibrary( test_core_stl )->MergeCPP( NUM_THREADS );
#	endif	// ENABLE_SCU
	//----------------------------------------------------------------------------
	

	builder->Save();
	return builder;

#else
	return null;

#endif	// ENABLE_STL
}
