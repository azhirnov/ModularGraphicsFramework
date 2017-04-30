// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

namespace Engine
{
	namespace _engine_hidden_
	{
		constexpr int BuildVersionNumber (int v0, int v1, int v2, int revision)
		{
			//ASSERT( v0 < 0x7 and v1 < 0xF and v2 < 0xFF );
			return	((v0 & 0x7) << 28) |
					((v1 & 0xF) << 24) |
					((v2 & 0xFF) << 16) |
					(revision & 0xFFFF);
		}

		constexpr int BuildResourceVersion (int engineVer, int resourceVer)
		{
			//ASSERT( resourceVer < 0xFFFF );
			return	(engineVer & ~0xFFFF) | (resourceVer & 0xFFFF);
		}

	}	// _engine_hidden_


	static const char	ENGINE_NAME[]		= "GraphXGenEngine2";
	static const char	ENGINE_BUILD_DATE[]	= __DATE__ " " __TIME__;
	static const int	ENGINE_VERSION		= _engine_hidden_::BuildVersionNumber( 0, 1, 0, 0 );


}	// Engine