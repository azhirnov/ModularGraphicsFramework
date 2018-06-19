// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Core/STL/Common/Platforms.h"

#if defined( PLATFORM_BASE_POSIX ) and not defined( PLATFORM_SDL )

#include "Core/STL/OS/Posix/SyncPrimitives.h"
#include "Core/STL/Math/BinaryMath.h"

namespace GX_STL
{
namespace OS
{

/*
=================================================
	constructor
=================================================
*/
	CriticalSection::CriticalSection ()
	{
		static const pthread_mutex_t	tmp = PTHREAD_MUTEX_INITIALIZER;

		_mutex = tmp;

		_Create();
	}
	
/*
=================================================
	destructor
=================================================
*/
	CriticalSection::~CriticalSection ()
	{
		_Delete();
	}
	
/*
=================================================
	_Create
=================================================
*/
	bool CriticalSection::_Create ()
	{
		_Delete();
		_inited = ::pthread_mutex_init( OUT &_mutex, null ) == 0;
		return IsValid();
	}
	
/*
=================================================
	_Delete
=================================================
*/
	bool CriticalSection::_Delete ()
	{
		bool	ret = true;

		if ( IsValid() )
		{
			static const pthread_mutex_t	tmp = PTHREAD_MUTEX_INITIALIZER;

			ret = ::pthread_mutex_destroy( &_mutex ) == 0;
			_mutex  = tmp;
			_inited = false;
		}
		return ret;
	}
	
/*
=================================================
	Lock
=================================================
*/
	void CriticalSection::Lock ()
	{
		ASSERT( IsValid() );
		bool	res = ::pthread_mutex_lock( &_mutex ) == 0;
		ASSERT( res );
	}
	
/*
=================================================
	TryLock
=================================================
*/
	bool CriticalSection::TryLock ()
	{
		ASSERT( IsValid() );
		return ::pthread_mutex_trylock( &_mutex ) == 0;
	}
	
/*
=================================================
	Unlock
=================================================
*/
	void CriticalSection::Unlock ()
	{
		ASSERT( IsValid() );
		bool	res = ::pthread_mutex_unlock( &_mutex ) == 0;
		ASSERT( res );
	}
//-----------------------------------------------------------------------------


		
/*
=================================================
	constructor
=================================================
*/
	ReadWriteSync::ReadWriteSync (): _inited(false)
	{
		_Create();
	}
	
/*
=================================================
	destructor
=================================================
*/
	ReadWriteSync::~ReadWriteSync ()
	{
		_Delete();
	}
	
/*
=================================================
	_Create
=================================================
*/
	bool ReadWriteSync::_Create ()
	{
		_Delete();
		_inited = ::pthread_rwlock_init( OUT &_rwlock, null ) == 0;
		return IsValid();
	}
	
/*
=================================================
	_Delete
=================================================
*/
	void ReadWriteSync::_Delete ()
	{
		if ( IsValid() )
		{
			::pthread_rwlock_destroy( &_rwlock );
			_inited = false;
		}
	}
	
/*
=================================================
	LockWrite
=================================================
*/
	void ReadWriteSync::LockWrite ()
	{
		ASSERT( IsValid() );
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
		ASSERT( IsValid() );
		return ::pthread_rwlock_trywrlock( &_rwlock ) == 0;
	}
	
/*
=================================================
	UnlockWrite
=================================================
*/
	void ReadWriteSync::UnlockWrite ()
	{
		ASSERT( IsValid() );
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
		ASSERT( IsValid() );
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
		ASSERT( IsValid() );
		return ::pthread_rwlock_tryrdlock( &_rwlock ) == 0;
	}
	
/*
=================================================
	UnlockRead
=================================================
*/
	void ReadWriteSync::UnlockRead ()
	{
		ASSERT( IsValid() );
		bool	res = ::pthread_rwlock_unlock( &_rwlock ) == 0;
		ASSERT( res );
	}
//-----------------------------------------------------------------------------


	
/*
=================================================
	constructor
=================================================
*/
	ConditionVariable::ConditionVariable (): _inited(false)
	{
		_Create();
	}
	
/*
=================================================
	destructor
=================================================
*/
	ConditionVariable::~ConditionVariable ()
	{
		_Delete();
	}
	
/*
=================================================
	_Create
=================================================
*/
	bool ConditionVariable::_Create ()
	{
		_Delete();
		_inited = ::pthread_cond_init( OUT &_cv, null ) == 0;
		return IsValid();
	}
	
/*
=================================================
	_Delete
=================================================
*/
	void ConditionVariable::_Delete ()
	{
		if ( IsValid() )
		{
			::pthread_cond_destroy( &_cv );
			_inited = false;
		}
	}
	
/*
=================================================
	Signal
=================================================
*/
	void ConditionVariable::Signal ()
	{
		ASSERT( IsValid() );
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
		ASSERT( IsValid() );
		bool	res = ::pthread_cond_broadcast( &_cv ) == 0;
		ASSERT( res );
	}
	
/*
=================================================
	Wait
=================================================
*/
	bool ConditionVariable::Wait (CriticalSection &cs)
	{
		ASSERT( IsValid() );
		return ::pthread_cond_wait( &_cv, &cs._mutex ) == 0;
	}
	
/*
=================================================
	Wait
=================================================
*/
	bool ConditionVariable::Wait (CriticalSection &cs, TimeL time)
	{
		ASSERT( IsValid() );

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
	Semaphore::Semaphore (GXTypes::uint initialValue) : _inited(false)
	{
		_Create( initialValue );
	}
	
/*
=================================================
	destructor
=================================================
*/
	Semaphore::~Semaphore ()
	{
		_Destroy();
	}
	
/*
=================================================
	_Create
=================================================
*/
	bool Semaphore::_Create (GXTypes::uint initialValue)
	{
		_inited = ::sem_init( OUT &_sem, 0, initialValue );
		return IsValid();
	}
	
/*
=================================================
	_Destroy
=================================================
*/
	void Semaphore::_Destroy ()
	{
		if ( _inited )
		{
			::sem_destroy( &_sem );
			_inited = false;
		}
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

#endif	// PLATFORM_BASE_POSIX
