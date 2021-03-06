// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Core/STL/Memory/MemoryContainer.h"
#include "Core/STL/Containers/CopyStrategy.h"
#include "Core/STL/Containers/String.h"
#include "Core/STL/Containers/UniBuffer.h"
#include "Core/STL/Math/Mathematics.h"

namespace GX_STL
{
namespace GXTypes
{

	#define RET_ERROR( _ret_ )	{ WARNING("error in array");  return _ret_; }
	#define RET_FALSE			RET_ERROR( false )
	#define RET_VOID			RET_ERROR( ; )
	


	//
	// Array
	//

	template <	typename T,
				typename S = typename AutoDetectCopyStrategy<T>::type,
				typename MC = MemoryContainer<T>
			 >
	struct Array : public CompileTime::CopyQualifiers< CompileTime::FastCopyable, MC >
	{
	// types
	public:
		using Self				= Array<T,S,MC>;
		using Strategy_t		= S;
		using MemoryContainer_t	= MC;
		using Value_t			= T;
		using iterator			= T *;
		using const_iterator	= const T *;


	// variables
	private:
		MemoryContainer_t	_memory;
		usize				_count;
		usize				_size;

		//  _____________ _________________
		// | used memory | reserved memory |
		// 0          _count            _size
		

	// methods
	private:
		void _Reallocate (usize newSize, bool allowReserve = true);
		void _Move (Self &&other);

		template <typename B>	void _Insert (B *pArray, usize count, usize pos);
		
		static bool  _CheckIntersection (const void *leftBegin, const void *leftEnd,
										 const void *rightBegin, const void *rightEnd);

	public:
		Array (GX_DEFCTOR);
		Array (Self &&other);
		Array (const Self &other);
		Array (ArrayCRef<T> other);
		Array (InitializerList<T> list);

		~Array ()														{ Free(); }

		ND_ T		*	ptr ();
		ND_ T const *	ptr () const;

		ND_ T *			RawPtr ()										{ return _memory.Pointer(); }
		ND_ T const *	RawPtr () const									{ return _memory.Pointer(); }

		ND_ T		&	Back ();
		ND_ T const &	Back () const;
		ND_ T		&	Front ();
		ND_ T const &	Front () const;

		ND_ T		&	operator [] (usize i);
		ND_ T const &	operator [] (usize i) const;

			Self &		operator << (const T &value)					{ PushBack( value );		return *this; }
			Self &		operator << (ArrayCRef<T> other)				{ Append( other );			return *this; }
			Self &		operator << (T &&value)							{ PushBack( RVREF(value) );	return *this; }
			Self &		operator << (Self &&other)						{ Append( RVREF(other) );	return *this; }

		friend Self &	operator >> (const T &left, Self &right)		{ right.PushFront( left );  return right; }
		friend Self &	operator >> (ArrayCRef<T> left, Self &right)	{ right.Insert( left, 0 );  return right; }
		friend Self &	operator >> (T &&left, Self &right)				{ right.PushFront( RVREF(left) );  return right; }
		friend Self &	operator >> (Self &&left, Self &right)			{ right.Insert( RVREF(left), 0 );  return right; }
		
			Self &		operator =  (ArrayCRef<T> right)				{ Copy( right );					return *this; }
			Self &		operator =  (const Self &right)					{ Copy( right );					return *this; }
			Self &		operator =  (Self &&right)						{ Free();  _Move( RVREF(right) );	return *this; }

		ND_ bool		operator == (ArrayCRef<T> right) const			{ return ArrayCRef<T>(*this) == right; }
		ND_ bool		operator != (ArrayCRef<T> right) const			{ return not ( (*this) == right ); }
		
		ND_ bool		operator >  (ArrayCRef<T> right) const			{ return ArrayCRef<T>(*this) >  right; }
		ND_ bool		operator >= (ArrayCRef<T> right) const			{ return ArrayCRef<T>(*this) >= right; }
		ND_ bool		operator <  (ArrayCRef<T> right) const			{ return ArrayCRef<T>(*this) <  right; }
		ND_ bool		operator <= (ArrayCRef<T> right) const			{ return ArrayCRef<T>(*this) <= right; }

		ND_ operator UniBuffer<T> ()									{ return UniBuffer<T>{ RawPtr(), Count(), SizeOf<T> }; }
		ND_ operator UniBuffer<const T> () const						{ return UniBuffer<const T>{ RawPtr(), Count(), SizeOf<T> }; }
		

		ND_ bool At (usize index, OUT T & value) const;

		void Append (InitializerList<T> other)						{ Append( ArrayCRef<T>{other} ); }
		void Append (ArrayCRef<T> other)							{ _Insert<const T>( other.RawPtr(), other.Count(), Count() ); }
		void Append (const Self &other)								{ Append( ArrayCRef<T>{other} ); }
		void Append (Self &&other)									{ _Insert<T>( other._memory.Pointer(), other.Count(), Count() ); }
		
		void AppendFront (InitializerList<T> other)					{ AppendFront( ArrayCRef<T>{other} ); }
		void AppendFront (ArrayCRef<T> other)						{ Insert( other, 0 ); }
		void AppendFront (const Self &other)						{ AppendFront( ArrayCRef<T>{other} ); }
		void AppendFront (Self &&other)								{ _Insert<T>( other._memory.Pointer(), other.Count(), Count() ); }
		
		void AppendBack (InitializerList<T> other)					{ Append( ArrayCRef<T>{other} ); }
		void AppendBack (ArrayCRef<T> other)						{ Append( other ); }
		void AppendBack (const Self &other)							{ Append( other ); }
		void AppendBack (Self &&other)								{ Append( RVREF( other ) ); }

		void Copy (InitializerList<T> other)						{ Copy( ArrayCRef<T>( other ) ); }
		void Copy (ArrayCRef<T> other)								{ Clear();  Append( other ); }
		void Copy (const Self &other)								{ Clear();  Append( other ); }

		void Free ();
		void FreeReserve ();

		void Clear ();

		void Erase (usize pos, usize count = 1);
		void EraseFromBack (usize count);

		// breake order
		void FastErase (usize pos, usize count);
		void FastErase (usize pos);

		template <typename E>
		bool Find (OUT usize &index, const E &value, usize start = 0) const		{ return ArrayCRef<T>(*this).Find( index, value, start ); }

		template <typename E>
		ND_ bool IsExist (const E &value) const							{ return ArrayCRef<T>(*this).IsExist( value ); }

		template <typename E>
		bool FindAndErase (const E &value);

		template <typename E>
		bool FindAndFastErase (const E &value);

		void Insert (const T& value, usize pos)						{ _Insert<const T>( AddressOf(value), 1, pos ); }
		void Insert (T&& value, usize pos)							{ _Insert<T>( AddressOf(value), 1, pos ); }

		void Insert (ArrayCRef<T> other, usize pos)					{ _Insert<const T>( other.RawPtr(), other.Count(), pos ); }
		void Insert (const Self &other, usize pos)					{ Insert( ArrayCRef<T>{other}, pos ); }
		void Insert (Self &&other, usize pos)						{ _Insert<T>( other._memory.Pointer(), other.Count(), Count() ); }

		void PushBack (const T& value);
		void PushBack (T&& value);

		void PushFront (const T& value)								{ _Insert<const T>( AddressOf(value), 1, 0 ); }
		void PushFront (T&& value)									{ _Insert<T>( AddressOf(value), 1, 0 ); }
		
		void PopBack ();
		void PopFront ();
		
		void Resize (usize newSize, bool allowReserve = true);
		
		void Reserve (usize size);
		
		bool Set (usize index, const T &value);
		bool Set (usize i, T &&value);
		
		void Swap (usize first, usize second);

		ND_ ArrayRef<T>		SubArray (usize pos, usize count = UMax)			{ return ArrayRef<T>(*this).SubArray( pos, count ); }
		ND_ ArrayCRef<T>	SubArray (usize pos, usize count = UMax)	const	{ return ArrayCRef<T>(*this).SubArray( pos, count ); }

		template <typename T2, typename S2, typename A2>
		void Convert (OUT Array<T2,S2,A2> &other) const;

		ND_ usize				GetIndex (const T &value)	const				{ return ArrayCRef<T>(*this).GetIndex( value ); }
		ND_ bool				IsInArray (const T &value)	const				{ return ArrayCRef<T>(*this).IsInArray( value ); }

		ND_ bool				Empty ()			const		{ return _count == 0; }
		ND_ usize				Count ()			const		{ return _count; }
		ND_ usize				Capacity ()			const		{ return _size; }
		ND_ constexpr usize		MaxCapacity ()		const		{ return _memory.MaxSize(); }	// max available for allocation count of elements
		ND_ BytesU				Size ()				const		{ return Count() * SizeOf<T>; }
		ND_ BytesU				FullSize ()			const		{ return Capacity() * SizeOf<T>; }
		ND_ usize				LastIndex ()		const		{ return Count()-1; }


		ND_ iterator			begin ()						{ return RawPtr(); }
		ND_ const_iterator		begin ()			const		{ return RawPtr(); }
		ND_ iterator			end ()							{ return RawPtr() + _count; }
		ND_ const_iterator		end ()				const		{ return RawPtr() + _count; }


		static constexpr bool		IsLinearMemory ()			{ return true; }
		constexpr bool				IsStaticMemory ()	const	{ return _memory.IsStatic(); }

		friend void SwapValues (INOUT Self &left, INOUT Self &right)
		{
			left._memory.SwapMemory( right._memory );

			SwapValues( left._count,	right._count );
			SwapValues( left._size,		right._size );
		}
	};
	

	using BinaryArray	= Array< ubyte >;


	template <typename T, usize Size>
	using FixedSizeArray = Array< T, typename AutoDetectCopyStrategy<T>::type, StaticMemoryContainer<T, Size> >;
	
	template <typename T, usize Size>
	using MixedSizeArray = Array< T, typename AutoDetectCopyStrategy<T>::type, MixedMemoryContainer<T, Size> >;
	
/*
=================================================
	ArrayRef::From
=================================================
*/
	template <typename T>
	template <typename B, typename S, typename MC>
	inline ArrayRef<T>  ArrayRef<T>::From (const Array<B,S,MC> &arr)
	{
		return FromVoid( Cast<void_ptr_t>(arr.ptr()), arr.Size() );
	}

	template <typename T>
	template <typename B, typename S, typename MC>
	inline ArrayRef<T>  ArrayRef<T>::From (Array<B,S,MC> &arr)
	{
		return FromVoid( Cast<void_ptr_t>(arr.ptr()), arr.Size() );
	}

/*
=================================================
	constructor
=================================================
*/
	template <typename T, typename S, typename MC>
	inline Array<T,S,MC>::Array (UninitializedType) : _count(0), _size(0)
	{}
	
/*
=================================================
	constructor
=================================================
*/
	template <typename T, typename S, typename MC>
	inline Array<T,S,MC>::Array (const Self &other) : _count(0), _size(0)
	{
		Copy( other );
	}
	
/*
=================================================
	constructor
=================================================
*/
	template <typename T, typename S, typename MC>
	inline Array<T,S,MC>::Array (Self &&other) : _count(0), _size(0)
	{
		_Move( RVREF(other) );
	}
	
/*
=================================================
	constructor
=================================================
*/
	template <typename T, typename S, typename MC>
	inline Array<T,S,MC>::Array (ArrayCRef<T> other) : _count(0), _size(0)
	{
		Copy( other );
	}
	
/*
=================================================
	constructor
=================================================
*/
	template <typename T, typename S, typename MC>
	inline Array<T,S,MC>::Array (InitializerList<T> list) : _count(0), _size(0)
	{
		Copy( ArrayCRef<T>( list ) );
	}
	
/*
=================================================
	_Move
=================================================
*/
	template <typename T, typename S, typename MC>
	inline void Array<T,S,MC>::_Move (Self &&other)
	{
		_memory	= RVREF( other._memory );
		_count	= other._count;
		_size	= other._size;

		other._count = other._size = 0;
	}
	
/*
=================================================
	_Reallocate
=================================================
*/
	template <typename T, typename S, typename MC>
	inline void Array<T,S,MC>::_Reallocate (usize newSize, const bool allowReserve)
	{
		ASSERT( newSize > 0 );

		MemoryContainer_t	old_memcont;	old_memcont.MoveFrom( INOUT _memory );
		_size = newSize;

		CHECK( _memory.Allocate( INOUT _size, allowReserve ) );
		newSize = GXMath::Min( newSize, _size );

		const usize	new_count = _count < newSize ? _count : newSize;

		if ( old_memcont.Pointer() == null or _count == 0 ) {
			_count = 0;
			return;
		}

		Strategy_t::Replace( _memory.Pointer(), old_memcont.Pointer(), new_count );

		if ( new_count < _count )
			Strategy_t::Destroy( old_memcont.Pointer() + new_count, _count - new_count );

		_count = new_count;
	}
	
/*
=================================================
	_CheckIntersection
=================================================
*/
	template <typename T, typename S, typename MC>
	inline bool Array<T,S,MC>::_CheckIntersection (const void *leftBegin, const void *leftEnd,
													const void *rightBegin, const void *rightEnd)
	{
		return CheckPointersAliasing( leftBegin, leftEnd, rightBegin, rightEnd );
	}
	
/*
=================================================
	_Insert
=================================================
*/
	template <typename T, typename S, typename MC>
	template <typename B>
	inline void Array<T,S,MC>::_Insert (B *data, const usize dataSize, const usize pos)
	{
		if ( data == null or dataSize == 0 )
			return;

		if ( pos > _count or (not Empty() and _CheckIntersection( begin(), end(), data, data + dataSize )) )
			RET_VOID;
		
		const usize	num_move = _count - pos;
		
		_count += dataSize;

		// initialize
		if ( _size == 0 )
		{
			_size = _count;
			CHECK( _memory.Allocate( INOUT _size ) );
			_count = GXMath::Min( _count, _size );
		}
		else
		// reallocate
		if ( _count > _size )
		{
			MemoryContainer_t	old_memcont;	old_memcont.MoveFrom( INOUT _memory );

			_size = _count;
			CHECK( _memory.Allocate( INOUT _size ) );
			_count = GXMath::Min( _count, _size );

			Strategy_t::Replace( _memory.Pointer(), old_memcont.Pointer(), pos );
			Strategy_t::Replace( _memory.Pointer() + pos + dataSize, old_memcont.Pointer() + pos, num_move );
		}
		else
		// move
		{
			// if not intersected
			if ( num_move <= dataSize ) {
				Strategy_t::Replace( _memory.Pointer() + pos + dataSize, _memory.Pointer() + pos, num_move, true );
			}
			else {
				for (usize i = 0; i < num_move; ++i) {
					Strategy_t::Replace( _memory.Pointer() + _count-1 - i, _memory.Pointer() + _count-1 - dataSize - i, 1, true );
				}
			}
		}

		if_constexpr( TypeTraits::IsConst<B> )
			Strategy_t::Copy( _memory.Pointer() + pos, data, dataSize );
		else
			Strategy_t::Move( _memory.Pointer() + pos, Cast<T *>(data), dataSize );
	}
	
/*
=================================================
	At
=================================================
*/
	template <typename T, typename S, typename MC>
	inline bool Array<T,S,MC>::At (const usize index, OUT T& value) const
	{
		if ( index >= Count() )  return false;
		Strategy_t::Copy( AddressOf(value), _memory.Pointer() + index, 1 );
		return true;
	}
	
/*
=================================================
	Set
=================================================
*/
	template <typename T, typename S, typename MC>
	inline bool Array<T,S,MC>::Set (const usize index, const T &value)
	{
		if ( index >= Count() ) return false;
		Strategy_t::Copy( _memory.Pointer() + index, AddressOf(value), 1 );
		return true;
	}
	
/*
=================================================
	Set
=================================================
*/
	template <typename T, typename S, typename MC>
	inline bool Array<T,S,MC>::Set (const usize index, T&& value)
	{
		if ( index >= Count() ) return false;
		Strategy_t::Move( _memory.Pointer() + index, AddressOf(value), 1 );
		return true;
	}
	
/*
=================================================
	ptr
=================================================
*/
	template <typename T, typename S, typename MC>
	inline T * Array<T,S,MC>::ptr ()
	{
		ASSUME( _memory.Pointer() != null );
		return _memory.Pointer();
	}
	
/*
=================================================
	ptr
=================================================
*/
	template <typename T, typename S, typename MC>
	inline const T * Array<T,S,MC>::ptr () const
	{
		ASSUME( _memory.Pointer() != null );
		return _memory.Pointer();
	}
	
/*
=================================================
	operator []
=================================================
*/
	template <typename T, typename S, typename MC>
	inline T & Array<T,S,MC>::operator [] (const usize index)
	{
		ASSUME( index < _count );
		return _memory.Pointer()[index];
	}
	
/*
=================================================
	operator []
=================================================
*/
	template <typename T, typename S, typename MC>
	inline const T & Array<T,S,MC>::operator [] (const usize index) const
	{
		ASSUME( index < _count );
		return _memory.Pointer()[index];
	}
	
/*
=================================================
	Back
=================================================
*/
	template <typename T, typename S, typename MC>
	inline T & Array<T,S,MC>::Back ()
	{
		ASSUME( _count != 0 );
		return _memory.Pointer()[_count-1];
	}
	
/*
=================================================
	Back
=================================================
*/
	template <typename T, typename S, typename MC>
	inline const T & Array<T,S,MC>::Back () const
	{
		ASSUME( _count != 0 );
		return _memory.Pointer()[_count-1];
	}
	
/*
=================================================
	Front
=================================================
*/
	template <typename T, typename S, typename MC>
	inline T & Array<T,S,MC>::Front ()
	{
		ASSUME( _memory.Pointer() != null );
		return _memory.Pointer()[0];
	}
	
/*
=================================================
	Front
=================================================
*/
	template <typename T, typename S, typename MC>
	inline const T & Array<T,S,MC>::Front () const
	{
		ASSUME( _memory.Pointer() != null );
		return _memory.Pointer()[0];
	}
	
/*
=================================================
	Free
=================================================
*/
	template <typename T, typename S, typename MC>
	inline void Array<T,S,MC>::Free ()
	{
		if ( _memory.Pointer() != null )
		{
			Strategy_t::Destroy( _memory.Pointer(), _count );
			_memory.Deallocate();
		}
		_count = 0;
		_size  = 0;
	}
	
/*
=================================================
	Clear
=================================================
*/
	template <typename T, typename S, typename MC>
	inline void Array<T,S,MC>::Clear ()
	{
		if ( _memory.Pointer() != null )
		{
			Strategy_t::Destroy( _memory.Pointer(), _count );
		}
		_count = 0;
	}
	
/*
=================================================
	Erase
=================================================
*/
	template <typename T, typename S, typename MC>
	inline void Array<T,S,MC>::Erase (const usize pos, usize count)
	{
		if ( pos >= _count or count == 0 or _count == 0 )
			RET_VOID;

		if ( pos + count > _count )
			count = _count - pos;

		_count -= count;

		Strategy_t::Destroy( _memory.Pointer() + pos, count );
		Strategy_t::Replace( _memory.Pointer() + pos, _memory.Pointer() + pos + count, _count - pos, true );
	}
	
/*
=================================================
	EraseFromBack
=================================================
*/
	template <typename T, typename S, typename MC>
	inline void Array<T,S,MC>::EraseFromBack (const usize count)
	{
		ASSERT( count < _count );

		if ( count >= _count ) {
			Clear();
			return;
		}

		Strategy_t::Destroy( _memory.Pointer() + _count - count, count );
		_count -= count;
	}
	
/*
=================================================
	FastErase
=================================================
*/
	template <typename T, typename S, typename MC>
	inline void Array<T,S,MC>::FastErase (const usize pos, usize count)
	{
		if ( pos >= _count or count == 0 or _count == 0 )
			RET_VOID;
		
		if ( pos + count >= _count )
			count = _count - pos;

		usize const	u_max = pos + count;
		usize		j	  = _count-1;
		usize		i	  = pos;
		
		_count -= count;

		// move from back
		for (; j >= u_max and i < u_max; --j or ++i)
		{
			Strategy_t::Destroy( _memory.Pointer() + i, 1 );
			Strategy_t::Replace( _memory.Pointer() + i, _memory.Pointer() + j, true );
		}

		if ( i == u_max )
			return;

		// delete
		for (; j >= i; --j)
		{
			Strategy_t::Destroy( _memory.Pointer() + j, 1 );
		}
	}
	
/*
=================================================
	FindAndErase
=================================================
*/
	template <typename T, typename S, typename MC>
	template <typename E>
	inline bool Array<T,S,MC>::FindAndErase (const E &value)
	{
		usize	idx = UMax;

		if ( Find<E>( idx, value ) )
		{
			Erase( idx );
			return true;
		}
		return false;
	}
	
/*
=================================================
	FindAndFastErase
=================================================
*/
	template <typename T, typename S, typename MC>
	template <typename E>
	inline bool Array<T,S,MC>::FindAndFastErase (const E &value)
	{
		usize	idx = UMax;

		if ( Find<E>( idx, value ) )
		{
			FastErase( idx );
			return true;
		}
		return false;
	}
	
/*
=================================================
	FastErase
=================================================
*/
	template <typename T, typename S, typename MC>
	inline void Array<T,S,MC>::FastErase (const usize pos)
	{
		ASSERT( pos < _count );

		if ( pos == _count-1 )
			PopBack();
		else
		{
			Strategy_t::Destroy( _memory.Pointer() + pos, 1 );
			Strategy_t::Replace( _memory.Pointer() + pos, AddressOf(Back()), 1, true );
			--_count;
		}
	}
	
/*
=================================================
	PushBack
=================================================
*/
	template <typename T, typename S, typename MC>
	inline void Array<T,S,MC>::PushBack (const T& value)
	{
		if ( _count >= _size )
			_Reallocate( _count + 1 );

		Strategy_t::Copy( _memory.Pointer() + _count, AddressOf(value), 1 );
		++_count;
	}
	
/*
=================================================
	PushBack
=================================================
*/
	template <typename T, typename S, typename MC>
	inline void Array<T,S,MC>::PushBack (T&& value)
	{
		if ( _count >= _size )
			_Reallocate( _count + 1 );

		Strategy_t::Move( _memory.Pointer() + _count, AddressOf(value), 1 );
		++_count;
	}
	
/*
=================================================
	PopBack
=================================================
*/
	template <typename T, typename S, typename MC>
	inline void Array<T,S,MC>::PopBack ()
	{
		ASSERT( _count != 0 );

		if ( _count != 0 ) {
			--_count;
			Strategy_t::Destroy( _memory.Pointer() + _count, 1 );
		}
	}

/*
=================================================
	PopFront
=================================================
*/
	template <typename T, typename S, typename MC>
	inline void Array<T,S,MC>::PopFront ()
	{
		ASSERT( _count != 0 );

		Erase( 0 );
	}
	
/*
=================================================
	FreeReserve
=================================================
*/
	template <typename T, typename S, typename MC>
	inline void Array<T,S,MC>::FreeReserve ()
	{
		if ( _count > _size )
			_Reallocate( _count, false );
	}
	
/*
=================================================
	Resize
=================================================
*/
	template <typename T, typename S, typename MC>
	inline void Array<T,S,MC>::Resize (const usize newSize, const bool allowReserve)
	{
		if ( newSize == _count )
			return;
		
		if ( newSize == 0 )
		{
			Clear();
			return;
		}

		if ( newSize < _count )
		{
			Strategy_t::Destroy( _memory.Pointer() + newSize, _count - newSize );
			_count = newSize;
			return;
		}

		_Reallocate( newSize, allowReserve );
		Strategy_t::Create( _memory.Pointer() + _count, newSize - _count );
		_count = newSize;
	}
	
/*
=================================================
	Reserve
=================================================
*/
	template <typename T, typename S, typename MC>
	inline void Array<T,S,MC>::Reserve (usize size)
	{
		if ( size > _memory.MaxSize() )
			size = _memory.MaxSize();

		if ( size <= _size )
			return;

		_Reallocate( size, false );
	}
	
/*
=================================================
	Swap
=================================================
*/
	template <typename T, typename S, typename MC>
	inline void Array<T,S,MC>::Swap (const usize first, const usize second)
	{
		if ( first >= _count or second >= _count )
			RET_VOID;

		T	temp;
		Strategy_t::Replace( AddressOf(temp),				_memory.Pointer() + second,	1 );
		Strategy_t::Replace( _memory.Pointer() + second,	_memory.Pointer() + first,	1 );
		Strategy_t::Replace( _memory.Pointer() + first,		AddressOf(temp),			1 );
	}
	
/*
=================================================
	Convert
=================================================
*/
	template <typename T, typename S, typename MC>
	template <typename T2, typename S2, typename A2>
	inline void Array<T,S,MC>::Convert (Array<T2,S2,A2> &other) const
	{
		other.Resize( Count(), false );

		for (usize i = 0; i < Count(); ++i)
		{
			other[i] = T2( _memory.Pointer()[i] );
		}
	}

	
	#undef  RET_ERROR
	#undef	RET_FALSE
	#undef  RET_VOID

	
/*
=================================================
	TStringRef<T>::From
=================================================
*/
	template <typename T>
	template <typename B, typename S, typename MC>
	inline TStringRef<const T>  TStringRef<T>::From (const Array<B,S,MC> &arr)
	{
		return From( ArrayCRef<B>(arr) );
	}

/*
=================================================
	Hash
=================================================
*/
	template <typename T, typename S, typename MC>
	struct Hash< Array<T,S,MC> >
	{
		ND_ HashResult  operator () (const Array<T,S,MC> &x) const noexcept
		{
			return HashOf( ArrayCRef<T>(x) );
		}
	};

}	// GX_STL
}	// GXTypes
