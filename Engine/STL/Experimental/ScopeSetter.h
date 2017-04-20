// Copyright © 2014-2017  Zhirnov Andrey. All rights reserved.

#pragma once

#include "Engine/STL/Types/Noncopyable.h"

namespace GX_STL
{
namespace GXTypes
{

	//
	// Scope Setter
	//

	template <typename T>
	struct ScopeSetter : Noncopyable
	{
	// variables
	private:
		T &			_ref;
		const T		_value;


	// methods
	public:
		ScopeSetter (INOUT T &ref, const T &value) : _ref( ref ), _value( value )
		{}

		~ScopeSetter ()
		{
			_ref = _value;
		}
	};
	
#	define SCOPE_SETTER( _valueRef_, _newValue_ ) \
		GX_STL::OS::ScopeSetter< TypeTraits::RemoveAnyReference< decltype(_valueRef_) > >	AUXDEF_UNITE_RAW( __scopeSetter, __COUNTER__ ) ( _valueRef_, _newValue_ )


}	// GXTypes
}	// GX_STL
