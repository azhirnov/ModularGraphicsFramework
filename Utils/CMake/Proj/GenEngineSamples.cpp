// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "PipelineBuilding.h"

CMakeBuilderPtr  GenEngineSamples (Ptr<CMakeBuilder> engine)
{
#ifdef ENABLE_ENGINE_SAMPLES

	CMakeBuilderPtr		builder{ new CMakeBuilder{ "EngineSamples", "EngineSamples" } };
	
	builder->InheritCompilers( engine );
	builder->InheritIncludeDirectories( engine );
	builder->SetSecondary();

	auto	engine_scene		= engine->GetProject( "Engine.Scene" );
	auto	engine_profilers	= engine->GetProject( "Engine.Profilers" );
	auto	engine_importexport	= engine->GetProject( "Engine.ImportExport" );
	auto	engine_res_pack		= engine->GetProject( "Engine.ResourcePacker" );
	//----------------------------------------------------------------------------


	auto	sample_particles = builder->AddExecutable( "Samples.Particles", "Particles" );
	{
		sample_particles->AddFoldersRecursive( "" );
		sample_particles->ProjFolder( "EngineSamples" );
		sample_particles->IncludeDirectory( "" );
		sample_particles->LinkLibrary( engine_scene )->LinkLibrary( engine_profilers )->LinkLibrary( engine_importexport );
	}

	auto	sample_shadows = builder->AddExecutable( "Samples.Shadows", "Shadows" );
	{
		sample_shadows->AddFoldersRecursive( "" );
		sample_shadows->ProjFolder( "EngineSamples" );
		sample_shadows->IncludeDirectory( "" );
		sample_shadows->LinkLibrary( engine_scene )->LinkLibrary( engine_profilers )->LinkLibrary( engine_importexport );
	}

	auto	sample_procedural_terrain = builder->AddExecutable( "Samples.ProceduralTerrain", "ProceduralTerrain" );
	{
		sample_procedural_terrain->AddFoldersRecursive( "" );
		sample_procedural_terrain->ProjFolder( "EngineSamples" );
		sample_procedural_terrain->IncludeDirectory( "" );
		sample_procedural_terrain->LinkLibrary( engine_scene )->LinkLibrary( engine_profilers )->LinkLibrary( engine_importexport );
	}

	auto	sample_procedural_city = builder->AddExecutable( "Samples.ProceduralCity", "ProceduralCity" );
	{
		sample_procedural_city->AddFoldersRecursive( "" );
		sample_procedural_city->ProjFolder( "EngineSamples" );
		sample_procedural_city->IncludeDirectory( "" );
		sample_procedural_city->LinkLibrary( engine_scene )->LinkLibrary( engine_profilers )->LinkLibrary( engine_importexport );
	}
	//----------------------------------------------------------------------------

	
#	ifdef ENABLE_PIPELINE_BUILDING
	const auto	PackRes =	LAMBDA( engine_res_pack ) (auto* proj, StringCRef resourceScript) {
								PackResources( proj, resourceScript, engine_res_pack );
							};

	PackRes( sample_particles, "resources.as" );
	PackRes( sample_shadows, "resources.as" );
	PackRes( sample_procedural_terrain, "resources.as" );
	PackRes( sample_procedural_city, "resources.as" );
#	endif	// ENABLE_PIPELINE_BUILDING
	//----------------------------------------------------------------------------
	
	builder->Save();
	return builder;

#else
	return null;

#endif	// ENABLE_ENGINE_SAMPLES
}
