// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Core/Config/Engine.Config.h"

#ifdef GRAPHICS_API_VULKAN

#include "Core/STL/Algorithms/StringParser.h"
#include "Engine/Platforms/Public/Tools/WindowHelper.h"
#include "Engine/Platforms/Public/GPU/Thread.h"
#include "Engine/Platforms/Public/GPU/CommandBuffer.h"
#include "Engine/Platforms/Public/GPU/Memory.h"
#include "Engine/Platforms/Vulkan/VulkanObjectsConstructor.h"
#include "Engine/Platforms/Vulkan/Windows/VkWinSurface.h"
#include "Engine/Platforms/Vulkan/Android/VkAndSurface.h"
#include "Engine/Platforms/Vulkan/110/Vk1Device.h"
#include "Engine/Platforms/Vulkan/110/Vk1PipelineCache.h"
#include "Engine/Platforms/Vulkan/110/Vk1PipelineLayout.h"
#include "Engine/Platforms/Vulkan/110/Vk1RenderPassCache.h"

#include "Engine/Platforms/Vulkan/110/Vk1SamplerCache.h"
#include "Engine/Platforms/Vulkan/110/Vk1ResourceCache.h"

namespace Engine
{
namespace Platforms
{

	//
	// Vulkan Thread
	//
	
	class VulkanThread final : public Module
	{
	// types
	private:
		using QueueMsgList_t		= MessageListFrom<
											GpuMsg::SubmitGraphicsQueueCommands,
											GpuMsg::SubmitComputeQueueCommands
										>;
		using QueueEventList_t		= MessageListFrom< GpuMsg::DeviceLost >;

		using SupportedMessages_t	= Module::SupportedMessages_t::Append< MessageListFrom<
											ModuleMsg::AddToManager,
											ModuleMsg::RemoveFromManager,
											ModuleMsg::OnManagerChanged,
											OSMsg::WindowCreated,
											OSMsg::WindowBeforeDestroy,
											GpuMsg::GetGraphicsModules,
											GpuMsg::ThreadBeginFrame,
											GpuMsg::ThreadEndFrame,
											GpuMsg::GetGraphicsSettings,
											GpuMsg::GetComputeSettings,
											GpuMsg::GetDeviceInfo,
											GpuMsg::GetVkDeviceInfo,
											GpuMsg::GetVkPrivateClasses,
											GpuMsg::GetDeviceProperties
										> >::Append< QueueMsgList_t >;

		using SupportedEvents_t		= Module::SupportedEvents_t::Append< MessageListFrom<
											GpuMsg::ThreadBeginFrame,
											GpuMsg::ThreadEndFrame,
											GpuMsg::DeviceCreated,
											GpuMsg::DeviceBeforeDestroy
										> >::Append< QueueEventList_t >;

		using Surface				= PlatformVK::Vk1Surface;
		using Device				= PlatformVK::Vk1Device;
		using SamplerCache			= PlatformVK::Vk1SamplerCache;
		using PipelineCache			= PlatformVK::Vk1PipelineCache;
		using LayoutCache			= PlatformVK::Vk1PipelineLayoutCache;
		using RenderPassCache		= PlatformVK::Vk1RenderPassCache;
		using ResourceCache			= PlatformVK::Vk1ResourceCache;


	// constants
	private:
		static const TypeIdList		_msgTypes;
		static const TypeIdList		_eventTypes;

		
	// variables
	private:
		GraphicsSettings		_settings;

		ModulePtr				_window;
		ModulePtr				_memManager;
		ModulePtr				_syncManager;
		ModulePtr				_cmdQueue;

		Surface					_surface;
		Device					_device;
		
		GpuSemaphoreId			_imageAvailable;
		GpuSemaphoreId			_renderFinished;

		SamplerCache			_samplerCache;
		PipelineCache			_pipelineCache;
		LayoutCache				_layoutCache;
		RenderPassCache			_renderPassCache;
		ResourceCache			_resourceCache;

		bool					_isWindowVisible;


	// methods
	public:
		VulkanThread (UntypedID_t, GlobalSystemsRef gs, const CreateInfo::GpuThread &ci);
		~VulkanThread ();
		

	// message handlers
	private:
		bool _Delete (const ModuleMsg::Delete &);
		bool _Link (const ModuleMsg::Link &);
		bool _Compose (const ModuleMsg::Compose &);
		bool _Update (const ModuleMsg::Update &);
		bool _AddToManager (const ModuleMsg::AddToManager &);
		bool _RemoveFromManager (const ModuleMsg::RemoveFromManager &);
		bool _GetGraphicsModules (const GpuMsg::GetGraphicsModules &);

		bool _ThreadBeginFrame (const GpuMsg::ThreadBeginFrame &);
		bool _ThreadEndFrame (const GpuMsg::ThreadEndFrame &);
		bool _GetGraphicsSettings (const GpuMsg::GetGraphicsSettings &);
		bool _GetComputeSettings (const GpuMsg::GetComputeSettings &);

		bool _WindowCreated (const OSMsg::WindowCreated &);
		bool _WindowBeforeDestroy (const OSMsg::WindowBeforeDestroy &);
		bool _WindowVisibilityChanged (const OSMsg::WindowVisibilityChanged &);
		bool _WindowDescriptionChanged (const OSMsg::WindowDescriptionChanged &);
		
		bool _GetDeviceInfo (const GpuMsg::GetDeviceInfo &);
		bool _GetVkDeviceInfo (const GpuMsg::GetVkDeviceInfo &);
		bool _GetVkPrivateClasses (const GpuMsg::GetVkPrivateClasses &);
		bool _GetDeviceProperties (const GpuMsg::GetDeviceProperties &);

	private:
		bool _CreateDevice ();
		void _DestroyDevice ();
		
		void _DetachWindow ();

		bool _CreateSemaphores ();
		void _DestroySemaphores ();
	};
//-----------------------------------------------------------------------------


	
	const TypeIdList	VulkanThread::_msgTypes{ UninitializedT< SupportedMessages_t >() };
	const TypeIdList	VulkanThread::_eventTypes{ UninitializedT< SupportedEvents_t >() };

/*
=================================================
	constructor
=================================================
*/
	VulkanThread::VulkanThread (UntypedID_t id, GlobalSystemsRef gs, const CreateInfo::GpuThread &ci) :
		Module( gs, ModuleConfig{ id, 1 }, &_msgTypes, &_eventTypes ),
		_settings( ci.settings ),		_device( GlobalSystems() ),
		_samplerCache( &_device ),		_pipelineCache( &_device ),
		_layoutCache( &_device ),		_renderPassCache( &_device ),
		_isWindowVisible( false )
	{
		SetDebugName( "VulkanThread" );
		
		_SubscribeOnMsg( this, &VulkanThread::_OnModuleAttached_Impl );
		_SubscribeOnMsg( this, &VulkanThread::_OnModuleDetached_Impl );
		_SubscribeOnMsg( this, &VulkanThread::_AttachModule_Impl );
		_SubscribeOnMsg( this, &VulkanThread::_DetachModule_Impl );
		_SubscribeOnMsg( this, &VulkanThread::_AddToManager );
		_SubscribeOnMsg( this, &VulkanThread::_RemoveFromManager );
		_SubscribeOnMsg( this, &VulkanThread::_OnManagerChanged_Empty );
		_SubscribeOnMsg( this, &VulkanThread::_FindModule_Impl );
		_SubscribeOnMsg( this, &VulkanThread::_ModulesDeepSearch_Impl );
		_SubscribeOnMsg( this, &VulkanThread::_Update );
		_SubscribeOnMsg( this, &VulkanThread::_Link );
		_SubscribeOnMsg( this, &VulkanThread::_Compose );
		_SubscribeOnMsg( this, &VulkanThread::_Delete );
		_SubscribeOnMsg( this, &VulkanThread::_GetGraphicsModules );
		_SubscribeOnMsg( this, &VulkanThread::_ThreadBeginFrame );
		_SubscribeOnMsg( this, &VulkanThread::_ThreadEndFrame );
		_SubscribeOnMsg( this, &VulkanThread::_WindowCreated );
		_SubscribeOnMsg( this, &VulkanThread::_WindowBeforeDestroy );
		_SubscribeOnMsg( this, &VulkanThread::_GetDeviceInfo );
		_SubscribeOnMsg( this, &VulkanThread::_GetVkDeviceInfo );
		_SubscribeOnMsg( this, &VulkanThread::_GetVkPrivateClasses );
		_SubscribeOnMsg( this, &VulkanThread::_GetGraphicsSettings );
		_SubscribeOnMsg( this, &VulkanThread::_GetComputeSettings );
		_SubscribeOnMsg( this, &VulkanThread::_GetDeviceProperties );
		
		CHECK( ci.shared.IsNull() );	// sharing is not supported yet

		_AttachSelfToManager( ci.context, VkContextModuleID, false );
	}
	
/*
=================================================
	destructor
=================================================
*/
	VulkanThread::~VulkanThread ()
	{
		LOG( "VulkanThread finalized", ELog::Debug );

		ASSERT( _window.IsNull() );
	}
	
/*
=================================================
	_Link
=================================================
*/
	bool VulkanThread::_Link (const ModuleMsg::Link &msg)
	{
		if ( _IsComposedOrLinkedState( GetState() ) )
			return true;	// already linked

		CHECK_ERR( _IsInitialState( GetState() ) );
		
		const bool	with_surface = not _settings.flags[ GraphicsSettings::EFlags::NoSurface ];

		if ( with_surface )
		{
			CHECK_LINKING(( _window = PlatformTools::WindowHelper::FindWindow( GlobalSystems() ) ));

			_window->Subscribe( this, &VulkanThread::_WindowCreated );
			_window->Subscribe( this, &VulkanThread::_WindowBeforeDestroy );
			_window->Subscribe( this, &VulkanThread::_WindowVisibilityChanged );
			_window->Subscribe( this, &VulkanThread::_WindowDescriptionChanged );
		}

		CHECK_ERR( Module::_Link_Impl( msg ) );

		// create sync manager
		{
			CHECK_ERR( GlobalSystems()->modulesFactory->Create(
											VkSyncManagerModuleID,
											GlobalSystems(),
											CreateInfo::GpuSyncManager{ this },
											OUT _syncManager ) );

			CHECK_ERR( _Attach( "sync", _syncManager ) );
		}

		// create queue
		{
			CHECK_ERR( GlobalSystems()->modulesFactory->Create(
											VkCommandQueueModuleID,
											GlobalSystems(),
											CreateInfo::GpuCommandQueue{ this, EQueueFamily::Default },
											OUT _cmdQueue ) );
			
			CHECK_ERR( _Attach( "queue", _cmdQueue ) );

			CHECK_ERR( _CopySubscriptions< QueueMsgList_t >( _cmdQueue ) );
			CHECK_ERR( ReceiveEvents< QueueEventList_t >( _cmdQueue ) );
		}

		_syncManager->Send( msg );
		_cmdQueue->Send( msg );
		
		// if window already created
		if ( with_surface and _IsComposedState( _window->GetState() ) )
		{
			_SendMsg( OSMsg::WindowCreated{} );
		}
		return true;
	}
	
/*
=================================================
	_Compose
=================================================
*/
	bool VulkanThread::_Compose (const ModuleMsg::Compose &)
	{
		if ( _IsComposedState( GetState() ) )
			return true;	// already composed

		CHECK_ERR( GetState() == EState::Linked );
		
		if ( _window )
			return false;

		CHECK_COMPOSING( _CreateDevice() );
		return true;
	}
	
/*
=================================================
	_Update
=================================================
*/
	bool VulkanThread::_Update (const ModuleMsg::Update &msg)
	{
		if ( not _IsComposedState( GetState() ) or
			 not _isWindowVisible )
		{
			return false;
		}

		CHECK_ERR( Module::_Update_Impl( msg ) );
		return true;
	}

/*
=================================================
	_Delete
=================================================
*/
	bool VulkanThread::_Delete (const ModuleMsg::Delete &msg)
	{
		_DestroyDevice();
		_DetachWindow();

		CHECK_ERR( Module::_Delete_Impl( msg ) );
		return true;
	}
	
/*
=================================================
	_AddToManager
=================================================
*/
	bool VulkanThread::_AddToManager (const ModuleMsg::AddToManager &)
	{
		return true;
	}
	
/*
=================================================
	_RemoveFromManager
=================================================
*/
	bool VulkanThread::_RemoveFromManager (const ModuleMsg::RemoveFromManager &)
	{
		return true;
	}
	
/*
=================================================
	_GetGraphicsModules
=================================================
*/	
	bool VulkanThread::_GetGraphicsModules (const GpuMsg::GetGraphicsModules &msg)
	{
		msg.compute.Set( VulkanObjectsConstructor::GetComputeModules() );
		msg.graphics.Set( VulkanObjectsConstructor::GetGraphicsModules() );
		return true;
	}

/*
=================================================
	_ThreadBeginFrame
=================================================
*/
	bool VulkanThread::_ThreadBeginFrame (const GpuMsg::ThreadBeginFrame &msg)
	{
		CHECK_ERR( _IsComposedState( GetState() ) );

		GpuMsg::GetVkSemaphore	req_sem{ _imageAvailable };
		_syncManager->Send( req_sem );

		CHECK_ERR( _device.BeginFrame( *req_sem.result ) );

		msg.result.Set({ _device.GetCurrentFramebuffer(), _device.GetImageIndex() });
		return true;
	}

/*
=================================================
	_ThreadEndFrame
----
	this method used for submit command buffers with synchronization between frame changes
=================================================
*/
	bool VulkanThread::_ThreadEndFrame (const GpuMsg::ThreadEndFrame &msg)
	{
		CHECK_ERR( _device.IsFrameStarted() );

		if ( msg.framebuffer )
			CHECK_ERR( msg.framebuffer == _device.GetCurrentFramebuffer() );

		if ( not msg.commands.Empty() )
		{
			GpuMsg::SubmitGraphicsQueueCommands		submit = msg._Cast<GpuMsg::SubmitGraphicsQueueCommands>();

			submit.waitSemaphores.PushBack({ _imageAvailable, EPipelineStage::ColorAttachmentOutput });
			submit.signalSemaphores.PushBack( _renderFinished );

			_cmdQueue->Send( submit );
		}
		else
		{
			CHECK_ERR( msg.fence == GpuFenceId::Unknown );
		}
		
		GpuMsg::GetVkSemaphore	req_sem{ _renderFinished };
		_syncManager->Send( req_sem );

		CHECK_ERR( _device.EndFrame( *req_sem.result ) );
		return true;
	}

/*
=================================================
	_WindowCreated
=================================================
*/
	bool VulkanThread::_WindowCreated (const OSMsg::WindowCreated &)
	{
		CHECK_ERR( GetState() == EState::Linked );
		
		CHECK_COMPOSING( _CreateDevice() );
		return true;
	}

/*
=================================================
	_CreateDevice
=================================================
*/
	bool VulkanThread::_CreateDevice ()
	{
		using namespace vk;
		using namespace Engine::PlatformVK;
		using namespace Engine::CreateInfo;
		
		using EContextFlags = CreateInfo::GpuContext::EFlags;
		
		uint	vk_version	 = 0;

		// choose version
		switch ( _settings.version )
		{
			case "VK 1.0"_GAPI :
			case "vulkan 1.0"_GAPI :	vk_version = VK_API_VERSION_1_0; break;
			case "VK 1.1"_GAPI :
			case "vulkan 1.1"_GAPI :	vk_version = VK_API_VERSION_1_1; break;
			case GAPI::type(0) :		vk_version = VK_API_VERSION_1_0; break;
			default :					RETURN_ERR( "unsupported Vulkan version" );
		}

		// create instance
		{
			const bool				is_debug		= _settings.flags[EContextFlags::DebugContext];
			Array<const char*>		extensions		= {};
			Array<const char*>		layers			= {};
			const int				debug_flags		= VK_DEBUG_REPORT_WARNING_BIT_EXT |
													  VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT |
													  VK_DEBUG_REPORT_ERROR_BIT_EXT;
			
			if ( is_debug ) {
				layers << "VK_LAYER_LUNARG_standard_validation";
				extensions << VK_EXT_DEBUG_REPORT_EXTENSION_NAME;
			}

			_surface.RequestExtensions( INOUT extensions );

			CHECK_ERR( _device.CreateInstance( "", 0, vk_version, extensions, layers ) );

			if ( is_debug )
				CHECK( _device.CreateDebugCallback( VkDebugReportFlagBitsEXT(debug_flags) ) );
		}

		// Choose physical device
		{
			Array<Device::DeviceInfo>	dev_info;
			CHECK_ERR( _device.GetPhysicalDeviceInfo( OUT dev_info ) );

			VkPhysicalDevice	match_name_device	= 0;
			VkPhysicalDevice	high_perf_device	= 0;
			float				max_performance		= 0.0f;
			float				name_match			= 0.0f;

			for (auto& info : dev_info)
			{
																								// magic function:
				const float		perf	= float(info.globalMemory.Mb()) / 1024.0f +						// 
										  float(info.isGPU and not info.integratedGPU ? 4 : 1) +		// 
										  float(info.maxInvocations) / 1024.0f +						// 
										  float(info.supportsTesselation + info.supportsGeometryShader);

				const float		nmatch	= _settings.device.Empty() ? 0.0f : StringParser::CompareByWords( info.device, _settings.device );

				if ( perf > max_performance ) {
					max_performance		= perf;
					high_perf_device	= info.id;
				}

				if ( nmatch > name_match ) {
					match_name_device	= info.id;
					name_match			= nmatch;
				}
			}

			VkPhysicalDevice	dev	= match_name_device ? match_name_device : high_perf_device;

			CHECK_ERR( _device.CreatePhysicalDevice( dev ) );

			// update settings
			for (auto& info : dev_info) {
				if ( info.id == dev ) {
					_settings.version	= GAPI::FromString( "VK "_str << VK_VERSION_MAJOR(info.version) << '.' << VK_VERSION_MINOR(info.version) );
					_settings.device	= info.device;
					break;
				}
			}
		}

		// create surface
		if ( _window )
		{
			using namespace Engine::PlatformTools;

			VkSurfaceKHR	surface	= VK_NULL_HANDLE;

			CHECK_ERR( WindowHelper::GetWindowHandle( _window,
							LAMBDA( this, &surface ) (const auto &data)
							{
								return _surface.Create( _device.GetInstance(), data.window, OUT surface );
							}) );

			CHECK_ERR( _device.SetSurface( surface, _settings.colorFmt ) );
		}

		// create device
		{
			VkPhysicalDeviceFeatures	device_features	= {};
			vkGetPhysicalDeviceFeatures( _device.GetPhyiscalDevice(), &device_features );

			EQueueFamily::bits	family = EQueueFamily::Default;

			if ( not _window )
			{
				family.Reset( EQueueFamily::Present );
				_settings.flags.Reset( EContextFlags::NoSurface );
			}

			CHECK_ERR( _device.CreateDevice( device_features, family ) );
		}

		// create swapchain
		if ( _window )
		{
			using EFlags = CreateInfo::GpuContext::EFlags;
			
			OSMsg::WindowGetDescription	req_descr;
			_window->Send( req_descr );

			CHECK_ERR( _device.CreateSwapchain( req_descr.result->surfaceSize, _settings.flags[ EFlags::VSync ] ) );
		}

		CHECK_ERR( _device.CreateQueue() );

		_device.WritePhysicalDeviceInfo();
		
		CHECK_ERR( GlobalSystems()->modulesFactory->Create(
								VkMemoryManagerModuleID,
								GlobalSystems(),
								CreateInfo::GpuMemoryManager{ BytesUL(0), ~BytesUL(0), EGpuMemory::bits().SetAll(), EMemoryAccess::bits().SetAll() },
								OUT _memManager ) );
		
		CHECK( _DefCompose( false ) );

		_SendEvent( GpuMsg::DeviceCreated{} );
		
		CHECK_ERR( _CreateSemaphores() );
		return true;
	}
	
/*
=================================================
	_WindowBeforeDestroy
=================================================
*/
	bool VulkanThread::_WindowBeforeDestroy (const OSMsg::WindowBeforeDestroy &)
	{
		_DestroyDevice();
		_DetachWindow();
		
		if ( GetState() != EState::Deleting ) {
			CHECK( _SetState( EState::Initial ) );
		}
		return true;
	}
	
/*
=================================================
	_DestroyDevice
=================================================
*/
	void VulkanThread::_DestroyDevice ()
	{
		if ( _device.IsDeviceCreated() )
		{
			_device.DeviceWaitIdle();

			_SendEvent( GpuMsg::DeviceBeforeDestroy{} );
		}

		if ( _cmdQueue )
		{
			_cmdQueue->Send( ModuleMsg::Delete{} );
			_cmdQueue = null;
		}

		if ( _memManager )
		{
			_memManager->Send( ModuleMsg::Delete{} );
			_memManager = null;
		}

		_DestroySemaphores();

		if ( _syncManager )
		{
			_syncManager->Send( ModuleMsg::Delete{} );
			_syncManager = null;
		}

		_renderPassCache.Destroy();
		_pipelineCache.Destroy();
		_samplerCache.Destroy();
		_layoutCache.Destroy();

		_device.DestroyQueue();
		_device.DestroySwapchain();
		_device.DestroySurface();
		_surface.Destroy();
		_device.DestroyDevice();
		_device.DestroyDebugCallback();
		_device.DestroyInstance();
	}
	
/*
=================================================
	_CreateSemaphores
=================================================
*/
	bool VulkanThread::_CreateSemaphores ()
	{
		CHECK_ERR( _syncManager );

		_DestroySemaphores();

		GpuMsg::CreateSemaphore		sem1_ctor;
		GpuMsg::CreateSemaphore		sem2_ctor;

		_syncManager->Send( sem1_ctor );
		_syncManager->Send( sem2_ctor );

		CHECK_ERR( sem1_ctor.result and sem2_ctor.result );

		_imageAvailable	= *sem1_ctor.result;
		_renderFinished	= *sem2_ctor.result;

		return true;
	}
	
/*
=================================================
	_DestroySemaphores
=================================================
*/
	void VulkanThread::_DestroySemaphores ()
	{
		if ( _syncManager and _imageAvailable != GpuSemaphoreId::Unknown )
		{
			_syncManager->Send( GpuMsg::DestroySemaphore{ _imageAvailable });

			_imageAvailable = GpuSemaphoreId::Unknown;
		}
		
		if ( _syncManager and _renderFinished != GpuSemaphoreId::Unknown )
		{
			_syncManager->Send( GpuMsg::DestroySemaphore{ _renderFinished });

			_renderFinished = GpuSemaphoreId::Unknown;
		}
	}

/*
=================================================
	_WindowVisibilityChanged
=================================================
*/
	bool VulkanThread::_WindowVisibilityChanged (const OSMsg::WindowVisibilityChanged &msg)
	{
		_isWindowVisible = (msg.state != WindowDesc::EVisibility::Invisible);
		return true;
	}

/*
=================================================
	_WindowDescriptionChanged
=================================================
*/
	bool VulkanThread::_WindowDescriptionChanged (const OSMsg::WindowDescriptionChanged &msg)
	{
		if ( _device.IsDeviceCreated()									and
			 msg.descr.visibility != WindowDesc::EVisibility::Invisible	and
			 Any( msg.descr.surfaceSize != _device.GetSurfaceSize() ) )
		{
			_device.DeviceWaitIdle();

			// TODO:
			// destroy command buffers with old framebuffers and images

			CHECK_ERR( _device.RecreateSwapchain( msg.descr.surfaceSize ) );
		}

		return true;
	}

/*
=================================================
	_DetachWindow
=================================================
*/
	void VulkanThread::_DetachWindow ()
	{
		if ( _window )
		{
			CHECK( not _device.IsDeviceCreated() );

			_window->UnsubscribeAll( this );
			_window = null;
		}
	}

/*
=================================================
	_GetDeviceInfo
=================================================
*/
	bool VulkanThread::_GetDeviceInfo (const GpuMsg::GetDeviceInfo &msg)
	{
		GpuMsg::GetDeviceInfo::Info	info;
		info.gpuThread		= this;
		info.sharedThread	= null;
		info.syncManager	= _syncManager;
		info.memManager		= _memManager;
		info.renderPass		= _device.GetDefaultRenderPass();
		info.imageCount		= _device.GetSwapchainLength();

		msg.result.Set( info );
		return true;
	}
	
/*
=================================================
	_GetVkDeviceInfo
=================================================
*/
	bool VulkanThread::_GetVkDeviceInfo (const GpuMsg::GetVkDeviceInfo &msg)
	{
		msg.result.Set({
			_device.GetInstance(),
			_device.GetPhyiscalDevice(),
			_device.GetLogicalDevice()
		});
		return true;
	}

/*
=================================================
	_GetVkPrivateClasses
=================================================
*/
	bool VulkanThread::_GetVkPrivateClasses (const GpuMsg::GetVkPrivateClasses &msg)
	{
		msg.result.Set({
			&_device,
			&_samplerCache,
			&_pipelineCache,
			&_layoutCache,
			&_renderPassCache,
			&_resourceCache,
			_memManager.RawPtr()
		});
		return true;
	}
	
/*
=================================================
	_GetGraphicsSettings
=================================================
*/
	bool VulkanThread::_GetGraphicsSettings (const GpuMsg::GetGraphicsSettings &msg)
	{
		msg.result.Set({ _settings, _device.GetSurfaceSize() });
		return true;
	}

/*
=================================================
	_GetComputeSettings
=================================================
*/
	bool VulkanThread::_GetComputeSettings (const GpuMsg::GetComputeSettings &msg)
	{
		ComputeSettings	cs;
		cs.device	= _settings.device;
		cs.isDebug	= _settings.flags[ GraphicsSettings::EFlags::DebugContext ];
		cs.version	= _settings.version;

		msg.result.Set( RVREF(cs) );
		return true;
	}
	
/*
=================================================
	_GetDeviceProperties
=================================================
*/
	bool VulkanThread::_GetDeviceProperties (const GpuMsg::GetDeviceProperties &msg)
	{
		msg.result.Set( _device.GetProperties() );
		return true;
	}
//-----------------------------------------------------------------------------


/*
=================================================
	CreateVulkanThread
=================================================
*/	
	ModulePtr VulkanObjectsConstructor::CreateVulkanThread (ModuleMsg::UntypedID_t id, GlobalSystemsRef gs, const CreateInfo::GpuThread &ci)
	{
		return New< VulkanThread >( id, gs, ci );
	}

}	// Platforms
}	// Engine

#endif	// GRAPHICS_API_VULKAN
