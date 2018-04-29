// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Generators/GenFunctionSerializer.h"

namespace CodeGen
{
	
/*
=================================================
	ToSource
=================================================
*/
	bool GenFunctionSerializer::ToSource (const GenFunction &func, OUT String &src)
	{
		switch ( func.typeId )
		{
			case "float"_ValueID :		return _ToAngelScript<float>( func, src );
			case "double"_ValueID :		return _ToAngelScript<float>( func, src );
			case "int"_ValueID :		return _ToAngelScript<float>( func, src );
			case "scalarf"_ValueID :	return _ToAngelScript<float>( func, src );
		}

		RETURN_ERR( "unsupported type" );
	}
	
/*
=================================================
	_ToAngelScript
=================================================
*/
	template <typename T>
	bool GenFunctionSerializer::_ToAngelScript (const GenFunction &constFunc, OUT String &src)
	{
		src.Clear();

		const auto		data			= Commands::GetCommands( constFunc.commandSet );
		const BitsU		bits_per_cmd	= BitsU( IntLog2( data.Count() ) );
		GenFunction		func			= constFunc;
		//const Array<T>	constants	= Commands::GetConstants<T>( constFunc.constants );
		NodeArray_t		tmp_nodes;
		NodeArray_t		nodes;
		Array<bool>		used_nodes;
		ArgStrings_t	args_str;

		CHECK_ERR( func.inputsCount > 0 );
		CHECK_ERR( _BuildFuncList( data, bits_per_cmd, INOUT func, INOUT tmp_nodes, INOUT nodes ) );

		CHECK( constFunc.ticks == func.ticks );

		String	type_name = StringCRef(ValueID::ToString( func.typeId ));
		{
			FOR( i, type_name ) {
				type_name[i] = StringUtils::ToLower( type_name[i] );
			}

			usize	pos = UMax;
			if ( type_name.Find( ' ', OUT pos ) )
				type_name = type_name.SubString( 0, pos );
		}

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

		src << type_name << " main (";

		usize	last_node = UMax;

		FOR( i, nodes )
		{
			CmdNode &	n = nodes[i];

			if ( not used_nodes[i] )
				continue;

			if ( i < func.argsCount )
			{
				n.src << "arg" << i; 
				src << (i ? ", " : "") << type_name << ' ' << n.src;
				continue;
			}

			if ( i == func.argsCount )
			{
				src << ")\n{";
			}

			if ( i < func.inputsCount )
			{
				TODO( "constants" );
				//src << "T val_" << i << " = " << constants[i - func.argsCount] << ";\n";
				continue;
			}

			args_str.Clear();
			FOR( j, n.args ) {
				args_str << nodes[ n.args[j] ].src;
			}
			
			data[ n.id ].toStr( args_str, OUT n.src );

			last_node = i;
		}

		StringCRef	func_src = nodes[last_node].src;

		src << "\n\t// hash:     " << func.hash.ToString()
			<< "\n\t// accuracy: " << func.accuracy
			<< "\n\t// ticks:    " << func.ticks
			<< "\n\t// commands: " << (used_nodes.Count() - func.inputsCount);
		
		if ( func_src.HasSubString( "c_zero" ) )
			src << "\n\tconst " << type_name << " c_zero = " << type_name << "(0);";

		if ( func_src.HasSubString( "c_one" ) )
			src << "\n\tconst " << type_name << " c_one = " << type_name << "(1);";

		src << "\n\treturn " << func_src << "\n}\n\n";

		return true;
	}
	
/*
=================================================
	_BuildFuncList
=================================================
*/
	bool GenFunctionSerializer::_BuildFuncList (const ArrayCRef<CmdData> data, const BitsU bitsPerCommand,
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
