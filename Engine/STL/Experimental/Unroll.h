// Copyright © 2014-2017  Zhirnov Andrey. All rights reserved.

#pragma once

#include "Engine/STL/Common/Types.h"

namespace GX_STL
{
namespace GXTypes
{
	
/*
=================================================
	UnrollLoop
=================================================
*/
	template <usize Size>
	struct UnrollLoop
	{
		template <typename T>
		forceinline static void _Iterate (T &iter, usize index)
		{
			iter( index );
			UnrollLoop< Size-1 >::_Iterate( iter, index+1 );
		}

		template <typename T>
		forceinline static void Run (T &iter)
		{
			UnrollLoop< Size-1 >::_Iterate( iter, 0 );
		}
	};


	template <>
	struct UnrollLoop <0>
	{
		template <typename T>
		forceinline static void _Iterate (T &iter, usize index)
		{
			iter( index );
		}
	};


}	// GXTypes
}	// GX_STL
