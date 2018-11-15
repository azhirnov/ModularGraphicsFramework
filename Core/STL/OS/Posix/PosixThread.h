// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Core/STL/Common/Platforms.h"

#if defined( PLATFORM_BASE_POSIX ) and defined( GX_USE_NATIVE_API )

#include "Core/STL/OS/Posix/OSPosix.h"
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
		explicit CurrentThread (pthread_t thread) : _thread{thread} {}

	public:
		CurrentThread ();

		ND_ usize Id () const;
		ND_ bool  IsValid () const	{ return _thread != INVALID_ID; }
		ND_ bool  IsCurrent () const;

		bool SetPriority (EThreadPriority::type priority) const;

		ND_ static usize GetCurrentThreadId ();

		static void SetCurrentThreadName (StringCRef name);

		static void Sleep (TimeL time);
		static void Yield ();
	};
	


	//
	// Thread
	//

	struct Thread : CurrentThread
	{
	// types
	public:
		using PThreadProc_t	= void (*) (void *param);

		static constexpr usize	UNKNOWN_EXIT_CODE	= UMax;


	// variables
	private:
		PThreadProc_t	_proc;
		void *			_parameter;


	// methods
	private:
		static void * _ThreadProcWrapper (void *param);

	public:
		Thread ();
		explicit Thread (PThreadProc_t proc, void *param = null);
		
		~Thread ();

		bool Create (PThreadProc_t proc, void *param = null);
		void Delete ();
		
		// call in ThreadProc only
		void Exit (GXTypes::usize exitCode = UNKNOWN_EXIT_CODE);

		bool Terminate ();
		bool Wait () const;
	};


}	// OS
}	// GX_STL

#endif	// PLATFORM_BASE_POSIX and GX_USE_NATIVE_API
