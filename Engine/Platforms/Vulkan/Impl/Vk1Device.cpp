// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Engine/Platforms/Vulkan/Impl/Vk1Device.h"
#include "Engine/Platforms/Vulkan/Impl/vulkan1_utils.h"

#if defined( GRAPHICS_API_VULKAN )

#include "Engine/Platforms/Vulkan/Impl/Vk1SystemFramebuffer.h"
#include "Engine/Platforms/Shared/GPU/RenderPass.h"
#include "Engine/Platforms/Shared/GPU/CommandBuffer.h"

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
	Vk1Device::Vk1Device (GlobalSystemsRef gs) :
		BaseObject( gs ),
		_logicalDevice( VK_NULL_HANDLE ),
		_physicalDevice( VK_NULL_HANDLE ),
		_instance( VK_NULL_HANDLE ),
		_surface( VK_NULL_HANDLE ),
		_swapchain( VK_NULL_HANDLE ),
		_vsync( false ),
		_depthStencilImage( VK_NULL_HANDLE ),
		_depthStencilMemory( VK_NULL_HANDLE ),
		_depthStencilView( VK_NULL_HANDLE ),
		_colorPixelFormat( EPixelFormat::Unknown ),
		_depthStencilPixelFormat( EPixelFormat::Unknown ),
		_colorFormat( VK_FORMAT_UNDEFINED ),
		_colorSpace( VK_COLOR_SPACE_MAX_ENUM_KHR ),
		_depthStencilFormat( VK_FORMAT_UNDEFINED ),
		_queue( VK_NULL_HANDLE ),
		_queueIndex( UMax ),
		_queueFamily(),
		_currentImageIndex( UMax ),
		_graphicsQueueSubmited( false ),
		_imageAvailable( VK_NULL_HANDLE ),
		_renderFinished( VK_NULL_HANDLE ),
		_debugCallback( VK_NULL_HANDLE ),
		_enableDebugMarkers( false ),
		_isInstanceFunctionsLoaded( false ),
		_isDeviceFunctionsLoaded( false )
	{
		SetDebugName( "Vk1Device" );

		_imageBuffers.Reserve( 8 );
	}
	
/*
=================================================
	destructor
=================================================
*/
	Vk1Device::~Vk1Device ()
	{
		CHECK( not IsInstanceCreated() );
		CHECK( not HasPhyiscalDevice() );
		CHECK( not IsDeviceCreated() );
		CHECK( not IsSurfaceCreated() );
		CHECK( not IsSwapchainCreated() );
		CHECK( not IsDebugCallbackCreated() );
	}
	
/*
=================================================
	_LoadFunctions
=================================================
*/
	bool Vk1Device::_LoadFunctions () const
	{
		if ( _isInstanceFunctionsLoaded and _isDeviceFunctionsLoaded )
			return true;

		if ( not _isInstanceFunctionsLoaded )
		{
			CHECK_ERR( Vk1_Init( null ) );
			_isInstanceFunctionsLoaded = true;
		}

		if ( not _isDeviceFunctionsLoaded and IsInstanceCreated() )
		{
			CHECK_ERR( Vk1_Init( GetInstance() ) );
			_isDeviceFunctionsLoaded = true;
		}
		return true;
	}
	
/*
=================================================
	_LoadInstanceLayers
=================================================
*/
	bool Vk1Device::_LoadInstanceLayers () const
	{
		if ( not _instanceLayers.Empty() )
			return true;

		_LoadFunctions();
		
		uint32_t	count = 0;
		VK_CALL( vkEnumerateInstanceLayerProperties( OUT &count, null ) );

		if (count == 0)
			return true;

		_instanceLayers.Resize( count );
		VK_CALL( vkEnumerateInstanceLayerProperties( OUT &count, OUT _instanceLayers.ptr() ) );

		return true;
	}
	
/*
=================================================
	_LoadInstanceExtensions
=================================================
*/
	bool Vk1Device::_LoadInstanceExtensions () const
	{
		if ( not _instanceExtensions.Empty() )
			return true;

		_LoadFunctions();
		
		uint32_t	count = 0;
		VK_CALL( vkEnumerateInstanceExtensionProperties( null, OUT &count, null ) );

		if ( count == 0 )
			return false;

		_instanceExtensions.Resize( count );
		VK_CALL( vkEnumerateInstanceExtensionProperties( null, OUT &count, OUT _instanceExtensions.ptr() ) );

		return true;
	}

/*
=================================================
	CreateInstance
=================================================
*/
	bool Vk1Device::CreateInstance (StringCRef applicationName, uint32_t applicationVersion,
									 uint32_t vulkanVersion, ExtensionNames_t ext, ValidationLayers_t layers)
	{
		CHECK_ERR( not IsInstanceCreated() );

		_LoadFunctions();

		VkApplicationInfo		app_info				= {};
		VkInstanceCreateInfo	instance_create_info	= {};
		Array< const char* >	instance_layers			= layers;
		Array< const char* >	instance_extensions		= { VK_KHR_SURFACE_EXTENSION_NAME };

		instance_extensions << ext;

		CHECK_ERR( _CheckLayers( INOUT instance_layers ) );
		CHECK_ERR( _CheckExtensions( INOUT instance_extensions ) );

		app_info.sType				= VK_STRUCTURE_TYPE_APPLICATION_INFO;
		app_info.apiVersion			= vulkanVersion;

		app_info.pApplicationName	= applicationName.cstr();
		app_info.applicationVersion	= applicationVersion;

		app_info.pEngineName		= Engine::ENGINE_NAME;
		app_info.engineVersion		= Engine::ENGINE_VERSION;

		
		instance_create_info.sType						= VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
		instance_create_info.pApplicationInfo			= &app_info;

		instance_create_info.enabledExtensionCount		= (uint32_t) instance_extensions.Count();
		instance_create_info.ppEnabledExtensionNames	= instance_extensions.ptr();
		
		instance_create_info.enabledLayerCount			= (uint32_t) layers.Count();
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

		_isDeviceFunctionsLoaded	= false;
		_isInstanceFunctionsLoaded	= false;

		_instanceLayers.Clear();
		_instanceExtensions.Clear();
		
		Vk1_Delete();
		return true;
	}
	
/*
=================================================
	HasLayer
=================================================
*/
	bool Vk1Device::HasLayer (StringCRef name) const
	{
		_LoadInstanceLayers();

		// TODO: optimize search
		FOR( i, _instanceLayers )
		{
			if ( name.EqualsIC( _instanceLayers[i].layerName ) )
				return true;
		}
		return false;
	}
	
/*
=================================================
	HasExtension
=================================================
*/
	bool Vk1Device::HasExtension (StringCRef name) const
	{
		_LoadInstanceExtensions();

		// TODO: optimize search
		FOR( i, _instanceExtensions )
		{
			if ( name.EqualsIC( _instanceExtensions[i].extensionName ) )
				return true;
		}
		return false;
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

		_LoadFunctions();

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
	GetPhysicalDeviceInfo
=================================================
*/
	bool Vk1Device::GetPhysicalDeviceInfo (OUT AppendableAdaptor<DeviceInfo> deviceInfo) const
	{
		CHECK_ERR( IsInstanceCreated() );

		_LoadFunctions();
		
		uint32_t						count	= 0;
		Array< VkPhysicalDevice >		devices;
		Array< VkDisplayPropertiesKHR >	display_props;		display_props.Reserve( 8 );

		VK_CALL( vkEnumeratePhysicalDevices( _instance, OUT &count, null ) );
		CHECK_ERR( count > 0 );

		devices.Resize( count );
		VK_CALL( vkEnumeratePhysicalDevices( _instance, OUT &count, OUT devices.ptr() ) );

		FOR( i, devices )
		{
			VkPhysicalDeviceProperties			prop;
			VkPhysicalDeviceFeatures			feat;
			VkPhysicalDeviceMemoryProperties	mem_prop;
			DeviceInfo							info;

			vkGetPhysicalDeviceProperties( devices[i], OUT &prop );
			vkGetPhysicalDeviceFeatures( devices[i], OUT &feat );
			vkGetPhysicalDeviceMemoryProperties( devices[i], OUT &mem_prop );
			// vkGetPhysicalDeviceQueueFamilyProperties
			// vkGetPhysicalDeviceSparseImageFormatProperties
			// vkGetPhysicalDeviceDisplayPlanePropertiesKHR
			// vkGetPhysicalDeviceFeatures2KHR
			// vkGetPhysicalDeviceProperties2KHR
			// vkGetPhysicalDeviceMemoryProperties2KHR
			// vkGetPhysicalDeviceSparseImageFormatProperties2KHR

			/*uint32_t	prop_count = 0;
			VK_CALL( vkGetPhysicalDeviceDisplayPropertiesKHR( devices[i], OUT &prop_count, null ) );

			if ( prop_count > 0 )
			{
				display_props.Resize( prop_count );
				VK_CALL( vkGetPhysicalDeviceDisplayPropertiesKHR( devices[i], OUT &prop_count, OUT display_props.ptr() ) );

				FOR( j, display_props )
				{
				}
			}*/


			info.id				= devices[i];
			info.device			= prop.deviceName;
			info.version		= prop.apiVersion;
			info.integratedGPU	= prop.deviceType == VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU;
			info.isGPU			= (prop.deviceType == VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU or
								   prop.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU);
			info.isCPU			= prop.deviceType == VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU;
			info.maxInvocations	= prop.limits.maxComputeWorkGroupInvocations;

			info.supportsTesselation	= feat.tessellationShader;
			info.supportsGeometryShader	= feat.geometryShader;

			for (uint32_t j = 0; j < mem_prop.memoryTypeCount; ++j)
			{
				if ( mem_prop.memoryTypes[j].propertyFlags == VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT )
				{
					uint32_t idx = mem_prop.memoryTypes[j].heapIndex;

					ASSERT( !!(mem_prop.memoryHeaps[idx].flags & VK_MEMORY_HEAP_DEVICE_LOCAL_BIT) );
					
					info.globalMemory += BytesUL( mem_prop.memoryHeaps[idx].size );
					mem_prop.memoryHeaps[idx].size = 0;
				}
			}

			deviceInfo.PushBack( RVREF(info) );
		}
		return true;
	}
	
/*
=================================================
	CreatePhysicalDevice
=================================================
*/
	bool Vk1Device::CreatePhysicalDevice (vk::VkPhysicalDevice id)
	{
		CHECK_ERR( IsInstanceCreated() );
		
		_LoadFunctions();

		_physicalDevice = id;

		vkGetPhysicalDeviceProperties( _physicalDevice, OUT &_deviceProperties );
		vkGetPhysicalDeviceFeatures( _physicalDevice, OUT &_deviceFeatures );
		vkGetPhysicalDeviceMemoryProperties( _physicalDevice, OUT &_deviceMemoryProperties );

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
		
		_LoadFunctions();

		uint32_t					count				= 0;

		usize						name_match_idx		= UMax;
		usize						high_version_idx	= UMax;
		usize						best_gpu_idx		= UMax;

		uint32_t					version				= 0;
		VkPhysicalDeviceType		gpu_type			= VK_PHYSICAL_DEVICE_TYPE_OTHER;
		Array< VkPhysicalDevice >	devices;

		VK_CALL( vkEnumeratePhysicalDevices( _instance, OUT &count, null ) );
		CHECK_ERR( count > 0 );

		devices.Resize( count );
		VK_CALL( vkEnumeratePhysicalDevices( _instance, OUT &count, OUT devices.ptr() ) );

		FOR( i, devices )
		{
			VkPhysicalDeviceProperties	prop;

			vkGetPhysicalDeviceProperties( devices[i], OUT &prop );

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

			if ( name_match_idx == UMax and
				 StringCRef( prop.deviceName ).HasSubStringIC( deviceName ) )
			{
				name_match_idx = i;
			}
		}

		if ( name_match_idx != UMax )
			_physicalDevice = devices[name_match_idx];
		else
		if ( best_gpu_idx != UMax )
			_physicalDevice = devices[best_gpu_idx];
		else
		if ( high_version_idx != UMax )
			_physicalDevice = devices[high_version_idx];
		else
			_physicalDevice = devices[0];


		vkGetPhysicalDeviceProperties( _physicalDevice, OUT &_deviceProperties );
		vkGetPhysicalDeviceFeatures( _physicalDevice, OUT &_deviceFeatures );
		vkGetPhysicalDeviceMemoryProperties( _physicalDevice, OUT &_deviceMemoryProperties );

		return true;
	}
	
/*
=================================================
	_DeviceTypeToString
=================================================
*/
	StringCRef Vk1Device::_DeviceTypeToString (VkPhysicalDeviceType value)
	{
		switch ( value )
		{
			case VK_PHYSICAL_DEVICE_TYPE_OTHER :			return "Other";
			case VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU :	return "Intergrated GPU";
			case VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU :		return "Discrete GPU";
			case VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU :		return "Virtual GPU";
			case VK_PHYSICAL_DEVICE_TYPE_CPU :				return "CPU";
		}
		RETURN_ERR( "unknown physical device type!" );
	}
	
/*
=================================================
	WritePhysicalDeviceInfo
=================================================
*/
	bool Vk1Device::WritePhysicalDeviceInfo () const
	{
		CHECK_ERR( HasPhyiscalDevice() );

		String	str;

		str << "Vulkan info\n---------------------";
		str << "\ndevice name: " << _deviceProperties.deviceName;
		str << "\ndevice type: " << _DeviceTypeToString( _deviceProperties.deviceType );

		str << "\n---------------------";

		LOG( str.cstr(), ELog::Info | ELog::SpoilerFlag );
		return true;
	}
	
/*
=================================================
	CreateDevice
=================================================
*/
	bool Vk1Device::CreateDevice (const VkPhysicalDeviceFeatures &enabledFeatures,
								   EQueueFamily::bits queueFamilies,
								   ExtensionNames_t enabledExtensions)
	{
		const bool useSwapchain = queueFamilies[ EQueueFamily::Present ];

		CHECK_ERR( HasPhyiscalDevice() );
		CHECK_ERR( useSwapchain == IsSurfaceCreated() );
		CHECK_ERR( not IsDeviceCreated() );

		Array< VkDeviceQueueCreateInfo >	queue_infos;
		Array< const char * >				device_extensions	= enabledExtensions;
		VkDeviceCreateInfo					device_info			= {};
		
		_queueFamily = queueFamilies;
		_queueIndex  = UMax;

		CHECK_ERR( _GetQueueCreateInfos( OUT queue_infos, INOUT _queueFamily, OUT _queueIndex ) );

		if ( _queueFamily[ EQueueFamily::Present ] )
			device_extensions << VK_KHR_SWAPCHAIN_EXTENSION_NAME;
		
		CHECK_ERR( _CheckDeviceExtensions( INOUT device_extensions ) );

		_enableDebugMarkers = HasExtension( VK_EXT_DEBUG_MARKER_EXTENSION_NAME );
		
		device_info.sType					= VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
		device_info.queueCreateInfoCount	= (uint32_t) queue_infos.Count();
		device_info.pQueueCreateInfos		= queue_infos.ptr();
		device_info.pEnabledFeatures		= &enabledFeatures;
		
		if ( not device_extensions.Empty() )
		{
			device_info.enabledExtensionCount	= (uint32_t) device_extensions.Count();
			device_info.ppEnabledExtensionNames	= device_extensions.ptr();
		}

		VK_CHECK( vkCreateDevice( _physicalDevice, &device_info, null, OUT &_logicalDevice ) );

		if ( &_deviceFeatures != &enabledFeatures )
			_deviceFeatures = enabledFeatures;

		// write extensions to log
		{
			String	log = "Vulkan layers:";
			
			FOR( i, _instanceLayers ) {
				log << _instanceLayers[i].layerName << " (" << _instanceLayers[i].description << ")\n";
			}
			log << "\nVulkan instance extensions:";

			FOR( i, _instanceExtensions ) {
				log << (i ? ", " : "") << ((i&3) ? "" : "\n") << _instanceExtensions[i].extensionName;
			}
			log << "\nVulkan device extensions:";

			FOR( i, _deviceExtensions ) {
				log << (i ? ", " : "") << ((i&3) ? "" : "\n") << _deviceExtensions[i].extensionName;
			}
			log << "\n------------------------";

			LOG( log.cstr(), ELog::Info | ELog::SpoilerFlag );
		}
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
		CHECK_ERR( not IsQueueCreated() );
		
		vkDestroyDevice( _logicalDevice, null );

		_logicalDevice = VK_NULL_HANDLE;

		_deviceExtensions.Clear();
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
	bool Vk1Device::CreateSwapchain (const uint2 &size, bool vsync, uint32_t imageArrayLayers, EPixelFormat::type depthStencilFormat, MultiSamples samples)
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

		// create dependent resources
		CHECK_ERR( _CreateColorAttachment( samples, OUT _imageBuffers ) );
		CHECK_ERR( _CreateDepthStencilAttachment( depthStencilFormat ) );
		CHECK_ERR( _CreateRenderPass() );
		CHECK_ERR( _CreateFramebuffers( OUT _framebuffers ) );
		CHECK_ERR( _CreateSemaphores() );

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
		_DestroySemaphores();

		_surfaceSize = uint2();
		return true;
	}
	
/*
=================================================
	SetSurface
=================================================
*/
	bool Vk1Device::SetSurface (VkSurfaceKHR surface, EPixelFormat::type colorFmt)
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
	_ChooseQueueIndex
=================================================
*/
	bool Vk1Device::_ChooseQueueIndex (INOUT EQueueFamily::bits &family, OUT uint32_t &index) const
	{
		Array< VkQueueFamilyProperties >	queue_family_props;
		CHECK_ERR( _GetQueueFamilyProperties( OUT queue_family_props ) );
		
		FOR( i, queue_family_props )
		{
			EQueueFamily::bits	flags;

			VkBool32	supports_present = false;
			VK_CALL( vkGetPhysicalDeviceSurfaceSupportKHR( _physicalDevice, uint32_t(i), _surface, OUT &supports_present ) );

			if ( supports_present )
				flags |= EQueueFamily::Present;

			if ( queue_family_props[i].queueFlags & VK_QUEUE_GRAPHICS_BIT )
			{
				flags |= EQueueFamily::Graphics;
			}
	
			if ( queue_family_props[i].queueFlags & VK_QUEUE_COMPUTE_BIT )
			{
				flags |= EQueueFamily::Compute;
			}
			
			if ( queue_family_props[i].queueFlags & VK_QUEUE_TRANSFER_BIT )
			{
				flags |= EQueueFamily::Transfer;
			}
			
			if ( queue_family_props[i].queueFlags & VK_QUEUE_SPARSE_BINDING_BIT )
			{
				flags |= EQueueFamily::SparseBinding;
			}

			if ( flags == family )
			{
				index = uint32_t(i);
				return true;
			}
		}

		// TODO: find nearest queue family

		RETURN_ERR( "no suitable queue family found!" );
	}
	
/*
=================================================
	_GetQueueCreateInfos
=================================================
*/
	bool Vk1Device::_GetQueueCreateInfos (OUT Array<VkDeviceQueueCreateInfo> &queueCreateInfos,
										  INOUT EQueueFamily::bits &queueFamily,
										  OUT uint32_t &queueIndex) const
	{
		static const float	default_queue_priority	= 1.0f;	// high priority

		CHECK_ERR( _ChooseQueueIndex( INOUT queueFamily, OUT queueIndex ) );

		VkDeviceQueueCreateInfo		queue_info = {};
		queue_info.sType			= VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		queue_info.queueFamilyIndex	= queueIndex;
		queue_info.queueCount		= 1;
		queue_info.pQueuePriorities	= &default_queue_priority;

		queueCreateInfos.PushBack( queue_info );
		return true;
	}

/*
=================================================
	CreateQueue
=================================================
*/
	bool Vk1Device::CreateQueue ()
	{
		CHECK_ERR( IsDeviceCreated() );
		CHECK_ERR( not IsQueueCreated() );

		vkGetDeviceQueue( _logicalDevice, _queueIndex, 0, OUT &_queue );
		return true;
	}
	
/*
=================================================
	DestroyQueue
=================================================
*/
	void Vk1Device::DestroyQueue ()
	{
		_queue			= VK_NULL_HANDLE;
		_queueIndex		= UMax;
		_queueFamily	= EQueueFamily::bits();
	}

/*
=================================================
	BeginFrame
=================================================
*/
	bool Vk1Device::BeginFrame ()
	{
		CHECK_ERR( IsSwapchainCreated() );

		_graphicsQueueSubmited	= false;
		_currentImageIndex		= UMax;

		uint32_t	image_index = UMax;
		VkResult	result		= vkAcquireNextImageKHR( _logicalDevice, _swapchain, UMax, _imageAvailable,
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
		CHECK_ERR( IsSwapchainCreated() );
		CHECK_ERR( _currentImageIndex < _framebuffers.Count() );
		
		VkPresentInfoKHR	present_info		= {};
		VkSwapchainKHR		swap_chains[]		= { _swapchain };
		VkSemaphore			signal_semaphores[] = { _renderFinished };

		present_info.sType			= VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
		present_info.swapchainCount	= (uint32_t) CountOf( swap_chains );
		present_info.pSwapchains	= swap_chains;
		present_info.pImageIndices	= &_currentImageIndex;

		if ( _graphicsQueueSubmited ) {
			present_info.waitSemaphoreCount	= (uint32_t) CountOf( signal_semaphores );
			present_info.pWaitSemaphores	= signal_semaphores;
		}
		else {
			present_info.waitSemaphoreCount	= 0;
			present_info.pWaitSemaphores	= null;
		}

		VK_CHECK( vkQueuePresentKHR( GetQueue(), &present_info ) );
		
		_graphicsQueueSubmited	= false;
		_currentImageIndex		= UMax;
		return true;
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
	SubmitQueue
----
	this method used for submit command buffers with synchronization between frame changes,
	if you don't need sync, use GetQueue() and write your own submission.
=================================================
*/
	bool Vk1Device::SubmitQueue (ArrayCRef<VkCommandBuffer> cmdBuffers, VkFence fence)
	{
		CHECK_ERR( _currentImageIndex < _framebuffers.Count() );
		CHECK_ERR( IsQueueCreated() );

		// submit command buffers to grpahics/compute queue
		VkSubmitInfo			submit_info			= {};
		VkSemaphore				wait_semaphores[]	= { _imageAvailable };
		VkPipelineStageFlags	wait_stages[]		= { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
		VkSemaphore				signal_semaphores[] = { _renderFinished };

		submit_info.sType					= VK_STRUCTURE_TYPE_SUBMIT_INFO;
		submit_info.waitSemaphoreCount		= (uint32_t) CountOf( wait_semaphores );
		submit_info.pWaitSemaphores			= wait_semaphores;
		submit_info.pWaitDstStageMask		= wait_stages;
		submit_info.commandBufferCount		= (uint32_t) cmdBuffers.Count();
		submit_info.pCommandBuffers			= cmdBuffers.RawPtr();
		submit_info.signalSemaphoreCount	= (uint32_t) CountOf( signal_semaphores );
		submit_info.pSignalSemaphores		= signal_semaphores;

		VK_CHECK( vkQueueSubmit( GetQueue(), 1, &submit_info, fence ) );

		_graphicsQueueSubmited = true;
		return true;
	}

/*
=================================================
	_GetSwapChainExtent
=================================================
*/
	void Vk1Device::_GetSwapChainExtent (OUT VkExtent2D &extent, const VkSurfaceCapabilitiesKHR &surfaceCaps) const
	{
		if ( surfaceCaps.currentExtent.width  == UMax and
			 surfaceCaps.currentExtent.height == UMax )
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
		uint32_t					count		= 0;
		Array< VkPresentModeKHR >	present_modes;
		
		presentMode = VK_PRESENT_MODE_FIFO_KHR;

		VK_CALL( vkGetPhysicalDeviceSurfacePresentModesKHR( _physicalDevice, _surface, OUT &count, null ) );
		CHECK_ERR( count > 0, void() );

		present_modes.Resize( count );
		VK_CALL( vkGetPhysicalDeviceSurfacePresentModesKHR( _physicalDevice, _surface, OUT &count, OUT present_modes.ptr() ) );

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
	void Vk1Device::_GetSurfaceImageCount (OUT uint32_t &minImageCount, const VkSurfaceCapabilitiesKHR &surfaceCaps) const
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
		
		vkGetPhysicalDeviceFormatProperties( _physicalDevice, _colorFormat, OUT &format_props );

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
		sharingMode = VK_SHARING_MODE_EXCLUSIVE;
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
	bool Vk1Device::_CreateColorAttachment (MultiSamples samples, OUT SwapChainBuffers_t &buffers) const
	{
		uint32_t			count = 0;
		Array< VkImage >	images;

		VK_CHECK( vkGetSwapchainImagesKHR( _logicalDevice, _swapchain, OUT &count, null ) );
		CHECK_ERR( count > 0 );
		images.Resize( count );

		VK_CHECK( vkGetSwapchainImagesKHR( _logicalDevice, _swapchain, OUT &count, OUT images.ptr() ) );
		CHECK_ERR( count > 0 );
		buffers.Resize( count );

		FOR( i, buffers )
		{
			VkImageViewCreateInfo	color_attachment_view = {};

			color_attachment_view.sType			= VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
			color_attachment_view.viewType		= VK_IMAGE_VIEW_TYPE_2D;
			color_attachment_view.flags			= 0;
			color_attachment_view.format		= _colorFormat;
			color_attachment_view.components	= { VK_COMPONENT_SWIZZLE_R, VK_COMPONENT_SWIZZLE_G,
													VK_COMPONENT_SWIZZLE_B, VK_COMPONENT_SWIZZLE_A };

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

		VK_CHECK( vkCreateImage( _logicalDevice, &image_info, null, OUT &_depthStencilImage ) );
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
	bool Vk1Device::GetMemoryTypeIndex (uint32_t memoryTypeBits, VkMemoryPropertyFlags flags, OUT uint32_t &index) const
	{
		index = UMax;

		for (uint32_t i = 0; i < _deviceMemoryProperties.memoryTypeCount; ++i)
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
	SetObjectName
=================================================
*/
	bool Vk1Device::SetObjectName (uint64_t id, StringCRef name, EGpuObject::type type) const
	{
		if ( name.Empty() or id == VK_NULL_HANDLE or not _enableDebugMarkers )
			return false;

		VkDebugMarkerObjectNameInfoEXT	info = {};
		info.sType			= VK_STRUCTURE_TYPE_DEBUG_MARKER_OBJECT_NAME_INFO_EXT;
		info.objectType		= Vk1Enum( type );
		info.object			= id;
		info.pObjectName	= name.cstr();

		VK_CALL( vkDebugMarkerSetObjectNameEXT( GetLogicalDevice(), &info ) );
		return true;
	}

/*
=================================================
	_CreateRenderPass
=================================================
*/
	bool Vk1Device::_CreateRenderPass ()
	{
		//CHECK_ERR( not _renderPass );

		if ( _renderPass )
			return true;

		ASSERT( _depthStencilView != VK_NULL_HANDLE ?
					_depthStencilPixelFormat != EPixelFormat::Unknown :
					_depthStencilPixelFormat == EPixelFormat::Unknown );

		ModulePtr	module;
		CHECK_ERR( GlobalSystems()->modulesFactory->Create(
					VkRenderPassModuleID,
					GlobalSystems(),
					CreateInfo::GpuRenderPass{
						null,
						RenderPassDescrBuilder::CreateForSurface( _colorPixelFormat, _depthStencilPixelFormat )
					},
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
		
		Message< GpuMsg::GetVkRenderPassID >	req_id;
		SendTo( _renderPass, req_id );

		FOR( i, frameBuffers )
		{
			auto fb = New< Vk1SystemFramebuffer >( GlobalSystems() );

			SendTo< ModuleMsg::AttachModule >( fb, { _renderPass } );

			CHECK_ERR( fb->CreateFramebuffer( _surfaceSize, uint(i),
											  *(req_id->result),
											  _imageBuffers[i].view, _colorPixelFormat,
											  _depthStencilView, _depthStencilPixelFormat,
											  EImage::Tex2D	// TODO: multisampling
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

		FOR( i, frameBuffers ) {
			SendTo( frameBuffers[i], msg );
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
		uint32_t					count		= 0;
		Array< VkSurfaceFormatKHR >	surf_formats;
		const VkFormat				def_format	= VK_FORMAT_B8G8R8A8_UNORM;
		const VkColorSpaceKHR		def_space	= VK_COLOR_SPACE_SRGB_NONLINEAR_KHR;

		VK_CHECK( vkGetPhysicalDeviceSurfaceFormatsKHR( _physicalDevice, _surface, OUT &count, null ) );
		CHECK_ERR( count > 0 );

		surf_formats.Resize( count );
		VK_CHECK( vkGetPhysicalDeviceSurfaceFormatsKHR( _physicalDevice, _surface, OUT &count, OUT surf_formats.ptr() ) );
		
		if ( count == 1 and
			 surf_formats[0].format == VK_FORMAT_UNDEFINED )
		{
			colorFormat = requiredFormat;
			colorSpace  = surf_formats[0].colorSpace;
		}
		else
		{
			usize	both_match_idx		= UMax;
			usize	format_match_idx	= UMax;
			usize	space_match_idx		= UMax;
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

			if ( both_match_idx != UMax )
				idx = both_match_idx;
			else
			if ( format_match_idx != UMax )
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
		uint32_t	count = 0;

		vkGetPhysicalDeviceQueueFamilyProperties( _physicalDevice, OUT &count, null );
		CHECK_ERR( count > 0 );

		properties.Resize( count );
		vkGetPhysicalDeviceQueueFamilyProperties( _physicalDevice, OUT &count, OUT properties.ptr() );
		return true;
	}

/*
=================================================
	_LoadDeviceExtensions
=================================================
*/
	bool Vk1Device::_LoadDeviceExtensions () const
	{
		CHECK_ERR( HasPhyiscalDevice() );

		if ( not _deviceExtensions.Empty() )
			return true;

		uint32_t	count = 0;
		VK_CALL( vkEnumerateDeviceExtensionProperties( _physicalDevice, null, OUT &count, null ) );

		if ( count == 0 )
			return true;

		_deviceExtensions.Resize( count );
		VK_CHECK( vkEnumerateDeviceExtensionProperties( _physicalDevice, null, OUT &count, OUT _deviceExtensions.ptr() ) );

		return true;
	}
	
/*
=================================================
	HasDeviceExtension
=================================================
*/
	bool Vk1Device::HasDeviceExtension (StringCRef name) const
	{
		_LoadDeviceExtensions();

		FOR( i, _deviceExtensions )
		{
			if ( name.EqualsIC( _deviceExtensions[i].extensionName ) )
				return true;
		}
		return false;
	}

/*
=================================================
	_CheckDeviceExtensions
=================================================
*/
	bool Vk1Device::_CheckDeviceExtensions (INOUT Array<const char*> &extensions) const
	{
		CHECK_ERR( HasPhyiscalDevice() );

		FOR( i, extensions )
		{
			StringCRef	name = extensions[i];
			
			if ( not HasDeviceExtension( name ) )
			{
				LOG( ("Vulkan device extension \""_str << name << "\" not supported and will be removed").cstr(), ELog::Info );

				extensions.Erase( i );
				--i;
			}
		}
		return true;
	}
	
/*
=================================================
	_CheckLayers
=================================================
*/
	bool Vk1Device::_CheckLayers (INOUT Array<const char*> &layers) const
	{
		FOR( i, layers )
		{
			StringCRef	name = layers[i];
			
			if ( not HasLayer( name ) )
			{
				LOG( ("Vulkan layer \""_str << name << "\" not supported and will be removed").cstr(), ELog::Info );

				layers.Erase( i );
				--i;
			}
		}
		return true;
	}
	
/*
=================================================
	_CheckExtensions
=================================================
*/
	bool Vk1Device::_CheckExtensions (INOUT Array<const char*> &extensions) const
	{
		FOR( i, extensions )
		{
			StringCRef	name = extensions[i];
			
			if ( not HasExtension( name ) )
			{
				LOG( ("Vulkan extension \""_str << name << "\" not supported and will be removed").cstr(), ELog::Info );

				extensions.Erase( i );
				--i;
			}
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
		if ( _imageAvailable != VK_NULL_HANDLE and
			 _renderFinished != VK_NULL_HANDLE )
		{
			return true;
		}

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
	_DebugReportCallback
=================================================
*/
	VkBool32 VKAPI_CALL Vk1Device::_DebugReportCallback (VkDebugReportFlagsEXT flags,
														  VkDebugReportObjectTypeEXT objectType,
														  uint64_t object,
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
			return "performance";

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
	ELog::type Vk1Device::_DebugReportFlagsToLogType (VkDebugReportFlagBitsEXT flags)
	{
		if ( EnumEq( flags, VK_DEBUG_REPORT_ERROR_BIT_EXT ) )
			return ELog::Warning;

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
