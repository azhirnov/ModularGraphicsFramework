// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Engine/Platforms/Public/Common.h"
#include "Engine/Platforms/Public/GPU/Thread.h"
#include "Engine/Platforms/Public/GPU/Context.h"
#include "Engine/Platforms/Public/GPU/VR.h"
#include "Engine/Platforms/Public/OS/Window.h"
#include "Engine/Platforms/Public/OS/Platform.h"
#include "Engine/Platforms/Public/GPU/Buffer.h"
#include "Engine/Platforms/Public/GPU/CommandBuffer.h"
#include "Engine/Platforms/Public/GPU/Framebuffer.h"
#include "Engine/Platforms/Public/GPU/Image.h"
#include "Engine/Platforms/Public/GPU/RenderPass.h"
#include "Engine/Platforms/Public/GPU/Sampler.h"
#include "Engine/Platforms/Public/GPU/Pipeline.h"

#ifdef PLATFORM_WINDOWS
#	include "Engine/Platforms/Windows/WinObjectsConstructor.h"
#endif

#ifdef PLATFORM_SDL
#	include "Engine/Platforms/SDL/SDLObjectsConstructor.h"
#endif

#ifdef GRAPHICS_API_OPENGL
#	include "Engine/Platforms/OpenGL/OpenGLObjectsConstructor.h"
#endif

#ifdef GRAPHICS_API_VULKAN
#	include "Engine/Platforms/Vulkan/VulkanObjectsConstructor.h"
#endif

#ifdef COMPUTE_API_OPENCL
#	include "Engine/Platforms/OpenCL/OpenCLObjectsConstructor.h"
#endif

#ifdef GRAPHICS_API_DIRECTX
#	include "Engine/Platforms/DirectX/DirectXObjectsConstructor.h"
#endif

#ifdef GRAPHICS_API_SOFT
#	include "Engine/Platforms/Soft/SoftRendererObjectsConstructor.h"
#endif

#if defined(PLATFORM_OCULUS_VR) || defined(GX_EMULATOR_VR)
#	include "Engine/Platforms/VR/VRObjectsConstructor.h"
#endif

#include "Engine/Platforms/Input/InputManager.h"
#include "Engine/Platforms/Public/Tools/GPUThreadHelper.h"

namespace Engine
{
namespace Platforms
{

	static ModulePtr CreateDefaultPlatform (ModuleMsg::UntypedID_t, GlobalSystemsRef, const CreateInfo::Platform &);
	static ModulePtr CreateDefaultWindow (ModuleMsg::UntypedID_t, GlobalSystemsRef, const CreateInfo::Window &);
	static ModulePtr CreateDefaultGpuContext (ModuleMsg::UntypedID_t, GlobalSystemsRef, const CreateInfo::GpuContext &);
	static ModulePtr CreateDefaultGpuThread (ModuleMsg::UntypedID_t, GlobalSystemsRef, const CreateInfo::GpuThread &);
	static ModulePtr CreateDefaultVRThread (ModuleMsg::UntypedID_t, GlobalSystemsRef, const CreateInfo::VRThread &);
	
	static ModulePtr CreateDefaultBuffer (ModuleMsg::UntypedID_t, GlobalSystemsRef, const CreateInfo::GpuBuffer &);
	static ModulePtr CreateDefaultCommandBuffer (ModuleMsg::UntypedID_t, GlobalSystemsRef, const CreateInfo::GpuCommandBuffer &);
	static ModulePtr CreateDefaultCommandBuilder (ModuleMsg::UntypedID_t, GlobalSystemsRef, const CreateInfo::GpuCommandBuilder &);
	static ModulePtr CreateDefaultFramebuffer (ModuleMsg::UntypedID_t, GlobalSystemsRef, const CreateInfo::GpuFramebuffer &);
	static ModulePtr CreateDefaultImage (ModuleMsg::UntypedID_t, GlobalSystemsRef, const CreateInfo::GpuImage &);
	static ModulePtr CreateDefaultRenderPass (ModuleMsg::UntypedID_t, GlobalSystemsRef, const CreateInfo::GpuRenderPass &);
	static ModulePtr CreateDefaultSampler (ModuleMsg::UntypedID_t, GlobalSystemsRef, const CreateInfo::GpuSampler &);
	static ModulePtr CreateDefaultGraphicsPipeline (ModuleMsg::UntypedID_t, GlobalSystemsRef, const CreateInfo::GraphicsPipeline &);
	static ModulePtr CreateDefaultComputePipeline (ModuleMsg::UntypedID_t, GlobalSystemsRef, const CreateInfo::ComputePipeline &);

/*
=================================================
	RegisterPlatforms
=================================================
*/
	void RegisterPlatforms ()
	{
		#ifdef PLATFORM_WINDOWS
			WinObjectsConstructor::Register();
		#endif

		#ifdef PLATFORM_SDL
			SDLObjectsConstructor::Register();
		#endif

		InputManager::Register();
		
		#ifdef GRAPHICS_API_OPENGL
			OpenGLObjectsConstructor::Register();
		#endif

		#ifdef GRAPHICS_API_VULKAN
			VulkanObjectsConstructor::Register();
		#endif

		#ifdef COMPUTE_API_OPENCL
			OpenCLObjectsConstructor::Register();
		#endif

		#ifdef GRAPHICS_API_DIRECTX
			DirectXObjectsConstructor::Register();
		#endif
			
		#ifdef GRAPHICS_API_SOFT
			SoftRendererObjectsConstructor::Register();
		#endif
			
		auto	factory = GetMainSystemInstance()->GlobalSystems()->modulesFactory;
		
		#if defined(PLATFORM_OCULUS_VR) || defined(GX_EMULATOR_VR)
			VRObjectsConstructor::Register();
		#endif

		factory->Register( 0, &CreateDefaultPlatform );
		factory->Register( 0, &CreateDefaultWindow );
		factory->Register( 0, &CreateDefaultGpuContext );
		factory->Register( 0, &CreateDefaultGpuThread );
		factory->Register( 0, &CreateDefaultVRThread );
		
		factory->Register( 0, &CreateDefaultBuffer );
		factory->Register( 0, &CreateDefaultCommandBuffer );
		factory->Register( 0, &CreateDefaultCommandBuilder );
		factory->Register( 0, &CreateDefaultFramebuffer );
		factory->Register( 0, &CreateDefaultImage );
		factory->Register( 0, &CreateDefaultRenderPass );
		factory->Register( 0, &CreateDefaultSampler );
		factory->Register( 0, &CreateDefaultGraphicsPipeline );
		factory->Register( 0, &CreateDefaultComputePipeline );
	}
	
/*
=================================================
	UnregisterPlatforms
=================================================
*/
	void UnregisterPlatforms ()
	{
		#ifdef PLATFORM_WINDOWS
			WinObjectsConstructor::Unregister();
		#endif
			
		#ifdef PLATFORM_SDL
			SDLObjectsConstructor::Unregister();
		#endif

		InputManager::Unregister();
		
		#ifdef GRAPHICS_API_OPENGL
			OpenGLObjectsConstructor::Unregister();
		#endif

		#ifdef GRAPHICS_API_VULKAN
			VulkanObjectsConstructor::Unregister();
		#endif

		#ifdef COMPUTE_API_OPENCL
			OpenCLObjectsConstructor::Unregister();
		#endif

		#ifdef GRAPHICS_API_DIRECTX
			DirectXObjectsConstructor::Unregister();
		#endif
			
		#ifdef GRAPHICS_API_SOFT
			SoftRendererObjectsConstructor::Unregister();
		#endif
		
		#if defined(PLATFORM_OCULUS_VR) || defined(GX_EMULATOR_VR)
			VRObjectsConstructor::Unregister();
		#endif
	}
	
/*
=================================================
	CreateDefaultPlatform
=================================================
*/
	static ModulePtr CreateDefaultPlatform (ModuleMsg::UntypedID_t, GlobalSystemsRef gs, const CreateInfo::Platform &ci)
	{
		auto	factory = gs->modulesFactory;

		Array< ModuleMsg::UntypedID_t >	ids;
		factory->Search< decltype(ci) >( "", OUT ids );
		
		FOR( i, ids ) {
			if ( ids[i] != GModID::type(0) ) {
				ModulePtr	mod;
				CHECK_ERR( factory->Create( ids[i], gs, ci, OUT mod ) );
				return mod;
			}
		}
		return null;
	}
	
/*
=================================================
	CreateDefaultWindow
=================================================
*/
	static ModulePtr CreateDefaultWindow (ModuleMsg::UntypedID_t, GlobalSystemsRef gs, const CreateInfo::Window &ci)
	{
		ModulePtr	platform = gs->mainSystem->GetModuleByMsg< CompileTime::TypeListFrom< Message<OSMsg::GetOSModules> > >();

		if ( not platform )
			return null;

		Message<OSMsg::GetOSModules>	req_ids;
		CHECK( platform->Send( req_ids ) );

		ModulePtr	window;
		CHECK_ERR( gs->modulesFactory->Create( req_ids->result->window, gs, ci, OUT window ) );

		return window;
	}
	
/*
=================================================
	CreateDefaultGpuContext
=================================================
*/
	static ModulePtr CreateDefaultGpuContext (ModuleMsg::UntypedID_t, GlobalSystemsRef gs, const CreateInfo::GpuContext &ci)
	{
		auto	factory = gs->modulesFactory;
		auto	api		= GAPI::ToString( ci.settings.version );
		usize	pos;

		if ( api.Find( ' ', OUT pos ) and pos > 0 )
		{
			api[pos] = '.';
			api[pos+1] = '\0';
		}
		else
			api[0] = '\0';

		Array< ModuleMsg::UntypedID_t >	ids;
		factory->Search< decltype(ci) >( api.cstr(), OUT ids );

		FOR( i, ids ) {
			if ( ids[i] != GModID::type(0) ) {
				ModulePtr	mod;
				CHECK_ERR( factory->Create( ids[i], gs, ci, OUT mod ) );
				return mod;
			}
		}
		return null;
	}
	
/*
=================================================
	CreateDefaultGpuThread
=================================================
*/
	static ModulePtr CreateDefaultGpuThread (ModuleMsg::UntypedID_t, GlobalSystemsRef gs, const CreateInfo::GpuThread &ci)
	{
		auto		ctx		= gs->mainSystem->GetModuleByMsg< CompileTime::TypeListFrom< Message<GpuMsg::GetGraphicsModules> > >();
		auto		factory	= gs->modulesFactory;
		ModulePtr	mod;

		if ( not ctx )
			return null;

		Message< GpuMsg::GetGraphicsModules >	req_ids;
		CHECK( ctx->Send( req_ids ) );
		CHECK_ERR( factory->Create( req_ids->graphics->thread, gs, ci, OUT mod ) );
		return mod;
	}
	
/*
=================================================
	CreateDefaultGpuThread
=================================================
*/
	static ModulePtr CreateDefaultVRThread (ModuleMsg::UntypedID_t, GlobalSystemsRef gs, const CreateInfo::VRThread &ci)
	{
		auto	factory	= gs->modulesFactory;
		
		Array< ModuleMsg::UntypedID_t >	ids;
		factory->Search< decltype(ci) >( "", OUT ids );

		FOR( i, ids ) {
			if ( ids[i] != TModID::type(0) ) {
				ModulePtr	mod;
				CHECK_ERR( factory->Create( ids[i], gs, ci, OUT mod ) );
				return mod;
			}
		}
		return null;
	}
	
/*
=================================================
	CreateDefaultBuffer
=================================================
*/
	template <typename Callback>
	static ModulePtr CreateGraphicsObject (GlobalSystemsRef gs, ModulePtr gpuThread, Callback &&cb)
	{
		if ( not gpuThread )
			gpuThread = PlatformTools::GPUThreadHelper::FindComputeThread( gs );
		
		if ( not gpuThread )
			gpuThread = PlatformTools::GPUThreadHelper::FindGraphicsThread( gs );

		CHECK_ERR( gpuThread );

		Message< GpuMsg::GetGraphicsModules >	req_ids;
		gpuThread->Send( req_ids );

		CHECK_ERR( req_ids->compute or req_ids->graphics );

		return cb( *req_ids );
	}

/*
=================================================
	CreateDefaultBuffer
=================================================
*/
	static ModulePtr CreateDefaultBuffer (ModuleMsg::UntypedID_t, GlobalSystemsRef gs, const CreateInfo::GpuBuffer &ci)
	{
		return CreateGraphicsObject( gs, ci.gpuThread,
					LAMBDA( &gs, &ci ) (auto &ids)
					{
						ModulePtr mod;
						gs->modulesFactory->Create( ids.compute->buffer, gs, ci, OUT mod );
						return mod;
					});
	}
	
/*
=================================================
	CreateDefaultCommandBuffer
=================================================
*/
	static ModulePtr CreateDefaultCommandBuffer (ModuleMsg::UntypedID_t, GlobalSystemsRef gs, const CreateInfo::GpuCommandBuffer &ci)
	{
		return CreateGraphicsObject( gs, ci.gpuThread,
					LAMBDA( &gs, &ci ) (auto &ids)
					{
						ModulePtr mod;
						gs->modulesFactory->Create( ids.compute->commandBuffer, gs, ci, OUT mod );
						return mod;
					});
	}
	
/*
=================================================
	CreateDefaultCommandBuilder
=================================================
*/
	static ModulePtr CreateDefaultCommandBuilder (ModuleMsg::UntypedID_t, GlobalSystemsRef gs, const CreateInfo::GpuCommandBuilder &ci)
	{
		return CreateGraphicsObject( gs, ci.gpuThread,
					LAMBDA( &gs, &ci ) (auto &ids)
					{
						ModulePtr mod;
						gs->modulesFactory->Create( ids.compute->commandBuilder, gs, ci, OUT mod );
						return mod;
					});
	}
	
/*
=================================================
	CreateDefaultFramebuffer
=================================================
*/
	static ModulePtr CreateDefaultFramebuffer (ModuleMsg::UntypedID_t, GlobalSystemsRef gs, const CreateInfo::GpuFramebuffer &ci)
	{
		return CreateGraphicsObject( gs, ci.gpuThread,
					LAMBDA( &gs, &ci ) (auto &ids)
					{
						ModulePtr mod;
						gs->modulesFactory->Create( ids.graphics->framebuffer, gs, ci, OUT mod );
						return mod;
					});
	}
	
/*
=================================================
	CreateDefaultImage
=================================================
*/
	static ModulePtr CreateDefaultImage (ModuleMsg::UntypedID_t, GlobalSystemsRef gs, const CreateInfo::GpuImage &ci)
	{
		return CreateGraphicsObject( gs, ci.gpuThread,
					LAMBDA( &gs, &ci ) (auto &ids)
					{
						ModulePtr mod;
						gs->modulesFactory->Create( ids.compute->image, gs, ci, OUT mod );
						return mod;
					});
	}
	
/*
=================================================
	CreateDefaultRenderPass
=================================================
*/
	static ModulePtr CreateDefaultRenderPass (ModuleMsg::UntypedID_t, GlobalSystemsRef gs, const CreateInfo::GpuRenderPass &ci)
	{
		return CreateGraphicsObject( gs, ci.gpuThread,
					LAMBDA( &gs, &ci ) (auto &ids)
					{
						ModulePtr mod;
						gs->modulesFactory->Create( ids.graphics->renderPass, gs, ci, OUT mod );
						return mod;
					});
	}
	
/*
=================================================
	CreateDefaultSampler
=================================================
*/
	static ModulePtr CreateDefaultSampler (ModuleMsg::UntypedID_t, GlobalSystemsRef gs, const CreateInfo::GpuSampler &ci)
	{
		return CreateGraphicsObject( gs, ci.gpuThread,
					LAMBDA( &gs, &ci ) (auto &ids)
					{
						ModulePtr mod;
						gs->modulesFactory->Create( ids.graphics->sampler, gs, ci, OUT mod );
						return mod;
					});
	}
	
/*
=================================================
	CreateDefaultGraphicsPipeline
=================================================
*/
	static ModulePtr CreateDefaultGraphicsPipeline (ModuleMsg::UntypedID_t, GlobalSystemsRef gs, const CreateInfo::GraphicsPipeline &ci)
	{
		return CreateGraphicsObject( gs, ci.gpuThread,
					LAMBDA( &gs, &ci ) (auto &ids)
					{
						ModulePtr mod;
						gs->modulesFactory->Create( ids.graphics->pipeline, gs, ci, OUT mod );
						return mod;
					});
	}
	
/*
=================================================
	CreateDefaultComputePipeline
=================================================
*/
	static ModulePtr CreateDefaultComputePipeline (ModuleMsg::UntypedID_t, GlobalSystemsRef gs, const CreateInfo::ComputePipeline &ci)
	{
		return CreateGraphicsObject( gs, ci.gpuThread,
					LAMBDA( &gs, &ci ) (auto &ids)
					{
						ModulePtr mod;
						gs->modulesFactory->Create( ids.compute->pipeline, gs, ci, OUT mod );
						return mod;
					});
	}

}	// Platforms
}	// Engine
