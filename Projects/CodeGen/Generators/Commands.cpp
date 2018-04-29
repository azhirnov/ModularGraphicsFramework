// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

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
		if ( flags == ECommandSet::FloatBaseCommands )
			return _GetFloatBaseCommands();

		RETURN_ERR( "unknown command set" );
	}
	
/*
=================================================
	_GetFloatBaseCommands
=================================================
*/
	ArrayCRef<Commands::CommandData>  Commands::_GetFloatBaseCommands ()
	{
		struct CmdToString
		{
			static void Minus	(ArrayCRef<StringCRef> in, String &str)	{ ASSERT( in.Count() == 1 );  str << '-' << in[0]; }
			static void Plus	(ArrayCRef<StringCRef> in, String &str)	{ ASSERT( in.Count() == 1 );  str << in[0]; }
			static void Add		(ArrayCRef<StringCRef> in, String &str)	{ ASSERT( in.Count() == 2 );  str << '(' << in[0] << " + " << in[1] << ')'; }
			static void Sub		(ArrayCRef<StringCRef> in, String &str)	{ ASSERT( in.Count() == 2 );  str << '(' << in[0] << " - " << in[1] << ')'; }
			static void Mul		(ArrayCRef<StringCRef> in, String &str)	{ ASSERT( in.Count() == 2 );  str << '(' << in[0] << " * " << in[1] << ')'; }
			static void Div		(ArrayCRef<StringCRef> in, String &str)	{ ASSERT( in.Count() == 2 );  str << '(' << in[0] << " / " << in[1] << ')'; }

			static void Pow		(ArrayCRef<StringCRef> in, String &str)	{ ASSERT( in.Count() == 2 );  str << "Pow(" << in[0] << ", " << in[1] << ")"; }
			static void Sin		(ArrayCRef<StringCRef> in, String &str)	{ ASSERT( in.Count() == 1 );  str << "Sin(" << in[0] << ')'; }
			static void Cos		(ArrayCRef<StringCRef> in, String &str)	{ ASSERT( in.Count() == 1 );  str << "Cos(" << in[0] << ')'; }
			static void Tan		(ArrayCRef<StringCRef> in, String &str)	{ ASSERT( in.Count() == 1 );  str << "Tan(" << in[0] << ')'; }

			static void Equals	(ArrayCRef<StringCRef> in, String &str)	{ ASSERT( in.Count() == 2 );  str << '(' << in[0] << " == " << in[1] << " ? c_one : -c_one)"; }
			static void Less	(ArrayCRef<StringCRef> in, String &str)	{ ASSERT( in.Count() == 2 );  str << '(' << in[0] <<  " < " << in[1] << " ? c_one : -c_one)"; }
			static void Greater	(ArrayCRef<StringCRef> in, String &str)	{ ASSERT( in.Count() == 2 );  str << '(' << in[0] <<  " > " << in[1] << " ? c_one : -c_one)"; }

			static void Select	(ArrayCRef<StringCRef> in, String &str)	{ ASSERT( in.Count() == 3 );  str << '(' << in[0] << " >= c_zero ? " << in[1] << " : " << in[2] << ')'; }
		};

		struct GPUCmdSource
		{
			static void Minus	(String &str)	{ str << "- args[0]"; }
			static void Plus	(String &str)	{ str << "+ args[0]"; }
			static void Add		(String &str)	{ str << "args[0] + args[1]"; }
			static void Sub		(String &str)	{ str << "args[0] - args[1]"; }
			static void Mul		(String &str)	{ str << "args[0] * args[1]"; }
			static void Div		(String &str)	{ str << "args[0] / args[1]"; }

			static void Pow		(String &str)	{ str << "pow( args[0], args[1] )"; }
			static void Sin		(String &str)	{ str << "sin( args[0] )"; }
			static void Cos		(String &str)	{ str << "cos( args[0] )"; }
			static void Tan		(String &str)	{ str << "tan( args[0] )"; }

			static void Equals	(String &str)	{ str << "args[0] == args[1] ? T(1) : T(-1)"; }
			static void Less	(String &str)	{ str << "args[0] < args[1] ? T(1) : T(-1)"; }
			static void Greater	(String &str)	{ str << "args[0] > args[1] ? T(1) : T(-1)"; }

			static void Select	(String &str)	{ str << "args[0] >= T(0) ? args[1] : args[2]"; }
		};
		
#		define COMMAND( _name_, _args_, _ticks_ ) \
			CommandData( &CmdToString::_name_, &GPUCmdSource::_name_, TOSTRING(_name_), _args_, _ticks_ )

		static const auto	commandsData = MakeStaticArray(
			COMMAND( Minus,		1,	2 ),
			COMMAND( Plus,		1,	1 ),
			COMMAND( Add,		2,	2 ),
			//COMMAND( Sub,		2,	2 ),
			COMMAND( Mul,		2,	4 ),
			COMMAND( Div,		2,	10 ),

			//COMMAND( Pow,		2,	20 ),
			//COMMAND( Ln,		1,	20 ),
			//COMMAND( Sin,		1,	20 ),
			//COMMAND( Cos,		1,	20 ),
			//COMMAND( Tan,		1,	20 ),

			//COMMAND( Equals,	2,	3 ),
			COMMAND( Less,		2,	3 ),
			COMMAND( Greater,	2,	3 ),

			COMMAND( Select,	3,	5 )
		);

		CHECK( IsPowerOfTwo( commandsData.Count() ) );
		return commandsData;
	}

}	// CodeGen
