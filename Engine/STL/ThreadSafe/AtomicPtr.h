// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/STL/ThreadSafe/Atomic.h"

namespace GX_STL
{
namespace GXTypes
{

	//
	// Atomic Pointer
	//

	template <typename T>
	struct AtomicPtr
	{
	// types
	public:
		using Self	= AtomicPtr<T>;
		using Ptr_t	= T *;


	// variables
	private:
		volatile Atomic<usize>	_ptr;


	// methods
	public:
		AtomicPtr (GX_DEFCTOR) : _ptr(0) {}
		
		explicit AtomicPtr (Ptr_t ptr) : _ptr( ReferenceCast<usize>(ptr) ) {}

		AtomicPtr (const Self &) = default;
		AtomicPtr (Self &&) = default;
		
		Self& operator = (const Self &) = default;
		Self& operator = (Self &&) = default;

		operator const Ptr_t () const			{ return Get(); }
		
		Ptr_t		Get ()	const				{ return ReferenceCast< Ptr_t >( _ptr.Get() ); }

		void		Set (Ptr_t right)			{ _ptr = ReferenceCast< usize >( right ); }
		
		Ptr_t operator -> () const
		{
			Ptr_t	ptr = Get();
			ASSUME( ptr != null );
			return ptr;
		}

		T & operator * () const
		{
			Ptr_t	ptr = Get();
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


}	// GXTypes
}	// GX_STL
