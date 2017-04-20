// Copyright © 2014-2017  Zhirnov Andrey. All rights reserved.

#pragma once


// for VA args
#define AUXDEF_GETARG_0( _0_, ... )								_0_
#define AUXDEF_GETARG_1( _0_, _1_, ... )						_1_
#define AUXDEF_GETARG_2( _0_, _1_, _2_, ... )					_2_
#define AUXDEF_GETARG_3( _0_, _1_, _2_, _3_, ... )				_3_
#define AUXDEF_GETARG_4( _0_, _1_, _2_, _3_, _4_, ... )			_4_
#define AUXDEF_GETARG_5( _0_, _1_, _2_, _3_, _4_, _5_, ... )	_5_

#define AUXDEF_GETARG_0_STR( _0_, ... )							TOSTRING( _0_ )
#define AUXDEF_GETARG_1_STR( _0_, _1_, ... )					TOSTRING( _1_ )
#define AUXDEF_GETARG_2_STR( _0_, _1_, _2_, ... )				TOSTRING( _2_ )

// get values from macros
#define AUXDEF_GETRAW( _value_ )				_value_


// unite
#define AUXDEF_UNITE_RAW( _arg0_, _arg1_ )		AUXDEF_GETRAW( _arg0_ ) ## AUXDEF_GETRAW( _arg1_ )
#define AUXDEF_UNITE( _arg0_, _arg1_ )			_arg0_ ## _arg1_


// Call function for each argument in variable template parameters
// from http://stackoverflow.com/questions/17339789/how-to-call-a-function-on-all-variadic-template-args
namespace GX_STL
{
	namespace _defines_hidden_
	{
		struct __ExpandType
		{
			template <typename... T>
			__ExpandType (T&&...)
			{}
		};
	}
}

// Example:
// void function (T&& value);
// GX_FOREACH_ARG( function( args ) );
#define GX_FOREACH_ARG( _pattern_ ) ::GX_STL::_defines_hidden_::__ExpandType{ 0, ((_pattern_), void(), 0 )... }
