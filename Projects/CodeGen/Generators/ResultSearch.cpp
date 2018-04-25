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
	
/*
=================================================
	ToSource
=================================================
*/
	bool ResultSearch::ToSource (const GenFunction &func, OUT String &src)
	{
		switch ( func.typeId )
		{
			case "float"_ValueID :	return _ToSource<float>( func, src );
		}

		return true;
	}
	
/*
=================================================
	_ToSource
=================================================
*/
	template <typename T>
	bool ResultSearch::_ToSource (const GenFunction &constFunc, OUT String &src)
	{
		src.Clear();

		Set< uint >		cached_cmd_ids;
		const auto		data			= Commands::GetCommands( constFunc.commandSet );
		const BitsU		bits_per_cmd	= BitsU( IntLog2( data.Count() ) );
		GenFunction		func			= constFunc;
		//const Array<T>	constants		= Commands::GetConstants<T>( constFunc.constants );
		Array<CmdNode>	tmp_nodes;
		Array<CmdNode>	nodes;
		Array<bool>		used_nodes;

		CHECK_ERR( func.inputsCount > 0 );
		CHECK_ERR( _BuildFuncList( data, bits_per_cmd, INOUT func, INOUT tmp_nodes, INOUT nodes ) );

		CHECK( constFunc.ticks == func.ticks );

		src << "// hash: " << func.hash.ToString()
			<< "\n// type: " << ValueID::ToString( func.typeId )
			<< "\n// ticks: " << func.ticks 
			<< "\n// accuracy: " << func.accuracy << "\n\n";

		used_nodes.Resize( nodes.Count() );

		FOR( i, nodes )
		{
			used_nodes[i] = false;

			CmdNode const&	n = nodes[i];

			FOR( j, n.args ) {
				used_nodes[ n.args[j] ] = true;
			}
		}
		used_nodes.Back() = true;

		FOR( i, nodes )
		{
			if ( not used_nodes[i] )
				continue;

			if ( i < func.argsCount )
			{
				src << "T val_" << i << " = in[" << i << "];\n";
				continue;
			}

			if ( i < func.inputsCount )
			{
				TODO( "constants" );
				//src << "T val_" << i << " = " << constants[i - func.argsCount] << ";\n";
				continue;
			}

			CmdNode const&	n	= nodes[i];
			auto const&		cd	= data[ n.id ];

			cached_cmd_ids.Add( n.id );
			
			src << "T val_" << i << " = " << cd.name << "( ";

			FOR( j, n.args ) {
				src << (j > 0 ? ", " : "") << "val_" << n.args[j];
			}
			src << " );\n";
		}

		src << "//--------------\n";

		FOR( i, cached_cmd_ids )
		{
			auto const&	cd	= data[ cached_cmd_ids[i] ];

			cd.toStr( src );
			src << "\n";
		}

		return true;
	}
	
/*
=================================================
	_BuildFuncList
=================================================
*/
	bool ResultSearch::_BuildFuncList (const ArrayCRef<CmdData> data, const BitsU bitsPerCommand,
										INOUT GenFunction &func, INOUT Array<CmdNode> &tmpNodes, INOUT Array<CmdNode> &nodes)
	{
		BitsU	pos;
		usize	arg_idx = 0;

		func.ticks = 0;
		
		// TODO: Optimization: make 'tmpNodes' a fixed size array
		// TODO: Optimization: make 'nodes' a fixed size array

		tmpNodes.Clear();

		do {
			CmdNode	n;
			n.id = (uint) func.hash.Read( pos, bitsPerCommand );

			auto&	cd = data[ n.id ];

			pos			+= bitsPerCommand;
			func.ticks	+= cd.ticks;

			arg_idx = Max( arg_idx, tmpNodes.Count() );

			n.args.Resize( cd.args );

			FOR_rev( j, n.args )
			{
				n.args[j] = uint(++arg_idx);
			}

			tmpNodes.PushBack( n );
		}
		while ( not func.hash.IsZero( pos ) );

		if ( tmpNodes.Empty() )
			return false;


		// build nodes
		nodes.Resize( func.inputsCount );
		
		const usize	cnt	= tmpNodes.LastIndex();

		FOR_rev( i, tmpNodes )
		{
			CmdNode &	n = tmpNodes[i];
			
			FOR( j, n.args )
			{
				usize	idx = n.args[j];

				if ( idx <= cnt )
					idx = (cnt - idx) + func.inputsCount;		// from node
				else
					idx = (arg_idx - idx) % func.inputsCount;	// from input

				ASSERT( idx < nodes.Count() );

				n.args[j] = uint(idx);
			}

			nodes.PushBack( n );
		}

		return true;
	}

}	// CodeGen
