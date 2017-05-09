// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "CopyStrategy.h"
#include "ArrayRef.h"

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
	// types
	public:
		typedef UniBuffer<T>		Self;
		typedef T					value_t;
		
	private:
		typedef typename TypeTraits::CopyConstToPointer< T, void *>		void_ptr_t;

		struct _CompareElements
		{
			bool operator () (const T& left, const T& right) const
			{
				return left == right;
			}
		};


	// variables
	private:
		void_ptr_t *	_memory;
		usize			_count;
		ushort			_offset;
		ushort			_stride;


	// methods
	private:
		char const * const	_Begin ()	const	{ return static_cast<char const * const>(_memory); }
		char const * const	_End ()		const	{ return static_cast<char const * const>(_memory) + _count * _stride; }

		UniBuffer (void_ptr_t memory, usize count, ushort offset, ushort stride);

	public:
		UniBuffer (GX_DEFCTOR);

		template <typename B>
		UniBuffer (ArrayRef<B> buffer, T (B::*member));
		
		template <typename B>
		UniBuffer (B *memory, usize count, T (B::*member));

		template <typename B, usize I>
		UniBuffer (const B (&arr)[I], T (B::*member));

		UniBuffer (Self &&other) = default;
		UniBuffer (const Self &other) = default;

		bool		operator == (const Self &other) const;
		bool		operator != (const Self &other) const;
		
		bool		operator ! ()	const			{ return not Empty(); }

		operator	UniBuffer<const T> () const		{ return UniBuffer<const T>( _memory, _count, _offset, _stride ); }

		T		&	operator [] (usize i);
		T const	&	operator [] (usize i) const;
		
		Self &		operator =  (Self &&right)		= default;
		Self &		operator =  (const Self &right)	= default;

		T		&	Back ()							{ return (*this)[ LastIndex() ]; }
		T const	&	Back () const					{ return (*this)[ LastIndex() ]; }
		T		&	Front ()						{ return (*this)[0]; }
		T const	&	Front () const					{ return (*this)[0]; }

		bool		Empty ()		const			{ return _count == 0; }
		usize		Count ()		const			{ return _count; }
		BytesU		Size ()			const			{ return BytesU( _count * _stride ); }
		BytesU		Stride ()		const			{ return BytesU( _stride ); }
		usize		LastIndex ()	const			{ return Count()-1; }

		template <typename Cmp>
		bool  Equals (const Self &other, Cmp sCmp) const;
		
		usize GetIndex (const T &value) const;
		bool  IsInArray (const T &value) const;
		
		template <typename E>
		bool  Find (OUT usize &index, const E &value, usize start = 0) const;

		template <typename E>
		bool  IsExist (const E &value) const;

		bool  Intersects (const Self &other) const;

		static constexpr bool	IsLinearMemory ()	{ return false; }
		static constexpr bool	IsStaticMemory ()	{ return false; }
	};


	
/*
=================================================
	constructor
=================================================
*/
	template <typename T>
	inline UniBuffer<T>::UniBuffer (UninitializedType) :
		_memory(null), _count(0), _offset(0), _stride(0)
	{}
	
/*
=================================================
	constructor
=================================================
*/
	template <typename T>
	inline UniBuffer<T>::UniBuffer (void_ptr_t memory, usize count, ushort offset, ushort stride) :
		_memory(memory), _count(count), _offset(offset), _stride(stride)
	{
		ASSUME( _count == 0 or _memory != null );
	}
	
/*
=================================================
	constructor
=================================================
*/
	template <typename T>
	template <typename B>
	inline UniBuffer<T>::UniBuffer (ArrayRef<B> buffer, T (B::*member)) :
		_memory(null), _count(0), _offset(0), _stride(0)
	{
		if ( not buffer.Empty() )
		{
			_memory = static_cast<void_ptr_t>( buffer.ptr() );
			_count  = buffer.Count();
			_offset = (ushort) OffsetOf( member );
			_stride = (ushort) sizeof(B);
		}

		ASSUME( _count == 0 or _memory != null );
	}
	
/*
=================================================
	constructor
=================================================
*/
	template <typename T>
	template <typename B>
	inline UniBuffer<T>::UniBuffer (B *memory, usize count, T (B::*member)) :
		_memory( static_cast<void_ptr_t>( memory ) ),
		_count( count ),
		_offset( (ushort) OffsetOf( member ) ),
		_stride( (ushort) sizeof(B) )
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
	inline UniBuffer<T>::UniBuffer (const B (&arr)[I], T (B::*member)) :
		_memory( static_cast<void_ptr_t>( arr ) ),
		_count( I ),
		_offset( (ushort) OffsetOf( member ) ),
		_stride( (ushort) sizeof(B) )
	{
		ASSUME( _count == 0 or _memory != null );
	}
	
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
	inline T &  UniBuffer<T>::operator [] (usize i)
	{
		ASSERT( i < _count );
		return *Cast<T *>( Cast<char *>(_memory) + (_offset + usize(_stride) * i) );
	}
	
/*
=================================================
	operator []
=================================================
*/
	template <typename T>
	inline T const &  UniBuffer<T>::operator [] (usize i) const
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
		return (usize( ((const char * const) &value) - _Begin() ) - _offset) / _stride;
	}
	
/*
=================================================
	IsInArray
=================================================
*/
	template <typename T>
	inline bool UniBuffer<T>::IsInArray (const T &value) const
	{
		char const * const	value_ptr	= (char const * const) &value;

		return value_ptr >= _Begin() and value_ptr < _End();
	}
		
/*
=================================================
	Find
=================================================
*/
	template <typename T>
	template <typename E>
	inline bool UniBuffer<T>::Find (OUT usize &index, const E &value, usize start) const
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
