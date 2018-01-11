// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/Platforms/Vulkan/Impl/Vk1BaseModule.h"
#include "Engine/Platforms/Vulkan/Impl/Vk1BaseObject.h"
#include "Engine/Platforms/Shared/GPU/RenderPass.h"

#if defined( GRAPHICS_API_VULKAN )

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
			bool operator <  (const SearchableRenderPass &right) const;
		};

		struct RenderPassSearch
		{
			RenderPassDescriptor	descr;

			explicit RenderPassSearch (const RenderPassDescriptor &s) : descr(s) {}

			bool operator == (const SearchableRenderPass &right) const;
			bool operator >  (const SearchableRenderPass &right) const;
			bool operator <  (const SearchableRenderPass &right) const;
		};

		using RenderPasses_t	= Set< SearchableRenderPass >;


	// variables
	private:
		RenderPasses_t			_renderPasses;


	// methods
	public:
		explicit Vk1RenderPassCache (Ptr<Vk1Device> dev);

		Vk1RenderPassPtr	Create (GlobalSystemsRef gs, const CreateInfo::GpuRenderPass &ci);

		void Destroy ();
	};


}	// PlatformVK
}	// Engine

#endif	// GRAPHICS_API_VULKAN
