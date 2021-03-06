// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

namespace GX_STL
{
namespace OS
{


	//
	// Synchronization Event
	//

	struct SyncEventEmulation final : public Noncopyable
	{
	//types
	public:
		typedef SyncEventEmulation		Self;
		
		enum EFlags {
			MANUAL_RESET		= 0,
			AUTO_RESET			= 0x1,
			INIT_STATE_SIGNALED = 0x2,
		};


	// variables
	private:
		ConditionVariable	_cv;
		Mutex				_cs;
		bool				_autoReset : 1;
		bool				_triggered : 1;


	// methods
	public:
		explicit
		SyncEventEmulation (EFlags flags = AUTO_RESET) : _autoReset(false), _triggered(false)
		{
			_autoReset = GXTypes::EnumEq( flags, AUTO_RESET );
			_triggered = GXTypes::EnumEq( flags, INIT_STATE_SIGNALED );
		}

		~SyncEventEmulation ()
		{
		}

		void Signal ()
		{
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

		bool Wait ()
		{
			bool res = true;
			
			_cs.Lock();

			while ( not _triggered ) {
				res = _cv.Wait( _cs );
			}

			if ( _autoReset )
				_triggered = false;

			_cs.Unlock();

			return res;
		}

		bool Wait (TimeL time)
		{
			bool	res = true;
			
			_cs.Lock();

			if ( not _triggered )
			{
				res = _cv.Wait( _cs, time );

				if ( not _triggered )
					res = false;
			}
			
			if ( res and _autoReset )
				_triggered = false;

			_cs.Unlock();
			
			return res;
		}

		static int WaitEvents (ArrayCRef<Self *> events, bool waitAll, TimeL time)
		{
			GX_UNUSED( events, waitAll, time );
			TODO( "WaitEvents" );
			return -1;
		}
	};

}	// OS
}	// GX_STL
