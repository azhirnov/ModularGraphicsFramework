// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Core/STL/Common/Platforms.h"

#if defined( PLATFORM_WINDOWS ) and defined( GX_USE_NATIVE_API )

#include "Core/STL/OS/Base/ScopeLock.h"
#include "Core/STL/OS/Windows/OSWindows.h"

namespace GX_STL
{
namespace OS
{
	
	struct Mutex;
	struct ReadWriteSync;
	struct ConditionVariable;


	//
	// Mutex
	//

	struct Mutex final : public Noncopyable
	{
		friend struct ConditionVariable;

	// types
	public:
		static const uint	_CSsize = sizeof(void*) * 4 + 8;

		using Handle_t	= DeferredType< _CSsize, alignof(void*) >;	// CRITICAL_SECTION
		using Self		= Mutex;


	// variables
	private:
		Handle_t	_crSection;
		DEBUG_ONLY(
			usize	_threadId;
		)

		
	// methods
	public:
		Mutex ();
		~Mutex ();

			void Lock ();
		ND_ bool TryLock ();
			void Unlock ();

		ND_ ScopeLock  GetScopeLock ()
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
		using Self		= ReadWriteSync;
		using Handle_t	= DeferredTypeFrom< void* >;	// SRWLOCK


	// variables
	private:
		Handle_t	_srw;
		

	private:
		static bool _isInitialized;
		static void _InitSRWFuncPointers ();


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

		ND_ ScopeLock  GetScopeWriteLock ()
		{
			struct Util {
				static void Lock (void *p)		{ ((Self *)p)->LockWrite(); }
				static void Unlock (void *p)	{ ((Self *)p)->UnlockWrite(); }
			};
			return ScopeLock( this, &Util::Lock, &Util::Unlock, false );
		}

		ND_ ScopeLock  GetScopeReadLock ()
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
		using Self		= ConditionVariable;
		using Handle_t	= DeferredTypeFrom< void* >;	// CONDITION_VARIABLE


	// variables
	private:
		Handle_t	_cv;
		bool		_inited;


	// methods
	private:
		static bool _isInitialized;
		static void _InitCondVarFuncPointers ();

	public:
		ConditionVariable ();
		~ConditionVariable ();

		void Signal ();
		void Broadcast ();

		bool Wait (Mutex &cs);
		bool Wait (Mutex &cs, TimeL time);
	};



	//
	// Synchronization Event
	//
	
	struct SyncEvent final : public Noncopyable
	{
	// types
	public:
		using Self		= SyncEvent;
		using Handle_t	= DeferredTypeFrom< void* >;	// HANDLE

		enum EFlags {
			MANUAL_RESET		= 0,
			AUTO_RESET			= 0x1,
			INIT_STATE_SIGNALED = 0x2,
		};


	// variables
	private:
		Handle_t	_event;


	// methods
	public:
		explicit SyncEvent (EFlags flags = AUTO_RESET);
		~SyncEvent ();

		bool Signal ();
		bool Reset ();

		bool Wait ();
		bool Wait (TimeL time);

		static int WaitEvents (ArrayRef<Self *> events, bool waitAll, TimeL time);
	};



	//
	// Semaphore
	//

	struct Semaphore final : public Noncopyable
	{
	// types
	public:
		using Self		= Semaphore;
		using Handle_t	= DeferredTypeFrom< void* >;	// HANDLE


	// variables
	private:
		Handle_t	_sem;


	// methods
	public:
		explicit Semaphore (uint initialValue);
		~Semaphore ();

			void Lock ();
		ND_ bool TryLock ();
			void Unlock ();

		ND_ uint GetValue ();
		
		/*ND_ ScopeLock GetScopeLock()
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
		}*/
	};


}	// OS
}	// GX_STL

#endif	// PLATFORM_WINDOWS and GX_USE_NATIVE_API
