// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Engine/STL/Common/Platforms.h"

#ifdef PLATFORM_SDL

#include "Engine/STL/OS/SDL/Thread.h"
#include "Engine/STL/Math/BinaryMath.h"

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
	
	CurrentThread::CurrentThread (CurrentThread &&other) : _id(other._id)
	{
		other._id = INVALID_ID;
	}

	CurrentThread& CurrentThread::operator = (CurrentThread &&right)
	{
		_id = right._id;
		right._id = INVALID_ID;
		return *this;
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
		::SDL_Delay( uint(time.MilliSeconds()) );
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
		::SDL_SetThreadPriority( SDL_ThreadPriority(priority) );
	}
	
//-----------------------------------------------------------------------------

	
	
/*
=================================================
	constructor
=================================================
*/
	Thread::Thread () :
		CurrentThread(INVALID_ID), _proc(null), _parameter(null)
	{}
	
	Thread::Thread (Thread &&other) :
		CurrentThread(RVREF(other)), _thread(other._thread),
		_proc(other._proc), _parameter(other._parameter)
	{
		other._thread = null;
		other._proc = null;
		other._parameter = null;
	}
	
/*
=================================================
	operator =
=================================================
*/
	Thread& Thread::operator = (Thread &&right)
	{
		CurrentThread::operator= ( RVREF(right) );
		
		_proc				= right._proc;
		_parameter			= right._parameter;
		_thread				= right._thread;

		right._proc			= null;
		right._parameter	= null;
		right._thread		= null;

		return *this;
	}

/*
=================================================
	_ThreadProcWrapper
=================================================
*/
	int SDLCALL Thread::_ThreadProcWrapper (void *param)
	{
		Thread *	thread = Cast<Thread *>(param);
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
	usize Thread::Id () const
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
