// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/STL/Containers/Array.h"
#include "Engine/STL/Algorithms/Sorts.h"

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

		T&			operator [] (usize index);
		T const&	operator [] (usize index) const;

		Self&		operator << (const T &value)					{ PushBack( value );		return *this; }
		Self&		operator << (T &&value)							{ PushBack( RVREF(value) );	return *this; }
		
		void Resize (usize newSize, bool allowReserve = true);
		void Reserve (usize size);
		
		void PushBack (const T& value);
		void PushBack (T&& value);

		void Insert (const T& value, usize pos);
		void Insert (T&& value, usize pos);

		template <typename SortComparator>
		void Sort (const SortComparator &func);
		void Sort ()								{ Sort( TSortCmp<T>() ); }

		void	Free ()								{ _array.Free(); }
		void	Clear ()							{ _array.Clear(); }

		bool	Empty ()		const				{ return _array.Empty(); }
		usize	Count ()		const				{ return _array.Count(); }
		usize	LastIndex ()	const				{ return _array.LastIndex(); }
		BytesU	Size ()			const				{ return _array.Size(); }

		constexpr bool IsStaticMemory ()	const	{ return _array.IsStaticMemory(); }
		static constexpr bool IsLinearMemory ()		{ return Array_t::IsLinearMemory(); }
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
	inline void IndexedArray<T,S,MC>::Resize (usize newSize, bool allowReserve)
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
	inline void IndexedArray<T,S,MC>::Reserve (usize size)
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
	inline void IndexedArray<T,S,MC>::Insert (const T& value, usize pos)
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
	inline void IndexedArray<T,S,MC>::Insert (T&& value, usize pos)
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
	struct Hash< IndexedArray<T,S,MC> > :
		private Hash< ArrayCRef<T> >
	{
		typedef IndexedArray<T,S,MC>		Key_t;
		typedef Hash< ArrayCRef<T> >		Base_t;
		typedef typename Base_t::Result_t	Result_t;

		Result_t operator () (const Key_t &x) const noexcept
		{
			return Base_t::operator ()( x );
		}
	};

}	// GXTypes
}	// GX_STL
