// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Engine/Platforms/Shared/GPU/Image.h"
#include "Engine/Platforms/Shared/GPU/Buffer.h"
#include "Engine/Platforms/Shared/GPU/Pipeline.h"
#include "Engine/Platforms/Vulkan/Impl/Vk1BaseModule.h"
#include "Engine/Platforms/Vulkan/VulkanContext.h"

#if defined( GRAPHICS_API_VULKAN )

namespace Engine
{
namespace PlatformVK
{

	//
	// Vulkan Pipeline Resource Table (DescriptorSet)
	//

	class Vk1PipelineResourceTable final : public Vk1BaseModule
	{
	// types
	private:
		using SupportedMessages_t	= Vk1BaseModule::SupportedMessages_t::Append< MessageListFrom<
											GpuMsg::GetPipelineLayoutDescriptor,
											GpuMsg::GetVkDescriptorLayouts,
											GpuMsg::GetVkPipelineLayoutID,
											GpuMsg::GetVkPipelineResourceTableID
										> >;

		using SupportedEvents_t		= Vk1BaseModule::SupportedEvents_t;

		struct BaseDescr
		{
			uint						binding			= ~0u;
			vk::VkDescriptorType		descriptorType	= vk::VK_DESCRIPTOR_TYPE_MAX_ENUM;
		};

		struct ImageDescr : BaseDescr
		{
			vk::VkDescriptorImageInfo	info;
		};

		struct BufferDescr : BaseDescr
		{
			vk::VkDescriptorBufferInfo	info;
		};

		struct TextureBufferDescr : BaseDescr
		{
			vk::VkBufferView			view;
		};

		using DescriptorPoolSizes_t	= FixedSizeArray< vk::VkDescriptorPoolSize, vk::VK_DESCRIPTOR_TYPE_RANGE_SIZE >;
		using LayoutMsgList_t		= MessageListFrom< GpuMsg::GetVkPipelineLayoutID, GpuMsg::GetPipelineLayoutDescriptor,
														GpuMsg::GetVkDescriptorLayouts >;

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
		vk::VkDescriptorPool	_descriptorPoolId;
		vk::VkDescriptorSet		_descriptorSetId;
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
		bool _GetVkPipelineLayoutID (const Message< GpuMsg::GetVkPipelineLayoutID > &);
		bool _GetVkDescriptorLayouts (const Message< GpuMsg::GetVkDescriptorLayouts > &);
		bool _GetPipelineLayoutDescriptor (const Message< GpuMsg::GetPipelineLayoutDescriptor > &);
		bool _GetVkPipelineResourceTableID (const Message< GpuMsg::GetVkPipelineResourceTableID > &);
		bool _AttachModule (const Message< ModuleMsg::AttachModule > &);
		bool _DetachModule (const Message< ModuleMsg::DetachModule > &);

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
		Vk1BaseModule( gs, ModuleConfig{ VkPipelineResourceTableModuleID, ~0u }, &_msgTypes, &_eventTypes ),
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
		_SubscribeOnMsg( this, &Vk1PipelineResourceTable::_DeviceBeforeDestroy );
		_SubscribeOnMsg( this, &Vk1PipelineResourceTable::_GetDeviceInfo );
		_SubscribeOnMsg( this, &Vk1PipelineResourceTable::_GetVkDeviceInfo );
		_SubscribeOnMsg( this, &Vk1PipelineResourceTable::_GetVkPrivateClasses );
		_SubscribeOnMsg( this, &Vk1PipelineResourceTable::_GetVkPipelineLayoutID );
		_SubscribeOnMsg( this, &Vk1PipelineResourceTable::_GetVkDescriptorLayouts );
		_SubscribeOnMsg( this, &Vk1PipelineResourceTable::_GetPipelineLayoutDescriptor );
		_SubscribeOnMsg( this, &Vk1PipelineResourceTable::_GetVkPipelineResourceTableID );

		CHECK( _ValidateMsgSubscriptions() );

		_AttachSelfToManager( ci.gpuThread, VkThreadModuleID, true );
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
		
		// very paranoic check
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
	_GetVkDescriptorLayouts
=================================================
*/
	bool Vk1PipelineResourceTable::_GetVkDescriptorLayouts (const Message< GpuMsg::GetVkDescriptorLayouts > &msg)
	{
		return _layout ? SendTo( _layout, msg ) : false;
	}
	
/*
=================================================
	_GetPipelineLayoutDescriptor
=================================================
*/
	bool Vk1PipelineResourceTable::_GetPipelineLayoutDescriptor (const Message< GpuMsg::GetPipelineLayoutDescriptor > &msg)
	{
		return _layout ? SendTo( _layout, msg ) : false;
	}
	
/*
=================================================
	_GetVkPipelineLayoutID
=================================================
*/
	bool Vk1PipelineResourceTable::_GetVkPipelineLayoutID (const Message< GpuMsg::GetVkPipelineLayoutID > &msg)
	{
		return _layout ? SendTo( _layout, msg ) : false;
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
		using Uniform			= PipelineLayoutDescriptor::Uniform;
		using ImageMsgList		= MessageListFrom< GpuMsg::GetVkImageID >;
		using SamplerMsgList	= MessageListFrom< GpuMsg::GetVkSamplerID >;
		using BufferMsgList		= MessageListFrom< GpuMsg::GetVkBufferID >;


	// variables
		Vk1PipelineResourceTable&	self;
		ResourceDescrArray_t&		resources;
		DescriptorPoolSizes_t&		poolSizes;
		const uint					set;


	// methods
		_CreateResourceDescriptor_Func (OUT ResourceDescrArray_t &resources, OUT DescriptorPoolSizes_t &poolSizes, Vk1PipelineResourceTable& self, uint set) :
			self( self ), resources( resources ),
			poolSizes( poolSizes ),	set( set )
		{}
		
		void operator () (const PushConstant &pc) const
		{
			WARNING( "not supported" );
		}
		
		void operator () (const SubpassInput &sp) const
		{
			WARNING( "not supported" );
		}
		
		void operator () (const Uniform &un) const
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
			if ( set != tex.descriptorSet )
				return false;

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

				ImageDescriptor	tex_descr;	tex_descr << req_img_descr->result;
				CHECK( tex_descr.imageType == tex.textureType );
				CHECK( EPixelFormatClass::StrongComparison( tex.format, EPixelFormatClass::From( tex_descr.format ) ) );

				// TODO: use default sampler
				// TODO: check format

				ImageDescr				descr;
				descr.info.sampler		<< req_sampler->result;
				descr.info.imageView	<< req_image->defaultView;
				descr.info.imageLayout	= vk::VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
				descr.descriptorType	= vk::VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
				descr.binding			= tex.binding;
				
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
			if ( set != samp.descriptorSet )
				return false;

			RETURN_ERR( "unsupported!" );
		}
		
/*
=================================================
	operator (ImageUniform)
=================================================
*/
		bool operator () (const ImageUniform &img) const
		{
			if ( set != img.descriptorSet )
				return false;
			
			ModulePtr	img_mod;
			CHECK_ERR( FindModule< ImageMsgList >( img.name, OUT img_mod ) );

			Message< GpuMsg::GetVkImageID >	req_image;
			self.SendTo( img_mod, req_image );	// TODO: check result

			// TODO: check format

			ImageDescr				descr;
			descr.info.sampler		= VK_NULL_HANDLE;
			descr.info.imageView	<< req_image->defaultView;
			descr.info.imageLayout	= vk::VK_IMAGE_LAYOUT_GENERAL;				// TODO
			descr.descriptorType	= vk::VK_DESCRIPTOR_TYPE_STORAGE_IMAGE;
			descr.binding			= img.binding;
			
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
			if ( set != buf.descriptorSet )
				return false;
			
			ModulePtr	buf_mod;
			CHECK_ERR( FindModule< BufferMsgList >( buf.name, OUT buf_mod ) );

			Message< GpuMsg::GetVkBufferID >		req_buffer;
			Message< GpuMsg::GetBufferDescriptor >	req_descr;

			// TODO: check result
			self.SendTo( buf_mod, req_buffer );
			self.SendTo( buf_mod, req_descr );

			CHECK( req_descr->result.Get().size == buf.size );

			BufferDescr				descr;
			descr.info.buffer		<< req_buffer->result;
			descr.info.offset		= 0;
			descr.info.range		= (uint) buf.size;
			descr.descriptorType	= vk::VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
			descr.binding			= buf.binding;

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
			if ( set != buf.descriptorSet )
				return false;
			
			ModulePtr	buf_mod;
			CHECK_ERR( FindModule< BufferMsgList >( buf.name, OUT buf_mod ) );

			Message< GpuMsg::GetVkBufferID >		req_buffer;
			Message< GpuMsg::GetBufferDescriptor >	req_descr;

			// TODO: check result
			self.SendTo( buf_mod, req_buffer );
			self.SendTo( buf_mod, req_descr );

			BufferDescriptor	buf_descr;	buf_descr << req_descr->result;

			CHECK( (buf_descr.size >= buf.staticSize) and
					(buf.arrayStride == 0 or
					(buf_descr.size - buf.staticSize) % buf.arrayStride == 0) );

			BufferDescr				descr;
			descr.info.buffer		<< req_buffer->result;
			descr.info.offset		= 0;
			descr.info.range		= (uint) buf_descr.size;
			descr.descriptorType	= vk::VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
			descr.binding			= buf.binding;
			
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
		using namespace vk;

		Message< GpuMsg::GetPipelineLayoutDescriptor >	req_descr;
		Message< GpuMsg::GetVkDescriptorLayouts >		req_layouts;

		SendTo( _layout, req_descr );
		SendTo( _layout, req_layouts );

		PipelineLayoutDescriptor			layout_descr;	layout_descr << req_descr->result;
		GpuMsg::GetVkDescriptorLayouts::IDs	descr_layouts;	descr_layouts << req_layouts->result;
		DescriptorPoolSizes_t				pool_sizes;
		_CreateResourceDescriptor_Func		func( OUT _resources, OUT pool_sizes, *this, 0 );

		// init pool sizes
		pool_sizes.Resize( vk::VK_DESCRIPTOR_TYPE_RANGE_SIZE );
		FOR( i, pool_sizes ) {
			pool_sizes[i] = { vk::VkDescriptorType( vk::VK_DESCRIPTOR_TYPE_BEGIN_RANGE+i ), 0 };
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

		ASSERT( descr_layouts.Count() == 1 );	// TODO: only 1 set are supported yet

		// create descriptor
		VkDescriptorSetAllocateInfo		alloc_info = {};
		alloc_info.sType				= VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
		alloc_info.descriptorPool		= _descriptorPoolId;
		alloc_info.pSetLayouts			= descr_layouts.RawPtr();
		alloc_info.descriptorSetCount	= (uint) descr_layouts.Count();

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
		Array< vk::VkWriteDescriptorSet >&	writeDescr;
		vk::VkDescriptorSet					descriptorSet;

	// methods
		_WriteDescriptor_Func (Array<vk::VkWriteDescriptorSet> &writeDescr, vk::VkDescriptorSet descriptorSet) :
			writeDescr( writeDescr ), descriptorSet( descriptorSet )
		{}


		void operator () (const ImageDescr &descr) const
		{
			vk::VkWriteDescriptorSet	wds = {};
			wds.sType			= vk::VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
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
			vk::VkWriteDescriptorSet	wds = {};
			wds.sType			= vk::VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
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
			vk::VkWriteDescriptorSet	wds = {};
			wds.sType			= vk::VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
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
		using namespace vk;

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
		using namespace vk;

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
	ModulePtr VulkanContext::_CreateVk1PipelineResourceTable (GlobalSystemsRef gs, const CreateInfo::PipelineResourceTable &ci)
	{
		return New< PlatformVK::Vk1PipelineResourceTable >( gs, ci );
	}
}	// Platforms
}	// Engine

#endif	// GRAPHICS_API_VULKAN
