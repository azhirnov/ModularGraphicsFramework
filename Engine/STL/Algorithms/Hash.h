// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/STL/Common/Types.h"
#include "Engine/STL/CompileTime/TypeTraits.h"
#include "Engine/STL/CompileTime/TypeQualifier.h"
#include "Engine/STL/Defines/OperatorHelpers.h"

namespace GX_STL
{

// BitRotateLeft depends of Bits
// Bits depends of Hash
// hidden _BitRotateLeft not depends of both
namespace GXMath
{
namespace _math_hidden_
{
	template <typename T>
	forceinline constexpr T _BitRotateLeft (T value, GXTypes::usize shift);

}	// _math_hidden_
}	// GXMath

namespace GXTypes
{

	//
	// Hash Result
	//

	struct HashResult final : public CompileTime::PODStruct
	{
	// types
	private:
		using Self		= HashResult;
		using Value_t	= usize;


	// variables
	private:
		usize	_value;
		

	// methods
	public:
		HashResult (GX_DEFCTOR) : _value(0) {}
		
		constexpr explicit HashResult (usize val) : _value(val) {}


		CHECKRES Value_t  Get () const noexcept
		{
			return _value;
		}

		CHECKRES Self operator ~ () const noexcept
		{
			return Self( ~_value );
		}

		CHECKRES Self operator + (const Self &right) const noexcept
		{
			return Self(*this) += right;
		}

		Self& operator += (const Self &right) noexcept
		{
			_value ^= GXMath::_math_hidden_::_BitRotateLeft( right._value, 1 );
			return *this;
		}


		_GX_DIM_CMP_OPERATORS_SELF( _value );
	};
	

	namespace _types_hidden_
	{
		inline static HashResult HashForMemoryBlock (const ubyte *ptr, usize count) noexcept
		{
			// MS Visual C++ std implementation
			#if defined(COMPILER_MSVC)
			# if defined(_HASH_SEQ_DEFINED)
				return (HashResult) std::_Hash_seq( ptr, count );
			# elif COMPILER_VERSION >= 1911
				return (HashResult) std::_Hash_bytes( ptr, count );
			# endif
			#elif defined(COMPILER_GCC)
				return (HashResult) std::_Hash_bytes( ptr, count, 0 );
			#elif defined(COMPILER_CLANG)
				return (HashResult) std::__murmur2_or_cityhash<size_t>()( ptr, count );
			#else
				#error "hash function not defined!"
			#endif
		}
		
	}	// _types_hidden_



	//
	// Hash function
	//

	template <typename T>
	struct Hash : public CompileTime::FastCopyable
	{
		using Key_t = typename TypeTraits::RemoveConstVolatile< T >;

		CHECKRES HashResult  operator () (const Key_t &x) const noexcept
		{
			return (HashResult) std::hash< Key_t >()( x );
		}
	};


	
/*
=================================================
	HashOf
=================================================
*/
	template <typename T>
	inline HashResult  HashOf (const T &x) noexcept
	{
		return Hash<T>()( x );
	}


}	// GXTypes
}	// GX_STL
