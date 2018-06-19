// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Core/Config/Engine.Config.h"

#ifdef GRAPHICS_API_OPENGL

#include "Engine/Platforms/Public/GPU/Sampler.h"

namespace Engine
{
namespace PlatformGL
{

	//
	// OpenGL Sampler Cache
	//

	class GL4SamplerCache final
	{
	// types
	private:
		using GL4SamplerPtr			= SharedPointerType< class GL4Sampler >;
		using SamplerDescription	= Platforms::SamplerDescription;

		struct SearchableSampler
		{
			GL4SamplerPtr	samp;

			explicit SearchableSampler (const GL4SamplerPtr &s) : samp(s) {}

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

		uint			_maxAnisotropy;
		bool			_initialized;


	// methods
	public:
		GL4SamplerCache ();

		ND_ GL4SamplerPtr	Create (ModuleMsg::UntypedID_t, GlobalSystemsRef gs, const CreateInfo::GpuSampler &ci);

		// device features
		ND_ bool	SupportAnisotropyFiltering ()	const	{ return _maxAnisotropy > 0; }
		ND_ uint	GetMaxAnisotropyLevel ()		const	{ return _maxAnisotropy; }

	private:
		void _Initialize ();
	};


}	// PlatformGL
}	// Engine

#endif	// GRAPHICS_API_OPENGL
