// Copyright © 2014-2017  Zhirnov Andrey. All rights reserved.

#pragma once

#include "StaticArray.h"
#include "Engine/STL/Types/Bitfield.h"

namespace GX_STL
{
namespace GXTypes
{

	//
	// Static Bit Array
	//

	template <usize C, typename IndexType = usize>
	struct StaticBitArray : public CompileTime::FastCopyable
	{
		STATIC_ASSERT( C > 0, "invalid array size" );

	// types
	private:
		static const usize	_BITS_PER_ELEM	=	C % 32 == 0 ? 32 :
												C % 16 == 0 ? 16 :
												8;

		static const usize	_BPE_POT		= CompileTime::IntLog2< usize, _BITS_PER_ELEM >;
		static const usize	_ELEM_IDX_MASK	= (1 << _BPE_POT) - 1;

		static const usize	_ARR_COUNT		= ( C + _BITS_PER_ELEM - 1 ) / _BITS_PER_ELEM;

		typedef Bitfield< _BITS_PER_ELEM, IndexType >	_elem_t;
		typedef typename _elem_t::BitRef				_elem_ref_t;
		typedef StaticArray< _elem_t, _ARR_COUNT >		_arr_t;


	public:
		typedef StaticBitArray< C, IndexType >		Self;
		typedef _elem_ref_t							BitRef;
		typedef _elem_t								value_t;
		typedef IndexType							index_t;
		
		static const usize	STATIC_COUNT = C;


	// variables
	private:
		_arr_t		_memory;


	// methods
	private:
		static usize   _ToArrayIndex (index_t i)	{ return i >> _BPE_POT; }
		static index_t _ToElemIndex (index_t i)		{ return index_t( i & _ELEM_IDX_MASK ); }


	public:
		StaticBitArray (GX_DEFCTOR) {}
		
		StaticBitArray (const Self &other) : _memory(other._memory) {}
		
		StaticBitArray (Self &&other) = default;
		
		Self &	operator =  (Self &&right)		= default;
		Self &	operator =  (const Self &right)	= default;

		bool	operator == (const Self &right) const;
		bool	operator != (const Self &right) const;

		BitRef	operator [] (index_t i);
		bool	operator [] (index_t i) const;
		
		Self &	SetAt (index_t i, bool bit);
		Self &	Set (index_t i);
		Self &	Reset (index_t i);
		bool	Get (index_t i) const;

		Self &	And (index_t i);
		Self &	Or (index_t i);
		Self &	Xor (index_t i);
		
		Self &	SetAll (bool bit = true);
		
		// add bit
		Self &	operator |= (index_t i)				{ return Set( i ); }
		Self	operator |  (index_t i) const		{ return Self(*this).Set( i ); }
		
		// remove bit
		Self &	operator ^= (index_t i)				{ return Reset( i ); }
		Self	operator ^  (index_t i) const		{ return Self(*this).Reset( i ); }

		bool	Empty ()		const				{ return false; }
		usize	Count ()		const				{ return C; }
		usize	Capacity ()		const				{ return C; }
		usize	MaxCapacity ()	const				{ return Capacity(); }
		BytesU	Size ()			const				{ return _memory.Size(); }
		BytesU	FullSize ()		const				{ return _memory.FullSize(); }
		usize	LastIndex ()	const				{ return Count()-1; }

		operator ArrayCRef<value_t> () const		{ return _memory; }
		

		static constexpr bool	IsLinearMemory ()	{ return true; }

		friend void SwapValues (INOUT Self &left, INOUT Self &right)
		{
			SwapValues( left._memory, right._memory );
		}
	};

	
	template <typename EnumType>
	using EnumSet = StaticBitArray< EnumType::_Count, typename EnumType::type >;
	

/*
=================================================
	operator ==
=================================================
*/
	template <usize C, typename I>
	inline bool  StaticBitArray<C,I>::operator == (const Self &right) const
	{
		return _memory == right._memory;
	}
	
/*
=================================================
	operator !=
=================================================
*/
	template <usize C, typename I>
	inline bool  StaticBitArray<C,I>::operator != (const Self &right) const
	{
		return not (*this == right);
	}
	
/*
=================================================
	operator []
=================================================
*/
	template <usize C, typename I>
	inline typename StaticBitArray<C,I>::BitRef  StaticBitArray<C,I>::operator [] (index_t index)
	{
		ASSUME( index < Count() );
		return _memory[ _ToArrayIndex( index ) ][ _ToElemIndex( index ) ];
	}
	
/*
=================================================
	operator []
=================================================
*/
	template <usize C, typename I>
	inline bool  StaticBitArray<C,I>::operator [] (index_t index) const
	{
		ASSUME( index < Count() );
		return _memory[ _ToArrayIndex( index ) ][ _ToElemIndex( index ) ];
	}
	
/*
=================================================
	SetAt
=================================================
*/
	template <usize C, typename I>
	inline StaticBitArray<C,I>&  StaticBitArray<C,I>::SetAt (index_t index, bool bit)
	{
		ASSUME( index < Count() );
		_memory[ _ToArrayIndex( index ) ].SetAt( _ToElemIndex( index ), bit );
		return *this;
	}
	
/*
=================================================
	Set
=================================================
*/
	template <usize C, typename I>
	inline StaticBitArray<C,I>&  StaticBitArray<C,I>::Set (index_t index)
	{
		ASSUME( index < Count() );
		_memory[ _ToArrayIndex( index ) ].Set( _ToElemIndex( index ) );
		return *this;
	}
	
/*
=================================================
	Reset
=================================================
*/
	template <usize C, typename I>
	inline StaticBitArray<C,I>&  StaticBitArray<C,I>::Reset (index_t index)
	{
		ASSUME( index < Count() );
		_memory[ _ToArrayIndex( index ) ].Reset( _ToElemIndex( index ) );
		return *this;
	}
	
/*
=================================================
	And
=================================================
*/
	template <usize C, typename I>
	inline StaticBitArray<C,I>&  StaticBitArray<C,I>::And (index_t index)
	{
		ASSUME( index < Count() );
		_memory[ _ToArrayIndex( index ) ].And( _ToElemIndex( index ) );
		return *this;
	}
	
/*
=================================================
	Or
=================================================
*/
	template <usize C, typename I>
	inline StaticBitArray<C,I>&  StaticBitArray<C,I>::Or (index_t index)
	{
		ASSUME( index < Count() );
		_memory[ _ToArrayIndex( index ) ].Or( _ToElemIndex( index ) );
		return *this;
	}
	
/*
=================================================
	Xor
=================================================
*/
	template <usize C, typename I>
	inline StaticBitArray<C,I>&  StaticBitArray<C,I>::Xor (index_t index)
	{
		ASSUME( index < Count() );
		_memory[ _ToArrayIndex( index ) ].Xor( _ToElemIndex( index ) );
		return *this;
	}
	
/*
=================================================
	SetAll
=================================================
*/
	template <usize C, typename I>
	inline StaticBitArray<C,I>&  StaticBitArray<C,I>::SetAll (bool bit)
	{
		FOR( i, _memory ) {
			_memory[i].SetAll( bit );
		}
		return *this;
	}
	
/*
=================================================
	Get
=================================================
*/
	template <usize C, typename I>
	inline bool  StaticBitArray<C,I>::Get (index_t index) const
	{
		ASSUME( index < Count() );
		return _memory[ _ToArrayIndex( index ) ].Get( _ToElemIndex( index ) );
	}


	
/*
=================================================
	Hash
=================================================
*/
	template <usize C, typename I>
	struct Hash< StaticBitArray<C,I> > :
		private Hash< ArrayCRef<typename StaticBitArray<C,I>::value_t > >
	{
		typedef StaticBitArray<C,I>							key_t;
		typedef Hash< ArrayCRef<typename key_t::value_t > >	base_t;
		typedef typename base_t::result_t					result_t;

		result_t operator () (const key_t &x) const
		{
			return base_t::operator ()( x );
		}
	};

}	// GXTypes
}	// GX_STL
