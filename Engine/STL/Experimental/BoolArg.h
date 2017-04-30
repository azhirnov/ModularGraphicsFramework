// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/STL/CompileTime/StringToID.h"

namespace GX_STL
{
namespace GXTypes
{
	DECL_STRING_TO_ID( Bool, 0 );


	//
	// Named Boolean Argument
	//

	template <Bool::type True, Bool::type False, bool DefaultValue = false>
	struct BoolArg
	{
	// types
	public:
		using Self	= BoolArg< True, False >;


	// variables
	private:
		const bool	_value;


	// methods
	public:
		constexpr BoolArg (GX_DEFCTOR) noexcept :				_value( DefaultValue )	{}
		constexpr BoolArg (Self &&other) noexcept :				_value( other._value )	{}
		constexpr BoolArg (const Self &other) noexcept :		_value( other._value )	{}

		constexpr BoolArg (const bool value) noexcept :			_value( value )			{}
		constexpr BoolArg (const Bool::type value) noexcept :	_value( value == True )	{ _Validate( value ); }

		constexpr operator bool () const noexcept				{ return _value; }


	private:
		constexpr bool _Validate (const Bool::type value) const
		{
			return
				(value == True or value == False) ?
					true :
				(throw std::logic_error("unsupported value"));
		}
	};


	/*
		Example:
		
		bool Test (BoolArg<"true"_Bool, "false"_Bool> sync = Uninititalized)
		{
			return sync;
		}

		Test( "true"_Bool );
		Test( Uninititalized );
		Test( true );
		Test();
	*/

}	// GXTypes
}	// GX_STL
