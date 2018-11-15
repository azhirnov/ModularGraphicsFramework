// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Core/STL/Common/Platforms.h"
#include "Core/Config/STL.Config.h"

#ifdef GX_USE_STD

#include "Core/STL/Types/Noncopyable.h"
#include "Core/STL/OS/Base/ScopeLock.h"

#include <mutex>
#include <shared_mutex>
#include <condition_variable>

namespace GX_STL
{
namespace OS
{
	using namespace GX_STL::GXTypes;


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
		std::mutex		_mutex;

		
	// methods
	public:
		Mutex () {}

			void Lock ()					{ _mutex.lock(); }
		ND_ bool TryLock ()					{ return _mutex.try_lock(); }
			void Unlock ()					{ _mutex.unlock(); }

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
	// types
	public:
		using Self	= ReadWriteSync;


	// variables
	private:
		std::shared_mutex	_srw;
		

	// methods
	public:
		ReadWriteSync () {}

			void LockWrite ()					{ _srw.lock(); }
		ND_ bool TryLockWrite ()				{ return _srw.try_lock(); }
			void UnlockWrite ()					{ _srw.unlock(); }

			void LockRead ()					{ _srw.lock_shared(); }
		ND_ bool TryLockRead ()					{ return _srw.try_lock_shared(); }
			void UnlockRead ()					{ _srw.unlock_shared(); }

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
		using Self	= ConditionVariable;


	// variables
	private:
		std::condition_variable		_cv;


	// methods
	public:
		ConditionVariable () {}

		void Signal ()			{ _cv.notify_one(); }
		void Broadcast ()		{ _cv.notify_all(); }

		bool Wait (Mutex &cs)
		{
			std::unique_lock	lock{ cs._mutex };
			_cv.wait( lock );
			return true;
		}

		bool Wait (Mutex &cs, TimeL time)
		{
			std::unique_lock	lock{ cs._mutex };
			return _cv.wait_for( lock, std::chrono::nanoseconds(time.NanoSeconds()) ) == std::cv_status::no_timeout;
		}
	};
	

}	// OS
}	// GX_STL

#include "Core/STL/OS/Base/SyncEventEmulation.h"

#endif	// GX_USE_STD
