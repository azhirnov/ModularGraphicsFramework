// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/STL/OS/Windows/OSWindows.h"
#include "Engine/STL/OS/SDL/Thread.h"

#if defined( PLATFORM_WINDOWS ) and \
	not defined( PLATFORM_SDL ) and \
	not defined( GX_USE_STD )

namespace GX_STL
{
namespace OS
{

	struct EThreadPriority
	{
		enum type
		{
			Low				= -2,
			BelowNormal		= -1,
			Normal			=  0,
			AboveNormal		=  1,
			High			=  2
		};
	};



	//
	// Current Thread
	//

	struct _STL_EXPORT_ CurrentThread
	{
	// types
	protected:
		typedef HiddenOSTypeFrom< void* >	Handle_t;	// HANDLE


	// varables
	protected:
		Handle_t	_thread;
		usize		_id;


	// methods
	protected:
		CurrentThread (CurrentThread &&other);
		CurrentThread& operator = (CurrentThread &&right);

		explicit CurrentThread (const Handle_t &thread, usize id) : _thread(thread), _id(id)
		{}

	public:
		CurrentThread ();

		usize Id () const
		{
			return _id;
		}

		bool IsCurrent () const
		{
			return GetCurrentThreadId() == _id;
		}

		bool IsValid () const;

		bool SetPriority (EThreadPriority::type priority) const;


		static usize GetCurrentThreadId ();

		static void Sleep (TimeL time);

		static void Yield ()
		{
			Sleep( TimeL::FromMilliSeconds(1) );
		}
	};



	//
	// Thread
	//

	struct _STL_EXPORT_ Thread final : public CurrentThread
	{
	// types
	public:
		typedef void (*PThreadProc_t) (void *param);

		static const uint	MAX_WAIT_TIME		= 0xFFFFFFFF;
		static const uint	UNKNOWN_EXIT_CODE	= UMax;


	// variables
	private:
		PThreadProc_t	_proc;
		void *			_parameter;


	// methods
	private:
		static uint CALLBACK _ThreadProcWrapper (void *param);

	public:
		Thread ();
		Thread (const Thread &) = delete;
		Thread (Thread &&other);
		explicit Thread (PThreadProc_t proc, void *param = null);
		~Thread ();

		Thread& operator = (const Thread &) = delete;
		Thread& operator = (Thread &&right);

		bool Create (PThreadProc_t proc, void *param = null);
		bool Delete ();
		
		// call in ThreadProc only
		void Exit (uint exitCode = UNKNOWN_EXIT_CODE);

		//!!! not destroy objects in ThreadProc
		bool Terminate ();

		bool Wait () const;

	private:
		uint _GetExitCode () const;

		bool _Wait (uint time = MAX_WAIT_TIME) const;
	};
	

}	// OS
}	// GX_STL

#endif	// PLATFORM_WINDOWS