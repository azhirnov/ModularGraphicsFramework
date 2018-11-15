// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Core/STL/ThreadSafe/Atomic.h"
#include "Core/STL/ThreadSafe/AtomicCounter.h"
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
		AtomicCounter< T >		_counter;


	// methods
	public:
		RefCounter ()
		{}

		~RefCounter ()
		{
			ASSERT( _counter.Load() == 0 );
		}

		void Inc ()
		{
			++_counter;
		}

		ND_ bool  DecAndTest ()
		{
			return _counter.DecAndTest();
		}

		ND_ T  Count () const
		{
			return _counter.Load();
		}
	};



	//
	// Reference Counter (Shared + Weak)
	//

	class RefCounter2 final
	{
	// types
	private:
		using T		= int;

		struct _Internal
		{
			AtomicCounter<T>	count		{ 0 };
			AtomicCounter<T>	weakCount	{ 1 };
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


		void  Inc ()
		{
			ASSUME( _ptr );
			++_ptr->count;
		}

		ND_ bool  TryInc ()
		{
			ASSUME( _ptr );
			for (;;)
			{
				const T	cnt = _ptr->count.Load();

				if ( cnt == 0 )
					return false;

				if ( _ptr->count.CmpExch( cnt+1, cnt ) == cnt )
					return true;
			}
		}

		ND_ bool  DecAndTest ()
		{
			ASSUME( _ptr );
			return _ptr->count.DecAndTest();
		}

		ND_ T  Count () const
		{
			return _ptr ? _ptr->count.Load() : 0;
		}

		void  IncWeak ()
		{
			ASSUME( _ptr );
			++_ptr->weakCount;
		}

		void  DecWeak ()
		{
			ASSUME( _ptr );
			if ( _ptr->weakCount.DecAndTest() )
				_Destroy();
		}

		ND_ T  CountWeak () const
		{
			return _ptr ? _ptr->weakCount.Load() : 0;
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
			ASSERT( _ptr->weakCount.Load() == 0 );
			delete _ptr;
			_ptr = null;
			DEBUG_ONLY( --_AllocCounter() );
		}
	};


}	// GXTypes
}	// GX_STL
