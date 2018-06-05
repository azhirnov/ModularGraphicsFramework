// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'
/*
	Crossplatform helper for Vulkan initialization.
*/

#pragma once

#include "Engine/Config/Engine.Config.h"

#ifdef GRAPHICS_API_VULKAN

#include "Engine/Platforms/Vulkan/110/vulkan1.h"
#include "Engine/Platforms/Vulkan/110/Vk1Enums.h"
#include "Engine/Platforms/Public/GPU/Thread.h"
#include "Engine/STL/Containers/Adaptors.h"

namespace Engine
{
namespace PlatformVK
{

	//
	// Vulkan Device
	//

	class Vk1Device final : public BaseObject
	{
	// types
	public:
		struct DeviceInfo
		{
			vk::VkPhysicalDevice	id						= VK_NULL_HANDLE;
			String					device;
			BytesUL					globalMemory;
			uint					version					= 0;
			uint					maxInvocations			= 0;		// for compute shader
			bool					isGPU					= false;	// may be discrete or integrated GPU
			bool					integratedGPU			= false;
			bool					isCPU					= false;
			bool					supportsTesselation		= false;
			bool					supportsGeometryShader	= false;
		};

		using DeviceProperties_t	= GpuMsg::GetDeviceProperties::Properties;


	private:
		class Vk1SystemFramebuffer;

		struct SwapChainBuffer : CompileTime::FastCopyable
		{
			ModulePtr			module;
			vk::VkImageView		view	= VK_NULL_HANDLE;
		};

		static const uint	MAX_SWAPCHAIN_SIZE = 8;
		
		using SwapChainBuffers_t	= FixedSizeArray< SwapChainBuffer, MAX_SWAPCHAIN_SIZE >;
		using Framebuffers_t		= FixedSizeArray< ModulePtr, MAX_SWAPCHAIN_SIZE >;
		using CommandBuffers_t		= Array< vk::VkCommandBuffer >;
		using _ConstCharPtrBuffer_t	= ArrayCRef< const char *>;

		using ExtensionNames_t		= _ConstCharPtrBuffer_t;
		using ValidationLayers_t	= _ConstCharPtrBuffer_t;


	// variables
	private:
		vk::VkDevice					_logicalDevice;
		vk::VkPhysicalDevice			_physicalDevice;
		vk::VkInstance					_instance;
		
		DeviceProperties_t				_properties;

		vk::VkPhysicalDeviceProperties				_deviceProperties;
		vk::VkPhysicalDeviceFeatures				_deviceFeatures;
		vk::VkPhysicalDeviceMemoryProperties		_deviceMemoryProperties;
		
		mutable Array< vk::VkLayerProperties >		_instanceLayers;
		mutable Array< vk::VkExtensionProperties >	_instanceExtensions;
		mutable Array< vk::VkExtensionProperties >	_deviceExtensions;

		vk::VkSurfaceKHR				_surface;
		vk::VkSwapchainKHR				_swapchain;
		SwapChainBuffers_t				_imageBuffers;
		uint2							_surfaceSize;
		bool							_vsync;
		
		ModulePtr						_depthStencilImage;
		vk::VkImageView					_depthStencilView;

		EPixelFormat::type				_colorPixelFormat;
		EPixelFormat::type				_depthStencilPixelFormat;
		MultiSamples					_samples;

		vk::VkFormat					_colorFormat;
		vk::VkColorSpaceKHR				_colorSpace;
		vk::VkFormat					_depthStencilFormat;

		vk::VkQueue						_queue;
		vk::uint32_t					_queueIndex;
		EQueueFamily::bits				_queueFamily;

		ModulePtr						_renderPass;
		Framebuffers_t					_framebuffers;

		uint							_currentImageIndex;

		vk::VkDebugReportCallbackEXT	_debugCallback;

		bool							_enableDebugMarkers;

		mutable bool					_primiryFunctionsLoaded : 1;
		mutable bool					_isInstanceFunctionsLoaded : 1;
		mutable bool					_isDeviceFunctionsLoaded : 1;

		uint							_debugReportCounter;


	// methods
	public:
		Vk1Device (GlobalSystemsRef gs);
		~Vk1Device ();

		bool CreateInstance (StringCRef applicationName, vk::uint32_t applicationVersion, vk::uint32_t vulkanVersion,
							 ExtensionNames_t ext = ExtensionNames_t(), ValidationLayers_t layers = ValidationLayers_t());
		bool DestroyInstance ();

		bool HasLayer (StringCRef name) const;
		bool HasExtension (StringCRef name) const;
		bool HasDeviceExtension (StringCRef name) const;

		bool CreateDebugCallback (vk::VkDebugReportFlagBitsEXT flags);
		bool DestroyDebugCallback ();

		bool GetPhysicalDeviceInfo (OUT AppendableAdaptor<DeviceInfo> deviceInfo) const;

		bool ChoosePhysicalDevice (StringCRef name = StringCRef());
		bool CreatePhysicalDevice (vk::VkPhysicalDevice id);
		void WritePhysicalDeviceInfo () const;

		bool CreateDevice (const vk::VkPhysicalDeviceFeatures &enabledFeatures, EQueueFamily::bits queueFamilies,
						   ExtensionNames_t ext = ExtensionNames_t());
		bool DestroyDevice ();
		void DeviceWaitIdle ();

		bool CreateSwapchain (const uint2 &size, bool vsync, vk::uint32_t imageArrayLayers = 1,
							  EPixelFormat::type depthStencilFormat = Uninitialized, MultiSamples samples = Uninitialized);
		bool RecreateSwapchain (const uint2 &size);
		bool RecreateSwapchain ();
		bool DestroySwapchain ();

		bool SetSurface (vk::VkSurfaceKHR surface, EPixelFormat::type colorFormat);
		bool DestroySurface ();

		bool CreateQueue ();
		void DestroyQueue ();

		bool BeginFrame (vk::VkSemaphore imageAvailable);
		bool EndFrame (vk::VkSemaphore renderFinished);
		bool IsFrameStarted () const;
		
		bool GetMemoryTypeIndex (vk::uint32_t memoryTypeBits, vk::VkMemoryPropertyFlags flags, OUT vk::uint32_t &index) const;

		bool SetObjectName (vk::uint64_t id, StringCRef name, EGpuObject::type type) const;

		bool IsInstanceCreated ()		const						{ return _instance		 != VK_NULL_HANDLE; }
		bool HasPhyiscalDevice ()		const						{ return _physicalDevice != VK_NULL_HANDLE; }
		bool IsDeviceCreated ()			const						{ return _logicalDevice  != VK_NULL_HANDLE; }
		bool IsSurfaceCreated ()		const						{ return _surface		 != VK_NULL_HANDLE; }
		bool IsSwapchainCreated ()		const						{ return _swapchain		 != VK_NULL_HANDLE; }
		bool IsDebugCallbackCreated ()	const						{ return _debugCallback	 != VK_NULL_HANDLE; }
		bool IsQueueCreated ()			const						{ return _queue			 != VK_NULL_HANDLE; }

		vk::VkInstance			GetInstance ()				const	{ ASSERT( IsInstanceCreated() );  return _instance; }
		vk::VkPhysicalDevice	GetPhyiscalDevice ()		const	{ ASSERT( HasPhyiscalDevice() );  return _physicalDevice; }
		vk::VkDevice			GetLogicalDevice ()			const	{ ASSERT( IsDeviceCreated() );    return _logicalDevice; }
		vk::VkSurfaceKHR		GetSurface ()				const	{ ASSERT( IsSurfaceCreated() );   return _surface; }
		vk::VkSwapchainKHR		GetSwapchain ()				const	{ ASSERT( IsSwapchainCreated() ); return _swapchain; }
		vk::VkFormat			GetColorFormat ()			const	{ return _colorFormat; }
		vk::VkColorSpaceKHR		GetColorSpace ()			const	{ return _colorSpace; }
		vk::VkFormat			GetDepthStencilFormat ()	const	{ return _depthStencilFormat; }

		ModulePtr				GetDefaultRenderPass ()		const	{ return _renderPass; }
		ModulePtr				GetCurrentFramebuffer ()	const	{ return _framebuffers[ _currentImageIndex ]; }
		uint					GetImageIndex ()			const	{ return _currentImageIndex; }
		uint					GetSwapchainLength ()		const	{ return uint(_framebuffers.Count()); }

		uint2 const&			GetSurfaceSize ()			const	{ return _surfaceSize; }

		vk::VkQueue				GetQueue ()					const	{ ASSERT( IsQueueCreated() );  return _queue; }
		vk::uint32_t			GetQueueIndex ()			const	{ return _queueIndex; }
		EQueueFamily::bits		GetQueueFamily ()			const	{ return _queueFamily; }
		
		DeviceProperties_t const&	GetProperties ()		const	{ return _properties; }

		vk::VkPhysicalDeviceProperties const&		GetDeviceProperties ()	const	{ return _deviceProperties; }
		vk::VkPhysicalDeviceFeatures const&			GetDeviceFeatures ()	const	{ return _deviceFeatures; }
		vk::VkPhysicalDeviceMemoryProperties const&	GetMemoryProperties ()	const	{ return _deviceMemoryProperties; }


	private:
		// Instance
		bool _CheckLayers (INOUT Array<const char*> &layers) const;
		bool _CheckExtensions (INOUT Array<const char*> &extensions) const;
		bool _LoadFunctions () const;
		bool _LoadInstanceLayers () const;
		bool _LoadInstanceExtensions () const;

		// Swapchain
		void _GetImageUsage (OUT vk::VkImageUsageFlags &imageUsage) const;
		void _GetSharingMode (OUT vk::VkSharingMode &sharingMode) const;
		void _GetPresentMode (OUT vk::VkPresentModeKHR &presentMode, bool vsync, const vk::VkSurfaceCapabilitiesKHR &surfaceCaps) const;
		void _GetSwapChainExtent (OUT vk::VkExtent2D &extent, const vk::VkSurfaceCapabilitiesKHR &surfaceCaps) const;
		void _GetSurfaceTransform (OUT vk::VkSurfaceTransformFlagBitsKHR &transform, const vk::VkSurfaceCapabilitiesKHR &surfaceCaps) const;
		void _GetSurfaceImageCount (OUT vk::uint32_t &minImageCount, const vk::VkSurfaceCapabilitiesKHR &surfaceCaps) const;
		void _DeleteSwapchain (INOUT vk::VkSwapchainKHR &swapchain);

		bool _CreateRenderPass ();
		void _DeleteRenderPass ();

		bool _CreateColorAttachment (MultiSamples samples);
		bool _CreateDepthStencilAttachment (EPixelFormat::type depthStencilFormat);
		void _DeleteDepthStencilAttachment ();

		bool _CreateFramebuffers ();
		void _DeleteFramebuffers ();

		// Surface
		bool _ChooseColorFormat (OUT vk::VkFormat &colorFormat, OUT vk::VkColorSpaceKHR &colorSpace,
								 vk::VkFormat requiredFormat, vk::VkColorSpaceKHR requiredColorSpace) const;

		// Device
		bool _LoadDeviceExtensions () const;
		bool _CheckDeviceExtensions (INOUT Array<const char*> &extensions) const;
		void _UpdateProperties ();

		// Queue
		bool _ChooseQueueIndex (INOUT EQueueFamily::bits &family, OUT vk::uint32_t &index) const;
		bool _GetQueueFamilyProperties (OUT Array<vk::VkQueueFamilyProperties> &prop) const;
		bool _GetQueueCreateInfos (OUT Array<vk::VkDeviceQueueCreateInfo> &queueCreateInfos,
								   INOUT EQueueFamily::bits &family,
								   OUT vk::uint32_t &queueIndex) const;

		// DebugReport
		static VKAPI_ATTR vk::VkBool32 VKAPI_CALL _DebugReportCallback (vk::VkDebugReportFlagsEXT flags,
																		vk::VkDebugReportObjectTypeEXT objectType,
																		vk::uint64_t object,
																		size_t location,
																		int32_t messageCode,
																		const char* pLayerPrefix,
																		const char* pMessage,
																		void* pUserData);
		
		static ELog::type _DebugReportFlagsToLogType (vk::VkDebugReportFlagBitsEXT flags);
		static StringCRef _DebugReportFlagsToString (vk::VkDebugReportFlagBitsEXT flags);
		static StringCRef _DebugReportObjectTypeToString (vk::VkDebugReportObjectTypeEXT objType);

		static StringCRef _DeviceTypeToString (vk::VkPhysicalDeviceType value);
	};

	
}	// PlatformVK
}	// Engine

#endif	// GRAPHICS_API_VULKAN
