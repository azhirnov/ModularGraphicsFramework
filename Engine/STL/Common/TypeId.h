// Copyright © 2014-2017  Zhirnov Andrey. All rights reserved.

#pragma once

#include "Engine/STL/Common/Types.h"
#include "Engine/STL/CompileTime/TypeQualifier.h"
#include "Engine/STL/Algorithms/Hash.h"
#include <typeindex>

namespace GX_STL
{
namespace GXTypes
{
	
	//
	// Type ID
	//

	namespace _types_hidden_
	{

#	if 0
		struct _TypeID : public CompileTime::PODType
		{
		private:
			usize	_value;

		public:
			_TypeID () : _value(0) {}

			forceinline bool operator == (_TypeID right) const	{ return _value == right._value; }
			forceinline bool operator != (_TypeID right) const	{ return _value != right._value; }
			forceinline bool operator >  (_TypeID right) const	{ return _value >  right._value; }
			forceinline bool operator <  (_TypeID right) const	{ return _value <  right._value; }
			forceinline bool operator >= (_TypeID right) const	{ return _value >= right._value; }
			forceinline bool operator <= (_TypeID right) const	{ return _value <= right._value; }

			forceinline usize			Get ()	const			{ return _value; }
			forceinline const char *	Name ()	const			{ return ""; }
		};


		template <typename T>
		struct _TypeId
		{
			static _TypeID  Get ()
			{
				static usize id = (usize) &id;
				return (_TypeID const &) id;
			}
		};

#	else

		struct _TypeID : public CompileTime::PODType
		{
		private:
			enum UnknownType {};

			std::type_index		_value;

		public:
			_TypeID () noexcept : _value(typeid(UnknownType)) {}

			_TypeID (const std::type_index &value) noexcept : _value(value) {}

			forceinline bool operator == (_TypeID right) const	{ return _value == right._value; }
			forceinline bool operator != (_TypeID right) const	{ return _value != right._value; }
			forceinline bool operator >  (_TypeID right) const	{ return _value >  right._value; }
			forceinline bool operator <  (_TypeID right) const	{ return _value <  right._value; }
			forceinline bool operator >= (_TypeID right) const	{ return _value >= right._value; }
			forceinline bool operator <= (_TypeID right) const	{ return _value <= right._value; }

			forceinline std::type_index Get ()	const			{ return _value; }
			forceinline const char *	Name ()	const			{ return _value.name(); }
		};

		
		template <typename T>
		struct _TypeId
		{
			forceinline static _TypeID  Get ()
			{
				return _TypeID( typeid(T) );
			}
		};

#	endif

	}	// _types_hidden_


	typedef _types_hidden_::_TypeID		TypeId;

	
/*
=================================================
	TypeIdOf
=================================================
*/
	template <typename T>
	forceinline static TypeId  TypeIdOf () noexcept
	{
		return _types_hidden_::_TypeId<T>::Get();
	}

	template <typename T>
	forceinline static TypeId  TypeIdOf (const T&) noexcept
	{
		return TypeIdOf<T>();
	}
	
	
/*
=================================================
	Hash
=================================================
*/
	template <>
	struct Hash< TypeId >
	{
		typedef TypeId													key_t;
		typedef Hash< TypeTraits::ResultOf< decltype(&key_t::Get) > >	base_t;
		typedef base_t::result_t										result_t;

		result_t operator () (const key_t &x) const
		{
			return base_t()( x.Get() );
		}
	};
	
}	// GXTypes
}	// GX_STL
