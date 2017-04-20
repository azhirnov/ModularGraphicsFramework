// Copyright © 2014-2017  Zhirnov Andrey. All rights reserved.

#pragma once

namespace GX_STL
{
namespace OS
{


	//
	// Synchronization Event
	//

	struct SyncEventEmulation
	{
	//types
	public:
		typedef SyncEventEmulation		Self;
		
		enum EFlags {
			MANUAL_RESET		= 0,
			AUTO_RESET			= 0x1,
			INIT_STATE_SIGNALED = 0x2,
		};

		enum {
			MAX_WAIT_TIME	= 0xFFFFFFFF
		};


	// variables
	private:
		ConditionVariable	_cv;
		CriticalSection		_cs;
		bool				_autoReset : 1;
		bool				_triggered : 1;


	// methods
	private:
		bool _Create (EFlags flags)
		{
			_autoReset = GXTypes::EnumEq( flags, AUTO_RESET );
			_triggered = GXTypes::EnumEq( flags, INIT_STATE_SIGNALED );

			return IsValid();
		}

	public:
		explicit
		SyncEventEmulation (EFlags flags = AUTO_RESET) : _autoReset(false), _triggered(false)
		{
			_Create( flags );
		}

		~SyncEventEmulation ()
		{
		}

		bool IsValid () const
		{
			return _cv.IsValid() and _cs.IsValid();
		}

		void Signal ()
		{
			ASSERT( IsValid() );

			_cs.Lock();
			_triggered = true;
			_autoReset ? _cv.Signal() : _cv.Broadcast();
			_cs.Unlock();
		}

		void Reset ()
		{
			_cs.Lock();
			_triggered = false;
			_cs.Unlock();
		}

		void Pulse ()
		{
			TODO("");
		}

		bool Wait ()
		{
			ASSERT( IsValid() );
			bool res = false;
			
			_cs.Lock();

			while ( not _triggered ) {
				res = _cv.Wait( _cs );
			}

			if ( _autoReset )
				_triggered = false;

			_cs.Unlock();

			return res;
		}

		bool Wait (TimeU time)
		{
			ASSERT( IsValid() );
			bool	res = false;
			
			_cs.Lock();

			//while ( not _triggered ) {
			//	res = _cv.Wait( _cs, timeMilisec );
			//}

			if ( not _triggered )
			{
				res = _cv.Wait( _cs, time.MilliSeconds() );

				if ( not _triggered )
					res = false;
			}
			
			if ( res and _autoReset )
				_triggered = false;

			_cs.Unlock();
			
			return res;
		}

		static int WaitEvents (ArrayCRef<Self *> events, bool waitAll, TimeU time)
		{
			TODO( "WaitEvents" );
			return -1;
		}
		
		/*ScopeLock GetScopeLock ()
		{
			struct Util {
				static void Lock(void *p)		{ ((Self *)p)->Wait(); }
				static void Unlock(void *p)		{ ((Self *)p)->Signal(); }
			};
			return ScopeLock( this, &Util::Lock, &Util::Unlock, false );
		}

		operator ScopeLock ()
		{
			return GetScopeLock();
		}*/
	};

}	// OS
}	// GX_STL
