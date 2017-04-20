// Copyright © 2014-2017  Zhirnov Andrey. All rights reserved.

#pragma once

#include "Engine/STL/Common/Types.h"
#include "Engine/STL/CompileTime/NewTypeInfo.h"
#include "Engine/STL/Dimensions/ByteAndBit.h"
#include "Engine/STL/Common/Cast.h"

namespace GX_STL
{
namespace GXTypes
{
	
/*
=================================================
	helpers
=================================================
*/
	namespace _types_hidden_
	{
		template <typename T, typename R>
		struct _PtrMove
		{
		};
		
		template <typename T, typename R>
		struct _PtrMove< T *, R >
		{
			typedef R *	result_t;

			static result_t  Move (T * val, isize offset) {
				return PointerCast< R >( PointerCast< ubyte >( val ) + offset );
			}

			static result_t  Move (T * val, usize offset) {
				return PointerCast< R >( PointerCast< ubyte >( val ) + offset );
			}
		};
		
		template <typename T, typename R>
		struct _PtrMove < const T *, R >
		{
			typedef const R *	result_t;

			static result_t  Move (const T * val, isize offset) {
				return PointerCast< R >( PointerCast< ubyte >( val ) + offset );
			}

			static result_t  Move (const T * val, usize offset) {
				return PointerCast< R >( PointerCast< ubyte >( val ) + offset );
			}
		};
	}


	namespace UnsafeMem
	{
/*
=================================================
	MovePointer
=================================================
*/
		template <typename R, typename T>
		forceinline typename _types_hidden_::_PtrMove< T, R >::result_t  MovePointer (T ptr, BytesI offset)
		{
			STATIC_ASSERT( TypeTraits::IsPointer<T>, "argument must be pointer" );
			return _types_hidden_::_PtrMove< T, R >::Move( ptr, (isize)offset );
		}

		template <typename R, typename T>
		forceinline typename _types_hidden_::_PtrMove< T, R >::result_t  MovePointer (T ptr, BytesU offset)
		{
			STATIC_ASSERT( TypeTraits::IsPointer<T>, "argument must be pointer" );
			return _types_hidden_::_PtrMove< T, R >::Move( ptr, (usize)offset );
		}

		template <typename T>
		forceinline T  MovePointer (T ptr, BytesI offset)
		{
			return MovePointer< TypeTraits::RemovePointer< T >, T >( ptr, offset );
		}

		template <typename T>
		forceinline T  MovePointer (T ptr, BytesU offset)
		{
			return MovePointer< TypeTraits::RemovePointer< T >, T >( ptr, offset );
		}

	}	// UnsafeMem
	


}	// GXTypes
}	// GX_STL

