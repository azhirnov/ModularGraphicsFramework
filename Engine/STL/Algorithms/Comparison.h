// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/STL/Math/Vec.h"

namespace GX_STL
{
namespace GXTypes
{
namespace Comparison
{
	template <typename T>	bool Equal (const T &left, const T &right);
	template <typename T>	bool Less (const T &left, const T &right);
	template <typename T>	bool Greater (const T &left, const T &right);
	

/*
=================================================
	Equal
=================================================
*/
	template <typename T>
	inline bool Equal (const T &left, const T &right)
	{
		return GXMath::All( left == right );
	}
		
/*
=================================================
	Less
=================================================
*/
	template <typename T>
	inline bool Less (const T &left, const T &right)
	{
		return left < right;
	}
	
	template <typename T>
	inline bool Less (const GXMath::Vec<T,2> &left, const GXMath::Vec<T,2> &right)
	{
		return	left.x != right.x ?	left.x < right.x :
									left.y < right.y;
	}
	
	template <typename T>
	inline bool Less (const GXMath::Vec<T,3> &left, const GXMath::Vec<T,3> &right)
	{
		return	left.x != right.x ?	left.x < right.x :
				left.y != right.y ?	left.y < right.y :
									left.z < right.z;
	}
	
	template <typename T>
	inline bool Less (const GXMath::Vec<T,4> &left, const GXMath::Vec<T,4> &right)
	{
		return	left.x != right.x ?	left.x < right.x :
				left.y != right.y ?	left.y < right.y :
				left.z != right.z ?	left.z < right.z :
									left.w < right.w;
	}
		
/*
=================================================
	Greater
=================================================
*/
	template <typename T>
	inline bool Greater (const T &left, const T &right)
	{
		return left > right;
	}
	
	template <typename T>
	inline bool Greater (const GXMath::Vec<T,2> &left, const GXMath::Vec<T,2> &right)
	{
		return	left.x != right.x ?	left.x > right.x :
									left.y > right.y;
	}
	
	template <typename T>
	inline bool Greater (const GXMath::Vec<T,3> &left, const GXMath::Vec<T,3> &right)
	{
		return	left.x != right.x ?	left.x > right.x :
				left.y != right.y ?	left.y > right.y :
									left.z > right.z;
	}
	
	template <typename T>
	inline bool Greater (const GXMath::Vec<T,4> &left, const GXMath::Vec<T,4> &right)
	{
		return	left.x != right.x ?	left.x > right.x :
				left.y != right.y ?	left.y > right.y :
				left.z != right.z ?	left.z > right.z :
									left.w > right.w;
	}


}	// Comparison
}	// GXTypes
}	// GX_STL
