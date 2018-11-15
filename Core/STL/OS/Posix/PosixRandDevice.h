// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Core/STL/Common/Platforms.h"

#ifdef PLATFORM_BASE_POSIX

#include "Core/STL/OS/Posix/OSPosix.h"

namespace GX_STL
{
namespace OS
{

	//
	// Random Device
	//

	struct PlatformRandomDevice final : public Noncopyable
	{
	// variables
	private:
		int		_fd;


	// methods
	public:
		PlatformRandomDevice (StringCRef provider);
		~PlatformRandomDevice ();

		template <typename T>
		bool Generate (OUT T &value) const
		{
			return _Generate( Cast<char *>(AddressOf( value )), sizeof(value) );
		}

	private:
		bool _Create (StringCRef provider);
		bool _CreateDefault ();

		bool _Generate (char *ptr, usize size) const;
	};

	
}	// OS
}	// GX_STL

#endif	// PLATFORM_BASE_POSIX
