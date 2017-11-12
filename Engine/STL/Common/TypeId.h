// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/STL/Common/Types.h"
#include "Engine/STL/CompileTime/TypeQualifier.h"
#include "Engine/STL/Algorithms/Hash.h"

namespace GX_STL
{
namespace GXTypes
{
	
	namespace _types_hidden_
	{

		//
		// Static Type ID
		//
		struct _StaticTypeID final : public CompileTime::PODType
		{
		private:
			usize	_value;

		public:
			_StaticTypeID () : _value(0) {}

			forceinline bool operator == (_StaticTypeID right) const	{ return _value == right._value; }
			forceinline bool operator != (_StaticTypeID right) const	{ return _value != right._value; }
			forceinline bool operator >  (_StaticTypeID right) const	{ return _value >  right._value; }
			forceinline bool operator <  (_StaticTypeID right) const	{ return _value <  right._value; }
			forceinline bool operator >= (_StaticTypeID right) const	{ return _value >= right._value; }
			forceinline bool operator <= (_StaticTypeID right) const	{ return _value <= right._value; }

			forceinline usize			Get ()	const					{ return _value; }
			forceinline const char *	Name ()	const					{ return ""; }
		};


		template <typename T>
		struct _StaticTypeId
		{
			static _StaticTypeID  Get () noexcept
			{
				static usize id = (usize) &id;
				return (_StaticTypeID const &) id;
			}
		};

		template <typename T>	struct _StaticTypeId< const T > : _StaticTypeId<T> {};
		template <typename T>	struct _StaticTypeId< volatile T > : _StaticTypeId<T> {};
		template <typename T>	struct _StaticTypeId< const volatile T > : _StaticTypeId<T> {};


		//
		// STD Type ID
		//
		struct _StdTypeID final : public CompileTime::PODType
		{
		private:
			enum UnknownType {};

			std::type_index		_value;

		public:
			_StdTypeID () noexcept : _value(typeid(UnknownType)) {}

			_StdTypeID (const std::type_index &value) noexcept : _value(value) {}

			forceinline bool operator == (_StdTypeID right) const	{ return _value == right._value; }
			forceinline bool operator != (_StdTypeID right) const	{ return _value != right._value; }
			forceinline bool operator >  (_StdTypeID right) const	{ return _value >  right._value; }
			forceinline bool operator <  (_StdTypeID right) const	{ return _value <  right._value; }
			forceinline bool operator >= (_StdTypeID right) const	{ return _value >= right._value; }
			forceinline bool operator <= (_StdTypeID right) const	{ return _value <= right._value; }

			forceinline std::type_index Get ()	const				{ return _value; }
			forceinline const char *	Name ()	const				{ return _value.name(); }
		};

		
		template <typename T>
		struct _StdTypeId
		{
			forceinline static _StdTypeID  Get () noexcept
			{
				return _StdTypeID( typeid(T) );
			}
		};
		
#	ifdef GX_USE_STD_TYPEID
		template <typename T>
		using _TypeIdOf	= _types_hidden_::_StdTypeId<T>;
		using _TypeID	= _types_hidden_::_StdTypeID;
#	else
		template <typename T>
		using _TypeIdOf	= _types_hidden_::_StaticTypeId<T>;
		using _TypeID	= _types_hidden_::_StaticTypeID;
#	endif

	}	// _types_hidden_


	using TypeId	= _types_hidden_::_TypeID;

/*
=================================================
	TypeIdOf
=================================================
*/
	template <typename T>
	forceinline static TypeId  TypeIdOf () noexcept
	{
		return _types_hidden_::_TypeIdOf<T>::Get();
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
		using Key_t		= TypeId;
		using Base_t	= Hash< TypeTraits::ResultOf< decltype(&Key_t::Get) > >;
		using Result_t	= Base_t::Result_t;

		Result_t operator () (const Key_t &x) const noexcept
		{
			return Base_t()( x.Get() );
		}
	};
	
}	// GXTypes
}	// GX_STL
