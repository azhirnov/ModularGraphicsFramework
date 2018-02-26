// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Engine/Platforms/Shared/GPU/Image.h"
#include "Engine/Platforms/Shared/GPU/Buffer.h"
#include "Engine/Platforms/Shared/GPU/Pipeline.h"
#include "Engine/Platforms/Soft/Impl/SWBaseModule.h"
#include "Engine/Platforms/Soft/SoftRendererObjectsConstructor.h"

#ifdef GRAPHICS_API_SOFT

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
		using Fwd_GetSWBufferMemoryLayout		= GpuMsg::ResourceTableForwardMsg< Message< GpuMsg::GetSWBufferMemoryLayout > >;
		using Fwd_GetSWImageViewMemoryLayout	= GpuMsg::ResourceTableForwardMsg< Message< GpuMsg::GetSWImageViewMemoryLayout > >;
		using Fwd_GetSWTextureMemoryLayout		= GpuMsg::ResourceTableForwardMsg< Message< GpuMsg::GetSWTextureMemoryLayout > >;

		using LayoutMsgList_t		= MessageListFrom<
											GpuMsg::GetPipelineLayoutDescriptor
										>;

		using SupportedMessages_t	= SWBaseModule::SupportedMessages_t::Append< MessageListFrom<
											Fwd_GetSWBufferMemoryLayout,
											Fwd_GetSWImageViewMemoryLayout,
											Fwd_GetSWTextureMemoryLayout
										> >
										::Append< LayoutMsgList_t >;

		using SupportedEvents_t		= SWBaseModule::SupportedEvents_t;

		using CachedResources_t		= Array<Pair< ModulePtr, ModulePtr >>;	// sorted by binding index, second used for samplers

		struct _CacheResources_Func;


	// constants
	private:
		static const TypeIdList		_msgTypes;
		static const TypeIdList		_eventTypes;


	// variables
	private:
		ModulePtr			_layout;
		CachedResources_t	_cached;


	// methods
	public:
		SWPipelineResourceTable (GlobalSystemsRef gs, const CreateInfo::PipelineResourceTable &ci);
		~SWPipelineResourceTable ();


	// message handlers
	private:
		bool _Link (const Message< ModuleMsg::Link > &);
		bool _Compose (const Message< ModuleMsg::Compose > &);
		bool _Delete (const Message< ModuleMsg::Delete > &);
		bool _AttachModule (const Message< ModuleMsg::AttachModule > &);
		bool _DetachModule (const Message< ModuleMsg::DetachModule > &);

		bool _GetSWBufferMemoryLayout (const Message< Fwd_GetSWBufferMemoryLayout > &);
		bool _GetSWTextureMemoryLayout (const Message< Fwd_GetSWTextureMemoryLayout > &);
		bool _GetSWImageViewMemoryLayout (const Message< Fwd_GetSWImageViewMemoryLayout > &);

	private:
		bool _CreateResourceTable ();
		void _DestroyResourceTable ();
	};
//-----------------------------------------------------------------------------


	
	const TypeIdList	SWPipelineResourceTable::_msgTypes{ UninitializedT< SupportedMessages_t >() };
	const TypeIdList	SWPipelineResourceTable::_eventTypes{ UninitializedT< SupportedEvents_t >() };

/*
=================================================
	constructor
=================================================
*/
	SWPipelineResourceTable::SWPipelineResourceTable (GlobalSystemsRef gs, const CreateInfo::PipelineResourceTable &ci) :
		SWBaseModule( gs, ModuleConfig{ SWPipelineResourceTableModuleID, UMax }, &_msgTypes, &_eventTypes )
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
	bool SWPipelineResourceTable::_Link (const Message< ModuleMsg::Link > &)
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
	bool SWPipelineResourceTable::_Compose (const Message< ModuleMsg::Compose > &msg)
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
		return true;
	}

/*
=================================================
	_Delete
=================================================
*/
	bool SWPipelineResourceTable::_Delete (const Message< ModuleMsg::Delete > &msg)
	{
		_SendForEachAttachments( msg );

		_DestroyResourceTable();

		return Module::_Delete_Impl( msg );
	}

/*
=================================================
	_AttachModule
=================================================
*/
	bool SWPipelineResourceTable::_AttachModule (const Message< ModuleMsg::AttachModule > &msg)
	{
		CHECK_ERR( msg->newModule );

		// pipeline layout must be unique
		bool	is_layout = msg->newModule->GetSupportedMessages().HasAllTypes< LayoutMsgList_t >();

		CHECK( _Attach( msg->name, msg->newModule, is_layout ) );
		CHECK( _SetState( EState::Initial ) );

		if ( is_layout )
			_DestroyResourceTable();

		return true;
	}
	
/*
=================================================
	_DetachModule
=================================================
*/
	bool SWPipelineResourceTable::_DetachModule (const Message< ModuleMsg::DetachModule > &msg)
	{
		if ( _Detach( msg->oldModule ) )
		{
			CHECK( _SetState( EState::Initial ) );

			if ( msg->oldModule->GetSupportedMessages().HasAllTypes< LayoutMsgList_t >() )
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
		using TextureUniform	= PipelineLayoutDescriptor::TextureUniform;
		using SamplerUniform	= PipelineLayoutDescriptor::SamplerUniform;
		using ImageUniform		= PipelineLayoutDescriptor::ImageUniform;
		using UniformBuffer		= PipelineLayoutDescriptor::UniformBuffer;
		using StorageBuffer		= PipelineLayoutDescriptor::StorageBuffer;
		using PushConstant		= PipelineLayoutDescriptor::PushConstant;
		using SubpassInput		= PipelineLayoutDescriptor::SubpassInput;
		using ImageMsgList		= MessageListFrom< GpuMsg::GetSWImageViewMemoryLayout >;
		using SamplerMsgList	= MessageListFrom< GpuMsg::GetSamplerDescriptor >;
		using BufferMsgList		= MessageListFrom< GpuMsg::GetSWBufferMemoryLayout >;


	// variables
		SWPipelineResourceTable&	self;


	// methods
		explicit _CacheResources_Func (SWPipelineResourceTable &self) : self(self)
		{}

		void operator () (const PushConstant &pc) const
		{
			TODO( "PushConstant" );
		}
		
		void operator () (const SubpassInput &sp) const
		{
			TODO( "SubpassInput" );
		}
		
		void operator () (const SamplerUniform &samp) const
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
			CHECK_ERR( tex.binding != UMax );
			self._cached.Resize( tex.binding+1 );

			auto&	cached = self._cached[ tex.binding ];

			CHECK_ERR( FindModule< ImageMsgList >( tex.name, OUT cached.first ) );
			CHECK_ERR( FindModule< SamplerMsgList >( String(tex.name) << ".sampler", OUT cached.second ) );
			
			Message< GpuMsg::GetImageDescriptor >	req_img_descr;
			self.SendTo( cached.first, req_img_descr );
			
			CHECK( req_img_descr->result->imageType == tex.textureType );
			CHECK( req_img_descr->result->usage[ EImageUsage::Sampled ] );
			CHECK( EPixelFormatClass::StrongComparison( tex.format, EPixelFormatClass::From( req_img_descr->result->format ) ) );
			return true;
		}
		
/*
=================================================
	operator (ImageUniform)
=================================================
*/
		bool operator () (const ImageUniform &img) const
		{
			CHECK_ERR( img.binding != UMax );
			self._cached.Resize( img.binding+1 );
			
			CHECK_ERR( FindModule< ImageMsgList >( img.name, OUT self._cached[ img.binding ].first ) );
			
			Message< GpuMsg::GetImageDescriptor >	req_img_descr;
			self.SendTo( self._cached[ img.binding ].first , req_img_descr );
			
			CHECK( req_img_descr->result->usage[ EImageUsage::Storage ] );
			return true;
		}
		
/*
=================================================
	operator (UniformBuffer)
=================================================
*/
		bool operator () (const UniformBuffer &buf) const
		{
			CHECK_ERR( buf.binding != UMax );
			self._cached.Resize( buf.binding+1 );
			
			CHECK_ERR( FindModule< BufferMsgList >( buf.name, OUT self._cached[ buf.binding ].first ) );
			
			Message< GpuMsg::GetBufferDescriptor >	req_descr;
			self.SendTo( self._cached[ buf.binding ].first, req_descr );
			
			CHECK( req_descr->result->size == BytesUL(buf.size) );
			CHECK( req_descr->result->usage[ EBufferUsage::Uniform ] );
			return true;
		}
		
/*
=================================================
	operator (StorageBuffer)
=================================================
*/
		bool operator () (const StorageBuffer &buf) const
		{
			CHECK_ERR( buf.binding != UMax );
			self._cached.Resize( buf.binding+1 );
			
			CHECK_ERR( FindModule< BufferMsgList >( buf.name, OUT self._cached[ buf.binding ].first ) );
			
			Message< GpuMsg::GetBufferDescriptor >	req_descr;
			self.SendTo( self._cached[ buf.binding ].first, req_descr );
			
			CHECK( (req_descr->result->size >= buf.staticSize) and
					(buf.arrayStride == 0 or
					(req_descr->result->size - buf.staticSize) % buf.arrayStride == 0) );
			CHECK( req_descr->result->usage[ EBufferUsage::Storage ] );
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

		Message< GpuMsg::GetPipelineLayoutDescriptor >	req_descr;
		SendTo( _layout, req_descr );

		PipelineLayoutDescriptor const&	layout_descr = *req_descr->result;
		_CacheResources_Func			func{ *this };

		FOR( i, layout_descr.GetUniforms() ) {
			layout_descr.GetUniforms()[i].Apply( func );
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
	bool SWPipelineResourceTable::_GetSWBufferMemoryLayout (const Message< Fwd_GetSWBufferMemoryLayout > &msg)
	{
		CHECK_ERR( msg->index < _cached.Count() );
		CHECK_ERR( _cached[ msg->index ].first );

		return _cached[ msg->index ].first->Send( msg->message );
	}
	
/*
=================================================
	_GetSWTextureMemoryLayout
=================================================
*/
	bool SWPipelineResourceTable::_GetSWTextureMemoryLayout (const Message< Fwd_GetSWTextureMemoryLayout > &msg)
	{
		CHECK_ERR( msg->index < _cached.Count() );
		CHECK_ERR( _cached[ msg->index ].first and _cached[ msg->index ].second );
		
		CHECK( _cached[ msg->index ].first->Send( msg->message ) );

		Message< GpuMsg::GetSamplerDescriptor >	req_descr;
		CHECK( _cached[ msg->index ].second->Send( req_descr ) );

		msg->message->sampler.Set( *req_descr->result );
		return true;
	}

/*
=================================================
	_GetSWImageViewMemoryLayout
=================================================
*/
	bool SWPipelineResourceTable::_GetSWImageViewMemoryLayout (const Message< Fwd_GetSWImageViewMemoryLayout > &msg)
	{
		CHECK_ERR( msg->index < _cached.Count() );
		CHECK_ERR( _cached[ msg->index ].first );

		return _cached[ msg->index ].first->Send( msg->message );
	}

}	// PlatformSW
//-----------------------------------------------------------------------------

namespace Platforms
{
	ModulePtr SoftRendererObjectsConstructor::CreateSWPipelineResourceTable (GlobalSystemsRef gs, const CreateInfo::PipelineResourceTable &ci)
	{
		return New< PlatformSW::SWPipelineResourceTable >( gs, ci );
	}
}	// Platforms
}	// Engine

#endif	// GRAPHICS_API_SOFT
