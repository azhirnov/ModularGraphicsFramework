// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Core/STL/Containers/CopyStrategy.h"
#include "Core/STL/Containers/ArrayRef.h"

namespace GX_STL
{
namespace GXTypes
{

	//
	// Universal Buffer Wrapper
	//

	template <typename T>
	struct UniBuffer : public CompileTime::FastCopyable
	{
		friend struct UniBuffer< typename TypeTraits::InvertConst<T> >;

	// types
	public:
		using Self		= UniBuffer<T>;
		using Value_t	= T;
		
	private:
		using VoidPtr_t		= typename TypeTraits::CopyConstToPointer< T, void *>;
		using VoidCPtr_t	= const VoidPtr_t;

		struct _CompareElements
		{
			bool operator () (const T& left, const T& right) const
			{
				return left == right;
			}
		};


	// variables
	private:
		VoidPtr_t		_memory;
		usize			_count;
		ushort			_offset;
		ushort			_stride;


	// methods
	private:
		char const * const	_Begin ()	const	{ return Cast<char const * const>(_memory); }
		char const * const	_End ()		const	{ return Cast<char const * const>(_memory) + _count * _stride; }

		UniBuffer (VoidPtr_t memory, usize count, ushort offset, ushort stride);

	public:
		UniBuffer (GX_DEFCTOR);

		template <typename B>
		explicit UniBuffer (ArrayRef<B> buffer);

		template <typename B, typename C>
		UniBuffer (ArrayRef<B> buffer, T (C::*member));
		
		template <typename B, typename C>
		UniBuffer (B *memory, usize count, T (C::*member));
		
		UniBuffer (VoidPtr_t memory, usize count, BytesU stride);
		
		template <typename B, usize I, typename C>
		UniBuffer (B (&arr)[I], T (C::*member));

		template <typename B, usize I>
		explicit UniBuffer (B (&arr)[I]);

		UniBuffer (Self &&other) = default;
		UniBuffer (const Self &other) = default;

		ND_ bool		operator == (const Self &other) const;
		ND_ bool		operator != (const Self &other) const;

		ND_ operator	UniBuffer<const T> () const		{ return UniBuffer<const T>( _memory, _count, _offset, _stride ); }

		ND_ T		&	operator [] (usize i);
		ND_ T const &	operator [] (usize i) const;
		
			Self &		operator =  (Self &&right)		= default;
			Self &		operator =  (const Self &right)	= default;

		ND_ VoidPtr_t	RawPtr ()						{ return _memory; }
		ND_ VoidCPtr_t	RawPtr ()		const			{ return _memory; }

		ND_ T		&	Back ()							{ return (*this)[ LastIndex() ]; }
		ND_ T const &	Back ()			const			{ return (*this)[ LastIndex() ]; }
		ND_ T		&	Front ()						{ return (*this)[0]; }
		ND_ T const &	Front ()		const			{ return (*this)[0]; }

		ND_ bool		Empty ()		const			{ return _count == 0; }
		ND_ usize		Count ()		const			{ return _count; }
		ND_ BytesU		Size ()			const			{ return BytesU( _count * _stride ); }
		ND_ BytesU		Stride ()		const			{ return BytesU( _stride ); }
		ND_ usize		LastIndex ()	const			{ return Count()-1; }

		template <typename Cmp>
		ND_ bool  Equals (const Self &other, Cmp sCmp) const;
		
		ND_ usize GetIndex (const T &value) const;
		ND_ bool  IsInArray (const T &value) const;
		
		template <typename E>
			bool  Find (OUT usize &index, const E &value, usize start = 0) const;

		template <typename E>
		ND_ bool  IsExist (const E &value) const;

		ND_ bool  Intersects (const Self &other) const;

		static constexpr bool	IsLinearMemory ()		{ return false; }
		static constexpr bool	IsStaticMemory ()		{ return false; }
	};


	
/*
=================================================
	constructor
=================================================
*/
	template <typename T>
	inline UniBuffer<T>::UniBuffer (UninitializedType) :
		_memory( null ),	_count( 0 ),
		_offset( 0 ),		_stride( 0 )
	{}
	
/*
=================================================
	constructor
=================================================
*/
	template <typename T>
	template <typename B>
	inline UniBuffer<T>::UniBuffer (ArrayRef<B> buffer) :
		_memory( buffer.RawPtr() ),		_count( buffer.Count() ),
		_offset( 0 ),					_stride( sizeof(B) )
	{
		ASSUME( _count == 0 or _memory != null );
	}

/*
=================================================
	constructor
=================================================
*/
	template <typename T>
	inline UniBuffer<T>::UniBuffer (VoidPtr_t memory, const usize count, const ushort offset, const ushort stride) :
		_memory( memory ),	_count( count ),
		_offset( offset ),	_stride( stride )
	{
		ASSUME( _count == 0 or _memory != null );
	}
	
/*
=================================================
	constructor
=================================================
*/
	template <typename T>
	template <typename B, typename C>
	inline UniBuffer<T>::UniBuffer (ArrayRef<B> buffer, T (C::*member)) :
		_memory{ buffer.RawPtr() },				_count{ buffer.Count() },
		_offset{ ushort(OffsetOf( member )) },	_stride{ ushort(sizeof(B)) }
	{
		ASSUME( _count == 0 or _memory != null );
	}
	
/*
=================================================
	constructor
=================================================
*/
	template <typename T>
	template <typename B, typename C>
	inline UniBuffer<T>::UniBuffer (B *memory, const usize count, T (C::*member)) :
		_memory( Cast<VoidPtr_t>( memory ) ),		_count( count ),
		_offset( ushort(OffsetOf( member )) ),		_stride( ushort(sizeof(B)) )
	{
		ASSUME( _count == 0 or _memory != null );
	}
	
/*
=================================================
	constructor
=================================================
*/
	template <typename T>
	template <typename B, usize I, typename C>
	inline UniBuffer<T>::UniBuffer (B (&arr)[I], T (C::*member)) :
		_memory( Cast<VoidPtr_t>( arr ) ),			_count( I ),
		_offset( ushort(OffsetOf( member )) ),		_stride( ushort(sizeof(B)) )
	{
		ASSUME( _count == 0 or _memory != null );
	}
	
/*
=================================================
	constructor
=================================================
*/
	template <typename T>
	template <typename B, usize I>
	inline UniBuffer<T>::UniBuffer (B (&arr)[I]) : UniBuffer(ArrayRef<B>( arr ))
	{}

/*
=================================================
	constructor
=================================================
*/
	template <typename T>
	inline UniBuffer<T>::UniBuffer (VoidPtr_t memory, usize count, BytesU stride) :
		UniBuffer{ memory, count, 0, ushort(stride) }
	{}

/*
=================================================
	operator ==
=================================================
*/
	template <typename T>
	inline bool UniBuffer<T>::operator == (const Self &other) const
	{
		return Equals( other, _CompareElements() );
	}
	
/*
=================================================
	operator !=
=================================================
*/
	template <typename T>
	inline bool UniBuffer<T>::operator != (const Self &other) const
	{
		return not ( *this == other );
	}
	
/*
=================================================
	operator []
=================================================
*/
	template <typename T>
	inline T &  UniBuffer<T>::operator [] (const usize i)
	{
		ASSERT( i < _count );
		return *Cast<T *>( _memory + BytesU(_offset + usize(_stride) * i) );
	}
	
/*
=================================================
	operator []
=================================================
*/
	template <typename T>
	inline T const &  UniBuffer<T>::operator [] (const usize i) const
	{
		ASSERT( i < _count );
		return *Cast<const T * const>( _Begin() + (_offset + usize(_stride) * i) );
	}
	
/*
=================================================
	Equals
=================================================
*/
	template <typename T>
	template <typename Cmp>
	inline bool UniBuffer<T>::Equals (const Self &other, Cmp sCmp) const
	{
		if ( _count != other._count )
			return false;

		if ( other._memory == _memory ) {
			WARNING("");
			return true;
		}

		for (usize i = 0; i < _count; ++i) {
			if ( not sCmp( (*this)[i], other[i] ) )
				return false;
		}

		return true;
	}
		
/*
=================================================
	GetIndex
=================================================
*/
	template <typename T>
	inline usize UniBuffer<T>::GetIndex (const T &value) const
	{
		ASSERT( IsInArray( value ) );
		return (usize( Cast<const char * const>(&value) - _Begin() ) - _offset) / _stride;
	}
	
/*
=================================================
	IsInArray
=================================================
*/
	template <typename T>
	inline bool UniBuffer<T>::IsInArray (const T &value) const
	{
		char const * const	value_ptr	= Cast<char const * const>(&value);

		return value_ptr >= _Begin() and value_ptr < _End();
	}
		
/*
=================================================
	Find
=================================================
*/
	template <typename T>
	template <typename E>
	inline bool UniBuffer<T>::Find (OUT usize &index, const E &value, const usize start) const
	{
		index = -1;

		FORv( i, start, *this )
		{
			if ( value == (*this)[i] )
			{
				index = i;
				return true;
			}
		}
		return false;
	}
	
/*
=================================================
	IsExist
=================================================
*/
	template <typename T>
	template <typename E>
	inline bool UniBuffer<T>::IsExist (const E &value) const
	{
		usize	idx;
		return Find( idx, value, 0 );
	}
	
/*
=================================================
	Intersects
=================================================
*/
	template <typename T>
	inline bool UniBuffer<T>::Intersects (const Self &other) const
	{
		ASSERT( _Begin() <= _End() );
		ASSERT( other._Begin() <= other._End() );
		return _Begin() > other._End() or _End() < other._Begin();
	}


}	// GXTypes
}	// GX_STL
