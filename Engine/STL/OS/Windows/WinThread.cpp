// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Engine/STL/Common/Platforms.h"
#include "Engine/Config/STL.Config.h"

#if defined( PLATFORM_WINDOWS ) and \
	not defined( PLATFORM_SDL ) and \
	not defined( GX_USE_STD )

#include "Engine/STL/OS/Windows/Thread.h"
#include "Engine/STL/Math/BinaryMath.h"
#include "Engine/STL/Math/Interpolations.h"
#include "Engine/STL/OS/Windows/WinHeader.h"
#include <process.h>

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
		_thread( UninitializedT<HANDLE>() ), _id(0)
	{
		_thread	= ::GetCurrentThread();
		_id		= GetCurrentThreadId();
	}
	
/*
=================================================
	constructor
=================================================
*/
	CurrentThread::CurrentThread (CurrentThread &&other) :
		_thread( RVREF( other._thread ) ),
		_id( other._id )
	{
		other._id = 0;
	}
	
/*
=================================================
	operator =
=================================================
*/
	CurrentThread& CurrentThread::operator = (CurrentThread &&right)
	{
		_thread		= RVREF( right._thread );
		_id			= right._id;
		right._id	= 0;
		return *this;
	}

/*
=================================================
	IsValid
=================================================
*/
	bool CurrentThread::IsValid () const
	{
		return _thread.IsNotNull<HANDLE>();
	}
	
/*
=================================================
	SetPriority
=================================================
*/
	bool CurrentThread::SetPriority (EThreadPriority::type priority) const
	{
		return SetThreadPriority( _thread.Get<HANDLE>(), priority ) != FALSE;
	}
	
/*
=================================================
	GetCurrentThreadId
=================================================
*/
	usize CurrentThread::GetCurrentThreadId ()
	{
		return ::GetCurrentThreadId();
	}
	
/*
=================================================
	Sleep
=================================================
*/
	void CurrentThread::Sleep (TimeL time)
	{
		::Sleep( (uint) time.MilliSeconds() );
	}

//-----------------------------------------------------------------------------
	

	
/*
=================================================
	constructor
=================================================
*/
	Thread::Thread () :
		CurrentThread( Handle_t( UninitializedT<HANDLE>() ), 0 ),
		_proc(null),
		_parameter(null)
	{}
	
/*
=================================================
	constructor
=================================================
*/
	Thread::Thread (PThreadProc_t proc, void *param) :
		CurrentThread( Handle_t( UninitializedT<HANDLE>() ), 0 ),
		_proc(null),
		_parameter(null)
	{
		Create( proc, param );
	}
		
/*
=================================================
	constructor
=================================================
*/
	Thread::Thread (Thread &&other) :
		CurrentThread( RVREF( other ) ),
		_proc( other._proc ),
		_parameter( other._parameter )
	{
		other._proc			= null;
		other._parameter	= null;
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

		right._proc			= null;
		right._parameter	= null;

		return *this;
	}

/*
=================================================
	destructor
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
	uint CALLBACK Thread::_ThreadProcWrapper (void *param)
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
		Delete();

		uint	tmp	= 0;
		_parameter	= param;
		_proc		= proc;
		_thread		= (HANDLE) ::_beginthreadex( null, 0, &_ThreadProcWrapper, this, 0, &tmp );
		_id			= tmp;
		return IsValid();
	}
	
/*
=================================================
	Delete
=================================================
*/
	bool Thread::Delete ()
	{
		bool	ret = true;

		if ( IsValid() and not _Wait(10000) )
		{
			WARNING( "not complete the function ThreadProc" );
			Terminate();
		}

		if ( IsValid() )
		{
			ret		= ::CloseHandle( _thread.Get<HANDLE>() ) != FALSE;
			_thread = null;
			_id		= 0;
		}

		_proc		= null;
		_parameter	= null;

		return ret;
	}
		
/*
=================================================
	Exit
=================================================
*/
	void Thread::Exit (uint exitCode)
	{
		ASSERT( IsCurrent() );
		_id = 0;
		_endthreadex( exitCode == UNKNOWN_EXIT_CODE ? _GetExitCode() : exitCode );
	}
	
/*
=================================================
	Terminate
=================================================
*/
	bool Thread::Terminate ()
	{
		_id = 0;
		ASSERT( IsValid() );
		return IsValid() and ::TerminateThread( _thread.Get<HANDLE>(), _GetExitCode() ) != FALSE;
	}
	
/*
=================================================
	Wait
=================================================
*/
	bool Thread::Wait () const
	{
		return _Wait( INFINITE );
	}
	
/*
=================================================
	_GetExitCode
=================================================
*/
	uint Thread::_GetExitCode () const
	{
		DWORD	dwExitCode = 0;
		ASSERT( IsValid() );
		::GetExitCodeThread( _thread.Get<HANDLE>(), &dwExitCode );
		return (uint)dwExitCode;
	}
	
/*
=================================================
	_Wait
=================================================
*/
	bool Thread::_Wait (uint time) const
	{
		return not IsValid() or ::WaitForSingleObject( _thread.Get<HANDLE>(), time ) == WAIT_OBJECT_0;
	}

//-----------------------------------------------------------------------------


}	// OS
}	// GX_STL

#endif	// PLATFORM_WINDOWS and not PLATFORM_SDL and not GX_USE_STD
