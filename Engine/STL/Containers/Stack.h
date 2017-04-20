// Copyright © 2014-2017  Zhirnov Andrey. All rights reserved.

#pragma once

#include "Array.h"

namespace GX_STL
{
namespace GXTypes
{

	//
	// Dynamic Stack
	//

	template <	typename T,
				typename S = typename AutoDetectCopyStrategy<T>::type,
				typename MC = MemoryContainer<T>
			 >
	struct Stack : public CompileTime::CopyQualifiers< CompileTime::FastCopyable, MC >
	{
	// types
	public:
		typedef S						Strategy;
		typedef MC						MemoryContainer_t;
		typedef T						value_t;
		typedef Stack<T,S,MC>			Self;


	// variables
	private:
		Array<T,S,MC>	_memory;


	// methods
	public:
		Stack (GX_DEFCTOR)										{ _memory.Resize( 1 ); }
		
		Stack (const Self &other): _memory( other._memory )		{}

		Stack (Self &&other): _memory( RVREF(other._memory) )	{}

		~Stack ()										{}

		void SetDefault (const T &value)				{ _memory[0] = value; }
		void SetDefault (T &&value)						{ _memory[0] = RVREF( value ); }

		void Reserve (usize size)						{ _memory.Reserve( size ); }
		
		bool Pop ();
		
		void Push ();
		void Push (const T &value)						{ _memory.PushBack( value ); }
		void Push (T &&value)							{ _memory.PushBack( RVREF( value ) ); }

		T &  Get ()										{ return _memory.Back(); }
		const T & Get ()	const						{ return _memory.Back(); }
		
		void Set (const T &value)						{ _memory.Set( _memory.LastIndex(), value ); }
		void Set (T &&value)							{ _memory.Set( _memory.LastIndex(), RVREF( value ) ); }

		usize Capacity ()		const					{ return _memory.Capacity(); }
		usize MaxCapacity ()	const					{ return _memory.MaxSize(); }	// max available for allocation count of elements
		usize Count ()			const					{ return _memory.Count(); }
		void  Clear ()									{ _memory.Resize(1); }

		bool operator == (ArrayCRef<T> right) const		{ return ArrayCRef<T>(*this) == right; }
		bool operator != (ArrayCRef<T> right) const		{ return not ((*this) == right); }
		
		Self &	operator =  (Self &&right)		= default;
		Self &	operator =  (const Self &right)	= default;
		

		static constexpr bool	IsLinearMemory ()		{ return Array<T,S,MC>::IsLinearMemory(); }

		friend void SwapValues (INOUT Self &left, INOUT Self &right)
		{
			SwapValues( left._memory, right._memory );
		}
	};



	template <typename T, usize Count>
	using FixedSizeStack = Stack<T, typename AutoDetectCopyStrategy<T>::type, StaticMemoryContainer<T, Count> >;

	
/*
=================================================
	Pop
=================================================
*/
	template <typename T, typename S, typename MC>
	inline bool Stack<T,S,MC>::Pop ()
	{
		if ( _memory.Count() > 1 ) {
			_memory.PopBack();
			return true;
		}
		
		WARNING("stack is empty");
		return false;
	}
	
/*
=================================================
	Push
=================================================
*/
	template <typename T, typename S, typename MC>
	inline void Stack<T,S,MC>::Push ()
	{
		T	temp = _memory.Back();
		_memory.PushBack( RVREF( temp ) );
	}

	
/*
=================================================
	Hash
=================================================
*/
	template <typename T, typename S, typename MC>
	struct Hash< Stack<T,S,MC> > :
		private Hash< ArrayCRef<T> >
	{
		typedef Stack<T,S,MC>				key_t;
		typedef Hash< ArrayCRef<T> >		base_t;
		typedef typename base_t::result_t	result_t;

		result_t operator () (const key_t &x) const
		{
			return base_t::operator ()( x );
		}
	};

}	// GXTypes
}	// GX_STL
