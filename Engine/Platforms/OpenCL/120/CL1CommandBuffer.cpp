// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Core/Config/Engine.Config.h"

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
											GpuMsg::GetCommandBufferDescription,
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
		static const TypeIdList		_eventTypes;


	// variables
	private:
		ModulePtr					_tempBuffer;		// for UpdateBuffer command
		ModulePtr					_pushConstBuffer;	// for PushConstant command

		CommandBufferDescription		_descr;
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
		bool _Compose (const ModuleMsg::Compose &);
		bool _Delete (const ModuleMsg::Delete &);
		bool _SetCLCommandBufferQueue (const GpuMsg::SetCLCommandBufferQueue &);
		bool _GetCommandBufferDescription (const GpuMsg::GetCommandBufferDescription &);
		bool _SetCommandBufferDependency (const GpuMsg::SetCommandBufferDependency &);
		bool _SetCommandBufferState (const GpuMsg::SetCommandBufferState &);
		bool _GetCommandBufferState (const GpuMsg::GetCommandBufferState &);
		bool _ExecuteCLCommandBuffer (const GpuMsg::ExecuteCLCommandBuffer &);
		
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
		
	public:
		bool operator () (const GpuMsg::CmdBindComputePipeline &);
		bool operator () (const GpuMsg::CmdDispatch &);
		bool operator () (const GpuMsg::CmdExecute &);
		bool operator () (const GpuMsg::CmdBindComputeResourceTable &);
		bool operator () (const GpuMsg::CmdCopyBuffer &);
		bool operator () (const GpuMsg::CmdCopyImage &);
		bool operator () (const GpuMsg::CmdCopyBufferToImage &);
		bool operator () (const GpuMsg::CmdCopyImageToBuffer &);
		bool operator () (const GpuMsg::CLCmdUpdateBuffer &);
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



	const TypeIdList	CL1CommandBuffer::_eventTypes{ UninitializedT< SupportedEvents_t >() };

/*
=================================================
	constructor
=================================================
*/
	CL1CommandBuffer::CL1CommandBuffer (UntypedID_t id, GlobalSystemsRef gs, const CreateInfo::GpuCommandBuffer &ci) :
		CL1BaseModule( gs, ModuleConfig{ id, UMax }, &_eventTypes ),
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
		_SubscribeOnMsg( this, &CL1CommandBuffer::_GetCommandBufferDescription );
		_SubscribeOnMsg( this, &CL1CommandBuffer::_GetDeviceInfo );
		_SubscribeOnMsg( this, &CL1CommandBuffer::_GetCLDeviceInfo );
		_SubscribeOnMsg( this, &CL1CommandBuffer::_GetCLPrivateClasses );
		_SubscribeOnMsg( this, &CL1CommandBuffer::_SetCLCommandBufferQueue );
		_SubscribeOnMsg( this, &CL1CommandBuffer::_SetCommandBufferDependency );
		_SubscribeOnMsg( this, &CL1CommandBuffer::_SetCommandBufferState );
		_SubscribeOnMsg( this, &CL1CommandBuffer::_GetCommandBufferState );
		_SubscribeOnMsg( this, &CL1CommandBuffer::_ExecuteCLCommandBuffer );

		ASSERT( _ValidateMsgSubscriptions< SupportedMessages_t >() );

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
	bool CL1CommandBuffer::_Compose (const ModuleMsg::Compose &msg)
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
	bool CL1CommandBuffer::_Delete (const ModuleMsg::Delete &msg)
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
	bool CL1CommandBuffer::_SetCLCommandBufferQueue (const GpuMsg::SetCLCommandBufferQueue &msg)
	{
		_commands = RVREF(msg.commands.Get());
		
		// TODO: optimize
		if ( _tempBuffer )
		{
			_tempBuffer->Send( ModuleMsg::Delete{} );
			_tempBuffer = null;
		}

		if ( not msg.bufferData.Empty() )
		{
			CHECK_ERR( GlobalSystems()->modulesFactory->Create(
								CLBufferModuleID,
								GlobalSystems(),
								CreateInfo::GpuBuffer{
									BufferDescription{ msg.bufferData.Size(), EBufferUsage::TransferSrc },
									null,
									EGpuMemory::CoherentWithCPU,
									EMemoryAccess::GpuRead | EMemoryAccess::CpuWrite
								},
								OUT _tempBuffer ) );

			ModuleUtils::Initialize({ _tempBuffer });

			CHECK( _tempBuffer->Send( GpuMsg::WriteToGpuMemory{ msg.bufferData }) );
		}
		return true;
	}

/*
=================================================
	_GetCommandBufferDescription
=================================================
*/
	bool CL1CommandBuffer::_GetCommandBufferDescription (const GpuMsg::GetCommandBufferDescription &msg)
	{
		msg.result.Set( _descr );
		return true;
	}
	
/*
=================================================
	_SetCommandBufferDependency
=================================================
*/
	bool CL1CommandBuffer::_SetCommandBufferDependency (const GpuMsg::SetCommandBufferDependency &msg)
	{
		_resources = RVREF( msg.resources.Get() );
		return true;
	}

/*
=================================================
	_GetCommandBufferDescription
=================================================
*/
	bool CL1CommandBuffer::_SetCommandBufferState (const GpuMsg::SetCommandBufferState &msg)
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
	bool CL1CommandBuffer::_GetCommandBufferState (const GpuMsg::GetCommandBufferState &msg)
	{
		msg.result.Set( _recordingState );
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
	bool CL1CommandBuffer::_ExecuteCLCommandBuffer (const GpuMsg::ExecuteCLCommandBuffer &)
	{
		CHECK_ERR( _recordingState == ERecordingState::Pending );
		
		FOR( i, _commands ) {
			_commands[i].data.Accept( *this );
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

		_SendEvent( GpuMsg::OnCommandBufferStateChanged{ old_state, newState });
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
			_tempBuffer->Send( ModuleMsg::Delete{} );
			_tempBuffer = null;
		}
		return true;
	}

/*
=================================================
	operator (CmdBindComputePipeline)
=================================================
*/
	bool CL1CommandBuffer::operator () (const GpuMsg::CmdBindComputePipeline &msg)
	{
		const auto&	ppln_res	= GetResourceCache()->GetPipelineID( msg.pipeline );

		_kernelId			= ppln_res.Get<0>().kernel;
		_kernelLocalSize	= ppln_res.Get<1>().localGroupSize;

		return true;
	}
	
/*
=================================================
	operator (CmdDispatch)
=================================================
*/
	bool CL1CommandBuffer::operator () (const GpuMsg::CmdDispatch &msg)
	{
		const usize3	global_size		= usize3(Max( msg.groupCount, 1u )) * usize3(_kernelLocalSize);
		const uint		invocations		= _kernelLocalSize.Volume();

		const uint3		max_group_count	= GetDevice()->GetProperties().maxComputeWorkGroupCount;
		const uint3		max_local_size	= GetDevice()->GetProperties().maxComputeWorkGroupSize;
		const uint		max_invocations	= GetDevice()->GetProperties().maxComputeWorkGroupInvocations;

		CHECK_ERR( invocations <= max_invocations );
		CHECK_ERR( All( _kernelLocalSize <= max_local_size ) );
		CHECK_ERR( All( msg.groupCount <= max_group_count ) );

		if ( _resourceTable )
			CHECK( _resourceTable->Send( GpuMsg::CLBindPipelineResourceTable{ _kernelId }) );
		
		CHECK( GetDevice()->AcquireSharedObj( _resourceTable ) );

		CL_CALL( clEnqueueNDRangeKernel(
						GetCommandQueue(),
						_kernelId,
						cl_uint(_kernelLocalSize.Count()),
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
	operator (CmdExecute)
=================================================
*/
	bool CL1CommandBuffer::operator () (const GpuMsg::CmdExecute &msg)
	{
		ModuleUtils::Send( msg.cmdBuffers, GpuMsg::SetCommandBufferState{ GpuMsg::SetCommandBufferState::EState::Pending });
		ModuleUtils::Send( msg.cmdBuffers, GpuMsg::ExecuteCLCommandBuffer{} );
		//ModuleUtils::Send( msg.cmdBuffers, GpuMsg::SetCommandBufferState{ GpuMsg::SetCommandBufferState::EState::Completed });

		return true;
	}
	
/*
=================================================
	operator (CmdBindComputeResourceTable)
=================================================
*/
	bool CL1CommandBuffer::operator () (const GpuMsg::CmdBindComputeResourceTable &msg)
	{
		ASSERT( msg.index == 0 );
		_resourceTable = msg.resourceTable;
		return true;
	}
	
/*
=================================================
	operator (CmdCopyBuffer)
=================================================
*/
	bool CL1CommandBuffer::operator () (const GpuMsg::CmdCopyBuffer &msg)
	{
		const auto	src_res		= GetResourceCache()->GetBufferID( msg.srcBuffer );
		const auto	dst_res		= GetResourceCache()->GetBufferID( msg.dstBuffer );

		cl_mem		src_id		= src_res.Get<0>().id;
		cl_mem		dst_id		= dst_res.Get<0>().id;
		BytesU		src_size	= src_res.Get<1>().size;
		BytesU		dst_size	= dst_res.Get<1>().size;

		CHECK( GetDevice()->AddSharedObj( this, msg.srcBuffer ) );
		CHECK( GetDevice()->AddSharedObj( this, msg.dstBuffer ) );
		CHECK( GetDevice()->AcquireSharedObj( this ) );

		for (auto& reg : msg.regions)
		{
			CHECK_ERR( reg.srcOffset + reg.size <= src_size );
			CHECK_ERR( reg.dstOffset + reg.size <= dst_size );
			
			CL_CHECK( clEnqueueCopyBuffer(
						GetCommandQueue(),
						src_id,
						dst_id,
						size_t(reg.srcOffset),
						size_t(reg.dstOffset),
						size_t(reg.size),
						0, null,
						null ) );
		}

		CHECK( GetDevice()->ReleaseSharedObj( this ) );
		return true;
	}
	
/*
=================================================
	operator (CmdCopyImage)
=================================================
*/
	bool CL1CommandBuffer::operator () (const GpuMsg::CmdCopyImage &msg)
	{
		const auto	src_res	= GetResourceCache()->GetImageID( msg.srcImage );
		const auto	dst_res	= GetResourceCache()->GetImageID( msg.dstImage );
		cl_mem		src_id	= src_res.Get<0>().id;
		cl_mem		dst_id	= dst_res.Get<0>().id;
		
		CHECK( GetDevice()->AddSharedObj( this, msg.srcImage ) );
		CHECK( GetDevice()->AddSharedObj( this, msg.dstImage ) );
		CHECK( GetDevice()->AcquireSharedObj( this ) );

		for (auto& reg : msg.regions)
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
	operator (CmdCopyBufferToImage)
=================================================
*/
	bool CL1CommandBuffer::operator () (const GpuMsg::CmdCopyBufferToImage &msg)
	{
		const auto	src_res		= GetResourceCache()->GetBufferID( msg.srcBuffer );
		const auto	dst_res		= GetResourceCache()->GetImageID( msg.dstImage );
		
		cl_mem		src_id		= src_res.Get<0>().id;
		cl_mem		dst_id		= dst_res.Get<0>().id;
		BytesU		src_size	= src_res.Get<1>().size;
		BytesU		bpp			= BytesU( EPixelFormat::BitPerPixel( dst_res.Get<1>().format ) );
		uint		depth		= dst_res.Get<1>().dimension.z;
		
		CHECK( GetDevice()->AddSharedObj( this, msg.srcBuffer ) );
		CHECK( GetDevice()->AddSharedObj( this, msg.dstImage ) );
		CHECK( GetDevice()->AcquireSharedObj( this ) );

		for (auto& reg : msg.regions)
		{
			CHECK_ERR( reg.imageLayers.mipLevel.Get() == 0 and reg.imageLayers.aspectMask[EImageAspect::Color] );

			BytesU	row_size		= reg.imageSize.x * bpp;
			BytesU	src_row_pitch	= reg.bufferRowLength * bpp;
			BytesU	src_depth_pitch	= reg.bufferImageHeight * src_row_pitch;
			BytesU	src_array_pitch	= src_depth_pitch;	// TODO
			
			for (uint layer = 0; layer < reg.imageLayers.layerCount; ++layer)
			{
				for (uint z = 0; z < reg.imageSize.z; ++z)
				{
					for (uint y = 0; y < reg.imageSize.y; ++y)	// TODO
					{
						usize4	img_offset	= usize4( reg.imageOffset.x, y + reg.imageOffset.y, z + reg.imageOffset.z, layer + reg.imageLayers.baseLayer.Get() );
						usize3	dst_offset	= usize3( img_offset.xy(), img_offset.z + img_offset.w * depth );
						usize3	dst_size	= usize3( reg.imageSize.x, 1, 1 );
						BytesU	src_off		= reg.bufferOffset + layer * src_array_pitch + z * src_depth_pitch + y * src_row_pitch;
						
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
	operator (CmdCopyImageToBuffer)
=================================================
*/
	bool CL1CommandBuffer::operator () (const GpuMsg::CmdCopyImageToBuffer &msg)
	{
		const auto	src_res		= GetResourceCache()->GetImageID( msg.srcImage );
		const auto	dst_res		= GetResourceCache()->GetBufferID( msg.dstBuffer );
		
		cl_mem		src_id		= src_res.Get<0>().id;
		cl_mem		dst_id		= dst_res.Get<0>().id;
		BytesU		dst_size	= dst_res.Get<1>().size;
		BytesU		bpp			= BytesU( EPixelFormat::BitPerPixel( src_res.Get<1>().format ) );
		uint		depth		= src_res.Get<1>().dimension.z;
		
		CHECK( GetDevice()->AddSharedObj( this, msg.srcImage ) );
		CHECK( GetDevice()->AddSharedObj( this, msg.dstBuffer ) );
		CHECK( GetDevice()->AcquireSharedObj( this ) );

		for (auto& reg : msg.regions)
		{
			CHECK_ERR( reg.imageLayers.mipLevel.Get() == 0 and reg.imageLayers.aspectMask[EImageAspect::Color] );

			BytesU	row_size		= reg.imageSize.x * bpp;
			BytesU	dst_row_pitch	= reg.bufferRowLength * bpp;
			BytesU	dst_depth_pitch	= reg.bufferImageHeight * dst_row_pitch;
			BytesU	dst_array_pitch	= dst_depth_pitch;	// TODO
			
			for (uint layer = 0; layer < reg.imageLayers.layerCount; ++layer)
			{
				for (uint z = 0; z < reg.imageSize.z; ++z)
				{
					for (uint y = 0; y < reg.imageSize.y; ++y)	// TODO
					{
						usize4	img_offset	= usize4( reg.imageOffset.x, y + reg.imageOffset.y, z + reg.imageOffset.z, layer + reg.imageLayers.baseLayer.Get() );
						usize3	src_offset	= usize3( img_offset.xy(), img_offset.z + img_offset.w * depth );
						usize3	src_size	= usize3( reg.imageSize.x, 1, 1 );
						BytesU	dst_off		= reg.bufferOffset + layer * dst_array_pitch + z * dst_depth_pitch + y * dst_row_pitch;
						
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
	operator (CLCmdUpdateBuffer)
=================================================
*/
	bool CL1CommandBuffer::operator () (const GpuMsg::CLCmdUpdateBuffer &msg)
	{
		const auto	src_res	= GetResourceCache()->GetBufferID( _tempBuffer );
		const auto	dst_res	= GetResourceCache()->GetBufferID( msg.dstBuffer );
		
		CHECK_ERR( msg.dstOffset < dst_res.Get<1>().size );
		CHECK_ERR( msg.size + msg.dstOffset <= dst_res.Get<1>().size );
		
		CHECK( GetDevice()->AddSharedObj( this, msg.dstBuffer ) );
		CHECK( GetDevice()->AcquireSharedObj( this ) );

		CL_CHECK( clEnqueueCopyBuffer(
					GetCommandQueue(),
					src_res.Get<0>().id,
					dst_res.Get<0>().id,
					size_t(msg.srcOffset),
					size_t(msg.dstOffset),
					size_t(msg.size),
					0, null,
					null ) );
		
		CHECK( GetDevice()->ReleaseSharedObj( this ) );
		return true;
	}
	
/*
=================================================
	operator (CmdFillBuffer)
=================================================
*/
	bool CL1CommandBuffer::operator () (const GpuMsg::CmdFillBuffer &msg)
	{
		const auto&	dst_res	= GetResourceCache()->GetBufferID( msg.dstBuffer );
		
		CHECK_ERR( msg.dstOffset < dst_res.Get<1>().size );
		CHECK_ERR( dst_res.Get<1>().usage[ EBufferUsage::TransferDst ] );

		const BytesU	size = Min( dst_res.Get<1>().size - msg.dstOffset, msg.size );
		
		CHECK( GetDevice()->AddSharedObj( this, msg.dstBuffer ) );
		CHECK( GetDevice()->AcquireSharedObj( this ) );

		CL_CHECK( clEnqueueFillBuffer(
					GetCommandQueue(),
					dst_res.Get<0>().id,
					&msg.pattern,
					sizeof(msg.pattern),
					size_t(msg.dstOffset),
					size_t(size),
					0, null,
					null ) );
		
		CHECK( GetDevice()->ReleaseSharedObj( this ) );
		return true;
	}
	
/*
=================================================
	operator (CmdClearColorImage)
=================================================
*/
	bool CL1CommandBuffer::operator () (const GpuMsg::CmdClearColorImage &msg)
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
	bool CL1CommandBuffer::operator () (const GpuMsg::CmdPipelineBarrier &)
	{
		CL_CALL( clEnqueueMarkerWithWaitList( GetCommandQueue(), 0, null, null ) );
		return true;
	}
	
/*
=================================================
	operator (CmdPushConstants)
=================================================
*/
	bool CL1CommandBuffer::operator () (const GpuMsg::CmdPushConstants &)
	{
		TODO( "" );
		return true;
	}
	
/*
=================================================
	operator (CmdPushNamedConstants)
=================================================
*/
	bool CL1CommandBuffer::operator () (const GpuMsg::CmdPushNamedConstants &)
	{
		TODO( "" );
		return true;
	}
	
/*
=================================================
	operator (CmdDebugMarker)
=================================================
*/
	bool CL1CommandBuffer::operator () (const GpuMsg::CmdDebugMarker &msg)
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
	bool CL1CommandBuffer::operator () (const GpuMsg::CmdPushDebugGroup &msg)
	{
		LOG( "===================================================================\nDebug group begin: "_str << msg.info, ELog::Debug );
		return true;
	}

/*
=================================================
	operator (CmdPopDebugGroup)
=================================================
*/
	bool CL1CommandBuffer::operator () (const GpuMsg::CmdPopDebugGroup &)
	{
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
