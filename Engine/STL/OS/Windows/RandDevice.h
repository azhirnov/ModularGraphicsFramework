// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "OSWindows.h"

#ifdef PLATFORM_WINDOWS

namespace GX_STL
{
namespace OS
{

	//
	// Random Device
	//

	struct _STL_EXPORT_ PlatformRandomDevice
	{
	// types
	private:
		typedef HiddenOSTypeFrom< usize >	Handle_t;	// HCRYPTPROV


	// variables
	private:
		Handle_t	_prov;


	// methods
	public:
		explicit
		PlatformRandomDevice (StringCRef provider);
		~PlatformRandomDevice ();


		template <typename T>
		bool Generate (T &value) const
		{
			return _Generate( (ubyte *) &value, sizeof(value) );
		}


	private:
		bool _CreateFromProviderName (StringCRef provider);
		bool _CreateFromType (uint type);

		bool _CreateDefault ();
		bool _CreateDefault2 ();

		bool _Generate (ubyte *ptr, usize size) const;
	};
	
	
}	// OS
}	// GX_STL

#endif	// PLATFORM_WINDOWS
