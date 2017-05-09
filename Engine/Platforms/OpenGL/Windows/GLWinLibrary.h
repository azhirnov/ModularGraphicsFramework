// Copyright �  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/Platforms/Common/Common.h"

#if defined( GRAPHICS_API_OPENGL ) and defined( PLATFORM_WINDOWS )

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
		using FuncPtr_t	= OS::HiddenOSTypeFrom<void*>;


	// variables
	private:
		OS::Library		_lib;
		FuncPtr_t		_getProc;	// wglGetProcAddress*


	// methods
	public:
		GLWinLibrary ();

		bool Load (StringCRef name = StringCRef());

		void Unload ();

		void * GetProc (StringCRef address) const;

		static StringCRef  GetDefaultName ();

	private:
		bool _OnInit ();
	};


	using GLLibrary		= GLWinLibrary;


}	// PlatformGL
}	// Engine

#endif	// GRAPHICS_API_OPENGL and PLATFORM_WINDOWS