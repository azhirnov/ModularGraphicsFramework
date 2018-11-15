// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Core/STL/Common/Platforms.h"

#if defined( PLATFORM_BASE_POSIX ) and defined( GX_USE_NATIVE_API )

#include "Core/STL/OS/Base/ScopeLock.h"
#include "Core/STL/OS/Posix/OSPosix.h"
#include <pthread.h>
#include <semaphore.h>

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
		pthread_mutex_t		_mutex;


	// methods
	public:
		Mutex ();
		~Mutex ();

			void Lock ();
		ND_ bool TryLock ();
			void Unlock ();

		ND_ ScopeLock GetScopeLock ()
		{
			struct Util {
				static void Lock (void *p)		{ ((Self *)p)->Lock(); }
				static void Unlock (void *p)	{ ((Self *)p)->Unlock(); }
			};
			return ScopeLock( this, &Util::Lock, &Util::Unlock, false );
		}

		ND_ operator ScopeLock ()
		{
			return GetScopeLock();
		}
	};



	//
	// Single Read, Multiple Write
	//

	struct ReadWriteSync final : public Noncopyable
	{
		friend struct ConditionVariable;

	// types
	public:
		using Self	= ReadWriteSync;


	// variables
	private:
		pthread_rwlock_t	_rwlock;


	// methods
	public:
		ReadWriteSync ();
		~ReadWriteSync ();

			void LockWrite ();
		ND_ bool TryLockWrite ();
			void UnlockWrite ();

			void LockRead ();
		ND_ bool TryLockRead ();
			void UnlockRead ();

		ND_ ScopeLock GetScopeWriteLock ()
		{
			struct Util {
				static void Lock (void *p)		{ ((Self *)p)->LockWrite(); }
				static void Unlock (void *p)	{ ((Self *)p)->UnlockWrite(); }
			};
			return ScopeLock( this, &Util::Lock, &Util::Unlock, false );
		}

		ND_ ScopeLock GetScopeReadLock ()
		{
			struct Util {
				static void Lock (void *p)		{ ((Self *)p)->LockRead(); }
				static void Unlock (void *p)	{ ((Self *)p)->UnlockRead(); }
			};
			return ScopeLock( this, &Util::Lock, &Util::Unlock, false );
		}
	};



	//
	// Condition Variable
	//

	struct ConditionVariable final : public Noncopyable
	{
	// types
	public:
		using Self	= ConditionVariable;


	// variables
	private:
		pthread_cond_t	_cv;
		

	// methods
	public:
		ConditionVariable ();
		~ConditionVariable ();

		void Signal ();
		void Broadcast ();

		bool Wait (Mutex &cs);
		bool Wait (Mutex &cs, TimeL time);
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
		sem_t	_sem;
		bool	_inited;


	// methods
	public:
		explicit Semaphore (GXTypes::uint initialValue);
		~Semaphore ();

			void Lock ();
		ND_ bool TryLock ();
			void Unlock ();

		ND_ GXTypes::uint GetValue ();
		
		ND_ ScopeLock GetScopeLock()
		{
			struct Util {
				static void Lock (void *p)		{ ((Self *)p)->Lock(); }
				static void Unlock (void *p)	{ ((Self *)p)->Unlock(); }
			};
			return ScopeLock( this, &Util::Lock, &Util::Unlock, false );
		}

		ND_ operator ScopeLock ()
		{
			return GetScopeLock();
		}
	};
	

	struct SyncEventEmulation;
	using SyncEvent	= SyncEventEmulation;

}	// OS
}	// GX_STL

#include "Core/STL/OS/Base/SyncEventEmulation.h"

#endif	// PLATFORM_BASE_POSIX and GX_USE_NATIVE_API
