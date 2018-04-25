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
	GetAPIName
=================================================
*/
	String  ComputeSettings::GetAPIName () const
	{
		return _GAPItoAPIName( version );
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

}	// Platforms
}	// Engine
