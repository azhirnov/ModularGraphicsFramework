// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Thread.h"

#if defined( GX_USE_STD ) and \
	not defined( PLATFORM_SDL )

namespace GX_STL
{
namespace OS
{
	
/*
=================================================
	CastThreadId
=================================================
*/
	static inline usize CastThreadId (std::thread::id id)
	{
		typedef CompileTime::SwitchType< sizeof(std::thread::id) == sizeof(uint), uint, ulong >	type;

		return (usize) ReferenceCast< type >( id );
	}
	
//-----------------------------------------------------------------------------


/*
=================================================
	constructor
=================================================
*/
	CurrentThread::CurrentThread ()
	{
		_id	= std::this_thread::get_id();
	}
	
/*
=================================================
	Id
=================================================
*/
	usize CurrentThread::Id () const
	{
		return CastThreadId( _id );
	}
	
/*
=================================================
	IsCurrent
=================================================
*/
	bool CurrentThread::IsCurrent () const
	{
		return std::this_thread::get_id() == _id;
	}
	
/*
=================================================
	IsValid
=================================================
*/
	bool CurrentThread::IsValid () const
	{
		return _id != std::thread::id();
	}
	
/*
=================================================
	SetPriority
=================================================
*/
	bool CurrentThread::SetPriority (EThreadPriority::type priority) const
	{
		WARNING( "priority for std::thread are not supported!" );
		return false;
	}
	
/*
=================================================
	GetCurrentThreadId
=================================================
*/
	usize CurrentThread::GetCurrentThreadId ()
	{
		return CastThreadId( std::this_thread::get_id() );
	}
	
/*
=================================================
	Sleep
=================================================
*/
	void CurrentThread::Sleep (TimeL time)
	{
		std::this_thread::sleep_for( std::chrono::duration<uint, std::nano>( time.NanoSeconds() ) );
	}
	
/*
=================================================
	Yield
=================================================
*/
	void CurrentThread::Yield ()
	{
		std::this_thread::yield();
	}
	
//-----------------------------------------------------------------------------
	

	
/*
=================================================
	constructor
=================================================
*/
	Thread::Thread () :
		_proc(null),
		_parameter(null)
	{}
	
/*
=================================================
	constructor
=================================================
*/
	Thread::Thread (PThreadProc_t proc, void *param) :
		_proc(null),
		_parameter(null)
	{
		Create( proc, param );
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
	uint Thread::_ThreadProcWrapper (void *param)
	{
		Thread *	thread = (Thread *)param;
		thread->_proc( thread->_parameter );
		return 0;
	}
	
/*
=================================================
	Id
=================================================
*/
	usize Thread::Id () const
	{
		return CastThreadId( _thread.get_id() );
	}
	
/*
=================================================
	IsValid
=================================================
*/
	bool Thread::IsValid () const
	{
		return _thread.joinable();
	}
	
/*
=================================================
	IsCurrent
=================================================
*/
	bool Thread::IsCurrent () const
	{
		return std::this_thread::get_id() == _thread.get_id();
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
		_thread		= std::thread( _ThreadProcWrapper, this );
		return IsValid();
	}
	
/*
=================================================
	Delete
=================================================
*/
	bool Thread::Delete ()
	{
		if ( IsValid() )
		{
			_thread = std::thread();
		}

		_proc		= null;
		_parameter	= null;

		return true;
	}
	
/*
=================================================
	Terminate
=================================================
*/
	bool Thread::Terminate ()
	{
		return Delete();
	}
	
/*
=================================================
	Wait
=================================================
*/
	bool Thread::Wait ()
	{
		ASSERT( IsValid() );
		_thread.join();
		return true;
	}

//-----------------------------------------------------------------------------

}	// OS
}	// GX_STL

#endif	// GX_USE_STD
