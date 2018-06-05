// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/STL/Common/Platforms.h"

#ifdef PLATFORM_SDL

#include "Engine/STL/OS/Base/ScopeLock.h"
#include "Engine/STL/OS/SDL/OS_SDL.h"

namespace GX_STL
{
namespace OS
{


	//
	// Critical Section
	//

	struct CriticalSection final : public Noncopyable
	{
		friend struct ConditionVariable;

	// types
	public:
		using Self	= CriticalSection;


	// variables
	private:
		Ptr< SDL_mutex >	_mutex;


	// methods
	private:
		bool _Create ();
		void _Delete ();

	public:
		CriticalSection ()
		{
			_Create();
		}

		~CriticalSection ()
		{
			_Delete();
		}

		bool IsValid () const
		{
			return _mutex.IsNotNull();
		}

		void Lock ();
		bool TryLock ();
		void Unlock ();
		
		ScopeLock GetScopeLock()
		{
			struct Util {
				static void Lock (void *p)		{ Cast<Self *>(p)->Lock(); }
				static void Unlock (void *p)	{ Cast<Self *>(p)->Unlock(); }
			};

			return ScopeLock( this, &Util::Lock, &Util::Unlock, false );
		}

		operator ScopeLock ()
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
	private:
		bool _Create (uint initialValue);
		void _Delete ();

	public:
		explicit
		Semaphore (uint initialValue)
		{
			_Create( initialValue );
		}

		~Semaphore ()
		{
			_Delete();
		}

		bool IsValid () const
		{
			return _sem.IsNotNull();
		}

		void Lock ();
		bool TryLock ();
		void Unlock ();

		uint GetValue ()
		{
			return ::SDL_SemValue( _sem );
		}
		
		ScopeLock GetScopeLock()
		{
			struct Util {
				static void Lock (void *p)		{ Cast<Self *>(p)->Lock(); }
				static void Unlock (void *p)	{ Cast<Self *>(p)->Unlock(); }
			};

			return ScopeLock( this, &Util::Lock, &Util::Unlock, false );
		}

		operator ScopeLock ()
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
	private:
		bool _Create ();
		void _Delete ();

	public:
		ConditionVariable ()
		{
			_Create();
		}

		~ConditionVariable ()
		{
			_Delete();
		}

		bool IsValid () const
		{
			return _cv.IsNotNull();
		}

		void Signal ();
		void Broadcast ();

		bool Wait (const CriticalSection &mutex);
		bool Wait (const CriticalSection &mutex, TimeL time);
	};


	struct SyncEventEmulation;
	struct ReadWriteSyncEmulation;

	using SyncEvent		= SyncEventEmulation;
	using ReadWriteSync	= ReadWriteSyncEmulation;

}	// OS
}	// GX_STL

#include "Engine/STL/OS/Base/SyncEventEmulation.h"
#include "Engine/STL/OS/Base/ReadWriteSyncEmulation.h"

#endif	// PLATFORM_SDL
