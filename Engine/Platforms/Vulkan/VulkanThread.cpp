// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Engine/Platforms/Shared/GPU/Thread.h"
#include "Engine/Platforms/Shared/GPU/CommandBuffer.h"
#include "Engine/Platforms/Windows/WinMessages.h"
#include "Engine/Platforms/Vulkan/VulkanContext.h"
#include "Engine/Platforms/Vulkan/Windows/VkWinSurface.h"
#include "Engine/Platforms/Vulkan/Impl/Vk1Device.h"
#include "Engine/Platforms/Vulkan/Impl/Vk1PipelineCache.h"
#include "Engine/Platforms/Vulkan/Impl/Vk1PipelineLayout.h"
#include "Engine/Platforms/Vulkan/Impl/Vk1RenderPass.h"
#include "Engine/Platforms/Vulkan/Impl/Vk1Sampler.h"
#include "Engine/Platforms/Vulkan/Impl/Vk1Fence.h"

#if defined( GRAPHICS_API_VULKAN )


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
											GpuMsg::GetGraphicsModules,
											OSMsg::WindowCreated,
											OSMsg::WindowBeforeDestroy,
											GpuMsg::GetGraphicsModules,
											GpuMsg::ThreadBeginFrame,
											GpuMsg::ThreadEndFrame,
											GpuMsg::SubmitGraphicsQueueCommands,
											GpuMsg::SubmitComputeQueueCommands,
											GpuMsg::GetDeviceInfo,
											GpuMsg::GetVkDeviceInfo,
											GpuMsg::GetVkPrivateClasses
										> >;
		using SupportedEvents_t		= Module::SupportedEvents_t::Append< MessageListFrom<
											GpuMsg::DeviceCreated,
											GpuMsg::DeviceBeforeDestroy
											// TODO: device lost event
										> >;
		
		using WindowMsgList_t		= MessageListFrom< OSMsg::GetWinWindowHandle >;
		using WindowEventList_t		= MessageListFrom< OSMsg::WindowCreated, OSMsg::WindowBeforeDestroy, OSMsg::OnWinWindowRawMessage >;

		using Surface				= PlatformVK::Vk1Surface;
		using Device				= PlatformVK::Vk1Device;
		using SamplerCache			= PlatformVK::Vk1SamplerCache;
		using PipelineCache			= PlatformVK::Vk1PipelineCache;
		using LayoutCache			= PlatformVK::Vk1PipelineLayoutCache;
		using RenderPassCache		= PlatformVK::Vk1RenderPassCache;
		using CommandBuffers_t		= Array< vk::VkCommandBuffer >;

		//
		// Command Buffers Life Control
		//
		struct CmdBufferLifeControl
		{
		// types
			struct PerFrame
			{
				Array< ModulePtr >			cmdBuffers;
				PlatformVK::Vk1FencePtr		fence;
				uint						imageIndex = 0;		// index of image in swapchain
			};
			using PerFrameData_t		= StaticArray< PerFrame, 8 >;
			using FrameIndex_t			= Limit< int, LimitStrategy::Wrap >;

		// variables
			PerFrameData_t		perFrameData;
			FrameIndex_t		frameIndex		{ 0, 0, 3 };

		// methods
			explicit CmdBufferLifeControl (Ptr<Device> dev);
			~CmdBufferLifeControl ();

			bool SubmitQueue (Ptr<Device> dev, ArrayCRef<ModulePtr> cmdBuffers, ArrayCRef<vk::VkCommandBuffer> cmdIDs);
			void FreeBuffers (int len);

			bool Resize (uint size);
			void FreeAll ();

			void Destroy ();
		};


	// constants
	private:
		static const TypeIdList		_msgTypes;
		static const TypeIdList		_eventTypes;

		
	// variables
	private:
		GraphicsSettings		_settings;

		ModulePtr				_window;

		Surface					_surface;
		Device					_device;

		SamplerCache			_samplerCache;
		PipelineCache			_pipelineCache;
		LayoutCache				_layoutCache;
		RenderPassCache			_renderPassCache;
		
		CommandBuffers_t		_tempCmdBuffers;
		CmdBufferLifeControl	_cmdControl;

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

		bool _WindowCreated (const Message< OSMsg::WindowCreated > &);
		bool _WindowBeforeDestroy (const Message< OSMsg::WindowBeforeDestroy > &);
		bool _WindowVisibilityChanged (const Message< OSMsg::WindowVisibilityChanged > &);
		bool _WindowDescriptorChanged (const Message< OSMsg::WindowDescriptorChanged > &);
		
		bool _GetDeviceInfo (const Message< GpuMsg::GetDeviceInfo > &);
		bool _GetVkDeviceInfo (const Message< GpuMsg::GetVkDeviceInfo > &);
		bool _GetVkPrivateClasses (const Message< GpuMsg::GetVkPrivateClasses > &);

	private:
		bool _CreateDevice ();
		void _DetachWindow ();

		ArrayCRef<vk::VkCommandBuffer> _ExtractCmdBufferIDs (ArrayCRef<ModulePtr> modules);
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
		_cmdControl( &_device ),		_isWindowVisible( false )
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
		
		CHECK( _ValidateMsgSubscriptions() );
		
		CHECK( ci.shared.IsNull() );	// sharing is not supported yet

		_AttachSelfToManager( ci.context, VkContextModuleID, false );

		_tempCmdBuffers.Reserve( 16 );
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
		
		// TODO: use SearchModule message
		CHECK_ATTACHMENT(( _window = GlobalSystems()->Get< ParallelThread >()->GetModuleByMsgEvent< WindowMsgList_t, WindowEventList_t >() ));

		_window->Subscribe( this, &VulkanThread::_WindowCreated );
		_window->Subscribe( this, &VulkanThread::_WindowBeforeDestroy );
		_window->Subscribe( this, &VulkanThread::_WindowVisibilityChanged );
		_window->Subscribe( this, &VulkanThread::_WindowDescriptorChanged );
		
		CHECK_ERR( Module::_Link_Impl( msg ) );

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
		msg->compute.Set( VulkanContext::GetComputeModules() );
		msg->graphics.Set( VulkanContext::GetGraphicsModules() );
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
			CHECK_ERR( _cmdControl.SubmitQueue( &_device, msg->commands, _ExtractCmdBufferIDs( msg->commands ) ) );

		CHECK_ERR( _device.EndFrame() );
		return true;
	}
	
/*
=================================================
	_ExtractCmdBufferIDs
=================================================
*/
	ArrayCRef<vk::VkCommandBuffer>  VulkanThread::_ExtractCmdBufferIDs (ArrayCRef<ModulePtr> cmdBuffers)
	{
		_tempCmdBuffers.Clear();
		_tempCmdBuffers.Reserve( cmdBuffers.Count() );

		FOR( i, cmdBuffers )
		{
			Message< GpuMsg::GetVkCommandBufferID >		req_id;
			cmdBuffers[i]->Send( req_id );

			vk::VkCommandBuffer		cmd = req_id->result.Get( VK_NULL_HANDLE );

			if ( cmd != VK_NULL_HANDLE )
				_tempCmdBuffers.PushBack( cmd );
		}

		return _tempCmdBuffers;
	}

/*
=================================================
	_SubmitGraphicsQueueCommands
=================================================
*/
	bool VulkanThread::_SubmitGraphicsQueueCommands (const Message< GpuMsg::SubmitGraphicsQueueCommands > &msg)
	{
		using namespace vk;
		
		CHECK_ERR( _device.IsDeviceCreated() );
		CHECK_ERR( _device.GetQueueFamily()[ Device::EQueueFamily::Graphics ] );
		
		PlatformVK::Vk1Fence	fence		{ &_device };
		const auto				cmd_ids		= _ExtractCmdBufferIDs( msg->commands );
		VkSubmitInfo			submit_info = {};

		if ( msg->sync )
			fence.Create();

		submit_info.sType				= VK_STRUCTURE_TYPE_SUBMIT_INFO;
		submit_info.commandBufferCount	= (uint) cmd_ids.Count();
		submit_info.pCommandBuffers		= cmd_ids.RawPtr();

		VK_CHECK( vkQueueSubmit( _device.GetQueue(), 1, &submit_info, fence.GetFenceID() ) );

		Message< GpuMsg::SetCommandBufferState >	pending_state{ GpuMsg::SetCommandBufferState::EState::Pending };
		Message< GpuMsg::SetCommandBufferState >	completed_state{ GpuMsg::SetCommandBufferState::EState::Completed };

		FOR( i, msg->commands ) {
			msg->commands[i]->Send( pending_state );
		}
		
		if ( msg->sync )
			fence.Wait();

		FOR( i, msg->commands ) {
			msg->commands[i]->Send( completed_state );
		}
		return true;
	}
	
/*
=================================================
	_SubmitComputeQueueCommands
=================================================
*/
	bool VulkanThread::_SubmitComputeQueueCommands (const Message< GpuMsg::SubmitComputeQueueCommands > &msg)
	{
		using namespace vk;

		CHECK_ERR( _device.IsDeviceCreated() );
		CHECK_ERR( _device.GetQueueFamily()[ Device::EQueueFamily::Compute ] );
		
		PlatformVK::Vk1Fence	fence		{ &_device };
		const auto				cmd_ids		= _ExtractCmdBufferIDs( msg->commands );
		VkSubmitInfo			submit_info = {};

		if ( msg->sync )
			fence.Create();
		
		submit_info.sType				= VK_STRUCTURE_TYPE_SUBMIT_INFO;
		submit_info.commandBufferCount	= (uint) cmd_ids.Count();
		submit_info.pCommandBuffers		= cmd_ids.RawPtr();

		VK_CHECK( vkQueueSubmit( _device.GetQueue(), 1, &submit_info, fence.GetFenceID() ) );

		Message< GpuMsg::SetCommandBufferState >	pending_state{ GpuMsg::SetCommandBufferState::EState::Pending };
		Message< GpuMsg::SetCommandBufferState >	completed_state{ GpuMsg::SetCommandBufferState::EState::Completed };

		FOR( i, msg->commands ) {
			msg->commands[i]->Send( pending_state );
		}
		
		if ( msg->sync )
			fence.Wait();

		FOR( i, msg->commands ) {
			msg->commands[i]->Send( completed_state );
		}
		return true;
	}

/*
=================================================
	_WindowCreated
=================================================
*/
	bool VulkanThread::_WindowCreated (const Message< OSMsg::WindowCreated > &)
	{
		if ( _CreateDevice() )
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
	bool VulkanThread::_CreateDevice ()
	{
		using namespace vk;
		using namespace PlatformVK;
		using namespace CreateInfo;
		
		using EContextFlags = CreateInfo::GpuContext::EFlags;

		Message< OSMsg::WindowGetDescriptor >	req_descr;
		Message< OSMsg::GetWinWindowHandle >	req_hwnd;
		SendTo( _window, req_hwnd );
		SendTo( _window, req_descr );

		uint	vk_version = 0;

		// choose version
		switch ( _settings.version )
		{
			case "VK 1.0"_GAPI :
			case "vulkan 1.0"_GAPI :	vk_version = VK_API_VERSION_1_0; break;
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

			vk::VkPhysicalDevice	match_name_device	= 0;
			vk::VkPhysicalDevice	high_perf_device	= 0;
			float					max_performance		= 0.0f;

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
			VkSurfaceKHR	surface	= VK_NULL_HANDLE;

			_surface.Create( _device.GetInstance(), *req_hwnd->result, OUT surface );
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

		_DetachWindow();
		
		CHECK( _SetState( EState::Initial ) );
		return true;
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

			_cmdControl.FreeAll();

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
			&_renderPassCache
		});
		return true;
	}
//-----------------------------------------------------------------------------


	
/*
=================================================
	constructor
=================================================
*/
	VulkanThread::CmdBufferLifeControl::CmdBufferLifeControl (Ptr<Device> dev)
	{
		FOR( i, perFrameData )
		{
			perFrameData[i].fence = New< PlatformVK::Vk1Fence >( dev );
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
	bool VulkanThread::CmdBufferLifeControl::SubmitQueue (Ptr<Device> device, ArrayCRef<ModulePtr> cmdBuffers, ArrayCRef<vk::VkCommandBuffer> cmdIDs)
	{
		using namespace vk;

		++frameIndex;
		FreeBuffers( device->GetSwapchainLength() );	// TODO: call on frame begin

		auto&	frame_data	= perFrameData[ frameIndex ];

		CHECK_ERR( not cmdIDs.Empty() );
		
		// submit
		if ( frame_data.fence->IsCreated() ) {
			CHECK_ERR( frame_data.fence->Reset() );
		} else {
			CHECK_ERR( frame_data.fence->Create( false ) );
		}
		CHECK_ERR( device->SubmitQueue( cmdIDs, frame_data.fence->GetFenceID() ) );

		// change state
		FOR( i, cmdBuffers )
		{
			Message< GpuMsg::SetCommandBufferState >	pending_state{ GpuMsg::SetCommandBufferState::EState::Pending };
			cmdBuffers[i]->Send( pending_state );
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
	void VulkanThread::CmdBufferLifeControl::FreeBuffers (int length)
	{
		Message< GpuMsg::SetCommandBufferState >	completed_state{ GpuMsg::SetCommandBufferState::EState::Completed };

		FrameIndex_t	begin_index = frameIndex;
		FrameIndex_t	end_index	= frameIndex - length;

		ASSERT( begin_index != end_index );

		for (FrameIndex_t i = begin_index; i != end_index; ++i)
		{
			auto&	frame_data	= perFrameData[i];

			// sync
			if ( frame_data.fence->IsCreated() )
				frame_data.fence->Wait();
			
			// change state
			FOR( j, frame_data.cmdBuffers ) {
				frame_data.cmdBuffers[j]->Send( completed_state );
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
		perFrameData.Clear();
	}
	
/*
=================================================
	_CreateVulkanThread
=================================================
*/	
	ModulePtr VulkanContext::_CreateVulkanThread (GlobalSystemsRef gs, const CreateInfo::GpuThread &ci)
	{
		return New< VulkanThread >( gs, ci );
	}

}	// Platforms
}	// Engine

#endif	// GRAPHICS_API_VULKAN
