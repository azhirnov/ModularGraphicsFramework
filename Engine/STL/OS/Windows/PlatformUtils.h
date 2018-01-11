// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "OSWindows.h"

#ifdef PLATFORM_WINDOWS

namespace GX_STL
{
namespace OS
{

	//
	// OS Utils
	//

	struct PlatformUtils
	{
		static void OpenURL (StringCRef url);

		static void OpenFile (StringCRef filename);

		static bool CreateLink (StringCRef linkFilename, StringCRef link);

		static bool _CheckError (StringCRef file, int line);

		static void IDEConsoleMessage (StringCRef message, StringCRef file, int line);

		static bool ValidateHeap ();

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

			static EResult ShowAssertion (StringCRef caption, StringCRef msg, StringCRef file, int line);
		};
	};


#ifndef __GX_OS_ERROR_CHECKS__

#	define CHECK_OS_ERROR() \
		{}

#else

#	define CHECK_OS_ERROR() \
		::GX_STL::OS::PlatformUtils::_CheckError( __FILE__, __LINE__ )

#endif	// !__GX_OS_ERROR_CHECKS__


}	// OS
}	// GX_STL

#endif	// PLATFORM_WINDOWS