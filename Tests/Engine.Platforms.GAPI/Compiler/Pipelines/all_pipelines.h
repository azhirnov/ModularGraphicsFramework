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
// From file 'vecswizzle.cpp'
void Create_vecswizzle (PipelineTemplateDescriptor& descr);

// From file 'atomicadd.cpp'
void Create_atomicadd (PipelineTemplateDescriptor& descr);

// From file 'globaltolocal.cpp'
void Create_globaltolocal (PipelineTemplateDescriptor& descr);

// From file 'unnamedbuffer.cpp'
void Create_unnamedbuffer (PipelineTemplateDescriptor& descr);

// From file 'inlineall.cpp'
void Create_inlineall (PipelineTemplateDescriptor& descr);

// From file 'findlsb.cpp'
void Create_findlsb (PipelineTemplateDescriptor& descr);

// From file 'findmsb.cpp'
void Create_findmsb (PipelineTemplateDescriptor& descr);

}	// Pipelines
