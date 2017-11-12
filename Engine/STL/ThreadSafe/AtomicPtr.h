// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

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
		typedef T *				Value_t;

	private:
		typedef OS::AtomicOp	Op;


	// variables
	private:
		volatile usize	_ptr;


	// methods
	public:
		AtomicPtr (GX_DEFCTOR) : _ptr(0) {}
		
		explicit AtomicPtr (Value_t ptr) : _ptr( ReferenceCast<usize>(ptr) ) {}
		explicit AtomicPtr (const AtomicPtr<T> &other) : _ptr(other.Get()) {}
		
		operator const volatile vaue_t () const		{ return Get(); }
		
		Value_t		Get ()	const					{ return ReferenceCast< Value_t >( Op::Get<usize>( _ptr ) ); }

		void		Set (vaue_t right)				{ Op::Set<usize>( _ptr, ReferenceCast< usize >( right ) ); }
		
		Value_t operator -> () const
		{
			Value_t ptr = Get();
			ASSUME( ptr != null );
			return ptr;
		}

		T & operator * () const
		{
			Value_t ptr = Get();
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
		typedef T *				Value_t;


	// variables
	private:
		volatile std::atomic< Value_t >		_atomic;


	// methods
	public:
		AtomicPtr (GX_DEFCTOR) : _atomic(null) {}
		
		explicit AtomicPtr (Value_t ptr) : _atomic( ptr ) {}
		explicit AtomicPtr (const AtomicPtr<T> &other) : _atomic(other.Get()) {}
		
		operator const volatile vaue_t () const		{ return Get(); }
		
		Value_t		Get ()	const					{ return _atomic.load(); }

		void		Set (vaue_t right)				{ _atomic.store( right ); }
		
		Value_t operator -> () const
		{
			Value_t ptr = Get();
			ASSUME( ptr != null );
			return ptr;
		}

		T & operator * () const
		{
			Value_t ptr = Get();
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
