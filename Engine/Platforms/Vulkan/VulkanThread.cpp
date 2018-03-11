// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Engine/Config/Engine.Config.h"

#ifdef GRAPHICS_API_VULKAN

#include "Engine/Platforms/Public/Tools/WindowHelper.h"
#include "Engine/Platforms/Public/GPU/Thread.h"
#include "Engine/Platforms/Public/GPU/CommandBuffer.h"
#include "Engine/Platforms/Public/GPU/Memory.h"
#include "Engine/Platforms/Vulkan/VulkanObjectsConstructor.h"
#include "Engine/Platforms/Vulkan/Windows/VkWinSurface.h"
#include "Engine/Platforms/Vulkan/Impl/Vk1Device.h"
#include "Engine/Platforms/Vulkan/Impl/Vk1PipelineCache.h"
#include "Engine/Platforms/Vulkan/Impl/Vk1PipelineLayout.h"
#include "Engine/Platforms/Vulkan/Impl/Vk1RenderPass.h"
#include "Engine/Platforms/Vulkan/Impl/Vk1Sampler.h"

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
		using SupportedMessages_t	= Module::SupportedMessages_t::Erase< MessageListFrom<
											ModuleMsg::Compose
										> >
										::Append< MessageListFrom<
											ModuleMsg::AddToManager,
											ModuleMsg::RemoveFromManager,
											ModuleMsg::OnManagerChanged,
											OSMsg::WindowCreated,
											OSMsg::WindowBeforeDestroy,
											GpuMsg::GetGraphicsModules,
											GpuMsg::ThreadBeginFrame,
											GpuMsg::ThreadEndFrame,
											GpuMsg::SubmitGraphicsQueueCommands,
											GpuMsg::SubmitComputeQueueCommands,
											GpuMsg::GetDeviceInfo,
											GpuMsg::GetVkDeviceInfo,
											GpuMsg::GetVkPrivateClasses,
											GpuMsg::GetGraphicsSettings,
											GpuMsg::GetComputeSettings
										> >;
		using SupportedEvents_t		= Module::SupportedEvents_t::Append< MessageListFrom<
											GpuMsg::ThreadBeginFrame,
											GpuMsg::ThreadEndFrame,
											GpuMsg::DeviceCreated,
											GpuMsg::DeviceBeforeDestroy
											// TODO: device lost event
										> >;

		using CmdBUffers_t			= GpuMsg::ThreadEndFrame::Commands_t;
		using VkCmdBuffers_t		= FixedSizeArray< vk::VkCommandBuffer, CmdBUffers_t::MemoryContainer_t::SIZE >;
		using Semaphores_t			= FixedSizeArray< vk::VkSemaphore, GpuMsg::SubmitGraphicsQueueCommands::Semaphores_t::MemoryContainer_t::SIZE + 2 >;
		using PipelineStages_t		= FixedSizeArray< vk::VkPipelineStageFlags, Semaphores_t::MemoryContainer_t::SIZE >;

		using Surface				= PlatformVK::Vk1Surface;
		using Device				= PlatformVK::Vk1Device;
		using SamplerCache			= PlatformVK::Vk1SamplerCache;
		using PipelineCache			= PlatformVK::Vk1PipelineCache;
		using LayoutCache			= PlatformVK::Vk1PipelineLayoutCache;
		using RenderPassCache		= PlatformVK::Vk1RenderPassCache;


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

		Surface					_surface;
		Device					_device;

		SamplerCache			_samplerCache;
		PipelineCache			_pipelineCache;
		LayoutCache				_layoutCache;
		RenderPassCache			_renderPassCache;

		bool					_isWindowVisible;


	// methods
	public:
		VulkanThread (GlobalSystemsRef gs, const CreateInfo::GpuThread &ci);
		~VulkanThread ();
		

	// message handlers
	private:
		bool _Delete (const Message< ModuleMsg::Delete > &);
		bool _Link (const Message< ModuleMsg::Link > &);
		bool _Update (const Message< ModuleMsg::Update > &);
		bool _AddToManager (const Message< ModuleMsg::AddToManager > &);
		bool _RemoveFromManager (const Message< ModuleMsg::RemoveFromManager > &);
		bool _GetGraphicsModules (const Message< GpuMsg::GetGraphicsModules > &);

		bool _ThreadBeginFrame (const Message< GpuMsg::ThreadBeginFrame > &);
		bool _ThreadEndFrame (const Message< GpuMsg::ThreadEndFrame > &);
		bool _SubmitGraphicsQueueCommands (const Message< GpuMsg::SubmitGraphicsQueueCommands > &);
		bool _SubmitComputeQueueCommands (const Message< GpuMsg::SubmitComputeQueueCommands > &);
		bool _GetGraphicsSettings (const Message< GpuMsg::GetGraphicsSettings > &);
		bool _GetComputeSettings (const Message< GpuMsg::GetComputeSettings > &);

		bool _WindowCreated (const Message< OSMsg::WindowCreated > &);
		bool _WindowBeforeDestroy (const Message< OSMsg::WindowBeforeDestroy > &);
		bool _WindowVisibilityChanged (const Message< OSMsg::WindowVisibilityChanged > &);
		bool _WindowDescriptorChanged (const Message< OSMsg::WindowDescriptorChanged > &);
		
		bool _GetDeviceInfo (const Message< GpuMsg::GetDeviceInfo > &);
		bool _GetVkDeviceInfo (const Message< GpuMsg::GetVkDeviceInfo > &);
		bool _GetVkPrivateClasses (const Message< GpuMsg::GetVkPrivateClasses > &);

	private:
		bool _CreateDevice ();
		void _DestroyDevice ();
		void _DetachWindow ();
		
		bool _SubmitQueue (const GpuMsg::SubmitGraphicsQueueCommands *);

		static bool _GetCmdBufferIds (const CmdBUffers_t &in, OUT VkCmdBuffers_t &out);
	};
//-----------------------------------------------------------------------------


	
	const TypeIdList	VulkanThread::_msgTypes{ UninitializedT< SupportedMessages_t >() };
	const TypeIdList	VulkanThread::_eventTypes{ UninitializedT< SupportedEvents_t >() };

/*
=================================================
	constructor
=================================================
*/
	VulkanThread::VulkanThread (GlobalSystemsRef gs, const CreateInfo::GpuThread &ci) :
		Module( gs, ModuleConfig{ VkThreadModuleID, 1 }, &_msgTypes, &_eventTypes ),
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
		_SubscribeOnMsg( this, &VulkanThread::_Delete );
		_SubscribeOnMsg( this, &VulkanThread::_GetGraphicsModules );
		_SubscribeOnMsg( this, &VulkanThread::_ThreadBeginFrame );
		_SubscribeOnMsg( this, &VulkanThread::_ThreadEndFrame );
		_SubscribeOnMsg( this, &VulkanThread::_SubmitGraphicsQueueCommands );
		_SubscribeOnMsg( this, &VulkanThread::_SubmitComputeQueueCommands );
		_SubscribeOnMsg( this, &VulkanThread::_WindowCreated );
		_SubscribeOnMsg( this, &VulkanThread::_WindowBeforeDestroy );
		_SubscribeOnMsg( this, &VulkanThread::_GetDeviceInfo );
		_SubscribeOnMsg( this, &VulkanThread::_GetVkDeviceInfo );
		_SubscribeOnMsg( this, &VulkanThread::_GetVkPrivateClasses );
		_SubscribeOnMsg( this, &VulkanThread::_GetGraphicsSettings );
		_SubscribeOnMsg( this, &VulkanThread::_GetComputeSettings );
		
		CHECK( _ValidateMsgSubscriptions() );
		
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
	bool VulkanThread::_Link (const Message< ModuleMsg::Link > &msg)
	{
		if ( _IsComposedOrLinkedState( GetState() ) )
			return true;	// already linked

		CHECK_ERR( GetState() == EState::Initial or GetState() == EState::LinkingFailed );
		
		CHECK_LINKING(( _window = PlatformTools::WindowHelper::FindWindow( GlobalSystems() ) ));

		_window->Subscribe( this, &VulkanThread::_WindowCreated );
		_window->Subscribe( this, &VulkanThread::_WindowBeforeDestroy );
		_window->Subscribe( this, &VulkanThread::_WindowVisibilityChanged );
		_window->Subscribe( this, &VulkanThread::_WindowDescriptorChanged );
		
		CHECK_ERR( Module::_Link_Impl( msg ) );

		// create sync manager
		{
			CHECK_ERR( GlobalSystems()->modulesFactory->Create(
											VkSyncManagerModuleID,
											GlobalSystems(),
											CreateInfo::GpuSyncManager{ this },
											OUT _syncManager ) );

			CHECK_ERR( _Attach( "sync", _syncManager, true ) );
			_syncManager->Send( msg );
		}
		
		// if window already created
		if ( _IsComposedState( _window->GetState() ) )
		{
			_SendMsg< OSMsg::WindowCreated >({});
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
	bool VulkanThread::_Delete (const Message< ModuleMsg::Delete > &msg)
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
	bool VulkanThread::_AddToManager (const Message< ModuleMsg::AddToManager > &)
	{
		return true;
	}
	
/*
=================================================
	_RemoveFromManager
=================================================
*/
	bool VulkanThread::_RemoveFromManager (const Message< ModuleMsg::RemoveFromManager > &)
	{
		return true;
	}
	
/*
=================================================
	_GetGraphicsModules
=================================================
*/	
	bool VulkanThread::_GetGraphicsModules (const Message< GpuMsg::GetGraphicsModules > &msg)
	{
		msg->compute.Set( VulkanObjectsConstructor::GetComputeModules() );
		msg->graphics.Set( VulkanObjectsConstructor::GetGraphicsModules() );
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

		msg->result.Set({ _device.GetCurrentFramebuffer(), _device.GetImageIndex() });
		return true;
	}
	
/*
=================================================
	_GetCmdBufferIds
=================================================
*/
	bool VulkanThread::_GetCmdBufferIds (const CmdBUffers_t &in, OUT VkCmdBuffers_t &out)
	{
		FOR( i, in )
		{
			CHECK_ERR( in[i] );

			Message< GpuMsg::GetVkCommandBufferID >	req_id;
			in[i]->Send( req_id );

			DEBUG_ONLY(
				Message< GpuMsg::GetCommandBufferDescriptor >	req_descr;
				in[i]->Send( req_descr );
				CHECK_ERR( req_descr->result and not req_descr->result->flags[ ECmdBufferCreate::Secondary ] );
			);

			out.PushBack( *req_id->result );
		}
		return true;
	}

/*
=================================================
	_ThreadEndFrame
=================================================
*/
	bool VulkanThread::_ThreadEndFrame (const Message< GpuMsg::ThreadEndFrame > &msg)
	{
		using namespace vk;

		CHECK_ERR( _device.IsFrameStarted() );

		if ( msg->framebuffer )
			CHECK_ERR( msg->framebuffer == _device.GetCurrentFramebuffer() );

		if ( not msg->commands.Empty() )
		{
			// commands
			VkCmdBuffers_t	cmd_ids;
			CHECK_ERR( _GetCmdBufferIds( msg->commands, OUT cmd_ids ) );

			// wait semaphores
			Semaphores_t		wait_semaphores;
			PipelineStages_t	wait_stages;

			FOR( i, msg->waitSemaphores )
			{
				Message< GpuMsg::GetVkSemaphore >	req_sem{ msg->waitSemaphores[i].first };
				_syncManager->Send( req_sem );

				wait_semaphores.PushBack( *req_sem->result );
				wait_stages.PushBack( PlatformVK::Vk1Enum( msg->waitSemaphores[i].second ) );
			}

			// signal semaphores
			Semaphores_t	signal_semaphores;

			FOR( i, msg->signalSemaphores )
			{
				Message< GpuMsg::GetVkSemaphore >	req_sem{ msg->signalSemaphores[i] };
				_syncManager->Send( req_sem );

				signal_semaphores.PushBack( *req_sem->result );
			}
		
			// fence
			VkFence		fence = VK_NULL_HANDLE;

			if ( msg->fence != GpuFenceId::Unknown )
			{
				Message< GpuMsg::GetVkFence >	req_fence{ msg->fence };
				CHECK( _syncManager->Send( req_fence ) );

				fence = *req_fence->result;
				VK_CALL( vkResetFences( _device.GetLogicalDevice(), 1, &fence ) );
			}

			// submit
			CHECK_ERR( _device.SubmitQueue( cmd_ids, fence, wait_semaphores, wait_stages, signal_semaphores ) );
			
			ModuleUtils::Send( msg->commands, Message< GpuMsg::SetCommandBufferState >{ GpuMsg::SetCommandBufferState::EState::Pending });
		}
		else
		{
			CHECK_ERR( msg->fence == GpuFenceId::Unknown );
		}

		CHECK_ERR( _device.EndFrame() );
		return true;
	}

/*
=================================================
	_SubmitQueue
=================================================
*/
	bool VulkanThread::_SubmitQueue (const GpuMsg::SubmitGraphicsQueueCommands *msg)
	{
		using namespace vk;

		// commands
		VkCmdBuffers_t	cmd_ids;
		CHECK_ERR( _GetCmdBufferIds( msg->commands, OUT cmd_ids ) );

		// wait semaphores
		Semaphores_t		wait_semaphores;
		PipelineStages_t	wait_stages;

		FOR( i, msg->waitSemaphores )
		{
			Message< GpuMsg::GetVkSemaphore >	req_sem{ msg->waitSemaphores[i].first };
			_syncManager->Send( req_sem );

			wait_semaphores.PushBack( *req_sem->result );
			wait_stages.PushBack( PlatformVK::Vk1Enum( msg->waitSemaphores[i].second ) );
		}

		// signal semaphores
		Semaphores_t	signal_semaphores;

		FOR( i, msg->signalSemaphores )
		{
			Message< GpuMsg::GetVkSemaphore >	req_sem{ msg->signalSemaphores[i] };
			_syncManager->Send( req_sem );

			signal_semaphores.PushBack( *req_sem->result );
		}
		
		// fence
		VkFence		fence = VK_NULL_HANDLE;

		if ( msg->fence != GpuFenceId::Unknown )
		{
			Message< GpuMsg::GetVkFence >	req_fence{ msg->fence };
			CHECK( _syncManager->Send( req_fence ) );

			fence = *req_fence->result;
			VK_CALL( vkResetFences( _device.GetLogicalDevice(), 1, &fence ) );
		}
		
		VkSubmitInfo					submit_info = {};
		submit_info.sType					= VK_STRUCTURE_TYPE_SUBMIT_INFO;
		submit_info.commandBufferCount		= (uint) cmd_ids.Count();
		submit_info.pCommandBuffers			= cmd_ids.RawPtr();
		submit_info.waitSemaphoreCount		= (uint32_t) wait_semaphores.Count();
		submit_info.pWaitSemaphores			= wait_semaphores.RawPtr();
		submit_info.pWaitDstStageMask		= wait_stages.RawPtr();
		submit_info.signalSemaphoreCount	= (uint32_t) signal_semaphores.Count();
		submit_info.pSignalSemaphores		= signal_semaphores.RawPtr();

		VK_CHECK( vkQueueSubmit( _device.GetQueue(), 1, &submit_info, fence ) );
		
		ModuleUtils::Send( msg->commands, Message< GpuMsg::SetCommandBufferState >{ GpuMsg::SetCommandBufferState::EState::Pending });
		return true;
	}
	
/*
=================================================
	_SubmitGraphicsQueueCommands
=================================================
*/
	bool VulkanThread::_SubmitGraphicsQueueCommands (const Message< GpuMsg::SubmitGraphicsQueueCommands > &msg)
	{
		CHECK_ERR( _device.IsDeviceCreated() );
		CHECK_ERR( _device.GetQueueFamily()[ Device::EQueueFamily::Graphics ] );

		return _SubmitQueue( msg.operator->() );
	}
	
/*
=================================================
	_SubmitComputeQueueCommands
=================================================
*/
	bool VulkanThread::_SubmitComputeQueueCommands (const Message< GpuMsg::SubmitComputeQueueCommands > &msg)
	{
		CHECK_ERR( _device.IsDeviceCreated() );
		CHECK_ERR( _device.GetQueueFamily()[ Device::EQueueFamily::Compute ] );
		
		return _SubmitQueue( msg.operator->() );
	}

/*
=================================================
	_WindowCreated
=================================================
*/
	bool VulkanThread::_WindowCreated (const Message< OSMsg::WindowCreated > &)
	{
		CHECK_ERR( GetState() == EState::Linked );

		if ( _CreateDevice() ) {
			CHECK( _DefCompose( false ) );
		} else {
			CHECK( _SetState( EState::ComposingFailed ) );
		}
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
		using namespace PlatformVK;
		using namespace CreateInfo;
		
		using EContextFlags = CreateInfo::GpuContext::EFlags;

		Message< OSMsg::WindowGetDescriptor >	req_descr;
		SendTo( _window, req_descr );

		uint	vk_version = 0;

		// choose version
		switch ( _settings.version )
		{
			case "VK 1.0"_GAPI :
			case "vulkan 1.0"_GAPI :	vk_version = VK_API_VERSION_1_0; break;
			case GAPI::type(0) :		vk_version = VK_API_VERSION_1_0; break;
			default :					RETURN_ERR( "unsupported Vulkan version" );
		}

		// create instance
		{
			const bool				is_debug		= _settings.flags[EContextFlags::DebugContext];
			Array<const char*>		extensions		= {};
			Array<const char*>		layers			= {	"VK_LAYER_NV_optimus" };
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
				CHECK( _device.CreateDebugCallback( (VkDebugReportFlagBitsEXT) debug_flags ) );
		}

		// Choose physical device
		{
			Array<Device::DeviceInfo>	dev_info;
			CHECK_ERR( _device.GetPhysicalDeviceInfo( OUT dev_info ) );

			VkPhysicalDevice	match_name_device	= 0;
			VkPhysicalDevice	high_perf_device	= 0;
			float				max_performance		= 0.0f;

			FOR( i, dev_info )
			{
				const auto&		info	= dev_info[i];
																								// magic function:
				float			perf	= float(info.globalMemory.Mb()) / 1024.0f +						// 
										  float(info.isGPU and not info.integratedGPU ? 4 : 1) +		// 
										  float(info.maxInvocations) / 1024.0f +						// 
										  float(info.supportsTesselation + info.supportsGeometryShader);

				if ( perf > max_performance ) {
					max_performance		= perf;
					high_perf_device	= info.id;
				}

				if ( not _settings.device.Empty() and info.device.HasSubStringIC( _settings.device ) )
					match_name_device = info.id;
			}

			CHECK_ERR( _device.CreatePhysicalDevice( match_name_device ? match_name_device : high_perf_device ) );
		}

		// create surface
		{
			using namespace Engine::PlatformTools;

			VkSurfaceKHR	surface	= VK_NULL_HANDLE;

			CHECK_ERR( WindowHelper::GetWindowHandle( _window,
							LAMBDA( this, &surface ) (const WindowHelper::WinAPIWindow &data)
							{
								return _surface.Create( _device.GetInstance(), data.window, OUT surface );
							}) );

			CHECK_ERR( _device.SetSurface( surface, _settings.colorFmt ) );
		}

		// create device
		{
			using EQueueFamily = Vk1Device::EQueueFamily;

			VkPhysicalDeviceFeatures	device_features	= {};
			vkGetPhysicalDeviceFeatures( _device.GetPhyiscalDevice(), &device_features );

			CHECK_ERR( _device.CreateDevice( device_features, EQueueFamily::All ) );
		}

		// create swapchain
		{
			using EFlags = CreateInfo::GpuContext::EFlags;

			CHECK_ERR( _device.CreateSwapchain( req_descr->result->surfaceSize, _settings.flags[ EFlags::VSync ] ) );
			CHECK_ERR( _device.CreateQueue() );
		}

		_device.WritePhysicalDeviceInfo();
		
		CHECK_ERR( GlobalSystems()->modulesFactory->Create(
								VkMemoryManagerModuleID,
								GlobalSystems(),
								CreateInfo::GpuMemoryManager{ this, BytesUL(0), ~BytesUL(0), EGpuMemory::bits().SetAll(), EMemoryAccess::bits().SetAll() },
								OUT _memManager ) );

		_SendEvent( Message< GpuMsg::DeviceCreated >{} );
		return true;
	}
	
/*
=================================================
	_WindowBeforeDestroy
=================================================
*/
	bool VulkanThread::_WindowBeforeDestroy (const Message< OSMsg::WindowBeforeDestroy > &)
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

			_SendEvent( Message< GpuMsg::DeviceBeforeDestroy >{} );
		}

		if ( _memManager )
		{
			_memManager->Send< ModuleMsg::Delete >({});
			_memManager = null;
		}

		if ( _syncManager )
		{
			_syncManager->Send< ModuleMsg::Delete >({});
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
	_WindowVisibilityChanged
=================================================
*/
	bool VulkanThread::_WindowVisibilityChanged (const Message< OSMsg::WindowVisibilityChanged > &msg)
	{
		_isWindowVisible = (msg->state != WindowDesc::EVisibility::Invisible);
		return true;
	}

/*
=================================================
	_WindowDescriptorChanged
=================================================
*/
	bool VulkanThread::_WindowDescriptorChanged (const Message< OSMsg::WindowDescriptorChanged > &msg)
	{
		if ( _device.IsDeviceCreated()									and
			 msg->desc.visibility != WindowDesc::EVisibility::Invisible	and
			 Any( msg->desc.surfaceSize != _device.GetSurfaceSize() ) )
		{
			_device.DeviceWaitIdle();

			// TODO:
			// destroy command buffers with old framebuffers and images

			CHECK_ERR( _device.RecreateSwapchain( msg->desc.surfaceSize ) );
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
	bool VulkanThread::_GetDeviceInfo (const Message< GpuMsg::GetDeviceInfo > &msg)
	{
		msg->result.Set({
			this,
			_memManager,
			_syncManager,
			_device.GetDefaultRenderPass(),
			_device.GetSwapchainLength()
		});
		return true;
	}
	
/*
=================================================
	_GetVkDeviceInfo
=================================================
*/
	bool VulkanThread::_GetVkDeviceInfo (const Message< GpuMsg::GetVkDeviceInfo > &msg)
	{
		msg->result.Set({
			_device.GetInstance(),
			_device.GetPhyiscalDevice(),
			_device.GetLogicalDevice(),
			_device.GetQueue(),	_device.GetQueueIndex(),
			_device.GetQueue(),	_device.GetQueueIndex(),
			_device.GetQueue(),	_device.GetQueueIndex(),
		});
		return true;
	}

/*
=================================================
	_GetVkPrivateClasses
=================================================
*/
	bool VulkanThread::_GetVkPrivateClasses (const Message< GpuMsg::GetVkPrivateClasses > &msg)
	{
		msg->result.Set({
			&_device,
			&_samplerCache,
			&_pipelineCache,
			&_layoutCache,
			&_renderPassCache,
			_memManager.RawPtr()
		});
		return true;
	}
	
/*
=================================================
	_GetGraphicsSettings
=================================================
*/
	bool VulkanThread::_GetGraphicsSettings (const Message< GpuMsg::GetGraphicsSettings > &msg)
	{
		msg->result.Set({ _settings, _device.GetSurfaceSize() });
		return true;
	}

/*
=================================================
	_GetComputeSettings
=================================================
*/
	bool VulkanThread::_GetComputeSettings (const Message< GpuMsg::GetComputeSettings > &msg)
	{
		ComputeSettings	cs;
		cs.device	= _settings.device;
		cs.isDebug	= _settings.flags[ GraphicsSettings::EFlags::DebugContext ];
		cs.version	= _settings.version;

		msg->result.Set( RVREF(cs) );
		return true;
	}
//-----------------------------------------------------------------------------


/*
=================================================
	CreateVulkanThread
=================================================
*/	
	ModulePtr VulkanObjectsConstructor::CreateVulkanThread (GlobalSystemsRef gs, const CreateInfo::GpuThread &ci)
	{
		return New< VulkanThread >( gs, ci );
	}

}	// Platforms
}	// Engine

#endif	// GRAPHICS_API_VULKAN
