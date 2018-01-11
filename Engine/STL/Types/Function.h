// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'
/*
	Wrapper for std::function.
*/

#pragma once

#include "Engine/STL/CompileTime/TypeTraits.h"

namespace GX_STL
{
namespace GXTypes
{

	//
	// Function
	//
	
	template <typename T>
	struct Function
	{
	// types
	public:
		typedef Function<T>						Self;
		typedef std::function< T () >			Func_t;
		typedef T								Result_t;
		//typedef typename Func_t::result_type	Result_t;


	// variables
	private:
		Func_t	_func;


	// methods
	public:
		Function (GX_DEFCTOR) noexcept : _func()					{}
		Function (NullPtr_t value) noexcept : _func(value)			{}
	
		template <typename FN>
		Function (FN value) noexcept : _func(value)					{}

		Function (const Func_t &fn) noexcept : _func(fn)			{}
		Function (Func_t &&fn) noexcept : _func(RVREF(fn))			{}

		Function (const Self &other) noexcept : _func(other._func)	{}
		Function (Self &&other) noexcept : _func(RVREF(other._func)){}

		forceinline Self& operator = (const Self &) = default;
		forceinline Self& operator = (Self &&) = default;

		forceinline Self& operator = (const Func_t &other) noexcept	{ _func = other;		return *this; }
		forceinline Self& operator = (Func_t &&other) noexcept		{ _func = RVREF(other);	return *this; }

		forceinline void Swap (INOUT Self &other)					{ _func.swap( other._func ); }
		forceinline void Swap (INOUT Func_t &other)					{ _func.swap( other._func ); }

		forceinline explicit operator bool ()	const				{ return bool(_func); }

		forceinline Result_t	operator () ()	const noexcept		{ return _func(); }

		forceinline bool		IsValid ()		const				{ return bool(_func); }

		forceinline Result_t	Call ()			const noexcept		{ ASSERT( IsValid() );  return _func(); }

		forceinline Result_t	SafeCall ()		const noexcept		{ return IsValid() ? _func() : Result_t(); }
		
		forceinline TypeId		GetType ()		const				{ return _func.target_type(); }
	};


	
/*
=================================================
	helpers
=================================================
*/
	namespace _types_hidden_
	{
		template <typename T1, typename T2>
		struct FB_IsRawTypesSame {
			using Left	= TypeTraits::RemoveConstVolatile< T1 >;
			using Right	= TypeTraits::RemoveConstVolatile< T2 >;

			static const bool	value = CompileTime::IsSameTypes< Left, Right > or
										CompileTime::IsBaseOf< Left, Right > or
										CompileTime::IsBaseOf< Right, Left >;
		};

		template <typename T, typename C, bool IsPtr>
		struct FB_GetBaseClass {
			static const bool	value = FB_IsRawTypesSame< TypeTraits::RemovePointer<T>, C >::value;
		};

		template <typename T, typename C>
		struct FB_GetBaseClass< T, C, false > {
			static const bool	value = FB_IsRawTypesSame< TypeTraits::RemovePointer<typename T::Value_t>, C >::value;
		};
		
		template <typename T, typename C>
		constexpr bool FB_IsSame = FB_GetBaseClass< T, C, TypeTraits::IsPointer<T> >::value;

	}	// _types_hidden_
	
/*
=================================================
	FunctionBuilder
=================================================
*/
	template <typename C, typename Class, typename Res, typename ...Args, typename ...Args2>
	forceinline Function<Res>  FunctionBuilder (const C &cl, Res (Class::* fn) (Args...), Args2&&... args) noexcept
	{
		STATIC_ASSERT(( _types_hidden_::FB_IsSame< C, Class > ));
		return std::bind( fn, cl, FW<Args2>(args)... );
	}
	
	template <typename C, typename Class, typename Res, typename ...Args, typename ...Args2>
	forceinline Function<Res>  FunctionBuilder (const C &cl, Res (Class::* fn) (Args...) const, Args2&&... args) noexcept
	{
		STATIC_ASSERT(( _types_hidden_::FB_IsSame< C, Class > ));
		return std::bind( fn, cl, FW<Args2>(args)... );
	}
	
	template <typename Res, typename ...Args, typename ...Args2>
	forceinline Function<Res>  FunctionBuilder (Res (*fn) (Args...), Args2&&... args) noexcept
	{
		return std::bind( fn, FW<Args2>(args)... );
	}


}	// GXTypes
}	// GX_STL
