// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/Platforms/OpenGL/Impl/GL4Enums.h"
#include "Engine/Platforms/OpenGL/Impl/GL4Emulator.h"

#if defined( GRAPHICS_API_OPENGL )

namespace Engine
{
namespace PlatformGL
{

	//
	// OpenGL 4.x Device
	//

	class GL4Device final : public BaseObject
	{
	// types
	private:
		class GL4SystemFramebuffer;
		class GL4FlippedSystemFramebuffer;


	// variables
	private:
		uint2						_surfaceSize;

		EPixelFormat::type			_colorPixelFormat;
		EPixelFormat::type			_depthStencilPixelFormat;

		ModulePtr					_renderPass;
		ModulePtr					_framebuffer;
		
		EPixelFormat::type			_colorFormat;
		EPixelFormat::type			_depthStencilFormat;
		MultiSamples				_samples;

		uint						_currentImageIndex;
		uint						_swapchainLength;

		uint						_numExtensions;
		bool						_initialized;
		bool						_frameStarted;

		const bool					_vulkanCompatibility;	// if 'true' then opengl will be emulate vulkan bahavior


	// methods
	public:
		GL4Device (GlobalSystemsRef gs);
		~GL4Device ();

		bool Initialize (const uint2 &surfaceSize, EPixelFormat::type colorFormat, EPixelFormat::type depthStencilFormat = Uninitialized,
						 MultiSamples samples = Uninitialized);
		bool InitDebugReport ();

		void Deinitialize ();
		
		bool BeginFrame ();
		bool EndFrame ();

		bool OnResize (const uint2 &surfaceSize);
		
		bool IsExtensionSupported (StringCRef name);

		bool SetObjectName (gl::GLuint id, StringCRef name, EGpuObject::type type) const;
		bool SetObjectName (void* ptr, StringCRef name, EGpuObject::type type) const;

		bool		IsDeviceCreated ()			const	{ return _initialized; }
		bool		IsFrameStarted ()			const	{ return _frameStarted; }

		ModulePtr	GetDefaultRenderPass ()		const	{ return _renderPass; }
		ModulePtr	GetCurrentFramebuffer ()	const	{ return _framebuffer; }
		uint		GetImageIndex ()			const	{ return _currentImageIndex; }
		uint		GetSwapchainLength ()		const	{ return _swapchainLength; }

		bool		IsVulkanCompatibility ()	const	{ return _vulkanCompatibility; }

	private:
		bool _CreateRenderPass ();
		bool _CreateFramebuffer ();

		static void GL4_APIENTRY _StaticDebugCallback (gl::GLenum source, gl::GLenum type, gl::GLuint id,
													   gl::GLenum severity, gl::GLsizei length,
													   const gl::GLchar* message, const void* userParam);
	};


}	// PlatformGL
}	// Engine

#endif	// GRAPHICS_API_OPENGL
