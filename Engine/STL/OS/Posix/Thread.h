// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/STL/Common/Platforms.h"

#if defined( PLATFORM_BASE_POSIX ) and not defined( PLATFORM_SDL )

#include "Engine/STL/OS/Posix/OSPosix.h"
#include <pthread.h>

namespace GX_STL
{
namespace OS
{

	struct EThreadPriority
	{
		enum type
		{
			Low				= 0,
			BelowNormal		= 0,
			Normal			= 0,
			AboveNormal		= 0,
			High			= 0
		};
	};



	//
	// Current Thread
	//

	struct CurrentThread
	{
	// types
	protected:
		static const usize	INVALID_ID = 0;


	// variables
	protected:
		pthread_t	_thread;


	// methods
	protected:
		explicit
		CurrentThread (pthread_t thread) : _thread(thread)
		{}

	public:
		CurrentThread(): _thread(INVALID_ID)
		{
			_thread = ::pthread_self();
		}

		GXTypes::usize Id() const
		{
			return GXTypes::ReferenceCast< GXTypes::usize >( _thread );
		}

		bool IsValid() const
		{
			return _thread != INVALID_ID;
		}

		bool IsCurrent() const
		{
			return ::pthread_equal( pthread_self(), _thread );
		}

		bool SetPriority (EThreadPriority::type priority) const
		{
			return false;
		}

		static GXTypes::usize GetCurrentThreadId()
		{
			return GXTypes::ReferenceCast< GXTypes::usize >( ::pthread_self() );
		}

		static void Sleep (TimeL time)
		{
			struct timespec ts;

			ts.tv_sec = time.Seconds();
			ts.tv_nsec = (time.NanoSeconds() % 1000) * 1000000;

			::nanosleep( &ts, 0);
		}

		static void Yield ()
		{
			Sleep( TimeL::FromMilliSeconds(1) );
		}

		// TODO: priority
	};
	


	//
	// Thread
	//

	struct Thread : CurrentThread
	{
	// types
	public:
		typedef void (*PThreadProc_t) (void *param);

		enum {
			UNKNOWN_EXIT_CODE	= -1,
		};


	// variables
	private:
		PThreadProc_t	_proc;
		void *			_parameter;


	// methods
	private:
		static void * _ThreadProcWrapper (void *param)
		{
			Thread *	thread = (Thread *)param;
			thread->_proc( thread->_parameter );
			return null;
		}


	public:
		Thread () :
			CurrentThread(INVALID_ID), _proc(null), _parameter(null)
		{}

		explicit
		Thread (PThreadProc_t proc, void *param = null) :
			CurrentThread(INVALID_ID), _proc(null), _parameter(null)
		{
			Create( proc, param );
		}
		
		~Thread ()
		{
			Delete();
		}

		bool Create (PThreadProc_t proc, void *param = null)
		{
			Delete();
			_parameter	= param;
			_proc		= proc;
			return ::pthread_create( &_thread, null, &_ThreadProcWrapper, this ) == 0;
		}

		bool Delete ()
		{
			if ( IsValid() and ::pthread_kill( _thread, 0 ) != 0 )	// ESRCH
			{
				Terminate();
				Wait();
				_thread = INVALID_ID;
			}
			_proc		= null;
			_parameter	= null;
			return true;
		}
		
		// call in ThreadProc only
		void Exit (GXTypes::uint exitCode = UNKNOWN_EXIT_CODE)
		{
			ASSERT( IsCurrent() );
			::pthread_exit( (void *)exitCode );
			_thread = INVALID_ID;
		}

		bool Terminate ()
		{
			ASSERT( IsValid() );
			return IsValid() and ::pthread_kill( _thread, SIGTERM ) == 0;
		}

		bool Wait () const
		{
			ASSERT( IsValid() );
			return IsValid() and ::pthread_join( _thread, null ) == 0;
		}
	};


}	// OS
}	// GX_STL

#endif	// PLATFORM_BASE_POSIX