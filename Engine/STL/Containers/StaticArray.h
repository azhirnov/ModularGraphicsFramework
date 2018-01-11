// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Array.h"

namespace GX_STL
{
namespace GXTypes
{

	
	#define RET_ERROR( _ret_ )	{ WARNING("error in static array");  return _ret_; }
	#define RET_FALSE			RET_ERROR( false )
	#define RET_VOID			RET_ERROR( ; )



	//
	// Static Array
	//

	template <	typename T,
				usize C,
				typename S = typename AutoDetectCopyStrategy<T>::type
			 >
	struct StaticArray : public CompileTime::CopyQualifiers< CompileTime::PODStruct, T >
	{
		STATIC_ASSERT( C > 0, "invalid array size" );

	// types
	public:
		using Self				= StaticArray<T,C,S>;
		using Strategy_t		= S;
		using Value_t			= T;
		using iterator			= T *;
		using const_iterator	= const T *;
		
		static const usize	STATIC_COUNT	= C;
		static const usize	STATIC_SIZE		= C * sizeof(T);


	// variables
	protected:
		union {
			T		_memory[C];
			char	_buffer[ sizeof(T)*C ];
		};


	// methods
	private:
		void _Move (Self &&other);

	public:
		StaticArray (GX_DEFCTOR);
		StaticArray (Self &&other);
		StaticArray (const Self &other);

		explicit StaticArray (const T& value);
		explicit StaticArray (ArrayCRef<T> other);
		explicit StaticArray (InitializerList<T> list);

		~StaticArray ();

		T		*	ptr ()										{ return _memory; }
		const T	*	ptr () const								{ return _memory; }
		
		T		&	Back ()										{ return _memory[Count()-1]; }
		const T	&	Back () const								{ return _memory[Count()-1]; }
		T		&	Front ()									{ return _memory[0]; }
		const T	&	Front () const								{ return _memory[0]; }

		bool		operator !  () const						{ return not Empty(); }
		
		bool		operator == (ArrayCRef<T> right) const		{ return ArrayCRef<T>(*this) == right; }
		bool		operator != (ArrayCRef<T> right) const		{ return not ( *this == right ); }
		
		Self &		operator =  (Self &&right)					{ _Move( RVREF(right) );  return *this; }
		Self &		operator =  (const Self &right)				{ Copy( right );  return *this; }

		operator	ArrayRef<T> ()								{ return ArrayRef<T>( _memory, Count() ); }
		operator	ArrayCRef<T> () const						{ return ArrayCRef<T>( _memory, Count() ); }

		T		&	operator [] (usize i);
		const T	&	operator [] (usize i) const;

		
		bool At (usize index, OUT T & value) const;

		void Free ();
		void Clear ();
		
		void Copy (ArrayCRef<T> other);

		bool Set (usize index, const T &value);
		bool Set (usize i, T &&value);

		void Swap (usize first, usize second);
		
		usize GetIndex (const T &value) const		{ return ArrayCRef<T>(*this).GetIndex( value ); }

		static constexpr bool		Empty ()		{ return false; }
		static constexpr usize		Count ()		{ return C; }
		static constexpr usize		Capacity ()		{ return C; }
		static constexpr usize		MaxCapacity ()	{ return Capacity(); }	// max available for allocation count of elements
		static constexpr BytesU		Size ()			{ return BytesU( Count() * sizeof(T) ); }
		static constexpr BytesU		FullSize ()		{ return BytesU( Capacity() * sizeof(T) ); }
		static constexpr usize		LastIndex ()	{ return Count()-1; }
	
		ArrayRef<T>			SubArray (usize pos, usize count = UMax)			{ return ArrayRef<T>(*this).SubArray( pos, count ); }
		ArrayCRef<T>		SubArray (usize pos, usize count = UMax)	const	{ return ArrayCRef<T>(*this).SubArray( pos, count ); }


		iterator		Begin ()				{ return _memory; }
		const_iterator	Begin () const			{ return _memory; }

		iterator		End ()					{ return _memory + Count(); }
		const_iterator	End () const			{ return _memory + Count(); }

		iterator		GetIter (usize index);
		const_iterator	GetIter (usize index) const;

		bool IsBegin (const_iterator iter)	const;
		bool IsEnd (const_iterator iter)	const;
		

		static constexpr bool	IsLinearMemory ()	{ return true; }
		static constexpr bool	IsStaticMemory ()	{ return true; }


		friend void SwapValues (INOUT Self &left, INOUT Self &right)
		{
			for (usize i = 0; i < Count(); ++i) {
				SwapValues( left[i], right[i] );
			}
		}
	};


	
/*
=================================================
	constructor
=================================================
*/
	template <typename T, usize C, typename S>
	inline StaticArray<T,C,S>::StaticArray (UninitializedType)
	{
		Strategy_t::Create( _memory, Count() );
	}
		
/*
=================================================
	constructor
=================================================
*/
	template <typename T, usize C, typename S>
	inline StaticArray<T,C,S>::StaticArray (const StaticArray<T,C,S> &other)
	{
		Strategy_t::Copy( _memory, other._memory, Count() );
	}
	
/*
=================================================
	constructor
=================================================
*/
	template <typename T, usize C, typename S>
	inline StaticArray<T,C,S>::StaticArray (ArrayCRef<T> other)
	{
		Copy( other );
	}
	
/*
=================================================
	constructor
=================================================
*/
	template <typename T, usize C, typename S>
	inline StaticArray<T,C,S>::StaticArray (Self &&other)
	{
		_Move( RVREF(other) );
	}
	
/*
=================================================
	_Move
=================================================
*/
	template <typename T, usize C, typename S>
	inline void StaticArray<T,C,S>::_Move (Self &&other)
	{
		// call move-ctor, but don't call destructor for elements in 'other'
		Strategy_t::Move( _memory, other._memory, Count() );
	}

/*
=================================================
	destructor
=================================================
*/
	template <typename T, usize C, typename S>
	inline StaticArray<T,C,S>::~StaticArray ()
	{
		Strategy_t::Destroy( _memory, Count() );
	}

/*
=================================================
	constructor
=================================================
*/
	template <typename T, usize C, typename S>
	inline void StaticArray<T,C,S>::Copy (ArrayCRef<T> other)
	{
		if ( other.Empty() )
		{
			Free();
			return;
		}

		if ( CheckPointersAliasing( Begin(), End(), other.Begin(), other.End() ) )
			RET_VOID;

		if ( other.Count() > Count() )
		{
			WARNING( "source array is too big!" );
			Strategy_t::Copy( _memory, other.ptr(), Count() );
		}
		else
		{
			Strategy_t::Copy( _memory, other.ptr(), other.Count());
			Strategy_t::Create( _memory + other.Count(), Count() - other.Count() );
		}
	}
	
/*
=================================================
	constructor
=================================================
*/
	template <typename T, usize C, typename S>
	inline StaticArray<T,C,S>::StaticArray (const T& value)
	{
		for (usize i = 0; i < Count(); ++i) {
			Strategy_t::Copy( _memory + i, &value, 1 );
		}
	}
	
/*
=================================================
	constructor
=================================================
*/
	template <typename T, usize C, typename S>
	inline StaticArray<T,C,S>::StaticArray (InitializerList<T> list)
	{
		Copy( ArrayCRef<T>( list ) );
	}
	
/*
=================================================
	operator []
=================================================
*/
	template <typename T, usize C, typename S>
	inline T & StaticArray<T,C,S>::operator [] (usize index)
	{
		ASSUME( index < Count() );
		return _memory[ index ];
	}

	template <typename T, usize C, typename S>
	inline const T & StaticArray<T,C,S>::operator [] (usize index) const
	{
		ASSUME( index < Count() );
		return _memory[ index ];
	}
	
/*
=================================================
	At
=================================================
*/
	template <typename T, usize C, typename S>
	inline bool StaticArray<T,C,S>::At (usize index, T & value) const
	{
		if ( index >= Count() )  return false;
		Strategy_t::Copy( &value, &_memory + index, 1 );
		return true;
	}
	
/*
=================================================
	Set
=================================================
*/
	template <typename T, usize C, typename S>
	inline bool StaticArray<T,C,S>::Set (usize index, const T &value)
	{
		if ( index >= Count() ) return false;
		Strategy_t::Copy( _memory + index, &value, 1 );
		return true;
	}
	
	template <typename T, usize C, typename S>
	inline bool StaticArray<T,C,S>::Set (usize index, T&& value)
	{
		if ( index >= Count() ) return false;
		Strategy_t::Move( _memory + index, &value, 1 );
		return true;
	}
	
/*
=================================================
	Free
=================================================
*/
	template <typename T, usize C, typename S>
	inline void StaticArray<T,C,S>::Free ()
	{
		Clear();
	}
	
/*
=================================================
	Clear
=================================================
*/
	template <typename T, usize C, typename S>
	inline void StaticArray<T,C,S>::Clear ()
	{
		Strategy_t::Destroy( _memory, Count() );
		Strategy_t::Create(  _memory, Count() );
	}
	
/*
=================================================
	Swap
=================================================
*/
	template <typename T, usize C, typename S>
	inline void StaticArray<T,C,S>::Swap (usize first, usize second)
	{
		if ( first >= Count() or second >= Count() )
			RET_VOID;

		T	temp;
		Strategy_t::Replace( &temp,				_memory + second,	1 );
		Strategy_t::Replace( _memory + second,	_memory + first,	1 );
		Strategy_t::Replace( _memory + first,	&temp,				1 );
	}
	
/*
=================================================
	GetIter
=================================================
*/
	template <typename T, usize C, typename S>
	inline T * StaticArray<T,C,S>::GetIter (usize index)
	{
		ASSERT( index < Count() );
		return _memory + index;
	}
	
	template <typename T, usize C, typename S>
	inline const T * StaticArray<T,C,S>::GetIter (usize index) const
	{
		ASSERT( index < Count() );
		return _memory + index;
	}
	
/*
=================================================
	IsBegin
=================================================
*/
	template <typename T, usize C, typename S>
	inline bool StaticArray<T,C,S>::IsBegin (const_iterator iter) const
	{
		return iter == Begin();
	}
	
/*
=================================================
	IsEnd
=================================================
*/
	template <typename T, usize C, typename S>
	inline bool StaticArray<T,C,S>::IsEnd (const_iterator iter) const
	{
		return iter == End();
	}


	#undef RET_ERROR
	#undef RET_FALSE
	#undef RET_VOID
	
	
/*
=================================================
	Hash
=================================================
*/
	template <typename T, usize C, typename S>
	struct Hash< StaticArray<T,C,S> > :
		private Hash< ArrayCRef<T> >
	{
		typedef StaticArray<T,C,S>			Key_t;
		typedef Hash< ArrayCRef<T> >		Base_t;
		typedef typename Base_t::Result_t	Result_t;

		Result_t operator () (const Key_t &x) const noexcept
		{
			return Base_t::operator ()( x );
		}
	};

/*
=================================================
	MakeStaticArray
=================================================
*/
	namespace _types_hidden_
	{
		template <typename ...Types>
		struct _MakeStaticArrayHelper
		{
			typedef CompileTime::TypeListFrom< Types... >	list;

			typedef StaticArray< typename list::Front, list::Count >	type;

			STATIC_ASSERT( list::AllAreSame );
		};
		
		template <typename ...Types>
		using StaticArrayFrom = typename _MakeStaticArrayHelper< Types... >::type;

	}	// _types_hidden_

	template <typename ...Args>
	forceinline typename _types_hidden_::StaticArrayFrom<Args...>  MakeStaticArray (Args... args)
	{
		return typename _types_hidden_::StaticArrayFrom<Args...>{ FW<Args>(args)... };
	}


}	// GXTypes
}	// GX_STL
