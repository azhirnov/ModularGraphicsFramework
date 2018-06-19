// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Core/STL/OS/SDL/SyncPrimitives.h"

#ifdef PLATFORM_SDL

#include "Core/STL/Math/BinaryMath.h"

namespace GX_STL
{
namespace OS
{

/*
=================================================
	_Create
=================================================
*/
	bool CriticalSection::_Create ()
	{
		_Delete();
		_mutex = ::SDL_CreateMutex();
		return IsValid();
	}
	
/*
=================================================
	_Delete
=================================================
*/
	void CriticalSection::_Delete ()
	{
		if ( _mutex )
		{
			::SDL_DestroyMutex( _mutex );
			_mutex = null;
		}
	}
	
/*
=================================================
	Lock
=================================================
*/
	void CriticalSection::Lock ()
	{
		int result = ::SDL_LockMutex( _mutex );
		ASSERT( result != -1 );
		GX_UNUSED( result );
	}
	
/*
=================================================
	TryLock
=================================================
*/
	bool CriticalSection::TryLock ()
	{
		int result = ::SDL_TryLockMutex( _mutex );
		ASSERT( result != -1 );
		return result == 0;
	}
	
/*
=================================================
	Unlock
=================================================
*/
	void CriticalSection::Unlock ()
	{
		int result = ::SDL_UnlockMutex( _mutex );
		ASSERT( result != -1 );
		GX_UNUSED( result );
	}
//-----------------------------------------------------------------------------
	


/*
=================================================
	_Create
=================================================
*/
	bool Semaphore::_Create (uint initialValue)
	{
		_Delete();
		_sem = ::SDL_CreateSemaphore( initialValue );
		return IsValid();
	}
	
/*
=================================================
	_Delete
=================================================
*/
	void Semaphore::_Delete ()
	{
		if ( _sem )
		{
			::SDL_DestroySemaphore( _sem );
			_sem = null;
		}
	}
	
/*
=================================================
	Lock
=================================================
*/
	void Semaphore::Lock ()
	{
		int result = ::SDL_SemWait( _sem );
		ASSERT( result != -1 );
		GX_UNUSED( result );
	}
	
/*
=================================================
	TryLock
=================================================
*/
	bool Semaphore::TryLock ()
	{
		int result = ::SDL_SemTryWait( _sem );
		ASSERT( result != -1 );
		return result == 0;
	}
	
/*
=================================================
	Unlock
=================================================
*/
	void Semaphore::Unlock ()
	{
		int result = ::SDL_SemPost( _sem );
		ASSERT( result != -1 );
		GX_UNUSED( result );
	}
//-----------------------------------------------------------------------------
	


/*
=================================================
	_Create
=================================================
*/
	bool ConditionVariable::_Create ()
	{
		_Delete();
		_cv = ::SDL_CreateCond();
		return IsValid();
	}
	
/*
=================================================
	_Delete
=================================================
*/
	void ConditionVariable::_Delete ()
	{
		if ( _cv )
		{
			::SDL_DestroyCond( _cv );
			_cv = null;
		}
	}
	
/*
=================================================
	Signal
=================================================
*/
	void ConditionVariable::Signal ()
	{
		int result = ::SDL_CondSignal( _cv );
		ASSERT( result != -1 );
		GX_UNUSED( result );
	}
	
/*
=================================================
	Broadcast
=================================================
*/
	void ConditionVariable::Broadcast ()
	{
		int result = ::SDL_CondBroadcast( _cv );
		ASSERT( result != -1 );
		GX_UNUSED( result );
	}
	
/*
=================================================
	Wait
=================================================
*/
	bool ConditionVariable::Wait (const CriticalSection &mutex)
	{
		int result = ::SDL_CondWait( _cv, mutex._mutex );
		ASSERT( result != -1 );
		return result != -1;
	}
	
/*
=================================================
	Wait
=================================================
*/
	bool ConditionVariable::Wait (const CriticalSection &mutex, TimeL time)
	{
		int result = ::SDL_CondWaitTimeout( _cv, mutex._mutex, uint(time.MilliSeconds()) );
		ASSERT( result != -1 );
		return result != -1;
	}
//-----------------------------------------------------------------------------

}	// OS
}	// GX_STL

#endif	// PLATFORM_SDL
