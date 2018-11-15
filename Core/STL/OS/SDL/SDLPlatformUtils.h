// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Core/STL/Common/Platforms.h"

#if defined( PLATFORM_SDL )

#include "Core/STL/OS/Windows/WinPlatformUtils.h"
#include "Core/STL/OS/Posix/PosixPlatformUtils.h"

namespace GX_STL
{
namespace OS
{
	
	//
	// SDL Utils
	//

	struct SDLUtils final : Noninstancable
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
