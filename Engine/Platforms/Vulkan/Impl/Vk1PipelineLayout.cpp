// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Engine/Platforms/Vulkan/Impl/Vk1PipelineLayout.h"
#include "Engine/Platforms/Vulkan/VulkanThread.h"

#if defined( GRAPHICS_API_VULKAN )

namespace Engine
{
namespace PlatformVK
{

/*
=================================================
	constructor
=================================================
*/
	Vk1PipelineLayout::Vk1PipelineLayout () :
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
	bool Vk1PipelineLayout::Create (const PipelineLayoutDescriptor &descr, vk::VkDevice device)
	{
		using namespace vk;

		CHECK_ERR( device );

		Destroy( device );

		_layoutDescr = descr;

		CHECK_ERR( _CreateLayoutDescriptors( descr, device ) );
		
		VkPipelineLayoutCreateInfo			layout_info = {};
		layout_info.sType					= VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		layout_info.setLayoutCount			= (uint) _descriptorIds.Count();
		layout_info.pSetLayouts				= _descriptorIds.RawPtr();
		layout_info.pushConstantRangeCount	= (uint) _pushConstRanges.Count();
		layout_info.pPushConstantRanges		= _pushConstRanges.RawPtr();

		VK_CHECK( vkCreatePipelineLayout( device, &layout_info, null, OUT &_layoutId ) );

		//GetDevice()->SetObjectName( _layoutId, GetDebugName(), EGpuObject::PipelineLayout );	// TODO
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
			using namespace vk;

			ASSERT( tex.dimension != EImage::Buffer );	// type must be VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER or VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER

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
			using namespace vk;
			
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
			using namespace vk;
			
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
			using namespace vk;
			
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
			using namespace vk;
			
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
			using namespace vk;

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

			// TODO: VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT
		}
	};
	
/*
=================================================
	Destroy
=================================================
*/
	void Vk1PipelineLayout::Destroy (vk::VkDevice device)
	{
		_DestroyLayout( device );
		_DestroyLayoutDescriptors( device );

		_pushConstRanges.Clear();
	}
	
/*
=================================================
	_CreateLayoutDescriptors
=================================================
*/
	bool Vk1PipelineLayout::_CreateLayoutDescriptors (const PipelineLayoutDescriptor &descr, vk::VkDevice device)
	{
		using namespace vk;

		DescriptorBindings	descr_binding;

		CHECK_ERR( descr.GetUniforms().Count() <= DescriptorBindings::MemoryContainer_t::SIZE );

		for (usize set = 0; set <= descr.MaxDescriptorSet(); ++set)
		{
			_CreateDescriptor_Func	func( OUT descr_binding, OUT _pushConstRanges, set );

			FOR( i, descr.GetUniforms() ) {
				descr.GetUniforms()[i].Apply( func );
			}

			CHECK_ERR( not descr_binding.Empty() );	// empty descriptor set is not allowed

			VkDescriptorSetLayoutCreateInfo	descriptor_info		= {};
			VkDescriptorSetLayout			descriptor_layout	= VK_NULL_HANDLE;

			descriptor_info.sType			= VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
			descriptor_info.pBindings		= descr_binding.RawPtr();
			descriptor_info.bindingCount	= (uint) descr_binding.Count();

			VK_CHECK( vkCreateDescriptorSetLayout (device, &descriptor_info, null, OUT &descriptor_layout ) );
			_descriptorIds.PushBack( descriptor_layout );
		}
		return true;
	}
	
/*
=================================================
	_DestroyLayoutDescriptors
=================================================
*/
	void Vk1PipelineLayout::_DestroyLayoutDescriptors (vk::VkDevice device)
	{
		using namespace vk;

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
	void Vk1PipelineLayout::_DestroyLayout (vk::VkDevice device)
	{
		using namespace vk;

		if ( _layoutId != VK_NULL_HANDLE )
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
	Vk1PipelineLayoutCache::Vk1PipelineLayoutCache (VkSystemsRef vkSys) :
		_vkSystems( vkSys )
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

		vk::VkDevice			device	= VkSystems()->Get< Vk1Device >()->GetLogicalDevice();
		Vk1PipelineLayoutPtr	res		= new Vk1PipelineLayout();
		LayoutIter_t			iter;

		if ( _layouts.CustomSearch().Find( LayoutSearch(descr), OUT iter ) ) {
			return iter->ptr;
		}

		CHECK_ERR( res->Create( descr, device ) );
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
		vk::VkDevice	device = VkSystems()->Get< Vk1Device >()->GetLogicalDevice();

		FOR( i, _layouts )
		{
			_layouts[i].ptr->Destroy( device );
		}
		_layouts.Clear();
	}

}	// PlatformVK
}	// Engine

#endif	// GRAPHICS_API_VULKAN
