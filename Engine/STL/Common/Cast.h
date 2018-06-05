// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Types.h"
#include "AllFunc.h"
#include "Engine/STL/CompileTime/TypeTraits.h"
#include "Engine/STL/CompileTime/TemplateMath.h"

namespace GX_STL
{
namespace GXTypes
{
	
/*
=================================================
	PointerCast
=================================================
*/
	// TODO: remove this strange code
	namespace _types_hidden_
	{
		template <typename T, typename B>
		struct _PointerCast
		{};
		
		template <typename T, typename B>
		struct _PointerCast< T *, B >
		{
			using Result_t	= B *;

			forceinline constexpr static Result_t Cast (T * val) {
				return static_cast< Result_t >( static_cast< void * >( val ) );
			}
		};
		
		template <typename T, typename B>
		struct _PointerCast < const T *, B >
		{
			using Result_t	= const B *;

			forceinline constexpr static Result_t Cast (const T * val) {
				return static_cast< Result_t >( static_cast< const void * const >( val ) );
			}
		};
		
		template <typename T, typename B>
		struct _PointerCast< volatile T *, B >
		{
			using Result_t	= volatile B *;

			forceinline constexpr static Result_t Cast (volatile T * val) {
				return static_cast< Result_t >( static_cast< volatile void * >( val ) );
			}
		};
		
		template <typename T, typename B>
		struct _PointerCast < const volatile T *, B >
		{
			using Result_t	= const volatile B *;

			forceinline constexpr static Result_t Cast (const volatile T * val) {
				return static_cast< Result_t >( static_cast< const volatile void * const >( val ) );
			}
		};
	}	// _types_hidden_

	template <typename T, typename B>
	ND_ forceinline constexpr typename _types_hidden_::_PointerCast< B, T >::Result_t  PointerCast (B val)
	{
		STATIC_ASSERT( TypeTraits::IsPointer<B>, "argument must be pointer" );
		return _types_hidden_::_PointerCast< B, T >::Cast( val );
	}
	
/*
=================================================
	PointerSafeCast
=================================================
*/
	template <typename R, typename B>
	ND_ forceinline constexpr typename _types_hidden_::_PointerCast< B, R >::Result_t  PointerSafeCast (B val)
	{
		STATIC_ASSERT(( sizeof(R) == sizeof(TypeTraits::RemovePointer<B>) or CompileTime::IsBaseOf< B, R > ));
		return PointerCast<R,B>( val );
	}

/*
=================================================
	ReferenceCast
=================================================
*/
	template <typename R, typename T>
	ND_ forceinline constexpr const R & ReferenceCast (const T & t)
	{
		STATIC_ASSERT( sizeof(R) == sizeof(T), "type size mismatch" );
		return reinterpret_cast<const R &>( t );
	}
	
	template <typename R, typename T>
	ND_ forceinline constexpr R & ReferenceCast (T & t)
	{
		STATIC_ASSERT( sizeof(R) == sizeof(T), "type size mismatch" );
		return reinterpret_cast<R &>( t );
	}
	
/*
=================================================
	IsAlignedPointer
=================================================
*/
	template <usize AlignInBytes>
	forceinline constexpr bool IsAlignedPointer (void const * const ptr)
	{
		STATIC_ASSERT( (CompileTime::IsPowerOfTwo< usize, AlignInBytes >), "Align must be power of 2" );
		return (ReferenceCast<usize>( ptr ) & (AlignInBytes-1)) == 0;
	}

/*
=================================================
	Cast
=================================================
*/
	namespace _types_hidden_
	{
		template <typename R, typename T, bool IsArithmetic>
		struct _Cast
		{
			forceinline constexpr static R Get (const T& value)
			{
				// return R( value );
				return static_cast<R>( value );
			}

			forceinline constexpr static R Get (T& value)
			{
				// return R( value );
				return static_cast<R>( value );
			}
		};

		template <typename R, typename T>
		struct _Cast< R, T, false >
		{
			forceinline constexpr static R Get (const T& value)
			{
				return static_cast<R>( value );
			}

			forceinline constexpr static R Get (T& value)
			{
				return static_cast<R>( value );
			}
		};
	}	// _types_hidden_

	template <typename R, typename T>
	ND_ forceinline constexpr R Cast (const T& value)
	{
		return _types_hidden_::_Cast< R, T, CompileTime::IsArithmetic<T> and CompileTime::IsArithmetic<R> >::Get( value );
	}
	
	template <typename R, typename T>
	ND_ forceinline constexpr R Cast (T& value)
	{
		return _types_hidden_::_Cast< R, T, CompileTime::IsArithmetic<T> and CompileTime::IsArithmetic<R> >::Get( value );
	}

	template <typename R, typename T>
	ND_ forceinline constexpr R Cast (const T* value)
	{
		return PointerCast< TypeTraits::RemovePointer<R> >( value );
	}
	
	template <typename R, typename T>
	ND_ forceinline constexpr R Cast (T* value)
	{
		return PointerCast< TypeTraits::RemovePointer<R> >( value );
	}


}	// GXTypes
}	// GX_STL
