// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Engine/Platforms/Vulkan/Impl/Vk1PipelineLayout.h"

#if defined( GRAPHICS_API_VULKAN )

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
		_layoutId( VK_NULL_HANDLE )
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
	bool Vk1PipelineLayout::Create (const PipelineLayoutDescriptor &descr)
	{
		Destroy();

		_layoutDescr = descr;

		CHECK_ERR( _CreateLayoutDescriptors( descr ) );
		
		VkPipelineLayoutCreateInfo			layout_info = {};
		layout_info.sType					= VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		layout_info.setLayoutCount			= (uint) _descriptorIds.Count();
		layout_info.pSetLayouts				= _descriptorIds.RawPtr();
		layout_info.pushConstantRangeCount	= (uint) _pushConstRanges.Count();
		layout_info.pPushConstantRanges		= _pushConstRanges.RawPtr();

		VK_CHECK( vkCreatePipelineLayout( GetVkDevice(), &layout_info, null, OUT &_layoutId ) );

		GetDevice()->SetObjectName( ReferenceCast<uint64_t>(_layoutId), "Vk1PipelineLayout", EGpuObject::PipelineLayout );
		return true;
	}
	
/*
=================================================
	_CreateDescriptor_Func
=================================================
*/
	struct Vk1PipelineLayout::_CreateDescriptor_Func
	{
	// types
		using TextureUniform	= PipelineLayoutDescriptor::TextureUniform;
		using SamplerUniform	= PipelineLayoutDescriptor::SamplerUniform;
		using ImageUniform		= PipelineLayoutDescriptor::ImageUniform;
		using UniformBuffer		= PipelineLayoutDescriptor::UniformBuffer;
		using StorageBuffer		= PipelineLayoutDescriptor::StorageBuffer;
		using PushConstant		= PipelineLayoutDescriptor::PushConstant;
		using SubpassInput		= PipelineLayoutDescriptor::SubpassInput;
		using Uniform			= PipelineLayoutDescriptor::Uniform;


	// variables
		DescriptorBindings&		bindings;
		PushConstantRanges&		pushConstRanges;
		const uint				set;


	// methods
		_CreateDescriptor_Func (OUT DescriptorBindings &bindings, OUT PushConstantRanges &pushConstRanges, uint set) :
			bindings( bindings ), pushConstRanges( pushConstRanges ), set( set )
		{}


		void operator () (const TextureUniform &tex) const
		{
			ASSERT( tex.textureType != EImage::Buffer );	// type must be VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER or VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER

			if ( set != tex.descriptorSet )
				return;

			VkDescriptorSetLayoutBinding	bind = {};
			bind.descriptorType		= VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
			bind.stageFlags			= Vk1Enum( tex.stageFlags );
			bind.binding			= tex.binding;
			bind.descriptorCount	= 1;

			bindings.PushBack( bind );
		}
		

		void operator () (const SamplerUniform &samp) const
		{
			if ( set != samp.descriptorSet )
				return;

			VkDescriptorSetLayoutBinding	bind = {};
			bind.descriptorType		= VK_DESCRIPTOR_TYPE_SAMPLER;
			bind.stageFlags			= Vk1Enum( samp.stageFlags );
			bind.binding			= samp.binding;
			bind.descriptorCount	= 1;

			bindings.PushBack( bind );
		}
		

		void operator () (const ImageUniform &img) const
		{
			if ( set != img.descriptorSet )
				return;

			VkDescriptorSetLayoutBinding	bind = {};
			bind.descriptorType		= VK_DESCRIPTOR_TYPE_STORAGE_IMAGE;
			bind.stageFlags			= Vk1Enum( img.stageFlags );
			bind.binding			= img.binding;
			bind.descriptorCount	= 1;

			bindings.PushBack( bind );
		}


		void operator () (const UniformBuffer &buf) const
		{
			if ( set != buf.descriptorSet )
				return;

			VkDescriptorSetLayoutBinding	bind = {};
			bind.descriptorType		= VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;	// TODO: VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC
			bind.stageFlags			= Vk1Enum( buf.stageFlags );
			bind.binding			= buf.binding;
			bind.descriptorCount	= 1;

			bindings.PushBack( bind );
		}
		

		void operator () (const StorageBuffer &buf) const
		{
			if ( set != buf.descriptorSet )
				return;

			VkDescriptorSetLayoutBinding	bind = {};
			bind.descriptorType		= VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;	// TODO: VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC
			bind.stageFlags			= Vk1Enum( buf.stageFlags );
			bind.binding			= buf.binding;
			bind.descriptorCount	= 1;

			bindings.PushBack( bind );
		}
		

		void operator () (const PushConstant &pc) const
		{
			if ( set != 0 )
				return;

			VkPushConstantRange		range = {};
			range.stageFlags	= Vk1Enum( pc.stageFlags );
			range.offset		= (uint) pc.offset;
			range.size			= (uint) pc.size;

			pushConstRanges.PushBack( range );
		}
		

		void operator () (const SubpassInput &sp) const
		{
			if ( set != sp.descriptorSet )
				return;
			
			WARNING( "not supported" );
			// TODO: VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT
		}
		

		void operator () (const Uniform &un) const
		{
			WARNING( "not supported" );
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
		_DestroyLayoutDescriptors();

		_pushConstRanges.Clear();
	}
	
/*
=================================================
	_CreateLayoutDescriptors
=================================================
*/
	bool Vk1PipelineLayout::_CreateLayoutDescriptors (const PipelineLayoutDescriptor &descr)
	{
		DescriptorBindings	descr_binding;

		CHECK_ERR( descr.GetUniforms().Count() <= DescriptorBindings::MemoryContainer_t::SIZE );

		for (usize set = 0; set <= descr.MaxDescriptorSet(); ++set)
		{
			_CreateDescriptor_Func	func( OUT descr_binding, OUT _pushConstRanges, uint(set) );

			FOR( i, descr.GetUniforms() ) {
				descr.GetUniforms()[i].Apply( func );
			}

			CHECK_ERR( not descr_binding.Empty() );	// empty descriptor set is not allowed

			VkDescriptorSetLayoutCreateInfo	descriptor_info		= {};
			VkDescriptorSetLayout			descriptor_layout	= VK_NULL_HANDLE;

			descriptor_info.sType			= VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
			descriptor_info.pBindings		= descr_binding.RawPtr();
			descriptor_info.bindingCount	= (uint) descr_binding.Count();

			VK_CHECK( vkCreateDescriptorSetLayout( GetVkDevice(), &descriptor_info, null, OUT &descriptor_layout ) );
			_descriptorIds.PushBack( descriptor_layout );
		}
		return true;
	}
	
/*
=================================================
	_DestroyLayoutDescriptors
=================================================
*/
	void Vk1PipelineLayout::_DestroyLayoutDescriptors ()
	{
		auto	device = GetVkDevice();

		if ( device == VK_NULL_HANDLE )
			return;

		FOR( i, _descriptorIds ) {
			vkDestroyDescriptorSetLayout( device, _descriptorIds[i], null );
		}
		_descriptorIds.Clear();
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
	Vk1PipelineLayoutPtr  Vk1PipelineLayoutCache::Create (const PipelineLayoutDescriptor &descr)
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
