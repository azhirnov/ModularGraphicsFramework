// This is generated file
// Created at: 2018/04/26 - 11:15:56

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
void Create_copyfloatimage2d (PipelineTemplateDescriptor& descr);

// From file 'findlsb.cpp'
void Create_findlsb (PipelineTemplateDescriptor& descr);

// From file 'findmsb.cpp'
void Create_findmsb (PipelineTemplateDescriptor& descr);

// From file 'shaderbarrier.cpp'
void Create_shaderbarrier (PipelineTemplateDescriptor& descr);

// From file 'bufferalign.cpp'
void Create_bufferalign (PipelineTemplateDescriptor& descr);

// From file 'atomicadd.cpp'
void Create_atomicadd (PipelineTemplateDescriptor& descr);

// From file 'dynamicbuffer.cpp'
void Create_dynamicbuffer (PipelineTemplateDescriptor& descr);

};
