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
// From file 'copyfloatimage2d.cpp'
void Create_copyfloatimage2d (PipelineTemplateDescription& descr);

// From file 'dynamicbuffer.cpp'
void Create_dynamicbuffer (PipelineTemplateDescription& descr);

// From file 'image2dbilinearfilter.cpp'
void Create_image2dbilinearfilter (PipelineTemplateDescription& descr);

// From file 'image2dnearestfilter.cpp'
void Create_image2dnearestfilter (PipelineTemplateDescription& descr);

// From file 'shaderbarrier.cpp'
void Create_shaderbarrier (PipelineTemplateDescription& descr);

// From file 'bufferalign.cpp'
void Create_bufferalign (PipelineTemplateDescription& descr);

}	// Pipelines
