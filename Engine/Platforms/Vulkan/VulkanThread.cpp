// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Engine/Platforms/Vulkan/VulkanThread.h"
#include "Engine/Platforms/Vulkan/VulkanContext.h"
#include "Engine/Platforms/Windows/WinWindow.h"

#if defined( GRAPHICS_API_VULKAN )

namespace Engine
{
namespace Platforms
{
	
	const Runtime::VirtualTypeList	VulkanThread::_msgTypes{ UninitializedT< SupportedMessages_t >() };
	const Runtime::VirtualTypeList	VulkanThread::_eventTypes{ UninitializedT< SupportedEvents_t >() };

/*
=================================================
	constructor
=================================================
*/
	VulkanThread::VulkanThread (const GlobalSystemsRef gs, const CreateInfo::GpuThread &ci) :
		Module( gs, ModuleConfig{ GetStaticID(), 1 }, &_msgTypes, &_eventTypes ),
		_settings( ci.settings ),
		_device( GlobalSystems(), VkSystems() ),
		_samplerCache( _device.VkSystems() ),
		_pipelineCache( _device.VkSystems() ),
		_renderPassCache( _device.VkSystems() ),
		_updateInProgress( false )
	{
		SetDebugName( "VulkanThread" );
		
		_SubscribeOnMsg( this, &VulkanThread::_OnModuleAttached_Impl );
		_SubscribeOnMsg( this, &VulkanThread::_OnModuleDetached_Impl );
		_SubscribeOnMsg( this, &VulkanThread::_AttachModule_Impl );
		_SubscribeOnMsg( this, &VulkanThread::_DetachModule_Impl );
		_SubscribeOnMsg( this, &VulkanThread::_OnManagerChanged_Empty );
		_SubscribeOnMsg( this, &VulkanThread::_FindModule_Impl );
		_SubscribeOnMsg( this, &VulkanThread::_ModulesDeepSearch_Impl );
		_SubscribeOnMsg( this, &VulkanThread::_Update );
		_SubscribeOnMsg( this, &VulkanThread::_Link );
		_SubscribeOnMsg( this, &VulkanThread::_Delete );
		_SubscribeOnMsg( this, &VulkanThread::_GpuThreadBeginFrame );
		_SubscribeOnMsg( this, &VulkanThread::_GpuThreadEndFrame );
		_SubscribeOnMsg( this, &VulkanThread::_SubmitGraphicsQueueCommands );
		_SubscribeOnMsg( this, &VulkanThread::_WindowCreated );
		_SubscribeOnMsg( this, &VulkanThread::_WindowBeforeDestroy );
		_SubscribeOnMsg( this, &VulkanThread::_WindowDescriptorChanged );
		
		CHECK( _ValidateMsgSubscriptions() );

		VkSystems()->GetSetter< VulkanThread >().Set( this );
		
		CHECK( ci.shared.IsNull() );	// sharing is not supported yet

		_AttachSelfToManager( ci.context, VulkanContext::GetStaticID(), true );
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

		VkSystems()->GetSetter< VulkanThread >().Set( null );
	}
	
/*
=================================================
	_Link
=================================================
*/
	bool VulkanThread::_Link (const Message< ModuleMsg::Link > &msg)
	{
		CHECK_ERR( _IsMutableState( GetState() ) );

		_window = _GetParents().Front()->GetModule( WinWindow::GetStaticID() );
		CHECK_ERR( _window );

		_window->Subscribe( this, &VulkanThread::_WindowCreated );
		_window->Subscribe( this, &VulkanThread::_WindowBeforeDestroy );
		_window->Subscribe( this, &VulkanThread::_WindowDescriptorChanged );
		
		CHECK_ERR( Module::_Link_Impl( msg ) );

		if ( _IsComposedState( _window->GetState() ) )
		{
			Message< ModuleMsg::WindowGetHandle >	request_hwnd;

			_window->Send( request_hwnd );

			if ( request_hwnd->hwnd.IsDefined() )
			{
				_SendMsg( Message< ModuleMsg::WindowCreated >{ this, WindowDesc(), request_hwnd->hwnd.Get() } );
			}
		}
		return true;
	}
	
/*
=================================================
	_Update
=================================================
*/
	bool VulkanThread::_Update (const Message< ModuleMsg::Update > &msg)
	{
		/*if ( not _IsComposedState( GetState() ) )
			return true;
		
		CHECK_ERR( _device.BeginFrame() );
		_updateInProgress = true;

		CHECK_ERR( Module::_Update_Impl( msg ) );

		CHECK_ERR( _device.EndFrame() );
		_updateInProgress = false;
		*/
		return true;
	}

/*
=================================================
	_Delete
=================================================
*/
	bool VulkanThread::_Delete (const Message< ModuleMsg::Delete > &msg)
	{
		_DetachWindow();

		CHECK_ERR( Module::_Delete_Impl( msg ) );
		return true;
	}
	
/*
=================================================
	_GpuThreadBeginFrame
=================================================
*/
	bool VulkanThread::_GpuThreadBeginFrame (const Message< ModuleMsg::GpuThreadBeginFrame > &msg)
	{
		CHECK_ERR( not _device.IsFrameStarted() );
		CHECK_ERR( _device.BeginFrame() );

		msg->framebuffer.Set( _device.GetCurrentFramebuffer() );
		return true;
	}
	
/*
=================================================
	_GpuThreadEndFrame
=================================================
*/
	bool VulkanThread::_GpuThreadEndFrame (const Message< ModuleMsg::GpuThreadEndFrame > &msg)
	{
		CHECK_ERR( _device.IsFrameStarted() );

		if ( msg->framebuffer )
			CHECK_ERR( msg->framebuffer == _device.GetCurrentFramebuffer() );

		if ( not msg->commands.Empty() )
			CHECK_ERR( _device.SubmitGraphicsQueue( msg->commands ) );

		CHECK_ERR( _device.EndFrame() );
		return true;
	}

/*
=================================================
	_SubmitGraphicsQueueCommands
=================================================
*/
	bool VulkanThread::_SubmitGraphicsQueueCommands (const Message< ModuleMsg::SubmitGraphicsQueueCommands > &msg)
	{
		CHECK_ERR( _device.IsFrameStarted() );

		CHECK_ERR( _device.SubmitGraphicsQueue( msg->commands ) );
		return true;
	}

/*
=================================================
	_WindowCreated
=================================================
*/
	bool VulkanThread::_WindowCreated (const Message< ModuleMsg::WindowCreated > &msg)
	{
		if ( _CreateDevice( *msg ) )
		{
			CHECK( _Compose( true ) );
		}
		else
		{
			CHECK( _SetState( EState::ComposingFailed ) );
		}
		return true;
	}

/*
=================================================
	_CreateDevice
=================================================
*/
	bool VulkanThread::_CreateDevice (const ModuleMsg::WindowCreated &msg)
	{
		using namespace vk;
		using namespace PlatformVK;

		// create instance
		{
			Array<const char*>		extensions		= {	VK_EXT_DEBUG_REPORT_EXTENSION_NAME };

			Array<const char*>		validation		= {	"VK_LAYER_LUNARG_standard_validation" };

			const int				debug_flags		= VK_DEBUG_REPORT_WARNING_BIT_EXT |
													  VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT |
													  VK_DEBUG_REPORT_ERROR_BIT_EXT;

			_surface.RequestExtensions( INOUT extensions );

			CHECK_ERR( Vk1_Init( null ) );
			CHECK_ERR( _device.CreateInstance( "", 0, VK_API_VERSION_1_0, extensions, validation ) );
			CHECK_ERR( Vk1_Init( _device.GetInstance() ) );

			CHECK_ERR( _device.CreateDebugCallback( (VkDebugReportFlagBitsEXT) debug_flags ) );
			CHECK_ERR( _device.ChoosePhysicalDevice( "" ) );
		}

		// create surface
		{
			VkSurfaceKHR	surface	= VK_NULL_HANDLE;

			_surface.Create( _device.GetInstance(), msg.hwnd, OUT surface );
			CHECK_ERR( _device.SetSurface( surface, _settings.colorFmt ) );
		}

		// create device
		{
			VkPhysicalDeviceFeatures	device_features	= {};
			vkGetPhysicalDeviceFeatures( _device.GetPhyiscalDevice(), &device_features );

			CHECK_ERR( _device.CreateDevice( device_features, VK_QUEUE_GRAPHICS_BIT | VK_QUEUE_COMPUTE_BIT, true ) );
		}

		// create swapchain
		{
			CHECK_ERR( _device.CreateSwapchain( msg.descr.surfaceSize, true ) );
		}
		
		_SendEvent( Message< ModuleMsg::GpuDeviceCreated >{ this } );
		return true;
	}
	
/*
=================================================
	_WindowBeforeDestroy
=================================================
*/
	bool VulkanThread::_WindowBeforeDestroy (const Message< ModuleMsg::WindowBeforeDestroy > &msg)
	{
		if ( _device.IsDeviceCreated() )
		{
			_device.DeviceWaitIdle();

			_SendEvent( Message< ModuleMsg::GpuDeviceBeforeDestory >{ this } );
		}

		_pipelineCache.DestroyCache();

		_device.DestroySwapchain();
		_device.DestroySurface();
		_surface.Destroy();
		_device.DestroyDevice();
		_device.DestroyDebugCallback();
		_device.DestroyInstance();
		vk::Vk1_Delete();

		_DetachWindow();
		
		CHECK( _SetState( EState::Initial ) );
		return true;
	}
	
/*
=================================================
	_WindowDescriptorChanged
=================================================
*/
	bool VulkanThread::_WindowDescriptorChanged (const Message< ModuleMsg::WindowDescriptorChanged > &msg)
	{
		if ( _device.IsDeviceCreated() and
			 Any( msg->desc.surfaceSize != _device.GetSurfaceSize() ) )
		{
			CHECK_ERR( _device.RecreateSwapchain( msg->desc.surfaceSize ) );
		}

		return true;
	}

/*
=================================================
	_WindowBeforeDestroy
=================================================
*/
	void VulkanThread::_DetachWindow ()
	{
		if ( _window )
		{
			CHECK( not _device.IsDeviceCreated() );
			//CHECK( not _context.IsCreated() );

			_window->UnsubscribeAll( this );
			_window = null;
		}
	}


}	// Platforms
}	// Engine

#endif	// GRAPHICS_API_VULKAN
