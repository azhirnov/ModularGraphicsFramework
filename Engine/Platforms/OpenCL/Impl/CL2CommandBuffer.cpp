// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Engine/Platforms/Shared/GPU/CommandBuffer.h"
#include "Engine/Platforms/Shared/GPU/Image.h"
#include "Engine/Platforms/Shared/GPU/Buffer.h"
#include "Engine/Platforms/Shared/GPU/Pipeline.h"
#include "Engine/Platforms/OpenCL/Impl/CL2BaseModule.h"
#include "Engine/Platforms/OpenCL/OpenCLContext.h"

#if defined( COMPUTE_API_OPENCL )

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
											GpuMsg::SetCLCommandBufferQueue
										> >;

		using SupportedEvents_t		= CL2BaseModule::SupportedEvents_t::Append< MessageListFrom<
											GpuMsg::OnCommandBufferStateChanged
										> >;

		using UsedResources_t		= Set< ModulePtr >;
		using Command_t				= GpuMsg::SetCLCommandBufferQueue::Command;
		using CommandArray_t		= Array< Command_t >;
		//using CmdFunc_t				= void (*) (const Command_t &cmd);
		using CmdDataTypes_t		= GpuMsg::SetCLCommandBufferQueue::Data_t::TypeList_t;

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
		CL2CommandBuffer (GlobalSystemsRef gs, const CreateInfo::GpuCommandBuffer &ci);
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
		
	private:
		bool _Submit ();
		bool _BeginRecording ();
		bool _EndRecording ();
		bool _ResetCmdBuffer ();
		bool _OnCompleted ();

		void _ChangeState (ERecordingState newState);

		bool _Initialize ();
		void _ClearStates ();
		
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
	};
//-----------------------------------------------------------------------------



	const TypeIdList	CL2CommandBuffer::_msgTypes{ UninitializedT< SupportedMessages_t >() };
	const TypeIdList	CL2CommandBuffer::_eventTypes{ UninitializedT< SupportedEvents_t >() };

/*
=================================================
	constructor
=================================================
*/
	CL2CommandBuffer::CL2CommandBuffer (GlobalSystemsRef gs, const CreateInfo::GpuCommandBuffer &ci) :
		CL2BaseModule( gs, ModuleConfig{ CLCommandBufferModuleID, ~0u }, &_msgTypes, &_eventTypes ),
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
		_SubscribeOnMsg( this, &CL2CommandBuffer::_DeviceBeforeDestroy );
		_SubscribeOnMsg( this, &CL2CommandBuffer::_GetCommandBufferDescriptor );
		_SubscribeOnMsg( this, &CL2CommandBuffer::_GetDeviceInfo );
		_SubscribeOnMsg( this, &CL2CommandBuffer::_GetCLDeviceInfo );
		_SubscribeOnMsg( this, &CL2CommandBuffer::_GetCLPrivateClasses );
		_SubscribeOnMsg( this, &CL2CommandBuffer::_SetCLCommandBufferQueue );
		_SubscribeOnMsg( this, &CL2CommandBuffer::_SetCommandBufferDependency );
		_SubscribeOnMsg( this, &CL2CommandBuffer::_SetCommandBufferState );
		_SubscribeOnMsg( this, &CL2CommandBuffer::_GetCommandBufferState );

		CHECK( _ValidateMsgSubscriptions() );

		_AttachSelfToManager( ci.gpuThread, Platforms::CLThreadModuleID, true );
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

		CL_CALL( clGetDeviceInfo( GetCLDevice(), CL_DEVICE_MAX_WORK_GROUP_SIZE, sizeof(size), &size, null ) );
		CL_CALL( clGetDeviceInfo( GetCLDevice(), CL_DEVICE_MAX_WORK_ITEM_SIZES, sizeof(size3), size3.ptr(), null ) );
		CL_CALL( clGetDeviceInfo( GetCLDevice(), CL_DEVICE_ADDRESS_BITS, sizeof(value32), &value32, null ) );
		
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
				  (_recordingState == ERecordingState::Executable and _descr.implicitResetable) );

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
		_kernelLocalSize	= req_descr->result->localGroupSize;

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

		const usize3	global_size	= Max( usize3(data.groupCount), usize3(1) ) *
									  Max( _kernelLocalSize, usize3(1) );

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

		CL_CALL( clEnqueueBarrierWithWaitList( GetCommandQueue(), 0, null, null ) );
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
		
		Message< GpuMsg::SetCommandBufferState >	pending_state{ GpuMsg::SetCommandBufferState::EState::Pending };
		Message< GpuMsg::SetCommandBufferState >	completed_state{ GpuMsg::SetCommandBufferState::EState::Completed };

		FOR( i, data.cmdBuffers ) {
			data.cmdBuffers[i]->Send( pending_state );
		}

		CL_CALL( clEnqueueBarrierWithWaitList( GetCommandQueue(), 0, null, null ) );	// TODO: use fence
		
		FOR( i, data.cmdBuffers ) {
			data.cmdBuffers[i]->Send( completed_state );
		}
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

		FOR( i, data.regions )
		{
			const auto&	r = data.regions[i];
			CHECK_ERR( r.srcOffset + r.size <= src_size );
			CHECK_ERR( r.dstOffset + r.size <= dst_size );
			
			CL_CHECK( clEnqueueCopyBuffer(
						GetCommandQueue(),
						src_id,
						dst_id,
						(size_t) r.srcOffset,
						(size_t) r.dstOffset,
						(size_t) r.size,
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
		
		FOR( i, data.regions )
		{
			const auto&	r = data.regions[i];

			CHECK_ERR( r.srcLayers.mipLevel.Get() == 0 and r.srcLayers.aspectMask[EImageAspect::Color] );
			CHECK_ERR( r.dstLayers.mipLevel.Get() == 0 and r.dstLayers.aspectMask[EImageAspect::Color] );

			uint4	img_src_offset	= uint4( r.srcOffset, r.srcLayers.baseLayer.Get() );
			uint4	img_src_size	= uint4( r.size, r.srcLayers.layerCount );
			uint4	img_dst_offset	= uint4( r.dstOffset, r.dstLayers.baseLayer.Get() );
			uint4	img_dst_size	= uint4( r.size, r.dstLayers.layerCount );

			uint3	src_offset		= ImageUtils::ConvertOffset( src_descr.imageType, img_src_offset );
			uint3	src_size		= ImageUtils::ConvertSize( src_descr.imageType, img_src_size );
			uint3	dst_offset		= ImageUtils::ConvertOffset( dst_descr.imageType, img_dst_offset );
			uint3	dst_size		= ImageUtils::ConvertSize( dst_descr.imageType, img_dst_size );
			
			CHECK_ERR( All( src_size == dst_size ) );

			CL_CHECK( clEnqueueCopyImage(
						GetCommandQueue(),
						src_id,
						dst_id,
						usize3(src_offset).ptr(),
						usize3(dst_offset).ptr(),
						usize3(dst_size).ptr(),
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
		
		cl_mem			src_id		= req_src_id->result.Get(null);
		cl_mem			dst_id		= req_dst_id->result.Get(null);
		BytesUL			src_size	= req_src_descr->result->size;
		ImageDescriptor	img_descr	= req_dst_descr->result.Get();

		FOR( i, data.regions )
		{
			const auto&	r = data.regions[i];
			CHECK_ERR( r.imageLayers.mipLevel.Get() == 0 and r.imageLayers.aspectMask[EImageAspect::Color] );

			uint4	img_offset	= uint4( r.imageOffset, r.imageLayers.baseLayer.Get() );
			uint4	img_size	= uint4( r.imageSize, r.imageLayers.layerCount );
			uint3	dst_offset	= ImageUtils::ConvertOffset( img_descr.imageType, img_offset );
			uint3	dst_size	= ImageUtils::ConvertSize( img_descr.imageType, img_size );
			BytesUL	bpp			= BytesUL( EPixelFormat::BitPerPixel( img_descr.format ) );
			BytesUL	data_size	= dst_size.Volume() * bpp;

			CHECK_ERR( All( img_offset + img_size <= img_descr.dimension ) );
			CHECK_ERR( data_size <= src_size );
			
			CL_CHECK( clEnqueueCopyBufferToImage(
						GetCommandQueue(),
						src_id,
						dst_id,
						(size_t) r.bufferOffset,
						usize3(dst_offset).ptr(),
						usize3(dst_size).ptr(),
						0, null,
						null ) );
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
		
		cl_mem			src_id		= req_src_id->result.Get(null);
		cl_mem			dst_id		= req_dst_id->result.Get(null);
		ImageDescriptor	img_descr	= req_src_descr->result.Get();
		BytesUL			dst_size	= req_dst_descr->result->size;
		
		FOR( i, data.regions )
		{
			const auto&	r = data.regions[i];
			CHECK_ERR( r.imageLayers.mipLevel.Get() == 0 and r.imageLayers.aspectMask[EImageAspect::Color] );
			
			uint4	img_offset	= uint4( r.imageOffset, r.imageLayers.baseLayer.Get() );
			uint4	img_size	= uint4( r.imageSize, r.imageLayers.layerCount );
			uint3	src_offset	= ImageUtils::ConvertOffset( img_descr.imageType, img_offset );
			uint3	src_size	= ImageUtils::ConvertSize( img_descr.imageType, img_size );
			BytesUL	bpp			= BytesUL( EPixelFormat::BitPerPixel( img_descr.format ) );
			BytesUL	data_size	= src_size.Volume() * bpp;

			CHECK_ERR( data_size <= dst_size );

			CL_CHECK( clEnqueueCopyImageToBuffer(
						GetCommandQueue(),
						src_id,
						dst_id,
						usize3(src_offset).ptr(),
						usize3(src_size).ptr(),
						(size_t) r.bufferOffset,
						0, null,
						null ) );
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
		const auto&	data = cmd.data.Get< GpuMsg::CmdUpdateBuffer >();

		TODO( "" );
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
		
		TODO( "" );
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
		
		CL_CALL( clEnqueueBarrierWithWaitList( GetCommandQueue(), 0, null, null ) );
		return true;
	}

}	// PlatformCL
//-----------------------------------------------------------------------------

namespace Platforms
{
	ModulePtr OpenCLContext::_CreateCL2CommandBuffer (GlobalSystemsRef gs, const CreateInfo::GpuCommandBuffer &ci)
	{
		return New< PlatformCL::CL2CommandBuffer >( gs, ci );
	}
}	// Platforms
}	// Engine

#endif	// COMPUTE_API_OPENCL
