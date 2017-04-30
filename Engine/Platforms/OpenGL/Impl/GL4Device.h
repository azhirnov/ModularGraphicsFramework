// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/Platforms/OpenGL/Impl/GL4BaseModule.h"

#if defined( GRAPHICS_API_OPENGL )

namespace Engine
{
namespace PlatformGL
{

	//
	// OpenGL 4.x Device
	//

	class GL4Device final
	{
	// variables
	private:
		GLSubSystems			_glSystems;

		//EVideoCardVendor::type	_vendor;
		
		uint					_numExtensions;
		bool					_initialized;


	// methods
	public:
		GL4Device ();
		~GL4Device ();

		bool Initialize ();
		void Deinitialize ();

		bool IsInitialized () const;

		GLSystemsRef	GLSystems ()	const	{ return GLSystemsRef(&_glSystems); }
	};


}	// PlatformGL
}	// Engine

#endif	// GRAPHICS_API_OPENGL
