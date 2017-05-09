// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Engine/Platforms/OpenGL/Impl/GL4BaseModule.h"
#include "Engine/Platforms/OpenGL/OpenGLThread.h"

#if defined( GRAPHICS_API_OPENGL )

namespace Engine
{
namespace PlatformGL
{
	
/*
=================================================
	GetGLSubSystems
=================================================
*/
	forceinline GLSystemsRef GetGLSubSystems (const GlobalSystemsRef gs, const ModulePtr &gpuThread)
	{
		if ( gpuThread )
			return gpuThread.ToPtr< OpenGLThread >()->GetDevice()->GLSystems();

		ModulePtr const&	gpu_thread = gs->Get< ParallelThread >()->GetModule( OpenGLThread::GetStaticID() );
		
		if ( gpu_thread )
			return gpu_thread.ToPtr< OpenGLThread >()->GetDevice()->GLSystems();

		return GLSystemsRef(null);
	}

/*
=================================================
	constructor
=================================================
*/
	GL4BaseModule::GL4BaseModule (const GlobalSystemsRef gs,
								  const ModuleConfig &config,
								  const Runtime::VirtualTypeList *msgTypes,
								  const Runtime::VirtualTypeList *eventTypes) :
		Module( gs, config, msgTypes, eventTypes ),
		_glSystems( GetGLSubSystems( gs, null ) )
	{
	}
	
/*
=================================================
	constructor
=================================================
*/
	GL4BaseModule::GL4BaseModule (const GlobalSystemsRef gs,
								  const ModulePtr &gpuThread,
								  const ModuleConfig &config,
								  const Runtime::VirtualTypeList *msgTypes,
								  const Runtime::VirtualTypeList *eventTypes) :
		Module( gs, config, msgTypes, eventTypes ),
		_glSystems( GetGLSubSystems( gs, gpuThread ) )		
	{
	}
	
/*
=================================================
	constructor
=================================================
*/
	GL4BaseModule::GL4BaseModule (const GlobalSystemsRef gs,
								  const GLSystemsRef glSys,
								  const ModuleConfig &config,
								  const Runtime::VirtualTypeList *msgTypes,
								  const Runtime::VirtualTypeList *eventTypes) :
		Module( gs, config, msgTypes, eventTypes ),
		_glSystems( glSys )
	{}


}	// PlatformGL
}	// Engine

#endif	// GRAPHICS_API_OPENGL
