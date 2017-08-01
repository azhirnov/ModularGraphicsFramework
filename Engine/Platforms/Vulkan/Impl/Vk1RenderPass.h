// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/Platforms/Vulkan/Impl/Vk1BaseModule.h"
#include "Engine/Platforms/Shared/GPU/RenderPass.h"

#if defined( GRAPHICS_API_VULKAN )

namespace Engine
{
namespace PlatformVK
{
	
	//
	// Vulkan Render Pass
	//

	class Vk1RenderPass final : public Vk1BaseModule
	{
	// types
	private:
		using SupportedMessages_t	= Vk1BaseModule::SupportedMessages_t::Append< MessageListFrom<
											ModuleMsg::GetGpuRenderPassDescriptor,
											ModuleMsg::GetVkRenderPassID
										> >;

		using SupportedEvents_t		= Vk1BaseModule::SupportedEvents_t;


	// constants
	private:
		static const Runtime::VirtualTypeList	_msgTypes;
		static const Runtime::VirtualTypeList	_eventTypes;


	// variables
	private:
		RenderPassDescriptor	_descr;

		vk::VkRenderPass		_renderPassId;


	// methods
	public:
		Vk1RenderPass (const GlobalSystemsRef gs, const CreateInfo::GpuRenderPass &ci);
		~Vk1RenderPass ();

		static OModID::type			GetStaticID ()				{ return "vk1.renpass"_OModID; }


	//protected:
		RenderPassDescriptor const&	GetDescriptor ()	const	{ return _descr; }
		vk::VkRenderPass			GetRenderPassID ()	const	{ return _renderPassId; }


	// message handlers
	private:
		bool _Compose (const  Message< ModuleMsg::Compose > &);
		bool _Delete (const Message< ModuleMsg::Delete > &);
		bool _GetVkRenderPassID (const Message< ModuleMsg::GetVkRenderPassID > &);
		bool _GetGpuRenderPassDescriptor (const Message< ModuleMsg::GetGpuRenderPassDescriptor > &);

	private:
		bool _IsCreated () const;
		bool _CreateRenderPass ();
		void _DestroyRenderPass ();
	};

	

	//
	// Vulkan Render Pass Cache
	//

	class Vk1RenderPassCache final
	{
	// types
	private:
		SHARED_POINTER( Vk1RenderPass );
		
		struct SearchableRenderPass
		{
			Vk1RenderPassPtr	rp;

			explicit SearchableRenderPass (const Vk1RenderPassPtr &rp) : rp(rp) {}

			bool operator == (const SearchableRenderPass &right) const	{ return rp->GetDescriptor() == right.rp->GetDescriptor(); }
			bool operator >  (const SearchableRenderPass &right) const	{ return rp->GetDescriptor() >  right.rp->GetDescriptor(); }
			bool operator <  (const SearchableRenderPass &right) const	{ return rp->GetDescriptor() <  right.rp->GetDescriptor(); }
		};

		struct RenderPassSearch
		{
			RenderPassDescriptor	descr;

			explicit RenderPassSearch (const RenderPassDescriptor &s) : descr(s) {}

			bool operator == (const SearchableRenderPass &right) const	{ return descr == right.rp->GetDescriptor(); }
			bool operator >  (const SearchableRenderPass &right) const	{ return descr >  right.rp->GetDescriptor(); }
			bool operator <  (const SearchableRenderPass &right) const	{ return descr <  right.rp->GetDescriptor(); }
		};

		using RenderPasses_t	= Set< SearchableRenderPass >;


	// variables
	private:
		RenderPasses_t			_renderPasses;
		const VkSystemsRef		_vkSystems;


	// methods
	public:
		explicit Vk1RenderPassCache (VkSystemsRef vkSys);

		Vk1RenderPassPtr	Create (const GlobalSystemsRef gs, const CreateInfo::GpuRenderPass &ci);

		VkSystemsRef		VkSystems ()			const	{ return _vkSystems; }
		Ptr<Vk1Device>		GetDevice ()			const	{ return _vkSystems->Get< Vk1Device >(); }
	};


}	// PlatformVK
}	// Engine

#endif	// GRAPHICS_API_VULKAN
