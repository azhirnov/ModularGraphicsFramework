// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "OS_SDL.h"
#include "Engine/STL/Math/BinaryMath.h"

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
	CurrentThread::CurrentThread (): _id(INVALID_ID)
	{
		_id = ::SDL_ThreadID();
	}
	
/*
=================================================
	Id
=================================================
*/
	usize CurrentThread::Id () const
	{
		return _id;
	}
	
/*
=================================================
	IsValid
=================================================
*/
	bool CurrentThread::IsValid () const
	{
		return _id != INVALID_ID;
	}
	
/*
=================================================
	IsCurrent
=================================================
*/
	bool CurrentThread::IsCurrent () const
	{
		return GetCurrentThreadId() == _id;
	}
	
/*
=================================================
	SetPriority
=================================================
*/
	bool CurrentThread::SetPriority (EThreadPriority::type priority) const
	{
		ASSERT( IsValid() );
		ASSERT( IsCurrent() );

		if ( IsCurrent() )
		{
			_SetCurrentThreadPriority( priority );
			return true;
		}
		return false;
	}
	
/*
=================================================
	GetCurrentThreadId
=================================================
*/
	usize CurrentThread::GetCurrentThreadId ()
	{
		return ::SDL_ThreadID();
	}
	
/*
=================================================
	Sleep
=================================================
*/
	void CurrentThread::Sleep (TimeL time)
	{
		::SDL_Delay( time.MilliSeconds() );
	}
	
/*
=================================================
	Yield
=================================================
*/
	void CurrentThread::Yield ()
	{
		Sleep( TimeL::FromMilliSeconds( 1 ) );
	}
	
/*
=================================================
	_SetCurrentThreadPriority
=================================================
*/
	void CurrentThread::_SetCurrentThreadPriority (EThreadPriority::type priority)
	{
		::SDL_SetThreadPriority( (SDL_ThreadPriority) priority );
	}
	
//-----------------------------------------------------------------------------

	
	
/*
=================================================
	constructor
=================================================
*/
	Thread::Thread () : _thread(), CurrentThread(INVALID_ID)
	{}
	
/*
=================================================
	_ThreadProcWrapper
=================================================
*/
	int SDLCALL Thread::_ThreadProcWrapper (void *param)
	{
		Thread *	thread = (Thread *)param;
		thread->_proc( thread->_parameter );
		return 0;
	}
	
/*
=================================================
	Create
=================================================
*/
	bool Thread::Create (PThreadProc_t proc, void *param)
	{
		ASSERT( not IsValid() );
			
		_parameter	= param;
		_proc		= proc;
		_thread		= ::SDL_CreateThread( _ThreadProcWrapper, null, this );
		_id			= ::SDL_GetThreadID( _thread );
		return IsValid();
	}
	
/*
=================================================
	Delete
=================================================
*/
	void Thread::Delete ()
	{
		_proc		= null;
		_parameter	= null;
	}
	
/*
=================================================
	IsValid
=================================================
*/
	bool Thread::IsValid () const
	{
		return _thread.IsNotNull();
	}
	
/*
=================================================
	Id
=================================================
*/
	uint Thread::Id () const
	{
		return _id;
	}
	
/*
=================================================
	IsCurrent
=================================================
*/
	bool Thread::IsCurrent () const
	{
		return GetCurrentThreadId() == Id();
	}
		
/*
=================================================
	Terminate
=================================================
*/
	bool Thread::Terminate ()
	{
		// not supported in SDL
		return false;
	}
	
/*
=================================================
	Wait
=================================================
*/
	void Thread::Wait ()
	{
		ASSERT( IsValid() );
		::SDL_WaitThread( _thread, null );
		_thread = null;
		_id		= INVALID_ID;
	}


}	// OS
}	// GX_STL

#endif	// PLATFORM_SDL