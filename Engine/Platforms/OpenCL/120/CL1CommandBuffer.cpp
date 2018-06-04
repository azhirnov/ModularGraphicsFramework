// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Engine/Config/Engine.Config.h"

#ifdef COMPUTE_API_OPENCL

#include "Engine/Platforms/Public/GPU/CommandBuffer.h"
#include "Engine/Platforms/Public/GPU/Image.h"
#include "Engine/Platforms/Public/GPU/Buffer.h"
#include "Engine/Platforms/Public/GPU/Pipeline.h"
#include "Engine/Platforms/OpenCL/120/CL1BaseModule.h"
#include "Engine/Platforms/OpenCL/120/CL1ResourceCache.h"
#include "Engine/Platforms/OpenCL/OpenCLObjectsConstructor.h"
#include "Engine/Platforms/Public/Tools/ImageUtils.h"

namespace Engine
{
namespace PlatformCL
{
	using namespace cl;

	
	//
	// OpenCL Command Buffer
	//

	class CL1CommandBuffer final : public CL1BaseModule
	{
	// types
	private:
		using SupportedMessages_t	= CL1BaseModule::SupportedMessages_t::Append< MessageListFrom<
											GpuMsg::GetCommandBufferDescriptor,
											GpuMsg::SetCommandBufferState,
											GpuMsg::GetCommandBufferState,
											GpuMsg::SetCommandBufferDependency,
											GpuMsg::SetCLCommandBufferQueue,
											GpuMsg::ExecuteCLCommandBuffer
										> >;

		using SupportedEvents_t		= CL1BaseModule::SupportedEvents_t::Append< MessageListFrom<
											GpuMsg::OnCommandBufferStateChanged
										> >;

		using UsedResources_t		= Set< ModulePtr >;
		using Command_t				= GpuMsg::SetCLCommandBufferQueue::Command;
		using CommandArray_t		= Array< Command_t >;
		using CmdDataTypes_t		= GpuMsg::SetCLCommandBufferQueue::Data_t::TypeList_t;

		using ERecordingState		= GpuMsg::SetCommandBufferState::EState;
		using ImageUtils			= PlatformTools::ImageUtils;


	// constants
	private:
		static const TypeIdList		_msgTypes;
		static const TypeIdList		_eventTypes;


	// variables
	private:
		ModulePtr					_tempBuffer;		// for UpdateBuffer command
		ModulePtr					_pushConstBuffer;	// for PushConstant command

		CommandBufferDescriptor		_descr;
		CommandArray_t				_commands;
		UsedResources_t				_resources;
		ERecordingState				_recordingState;

		cl_kernel					_kernelId;
		uint3						_kernelLocalSize;
		ModulePtr					_resourceTable;

		bool						_isInitialized;


	// methods
	public:
		CL1CommandBuffer (UntypedID_t, GlobalSystemsRef gs, const CreateInfo::GpuCommandBuffer &ci);
		~CL1CommandBuffer ();


	// message handlers
	private:
		bool _Compose (const Message< ModuleMsg::Compose > &);
		bool _Delete (const Message< ModuleMsg::Delete > &);
		bool _SetCLCommandBufferQueue (const Message< GpuMsg::SetCLCommandBufferQueue > &);
		bool _GetCommandBufferDescriptor (const Message< GpuMsg::GetCommandBufferDescriptor > &);
		bool _SetCommandBufferDependency (const Message< GpuMsg::SetCommandBufferDependency > &);
		bool _SetCommandBufferState (const Message< GpuMsg::SetCommandBufferState > &);
		bool _GetCommandBufferState (const Message< GpuMsg::GetCommandBufferState > &);
		bool _ExecuteCLCommandBuffer (const Message< GpuMsg::ExecuteCLCommandBuffer > &);
		
	private:
		bool _Submit ();
		bool _BeginRecording ();
		bool _EndRecording ();
		bool _ResetCmdBuffer ();
		bool _OnCompleted ();

		void _ChangeState (ERecordingState newState);

		bool _Initialize ();
		void _ClearStates ();
		bool _DestroyResources ();
		
		bool _CmdBindComputePipeline (const Command_t &cmdData);
		bool _CmdDispatch (const Command_t &cmdData);
		bool _CmdExecute (const Command_t &cmdData);
		bool _CmdBindComputeResourceTable (const Command_t &cmdData);
		bool _CmdCopyBuffer (const Command_t &cmdData);
		bool _CmdCopyImage (const Command_t &cmdData);
		bool _CmdCopyBufferToImage (const Command_t &cmdData);
		bool _CmdCopyImageToBuffer (const Command_t &cmdData);
		bool _CmdUpdateBuffer (const Command_t &cmdData);
		bool _CmdFillBuffer (const Command_t &cmdData);
		bool _CmdClearColorImage (const Command_t &cmdData);
		bool _CmdPipelineBarrier (const Command_t &cmdData);
		bool _CmdPushConstants (const Command_t &cmdData);
		bool _CmdPushNamedConstants (const Command_t &cmdData);
		bool _CmdDebugMarker (const Command_t &cmdData);
		bool _CmdPushDebugGroup (const Command_t &cmdData);
		bool _CmdPopDebugGroup (const Command_t &cmdData);
	};
//-----------------------------------------------------------------------------



	const TypeIdList	CL1CommandBuffer::_msgTypes{ UninitializedT< SupportedMessages_t >() };
	const TypeIdList	CL1CommandBuffer::_eventTypes{ UninitializedT< SupportedEvents_t >() };

/*
=================================================
	constructor
=================================================
*/
	CL1CommandBuffer::CL1CommandBuffer (UntypedID_t id, GlobalSystemsRef gs, const CreateInfo::GpuCommandBuffer &ci) :
		CL1BaseModule( gs, ModuleConfig{ id, UMax }, &_msgTypes, &_eventTypes ),
		_descr( ci.descr ),			_recordingState( ERecordingState::Deleted ),
		_kernelId( null ),			_isInitialized( false )
	{
		SetDebugName( "CL1CommandBuffer" );

		_SubscribeOnMsg( this, &CL1CommandBuffer::_OnModuleAttached_Impl );
		_SubscribeOnMsg( this, &CL1CommandBuffer::_OnModuleDetached_Impl );
		_SubscribeOnMsg( this, &CL1CommandBuffer::_AttachModule_Impl );
		_SubscribeOnMsg( this, &CL1CommandBuffer::_DetachModule_Impl );
		_SubscribeOnMsg( this, &CL1CommandBuffer::_FindModule_Impl );
		_SubscribeOnMsg( this, &CL1CommandBuffer::_ModulesDeepSearch_Impl );
		_SubscribeOnMsg( this, &CL1CommandBuffer::_Link_Impl );
		_SubscribeOnMsg( this, &CL1CommandBuffer::_Compose );
		_SubscribeOnMsg( this, &CL1CommandBuffer::_Delete );
		_SubscribeOnMsg( this, &CL1CommandBuffer::_OnManagerChanged );
		_SubscribeOnMsg( this, &CL1CommandBuffer::_GetCommandBufferDescriptor );
		_SubscribeOnMsg( this, &CL1CommandBuffer::_GetDeviceInfo );
		_SubscribeOnMsg( this, &CL1CommandBuffer::_GetCLDeviceInfo );
		_SubscribeOnMsg( this, &CL1CommandBuffer::_GetCLPrivateClasses );
		_SubscribeOnMsg( this, &CL1CommandBuffer::_SetCLCommandBufferQueue );
		_SubscribeOnMsg( this, &CL1CommandBuffer::_SetCommandBufferDependency );
		_SubscribeOnMsg( this, &CL1CommandBuffer::_SetCommandBufferState );
		_SubscribeOnMsg( this, &CL1CommandBuffer::_GetCommandBufferState );
		_SubscribeOnMsg( this, &CL1CommandBuffer::_ExecuteCLCommandBuffer );

		CHECK( _ValidateMsgSubscriptions() );

		_AttachSelfToManager( _GetGPUThread( ci.gpuThread ), UntypedID_t(0), true );
	}
	
/*
=================================================
	destructor
=================================================
*/
	CL1CommandBuffer::~CL1CommandBuffer ()
	{
	}
	
/*
=================================================
	_Compose
=================================================
*/
	bool CL1CommandBuffer::_Compose (const Message< ModuleMsg::Compose > &msg)
	{
		if ( _IsComposedState( GetState() ) )
			return true;	// already composed

		CHECK_ERR( GetState() == EState::Linked );
		
		CHECK_COMPOSING( _Initialize() );

		_SendForEachAttachments( msg );
		
		// very paranoic check
		CHECK( _ValidateAllSubscriptions() );

		CHECK( _SetState( EState::ComposedMutable ) );
		
		_SendUncheckedEvent< ModuleMsg::AfterCompose >({});
		return true;
	}
	
/*
=================================================
	_Initialize
=================================================
*/
	bool CL1CommandBuffer::_Initialize ()
	{
		_ChangeState( ERecordingState::Initial );
		return true;
	}

/*
=================================================
	_Delete
=================================================
*/
	bool CL1CommandBuffer::_Delete (const Message< ModuleMsg::Delete > &msg)
	{
		_descr = Uninitialized;

		_ChangeState( ERecordingState::Deleted );

		_resources.Clear();
		_commands.Clear();
		
		_DestroyResources ();

		return Module::_Delete_Impl( msg );
	}

/*
=================================================
	_SetCLCommandBufferQueue
=================================================
*/
	bool CL1CommandBuffer::_SetCLCommandBufferQueue (const Message< GpuMsg::SetCLCommandBufferQueue > &msg)
	{
		_commands = RVREF(msg->commands.Get());
		
		// TODO: optimize
		if ( _tempBuffer )
		{
			_tempBuffer->Send< ModuleMsg::Delete >({});
			_tempBuffer = null;
		}

		if ( not msg->bufferData.Empty() )
		{
			CHECK_ERR( GlobalSystems()->modulesFactory->Create(
								CLBufferModuleID,
								GlobalSystems(),
								CreateInfo::GpuBuffer{
									BufferDescriptor{ msg->bufferData.Size(), EBufferUsage::TransferSrc },
									null,
									EGpuMemory::CoherentWithCPU,
									EMemoryAccess::GpuRead | EMemoryAccess::CpuWrite
								},
								OUT _tempBuffer ) );

			ModuleUtils::Initialize({ _tempBuffer });

			CHECK_ERR( _tempBuffer->Send< GpuMsg::WriteToGpuMemory >({ msg->bufferData }) );
		}
		return true;
	}

/*
=================================================
	_GetCommandBufferDescriptor
=================================================
*/
	bool CL1CommandBuffer::_GetCommandBufferDescriptor (const Message< GpuMsg::GetCommandBufferDescriptor > &msg)
	{
		msg->result.Set( _descr );
		return true;
	}
	
/*
=================================================
	_SetCommandBufferDependency
=================================================
*/
	bool CL1CommandBuffer::_SetCommandBufferDependency (const Message< GpuMsg::SetCommandBufferDependency > &msg)
	{
		_resources = RVREF( msg->resources.Get() );
		return true;
	}

/*
=================================================
	_GetCommandBufferDescriptor
=================================================
*/
	bool CL1CommandBuffer::_SetCommandBufferState (const Message< GpuMsg::SetCommandBufferState > &msg)
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
	bool CL1CommandBuffer::_GetCommandBufferState (const Message< GpuMsg::GetCommandBufferState > &msg)
	{
		msg->result.Set( _recordingState );
		return true;
	}
	
/*
=================================================
	_ResetCmdBuffer
=================================================
*/
	bool CL1CommandBuffer::_ResetCmdBuffer ()
	{
		if ( _recordingState == ERecordingState::Initial )
			return true;

		CHECK_ERR( _recordingState == ERecordingState::Pending );

		_ChangeState( ERecordingState::Initial );
		_resources.Clear();
		_commands.Clear();

		return true;
	}

/*
=================================================
	_Submit
=================================================
*/
	bool CL1CommandBuffer::_Submit ()
	{
		CHECK_ERR( _recordingState == ERecordingState::Executable );

		_ChangeState( ERecordingState::Pending );
		return true;
	}
	
/*
=================================================
	_ExecuteCLCommandBuffer
=================================================
*/
	bool CL1CommandBuffer::_ExecuteCLCommandBuffer (const Message< GpuMsg::ExecuteCLCommandBuffer > &)
	{
		CHECK_ERR( _recordingState == ERecordingState::Pending );

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
				case CmdDataTypes_t::IndexOf<GpuMsg::CLCmdUpdateBuffer> :			_CmdUpdateBuffer( data );				break;
				case CmdDataTypes_t::IndexOf<GpuMsg::CmdFillBuffer> :				_CmdFillBuffer( data );					break;
				case CmdDataTypes_t::IndexOf<GpuMsg::CmdClearColorImage> :			_CmdClearColorImage( data );			break;
				case CmdDataTypes_t::IndexOf<GpuMsg::CmdPipelineBarrier> :			_CmdPipelineBarrier( data );			break;
				case CmdDataTypes_t::IndexOf<GpuMsg::CmdPushConstants> :			_CmdPushConstants( data );				break;
				case CmdDataTypes_t::IndexOf<GpuMsg::CmdPushNamedConstants> :		_CmdPushNamedConstants( data );			break;
				case CmdDataTypes_t::IndexOf<GpuMsg::CmdDebugMarker> :				_CmdDebugMarker( data );				break;
				case CmdDataTypes_t::IndexOf<GpuMsg::CmdPushDebugGroup> :			_CmdPushDebugGroup( data );				break;
				case CmdDataTypes_t::IndexOf<GpuMsg::CmdPopDebugGroup> :			_CmdPopDebugGroup( data );				break;
				default :															WARNING( "unknown command!" );
			}
		}

		_ClearStates();
		return true;
	}
	
/*
=================================================
	_BeginRecording
=================================================
*/
	bool CL1CommandBuffer::_BeginRecording ()
	{
		CHECK_ERR( _recordingState == ERecordingState::Initial or
				  ((_recordingState == ERecordingState::Executable or
					_recordingState == ERecordingState::Pending) and
				    _descr.flags[ECmdBufferCreate::ImplicitResetable]) );

		if ( _recordingState == ERecordingState::Pending )
		{
			_resources.Clear();
			_commands.Clear();
		}

		_ChangeState( ERecordingState::Recording );
		return true;
	}
	
/*
=================================================
	_EndRecording
=================================================
*/
	bool CL1CommandBuffer::_EndRecording ()
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
	bool CL1CommandBuffer::_OnCompleted ()
	{
		CHECK_ERR( _recordingState == ERecordingState::Pending );

		_resources.Clear();
		_commands.Clear();

		_ChangeState( ERecordingState::Executable );
		return true;
	}
		
/*
=================================================
	_ChangeState
=================================================
*/
	void CL1CommandBuffer::_ChangeState (ERecordingState newState)
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
	void CL1CommandBuffer::_ClearStates ()
	{
		_kernelId			= null;
		_kernelLocalSize	= Uninitialized;
		_resourceTable		= null;
	}
	
/*
=================================================
	_DestroyResources
=================================================
*/
	bool CL1CommandBuffer::_DestroyResources ()
	{
		if ( _tempBuffer )
		{
			_tempBuffer->Send< ModuleMsg::Delete >({});
			_tempBuffer = null;
		}
		return true;
	}

/*
=================================================
	_CmdBindComputePipeline
=================================================
*/
	bool CL1CommandBuffer::_CmdBindComputePipeline (const Command_t &cmdData)
	{
		const auto&	cmd			= cmdData.data.Get< GpuMsg::CmdBindComputePipeline >();
		const auto&	ppln_res	= GetResourceCache()->GetPipelineID( cmd.pipeline );

		_kernelId			= ppln_res.Get<0>().kernel;
		_kernelLocalSize	= ppln_res.Get<1>().localGroupSize;

		return true;
	}
	
/*
=================================================
	_CmdDispatch
=================================================
*/
	bool CL1CommandBuffer::_CmdDispatch (const Command_t &cmdData)
	{
		const auto&		cmd				= cmdData.data.Get< GpuMsg::CmdDispatch >();

		const usize3	global_size		= usize3(Max( cmd.groupCount, 1u )) * usize3(_kernelLocalSize);
		const uint		invocations		= _kernelLocalSize.Volume();

		const uint3		max_group_count	= GetDevice()->GetProperties().maxComputeWorkGroupCount;
		const uint3		max_local_size	= GetDevice()->GetProperties().maxComputeWorkGroupSize;
		const uint		max_invocations	= GetDevice()->GetProperties().maxComputeWorkGroupInvocations;

		CHECK_ERR( invocations <= max_invocations );
		CHECK_ERR( All( _kernelLocalSize <= max_local_size ) );
		CHECK_ERR( All( cmd.groupCount <= max_group_count ) );

		if ( _resourceTable )
			CHECK( _resourceTable->Send< GpuMsg::CLBindPipelineResourceTable >({ _kernelId }) );
		
		CHECK( GetDevice()->AcquireSharedObj( _resourceTable ) );

		CL_CALL( clEnqueueNDRangeKernel(
						GetCommandQueue(),
						_kernelId,
						_kernelLocalSize.Count(),
						null,
						global_size.ptr(),
						usize3(_kernelLocalSize).ptr(),
						0, null,
						null ) );
		
		CHECK( GetDevice()->ReleaseSharedObj( _resourceTable ) );
		return true;
	}
	
/*
=================================================
	_CmdExecute
=================================================
*/
	bool CL1CommandBuffer::_CmdExecute (const Command_t &cmdData)
	{
		const auto&	cmd = cmdData.data.Get< GpuMsg::CmdExecute >();
		
		ModuleUtils::Send( cmd.cmdBuffers, Message<GpuMsg::SetCommandBufferState>{ GpuMsg::SetCommandBufferState::EState::Pending });
		ModuleUtils::Send( cmd.cmdBuffers, Message<GpuMsg::ExecuteCLCommandBuffer>{} );
		//ModuleUtils::Send( cmd.cmdBuffers, Message<GpuMsg::SetCommandBufferState>{ GpuMsg::SetCommandBufferState::EState::Completed });

		return true;
	}
	
/*
=================================================
	_CmdBindComputeResourceTable
=================================================
*/
	bool CL1CommandBuffer::_CmdBindComputeResourceTable (const Command_t &cmdData)
	{
		const auto&	cmd = cmdData.data.Get< GpuMsg::CmdBindComputeResourceTable >();
		
		ASSERT( cmd.index == 0 );
		_resourceTable = cmd.resourceTable;
		return true;
	}
	
/*
=================================================
	_CmdCopyBuffer
=================================================
*/
	bool CL1CommandBuffer::_CmdCopyBuffer (const Command_t &cmdData)
	{
		const auto&	cmd = cmdData.data.Get< GpuMsg::CmdCopyBuffer >();

		const auto	src_res		= GetResourceCache()->GetBufferID( cmd.srcBuffer );
		const auto	dst_res		= GetResourceCache()->GetBufferID( cmd.dstBuffer );

		cl_mem		src_id		= src_res.Get<0>().id;
		cl_mem		dst_id		= dst_res.Get<0>().id;
		BytesUL		src_size	= src_res.Get<1>().size;
		BytesUL		dst_size	= dst_res.Get<1>().size;

		CHECK( GetDevice()->AddSharedObj( this, cmd.srcBuffer ) );
		CHECK( GetDevice()->AddSharedObj( this, cmd.dstBuffer ) );
		CHECK( GetDevice()->AcquireSharedObj( this ) );

		for (auto& reg : cmd.regions)
		{
			CHECK_ERR( reg.srcOffset + reg.size <= src_size );
			CHECK_ERR( reg.dstOffset + reg.size <= dst_size );
			
			CL_CHECK( clEnqueueCopyBuffer(
						GetCommandQueue(),
						src_id,
						dst_id,
						(size_t) reg.srcOffset,
						(size_t) reg.dstOffset,
						(size_t) reg.size,
						0, null,
						null ) );
		}

		CHECK( GetDevice()->ReleaseSharedObj( this ) );
		return true;
	}
	
/*
=================================================
	_CmdCopyImage
=================================================
*/
	bool CL1CommandBuffer::_CmdCopyImage (const Command_t &cmdData)
	{
		const auto&	cmd = cmdData.data.Get< GpuMsg::CmdCopyImage >();
		
		const auto	src_res	= GetResourceCache()->GetImageID( cmd.srcImage );
		const auto	dst_res	= GetResourceCache()->GetImageID( cmd.dstImage );
		cl_mem		src_id	= src_res.Get<0>().id;
		cl_mem		dst_id	= dst_res.Get<0>().id;
		
		CHECK( GetDevice()->AddSharedObj( this, cmd.srcImage ) );
		CHECK( GetDevice()->AddSharedObj( this, cmd.dstImage ) );
		CHECK( GetDevice()->AcquireSharedObj( this ) );

		for (auto& reg : cmd.regions)
		{
			CHECK_ERR( reg.srcLayers.mipLevel.Get() == 0 and reg.srcLayers.aspectMask[EImageAspect::Color] );
			CHECK_ERR( reg.dstLayers.mipLevel.Get() == 0 and reg.dstLayers.aspectMask[EImageAspect::Color] );

			uint4	img_src_offset	= uint4( reg.srcOffset, reg.srcLayers.baseLayer.Get() );
			uint4	img_src_size	= uint4( reg.size,      reg.srcLayers.layerCount );
			uint4	img_dst_offset	= uint4( reg.dstOffset, reg.dstLayers.baseLayer.Get() );
			uint4	img_dst_size	= uint4( reg.size,      reg.dstLayers.layerCount );

			usize3	src_offset		= ImageUtils::ConvertOffset( src_res.Get<1>().imageType, usize4(img_src_offset) );
			usize3	src_size		= Max( ImageUtils::ConvertSize( src_res.Get<1>().imageType, usize4(img_src_size) ), 1u );
			usize3	dst_offset		= ImageUtils::ConvertOffset( dst_res.Get<1>().imageType, usize4(img_dst_offset) );
			usize3	dst_size		= Max( ImageUtils::ConvertSize( dst_res.Get<1>().imageType, usize4(img_dst_size) ), 1u );
			
			CHECK_ERR( All( src_size == dst_size ) );

			CL_CHECK( clEnqueueCopyImage(
						GetCommandQueue(),
						src_id,
						dst_id,
						src_offset.ptr(),
						dst_offset.ptr(),
						dst_size.ptr(),
						0, null,
						null ) );
		}

		CHECK( GetDevice()->ReleaseSharedObj( this ) );
		return true;
	}
	
/*
=================================================
	_CmdCopyBufferToImage
=================================================
*/
	bool CL1CommandBuffer::_CmdCopyBufferToImage (const Command_t &cmdData)
	{
		const auto&	cmd = cmdData.data.Get< GpuMsg::CmdCopyBufferToImage >();
		
		const auto	src_res		= GetResourceCache()->GetBufferID( cmd.srcBuffer );
		const auto	dst_res		= GetResourceCache()->GetImageID( cmd.dstImage );
		
		cl_mem		src_id		= src_res.Get<0>().id;
		cl_mem		dst_id		= dst_res.Get<0>().id;
		BytesUL		src_size	= src_res.Get<1>().size;
		BytesUL		bpp			= BytesUL( EPixelFormat::BitPerPixel( dst_res.Get<1>().format ) );
		uint		depth		= dst_res.Get<1>().dimension.z;
		
		CHECK( GetDevice()->AddSharedObj( this, cmd.srcBuffer ) );
		CHECK( GetDevice()->AddSharedObj( this, cmd.dstImage ) );
		CHECK( GetDevice()->AcquireSharedObj( this ) );

		for (auto& reg : cmd.regions)
		{
			CHECK_ERR( reg.imageLayers.mipLevel.Get() == 0 and reg.imageLayers.aspectMask[EImageAspect::Color] );

			BytesUL	row_size		= reg.imageSize.x * bpp;
			BytesUL	src_row_pitch	= reg.bufferRowLength * bpp;
			BytesUL	src_depth_pitch	= reg.bufferImageHeight * src_row_pitch;
			BytesUL	src_array_pitch	= src_depth_pitch;	// TODO
			
			for (uint layer = 0; layer < reg.imageLayers.layerCount; ++layer)
			{
				for (uint z = 0; z < reg.imageSize.z; ++z)
				{
					for (uint y = 0; y < reg.imageSize.y; ++y)	// TODO
					{
						usize4	img_offset	= usize4( reg.imageOffset.x, y + reg.imageOffset.y, z + reg.imageOffset.z, layer + reg.imageLayers.baseLayer.Get() );
						usize3	dst_offset	= usize3( img_offset.xy(), img_offset.z + img_offset.w * depth );
						usize3	dst_size	= usize3( reg.imageSize.x, 1, 1 );
						BytesUL	src_off		= reg.bufferOffset + layer * src_array_pitch + z * src_depth_pitch + y * src_row_pitch;
						
						CHECK_ERR( src_off + row_size <= src_size );
						CHECK_ERR(All( img_offset + usize4(dst_size.x, 1, 1, 1) <= usize4(dst_res.Get<1>().dimension) ));

						CL_CHECK( clEnqueueCopyBufferToImage(
									GetCommandQueue(),
									src_id,
									dst_id,
									size_t(src_off),
									dst_offset.ptr(),
									dst_size.ptr(),
									0, null,
									null ) );
					}
				}
			}
		}

		CHECK( GetDevice()->ReleaseSharedObj( this ) );
		return true;
	}
	
/*
=================================================
	_CmdCopyImageToBuffer
=================================================
*/
	bool CL1CommandBuffer::_CmdCopyImageToBuffer (const Command_t &cmdData)
	{
		const auto&	cmd = cmdData.data.Get< GpuMsg::CmdCopyImageToBuffer >();
		
		const auto	src_res		= GetResourceCache()->GetImageID( cmd.srcImage );
		const auto	dst_res		= GetResourceCache()->GetBufferID( cmd.dstBuffer );
		
		cl_mem		src_id		= src_res.Get<0>().id;
		cl_mem		dst_id		= dst_res.Get<0>().id;
		BytesUL		dst_size	= dst_res.Get<1>().size;
		BytesUL		bpp			= BytesUL( EPixelFormat::BitPerPixel( src_res.Get<1>().format ) );
		uint		depth		= src_res.Get<1>().dimension.z;
		
		CHECK( GetDevice()->AddSharedObj( this, cmd.srcImage ) );
		CHECK( GetDevice()->AddSharedObj( this, cmd.dstBuffer ) );
		CHECK( GetDevice()->AcquireSharedObj( this ) );

		for (auto& reg : cmd.regions)
		{
			CHECK_ERR( reg.imageLayers.mipLevel.Get() == 0 and reg.imageLayers.aspectMask[EImageAspect::Color] );

			BytesUL	row_size		= reg.imageSize.x * bpp;
			BytesUL	dst_row_pitch	= reg.bufferRowLength * bpp;
			BytesUL	dst_depth_pitch	= reg.bufferImageHeight * dst_row_pitch;
			BytesUL	dst_array_pitch	= dst_depth_pitch;	// TODO
			
			for (uint layer = 0; layer < reg.imageLayers.layerCount; ++layer)
			{
				for (uint z = 0; z < reg.imageSize.z; ++z)
				{
					for (uint y = 0; y < reg.imageSize.y; ++y)	// TODO
					{
						usize4	img_offset	= usize4( reg.imageOffset.x, y + reg.imageOffset.y, z + reg.imageOffset.z, layer + reg.imageLayers.baseLayer.Get() );
						usize3	src_offset	= usize3( img_offset.xy(), img_offset.z + img_offset.w * depth );
						usize3	src_size	= usize3( reg.imageSize.x, 1, 1 );
						BytesUL	dst_off		= reg.bufferOffset + layer * dst_array_pitch + z * dst_depth_pitch + y * dst_row_pitch;
						
						CHECK_ERR( dst_off + row_size <= dst_size );
						CHECK_ERR(All( img_offset + usize4(src_size.x, 1, 1, 1) <= usize4(src_res.Get<1>().dimension) ));

						CL_CHECK( clEnqueueCopyImageToBuffer(
									GetCommandQueue(),
									src_id,
									dst_id,
									src_offset.ptr(),
									src_size.ptr(),
									size_t(dst_off),
									0, null,
									null ) );
					}
				}
			}
		}

		CHECK( GetDevice()->ReleaseSharedObj( this ) );
		return true;
	}
	
/*
=================================================
	_CmdUpdateBuffer
=================================================
*/
	bool CL1CommandBuffer::_CmdUpdateBuffer (const Command_t &cmdData)
	{
		const auto&	cmd = cmdData.data.Get< GpuMsg::CLCmdUpdateBuffer >();
		
		const auto	src_res	= GetResourceCache()->GetBufferID( _tempBuffer );
		const auto	dst_res	= GetResourceCache()->GetBufferID( cmd.dstBuffer );
		
		CHECK_ERR( cmd.dstOffset < dst_res.Get<1>().size );
		CHECK_ERR( cmd.size + cmd.dstOffset <= dst_res.Get<1>().size );
		
		CHECK( GetDevice()->AddSharedObj( this, cmd.dstBuffer ) );
		CHECK( GetDevice()->AcquireSharedObj( this ) );

		CL_CHECK( clEnqueueCopyBuffer(
					GetCommandQueue(),
					src_res.Get<0>().id,
					dst_res.Get<0>().id,
					size_t(cmd.srcOffset),
					size_t(cmd.dstOffset),
					size_t(cmd.size),
					0, null,
					null ) );
		
		CHECK( GetDevice()->ReleaseSharedObj( this ) );
		return true;
	}
	
/*
=================================================
	_CmdFillBuffer
=================================================
*/
	bool CL1CommandBuffer::_CmdFillBuffer (const Command_t &cmdData)
	{
		const auto&	cmd = cmdData.data.Get< GpuMsg::CmdFillBuffer >();
		
		const auto&	dst_res	= GetResourceCache()->GetBufferID( cmd.dstBuffer );
		
		CHECK_ERR( cmd.dstOffset < dst_res.Get<1>().size );
		CHECK_ERR( dst_res.Get<1>().usage[ EBufferUsage::TransferDst ] );

		const BytesUL	size = Min( dst_res.Get<1>().size - cmd.dstOffset, cmd.size );
		
		CHECK( GetDevice()->AddSharedObj( this, cmd.dstBuffer ) );
		CHECK( GetDevice()->AcquireSharedObj( this ) );

		CL_CHECK( clEnqueueFillBuffer(
					GetCommandQueue(),
					dst_res.Get<0>().id,
					&cmd.pattern,
					sizeof(cmd.pattern),
					size_t(cmd.dstOffset),
					size_t(size),
					0, null,
					null ) );
		
		CHECK( GetDevice()->ReleaseSharedObj( this ) );
		return true;
	}
	
/*
=================================================
	_CmdClearColorImage
=================================================
*/
	bool CL1CommandBuffer::_CmdClearColorImage (const Command_t &cmdData)
	{
		const auto&	cmd = cmdData.data.Get< GpuMsg::CmdClearColorImage >();
		
		TODO( "" );
		return true;
	}
	
/*
=================================================
	_CmdPipelineBarrier
=================================================
*/
	bool CL1CommandBuffer::_CmdPipelineBarrier (const Command_t &cmdData)
	{
		const auto&	cmd = cmdData.data.Get< GpuMsg::CmdPipelineBarrier >();
		
		CL_CALL( clEnqueueMarkerWithWaitList( GetCommandQueue(), 0, null, null ) );
		return true;
	}
	
/*
=================================================
	_CmdPushConstants
=================================================
*/
	bool CL1CommandBuffer::_CmdPushConstants (const Command_t &cmdData)
	{
		const auto&	cmd = cmdData.data.Get< GpuMsg::CmdPushConstants >();
		
		TODO( "" );
		return true;
	}
	
/*
=================================================
	_CmdPushNamedConstants
=================================================
*/
	bool CL1CommandBuffer::_CmdPushNamedConstants (const Command_t &cmdData)
	{
		const auto&	cmd = cmdData.data.Get< GpuMsg::CmdPushNamedConstants >();
		
		TODO( "" );
		return true;
	}
	
/*
=================================================
	_CmdDebugMarker
=================================================
*/
	bool CL1CommandBuffer::_CmdDebugMarker (const Command_t &cmdData)
	{
		const auto&	cmd = cmdData.data.Get< GpuMsg::CmdDebugMarker >();
		
		LOG( "Debug marker: "_str << cmd.info, ELog::Debug );

		if ( cmd.breakPoint ) {
			GX_BREAK_POINT();
		}
		return true;
	}

/*
=================================================
	_CmdPushDebugGroup
=================================================
*/
	bool CL1CommandBuffer::_CmdPushDebugGroup (const Command_t &cmdData)
	{
		const auto&	cmd = cmdData.data.Get< GpuMsg::CmdPushDebugGroup >();
		
		LOG( "===================================================================\nDebug group begin: "_str << cmd.info, ELog::Debug );
		return true;
	}

/*
=================================================
	_CmdPopDebugGroup
=================================================
*/
	bool CL1CommandBuffer::_CmdPopDebugGroup (const Command_t &cmdData)
	{
		const auto&	cmd = cmdData.data.Get< GpuMsg::CmdPopDebugGroup >();
		
		LOG( "Debug group end\n===================================================================", ELog::Debug );
		return true;
	}

}	// PlatformCL
//-----------------------------------------------------------------------------

namespace Platforms
{
	ModulePtr OpenCLObjectsConstructor::CreateCL1CommandBuffer (ModuleMsg::UntypedID_t id, GlobalSystemsRef gs, const CreateInfo::GpuCommandBuffer &ci)
	{
		return New< PlatformCL::CL1CommandBuffer >( id, gs, ci );
	}
}	// Platforms
}	// Engine

#endif	// COMPUTE_API_OPENCL
