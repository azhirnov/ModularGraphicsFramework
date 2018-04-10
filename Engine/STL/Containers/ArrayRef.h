// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/STL/Containers/CopyStrategy.h"
#include "Engine/STL/Math/MathFunc.h"
#include "Engine/STL/Algorithms/ArrayUtils.h"
#include "Engine/STL/Memory/MemoryViewer.h"

namespace GX_STL
{
namespace GXTypes
{
	
	template <typename T, typename S, typename MC>	struct Array;
	template <typename T, typename S, typename MC>	struct TString;
	template <typename T>							struct TStringRef;



	//
	// Array Wrapper
	//

	template <typename T>
	struct ArrayRef : public CompileTime::FastCopyable
	{
	// types
	public:
		using Value_t		= T;
		using Self			= ArrayRef<T>;

		using void_ptr_t	= typename TypeTraits::CopyConstToPointer< T, void *>;
		using C				= typename TypeTraits::RemoveConst<T>;

	private:
		struct _CompareElements
		{
			bool operator () (const T& left, const T& right) const
			{
				return left == right;
			}
		};
		

	// variables
	private:
		union {
			T	*				_memory;
			TMemoryViewer<T>	_memView;
		};
		usize					_count;


	// methods
	public:
		ArrayRef (GX_DEFCTOR);
		ArrayRef (T *arrayPtr, usize count);
		ArrayRef (void_ptr_t begin, void_ptr_t end);

		template <usize I>
		ArrayRef (T (&arr)[I]);

		ArrayRef (Self &&other) = default;
		ArrayRef (const Self &other) = default;
		
		ArrayRef (InitializerList<C> list);

		template <template <typename ...> class LinearMemoryContainerType, typename ...Types>
		ArrayRef (LinearMemoryContainerType< C, Types... > &container);
		
		template <template <typename ...> class LinearMemoryContainerType, typename ...Types>
		ArrayRef (const LinearMemoryContainerType< C, Types... > &container);

		std::vector<C> ToStdVector () const;

		Self &		operator =  (Self &&right)		= default;
		Self &		operator =  (const Self &right)	= default;

		T		*	ptr ();
		T const	*	ptr () const;
		
		T *			RawPtr ()					{ return _memory; }
		T const *	RawPtr () const				{ return _memory; }
		
		T		&	Back ()						{ return (*this)[ LastIndex() ]; }
		T const	&	Back () const				{ return (*this)[ LastIndex() ]; }
		T		&	Front ()					{ return (*this)[ 0 ]; }
		T const	&	Front () const				{ return (*this)[ 0 ]; }

		T		&	operator [] (usize i);
		T const	&	operator [] (usize i) const;
		
		bool		operator == (const Self &right) const;
		bool		operator != (const Self &right) const;
		bool		operator >  (const Self &right) const;
		bool		operator >= (const Self &right) const;
		bool		operator <  (const Self &right) const;
		bool		operator <= (const Self &right) const;

		operator	ArrayRef<const T> () const	{ return ArrayRef<const T>( _memory, _count ); }

		bool		Empty ()		const		{ return _count == 0; }
		usize		Count ()		const		{ return _count; }
		BytesU		Size ()			const		{ return BytesU( _count * sizeof(T) ); }
		usize		LastIndex ()	const		{ return Count()-1; }

		template <typename Cmp>
		bool		Equals (const Self &other, Cmp sCmp) const;
		
		usize		GetIndex (const T &valueRef) const;
		bool		IsInArray (const T &valueRef) const;
		
		template <typename E>
		bool		Find (OUT usize &index, const E &value, usize start = 0) const;

		template <typename E>
		bool		IsExist (const E &value) const;

		bool		Intersects (const Self &other) const;

		Self				SubArray (usize first, usize count = UMax);
		ArrayRef<const T>	SubArray (usize first, usize count = UMax) const;


		// iterators
		using iterator			= T *;
		using const_iterator	= const T *;

		bool			IsBegin (const_iterator iter) const;
		bool			IsEnd (const_iterator iter) const;

		iterator		Begin ()					{ return RawPtr(); }
		const_iterator	Begin () const				{ return RawPtr(); }

		iterator		End ()						{ return RawPtr() + _count; }
		const_iterator	End () const				{ return RawPtr() + _count; }

		iterator		GetIter (usize i);
		const_iterator	GetIter (usize i) const;


		// compare operators for binary data (POD types only)
		template <typename B>
		bool BinEquals (const ArrayRef<const B> &right) const;

		template <typename B>
		bool BinLess (const ArrayRef<const B> &right) const;

		template <typename B>
		bool BinGreater (const ArrayRef<const B> &right) const;
		


	// static methods
	public:
		template <typename B>
		CHECKRES static Self From (ArrayRef<B> arr);
		
		template <typename B>
		CHECKRES static Self From (TStringRef<B> str);

		template <typename B, typename S, typename MC>
		CHECKRES static Self From (const Array<B,S,MC> &arr);
		
		template <typename B, typename S, typename MC>
		CHECKRES static Self From (Array<B,S,MC> &arr);

		template <typename B, typename S, typename MC>
		CHECKRES static Self From (const TString<B,S,MC> &str);
		
		template <typename B, typename S, typename MC>
		CHECKRES static Self From (TString<B,S,MC> &str);

		CHECKRES static Self FromVoid (void_ptr_t ptr, BytesU size);
		
		template <typename B, usize I>
		CHECKRES static Self From (const B (&arr)[I]);
		
		template <typename B>
		CHECKRES static Self FromStd (const std::vector<B> &vec);
		
		template <typename B>
		CHECKRES static Self FromStd (std::vector<B> &vec);
		
		template <typename B>
		CHECKRES static Self FromStd (const std::basic_string< B, std::char_traits<B>, std::allocator<B> > &str);
		
		template <typename B>
		CHECKRES static Self FromStd (std::basic_string< B, std::char_traits<B>, std::allocator<B> > &str);

		template <typename B>
		CHECKRES static Self FromValue (B &ref);

		static constexpr bool	IsLinearMemory ()	{ return true; }
		static constexpr bool	IsStaticMemory ()	{ return false; }
	};
	

	using BinArrayCRef	= ArrayRef< const ubyte >;
	using BinArrayRef	= ArrayRef< ubyte >;

	template <typename T>
	using ArrayCRef = ArrayRef< const T >;

	
/*
=================================================
	constructor
=================================================
*/
	template <typename T>
	inline ArrayRef<T>::ArrayRef (UninitializedType) :
		_memory(null), _count(0)
	{}
	
/*
=================================================
	constructor
=================================================
*/
	template <typename T>
	inline ArrayRef<T>::ArrayRef (void_ptr_t begin, void_ptr_t end) :
		_memory( static_cast<T*>( begin ) ),
		_count( ( usize(end) - usize(begin) ) / sizeof(T) )
	{
		ASSERT( _count == 0 or _memory != null );
		ASSERT( begin <= end );
	}
	
/*
=================================================
	constructor
=================================================
*/
	template <typename T>
	inline ArrayRef<T>::ArrayRef (T *arrayPtr, const usize count) : _memory(arrayPtr), _count(count)
	{
		ASSERT( _count == 0 or _memory != null );
	}
	
/*
=================================================
	constructor
=================================================
*/
	template <typename T>
	template <usize I>
	inline ArrayRef<T>::ArrayRef (T (&arr)[I]) :
		_memory( static_cast<T *>(arr) ), _count( I )
	{
		ASSERT( _count == 0 or _memory != null );
	}
	
/*
=================================================
	constructor
=================================================
*/
	template <typename T>
	inline ArrayRef<T>::ArrayRef (InitializerList<C> list) :
		_memory( list.begin() ), _count( list.size() )
	{
		ASSERT( _count == 0 or _memory != null );
	}
	
/*
=================================================
	constructor
=================================================
*/
	template <typename T>
	template <template <typename ...> class LinearMemoryContainerType, typename ...Types>
	inline ArrayRef<T>::ArrayRef (LinearMemoryContainerType< C, Types... > &container) :
		_memory( null ), _count( container.Count() )
	{
		if ( container.Count() > 0 )
			_memory = container.ptr();
	}
		
	
	template <typename T>
	template <template <typename ...> class LinearMemoryContainerType, typename ...Types>
	inline ArrayRef<T>::ArrayRef (const LinearMemoryContainerType< C, Types... > &container) :
		_memory( null ), _count( container.Count() )
	{
		if ( container.Count() > 0 )
			_memory = container.ptr();
	}
	
/*
=================================================
	ToStdVector
=================================================
*/
	template <typename T>
	inline std::vector< typename ArrayRef<T>::C >  ArrayRef<T>::ToStdVector () const
	{
		return std::vector<C>( ptr(), Count() );
	}
	
/*
=================================================
	From
=================================================
*/
	template <typename T>
	template <typename B>
	inline ArrayRef<T>  ArrayRef<T>::From (ArrayRef<B> arr)
	{
		if ( not arr.Empty() )
			return FromVoid( static_cast<void_ptr_t>( const_cast< TypeTraits::RemoveConst<B *> >( arr.ptr() ) ), arr.Size() );
		else
			return ArrayRef<T>();
	}
	
	
	template <typename T>
	template <typename B, usize I>
	inline ArrayRef<T>  ArrayRef<T>::From (const B (&arr)[I])
	{
		return From( ArrayCRef<B>( arr ) );
	}
	
/*
=================================================
	FromStd
=================================================
*/
	template <typename T>
	template <typename B>
	inline ArrayRef<T>  ArrayRef<T>::FromStd (const std::vector<B> &vec)
	{
		if ( not vec.empty() )
			return FromVoid( static_cast<void_ptr_t>(vec.data()), vec.size() * SizeOf<B> );
		else
			return ArrayRef<T>();
	}
	
	template <typename T>
	template <typename B>
	inline ArrayRef<T>  ArrayRef<T>::FromStd (std::vector<B> &vec)
	{
		if ( not vec.empty() )
			return FromVoid( static_cast<void_ptr_t>(vec.data()), vec.size() * SizeOf<B> );
		else
			return ArrayRef<T>();
	}
	
/*
=================================================
	FromVoid
=================================================
*/
	template <typename T>
	inline ArrayRef<T>  ArrayRef<T>::FromVoid (void_ptr_t ptr, const BytesU size)
	{
		if ( ptr != null and size > 0 )
			return ArrayRef<T>( static_cast< T *>( const_cast< void *>( ptr ) ), usize( size / SizeOf<T> ) );
		else
			return ArrayRef<T>();
	}
	
/*
=================================================
	FromValue
=================================================
*/
	template <typename T>
	template <typename B>
	inline ArrayRef<T>  ArrayRef<T>::FromValue (B &ref)
	{
		STATIC_ASSERT( sizeof(B) % sizeof(T) == 0 );
		return ArrayRef<T>( PointerCast<T>( &ref ), sizeof(B) / sizeof(T) );
	}
	
/*
=================================================
	ptr
=================================================
*/
	template <typename T>
	inline T * ArrayRef<T>::ptr ()
	{
		ASSUME( _memory != null );
		return _memory;
	}
	
	template <typename T>
	inline T const * ArrayRef<T>::ptr () const
	{
		ASSUME( _memory != null );
		return _memory;
	}
	
/*
=================================================
	operator []
=================================================
*/
	template <typename T>
	inline T & ArrayRef<T>::operator [] (const usize i)
	{
		ASSUME( i < _count );
		return _memory[i];
	}

	template <typename T>
	inline T const & ArrayRef<T>::operator [] (const usize i) const
	{
		ASSUME( i < _count );
		return _memory[i];
	}

/*
=================================================
	operator ==
=================================================
*/
	template <typename T>
	inline bool ArrayRef<T>::operator == (const Self &other) const
	{
		return Equals( other, _CompareElements() );
	}

	template <typename T>
	inline bool ArrayRef<T>::operator != (const Self &other) const
	{
		return not ( *this == other );
	}
	
/*
=================================================
	operator >
=================================================
*/
	template <typename T>
	inline bool ArrayRef<T>::operator >  (const Self &right) const
	{
		if ( Count() != right.Count() )
			return (Count() > right.Count());

		FOR( i, right )
		{
			if ( _memory[i] != right._memory[i] )
				return (_memory[i] > right._memory[i]);
		}
		return false;	// equals
	}
	
	template <typename T>
	inline bool ArrayRef<T>::operator >= (const Self &right) const
	{
		return not (*this < right);
	}
	
/*
=================================================
	operator <
=================================================
*/
	template <typename T>
	inline bool ArrayRef<T>::operator <  (const Self &right) const
	{
		if ( Count() != right.Count() )
			return (Count() < right.Count());

		FOR( i, right )
		{
			if ( _memory[i] != right._memory[i] )
				return (_memory[i] < right._memory[i]);
		}
		return false;	// equals
	}
	
	template <typename T>
	inline bool ArrayRef<T>::operator <= (const Self &right) const
	{
		return not (*this > right);
	}

/*
=================================================
	Equals
=================================================
*/
	template <typename T>
	template <typename Cmp>
	inline bool ArrayRef<T>::Equals (const Self &other, Cmp sCmp) const
	{
		if ( _count != other._count )
			return false;

		if ( other._memory == _memory )
		{
			if ( _memory != null )
				WARNING( "compared with self!" );
			return true;
		}

		for (usize i = 0; i < _count; ++i) {
			if ( not sCmp( _memory[i], other._memory[i] ) )
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
	inline usize ArrayRef<T>::GetIndex (const T &valueRef) const
	{
		ASSERT( IsInArray( valueRef ) );
		return usize( &valueRef - Begin() );
	}
	
/*
=================================================
	IsInArray
=================================================
*/
	template <typename T>
	inline bool ArrayRef<T>::IsInArray (const T &valueRef) const
	{
		return ( &valueRef >= Begin() and &valueRef < End() );
	}
	
/*
=================================================
	Find
=================================================
*/
	template <typename T>
	template <typename E>
	inline bool ArrayRef<T>::Find (OUT usize &index, const E &value, const usize start) const
	{
		index = UMax;

		FORv( i, start, *this )
		{
			if ( value == _memory[i] )
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
	inline bool ArrayRef<T>::IsExist (const E &value) const
	{
		usize	idx;
		return Find( OUT idx, value, 0 );
	}
	
/*
=================================================
	SubArray
=================================================
*/
	template <typename T>
	inline ArrayRef<T>  ArrayRef<T>::SubArray (const usize first, usize count)
	{
		ASSERT( first <= Count() and (count == UMax or first + count <= Count()) );
		
		if ( first >= Count() )
			return ArrayRef<T>();
		
		// 'count' can be -1
		if ( count == UMax or count + first > Count() )
			count = Count() - first;

		return ( ArrayRef<T>( ptr() + first, count ) );
	}
	
/*
=================================================
	SubArray
=================================================
*/
	template <typename T>
	inline ArrayCRef<T>  ArrayRef<T>::SubArray (const usize first, usize count) const
	{
		ASSERT( first <= Count() and (count == UMax or first + count <= Count()) );
		
		if ( first >= Count() )
			return ArrayCRef<T>();

		// 'count' can be -1
		if ( count == UMax or count + first > Count() )
			count = Count() - first;

		return ( ArrayCRef<T>( ptr() + first, count ) );
	}
	
/*
=================================================
	Intersects
=================================================
*/
	template <typename T>
	inline bool  ArrayRef<T>::Intersects (const Self &other) const
	{
		ASSERT( Begin() <= End() );
		ASSERT( other.Begin() <= other.End() );
		return Begin() > other.End() or End() < other.Begin();
	}
	
/*
=================================================
	GetIter
=================================================
*/
	template <typename T>
	inline T * ArrayRef<T>::GetIter (const usize i)
	{
		ASSUME( i < _count );
		return & _memory[i];
	}
	
/*
=================================================
	GetIter
=================================================
*/
	template <typename T>
	inline const T * ArrayRef<T>::GetIter (const usize i) const
	{
		ASSUME( i < _count );
		return & _memory[i];
	}
	
/*
=================================================
	IsBegin
=================================================
*/
	template <typename T>
	inline bool ArrayRef<T>::IsBegin (const_iterator iter) const
	{
		return Begin() == iter;
	}
	
/*
=================================================
	IsEnd
=================================================
*/
	template <typename T>
	inline bool ArrayRef<T>::IsEnd (const_iterator iter) const
	{
		return End() == iter;
	}
	
/*
=================================================
	BinEquals
=================================================
*/
	template <typename T>
	template <typename B>
	inline bool ArrayRef<T>::BinEquals (const ArrayCRef<B> &right) const
	{
		STATIC_ASSERT( not CompileTime::IsCompareOpAvailable<T> and CompileTime::IsMemCopyFromFileAvailable<T> and
					   not CompileTime::IsCompareOpAvailable<B> and CompileTime::IsMemCopyFromFileAvailable<B> );

		return this->Size() == right.Size() and MemCmp( *this, right ) == 0;
	}
	
/*
=================================================
	BinLess
=================================================
*/
	template <typename T>
	template <typename B>
	inline bool ArrayRef<T>::BinLess (const ArrayCRef<B> &right) const
	{
		STATIC_ASSERT( not CompileTime::IsCompareOpAvailable<T> and CompileTime::IsMemCopyFromFileAvailable<T> and
					   not CompileTime::IsCompareOpAvailable<B> and CompileTime::IsMemCopyFromFileAvailable<B> );

		return	this->Size() != right.Size()	?	this->Size() < right.Size()	:
													MemCmp( *this, right ) < 0;		// TODO: check
	}
	
/*
=================================================
	BinGreater
=================================================
*/
	template <typename T>
	template <typename B>
	inline bool ArrayRef<T>::BinGreater (const ArrayCRef<B> &right) const
	{
		STATIC_ASSERT( not CompileTime::IsCompareOpAvailable<T> and CompileTime::IsMemCopyFromFileAvailable<T> and
					   not CompileTime::IsCompareOpAvailable<B> and CompileTime::IsMemCopyFromFileAvailable<B> );

		return	this->Size() != right.Size()	?	this->Size() > right.Size()	:
													MemCmp( *this, right ) > 0;		// TODO: check
	}
	
	
/*
=================================================
	Hash
=================================================
*/
	template <typename T>
	struct Hash< ArrayRef<T> >
	{
		CHECKRES HashResult  operator () (const ArrayRef<T> &x) const noexcept
		{
			HashResult	value = ~HashOf( x.Count() );

			if ( CompileTime::IsPOD<T> )
			{
				value += _types_hidden_::HashForMemoryBlock( (const ubyte *)x.RawPtr(), (usize)x.Size() );
			}
			else
			{
				Hash<T>	hasher;

				FOR( i, x ) {
					value += hasher( x[i] );
				}
			}
			return value;
		}
	};
	

/*
=================================================
	PlacementNew
=================================================
*/
	template <typename T, typename C>
	forceinline T * PlacementNew (ArrayRef<C> buf) noexcept
	{
		STATIC_ASSERT( not TypeTraits::IsConst<C> );
		STATIC_ASSERT( CompileTime::IsPOD<C> );
		ASSERT( buf.Size() >= SizeOf<T> );

		return UnsafeMem::PlacementNew<T>( buf.ptr() );
	}

	template <typename T, typename C, typename ...Types>
	forceinline T * PlacementNew (ArrayRef<C> buf, Types&&... args) noexcept
	{
		STATIC_ASSERT( not TypeTraits::IsConst<C> );
		STATIC_ASSERT( CompileTime::IsPOD<C> );
		ASSERT( buf.Size() >= SizeOf<T> );

		return UnsafeMem::PlacementNew<T>( buf.ptr(), FW<Types>(args)... );
	}
	
/*
=================================================
	ZeroMem
=================================================
*/
	template <typename T>
	inline void ZeroMem (ArrayRef<T> buf)
	{
		STATIC_ASSERT( not TypeTraits::IsConst<T> );
		STATIC_ASSERT( CompileTime::IsPOD<T> );

		UnsafeMem::ZeroMem( buf.ptr(), buf.Size() );
	}
	
/*
=================================================
	MemCopy
----
	memory blocks must not intersects
=================================================
*/
	template <typename T0, typename T1>
	inline void MemCopy (ArrayRef<T0> dst, ArrayCRef<T1> src)
	{
		STATIC_ASSERT( not TypeTraits::IsConst<T0> );
		STATIC_ASSERT( CompileTime::IsPOD<T0> and CompileTime::IsPOD<T1> );
		ASSERT( dst.Size() >= src.Size() );

		UnsafeMem::MemCopy( dst.ptr(), src.ptr(), GXMath::Min( dst.Size(), src.Size() ) );
	}
	
/*
=================================================
	MemMove
----
	memory blocks may intersects
=================================================
*/
	template <typename T0, typename T1>
	inline void MemMove (ArrayRef<T0> dst, ArrayCRef<T1> src)
	{
		STATIC_ASSERT( not TypeTraits::IsConst<T0> );
		STATIC_ASSERT( CompileTime::IsPOD<T0> and CompileTime::IsPOD<T1> );
		ASSERT( dst.Size() >= src.Size() );

		UnsafeMem::MemMove( dst.ptr(), src.ptr(), GXMath::Min( dst.Size(), src.Size() ) );
	}
	
/*
=================================================
	MemCmp
=================================================
*/
	template <typename T0, typename T1>
	inline int MemCmp (ArrayCRef<T0> left, ArrayCRef<T1> right)
	{
		ASSERT( left.Size() == right.Size() );

		return UnsafeMem::MemCmp( left.ptr(), right.ptr(), GXMath::Min( left.Size(), right.Size() ) );
	}


}	// GXTypes
}	// GX_STL
