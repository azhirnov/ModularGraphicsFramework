// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/STL/OS/Base/SyncPrimitives.h"
#include "OSPosix.h"

#ifdef PLATFORM_BASE_POSIX_SHELL

namespace GX_STL
{
namespace OS
{
	using namespace posix;


	//
	// Critical Section
	//

	struct CriticalSection
	{
		friend struct ConditionVariable;

	// types
	public:
		typedef CriticalSection		Self;


	// variables
	private:
		pthread_mutex_t	_mutex;
		bool			_inited;


	// methods
	private:
		bool _Create ()
		{
			_Delete();
			_inited = pthread_mutex_init( &_mutex, null ) == 0;
			return IsValid();
		}

		bool _Delete ()
		{
			bool	ret = true;

			if ( IsValid() )
			{
				static const pthread_mutex_t	tmp = PTHREAD_MUTEX_INITIALIZER;

				ret = pthread_mutex_destroy( &_mutex ) == 0;
				_mutex  = tmp;
				_inited = false;
			}
			return ret;
		}

	public:
		CriticalSection ()
		{
			static const pthread_mutex_t	tmp = PTHREAD_MUTEX_INITIALIZER;

			_mutex = tmp;

			_Create();
		}

		~CriticalSection ()
		{
			_Delete();
		}

		bool IsValid () const
		{
			return _inited;
		}

		void Lock ()
		{
			ASSERT( IsValid() );
			bool	res = pthread_mutex_lock( &_mutex ) == 0;
			ASSERT( res );
		}

		bool TryLock ()
		{
			ASSERT( IsValid() );
			return pthread_mutex_trylock( &_mutex ) == 0;
		}

		void Unlock ()
		{
			ASSERT( IsValid() );
			bool	res = pthread_mutex_unlock( &_mutex ) == 0;
			ASSERT( res );
		}

		ScopeLock GetScopeLock ()
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
	// Single Read, Multiple Write
	//

	struct ReadWriteSync
	{
		friend struct ConditionVariable;

	// types
	public:
		typedef ReadWriteSync	Self;


	// variables
	private:
		pthread_rwlock_t	_rwlock;
		bool				_inited;


	// methods
	private:
		bool _Create ()
		{
			_Delete();
			_inited = pthread_rwlock_init( &_rwlock, null ) == 0;
			return IsValid();
		}

		void _Delete ()
		{
			if ( IsValid() )
			{
				pthread_rwlock_destroy( &_rwlock );
				_inited = false;
			}
		}

	public:
		ReadWriteSync (): _inited(false)
		{
			_Create();
		}

		~ReadWriteSync ()
		{
			_Delete();
		}

		bool IsValid () const
		{
			return _inited;
		}

		void LockWrite ()
		{
			ASSERT( IsValid() );
			bool	res = pthread_rwlock_wrlock( &_rwlock ) == 0;
			ASSERT( res );
		}

		bool TryLockWrite ()
		{
			ASSERT( IsValid() );
			return pthread_rwlock_trywrlock( &_rwlock ) == 0;
		}

		void UnlockWrite ()
		{
			ASSERT( IsValid() );
			bool	res = pthread_rwlock_unlock( &_rwlock ) == 0;
			ASSERT( res );
		}

		void LockRead ()
		{
			ASSERT( IsValid() );
			bool	res = pthread_rwlock_rdlock( &_rwlock ) == 0;
			ASSERT( res );
		}

		bool TryLockRead ()
		{
			ASSERT( IsValid() );
			return pthread_rwlock_tryrdlock( &_rwlock ) == 0;
		}

		void UnlockRead ()
		{
			ASSERT( IsValid() );
			bool	res = pthread_rwlock_unlock( &_rwlock ) == 0;
			ASSERT( res );
		}

		ScopeLock GetScopeWriteLock ()
		{
			struct Util {
				static void Lock (void *p)		{ ((Self *)p)->LockWrite(); }
				static void Unlock (void *p)	{ ((Self *)p)->UnlockWrite(); }
			};
			return ScopeLock( this, &Util::Lock, &Util::Unlock, false );
		}

		ScopeLock GetScopeReadLock ()
		{
			struct Util {
				static void Lock (void *p)		{ ((Self *)p)->LockRead(); }
				static void Unlock (void *p)	{ ((Self *)p)->UnlockRead(); }
			};
			return ScopeLock( this, &Util::Lock, &Util::Unlock, false );
		}
	};



	//
	// Condition Variable
	//

	struct ConditionVariable
	{
	// types
	public:
		typedef	ConditionVariable	Self;


	// variables
	private:
		pthread_cond_t	_cv;
		bool			_inited;
		

	// methods
	private:
		bool _Create ()
		{
			_Delete();
			_inited = pthread_cond_init( &_cv, null ) == 0;
			return IsValid();
		}

		void _Delete ()
		{
			if ( IsValid() )
			{
				pthread_cond_destroy( &_cv );
				_inited = false;
			}
		}

	public:
		ConditionVariable (): _inited(false)
		{
			_Create();
		}

		~ConditionVariable ()
		{
			_Delete();
		}

		bool IsValid () const
		{
			return _inited;
		}

		void Signal ()
		{
			ASSERT( IsValid() );
			bool	res = pthread_cond_signal( &_cv ) == 0;
			ASSERT( res );
		}

		void Broadcast ()
		{
			ASSERT( IsValid() );
			bool	res = pthread_cond_broadcast( &_cv ) == 0;
			ASSERT( res );
		}

		bool Wait (CriticalSection &cs)
		{
			ASSERT( IsValid() );
			return pthread_cond_wait( &_cv, &cs._mutex ) == 0;
		}

		bool Wait (CriticalSection &cs, TimeU time)
		{
			ASSERT( IsValid() );

			timespec currTime;
			clock_gettime( CLOCK_REALTIME, &currTime );

			// TODO: check
			currTime.tv_nsec += time.MilliSeconds() * 1000;
			currTime.tv_sec  += currTime.tv_nsec / 1000000000;

			return pthread_cond_timedwait( &_cv, &cs._mutex, &currTime );
		}
	};



	//
	// Semaphore
	//

	struct Semaphore
	{
	// types
	public:
		typedef Semaphore	Self;


	// variables
	private:
		sem_t	_sem;
		bool	_inited;


	// methods
	private:
		bool _Create (GXTypes::uint initialValue)
		{
			_inited = sem_init( &_sem, 0, initialValue );
			return IsValid();
		}

		void _Destroy ()
		{
			if ( _inited )
			{
				sem_destroy( &_sem );
				_inited = false;
			}
		}

	public:
		explicit
		Semaphore (GXTypes::uint initialValue) : _inited(false)
		{
			_Create( initialValue );
		}

		~Semaphore ()
		{
			_Destroy();
		}

		bool IsValid () const
		{
			return _inited;
		}

		void Lock ()
		{
			int result = sem_wait( &_sem );
			ASSERT( result == 0 );
		}

		bool TryLock ()
		{
			int result = sem_trywait( &_sem );
			return result == 0;
		}

		void Unlock ()
		{
			int result = sem_post( &_sem );
			ASSERT( result == 0 );
		}

		GXTypes::uint GetValue ()
		{
			int value = 0;
			int result = sem_getvalue( &_sem, &value );
			ASSERT( result == 0 );
			return value;
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
	

	struct SyncEventEmulation;
	typedef SyncEventEmulation	SyncEvent;

}	// OS
}	// GX_STL

#include "Engine/STL/OS/Base/SyncEventEmulation.h"

#endif	// PLATFORM_BASE_POSIX_SHELL