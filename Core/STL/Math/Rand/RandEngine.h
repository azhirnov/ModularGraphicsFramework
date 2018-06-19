// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Core/STL/Math/Mathematics.h"
#include "Core/STL/OS/OSLowLevel.h"

namespace GX_STL
{
namespace GXMath
{

	
	//
	// Default C Random Engne
	//
	
	struct DefaultRandomEngine
	{
		ND_ uint	operator () ()	const	{ return uint(rand()); }
		ND_ uint	Max()			const	{ return RAND_MAX; }
	};



	//
	// Platfomr Random Engine
	//

	struct PlatformRandomEngine
	{
	// variables
	private:
		OS::PlatformRandomDevice	_device;

	// methods
	public:
		PlatformRandomEngine () : _device( StringCRef() )
		{}
		
		ND_ uint	operator () ()	const	{ uint result;  _device.Generate( result );  return result; }
		ND_ uint	Max()			const	{ return MaxValue< uint >(); }
	};


	/*
#ifdef PLATFORM_WINDOWS
	template <uint MAX = 0xffff>
	struct TRandomEngineTicks
	{
		ND_ uint operator () ()	const	{ return ::GetTickCount() & MAX; }
		ND_ uint Max()			const	{ return MAX; }
	};
#endif
	*/


}	// GXMath
}	// GX_STL
