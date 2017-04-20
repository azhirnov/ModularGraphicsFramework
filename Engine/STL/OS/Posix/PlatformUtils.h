// Copyright © 2014-2017  Zhirnov Andrey. All rights reserved.

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