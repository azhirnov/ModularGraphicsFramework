// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'
/*
	Crossplatform helper for Vulkan initialization.

	based on code from https://github.com/SaschaWillems/Vulkan
*/

#pragma once

#include "Engine/Platforms/Vulkan/Impl/vulkan1.h"
#include "Engine/Platforms/Vulkan/Impl/Vk1Enums.h"

#if defined( GRAPHICS_API_VULKAN )

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
		struct EQueueFamily
		{
			enum type : uint
			{
				Graphics = 0,
				Compute,
				Transfer,
				SparseBinding,
				Present,
				_Count
			};

			GX_ENUM_BITFIELD( EQueueFamily );

			static constexpr bits	All = bits().SetAll();
		};

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


	private:
		class Vk1SystemFramebuffer;

		struct SwapChainBuffer : CompileTime::PODStruct
		{
			vk::VkImage		image	= VK_NULL_HANDLE;
			vk::VkImageView	view	= VK_NULL_HANDLE;
		};

		static const uint	MAX_SWAPCHAIN_SIZE = 8;
		
		using SwapChainBuffers_t	= FixedSizeArray< SwapChainBuffer, MAX_SWAPCHAIN_SIZE >;
		using Framebuffers_t		= FixedSizeArray< ModulePtr, MAX_SWAPCHAIN_SIZE >;
		using CommandBuffers_t		= Array< vk::VkCommandBuffer >;
		using _ConstCharPtrBuffer_t	= ArrayCRef< const char *>;

		using ExtensionNames_t		= _ConstCharPtrBuffer_t;
		using ValidationLayers_t	= _ConstCharPtrBuffer_t;

		using uint2					= GX_STL::GXMath::uint2;


	// variables
	private:
		vk::VkDevice					_logicalDevice;
		vk::VkPhysicalDevice			_physicalDevice;
		vk::VkInstance					_instance;

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
		
		vk::VkImage						_depthStencilImage;
		vk::VkDeviceMemory				_depthStencilMemory;
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
		bool							_graphicsQueueSubmited;

		vk::VkSemaphore					_imageAvailable;
		vk::VkSemaphore					_renderFinished;

		vk::VkDebugReportCallbackEXT	_debugCallback;

		bool							_enableDebugMarkers;

		mutable bool					_isInstanceFunctionsLoaded;
		mutable bool					_isDeviceFunctionsLoaded;


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
		bool WritePhysicalDeviceInfo () const;

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

		bool SubmitQueue (ArrayCRef<vk::VkCommandBuffer> cmdBuffers, vk::VkFence fence = VK_NULL_HANDLE,
						  ArrayCRef<vk::VkSemaphore> waitSemaphore = Uninitialized, ArrayCRef<vk::VkPipelineStageFlags> waitStages = Uninitialized,
						  ArrayCRef<vk::VkSemaphore> signalSemaphores = Uninitialized);

		bool BeginFrame ();
		bool EndFrame ();
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

		vk::VkInstance			GetInstance ()				const	{ return _instance; }
		vk::VkPhysicalDevice	GetPhyiscalDevice ()		const	{ return _physicalDevice; }
		vk::VkDevice			GetLogicalDevice ()			const	{ return _logicalDevice; }
		vk::VkSurfaceKHR		GetSurface ()				const	{ return _surface; }
		vk::VkSwapchainKHR		GetSwapchain ()				const	{ return _swapchain; }
		vk::VkFormat			GetColorFormat ()			const	{ return _colorFormat; }
		vk::VkColorSpaceKHR		GetColorSpace ()			const	{ return _colorSpace; }
		vk::VkFormat			GetDepthStencilFormat ()	const	{ return _depthStencilFormat; }

		ModulePtr				GetDefaultRenderPass ()		const	{ return _renderPass; }
		ModulePtr				GetCurrentFramebuffer ()	const	{ return _framebuffers[ _currentImageIndex ]; }
		uint					GetImageIndex ()			const	{ return _currentImageIndex; }
		uint					GetSwapchainLength ()		const	{ return (uint)_framebuffers.Count(); }

		uint2 const&			GetSurfaceSize ()			const	{ return _surfaceSize; }

		vk::VkQueue				GetQueue ()					const	{ return _queue; }
		vk::uint32_t			GetQueueIndex ()			const	{ return _queueIndex; }
		EQueueFamily::bits		GetQueueFamily ()			const	{ return _queueFamily; }
		
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
		void _DeleteSwapchain (INOUT vk::VkSwapchainKHR &swapchain, SwapChainBuffers_t &buffers) const;

		bool _CreateRenderPass ();
		void _DeleteRenderPass ();

		bool _CreateColorAttachment (MultiSamples samples, OUT SwapChainBuffers_t &imageBuffers) const;
		bool _CreateDepthStencilAttachment (EPixelFormat::type depthStencilFormat);
		void _DeleteDepthStencilAttachment ();

		bool _CreateFramebuffers (OUT Framebuffers_t &frameBuffers) const;
		void _DeleteFramebuffers (INOUT Framebuffers_t &frameBuffers) const;

		bool _CreateSemaphores ();
		void _DestroySemaphores ();

		// Surface
		bool _ChooseColorFormat (OUT vk::VkFormat &colorFormat, OUT vk::VkColorSpaceKHR &colorSpace,
								 vk::VkFormat requiredFormat, vk::VkColorSpaceKHR requiredColorSpace) const;

		// Device
		bool _LoadDeviceExtensions () const;
		bool _CheckDeviceExtensions (INOUT Array<const char*> &extensions) const;

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
