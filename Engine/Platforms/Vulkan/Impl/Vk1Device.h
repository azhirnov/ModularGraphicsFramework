// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'
/*
	Crossplatform helper for Vulkan initialization.

	based on code from https://github.com/SaschaWillems/Vulkan
*/

#pragma once

#include "vulkan1.h"

#if defined( GRAPHICS_API_VULKAN )

#include "Engine/Platforms/Vulkan/Impl/Vk1Enums.h"

namespace Engine
{
namespace Platforms
{

	//
	// Vulkan Wrapper
	//

	struct Vk1Device
	{
	// types
	public:

		// TODO: separate compute queue and graphics & compute & present queue,
		//		 may be it is not needed, check docs...
		/*struct EQueueFamily
		{
			enum type : uint
			{
				Graphics = 0,
				Compute,
				Transfer,
				Present,
				_Count
			};
		};

		struct */

		struct QueueFamilyIndices
		{
		// variables
			vk::uint32_t	graphics;
			vk::uint32_t	compute;
			vk::uint32_t	transfer;
			vk::uint32_t	present;

		// methods
			QueueFamilyIndices () :
				graphics( -1 ),
				compute( -1 ),
				transfer( -1 ),
				present( -1 )
			{}
		};


	private:
		struct SwapChainBuffer
		{
		// variables
			vk::VkImage		image;
			vk::VkImageView	view;
			
		// methods
			SwapChainBuffer () : image(0), view(0)
			{}
		};
		
		using SwapChainBuffers_t	= Array< SwapChainBuffer >;
		using _ConstCharPtrBuffer_t	= ArrayRef< const char *>;
		using Framebuffers_t		= Array< vk::VkFramebuffer >;
		using CommandBuffers_t		= Array< vk::VkCommandBuffer >;

		using ExtensionNames_t		= _ConstCharPtrBuffer_t;
		using ValidationLayers_t	= _ConstCharPtrBuffer_t;

		using uint2					= GX_STL::GXMath::uint2;


	// variables
	private:
		vk::VkPhysicalDeviceProperties			_deviceProperties;
		vk::VkPhysicalDeviceFeatures			_deviceFeatures;
		vk::VkPhysicalDeviceMemoryProperties	_deviceMemoryProperties;

		vk::VkInstance					_instance;
		vk::VkPhysicalDevice			_physicalDevice;
		vk::VkDevice					_logicalDevice;

		vk::VkSurfaceKHR				_surface;
		vk::VkSwapchainKHR				_swapchain;
		SwapChainBuffers_t				_imageBuffers;
		uint2							_surfaceSize;
		bool							_vsync;
		
		vk::VkImage						_depthStencilImage;
		vk::VkDeviceMemory				_depthStencilMemory;
		vk::VkImageView					_depthStencilView;

		vk::VkFormat					_colorFormat;
		vk::VkColorSpaceKHR				_colorSpace;
		vk::VkFormat					_depthStencilFormat;

		QueueFamilyIndices				_queueFamilyIndices;

		vk::VkCommandPool				_commandPool;
		CommandBuffers_t				_commandBuffers;

		vk::VkRenderPass				_renderPass;
		Framebuffers_t					_framebuffers;

		vk::VkSemaphore					_imageAvailable;
		vk::VkSemaphore					_renderFinished;

		vk::VkDebugReportCallbackEXT	_debugCallback;

		bool							_enableDebugMarkers;


	// methods
	public:
		Vk1Device ();
		~Vk1Device ();

		bool CreateInstance (StringCRef applicationName, vk::uint32_t applicationVersion, vk::uint32_t vulkanVersion,
							 ExtensionNames_t ext = ExtensionNames_t(), ValidationLayers_t layers = ValidationLayers_t());
		bool DestroyInstance ();

		bool CreateDebugCallback (vk::VkDebugReportFlagBitsEXT flags);
		bool DestroyDebugCallback ();

		bool ChoosePhysicalDevice (StringCRef name = StringCRef());
		bool WritePhysicalDeviceInfo () const;

		bool CreateDevice (const vk::VkPhysicalDeviceFeatures &enabledFeatures, vk::VkQueueFlags requestedQueueTypes,
						   bool useSwapchain, ExtensionNames_t ext = ExtensionNames_t());
		bool DestroyDevice ();
		void DeviceWaitIdle ();

		bool CreateSwapchain (const uint2 &size, bool vsync, vk::uint32_t imageArrayLayers = 1,
							  vk::VkFormat depthStencilFormat = vk::VK_FORMAT_UNDEFINED);
		bool RecreateSwapchain (const uint2 &size);
		bool RecreateSwapchain ();
		bool DestroySwapchain ();

		bool SetSurface (vk::VkSurfaceKHR surface, vk::VkFormat colorFormat, vk::VkColorSpaceKHR colorSpace);
		bool DestroySurface ();

		bool DrawFrame ();
		
		bool GetMemoryTypeIndex (vk::uint32_t memoryTypeBits, vk::VkMemoryPropertyFlags flags, OUT vk::uint32_t &index) const;

		bool IsInstanceCreated ()		const						{ return _instance		 != VK_NULL_HANDLE; }
		bool HasPhyiscalDevice ()		const						{ return _physicalDevice != VK_NULL_HANDLE; }
		bool IsDeviceCreated ()			const						{ return _logicalDevice  != VK_NULL_HANDLE; }
		bool IsSurfaceCreated ()		const						{ return _surface		 != VK_NULL_HANDLE; }
		bool IsSwapchainCreated ()		const						{ return _swapchain		 != VK_NULL_HANDLE; }
		bool IsDebugCallbackCreated ()	const						{ return _debugCallback	 != VK_NULL_HANDLE; }

		vk::VkInstance			GetInstance ()				const	{ return _instance; }
		vk::VkPhysicalDevice	GetPhyiscalDevice ()		const	{ return _physicalDevice; }
		vk::VkDevice			GetDevice ()				const	{ return _logicalDevice; }
		vk::VkSurfaceKHR		GetSurface ()				const	{ return _surface; }
		vk::VkSwapchainKHR		GetSwapchain ()				const	{ return _swapchain; }
		vk::VkFormat			GetColorFormat ()			const	{ return _colorFormat; }
		vk::VkColorSpaceKHR		GetColorSpace ()			const	{ return _colorSpace; }
		vk::VkFormat			GetDepthStencilFormat ()	const	{ return _depthStencilFormat; }
		vk::VkRenderPass		GetDefaultRenderPass ()		const	{ return _renderPass; }
		vk::VkCommandPool		GetCommandPool ()			const	{ return _commandPool; }
		QueueFamilyIndices		GetQueueFamilyIndices ()	const	{ return _queueFamilyIndices; }
		//vk::VkDebugReportCallbackEXT	GetDebugCallback ()	const	{ return _debugCallback; }
		uint2 const&			GetSurfaceSize ()			const	{ return _surfaceSize; }

		vk::VkQueue				GetGraphicsQueue ()			const;
		vk::VkQueue				GetComputeQueue ()			const;
		vk::VkQueue				GetTransferQueue ()			const;
		vk::VkQueue				GetPresentQueue ()			const;
		
		ArrayCRef< vk::VkCommandBuffer >			GetCommandBuffers ()	const	{ return _commandBuffers; }
		ArrayCRef< vk::VkFramebuffer >				GetFramebuffers ()		const	{ return _framebuffers; }
		vk::VkPhysicalDeviceProperties const&		GetDeviceProperties ()	const	{ return _deviceProperties; }
		vk::VkPhysicalDeviceFeatures const&			GetDeviceFeatures ()	const	{ return _deviceFeatures; }
		vk::VkPhysicalDeviceMemoryProperties const&	GetMemoryProperties ()	const	{ return _deviceMemoryProperties; }


	private:
		// Instance
		bool _CheckLayers (ValidationLayers_t layers) const;


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

		bool _CreateColorAttachment (OUT SwapChainBuffers_t &imageBuffers) const;
		bool _CreateDepthStencilAttachment (vk::VkFormat depthStencilFormat);
		void _DeleteDepthStencilAttachment ();

		bool _CreateFramebuffers (OUT Framebuffers_t &frameBuffers) const;
		void _DeleteFramebuffers (INOUT Framebuffers_t &frameBuffers) const;

		bool _CreateSemaphores ();
		void _DestroySemaphores ();

		bool _CreateCommandPool ();
		bool _CreateCommandBuffers ();
		void _DeleteCommandBuffers ();


		// Surface
		bool _ChooseColorFormat (OUT vk::VkFormat &colorFormat, OUT vk::VkColorSpaceKHR &colorSpace,
								 vk::VkFormat requiredFormat, vk::VkColorSpaceKHR requiredColorSpace) const;

		// Device
		bool _GetDeviceExtensions (OUT Array<String> &ext) const;

		bool _GetQueueFamilyProperties (OUT Array<vk::VkQueueFamilyProperties> &prop) const;
		bool _GetQueueFamilyIndex (OUT vk::uint32_t &index, vk::VkQueueFlags queueFlags, const Array<vk::VkQueueFamilyProperties> &props) const;

		bool _ChooseGraphicsQueueFamilyIndex (OUT vk::uint32_t &index, const Array<vk::VkQueueFamilyProperties> &props) const;
		bool _ChooseComputeQueueFamilyIndex (OUT vk::uint32_t &index, const Array<vk::VkQueueFamilyProperties> &props) const;
		bool _ChooseTransferQueueFamilyIndex (OUT vk::uint32_t &index, const Array<vk::VkQueueFamilyProperties> &props) const;
		bool _ChooseGraphicsAndPresentQueueFamilyIndex (OUT vk::uint32_t &graphicsIndex, OUT vk::uint32_t &presentIndex,
														const Array<vk::VkQueueFamilyProperties> &props) const;

		bool _GetQueueCreateInfos (OUT Array<vk::VkDeviceQueueCreateInfo> &queueCreateInfos,
								   OUT QueueFamilyIndices &queueFamilyIndices,
								   vk::VkQueueFlags queueTypes, bool requirePresent) const;

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
	};

	
}	// Platforms
}	// Engine

#endif	// GRAPHICS_API_VULKAN
