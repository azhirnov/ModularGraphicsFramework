// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Core/STL/Common/Platforms.h"

#if defined( PLATFORM_BASE_POSIX ) and defined( GX_USE_NATIVE_API )

#include "Core/STL/OS/Posix/SyncPrimitives.h"

namespace GX_STL
{
namespace OS
{

/*
=================================================
	constructor
=================================================
*/
	Mutex::Mutex ()
	{
		static const pthread_mutex_t	tmp = PTHREAD_MUTEX_INITIALIZER;

		_mutex = tmp;
		
#	if 1
        ::pthread_mutex_init( OUT &_mutex, null ) == 0;
#	else
        pthread_mutexattr_t attr;
        ::pthread_mutexattr_init( &attr );
        ::pthread_mutexattr_settype( &attr, PTHREAD_MUTEX_RECURSIVE );
        ::pthread_mutex_init( &_mutex, &attr ) == 0;
#	endif
	}
	
/*
=================================================
	destructor
=================================================
*/
	Mutex::~Mutex ()
	{
		::pthread_mutex_destroy( &_mutex );
	}
	
/*
=================================================
	Lock
=================================================
*/
	void Mutex::Lock ()
	{
		bool	res = ::pthread_mutex_lock( &_mutex ) == 0;
		ASSERT( res );
	}
	
/*
=================================================
	TryLock
=================================================
*/
	bool Mutex::TryLock ()
	{
		return ::pthread_mutex_trylock( &_mutex ) == 0;
	}
	
/*
=================================================
	Unlock
=================================================
*/
	void Mutex::Unlock ()
	{
		bool	res = ::pthread_mutex_unlock( &_mutex ) == 0;
		ASSERT( res );
	}
//-----------------------------------------------------------------------------


		
/*
=================================================
	constructor
=================================================
*/
	ReadWriteSync::ReadWriteSync ()
	{
		::pthread_rwlock_init( OUT &_rwlock, null );
	}
	
/*
=================================================
	destructor
=================================================
*/
	ReadWriteSync::~ReadWriteSync ()
	{
		::pthread_rwlock_destroy( &_rwlock );
	}
	
/*
=================================================
	LockWrite
=================================================
*/
	void ReadWriteSync::LockWrite ()
	{
		bool	res = ::pthread_rwlock_wrlock( &_rwlock ) == 0;
		ASSERT( res );
	}
	
/*
=================================================
	TryLockWrite
=================================================
*/
	bool ReadWriteSync::TryLockWrite ()
	{
		return ::pthread_rwlock_trywrlock( &_rwlock ) == 0;
	}
	
/*
=================================================
	UnlockWrite
=================================================
*/
	void ReadWriteSync::UnlockWrite ()
	{
		bool	res = ::pthread_rwlock_unlock( &_rwlock ) == 0;
		ASSERT( res );
	}
	
/*
=================================================
	LockRead
=================================================
*/
	void ReadWriteSync::LockRead ()
	{
		bool	res = ::pthread_rwlock_rdlock( &_rwlock ) == 0;
		ASSERT( res );
	}
	
/*
=================================================
	TryLockRead
=================================================
*/
	bool ReadWriteSync::TryLockRead ()
	{
		return ::pthread_rwlock_tryrdlock( &_rwlock ) == 0;
	}
	
/*
=================================================
	UnlockRead
=================================================
*/
	void ReadWriteSync::UnlockRead ()
	{
		bool	res = ::pthread_rwlock_unlock( &_rwlock ) == 0;
		ASSERT( res );
	}
//-----------------------------------------------------------------------------


	
/*
=================================================
	constructor
=================================================
*/
	ConditionVariable::ConditionVariable ()
	{
		::pthread_cond_init( OUT &_cv, null );
	}
	
/*
=================================================
	destructor
=================================================
*/
	ConditionVariable::~ConditionVariable ()
	{
		::pthread_cond_destroy( &_cv );
	}
	
/*
=================================================
	Signal
=================================================
*/
	void ConditionVariable::Signal ()
	{
		bool	res = ::pthread_cond_signal( &_cv ) == 0;
		ASSERT( res );
	}
	
/*
=================================================
	Broadcast
=================================================
*/
	void ConditionVariable::Broadcast ()
	{
		bool	res = ::pthread_cond_broadcast( &_cv ) == 0;
		ASSERT( res );
	}
	
/*
=================================================
	Wait
=================================================
*/
	bool ConditionVariable::Wait (Mutex &cs)
	{
		return ::pthread_cond_wait( &_cv, &cs._mutex ) == 0;
	}
	
/*
=================================================
	Wait
=================================================
*/
	bool ConditionVariable::Wait (Mutex &cs, TimeL time)
	{
		struct timespec currTime;
		::clock_gettime( CLOCK_REALTIME, OUT &currTime );

		// TODO: check
		currTime.tv_nsec += time.MilliSeconds() * 1000;
		currTime.tv_sec  += currTime.tv_nsec / 1000000000;

		return ::pthread_cond_timedwait( &_cv, &cs._mutex, &currTime );
	}
//-----------------------------------------------------------------------------


	
/*
=================================================
	constructor
=================================================
*/
	Semaphore::Semaphore (GXTypes::uint initialValue)
	{
		::sem_init( OUT &_sem, 0, initialValue );
	}
	
/*
=================================================
	destructor
=================================================
*/
	Semaphore::~Semaphore ()
	{
		::sem_destroy( &_sem );
	}
	
/*
=================================================
	Lock
=================================================
*/
	void Semaphore::Lock ()
	{
		int result = ::sem_wait( &_sem );
		ASSERT( result == 0 );
	}
	
/*
=================================================
	TryLock
=================================================
*/
	bool Semaphore::TryLock ()
	{
		int result = ::sem_trywait( &_sem );
		return result == 0;
	}
	
/*
=================================================
	Unlock
=================================================
*/
	void Semaphore::Unlock ()
	{
		int result = ::sem_post( &_sem );
		ASSERT( result == 0 );
	}
	
/*
=================================================
	GetValue
=================================================
*/
	GXTypes::uint  Semaphore::GetValue ()
	{
		int value  = 0;
		int result = ::sem_getvalue( &_sem, &value );

		ASSERT( result == 0 );
		return value;
	}


}	// OS
}	// GX_STL

#endif	// PLATFORM_BASE_POSIX and GX_USE_NATIVE_API
