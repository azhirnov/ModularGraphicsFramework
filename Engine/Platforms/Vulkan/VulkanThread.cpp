// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Engine/Platforms/Vulkan/VulkanThread.h"
#include "Engine/Platforms/Vulkan/VulkanContext.h"
#include "Engine/Platforms/Shared/GPU/CommandBuffer.h"
#include "Engine/Platforms/Windows/WinMessages.h"

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
		Module( gs, ModuleConfig{ VkThreadModuleID, 1 }, &_msgTypes, &_eventTypes ),
		_settings( ci.settings ),				_device( GlobalSystems(), VkSystems() ),
		_samplerCache( _device.VkSystems() ),	_pipelineCache( _device.VkSystems() ),
		_layoutCache( _device.VkSystems() ),	_renderPassCache( _device.VkSystems() ),
		_cmdControl( VkSystems() ),				_isWindowVisible( false )
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
		_SubscribeOnMsg( this, &VulkanThread::_Delete );
		_SubscribeOnMsg( this, &VulkanThread::_ThreadBeginFrame );
		_SubscribeOnMsg( this, &VulkanThread::_ThreadEndFrame );
		_SubscribeOnMsg( this, &VulkanThread::_SubmitGraphicsQueueCommands );
		_SubscribeOnMsg( this, &VulkanThread::_WindowCreated );
		_SubscribeOnMsg( this, &VulkanThread::_WindowBeforeDestroy );
		_SubscribeOnMsg( this, &VulkanThread::_WindowVisibilityChanged );
		_SubscribeOnMsg( this, &VulkanThread::_WindowDescriptorChanged );
		_SubscribeOnMsg( this, &VulkanThread::_GetVkLogicDevice );
		
		CHECK( _ValidateMsgSubscriptions() );

		VkSystems()->GetSetter< VulkanThread >().Set( this );
		
		CHECK( ci.shared.IsNull() );	// sharing is not supported yet

		_AttachSelfToManager( ci.context, VkContextModuleID, true );
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
		if ( _IsComposedOrLinkedState( GetState() ) )
			return true;	// already linked

		CHECK_ERR( GetState() == EState::Initial or GetState() == EState::LinkingFailed );
		
		// TODO: use SearchModule message
		// TODO: reset to initial state if window was detached
		_window = _GetParents().Front()->GetModuleByID( WinWindowModuleID );
		CHECK_ATTACHMENT( _window );

		_window->Subscribe( this, &VulkanThread::_WindowCreated );
		_window->Subscribe( this, &VulkanThread::_WindowBeforeDestroy );
		_window->Subscribe( this, &VulkanThread::_WindowDescriptorChanged );
		
		CHECK_ERR( Module::_Link_Impl( msg ) );

		if ( _IsComposedState( _window->GetState() ) )
		{
			Message< ModuleMsg::WindowGetHandle >	request_hwnd;

			SendTo( _window, request_hwnd );

			if ( request_hwnd->hwnd.IsDefined() )
			{
				_SendMsg< ModuleMsg::WindowCreated >({ WindowDesc(), request_hwnd->hwnd.Get() });
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
		if ( not _IsComposedState( GetState() ) or
			 not _isWindowVisible )
		{
			return true;
		}

		CHECK_ERR( Module::_Update_Impl( msg ) );
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
	_AddToManager
=================================================
*/
	bool VulkanThread::_AddToManager (const Message< ModuleMsg::AddToManager > &msg)
	{
		return true;
	}
	
/*
=================================================
	_RemoveFromManager
=================================================
*/
	bool VulkanThread::_RemoveFromManager (const Message< ModuleMsg::RemoveFromManager > &msg)
	{
		return true;
	}

/*
=================================================
	_ThreadBeginFrame
=================================================
*/
	bool VulkanThread::_ThreadBeginFrame (const Message< GpuMsg::ThreadBeginFrame > &msg)
	{
		CHECK_ERR( _IsComposedState( GetState() ) );

		CHECK_ERR( not _device.IsFrameStarted() );
		CHECK_ERR( _device.BeginFrame() );

		msg->result.Set({ _device.GetCurrentFramebuffer(), _device.GetCommandBuilder(), _device.GetImageIndex() });
		return true;
	}

/*
=================================================
	_ThreadEndFrame
=================================================
*/
	bool VulkanThread::_ThreadEndFrame (const Message< GpuMsg::ThreadEndFrame > &msg)
	{
		CHECK_ERR( _device.IsFrameStarted() );

		if ( msg->framebuffer )
			CHECK_ERR( msg->framebuffer == _device.GetCurrentFramebuffer() );

		if ( not msg->commands.Empty() )
			CHECK_ERR( _cmdControl.SubmitQueue( &_device, msg->commands ) );

		CHECK_ERR( _device.EndFrame() );
		return true;
	}

/*
=================================================
	_SubmitGraphicsQueueCommands
=================================================
*/
	bool VulkanThread::_SubmitGraphicsQueueCommands (const Message< GpuMsg::SubmitGraphicsQueueCommands > &msg)
	{
		CHECK_ERR( _device.IsFrameStarted() );

		TODO( "_SubmitGraphicsQueueCommands" );
		//CHECK_ERR( _device.SubmitQueue( msg->commands ) );
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
			CHECK( _DefCompose( false ) );
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
			using EQueueFamily = Vk1Device::EQueueFamily;

			VkPhysicalDeviceFeatures	device_features	= {};
			vkGetPhysicalDeviceFeatures( _device.GetPhyiscalDevice(), &device_features );

			CHECK_ERR( _device.CreateDevice( device_features, EQueueFamily::bits().SetAll() ) );
		}

		// create swapchain
		{
			using EFlags = CreateInfo::GpuContext::EFlags;

			CHECK_ERR( _device.CreateSwapchain( msg.descr.surfaceSize, _settings.flags.Get( EFlags::VSync ) ) );
			CHECK_ERR( _device.CreateQueue() );
		}
		
		_SendEvent( Message< GpuMsg::DeviceCreated >{} );
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

			_cmdControl.Destroy();

			_SendEvent( Message< GpuMsg::DeviceBeforeDestroy >{} );
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
		vk::Vk1_Delete();

		_DetachWindow();
		
		CHECK( _SetState( EState::Initial ) );
		return true;
	}
	
/*
=================================================
	_WindowVisibilityChanged
=================================================
*/
	bool VulkanThread::_WindowVisibilityChanged (const Message< ModuleMsg::WindowVisibilityChanged > &msg)
	{
		_isWindowVisible = (msg->state != WindowDesc::EVisibility::Invisible);
		return true;
	}

/*
=================================================
	_WindowDescriptorChanged
=================================================
*/
	bool VulkanThread::_WindowDescriptorChanged (const Message< ModuleMsg::WindowDescriptorChanged > &msg)
	{
		if ( _device.IsDeviceCreated()									and
			 msg->desc.visibility != WindowDesc::EVisibility::Invisible	and
			 Any( msg->desc.surfaceSize != _device.GetSurfaceSize() ) )
		{
			_device.DeviceWaitIdle();

			_cmdControl.FreeAll();

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

			_window->UnsubscribeAll( this );
			_window = null;
		}
	}
	
/*
=================================================
	_GetVkLogicDevice
=================================================
*/
	bool VulkanThread::_GetVkLogicDevice (const Message< GpuMsg::GetVkLogicDevice > &msg)
	{
		msg->result.Set( GetDevice()->GetLogicalDevice() );
		return true;
	}
//-----------------------------------------------------------------------------


	
/*
=================================================
	constructor
=================================================
*/
	VulkanThread::CmdBufferLifeControl::CmdBufferLifeControl (const VkSystemsRef vkSys)
	{
		tempCmdBuffers.Reserve( 16 );

		FOR( i, perFrameData )
		{
			perFrameData[i].fence = New< PlatformVK::Vk1Fence >( vkSys );
			perFrameData[i].cmdBuffers.Reserve( 16 );
		}
	}
	
/*
=================================================
	destructor
=================================================
*/
	VulkanThread::CmdBufferLifeControl::~CmdBufferLifeControl ()
	{
		FOR( i, perFrameData )
		{
			if ( perFrameData[i].fence )
				ASSERT( not perFrameData[i].fence->IsCreated() );

			ASSERT( perFrameData[i].cmdBuffers.Empty() );
		}
	}

/*
=================================================
	SubmitQueue
=================================================
*/
	bool VulkanThread::CmdBufferLifeControl::SubmitQueue (Ptr<Device> device, ArrayCRef<ModulePtr> cmdBuffers)
	{
		using namespace vk;

		++frameIndex;
		FreeBuffers( device->GetSwapchainLength() );

		auto&	frame_data	= perFrameData[ frameIndex ];

		// get command buffer IDs
		tempCmdBuffers.Clear();
		tempCmdBuffers.Reserve( cmdBuffers.Count() );

		FOR( i, cmdBuffers )
		{
			Message< GpuMsg::GetVkCommandBufferID >		req_id;
			cmdBuffers[i]->Send( req_id );

			VkCommandBuffer	cmd = req_id->result.Get( VK_NULL_HANDLE );

			if ( cmd != VK_NULL_HANDLE )
				tempCmdBuffers.PushBack( cmd );
		}

		CHECK_ERR( not tempCmdBuffers.Empty() );
		
		// submit
		CHECK_ERR( frame_data.fence->Create( false ) );
		CHECK_ERR( device->SubmitQueue( tempCmdBuffers, frame_data.fence->GetFenceID() ) );

		// change state
		FOR( i, cmdBuffers )
		{
			Message< GpuMsg::SetCommandBufferState >	submit_state{ GpuMsg::SetCommandBufferState::EState::Pending };
			cmdBuffers[i]->Send( submit_state );
		}

		frame_data.imageIndex	= device->GetImageIndex();
		frame_data.cmdBuffers	= cmdBuffers;
		return true;
	}
	
/*
=================================================
	FreeBuffers
=================================================
*/
	void VulkanThread::CmdBufferLifeControl::FreeBuffers (uint length)
	{
		Message< GpuMsg::SetCommandBufferState >	submit_state{ GpuMsg::SetCommandBufferState::EState::Completed };

		FrameIndex_t	begin_index = frameIndex;
		FrameIndex_t	end_index	= frameIndex - length;

		ASSERT( begin_index != end_index );

		for (FrameIndex_t i = begin_index; i != end_index; ++i)
		{
			auto&	frame_data	= perFrameData[i];

			if ( frame_data.fence->IsCreated() )
				frame_data.fence->Wait();
			
			FOR( j, frame_data.cmdBuffers ) {
				frame_data.cmdBuffers[j]->Send( submit_state );
			}

			frame_data.cmdBuffers.Clear();
		}
	}
	
/*
=================================================
	Resize
=================================================
*/		
	bool VulkanThread::CmdBufferLifeControl::Resize (uint size)
	{
		CHECK_ERR( size < perFrameData.Count() );

		frameIndex.SetLimits( 0, size );
		return true;
	}
		
/*
=================================================
	FreeAll
=================================================
*/	
	void VulkanThread::CmdBufferLifeControl::FreeAll ()
	{
		tempCmdBuffers.Clear();
		frameIndex = 0;
		
		Message< GpuMsg::SetCommandBufferState >	submit_state{ GpuMsg::SetCommandBufferState::EState::Completed };

		FOR( i, perFrameData )
		{
			auto&	frame_data	= perFrameData[i];

			frame_data.fence->Destroy();
			
			FOR( j, frame_data.cmdBuffers ) {
				frame_data.cmdBuffers[j]->Send( submit_state );
			}
			frame_data.cmdBuffers.Clear();
		}
	}
	
/*
=================================================
	Destroy
=================================================
*/	
	void VulkanThread::CmdBufferLifeControl::Destroy ()
	{
		tempCmdBuffers.Clear();
		perFrameData.Clear();
	}


}	// Platforms
}	// Engine

#endif	// GRAPHICS_API_VULKAN
