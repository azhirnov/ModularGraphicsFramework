// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Engine/Platforms/Shared/GPU/CommandBuffer.h"
#include "Engine/Platforms/Shared/GPU/Image.h"
#include "Engine/Platforms/Shared/GPU/Buffer.h"
#include "Engine/Platforms/Shared/GPU/Pipeline.h"
#include "Engine/Platforms/Soft/Impl/SWBaseModule.h"
#include "Engine/Platforms/Soft/Impl/SWShader.h"
#include "Engine/Platforms/Soft/SoftRendererObjectsConstructor.h"

#ifdef GRAPHICS_API_SOFT

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

		SWShader					_helper;

		// states
		ModulePtr					_computeShader;
		ModulePtr					_computeResTable;


	// methods
	public:
		SWCommandBuffer (GlobalSystemsRef gs, const CreateInfo::GpuCommandBuffer &ci);
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
	SWCommandBuffer::SWCommandBuffer (GlobalSystemsRef gs, const CreateInfo::GpuCommandBuffer &ci) :
		SWBaseModule( gs, ModuleConfig{ SWCommandBufferModuleID, UMax }, &_msgTypes, &_eventTypes ),
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

		CHECK_ERR( _helper.DispatchCompute( data.groupCount, _computeShader, _computeResTable ) );
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
		
		Message< GpuMsg::GetSWBufferMemoryLayout >	req_src_mem;
		Message< GpuMsg::GetSWBufferMemoryLayout >	req_dst_mem;

		data.srcBuffer->Send( req_src_mem );
		data.dstBuffer->Send( req_dst_mem );

		CHECK_ERR( req_src_mem->result and req_src_mem->result->access[ EMemoryAccess::GpuRead ] );
		CHECK_ERR( req_dst_mem->result and req_dst_mem->result->access[ EMemoryAccess::GpuWrite ] );

		BinArrayCRef	src = req_src_mem->result->memory;
		BinArrayRef		dst = req_dst_mem->result->memory;

		for (auto& reg : Range(data.regions))
		{
			MemCopy( dst.SubArray( usize(reg.dstOffset), usize(reg.size) ),
					 src.SubArray( usize(reg.srcOffset), usize(reg.size) ) );
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
		
		Message< GpuMsg::GetSWImageMemoryLayout >	req_src_mem;
		Message< GpuMsg::GetSWImageMemoryLayout >	req_dst_mem;
		Message< GpuMsg::GetImageDescriptor >		req_src_descr;
		Message< GpuMsg::GetImageDescriptor >		req_dst_descr;

		SendTo( data.srcImage, req_src_mem );
		SendTo( data.dstImage, req_dst_mem );
		SendTo( data.srcImage, req_src_descr );
		SendTo( data.dstImage, req_dst_descr );

		CHECK_ERR( req_src_mem->result and req_src_mem->result->access[ EMemoryAccess::GpuRead ] );
		CHECK_ERR( req_dst_mem->result and req_dst_mem->result->access[ EMemoryAccess::GpuWrite ] );
		CHECK_ERR( req_src_descr->result and req_src_descr->result->usage[ EImageUsage::TransferSrc ] );
		CHECK_ERR( req_dst_descr->result and req_dst_descr->result->usage[ EImageUsage::TransferDst ] );

		for (auto& reg : Range(data.regions))
		{
			// TODO
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
		
		TODO( "" );
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
		
		TODO( "" );
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
		
		Message< GpuMsg::GetSWBufferMemoryLayout >	req_mem;
		Message< GpuMsg::GetBufferDescriptor >		req_descr;
		
		data.dstBuffer->Send( req_mem );
		data.dstBuffer->Send( req_descr );
		
		CHECK_ERR( data.dstOffset < req_descr->result->size );
		CHECK_ERR( req_descr->result->usage[ EBufferUsage::TransferDst ] );
		CHECK_ERR( req_mem->result->access[ EMemoryAccess::GpuWrite ] );
		CHECK_ERR( (BytesU(data.dstOffset) % req_mem->result->align) == 0 );

		MemCopy( OUT req_mem->result->memory.SubArray( usize(data.dstOffset), data.data.Count() ), BinArrayCRef(data.data) );
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
		
		Message< GpuMsg::GetSWBufferMemoryLayout >	req_mem;
		Message< GpuMsg::GetBufferDescriptor >		req_descr;

		data.dstBuffer->Send( req_mem );
		data.dstBuffer->Send( req_descr );
		
		CHECK_ERR( data.dstOffset < req_descr->result->size );
		CHECK_ERR( req_descr->result->usage[ EBufferUsage::TransferDst ] );
		CHECK_ERR( req_mem->result->access[ EMemoryAccess::GpuWrite ] );
		CHECK_ERR( (BytesU(data.dstOffset) % req_mem->result->align) == 0 );
		
		const ubyte	pattern[4]	= { data.pattern & 0xFF,
									(data.pattern >> 8) & 0xFF,
									(data.pattern >> 16) & 0xFF,
									(data.pattern >> 24) & 0xFF };

		for (usize i = 0; i < usize(data.size); ++i)
		{
			req_mem->result->memory[i + usize(data.dstOffset)] = pattern[i&3];
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
=================================================
*/
	bool SWCommandBuffer::_CmdPipelineBarrier (const Command_t &cmd)
	{
		const auto&	data = cmd.data.Get< GpuMsg::CmdPipelineBarrier >();
		
		TODO( "" );
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
	ModulePtr SoftRendererObjectsConstructor::CreateSWCommandBuffer (GlobalSystemsRef gs, const CreateInfo::GpuCommandBuffer &ci)
	{
		return New< PlatformSW::SWCommandBuffer >( gs, ci );
	}
}	// Platforms
}	// Engine

#endif	// GRAPHICS_API_SOFT
