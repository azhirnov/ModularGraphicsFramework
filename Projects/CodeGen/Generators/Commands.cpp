// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'
/*
	Copy, Copy2, CopyN - helper commands that push argument down in stack.
	For example:
		variant 1:
			arg0 = 1;
			arg1 = 2;
			arg2 = 10;
			arg3 = Add( arg0, arg1 );
			TEST( arg3 == 12 );		// expected 12, got 3

		variant 2:
			arg0 = 1;
			arg1 = 2;
			arg2 = 10;
			arg3 = Copy( arg0 );
			arg4 = Add( arg1, arg2 );
			TEST( arg4 == 12 );		// expected 12, got 12

		variant 3:
			arg0 = 1;
			arg1 = 2;
			arg2 = 10;
			arg3 = Copy2( arg0, arg1 );	// return 'arg1'
			arg4 = Add( arg2, arg3 );
			TEST( arg4 == 12 );		// expected 12, got 12
*/

#include "Generators/Commands.h"

namespace CodeGen
{

/*
=================================================
	GetCommands
=================================================
*/
	ArrayCRef<Commands::CommandData>  Commands::GetCommands (ECommandSet::bits flags)
	{
		if ( flags == ECommandSet::FloatArithmetic )
			return _GetFloatArithmetic();

		if ( flags == ECommandSet::IntArithmetic )
			return _GetIntArithmetic();

		if ( flags == ECommandSet::IntBitwise )
			return _GetIntBitwise();

		RETURN_ERR( "unknown command set" );
	}
	
/*
=================================================
	CmdToAngelScript
=================================================
*/
	struct CmdToAngelScript
	{
		static void Copy	(ArrayCRef<StringCRef> in, String &str)	{ ASSERT( in.Count() == 1 );  str << in[0]; }
		static void Copy2	(ArrayCRef<StringCRef> in, String &str)	{ ASSERT( in.Count() == 2 );  str << in[1]; }
		static void Copy3	(ArrayCRef<StringCRef> in, String &str)	{ ASSERT( in.Count() == 3 );  str << in[2]; }

		static void Minus	(ArrayCRef<StringCRef> in, String &str)	{ ASSERT( in.Count() == 1 );  str << '-' << in[0]; }
		static void Add		(ArrayCRef<StringCRef> in, String &str)	{ ASSERT( in.Count() == 2 );  str << '(' << in[0] << " + " << in[1] << ')'; }
		static void Sub		(ArrayCRef<StringCRef> in, String &str)	{ ASSERT( in.Count() == 2 );  str << '(' << in[0] << " - " << in[1] << ')'; }
		static void Mul		(ArrayCRef<StringCRef> in, String &str)	{ ASSERT( in.Count() == 2 );  str << '(' << in[0] << " * " << in[1] << ')'; }
		static void Div		(ArrayCRef<StringCRef> in, String &str)	{ ASSERT( in.Count() == 2 );  str << '(' << in[0] << " / " << in[1] << ')'; }
		static void DivI	(ArrayCRef<StringCRef> in, String &str)	{ ASSERT( in.Count() == 2 );  str << "(" << in[1] << " != 0 ? " << in[0] << " / " << in[1] << " : c_zero)"; }
		static void ModI	(ArrayCRef<StringCRef> in, String &str)	{ ASSERT( in.Count() == 2 );  str << "(" << in[1] << " != 0 ? " << in[0] << " % " << in[1] << " : c_zero)"; }
		static void ModF	(ArrayCRef<StringCRef> in, String &str)	{ ASSERT( in.Count() == 2 );  str << "Mod(" << in[0] << ", " << in[1] << ')'; }

		static void Pow		(ArrayCRef<StringCRef> in, String &str)	{ ASSERT( in.Count() == 2 );  str << "Pow(" << in[0] << ", " << in[1] << ")"; }
		static void Sin		(ArrayCRef<StringCRef> in, String &str)	{ ASSERT( in.Count() == 1 );  str << "Sin(" << in[0] << ')'; }
		static void Cos		(ArrayCRef<StringCRef> in, String &str)	{ ASSERT( in.Count() == 1 );  str << "Cos(" << in[0] << ')'; }
		static void Tan		(ArrayCRef<StringCRef> in, String &str)	{ ASSERT( in.Count() == 1 );  str << "Tan(" << in[0] << ')'; }
		
		static void Inv		(ArrayCRef<StringCRef> in, String &str)	{ ASSERT( in.Count() == 1 );  str << '~' << in[0]; }
		static void And		(ArrayCRef<StringCRef> in, String &str)	{ ASSERT( in.Count() == 2 );  str << '(' << in[0] << " & " << in[1] << ')'; }
		static void Or		(ArrayCRef<StringCRef> in, String &str)	{ ASSERT( in.Count() == 2 );  str << '(' << in[0] << " | " << in[1] << ')'; }
		static void Xor		(ArrayCRef<StringCRef> in, String &str)	{ ASSERT( in.Count() == 2 );  str << '(' << in[0] << " ^ " << in[1] << ')'; }
		static void LShift	(ArrayCRef<StringCRef> in, String &str)	{ ASSERT( in.Count() == 2 );  str << '(' << in[0] << " << (" << in[1] << "&31)"; }
		static void RShift	(ArrayCRef<StringCRef> in, String &str)	{ ASSERT( in.Count() == 2 );  str << '(' << in[0] << " >> (" << in[1] << "&31)"; }
		static void ILog2	(ArrayCRef<StringCRef> in, String &str)	{ ASSERT( in.Count() == 1 );  str << "IntLog2(" << in[0] << ')'; }

		static void Equals	(ArrayCRef<StringCRef> in, String &str)	{ ASSERT( in.Count() == 2 );  str << '(' << in[0] << " == " << in[1] << " ? c_one : -c_one)"; }
		static void Less	(ArrayCRef<StringCRef> in, String &str)	{ ASSERT( in.Count() == 2 );  str << '(' << in[0] <<  " < " << in[1] << " ? c_one : -c_one)"; }
		static void Greater	(ArrayCRef<StringCRef> in, String &str)	{ ASSERT( in.Count() == 2 );  str << '(' << in[0] <<  " > " << in[1] << " ? c_one : -c_one)"; }

		static void Select	(ArrayCRef<StringCRef> in, String &str)	{ ASSERT( in.Count() == 3 );  str << '(' << in[0] << " >= c_zero ? " << in[1] << " : " << in[2] << ')'; }
	};
	
/*
=================================================
	GPUCmdSource
=================================================
*/
	struct GPUCmdSource
	{
		static void Copy	(String &str)	{ str << "args[0]"; }
		static void Copy2	(String &str)	{ str << "args[1]"; }
		static void Copy3	(String &str)	{ str << "args[2]"; }

		static void Minus	(String &str)	{ str << "- args[0]"; }
		static void Add		(String &str)	{ str << "args[0] + args[1]"; }
		static void Sub		(String &str)	{ str << "args[0] - args[1]"; }
		static void Mul		(String &str)	{ str << "args[0] * args[1]"; }
		static void Div		(String &str)	{ str << "args[0] / args[1]"; }
		static void DivI	(String &str)	{ str << "args[1] != 0 ? args[0] / args[1] : T(0)"; }
		static void ModI	(String &str)	{ str << "args[1] != 0 ? args[0] % args[1] : T(0)"; }
		static void ModF	(String &str)	{ str << "mod( args[0], args[1] )"; }

		static void Pow		(String &str)	{ str << "pow( args[0], args[1] )"; }
		static void Sin		(String &str)	{ str << "sin( args[0] )"; }
		static void Cos		(String &str)	{ str << "cos( args[0] )"; }
		static void Tan		(String &str)	{ str << "tan( args[0] )"; }
		
		static void Inv		(String &str)	{ str << "~args[0]"; }
		static void And		(String &str)	{ str << "args[0] & args[1]"; }
		static void Or		(String &str)	{ str << "args[0] | args[1]"; }
		static void Xor		(String &str)	{ str << "args[0] ^ args[1]"; }
		static void LShift	(String &str)	{ str << "args[0] << (args[1] & 31)"; }
		static void RShift	(String &str)	{ str << "args[0] << (args[1] & 31)"; }
		static void ILog2	(String &str)	{ str << "findMSB( args[0] )"; }

		static void Equals	(String &str)	{ str << "args[0] == args[1] ? T(1) : T(-1)"; }
		static void Less	(String &str)	{ str << "args[0] < args[1] ? T(1) : T(-1)"; }
		static void Greater	(String &str)	{ str << "args[0] > args[1] ? T(1) : T(-1)"; }

		static void Select	(String &str)	{ str << "args[0] >= T(0) ? args[1] : args[2]"; }
	};
		
#	define COMMAND( _name_, _args_, _ticks_ ) \
		CommandData( &CmdToAngelScript::_name_, &GPUCmdSource::_name_, TOSTRING(_name_), _args_, _ticks_ )

/*
=================================================
	_GetFloatArithmetic
=================================================
*/
	ArrayCRef<Commands::CommandData>  Commands::_GetFloatArithmetic ()
	{
		static const auto	commandsData = MakeStaticArray(
			COMMAND( Copy,		1,	0 ),
			COMMAND( Minus,		1,	1 ),
			COMMAND( Add,		2,	1 ),
			COMMAND( Mul,		2,	4 ),
			COMMAND( Div,		2,	10 ),
			COMMAND( Less,		2,	2 ),
			COMMAND( Greater,	2,	2 ),
			COMMAND( Select,	3,	5 )
		);

		STATIC_ASSERT( IsPowerOfTwo( commandsData.Count() ) );
		return commandsData;
	}
	
/*
=================================================
	_GetIntArithmetic
=================================================
*/
	ArrayCRef<Commands::CommandData>  Commands::_GetIntArithmetic ()
	{
		static const auto	commandsData = MakeStaticArray(
			COMMAND( Copy,		1,	0 ),
			COMMAND( Minus,		1,	1 ),
			COMMAND( Inv,		1,	1 ),
			COMMAND( Add,		2,	1 ),
			COMMAND( And,		2,	1 ),
			COMMAND( Or,		2,	1 ),
			COMMAND( Xor,		2,	1 ),
			COMMAND( LShift,	2,	1 ),
			COMMAND( RShift,	2,	1 ),
			COMMAND( ILog2,		1,	4 ),
			COMMAND( Mul,		2,	4 ),
			COMMAND( DivI,		2,	10 ),
			COMMAND( ModI,		2,	12 ),
			COMMAND( Less,		2,	2 ),
			COMMAND( Greater,	2,	2 ),
			COMMAND( Select,	3,	5 )
		);

		STATIC_ASSERT( IsPowerOfTwo( commandsData.Count() ) );
		return commandsData;
	}
	
/*
=================================================
	_GetIntBitwise
=================================================
*/
	ArrayCRef<Commands::CommandData>  Commands::_GetIntBitwise ()
	{
		static const auto	commandsData = MakeStaticArray(
			COMMAND( Copy,		1,	0 ),
			COMMAND( Inv,		1,	1 ),
			COMMAND( And,		2,	1 ),
			COMMAND( Or,		2,	1 ),
			COMMAND( Xor,		2,	1 ),
			COMMAND( LShift,	2,	1 ),
			COMMAND( RShift,	2,	1 ),
			COMMAND( ILog2,		1,	4 )
		);

		STATIC_ASSERT( IsPowerOfTwo( commandsData.Count() ) );
		return commandsData;
	}

}	// CodeGen
