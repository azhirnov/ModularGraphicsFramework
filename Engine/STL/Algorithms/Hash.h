// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/STL/Common/Types.h"
#include "Engine/STL/CompileTime/TypeTraits.h"
#include <functional>

namespace GX_STL
{

// BitRotateLeft depends of Bits
// Bits depends of Hash
// hidden _BitRotateLeft not depends of this
namespace GXMath
{
namespace _math_hidden_
{
	template <typename T>
	forceinline T _BitRotateLeft (T value, usize shift);

}	// _math_hidden_
}	// GXMath

namespace GXTypes
{

	//
	// Hash Result
	//

	struct HashResult
	{
	// types
	private:
		typedef HashResult		Self;
		typedef usize			value_t;


	// variables
	private:
		usize	_value;
		

	// methods
	public:
		HashResult (GX_DEFCTOR) : _value(0) {}
		
		constexpr explicit HashResult (usize val) : _value(val) {}


		value_t  Get () const
		{
			return _value;
		}

		Self operator ~ () const
		{
			return Self( ~_value );
		}

		Self operator + (const Self &right) const
		{
			return Self(*this) += right;
		}

		Self& operator += (const Self &right)
		{
			_value ^= GXMath::_math_hidden_::_BitRotateLeft( right._value, 1 );
			return *this;
		}


		_GX_DIM_CMP_OPERATORS_SELF( _value );
	};
	

	namespace _types_hidden_
	{
		inline static HashResult HashForMemoryBlock (const ubyte *ptr, usize count)
		{
			// MS Visual C++ std implementation
			#if defined(COMPILER_MSVC) and defined(_HASH_SEQ_DEFINED)
				return (HashResult) std::_Hash_seq( ptr, count );
			#else
				#error "hash function not defined!"
			#endif
		}
		
	}	// _types_hidden_



	//
	// Hash function
	//

	template <typename T>
	struct Hash
	{
		typedef HashResult	result_t;

		typedef typename TypeTraits::RemoveConstVolatile< T >	key_t;

		result_t operator () (const key_t &x) const
		{
			return (result_t) std::hash< key_t >()( x );
		}
	};


	
/*
=================================================
	HashOf
=================================================
*/
	template <typename T>
	inline HashResult  HashOf (const T &x)
	{
		return Hash<T>()( x );
	}


}	// GXTypes
}	// GX_STL
