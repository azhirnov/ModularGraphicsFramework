// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "PipelineBuilding.h"
#include "Core/Config/Engine.Version.h"

CMakeBuilderPtr  GenEngine (Ptr<CMakeBuilder> core)
{
#ifdef ENABLE_ENGINE

	CMakeBuilderPtr		builder{ new CMakeBuilder{ "Engine", "Engine" } };

	builder->SetVersion( Engine::_ENGINE_VERSION_MAJ, Engine::_ENGINE_VERSION_MIN );
	builder->InheritCompilers( core );
	builder->InheritIncludeDirectories( core );
	builder->SetSecondary();

	// setup graphics api version or disable it (pass 0 to version)
	builder->AddOption( "OPENGL_VERSION", "Define OpenGL version, supported: 440, 450", 450 );
	builder->AddOption( "OPENGLES_VERSION", "Define OpenGLES version, supported: 200, 300, 310, 320", 320 );
	builder->AddOption( "OPENCL_VERSION", "Define OpenCL version, supported: 110, 120, 200", 120 );
	builder->AddOption( "VULKAN_VERSION", "Define Vulkan version, supported: 100, 110", 100 );
	builder->AddOption( "SOFTRENDER_VERSION", "Define Software renderer version, 100 is supported", 100 );
	
	// STL //
	auto	core_stl		= core->GetProject( "Core.STL" );
	auto	core_script		= core->GetProject( "Core.Script" );
	//----------------------------------------------------------------------------


	const String	engine_path = "";
	const String	tests_path	= "../EngineTests/";

	// Engine //
	auto	engine_base = builder->AddLibrary( "Engine.Base", "Base" );
	{
		engine_base->AddFoldersRecursive( "" );
		engine_base->ProjFolder( "Engine" );
		engine_base->LinkLibrary( core_stl );
	}

	auto	engine_platforms = builder->AddLibrary( "Engine.Platforms", "Platforms" );
	{
		engine_platforms->AddFoldersRecursive( "" );
		engine_platforms->LinkLibrary( engine_base );
		engine_platforms->ProjFolder( "Engine" );
		// OpenGL
		engine_platforms->LinkLibrary( "opengl32.lib", "WIN32" );
		// DirectX
		engine_platforms->LinkLibrary({ "dxguid.lib"_str, "dxgi.lib"_str }, "WIN32");
		// VR
		engine_platforms->LinkLibrary( "dxgi.lib", "WIN32" );
		// Windows Platform
		engine_platforms->LinkLibrary( "Shcore.lib", "(MSVC AND WIN32)" )->LinkLibrary( "Dxva2.lib", "WIN32" );
			
		engine_platforms->AddDefinition( "GRAPHICS_API_OPENGL=${OPENGL_VERSION}", "DEFINED OPENGL_VERSION AND NOT (OPENGL_VERSION EQUAL \"0\")" );
		engine_platforms->AddDefinition( "GRAPHICS_API_OPENGLES=${OPENGLES_VERSION}", "DEFINED OPENGLES_VERSION AND NOT (OPENGLES_VERSION EQUAL \"0\")" );
		engine_platforms->AddDefinition( "COMPUTE_API_OPENCL=${OPENCL_VERSION}", "DEFINED OPENCL_VERSION AND NOT (OPENCL_VERSION EQUAL \"0\")" );
		engine_platforms->AddDefinition( "GRAPHICS_API_VULKAN=${VULKAN_VERSION}", "DEFINED VULKAN_VERSION AND NOT (VULKAN_VERSION EQUAL \"0\")" );
		engine_platforms->AddDefinition( "GRAPHICS_API_SOFT=${SOFTRENDER_VERSION}", "DEFINED SOFTRENDER_VERSION AND NOT (SOFTRENDER_VERSION EQUAL \"0\")" );
	}
		
	auto	engine_graphics = builder->AddLibrary( "Engine.Graphics", "Graphics" );
	{
		engine_graphics->AddFoldersRecursive( "" );
		engine_graphics->ProjFolder( "Engine" );
		engine_graphics->LinkLibrary( engine_platforms );
	}

	auto	engine_profilers = builder->AddLibrary( "Engine.Profilers", "Profilers" );
	{
		engine_profilers->AddFoldersRecursive( "" );
		engine_profilers->ProjFolder( "Engine" );
		engine_profilers->LinkLibrary( engine_platforms );
	}

	auto	engine_importexport = builder->AddLibrary( "Engine.ImportExport", "ImportExport" );
	{
		engine_importexport->AddFoldersRecursive( "" );
		engine_importexport->ProjFolder( "Engine" );
		engine_importexport->LinkLibrary( engine_graphics );
	}
		
	auto	engine_scene = builder->AddLibrary( "Engine.Scene", "Scene" );
	{
		engine_scene->AddFoldersRecursive( "" );
		engine_scene->ProjFolder( "Engine" );
		engine_scene->LinkLibrary( engine_graphics );
	}
	//----------------------------------------------------------------------------


	// External //
#	ifdef ENABLE_EXTERNALS
	auto	engine_pipeline_compiler = builder->AddLibrary( "Engine.PipelineCompiler", "PipelineCompiler" );
	{
		engine_pipeline_compiler->ProjFolder( "EngineTools" );
		engine_pipeline_compiler->AddFoldersRecursive( "" );
		engine_pipeline_compiler->LinkLibrary( core_stl )
								->LinkLibrary( "glsl", "ENABLE_GLSLANG" )
								->LinkLibrary( "${D3DCOMPILER_LIBRARY}", "D3DCOMPILER_LIBRARY" );
		engine_pipeline_compiler->IncludeDirectory( "${EXTERNALS_PATH}/glslang" );
	}

	auto	engine_res_pack = builder->AddExecutable( "Engine.ResourcePacker", "ResourcePacker" );
	{
		engine_res_pack->ProjFolder( "EngineTools" );
		engine_res_pack->AddFoldersRecursive( "" );
		engine_res_pack->LinkLibrary( engine_pipeline_compiler )->LinkLibrary( core_script );
		engine_res_pack->LinkLibrary( "FreeImage", "ENABLE_FREEIMAGE" )
						->LinkLibrary( "lodepng", "ENABLE_LODEPNG" )
						->LinkLibrary( "DevIL", "ENABLE_DEVIL" );
		engine_res_pack->AddSource(
			"if ( CONFIGURATION_DEPENDENT_PATH )\n"
			"	set( RESOURCE_PACKER_EXE \"${MAIN_BINARY_DIR}/$<CONFIG>/"_str << engine_res_pack->GetName() << "${CMAKE_EXECUTABLE_SUFFIX}\" CACHE INTERNAL \"\" FORCE )\n"
			"else ()\n"
			"	set( RESOURCE_PACKER_EXE \"${MAIN_BINARY_DIR}/"_str << engine_res_pack->GetName() << "${CMAKE_EXECUTABLE_SUFFIX}\" CACHE INTERNAL \"\" FORCE )\n"
			"endif ()\n"
		);
	}
#	endif	// ENABLE_EXTERNALS
	//----------------------------------------------------------------------------
		
		
	// Tests //
	auto	test_engine_base = builder->AddExecutable( "Tests.Engine.Base", tests_path + "Engine.Base" );
	{
		test_engine_base->AddFoldersRecursive( "" );
		test_engine_base->ProjFolder( "EngineTests" );
		test_engine_base->LinkLibrary( engine_platforms )->LinkLibrary( engine_profilers );
	}
		
	auto	test_engine_graphics = builder->AddExecutable( "Tests.Engine.Graphics", tests_path + "Engine.Graphics" );
	{
		test_engine_graphics->AddFoldersRecursive( "" );
		test_engine_graphics->ProjFolder( "EngineTests" );
		test_engine_graphics->LinkLibrary( engine_importexport )->LinkLibrary( engine_profilers );
	}

	auto	test_engine_gapi = builder->AddExecutable( "Tests.Engine.Platforms.GAPI", tests_path + "Engine.Platforms.GAPI" );
	{
		test_engine_gapi->AddFoldersRecursive( "" );
		test_engine_gapi->ProjFolder( "EngineTests" );
		test_engine_gapi->LinkLibrary( engine_platforms );
	}
	//----------------------------------------------------------------------------


	// External //
#	ifdef ENABLE_PIPELINE_BUILDING
	const auto	PackRes =	LAMBDA( engine_res_pack ) (auto* proj, StringCRef resourceScript) {
								PackResources( proj, resourceScript, engine_res_pack );
							};

	PackRes( test_engine_base, "Pipelines/resources.as" );
	PackRes( test_engine_gapi, "resources.as" );
	PackRes( test_engine_graphics, "Pipelines/resources.as" );
#	endif	// ENABLE_PIPELINE_BUILDING
	//----------------------------------------------------------------------------
		
	
	// SCU //
#	ifdef ENABLE_SCU
	builder->AddExecutable( "Tests.Engine.Base.SCU" )->ProjFolder("SCU")->LinkLibrary( test_engine_base )->MergeCPP( NUM_THREADS );
	builder->AddExecutable( "Tests.Engine.Graphics.SCU" )->ProjFolder("SCU")->LinkLibrary( test_engine_graphics )->MergeCPP( NUM_THREADS );
	builder->AddExecutable( "Tests.Platforms.GAPI.SCU" )->ProjFolder("SCU")->LinkLibrary( test_engine_gapi )->MergeCPP( NUM_THREADS );
#	endif	// ENABLE_SCU
	//----------------------------------------------------------------------------
	

	builder->Save();

	return builder;

#else
	return null;

#endif	// ENABLE_ENGINE
}
