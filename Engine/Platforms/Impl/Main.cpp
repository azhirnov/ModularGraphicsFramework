// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Engine/Platforms/Shared/Common.h"
#include "Engine/Platforms/Shared/GPU/Thread.h"
#include "Engine/Platforms/Shared/GPU/Context.h"
#include "Engine/Platforms/Shared/GPU/VR.h"
#include "Engine/Platforms/Shared/OS/Window.h"
#include "Engine/Platforms/Shared/OS/Platform.h"

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


namespace Engine
{
namespace Platforms
{

	static ModulePtr CreateDefaultPlatform (GlobalSystemsRef, const CreateInfo::Platform &);
	static ModulePtr CreateDefaultWindow (GlobalSystemsRef, const CreateInfo::Window &);
	static ModulePtr CreateDefaultGpuContext (GlobalSystemsRef, const CreateInfo::GpuContext &);
	static ModulePtr CreateDefaultGpuThread (GlobalSystemsRef, const CreateInfo::GpuThread &);
	static ModulePtr CreateDefaultVRThread (GlobalSystemsRef, const CreateInfo::VRThread &);

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
	static ModulePtr CreateDefaultPlatform (GlobalSystemsRef gs, const CreateInfo::Platform &ci)
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
	static ModulePtr CreateDefaultWindow (GlobalSystemsRef gs, const CreateInfo::Window &ci)
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
	static ModulePtr CreateDefaultGpuContext (GlobalSystemsRef gs, const CreateInfo::GpuContext &ci)
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
	static ModulePtr CreateDefaultGpuThread (GlobalSystemsRef gs, const CreateInfo::GpuThread &ci)
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
	static ModulePtr CreateDefaultVRThread (GlobalSystemsRef gs, const CreateInfo::VRThread &ci)
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


}	// Platforms
}	// Engine
