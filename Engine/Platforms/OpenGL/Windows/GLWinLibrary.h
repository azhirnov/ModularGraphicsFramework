// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Core/STL/Common/Platforms.h"
#include "Core/Config/Engine.Config.h"

#if defined( GRAPHICS_API_OPENGL ) and defined( PLATFORM_WINDOWS )

#include "Engine/Platforms/Public/Common.h"

namespace Engine
{
namespace PlatformGL
{
	
	//
	// Windows OpenGL Library
	//

	struct GLWinLibrary
	{
	// types
	private:
		using FuncPtr_t	= DeferredTypeFrom<void*>;


	// variables
	private:
		OS::Library		_lib;
		FuncPtr_t		_getProc;	// wglGetProcAddress*


	// methods
	public:
		GLWinLibrary ();

		bool Load (StringCRef name = StringCRef());

		void Unload ();

		SharedLibFunction_t  GetProc (StringCRef address) const;

		static StringCRef  GetDefaultName ();

	private:
		bool _OnInit ();
	};


	using GLLibrary		= GLWinLibrary;


}	// PlatformGL
}	// Engine

#endif	// GRAPHICS_API_OPENGL and PLATFORM_WINDOWS
