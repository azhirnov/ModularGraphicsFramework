// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Engine/Config/Engine.Config.h"

#ifdef GRAPHICS_API_SOFT

#include "Engine/Platforms/Public/GPU/CommandBuffer.h"
#include "Engine/Platforms/Public/GPU/Image.h"
#include "Engine/Platforms/Public/GPU/Buffer.h"
#include "Engine/Platforms/Public/GPU/Pipeline.h"
#include "Engine/Platforms/Soft/Impl/SWBaseModule.h"
#include "Engine/Platforms/Soft/SoftRendererObjectsConstructor.h"
#include "Engine/STL/Math/Image/ImageUtils.h"

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
											GpuMsg::GetCommandBufferDescriptor,
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
		static const TypeIdList		_msgTypes;
		static const TypeIdList		_eventTypes;


	// variables
	private:
		CommandBufferDescriptor		_descr;
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
		bool _Compose (const Message< ModuleMsg::Compose > &);
		bool _Delete (const Message< ModuleMsg::Delete > &);
		bool _SetSWCommandBufferQueue (const Message< GpuMsg::SetSWCommandBufferQueue > &);
		bool _GetCommandBufferDescriptor (const Message< GpuMsg::GetCommandBufferDescriptor > &);
		bool _SetCommandBufferDependency (const Message< GpuMsg::SetCommandBufferDependency > &);
		bool _SetCommandBufferState (const Message< GpuMsg::SetCommandBufferState > &);
		bool _GetCommandBufferState (const Message< GpuMsg::GetCommandBufferState > &);
		bool _ExecuteSWCommandBuffer (const Message< GpuMsg::ExecuteSWCommandBuffer > &);
		
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

		bool _CmdBindComputePipeline (const Command_t &cmd);
		bool _CmdDispatch (const Command_t &cmd);
		bool _CmdExecute (const Command_t &cmd);
		bool _CmdBindComputeResourceTable (const Command_t &cmd);
		bool _CmdCopyBuffer (const Command_t &cmd);
		bool _CmdCopyImage (const Command_t &cmd);
		bool _CmdCopyBufferToImage (const Command_t &cmd);
		bool _CmdCopyImageToBuffer (const Command_t &cmd);
		bool _CmdUpdateBuffer (const Command_t &cmd);
		bool _CmdFillBuffer (const Command_t &cmd);
		bool _CmdClearColorImage (const Command_t &cmd);
		bool _CmdPipelineBarrier (const Command_t &cmd);
		bool _CmdPushConstants (const Command_t &cmd);
		bool _CmdPushNamedConstants (const Command_t &cmd);
	};
//-----------------------------------------------------------------------------



	const TypeIdList	SWCommandBuffer::_msgTypes{ UninitializedT< SupportedMessages_t >() };
	const TypeIdList	SWCommandBuffer::_eventTypes{ UninitializedT< SupportedEvents_t >() };

/*
=================================================
	constructor
=================================================
*/
	SWCommandBuffer::SWCommandBuffer (UntypedID_t id, GlobalSystemsRef gs, const CreateInfo::GpuCommandBuffer &ci) :
		SWBaseModule( gs, ModuleConfig{ id, UMax }, &_msgTypes, &_eventTypes ),
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
		_SubscribeOnMsg( this, &SWCommandBuffer::_GetCommandBufferDescriptor );
		_SubscribeOnMsg( this, &SWCommandBuffer::_GetDeviceInfo );
		_SubscribeOnMsg( this, &SWCommandBuffer::_GetSWDeviceInfo );
		_SubscribeOnMsg( this, &SWCommandBuffer::_GetSWPrivateClasses );
		_SubscribeOnMsg( this, &SWCommandBuffer::_SetSWCommandBufferQueue );
		_SubscribeOnMsg( this, &SWCommandBuffer::_SetCommandBufferDependency );
		_SubscribeOnMsg( this, &SWCommandBuffer::_SetCommandBufferState );
		_SubscribeOnMsg( this, &SWCommandBuffer::_GetCommandBufferState );
		_SubscribeOnMsg( this, &SWCommandBuffer::_ExecuteSWCommandBuffer );

		CHECK( _ValidateMsgSubscriptions() );

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
	bool SWCommandBuffer::_Compose (const Message< ModuleMsg::Compose > &msg)
	{
		if ( _IsComposedState( GetState() ) )
			return true;	// already composed

		CHECK_ERR( GetState() == EState::Linked );
		
		CHECK_COMPOSING( _Initialize() );

		_SendForEachAttachments( msg );
		
		// very paranoic check
		CHECK( _ValidateAllSubscriptions() );

		CHECK( _SetState( EState::ComposedMutable ) );
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
	bool SWCommandBuffer::_Delete (const Message< ModuleMsg::Delete > &msg)
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
	bool SWCommandBuffer::_SetSWCommandBufferQueue (const Message< GpuMsg::SetSWCommandBufferQueue > &msg)
	{
		_commands		= RVREF(msg->commands.Get());
		_bufferData		= msg->bufferData;
		_pushConstData	= msg->pushConstData;
		return true;
	}

/*
=================================================
	_GetCommandBufferDescriptor
=================================================
*/
	bool SWCommandBuffer::_GetCommandBufferDescriptor (const Message< GpuMsg::GetCommandBufferDescriptor > &msg)
	{
		msg->result.Set( _descr );
		return true;
	}
	
/*
=================================================
	_SetCommandBufferDependency
=================================================
*/
	bool SWCommandBuffer::_SetCommandBufferDependency (const Message< GpuMsg::SetCommandBufferDependency > &msg)
	{
		_resources = RVREF( msg->resources.Get() );
		return true;
	}

/*
=================================================
	_GetCommandBufferDescriptor
=================================================
*/
	bool SWCommandBuffer::_SetCommandBufferState (const Message< GpuMsg::SetCommandBufferState > &msg)
	{
		switch ( msg->newState )
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
	_GetCommandBufferDescriptor
=================================================
*/
	bool SWCommandBuffer::_GetCommandBufferState (const Message< GpuMsg::GetCommandBufferState > &msg)
	{
		msg->result.Set( _recordingState );
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
	bool SWCommandBuffer::_ExecuteSWCommandBuffer (const Message< GpuMsg::ExecuteSWCommandBuffer > &msg)
	{
		CHECK_ERR( _recordingState == ERecordingState::Pending );
		CHECK_ERR( msg->lastCmdIndex == 0 );

		msg->completed = false;

		FOR( i, _commands )
		{
			const auto&	data = _commands[i].data;

			switch ( data.GetCurrentIndex() )
			{
				case CmdDataTypes_t::IndexOf<GpuMsg::CmdBindComputePipeline> :		_CmdBindComputePipeline( data );		break;
				case CmdDataTypes_t::IndexOf<GpuMsg::CmdDispatch> :					_CmdDispatch( data );					break;
				case CmdDataTypes_t::IndexOf<GpuMsg::CmdExecute> :					_CmdExecute( data );					break;
				case CmdDataTypes_t::IndexOf<GpuMsg::CmdBindComputeResourceTable> :	_CmdBindComputeResourceTable( data );	break;
				case CmdDataTypes_t::IndexOf<GpuMsg::CmdCopyBuffer> :				_CmdCopyBuffer( data );					break;
				case CmdDataTypes_t::IndexOf<GpuMsg::CmdCopyImage> :				_CmdCopyImage( data );					break;
				case CmdDataTypes_t::IndexOf<GpuMsg::CmdCopyBufferToImage> :		_CmdCopyBufferToImage( data );			break;
				case CmdDataTypes_t::IndexOf<GpuMsg::CmdCopyImageToBuffer> :		_CmdCopyImageToBuffer( data );			break;
				case CmdDataTypes_t::IndexOf<GpuMsg::CmdUpdateBuffer> :				_CmdUpdateBuffer( data );				break;
				case CmdDataTypes_t::IndexOf<GpuMsg::CmdFillBuffer> :				_CmdFillBuffer( data );					break;
				case CmdDataTypes_t::IndexOf<GpuMsg::CmdClearColorImage> :			_CmdClearColorImage( data );			break;
				case CmdDataTypes_t::IndexOf<GpuMsg::CmdPipelineBarrier> :			_CmdPipelineBarrier( data );			break;
				case CmdDataTypes_t::IndexOf<GpuMsg::CmdPushConstants> :			_CmdPushConstants( data );				break;
				case CmdDataTypes_t::IndexOf<GpuMsg::CmdPushNamedConstants> :		_CmdPushNamedConstants( data );			break;
				default :															WARNING( "unknown command!" );
			}
		}

		msg->completed = true;

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

		_SendEvent< GpuMsg::OnCommandBufferStateChanged >({ old_state, newState });
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
	_CmdBindComputePipeline
=================================================
*/
	bool SWCommandBuffer::_CmdBindComputePipeline (const Command_t &cmd)
	{
		const auto&	data = cmd.data.Get< GpuMsg::CmdBindComputePipeline >();
		
		_computeShader = data.pipeline;
		return true;
	}
	
/*
=================================================
	_CmdDispatch
=================================================
*/
	bool SWCommandBuffer::_CmdDispatch (const Command_t &cmd)
	{
		const auto&	data = cmd.data.Get< GpuMsg::CmdDispatch >();
		
		_PrepareForCompute();

		CHECK_ERR( GetDevice()->DispatchCompute( data.groupCount, _computeShader, _computeResTable ) );
		return true;
	}
	
/*
=================================================
	_CmdExecute
=================================================
*/
	bool SWCommandBuffer::_CmdExecute (const Command_t &cmd)
	{
		const auto&	data = cmd.data.Get< GpuMsg::CmdExecute >();
		
		ModuleUtils::Send( data.cmdBuffers, Message<GpuMsg::SetCommandBufferState>{ GpuMsg::SetCommandBufferState::EState::Pending });
		ModuleUtils::Send( data.cmdBuffers, Message<GpuMsg::ExecuteSWCommandBuffer>{} );
		//ModuleUtils::Send( data.cmdBuffers, Message<GpuMsg::SetCommandBufferState>{ GpuMsg::SetCommandBufferState::EState::Completed });

		return true;
	}
	
/*
=================================================
	_CmdBindComputeResourceTable
=================================================
*/
	bool SWCommandBuffer::_CmdBindComputeResourceTable (const Command_t &cmd)
	{
		const auto&	data = cmd.data.Get< GpuMsg::CmdBindComputeResourceTable >();
		
		ASSERT( data.index == 0 );

		_computeResTable = data.resourceTable;
		return true;
	}
	
/*
=================================================
	_CmdCopyBuffer
=================================================
*/
	bool SWCommandBuffer::_CmdCopyBuffer (const Command_t &cmd)
	{
		const auto&	data = cmd.data.Get< GpuMsg::CmdCopyBuffer >();
		
		for (auto& reg : Range(data.regions))
		{
			Message< GpuMsg::GetSWBufferMemoryLayout >	req_src_mem { BytesU(reg.srcOffset), BytesU(reg.size), EPipelineAccess::TransferRead, EPipelineStage::Transfer };
			Message< GpuMsg::GetSWBufferMemoryLayout >	req_dst_mem { BytesU(reg.dstOffset), BytesU(reg.size), EPipelineAccess::TransferWrite, EPipelineStage::Transfer };

			data.srcBuffer->Send( req_src_mem );
			data.dstBuffer->Send( req_dst_mem );

			CHECK_ERR( req_src_mem->result and req_src_mem->result->memAccess[ EMemoryAccess::GpuRead ] );
			CHECK_ERR( req_dst_mem->result and req_dst_mem->result->memAccess[ EMemoryAccess::GpuWrite ] );
			
			BinArrayCRef	src_reg = req_src_mem->result->memory;
			BinArrayRef		dst_reg = req_dst_mem->result->memory;

			CHECK_ERR( src_reg.Size() == dst_reg.Size() );

			MemCopy( dst_reg, src_reg );
		}
		return true;
	}
	
/*
=================================================
	_CmdCopyImage
=================================================
*/
	bool SWCommandBuffer::_CmdCopyImage (const Command_t &cmd)
	{
		const auto&	data = cmd.data.Get< GpuMsg::CmdCopyImage >();
		
		Message< GpuMsg::GetSWImageMemoryLayout >	req_src_mem { EPipelineAccess::TransferRead, EPipelineStage::Transfer };
		Message< GpuMsg::GetSWImageMemoryLayout >	req_dst_mem { EPipelineAccess::TransferWrite, EPipelineStage::Transfer };
		Message< GpuMsg::GetImageDescriptor >		req_src_descr;
		Message< GpuMsg::GetImageDescriptor >		req_dst_descr;

		SendTo( data.srcImage, req_src_mem );
		SendTo( data.dstImage, req_dst_mem );
		SendTo( data.srcImage, req_src_descr );
		SendTo( data.dstImage, req_dst_descr );

		CHECK_ERR( req_src_mem->result and req_src_mem->result->memAccess[ EMemoryAccess::GpuRead ] );
		CHECK_ERR( req_dst_mem->result and req_dst_mem->result->memAccess[ EMemoryAccess::GpuWrite ] );
		CHECK_ERR( req_src_descr->result and req_src_descr->result->usage[ EImageUsage::TransferSrc ] );
		CHECK_ERR( req_dst_descr->result and req_dst_descr->result->usage[ EImageUsage::TransferDst ] );
		CHECK_ERR( req_src_descr->result->samples == req_dst_descr->result->samples );
		
		for (auto& reg : Range(data.regions))
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

			CHECK_ERR( src_off_z < req_src_mem->result->layers.Count() );
			CHECK_ERR( src_off_z + src_dim_z <= req_src_mem->result->layers.Count() );
			
			CHECK_ERR( dst_off_z < req_dst_mem->result->layers.Count() );
			CHECK_ERR( dst_off_z + dst_dim_z <= req_dst_mem->result->layers.Count() );
			
			for (uint z = 0; z < src_dim_z; ++z)
			{
				auto const&	src_layer	= req_src_mem->result->layers[ src_off_z + z ];
				auto&		dst_layer	= req_dst_mem->result->layers[ dst_off_z + z ];

				// find mipmap level
				CHECK_ERR( reg.srcLayers.mipLevel.Get() < src_layer.mipmaps.Count() );
				CHECK_ERR( reg.dstLayers.mipLevel.Get() < dst_layer.mipmaps.Count() );
			
				auto const&	src_level		= src_layer.mipmaps[ reg.srcLayers.mipLevel.Get() ];
				auto&		dst_level		= dst_layer.mipmaps[ reg.dstLayers.mipLevel.Get() ];
				BytesU		src_bpp			= BytesU(EPixelFormat::BitPerPixel( src_level.format ));
				BytesU		dst_bpp			= BytesU(EPixelFormat::BitPerPixel( dst_level.format ));
				BytesU		src_row_pitch	= GXImageUtils::AlignedRowSize( src_level.dimension.x, src_bpp, req_src_mem->result->align );
				BytesU		dst_row_pitch	= GXImageUtils::AlignedRowSize( dst_level.dimension.x, dst_bpp, req_dst_mem->result->align );
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
	_CmdCopyBufferToImage
=================================================
*/
	bool SWCommandBuffer::_CmdCopyBufferToImage (const Command_t &cmd)
	{
		const auto&	data = cmd.data.Get< GpuMsg::CmdCopyBufferToImage >();
		
		Message< GpuMsg::GetBufferDescriptor >		req_src_descr;
		Message< GpuMsg::GetImageDescriptor >		req_dst_descr;
		data.srcBuffer->Send( req_src_descr );
		data.dstImage->Send( req_dst_descr );
		
		CHECK_ERR( req_src_descr->result and req_src_descr->result->usage[ EBufferUsage::TransferSrc ] );
		CHECK_ERR( req_dst_descr->result and req_dst_descr->result->usage[ EImageUsage::TransferDst ] );

		BytesU	bpp	= BytesU(EPixelFormat::BitPerPixel( req_dst_descr->result->format ));

		for (auto& reg : Range(data.regions))
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
				Message< GpuMsg::GetSWImageViewMemoryLayout >	req_dst_mem { EPipelineAccess::TransferWrite, EPipelineStage::Transfer };
				req_dst_mem->viewDescr.viewType		= req_dst_descr->result->imageType;
				req_dst_mem->viewDescr.format		= req_dst_descr->result->format;
				req_dst_mem->viewDescr.baseLevel	= reg.imageLayers.mipLevel;
				req_dst_mem->viewDescr.baseLayer	= reg.imageLayers.baseLayer;
				data.dstImage->Send( req_dst_mem );

				CHECK_ERR( z + reg.imageOffset.z < req_dst_mem->result->layers.Count() );
				CHECK_ERR( reg.imageLayers.mipLevel.Get() < req_dst_mem->result->layers[z + reg.imageOffset.z].mipmaps.Count() );

				auto&	dst_layer		= req_dst_mem->result->layers[ z + reg.imageOffset.z ];
				auto&	dst_level		= dst_layer.mipmaps[ reg.imageLayers.mipLevel.Get() ];
				BytesU	dst_row_pitch	= GXImageUtils::AlignedRowSize( dst_level.dimension.x, bpp, req_dst_mem->result->align );

				for (uint y = 0; y < reg.imageSize.y; ++y)
				{
					const BytesU	dst_offset = reg.imageOffset.x * bpp + dst_row_pitch * (y + reg.imageOffset.y);
					const BytesU	src_offset	= BytesU(reg.bufferOffset) + z * src_slice_pitch + y * src_row_pitch;

					Message< GpuMsg::GetSWBufferMemoryLayout >	req_src_mem { src_offset, row_size, EPipelineAccess::TransferRead, EPipelineStage::Transfer };
					data.srcBuffer->Send( req_src_mem );

					SW_DEBUG_REPORT2( dst_level.layout == EImageLayout::TransferDstOptimal or dst_level.layout == EImageLayout::General, EDbgReport::Error );

					BinArrayRef		dst	= dst_level.Data().SubArray( usize(dst_offset), usize(row_size) );
					BinArrayCRef 	src	= req_src_mem->result->memory.SubArray( 0, usize(row_size) );

					MemCopy( OUT dst, src );
				}
			}
		}
		return true;
	}
	
/*
=================================================
	_CmdCopyImageToBuffer
=================================================
*/
	bool SWCommandBuffer::_CmdCopyImageToBuffer (const Command_t &cmd)
	{
		const auto&	data = cmd.data.Get< GpuMsg::CmdCopyImageToBuffer >();
		
		Message< GpuMsg::GetImageDescriptor >		req_src_descr;
		Message< GpuMsg::GetBufferDescriptor >		req_dst_descr;
		data.srcImage->Send( req_src_descr );
		data.dstBuffer->Send( req_dst_descr );
		
		CHECK_ERR( req_src_descr->result and req_src_descr->result->usage[ EImageUsage::TransferSrc ] );
		CHECK_ERR( req_dst_descr->result and req_dst_descr->result->usage[ EBufferUsage::TransferDst ] );

		BytesU	bpp	= BytesU(EPixelFormat::BitPerPixel( req_src_descr->result->format ));

		for (auto& reg : Range(data.regions))
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
				Message< GpuMsg::GetSWImageViewMemoryLayout >	req_src_mem { EPipelineAccess::TransferRead, EPipelineStage::Transfer };
				req_src_mem->viewDescr.viewType		= req_src_descr->result->imageType;
				req_src_mem->viewDescr.format		= req_src_descr->result->format;
				req_src_mem->viewDescr.baseLevel	= reg.imageLayers.mipLevel;
				req_src_mem->viewDescr.baseLayer	= reg.imageLayers.baseLayer;
				data.srcImage->Send( req_src_mem );

				CHECK_ERR( z + reg.imageOffset.z < req_src_mem->result->layers.Count() );
				CHECK_ERR( reg.imageLayers.mipLevel.Get() < req_src_mem->result->layers[z + reg.imageOffset.z].mipmaps.Count() );

				auto&	src_layer		= req_src_mem->result->layers[ z + reg.imageOffset.z ];
				auto&	src_level		= src_layer.mipmaps[ reg.imageLayers.mipLevel.Get() ];
				BytesU	src_row_pitch	= GXImageUtils::AlignedRowSize( src_level.dimension.x, bpp, req_src_mem->result->align );

				for (uint y = 0; y < reg.imageSize.y; ++y)
				{
					const BytesU	src_offset = reg.imageOffset.x * bpp + src_row_pitch * (y + reg.imageOffset.y);
					const BytesU	dst_offset	= BytesU(reg.bufferOffset) + z * dst_slice_pitch + y * dst_row_pitch;

					Message< GpuMsg::GetSWBufferMemoryLayout >	req_dst_mem { dst_offset, row_size, EPipelineAccess::TransferWrite, EPipelineStage::Transfer };
					data.dstBuffer->Send( req_dst_mem );

					SW_DEBUG_REPORT2( src_level.layout == EImageLayout::TransferSrcOptimal or src_level.layout == EImageLayout::General, EDbgReport::Error );

					BinArrayCRef	src	= src_level.Data().SubArray( usize(src_offset), usize(row_size) );
					BinArrayRef 	dst	= req_dst_mem->result->memory.SubArray( 0, usize(row_size) );

					MemCopy( OUT dst, src );
				}
			}
		}
		return true;
	}
	
/*
=================================================
	_CmdUpdateBuffer
=================================================
*/
	bool SWCommandBuffer::_CmdUpdateBuffer (const Command_t &cmd)
	{
		const auto&	data = cmd.data.Get< GpuMsg::CmdUpdateBuffer >();
		
		Message< GpuMsg::GetSWBufferMemoryLayout >	req_mem { BytesU(data.dstOffset), data.data.Size(), EPipelineAccess::TransferWrite, EPipelineStage::Transfer };
		Message< GpuMsg::GetBufferDescriptor >		req_descr;
		
		data.dstBuffer->Send( req_mem );
		data.dstBuffer->Send( req_descr );
		
		CHECK_ERR( data.dstOffset < req_descr->result->size );
		CHECK_ERR( req_descr->result->usage[ EBufferUsage::TransferDst ] );
		CHECK_ERR( req_mem->result->memAccess[ EMemoryAccess::GpuWrite ] );
		CHECK_ERR( (BytesU(data.dstOffset) % req_mem->result->align) == 0 );
		CHECK_ERR( req_mem->result->memory.Size() == data.data.Size() );

		MemCopy( OUT req_mem->result->memory, BinArrayCRef(data.data) );
		return true;
	}
	
/*
=================================================
	_CmdFillBuffer
=================================================
*/
	bool SWCommandBuffer::_CmdFillBuffer (const Command_t &cmd)
	{
		const auto&	data = cmd.data.Get< GpuMsg::CmdFillBuffer >();
		
		Message< GpuMsg::GetSWBufferMemoryLayout >	req_mem { BytesU(data.dstOffset), BytesU(data.size), EPipelineAccess::TransferWrite, EPipelineStage::Transfer };
		Message< GpuMsg::GetBufferDescriptor >		req_descr;

		data.dstBuffer->Send( req_mem );
		data.dstBuffer->Send( req_descr );
		
		CHECK_ERR( data.dstOffset < req_descr->result->size );
		CHECK_ERR( req_descr->result->usage[ EBufferUsage::TransferDst ] );
		CHECK_ERR( req_mem->result->memAccess[ EMemoryAccess::GpuWrite ] );
		CHECK_ERR( (BytesU(data.dstOffset) % req_mem->result->align) == 0 );
		CHECK_ERR( req_mem->result->memory.Size() == BytesU(data.size) );
		
		const ubyte	pattern[4]	= { data.pattern & 0xFF,
									(data.pattern >> 8) & 0xFF,
									(data.pattern >> 16) & 0xFF,
									(data.pattern >> 24) & 0xFF };

		for (usize i = 0; i < usize(data.size); ++i)
		{
			req_mem->result->memory[i] = pattern[i&3];
		}
		return true;
	}
	
/*
=================================================
	_CmdClearColorImage
=================================================
*/
	bool SWCommandBuffer::_CmdClearColorImage (const Command_t &cmd)
	{
		const auto&	data = cmd.data.Get< GpuMsg::CmdClearColorImage >();
		
		TODO( "" );
		return true;
	}
	
/*
=================================================
	_CmdPipelineBarrier
----
	https://gpuopen.com/vulkan-barriers-explained/
=================================================
*/
	bool SWCommandBuffer::_CmdPipelineBarrier (const Command_t &cmd)
	{
		const auto&	data = cmd.data.Get< GpuMsg::CmdPipelineBarrier >();

		for (auto& src : Range(data.imageBarriers))
		{
			CHECK_ERR( src.image );
			SendTo< GpuMsg::SWImageBarrier >( src.image, {src, data.srcStageMask, data.dstStageMask} );
		}

		for (auto& src : Range(data.bufferBarriers))
		{
			CHECK_ERR( src.buffer );
			SendTo< GpuMsg::SWBufferBarrier >( src.buffer, {src, data.srcStageMask, data.dstStageMask} );
		}
		
		ASSERT( data.memoryBarriers.Empty() );	// not supported yet

		return true;
	}
	
/*
=================================================
	_CmdPushConstants
=================================================
*/
	bool SWCommandBuffer::_CmdPushConstants (const Command_t &cmd)
	{
		const auto&	data = cmd.data.Get< GpuMsg::CmdPipelineBarrier >();
		
		TODO( "" );
		return true;
	}
	
/*
=================================================
	_CmdPushNamedConstants
=================================================
*/
	bool SWCommandBuffer::_CmdPushNamedConstants (const Command_t &cmd)
	{
		const auto&	data = cmd.data.Get< GpuMsg::CmdPushNamedConstants >();
		
		TODO( "" );
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
