// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/STL/Containers/Array.h"
#include "Engine/STL/Algorithms/Sorts.h"
#include "Engine/STL/Containers/IndexedIterator.h"

namespace GX_STL
{
namespace GXTypes
{

	//
	// Indexed Array
	//

	template <	typename T,
				typename S = typename AutoDetectCopyStrategy<T>::type,
				typename MC = MemoryContainer<T>
			 >
	struct IndexedArray : public CompileTime::CopyQualifiers< CompileTime::FastCopyable, MC >
	{
	// types
	public:
		using Self				= IndexedArray< T, S, MC >;
		using Strategy_t			= S;
		using MemoryContainer_t	= MC;
		using Value_t			= T;

	private:
		struct ItemRef;
		struct ItemCRef;

		using Array_t	= Array<T,S,MC>;
		using Indices_t	= Array<usize>;


	// variables
	private:
		Array_t		_array;
		Indices_t	_indices;


	// methods
	public:
		IndexedArray (GX_DEFCTOR) {}

		IndexedArray (Self &&) = default;
		IndexedArray (const Self &) = default;

			Self&		operator = (Self &&) = default;
			Self&		operator = (const Self &) = default;

			Self&		operator << (const T &value)			{ PushBack( value );		return *this; }
			Self&		operator << (T &&value)					{ PushBack( RVREF(value) );	return *this; }

		ND_ T&			operator [] (usize index);
		ND_ T const&	operator [] (usize index) const;
		
		void Resize (usize newSize, bool allowReserve = true);
		void Reserve (usize size);
		
		void PushBack (const T& value);
		void PushBack (T&& value);

		void Insert (const T& value, usize pos);
		void Insert (T&& value, usize pos);

		template <typename SortComparator>
		void Sort (const SortComparator &func);
		void Sort ()											{ Sort( TSortCmp<T>() ); }

		void		Free ()										{ _array.Free(); }
		void		Clear ()									{ _array.Clear(); }

		ND_ bool	Empty ()		const						{ return _array.Empty(); }
		ND_ usize	Count ()		const						{ return _array.Count(); }
		ND_ usize	LastIndex ()	const						{ return _array.LastIndex(); }
		ND_ BytesU	Size ()			const						{ return _array.Size(); }
		

		// iterators
		using iterator			= IndexedIterator< T >;
		using const_iterator	= IndexedIterator< const T >;

		ND_ auto	begin ()				{ return iterator{ *this, 0 }; }
		ND_ auto	begin ()		const	{ return const_iterator{ *this, 0 }; }
		ND_ auto	end ()					{ return iterator{ *this, Count() }; }
		ND_ auto	end ()			const	{ return const_iterator{ *this, Count() }; }


		constexpr bool			IsStaticMemory () const			{ return _array.IsStaticMemory(); }
		static constexpr bool	IsLinearMemory ()				{ return Array_t::IsLinearMemory(); }
	};

	
	
/*
=================================================
	operator []
=================================================
*/
	template <typename T, typename S, typename MC>
	inline T&  IndexedArray<T,S,MC>::operator [] (usize index)
	{
		return _array[ _indices[index] ];
	}

	template <typename T, typename S, typename MC>
	inline T const&  IndexedArray<T,S,MC>::operator [] (usize index) const
	{
		return _array[ _indices[index] ];
	}
		
/*
=================================================
	Resize
=================================================
*/
	template <typename T, typename S, typename MC>
	inline void IndexedArray<T,S,MC>::Resize (const usize newSize, const bool allowReserve)
	{
		const usize	old_size = _array.Count();

		_array.Resize( newSize, allowReserve );
		_indices.Resize( newSize, allowReserve );

		for (usize i = old_size; i < _indices.Count(); ++i) {
			_indices[i] = i;
		}
	}
	
/*
=================================================
	Reserve
=================================================
*/
	template <typename T, typename S, typename MC>
	inline void IndexedArray<T,S,MC>::Reserve (const usize size)
	{
		_array.Reserve( size );
		_indices.Reserve( size );
	}
	
/*
=================================================
	PushBack
=================================================
*/
	template <typename T, typename S, typename MC>	
	inline void IndexedArray<T,S,MC>::PushBack (const T& value)
	{
		_indices.PushBack( _indices.Count() );
		_array.PushBack( value );
	}
	
	template <typename T, typename S, typename MC>
	inline void IndexedArray<T,S,MC>::PushBack (T&& value)
	{
		_indices.PushBack( _indices.Count() );
		_array.PushBack( RVREF(value) );
	}
	
/*
=================================================
	Insert
=================================================
*/
	template <typename T, typename S, typename MC>
	inline void IndexedArray<T,S,MC>::Insert (const T& value, const usize pos)
	{
		if ( pos < _indices.Count() )
		{
			_indices.PushBack( _indices[pos] );
			_indices[pos] = _indices.LastIndex();
		}
		else
			_indices.PushBack( _indices.Count() );

		_array.PushBack( value );
	}
	
	template <typename T, typename S, typename MC>
	inline void IndexedArray<T,S,MC>::Insert (T&& value, const usize pos)
	{
		if ( pos < _indices.Count() )
		{
			_indices.PushBack( _indices[pos] );
			_indices[pos] = _indices.LastIndex();
		}
		else
			_indices.PushBack( _indices.Count() );

		_array.PushBack( RVREF(value) );
	}
	
/*
=================================================
	Sort
=================================================
*/
	template <typename T, typename S, typename MC>
	template <typename SortComparator>
	inline void IndexedArray<T,S,MC>::Sort (const SortComparator &func)
	{
		struct SortCmp
		{
			SortComparator const&		func;
			Array_t const&				data;

			SortCmp (const SortComparator &func, const Array_t &data) :
				func(func), data(data)
			{}

			bool operator () (const usize left, const usize right) const
			{
				return func( data[left], data[right] );
			}
		};

		GXTypes::Sort( _indices, SortCmp( func, _array ) );
	}
	
/*
=================================================
	Hash
=================================================
*/
	template <typename T, typename S, typename MC>
	struct Hash< IndexedArray<T,S,MC> >
	{
		ND_ HashResult  operator () (const IndexedArray<T,S,MC> &x) const noexcept
		{
			return HashOf( ArrayCRef<T>(x) );
		}
	};

}	// GXTypes
}	// GX_STL
