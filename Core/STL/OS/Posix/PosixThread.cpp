// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Core/STL/OS/Posix/PosixThread.h"

#if defined( PLATFORM_BASE_POSIX ) and defined( GX_USE_NATIVE_API )

#include <signal.h>

namespace GX_STL
{
namespace OS
{

/*
=================================================
	constructor
=================================================
*/
	CurrentThread::CurrentThread () :
		_thread{ ::pthread_self() }
	{}
	
/*
=================================================
	Id
=================================================
*/
	usize CurrentThread::Id () const
	{
		return GXTypes::ReferenceCast< GXTypes::usize >( _thread );
	}
	
/*
=================================================
	IsCurrent
=================================================
*/
	bool CurrentThread::IsCurrent () const
	{
		return ::pthread_equal( pthread_self(), _thread );
	}
	
/*
=================================================
	SetPriority
=================================================
*/
	bool CurrentThread::SetPriority (EThreadPriority::type priority) const
	{
		// TODO
		return false;
	}
	
/*
=================================================
	GetCurrentThreadId
=================================================
*/
	usize CurrentThread::GetCurrentThreadId ()
	{
		return GXTypes::ReferenceCast< GXTypes::usize >( ::pthread_self() );
	}
	
/*
=================================================
	SetCurrentThreadName
=================================================
*/
	void CurrentThread::SetCurrentThreadName (StringCRef name)
	{
		::pthread_setname_np( ::pthread_self(), name.cstr() );
	}
	
/*
=================================================
	Sleep
=================================================
*/
	void CurrentThread::Sleep (TimeL time)
	{
		struct timespec ts;

		ts.tv_sec = time.Seconds();
		ts.tv_nsec = (time.NanoSeconds() % 1000) * 1000000;

		::nanosleep( &ts, 0);
	}
	
/*
=================================================
	Yield
=================================================
*/
	void CurrentThread::Yield ()
	{
		Sleep( TimeL::FromMilliSeconds(1) );
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
	
/*
=================================================
	constructor
=================================================
*/
	Thread::Thread (PThreadProc_t proc, void *param = null) :
		CurrentThread(INVALID_ID), _proc(null), _parameter(null)
	{
		Create( proc, param );
	}
	
/*
=================================================
	destrictor
=================================================
*/	
	Thread::~Thread ()
	{
		Delete();
	}
	
/*
=================================================
	_ThreadProcWrapper
=================================================
*/
	void * Thread::_ThreadProcWrapper (void *param)
	{
		Thread *	thread = (Thread *)param;
		thread->_proc( thread->_parameter );
		return null;
	}
	
/*
=================================================
	Create
=================================================
*/
	bool Thread::Create (PThreadProc_t proc, void *param)
	{
		Delete();
		_parameter	= param;
		_proc		= proc;
		return ::pthread_create( &_thread, null, &_ThreadProcWrapper, this ) == 0;
	}
	
/*
=================================================
	Delete
=================================================
*/
	void Thread::Delete ()
	{
		if ( IsValid() and ::pthread_kill( _thread, 0 ) != 0 )	// ESRCH
		{
			Terminate();
			Wait();
		}

		_thread		= INVALID_ID;
		_proc		= null;
		_parameter	= null;
	}
	
/*
=================================================
	Exit
=================================================
*/
	void Thread::Exit (GXTypes::usize exitCode)
	{
		ASSERT( IsCurrent() );
		::pthread_exit( (void *)exitCode );
		_thread = INVALID_ID;
	}
	
/*
=================================================
	Terminate
=================================================
*/
	bool Thread::Terminate ()
	{
		ASSERT( IsValid() );
		return IsValid() and ::pthread_kill( _thread, SIGTERM ) == 0;
	}
	
/*
=================================================
	Wait
=================================================
*/
	bool Thread::Wait () const
	{
		return not IsValid() or ::pthread_join( _thread, null ) == 0;
	}

}	// OS
}	// GX_STL

#endif	// PLATFORM_BASE_POSIX and GX_USE_NATIVE_API
