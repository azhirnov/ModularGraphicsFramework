// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/STL/CompileTime/TypeListHelpers.h"
#include "Engine/STL/Algorithms/ArrayUtils.h"

namespace GX_STL
{
namespace GXTypes
{
	namespace _types_hidden_
	{

		//
		// _Tuple2
		//
		template <typename Typelist, usize InvIndex>
		struct _Tuple2 : _Tuple2< Typelist, InvIndex-1 >
		{
		// types
		private:
			static constexpr usize	Index = Typelist::Count-1-InvIndex;

			using type	= typename Typelist::template Get< Index >;
			using Base	= _Tuple2< Typelist, InvIndex-1 >;


		// variables
		private:
			type	_value;


		// methods
		public:
			_Tuple2 () : _value{} {}

			_Tuple2 (_Tuple2 &&) = default;
			_Tuple2 (const _Tuple2 &) = default;

			_Tuple2& operator = (_Tuple2 &&) = default;
			_Tuple2& operator = (const _Tuple2 &) = default;

			template <typename T, typename ...Args>
			explicit _Tuple2 (T&& arg0, Args&&... args) : Base{ FW<Args>(args)... }, _value{FW<T>(arg0)} {}

			template <usize I>
			auto const&  Get () const
			{
				if_constexpr( I == Index )
					return _value;
				else
					return Base::template Get<I>();
			}

			template <usize I, typename T>
			void Set (T &&value)
			{
				if_constexpr( I == Index )
					_value = FW<T>(value);
				else
					Base::template Set<I>( FW<T>(value) );
			}
		};
		

		//
		// _Tuple2
		//
		template <typename Typelist>
		struct _Tuple2< Typelist, 0 >
		{
		// types
		private:
			static constexpr usize	Index = Typelist::Count-1;

			using type	= typename Typelist::template Get< Index >;


		// variables
		private:
			type	_value;


		// methods
		public:
			_Tuple2 () : _value{} {}

			_Tuple2 (_Tuple2 &&) = default;
			_Tuple2 (const _Tuple2 &) = default;

			_Tuple2& operator = (_Tuple2 &&) = default;
			_Tuple2& operator = (const _Tuple2 &) = default;

			explicit _Tuple2 (type&& arg) : _value{FW<type>(arg)} {}

			template <usize I>
			auto const&  Get () const
			{
				if_constexpr( I == Index )
					return _value;
			}

			template <usize I, typename T>
			void Set (T &&value)
			{
				if_constexpr( I == Index )
					_value = FW<T>(value);
			}
		};
		

		//
		// _Tuple2
		//
		template <typename Typelist>
		struct _Tuple2< Typelist, ~usize(0) >
		{
			STATIC_ASSERT( Typelist::Empty );

			// empty
		};


		//
		// _TupleTL
		//
		template <typename Typelist>
		struct _TupleTL : _Tuple2< Typelist, Typelist::Count-1 >
		{
			STATIC_ASSERT( CompileTime::IsTypeList< Typelist > );

		// types
		private:
			using Base			= _Tuple2< Typelist, Typelist::Count-1 >;
			
		public:
			using Typelist_t	= Typelist;


		// methods
		public:
			_TupleTL (GX_DEFCTOR) {}

			_TupleTL (_TupleTL &&) = default;
			_TupleTL (const _TupleTL &) = default;

			_TupleTL& operator = (_TupleTL &&) = default;
			_TupleTL& operator = (const _TupleTL &) = default;

			template <typename ...Args>
			explicit _TupleTL (Args&&... args) : Base{ FW<Args>(args)... }
			{
				STATIC_ASSERT( sizeof...(Args) == Typelist_t::Count );
			}

			template <typename T>
			auto const&  GetByType () const
			{
				return this->template Get< typename Typelist::template IndexOf<T> >();
			}

			template <typename T>
			void SetByType (T &&value)
			{
				return this->template Set< typename Typelist::template IndexOf<T> >( FW<T>(value) );
			}

			constexpr usize Count () const
			{
				return Typelist_t::Count;
			}
		};
		

		template <bool IsTypelist, typename T0, typename ...Types>
		struct _TupleSwitch
		{
			using type = _TupleTL< CompileTime::TypeListFrom< T0, Types... > >;
		};
		
		template <typename T0>
		struct _TupleSwitch< true, T0 >
		{
			using type = _TupleTL< T0 >;
		};

	}	// _types_hidden_



	//
	// Tuple
	//
	
	template <typename T0, typename ...Types>
	using Tuple = typename _types_hidden_::_TupleSwitch< CompileTime::IsTypeList< T0 >, T0, Types... >::type;


}	// GXTypes
}	// GX_STL
