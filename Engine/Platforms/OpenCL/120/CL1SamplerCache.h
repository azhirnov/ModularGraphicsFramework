// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/Config/Engine.Config.h"

#ifdef COMPUTE_API_OPENCL

#include "Engine/Platforms/Public/GPU/Sampler.h"

namespace Engine
{
namespace PlatformCL
{

	//
	// OpenGL Sampler Cache
	//

	class CL1SamplerCache final
	{
	// types
	private:
		using CL1SamplerPtr		= SharedPointerType< class CL1Sampler >;
		using SamplerDescriptor	= Platforms::SamplerDescriptor;

		struct SearchableSampler
		{
			CL1SamplerPtr	samp;

			explicit SearchableSampler (const CL1SamplerPtr &s) : samp(s) {}

			bool operator == (const SearchableSampler &right) const;
			bool operator >  (const SearchableSampler &right) const;
		};

		struct SamplerSearch
		{
			SamplerDescriptor	descr;

			explicit SamplerSearch (const SamplerDescriptor &s) : descr(s) {}

			bool operator == (const SearchableSampler &right) const;
			bool operator >  (const SearchableSampler &right) const;
		};

		using Samplers_t	= Set< SearchableSampler >;


	// variables
	private:
		Samplers_t		_samplers;


	// methods
	public:
		CL1SamplerCache ();

		ND_ CL1SamplerPtr	Create (ModuleMsg::UntypedID_t, GlobalSystemsRef gs, const CreateInfo::GpuSampler &ci);
	};


}	// PlatformCL
}	// Engine

#endif	// COMPUTE_API_OPENCL
