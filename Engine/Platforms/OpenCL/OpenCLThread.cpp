// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Core/Config/Engine.Config.h"

#ifdef COMPUTE_API_OPENCL

#include "Core/STL/Algorithms/StringParser.h"
#include "Engine/Platforms/Public/GPU/Thread.h"
#include "Engine/Platforms/Public/GPU/CommandBuffer.h"
#include "Engine/Platforms/OpenCL/120/CL1BaseModule.h"
#include "Engine/Platforms/OpenCL/120/CL1SamplerCache.h"
#include "Engine/Platforms/OpenCL/120/CL1ResourceCache.h"
#include "Engine/Platforms/OpenCL/OpenCLObjectsConstructor.h"

#ifdef GRAPHICS_API_OPENGL
#	include "Engine/Platforms/OpenGL/450/GL4Messages.h"
#endif

#ifdef GRAPHICS_API_DIRECTX
#	include "Engine/Platforms/DirectX/Impl/DX11Messages.h"
#endif

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
		using QueueMsgList_t		= MessageListFrom< GpuMsg::SubmitComputeQueueCommands >;
		using QueueEventList_t		= MessageListFrom< GpuMsg::DeviceLost >;

		using SupportedMessages_t	= Module::SupportedMessages_t::Erase< MessageListFrom<
											ModuleMsg::Update
										> >
										::Append< MessageListFrom<
											ModuleMsg::AddToManager,
											ModuleMsg::RemoveFromManager,
											ModuleMsg::OnManagerChanged,
											GpuMsg::GetGraphicsModules,
											GpuMsg::GetComputeSettings,
											GpuMsg::GetDeviceInfo,
											GpuMsg::GetCLDeviceInfo,
											GpuMsg::GetCLPrivateClasses,
											GpuMsg::GetDeviceProperties
										> >::Append< QueueMsgList_t >;

		using SupportedEvents_t		= Module::SupportedEvents_t::Append< MessageListFrom<
											GpuMsg::DeviceCreated,
											GpuMsg::DeviceBeforeDestroy
										> >::Append< QueueEventList_t >;
		
		using Device				= PlatformCL::CL1Device;
		using SamplerCache			= PlatformCL::CL1SamplerCache;
		using ResourceCache			= PlatformCL::CL1ResourceCache;


	// constants
	private:
		static const TypeIdList		_msgTypes;
		static const TypeIdList		_eventTypes;

		
	// variables
	private:
		ComputeSettings		_settings;

		ModulePtr			_syncManager;
		ModulePtr			_cmdQueue;

		Device				_device;
		SamplerCache		_samplerCache;
		ResourceCache		_resourceCache;

		ModulePtr			_sharedThread;


	// methods
	public:
		OpenCLThread (UntypedID_t, GlobalSystemsRef gs, const CreateInfo::GpuThread &ci);
		~OpenCLThread ();
		

	// message handlers
	private:
		bool _Delete (const ModuleMsg::Delete &);
		bool _Link (const ModuleMsg::Link &);
		bool _Compose (const ModuleMsg::Compose &);
		bool _AddToManager (const ModuleMsg::AddToManager &);
		bool _RemoveFromManager (const ModuleMsg::RemoveFromManager &);

		bool _GetGraphicsModules (const GpuMsg::GetGraphicsModules &);
		bool _GetComputeSettings (const GpuMsg::GetComputeSettings &);
		bool _GetDeviceInfo (const GpuMsg::GetDeviceInfo &);
		bool _GetCLDeviceInfo (const GpuMsg::GetCLDeviceInfo &);
		bool _GetCLPrivateClasses (const GpuMsg::GetCLPrivateClasses &);
		bool _GetDeviceProperties (const GpuMsg::GetDeviceProperties &);

		// event handlers
		bool _OnSharedThreadComposed (const ModuleMsg::AfterCompose &);
		bool _OnSharedThreadDelete (const ModuleMsg::Delete &);

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
		_device( GlobalSystems() ),		_sharedThread{ ci.shared }
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
		_SubscribeOnMsg( this, &OpenCLThread::_GetDeviceInfo );
		_SubscribeOnMsg( this, &OpenCLThread::_GetCLDeviceInfo );
		_SubscribeOnMsg( this, &OpenCLThread::_GetCLPrivateClasses );
		_SubscribeOnMsg( this, &OpenCLThread::_GetDeviceProperties );

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
	bool OpenCLThread::_Link (const ModuleMsg::Link &msg)
	{
		if ( _IsComposedOrLinkedState( GetState() ) )
			return true;	// already linked

		CHECK_ERR( _IsInitialState( GetState() ) );

		// create sync manager
		{
			CHECK_ERR( GlobalSystems()->modulesFactory->Create(
											CLSyncManagerModuleID,
											GlobalSystems(),
											CreateInfo::GpuSyncManager{ this },
											OUT _syncManager ) );

			CHECK_ERR( _Attach( "sync", _syncManager ) );
		}

		// create queue
		{
			CHECK_ERR( GlobalSystems()->modulesFactory->Create(
											CLCommandQueueModuleID,
											GlobalSystems(),
											CreateInfo::GpuCommandQueue{ this, EQueueFamily::Default },
											OUT _cmdQueue ) );
			
			CHECK_ERR( _Attach( "queue", _cmdQueue ) );

			CHECK_ERR( _CopySubscriptions< QueueMsgList_t >( _cmdQueue ) );
			CHECK_ERR( ReceiveEvents< QueueEventList_t >( _cmdQueue ) );
		}

		_syncManager->Send( msg );
		_cmdQueue->Send( msg );

		Module::_Link_Impl( msg );

		if ( _sharedThread )
		{
			_sharedThread->Subscribe( this, &OpenCLThread::_OnSharedThreadComposed );
			_sharedThread->Subscribe( this, &OpenCLThread::_OnSharedThreadDelete );
			
			if ( _IsComposedState( _sharedThread->GetState() ) )
				_OnSharedThreadComposed({});
		}
		return true;
	}
	
/*
=================================================
	_Compose
=================================================
*/
	bool OpenCLThread::_Compose (const ModuleMsg::Compose &)
	{
		if ( _IsComposedState( GetState() ) )
			return true;	// already composed

		if ( _sharedThread )
			return true;	// wait for shared thread composing

		CHECK_ERR( GetState() == EState::Linked );

		CHECK_COMPOSING( _CreateDevice() );
		return true;
	}
	
/*
=================================================
	_OnSharedThreadComposed
=================================================
*/
	bool OpenCLThread::_OnSharedThreadComposed (const ModuleMsg::AfterCompose &)
	{
		CHECK_ERR( GetState() == EState::Linked );

		CHECK_COMPOSING( _CreateDevice() );
		return true;
	}
	
/*
=================================================
	_OnSharedThreadDelete
=================================================
*/
	bool OpenCLThread::_OnSharedThreadDelete (const ModuleMsg::Delete &)
	{
		// TODO

		_sharedThread	= null;
		return true;
	}

/*
=================================================
	_Delete
=================================================
*/
	bool OpenCLThread::_Delete (const ModuleMsg::Delete &msg)
	{
		_DestroyDevice();
		
		if ( _sharedThread )
		{
			_sharedThread->UnsubscribeAll( this );
			_sharedThread = null;
		}

		CHECK_ERR( Module::_Delete_Impl( msg ) );
		return true;
	}
	
/*
=================================================
	_AddToManager
=================================================
*/
	bool OpenCLThread::_AddToManager (const ModuleMsg::AddToManager &)
	{
		return true;
	}
	
/*
=================================================
	_RemoveFromManager
=================================================
*/
	bool OpenCLThread::_RemoveFromManager (const ModuleMsg::RemoveFromManager &)
	{
		return true;
	}
	
/*
=================================================
	_GetGraphicsModules
=================================================
*/
	bool OpenCLThread::_GetGraphicsModules (const GpuMsg::GetGraphicsModules &msg)
	{
		msg.compute.Set( OpenCLObjectsConstructor::GetComputeModules() );
		return true;
	}
	
/*
=================================================
	_GetComputeSettings
=================================================
*/
	bool OpenCLThread::_GetComputeSettings (const GpuMsg::GetComputeSettings &msg)
	{
		msg.result.Set( _settings );
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

		// check version
		{
			const String	api_name = _settings.GetAPIName();
			CHECK_ERR( api_name.StartsWithIC( "CL" ) or api_name.StartsWithIC( "opencl" ) );

			version = _settings.GetAPIVersion();
		}

		Array<Device::DeviceInfo>	dev_info;

		cl::cl_device_id	gpu_device			= null;
		cl::cl_device_id	high_perf_device	= null;
		cl::cl_device_id	match_name_device	= null;
		float				max_performance		= 0.0f;
		float				name_match			= 0.0f;

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
			
			const float		nmatch	= _settings.device.Empty() ? 0.0f : StringParser::CompareByWords( info.device, _settings.device );

			if ( OS::IsLittleEndian() != info.littleEndian	or
				 not info.compilerSupported					or
				 info.version < version )
				continue;

			if ( perf > max_performance ) {
				max_performance		= perf;
				high_perf_device	= info.id;
			}

			if ( gpu_device == null and info.isGPU )
				gpu_device = info.id;
			
			if ( nmatch > name_match ) {
				match_name_device	= info.id;
				name_match			= nmatch;
			}
		}

		cl::cl_device_id	dev = null;

		if ( match_name_device )	dev = match_name_device;	else
		if ( high_perf_device )		dev = high_perf_device;		else
		if ( gpu_device )			dev = gpu_device;			else
									RETURN_ERR( "no OpenCL device found" );

		// update settings
		{
			for (auto& info : dev_info) {
				if ( info.id == dev ) {
					_settings.device	= info.device;
					_settings.version	= GAPI::FromString( "CL "_str << (info.version / 100) << '.' << ((info.version / 10) % 10) );
					break;
				}
			}
		}

		CHECK_ERR( _device.CreateDevice( dev ) );
		
		_device.WriteInfo();

		if ( _sharedThread )
		{
			#ifdef GRAPHICS_API_OPENGL
				if ( _sharedThread->GetSupportedMessages().HasAllTypes<MessageListFrom< GpuMsg::GetGLDeviceInfo >>() )
					CHECK_ERR( _device.CreateGLSharedContext() );
			#endif

			#ifdef GRAPHICS_API_DIRECTX
				if ( _sharedThread->GetSupportedMessages().HasAllTypes<MessageListFrom< GpuMsg::GetDXDeviceInfo >>() )
					CHECK_ERR( _device.CreateDXSharedContext() );
			#endif
		}
		else
		{
			CHECK_ERR( _device.CreateContext() );
		}

		CHECK_ERR( _device.CreateQueue( _settings.isDebug ) );
		
		CHECK( _DefCompose( false ) );

		_SendEvent( GpuMsg::DeviceCreated{} );
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

			_SendEvent( GpuMsg::DeviceBeforeDestroy{} );
		}

		if ( _cmdQueue )
		{
			_cmdQueue->Send( ModuleMsg::Delete{} );
			_cmdQueue = null;
		}
		
		if ( _syncManager )
		{
			_syncManager->Send( ModuleMsg::Delete{} );
			_syncManager = null;
		}

		_device.Deinitialize();
	}

/*
=================================================
	_GetDeviceInfo
=================================================
*/
	bool OpenCLThread::_GetDeviceInfo (const GpuMsg::GetDeviceInfo &msg)
	{
		GpuMsg::GetDeviceInfo::Info	info;
		info.gpuThread		= this;
		info.sharedThread	= null;
		info.syncManager	= _syncManager;
		info.memManager		= null;
		info.renderPass		= null;
		info.imageCount		= 0;

		msg.result.Set( info );
		return true;
	}
	
/*
=================================================
	_GetCLDeviceInfo
=================================================
*/
	bool OpenCLThread::_GetCLDeviceInfo (const GpuMsg::GetCLDeviceInfo &msg)
	{
		msg.result.Set({
			_device.GetDevice(),
			_device.GetContext()
		});
		return true;
	}
	
/*
=================================================
	_GetCLPrivateClasses
=================================================
*/
	bool OpenCLThread::_GetCLPrivateClasses (const GpuMsg::GetCLPrivateClasses &msg)
	{
		msg.result.Set({
			&_device,
			&_samplerCache,
			&_resourceCache
		});
		return true;
	}
	
/*
=================================================
	_GetDeviceProperties
=================================================
*/
	bool OpenCLThread::_GetDeviceProperties (const GpuMsg::GetDeviceProperties &msg)
	{
		msg.result.Set( _device.GetProperties() );
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
