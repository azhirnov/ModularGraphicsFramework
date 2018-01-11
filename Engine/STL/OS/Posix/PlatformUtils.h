// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "OSPosix.h"

#ifdef PLATFORM_BASE_POSIX

namespace GX_STL
{
namespace OS
{
	using namespace posix;


	//
	// OS Utils
	//

	struct PlatformUtils : public Noninstancable
	{
		static void OpenURL (StringCRef url);
	};

	
	inline void PlatformUtils::OpenURL (StringCRef url)
	{
		TODO( "OpenURL" );
	}


}	// OS
}	// GX_STL

#endif	// PLATFORM_BASE_POSIX