// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Engine/Platforms/Public/GPU/Context.h"

namespace Engine
{
namespace Platforms
{

/*
=================================================
	_GAPItoAPIName
=================================================
*/
	static String _GAPItoAPIName (GAPI::type version)
	{
		auto	api	= GAPI::ToString( version );
		usize	pos;

		if ( api.Find( ' ', OUT pos ) and pos > 0 )
			api[pos] = '\0';
		else
			api[0] = '\0';

		return StringCRef( api.cstr() );
	}

/*
=================================================
	_GAPItoAPIVersion
=================================================
*/
	static uint _GAPItoAPIVersion (GAPI::type version)
	{
		auto	api	= GAPI::ToString( version );
		usize	pos;
		uint	ver = 0;

		if ( api.Find( ' ', OUT pos ) and pos > 0 )
		{
			ver += (Clamp( api[pos+1], '0', '9' ) - '0') * 100;

			if ( pos+3 < api.Length() )
			{
				ASSERT( api[pos+2] == '.' );
			
				ver += (Clamp( api[pos+3], '0', '9' ) - '0') * 10;
			}
		}

		return ver;
	}

/*
=================================================
	GetAPIName
=================================================
*/
	String  ComputeSettings::GetAPIName () const
	{
		return _GAPItoAPIName( version );
	}
	
/*
=================================================
	GetAPIVersion
=================================================
*/
	uint  ComputeSettings::GetAPIVersion () const
	{
		return _GAPItoAPIVersion( version );
	}

/*
=================================================
	GetAPIName
=================================================
*/
	String  GraphicsSettings::GetAPIName () const
	{
		return _GAPItoAPIName( version );
	}
	
/*
=================================================
	GetAPIVersion
=================================================
*/
	uint  GraphicsSettings::GetAPIVersion () const
	{
		return _GAPItoAPIVersion( version );
	}

}	// Platforms
}	// Engine
