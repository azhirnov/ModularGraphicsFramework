// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Generators/Commands.h"

namespace CodeGen
{

/*
=================================================
	GetConstants
=================================================
*/
	template <typename T>
	inline Set<T>  Commands::GetConstants (EConstantSet::bits flags)
	{
		Set<T>	result;

		if_constexpr( CompileTime::IsFloat<T>				or
					  CompileTime::IsSameTypes< T, MixedF >	or
					  CompileTime::IsSameTypes< T, MixedD > )
		{
			if ( flags[EConstantSet::Math] )
				result.AddArray( _GetMathConstantsF<T>() );

			if ( flags[EConstantSet::Physics] )
				result.AddArray( _GetPhysicsConstantsF<T>() );

			if ( flags[EConstantSet::MathMostPopularOnly] )
				result.AddArray( _GetMathMostPopularOnlyConstantsF<T>() );

			if ( flags[EConstantSet::PhysicsMostPopularOnly] )
				result.AddArray( _GetPhysicsMostPopularOnlyConstantsF<T>() );
		}
		else
		{
			ASSERT( not flags[EConstantSet::Math] );
			ASSERT( not flags[EConstantSet::Physics] );
			ASSERT( not flags[EConstantSet::MathMostPopularOnly] );
			ASSERT( not flags[EConstantSet::PhysicsMostPopularOnly] );
		}

		if ( flags[EConstantSet::PrimeNumbers] )
			result.AddArray( _GetPrimeNumbersConstants<T>() );
		
		if ( flags[EConstantSet::Values0to255] )
			result.AddArray( _GetValues0to255Constants<T>() );

		return result;
	}
	
/*
=================================================
	_GetMathConstantsF
=================================================
*/
	template <typename T>
	inline Array<T>  Commands::_GetMathConstantsF ()
	{
		return { T(0), T(1), T(2), T(3), T(5), T(10), T(0.5), T(0.25), T(0.1), T(180), Pi<T>, ReciporalPi<T>, EulerNumber<T> };
	}
	
/*
=================================================
	_GetPhysicsConstantsF
=================================================
*/
	template <typename T>
	inline Array<T>  Commands::_GetPhysicsConstantsF ()
	{
		return {};	// TODO
	}
	
/*
=================================================
	_GetMathMostPopularOnlyConstantsF
=================================================
*/
	template <typename T>
	inline Array<T>  Commands::_GetMathMostPopularOnlyConstantsF ()
	{
		return {};	// TODO
	}
	
/*
=================================================
	_GetPhysicsMostPopularOnlyConstantsF
=================================================
*/
	template <typename T>
	inline Array<T>  Commands::_GetPhysicsMostPopularOnlyConstantsF ()
	{
		return {};	// TODO
	}
	
/*
=================================================
	_GetPrimeNumbersConstants
=================================================
*/
	template <typename T>
	inline Array<T>  Commands::_GetPrimeNumbersConstants ()
	{
		return { T(1), T(2), T(3), T(5), T(7), T(11), T(13), T(17), T(19), T(23), T(29), T(31), T(37) };
	}
	
/*
=================================================
	_GetValues0to255Constants
=================================================
*/
	template <typename T>
	inline Array<T>  Commands::_GetValues0to255Constants ()
	{
		Array<T>	result;
		result.Resize( 256, false );

		FOR( i, result ) {
			result[i] = T(int(i));
		}
		return result;
	}

}	// CodeGen
