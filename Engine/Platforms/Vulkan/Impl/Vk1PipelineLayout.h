// Copyright �  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/Platforms/Vulkan/Impl/Vk1BaseObject.h"
#include "Engine/Platforms/Shared/GPU/PipelineLayout.h"

#if defined( GRAPHICS_API_VULKAN )

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
		using DescriptorBindings	= FixedSizeArray< vk::VkDescriptorSetLayoutBinding, 64 >;
		using PushConstantRanges	= FixedSizeArray< vk::VkPushConstantRange, 16 >;
		struct _CreateDescriptor_Func;


	// variables
	private:
		PipelineLayoutDescriptor	_layoutDescr;
		vk::VkDescriptorSetLayout	_descriptorId;
		PushConstantRanges			_pushConstRanges;
		vk::VkPipelineLayout		_layoutId;


	// methods
	public:
		explicit Vk1PipelineLayout (Ptr<Vk1Device> dev);
		~Vk1PipelineLayout ();

		bool Create (const PipelineLayoutDescriptor &layout);
		void Destroy ();

		PipelineLayoutDescriptor const&	GetDescriptor ()			const	{ return _layoutDescr; }
		vk::VkPipelineLayout			GetLayoutID ()				const	{ return _layoutId; }
		vk::VkDescriptorSetLayout		GetDescriptorLayouts ()		const	{ return _descriptorId; }

	private:
		void _Init (const PipelineLayoutDescriptor &layout);

		void _DestroyLayout ();
		void _DestroyLayoutDescriptors ();
			
		bool _CreateLayoutDescriptors (const PipelineLayoutDescriptor &layout);
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
			
			bool operator == (const SearchableLayout &right) const	{ return ptr->GetDescriptor() == right.ptr->GetDescriptor(); }
			bool operator >  (const SearchableLayout &right) const	{ return ptr->GetDescriptor() >  right.ptr->GetDescriptor(); }
			bool operator <  (const SearchableLayout &right) const	{ return ptr->GetDescriptor() <  right.ptr->GetDescriptor(); }
		};

		struct LayoutSearch
		{
			PipelineLayoutDescriptor const&		descr;

			explicit LayoutSearch (const PipelineLayoutDescriptor &d) : descr(d) {}

			bool operator == (const SearchableLayout &right) const	{ return descr == right.ptr->GetDescriptor(); }
			bool operator >  (const SearchableLayout &right) const	{ return descr >  right.ptr->GetDescriptor(); }
			bool operator <  (const SearchableLayout &right) const	{ return descr <  right.ptr->GetDescriptor(); }
		};

		using Layouts_t	= Set< SearchableLayout >;


	// variables
	private:
		Layouts_t		_layouts;


	// methods
	public:
		explicit Vk1PipelineLayoutCache (Ptr<Vk1Device> dev);
		~Vk1PipelineLayoutCache ();

		Vk1PipelineLayoutPtr	Create (const PipelineLayoutDescriptor &layout);

		void Destroy ();
	};


}	// PlatformVK
}	// Engine

#endif	// GRAPHICS_API_VULKAN
