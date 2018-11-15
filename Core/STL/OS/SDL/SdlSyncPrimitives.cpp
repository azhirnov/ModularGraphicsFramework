// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Core/STL/OS/SDL/SDLSyncPrimitives.h"

#ifdef PLATFORM_SDL

namespace GX_STL
{
namespace OS
{

/*
=================================================
	constructor
=================================================
*/
	Mutex::Mutex () :
		_mutex{ ::SDL_CreateMutex() }
	{
	}
	
/*
=================================================
	destructor
=================================================
*/
	Mutex::~Mutex ()
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
	void Mutex::Lock ()
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
	bool Mutex::TryLock ()
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
	void Mutex::Unlock ()
	{
		int result = ::SDL_UnlockMutex( _mutex );
		ASSERT( result != -1 );
		GX_UNUSED( result );
	}
//-----------------------------------------------------------------------------
	


/*
=================================================
	constructor
=================================================
*/
	Semaphore::Semaphore (uint initialValue) :
		_sem{ ::SDL_CreateSemaphore( initialValue ) }
	{
	}
	
/*
=================================================
	destructor
=================================================
*/
	Semaphore::~Semaphore ()
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
	constructor
=================================================
*/
	ConditionVariable::ConditionVariable () :
		_cv{ ::SDL_CreateCond() }
	{
	}
	
/*
=================================================
	destructor
=================================================
*/
	ConditionVariable::~ConditionVariable ()
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
	bool ConditionVariable::Wait (const Mutex &mutex)
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
	bool ConditionVariable::Wait (const Mutex &mutex, TimeL time)
	{
		int result = ::SDL_CondWaitTimeout( _cv, mutex._mutex, uint(time.MilliSeconds()) );
		ASSERT( result != -1 );
		return result != -1;
	}
//-----------------------------------------------------------------------------

}	// OS
}	// GX_STL

#endif	// PLATFORM_SDL
