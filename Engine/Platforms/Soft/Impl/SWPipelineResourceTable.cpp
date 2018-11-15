// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Core/Config/Engine.Config.h"

#ifdef GRAPHICS_API_SOFT

#include "Engine/Platforms/Public/GPU/Image.h"
#include "Engine/Platforms/Public/GPU/Buffer.h"
#include "Engine/Platforms/Public/GPU/Pipeline.h"
#include "Engine/Platforms/Soft/Impl/SWBaseModule.h"
#include "Engine/Platforms/Soft/SoftRendererObjectsConstructor.h"

namespace Engine
{
namespace PlatformSW
{

	//
	// Software Renderer Pipeline Resource Table (uniforms)
	//

	class SWPipelineResourceTable final : public SWBaseModule
	{
	// types
	private:
		using Fwd_GetSWBufferMemoryLayout		= GpuMsg::ResourceTableForwardMsg< GpuMsg::GetSWBufferMemoryLayout >;
		using Fwd_GetSWImageViewMemoryLayout	= GpuMsg::ResourceTableForwardMsg< GpuMsg::GetSWImageViewMemoryLayout >;
		using Fwd_GetSWTextureMemoryLayout		= GpuMsg::ResourceTableForwardMsg< GpuMsg::GetSWTextureMemoryLayout >;

		using LayoutMsgList_t		= MessageListFrom<
											GpuMsg::GetPipelineLayoutDescription
										>;

		using SupportedMessages_t	= SWBaseModule::SupportedMessages_t::Append< MessageListFrom<
											Fwd_GetSWBufferMemoryLayout,
											Fwd_GetSWImageViewMemoryLayout,
											Fwd_GetSWTextureMemoryLayout,
											GpuMsg::PipelineAttachBuffer,
											GpuMsg::PipelineAttachImage,
											GpuMsg::PipelineAttachTexture
										> >;

		using SupportedEvents_t		= SWBaseModule::SupportedEvents_t;
		
		using ImageMsgList_t		= MessageListFrom< GpuMsg::GetSWImageViewMemoryLayout >;
		using SamplerMsgList_t		= MessageListFrom< GpuMsg::GetSamplerDescription >;
		using BufferMsgList_t		= MessageListFrom< GpuMsg::GetSWBufferMemoryLayout >;

		struct ResCache
		{
			ModulePtr	resource;	// 
			ModulePtr	sampler;	// only for texture	
		};

		using CachedResources_t		= Array< ResCache >;	// sorted by binding index

		struct _CacheResources_Func;
		
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


	// constants
	private:
		static const TypeIdList		_eventTypes;


	// variables
	private:
		ModulePtr				_layout;
		CachedResources_t		_cached;
		AttachmentInfoMap_t		_attachmentInfo;


	// methods
	public:
		SWPipelineResourceTable (UntypedID_t, GlobalSystemsRef gs, const CreateInfo::PipelineResourceTable &ci);
		~SWPipelineResourceTable ();


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
		bool _GetSWBufferMemoryLayout (const Fwd_GetSWBufferMemoryLayout &);
		bool _GetSWTextureMemoryLayout (const Fwd_GetSWTextureMemoryLayout &);
		bool _GetSWImageViewMemoryLayout (const Fwd_GetSWImageViewMemoryLayout &);

	private:
		bool _CreateResourceTable ();
		void _DestroyResourceTable ();
	};
//-----------------------------------------------------------------------------


	
	const TypeIdList	SWPipelineResourceTable::_eventTypes{ UninitializedT< SupportedEvents_t >() };

/*
=================================================
	constructor
=================================================
*/
	SWPipelineResourceTable::SWPipelineResourceTable (UntypedID_t id, GlobalSystemsRef gs, const CreateInfo::PipelineResourceTable &ci) :
		SWBaseModule( gs, ModuleConfig{ id, UMax, true }, &_eventTypes )
	{
		SetDebugName( "SWPipelineResourceTable" );

		_SubscribeOnMsg( this, &SWPipelineResourceTable::_OnModuleAttached_Impl );
		_SubscribeOnMsg( this, &SWPipelineResourceTable::_OnModuleDetached_Impl );
		_SubscribeOnMsg( this, &SWPipelineResourceTable::_AttachModule );
		_SubscribeOnMsg( this, &SWPipelineResourceTable::_DetachModule );
		_SubscribeOnMsg( this, &SWPipelineResourceTable::_FindModule_Impl );
		_SubscribeOnMsg( this, &SWPipelineResourceTable::_ModulesDeepSearch_Impl );
		_SubscribeOnMsg( this, &SWPipelineResourceTable::_Link );
		_SubscribeOnMsg( this, &SWPipelineResourceTable::_Compose );
		_SubscribeOnMsg( this, &SWPipelineResourceTable::_Delete );
		_SubscribeOnMsg( this, &SWPipelineResourceTable::_OnManagerChanged );
		_SubscribeOnMsg( this, &SWPipelineResourceTable::_GetDeviceInfo );
		_SubscribeOnMsg( this, &SWPipelineResourceTable::_GetSWDeviceInfo );
		_SubscribeOnMsg( this, &SWPipelineResourceTable::_GetSWPrivateClasses );
		_SubscribeOnMsg( this, &SWPipelineResourceTable::_GetSWBufferMemoryLayout );
		_SubscribeOnMsg( this, &SWPipelineResourceTable::_GetSWTextureMemoryLayout );
		_SubscribeOnMsg( this, &SWPipelineResourceTable::_GetSWImageViewMemoryLayout );
		_SubscribeOnMsg( this, &SWPipelineResourceTable::_PipelineAttachImage );
		_SubscribeOnMsg( this, &SWPipelineResourceTable::_PipelineAttachBuffer );
		_SubscribeOnMsg( this, &SWPipelineResourceTable::_PipelineAttachTexture );
		
		ASSERT( _ValidateMsgSubscriptions< SupportedMessages_t >() );

		_AttachSelfToManager( _GetGPUThread( ci.gpuThread ), UntypedID_t(0), true );
	}
	
/*
=================================================
	destructor
=================================================
*/
	SWPipelineResourceTable::~SWPipelineResourceTable ()
	{
	}
	
/*
=================================================
	_Link
=================================================
*/
	bool SWPipelineResourceTable::_Link (const ModuleMsg::Link &)
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
	bool SWPipelineResourceTable::_Compose (const ModuleMsg::Compose &msg)
	{
		if ( _IsComposedState( GetState() ) )
			return true;	// already composed

		CHECK_ERR( GetState() == EState::Linked );

		CHECK_COMPOSING( _layout );
		CHECK_COMPOSING( _CreateResourceTable() );

		_SendForEachAttachments( msg );
		
		// very paranoic check
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
	bool SWPipelineResourceTable::_Delete (const ModuleMsg::Delete &msg)
	{
		_SendForEachAttachments( msg );

		_DestroyResourceTable();
		
		_attachmentInfo.Clear();

		return Module::_Delete_Impl( msg );
	}

/*
=================================================
	_AttachModule
=================================================
*/
	bool SWPipelineResourceTable::_AttachModule (const ModuleMsg::AttachModule &msg)
	{
		CHECK_ERR( msg.newModule );

		// pipeline layout must be unique
		const bool	is_layout = msg.newModule->SupportsAllMessages< LayoutMsgList_t >();

		CHECK( _Attach( msg.name, msg.newModule ) );
		CHECK( _SetState( EState::Initial ) );

		if ( is_layout )
			_DestroyResourceTable();

		return true;
	}
	
/*
=================================================
	_PipelineAttachBuffer
=================================================
*/
	bool SWPipelineResourceTable::_PipelineAttachBuffer (const GpuMsg::PipelineAttachBuffer &msg)
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
	_PipelineAttachImage
=================================================
*/
	bool SWPipelineResourceTable::_PipelineAttachImage (const GpuMsg::PipelineAttachImage &msg)
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
	bool SWPipelineResourceTable::_PipelineAttachTexture (const GpuMsg::PipelineAttachTexture &msg)
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
	_DetachModule
=================================================
*/
	bool SWPipelineResourceTable::_DetachModule (const ModuleMsg::DetachModule &msg)
	{
		if ( _Detach( msg.oldModule ) )
		{
			CHECK( _SetState( EState::Initial ) );

			if ( msg.oldModule->SupportsAllMessages< LayoutMsgList_t >() )
				_DestroyResourceTable();
		}
		return true;
	}
	
/*
=================================================
	_CacheResources_Func
=================================================
*/
	struct SWPipelineResourceTable::_CacheResources_Func
	{
	// types
		using TextureUniform	= PipelineLayoutDescription::TextureUniform;
		using SamplerUniform	= PipelineLayoutDescription::SamplerUniform;
		using ImageUniform		= PipelineLayoutDescription::ImageUniform;
		using UniformBuffer		= PipelineLayoutDescription::UniformBuffer;
		using StorageBuffer		= PipelineLayoutDescription::StorageBuffer;
		using PushConstant		= PipelineLayoutDescription::PushConstant;
		using SubpassInput		= PipelineLayoutDescription::SubpassInput;
		using ImageMsgList		= MessageListFrom< GpuMsg::GetSWImageViewMemoryLayout >;
		using SamplerMsgList	= MessageListFrom< GpuMsg::GetSamplerDescription >;
		using BufferMsgList		= MessageListFrom< GpuMsg::GetSWBufferMemoryLayout >;
		using ResourceMsgList	= CompileTime::TypeListFrom< ImageMsgList, SamplerMsgList, BufferMsgList >;
		using ModuleMap			= HashMap< ModuleName_t, ModulePtr >;


	// variables
		SWPipelineResourceTable&	self;
		mutable ModuleMap			moduleMap;


	// methods
		explicit _CacheResources_Func (SWPipelineResourceTable &self) : self(self)
		{
			moduleMap.Reserve( self._GetAttachments().Count() );

			for (const auto& mod : self._GetAttachments())
			{
				if ( mod.second->SupportsAnyMessage< ResourceMsgList >() )
					moduleMap.Add( mod.first, mod.second );
			}
		}

		~_CacheResources_Func ()
		{
			DEBUG_ONLY(
				FOR( i, moduleMap ) {
					LOG( "Unused module in resource table: "_str << moduleMap[i].first, ELog::Warning );
				}
			)
		}

		void operator () (const PushConstant &) const
		{
			TODO( "PushConstant" );
		}
		
		void operator () (const SubpassInput &) const
		{
			TODO( "SubpassInput" );
		}
		
		void operator () (const SamplerUniform &) const
		{
			TODO( "SamplerUniform" );
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
			CHECK_ERR( tex.uniqueIndex != UMax );
			self._cached.Resize( tex.uniqueIndex+1 );

			auto&	cached = self._cached[ tex.uniqueIndex ];

			CHECK_ERR( FindModule< ImageMsgList >( tex.name, OUT cached.resource ) );
			CHECK_ERR( FindModule< SamplerMsgList >( String(tex.name) << ".sampler", OUT cached.sampler ) );
			
			GpuMsg::GetImageDescription	req_img_descr;
			cached.resource->Send( req_img_descr );
			
			CHECK( req_img_descr.result->imageType == tex.textureType );
			CHECK( req_img_descr.result->usage[ EImageUsage::Sampled ] );
			CHECK( EPixelFormatClass::StrongComparison( tex.format, EPixelFormatClass::From( req_img_descr.result->format ) ) );
			
			if ( not self._attachmentInfo.IsExist( cached.resource.RawPtr() ) )
			{
				self._attachmentInfo.Add( cached.resource.RawPtr(), AttachmentInfo_t{ImageAttachment{ ImageViewDescription{*req_img_descr.result}, EImageLayout::General }} );
			}
			return true;
		}
		
/*
=================================================
	operator (ImageUniform)
=================================================
*/
		bool operator () (const ImageUniform &img) const
		{
			CHECK_ERR( img.uniqueIndex != UMax );
			self._cached.Resize( img.uniqueIndex+1 );
			
			auto&	cached = self._cached[ img.uniqueIndex ];

			CHECK_ERR( FindModule< ImageMsgList >( img.name, OUT cached.resource ) );
			
			GpuMsg::GetImageDescription	req_img_descr;
			cached.resource->Send( req_img_descr );
			
			CHECK( req_img_descr.result->usage[ EImageUsage::Storage ] );
			
			if ( not self._attachmentInfo.IsExist( cached.resource.RawPtr() ) )
			{
				self._attachmentInfo.Add( cached.resource.RawPtr(), AttachmentInfo_t{ImageAttachment{ ImageViewDescription{*req_img_descr.result}, EImageLayout::General }} );
			}
			return true;
		}
		
/*
=================================================
	operator (UniformBuffer)
=================================================
*/
		bool operator () (const UniformBuffer &buf) const
		{
			CHECK_ERR( buf.uniqueIndex != UMax );
			self._cached.Resize( buf.uniqueIndex+1 );

			auto&	cached = self._cached[ buf.uniqueIndex ];
			
			CHECK_ERR( FindModule< BufferMsgList >( buf.name, OUT cached.resource ) );
			
			GpuMsg::GetBufferDescription	req_descr;
			cached.resource->Send( req_descr );
			
			CHECK( req_descr.result->usage[ EBufferUsage::Uniform ] );
			
			AttachmentInfoMap_t::iterator	info;

			if ( self._attachmentInfo.Find( cached.resource.RawPtr(), OUT info ) )
			{
				CHECK_ERR( info->second.Get<BufferAttachment>().size == buf.size );
			}
			else
			{
				CHECK_ERR( req_descr.result->size >= buf.size );

				self._attachmentInfo.Add( cached.resource.RawPtr(), AttachmentInfo_t{BufferAttachment{ 0_b, req_descr.result->size }} );
			}
			return true;
		}
		
/*
=================================================
	operator (StorageBuffer)
=================================================
*/
		bool operator () (const StorageBuffer &buf) const
		{
			CHECK_ERR( buf.uniqueIndex != UMax );
			self._cached.Resize( buf.uniqueIndex+1 );
			
			auto&	cached = self._cached[ buf.uniqueIndex ];

			CHECK_ERR( FindModule< BufferMsgList >( buf.name, OUT cached.resource ) );
			
			GpuMsg::GetBufferDescription		req_descr;
			cached.resource->Send( req_descr );
			
			CHECK( req_descr.result->usage[ EBufferUsage::Storage ] );
			
			AttachmentInfoMap_t::iterator	info;

			if ( self._attachmentInfo.Find( cached.resource.RawPtr(), OUT info ) )
			{
				auto const &	buf_info = info->second.Get<BufferAttachment>();

				CHECK_ERR(	(buf_info.size >= buf.staticSize) and
							(buf.arrayStride == 0 or (buf_info.size - buf.staticSize) % buf.arrayStride == 0) );
			}
			else
			{
				CHECK_ERR(	(req_descr.result->size >= buf.staticSize) and
							(buf.arrayStride == 0 or (req_descr.result->size - buf.staticSize) % buf.arrayStride == 0) );

				self._attachmentInfo.Add( cached.resource.RawPtr(), AttachmentInfo_t{BufferAttachment{ 0_b, req_descr.result->size }} );
			}
			return true;
		}
	};

/*
=================================================
	_CreateResourceTable
=================================================
*/
	bool SWPipelineResourceTable::_CreateResourceTable ()
	{
		_cached.Clear();

		GpuMsg::GetPipelineLayoutDescription	req_descr;
		_layout->Send( req_descr );

		PipelineLayoutDescription const&	layout_descr = *req_descr.result;
		_CacheResources_Func				func{ *this };

		FOR( i, layout_descr.GetUniforms() ) {
			layout_descr.GetUniforms()[i].Accept( func );
		}
		return true;
	}

/*
=================================================
	_DestroyResourceTable
=================================================
*/
	void SWPipelineResourceTable::_DestroyResourceTable ()
	{
		_layout = null;
		_cached.Clear();
	}

/*
=================================================
	_GetSWBufferMemoryLayout
=================================================
*/
	bool SWPipelineResourceTable::_GetSWBufferMemoryLayout (const Fwd_GetSWBufferMemoryLayout &msg)
	{
		CHECK_ERR( msg.index < _cached.Count() );
		auto const&	cached = _cached[ msg.index ];
		
		AttachmentInfoMap_t::iterator	iter;
		CHECK_ERR( _attachmentInfo.Find( cached.resource.RawPtr(), OUT iter ) );

		auto const&	range = iter->second.Get< BufferAttachment >();

		msg.message.offset	= range.offset;
		msg.message.size	= range.size;

		CHECK( cached.resource->SendAsync( msg.message ) );
		return true;
	}
	
/*
=================================================
	_GetSWTextureMemoryLayout
=================================================
*/
	bool SWPipelineResourceTable::_GetSWTextureMemoryLayout (const Fwd_GetSWTextureMemoryLayout &msg)
	{
		CHECK_ERR( msg.index < _cached.Count() );
		auto const&	cached = _cached[ msg.index ];

		CHECK_ERR( cached.resource and cached.sampler );
		
		AttachmentInfoMap_t::iterator	iter;
		CHECK_ERR( _attachmentInfo.Find( cached.resource.RawPtr(), OUT iter ) );
		
		const auto&	img_info = iter->second.Get< ImageAttachment >();
		
		GpuMsg::GetSWImageViewMemoryLayout	req_img{ img_info.descr, msg.message.accessMask, msg.message.stage };
		CHECK( cached.resource->SendAsync( req_img ) );
		
		GpuMsg::GetSamplerDescription	req_samp;
		CHECK( cached.sampler->SendAsync( req_samp ) );
		
		msg.message.result.Set( *req_img.result );
		msg.message.sampler.Set( *req_samp.result );
		return true;
	}

/*
=================================================
	_GetSWImageViewMemoryLayout
=================================================
*/
	bool SWPipelineResourceTable::_GetSWImageViewMemoryLayout (const Fwd_GetSWImageViewMemoryLayout &msg)
	{
		CHECK_ERR( msg.index < _cached.Count() );
		auto const&	cached = _cached[ msg.index ];

		CHECK_ERR( cached.resource );
		
		AttachmentInfoMap_t::iterator	iter;
		CHECK_ERR( _attachmentInfo.Find( cached.resource.RawPtr(), OUT iter ) );
		
		const auto&	img_info = iter->second.Get< ImageAttachment >();

		// TODO: check layout
		msg.message.viewDescr = img_info.descr;

		CHECK( cached.resource->SendAsync( msg.message ) );
		return true;
	}

}	// PlatformSW
//-----------------------------------------------------------------------------

namespace Platforms
{
	ModulePtr SoftRendererObjectsConstructor::CreateSWPipelineResourceTable (ModuleMsg::UntypedID_t id, GlobalSystemsRef gs, const CreateInfo::PipelineResourceTable &ci)
	{
		return New< PlatformSW::SWPipelineResourceTable >( id, gs, ci );
	}
}	// Platforms
}	// Engine

#endif	// GRAPHICS_API_SOFT
