// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Core/STL/Common/Platforms.h"

#ifdef PLATFORM_SDL

#include "Core/STL/OS/SDL/OS_SDL.h"

namespace GX_STL
{
namespace OS
{

	struct EThreadPriority
	{
		enum type
		{
			Low				= SDL_THREAD_PRIORITY_LOW,
			BelowNormal		= SDL_THREAD_PRIORITY_LOW,
			Normal			= SDL_THREAD_PRIORITY_NORMAL,
			AboveNormal		= SDL_THREAD_PRIORITY_HIGH,
			High			= SDL_THREAD_PRIORITY_HIGH
		};
	};



	//
	// Current Thread
	//

	struct CurrentThread
	{
	// types
	protected:
		static const usize	INVALID_ID = UMax;


	// variables
	protected:
		usize	_id;


	// methods
	protected:
		CurrentThread (CurrentThread &&other);
		CurrentThread& operator = (CurrentThread &&right);

		explicit CurrentThread (usize id) : _id(id)
		{}

	public:
		CurrentThread ();

		ND_ usize Id () const;
		ND_ bool  IsValid () const;
		ND_ bool  IsCurrent () const;

		bool SetPriority (EThreadPriority::type priority) const;

		ND_ static usize GetCurrentThreadId ();

		static void SetCurrentThreadName (StringCRef name);

		static void Sleep (TimeL time);
		static void Yield ();

	private:
		static void _SetCurrentThreadPriority (EThreadPriority::type priority);
	};



	//
	// Thread
	//

	struct Thread final : CurrentThread
	{
	// types
	public:
		typedef void (*PThreadProc_t) (void *param);


	// variables
	private:
		Ptr< SDL_Thread >	_thread;
		PThreadProc_t		_proc;
		void *				_parameter;


	// methods
	private:
		static int SDLCALL _ThreadProcWrapper (void *param);

	public:
		Thread ();
		Thread (const Thread &) = delete;
		Thread (Thread &&other);
		~Thread ();
		
		Thread& operator = (const Thread &) = delete;
		Thread& operator = (Thread &&right);

		bool Create (PThreadProc_t proc, void *param);
		void Delete ();

		ND_ bool IsValid () const;

		bool Terminate ();
		void Wait ();
	};

}	// OS
}	// GX_STL

#endif	// PLATFORM_SDL
