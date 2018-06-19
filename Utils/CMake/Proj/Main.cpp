// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "PipelineBuilding.h"


int main ()
{
	CHECK( OS::FileSystem::FindAndSetCurrentDir( "Utils/CMake/Builder", 5 ) );
	CHECK( OS::FileSystem::SetCurrentDirectory( "../../.." ) );
	
	CMakeBuilder	builder{ "", "ModularGraphicsFramework" };

	CMakeBuilderPtr compilers		= GenCompilers();
	compilers->Save( "Core/compilers.cmake" );

	CMakeBuilderPtr	core			= GenCore( compilers.ptr() );
	CMakeBuilderPtr	engine			= GenEngine( core.ptr() );
	CMakeBuilderPtr	shader_editor	= GenShaderEditor( engine.ptr() );
	CMakeBuilderPtr	utils			= GenUtils( core.ptr() );
	

	builder.IncludeExternal( "Core/compilers.cmake" );
	builder.AddSource(R"#(
set( MAIN_BINARY_DIR "${CMAKE_BINARY_DIR}/bin" CACHE INTERNAL "" FORCE )
set( CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${MAIN_BINARY_DIR}" )
set( CMAKE_LIBRARY_OUTPUT_DIRECTORY "${MAIN_BINARY_DIR}" )
set( CMAKE_RUNTIME_OUTPUT_DIRECTORY "${MAIN_BINARY_DIR}" )
)#");

#	ifdef ENABLE_EXTERNALS
	builder.AddExternal( "External/cmake" )->AddSource( "include( \"External/cmake/options.cmake\" )" );
#	endif
	
	builder.AddExternal( core.ptr() );
	builder.AddExternal( engine.ptr() );
	builder.AddExternal( shader_editor.ptr() );
	builder.AddExternal( utils.ptr() );

	builder.Save();

	return 0;
}
