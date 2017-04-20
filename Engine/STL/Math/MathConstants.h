// Copyright © 2014-2017  Zhirnov Andrey. All rights reserved.

#pragma once

#include "Engine/STL/Common/Types.h"

namespace GX_STL
{
namespace GXMath
{
	

	//
	// Constants
	//

	template <typename T>
	static constexpr T Pi = T( 3.14159265358979323846 );
	
	
	//template <typename T>
	//constexpr T Pi (T scale)
	//{
	//	return _Pi<T> * scale;
	//}
	

	template <typename T>
	static constexpr T EulerNumber = T( 2.71828182845904523536 );

	
	template <typename T>
	static constexpr T ReciporalPi = T( 0.3183098861837906715377 );

	
	//template <typename T>
	//constexpr T ReciporalPi (T scale)
	//{
	//	return _ReciporalPi<T> * scale;
	//}


}	// GXMath
}	// GX_STL
