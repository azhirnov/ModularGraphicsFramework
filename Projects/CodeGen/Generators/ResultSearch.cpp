// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Generators/ResultSearch.h"

namespace CodeGen
{
	
/*
=================================================
	ReadBigInt
=================================================
*/
	template <typename T, usize S>
	static void ReadBigInt (BinArrayCRef resData, OUT BigInteger<T,S> &hash)
	{
		const BitsU		step		= BitsU::SizeOf<T>();
		const BitsU		bi_size		= resData.Size().ToBits() - step;

		for (BitsU pos; pos < bi_size; pos += step)
		{
			T	tmp = 0;
			MemCopy( BinArrayRef::FromValue( tmp ),	resData.SubArray(usize(pos.ToBytes()), sizeof(tmp)) );

			hash.Write( tmp, pos, step );
		}
	}

/*
=================================================
	SearchResults
=================================================
*/
	bool ResultSearch::SearchResults (BinArrayCRef results, usize resultsCount, uint maxConstants, uint argsCount, BytesU resultSize,
									  BytesU bigIntSize, const Parameters_t &params, usize maxOutputResults,
									  OUT GenFunctions_t &functions)
	{
		const usize		res_size	= usize(resultSize);

		CHECK_ERR( results.Count() % res_size == 0 );

		functions.Reserve( resultsCount );

		for (uint i = 0; i < resultsCount; ++i)
		{
			BinArrayCRef	res			= results.SubArray( i * res_size, res_size );
			float			ticks		= 0.0f;
			float			accuracy	= 0.0f;
			GenFunction		func;
			
			ReadBigInt( res.SubArray( 0, usize(bigIntSize) ), OUT func.funHash );
			ReadBigInt( res.SubArray( usize(bigIntSize), usize(bigIntSize) ), OUT func.constHash );

			MemCopy( BinArrayRef::FromValue( ticks ),		res.SubArray(usize(bigIntSize*2), sizeof(ticks)) );
			MemCopy( BinArrayRef::FromValue( accuracy ),	res.SubArray(usize(bigIntSize*2 + SizeOf<float>), sizeof(accuracy)) );

			func.ticks			= ulong(ticks);
			func.accuracy		= accuracy;
			func.argsCount		= argsCount;
			func.maxInputs		= argsCount + maxConstants;
			func.SetParams( params );

			ASSERT( func.ticks > 0 );
			ASSERT( func.accuracy >= 0.0 );

			functions.PushBack( func );
		}

		_SearchBestResults( INOUT functions, maxOutputResults );
		return true;
	}
	
/*
=================================================
	_SearchBestResults
=================================================
*/
	void ResultSearch::_SearchBestResults (INOUT GenFunctions_t &functions, usize maxResults, bool withBestAccuracy)
	{
		float		min_accur	= MaxValue<float>();
		float		mid_accur	= 0.0f;
		ulong		min_ticks	= UMax;
		
		GenFunctions_t	new_res;	new_res.Reserve( functions.Count()/2 );

		// find min and mid accuracy
		FOR( i, functions )
		{
			min_accur	= Min( min_accur, functions[i].accuracy );
			min_ticks	= Min( min_ticks, functions[i].ticks );
			mid_accur  += functions[i].accuracy;
		}

		mid_accur = mid_accur / functions.Count();

		LOG( "Min accuracy: "_str << min_accur << ", min ticks: " << min_ticks, ELog::Info );

		mid_accur = Max( _MinAccuracy(), mid_accur );

		// get best functions
		float	max_accur	= Lerp( min_accur, mid_accur, 0.25f );

		FOR( i, functions )
		{
			if ( functions[i].accuracy <= max_accur )
			{
				new_res.PushBack( functions[i] );
			}
		}

		functions = RVREF( new_res );

		if ( withBestAccuracy )
		{
			// best accuracy
			Sort( functions,
				LAMBDA() (const GenFunction &left, const GenFunction &right) -> bool
				{{
					return	left.accuracy != right.accuracy ?	left.accuracy > right.accuracy	:
							left.ticks    != right.ticks	?	left.ticks	  > right.ticks		:
																left.funHash  > right.funHash;
				}}
			);
		}
		else
		{
			// min ticks
			Sort( functions,
				LAMBDA() (const GenFunction &left, const GenFunction &right) -> bool
				{{
					return	left.ticks	  != right.ticks	?	left.ticks	  > right.ticks		:
							left.accuracy != right.accuracy	?	left.accuracy > right.accuracy	:
																left.funHash  > right.funHash;
				}}
			);
		}

		if ( functions.Count() > maxResults )
			functions.Resize( maxResults );
	}

}	// CodeGen
