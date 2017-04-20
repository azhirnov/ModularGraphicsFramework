// Copyright © 2014-2017  Zhirnov Andrey. All rights reserved.

#pragma once

#include "Engine/STL/OS/Base/Common.h"

#if defined( GX_USE_STD ) and \
	not defined( PLATFORM_SDL )

#include <thread>

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

	struct CurrentThread
	{
	// varables
	protected:
		std::thread::id		_id;


	// methods
	public:
		CurrentThread ();

		usize Id () const;

		bool IsCurrent () const;

		bool IsValid () const;

		bool SetPriority (EThreadPriority::type priority) const;

		static usize GetCurrentThreadId ();

		static void Sleep (TimeU time);

		static void Yield ();
	};



	//
	// Thread
	//

	struct Thread
	{
	// types
	public:
		typedef void (*PThreadProc_t) (void *param);

		static const uint	MAX_WAIT_TIME		= 0xFFFFFFFF;
		static const uint	UNKNOWN_EXIT_CODE	= -1;


	// variables
	private:
		std::thread		_thread;
		PThreadProc_t	_proc;
		void *			_parameter;


	// methods
	private:
		static uint _ThreadProcWrapper (void *param);

	public:
		Thread ();

		explicit
		Thread (PThreadProc_t proc, void *param = null);
		
		~Thread ();
		
		usize Id () const;
		bool IsValid () const;
		bool IsCurrent () const;
		
		bool SetPriority (EThreadPriority::type priority) const;

		bool Create (PThreadProc_t proc, void *param = null);
		bool Delete ();
		
		// call in ThreadProc only
		//void Exit (uint exitCode = UNKNOWN_EXIT_CODE);

		//!!! not destroy objects in ThreadProc
		bool Terminate ();

		bool Wait ();

		static usize GetCurrentThreadId ()	{ return CurrentThread::GetCurrentThreadId(); }

		static void Sleep (TimeU time)		{ return CurrentThread::Sleep( time ); }

		static void Yield ()				{ return CurrentThread::Yield(); }
	};
	

}	// OS
}	// GX_STL

#endif	// GX_USE_STD
