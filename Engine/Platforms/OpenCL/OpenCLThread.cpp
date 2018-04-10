// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Engine/Config/Engine.Config.h"

#ifdef COMPUTE_API_OPENCL

#include "Engine/Platforms/Public/GPU/Thread.h"
#include "Engine/Platforms/Public/GPU/CommandBuffer.h"
#include "Engine/Platforms/OpenCL/Impl/CL2BaseModule.h"
#include "Engine/Platforms/OpenCL/OpenCLObjectsConstructor.h"

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
											GpuMsg::GetGraphicsModules,
											GpuMsg::GetComputeSettings,
											GpuMsg::SubmitComputeQueueCommands,
											GpuMsg::GetDeviceInfo,
											GpuMsg::GetCLDeviceInfo,
											GpuMsg::GetCLPrivateClasses
										> >;
		using SupportedEvents_t		= Module::SupportedEvents_t::Append< MessageListFrom<
											GpuMsg::DeviceCreated,
											GpuMsg::DeviceBeforeDestroy,
											GpuMsg::DeviceLost
										> >;
		
		using Device				= PlatformCL::CL2Device;


	// constants
	private:
		static const TypeIdList		_msgTypes;
		static const TypeIdList		_eventTypes;

		
	// variables
	private:
		ComputeSettings		_settings;
		ModulePtr			_syncManager;

		Device				_device;


	// methods
	public:
		OpenCLThread (UntypedID_t, GlobalSystemsRef gs, const CreateInfo::GpuThread &ci);
		~OpenCLThread ();
		

	// message handlers
	private:
		bool _Delete (const Message< ModuleMsg::Delete > &);
		bool _Link (const Message< ModuleMsg::Link > &);
		bool _Compose (const Message< ModuleMsg::Compose > &);
		bool _AddToManager (const Message< ModuleMsg::AddToManager > &);
		bool _RemoveFromManager (const Message< ModuleMsg::RemoveFromManager > &);

		bool _GetGraphicsModules (const Message< GpuMsg::GetGraphicsModules > &);
		bool _GetComputeSettings (const Message< GpuMsg::GetComputeSettings > &);
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
	OpenCLThread::OpenCLThread (UntypedID_t id, GlobalSystemsRef gs, const CreateInfo::GpuThread &ci) :
		Module( gs, ModuleConfig{ id, 1 }, &_msgTypes, &_eventTypes ),
		_settings( ci.settings.version, ci.settings.device, ci.settings.flags[ GraphicsSettings::EFlags::DebugContext ] ),
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
		_SubscribeOnMsg( this, &OpenCLThread::_Link );
		_SubscribeOnMsg( this, &OpenCLThread::_Compose );
		_SubscribeOnMsg( this, &OpenCLThread::_Delete );
		_SubscribeOnMsg( this, &OpenCLThread::_GetGraphicsModules );
		_SubscribeOnMsg( this, &OpenCLThread::_GetComputeSettings );
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
	_Link
=================================================
*/
	bool OpenCLThread::_Link (const Message< ModuleMsg::Link > &msg)
	{
		if ( _IsComposedOrLinkedState( GetState() ) )
			return true;	// already linked

		CHECK_ERR( GetState() == EState::Initial or GetState() == EState::LinkingFailed );

		// create sync manager
		{
			CHECK_ERR( GlobalSystems()->modulesFactory->Create(
											CLSyncManagerModuleID,
											GlobalSystems(),
											CreateInfo::GpuSyncManager{ this },
											OUT _syncManager ) );

			CHECK_ERR( _Attach( "sync", _syncManager, true ) );
			_syncManager->Send( msg );
		}

		return Module::_Link_Impl( msg );
	}
	
/*
=================================================
	_Compose
=================================================
*/
	bool OpenCLThread::_Compose (const Message< ModuleMsg::Compose > &)
	{
		if ( _IsComposedState( GetState() ) )
			return true;	// already composed

		CHECK_ERR( GetState() == EState::Linked );

		CHECK_COMPOSING( _CreateDevice() );

		return _DefCompose( false );
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
	_GetGraphicsModules
=================================================
*/
	bool OpenCLThread::_GetGraphicsModules (const Message< GpuMsg::GetGraphicsModules > &msg)
	{
		msg->compute.Set( OpenCLObjectsConstructor::GetComputeModules() );
		return true;
	}
	
/*
=================================================
	_GetComputeSettings
=================================================
*/
	bool OpenCLThread::_GetComputeSettings (const Message< GpuMsg::GetComputeSettings > &msg)
	{
		msg->result.Set( _settings );
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
		
		DEBUG_ONLY(
			FOR( i, msg->commands )
			{
				Message< GpuMsg::GetCommandBufferDescriptor >	req_descr;
				msg->commands[i]->Send( req_descr );
				CHECK_ERR( req_descr->result and not req_descr->result->flags[ ECmdBufferCreate::Secondary ] );
			}
		);
		
		ModuleUtils::Send( msg->commands, Message<GpuMsg::SetCommandBufferState>{ GpuMsg::SetCommandBufferState::EState::Pending });

		// wait for signal
		{
			Message< GpuMsg::WaitCLSemaphore >	wait;

			for (auto& sem : Range(msg->waitSemaphores)) {
				ASSERT(sem.second == EPipelineStage::AllCommands );
				wait->semaphores.PushBack( sem.first );
			}

			if ( not wait->semaphores.Empty() ) {
				CHECK( _syncManager->Send( wait ) );
			}
		}

		// execute command buffers
		ModuleUtils::Send( msg->commands, Message<GpuMsg::ExecuteCLCommandBuffer>{} );
		
		// enqueue fence
		if ( msg->fence != GpuFenceId::Unknown )
		{
			Message< GpuMsg::CLFenceSync >	fence_sync{ msg->fence };
			CHECK( _syncManager->Send( fence_sync ) );
		}

		// enqueue semaphores
		FOR( i, msg->signalSemaphores )
		{
			Message< GpuMsg::CLSemaphoreEnqueue >	sem_sync{ msg->signalSemaphores[i] };
			CHECK( _syncManager->Send( sem_sync ) );
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

		switch ( _settings.version )
		{
			case "CL 1.0"_GAPI :	version = 100;	break;
			case "CL 1.1"_GAPI :	version = 110;	break;
			case "CL 1.2"_GAPI :	version = 120;	break;
			case "CL 2.0"_GAPI :	version = 200;	break;
			case "CL 2.1"_GAPI :	version = 210;	break;
			case GAPI::type(0) :	version = 210;	break;
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
				 not _settings.device.Empty()	and
				 info.device.HasSubStringIC( _settings.device ) )
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

		// TODO: write new device name and version to '_settings'

		CHECK_ERR( _device.CreateDevice( dev ) );
		
		CHECK_ERR( _device.CreateContext() );
		CHECK_ERR( _device.CreateQueue( _settings.isDebug ) );

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
		
		if ( _syncManager )
		{
			_syncManager->Send< ModuleMsg::Delete >({});
			_syncManager = null;
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
			this,
			null,
			_syncManager,
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
//-----------------------------------------------------------------------------


/*
=================================================
	CreateOpenCLThread
=================================================
*/
	ModulePtr OpenCLObjectsConstructor::CreateOpenCLThread (ModuleMsg::UntypedID_t id, GlobalSystemsRef gs, const CreateInfo::GpuThread &ci)
	{
		return New< OpenCLThread >( id, gs, ci );
	}


}	// Platforms
}	// Engine

#endif	// COMPUTE_API_OPENCL
