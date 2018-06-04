// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/Config/Engine.Config.h"

#ifdef GRAPHICS_API_VULKAN

#include "Engine/Platforms/Vulkan/110/Vk1BaseModule.h"
#include "Engine/Platforms/Vulkan/110/Vk1BaseObject.h"
#include "Engine/Platforms/Public/GPU/RenderPass.h"

namespace Engine
{
namespace PlatformVK
{

	//
	// Vulkan Render Pass Cache
	//

	class Vk1RenderPassCache final : public Vk1BaseObject
	{
	// types
	private:
		using Vk1RenderPassPtr = SharedPointerType< class Vk1RenderPass >;
		
		struct SearchableRenderPass
		{
			Vk1RenderPassPtr	rp;

			explicit SearchableRenderPass (const Vk1RenderPassPtr &rp) : rp(rp) {}

			bool operator == (const SearchableRenderPass &right) const;
			bool operator >  (const SearchableRenderPass &right) const;
		};

		struct RenderPassSearch
		{
			RenderPassDescriptor	descr;

			explicit RenderPassSearch (const RenderPassDescriptor &s) : descr(s) {}

			bool operator == (const SearchableRenderPass &right) const;
			bool operator >  (const SearchableRenderPass &right) const;
		};

		using RenderPasses_t	= Set< SearchableRenderPass >;


	// variables
	private:
		RenderPasses_t			_renderPasses;


	// methods
	public:
		explicit Vk1RenderPassCache (Ptr<Vk1Device> dev);

		ND_ Vk1RenderPassPtr  Create (ModuleMsg::UntypedID_t, GlobalSystemsRef gs, const CreateInfo::GpuRenderPass &ci);

		void Destroy ();
	};


}	// PlatformVK
}	// Engine

#endif	// GRAPHICS_API_VULKAN
