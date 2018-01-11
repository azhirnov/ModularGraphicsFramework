// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Random.h"
#include "NormalDistribution.h"

namespace GX_STL
{
namespace GXMath
{

	//
	// Random Generator With Chance
	//

	template <typename RndEngine>
	struct TRandomGenerator<RndEngine>::WithChance : public Noninstancable
	{
		typedef TRandomGenerator<RndEngine>	RG;
		
		
		template <typename T, usize I>
		static bool CheckChances (const T (&chances)[I])
		{
			return CheckChances( ArrayCRef<T>( chances ) );
		}
		

		template <typename T>
		static bool CheckChances (ArrayCRef<T> chances)
		{
			typedef typename TypeTraits::RemoveConst<T>		Value_t;

			Value_t	summ(0);
			
			FOR( i, chances ) {
				summ += chances[i];
			}
			return Equals( summ, Value_t(1) );
		}

		
		template <typename T, usize I>
		static usize Index (const T (&chances)[I])
		{
			return Index( ArrayCRef<T>( chances ) );
		}
		

		template <typename T>
		static usize Index (ArrayCRef<T> chances)
		{
			typedef typename TypeTraits::RemoveConst<T>		Value_t;

			Value_t f = RG::UNorm< Value_t >();
		
			FOR( i, chances )
			{
				if ( f < chances[i] )
					return i;

				f -= chances[i];
			}
			return chances.Count();
		}

		
		template <typename T, usize I, typename B>
		static usize Index (const T (&chances)[I], B expected)
		{
			return Index( ArrayCRef<T>( chances ), expected );
		}
		

		template <typename T, typename B>
		static usize Index (ArrayCRef<T> chances, B expected)
		{
			typedef typename TypeTraits::RemoveConst<T>		Value_t;

			Value_t f = RG::NormalDistribution::template UNorm< Value_t >( expected );
		
			FOR( i, chances )
			{
				if ( f < chances[i] )
					return i;

				f -= chances[i];
			}
			return chances.Count();
		}
		
		/*
		template <typename T>
		static T Indexed (ArrayCRef<T> values, const usize expectedIndex)
		{
			typedef TRandomGenerator<RndEngine>::NormalDistribution		NormalDistribution;

			const float	idx = NormalDistribution::UNorm( float(expectedIndex) / values.Count() ) * values.Count();
			return values[ Clamp( Round<usize>( idx ), 0, values.LastIndex() ) ];
		}
		*/

		template <typename T>
		static bool Bool (T chanceOfTrue)
		{
			return UNorm<T>() < chanceOfTrue;
		}
	};


}	// GXMath
}	// GX_STL
