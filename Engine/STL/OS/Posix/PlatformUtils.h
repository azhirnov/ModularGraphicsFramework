// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/STL/Common/Platforms.h"

#ifdef PLATFORM_BASE_POSIX

#include "Engine/STL/OS/Posix/OSPosix.h"

namespace GX_STL
{
namespace OS
{

	//
	// OS Utils
	//

	struct PlatformUtils final : public Noninstancable
	{
		static void OpenURL (StringCRef url);
		
		static void IDEConsoleMessage (StringCRef message, StringCRef file, int line) {}

		struct Dialog
		{
			enum class EResult
			{
				Unknown = -1,
				Skip	= 0,	// skip only this message
				Ignore,			// skip some messages in first 10 seconds
				Retry,			// run debugger
				Abort,			// exit from application
			};

			static EResult ShowAssertion (StringCRef caption, StringCRef msg, StringCRef file, int line) { return Uninitialized; }
		};
	};

	
	inline void PlatformUtils::OpenURL (StringCRef url)
	{
		TODO( "OpenURL" );
	}


}	// OS
}	// GX_STL

#endif	// PLATFORM_BASE_POSIX