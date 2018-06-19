// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Core/STL/ThreadSafe/Atomic.h"
#include "Core/STL/Types/Noncopyable.h"

namespace GX_STL
{
namespace GXTypes
{

	//
	// Reference Counter (Shared only)
	//

	class RefCounter final
	{
	// types
	private:
		using T	= int;


	// variables
	private:
		Atomic< T >		_counter;


	// methods
	public:
		RefCounter ()
		{}

		explicit RefCounter (RefCounter &&other) : _counter(other._counter)
		{
			other._counter = 0;
		}

		explicit RefCounter (const RefCounter &) : _counter(1)
		{}

		void operator = (RefCounter &&other)
		{
			_counter = other._counter.Get();
			other._counter = 0;
		}

		void operator = (const RefCounter &)
		{
			_counter = 1;
		}

		~RefCounter ()
		{
			ASSERT( _counter == 0 );
		}

		T  Inc ()
		{
			return ++_counter;
		}

		T  Dec ()
		{
			ASSERT( _counter > 0 );
			return --_counter;
		}

		ND_ T  Count () const
		{
			return _counter.Get();
		}
	};



	//
	// Reference Counter (Shared + Weak)
	//

	class RefCounter2 final
	{
	// types
	private:
		using Op	= OS::AtomicOp;
		using T		= int;

		struct _Internal
		{
			volatile T	count		= 0;
			volatile T	weakCount	= 1;
		};


	// variables
	private:
		_Internal *		_ptr	= null;


	// methods
	public:
		RefCounter2 () : _ptr{ new _Internal() }
		{
			DEBUG_ONLY( ++_AllocCounter() );
		}

		RefCounter2 (NullPtr_t)
		{}

		explicit RefCounter2 (const RefCounter2 &other) : _ptr( other._ptr )
		{}

		RefCounter2 (RefCounter2 &&) = delete;


		void operator = (const RefCounter2 &right)	{ _ptr = right._ptr; }

		void operator = (RefCounter2 &&) = delete;


		bool operator == (const RefCounter2 &right) const	{ return _ptr == right._ptr; }


		T  Inc ()
		{
			ASSUME( _ptr );
			return Op::Inc<T>( _ptr->count );
		}

		ND_ bool  TryInc ()
		{
			ASSUME( _ptr );
			for (;;)
			{
				T cnt = Op::Get<T>( _ptr->count );

				if ( cnt == 0 )
					return false;

				if ( Op::CmpExch<T>( _ptr->count, cnt+1, cnt ) == cnt )
					return true;
			}
		}

		T  Dec ()
		{
			ASSUME( _ptr );
			return Op::Dec<T>( _ptr->count );
		}

		ND_ T  Count () const
		{
			return _ptr ? Op::Get<T>( _ptr->count ) : 0;
		}

		T  IncWeak ()
		{
			ASSUME( _ptr );
			return Op::Inc<T>( _ptr->weakCount );
		}

		T  DecWeak ()
		{
			ASSUME( _ptr );
			const T res = Op::Dec<T>( _ptr->weakCount );
			if ( res == 0 ) _Destroy();
			return res;
		}

		ND_ T  CountWeak () const
		{
			return _ptr ? Op::Get<T>( _ptr->weakCount ) : 0;
		}
		
		DEBUG_ONLY(
		static void s_CheckAllocations (int refValue = 0)
		{
			const auto num_allocations = _AllocCounter().Get();

			if ( num_allocations != refValue )
				GX_BREAK_POINT();
		})

	private:
		DEBUG_ONLY(
		static Atomic<int>& _AllocCounter ()
		{
			static Atomic<int> staticCounter { 0 };
			return staticCounter;
		})

		void _Destroy ()
		{
			ASSUME( _ptr );
			ASSERT( _ptr->weakCount == 0 );
			delete _ptr;
			_ptr = null;
			DEBUG_ONLY( --_AllocCounter() );
		}
	};


}	// GXTypes
}	// GX_STL
