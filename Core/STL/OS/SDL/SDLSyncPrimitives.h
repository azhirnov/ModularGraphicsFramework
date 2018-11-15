// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Core/STL/Common/Platforms.h"

#ifdef PLATFORM_SDL

#include "Core/STL/OS/Base/ScopeLock.h"
#include "Core/STL/OS/SDL/OS_SDL.h"

namespace GX_STL
{
namespace OS
{


	//
	// Mutex
	//

	struct Mutex final : public Noncopyable
	{
		friend struct ConditionVariable;

	// types
	public:
		using Self	= Mutex;


	// variables
	private:
		Ptr< SDL_mutex >	_mutex;


	// methods
	public:
		Mutex ();
		~Mutex ();

			void Lock ();
		ND_ bool TryLock ();
			void Unlock ();
		
		ND_ ScopeLock GetScopeLock()
		{
			struct Util {
				static void Lock (void *p)		{ Cast<Self *>(p)->Lock(); }
				static void Unlock (void *p)	{ Cast<Self *>(p)->Unlock(); }
			};
			return ScopeLock( this, &Util::Lock, &Util::Unlock, false );
		}

		ND_ operator ScopeLock ()
		{
			return GetScopeLock();
		}
	};



	//
	// Semaphore
	//

	struct Semaphore final : public Noncopyable
	{
	// types
	public:
		using Self	= Semaphore;


	// variables
	private:
		Ptr< SDL_sem >	_sem;


	// methods
	public:
		explicit Semaphore (uint initialValue);
		~Semaphore ();

			void Lock ();
		ND_ bool TryLock ();
			void Unlock ();

		ND_ uint GetValue ()
		{
			return ::SDL_SemValue( _sem );
		}
		
		ND_ ScopeLock GetScopeLock()
		{
			struct Util {
				static void Lock (void *p)		{ Cast<Self *>(p)->Lock(); }
				static void Unlock (void *p)	{ Cast<Self *>(p)->Unlock(); }
			};
			return ScopeLock( this, &Util::Lock, &Util::Unlock, false );
		}

		ND_ operator ScopeLock ()
		{
			return GetScopeLock();
		}
	};



	//
	// Condition Variable
	//

	struct ConditionVariable final : public Noncopyable
	{
	// variables
	private:
		Ptr< SDL_cond >		_cv;


	// methods
	public:
		ConditionVariable ();
		~ConditionVariable ();

		void Signal ();
		void Broadcast ();

		bool Wait (const Mutex &mutex);
		bool Wait (const Mutex &mutex, TimeL time);
	};


	struct SyncEventEmulation;
	struct ReadWriteSyncEmulation;

	using SyncEvent		= SyncEventEmulation;
	using ReadWriteSync	= ReadWriteSyncEmulation;

}	// OS
}	// GX_STL

#include "Core/STL/OS/Base/SyncEventEmulation.h"
#include "Core/STL/OS/Base/ReadWriteSyncEmulation.h"

#endif	// PLATFORM_SDL
