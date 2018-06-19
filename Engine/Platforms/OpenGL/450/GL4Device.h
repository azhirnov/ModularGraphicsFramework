// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Core/Config/Engine.Config.h"

#ifdef GRAPHICS_API_OPENGL

#include "Engine/Platforms/OpenGL/450/GL4Enums.h"
#include "Engine/Platforms/OpenGL/450/GL4Emulator.h"
#include "Engine/Platforms/Public/GPU/Thread.h"

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
		
		using DeviceProperties_t	= GpuMsg::GetDeviceProperties::Properties;


	// variables
	private:
		uint2						_surfaceSize;

		EPixelFormat::type			_colorPixelFormat;
		EPixelFormat::type			_depthStencilPixelFormat;
		MultiSamples				_samples;

		ModulePtr					_renderPass;
		ModulePtr					_framebuffer;

		uint						_currentImageIndex;
		uint						_swapchainLength;

		uint						_numExtensions;
		bool						_initialized;
		bool						_frameStarted;
		
		DeviceProperties_t			_properties;

		const bool					_vulkanCompatibility;	// if 'true' then opengl will be emulate vulkan bahavior

		mutable uint				_debugReportCounter;


	// methods
	public:
		explicit GL4Device (GlobalSystemsRef gs);
		~GL4Device ();

		bool Initialize (const uint2 &surfaceSize, EPixelFormat::type colorFormat, EPixelFormat::type depthStencilFormat = Uninitialized,
						 MultiSamples samples = Uninitialized);
		bool InitDebugReport ();
		void WriteInfo ();

		void Deinitialize ();
		
		bool BeginFrame ();
		bool EndFrame ();

		bool OnResize (const uint2 &surfaceSize);
		
		bool IsExtensionSupported (StringCRef name);

		bool SetObjectName (gl::GLuint id, StringCRef name, EGpuObject::type type) const;
		bool SetObjectName (void* ptr, StringCRef name, EGpuObject::type type) const;

		ND_ bool		IsDeviceCreated ()					const	{ return _initialized; }
		ND_ bool		IsFrameStarted ()					const	{ return _frameStarted; }

		ND_ ModulePtr	GetDefaultRenderPass ()				const	{ return _renderPass; }
		ND_ ModulePtr	GetCurrentFramebuffer ()			const	{ return _framebuffer; }
		ND_ uint		GetImageIndex ()					const	{ return _currentImageIndex; }
		ND_ uint		GetSwapchainLength ()				const	{ return _swapchainLength; }

		ND_ bool		IsVulkanCompatibility ()			const	{ return _vulkanCompatibility; }
		
		ND_ uint2 const&				GetSurfaceSize ()	const	{ return _surfaceSize; }
		
		ND_ DeviceProperties_t const&	GetProperties ()	const	{ return _properties; }


	private:
		bool _CreateRenderPass ();
		bool _CreateFramebuffer ();
		
		void _UpdateProperties ();

		static void GL4_APIENTRY _StaticDebugCallback (gl::GLenum source, gl::GLenum type, gl::GLuint id,
													   gl::GLenum severity, gl::GLsizei length,
													   const gl::GLchar* message, const void* userParam);
	};


}	// PlatformGL
}	// Engine

#endif	// GRAPHICS_API_OPENGL
