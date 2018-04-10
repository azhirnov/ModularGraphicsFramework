// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/STL/Common/Init.h"
#include "Engine/STL/CompileTime/OldTypeInfo.h"

namespace GX_STL
{
namespace GXTypes
{

	//
	// Bool
	//

	template <typename T>
	struct Bool
	{
	// types
	public:
		using Value_t	= T;
		using Self		= Bool< T >;


	// variables
	private:
		T		_value	= 0;


	// methods
	public:
		Bool () {}
		
		Bool (Bool &&) = default;
		Bool (const Bool &) = default;

		Bool (bool value) : _value{ value ? 1 : 0 }	{}

		explicit Bool (T value) : _value{ value }	{}

		operator bool () const							{ return !!_value; }

		//bool	operator ! () const						{ return !_value; }

		Self &	operator = (const Self &right)			{ _value = right._value;  return *this; }
		Self &	operator = (bool right)					{ _value = (right ? 1 : 0);  return *this; }
		Self &	operator = (T right)					{ _value = right;  return *this; }

		bool	operator == (const Self &right)	const	{ return bool(*this) == bool(right); }
		//bool	operator == (bool right) const			{ return bool(*this) == right; }

		bool	operator != (const Self &right) const	{ return not (*this == right); }
		bool	operator >  (const Self &right) const	{ return bool(*this) > bool(right); }
		bool	operator <  (const Self &right) const	{ return bool(*this) < bool(right); }
		bool	operator >= (const Self &right) const	{ return bool(*this) >= bool(right); }
		bool	operator <= (const Self &right) const	{ return bool(*this) <= bool(right); }
	};

}	// GXTypes

namespace CompileTime
{
namespace TypeDescriptor
{
	template <typename T>
	struct IsScalar< GXTypes::Bool<T> > {
		static const bool	value = true;
	};

}	// TypeDescriptor
}	// CompileTime
}	// GX_STL
