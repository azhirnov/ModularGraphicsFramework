// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Core/STL/Common/Platforms.h"
#include "Core/Config/STL.Config.h"

#ifdef PLATFORM_WINDOWS

#include "Core/STL/OS/Windows/WinRandDevice.h"
#include "Core/STL/OS/Windows/WinHeader.h"

namespace GX_STL
{
namespace OS
{
	
/*
=================================================
	constructor
=================================================
*/
	PlatformRandomDevice::PlatformRandomDevice (StringCRef provider) :
		_prov( UninitializedT<HCRYPTPROV>() )
	{
		if ( not provider.Empty() ) {
			CHECK( _CreateFromProviderName( provider ) );
		}
		else {
			CHECK( _CreateDefault() );
		}
	}
	
/*
=================================================
	destructor
=================================================
*/
	PlatformRandomDevice::~PlatformRandomDevice ()
	{
		if ( _prov.IsNotNull<HCRYPTPROV>() ) {
			CHECK( ::CryptReleaseContext( _prov.Get<HCRYPTPROV>(), 0 ) != FALSE );
		}
	}
	
/*
=================================================
	_CreateFromProviderName
=================================================
*/
	bool PlatformRandomDevice::_CreateFromProviderName (StringCRef provider)
	{
		char	buf[ 128 ];
		DWORD	type;
		DWORD	len;

		// get type
		for (uint i = 0; ; ++i)
		{
			len = sizeof(buf);

			if ( ::CryptEnumProviders( i, null, 0, OUT &type, OUT buf, OUT &len ) == FALSE )
				return false;

			if ( provider == buf )
				break;
		}
		
		return ::CryptAcquireContext( &_prov.Get<HCRYPTPROV>(), null, provider.cstr(), type, CRYPT_VERIFYCONTEXT | CRYPT_SILENT ) != FALSE;
	}
	
/*
=================================================
	_CreateFromType
=================================================
*/
	bool PlatformRandomDevice::_CreateFromType (uint type)
	{
		return ::CryptAcquireContext( &_prov.Get<HCRYPTPROV>(), null, LPCSTR(null), type, CRYPT_VERIFYCONTEXT | CRYPT_SILENT ) != FALSE;
	}
	
/*
=================================================
	_CreateDefault
=================================================
*/
	bool PlatformRandomDevice::_CreateDefault ()
	{
		const DWORD	types[] = {
			PROV_RSA_FULL, PROV_RSA_AES, PROV_RSA_SIG, PROV_RSA_SCHANNEL, PROV_DSS,
			PROV_DSS_DH, PROV_DH_SCHANNEL, PROV_FORTEZZA, PROV_MS_EXCHANGE, PROV_SSL
		};

		FOR( i, types )
		{
			if ( _CreateFromType( types[i] ) )
				return true;
		}
		return false;
	}
	
/*
=================================================
	_CreateDefault2
=================================================
*/
	bool PlatformRandomDevice::_CreateDefault2 ()
	{
		return _CreateFromProviderName( "Microsoft Base Cryptographic Provider v1.0" );
	}
	
/*
=================================================
	_Generate
=================================================
*/
	bool PlatformRandomDevice::_Generate (ubyte *ptr, usize size) const
	{
		return CryptGenRandom( _prov.Get<HCRYPTPROV>(), DWORD(size), ptr ) != FALSE;
	}


}	// OS
}	// GX_STL

#endif	// PLATFORM_WINDOWS
