// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/Platforms/Common/Common.h"

#if defined( GRAPHICS_API_OPENGL )

#include "Engine/Platforms/OpenGL/Impl/gl4.h"

namespace Engine
{
namespace PlatformGL
{
	using namespace Platforms;

	using GLSubSystems	= EngineSubSystems< "opengl 4"_StringToID >;
	using GLSystemsRef	= ConstReference< GLSubSystems >;

	class GL4Device;
	class GL4StateManager;



	//
	// OpenGL Base Module
	//

	class GL4BaseModule : public Module
	{
	// variables
	private:
		const GLSystemsRef		_glSystems;
		

	// methods
	public:
		GL4BaseModule (const GlobalSystemsRef gs,
					   const ModuleConfig &config,
					   const Runtime::VirtualTypeList *msgTypes,
					   const Runtime::VirtualTypeList *eventTypes);

		GL4BaseModule (const GlobalSystemsRef gs,
					   const ModulePtr &gpuThread,
					   const ModuleConfig &config,
					   const Runtime::VirtualTypeList *msgTypes,
					   const Runtime::VirtualTypeList *eventTypes);

		GL4BaseModule (const GlobalSystemsRef gs,
					   const GLSystemsRef glSys,
					   const ModuleConfig &config,
					   const Runtime::VirtualTypeList *msgTypes,
					   const Runtime::VirtualTypeList *eventTypes);

		GLSystemsRef	GLSystems ()	const	{ return _glSystems; }
	};


}	// PlatformGL

	using GLSystemsTypeList_t	= SubSystemsTypeList< "opengl 4"_StringToID, CompileTime::TypeListFrom<
											PlatformGL::GL4Device,
											PlatformGL::GL4StateManager
										> >;

}	// Engine

GX_SUBSYSTEM_DECL( GLSystemsTypeList_t,		PlatformGL::GL4Device,			PlatformGL::GL4Device );
GX_SUBSYSTEM_DECL( GLSystemsTypeList_t,		PlatformGL::GL4StateManager,	PlatformGL::GL4StateManager );

#endif	// GRAPHICS_API_OPENGL
