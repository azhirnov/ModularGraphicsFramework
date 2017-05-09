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
namespace PlatformVK
{

	//
	// Vulkan Device
	//

	class Vk1Device final : public BaseObject
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
			vk::uint32_t	graphics	= -1;
			vk::uint32_t	compute		= -1;
			vk::uint32_t	transfer	= -1;
			vk::uint32_t	present		= -1;
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
		using _ConstCharPtrBuffer_t	= ArrayRef< const char *>;

		using ExtensionNames_t		= _ConstCharPtrBuffer_t;
		using ValidationLayers_t	= _ConstCharPtrBuffer_t;

		using uint2					= GX_STL::GXMath::uint2;


	// variables
	private:
		const VkSystemsRef						_vkSystems;

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

		EPixelFormat::type				_colorPixelFormat;
		EPixelFormat::type				_depthStencilPixelFormat;

		vk::VkFormat					_colorFormat;
		vk::VkColorSpaceKHR				_colorSpace;
		vk::VkFormat					_depthStencilFormat;

		QueueFamilyIndices				_queueFamilyIndices;

		ModulePtr						_commandBuilder;
		CommandBuffers_t				_tempCmdBuffers;

		ModulePtr						_renderPass;
		Framebuffers_t					_framebuffers;

		uint							_currentImageIndex;
		bool							_graphicsQueueSubmited;

		vk::VkSemaphore					_imageAvailable;
		vk::VkSemaphore					_renderFinished;

		vk::VkDebugReportCallbackEXT	_debugCallback;

		bool							_enableDebugMarkers;


	// methods
	public:
		Vk1Device (const GlobalSystemsRef gs, const VkSystemsRef vkSys);
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
							  EPixelFormat::type depthStencilFormat = Uninitialized);
		bool RecreateSwapchain (const uint2 &size);
		bool RecreateSwapchain ();
		bool DestroySwapchain ();

		bool SetSurface (vk::VkSurfaceKHR surface, EPixelFormat::type colorFormat);
		bool DestroySurface ();

		bool BeginFrame ();
		bool EndFrame ();
		//bool EndFrame (const ModulePtr &framebuffer);
		bool IsFrameStarted () const;

		bool SubmitGraphicsQueue (ArrayCRef< ModulePtr > cmdBuffers);
		bool SubmitComputeQueue (ArrayCRef< ModulePtr > cmdBuffers);

		//bool DrawFrame ();
		
		bool GetMemoryTypeIndex (vk::uint32_t memoryTypeBits, vk::VkMemoryPropertyFlags flags, OUT vk::uint32_t &index) const;

		bool IsInstanceCreated ()		const						{ return _instance		 != VK_NULL_HANDLE; }
		bool HasPhyiscalDevice ()		const						{ return _physicalDevice != VK_NULL_HANDLE; }
		bool IsDeviceCreated ()			const						{ return _logicalDevice  != VK_NULL_HANDLE; }
		bool IsSurfaceCreated ()		const						{ return _surface		 != VK_NULL_HANDLE; }
		bool IsSwapchainCreated ()		const						{ return _swapchain		 != VK_NULL_HANDLE; }
		bool IsDebugCallbackCreated ()	const						{ return _debugCallback	 != VK_NULL_HANDLE; }
		
		VkSystemsRef			VkSystems ()				const	{ return _vkSystems; }

		vk::VkInstance			GetInstance ()				const	{ return _instance; }
		vk::VkPhysicalDevice	GetPhyiscalDevice ()		const	{ return _physicalDevice; }
		vk::VkDevice			GetLogicalDevice ()			const	{ return _logicalDevice; }
		vk::VkSurfaceKHR		GetSurface ()				const	{ return _surface; }
		vk::VkSwapchainKHR		GetSwapchain ()				const	{ return _swapchain; }
		vk::VkFormat			GetColorFormat ()			const	{ return _colorFormat; }
		vk::VkColorSpaceKHR		GetColorSpace ()			const	{ return _colorSpace; }
		vk::VkFormat			GetDepthStencilFormat ()	const	{ return _depthStencilFormat; }

		ModulePtr				GetDefaultRenderPass ()		const	{ return _renderPass; }
		ModulePtr				GetCommandBuilder ()		const	{ return _commandBuilder; }
		ModulePtr				GetCurrentFramebuffer ()	const	{ return _framebuffers[ _currentImageIndex ]; }

		QueueFamilyIndices		GetQueueFamilyIndices ()	const	{ return _queueFamilyIndices; }
		uint2 const&			GetSurfaceSize ()			const	{ return _surfaceSize; }

		vk::VkQueue				GetGraphicsQueue ()			const;
		vk::VkQueue				GetComputeQueue ()			const;
		vk::VkQueue				GetTransferQueue ()			const;
		vk::VkQueue				GetPresentQueue ()			const;
		
		//ArrayCRef< ModulePtr >						GetCommandBuffers ()	const	{ return _commandBuffers; }
		//ArrayCRef< ModulePtr >						GetFramebuffers ()		const	{ return _framebuffers; }
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
		bool _CreateDepthStencilAttachment (EPixelFormat::type depthStencilFormat);
		void _DeleteDepthStencilAttachment ();

		bool _CreateFramebuffers (OUT Framebuffers_t &frameBuffers) const;
		void _DeleteFramebuffers (INOUT Framebuffers_t &frameBuffers) const;

		bool _CreateSemaphores ();
		void _DestroySemaphores ();

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

	
}	// PlatformVK
}	// Engine

#endif	// GRAPHICS_API_VULKAN
