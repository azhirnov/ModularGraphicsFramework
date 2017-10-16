// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/Platforms/Vulkan/Impl/Vk1BaseModule.h"
#include "Engine/Platforms/Shared/GPU/Sampler.h"

#if defined( GRAPHICS_API_VULKAN )

namespace Engine
{
namespace PlatformVK
{
	
	//
	// Vulkan Sampler
	//

	class Vk1Sampler final : public Vk1BaseModule
	{
	// types
	private:
		using SupportedMessages_t	= Vk1BaseModule::SupportedMessages_t/*::Erase< MessageListFrom<
											ModuleMsg::Delete
										> >*/
										::Append< MessageListFrom<
											GpuMsg::GetSamplerDescriptor,
											GpuMsg::GetVkSamplerID
										> >;

		using SupportedEvents_t		= Vk1BaseModule::SupportedEvents_t;


	// constants
	private:
		static const Runtime::VirtualTypeList	_msgTypes;
		static const Runtime::VirtualTypeList	_eventTypes;


	// variables
	private:
		SamplerDescriptor	_descr;

		vk::VkSampler		_samplerId;


	// methods
	public:
		Vk1Sampler (const GlobalSystemsRef gs, const CreateInfo::GpuSampler &ci);
		~Vk1Sampler ();

		SamplerDescriptor const&	GetDescriptor ()	const	{ return _descr; }


	// message handlers
	private:
		bool _Compose (const  Message< ModuleMsg::Compose > &);
		bool _Delete (const Message< ModuleMsg::Delete > &);
		bool _GetVkSamplerID (const Message< GpuMsg::GetVkSamplerID > &);
		bool _GetSamplerDescriptor (const Message< GpuMsg::GetSamplerDescriptor > &);

	private:
		bool _IsCreated () const;
		bool _CreateSampler ();
		void _DestroySampler ();
		
		void _DestroyResources () override;
	};



	//
	// Vulkan Sampler Cache
	//

	class Vk1SamplerCache final
	{
	// types
	private:
		SHARED_POINTER( Vk1Sampler );

		struct SearchableSampler
		{
			Vk1SamplerPtr	samp;

			explicit SearchableSampler (const Vk1SamplerPtr &s) : samp(s) {}

			bool operator == (const SearchableSampler &right) const	{ return samp->GetDescriptor() == right.samp->GetDescriptor(); }
			bool operator >  (const SearchableSampler &right) const	{ return samp->GetDescriptor() >  right.samp->GetDescriptor(); }
			bool operator <  (const SearchableSampler &right) const	{ return samp->GetDescriptor() <  right.samp->GetDescriptor(); }
		};

		struct SamplerSearch
		{
			SamplerDescriptor const&	descr;

			explicit SamplerSearch (const SamplerDescriptor &s) : descr(s) {}

			bool operator == (const SearchableSampler &right) const	{ return descr == right.samp->GetDescriptor(); }
			bool operator >  (const SearchableSampler &right) const	{ return descr >  right.samp->GetDescriptor(); }
			bool operator <  (const SearchableSampler &right) const	{ return descr <  right.samp->GetDescriptor(); }
		};

		using Samplers_t	= Set< SearchableSampler >;


	// variables
	private:
		Samplers_t				_samplers;
		const VkSystemsRef		_vkSystems;


	// methods
	public:
		explicit Vk1SamplerCache (VkSystemsRef vkSys);

		Vk1SamplerPtr	Create (const GlobalSystemsRef gs, const CreateInfo::GpuSampler &ci);

		void Destroy ();

		VkSystemsRef	VkSystems ()			const	{ return _vkSystems; }
		Ptr<Vk1Device>	GetDevice ()			const	{ return _vkSystems->Get< Vk1Device >(); }

		// device features
		bool	SupportAnisotropyFiltering ()	const	{ return GetDevice()->GetDeviceFeatures().samplerAnisotropy == VK_TRUE; }
		float	GetMaxAnisotropy ()				const	{ return GetDevice()->GetDeviceProperties().limits.maxSamplerAnisotropy; }
		uint	GetMaxAnisotropyLevel ()		const	{ return (uint) GetMaxAnisotropy(); }	// TODO: 2..16
	};


}	// PlatformVK
}	// Engine

#endif	// GRAPHICS_API_VULKAN
