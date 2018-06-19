// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "PipelineBuilding.h"

CMakeBuilderPtr  GenShaderEditor (Ptr<CMakeBuilder> engine)
{
#ifdef ENABLE_PROJECTS

	CMakeBuilderPtr		builder{ new CMakeBuilder{ "Projects/ShaderEditor", "ShaderEditor" } };
	
	builder->InheritCompilers( engine );
	builder->InheritIncludeDirectories( engine );
	builder->SetSecondary();
	builder->Projects_IncludeDirectory( "" );
	
	auto	engine_scene		= engine->GetProject( "Engine.Scene" );
	auto	engine_profilers	= engine->GetProject( "Engine.Profilers" );
	auto	engine_importexport	= engine->GetProject( "Engine.ImportExport" );
	auto	engine_res_pack		= engine->GetProject( "Engine.ResourcePacker" );
	//----------------------------------------------------------------------------

	
	auto	proj_shader_editor = builder->AddExecutable( "Projects.ShaderEditor", "" );
	{
		proj_shader_editor->AddFoldersRecursive( "" );
		proj_shader_editor->ProjFolder( "Projects" );
		proj_shader_editor->LinkLibrary( engine_scene )->LinkLibrary( engine_profilers )->LinkLibrary( engine_importexport );

		#ifdef ENABLE_PIPELINE_BUILDING
			PackResources( proj_shader_editor, "resources.as", engine_res_pack );
		#endif
	}
	//----------------------------------------------------------------------------
	
	builder->Save();
	return builder;

#else
	return null;

#endif	// ENABLE_PROJECTS
}
