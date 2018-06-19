// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "PipelineBuilding.h"

CMakeBuilderPtr  GenUtils (Ptr<CMakeBuilder> core)
{
#ifdef ENABLE_UTILS

	CMakeBuilderPtr		builder{ new CMakeBuilder{ "Utils", "Utils" } };
	
	builder->InheritCompilers( core );
	builder->InheritIncludeDirectories( core );
	builder->SetSecondary();
	
	auto	core_stl = core->GetProject( "Core.STL" );


	auto	util_cmake = builder->AddExecutable( "Utils.CMake", "CMake" );
	{
		util_cmake->EnableIf( "NOT DEFINED ANDROID" );
		util_cmake->AddFoldersRecursive( "" );
		util_cmake->ProjFolder( "Utils" );
		util_cmake->LinkLibrary( core_stl );
	}
	//----------------------------------------------------------------------------

	
	builder->Save();
	return builder;

#else
	return null;

#endif	// ENABLE_UTILS
}
