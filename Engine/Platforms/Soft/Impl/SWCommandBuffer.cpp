// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Core/Config/Engine.Config.h"

#ifdef GRAPHICS_API_SOFT

#include "Engine/Platforms/Public/GPU/CommandBuffer.h"
#include "Engine/Platforms/Public/GPU/Image.h"
#include "Engine/Platforms/Public/GPU/Buffer.h"
#include "Engine/Platforms/Public/GPU/Pipeline.h"
#include "Engine/Platforms/Soft/Impl/SWBaseModule.h"
#include "Engine/Platforms/Soft/SoftRendererObjectsConstructor.h"
#include "Core/STL/Math/Image/ImageUtils.h"

namespace Engine
{
namespace PlatformSW
{
	
	//
	// Software Renderer Command Buffer
	//

	class SWCommandBuffer final : public SWBaseModule
	{
	// types
	private:
		using SupportedMessages_t	= SWBaseModule::SupportedMessages_t::Append< MessageListFrom<
											GpuMsg::GetCommandBufferDescription,
											GpuMsg::SetCommandBufferState,
											GpuMsg::GetCommandBufferState,
											GpuMsg::SetCommandBufferDependency,
											GpuMsg::SetSWCommandBufferQueue,
											GpuMsg::ExecuteSWCommandBuffer
										> >;

		using SupportedEvents_t		= SWBaseModule::SupportedEvents_t::Append< MessageListFrom<
											GpuMsg::OnCommandBufferStateChanged
										> >;

		using UsedResources_t		= Set< ModulePtr >;
		using Command_t				= GpuMsg::SetSWCommandBufferQueue::Command;
		using CommandArray_t		= Array< Command_t >;
		using CmdDataTypes_t		= GpuMsg::SetSWCommandBufferQueue::Data_t::TypeList_t;

		using ERecordingState		= GpuMsg::SetCommandBufferState::EState;


	// constants
	private:
		static const TypeIdList		_eventTypes;


	// variables
	private:
		CommandBufferDescription	_descr;
		CommandArray_t				_commands;
		UsedResources_t				_resources;
		BinaryArray					_bufferData;
		BinaryArray					_pushConstData;
		ERecordingState				_recordingState;

		// states
		ModulePtr					_computeShader;
		ModulePtr					_computeResTable;


	// methods
	public:
		SWCommandBuffer (UntypedID_t, GlobalSystemsRef gs, const CreateInfo::GpuCommandBuffer &ci);
		~SWCommandBuffer ();


	// message handlers
	private:
		bool _Compose (const ModuleMsg::Compose &);
		bool _Delete (const ModuleMsg::Delete &);
		bool _SetSWCommandBufferQueue (const GpuMsg::SetSWCommandBufferQueue &);
		bool _GetCommandBufferDescription (const GpuMsg::GetCommandBufferDescription &);
		bool _SetCommandBufferDependency (const GpuMsg::SetCommandBufferDependency &);
		bool _SetCommandBufferState (const GpuMsg::SetCommandBufferState &);
		bool _GetCommandBufferState (const GpuMsg::GetCommandBufferState &);
		bool _ExecuteSWCommandBuffer (const GpuMsg::ExecuteSWCommandBuffer &);
		
	private:
		bool _Submit ();
		bool _BeginRecording ();
		bool _EndRecording ();
		bool _ResetCmdBuffer ();
		bool _OnCompleted ();

		void _ChangeState (ERecordingState newState);

		bool _Initialize ();
		void _ClearStates ();
		
		bool _PrepareForCompute ();

	public:
		bool operator () (const GpuMsg::CmdBindComputePipeline &);
		bool operator () (const GpuMsg::CmdDispatch &);
		bool operator () (const GpuMsg::CmdDispatchIndirect &);
		bool operator () (const GpuMsg::CmdExecute &);
		bool operator () (const GpuMsg::CmdBindComputeResourceTable &);
		bool operator () (const GpuMsg::CmdCopyBuffer &);
		bool operator () (const GpuMsg::CmdCopyImage &);
		bool operator () (const GpuMsg::CmdCopyBufferToImage &);
		bool operator () (const GpuMsg::CmdCopyImageToBuffer &);
		bool operator () (const GpuMsg::CmdUpdateBuffer &);
		bool operator () (const GpuMsg::CmdFillBuffer &);
		bool operator () (const GpuMsg::CmdClearColorImage &);
		bool operator () (const GpuMsg::CmdPipelineBarrier &);
		bool operator () (const GpuMsg::CmdPushConstants &);
		bool operator () (const GpuMsg::CmdPushNamedConstants &);
		bool operator () (const GpuMsg::CmdDebugMarker &);
		bool operator () (const GpuMsg::CmdPushDebugGroup &);
		bool operator () (const GpuMsg::CmdPopDebugGroup &);
	};
//-----------------------------------------------------------------------------



	const TypeIdList	SWCommandBuffer::_eventTypes{ UninitializedT< SupportedEvents_t >() };

/*
=================================================
	constructor
=================================================
*/
	SWCommandBuffer::SWCommandBuffer (UntypedID_t id, GlobalSystemsRef gs, const CreateInfo::GpuCommandBuffer &ci) :
		SWBaseModule( gs, ModuleConfig{ id, UMax }, &_eventTypes ),
		_descr{ ci.descr },			_recordingState{ ERecordingState::Deleted },
		_computeShader{ null }
	{
		SetDebugName( "SWCommandBuffer" );

		_SubscribeOnMsg( this, &SWCommandBuffer::_OnModuleAttached_Impl );
		_SubscribeOnMsg( this, &SWCommandBuffer::_OnModuleDetached_Impl );
		_SubscribeOnMsg( this, &SWCommandBuffer::_AttachModule_Impl );
		_SubscribeOnMsg( this, &SWCommandBuffer::_DetachModule_Impl );
		_SubscribeOnMsg( this, &SWCommandBuffer::_FindModule_Impl );
		_SubscribeOnMsg( this, &SWCommandBuffer::_ModulesDeepSearch_Impl );
		_SubscribeOnMsg( this, &SWCommandBuffer::_Link_Impl );
		_SubscribeOnMsg( this, &SWCommandBuffer::_Compose );
		_SubscribeOnMsg( this, &SWCommandBuffer::_Delete );
		_SubscribeOnMsg( this, &SWCommandBuffer::_OnManagerChanged );
		_SubscribeOnMsg( this, &SWCommandBuffer::_GetCommandBufferDescription );
		_SubscribeOnMsg( this, &SWCommandBuffer::_GetDeviceInfo );
		_SubscribeOnMsg( this, &SWCommandBuffer::_GetSWDeviceInfo );
		_SubscribeOnMsg( this, &SWCommandBuffer::_GetSWPrivateClasses );
		_SubscribeOnMsg( this, &SWCommandBuffer::_SetSWCommandBufferQueue );
		_SubscribeOnMsg( this, &SWCommandBuffer::_SetCommandBufferDependency );
		_SubscribeOnMsg( this, &SWCommandBuffer::_SetCommandBufferState );
		_SubscribeOnMsg( this, &SWCommandBuffer::_GetCommandBufferState );
		_SubscribeOnMsg( this, &SWCommandBuffer::_ExecuteSWCommandBuffer );

		ASSERT( _ValidateMsgSubscriptions< SupportedMessages_t >() );

		_AttachSelfToManager( _GetGPUThread( ci.gpuThread ), UntypedID_t(0), true );
	}
	
/*
=================================================
	destructor
=================================================
*/
	SWCommandBuffer::~SWCommandBuffer ()
	{
	}
	
/*
=================================================
	_Compose
=================================================
*/
	bool SWCommandBuffer::_Compose (const ModuleMsg::Compose &msg)
	{
		if ( _IsComposedState( GetState() ) )
			return true;	// already composed

		CHECK_ERR( GetState() == EState::Linked );
		
		CHECK_COMPOSING( _Initialize() );

		_SendForEachAttachments( msg );
		
		// very paranoic check
		CHECK( _ValidateAllSubscriptions() );

		CHECK( _SetState( EState::ComposedMutable ) );
		
		_SendUncheckedEvent( ModuleMsg::AfterCompose{} );
		return true;
	}
	
/*
=================================================
	_Initialize
=================================================
*/
	bool SWCommandBuffer::_Initialize ()
	{
		// TODO

		_ChangeState( ERecordingState::Initial );
		return true;
	}

/*
=================================================
	_Delete
=================================================
*/
	bool SWCommandBuffer::_Delete (const ModuleMsg::Delete &msg)
	{
		_descr = Uninitialized;

		_ChangeState( ERecordingState::Deleted );

		_resources.Clear();
		_commands.Clear();
		_bufferData.Clear();
		_pushConstData.Clear();

		return Module::_Delete_Impl( msg );
	}

/*
=================================================
	_SetSWCommandBufferQueue
=================================================
*/
	bool SWCommandBuffer::_SetSWCommandBufferQueue (const GpuMsg::SetSWCommandBufferQueue &msg)
	{
		_commands		= RVREF(msg.commands.Get());
		_bufferData		= msg.bufferData;
		_pushConstData	= msg.pushConstData;
		return true;
	}

/*
=================================================
	_GetCommandBufferDescription
=================================================
*/
	bool SWCommandBuffer::_GetCommandBufferDescription (const GpuMsg::GetCommandBufferDescription &msg)
	{
		msg.result.Set( _descr );
		return true;
	}
	
/*
=================================================
	_SetCommandBufferDependency
=================================================
*/
	bool SWCommandBuffer::_SetCommandBufferDependency (const GpuMsg::SetCommandBufferDependency &msg)
	{
		_resources = RVREF( msg.resources.Get() );
		return true;
	}

/*
=================================================
	_GetCommandBufferDescription
=================================================
*/
	bool SWCommandBuffer::_SetCommandBufferState (const GpuMsg::SetCommandBufferState &msg)
	{
		switch ( msg.newState )
		{
			case ERecordingState::Initial :		CHECK( _ResetCmdBuffer() );	break;
			case ERecordingState::Recording :	CHECK( _BeginRecording() );	break;
			case ERecordingState::Executable :	CHECK( _EndRecording() );	break;
			case ERecordingState::Pending :		CHECK( _Submit() );			break;
			case ERecordingState::Completed :	CHECK( _OnCompleted() );	break;
			default :							WARNING( "unsupported state" );
		}
		return true;
	}
	
/*
=================================================
	_GetCommandBufferDescription
=================================================
*/
	bool SWCommandBuffer::_GetCommandBufferState (const GpuMsg::GetCommandBufferState &msg)
	{
		msg.result.Set( _recordingState );
		return true;
	}
	
/*
=================================================
	_ResetCmdBuffer
=================================================
*/
	bool SWCommandBuffer::_ResetCmdBuffer ()
	{
		if ( _recordingState == ERecordingState::Initial )
			return true;

		CHECK_ERR( _recordingState == ERecordingState::Pending );

		_ChangeState( ERecordingState::Initial );
		_resources.Clear();
		_commands.Clear();
		_bufferData.Clear();
		_pushConstData.Clear();

		return true;
	}

/*
=================================================
	_Submit
=================================================
*/
	bool SWCommandBuffer::_Submit ()
	{
		CHECK_ERR( _recordingState == ERecordingState::Executable );

		_ChangeState( ERecordingState::Pending );
		return true;
	}
	
/*
=================================================
	_ExecuteSWCommandBuffer
=================================================
*/
	bool SWCommandBuffer::_ExecuteSWCommandBuffer (const GpuMsg::ExecuteSWCommandBuffer &msg)
	{
		CHECK_ERR( _recordingState == ERecordingState::Pending );
		CHECK_ERR( msg.lastCmdIndex == 0 );

		msg.completed = false;
		
		FOR( i, _commands ) {
			_commands[i].data.Accept( *this );
		}

		msg.completed = true;

		_ClearStates();
		return true;
	}
	
/*
=================================================
	_BeginRecording
=================================================
*/
	bool SWCommandBuffer::_BeginRecording ()
	{
		CHECK_ERR( _recordingState == ERecordingState::Initial or
				  ((_recordingState == ERecordingState::Executable or
					_recordingState == ERecordingState::Pending) and
				    _descr.flags[ECmdBufferCreate::ImplicitResetable]) );

		if ( _recordingState == ERecordingState::Pending )
		{
			_resources.Clear();
			_commands.Clear();
			_bufferData.Clear();
			_pushConstData.Clear();
		}

		_ChangeState( ERecordingState::Recording );
		return true;
	}
	
/*
=================================================
	_EndRecording
=================================================
*/
	bool SWCommandBuffer::_EndRecording ()
	{
		CHECK_ERR( _recordingState == ERecordingState::Recording );

		_ChangeState( ERecordingState::Executable );
		return true;
	}
	
/*
=================================================
	_OnCompleted
=================================================
*/
	bool SWCommandBuffer::_OnCompleted ()
	{
		CHECK_ERR( _recordingState == ERecordingState::Pending );

		_resources.Clear();
		_commands.Clear();
		_bufferData.Clear();
		_pushConstData.Clear();

		_ChangeState( ERecordingState::Executable );
		return true;
	}
		
/*
=================================================
	_ChangeState
=================================================
*/
	void SWCommandBuffer::_ChangeState (ERecordingState newState)
	{
		const auto	old_state = _recordingState;

		_recordingState = newState;

		_SendEvent( GpuMsg::OnCommandBufferStateChanged{ old_state, newState });
	}
	
/*
=================================================
	_ClearStates
=================================================
*/
	void SWCommandBuffer::_ClearStates ()
	{
		_computeShader		= null;
		_computeResTable	= null;
	}
	
/*
=================================================
	_PrepareForCompute
=================================================
*/
	bool SWCommandBuffer::_PrepareForCompute ()
	{
		// TODO

		return true;
	}

/*
=================================================
	operator (CmdBindComputePipeline)
=================================================
*/
	bool SWCommandBuffer::operator () (const GpuMsg::CmdBindComputePipeline &msg)
	{
		_computeShader = msg.pipeline;
		return true;
	}
	
/*
=================================================
	operator (CmdDispatch)
=================================================
*/
	bool SWCommandBuffer::operator () (const GpuMsg::CmdDispatch &msg)
	{
		_PrepareForCompute();

		CHECK_ERR( GetDevice()->DispatchCompute( msg.groupCount, _computeShader, _computeResTable ) );
		return true;
	}
	
/*
=================================================
	operator (CmdDispatchIndirect)
=================================================
*/
	bool SWCommandBuffer::operator () (const GpuMsg::CmdDispatchIndirect &)
	{
		TODO( "" );
		//_PrepareForCompute();

		//CHECK_ERR( GetDevice()->DispatchComputeIndirect( msg.groupCount, _computeShader, _computeResTable ) );
		return true;
	}

/*
=================================================
	operator (CmdExecute)
=================================================
*/
	bool SWCommandBuffer::operator () (const GpuMsg::CmdExecute &msg)
	{
		ModuleUtils::Send( msg.cmdBuffers, GpuMsg::SetCommandBufferState{ GpuMsg::SetCommandBufferState::EState::Pending });
		ModuleUtils::Send( msg.cmdBuffers, GpuMsg::ExecuteSWCommandBuffer{} );
		//ModuleUtils::Send( msg.cmdBuffers, GpuMsg::SetCommandBufferState{ GpuMsg::SetCommandBufferState::EState::Completed });

		return true;
	}
	
/*
=================================================
	operator (CmdBindComputeResourceTable)
=================================================
*/
	bool SWCommandBuffer::operator () (const GpuMsg::CmdBindComputeResourceTable &msg)
	{
		ASSERT( msg.index == 0 );

		_computeResTable = msg.resourceTable;
		return true;
	}
	
/*
=================================================
	operator (CmdCopyBuffer)
=================================================
*/
	bool SWCommandBuffer::operator () (const GpuMsg::CmdCopyBuffer &msg)
	{
		for (auto& reg : msg.regions)
		{
			auto	src_mem = msg.srcBuffer->Request(GpuMsg::GetSWBufferMemoryLayout{ reg.srcOffset, reg.size, EPipelineAccess::TransferRead, EPipelineStage::Transfer });
			auto	dst_mem = msg.dstBuffer->Request(GpuMsg::GetSWBufferMemoryLayout{ reg.dstOffset, reg.size, EPipelineAccess::TransferWrite, EPipelineStage::Transfer });

			CHECK_ERR( src_mem.memAccess[ EMemoryAccess::GpuRead ] );
			CHECK_ERR( dst_mem.memAccess[ EMemoryAccess::GpuWrite ] );
			
			CHECK_ERR( src_mem.memory.Size() == dst_mem.memory.Size() );

			MemCopy( OUT BinArrayRef(dst_mem.memory), BinArrayCRef(src_mem.memory) );
		}
		return true;
	}
	
/*
=================================================
	operator (CmdCopyImage)
=================================================
*/
	bool SWCommandBuffer::operator () (const GpuMsg::CmdCopyImage &msg)
	{
		GpuMsg::GetSWImageMemoryLayout	req_src_mem { EPipelineAccess::TransferRead, EPipelineStage::Transfer };
		GpuMsg::GetSWImageMemoryLayout	req_dst_mem { EPipelineAccess::TransferWrite, EPipelineStage::Transfer };
		GpuMsg::GetImageDescription		req_src_descr;
		GpuMsg::GetImageDescription		req_dst_descr;

		msg.srcImage->Send( req_src_mem );
		msg.dstImage->Send( req_dst_mem );
		msg.srcImage->Send( req_src_descr );
		msg.dstImage->Send( req_dst_descr );

		CHECK_ERR( req_src_mem.result and req_src_mem.result->memAccess[ EMemoryAccess::GpuRead ] );
		CHECK_ERR( req_dst_mem.result and req_dst_mem.result->memAccess[ EMemoryAccess::GpuWrite ] );
		CHECK_ERR( req_src_descr.result and req_src_descr.result->usage[ EImageUsage::TransferSrc ] );
		CHECK_ERR( req_dst_descr.result and req_dst_descr.result->usage[ EImageUsage::TransferDst ] );
		CHECK_ERR( req_src_descr.result->samples == req_dst_descr.result->samples );
		
		for (auto& reg : msg.regions)
		{
			// find array layer or z-slice
			CHECK_ERR( (reg.srcLayers.baseLayer.Get() == 0 and reg.srcLayers.layerCount == 1) or
					   (reg.srcOffset.z == 0 and reg.size.z == 1) );
			
			CHECK_ERR( (reg.dstLayers.baseLayer.Get() == 0 and reg.dstLayers.layerCount == 1) or
					   (reg.dstOffset.z == 0 and reg.size.z == 1) );

			const uint	src_off_z	= Max( reg.srcLayers.baseLayer.Get(), reg.srcOffset.z );
			const uint	dst_off_z	= Max( reg.dstLayers.baseLayer.Get(), reg.dstOffset.z );
			const uint	src_dim_z	= Max( reg.srcLayers.layerCount, reg.size.z );
			const uint	dst_dim_z	= Max( reg.dstLayers.layerCount, reg.size.z );

			CHECK_ERR( src_dim_z == dst_dim_z );

			CHECK_ERR( src_off_z < req_src_mem.result->layers.Count() );
			CHECK_ERR( src_off_z + src_dim_z <= req_src_mem.result->layers.Count() );
			
			CHECK_ERR( dst_off_z < req_dst_mem.result->layers.Count() );
			CHECK_ERR( dst_off_z + dst_dim_z <= req_dst_mem.result->layers.Count() );
			
			for (uint z = 0; z < src_dim_z; ++z)
			{
				auto const&	src_layer	= req_src_mem.result->layers[ src_off_z + z ];
				auto&		dst_layer	= req_dst_mem.result->layers[ dst_off_z + z ];

				// find mipmap level
				CHECK_ERR( reg.srcLayers.mipLevel.Get() < src_layer.mipmaps.Count() );
				CHECK_ERR( reg.dstLayers.mipLevel.Get() < dst_layer.mipmaps.Count() );
			
				auto const&	src_level		= src_layer.mipmaps[ reg.srcLayers.mipLevel.Get() ];
				auto&		dst_level		= dst_layer.mipmaps[ reg.dstLayers.mipLevel.Get() ];
				BytesU		src_bpp			= BytesU(EPixelFormat::BitPerPixel( src_level.format ));
				BytesU		dst_bpp			= BytesU(EPixelFormat::BitPerPixel( dst_level.format ));
				BytesU		src_row_pitch	= GXImageUtils::AlignedRowSize( src_level.dimension.x, src_bpp, req_src_mem.result->align );
				BytesU		dst_row_pitch	= GXImageUtils::AlignedRowSize( dst_level.dimension.x, dst_bpp, req_dst_mem.result->align );
				BytesU		row_size		= src_bpp * reg.size.x;

				CHECK_ERR( src_bpp == dst_bpp );
				CHECK_ERR( src_level.memory != null and dst_level.memory != null );
				CHECK_ERR( All( reg.srcOffset.xy() < src_level.dimension ) );
				CHECK_ERR( All( reg.srcOffset.xy() + reg.size.xy() <= src_level.dimension ) );
				CHECK_ERR( All( reg.dstOffset.xy() < dst_level.dimension ) );
				CHECK_ERR( All( reg.dstOffset.xy() + reg.size.xy() <= dst_level.dimension ) );

				SW_DEBUG_REPORT2( src_level.layout == EImageLayout::TransferSrcOptimal or src_level.layout == EImageLayout::General, EDbgReport::Error );
				SW_DEBUG_REPORT2( dst_level.layout == EImageLayout::TransferDstOptimal or dst_level.layout == EImageLayout::General, EDbgReport::Error );

				// copy pixels
				for (uint y = 0; y < reg.size.y; ++y)
				{
					BytesU			src_off	= (src_bpp * reg.srcOffset.x) + ((reg.srcOffset.y + y) * src_row_pitch);
					BytesU			dst_off	= (dst_bpp * reg.dstOffset.x) + ((reg.dstOffset.y + y) * dst_row_pitch);

					BinArrayCRef	src		= src_level.Data().SubArray( usize(src_off), usize(row_size) );
					BinArrayRef		dst		= dst_level.Data().SubArray( usize(dst_off), usize(row_size) );

					MemCopy( OUT dst, src );
				}
			}
		}
		return true;
	}
	
/*
=================================================
	operator (CmdCopyBufferToImage)
=================================================
*/
	bool SWCommandBuffer::operator () (const GpuMsg::CmdCopyBufferToImage &msg)
	{
		GpuMsg::GetBufferDescription	req_src_descr;
		GpuMsg::GetImageDescription		req_dst_descr;
		msg.srcBuffer->Send( req_src_descr );
		msg.dstImage->Send( req_dst_descr );
		
		CHECK_ERR( req_src_descr.result and req_src_descr.result->usage[ EBufferUsage::TransferSrc ] );
		CHECK_ERR( req_dst_descr.result and req_dst_descr.result->usage[ EImageUsage::TransferDst ] );

		const BytesU	bpp	= BytesU(EPixelFormat::BitPerPixel( req_dst_descr.result->format ));

		for (auto& reg : msg.regions)
		{
			// find array layer or z-slice
			CHECK_ERR( (reg.imageLayers.baseLayer.Get() == 0 and reg.imageLayers.layerCount == 1) or
					   (reg.imageOffset.z == 0 and reg.imageSize.z == 1) );
			
			const uint		dst_off_z		= Max( reg.imageLayers.baseLayer.Get(), reg.imageOffset.z );
			const uint		dst_dim_z		= Max( reg.imageLayers.layerCount, reg.imageSize.z );
			const BytesU	row_size		= reg.imageSize.x * bpp;
			const BytesU	src_row_pitch	= reg.bufferRowLength * bpp;
			const BytesU	src_slice_pitch	= reg.bufferImageHeight * src_row_pitch;

			for (uint z = 0; z < dst_dim_z; ++z)
			{
				GpuMsg::GetSWImageViewMemoryLayout	req_dst_mem { EPipelineAccess::TransferWrite, EPipelineStage::Transfer };
				req_dst_mem.viewDescr.viewType	= req_dst_descr.result->imageType;
				req_dst_mem.viewDescr.format	= req_dst_descr.result->format;
				req_dst_mem.viewDescr.baseLevel	= reg.imageLayers.mipLevel;
				req_dst_mem.viewDescr.baseLayer	= reg.imageLayers.baseLayer;
				msg.dstImage->Send( req_dst_mem );

				CHECK_ERR( z + reg.imageOffset.z < req_dst_mem.result->layers.Count() );
				CHECK_ERR( reg.imageLayers.mipLevel.Get() < req_dst_mem.result->layers[z + reg.imageOffset.z].mipmaps.Count() );

				auto&	dst_layer		= req_dst_mem.result->layers[ z + reg.imageOffset.z ];
				auto&	dst_level		= dst_layer.mipmaps[ reg.imageLayers.mipLevel.Get() ];
				BytesU	dst_row_pitch	= GXImageUtils::AlignedRowSize( dst_level.dimension.x, bpp, req_dst_mem.result->align );

				for (uint y = 0; y < reg.imageSize.y; ++y)
				{
					const BytesU	dst_offset	= reg.imageOffset.x * bpp + dst_row_pitch * (y + reg.imageOffset.y);
					const BytesU	src_offset	= reg.bufferOffset + z * src_slice_pitch + y * src_row_pitch;

					GpuMsg::GetSWBufferMemoryLayout		req_src_mem { src_offset, row_size, EPipelineAccess::TransferRead, EPipelineStage::Transfer };
					msg.srcBuffer->Send( req_src_mem );

					SW_DEBUG_REPORT2( dst_level.layout == EImageLayout::TransferDstOptimal or dst_level.layout == EImageLayout::General, EDbgReport::Error );

					BinArrayRef		dst	= dst_level.Data().SubArray( usize(dst_offset), usize(row_size) );
					BinArrayCRef 	src	= req_src_mem.result->memory.SubArray( 0, usize(row_size) );

					MemCopy( OUT dst, src );
				}
			}
		}
		return true;
	}
	
/*
=================================================
	operator (CmdCopyImageToBuffer)
=================================================
*/
	bool SWCommandBuffer::operator () (const GpuMsg::CmdCopyImageToBuffer &msg)
	{
		GpuMsg::GetImageDescription		req_src_descr;
		GpuMsg::GetBufferDescription	req_dst_descr;
		msg.srcImage->Send( req_src_descr );
		msg.dstBuffer->Send( req_dst_descr );
		
		CHECK_ERR( req_src_descr.result and req_src_descr.result->usage[ EImageUsage::TransferSrc ] );
		CHECK_ERR( req_dst_descr.result and req_dst_descr.result->usage[ EBufferUsage::TransferDst ] );

		const BytesU	bpp	= BytesU(EPixelFormat::BitPerPixel( req_src_descr.result->format ));

		for (auto& reg : msg.regions)
		{
			// find array layer or z-slice
			CHECK_ERR( (reg.imageLayers.baseLayer.Get() == 0 and reg.imageLayers.layerCount == 1) or
					   (reg.imageOffset.z == 0 and reg.imageSize.z == 1) );
			
			const uint		src_off_z		= Max( reg.imageLayers.baseLayer.Get(), reg.imageOffset.z );
			const uint		src_dim_z		= Max( reg.imageLayers.layerCount, reg.imageSize.z );
			const BytesU	row_size		= reg.imageSize.x * bpp;
			const BytesU	dst_row_pitch	= reg.bufferRowLength * bpp;
			const BytesU	dst_slice_pitch	= reg.bufferImageHeight * dst_row_pitch;

			for (uint z = 0; z < src_dim_z; ++z)
			{
				GpuMsg::GetSWImageViewMemoryLayout	req_src_mem { EPipelineAccess::TransferRead, EPipelineStage::Transfer };
				req_src_mem.viewDescr.viewType	= req_src_descr.result->imageType;
				req_src_mem.viewDescr.format	= req_src_descr.result->format;
				req_src_mem.viewDescr.baseLevel	= reg.imageLayers.mipLevel;
				req_src_mem.viewDescr.baseLayer	= reg.imageLayers.baseLayer;
				msg.srcImage->Send( req_src_mem );

				CHECK_ERR( z + reg.imageOffset.z < req_src_mem.result->layers.Count() );
				CHECK_ERR( reg.imageLayers.mipLevel.Get() < req_src_mem.result->layers[z + reg.imageOffset.z].mipmaps.Count() );

				auto&	src_layer		= req_src_mem.result->layers[ z + reg.imageOffset.z ];
				auto&	src_level		= src_layer.mipmaps[ reg.imageLayers.mipLevel.Get() ];
				BytesU	src_row_pitch	= GXImageUtils::AlignedRowSize( src_level.dimension.x, bpp, req_src_mem.result->align );

				for (uint y = 0; y < reg.imageSize.y; ++y)
				{
					const BytesU	src_offset	= reg.imageOffset.x * bpp + src_row_pitch * (y + reg.imageOffset.y);
					const BytesU	dst_offset	= reg.bufferOffset + z * dst_slice_pitch + y * dst_row_pitch;

					GpuMsg::GetSWBufferMemoryLayout		req_dst_mem { dst_offset, row_size, EPipelineAccess::TransferWrite, EPipelineStage::Transfer };
					msg.dstBuffer->Send( req_dst_mem );

					SW_DEBUG_REPORT2( src_level.layout == EImageLayout::TransferSrcOptimal or src_level.layout == EImageLayout::General, EDbgReport::Error );

					BinArrayCRef	src	= src_level.Data().SubArray( usize(src_offset), usize(row_size) );
					BinArrayRef 	dst	= req_dst_mem.result->memory.SubArray( 0, usize(row_size) );

					MemCopy( OUT dst, src );
				}
			}
		}
		return true;
	}
	
/*
=================================================
	operator (CmdUpdateBuffer)
=================================================
*/
	bool SWCommandBuffer::operator () (const GpuMsg::CmdUpdateBuffer &msg)
	{
		GpuMsg::GetSWBufferMemoryLayout		req_mem { msg.dstOffset, msg.data.Size(), EPipelineAccess::TransferWrite, EPipelineStage::Transfer };
		GpuMsg::GetBufferDescription		req_descr;
		
		msg.dstBuffer->Send( req_mem );
		msg.dstBuffer->Send( req_descr );
		
		CHECK_ERR( msg.dstOffset < req_descr.result->size );
		CHECK_ERR( req_descr.result->usage[ EBufferUsage::TransferDst ] );
		CHECK_ERR( req_mem.result->memAccess[ EMemoryAccess::GpuWrite ] );
		CHECK_ERR( (msg.dstOffset % req_mem.result->align) == 0 );
		CHECK_ERR( req_mem.result->memory.Size() == msg.data.Size() );

		MemCopy( OUT req_mem.result->memory, BinArrayCRef(msg.data) );
		return true;
	}
	
/*
=================================================
	operator (CmdFillBuffer)
=================================================
*/
	bool SWCommandBuffer::operator () (const GpuMsg::CmdFillBuffer &msg)
	{
		GpuMsg::GetBufferDescription	req_descr;
		msg.dstBuffer->Send( req_descr );

		CHECK_ERR( msg.dstOffset < req_descr.result->size );
		CHECK_ERR( req_descr.result->usage[ EBufferUsage::TransferDst ] );

		const BytesU	size = Min( req_descr.result->size - msg.dstOffset, msg.size );
		
		GpuMsg::GetSWBufferMemoryLayout		req_mem { msg.dstOffset, size, EPipelineAccess::TransferWrite, EPipelineStage::Transfer };
		msg.dstBuffer->Send( req_mem );

		CHECK_ERR( req_mem.result->memAccess[ EMemoryAccess::GpuWrite ] );
		CHECK_ERR( (msg.dstOffset % req_mem.result->align) == 0 );
		CHECK_ERR( req_mem.result->memory.Size() == size );
		
		const ubyte	pattern[4]	= {  msg.pattern & 0xFF,
									(msg.pattern >> 8) & 0xFF,
									(msg.pattern >> 16) & 0xFF,
									(msg.pattern >> 24) & 0xFF };

		for (usize i = 0; i < usize(size); ++i)
		{
			req_mem.result->memory[i] = pattern[i&3];
		}
		return true;
	}
	
/*
=================================================
	operator (CmdClearColorImage)
=================================================
*/
	bool SWCommandBuffer::operator () (const GpuMsg::CmdClearColorImage &msg)
	{
		ASSERT( not msg.ranges.Empty() );

		TODO( "" );
		return true;
	}
	
/*
=================================================
	operator (CmdPipelineBarrier)
=================================================
*/
	bool SWCommandBuffer::operator () (const GpuMsg::CmdPipelineBarrier &msg)
	{
		for (auto& src : msg.imageBarriers)
		{
			CHECK_ERR( src.image );
			src.image->Send( GpuMsg::SWImageBarrier{ src, msg.srcStageMask, msg.dstStageMask });
		}

		/*for (auto& src : msg.bufferBarriers)
		{
			CHECK_ERR( src.buffer );
			src.buffer->Send( GpuMsg::SWBufferBarrier{ src, msg.srcStageMask, msg.dstStageMask });
		}*/
		
		ASSERT( msg.memoryBarriers.Empty() );	// not supported yet

		return true;
	}
	
/*
=================================================
	operator (CmdPushConstants)
=================================================
*/
	bool SWCommandBuffer::operator () (const GpuMsg::CmdPushConstants &)
	{
		TODO( "" );
		return true;
	}
	
/*
=================================================
	operator (CmdPushNamedConstants)
=================================================
*/
	bool SWCommandBuffer::operator () (const GpuMsg::CmdPushNamedConstants &)
	{
		TODO( "" );
		return true;
	}
	
/*
=================================================
	operator (CmdDebugMarker)
=================================================
*/
	bool SWCommandBuffer::operator () (const GpuMsg::CmdDebugMarker &msg)
	{
		LOG( "Debug marker: "_str << msg.info, ELog::Debug );

		if ( msg.breakPoint ) {
			GX_BREAK_POINT();
		}
		return true;
	}

/*
=================================================
	operator (CmdPushDebugGroup)
=================================================
*/
	bool SWCommandBuffer::operator () (const GpuMsg::CmdPushDebugGroup &msg)
	{
		LOG( "===================================================================\nDebug group begin: "_str << msg.info, ELog::Debug );
		return true;
	}

/*
=================================================
	operator (CmdPopDebugGroup)
=================================================
*/
	bool SWCommandBuffer::operator () (const GpuMsg::CmdPopDebugGroup &)
	{
		//const auto&	msg = cmdData.data.Get< GpuMsg::CmdPopDebugGroup >();
		
		LOG( "Debug group end\n===================================================================", ELog::Debug );
		return true;
	}

}	// PlatformSW
//-----------------------------------------------------------------------------

namespace Platforms
{
	ModulePtr SoftRendererObjectsConstructor::CreateSWCommandBuffer (ModuleMsg::UntypedID_t id, GlobalSystemsRef gs, const CreateInfo::GpuCommandBuffer &ci)
	{
		return New< PlatformSW::SWCommandBuffer >( id, gs, ci );
	}
}	// Platforms
}	// Engine

#endif	// GRAPHICS_API_SOFT
