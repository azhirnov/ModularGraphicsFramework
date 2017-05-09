// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Vk1Device.h"
#include "vulkan1_utils.h"

#if defined( GRAPHICS_API_VULKAN )

#include "Engine/Platforms/Vulkan/Impl/Vk1RenderPass.h"
#include "Engine/Platforms/Vulkan/Impl/Vk1CommandBuffer.h"
#include "Engine/Platforms/Vulkan/Impl/Vk1CommandBufferBuilder.h"
#include "Engine/Platforms/Vulkan/Impl/Vk1SystemFramebuffer.h"

using namespace vk;

namespace Engine
{
namespace PlatformVK
{

/*
=================================================
	constructor
=================================================
*/
	Vk1Device::Vk1Device (const GlobalSystemsRef gs, const VkSystemsRef vkSys) :
		BaseObject( gs ),
		_vkSystems( vkSys ),
		_instance( VK_NULL_HANDLE ),
		_physicalDevice( VK_NULL_HANDLE ),
		_logicalDevice( VK_NULL_HANDLE ),
		_surface( VK_NULL_HANDLE ),
		_swapchain( VK_NULL_HANDLE ),
		_vsync( false ),
		_debugCallback( VK_NULL_HANDLE ),
		_colorPixelFormat( EPixelFormat::Unknown ),
		_depthStencilPixelFormat( EPixelFormat::Unknown ),
		_colorFormat( VK_FORMAT_UNDEFINED ),
		_colorSpace( VK_COLOR_SPACE_MAX_ENUM_KHR ),
		_imageAvailable( VK_NULL_HANDLE ),
		_renderFinished( VK_NULL_HANDLE ),
		_enableDebugMarkers( false ),
		_depthStencilImage( VK_NULL_HANDLE ),
		_depthStencilMemory( VK_NULL_HANDLE ),
		_depthStencilView( VK_NULL_HANDLE ),
		_depthStencilFormat( VK_FORMAT_UNDEFINED ),
		_renderPass( VK_NULL_HANDLE ),
		_currentImageIndex( -1 ),
		_graphicsQueueSubmited( false )
	{
		SetDebugName( "Vk1Device" );

		_imageBuffers.Reserve( 8 );

		VkSystems()->GetSetter< Vk1Device >().Set( this );

		CHECK( GlobalSystems()->Get< ModulesFactory >()->Register(
			Vk1SystemFramebuffer::GetStaticID(),
			&Vk1SystemFramebuffer::CreateModule
		) );
	}
	
/*
=================================================
	destructor
=================================================
*/
	Vk1Device::~Vk1Device ()
	{
		GlobalSystems()->Get< ModulesFactory >()->UnregisterAll<Vk1SystemFramebuffer>();

		CHECK( not IsInstanceCreated() );
		CHECK( not HasPhyiscalDevice() );
		CHECK( not IsDeviceCreated() );
		CHECK( not IsSurfaceCreated() );
		CHECK( not IsSwapchainCreated() );
		CHECK( not IsDebugCallbackCreated() );

		VkSystems()->GetSetter< Vk1Device >().Set( null );
	}

/*
=================================================
	CreateInstance
=================================================
*/
	bool Vk1Device::CreateInstance (StringCRef applicationName, vk::uint32_t applicationVersion,
									 vk::uint32_t vulkanVersion, ExtensionNames_t ext, ValidationLayers_t layers)
	{
		CHECK_ERR( not IsInstanceCreated() );

		VkApplicationInfo		app_info				= {};
		VkInstanceCreateInfo	instance_create_info	= {};
		Array< const char* >	instance_extensions		= { VK_KHR_SURFACE_EXTENSION_NAME };

		instance_extensions << ext;

		CHECK_ERR( _CheckLayers( layers ) );


		app_info.sType				= VK_STRUCTURE_TYPE_APPLICATION_INFO;
		app_info.apiVersion			= vulkanVersion;

		app_info.pApplicationName	= applicationName.cstr();
		app_info.applicationVersion	= applicationVersion;

		app_info.pEngineName		= Engine::ENGINE_NAME;
		app_info.engineVersion		= Engine::ENGINE_VERSION;

		
		instance_create_info.sType						= VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		instance_create_info.pApplicationInfo			= &app_info;

		instance_create_info.enabledExtensionCount		= (vk::uint32_t) instance_extensions.Count();
		instance_create_info.ppEnabledExtensionNames	= instance_extensions.ptr();
		
		instance_create_info.enabledLayerCount			= (vk::uint32_t) layers.Count();
		instance_create_info.ppEnabledLayerNames		= layers.ptr();


		VK_CHECK( vkCreateInstance( &instance_create_info, null, OUT &_instance ) );
		return true;
	}
	
/*
=================================================
	DestroyInstance
=================================================
*/
	bool Vk1Device::DestroyInstance ()
	{
		if ( _instance == VK_NULL_HANDLE )
			return true;

		CHECK_ERR( not IsDeviceCreated() );
		CHECK_ERR( not IsSurfaceCreated() );
		CHECK_ERR( not IsSwapchainCreated() );
		CHECK_ERR( not IsDebugCallbackCreated() );

		vkDestroyInstance( _instance, null );

		_physicalDevice	= VK_NULL_HANDLE;
		_instance		= VK_NULL_HANDLE;

		return true;
	}
	
/*
=================================================
	CreateDebugCallback
=================================================
*/
	bool Vk1Device::CreateDebugCallback (VkDebugReportFlagBitsEXT flags)
	{
		CHECK_ERR( IsInstanceCreated() );
		CHECK_ERR( not IsDebugCallbackCreated() );

		VkDebugReportCallbackCreateInfoEXT	dbg_callback_info = {};

		dbg_callback_info.sType			= VK_STRUCTURE_TYPE_DEBUG_REPORT_CALLBACK_CREATE_INFO_EXT;
		dbg_callback_info.flags			= flags;
		dbg_callback_info.pfnCallback	= &_DebugReportCallback;

		VK_CHECK( vkCreateDebugReportCallbackEXT( _instance, &dbg_callback_info, null, OUT &_debugCallback ) );
		return true;
	}
	
/*
=================================================
	DestroyDebugCallback
=================================================
*/
	bool Vk1Device::DestroyDebugCallback ()
	{
		if ( _debugCallback == VK_NULL_HANDLE )
			return true;

		CHECK_ERR( IsInstanceCreated() );

		vkDestroyDebugReportCallbackEXT( _instance, _debugCallback, null );

		_debugCallback = VK_NULL_HANDLE;
		return true;
	}

/*
=================================================
	ChoosePhysicalDevice
=================================================
*/
	bool Vk1Device::ChoosePhysicalDevice (StringCRef deviceName)
	{
		CHECK_ERR( IsInstanceCreated() );
		//CHECK_ERR( not HasPhyiscalDevice() );

		vk::uint32_t				count				= 0;

		usize						name_math_idx		= -1;
		usize						high_version_idx	= -1;
		usize						best_gpu_idx		= -1;

		vk::uint32_t				version				= 0;
		VkPhysicalDeviceType		gpu_type			= VK_PHYSICAL_DEVICE_TYPE_OTHER;
		Array< VkPhysicalDevice >	devices;

		VK_CALL( vkEnumeratePhysicalDevices( _instance, &count, null ) );
		devices.Resize( count );

		VK_CALL( vkEnumeratePhysicalDevices( _instance, &count, devices.ptr() ) );

		FOR( i, devices )
		{
			VkPhysicalDeviceProperties	prop;

			vkGetPhysicalDeviceProperties( devices[i], &prop );

			// select higher version
			if ( prop.apiVersion > version )
			{
				version			 = prop.apiVersion;
				high_version_idx = i;
			}

			// select best GPU device
			if ( (prop.deviceType == VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU or
				  prop.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) and
				 prop.deviceType > gpu_type )
			{
				gpu_type	 = prop.deviceType;
				best_gpu_idx = i;
			}

			if ( deviceName.Empty() )
				continue;

			if ( name_math_idx == -1 and
				 StringCRef( prop.deviceName ).HasSubStringIC( deviceName ) )
			{
				name_math_idx = i;
			}
		}

		if ( name_math_idx != -1 )
			_physicalDevice = devices[name_math_idx];
		else
		if ( best_gpu_idx != -1 )
			_physicalDevice = devices[best_gpu_idx];
		else
		if ( high_version_idx != -1 )
			_physicalDevice = devices[high_version_idx];
		else
			_physicalDevice = devices[0];


		vkGetPhysicalDeviceProperties( _physicalDevice, &_deviceProperties );
		vkGetPhysicalDeviceFeatures( _physicalDevice, &_deviceFeatures );
		vkGetPhysicalDeviceMemoryProperties( _physicalDevice, &_deviceMemoryProperties );

		return true;
	}
	
/*
=================================================
	WritePhysicalDeviceInfo
=================================================
*/
	bool Vk1Device::WritePhysicalDeviceInfo () const
	{
		CHECK_ERR( HasPhyiscalDevice() );

		TODO( "" );
		return false;
	}
	
/*
=================================================
	CreateDevice
=================================================
*/
	bool Vk1Device::CreateDevice (const VkPhysicalDeviceFeatures &enabledFeatures,
								   VkQueueFlags requestedQueueTypes,
								   bool useSwapchain,
								   ExtensionNames_t enabledExtensions)
	{
		CHECK_ERR( HasPhyiscalDevice() );
		CHECK_ERR( useSwapchain == IsSurfaceCreated() );
		CHECK_ERR( not IsDeviceCreated() );

		Array< String >						supported_extensions;
		Array< VkDeviceQueueCreateInfo >	queue_infos;
		Array< const char * >				device_extensions	= enabledExtensions;
		VkDeviceCreateInfo					device_info			= {};

		CHECK_ERR( _GetDeviceExtensions( OUT supported_extensions ) );
		CHECK_ERR( _GetQueueCreateInfos( OUT queue_infos, OUT _queueFamilyIndices,
										 requestedQueueTypes, useSwapchain ) );

		if ( useSwapchain )
		{
			CHECK_ERR( supported_extensions.IsExist( VK_KHR_SWAPCHAIN_EXTENSION_NAME ) );

			device_extensions << VK_KHR_SWAPCHAIN_EXTENSION_NAME;
		}

		if ( supported_extensions.IsExist( VK_EXT_DEBUG_MARKER_EXTENSION_NAME ) )
		{
			device_extensions.PushBack( VK_EXT_DEBUG_MARKER_EXTENSION_NAME );
			_enableDebugMarkers = true;
		}
		
		device_info.sType					= VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
		device_info.queueCreateInfoCount	= (vk::uint32_t) queue_infos.Count();
		device_info.pQueueCreateInfos		= queue_infos.ptr();
		device_info.pEnabledFeatures		= &enabledFeatures;
		
		if ( not device_extensions.Empty() )
		{
			device_info.enabledExtensionCount	= (vk::uint32_t) device_extensions.Count();
			device_info.ppEnabledExtensionNames	= device_extensions.ptr();
		}

		VK_CHECK( vkCreateDevice( _physicalDevice, &device_info, null, OUT &_logicalDevice ) );

		if ( &_deviceFeatures != &enabledFeatures )
			_deviceFeatures = enabledFeatures;

		return true;
	}
	
/*
=================================================
	DestroyDevice
=================================================
*/
	bool Vk1Device::DestroyDevice ()
	{
		if ( _logicalDevice == VK_NULL_HANDLE )
			return true;

		CHECK_ERR( IsInstanceCreated() );
		CHECK_ERR( not IsSwapchainCreated() );
		
		vkDestroyDevice( _logicalDevice, null );

		_logicalDevice = VK_NULL_HANDLE;
		return true;
	}
	
/*
=================================================
	DeviceWaitIdle
=================================================
*/
	void Vk1Device::DeviceWaitIdle ()
	{
		ASSERT( IsDeviceCreated() );

		VK_CALL( vkDeviceWaitIdle( _logicalDevice ) );
	}

/*
=================================================
	CreateSwapchain
=================================================
*/
	bool Vk1Device::CreateSwapchain (const uint2 &size, bool vsync, vk::uint32_t imageArrayLayers, EPixelFormat::type depthStencilFormat)
	{
		CHECK_ERR( HasPhyiscalDevice() );
		CHECK_ERR( IsDeviceCreated() );
		CHECK_ERR( IsSurfaceCreated() );
		//CHECK_ERR( not IsSwapchainCreated() );

		VkSurfaceCapabilitiesKHR	surf_caps;
		VK_CHECK( vkGetPhysicalDeviceSurfaceCapabilitiesKHR( _physicalDevice, _surface, &surf_caps ) );

		VkSwapchainKHR				old_swapchain	= _swapchain;
		VkSwapchainCreateInfoKHR	swapchain_info	= {};
		
		swapchain_info.sType					= VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
		swapchain_info.pNext					= null;
		swapchain_info.surface					= _surface;
		swapchain_info.imageFormat				= _colorFormat;
		swapchain_info.imageColorSpace			= _colorSpace;
		swapchain_info.imageExtent				= { size.x, size.y };
		swapchain_info.imageArrayLayers			= imageArrayLayers;
		swapchain_info.queueFamilyIndexCount	= 0;
		swapchain_info.pQueueFamilyIndices		= null;
		swapchain_info.oldSwapchain				= old_swapchain;
		swapchain_info.clipped					= VK_TRUE;
		swapchain_info.compositeAlpha			= VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;	// TODO
		
		_GetSurfaceImageCount( OUT swapchain_info.minImageCount, surf_caps );
		_GetSurfaceTransform( OUT swapchain_info.preTransform, surf_caps );
		_GetSwapChainExtent( OUT swapchain_info.imageExtent, surf_caps );
		_GetPresentMode( OUT swapchain_info.presentMode, vsync, surf_caps );
		_GetSharingMode( OUT swapchain_info.imageSharingMode );
		_GetImageUsage( OUT swapchain_info.imageUsage );

		VK_CHECK( vkCreateSwapchainKHR( _logicalDevice, &swapchain_info, null, OUT &_swapchain ) );

		_surfaceSize.x	= swapchain_info.imageExtent.width;
		_surfaceSize.y	= swapchain_info.imageExtent.height;
		_vsync			= vsync;

		// destroy obsolete resources
		_DeleteSwapchain( old_swapchain, _imageBuffers );
		_DeleteFramebuffers( _framebuffers );
		_DeleteCommandBuffers();

		// create dependent resources
		CHECK_ERR( _CreateColorAttachment( OUT _imageBuffers ) );
		CHECK_ERR( _CreateDepthStencilAttachment( depthStencilFormat ) );
		CHECK_ERR( _CreateRenderPass() );
		CHECK_ERR( _CreateFramebuffers( OUT _framebuffers ) );
		CHECK_ERR( _CreateSemaphores() );
		CHECK_ERR( _CreateCommandBuffers() );

		return true;
	}
	
/*
=================================================
	RecreateSwapchain
=================================================
*/
	bool Vk1Device::RecreateSwapchain (const uint2 &size)
	{
		CHECK_ERR( IsSwapchainCreated() );
		VK_CALL( vkDeviceWaitIdle( _logicalDevice ) );

		CHECK_ERR( CreateSwapchain( size, _vsync ) );	// TODO imageArrayLayers
		VK_CALL( vkDeviceWaitIdle( _logicalDevice ) );
		return true;
	}

	bool Vk1Device::RecreateSwapchain ()
	{
		return RecreateSwapchain( _surfaceSize );
	}

/*
=================================================
	DestroySwapchain
=================================================
*/
	bool Vk1Device::DestroySwapchain ()
	{
		if ( _swapchain == VK_NULL_HANDLE )
			return true;

		CHECK_ERR( IsDeviceCreated() );

		_DeleteSwapchain( _swapchain, _imageBuffers );
		_framebuffers.Clear();
		
		_renderPass = null;

		_DeleteDepthStencilAttachment();
		_DeleteCommandBuffers();
		_DestroySemaphores();

		_surfaceSize = uint2();
		return true;
	}
	
/*
=================================================
	SetSurface
=================================================
*/
	bool Vk1Device::SetSurface (vk::VkSurfaceKHR surface, EPixelFormat::type colorFmt)
	{
		CHECK_ERR( surface != VK_NULL_HANDLE );
		CHECK_ERR( HasPhyiscalDevice() );
		CHECK_ERR( not IsSurfaceCreated() );

		const VkFormat			required_format			= Vk1Enum( colorFmt );
		const VkColorSpaceKHR	required_color_space	= VK_COLOR_SPACE_SRGB_NONLINEAR_KHR;	// EPixelFormat::IsNonLinear( _settings.format );

		_surface = surface;

		CHECK_ERR( _ChooseColorFormat( OUT _colorFormat, OUT _colorSpace, required_format, required_color_space ) );

		_colorPixelFormat = Vk1Enum( _colorFormat );
		return true;
	}
	
/*
=================================================
	DestroySurface
=================================================
*/
	bool Vk1Device::DestroySurface ()
	{
		if ( _surface == VK_NULL_HANDLE )
			return true;

		CHECK_ERR( IsInstanceCreated() );
		CHECK_ERR( not IsSwapchainCreated() );

		vkDestroySurfaceKHR( _instance, _surface, null );

		_surface			= VK_NULL_HANDLE;
		_colorFormat		= VK_FORMAT_UNDEFINED;
		_colorSpace			= VK_COLOR_SPACE_MAX_ENUM_KHR;
		_colorPixelFormat	= EPixelFormat::Unknown;
		return true;
	}
	
/*
=================================================
	GetGraphicsQueue
=================================================
*/
	VkQueue Vk1Device::GetGraphicsQueue () const
	{
		CHECK( _queueFamilyIndices.graphics != -1 );

		VkQueue	queue;
		vkGetDeviceQueue( _logicalDevice, _queueFamilyIndices.graphics, 0, &queue );
		return queue;
	}
	
/*
=================================================
	GetComputeQueue
=================================================
*/
	VkQueue Vk1Device::GetComputeQueue () const
	{
		CHECK( _queueFamilyIndices.compute != -1 );

		VkQueue	queue;
		vkGetDeviceQueue( _logicalDevice, _queueFamilyIndices.compute, 0, &queue );
		return queue;
	}
	
/*
=================================================
	GetTransferQueue
=================================================
*/
	VkQueue Vk1Device::GetTransferQueue () const
	{
		CHECK( _queueFamilyIndices.transfer != -1 );

		VkQueue	queue;
		vkGetDeviceQueue( _logicalDevice, _queueFamilyIndices.transfer, 0, &queue );
		return queue;
	}
	
/*
=================================================
	GetPresentQueue
=================================================
*/
	VkQueue Vk1Device::GetPresentQueue () const
	{
		CHECK( _queueFamilyIndices.present != -1 );

		VkQueue	queue;
		vkGetDeviceQueue( _logicalDevice, _queueFamilyIndices.present, 0, &queue );
		return queue;
	}
	
/*
=================================================
	BeginFrame
=================================================
*/
	bool Vk1Device::BeginFrame ()
	{
		_graphicsQueueSubmited	= false;
		_currentImageIndex		= -1;

		vk::uint32_t	image_index = -1;
		VkResult		result		= vkAcquireNextImageKHR( _logicalDevice, _swapchain, vk::uint64_t(-1), _imageAvailable,
															 VK_NULL_HANDLE, OUT &image_index );

		if ( result == VK_SUCCESS )
		{}
		else
		if ( result == VK_ERROR_OUT_OF_DATE_KHR or
			 result == VK_SUBOPTIMAL_KHR )
		{
			CHECK_ERR( RecreateSwapchain() );
			return false;
		}
		else
		{
			if ( not Vk1_CheckErrors( result, "vkAcquireNextImageKHR", GX_FUNCTION_NAME, __FILE__, __LINE__ ) )
				return false;
		}

		_currentImageIndex = image_index;
		return true;
	}
	
/*
=================================================
	EndFrame
=================================================
*/
	bool Vk1Device::EndFrame ()
	{
		CHECK_ERR( _currentImageIndex < _framebuffers.Count() );
		
		VkPresentInfoKHR	present_info		= {};
		VkSwapchainKHR		swap_chains[]		= { _swapchain };
		VkSemaphore			signal_semaphores[] = { _renderFinished };

		present_info.sType			= VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
		present_info.swapchainCount	= CountOf( swap_chains );
		present_info.pSwapchains	= swap_chains;
		present_info.pImageIndices	= &_currentImageIndex;

		if ( _graphicsQueueSubmited ) {
			present_info.waitSemaphoreCount	= CountOf( signal_semaphores );
			present_info.pWaitSemaphores	= signal_semaphores;
		}
		else {
			present_info.waitSemaphoreCount	= 0;
			present_info.pWaitSemaphores	= null;
		}

		VK_CHECK( vkQueuePresentKHR( GetGraphicsQueue(), &present_info ) );
		
		_graphicsQueueSubmited	= false;
		_currentImageIndex		= -1;
		return true;
	}
	
/*
=================================================
	EndFrame
=================================================
*
	bool Vk1Device::EndFrame (const ModulePtr &framebuffer)
	{
		// check if framebuffer is current
		CHECK_ERR
		//CHECK_ERR( framebuffer.ToPtr< Vk1SystemFramebuffer >()->GetImageIndex() == _currentImageIndex );

		return EndFrame();
	}

/*
=================================================
	IsFrameStarted
=================================================
*/
	bool Vk1Device::IsFrameStarted () const
	{
		return _currentImageIndex < _framebuffers.Count();
	}

/*
=================================================
	SubmitGraphicsQueue
=================================================
*/
	bool Vk1Device::SubmitGraphicsQueue (ArrayCRef< ModulePtr > cmdBuffers)
	{
		CHECK_ERR( _currentImageIndex < _framebuffers.Count() );

		// get command buffer IDs
		_tempCmdBuffers.Clear();
		_tempCmdBuffers.Reserve( cmdBuffers.Count() );

		FOR( i, cmdBuffers )
		{
			Message< ModuleMsg::GetGpuCommandBufferID >	id_request;
			cmdBuffers[i]->Send( id_request );

			VkCommandBuffer	cmd = id_request->result.Get( VK_NULL_HANDLE );

			if ( cmd != VK_NULL_HANDLE )
				_tempCmdBuffers.PushBack( cmd );
		}

		CHECK_ERR( not _tempCmdBuffers.Empty() );


		// submit command buffers to grpahics queue
		VkSubmitInfo			submit_info			= {};
		VkSemaphore				wait_semaphores[]	= { _imageAvailable };
		VkPipelineStageFlags	wait_stages[]		= { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
		VkSemaphore				signal_semaphores[] = { _renderFinished };

		submit_info.sType					= VK_STRUCTURE_TYPE_SUBMIT_INFO;
		submit_info.waitSemaphoreCount		= CountOf( wait_semaphores );
		submit_info.pWaitSemaphores			= wait_semaphores;
		submit_info.pWaitDstStageMask		= wait_stages;
		submit_info.commandBufferCount		= _tempCmdBuffers.Count();
		submit_info.pCommandBuffers			= _tempCmdBuffers.ptr();
		submit_info.signalSemaphoreCount	= CountOf( signal_semaphores );
		submit_info.pSignalSemaphores		= signal_semaphores;

		VK_CHECK( vkQueueSubmit( GetGraphicsQueue(), 1, &submit_info, VK_NULL_HANDLE ) );

		_graphicsQueueSubmited = true;
		return true;
	}
	
/*
=================================================
	SubmitComputeQueue
=================================================
*/
	bool Vk1Device::SubmitComputeQueue (ArrayCRef< ModulePtr > cmdBuffers)
	{
		return false;
	}

/*
=================================================
	DrawFrame
=================================================
*
	bool Vk1Device::DrawFrame ()
	{
		vk::uint32_t	image_index = -1;
		VkResult		result		= vkAcquireNextImageKHR( _logicalDevice, _swapchain, vk::uint64_t(-1), _imageAvailable,
															 VK_NULL_HANDLE, &image_index );

		if ( result == VK_SUCCESS )
		{}
		else
		if ( result == VK_ERROR_OUT_OF_DATE_KHR or
			 result == VK_SUBOPTIMAL_KHR )
		{
			CHECK_ERR( RecreateSwapchain() );
			return true;
		}
		else
		{
			if ( not Vk1_CheckErrors( result, "vkAcquireNextImageKHR", GX_FUNCTION_NAME, __FILE__, __LINE__ ) )
				return false;
		}

		VkSubmitInfo			submit_info			= {};
		VkSemaphore				wait_semaphores[]	= { _imageAvailable };
		VkPipelineStageFlags	wait_stages[]		= { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
		VkSemaphore				signal_semaphores[] = { _renderFinished };
		VkCommandBuffer			cmd_buffers[]		= { _commandBuffers[ image_index ].ToPtr< Vk1CommandBuffer >()->GetCmdBufferID() };

		submit_info.sType					= VK_STRUCTURE_TYPE_SUBMIT_INFO;
		submit_info.waitSemaphoreCount		= 1;
		submit_info.pWaitSemaphores			= wait_semaphores;
		submit_info.pWaitDstStageMask		= wait_stages;
		submit_info.commandBufferCount		= CountOf( cmd_buffers );
		submit_info.pCommandBuffers			= cmd_buffers;
		submit_info.signalSemaphoreCount	= 1;
		submit_info.pSignalSemaphores		= signal_semaphores;

		VK_CHECK( vkQueueSubmit( GetGraphicsQueue(), 1, &submit_info, VK_NULL_HANDLE ) );
		
		
		VkPresentInfoKHR	present_info	= {};
		VkSwapchainKHR		swap_chains[]	= { _swapchain };

		present_info.sType				= VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
		present_info.waitSemaphoreCount	= 1;
		present_info.pWaitSemaphores	= signal_semaphores;
		present_info.swapchainCount		= 1;
		present_info.pSwapchains		= swap_chains;
		present_info.pImageIndices		= &image_index;

		VK_CHECK( vkQueuePresentKHR( GetGraphicsQueue(), &present_info ) );

		return true;
	}

/*
=================================================
	_GetSwapChainExtent
=================================================
*/
	void Vk1Device::_GetSwapChainExtent (OUT VkExtent2D &extent, const VkSurfaceCapabilitiesKHR &surfaceCaps) const
	{
		if ( surfaceCaps.currentExtent.width  == -1 and
			 surfaceCaps.currentExtent.height == -1 )
		{
			// keep window size
		}
		else
		{
			extent.width  = surfaceCaps.currentExtent.width;
			extent.height = surfaceCaps.currentExtent.height;
		}
	}
	
/*
=================================================
	_GetPresentMode
=================================================
*/
	void Vk1Device::_GetPresentMode (OUT VkPresentModeKHR &presentMode, bool vsync,
											 const VkSurfaceCapabilitiesKHR &surfaceCaps) const
	{
		vk::uint32_t				count		= 0;
		Array< VkPresentModeKHR >	present_modes;
		
		presentMode = VK_PRESENT_MODE_FIFO_KHR;

		VK_CALL( vkGetPhysicalDeviceSurfacePresentModesKHR( _physicalDevice, _surface, &count, null ) );

		present_modes.Resize( count );
		VK_CALL( vkGetPhysicalDeviceSurfacePresentModesKHR( _physicalDevice, _surface, &count, present_modes.ptr() ) );

		if ( not vsync )
		{
			FOR( i, present_modes )
			{
				if ( present_modes[i] == VK_PRESENT_MODE_MAILBOX_KHR )
				{
					presentMode = VK_PRESENT_MODE_MAILBOX_KHR;
					break;
				}

				/*if ( presentMode != VK_PRESENT_MODE_MAILBOX_KHR and
					 present_modes[i] == VK_PRESENT_MODE_IMMEDIATE_KHR )
				{
					presentMode = VK_PRESENT_MODE_IMMEDIATE_KHR;
				}*/
			}
		}
	}

/*
=================================================
	_GetSurfaceImageCount
=================================================
*/
	void Vk1Device::_GetSurfaceImageCount (OUT vk::uint32_t &minImageCount, const VkSurfaceCapabilitiesKHR &surfaceCaps) const
	{
		minImageCount = surfaceCaps.minImageCount + 1;

		if ( surfaceCaps.maxImageCount > 0 and minImageCount > surfaceCaps.maxImageCount )
		{
			minImageCount = surfaceCaps.maxImageCount;
		}
	}
	
/*
=================================================
	_GetSurfaceTransform
=================================================
*/
	void Vk1Device::_GetSurfaceTransform (OUT VkSurfaceTransformFlagBitsKHR &transform,
											const VkSurfaceCapabilitiesKHR &surfaceCaps) const
	{
		if ( surfaceCaps.supportedTransforms & VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR )
		{
			transform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
		}
		else
		{
			transform = surfaceCaps.currentTransform;
		}
	}
	
/*
=================================================
	_GetImageUsage
=================================================
*/
	void Vk1Device::_GetImageUsage (OUT VkImageUsageFlags &imageUsage) const
	{
		imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

		VkFormatProperties	format_props;
		
		vkGetPhysicalDeviceFormatProperties( _physicalDevice, _colorFormat, &format_props );

		if ( format_props.optimalTilingFeatures & VK_FORMAT_FEATURE_BLIT_DST_BIT )
		{
			imageUsage |= VK_IMAGE_USAGE_TRANSFER_SRC_BIT;
		}
	}
	
/*
=================================================
	_GetSharingMode
=================================================
*/
	void Vk1Device::_GetSharingMode (OUT VkSharingMode &sharingMode) const
	{
		sharingMode = _queueFamilyIndices.present == _queueFamilyIndices.graphics ?
							VK_SHARING_MODE_EXCLUSIVE :
							VK_SHARING_MODE_CONCURRENT;
	}

/*
=================================================
	_DeleteSwapchain
=================================================
*/
	void Vk1Device::_DeleteSwapchain (VkSwapchainKHR &swapchain, SwapChainBuffers_t &buffers) const
	{
		if ( swapchain != VK_NULL_HANDLE ) 
		{ 
			FOR( i, buffers )
			{
				vkDestroyImageView( _logicalDevice, buffers[i].view, null );
			}

			vkDestroySwapchainKHR( _logicalDevice, swapchain, null );
		}

		swapchain = VK_NULL_HANDLE;
		buffers.Clear();
	}
	
/*
=================================================
	_CreateColorAttachment
=================================================
*/
	bool Vk1Device::_CreateColorAttachment (OUT SwapChainBuffers_t &buffers) const
	{
		vk::uint32_t		count = 0;
		Array< VkImage >	images;

		VK_CHECK( vkGetSwapchainImagesKHR( _logicalDevice, _swapchain, &count, null ) );
		images.Resize( count );

		VK_CHECK( vkGetSwapchainImagesKHR( _logicalDevice, _swapchain, &count, images.ptr() ) );
		buffers.Resize( count );

		FOR( i, buffers )
		{
			VkImageViewCreateInfo	color_attachment_view = {};

			color_attachment_view.sType			= VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
			color_attachment_view.viewType		= VK_IMAGE_VIEW_TYPE_2D;
			color_attachment_view.flags			= 0;
			color_attachment_view.format		= _colorFormat;
			color_attachment_view.components	= {
				VK_COMPONENT_SWIZZLE_R,
				VK_COMPONENT_SWIZZLE_G,
				VK_COMPONENT_SWIZZLE_B,
				VK_COMPONENT_SWIZZLE_A
			};
			color_attachment_view.subresourceRange.aspectMask		= VK_IMAGE_ASPECT_COLOR_BIT;
			color_attachment_view.subresourceRange.baseMipLevel		= 0;
			color_attachment_view.subresourceRange.levelCount		= 1;
			color_attachment_view.subresourceRange.baseArrayLayer	= 0;
			color_attachment_view.subresourceRange.layerCount		= 1;	// TODO
			
			buffers[i].image			= images[i];
			color_attachment_view.image	= buffers[i].image;
			
			VK_CHECK( vkCreateImageView( _logicalDevice, &color_attachment_view, null, OUT &buffers[i].view ) );
		}

		return true;
	}
	
/*
=================================================
	_CreateDepthStencilAttachment
=================================================
*/
	bool Vk1Device::_CreateDepthStencilAttachment (EPixelFormat::type depthStencilFormat)
	{
		if ( depthStencilFormat == EPixelFormat::Unknown )
		{
			_DeleteDepthStencilAttachment();
			return true;
		}

		CHECK_ERR( _depthStencilView != VK_NULL_HANDLE );
		CHECK_ERR( _depthStencilImage != VK_NULL_HANDLE );
		CHECK_ERR( _depthStencilMemory != VK_NULL_HANDLE );

		VkFormat				depth_stencil_fmt	= Vk1Enum( depthStencilFormat );

		VkMemoryRequirements	mem_reqs = {};

		VkImageCreateInfo		image_info = {};
		image_info.sType		= VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
		image_info.pNext		= null;
		image_info.imageType	= VK_IMAGE_TYPE_2D;
		image_info.format		= depth_stencil_fmt;
		image_info.extent		= { _surfaceSize.x, _surfaceSize.y, 1 };
		image_info.mipLevels	= 1;
		image_info.arrayLayers	= 1;
		image_info.samples		= VK_SAMPLE_COUNT_1_BIT;
		image_info.tiling		= VK_IMAGE_TILING_OPTIMAL;
		image_info.usage		= VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT;
		image_info.flags		= 0;

		VK_CHECK( vkCreateImage( _logicalDevice, &image_info, nullptr, OUT &_depthStencilImage ) );
		vkGetImageMemoryRequirements( _logicalDevice, _depthStencilImage, OUT &mem_reqs );


		VkMemoryAllocateInfo		mem_alloc = {};
		mem_alloc.sType				= VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		mem_alloc.pNext				= null;
		mem_alloc.allocationSize	= 0;
		mem_alloc.memoryTypeIndex	= 0;
		mem_alloc.allocationSize	= mem_reqs.size;

		CHECK_ERR( GetMemoryTypeIndex( mem_reqs.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, OUT mem_alloc.memoryTypeIndex ) );
		VK_CHECK( vkAllocateMemory( _logicalDevice, &mem_alloc, null, OUT &_depthStencilMemory ) );
		VK_CHECK( vkBindImageMemory( _logicalDevice, _depthStencilImage, _depthStencilMemory, 0 ) );


		VkImageViewCreateInfo		depth_stencil_view = {};
		depth_stencil_view.sType	= VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		depth_stencil_view.pNext	= null;
		depth_stencil_view.viewType	= VK_IMAGE_VIEW_TYPE_2D;
		depth_stencil_view.format	= depth_stencil_fmt;
		depth_stencil_view.flags	= 0;
		depth_stencil_view.image	= _depthStencilImage;

		depth_stencil_view.subresourceRange					= {};
		depth_stencil_view.subresourceRange.aspectMask		= VK_IMAGE_ASPECT_DEPTH_BIT | VK_IMAGE_ASPECT_STENCIL_BIT;
		depth_stencil_view.subresourceRange.baseMipLevel	= 0;
		depth_stencil_view.subresourceRange.levelCount		= 1;
		depth_stencil_view.subresourceRange.baseArrayLayer	= 0;
		depth_stencil_view.subresourceRange.layerCount		= 1;

		VK_CHECK( vkCreateImageView( _logicalDevice, &depth_stencil_view, null, OUT &_depthStencilView ) );

		_depthStencilFormat			= depth_stencil_fmt;
		_depthStencilPixelFormat	= depthStencilFormat;
		return true;
	}
	
/*
=================================================
	_DeleteDepthStencilAttachment
=================================================
*/
	void Vk1Device::_DeleteDepthStencilAttachment ()
	{
		_depthStencilFormat			= VK_FORMAT_UNDEFINED;
		_depthStencilPixelFormat	= EPixelFormat::Unknown;

		if ( _depthStencilView != VK_NULL_HANDLE )
		{
			vkDestroyImageView( _logicalDevice, _depthStencilView, null );
			_depthStencilView = VK_NULL_HANDLE;
		}
		
		if ( _depthStencilImage != VK_NULL_HANDLE )
		{
			vkDestroyImage( _logicalDevice, _depthStencilImage, null );
			_depthStencilImage = VK_NULL_HANDLE;
		}

		if ( _depthStencilMemory != VK_NULL_HANDLE )
		{
			vkFreeMemory( _logicalDevice, _depthStencilMemory, null );
			_depthStencilMemory = VK_NULL_HANDLE;
		}
	}
	
/*
=================================================
	GetMemoryTypeIndex
=================================================
*/
	bool Vk1Device::GetMemoryTypeIndex (vk::uint32_t memoryTypeBits, VkMemoryPropertyFlags flags, OUT vk::uint32_t &index) const
	{
		index = -1;

		for (vk::uint32_t i = 0; i < _deviceMemoryProperties.memoryTypeCount; ++i)
		{
			if ( (memoryTypeBits & 1) == 1 )
			{
				if ( (_deviceMemoryProperties.memoryTypes[i].propertyFlags & flags) == flags )
				{
					index = i;
					return true;
				}
			}

			memoryTypeBits >>= 1;
		}
		return false;
	}
	
/*
=================================================
	_CreateRenderPass
=================================================
*/
	bool Vk1Device::_CreateRenderPass ()
	{
		CHECK_ERR( not _renderPass );

		using Builder = RenderPassDescrBuilder;
		
		const bool	has_depth = (_depthStencilView != VK_NULL_HANDLE);

		Builder		builder;
		auto		subpass	= builder.AddSubpass();

		// depth attachment
		if ( has_depth )
		{
			Builder::DepthStencilAttachment_t	attach;
			Builder::AttachmentRef_t			attach_ref;
			Builder::SubpassDependency_t		dep;
			
			attach.format			= _depthStencilPixelFormat;
			attach.loadOp			= EAttachmentLoadOp::Clear;
			attach.storeOp			= EAttachmentStoreOp::Store;
			attach.stencilLoadOp	= EAttachmentLoadOp::None;
			attach.stencilStoreOp	= EAttachmentStoreOp::None;
			attach.initialLayout	= EImageLayout::Undefined;
			attach.finalLayout		= EImageLayout::DepthStencilAttachmentOptimal;
			builder.SetDepthStencilAttachment( attach );

			attach_ref.index		= Builder::AttachmentIndex(1);
			attach_ref.layout		= EImageLayout::DepthStencilAttachmentOptimal;
			subpass.SetDepthStencilAttachment( attach_ref );

			dep.srcPass				= Builder::SubpassIndex(0);
			dep.srcStage			= EPipelineStage::bits() | EPipelineStage::ColorAttachmentOutput;
			dep.srcAccess			= ESubpassAccess::bits() | ESubpassAccess::ColorAttachmentRead | ESubpassAccess::ColorAttachmentWrite;
			dep.dstPass				= Builder::SubpassIndexExternal;
			dep.dstStage			= EPipelineStage::bits() | EPipelineStage::BottomOfPipe;
			dep.dstAccess			= ESubpassAccess::bits() | ESubpassAccess::MemoryRead;
			dep.dependency			= ESubpassDependency::bits() | ESubpassDependency::ByRegion;
			builder.AddDependency( dep );
		}

		// color attachment
		{
			Builder::ColorAttachment_t		attach;
			Builder::AttachmentRef_t		attach_ref;
			Builder::SubpassDependency_t	dep;

			attach.format			= _colorPixelFormat;
			attach.loadOp			= EAttachmentLoadOp::Clear;
			attach.storeOp			= EAttachmentStoreOp::Store;
			attach.initialLayout	= EImageLayout::Undefined;
			attach.finalLayout		= EImageLayout::PresentSrc;
			builder.AddColorAttachment( attach );

			attach_ref.index		= Builder::AttachmentIndex(0);
			attach_ref.layout		= EImageLayout::ColorAttachmentOptimal;
			subpass.AddColorAttachment( attach_ref );

			dep.srcPass				= Builder::SubpassIndexExternal;
			dep.srcStage			= EPipelineStage::bits() | EPipelineStage::BottomOfPipe;
			dep.srcAccess			= ESubpassAccess::bits() | ESubpassAccess::MemoryRead;
			dep.dstPass				= Builder::SubpassIndex(0);
			dep.dstStage			= EPipelineStage::bits() | EPipelineStage::ColorAttachmentOutput;
			dep.dstAccess			= ESubpassAccess::bits() | ESubpassAccess::ColorAttachmentRead | ESubpassAccess::ColorAttachmentWrite;
			dep.dependency			= ESubpassDependency::bits() | ESubpassDependency::ByRegion;
			builder.AddDependency( dep );
		}

		ModulePtr	module;
		CHECK_ERR( GlobalSystems()->Get< ModulesFactory >()->Create(
					Vk1RenderPass::GetStaticID(),
					GlobalSystems(),
					CreateInfo::GpuRenderPass{ null, builder.Finish() },
					OUT module ) );

		_renderPass = module;
		return true;
	}

/*
=================================================
	_CreateFramebuffers
=================================================
*/
	bool Vk1Device::_CreateFramebuffers (OUT Framebuffers_t &frameBuffers) const
	{
		CHECK_ERR( not _imageBuffers.Empty() );
		CHECK_ERR( frameBuffers.Empty() );
		CHECK_ERR( _renderPass );

		frameBuffers.Resize( _imageBuffers.Count() );
		
		FOR( i, frameBuffers )
		{
			auto fb = GXTypes::New< Vk1SystemFramebuffer >( GlobalSystems(), VkSystems() );

			fb->Send( Message< ModuleMsg::AttachModule >{ null, _renderPass } );

			CHECK_ERR( fb->CreateFramebuffer( _surfaceSize, uint(i),
											  _renderPass.ToPtr< Vk1RenderPass >()->GetRenderPassID(),
											  _imageBuffers[i].view, _colorPixelFormat,
											  _depthStencilView, _depthStencilPixelFormat,
											  ETexture::Tex2D
			) );

			frameBuffers[i] = fb;
		}
		return true;
	}
	
/*
=================================================
	_DeleteFramebuffers
=================================================
*/
	void Vk1Device::_DeleteFramebuffers (INOUT Framebuffers_t &frameBuffers) const
	{
		Message< ModuleMsg::Delete >	msg;

		FOR( i, frameBuffers )
		{
			frameBuffers[i]->Send( msg );
		}

		frameBuffers.Clear();
	}

/*
=================================================
	_ChooseColorFormat
=================================================
*/
	bool Vk1Device::_ChooseColorFormat (OUT VkFormat &colorFormat, OUT VkColorSpaceKHR &colorSpace,
										const VkFormat requiredFormat, const VkColorSpaceKHR requiredColorSpace) const
	{
		vk::uint32_t				count		= 0;
		Array< VkSurfaceFormatKHR >	surf_formats;
		const VkFormat				def_format	= VK_FORMAT_B8G8R8A8_UNORM;
		const VkColorSpaceKHR		def_space	= VK_COLOR_SPACE_SRGB_NONLINEAR_KHR;

		VK_CHECK( vkGetPhysicalDeviceSurfaceFormatsKHR( _physicalDevice, _surface, &count, null ) );
		CHECK_ERR( count > 0 );

		surf_formats.Resize( count );
		VK_CHECK( vkGetPhysicalDeviceSurfaceFormatsKHR( _physicalDevice, _surface, &count, surf_formats.ptr() ) );
		
		if ( count == 1 and
			 surf_formats[0].format == VK_FORMAT_UNDEFINED )
		{
			colorFormat = requiredFormat;
			colorSpace  = surf_formats[0].colorSpace;
		}
		else
		{
			usize	both_match_idx		= -1;
			usize	format_match_idx	= -1;
			usize	space_match_idx		= -1;
			usize	def_format_idx		= 0;
			usize	def_space_idx		= 0;

			FOR( i, surf_formats )
			{
				if ( surf_formats[i].format		== requiredFormat and
					 surf_formats[i].colorSpace	== requiredColorSpace )
				{
					both_match_idx = i;
				}
				else
				// separate check
				if ( surf_formats[i].format		== requiredFormat )
					format_match_idx = i;
				else
				if ( surf_formats[i].colorSpace	== requiredColorSpace )
					space_match_idx = i;

				// check with default
				if ( surf_formats[i].format		== def_format )
					def_format_idx = i;

				if ( surf_formats[i].colorSpace	== def_space )
					def_space_idx = i;
			}

			usize	idx = 0;

			if ( both_match_idx != -1 )
				idx = both_match_idx;
			else
			if ( format_match_idx != -1 )
				idx = format_match_idx;
			else
				idx = def_format_idx;

			// TODO: space_match_idx and def_space_idx are unused yet

			colorFormat	= surf_formats[ idx ].format;
			colorSpace	= surf_formats[ idx ].colorSpace;
		}

		return true;
	}
	
/*
=================================================
	_GetQueueFamilyProperties
=================================================
*/
	bool Vk1Device::_GetQueueFamilyProperties (OUT Array<VkQueueFamilyProperties> &properties) const
	{
		vk::uint32_t	count = 0;

		vkGetPhysicalDeviceQueueFamilyProperties( _physicalDevice, &count, null );

		CHECK_ERR( count > 0 );
		properties.Resize( count );
	
		vkGetPhysicalDeviceQueueFamilyProperties( _physicalDevice, &count, properties.ptr() );
		return true;
	}
	
/*
=================================================
	_GetDeviceExtensions
=================================================
*/
	bool Vk1Device::_GetDeviceExtensions (OUT Array<String> &supportedExtensions) const
	{
		vk::uint32_t					count = 0;
		Array<VkExtensionProperties>	extensions;

		VK_CALL( vkEnumerateDeviceExtensionProperties( _physicalDevice, null, &count, null ) );

		if ( count > 0 )
		{
			extensions.Resize( count );

			VK_CHECK( vkEnumerateDeviceExtensionProperties( _physicalDevice, null, &count, extensions.ptr() ) );

			FOR( i, extensions )
			{
				supportedExtensions << extensions[i].extensionName;
			}
		}
		return true;
	}

/*
=================================================
	_GetQueueFamilyIndex
=================================================
*/
	bool Vk1Device::_GetQueueFamilyIndex (OUT vk::uint32_t &index, VkQueueFlags queueFlags,
										   const Array<VkQueueFamilyProperties> &queueFamilyProperties) const
	{
		index = -1;

		if ( queueFlags & VK_QUEUE_COMPUTE_BIT )
		{
			FOR( i, queueFamilyProperties )
			{
				if ( (queueFamilyProperties[i].queueFlags & queueFlags) == queueFlags and
					 (queueFamilyProperties[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) == 0 )
				{
					index = i;
					return true;
				}
			}
		}

		if ( queueFlags & VK_QUEUE_TRANSFER_BIT )
		{
			FOR( i, queueFamilyProperties )
			{
				if ( (queueFamilyProperties[i].queueFlags & queueFlags) == queueFlags and
					 (queueFamilyProperties[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) == 0 and
					 (queueFamilyProperties[i].queueFlags & VK_QUEUE_COMPUTE_BIT) == 0 )
				{
					index = i;
					return true;
				}
			}
		}
		
		FOR( i, queueFamilyProperties )
		{
			if ( (queueFamilyProperties[i].queueFlags & queueFlags) == queueFlags )
			{
				index = i;
				return true;
			}
		}

		RETURN_ERR( "no supported queue family found" );
	}
	
/*
=================================================
	_ChooseGraphicsAndPresentQueueFamilyIndex
=================================================
*/
	bool Vk1Device::_ChooseGraphicsAndPresentQueueFamilyIndex (OUT vk::uint32_t &graphicsIndex, OUT vk::uint32_t &presentIndex,
																const Array<VkQueueFamilyProperties> &properties) const
	{
		graphicsIndex	= -1;
		presentIndex	= -1;

		vk::uint32_t	both = -1;

		FOR( i, properties )
		{
			VkBool32	supports_present = false;
			VK_CALL( vkGetPhysicalDeviceSurfaceSupportKHR( _physicalDevice, i, _surface, &supports_present ) );

			if ( supports_present )
				presentIndex = i;

			if ( properties[i].queueFlags & VK_QUEUE_GRAPHICS_BIT )
			{
				graphicsIndex = i;

				if ( supports_present )
					both = i;
			}
		}

		if ( both != -1 )
		{
			graphicsIndex = presentIndex = both;
		}

		CHECK_ERR( graphicsIndex != -1 and presentIndex != -1 );
		return true;
	}
	
/*
=================================================
	_ChooseGraphicsQueueFamilyIndex
=================================================
*/
	bool Vk1Device::_ChooseGraphicsQueueFamilyIndex (OUT vk::uint32_t &index, const Array<VkQueueFamilyProperties> &properties) const
	{
		index = -1;
		
		FOR( i, properties )
		{
			if ( properties[i].queueFlags & VK_QUEUE_GRAPHICS_BIT )
			{
				index = i;
				return true;
			}
		}

		RETURN_ERR( "can't find graphics queue family index" );
	}

/*
=================================================
	_ChooseComputeQueueFamilyIndex
=================================================
*/
	bool Vk1Device::_ChooseComputeQueueFamilyIndex (OUT vk::uint32_t &index, const Array<VkQueueFamilyProperties> &properties) const
	{
		vk::uint32_t	alt_idx = -1;

		index = -1;
		
		FOR( i, properties )
		{
			if ( properties[i].queueFlags & VK_QUEUE_COMPUTE_BIT )
			{
				if ( (properties[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) == 0 )
				{
					index = i;
					return true;
				}

				alt_idx = i;
			}
		}

		if ( index == -1 and alt_idx != -1 )
		{
			index = alt_idx;
			return true;
		}
		
		RETURN_ERR( "can't find compute queue family index" );
	}
	
/*
=================================================
	_ChooseTransferQueueFamilyIndex
=================================================
*/
	bool Vk1Device::_ChooseTransferQueueFamilyIndex (OUT vk::uint32_t &index, const Array<VkQueueFamilyProperties> &properties) const
	{
		vk::uint32_t	alt_idx = -1;

		index = -1;

		FOR( i, properties )
		{
			if ( properties[i].queueFlags & VK_QUEUE_TRANSFER_BIT )
			{
				if ( (properties[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) == 0 and
					 (properties[i].queueFlags & VK_QUEUE_COMPUTE_BIT) == 0 )
				{
					index = i;
					return true;
				}

				alt_idx = i;
			}
		}
		
		if ( index == -1 and alt_idx != -1 )
		{
			index = alt_idx;
			return true;
		}
		
		RETURN_ERR( "can't find transfer queue family index" );
	}

/*
=================================================
	_GetQueueCreateInfos
=================================================
*/
	bool Vk1Device::_GetQueueCreateInfos (OUT Array<VkDeviceQueueCreateInfo> &queueCreateInfos,
										   OUT QueueFamilyIndices &queueFamilyIndices,
										   VkQueueFlags queueTypes, bool requirePresent) const
	{
		queueFamilyIndices = QueueFamilyIndices();

		Array< VkQueueFamilyProperties >	queue_family_props;
		Set< vk::uint32_t >					unique_indices;
		static const float					default_queue_priority	= 1.0f;	// high priority
		
		CHECK_ERR( _GetQueueFamilyProperties( OUT queue_family_props ) );

		// graphics
		if ( queueTypes & VK_QUEUE_GRAPHICS_BIT )
		{
			if ( requirePresent )
			{
				CHECK_ERR( _ChooseGraphicsAndPresentQueueFamilyIndex( OUT queueFamilyIndices.graphics,
																	  OUT queueFamilyIndices.present,
																	  queue_family_props ) );

				unique_indices.Add( queueFamilyIndices.graphics );
				unique_indices.Add( queueFamilyIndices.present );
			}
			else
			{
				CHECK_ERR( _ChooseGraphicsQueueFamilyIndex( OUT queueFamilyIndices.graphics, queue_family_props ) );

				unique_indices.Add( queueFamilyIndices.graphics );
			}
		}
		
		// compute
		if ( queueTypes & VK_QUEUE_COMPUTE_BIT )
		{
			CHECK_ERR( _ChooseComputeQueueFamilyIndex( OUT queueFamilyIndices.compute, queue_family_props ) );

			unique_indices.Add( queueFamilyIndices.compute );
		}
		
		// transfer
		if ( queueTypes & VK_QUEUE_TRANSFER_BIT )
		{
			CHECK_ERR( _ChooseTransferQueueFamilyIndex( OUT queueFamilyIndices.transfer, queue_family_props ) );

			unique_indices.Add( queueFamilyIndices.transfer );
		}


		FOR( i, unique_indices )
		{
			VkDeviceQueueCreateInfo		queue_info = {};
			queue_info.sType			= VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
			queue_info.queueFamilyIndex	= unique_indices[i];
			queue_info.queueCount		= 1;
			queue_info.pQueuePriorities	= &default_queue_priority;

			queueCreateInfos.PushBack( queue_info );
		}

		return true;
	}
	
/*
=================================================
	_CheckLayers
=================================================
*/
	bool Vk1Device::_CheckLayers (ValidationLayers_t layers) const
	{
		vk::uint32_t				count = 0;
		Array< VkLayerProperties >	props;

		VK_CALL( vkEnumerateInstanceLayerProperties( &count, null ) );
		props.Resize( count );
		
		VK_CALL( vkEnumerateInstanceLayerProperties( &count, props.ptr() ) );

		FOR( j, layers )
		{
			StringCRef	name	= layers[j];
			bool		found	= false;

			FOR( i, props )
			{
				if ( name.EqualsIC( props[i].layerName ) )
				{
					found = true;
					break;
				}
			}

			if ( not found )
				RETURN_ERR( "validation layer \"" << name << "\" not supported" );
		}

		return true;
	}
	
/*
=================================================
	_CreateSemaphores
=================================================
*/
	bool Vk1Device::_CreateSemaphores ()
	{
		CHECK_ERR( _imageAvailable == VK_NULL_HANDLE );
		CHECK_ERR( _renderFinished == VK_NULL_HANDLE );

		VkSemaphoreCreateInfo	info = {};
		info.sType	= VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

		VK_CHECK( vkCreateSemaphore( _logicalDevice, &info, null, OUT &_imageAvailable ) );
		VK_CHECK( vkCreateSemaphore( _logicalDevice, &info, null, OUT &_renderFinished ) );
		return true;
	}
	
/*
=================================================
	_DestroySemaphores
=================================================
*/
	void Vk1Device::_DestroySemaphores ()
	{
		if ( _imageAvailable != VK_NULL_HANDLE )
		{
			vkDestroySemaphore( _logicalDevice, _imageAvailable, null );
			_imageAvailable = VK_NULL_HANDLE;
		}

		if ( _renderFinished != VK_NULL_HANDLE )
		{
			vkDestroySemaphore( _logicalDevice, _renderFinished, null );
			_renderFinished = VK_NULL_HANDLE;
		}
	}
	
/*
=================================================
	_CreateCommandBuffers
=================================================
*/
	bool Vk1Device::_CreateCommandBuffers ()
	{
		CHECK_ERR( not _framebuffers.Empty() );

		Message< ModuleMsg::Link >		link_msg;
		Message< ModuleMsg::Compose >	comp_msg;

		_commandBuilder = GXTypes::New< Vk1CommandBufferBuilder >( GlobalSystems(), CreateInfo::GpuCommandBufferBuilder{} );
		_commandBuilder->Send( link_msg );
		_commandBuilder->Send( comp_msg );
		return true;
	}
	
/*
=================================================
	_DeleteCommandBuffers
=================================================
*/
	void Vk1Device::_DeleteCommandBuffers ()
	{
		Message< ModuleMsg::Delete >	msg;

		if ( _commandBuilder )
			_commandBuilder->Send( msg );

		_commandBuilder	= null;
	}

/*
=================================================
	_DebugReportCallback
=================================================
*/
	VkBool32 VKAPI_CALL Vk1Device::_DebugReportCallback (VkDebugReportFlagsEXT flags,
														  VkDebugReportObjectTypeEXT objectType,
														  vk::uint64_t object,
														  size_t location,
														  int32_t messageCode,
														  const char* pLayerPrefix,
														  const char* pMessage,
														  void* pUserData)
	{
		String	log;

		log << "Vulkan " << _DebugReportFlagsToString( (VkDebugReportFlagBitsEXT)flags )
			<< " in object: " << _DebugReportObjectTypeToString( objectType )
			<< '(' << String().FormatAlignedI( object, 8, '0', 16 ) << ')'
			<< ", layer: " << pLayerPrefix << '(' << messageCode << ')'
			<< ", message:\n" << pMessage;

		LOG( log.cstr(), _DebugReportFlagsToLogType( (VkDebugReportFlagBitsEXT)flags ) );
		return VK_FALSE;
	}
	
/*
=================================================
	_DebugReportFlagsToString
=================================================
*/
	StringCRef Vk1Device::_DebugReportFlagsToString (VkDebugReportFlagBitsEXT flags)
	{
		if ( EnumEq( flags, VK_DEBUG_REPORT_ERROR_BIT_EXT ) )
			return "error";

		if ( EnumEq( flags, VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT ) )
			return "perfomance";

		if ( EnumEq( flags, VK_DEBUG_REPORT_WARNING_BIT_EXT ) )
			return "warning";

		if ( EnumEq( flags, VK_DEBUG_REPORT_INFORMATION_BIT_EXT ) )
			return "info";

		if ( EnumEq( flags, VK_DEBUG_REPORT_DEBUG_BIT_EXT ) )
			return "debug";

		return "";
	}
	
/*
=================================================
	_DebugReportFlagsToLogType
=================================================
*/
	ELog::type Vk1Device::_DebugReportFlagsToLogType (vk::VkDebugReportFlagBitsEXT flags)
	{
		if ( EnumEq( flags, VK_DEBUG_REPORT_ERROR_BIT_EXT ) )
			return ELog::Error;

		if ( EnumEq( flags, VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT ) )
			return ELog::Debug;

		if ( EnumEq( flags, VK_DEBUG_REPORT_WARNING_BIT_EXT ) )
			return ELog::Debug;

		if ( EnumEq( flags, VK_DEBUG_REPORT_INFORMATION_BIT_EXT ) )
			return ELog::Debug;

		if ( EnumEq( flags, VK_DEBUG_REPORT_DEBUG_BIT_EXT ) )
			return ELog::Debug;

		return ELog::Debug;
	}

/*
=================================================
	_DebugReportObjectTypeToString
=================================================
*/
	StringCRef Vk1Device::_DebugReportObjectTypeToString (VkDebugReportObjectTypeEXT objType)
	{
		switch ( objType )
		{
			//case VK_DEBUG_REPORT_OBJECT_TYPE_UNKNOWN_EXT : return "unknown";
			case VK_DEBUG_REPORT_OBJECT_TYPE_INSTANCE_EXT			: return "Instance";
			case VK_DEBUG_REPORT_OBJECT_TYPE_PHYSICAL_DEVICE_EXT	: return "PhysicalDevice";
			case VK_DEBUG_REPORT_OBJECT_TYPE_DEVICE_EXT				: return "Device";
			case VK_DEBUG_REPORT_OBJECT_TYPE_QUEUE_EXT				: return "Queue";
			case VK_DEBUG_REPORT_OBJECT_TYPE_SEMAPHORE_EXT			: return "Semaphore";
			case VK_DEBUG_REPORT_OBJECT_TYPE_COMMAND_BUFFER_EXT		: return "CommandBuffer";
			case VK_DEBUG_REPORT_OBJECT_TYPE_FENCE_EXT				: return "Fence";
			case VK_DEBUG_REPORT_OBJECT_TYPE_DEVICE_MEMORY_EXT		: return "DeviceMemory";
			case VK_DEBUG_REPORT_OBJECT_TYPE_BUFFER_EXT				: return "Buffer";
			case VK_DEBUG_REPORT_OBJECT_TYPE_IMAGE_EXT				: return "Image";
			case VK_DEBUG_REPORT_OBJECT_TYPE_EVENT_EXT				: return "Event";
			case VK_DEBUG_REPORT_OBJECT_TYPE_QUERY_POOL_EXT			: return "QueryPool";
			case VK_DEBUG_REPORT_OBJECT_TYPE_BUFFER_VIEW_EXT		: return "BufferView";
			case VK_DEBUG_REPORT_OBJECT_TYPE_IMAGE_VIEW_EXT			: return "ImageView";
			case VK_DEBUG_REPORT_OBJECT_TYPE_SHADER_MODULE_EXT		: return "ShaderModule";
			case VK_DEBUG_REPORT_OBJECT_TYPE_PIPELINE_CACHE_EXT		: return "PipelineCache";
			case VK_DEBUG_REPORT_OBJECT_TYPE_PIPELINE_LAYOUT_EXT	: return "PipelineLayout";
			case VK_DEBUG_REPORT_OBJECT_TYPE_RENDER_PASS_EXT		: return "RenderPass";
			case VK_DEBUG_REPORT_OBJECT_TYPE_PIPELINE_EXT			: return "Pipeline";
			case VK_DEBUG_REPORT_OBJECT_TYPE_DESCRIPTOR_SET_LAYOUT_EXT	: return "DescriptorSetLayout";
			case VK_DEBUG_REPORT_OBJECT_TYPE_SAMPLER_EXT			: return "Sampler";
			case VK_DEBUG_REPORT_OBJECT_TYPE_DESCRIPTOR_POOL_EXT	: return "DescriptorPool";
			case VK_DEBUG_REPORT_OBJECT_TYPE_DESCRIPTOR_SET_EXT		: return "DescriptorSet";
			case VK_DEBUG_REPORT_OBJECT_TYPE_FRAMEBUFFER_EXT		: return "Framebuffer";
			case VK_DEBUG_REPORT_OBJECT_TYPE_COMMAND_POOL_EXT		: return "CommandPool";
			case VK_DEBUG_REPORT_OBJECT_TYPE_SURFACE_KHR_EXT		: return "Surface";
			case VK_DEBUG_REPORT_OBJECT_TYPE_SWAPCHAIN_KHR_EXT		: return "Swapchain";
			case VK_DEBUG_REPORT_OBJECT_TYPE_DEBUG_REPORT_EXT		: return "DebugReport";
			case VK_DEBUG_REPORT_OBJECT_TYPE_DISPLAY_KHR_EXT		: return "Display";
			case VK_DEBUG_REPORT_OBJECT_TYPE_DISPLAY_MODE_KHR_EXT	: return "DisplayMode";
			case VK_DEBUG_REPORT_OBJECT_TYPE_OBJECT_TABLE_NVX_EXT	: return "ObjectTableNvx";
			case VK_DEBUG_REPORT_OBJECT_TYPE_INDIRECT_COMMANDS_LAYOUT_NVX_EXT	: return "IndirectCommandsLayoutNvx";
			default :
				CHECK( objType >= VK_DEBUG_REPORT_OBJECT_TYPE_BEGIN_RANGE_EXT and
					   objType <= VK_DEBUG_REPORT_OBJECT_TYPE_END_RANGE_EXT );
				return "unknown";
		}
	}
	
	
}	// PlatformVK
}	// Engine

#endif	// GRAPHICS_API_VULKAN
