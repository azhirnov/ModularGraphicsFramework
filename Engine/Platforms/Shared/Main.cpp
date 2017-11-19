// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Engine/Platforms/Common/Common.h"


#if defined( PLATFORM_WINDOWS )
#	include "Engine/Platforms/Windows/WinPlatform.h"
	using OSPlatform = Engine::Platforms::WinPlatform;
#endif


#ifdef GRAPHICS_API_OPENGL
#	include "Engine/Platforms/OpenGL/OpenGLContext.h"
#endif


#ifdef GRAPHICS_API_VULKAN
#	include "Engine/Platforms/Vulkan/VulkanContext.h"
#endif


#ifdef COMPUTE_API_OPENCL
#	include "Engine/Platforms/OpenCL/OpenCLContext.h"
#endif


#ifdef GRAPHICS_API_DIRECTX
#	include "Engine/Platforms/DirectX/DirectXContext.h"
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

/*
=================================================
	RegisterPlatforms
=================================================
*/
	void RegisterPlatforms ()
	{
		auto	ms	= GetMainSystemInstace();

		OSPlatform::Register( ms->GlobalSystems() );
		InputManager::Register( ms->GlobalSystems() );
		
		#ifdef GRAPHICS_API_OPENGL
			OpenGLContext::Register( ms->GlobalSystems() );
		#endif

		#ifdef GRAPHICS_API_VULKAN
			VulkanContext::Register( ms->GlobalSystems() );
		#endif

		#ifdef COMPUTE_API_OPENCL
			OpenCLContext::Register( ms->GlobalSystems() );
		#endif

		#ifdef GRAPHICS_API_DIRECTX
			DirectXContext::Register( ms->GlobalSystems() );
		#endif

		auto	factory = ms->GlobalSystems()->Get< ModulesFactory >();

		factory->Register( 0, &CreateDefaultPlatform );
		factory->Register( 0, &CreateDefaultWindow );
		factory->Register( 0, &CreateDefaultGpuContext );
		factory->Register( 0, &CreateDefaultGpuThread );
	}
	
/*
=================================================
	UnregisterPlatforms
=================================================
*/
	void UnregisterPlatforms ()
	{
		auto	ms	= GetMainSystemInstace();

		OSPlatform::Unregister( ms->GlobalSystems() );
		InputManager::Unregister( ms->GlobalSystems() );
		
		#ifdef GRAPHICS_API_OPENGL
			OpenGLContext::Unregister( ms->GlobalSystems() );
		#endif

		#ifdef GRAPHICS_API_VULKAN
			VulkanContext::Unregister( ms->GlobalSystems() );
		#endif

		#ifdef COMPUTE_API_OPENCL
			OpenCLContext::Unregister( ms->GlobalSystems() );
		#endif

		#ifdef GRAPHICS_API_DIRECTX
			DirectXContext::Unregister( ms->GlobalSystems() );
		#endif
	}
	
/*
=================================================
	CreateDefaultPlatform
=================================================
*/
	static ModulePtr CreateDefaultPlatform (GlobalSystemsRef gs, const CreateInfo::Platform &ci)
	{
		auto	factory = gs->Get< ModulesFactory >();

		Array< ModuleMsg::UntypedID_t >	ids;
		factory->Search<decltype(ci)>( "", OUT ids );
		
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
		auto	factory = gs->Get< ModulesFactory >();

		Array< ModuleMsg::UntypedID_t >	ids;
		factory->Search<decltype(ci)>( "", OUT ids );

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
	CreateDefaultGpuContext
=================================================
*/
	static ModulePtr CreateDefaultGpuContext (GlobalSystemsRef gs, const CreateInfo::GpuContext &ci)
	{
		auto	factory = gs->Get< ModulesFactory >();
		auto	api		= GAPI::ToString( ci.settings.version );
		usize	pos;

		if ( api.Find( ' ', OUT pos ) ) {
			api[pos] = '.';
			api[pos+1] = '\0';
		}

		Array< ModuleMsg::UntypedID_t >	ids;
		factory->Search<decltype(ci)>( api.cstr(), OUT ids );

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
		auto		ctx		= gs->Get< MainSystem >()->GetModuleByMsg< CompileTime::TypeListFrom< Message<GpuMsg::GetGraphicsModules> > >();
		auto		factory	= gs->Get< ModulesFactory >();
		ModulePtr	mod;

		if ( ctx ) {
			Message< GpuMsg::GetGraphicsModules >	req_ids;
			ctx->Send( req_ids );
			CHECK_ERR( factory->Create( req_ids->graphics->thread, gs, ci, OUT mod ) );
			return mod;
		}
		return null;
	}

}	// Platforms
}	// Engine
