// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Engine/ImportExport/Impl/ImportExportObjectsConstructor.h"
#include "Engine/ImportExport/Public/Image.h"
#include "Engine/ImportExport/Impl/GX_ImageFormat.h"

namespace Engine
{
namespace ImportExport
{

	//
	// GX Image Loader Module
	//

	class GX_ImageLoader final : public Module
	{
	// types
	private:
		using SupportedMessages_t	= MessageListFrom<
											ModuleMsg::AttachModule,
											ModuleMsg::DetachModule,
											ModuleMsg::OnModuleAttached,
											ModuleMsg::OnModuleDetached,
											ModuleMsg::FindModule,
											ModuleMsg::ModulesDeepSearch,
											ModuleMsg::Link,
											ModuleMsg::Delete
										>;
		
		using SupportedEvents_t		= MessageListFrom<
											ModuleMsg::Link,
											ModuleMsg::Compose,
											ModuleMsg::Delete
										>;

		using ImageMsgList_t		= MessageListFrom<
											GpuMsg::GetImageDescriptor,
											GpuMsg::WriteToImageMemory
										>;

		using AsyncCmdBufMsgList_t	= MessageListFrom<
											GraphicsMsg::CmdBeginAsync,
											GraphicsMsg::CmdEndAsync
										>;

		using CmdBufferMngrMsgList_t = MessageListFrom<
											GraphicsMsg::CmdBegin,
											GraphicsMsg::CmdEnd,
											GraphicsMsg::SubscribeOnFrameCompleted
										>;

		using Level_t	= GXImageFormat::Level;

		
	// constants
	private:
		static const TypeIdList		_msgTypes;
		static const TypeIdList		_eventTypes;


	// variables
	private:
		ModulePtr			_dataInput;
		Array<Level_t>		_levels;
		EPixelFormat::type	_format;
		EImageLayout::type	_imageLayout;


	// methods
	public:
		GX_ImageLoader (UntypedID_t, GlobalSystemsRef, const CreateInfo::ImageLoader &);
		~GX_ImageLoader ();

		
	// message handlers
	private:
		bool _Link (const Message< ModuleMsg::Link > &);
		bool _Delete (const Message< ModuleMsg::Delete > &);
		bool _OnModuleAttached (const Message< ModuleMsg::OnModuleAttached > &msg);
		bool _OnModuleDetached (const Message< ModuleMsg::OnModuleDetached > &msg);

		// event handler
		bool _OnImageComposed (const Message< ModuleMsg::AfterCompose > &);


	private:
		bool _PreLoadImage ();
		bool _LoadImage ();
		bool _UnloadImage ();
		bool _UpdateTexture ();
	};
//-----------------------------------------------------------------------------


	
	const TypeIdList	GX_ImageLoader::_msgTypes{ UninitializedT< SupportedMessages_t >() };
	const TypeIdList	GX_ImageLoader::_eventTypes{ UninitializedT< SupportedEvents_t >() };

/*
=================================================
	constructor
=================================================
*/
	GX_ImageLoader::GX_ImageLoader (UntypedID_t id, GlobalSystemsRef gs, const CreateInfo::ImageLoader &ci) :
		Module( gs, ModuleConfig{ id, 1 }, &_msgTypes, &_eventTypes ),
		_dataInput{ ci.dataInput },		_format{ Uninitialized },
		_imageLayout{ ci.imageLayout }
	{
		_SubscribeOnMsg( this, &GX_ImageLoader::_OnModuleAttached );
		_SubscribeOnMsg( this, &GX_ImageLoader::_OnModuleDetached );
		_SubscribeOnMsg( this, &GX_ImageLoader::_AttachModule_Impl );
		_SubscribeOnMsg( this, &GX_ImageLoader::_DetachModule_Impl );
		_SubscribeOnMsg( this, &GX_ImageLoader::_FindModule_Empty );
		_SubscribeOnMsg( this, &GX_ImageLoader::_ModulesDeepSearch_Empty );
		_SubscribeOnMsg( this, &GX_ImageLoader::_Link );
		_SubscribeOnMsg( this, &GX_ImageLoader::_Delete );

		CHECK( _ValidateMsgSubscriptions() );
	}
	
/*
=================================================
	destructor
=================================================
*/
	GX_ImageLoader::~GX_ImageLoader ()
	{
		ASSERT( not _dataInput );
	}
	
/*
=================================================
	_Link
=================================================
*/
	bool GX_ImageLoader::_Link (const Message< ModuleMsg::Link > &msg)
	{
		if ( _IsComposedOrLinkedState( GetState() ) )
			return true;	// already linked

		CHECK_ERR( _IsInitialState( GetState() ) );

		// check parent
		CHECK_LINKING( _GetParents().Count() == 1 );
		CHECK_LINKING( _GetParents().Front()->GetSupportedMessages().HasAllTypes< ImageMsgList_t >() );

		// check attachment
		CHECK_ATTACHMENT( GetModuleByMsg< AsyncCmdBufMsgList_t >() );

		// check file
		CHECK_LINKING( _PreLoadImage() );

		return Module::_Link_Impl( msg );
	}

/*
=================================================
	_Delete
=================================================
*/
	bool GX_ImageLoader::_Delete (const Message< ModuleMsg::Delete > &msg)
	{
		_dataInput = null;
		_levels.Clear();

		return Module::_Delete_Impl( msg );
	}
	
/*
=================================================
	_OnModuleAttached
=================================================
*/
	bool GX_ImageLoader::_OnModuleAttached (const Message< ModuleMsg::OnModuleAttached > &msg)
	{
		if ( msg->attachedModule == this )
		{
			_OnAttachedToParent( msg->parent );
			CHECK( _SetState( EState::Initial ) );
		}
		return true;
	}
	
/*
=================================================
	_OnModuleDetached
=================================================
*/
	bool GX_ImageLoader::_OnModuleDetached (const Message< ModuleMsg::OnModuleDetached > &msg)
	{
		if ( msg->detachedModule == this )
		{
			if ( msg->isLast )
			{
				_OnDetachedFromParent( msg->parent );
				CHECK( _UnloadImage() );
			}
		}
		else
		{
			UnsubscribeAll( msg->detachedModule );
		}
		return true;
	}
	
/*
=================================================
	_OnImageComposed
=================================================
*/
	bool GX_ImageLoader::_OnImageComposed (const Message< ModuleMsg::AfterCompose > &)
	{
		CHECK_ERR( _UpdateTexture() );

		_GetParents().Front()->UnsubscribeAll( this );
		return true;
	}

/*
=================================================
	_PreLoadImage
=================================================
*/
	bool GX_ImageLoader::_PreLoadImage ()
	{
		CHECK_ERR( _dataInput and _IsComposedState( _dataInput->GetState() ) );

		// read header
		GXImageFormat::Header	header = {};
		{
			_dataInput->Send< DSMsg::ReadRegion >({ BinArrayRef::FromValue( header ) });

			const usize		num_levels	= header.layers * header.maxLevel;
			_levels.Resize( num_levels, false );

			_dataInput->Send< DSMsg::ReadRegion >({ BinArrayRef::From( _levels ) });
			_dataInput->Send< DSMsg::ReleaseData >({});

			_format = header.pixelFormat;
		}

		// update image descriptor
		ImageDescriptor		descr;
		ModulePtr			image = _GetParents().Front();

		Message< GpuMsg::GetImageDescriptor >	req_descr;
		CHECK( image->Send( req_descr ) );

		descr			= *req_descr->result;
		descr.dimension	= Max( uint4( header.width, header.height, header.depth, header.layers ), 1u );
		descr.format	= header.pixelFormat;
		descr.maxLevel	= MipmapLevel( header.maxLevel );
		descr.samples	= MultiSamples( header.samples );
		descr.usage		|= EImageUsage::TransferDst;	// to copy from buffer

		EImage::type	img_type = EImage::Tex2D;

		// choose image type
		{
			if ( All( descr.dimension.yzw() == 1 ) )
				img_type = EImage::Tex1D;
			else
			if ( All( descr.dimension.yz() == 1 ) and descr.dimension.w > 1 )
				img_type = EImage::Tex1DArray;
			else
			if ( All( descr.dimension.zw() == 1 ) and All( descr.dimension.xy() > 1 ) )
				img_type = descr.samples.IsEnabled() ? EImage::Tex2DMS : EImage::Tex2D;
			else
			if ( descr.dimension.z == 1 and All( descr.dimension.xyw() > 1 ) )
			{
				if ( EnumEq( header.flags, GXImageFormat::EFlags::CubeMap ) )
					img_type = descr.dimension.w == 6 ? EImage::TexCube : EImage::TexCubeArray;
				else
					img_type = descr.samples.IsEnabled() ? EImage::Tex2DMSArray : EImage::Tex2DArray;
			}
			else
			if ( All( descr.dimension.xyz() > 1 ) )
				img_type = EImage::Tex3D;
		}

		if ( descr.imageType == EImage::Unknown )
			descr.imageType = img_type;
		else
			CHECK_ERR( img_type == descr.imageType );
		
		CHECK( image->Send< GpuMsg::SetImageDescriptor >({ descr }) );

		CHECK( image->Subscribe( this, &GX_ImageLoader::_OnImageComposed ) );
		return true;
	}
	
/*
=================================================
	_UnloadImage
=================================================
*/
	bool GX_ImageLoader::_UnloadImage ()
	{
		if ( _dataInput )
			_dataInput->Send< DSMsg::ReleaseData >({});

		_format = Uninitialized;
		_levels.Clear();

		CHECK( _SetState( EState::Initial ) );
		return true;
	}
	
/*
=================================================
	_UpdateTexture
=================================================
*/
	bool GX_ImageLoader::_UpdateTexture ()
	{
		ModulePtr	cmd			= GetModuleByMsg< AsyncCmdBufMsgList_t >();
		ModulePtr	perframe	= cmd->GetModuleByMsg< CmdBufferMngrMsgList_t >();
		ModulePtr	image		= _GetParents().Front();
		BitsU		bpp			= EPixelFormat::BitPerPixel( _format );

		// prepare
		Message< GpuMsg::GetDeviceInfo >	req_dev;
		CHECK( image->Send( req_dev ) );

		Message< GpuMsg::GetGraphicsModules >	req_ids;
		CHECK( req_dev->result->gpuThread->Send( req_ids ) );

		Message< GpuMsg::GetImageDescriptor >	req_descr;
		CHECK( image->Send( req_descr ) );

		Message< GpuMsg::GetImageMemoryLayout >	req_layout;
		CHECK( image->Send( req_layout ) );


		// copy buffer to image
		Message< GraphicsMsg::CmdBeginAsync >	begin;
		begin->syncMode = GraphicsMsg::CmdBeginAsync::EMode::BeforeFrame;

		CHECK( cmd->Send( begin ) );
		
		cmd->Send< GpuMsg::CmdPipelineBarrier >({ EPipelineStage::TopOfPipe,
												  EPipelineStage::Transfer,
												  EPipelineAccess::bits(),
												  EPipelineAccess::TransferWrite,
												  EImageLayout::Undefined,
												  EImageLayout::TransferDstOptimal,
												  image,
												  EImageAspect::Color });

		// load levels
		for (const auto& level : _levels)
		{
			ModulePtr	buffer;
			CHECK_ERR( GlobalSystems()->modulesFactory->Create(
										req_ids->graphics->buffer,
										GlobalSystems(),
										CreateInfo::GpuBuffer{
											BufferDescriptor{ level.slicePitch * level.dimension.z, EBufferUsage::TransferSrc },
											EGpuMemory::CoherentWithCPU,
											EMemoryAccess::CpuWrite | EMemoryAccess::GpuRead },
										OUT buffer ) );
			ModuleUtils::Initialize({ buffer });

			Message< GpuMsg::MapMemoryToCpu >	map_cmd{ GpuMsg::EMappingFlags::WriteDiscard };
			buffer->Send( map_cmd );

			_dataInput->Send< DSMsg::ReadRegion >({ *map_cmd->result });

			buffer->Send< GpuMsg::UnmapMemory >({});
			
			cmd->Send< GpuMsg::CmdPipelineBarrier >({ EPipelineStage::Host,
													  EPipelineStage::Transfer,
													  EPipelineAccess::HostWrite,
													  EPipelineAccess::TransferRead,
													  buffer,
													  BytesUL(0), BytesUL(ulong(UMax)) });

			Message< GpuMsg::CmdCopyBufferToImage >	copy;
			copy->dstImage		= image;
			copy->srcBuffer		= buffer;
			copy->dstLayout		= EImageLayout::TransferDstOptimal;
			copy->regions.PushBack({});
			copy->regions.Back().bufferRowLength		 = uint(level.rowPitch.ToBits()) / uint(bpp);
			copy->regions.Back().bufferImageHeight		 = uint(level.slicePitch / level.rowPitch);
			copy->regions.Back().imageSize				 = level.dimension;
			copy->regions.Back().imageLayers.aspectMask	|= EImageAspect::Color;

			CHECK( cmd->Send( copy ) );
		}
		
		EPipelineStage::bits	dst_stage;
		EPipelineAccess::type	dst_access;

		switch ( _imageLayout )
		{
			case EImageLayout::General :
			case EImageLayout::ShaderReadOnlyOptimal :
				dst_stage	= EPipelineStage::AllShaders;
				dst_access	= EPipelineAccess::ShaderRead;
				break;

			default :
				RETURN_ERR( "image layout " << EImageLayout::ToString( _imageLayout ) << " is not supported!" );
		}

		cmd->Send< GpuMsg::CmdPipelineBarrier >({ EPipelineStage::Transfer,
												  dst_stage,
												  EPipelineAccess::TransferWrite,
												  dst_access,
												  EImageLayout::TransferDstOptimal,
												  _imageLayout,
												  image,
												  EImageAspect::Color });

		CHECK( cmd->Send< GraphicsMsg::CmdEndAsync >({}) );
		
		_UnloadImage();
		return true;
	}
//-----------------------------------------------------------------------------
	
	
/*
=================================================
	CreateGXImageLoader
=================================================
*/
	ModulePtr ImportExportObjectsConstructor::CreateGXImageLoader (ModuleMsg::UntypedID_t id, GlobalSystemsRef gs, const CreateInfo::ImageLoader &ci)
	{
		return New< GX_ImageLoader >( id, gs, ci );
	}

}	// ImportExport
}	// Engine
