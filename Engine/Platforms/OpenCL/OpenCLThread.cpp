// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Engine/Platforms/Shared/GPU/Thread.h"
#include "Engine/Platforms/Shared/GPU/CommandBuffer.h"
#include "Engine/Platforms/OpenCL/OpenCLContext.h"
#include "Engine/Platforms/OpenCL/Impl/CL2BaseModule.h"

#if defined( COMPUTE_API_OPENCL )

namespace Engine
{
namespace Platforms
{

	//
	// OpenCL Thread
	//
	
	class OpenCLThread final : public Module
	{
	// types
	private:
		using SupportedMessages_t	= Module::SupportedMessages_t::Erase< MessageListFrom<
											ModuleMsg::Update
										> >
										::Append< MessageListFrom<
											ModuleMsg::AddToManager,
											ModuleMsg::RemoveFromManager,
											ModuleMsg::OnManagerChanged,
											GpuMsg::SubmitComputeQueueCommands,
											GpuMsg::GetDeviceInfo,
											GpuMsg::GetCLDeviceInfo,
											GpuMsg::GetCLPrivateClasses
										> >;
		using SupportedEvents_t		= Module::SupportedEvents_t::Append< MessageListFrom<
											GpuMsg::DeviceCreated,
											GpuMsg::DeviceBeforeDestroy
											// TODO: device lost event
										> >;
		
		using Device				= PlatformCL::CL2Device;


	// constants
	private:
		static const TypeIdList		_msgTypes;
		static const TypeIdList		_eventTypes;

		
	// variables
	private:
		String		_reqDeviceName;
		GAPI::type	_reqVersion;

		Device		_device;


	// methods
	public:
		OpenCLThread (GlobalSystemsRef gs, const CreateInfo::GpuThread &ci);
		~OpenCLThread ();

		Ptr< Device >	GetDevice ()		{ return &_device; }
		

	// message handlers
	private:
		bool _Delete (const Message< ModuleMsg::Delete > &);
		bool _Compose (const Message< ModuleMsg::Compose > &);
		bool _AddToManager (const Message< ModuleMsg::AddToManager > &);
		bool _RemoveFromManager (const Message< ModuleMsg::RemoveFromManager > &);
		bool _SubmitComputeQueueCommands (const Message< GpuMsg::SubmitComputeQueueCommands > &);
		bool _GetDeviceInfo (const Message< GpuMsg::GetDeviceInfo > &);
		bool _GetCLDeviceInfo (const Message< GpuMsg::GetCLDeviceInfo > &);
		bool _GetCLPrivateClasses (const Message< GpuMsg::GetCLPrivateClasses > &);

	private:
		bool _CreateDevice ();
		void _DestroyDevice ();
	};
//-----------------------------------------------------------------------------


	
	const TypeIdList	OpenCLThread::_msgTypes{ UninitializedT< SupportedMessages_t >() };
	const TypeIdList	OpenCLThread::_eventTypes{ UninitializedT< SupportedEvents_t >() };

/*
=================================================
	constructor
=================================================
*/
	OpenCLThread::OpenCLThread (GlobalSystemsRef gs, const CreateInfo::GpuThread &ci) :
		Module( gs, ModuleConfig{ CLThreadModuleID, 1 }, &_msgTypes, &_eventTypes ),
		_reqDeviceName( ci.settings.device ),	_reqVersion( ci.settings.version ),
		_device( GlobalSystems() )
	{
		SetDebugName( "OpenCLThread" );
		
		_SubscribeOnMsg( this, &OpenCLThread::_OnModuleAttached_Impl );
		_SubscribeOnMsg( this, &OpenCLThread::_OnModuleDetached_Impl );
		_SubscribeOnMsg( this, &OpenCLThread::_AttachModule_Impl );
		_SubscribeOnMsg( this, &OpenCLThread::_DetachModule_Impl );
		_SubscribeOnMsg( this, &OpenCLThread::_AddToManager );
		_SubscribeOnMsg( this, &OpenCLThread::_RemoveFromManager );
		_SubscribeOnMsg( this, &OpenCLThread::_OnManagerChanged_Empty );
		_SubscribeOnMsg( this, &OpenCLThread::_FindModule_Impl );
		_SubscribeOnMsg( this, &OpenCLThread::_ModulesDeepSearch_Impl );
		_SubscribeOnMsg( this, &OpenCLThread::_Link_Impl );
		_SubscribeOnMsg( this, &OpenCLThread::_Compose );
		_SubscribeOnMsg( this, &OpenCLThread::_Delete );
		_SubscribeOnMsg( this, &OpenCLThread::_SubmitComputeQueueCommands );
		_SubscribeOnMsg( this, &OpenCLThread::_GetDeviceInfo );
		_SubscribeOnMsg( this, &OpenCLThread::_GetCLDeviceInfo );
		_SubscribeOnMsg( this, &OpenCLThread::_GetCLPrivateClasses );
		
		CHECK( _ValidateMsgSubscriptions() );
		
		CHECK( ci.shared.IsNull() );	// sharing is not supported yet

		_AttachSelfToManager( ci.context, CLContextModuleID, false );
	}
	
/*
=================================================
	destructor
=================================================
*/
	OpenCLThread::~OpenCLThread ()
	{
		LOG( "OpenCLThread finalized", ELog::Debug );
	}
	
/*
=================================================
	_Compose
=================================================
*/
	bool OpenCLThread::_Compose (const Message< ModuleMsg::Compose > &msg)
	{
		if ( _IsComposedState( GetState() ) )
			return true;	// already composed

		CHECK_ERR( GetState() == EState::Linked );

		CHECK_COMPOSING( _CreateDevice() );

		CHECK( _DefCompose( false ) );
		return true;
	}

/*
=================================================
	_Delete
=================================================
*/
	bool OpenCLThread::_Delete (const Message< ModuleMsg::Delete > &msg)
	{
		_DestroyDevice();

		CHECK_ERR( Module::_Delete_Impl( msg ) );
		return true;
	}
	
/*
=================================================
	_AddToManager
=================================================
*/
	bool OpenCLThread::_AddToManager (const Message< ModuleMsg::AddToManager > &)
	{
		return true;
	}
	
/*
=================================================
	_RemoveFromManager
=================================================
*/
	bool OpenCLThread::_RemoveFromManager (const Message< ModuleMsg::RemoveFromManager > &)
	{
		return true;
	}
	
/*
=================================================
	_SubmitComputeQueueCommands
=================================================
*/
	bool OpenCLThread::_SubmitComputeQueueCommands (const Message< GpuMsg::SubmitComputeQueueCommands > &msg)
	{
		using namespace cl;

		CHECK_ERR( _device.HasCommandQueue() );

		Message< GpuMsg::SetCommandBufferState >	pending_state{ GpuMsg::SetCommandBufferState::EState::Pending };
		Message< GpuMsg::SetCommandBufferState >	completed_state{ GpuMsg::SetCommandBufferState::EState::Completed };

		FOR( i, msg->commands ) {
			msg->commands[i]->Send( pending_state );
		}

		CL_CALL( clEnqueueBarrierWithWaitList( _device.GetCommandQueue(), 0, null, null ) );	// TODO: use fence
		
		FOR( i, msg->commands ) {
			msg->commands[i]->Send( completed_state );
		}
		return true;
	}

/*
=================================================
	_CreateDevice
=================================================
*/
	bool OpenCLThread::_CreateDevice ()
	{
		uint	version = 0;

		switch ( _reqVersion )
		{
			case "CL 1.0"_GAPI :	version = 100;	break;
			case "CL 1.1"_GAPI :	version = 110;	break;
			case "CL 1.2"_GAPI :	version = 120;	break;
			case "CL 2.0"_GAPI :	version = 200;	break;
			case "CL 2.1"_GAPI :	version = 210;	break;
			default :				RETURN_ERR( "unsupported OpenCL version" );
		}

		Array<Device::DeviceInfo>	dev_info;

		cl::cl_device_id	gpu_device			= null;
		cl::cl_device_id	high_perf_device	= null;
		float				max_performance		= 0.0f;
		cl::cl_device_id	match_name_device	= null;
		cl::cl_device_id	match_ver_device	= null;

		CHECK_ERR( _device.GetDeviceInfo( OUT dev_info ) );

		// choose device
		FOR( i, dev_info )
		{
			const auto&		info = dev_info[i];
																								// magic formula:
			const float		perf = float(info.computeUnits) * float(info.frequencyMHz) * 0.0002f *	// compute units * frequence = real performance
									float(info.vendor.HasSubStringIC("nvidia") ? 12 : 1) +			// for NVidia convert CU to CUDA cores count (must be 96 * CU)
									float(info.isGPU and not info.unifiedMemory ? 4 : 1) +			// cpu and integrated gpu has unified memory
									float(info.globalMemory.Mb()) / 1024.0f +						// we need more memory
									float(info.isGPU ? info.maxInvocations : 1) / 1024.0f;

			if ( OS::IsLittleEndian() != info.littleEndian or not info.compilerSupported )
				continue;

			if ( perf > max_performance ) {
				max_performance	 = perf;
				high_perf_device = info.id;
			}

			if ( gpu_device == null and info.isGPU )
				gpu_device = info.id;

			if ( match_ver_device == null and info.version == version )
				match_ver_device = info.id;

			if ( match_name_device == null		and
				 not _reqDeviceName.Empty()		and
				 info.device.HasSubStringIC( _reqDeviceName ) )
			{
				match_name_device = info.id;
			}
		}

		cl::cl_device_id	dev = null;

		if ( high_perf_device )		dev = high_perf_device;		else
		if ( match_ver_device )		dev = match_ver_device;		else
		if ( match_name_device )	dev = match_name_device;	else
		if ( gpu_device )			dev = gpu_device;			else
									RETURN_ERR( "no OpenCL device found" );

		CHECK_ERR( _device.CreateDevice( dev ) );
		
		CHECK_ERR( _device.CreateContext() );
		CHECK_ERR( _device.CreateQueue() );

		_device.WriteInfo();

		_SendEvent( Message< GpuMsg::DeviceCreated >{} );
		return true;
	}
	
/*
=================================================
	_DestroyDevice
=================================================
*/
	void OpenCLThread::_DestroyDevice ()
	{
		if ( _device.HasDevice() )
		{
			_device.Synchronize();

			_SendEvent( Message< GpuMsg::DeviceBeforeDestroy >{} );
		}

		_device.Deinitialize();
	}

/*
=================================================
	_GetDeviceInfo
=================================================
*/
	bool OpenCLThread::_GetDeviceInfo (const Message< GpuMsg::GetDeviceInfo > &msg)
	{
		msg->result.Set({
			null,
			0
		});
		return true;
	}
	
/*
=================================================
	_GetCLDeviceInfo
=================================================
*/
	bool OpenCLThread::_GetCLDeviceInfo (const Message< GpuMsg::GetCLDeviceInfo > &msg)
	{
		msg->result.Set({
			_device.GetDevice(),
			_device.GetContext(),
			_device.GetCommandQueue()
		});
		return true;
	}
	
/*
=================================================
	_GetCLPrivateClasses
=================================================
*/
	bool OpenCLThread::_GetCLPrivateClasses (const Message< GpuMsg::GetCLPrivateClasses > &msg)
	{
		msg->result.Set({ &_device });
		return true;
	}

/*
=================================================
	_CreateOpenCLThread
=================================================
*/
	ModulePtr OpenCLContext::_CreateOpenCLThread (GlobalSystemsRef gs, const CreateInfo::GpuThread &ci)
	{
		return New< OpenCLThread >( gs, ci );
	}


}	// Platforms
}	// Engine

#endif	// COMPUTE_API_OPENCL
