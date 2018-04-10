// This is generated file
// Created at: 2018/04/10 - 21:49:15

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
// From file 'atomicadd.cpp'
void Create_atomicadd (PipelineTemplateDescriptor& descr);

// From file 'bufferalign.cpp'
void Create_bufferalign (PipelineTemplateDescriptor& descr);

// From file 'findmsb.cpp'
void Create_findmsb (PipelineTemplateDescriptor& descr);

// From file 'findlsb.cpp'
void Create_findlsb (PipelineTemplateDescriptor& descr);

// From file 'dynamicbuffer.cpp'
void Create_dynamicbuffer (PipelineTemplateDescriptor& descr);

// From file 'copyfloatimage2d.cpp'
void Create_copyfloatimage2d (PipelineTemplateDescriptor& descr);

// From file 'shaderbarrier.cpp'
void Create_shaderbarrier (PipelineTemplateDescriptor& descr);

};
