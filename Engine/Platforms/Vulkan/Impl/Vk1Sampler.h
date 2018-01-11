// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/Platforms/Vulkan/Impl/Vk1BaseModule.h"
#include "Engine/Platforms/Vulkan/Impl/Vk1BaseObject.h"
#include "Engine/Platforms/Shared/GPU/Sampler.h"

#if defined( GRAPHICS_API_VULKAN )

namespace Engine
{
namespace PlatformVK
{

	//
	// Vulkan Sampler Cache
	//

	class Vk1SamplerCache final : public Vk1BaseObject
	{
	// types
	private:
		using Vk1SamplerPtr = SharedPointerType< class Vk1Sampler >;

		struct SearchableSampler
		{
			Vk1SamplerPtr	samp;

			explicit SearchableSampler (const Vk1SamplerPtr &s) : samp(s) {}

			bool operator == (const SearchableSampler &right) const;
			bool operator >  (const SearchableSampler &right) const;
			bool operator <  (const SearchableSampler &right) const;
		};

		struct SamplerSearch
		{
			SamplerDescriptor const&	descr;

			explicit SamplerSearch (const SamplerDescriptor &s) : descr(s) {}

			bool operator == (const SearchableSampler &right) const;
			bool operator >  (const SearchableSampler &right) const;
			bool operator <  (const SearchableSampler &right) const;
		};

		using Samplers_t	= Set< SearchableSampler >;


	// variables
	private:
		Samplers_t		_samplers;


	// methods
	public:
		explicit Vk1SamplerCache (Ptr<Vk1Device> dev);

		Vk1SamplerPtr	Create (GlobalSystemsRef gs, const CreateInfo::GpuSampler &ci);

		void Destroy ();

		// device features
		bool	SupportAnisotropyFiltering ()	const	{ return GetDevice()->GetDeviceFeatures().samplerAnisotropy == VK_TRUE; }
		float	GetMaxAnisotropy ()				const	{ return GetDevice()->GetDeviceProperties().limits.maxSamplerAnisotropy; }
		uint	GetMaxAnisotropyLevel ()		const	{ return (uint) GetMaxAnisotropy(); }	// TODO: 2..16
	};


}	// PlatformVK
}	// Engine

#endif	// GRAPHICS_API_VULKAN
