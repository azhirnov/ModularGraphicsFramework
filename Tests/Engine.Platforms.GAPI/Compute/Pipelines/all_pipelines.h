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
// From file 'dynamicbuffer.cpp'
void Create_dynamicbuffer (PipelineTemplateDescriptor& descr);

// From file 'bufferalign.cpp'
void Create_bufferalign (PipelineTemplateDescriptor& descr);

// From file 'copyfloatimage2d.cpp'
void Create_copyfloatimage2d (PipelineTemplateDescriptor& descr);

// From file 'image2dbilinearfilter.cpp'
void Create_image2dbilinearfilter (PipelineTemplateDescriptor& descr);

// From file 'image2dnearestfilter.cpp'
void Create_image2dnearestfilter (PipelineTemplateDescriptor& descr);

// From file 'shaderbarrier.cpp'
void Create_shaderbarrier (PipelineTemplateDescriptor& descr);

};
