// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Generators/ResultSearch.h"

namespace CodeGen
{
	
/*
=================================================
	SearchResults
=================================================
*/
	bool ResultSearch::SearchResults (BinArrayCRef results, usize resultsCount, uint maxInputs, BytesU resultSize,
									  BytesU bigIntSize, ValueID::type typeId, ECommandSet::bits commandSet, EConstantSet::bits constantSet,
									  OUT GenFunctions_t &functions)
	{
		const usize		res_size	= usize(resultSize);
		const BitsU		step		= BitsU::SizeOf<uint>();
		const BitsU		bi_size		= bigIntSize.ToBits() - step;

		CHECK_ERR( results.Count() % res_size == 0 );

		functions.Reserve( resultsCount );

		for (uint i = 0; i < resultsCount; ++i)
		{
			BinArrayCRef	res			= results.SubArray( i * res_size, res_size );
			float			ticks		= 0.0f;
			float			accuracy	= 0.0f;
			GenFunction		func;
			
			for (BitsU pos; pos < bi_size; pos += step)
			{
				uint	tmp = 0;
				MemCopy( BinArrayRef::FromValue( tmp ),	res.SubArray(usize(pos.ToBytes()), sizeof(tmp)) );

				func.hash.Write( tmp, pos, step );
			}

			MemCopy( BinArrayRef::FromValue( ticks ),		res.SubArray(usize(bigIntSize), sizeof(ticks)) );
			MemCopy( BinArrayRef::FromValue( accuracy ),	res.SubArray(usize(bigIntSize + SizeOf<float>), sizeof(accuracy)) );

			func.ticks			= ulong(ticks);
			func.accuracy		= accuracy;
			func.inputsCount	= maxInputs;	// TODO: add constants
			func.argsCount		= maxInputs;
			func.typeId			= typeId;
			func.commandSet		= commandSet;
			func.constants		= constantSet;

			ASSERT( func.ticks > 0 );
			ASSERT( func.accuracy >= 0.0 );

			functions.PushBack( func );
		}

		_SearchBestResults( INOUT functions );
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
		
		GenFunctions_t	new_res;	new_res.Reserve( functions.Count()/2 );

		// find min and mid accuracy
		FOR( i, functions )
		{
			min_accur  = Min( min_accur, functions[i].accuracy );
			mid_accur += functions[i].accuracy;
		}

		mid_accur = Max( _MinAccuracy(), mid_accur / functions.Count() );

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
					return left.accuracy != right.accuracy ?	left.accuracy > right.accuracy :
																left.ticks    > right.ticks;
				}}
			);
		}
		else
		{
			// min ticks
			Sort( functions,
				LAMBDA() (const GenFunction &left, const GenFunction &right) -> bool
				{{
					return left.ticks != right.ticks ?	left.ticks    > right.ticks :
														left.accuracy > right.accuracy;
				}}
			);
		}

		if ( functions.Count() > maxResults )
			functions.Resize( maxResults );
	}

}	// CodeGen
