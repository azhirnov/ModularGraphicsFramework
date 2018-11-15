// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

namespace GX_STL
{
namespace OS
{
	
	//
	// Read Write Sync
	//
	
	struct ReadWriteSyncEmulation final : public Noncopyable
	{
	// types
	public:
		typedef ReadWriteSyncEmulation	Self;


	// variables
	private:
		Mutex		_sCSWrite,
							_sCSRead;
		SyncEvent			_sELocked;
		int					_iCounter;


	// methods
	public:
		ReadWriteSyncEmulation ():
			_sELocked( SyncEvent::INIT_STATE_SIGNALED ), _iCounter(0)
		{}

		~ReadWriteSyncEmulation ()
		{}

		bool IsValid () const
		{
			return true;
		}
		
		void LockWrite ()
		{
			_sELocked.Wait( TimeL(ulong(UMax)) );
			_sCSWrite.Lock();
		}

		bool TryLockWrite ()
		{
			return ( _sELocked.Wait( TimeL::FromMilliSeconds(10) ) and _sCSWrite.TryLock() );
		}

		void UnlockWrite ()
		{
			_sCSWrite.Unlock();
		}

		void LockRead ()
		{
			_sCSWrite.Lock();
			_sCSRead.Lock();

			if ( ++_iCounter == 1 )
				_sELocked.Reset();

			_sCSWrite.Unlock();
			_sCSRead.Unlock();
		}

		bool TryLockRead ()
		{
			if ( not _sCSWrite.TryLock() )
				return false;
			else
				if ( not _sCSRead.TryLock() ) {
					_sCSWrite.Unlock();
					return false;
				}

			if ( ++_iCounter == 1 )
				_sELocked.Reset();

			_sCSWrite.Unlock();
			_sCSRead.Unlock();
			return true;
		}

		void UnlockRead ()
		{
			_sCSRead.Lock();

			if ( --_iCounter == 0 )
				_sELocked.Signal();

			_sCSRead.Unlock();
		}

		ScopeLock GetScopeWriteLock ()
		{
			struct Util {
				static void Lock (void *p)		{ Cast<Self *>(p)->LockWrite(); }
				static void Unlock (void *p)	{ Cast<Self *>(p)->UnlockWrite(); }
			};
			return ScopeLock( this, &Util::Lock, &Util::Unlock, false );
		}

		ScopeLock GetScopeReadLock ()
		{
			struct Util {
				static void Lock (void *p)		{ Cast<Self *>(p)->LockRead(); }
				static void Unlock (void *p)	{ Cast<Self *>(p)->UnlockRead(); }
			};
			return ScopeLock( this, &Util::Lock, &Util::Unlock, false );
		}
	};


}	// OS
}	// GX_STL
