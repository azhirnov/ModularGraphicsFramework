// Copyright © 2014-2017  Zhirnov Andrey. All rights reserved.

#pragma once

#include "OS_SDL.h"

#ifdef PLATFORM_SDL

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
		static const usize	INVALID_ID = -1;


	// variables
	protected:
		usize	_id;


	// methods
	protected:
		explicit
		CurrentThread (usize id) : _id(id)
		{}

	public:
		CurrentThread ();

		usize Id () const;

		bool IsValid () const;

		bool IsCurrent () const;

		bool SetPriority (EThreadPriority::type priority) const;

		static usize GetCurrentThreadId ();

		static void Sleep (TimeU time);

		static void Yield ();

	private:
		static void _SetCurrentThreadPriority (EThreadPriority::type priority);
	};



	//
	// Thread
	//

	struct Thread : CurrentThread
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

		bool Create (PThreadProc_t proc, void *param);

		void Delete ();

		bool IsValid () const;

		uint Id () const;

		bool IsCurrent () const;
		
		bool Terminate ();

		void Wait ();
	};

}	// OS
}	// GX_STL

#endif	// PLATFORM_SDL
