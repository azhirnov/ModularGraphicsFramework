// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/STL/Containers/Array.h"
#include "Engine/STL/Containers/StaticArray.h"

namespace GX_STL
{
namespace GXTypes
{

	//
	// Sort Compare
	//

	template <typename T>
	struct TSortCmp
	{
		// sort from smallest to biggest
		forceinline bool operator () (const T& val1, const T& val2)	const  { return val1 > val2; }
	};

	
# define SORT_FUNCTIONS( _name_ ) \
	\
	namespace _sort_hidden_ \
	{ \
		template <typename CmpOp, typename T> \
		forceinline void _name_ (ArrayRef<T> arr, CmpOp cmp) \
		{ \
			STATIC_ASSERT( not TypeTraits::IsConst<T> ); \
			\
			if ( arr.Empty() ) return; \
			_name_( arr.ptr(), arr.Count(), cmp ); \
		} \
	} \
	\
	template <template <typename ...> class LinearMemoryContainer, typename ...Types> \
	forceinline void _name_ (LinearMemoryContainer<Types...> &arr) \
	{ \
		typedef typename LinearMemoryContainer<Types...>::value_t	T; \
		TSortCmp<T>	cmp; \
		_sort_hidden_::_name_( ArrayRef<T>( arr ), cmp ); \
	} \
	\
	template <typename CmpOp, template <typename ...> class LinearMemoryContainer, typename ...Types> \
	forceinline void _name_ (LinearMemoryContainer<Types...> &arr, CmpOp cmp) \
	{ \
		typedef typename LinearMemoryContainer<Types...>::value_t	T; \
		_sort_hidden_::_name_( ArrayRef<T>( arr ), cmp ); \
	}


/*
=================================================
	BubbleSort	O(n^2)
=================================================
*/
	namespace _sort_hidden_
	{
		template <typename T, typename C>
		inline void BubbleSort (T * pArray, const usize uCount, C sCmp)
		{
			if ( uCount == 0 )
				return;

			ArrayRef<T>	arr( pArray, uCount );
			bool		b_flag = true;

			while ( b_flag )
			{
				b_flag = false;
				for (usize i = 0; i < uCount-1; ++i)
				{
					if ( sCmp( arr[i], arr[i+1] ) )
					{
						SwapValues( arr[i], arr[i+1] );
						b_flag = true;
					}
				}
			}
		}
	}	// _sort_hidden_

	SORT_FUNCTIONS( BubbleSort );
		
/*
=================================================
	SelectSort  	O(n^2)
=================================================
*/
	namespace _sort_hidden_
	{
		template <typename T, typename C>
		inline void SelectSort (T * pArray, const usize uCount, C sCmp) 
		{
			if ( uCount == 0 )
				return;

			usize		a			= 0,
						b			= 0,
						c			= 0;
			bool		exchange	= false;
			T			temp;
			ArrayRef<T>	arr( pArray, uCount );
		
			for (a = 0; a < uCount-1; ++a)
			{
				exchange = false;
				c		 = a;
				temp	 = arr[a];

				for (b = a+1; b < uCount; ++b) 
				{
					if ( sCmp( temp, arr[b] ) )
					{
						c		 = b;
						exchange = true;
						temp	 = arr[b];
					}
				}

				if ( exchange )
				{
					arr[c] = arr[a];
					arr[a] = temp;
				}
			}
		}
	}	// _sort_hidden_

	SORT_FUNCTIONS( SelectSort );

/*
=================================================
	InsertionSort		O(n^2)
=================================================
*/
	namespace _sort_hidden_
	{
		template <typename T, typename C>
		inline void InsertionSort (T * pArray, const usize uCount, C sCmp)
		{
			if ( uCount == 0 )
				return;

			isize		i	= 0;
			T			temp;
			ArrayRef<T>	arr( pArray, uCount );

			for (usize j = 1; j < uCount; ++j)
			{
				temp = arr[j];
				i	 = j-1;

				while ( not sCmp( temp, arr[i] ) and i >= 0 )
				{
					arr[i+1] = arr[i];
					--i;
				}
				arr[i+1] = temp;
			}
		}
	}	// _sort_hidden_
	
	SORT_FUNCTIONS( InsertionSort );

/*
=================================================
	ShakerSort		O(n^2)
=================================================
*/
	namespace _sort_hidden_
	{
		template <typename T, typename C>
		inline void ShakerSort (T * pArray, const usize uCount, C sCmp)
		{
			if ( uCount == 0 )
				return;

			usize		i			= 0;
			bool		exchange	= false;
			ArrayRef<T>	arr( pArray, uCount );

			do {
				exchange = false;

				for (i = uCount-1; i > 0; --i)
				{
					if ( not sCmp( arr[i], arr[i-1] ) )
					{
						SwapValues( arr[i-1], arr[i] );
						exchange	= true;
					}
				}
			
				for (i = 1; i < uCount; ++i)
				{
					if ( not sCmp( arr[i], arr[i-1] ) )
					{
						SwapValues( arr[i-1], arr[i] );
						exchange	= true;
					}
				}
			}
			while ( exchange );
		}
	}	// _sort_hidden_
	
	SORT_FUNCTIONS( ShakerSort );

/*
=================================================
	ShellSort	O(n * log^2(n))
=================================================
*/
	namespace _sort_hidden_
	{
		template <typename T, typename C>
		inline void ShellSort (T * pArray, const usize uCount, C sCmp)
		{
			if ( uCount == 0 )
				return;

			usize		i	= 0,
						gap	= 0;
			isize		j	= 0,
						k	= 0;
			T			temp;
			char		a[5] = { 8, 5, 3, 2, 1 };
			ArrayRef<T>	arr( pArray, uCount );

			for (k = 0; k < 5; ++k)
			{
				gap = a[k];

				for (i = gap; i < uCount; ++i)
				{
					temp = RVREF( arr[i] );

					for (j = i-gap; (j >= 0) and sCmp( arr[j], temp ); j = j-gap)
					{
						arr[j+gap] = RVREF( arr[j] );
					}
					arr[j+gap] = RVREF( temp );
				}
			}
		}
	}	// _sort_hidden_

	SORT_FUNCTIONS( ShellSort );

/*
=================================================
	QuickSort	O(n log n)
=================================================
*/
	namespace _sort_hidden_
	{
		template <typename T, typename C>
		inline void _QuickSort (ArrayRef<T> sArray, const isize iLeft, const isize iRight, C sCmp)
		{
			isize	i	= iLeft,
					j	= iRight,
					idx	= (iLeft + iRight) >> 1;

			do {
				while ( (i < iRight) and sCmp( sArray[idx], sArray[i]   ) )		++i;
				while ( (j > iLeft)  and sCmp( sArray[j],   sArray[idx] ) )		--j;
			
				if ( i <= j )
				{
					if ( i == idx )
						idx = j;
					else
					if ( j == idx )
						idx = i;

					SwapValues( sArray[i], sArray[j] );
					++i;
					--j;
				}
			}
			while( i <= j );

			if ( i < iRight )
				_QuickSort( sArray, i, iRight, sCmp );

			if ( iLeft < j )
				_QuickSort( sArray, iLeft, j, sCmp );
		}
	
		template <typename T, typename C>
		void QuickSort (T * pArray, const usize uCount, C sCmp)
		{
			if ( uCount == 0 )
				return;
		
			_QuickSort( ArrayRef<T>( pArray, uCount ), 0, uCount-1, sCmp );
		}
	}	// _sort_hidden_
	
	SORT_FUNCTIONS( QuickSort );

/*
=================================================
	Sort
=================================================
*/
	namespace _sort_hidden_
	{
		template <typename T, typename C>
		inline void Sort (T * pArray, const usize uCount, C sCmp)
		{
			_sort_hidden_::QuickSort( pArray, uCount, sCmp );
			//_sort_hidden_::ShellSort( pArray, uCount, sCmp );
		}
	}	// _sort_hidden_
	
	SORT_FUNCTIONS( Sort );
	
/*
=================================================
	SortArrayIndices
=================================================
*/
	namespace _sort_hidden_
	{
		template <typename CmpOp, typename T, typename I>
		inline void _SortArrayIndices (ArrayCRef<T> arr, ArrayRef<I> idxs, CmpOp cmp)
		{
			CompileTime::MustBeInteger<I>();

			if ( arr.Empty() or idxs.Empty() )
				return;

			if ( arr.Count() != idxs.Count() )
			{
				WARNING( "array sizes missmatch!" );
				return;
			}

			usize	i	 = 0,
					gap	 = 0;
			isize	j	 = 0,
					k	 = 0;
			char	a[5] = { 8, 5, 3, 2, 1 };
			I		temp;

			FOR( u, idxs ) {
				idxs[u] = u;
			}

			for (k = 0; k < 5; ++k)
			{
				gap = a[k];

				for (i = gap; i < idxs.Count(); ++i)
				{
					temp = idxs[i];

					for (j = i-gap; (j >= 0) and cmp( arr[ idxs[j] ], arr[ temp ] ); j = j-gap)
					{
						idxs[j+gap] = idxs[j];
					}

					idxs[j+gap] = temp;
				}
			}
		}
	}	// _sort_hidden_

	template <typename ArrayType, typename IndicesArrayType>
	inline void SortArrayIndices (const ArrayType &arr, INOUT IndicesArrayType &indices)
	{
		TSortCmp< ArrayType::value_t >	cmp;
		_SortArrayIndices( ArrayCRef<ArrayType::value_t>( arr ), ArrayRef<IndicesArrayType::value_t>( indices ), cmp );
	}

	template <typename CmpOp, typename ArrayType, typename IndicesArrayType>
	inline void SortArrayIndices (const ArrayType &arr, INOUT IndicesArrayType &indices, CmpOp cmp)
	{
		_SortArrayIndices( ArrayCRef<ArrayType::value_t>( arr ), ArrayRef<IndicesArrayType::value_t>( indices ), cmp );
	}


#undef SORT_FUNCTIONS


}	// GXTypes
}	// GX_STL
