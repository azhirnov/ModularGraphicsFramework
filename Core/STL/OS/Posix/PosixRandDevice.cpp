// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Core/STL/Common/Platforms.h"

#ifdef PLATFORM_BASE_POSIX

#include "Core/STL/OS/Posix/PosixHeader.h"
#include "Core/STL/OS/Posix/RandDevice.h"
#include "Core/STL/Math/BinaryMath.h"

namespace GX_STL
{
namespace OS
{
	
/*
=================================================
	constructor
=================================================
*/
	PlatformRandomDevice::PlatformRandomDevice (StringCRef provider) : _fd(-1)
	{
		if ( not provider.Empty() ) {
			CHECK_ERR( _Create( provider ), );
		}
		else {
			CHECK_ERR( _CreateDefault(), );
		}
	}

/*
=================================================
	destructor
=================================================
*/
	PlatformRandomDevice::~PlatformRandomDevice ()
	{
		CHECK( ::close( _fd ) >= 0 );
	}
	
/*
=================================================
	_Generate
=================================================
*/
	bool PlatformRandomDevice::_Generate (char *ptr, usize size) const
	{
		const usize	readn = ::read( _fd, ptr, size );
		return readn == size;
	}
	
/*
=================================================
	_Create
=================================================
*/
	bool PlatformRandomDevice::_Create (StringCRef provider)
	{
		_fd = ::open( provider.cstr(), O_RDONLY );
		return ( _fd >= 0 );
	}
	
/*
=================================================
	_CreateDefault
=================================================
*/
	bool PlatformRandomDevice::_CreateDefault ()
	{
		return _Create( "/dev/urandom" );
	}


}	// OS
}	// GX_STL

#endif	// PLATFORM_BASE_POSIX
