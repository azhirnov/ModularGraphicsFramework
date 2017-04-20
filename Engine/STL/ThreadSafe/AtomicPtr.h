// Copyright © 2014-2017  Zhirnov Andrey. All rights reserved.

#pragma once

#include "Atomic.h"

namespace GX_STL
{
namespace GXTypes
{

# ifndef GX_USE_STD


	//
	// Atomic Pointer
	//

	template <typename T>
	struct AtomicPtr
	{
	// types
	public:
		typedef AtomicPtr<T>	Self;
		typedef T *				value_t;

	private:
		typedef OS::AtomicOp	Op;


	// variables
	private:
		volatile usize	_ptr;


	// methods
	public:
		AtomicPtr (GX_DEFCTOR) : _ptr(0) {}
		
		explicit AtomicPtr (value_t ptr) : _ptr( ReferenceCast<usize>(ptr) ) {}
		explicit AtomicPtr (const AtomicPtr<T> &other) : _ptr(other.Get()) {}
		
		operator const volatile vaue_t () const		{ return Get(); }
		
		value_t		Get ()	const					{ return ReferenceCast< value_t >( Op::Get<usize>( _ptr ) ); }

		void		Set (vaue_t right)				{ Op::Set<usize>( _ptr, ReferenceCast< usize >( right ) ); }
		
		value_t operator -> () const
		{
			value_t ptr = Get();
			ASSUME( ptr != null );
			return ptr;
		}

		T & operator * () const
		{
			value_t ptr = Get();
			ASSUME( ptr != null );
			return *ptr;
		}

		bool IsNull () const
		{
			return ( Get() == null );
		}

		bool IsNotNull () const
		{
			return ( Get() != null );
		}
	};

# else
	

	//
	// Atomic Pointer
	//

	template <typename T>
	struct AtomicPtr
	{
	// types
	public:
		typedef AtomicPtr<T>	Self;
		typedef T *				value_t;


	// variables
	private:
		volatile std::atomic< value_t >		_atomic;


	// methods
	public:
		AtomicPtr (GX_DEFCTOR) : _atomic(null) {}
		
		explicit AtomicPtr (value_t ptr) : _atomic( ptr ) {}
		explicit AtomicPtr (const AtomicPtr<T> &other) : _atomic(other.Get()) {}
		
		operator const volatile vaue_t () const		{ return Get(); }
		
		value_t		Get ()	const					{ return _atomic.load(); }

		void		Set (vaue_t right)				{ _atomic.store( right ); }
		
		value_t operator -> () const
		{
			value_t ptr = Get();
			ASSUME( ptr != null );
			return ptr;
		}

		T & operator * () const
		{
			value_t ptr = Get();
			ASSUME( ptr != null );
			return *ptr;
		}

		bool IsNull () const
		{
			return ( Get() == null );
		}

		bool IsNotNull () const
		{
			return ( Get() != null );
		}
	};

# endif	// GX_USE_STD

}	// GXTypes
}	// GX_STL
