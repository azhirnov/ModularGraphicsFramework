// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Core/Config/Engine.Config.h"

#ifdef GRAPHICS_API_SOFT

#include "Engine/Platforms/Public/GPU/Sampler.h"

namespace Engine
{
namespace PlatformSW
{

	//
	// Software Sampler Cache
	//

	class SWSamplerCache final
	{
	// types
	private:
		using SWSamplerPtr			= SharedPointerType< class SWSampler >;
		using SamplerDescription	= Platforms::SamplerDescription;

		struct SearchableSampler
		{
			SWSamplerPtr	samp;

			explicit SearchableSampler (const SWSamplerPtr &s) : samp(s) {}

			bool operator == (const SearchableSampler &right) const;
			bool operator >  (const SearchableSampler &right) const;
		};

		struct SamplerSearch
		{
			SamplerDescription	descr;

			explicit SamplerSearch (const SamplerDescription &s) : descr(s) {}

			bool operator == (const SearchableSampler &right) const;
			bool operator >  (const SearchableSampler &right) const;
		};

		using Samplers_t	= Set< SearchableSampler >;


	// variables
	private:
		Samplers_t		_samplers;


	// methods
	public:
		SWSamplerCache ();

		SWSamplerPtr	Create (ModuleMsg::UntypedID_t, GlobalSystemsRef gs, const CreateInfo::GpuSampler &ci);
	};


}	// PlatformSW
}	// Engine

#endif	// GRAPHICS_API_SOFT
