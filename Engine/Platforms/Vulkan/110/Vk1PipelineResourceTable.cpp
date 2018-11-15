// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Core/Config/Engine.Config.h"

#ifdef GRAPHICS_API_VULKAN

#include "Engine/Platforms/Public/GPU/Image.h"
#include "Engine/Platforms/Public/GPU/Buffer.h"
#include "Engine/Platforms/Public/GPU/Pipeline.h"
#include "Engine/Platforms/Vulkan/110/Vk1BaseModule.h"
#include "Engine/Platforms/Vulkan/110/Vk1ResourceCache.h"
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
											GpuMsg::GetVkDescriptionLayouts,
											GpuMsg::GetPipelineLayoutDescription,
											GpuMsg::GetVkPipelineLayoutPushConstants
										>;

		using SupportedMessages_t	= Vk1BaseModule::SupportedMessages_t::Append< MessageListFrom<
											GpuMsg::GetVkPipelineResourceTableID,
											GpuMsg::PipelineAttachBuffer,
											GpuMsg::PipelineAttachImage,
											GpuMsg::PipelineAttachTexture
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
			BytesU		offset;
			BytesU		size;
		};

		using ResourceDescr_t		= Union< ImageDescr, BufferDescr, TextureBufferDescr >;
		using ResourceDescrArray_t	= Array< ResourceDescr_t >;

		struct BufferAttachment
		{
			BytesU		offset;
			BytesU		size;
		};

		struct ImageAttachment
		{
			ImageViewDescription	descr;
			EImageLayout::type		layout	= Uninitialized;
		};

		using AttachmentInfo_t		= Union< BufferAttachment, ImageAttachment >;
		using AttachmentInfoMap_t	= Map< UntypedKey<ModulePtr>, AttachmentInfo_t >;

		using DescriptorPoolSizes_t	= FixedSizeArray< VkDescriptorPoolSize, VK_DESCRIPTOR_TYPE_RANGE_SIZE >;
		
		struct _CreateResourceDescription_Func;
		struct _WriteDescription_Func;


	// constants
	private:
		static const TypeIdList		_eventTypes;


	// variables
	private:
		VkDescriptorPool		_descriptorPoolId;
		VkDescriptorSet			_descriptorSetId;
		AttachmentInfoMap_t		_attachmentInfo;
		ResourceDescrArray_t	_resources;
		ModulePtr				_layout;


	// methods
	public:
		Vk1PipelineResourceTable (UntypedID_t, GlobalSystemsRef gs, const CreateInfo::PipelineResourceTable &ci);
		~Vk1PipelineResourceTable ();


	// message handlers
	private:
		bool _Link (const ModuleMsg::Link &);
		bool _Compose (const ModuleMsg::Compose &);
		bool _Delete (const ModuleMsg::Delete &);
		bool _AttachModule (const ModuleMsg::AttachModule &);
		bool _DetachModule (const ModuleMsg::DetachModule &);
		bool _PipelineAttachImage (const GpuMsg::PipelineAttachImage &);
		bool _PipelineAttachBuffer (const GpuMsg::PipelineAttachBuffer &);
		bool _PipelineAttachTexture (const GpuMsg::PipelineAttachTexture &);
		bool _GetVkPipelineResourceTableID (const GpuMsg::GetVkPipelineResourceTableID &);

	private:
		bool _IsCreated ();
		bool _CreateResourceTable ();
		bool _UpdateResourceTable ();
		void _DestroyResourceTable ();
	};
//-----------------------------------------------------------------------------


	
	const TypeIdList	Vk1PipelineResourceTable::_eventTypes{ UninitializedT< SupportedEvents_t >() };

/*
=================================================
	constructor
=================================================
*/
	Vk1PipelineResourceTable::Vk1PipelineResourceTable (UntypedID_t id, GlobalSystemsRef gs, const CreateInfo::PipelineResourceTable &ci) :
		Vk1BaseModule( gs, ModuleConfig{ id, UMax, true }, &_eventTypes ),
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
		_SubscribeOnMsg( this, &Vk1PipelineResourceTable::_PipelineAttachTexture );
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
	bool Vk1PipelineResourceTable::_Link (const ModuleMsg::Link &)
	{
		if ( _IsComposedOrLinkedState( GetState() ) )
			return true;	// already linked

		CHECK_ERR( _IsInitialState( GetState() ) );

		CHECK_ATTACHMENT( _layout = GetModuleByMsg< LayoutMsgList_t >() );
		CHECK_ERR( _CopySubscriptions< LayoutMsgList_t >( _layout ) );

		CHECK( _SetState( EState::Linked ) );
		
		_SendUncheckedEvent( ModuleMsg::AfterLink{} );
		return true;
	}

/*
=================================================
	_Compose
=================================================
*/
	bool Vk1PipelineResourceTable::_Compose (const ModuleMsg::Compose &msg)
	{
		if ( _IsComposedState( GetState() ) )
			return true;	// already composed

		CHECK_ERR( GetState() == EState::Linked );
		
		CHECK_COMPOSING( _layout );
		CHECK_COMPOSING( _CreateResourceTable() and _UpdateResourceTable() );

		_SendForEachAttachments( msg );
		
		CHECK( _ValidateAllSubscriptions() );

		CHECK( _SetState( EState::ComposedMutable ) );
		
		_SendUncheckedEvent( ModuleMsg::AfterCompose{} );
		return true;
	}

/*
=================================================
	_Delete
=================================================
*/
	bool Vk1PipelineResourceTable::_Delete (const ModuleMsg::Delete &msg)
	{
		_SendForEachAttachments( msg );

		_DestroyResourceTable();

		_attachmentInfo.Clear();

		return Module::_Delete_Impl( msg );
	}

/*
=================================================
	_GetVkPipelineResourceTableID
=================================================
*/
	bool Vk1PipelineResourceTable::_GetVkPipelineResourceTableID (const GpuMsg::GetVkPipelineResourceTableID &msg)
	{
		ASSERT( _IsCreated() );

		msg.result.Set( _descriptorSetId );
		return true;
	}
	
/*
=================================================
	_AttachModule
=================================================
*/
	bool Vk1PipelineResourceTable::_AttachModule (const ModuleMsg::AttachModule &msg)
	{
		CHECK_ERR( msg.newModule );

		CHECK( _Attach( msg.name, msg.newModule ) );
		CHECK( _SetState( EState::Initial ) );

		_DestroyResourceTable();
		return true;
	}
	
/*
=================================================
	_PipelineAttachImage
=================================================
*/
	bool Vk1PipelineResourceTable::_PipelineAttachImage (const GpuMsg::PipelineAttachImage &msg)
	{
		CHECK_ERR( msg.newModule );

		ImageAttachment	img;
		img.descr	= msg.descr;
		img.layout	= msg.layout;

		_attachmentInfo.Add( msg.newModule.RawPtr(), AttachmentInfo_t{img} );
		
		CHECK( _Attach( msg.name, msg.newModule ) );
		CHECK( _SetState( EState::Initial ) );

		return true;
	}
	
/*
=================================================
	_PipelineAttachTexture
=================================================
*/
	bool Vk1PipelineResourceTable::_PipelineAttachTexture (const GpuMsg::PipelineAttachTexture &msg)
	{
		CHECK_ERR( msg.newModule and msg.sampler );

		ImageAttachment	img;
		img.layout	= msg.layout;
		
		if ( not msg.descr.IsDefined() )
		{
			GpuMsg::GetImageDescription	req_descr;
			CHECK( msg.newModule->Send( req_descr ) );

			img.descr = ImageViewDescription{ *req_descr.result };
		}
		else
			img.descr = *msg.descr;

		_attachmentInfo.Add( msg.newModule.RawPtr(), AttachmentInfo_t{img} );
		
		CHECK( _Attach( msg.name, msg.newModule ) );
		CHECK( _Attach( msg.name + ".sampler", msg.sampler ) );
		CHECK( _SetState( EState::Initial ) );

		return true;
	}

/*
=================================================
	_PipelineAttachBuffer
=================================================
*/
	bool Vk1PipelineResourceTable::_PipelineAttachBuffer (const GpuMsg::PipelineAttachBuffer &msg)
	{
		CHECK_ERR( msg.newModule );

		BufferAttachment	buf;
		buf.offset	= msg.offset;
		buf.size	= msg.size;

		_attachmentInfo.Add( msg.newModule.RawPtr(), AttachmentInfo_t{buf} );
		
		CHECK( _Attach( msg.name, msg.newModule ) );
		CHECK( _SetState( EState::Initial ) );

		return true;
	}

/*
=================================================
	_DetachModule
=================================================
*/
	bool Vk1PipelineResourceTable::_DetachModule (const ModuleMsg::DetachModule &msg)
	{
		if ( _Detach( msg.oldModule ) )
		{
			_attachmentInfo.Erase( msg.oldModule.RawPtr() );
			CHECK( _SetState( EState::Initial ) );
			_DestroyResourceTable();
		}
		return true;
	}

/*
=================================================
	_CreateResourceDescription_Func
=================================================
*/
	struct Vk1PipelineResourceTable::_CreateResourceDescription_Func
	{
	// types
		using TextureUniform	= PipelineLayoutDescription::TextureUniform;
		using SamplerUniform	= PipelineLayoutDescription::SamplerUniform;
		using ImageUniform		= PipelineLayoutDescription::ImageUniform;
		using UniformBuffer		= PipelineLayoutDescription::UniformBuffer;
		using StorageBuffer		= PipelineLayoutDescription::StorageBuffer;
		using PushConstant		= PipelineLayoutDescription::PushConstant;
		using SubpassInput		= PipelineLayoutDescription::SubpassInput;
		using ImageMsgList		= MessageListFrom< GpuMsg::GetVkImageID >;
		using SamplerMsgList	= MessageListFrom< GpuMsg::GetVkSamplerID >;
		using BufferMsgList		= MessageListFrom< GpuMsg::GetVkBufferID >;
		using ResourceMsgList	= CompileTime::TypeListFrom< ImageMsgList, SamplerMsgList, BufferMsgList >;
		using ModuleMap			= HashMap< ModuleName_t, ModulePtr >;


	// variables
		Vk1PipelineResourceTable&	self;
		ResourceDescrArray_t&		resources;
		DescriptorPoolSizes_t&		poolSizes;
		mutable ModuleMap			moduleMap;


	// methods
		_CreateResourceDescription_Func (OUT ResourceDescrArray_t &resources, OUT DescriptorPoolSizes_t &poolSizes, Vk1PipelineResourceTable& self) :
			self( self ), resources( resources ), poolSizes( poolSizes )
		{
			moduleMap.Reserve( self._GetAttachments().Count() );

			for (const auto& mod : self._GetAttachments())
			{
				if ( mod.second->SupportsAnyMessage< ResourceMsgList >() )
					moduleMap.Add( mod.first, mod.second );
			}
		}

		~_CreateResourceDescription_Func ()
		{
			DEBUG_ONLY(
				for (auto& mod : moduleMap) {
					LOG( "Unused module in resource table: "_str << mod.first, ELog::Warning );
				}
			)
		}
		
		void operator () (const PushConstant &) const
		{
			WARNING( "not supported" );
		}
		
		void operator () (const SubpassInput &) const
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
			ModuleMap::const_iterator	iter;
			
			if ( moduleMap.Find( name, OUT iter ) )
			{
				CHECK_ERR( iter->second->SupportsAllMessages< MsgList >() );
				result = iter->second;
				
				DEBUG_ONLY( moduleMap.EraseByIter( iter ) );
				return true;
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
				
				GpuMsg::GetVkSamplerID	req_sampler;
				samp_mod->Send( req_sampler );

				const auto&	img_res = self.GetResourceCache()->GetImageID( tex_mod );	// warning: reference may be invalid after any changes

				CHECK( img_res.Get<1>().imageType == tex.textureType );
				CHECK( img_res.Get<1>().usage[ EImageUsage::Sampled ] );
				CHECK( EPixelFormatClass::StrongComparison( tex.format, EPixelFormatClass::From( img_res.Get<1>().format ) ) );

				// TODO: use default sampler
				// TODO: check format
				
				// find attachment info
				VkImageView						img_view = img_res.Get<0>().defaultView;
				VkImageLayout					layout	 = VK_IMAGE_LAYOUT_GENERAL;
				AttachmentInfoMap_t::iterator	info;

				if ( self._attachmentInfo.Find( tex_mod.RawPtr(), OUT info ) )
				{
					auto const&					att_info = info->second.Get<ImageAttachment>();
					GpuMsg::CreateVkImageView	req_imageview{ att_info.descr };
					tex_mod->Send( req_imageview );

					img_view	= req_imageview.result.Get( VK_NULL_HANDLE );
					layout		= Vk1Enum( att_info.layout );

					CHECK( att_info.layout == EImageLayout::General or att_info.layout == EImageLayout::ShaderReadOnlyOptimal );
					CHECK( att_info.descr.viewType == EImage::Unknown or att_info.descr.viewType == tex.textureType );
				}
				
				// create descriptor
				ImageDescr				descr;
				descr.info.sampler		= req_sampler.result.Get( VK_NULL_HANDLE );
				descr.info.imageView	= img_view;
				descr.info.imageLayout	= layout;
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
		bool operator () (const SamplerUniform &) const
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
			
			const auto&	img_res = self.GetResourceCache()->GetImageID( img_mod );	// warning: reference may be invalid after any changes

			// TODO: check format
			CHECK( img_res.Get<1>().usage[ EImageUsage::Storage ] );
			
			// find attachment info
			VkImageView						img_view = img_res.Get<0>().defaultView;
			VkImageLayout					layout	 = VK_IMAGE_LAYOUT_GENERAL;
			AttachmentInfoMap_t::iterator	info;

			if ( self._attachmentInfo.Find( img_mod.RawPtr(), OUT info ) )
			{
				auto const&					att_info = info->second.Get<ImageAttachment>();
				GpuMsg::CreateVkImageView	req_imageview{ att_info.descr };
				img_mod->Send( req_imageview );

				img_view	= req_imageview.result.Get( VK_NULL_HANDLE );
				layout		= Vk1Enum( att_info.layout );

				CHECK( att_info.layout == EImageLayout::General  );
				CHECK( att_info.descr.viewType == EImage::Unknown or att_info.descr.viewType == img.imageType );
			}

			// create descriptor
			ImageDescr				descr;
			descr.info.sampler		= VK_NULL_HANDLE;
			descr.info.imageView	= img_view;
			descr.info.imageLayout	= layout;
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

			const auto&	buf_res = self.GetResourceCache()->GetBufferID( buf_mod );

			CHECK( buf_res.Get<1>().usage[ EBufferUsage::Uniform ] );
			
			// find attachment info
			AttachmentInfoMap_t::iterator	info;
			BytesU							offset;

			if ( self._attachmentInfo.Find( buf_mod.RawPtr(), OUT info ) )
			{
				auto const&		buf_info = info->second.Get<BufferAttachment>();

				offset = buf_info.offset;

				CHECK_ERR( buf_info.size == buf.size );
			}
			else
			{
				CHECK_ERR( buf_res.Get<1>().size >= buf.size );
			}
			
			// create descriptor
			BufferDescr				descr;
			descr.info.buffer		= buf_res.Get<0>();
			descr.info.offset		= VkDeviceSize( offset );
			descr.info.range		= VkDeviceSize( buf.size );
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
			
			const auto&	buf_res = self.GetResourceCache()->GetBufferID( buf_mod );

			CHECK( buf_res.Get<1>().usage[ EBufferUsage::Storage ] );
			
			// find attachment info
			AttachmentInfoMap_t::iterator	info;
			BytesU							offset;
			BytesU							size	= buf_res.Get<1>().size;

			if ( self._attachmentInfo.Find( buf_mod.RawPtr(), OUT info ) )
			{
				auto const&		buf_info = info->second.Get<BufferAttachment>();

				offset	= buf_info.offset;
				size	= buf_info.size;
				
				CHECK_ERR(	(size >= buf.staticSize) and
							(buf.arrayStride == 0 or (size - buf.staticSize) % buf.arrayStride == 0) );
			}
			else
			{
				CHECK_ERR(	(buf_res.Get<1>().size >= buf.staticSize) and
							(buf.arrayStride == 0 or (buf_res.Get<1>().size - buf.staticSize) % buf.arrayStride == 0) );
			}
			
			// create descriptor
			BufferDescr				descr;
			descr.info.buffer		= buf_res.Get<0>();
			descr.info.offset		= VkDeviceSize( offset );
			descr.info.range		= VkDeviceSize( size );
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
		GpuMsg::GetPipelineLayoutDescription	req_descr;
		GpuMsg::GetVkDescriptionLayouts			req_layouts;

		_layout->Send( req_descr );
		_layout->Send( req_layouts );

		PipelineLayoutDescription const&	layout_descr = *req_descr.result;
		VkDescriptorSetLayout				descr_layout = *req_layouts.result;
		DescriptorPoolSizes_t				pool_sizes;
		_CreateResourceDescription_Func		func( OUT _resources, OUT pool_sizes, *this );

		// init pool sizes
		pool_sizes.Resize( VK_DESCRIPTOR_TYPE_RANGE_SIZE );
		FOR( i, pool_sizes ) {
			pool_sizes[i] = { VkDescriptorType( VK_DESCRIPTOR_TYPE_BEGIN_RANGE+i ), 0 };
		}

		// initialize table
		for (auto& un : layout_descr.GetUniforms()) {
			un.Accept( func );
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
		pool_info.poolSizeCount		= uint( pool_sizes.Count() );
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
	_WriteDescription_Func
=================================================
*/
	struct Vk1PipelineResourceTable::_WriteDescription_Func
	{
	// variables
		Array< VkWriteDescriptorSet >&	writeDescr;
		VkDescriptorSet					descriptorSet;

	// methods
		_WriteDescription_Func (Array<VkWriteDescriptorSet> &writeDescr, VkDescriptorSet descriptorSet) :
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
		_WriteDescription_Func		func( OUT write_descr, _descriptorSetId );

		for (auto& res : _resources) {
			res.Accept( func );
		}
		
		vkUpdateDescriptorSets( GetVkDevice(), uint(write_descr.Count()), write_descr.RawPtr(), 0, null );
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
	ModulePtr VulkanObjectsConstructor::CreateVk1PipelineResourceTable (ModuleMsg::UntypedID_t id, GlobalSystemsRef gs, const CreateInfo::PipelineResourceTable &ci)
	{
		return New< PlatformVK::Vk1PipelineResourceTable >( id, gs, ci );
	}
}	// Platforms
}	// Engine

#endif	// GRAPHICS_API_VULKAN
