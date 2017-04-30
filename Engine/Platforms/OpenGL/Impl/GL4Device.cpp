// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Engine/Platforms/OpenGL/Impl/GL4Device.h"
#include "Engine/Platforms/OpenGL/OpenGLThread.h"

#if defined( GRAPHICS_API_OPENGL )

namespace Engine
{
namespace PlatformGL
{
	
/*
=================================================
	constructor
=================================================
*/
	GL4BaseModule::GL4BaseModule (const GlobalSystemsRef gs,
								  UntypedID_t id,
								  const Runtime::VirtualTypeList *msgTypes,
								  const Runtime::VirtualTypeList *eventTypes) :
		Module( gs, id, msgTypes, eventTypes ),
		_glSystems( gs->Get< ParallelThread >()->GetModule( OpenGLThread::GetStaticID() )
						.ToPtr< OpenGLThread >()->GetDevice()->GLSystems() )
	{
	}

/*
=================================================
	constructor
=================================================
*/
	GL4Device::GL4Device () :
		_numExtensions( 0 ),
		_initialized( false )
	{
		GLSystems()->GetSetter< GL4Device >().Set( this );
	}
	
/*
=================================================
	destructor
=================================================
*/
	GL4Device::~GL4Device ()
	{
		GLSystems()->GetSetter< GL4Device >().Set( null );
	}

/*
=================================================
	Initialize
=================================================
*/
	bool GL4Device::Initialize ()
	{
		using namespace gl;

		CHECK_ERR( GL4_Init() );

		GL4_WriteInfo();

		_initialized = true;
		return true;
	}
	
/*
=================================================
	Deinitialize
=================================================
*/
	void GL4Device::Deinitialize ()
	{
		using namespace gl;

		GL4_Delete();

		_initialized = false;
	}
	
/*
=================================================
	IsInitialized
=================================================
*/
	bool GL4Device::IsInitialized () const
	{
		return _initialized;
	}


}	// PlatformGL
}	// Engine

#endif	// GRAPHICS_API_OPENGL
