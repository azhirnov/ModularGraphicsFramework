// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Engine/Config/Engine.Config.h"

#ifdef GRAPHICS_API_VULKAN

#include "Engine/Platforms/Public/GPU/Image.h"
#include "Engine/Platforms/Public/GPU/Buffer.h"
#include "Engine/Platforms/Public/GPU/Pipeline.h"
#include "Engine/Platforms/Vulkan/Impl/Vk1BaseModule.h"
#include "Engine/Platforms/Vulkan/VulkanObjectsConstructor.h"

namespace Engine
{
namespace PlatformVK
{
	using namespace vk;



	//
	// Vulkan Pipeline Resource Table (DescriptorSet)
	//

	class Vk1PipelineResourceTable final : public Vk1BaseModule
	{
	// types
	private:
		using LayoutMsgList_t		= MessageListFrom<
											GpuMsg::GetVkPipelineLayoutID,
											GpuMsg::GetVkDescriptorLayouts,
											GpuMsg::GetPipelineLayoutDescriptor,
											GpuMsg::GetVkPipelineLayoutPushConstants
										>;

		using SupportedMessages_t	= Vk1BaseModule::SupportedMessages_t::Append< MessageListFrom<
											GpuMsg::GetVkPipelineResourceTableID,
											GpuMsg::PipelineAttachBuffer,
											GpuMsg::PipelineAttachImage
										> >::Append< LayoutMsgList_t >;

		using SupportedEvents_t		= Vk1BaseModule::SupportedEvents_t;

		struct BaseDescr
		{
			uint					binding			= UMax;
			VkDescriptorType		descriptorType	= VK_DESCRIPTOR_TYPE_MAX_ENUM;
		};

		struct ImageDescr final : BaseDescr
		{
			VkDescriptorImageInfo	info;
		};

		struct BufferDescr final : BaseDescr
		{
			VkDescriptorBufferInfo	info;
		};

		struct TextureBufferDescr final : BaseDescr
		{
			VkBufferView			view;
		};
		
		struct BufferRange
		{
			BytesUL		offset;
			BytesUL		size;
		};

		using DescriptorPoolSizes_t	= FixedSizeArray< VkDescriptorPoolSize, VK_DESCRIPTOR_TYPE_RANGE_SIZE >;
		
		using ResourceCache_t		= Union< ImageViewDescriptor, BufferRange >;
		using CachedResources_t		= Array< ResourceCache_t >;

		using ResourceDescr_t		= Union< ImageDescr, BufferDescr, TextureBufferDescr >;
		using ResourceDescrArray_t	= Array< ResourceDescr_t >;
		
		struct _CreateResourceDescriptor_Func;
		struct _WriteDescriptor_Func;


	// constants
	private:
		static const TypeIdList		_msgTypes;
		static const TypeIdList		_eventTypes;


	// variables
	private:
		VkDescriptorPool		_descriptorPoolId;
		VkDescriptorSet			_descriptorSetId;
		ResourceDescrArray_t	_resources;
		ModulePtr				_layout;


	// methods
	public:
		Vk1PipelineResourceTable (GlobalSystemsRef gs, const CreateInfo::PipelineResourceTable &ci);
		~Vk1PipelineResourceTable ();


	// message handlers
	private:
		bool _Link (const Message< ModuleMsg::Link > &);
		bool _Compose (const Message< ModuleMsg::Compose > &);
		bool _Delete (const Message< ModuleMsg::Delete > &);
		bool _AttachModule (const Message< ModuleMsg::AttachModule > &);
		bool _DetachModule (const Message< ModuleMsg::DetachModule > &);
		bool _PipelineAttachImage (const Message< GpuMsg::PipelineAttachImage > &);
		bool _PipelineAttachBuffer (const Message< GpuMsg::PipelineAttachBuffer > &);
		bool _GetVkPipelineResourceTableID (const Message< GpuMsg::GetVkPipelineResourceTableID > &);

	private:
		bool _IsCreated ();
		bool _CreateResourceTable ();
		bool _UpdateResourceTable ();
		void _DestroyResourceTable ();
	};
//-----------------------------------------------------------------------------


	
	const TypeIdList	Vk1PipelineResourceTable::_msgTypes{ UninitializedT< SupportedMessages_t >() };
	const TypeIdList	Vk1PipelineResourceTable::_eventTypes{ UninitializedT< SupportedEvents_t >() };

/*
=================================================
	constructor
=================================================
*/
	Vk1PipelineResourceTable::Vk1PipelineResourceTable (GlobalSystemsRef gs, const CreateInfo::PipelineResourceTable &ci) :
		Vk1BaseModule( gs, ModuleConfig{ VkPipelineResourceTableModuleID, UMax }, &_msgTypes, &_eventTypes ),
		_descriptorPoolId( VK_NULL_HANDLE ),
		_descriptorSetId( VK_NULL_HANDLE )
	{
		SetDebugName( "Vk1PipelineResourceTable" );

		_SubscribeOnMsg( this, &Vk1PipelineResourceTable::_OnModuleAttached_Impl );
		_SubscribeOnMsg( this, &Vk1PipelineResourceTable::_OnModuleDetached_Impl );
		_SubscribeOnMsg( this, &Vk1PipelineResourceTable::_AttachModule );
		_SubscribeOnMsg( this, &Vk1PipelineResourceTable::_DetachModule );
		_SubscribeOnMsg( this, &Vk1PipelineResourceTable::_FindModule_Impl );
		_SubscribeOnMsg( this, &Vk1PipelineResourceTable::_ModulesDeepSearch_Impl );
		_SubscribeOnMsg( this, &Vk1PipelineResourceTable::_Link );
		_SubscribeOnMsg( this, &Vk1PipelineResourceTable::_Compose );
		_SubscribeOnMsg( this, &Vk1PipelineResourceTable::_Delete );
		_SubscribeOnMsg( this, &Vk1PipelineResourceTable::_OnManagerChanged );
		_SubscribeOnMsg( this, &Vk1PipelineResourceTable::_GetDeviceInfo );
		_SubscribeOnMsg( this, &Vk1PipelineResourceTable::_GetVkDeviceInfo );
		_SubscribeOnMsg( this, &Vk1PipelineResourceTable::_GetVkPrivateClasses );
		_SubscribeOnMsg( this, &Vk1PipelineResourceTable::_PipelineAttachImage );
		_SubscribeOnMsg( this, &Vk1PipelineResourceTable::_PipelineAttachBuffer );
		_SubscribeOnMsg( this, &Vk1PipelineResourceTable::_GetVkPipelineResourceTableID );

		_AttachSelfToManager( _GetGPUThread( ci.gpuThread ), UntypedID_t(0), true );
	}
	
/*
=================================================
	destructor
=================================================
*/
	Vk1PipelineResourceTable::~Vk1PipelineResourceTable ()
	{
		ASSERT( not _IsCreated() );
	}
	
/*
=================================================
	_Link
=================================================
*/
	bool Vk1PipelineResourceTable::_Link (const Message< ModuleMsg::Link > &)
	{
		if ( _IsComposedOrLinkedState( GetState() ) )
			return true;	// already linked

		CHECK_ERR( GetState() == EState::Initial or GetState() == EState::LinkingFailed );

		CHECK_ATTACHMENT( _layout = GetModuleByMsg< LayoutMsgList_t >() );
		CHECK_ERR( _CopySubscriptions< LayoutMsgList_t >( _layout ) );

		CHECK( _SetState( EState::Linked ) );
		return true;
	}

/*
=================================================
	_Compose
=================================================
*/
	bool Vk1PipelineResourceTable::_Compose (const Message< ModuleMsg::Compose > &msg)
	{
		if ( _IsComposedState( GetState() ) )
			return true;	// already composed

		CHECK_ERR( GetState() == EState::Linked );
		
		CHECK_COMPOSING( _layout );
		CHECK_COMPOSING( _CreateResourceTable() and _UpdateResourceTable() );

		_SendForEachAttachments( msg );
		
		CHECK( _ValidateAllSubscriptions() );

		CHECK( _SetState( EState::ComposedMutable ) );
		return true;
	}

/*
=================================================
	_Delete
=================================================
*/
	bool Vk1PipelineResourceTable::_Delete (const Message< ModuleMsg::Delete > &msg)
	{
		_SendForEachAttachments( msg );

		_DestroyResourceTable();

		return Module::_Delete_Impl( msg );
	}

/*
=================================================
	_GetVkPipelineResourceTableID
=================================================
*/
	bool Vk1PipelineResourceTable::_GetVkPipelineResourceTableID (const Message< GpuMsg::GetVkPipelineResourceTableID > &msg)
	{
		msg->result.Set( _descriptorSetId );
		return true;
	}
	
/*
=================================================
	_AttachModule
=================================================
*/
	bool Vk1PipelineResourceTable::_AttachModule (const Message< ModuleMsg::AttachModule > &msg)
	{
		CHECK_ERR( msg->newModule );

		// pipeline layout must be unique
		bool	is_layout = msg->newModule->GetSupportedMessages().HasAllTypes< LayoutMsgList_t >();

		CHECK( _Attach( msg->name, msg->newModule, is_layout ) );
		CHECK( _SetState( EState::Initial ) );

		_DestroyResourceTable();
		return true;
	}
	
/*
=================================================
	_AttachModule
=================================================
*/
	bool Vk1PipelineResourceTable::_PipelineAttachImage (const Message< GpuMsg::PipelineAttachImage > &msg)
	{
		return true;
	}
	
/*
=================================================
	_AttachModule
=================================================
*/
	bool Vk1PipelineResourceTable::_PipelineAttachBuffer (const Message< GpuMsg::PipelineAttachBuffer > &msg)
	{
		return true;
	}

/*
=================================================
	_DetachModule
=================================================
*/
	bool Vk1PipelineResourceTable::_DetachModule (const Message< ModuleMsg::DetachModule > &msg)
	{
		if ( _Detach( msg->oldModule ) )
		{
			CHECK( _SetState( EState::Initial ) );
			_DestroyResourceTable();
		}
		return true;
	}

/*
=================================================
	_CreateResourceDescriptor_Func
=================================================
*/
	struct Vk1PipelineResourceTable::_CreateResourceDescriptor_Func
	{
	// types
		using TextureUniform	= PipelineLayoutDescriptor::TextureUniform;
		using SamplerUniform	= PipelineLayoutDescriptor::SamplerUniform;
		using ImageUniform		= PipelineLayoutDescriptor::ImageUniform;
		using UniformBuffer		= PipelineLayoutDescriptor::UniformBuffer;
		using StorageBuffer		= PipelineLayoutDescriptor::StorageBuffer;
		using PushConstant		= PipelineLayoutDescriptor::PushConstant;
		using SubpassInput		= PipelineLayoutDescriptor::SubpassInput;
		using ImageMsgList		= MessageListFrom< GpuMsg::GetVkImageID >;
		using SamplerMsgList	= MessageListFrom< GpuMsg::GetVkSamplerID >;
		using BufferMsgList		= MessageListFrom< GpuMsg::GetVkBufferID >;


	// variables
		Vk1PipelineResourceTable&	self;
		ResourceDescrArray_t&		resources;
		DescriptorPoolSizes_t&		poolSizes;


	// methods
		_CreateResourceDescriptor_Func (OUT ResourceDescrArray_t &resources, OUT DescriptorPoolSizes_t &poolSizes, Vk1PipelineResourceTable& self) :
			self( self ), resources( resources ),
			poolSizes( poolSizes )
		{}
		
		void operator () (const PushConstant &pc) const
		{
			WARNING( "not supported" );
		}
		
		void operator () (const SubpassInput &sp) const
		{
			WARNING( "not supported" );
		}
		
/*
=================================================
	FindModule
=================================================
*/
		template <typename MsgList>
		bool FindModule (StringCRef name, OUT ModulePtr &result) const
		{
			FOR( i, self._GetAttachments() )
			{
				const auto&	pair = self._GetAttachments()[i];

				if ( pair.first == name )
				{
					CHECK_ERR( pair.second->GetSupportedMessages().HasAllTypes< MsgList >() );
					result = pair.second;
					return true;
				}
			}
			return false;
		}

/*
=================================================
	operator (TextureUniform)
=================================================
*/
		bool operator () (const TextureUniform &tex) const
		{
			CHECK( tex.textureType != EImage::Buffer );

			// texture buffer
			/*if ( tex.dimension == EImage::Buffer )
			{

				TextureBufferDescr	descr;
				descr.texBuffer	= iter->second;
				//descr.view.
				resources.PushBack( descr );
			}
			else*/
			// texture
			{
				ModulePtr	tex_mod, samp_mod;
				CHECK_ERR( FindModule< ImageMsgList >( tex.name, OUT tex_mod ) );
				CHECK_ERR( FindModule< SamplerMsgList >( String(tex.name) << ".sampler", OUT samp_mod ) );

				Message< GpuMsg::GetVkImageID >			req_image;
				Message< GpuMsg::GetVkSamplerID >		req_sampler;
				Message< GpuMsg::GetImageDescriptor >	req_img_descr;

				// TODO: check result
				self.SendTo( tex_mod, req_image );
				self.SendTo( tex_mod, req_img_descr );
				self.SendTo( samp_mod, req_sampler );

				CHECK( req_img_descr->result->imageType == tex.textureType );
				CHECK( req_img_descr->result->usage[ EImageUsage::Sampled ] );
				CHECK( EPixelFormatClass::StrongComparison( tex.format, EPixelFormatClass::From( req_img_descr->result->format ) ) );

				// TODO: use default sampler
				// TODO: check format

				ImageDescr				descr;
				descr.info.sampler		= req_sampler->result.Get( VK_NULL_HANDLE );
				descr.info.imageView	= req_image->defaultView.Get( VK_NULL_HANDLE );
				descr.info.imageLayout	= VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
				descr.descriptorType	= VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
				descr.binding			= tex.uniqueIndex;
				
				CHECK_ERR(	descr.info.sampler   != VK_NULL_HANDLE and
							descr.info.imageView != VK_NULL_HANDLE );

				resources.PushBack(ResourceDescr_t( descr ));
				poolSizes[ descr.descriptorType ].descriptorCount++;
			}
			return true;
		}
		
/*
=================================================
	operator (SamplerUniform)
=================================================
*/
		bool operator () (const SamplerUniform &samp) const
		{
			RETURN_ERR( "unsupported!" );
		}
		
/*
=================================================
	operator (ImageUniform)
=================================================
*/
		bool operator () (const ImageUniform &img) const
		{
			ModulePtr	img_mod;
			CHECK_ERR( FindModule< ImageMsgList >( img.name, OUT img_mod ) );

			Message< GpuMsg::GetVkImageID >			req_image;
			Message< GpuMsg::GetImageDescriptor >	req_img_descr;

			self.SendTo( img_mod, req_image );	// TODO: check result
			self.SendTo( img_mod, req_img_descr );

			// TODO: check format
			
			CHECK( req_img_descr->result->usage[ EImageUsage::Storage ] );

			ImageDescr				descr;
			descr.info.sampler		= VK_NULL_HANDLE;
			descr.info.imageView	= req_image->defaultView.Get( VK_NULL_HANDLE );
			descr.info.imageLayout	= VK_IMAGE_LAYOUT_GENERAL;
			descr.descriptorType	= VK_DESCRIPTOR_TYPE_STORAGE_IMAGE;
			descr.binding			= img.uniqueIndex;
			
			CHECK_ERR( descr.info.imageView != VK_NULL_HANDLE );

			resources.PushBack(ResourceDescr_t( descr ));
			poolSizes[ descr.descriptorType ].descriptorCount++;
			return true;
		}
		
/*
=================================================
	operator (UniformBuffer)
=================================================
*/
		bool operator () (const UniformBuffer &buf) const
		{
			ModulePtr	buf_mod;
			CHECK_ERR( FindModule< BufferMsgList >( buf.name, OUT buf_mod ) );

			Message< GpuMsg::GetVkBufferID >		req_buffer;
			Message< GpuMsg::GetBufferDescriptor >	req_descr;

			// TODO: check result
			self.SendTo( buf_mod, req_buffer );
			self.SendTo( buf_mod, req_descr );

			CHECK( req_descr->result->size == BytesUL(buf.size) );
			CHECK( req_descr->result->usage[ EBufferUsage::Uniform ] );

			BufferDescr				descr;
			descr.info.buffer		= req_buffer->result.Get( VK_NULL_HANDLE );
			descr.info.offset		= 0;
			descr.info.range		= (VkDeviceSize) buf.size;
			descr.descriptorType	= VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
			descr.binding			= buf.uniqueIndex;

			CHECK_ERR( descr.info.buffer != VK_NULL_HANDLE );
			
			resources.PushBack(ResourceDescr_t( descr ));
			poolSizes[ descr.descriptorType ].descriptorCount++;
			return true;
		}
		
/*
=================================================
	operator (StorageBuffer)
=================================================
*/
		bool operator () (const StorageBuffer &buf) const
		{
			ModulePtr	buf_mod;
			CHECK_ERR( FindModule< BufferMsgList >( buf.name, OUT buf_mod ) );

			Message< GpuMsg::GetVkBufferID >		req_buffer;
			Message< GpuMsg::GetBufferDescriptor >	req_descr;

			// TODO: check result
			self.SendTo( buf_mod, req_buffer );
			self.SendTo( buf_mod, req_descr );

			CHECK( (req_descr->result->size >= buf.staticSize) and
					(buf.arrayStride == 0 or
					(req_descr->result->size - buf.staticSize) % buf.arrayStride == 0) );
			CHECK( req_descr->result->usage[ EBufferUsage::Storage ] );

			BufferDescr				descr;
			descr.info.buffer		= req_buffer->result.Get( VK_NULL_HANDLE );
			descr.info.offset		= 0;
			descr.info.range		= (VkDeviceSize) req_descr->result->size;
			descr.descriptorType	= VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
			descr.binding			= buf.uniqueIndex;
			
			CHECK_ERR( descr.info.buffer != VK_NULL_HANDLE );

			resources.PushBack(ResourceDescr_t( descr ));
			poolSizes[ descr.descriptorType ].descriptorCount++;
			return true;
		}
	};

/*
=================================================
	_CreateResourceTable
=================================================
*/
	bool Vk1PipelineResourceTable::_CreateResourceTable ()
	{
		Message< GpuMsg::GetPipelineLayoutDescriptor >	req_descr;
		Message< GpuMsg::GetVkDescriptorLayouts >		req_layouts;

		SendTo( _layout, req_descr );
		SendTo( _layout, req_layouts );

		PipelineLayoutDescriptor const&		layout_descr = *req_descr->result;
		VkDescriptorSetLayout				descr_layout = *req_layouts->result;
		DescriptorPoolSizes_t				pool_sizes;
		_CreateResourceDescriptor_Func		func( OUT _resources, OUT pool_sizes, *this );

		// init pool sizes
		pool_sizes.Resize( VK_DESCRIPTOR_TYPE_RANGE_SIZE );
		FOR( i, pool_sizes ) {
			pool_sizes[i] = { VkDescriptorType( VK_DESCRIPTOR_TYPE_BEGIN_RANGE+i ), 0 };
		}

		// initialize table
		FOR( i, layout_descr.GetUniforms() ) {
			layout_descr.GetUniforms()[i].Apply( func );
		}

		// remove empty pool sizes
		FOR( i, pool_sizes ) {
			if ( pool_sizes[i].descriptorCount == 0 ) {
				pool_sizes.Erase( i );
				--i;
			}
		}

		// create pool
		VkDescriptorPoolCreateInfo	pool_info = {};
		pool_info.sType				= VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
		pool_info.poolSizeCount		= (uint) pool_sizes.Count();
		pool_info.pPoolSizes		= pool_sizes.RawPtr();
		pool_info.maxSets			= 1;

		VK_CHECK( vkCreateDescriptorPool( GetVkDevice(), &pool_info, null, OUT &_descriptorPoolId ) );
		
		GetDevice()->SetObjectName( ReferenceCast<uint64_t>(_descriptorPoolId), GetDebugName(), EGpuObject::DescriptorPool );

		// create descriptor
		VkDescriptorSetAllocateInfo		alloc_info = {};
		alloc_info.sType				= VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
		alloc_info.descriptorPool		= _descriptorPoolId;
		alloc_info.pSetLayouts			= &descr_layout;
		alloc_info.descriptorSetCount	= 1u;

		VK_CHECK( vkAllocateDescriptorSets( GetVkDevice(), &alloc_info, OUT &_descriptorSetId ) );

		GetDevice()->SetObjectName( ReferenceCast<uint64_t>(_descriptorSetId), GetDebugName(), EGpuObject::DescriptorSet );
		return true;
	}
	
/*
=================================================
	_WriteDescriptor_Func
=================================================
*/
	struct Vk1PipelineResourceTable::_WriteDescriptor_Func
	{
	// variables
		Array< VkWriteDescriptorSet >&	writeDescr;
		VkDescriptorSet					descriptorSet;

	// methods
		_WriteDescriptor_Func (Array<VkWriteDescriptorSet> &writeDescr, VkDescriptorSet descriptorSet) :
			writeDescr( writeDescr ), descriptorSet( descriptorSet )
		{}


		void operator () (const ImageDescr &descr) const
		{
			VkWriteDescriptorSet	wds = {};
			wds.sType			= VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
			wds.descriptorType	= descr.descriptorType;
			wds.descriptorCount	= 1;
			wds.dstArrayElement	= 0;
			wds.dstBinding		= descr.binding;
			wds.dstSet			= descriptorSet;
			wds.pImageInfo		= &descr.info;

			writeDescr.PushBack( wds );
		}
		

		void operator () (const BufferDescr &descr) const
		{
			VkWriteDescriptorSet	wds = {};
			wds.sType			= VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
			wds.descriptorType	= descr.descriptorType;
			wds.descriptorCount	= 1;
			wds.dstArrayElement	= 0;
			wds.dstBinding		= descr.binding;
			wds.dstSet			= descriptorSet;
			wds.pBufferInfo		= &descr.info;

			writeDescr.PushBack( wds );
		}
		

		void operator () (const TextureBufferDescr &descr) const
		{
			VkWriteDescriptorSet	wds = {};
			wds.sType			= VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
			wds.descriptorType	= descr.descriptorType;
			wds.descriptorCount	= 1;
			wds.dstArrayElement	= 0;
			wds.dstBinding		= descr.binding;
			wds.dstSet			= descriptorSet;
			wds.pTexelBufferView= &descr.view;

			writeDescr.PushBack( wds );
		}
	};
	
/*
=================================================
	_UpdateResourceTable
=================================================
*/
	bool Vk1PipelineResourceTable::_UpdateResourceTable ()
	{
		CHECK_ERR( _descriptorSetId != VK_NULL_HANDLE and not _resources.Empty() );

		Array<VkWriteDescriptorSet>	write_descr;
		_WriteDescriptor_Func		func( OUT write_descr, _descriptorSetId );

		FOR( i, _resources ) {
			_resources[i].Apply( func );
		}
		
		vkUpdateDescriptorSets( GetVkDevice(), (uint)write_descr.Count(), write_descr.RawPtr(), 0, null );
		return true;
	}

/*
=================================================
	_DestroyResourceTable
=================================================
*/
	void Vk1PipelineResourceTable::_DestroyResourceTable ()
	{
		auto	dev = GetVkDevice();

		/*if ( dev != VK_NULL_HANDLE and _descriptorSetId != VK_NULL_HANDLE )
		{
			VK_CALL( vkFreeDescriptorSets( dev, _descriptorPoolId, 1, &_descriptorSetId ) );
		}*/

		if ( dev != VK_NULL_HANDLE and _descriptorPoolId != VK_NULL_HANDLE )
		{
			vkDestroyDescriptorPool( dev, _descriptorPoolId, null );
		}

		_descriptorSetId	= VK_NULL_HANDLE;
		_descriptorPoolId	= VK_NULL_HANDLE;
		_layout				= null;

		_resources.Clear();
	}

/*
=================================================
	_IsCreated
=================================================
*/
	bool Vk1PipelineResourceTable::_IsCreated ()
	{
		return _descriptorSetId != VK_NULL_HANDLE and _descriptorPoolId != VK_NULL_HANDLE;
	}

}	// PlatformVK
//-----------------------------------------------------------------------------

namespace Platforms
{
	ModulePtr VulkanObjectsConstructor::CreateVk1PipelineResourceTable (GlobalSystemsRef gs, const CreateInfo::PipelineResourceTable &ci)
	{
		return New< PlatformVK::Vk1PipelineResourceTable >( gs, ci );
	}
}	// Platforms
}	// Engine

#endif	// GRAPHICS_API_VULKAN
