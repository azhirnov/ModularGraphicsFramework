// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Core/STL/Common/Platforms.h"

#if defined( PLATFORM_SDL )

#include "Core/STL/OS/SDL/SDLUtils.h"
#include "Core/STL/Math/BinaryMath.h"
#include "Core/STL/Math/Interpolations.h"
#include "Core/STL/Log/ToString.h"

namespace GX_STL
{
namespace OS
{
	
/*
=================================================
	_CheckError
=================================================
*/
	bool SDLUtils::_CheckError (StringCRef sdlCall, StringCRef func, StringCRef file, int line)
	{
		StringCRef	err = SDL_GetError();

		if ( err.Empty() )
			return true;
		
		String	str("SDL error: ");
		
		str << err << "\n in " << sdlCall << ", function " << func;

		LOG( str, ELog::Warning, file.cstr(), line );

		SDL_ClearError();
		return false;
	}


}	// OS
}	// GX_STL

#endif	// PLATFORM_SDL
