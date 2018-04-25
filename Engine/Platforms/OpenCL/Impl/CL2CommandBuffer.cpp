// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Engine/Config/Engine.Config.h"

#ifdef COMPUTE_API_OPENCL

#include "Engine/Platforms/Public/GPU/CommandBuffer.h"
#include "Engine/Platforms/Public/GPU/Image.h"
#include "Engine/Platforms/Public/GPU/Buffer.h"
#include "Engine/Platforms/Public/GPU/Pipeline.h"
#include "Engine/Platforms/OpenCL/Impl/CL2BaseModule.h"
#include "Engine/Platforms/OpenCL/OpenCLObjectsConstructor.h"

namespace Engine
{
namespace PlatformCL
{
	using namespace cl;

	
	//
	// OpenCL Command Buffer
	//

	class CL2CommandBuffer final : public CL2BaseModule
	{
	// types
	private:
		using SupportedMessages_t	= CL2BaseModule::SupportedMessages_t::Append< MessageListFrom<
											GpuMsg::GetCommandBufferDescriptor,
											GpuMsg::SetCommandBufferState,
											GpuMsg::GetCommandBufferState,
											GpuMsg::SetCommandBufferDependency,
											GpuMsg::SetCLCommandBufferQueue,
											GpuMsg::ExecuteCLCommandBuffer
										> >;

		using SupportedEvents_t		= CL2BaseModule::SupportedEvents_t::Append< MessageListFrom<
											GpuMsg::OnCommandBufferStateChanged
										> >;

		using UsedResources_t		= Set< ModulePtr >;
		using Command_t				= GpuMsg::SetCLCommandBufferQueue::Command;
		using CommandArray_t		= Array< Command_t >;
		using CmdDataTypes_t		= GpuMsg::SetCLCommandBufferQueue::Data_t::TypeList_t;

		using ERecordingState		= GpuMsg::SetCommandBufferState::EState;


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
		usize3						_kernelLocalSize;
		ModulePtr					_resourceTable;

		ulong3						_maxThreads;
		ulong3						_maxLocalGroupSize;
		ulong3						_maxWorkGroupCount;
		ulong						_maxInvocations;
		bool						_isInitialized;


	// methods
	public:
		CL2CommandBuffer (UntypedID_t, GlobalSystemsRef gs, const CreateInfo::GpuCommandBuffer &ci);
		~CL2CommandBuffer ();


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



	const TypeIdList	CL2CommandBuffer::_msgTypes{ UninitializedT< SupportedMessages_t >() };
	const TypeIdList	CL2CommandBuffer::_eventTypes{ UninitializedT< SupportedEvents_t >() };

/*
=================================================
	constructor
=================================================
*/
	CL2CommandBuffer::CL2CommandBuffer (UntypedID_t id, GlobalSystemsRef gs, const CreateInfo::GpuCommandBuffer &ci) :
		CL2BaseModule( gs, ModuleConfig{ id, UMax }, &_msgTypes, &_eventTypes ),
		_descr( ci.descr ),			_recordingState( ERecordingState::Deleted ),
		_kernelId( null ),			_maxInvocations( 0 ),
		_isInitialized( false )
	{
		SetDebugName( "CL2CommandBuffer" );

		_SubscribeOnMsg( this, &CL2CommandBuffer::_OnModuleAttached_Impl );
		_SubscribeOnMsg( this, &CL2CommandBuffer::_OnModuleDetached_Impl );
		_SubscribeOnMsg( this, &CL2CommandBuffer::_AttachModule_Impl );
		_SubscribeOnMsg( this, &CL2CommandBuffer::_DetachModule_Impl );
		_SubscribeOnMsg( this, &CL2CommandBuffer::_FindModule_Impl );
		_SubscribeOnMsg( this, &CL2CommandBuffer::_ModulesDeepSearch_Impl );
		_SubscribeOnMsg( this, &CL2CommandBuffer::_Link_Impl );
		_SubscribeOnMsg( this, &CL2CommandBuffer::_Compose );
		_SubscribeOnMsg( this, &CL2CommandBuffer::_Delete );
		_SubscribeOnMsg( this, &CL2CommandBuffer::_OnManagerChanged );
		_SubscribeOnMsg( this, &CL2CommandBuffer::_GetCommandBufferDescriptor );
		_SubscribeOnMsg( this, &CL2CommandBuffer::_GetDeviceInfo );
		_SubscribeOnMsg( this, &CL2CommandBuffer::_GetCLDeviceInfo );
		_SubscribeOnMsg( this, &CL2CommandBuffer::_GetCLPrivateClasses );
		_SubscribeOnMsg( this, &CL2CommandBuffer::_SetCLCommandBufferQueue );
		_SubscribeOnMsg( this, &CL2CommandBuffer::_SetCommandBufferDependency );
		_SubscribeOnMsg( this, &CL2CommandBuffer::_SetCommandBufferState );
		_SubscribeOnMsg( this, &CL2CommandBuffer::_GetCommandBufferState );
		_SubscribeOnMsg( this, &CL2CommandBuffer::_ExecuteCLCommandBuffer );

		CHECK( _ValidateMsgSubscriptions() );

		_AttachSelfToManager( _GetGPUThread( ci.gpuThread ), UntypedID_t(0), true );
	}
	
/*
=================================================
	destructor
=================================================
*/
	CL2CommandBuffer::~CL2CommandBuffer ()
	{
	}
	
/*
=================================================
	_Compose
=================================================
*/
	bool CL2CommandBuffer::_Compose (const Message< ModuleMsg::Compose > &msg)
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
	bool CL2CommandBuffer::_Initialize ()
	{
		cl_uint		value32  = 0;
		size_t		size	 = 0;
		usize3		size3;

		CL_CALL( clGetDeviceInfo( GetCLDevice(), CL_DEVICE_MAX_WORK_GROUP_SIZE, sizeof(size), OUT &size, null ) );
		CL_CALL( clGetDeviceInfo( GetCLDevice(), CL_DEVICE_MAX_WORK_ITEM_SIZES, sizeof(size3), OUT size3.ptr(), null ) );
		CL_CALL( clGetDeviceInfo( GetCLDevice(), CL_DEVICE_ADDRESS_BITS, sizeof(value32), OUT &value32, null ) );
		
		_maxInvocations		= ulong( size );
		_maxLocalGroupSize	= ulong3( size3 );
		_maxThreads			= ToMask< ulong3 >( BitsU(value32) );
		_maxWorkGroupCount	= _maxThreads / _maxLocalGroupSize;
		
		_ChangeState( ERecordingState::Initial );
		return true;
	}

/*
=================================================
	_Delete
=================================================
*/
	bool CL2CommandBuffer::_Delete (const Message< ModuleMsg::Delete > &msg)
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
	bool CL2CommandBuffer::_SetCLCommandBufferQueue (const Message< GpuMsg::SetCLCommandBufferQueue > &msg)
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
	bool CL2CommandBuffer::_GetCommandBufferDescriptor (const Message< GpuMsg::GetCommandBufferDescriptor > &msg)
	{
		msg->result.Set( _descr );
		return true;
	}
	
/*
=================================================
	_SetCommandBufferDependency
=================================================
*/
	bool CL2CommandBuffer::_SetCommandBufferDependency (const Message< GpuMsg::SetCommandBufferDependency > &msg)
	{
		_resources = RVREF( msg->resources.Get() );
		return true;
	}

/*
=================================================
	_GetCommandBufferDescriptor
=================================================
*/
	bool CL2CommandBuffer::_SetCommandBufferState (const Message< GpuMsg::SetCommandBufferState > &msg)
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
	bool CL2CommandBuffer::_GetCommandBufferState (const Message< GpuMsg::GetCommandBufferState > &msg)
	{
		msg->result.Set( _recordingState );
		return true;
	}
	
/*
=================================================
	_ResetCmdBuffer
=================================================
*/
	bool CL2CommandBuffer::_ResetCmdBuffer ()
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
	bool CL2CommandBuffer::_Submit ()
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
	bool CL2CommandBuffer::_ExecuteCLCommandBuffer (const Message< GpuMsg::ExecuteCLCommandBuffer > &)
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
	bool CL2CommandBuffer::_BeginRecording ()
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
	bool CL2CommandBuffer::_EndRecording ()
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
	bool CL2CommandBuffer::_OnCompleted ()
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
	void CL2CommandBuffer::_ChangeState (ERecordingState newState)
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
	void CL2CommandBuffer::_ClearStates ()
	{
		_kernelId			= null;
		_kernelLocalSize	= usize3();
		_resourceTable		= null;
	}
	
/*
=================================================
	_DestroyResources
=================================================
*/
	bool CL2CommandBuffer::_DestroyResources ()
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
	bool CL2CommandBuffer::_CmdBindComputePipeline (const Command_t &cmd)
	{
		const auto&	data = cmd.data.Get< GpuMsg::CmdBindComputePipeline >();

		Message< GpuMsg::GetCLComputePipelineID >		req_id;
		Message< GpuMsg::GetComputePipelineDescriptor >	req_descr;

		data.pipeline->Send( req_id );
		data.pipeline->Send( req_descr );

		CHECK_ERR( req_id->result and req_descr->result );

		_kernelId			= req_id->result->kernel;
		_kernelLocalSize	= usize3(req_descr->result->localGroupSize);

		return true;
	}
	
/*
=================================================
	_CmdDispatch
=================================================
*/
	bool CL2CommandBuffer::_CmdDispatch (const Command_t &cmd)
	{
		const auto&		data		= cmd.data.Get< GpuMsg::CmdDispatch >();

		const usize3	global_size	= Max( usize3(data.groupCount), 1u ) *
									  Max( _kernelLocalSize, 1u );

		CHECK_ERR( All( ulong3(data.groupCount) <= _maxWorkGroupCount ) );
		CHECK_ERR( All( ulong3(global_size) <= _maxThreads ) );

		if ( _resourceTable )
			CHECK( _resourceTable->Send< GpuMsg::CLPipelineResourceTableApply >({ _kernelId }) );
		
		CL_CALL( clEnqueueNDRangeKernel(
						GetCommandQueue(),
						_kernelId,
						3,
						null,
						global_size.ptr(),
						_kernelLocalSize.ptr(),
						0, null,
						null ) );
		return true;
	}
	
/*
=================================================
	_CmdExecute
=================================================
*/
	bool CL2CommandBuffer::_CmdExecute (const Command_t &cmd)
	{
		const auto&	data = cmd.data.Get< GpuMsg::CmdExecute >();
		
		ModuleUtils::Send( data.cmdBuffers, Message<GpuMsg::SetCommandBufferState>{ GpuMsg::SetCommandBufferState::EState::Pending });
		ModuleUtils::Send( data.cmdBuffers, Message<GpuMsg::ExecuteCLCommandBuffer>{} );
		//ModuleUtils::Send( data.cmdBuffers, Message<GpuMsg::SetCommandBufferState>{ GpuMsg::SetCommandBufferState::EState::Completed });

		return true;
	}
	
/*
=================================================
	_CmdBindComputeResourceTable
=================================================
*/
	bool CL2CommandBuffer::_CmdBindComputeResourceTable (const Command_t &cmd)
	{
		const auto&	data = cmd.data.Get< GpuMsg::CmdBindComputeResourceTable >();
		
		ASSERT( data.index == 0 );
		_resourceTable = data.resourceTable;
		return true;
	}
	
/*
=================================================
	_CmdCopyBuffer
=================================================
*/
	bool CL2CommandBuffer::_CmdCopyBuffer (const Command_t &cmd)
	{
		const auto&	data = cmd.data.Get< GpuMsg::CmdCopyBuffer >();

		Message< GpuMsg::GetCLBufferID >		req_src_id;
		Message< GpuMsg::GetCLBufferID >		req_dst_id;
		Message< GpuMsg::GetBufferDescriptor >	req_src_descr;
		Message< GpuMsg::GetBufferDescriptor >	req_dst_descr;

		data.srcBuffer->Send( req_src_id );
		data.dstBuffer->Send( req_dst_id );
		data.srcBuffer->Send( req_src_descr );
		data.dstBuffer->Send( req_dst_descr );

		cl_mem		src_id		= req_src_id->result.Get(null);
		cl_mem		dst_id		= req_dst_id->result.Get(null);
		BytesUL		src_size	= req_src_descr->result->size;
		BytesUL		dst_size	= req_dst_descr->result->size;

		for (auto& reg : Range(data.regions))
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
		return true;
	}
	
/*
=================================================
	_CmdCopyImage
=================================================
*/
	bool CL2CommandBuffer::_CmdCopyImage (const Command_t &cmd)
	{
		const auto&	data = cmd.data.Get< GpuMsg::CmdCopyImage >();
		
		Message< GpuMsg::GetCLImageID >			req_src_id;
		Message< GpuMsg::GetCLImageID >			req_dst_id;
		Message< GpuMsg::GetImageDescriptor >	req_src_descr;
		Message< GpuMsg::GetImageDescriptor >	req_dst_descr;
		
		data.srcImage->Send( req_src_id );
		data.dstImage->Send( req_dst_id );
		data.srcImage->Send( req_src_descr );
		data.dstImage->Send( req_dst_descr );
		
		cl_mem			src_id		= req_src_id->result.Get(null);
		cl_mem			dst_id		= req_dst_id->result.Get(null);
		ImageDescriptor	src_descr	= req_src_descr->result.Get();
		ImageDescriptor	dst_descr	= req_dst_descr->result.Get();
		
		for (auto& reg : Range(data.regions))
		{
			CHECK_ERR( reg.srcLayers.mipLevel.Get() == 0 and reg.srcLayers.aspectMask[EImageAspect::Color] );
			CHECK_ERR( reg.dstLayers.mipLevel.Get() == 0 and reg.dstLayers.aspectMask[EImageAspect::Color] );

			uint4	img_src_offset	= uint4( reg.srcOffset, reg.srcLayers.baseLayer.Get() );
			uint4	img_src_size	= uint4( reg.size, reg.srcLayers.layerCount );
			uint4	img_dst_offset	= uint4( reg.dstOffset, reg.dstLayers.baseLayer.Get() );
			uint4	img_dst_size	= uint4( reg.size, reg.dstLayers.layerCount );

			usize3	src_offset		= ImageUtils::ConvertOffset( src_descr.imageType, usize4(img_src_offset) );
			usize3	src_size		= Max( ImageUtils::ConvertSize( src_descr.imageType, usize4(img_src_size) ), 1u );
			usize3	dst_offset		= ImageUtils::ConvertOffset( dst_descr.imageType, usize4(img_dst_offset) );
			usize3	dst_size		= Max( ImageUtils::ConvertSize( dst_descr.imageType, usize4(img_dst_size) ), 1u );
			
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
		return true;
	}
	
/*
=================================================
	_CmdCopyBufferToImage
=================================================
*/
	bool CL2CommandBuffer::_CmdCopyBufferToImage (const Command_t &cmd)
	{
		const auto&	data = cmd.data.Get< GpuMsg::CmdCopyBufferToImage >();
		
		Message< GpuMsg::GetCLBufferID >		req_src_id;
		Message< GpuMsg::GetCLImageID >			req_dst_id;
		Message< GpuMsg::GetBufferDescriptor >	req_src_descr;
		Message< GpuMsg::GetImageDescriptor >	req_dst_descr;

		data.srcBuffer->Send( req_src_id );
		data.dstImage->Send( req_dst_id );
		data.srcBuffer->Send( req_src_descr );
		data.dstImage->Send( req_dst_descr );
		
		cl_mem		src_id		= req_src_id->result.Get(null);
		cl_mem		dst_id		= req_dst_id->result.Get(null);
		BytesUL		src_size	= req_src_descr->result->size;
		BytesUL		bpp			= BytesUL( EPixelFormat::BitPerPixel( req_dst_descr->result->format ) );
		uint		depth		= req_dst_descr->result->dimension.z;

		for (auto& reg : Range(data.regions))
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
					for (uint y = 0; y < reg.imageSize.y; ++y)
					{
						usize4	img_offset	= usize4( reg.imageOffset.x, y + reg.imageOffset.y, z + reg.imageOffset.z, layer + reg.imageLayers.baseLayer.Get() );
						usize3	dst_offset	= usize3( img_offset.xy(), img_offset.z + img_offset.w * depth );
						usize3	dst_size	= usize3( reg.imageSize.x, 1, 1 );
						BytesUL	src_off		= reg.bufferOffset + layer * src_array_pitch + z * src_depth_pitch + y * src_row_pitch;
						
						CHECK_ERR( src_off + row_size <= src_size );
						CHECK_ERR(All( img_offset + usize4(dst_size.x, 1, 1, 1) <= usize4(req_dst_descr->result->dimension) ));

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
		return true;
	}
	
/*
=================================================
	_CmdCopyImageToBuffer
=================================================
*/
	bool CL2CommandBuffer::_CmdCopyImageToBuffer (const Command_t &cmd)
	{
		const auto&	data = cmd.data.Get< GpuMsg::CmdCopyImageToBuffer >();
		
		Message< GpuMsg::GetCLImageID >			req_src_id;
		Message< GpuMsg::GetCLBufferID >		req_dst_id;
		Message< GpuMsg::GetImageDescriptor >	req_src_descr;
		Message< GpuMsg::GetBufferDescriptor >	req_dst_descr;

		data.srcImage->Send( req_src_id );
		data.dstBuffer->Send( req_dst_id );
		data.srcImage->Send( req_src_descr );
		data.dstBuffer->Send( req_dst_descr );
		
		cl_mem		src_id		= req_src_id->result.Get(null);
		cl_mem		dst_id		= req_dst_id->result.Get(null);
		BytesUL		dst_size	= req_dst_descr->result->size;
		BytesUL		bpp			= BytesUL( EPixelFormat::BitPerPixel( req_src_descr->result->format ) );
		uint		depth		= req_src_descr->result->dimension.z;

		for (auto& reg : Range(data.regions))
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
					for (uint y = 0; y < reg.imageSize.y; ++y)
					{
						usize4	img_offset	= usize4( reg.imageOffset.x, y + reg.imageOffset.y, z + reg.imageOffset.z, layer + reg.imageLayers.baseLayer.Get() );
						usize3	src_offset	= usize3( img_offset.xy(), img_offset.z + img_offset.w * depth );
						usize3	src_size	= usize3( reg.imageSize.x, 1, 1 );
						BytesUL	dst_off		= reg.bufferOffset + layer * dst_array_pitch + z * dst_depth_pitch + y * dst_row_pitch;
						
						CHECK_ERR( dst_off + row_size <= dst_size );
						CHECK_ERR(All( img_offset + usize4(src_size.x, 1, 1, 1) <= usize4(req_src_descr->result->dimension) ));

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
		return true;
	}
	
/*
=================================================
	_CmdUpdateBuffer
=================================================
*/
	bool CL2CommandBuffer::_CmdUpdateBuffer (const Command_t &cmd)
	{
		const auto&	data = cmd.data.Get< GpuMsg::CLCmdUpdateBuffer >();
		
		Message< GpuMsg::GetCLBufferID >		req_dst_id;
		Message< GpuMsg::GetCLBufferID >		req_src_id;
		Message< GpuMsg::GetBufferDescriptor >	req_descr;
		
		data.dstBuffer->Send( req_dst_id );
		data.dstBuffer->Send( req_descr );
		_tempBuffer->Send( req_src_id );
		
		CHECK_ERR( data.dstOffset < req_descr->result->size );
		CHECK_ERR( data.size + data.dstOffset <= req_descr->result->size );

		CL_CHECK( clEnqueueCopyBuffer(
					GetCommandQueue(),
					*req_src_id->result,
					*req_dst_id->result,
					(size_t) data.srcOffset,
					(size_t) data.dstOffset,
					(size_t) data.size,
					0, null,
					null ) );
		return true;
	}
	
/*
=================================================
	_CmdFillBuffer
=================================================
*/
	bool CL2CommandBuffer::_CmdFillBuffer (const Command_t &cmd)
	{
		const auto&	data = cmd.data.Get< GpuMsg::CmdFillBuffer >();

		Message< GpuMsg::GetCLBufferID >		req_id;
		Message< GpuMsg::GetBufferDescriptor >	req_descr;

		data.dstBuffer->Send( req_id );
		data.dstBuffer->Send( req_descr );
		
		CHECK_ERR( data.dstOffset < req_descr->result->size );
		CHECK_ERR( req_descr->result->usage[ EBufferUsage::TransferDst ] );

		const BytesUL	size = Min( req_descr->result->size - data.dstOffset, data.size );
		
		CL_CHECK( clEnqueueFillBuffer(
					GetCommandQueue(),
					*req_id->result,
					&data.pattern,
					sizeof(data.pattern),
					size_t(data.dstOffset),
					size_t(size),
					0, null,
					null ) );

		return true;
	}
	
/*
=================================================
	_CmdClearColorImage
=================================================
*/
	bool CL2CommandBuffer::_CmdClearColorImage (const Command_t &cmd)
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
	bool CL2CommandBuffer::_CmdPipelineBarrier (const Command_t &cmd)
	{
		const auto&	data = cmd.data.Get< GpuMsg::CmdPipelineBarrier >();
		
		CL_CALL( clEnqueueMarkerWithWaitList( GetCommandQueue(), 0, null, null ) );
		return true;
	}
	
/*
=================================================
	_CmdPushConstants
=================================================
*/
	bool CL2CommandBuffer::_CmdPushConstants (const Command_t &cmd)
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
	bool CL2CommandBuffer::_CmdPushNamedConstants (const Command_t &cmd)
	{
		const auto&	data = cmd.data.Get< GpuMsg::CmdPushNamedConstants >();
		
		TODO( "" );
		return true;
	}

}	// PlatformCL
//-----------------------------------------------------------------------------

namespace Platforms
{
	ModulePtr OpenCLObjectsConstructor::CreateCL2CommandBuffer (ModuleMsg::UntypedID_t id, GlobalSystemsRef gs, const CreateInfo::GpuCommandBuffer &ci)
	{
		return New< PlatformCL::CL2CommandBuffer >( id, gs, ci );
	}
}	// Platforms
}	// Engine

#endif	// COMPUTE_API_OPENCL
