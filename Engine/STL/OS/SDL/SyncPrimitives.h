// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/STL/Common/Platforms.h"

#ifdef PLATFORM_SDL

#include "Engine/STL/OS/Base/ScopeLock.h"
#include "Engine/STL/OS/SDL/OS_SDL.h"

namespace GX_STL
{
namespace OS
{


	//
	// Critical Section
	//

	struct CriticalSection
	{
		friend struct ConditionVariable;

	// types
	public:
		typedef	CriticalSection		Self;


	// variables
	private:
		Ptr< SDL_mutex >	_mutex;


	// methods
	private:
		bool _Create ()
		{
			_Delete();
			_mutex = ::SDL_CreateMutex();
			return IsValid();
		}

		void _Delete ()
		{
			if ( _mutex )
			{
				::SDL_DestroyMutex( _mutex );
				_mutex = null;
			}
		}

	public:
		CriticalSection ()
		{
			_Create();
		}

		~CriticalSection ()
		{
			_Delete();
		}

		bool IsValid () const
		{
			return _mutex.IsNotNull();
		}

		void Lock ()
		{
			int result = ::SDL_LockMutex( _mutex );
			ASSERT( result != -1 );
		}

		bool TryLock ()
		{
			int result = ::SDL_TryLockMutex( _mutex );
			ASSERT( result != -1 );
			return result == 0;
		}

		void Unlock ()
		{
			int result = ::SDL_UnlockMutex( _mutex );
			ASSERT( result != -1 );
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



	//
	// Semaphore
	//

	struct Semaphore
	{
	// types
	public:
		typedef	Semaphore	Self;


	// variables
	private:
		Ptr< SDL_sem >	_sem;


	// methods
	private:
		bool _Create (uint initialValue)
		{
			_Delete();
			_sem = ::SDL_CreateSemaphore( initialValue );
			return IsValid();
		}

		void _Delete ()
		{
			if ( _sem )
			{
				::SDL_DestroySemaphore( _sem );
				_sem = null;
			}
		}

	public:
		explicit
		Semaphore (uint initialValue)
		{
			_Create( initialValue );
		}

		~Semaphore ()
		{
			_Delete();
		}

		bool IsValid () const
		{
			return _sem.IsNotNull();
		}

		void Lock ()
		{
			int result = ::SDL_SemWait( _sem );
			ASSERT( result != -1 );
		}

		bool TryLock ()
		{
			int result = ::SDL_SemTryWait( _sem );
			ASSERT( result != -1 );
			return result == 0;
		}

		void Unlock ()
		{
			int result = ::SDL_SemPost( _sem );
			ASSERT( result != -1 );
		}

		uint GetValue ()
		{
			return ::SDL_SemValue( _sem );
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



	//
	// Condition Variable
	//

	struct ConditionVariable
	{
	// variables
	private:
		Ptr< SDL_cond >		_cv;


	// methods
	private:
		bool _Create ()
		{
			_Delete();
			_cv = ::SDL_CreateCond();
			return IsValid();
		}

		void _Delete ()
		{
			if ( _cv )
			{
				::SDL_DestroyCond( _cv );
				_cv = null;
			}
		}

	public:
		ConditionVariable ()
		{
			_Create();
		}

		~ConditionVariable ()
		{
			_Delete();
		}

		bool IsValid () const
		{
			return _cv.IsNotNull();
		}

		void Signal ()
		{
			int result = ::SDL_CondSignal( _cv );
			ASSERT( result != -1 );
		}

		void Broadcast ()
		{
			int result = ::SDL_CondBroadcast( _cv );
			ASSERT( result != -1 );
		}

		bool Wait (const CriticalSection &mutex)
		{
			int result = ::SDL_CondWait( _cv, mutex._mutex );
			ASSERT( result != -1 );
			return result != -1;
		}

		bool Wait (const CriticalSection &mutex, TimeL time)
		{
			int result = ::SDL_CondWaitTimeout( _cv, mutex._mutex, (uint)time.MilliSeconds() );
			ASSERT( result != -1 );
			return result != -1;
		}
	};


	struct SyncEventEmulation;
	struct ReadWriteSyncEmulation;

	typedef SyncEventEmulation		SyncEvent;
	typedef ReadWriteSyncEmulation	ReadWriteSync;

}	// OS
}	// GX_STL

#include "Engine/STL/OS/Base/SyncEventEmulation.h"
#include "Engine/STL/OS/Base/ReadWriteSyncEmulation.h"

#endif	// PLATFORM_SDL