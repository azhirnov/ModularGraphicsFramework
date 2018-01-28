// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/STL/Containers/String.h"
#include "Engine/STL/Containers/ArrayRef.h"

namespace GX_STL
{
namespace GXTypes
{
namespace StringUtils
{
	
/*
=================================================
	IsUpper
=================================================
*/
	template <typename T>
	constexpr forceinline bool IsUpper (const T &c)
	{
		return ( c >= T('A') and c <= T('Z') );
	}
	
/*
=================================================
	IsLower
=================================================
*/
	template <typename T>
	constexpr forceinline bool IsLower (const T &c)
	{
		return ( c >= T('a') and c <= T('z') );
	}
	
/*
=================================================
	ToUpper
=================================================
*/
	template <typename T>
	constexpr forceinline T ToUpper (const T &c)
	{
		return ( IsLower( c ) ? c + T('A' - 'a') : c );
	}
	
/*
=================================================
	ToLower
=================================================
*/
	template <typename T>
	constexpr forceinline T ToLower (const T &c)
	{
		return ( IsUpper( c ) ? c + T('a' - 'A') : c );
	}
	
/*
=================================================
	ConvertArrayToString
----
	write array values as string (binary format)
=================================================
*/
	template <typename T>
	inline String  ConvertArrayToString (ArrayCRef<T> arr)
	{
		String	str;
		str.Reserve( (usize)arr.Size() * 4 );

		FOR( i, arr )
		{
			for (usize j = 0; j < sizeof(T); ++j)
			{
				T const&	t    = arr[i];
				const char	i0   = (t >> (j*8)) & 0xF;
				const char	i1   = (t >> (j*8 + 4)) & 0xF;

				const char	c[5] = { '\\', 'x', (i0>9 ? 'A'+i0-10 : '0'+i0), (i1>9 ? 'A'+i1-10 : '0'+i1), '\0' };

				str << StringCRef( c, 4 );
			}
		}
		return str;
	}
	
/*
=================================================
	Repeat String
=================================================
*/
	inline String  RepeatString (StringCRef part, uint count)
	{
		String	str = part;

		for (uint i = 0; i < count; ++i) {
			str << part;
		}
		return str;
	}


	
	inline int		ToInt32 (StringCRef sStr)			{ return atoi( sStr.cstr() ); }

#ifdef COMPILER_MSVC
	inline ilong	ToInt64 (StringCRef sStr)			{ return _atoi64( sStr.cstr() ); }
#endif

#ifdef COMPILER_GCC
	inline ilong	ToInt64 (StringCRef sStr)			{ return atoll( sStr.cstr() ); }
#endif
	
	inline float	ToFloat (StringCRef sStr)			{ return float( atof( sStr.cstr() ) ); }
	inline double	ToDouble (StringCRef sStr)			{ return atof( sStr.cstr() ); }


}	// StringUtils
}	// GXTypes
}	// GX_STL
