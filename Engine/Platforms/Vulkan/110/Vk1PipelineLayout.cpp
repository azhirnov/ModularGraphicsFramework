// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Core/Config/Engine.Config.h"

#ifdef GRAPHICS_API_VULKAN

#include "Engine/Platforms/Vulkan/110/Vk1PipelineLayout.h"

namespace Engine
{
namespace PlatformVK
{
	using namespace vk;

/*
=================================================
	constructor
=================================================
*/
	Vk1PipelineLayout::Vk1PipelineLayout (Ptr<Vk1Device> dev) :
		Vk1BaseObject( dev ),
		_descriptorId( VK_NULL_HANDLE ),	_layoutId( VK_NULL_HANDLE )
	{
	}
	
/*
=================================================
	destructor
=================================================
*/
	Vk1PipelineLayout::~Vk1PipelineLayout ()
	{
		ASSERT( _layoutId == VK_NULL_HANDLE );
	}

/*
=================================================
	Create
=================================================
*/
	bool Vk1PipelineLayout::Create (const PipelineLayoutDescription &descr)
	{
		Destroy();

		_layoutDescr = descr;

		PushConstantRanges_t	pc_ranges;

		CHECK_ERR( _CreateLayoutDescriptions( descr, OUT pc_ranges ) );
		
		VkPipelineLayoutCreateInfo			layout_info = {};
		layout_info.sType					= VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		layout_info.setLayoutCount			= 1u;
		layout_info.pSetLayouts				= &_descriptorId;
		layout_info.pushConstantRangeCount	= uint(pc_ranges.Count());
		layout_info.pPushConstantRanges		= pc_ranges.RawPtr();

		VK_CHECK( vkCreatePipelineLayout( GetVkDevice(), &layout_info, null, OUT &_layoutId ) );

		GetDevice()->SetObjectName( ReferenceCast<uint64_t>(_layoutId), "Vk1PipelineLayout", EGpuObject::PipelineLayout );
		return true;
	}
	
/*
=================================================
	_CreateDescription_Func
=================================================
*/
	struct Vk1PipelineLayout::_CreateDescription_Func
	{
	// types
		using TextureUniform	= PipelineLayoutDescription::TextureUniform;
		using SamplerUniform	= PipelineLayoutDescription::SamplerUniform;
		using ImageUniform		= PipelineLayoutDescription::ImageUniform;
		using UniformBuffer		= PipelineLayoutDescription::UniformBuffer;
		using StorageBuffer		= PipelineLayoutDescription::StorageBuffer;
		using PushConstant		= PipelineLayoutDescription::PushConstant;
		using SubpassInput		= PipelineLayoutDescription::SubpassInput;


	// variables
		DescriptionBindings_t&	bindings;
		PushConstantRanges_t&	pushConstRanges;
		PushConstants_t&		pushConstMap;


	// methods
		_CreateDescription_Func (OUT DescriptionBindings_t &bindings, OUT PushConstantRanges_t &pushConstRanges, OUT PushConstants_t &pcMap) :
			bindings( bindings ), pushConstRanges( pushConstRanges ), pushConstMap( pcMap )
		{}


		void operator () (const TextureUniform &tex) const
		{
			ASSERT( tex.textureType != EImage::Buffer );	// type must be VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER or VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER

			VkDescriptorSetLayoutBinding	bind = {};
			bind.descriptorType		= VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
			bind.stageFlags			= Vk1Enum( tex.stageFlags );
			bind.binding			= tex.uniqueIndex;
			bind.descriptorCount	= 1;

			bindings.PushBack( bind );
		}
		

		void operator () (const SamplerUniform &samp) const
		{
			VkDescriptorSetLayoutBinding	bind = {};
			bind.descriptorType		= VK_DESCRIPTOR_TYPE_SAMPLER;
			bind.stageFlags			= Vk1Enum( samp.stageFlags );
			bind.binding			= samp.uniqueIndex;
			bind.descriptorCount	= 1;

			bindings.PushBack( bind );
		}
		

		void operator () (const ImageUniform &img) const
		{
			VkDescriptorSetLayoutBinding	bind = {};
			bind.descriptorType		= VK_DESCRIPTOR_TYPE_STORAGE_IMAGE;
			bind.stageFlags			= Vk1Enum( img.stageFlags );
			bind.binding			= img.uniqueIndex;
			bind.descriptorCount	= 1;

			bindings.PushBack( bind );
		}


		void operator () (const UniformBuffer &buf) const
		{
			VkDescriptorSetLayoutBinding	bind = {};
			bind.descriptorType		= VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;	// TODO: VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC
			bind.stageFlags			= Vk1Enum( buf.stageFlags );
			bind.binding			= buf.uniqueIndex;
			bind.descriptorCount	= 1;

			bindings.PushBack( bind );
		}
		

		void operator () (const StorageBuffer &buf) const
		{
			VkDescriptorSetLayoutBinding	bind = {};
			bind.descriptorType		= VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;	// TODO: VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC
			bind.stageFlags			= Vk1Enum( buf.stageFlags );
			bind.binding			= buf.uniqueIndex;
			bind.descriptorCount	= 1;

			bindings.PushBack( bind );
		}
		

		void operator () (const PushConstant &pc) const
		{
			VkPushConstantRange		range = {};
			range.stageFlags	= Vk1Enum( pc.stageFlags );
			range.offset		= uint(pc.offset);
			range.size			= uint(pc.size);

			pushConstRanges.PushBack( range );
			pushConstMap.Add( pc.name, { pc.stageFlags, pc.offset, pc.size } );
		}
		

		void operator () (const SubpassInput &) const
		{
			WARNING( "not supported" );
			// TODO: VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT
		}
	};
	
/*
=================================================
	Destroy
=================================================
*/
	void Vk1PipelineLayout::Destroy ()
	{
		_DestroyLayout();
		_DestroyLayoutDescriptions();

		_pushConstants.Clear();
	}
	
/*
=================================================
	_CreateLayoutDescriptions
=================================================
*/
	bool Vk1PipelineLayout::_CreateLayoutDescriptions (const PipelineLayoutDescription &descr, OUT PushConstantRanges_t &pushConstRanges)
	{
		CHECK_ERR( descr.GetUniforms().Count() <= DescriptionBindings_t::MemoryContainer_t::SIZE );
		CHECK_ERR( _descriptorId == VK_NULL_HANDLE );

		DescriptionBindings_t	descr_binding;
		_CreateDescription_Func	func( OUT descr_binding, OUT pushConstRanges, OUT _pushConstants );

		FOR( i, descr.GetUniforms() ) {
			descr.GetUniforms()[i].Accept( func );
		}

		CHECK_ERR( not descr_binding.Empty() );	// empty descriptor set is not allowed

		VkDescriptorSetLayoutCreateInfo	descriptor_info = {};

		descriptor_info.sType			= VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
		descriptor_info.pBindings		= descr_binding.RawPtr();
		descriptor_info.bindingCount	= uint(descr_binding.Count());

		VK_CHECK( vkCreateDescriptorSetLayout( GetVkDevice(), &descriptor_info, null, OUT &_descriptorId ) );
		return true;
	}
	
/*
=================================================
	_DestroyLayoutDescriptions
=================================================
*/
	void Vk1PipelineLayout::_DestroyLayoutDescriptions ()
	{
		auto	device = GetVkDevice();

		if ( device != VK_NULL_HANDLE and _descriptorId != VK_NULL_HANDLE ) {
			vkDestroyDescriptorSetLayout( device, _descriptorId, null );
		}

		_descriptorId = VK_NULL_HANDLE;
	}

/*
=================================================
	_DestroyLayout
=================================================
*/
	void Vk1PipelineLayout::_DestroyLayout ()
	{
		auto	device = GetVkDevice();

		if ( device != VK_NULL_HANDLE and _layoutId != VK_NULL_HANDLE )
		{
			vkDestroyPipelineLayout( device, _layoutId, null );
		}

		_layoutId = VK_NULL_HANDLE;
	}
//-----------------------------------------------------------------------------



/*
=================================================
	constructor
=================================================
*/
	Vk1PipelineLayoutCache::Vk1PipelineLayoutCache (Ptr<Vk1Device> dev) :
		Vk1BaseObject( dev )
	{
	}
	
/*
=================================================
	destructor
=================================================
*/
	Vk1PipelineLayoutCache::~Vk1PipelineLayoutCache ()
	{
		ASSERT( _layouts.Empty() );
	}
	
/*
=================================================
	Create
=================================================
*/
	Vk1PipelineLayoutPtr  Vk1PipelineLayoutCache::Create (const PipelineLayoutDescription &descr)
	{
		using LayoutIter_t	= Layouts_t::const_iterator;

		Vk1PipelineLayoutPtr	res		= New< Vk1PipelineLayout >( GetDevice() );
		LayoutIter_t			iter;

		if ( _layouts.CustomSearch().Find( LayoutSearch(descr), OUT iter ) ) {
			return iter->ptr;
		}

		CHECK_ERR( res->Create( descr ) );
		_layouts.Add( SearchableLayout(res) );

		return res;
	}
	
/*
=================================================
	Destroy
=================================================
*/
	void Vk1PipelineLayoutCache::Destroy ()
	{
		FOR( i, _layouts )
		{
			_layouts[i].ptr->Destroy();
		}
		_layouts.Clear();
	}

}	// PlatformVK
}	// Engine

#endif	// GRAPHICS_API_VULKAN
