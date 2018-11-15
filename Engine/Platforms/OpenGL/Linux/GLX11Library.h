// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Core/STL/Common/Platforms.h"
#include "Core/Config/Engine.Config.h"

#if defined( GRAPHICS_API_OPENGL ) and defined( PLATFORM_LINUX )

#include "Engine/Platforms/Public/Common.h"

namespace Engine
{
namespace PlatformGL
{
	
	//
	// Linux OpenGL Library
	//

	struct GLX11Library
	{
	// types
	private:
		using FuncPtr_t	= DeferredTypeFrom<void*>;


	// variables
	private:
		OS::Library		_lib;
		FuncPtr_t		_getProc;
		FuncPtr_t		_getProcARB;


	// methods
	public:
		GLX11Library ();

		bool Load (StringCRef name = StringCRef());

		void Unload ();

		SharedLibFunction_t  GetProc (StringCRef address) const;

		static StringCRef  GetDefaultName ();

	private:
		bool _OnInit ();
	};


	using GLLibrary		= GLX11Library;


}	// PlatformGL
}	// Engine

#endif	// GRAPHICS_API_OPENGL and PLATFORM_LINUX
