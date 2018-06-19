// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Core/Config/Engine.Config.h"

#ifdef GRAPHICS_API_VULKAN

#include "Engine/Platforms/Vulkan/110/Vk1BaseObject.h"
#include "Engine/Platforms/Public/GPU/PipelineLayout.h"

namespace Engine
{
namespace PlatformVK
{
	class Vk1PipelineLayout;
	SHARED_POINTER( Vk1PipelineLayout );


	
	//
	// Vulkan Pipeline Layout
	//

	class Vk1PipelineLayout final : public Vk1BaseObject
	{
		friend class Vk1PipelineLayoutCache;

	// types
	private:
		using PushConstants_t		= GpuMsg::GetVkPipelineLayoutPushConstants::PushConstants_t;
		using DescriptionBindings_t	= FixedSizeArray< vk::VkDescriptorSetLayoutBinding, 64 >;
		using PushConstantRanges_t	= FixedSizeArray< vk::VkPushConstantRange, 16 >;
		struct _CreateDescription_Func;


	// variables
	private:
		PipelineLayoutDescription	_layoutDescr;
		vk::VkDescriptorSetLayout	_descriptorId;
		vk::VkPipelineLayout		_layoutId;
		PushConstants_t				_pushConstants;


	// methods
	public:
		explicit Vk1PipelineLayout (Ptr<Vk1Device> dev);
		~Vk1PipelineLayout ();

		bool Create (const PipelineLayoutDescription &layout);
		void Destroy ();

		ND_ PipelineLayoutDescription const&	GetDescription ()			const	{ return _layoutDescr; }
		ND_ vk::VkPipelineLayout				GetLayoutID ()				const	{ return _layoutId; }
		ND_ vk::VkDescriptorSetLayout			GetDescriptionLayouts ()	const	{ return _descriptorId; }
		ND_ PushConstants_t const&				GetPushConstants ()			const	{ return _pushConstants; }

	private:
		void _Init (const PipelineLayoutDescription &layout);

		void _DestroyLayout ();
		void _DestroyLayoutDescriptions ();
			
		bool _CreateLayoutDescriptions (const PipelineLayoutDescription &layout, OUT PushConstantRanges_t &pc);
	};
	
	

	//
	// Vulkan Pipeline Layout Cache
	//

	class Vk1PipelineLayoutCache final : public Vk1BaseObject
	{
	// types
	private:
		struct SearchableLayout
		{
			Vk1PipelineLayoutPtr	ptr;

			explicit SearchableLayout (const Vk1PipelineLayoutPtr &ptr) : ptr(ptr) {}
			
			bool operator == (const SearchableLayout &right) const	{ return ptr->GetDescription() == right.ptr->GetDescription(); }
			bool operator >  (const SearchableLayout &right) const	{ return ptr->GetDescription() >  right.ptr->GetDescription(); }
		};

		struct LayoutSearch
		{
			PipelineLayoutDescription const&		descr;

			explicit LayoutSearch (const PipelineLayoutDescription &d) : descr(d) {}

			bool operator == (const SearchableLayout &right) const	{ return descr == right.ptr->GetDescription(); }
			bool operator >  (const SearchableLayout &right) const	{ return descr >  right.ptr->GetDescription(); }
		};

		using Layouts_t	= Set< SearchableLayout >;


	// variables
	private:
		Layouts_t		_layouts;


	// methods
	public:
		explicit Vk1PipelineLayoutCache (Ptr<Vk1Device> dev);
		~Vk1PipelineLayoutCache ();

		ND_ Vk1PipelineLayoutPtr  Create (const PipelineLayoutDescription &layout);

		void Destroy ();
	};


}	// PlatformVK
}	// Engine

#endif	// GRAPHICS_API_VULKAN
