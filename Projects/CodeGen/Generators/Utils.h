// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Common.h"

namespace CodeGen
{

/*
=================================================
	GetMinBitsForNumber
=================================================
*/
	inline BitsU GetMinBitsForNumber (usize value)
	{
		if ( value == 0 )
			return 0_bit;

		BitsU	count	= BitsU(IntLog2( value )) + uint(not IsPowerOfTwo( value ) or value == 1);

		ASSERT( count > 0 );
		ASSERT( value <= (1u << usize(count)) );

		return count;
	}
	

}	// CodeGen
