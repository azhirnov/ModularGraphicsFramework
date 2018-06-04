// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/STL/Common/Platforms.h"

#if defined( PLATFORM_BASE_POSIX ) and not defined( PLATFORM_SDL )

#include "Engine/STL/OS/Base/ScopeLock.h"
#include "Engine/STL/OS/Posix/OSPosix.h"
#include <pthread.h>
#include <semaphore.h>

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
		pthread_mutex_t	_mutex;
		bool			_inited;


	// methods
	private:
		bool _Create ();
		bool _Delete ();

	public:
		CriticalSection ();
		~CriticalSection ();

		ND_ bool IsValid () const	{ return _inited; }

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
		bool				_inited;


	// methods
	private:
		bool _Create ();
		void _Delete ();

	public:
		ReadWriteSync ();
		~ReadWriteSync ();

		ND_ bool IsValid () const	{ return _inited; }

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
		bool			_inited;
		

	// methods
	private:
		bool _Create ();
		void _Delete ();

	public:
		ConditionVariable ();
		~ConditionVariable ();

		ND_ bool IsValid () const	{ return _inited; }

		void Signal ();
		void Broadcast ();

		bool Wait (CriticalSection &cs);
		bool Wait (CriticalSection &cs, TimeL time);
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
	private:
		bool _Create (GXTypes::uint initialValue);
		void _Destroy ();

	public:
		explicit Semaphore (GXTypes::uint initialValue);
		~Semaphore ();

		ND_ bool IsValid () const	{ return _inited; }

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

#include "Engine/STL/OS/Base/SyncEventEmulation.h"

#endif	// PLATFORM_BASE_POSIX