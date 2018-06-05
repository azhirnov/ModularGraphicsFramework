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
// From file 'texture2dnearestfilter.cpp'
void Create_texture2dnearestfilter (PipelineTemplateDescriptor& descr);

// From file 'texture2dbilinearfilter.cpp'
void Create_texture2dbilinearfilter (PipelineTemplateDescriptor& descr);

}	// Pipelines
