// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Core/Config/Engine.Config.h"

#ifdef GRAPHICS_API_SOFT

#include "Engine/Platforms/Public/GPU/Image.h"
#include "Engine/Platforms/Public/GPU/Memory.h"
#include "Engine/Platforms/Public/GPU/Enums.ToString.h"
#include "Engine/Platforms/Soft/Impl/SWBaseModule.h"
#include "Engine/Platforms/Soft/SoftRendererObjectsConstructor.h"
#include "Engine/Platforms/Public/Tools/ImageUtils.h"

namespace Engine
{
namespace PlatformSW
{

	//
	// Software Renderer Texture
	//

	class SWImage final : public SWBaseModule
	{
	// types
	private:
		using ForwardToMem_t		= MessageListFrom< 
											DSMsg::GetDataSourceDescription,
											DSMsg::ReadMemRange,
											DSMsg::WriteMemRange,
											GpuMsg::GetGpuMemoryDescription,
											GpuMsg::MapMemoryToCpu,
											GpuMsg::MapImageToCpu,
											GpuMsg::FlushMemoryRange,
											GpuMsg::UnmapMemory,
											GpuMsg::ReadFromGpuMemory,
											GpuMsg::WriteToGpuMemory,
											GpuMsg::ReadFromImageMemory,
											GpuMsg::WriteToImageMemory
										>;

		using SupportedMessages_t	= SWBaseModule::SupportedMessages_t::Append< MessageListFrom<
											GpuMsg::GetImageDescription,
											GpuMsg::SetImageDescription,
											GpuMsg::GetSWImageMemoryLayout,
											GpuMsg::GetSWImageViewMemoryLayout,
											GpuMsg::GetSWImageMemoryRequirements,
											GpuMsg::GetImageMemoryLayout,
											GpuMsg::SWImageBarrier
										> >;

		using SupportedEvents_t		= SWBaseModule::SupportedEvents_t::Append< MessageListFrom<
											GpuMsg::SetImageDescription
										> >;
		
		using MemoryEvents_t		= MessageListFrom< GpuMsg::OnMemoryBindingChanged >;

		using Utils					= PlatformTools::ImageUtils;
		using ImgLayer_t			= GpuMsg::GetSWImageMemoryLayout::ImgLayer;
		using ImgLayers3D_t			= GpuMsg::GetSWImageMemoryLayout::ImgLayers3D;


		
	// constants
	private:
		static const TypeIdList		_eventTypes;


	// variables
	private:
		ImageDescription		_descr;
		ModulePtr				_memObj;
		ImgLayers3D_t			_memLayout;
		EImageLayout::type		_imageLayout;
		
		EGpuMemory::bits		_memFlags;		// -|-- this flags is requirements for memory obj, don't use it anywhere
		EMemoryAccess::bits		_memAccess;		// -|
		bool					_useMemMngr;	// -|

		bool					_isBindedToMemory;
		

	// methods
	public:
		SWImage (UntypedID_t, GlobalSystemsRef gs, const CreateInfo::GpuImage &ci);
		~SWImage ();


	// message handlers
	private:
		bool _Link (const ModuleMsg::Link &);
		bool _Compose (const ModuleMsg::Compose &);
		bool _Delete (const ModuleMsg::Delete &);
		bool _AttachModule (const ModuleMsg::AttachModule &);
		bool _DetachModule (const ModuleMsg::DetachModule &);

		bool _GetImageDescription (const GpuMsg::GetImageDescription &);
		bool _SetImageDescription (const GpuMsg::SetImageDescription &);
		bool _GetSWImageMemoryLayout (const GpuMsg::GetSWImageMemoryLayout &);
		bool _GetSWImageViewMemoryLayout (const GpuMsg::GetSWImageViewMemoryLayout &);
		bool _GetSWImageMemoryRequirements (const GpuMsg::GetSWImageMemoryRequirements &);
		bool _GetImageMemoryLayout (const GpuMsg::GetImageMemoryLayout &);
		bool _SWImageBarrier (const GpuMsg::SWImageBarrier &);
		
	// event handlers
		bool _OnMemoryBindingChanged (const GpuMsg::OnMemoryBindingChanged &);


	private:
		bool _IsImageCreated () const;

		bool _CreateImage ();
		bool _CreateDefaultView ();
		
		void _DestroyAll ();
		void _DestroyViews ();

		bool _CanHaveImageView () const;
		
		static void _ValidateMemFlags (INOUT EGpuMemory::bits &flags);
		static void _ValidateDescription (INOUT ImageDescription &descr);
	};
//-----------------------------------------------------------------------------


	
	const TypeIdList	SWImage::_eventTypes{ UninitializedT< SupportedEvents_t >() };

/*
=================================================
	constructor
=================================================
*/
	SWImage::SWImage (UntypedID_t id, GlobalSystemsRef gs, const CreateInfo::GpuImage &ci) :
		SWBaseModule( gs, ModuleConfig{ id, UMax }, &_eventTypes ),
		_descr( ci.descr ),				_imageLayout( Uninitialized ),
		_memFlags( ci.memFlags ),		_memAccess( ci.access ),
		_useMemMngr( ci.allocMem ),		_isBindedToMemory( false )
	{
		SetDebugName( "SWImage" );
		
		_SubscribeOnMsg( this, &SWImage::_OnModuleAttached_Impl );
		_SubscribeOnMsg( this, &SWImage::_OnModuleDetached_Impl );
		_SubscribeOnMsg( this, &SWImage::_AttachModule );
		_SubscribeOnMsg( this, &SWImage::_DetachModule );
		_SubscribeOnMsg( this, &SWImage::_FindModule_Impl );
		_SubscribeOnMsg( this, &SWImage::_ModulesDeepSearch_Impl );
		_SubscribeOnMsg( this, &SWImage::_Link );
		_SubscribeOnMsg( this, &SWImage::_Compose );
		_SubscribeOnMsg( this, &SWImage::_Delete );
		_SubscribeOnMsg( this, &SWImage::_OnManagerChanged );
		_SubscribeOnMsg( this, &SWImage::_GetImageDescription );
		_SubscribeOnMsg( this, &SWImage::_SetImageDescription );
		_SubscribeOnMsg( this, &SWImage::_GetDeviceInfo );
		_SubscribeOnMsg( this, &SWImage::_GetSWDeviceInfo );
		_SubscribeOnMsg( this, &SWImage::_GetSWPrivateClasses );
		_SubscribeOnMsg( this, &SWImage::_GetSWImageMemoryLayout );
		_SubscribeOnMsg( this, &SWImage::_GetSWImageViewMemoryLayout );
		_SubscribeOnMsg( this, &SWImage::_GetSWImageMemoryRequirements );
		_SubscribeOnMsg( this, &SWImage::_GetImageMemoryLayout );
		_SubscribeOnMsg( this, &SWImage::_SWImageBarrier );
		
		ASSERT( _ValidateMsgSubscriptions< SupportedMessages_t >() );

		_AttachSelfToManager( _GetGPUThread( ci.gpuThread ), UntypedID_t(0), true );
		
		_ValidateMemFlags( INOUT _memFlags );
		_ValidateDescription( INOUT _descr );
	}
	
/*
=================================================
	destructor
=================================================
*/
	SWImage::~SWImage ()
	{
		ASSERT( not _IsImageCreated() );
	}
	
/*
=================================================
	_Link
=================================================
*/
	bool SWImage::_Link (const ModuleMsg::Link &msg)
	{
		if ( _IsComposedOrLinkedState( GetState() ) )
			return true;	// already linked

		CHECK_ERR( _IsInitialState( GetState() ) );
		
		_memObj = GetModuleByEvent< MemoryEvents_t >();

		if ( not _memObj and _useMemMngr )
		{
			ModulePtr	mem_module;
			CHECK_ERR( GlobalSystems()->modulesFactory->Create(
								SWMemoryModuleID,
								GlobalSystems(),
								CreateInfo::GpuMemory{ _memFlags, _memAccess },
								OUT mem_module ) );

			CHECK_ERR( _Attach( "mem", mem_module ) );
			_memObj = mem_module;
		}
		CHECK_ATTACHMENT( _memObj );

		_memObj->Subscribe( this, &SWImage::_OnMemoryBindingChanged );
		
		CHECK_LINKING( _CopySubscriptions< ForwardToMem_t >( _memObj ) );
		
		return Module::_Link_Impl( msg );
	}
	
/*
=================================================
	_Compose
=================================================
*/
	bool SWImage::_Compose (const ModuleMsg::Compose &msg)
	{
		if ( _IsComposedState( GetState() ) )
			return true;	// already composed

		CHECK_ERR( GetState() == EState::Linked );

		CHECK_COMPOSING( _CreateImage() );
		
		_SendForEachAttachments( msg );
		
		CHECK( _ValidateAllSubscriptions() );

		// composed state will be changed when memory binded to image
		return true;
	}
	
/*
=================================================
	_Delete
=================================================
*/
	bool SWImage::_Delete (const ModuleMsg::Delete &msg)
	{
		_DestroyAll();

		return Module::_Delete_Impl( msg );
	}
	
/*
=================================================
	_AttachModule
=================================================
*/
	bool SWImage::_AttachModule (const ModuleMsg::AttachModule &msg)
	{
		const bool	is_mem	= msg.newModule->GetSupportedEvents().HasAllTypes< MemoryEvents_t >();

		CHECK( _Attach( msg.name, msg.newModule ) );

		if ( is_mem )
		{
			CHECK( _SetState( EState::Initial ) );
			_DestroyViews();
		}
		return true;
	}
	
/*
=================================================
	_DetachModule
=================================================
*/
	bool SWImage::_DetachModule (const ModuleMsg::DetachModule &msg)
	{
		CHECK( _Detach( msg.oldModule ) );

		if ( msg.oldModule == _memObj )
		{
			CHECK( _SetState( EState::Initial ) );
			_DestroyViews();
		}
		return true;
	}
		
/*
=================================================
	_IsImageCreated
=================================================
*/
	bool SWImage::_IsImageCreated () const
	{
		return	not _memLayout.layers.Empty() and
				not _memLayout.layers.Front().mipmaps.Empty();
	}

/*
=================================================
	_CreateImage
=================================================
*/
	bool SWImage::_CreateImage ()
	{
		using Mipmap_t = GpuMsg::GetSWImageMemoryLayout::ImgLayer;

		CHECK_ERR( not _IsImageCreated() );
		CHECK_ERR( not EPixelFormat::IsCompressed( _descr.format ) );
		CHECK_ERR( not _descr.samples.IsEnabled() );
		CHECK_ERR( _descr.dimension.z == 1 or _descr.dimension.w == 1 );
		
		_memLayout	= Uninitialized;

		BitsU	bpp = EPixelFormat::BitPerPixel( _descr.format );
		CHECK_ERR( bpp % 8 == 0 );

		_memLayout.dimension = uint3( _descr.dimension.xy(), _descr.dimension.z*_descr.dimension.w );
		_memLayout.layers.Resize( _memLayout.dimension.z );

		for (auto& layer : _memLayout.layers)
		{
			uint2	dim = _descr.dimension.xy();

			layer.dimension = dim;

			for (; layer.mipmaps.Count() < _descr.maxLevel.Get();)
			{
				Mipmap_t		mip;
				mip.dimension	= dim;
				mip.size		= GXImageUtils::AlignedRowSize( dim.x, bpp, _memLayout.align ) * dim.y;
				mip.format		= _descr.format;
				layer.size		= AlignToLarge( layer.size + mip.size, _memLayout.align );

				layer.mipmaps.PushBack( mip );

				if ( All( dim <= uint2(1) ) )
					break;

				dim = Max( 1u, dim>>1 );
			}

			_memLayout.size = AlignToLarge( _memLayout.size + layer.size, _memLayout.align );
		}

		_imageLayout = EImageLayout::Undefined;
		return true;
	}
	
/*
=================================================
	_CreateDefaultView
=================================================
*/
	bool SWImage::_CreateDefaultView ()
	{
		CHECK_ERR( _IsImageCreated() );

		GpuMsg::GetSWMemoryData			req_data;
		GpuMsg::GetGpuMemoryDescription	req_descr;

		_memObj->Send( req_data );
		_memObj->Send( req_descr );

		void *	ptr = req_data.result->ptr();
		BytesU	off;

		for (auto& layer : _memLayout.layers)
		{
			for (auto& mip : layer.mipmaps)
			{
				mip.memory	= ptr + off;
				off			= AlignToLarge( off + mip.size, _memLayout.align );
			}
		}

		_memLayout.memAccess = req_descr.result->access;

		CHECK_ERR( off == _memLayout.size );
		CHECK_ERR( off == req_data.result->Size() );
		return true;
	}

/*
=================================================
	_DestroyAll
=================================================
*/
	void SWImage::_DestroyAll ()
	{
		_DestroyViews();

		_memLayout		= Uninitialized;
		_imageLayout	= Uninitialized;
		_descr			= Uninitialized;
	}
	
/*
=================================================
	_DestroyViews
=================================================
*/
	void SWImage::_DestroyViews ()
	{
		if ( _memObj )
		{
			this->UnsubscribeAll( _memObj );
			_memObj->UnsubscribeAll( this );
			_memObj = null;
		}

		_isBindedToMemory	= false;
	}

/*
=================================================
	_GetImageDescription
=================================================
*/
	bool SWImage::_GetImageDescription (const GpuMsg::GetImageDescription &msg)
	{
		msg.result.Set( _descr );
		return true;
	}
	
/*
=================================================
	_SetImageDescription
=================================================
*/
	bool SWImage::_SetImageDescription (const GpuMsg::SetImageDescription &msg)
	{
		CHECK_ERR( GetState() == EState::Initial );

		_descr = msg.descr;

		Utils::ValidateDescription( INOUT _descr );
		return true;
	}

/*
=================================================
	_OnMemoryBindingChanged
=================================================
*/
	bool SWImage::_OnMemoryBindingChanged (const GpuMsg::OnMemoryBindingChanged &msg)
	{
		CHECK_ERR( _IsComposedOrLinkedState( GetState() ) );

		using EBindingTarget = GpuMsg::OnMemoryBindingChanged::EBindingTarget;

		if (  msg.targetObject == this )
		{
			_isBindedToMemory = ( msg.newState == EBindingTarget::Image );

			if ( _isBindedToMemory )
			{
				CHECK( _CreateDefaultView() );
				CHECK( _SetState( EState::ComposedMutable ) );
				
				_SendUncheckedEvent( ModuleMsg::AfterCompose{} );
			}
			else
			{
				Send( ModuleMsg::Delete{} );
			}
		}
		return true;
	}
	
/*
=================================================
	_GetSWImageMemoryLayout
=================================================
*/
	bool SWImage::_GetSWImageMemoryLayout (const GpuMsg::GetSWImageMemoryLayout &msg)
	{
		CHECK_ERR( _IsImageCreated() );

		msg.result.Set( _memLayout );
		return true;
	}
	
/*
=================================================
	_GetSWImageViewMemoryLayout
=================================================
*/
	bool SWImage::_GetSWImageViewMemoryLayout (const GpuMsg::GetSWImageViewMemoryLayout &msg)
	{
		CHECK_ERR( _IsImageCreated() and _isBindedToMemory );
		CHECK_ERR( _CanHaveImageView() or msg.accessMask[EPipelineAccess::TransferRead] or msg.accessMask[EPipelineAccess::TransferWrite] );
		CHECK_ERR( EPixelFormat::BitPerPixel( _descr.format ) == EPixelFormat::BitPerPixel( msg.viewDescr.format ) );

		CHECK_ERR( msg.viewDescr.baseLevel < _descr.maxLevel and
				   msg.viewDescr.levelCount > 0 and
				   msg.viewDescr.baseLevel.Get() + msg.viewDescr.levelCount <= _descr.maxLevel.Get() );

		CHECK_ERR( msg.viewDescr.baseLayer.Get() < _descr.dimension.w and
				   msg.viewDescr.layerCount > 0 and
				   msg.viewDescr.baseLayer.Get() + msg.viewDescr.layerCount <= _descr.dimension.w );

		ImgLayers3D_t		layout;
		layout.align		= _memLayout.align;
		layout.dimension	= _memLayout.dimension;
		layout.memAccess	= _memLayout.memAccess;
		layout.layers.Resize( msg.viewDescr.layerCount );

		for (uint z = 0; z < msg.viewDescr.layerCount; ++z)
		{
			auto const&	src_layer	= _memLayout.layers[msg.viewDescr.baseLayer.Get() + z];
			auto&		dst_layer	= layout.layers[z];

			dst_layer.dimension	= Max( 1u, src_layer.dimension >> msg.viewDescr.baseLevel.Get() );
			dst_layer.mipmaps.Resize( msg.viewDescr.levelCount );

			for (uint m = 0; m < msg.viewDescr.levelCount; ++m)
			{
				auto const&	src_level	= src_layer.mipmaps[msg.viewDescr.baseLevel.Get() + m];
				auto&		dst_level	= dst_layer.mipmaps[m];

				dst_level			= src_level;
				dst_level.format	= msg.viewDescr.format;

				dst_layer.size		= AlignToLarge( dst_layer.size + dst_level.size, layout.align );
				
				CHECK_ERR( EPixelFormat::BitPerPixel( dst_level.format ) == EPixelFormat::BitPerPixel( src_level.format ) );
			}

			ASSERT( All( dst_layer.dimension.xy() == dst_layer.mipmaps.Front().dimension ) );

			layout.size = AlignToLarge( layout.size + dst_layer.size, layout.align );
		}

		msg.result.Set( RVREF(layout) );
		return true;
	}

/*
=================================================
	_GetSWImageMemoryRequirements
=================================================
*/
	bool SWImage::_GetSWImageMemoryRequirements (const GpuMsg::GetSWImageMemoryRequirements &msg)
	{
		CHECK_ERR( _IsImageCreated() );

		msg.result.Set({ _memLayout.size, _memLayout.align });
		return true;
	}
	
/*
=================================================
	_GetImageMemoryLayout
=================================================
*/
	bool SWImage::_GetImageMemoryLayout (const GpuMsg::GetImageMemoryLayout &msg)
	{
		const BytesU	bpp = BytesU(EPixelFormat::BitPerPixel( _descr.format ));

		GpuMsg::GetImageMemoryLayout::MemLayout	result;
		result.offset		= 0_b;
		result.dimension	= Max( Utils::LevelDimension( _descr.imageType, _descr.dimension, msg.mipLevel.Get() ).xyz(), 1u );
		result.size			= _memLayout.size;
		result.rowPitch		= GXImageUtils::AlignedRowSize( result.dimension.x, bpp, _memLayout.align );
		result.slicePitch	= GXImageUtils::AlignedRowSize( ulong(result.rowPitch) * result.dimension.y, 1_b, _memLayout.align );

		ASSERT( result.slicePitch * result.dimension.z <= result.size );

		msg.result.Set( result );
		return true;
	}
	
/*
=================================================
	_SWImageBarrier
=================================================
*/
	bool SWImage::_SWImageBarrier (const GpuMsg::SWImageBarrier &msg)
	{
		CHECK_ERR( _IsImageCreated() and _isBindedToMemory );

		for (auto& br : msg.barriers)
		{
			ASSERT( br.image == this );

			const uint	layer_cnt = Min( br.range.baseLayer.Get() + br.range.layerCount, uint(_memLayout.layers.Count()) );

			for (uint z = br.range.baseLayer.Get(); z < layer_cnt; ++z)
			{
				auto&		layer	= _memLayout.layers[z];
				const uint	mip_cnt = Min( br.range.baseMipLevel.Get() + br.range.levelCount, uint(layer.mipmaps.Count()) );

				for (uint m = br.range.baseMipLevel.Get(); m < mip_cnt; ++m)
				{
					auto&	mipmap	= layer.mipmaps[m];

					mipmap.layout = br.newLayout;
				}
			}
		}
		return true;
	}
	
/*
=================================================
	_CanHaveImageView
=================================================
*/
	bool SWImage::_CanHaveImageView () const
	{
		return _descr.usage != (_descr.usage & (EImageUsage::TransferSrc | EImageUsage::TransferDst));
	}
	
/*
=================================================
	_ValidateMemFlags
=================================================
*/
	void SWImage::_ValidateMemFlags (INOUT EGpuMemory::bits &flags)
	{
		if ( flags[EGpuMemory::Dedicated] and flags[EGpuMemory::SupportAliasing] )
		{
			WARNING( "not supported" );

			flags[EGpuMemory::Dedicated] = false;
		}
	}
	
/*
=================================================
	_ValidateDescription
=================================================
*/
	void SWImage::_ValidateDescription (INOUT ImageDescription &descr)
	{
		// description may be invalid for sharing or for delayed initialization
		if ( Utils::IsValidDescription( descr ) )
			Utils::ValidateDescription( INOUT descr );
	}

}	// PlatformSW
//-----------------------------------------------------------------------------

namespace Platforms
{
	ModulePtr SoftRendererObjectsConstructor::CreateSWImage (ModuleMsg::UntypedID_t id, GlobalSystemsRef gs, const CreateInfo::GpuImage &ci)
	{
		return New< PlatformSW::SWImage >( id, gs, ci );
	}

}	// Platforms
}	// Engine

#endif	// GRAPHICS_API_SOFT
