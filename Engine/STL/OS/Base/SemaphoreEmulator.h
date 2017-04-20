// Copyright © 2014-2017  Zhirnov Andrey. All rights reserved.

#pragma once

namespace GX_STL
{
namespace OS
{


	//
	// Semaphore
	//

	struct SemaphoreEmulation
	{
	// types
	public:
		typedef SemaphoreEmulation	Self;

	// variables
	private:
		CriticalSection		_cs;
		ConditionVariable	_cv;
		uint				_count,
							_waitersCount;


	// methods
	private:
		bool _Create (uint initialValue)
		{
			_count = 0;
			return IsValid();
		}

	public:
		explicit
		SemaphoreEmulation (uint initialValue) : _count(0), _waitersCount(0)
		{
			_Create( initialValue );
		}

		~SemaphoreEmulation ()
		{
		}

		bool IsValid () const
		{
			return _cs.IsValid() and _cv.IsValid();
		}

		void Lock ()
		{
			_cs.Lock();

			++_waitersCount;

			while ( _count == 0 )
			{
				_cv.Wait( _cs );
			}

			--_waitersCount;
			--_count;

			_cs.Unlock();
		}

		bool TryLock ()
		{
			TODO( "TryLock" );
			return false;
		}

		void Unlock ()
		{
			_cs.Lock();

			if ( _waitersCount > 0 ) {
				_cv.Signal();
			}

			++_count;

			_cs.Unlock();
		}

		uint GetValue ()
		{
			uint	res = 0;

			_cs.Lock();
			res = _count;
			_cs.Unlock();

			return res;
		}
		
		ScopeLock GetScopeLock()
		{
			struct Util {
				static void Lock (void *p)		{ ((Self *)p)->Lock(); }
				static void Unlock (void *p)	{ ((Self *)p)->Unlock(); }
			};

			return ScopeLock( this, &Util::Lock, &Util::Unlock, false );
		}

		operator ScopeLock ()
		{
			return GetScopeLock();
		}
	};

}	// OS
}	// GX_STL
