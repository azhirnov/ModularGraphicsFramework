// This is generated file
#pragma once

#include "Engine/Platforms/Engine.Platforms.h"

#ifdef GRAPHICS_API_SOFT
#include "Engine/Platforms/Soft/ShaderLang/SWLang.h"
#endif

namespace Pipelines
{
	using namespace GX_STL;
	using namespace GX_STL::GXTypes;
	using namespace GX_STL::GXMath;

	using namespace Engine::Platforms;
}

#include "shared_types.h"

namespace Pipelines
{
// From file 'drawtexturedquad.cpp'
void Create_drawtexturedquad (PipelineTemplateDescription& descr);

// From file 'voronnoirecursion.cpp'
void Create_voronnoirecursion (PipelineTemplateDescription& descr);

// From file 'glowballs.cpp'
void Create_glowballs (PipelineTemplateDescription& descr);

// From file 'experimental.cpp'
void Create_experimental (PipelineTemplateDescription& descr);

// From file 'sireniandawn2.cpp'
void Create_sireniandawn2 (PipelineTemplateDescription& descr);

// From file 'skyline.cpp'
void Create_skyline (PipelineTemplateDescription& descr);

// From file 'sireniandawn1.cpp'
void Create_sireniandawn1 (PipelineTemplateDescription& descr);

}	// Pipelines
