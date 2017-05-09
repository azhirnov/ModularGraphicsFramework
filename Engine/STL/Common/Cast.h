// Copyright �  Zhirnov Andrey. For more information see 'LICENSE.txt'

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
			typedef B *	result_t;

			forceinline constexpr static result_t Cast (T * val) {
				return static_cast< result_t >( static_cast< void * >( val ) );
			}
		};
		
		template <typename T, typename B>
		struct _PointerCast < const T *, B >
		{
			typedef const B *	result_t;

			forceinline constexpr static result_t Cast (const T * val) {
				return static_cast< result_t >( static_cast< const void * const >( val ) );
			}
		};
	}	// _types_hidden_

	template <typename T, typename B>
	forceinline constexpr typename _types_hidden_::_PointerCast< B, T >::result_t  PointerCast (B val)
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
	forceinline constexpr typename _types_hidden_::_PointerCast< B, R >::result_t  PointerSafeCast (B val)
	{
		STATIC_ASSERT( sizeof(R) == sizeof(TypeTraits::RemovePointer<B>) );
		return PointerCast<R,B>( val );
	}

/*
=================================================
	ReferenceCast
=================================================
*/
	template <typename R, typename T>
	forceinline constexpr const R & ReferenceCast (const T & t)
	{
		STATIC_ASSERT( sizeof(R) == sizeof(T), "type size mismatch" );
		return reinterpret_cast<const R &>( t );
	}
	
	template <typename R, typename T>
	forceinline constexpr R & ReferenceCast (T & t)
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
	forceinline constexpr R Cast (const T& value)
	{
		return _types_hidden_::_Cast< R, T, CompileTime::IsArithmetic<T> and CompileTime::IsArithmetic<R> >::Get( value );
	}
	
	template <typename R, typename T>
	forceinline constexpr R Cast (T& value)
	{
		return _types_hidden_::_Cast< R, T, CompileTime::IsArithmetic<T> and CompileTime::IsArithmetic<R> >::Get( value );
	}


}	// GXTypes
}	// GX_STL