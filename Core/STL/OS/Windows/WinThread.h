// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Core/STL/Common/Platforms.h"
#include "Core/Config/STL.Config.h"

#if defined( PLATFORM_WINDOWS ) and defined( GX_USE_NATIVE_API )

#include "Core/STL/OS/Windows/OSWindows.h"

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
	// types
	protected:
		typedef DeferredTypeFrom< void* >	Handle_t;	// HANDLE


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

		ND_ usize Id () const
		{
			return _id;
		}

		ND_ bool IsCurrent () const
		{
			return GetCurrentThreadId() == _id;
		}

		ND_ bool IsValid () const;

		bool SetPriority (EThreadPriority::type priority) const;


		ND_ static usize GetCurrentThreadId ();

		static void SetCurrentThreadName (StringCRef name);

		static void Sleep (TimeL time);

		static void Yield ()
		{
			Sleep( TimeL::FromMilliSeconds(1) );
		}
	};



	//
	// Thread
	//

	struct Thread final : public CurrentThread
	{
	// types
	public:
		typedef void (*PThreadProc_t) (void *param);

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
		void Delete ();
		
		// call in ThreadProc only
		void Exit (uint exitCode = UNKNOWN_EXIT_CODE);

		//!!! not destroy objects in ThreadProc
		bool Terminate ();

		bool Wait () const;

	private:
		uint _GetExitCode () const;

		bool _Wait (uint time) const;
	};
	

}	// OS
}	// GX_STL

#endif	// PLATFORM_WINDOWS and GX_USE_NATIVE_API
