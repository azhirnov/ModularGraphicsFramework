// Copyright © 2014-2017  Zhirnov Andrey. All rights reserved.

#pragma once

#include "Engine/STL/OS/Windows/PlatformUtils.h"
#include "Engine/STL/OS/Posix/PlatformUtils.h"

#if defined( PLATFORM_SDL )

namespace GX_STL
{
namespace OS
{
	
	//
	// SDL Utils
	//

	struct SDLUtils
	{
	public:
		
		static bool _CheckError (StringCRef sdlCall, StringCRef func, StringCRef file, int line);
	};

	
# if !defined( __GX_OS_ERROR_CHECKS__ )

#	define SDL_CALL( ... )		__VA_ARGS__
#	define SDL_CHECK( ... )		__VA_ARGS__

# else

#	define SDL_CALL( ... ) \
	{ \
		bool __sdl_err = not ( __VA_ARGS__ ); \
		if ( __sdl_err ) \
			::GX_STL::OS::SDLUtils::_CheckError( TOSTRING(__VA_ARGS__), GX_FUNCTION_NAME, __FILE__, __LINE__ ); \
	}
	
#	define __SDL_CHECK( _func_, _ret_, ... ) \
	{ \
		bool __sdl_err = not ( _func_ ); \
		if ( __sdl_err ) { \
			::GX_STL::OS::SDLUtils::_CheckError( TOSTRING(_func_), GX_FUNCTION_NAME, __FILE__, __LINE__ ); \
			return _ret_; \
		} \
	}

#	define SDL_CHECK( ... ) \
		__SDL_CHECK( AUXDEF_GETARG_0( __VA_ARGS__ ), AUXDEF_GETARG_1( __VA_ARGS__, ::GX_STL::GXTypes::Uninitialized ) )

#endif	// !__GX_OS_ERROR_CHECKS__


}	// OS
}	// GX_STL

#endif	// PLATFORM_SDL
