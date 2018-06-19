// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

namespace GX_STL
{
namespace OS
{
	
	//
	// Condition Variable version 1
	//

	struct ConditionVariableEmulation_v1 final : public Noncopyable
	{
	// variables
	private:
		SyncEvent	_signal;
		SyncEvent	_broadcast;


	// methods
	public:
		ConditionVariableEmulation_v1 () :
		  _signal( SyncEvent::AUTO_RESET ),
		  _broadcast( SyncEvent::MANUAL_RESET )
		{
		}

		~ConditionVariableEmulation_v1 ()
		{
		}

		bool IsValid () const
		{
			return _signal.IsValid() and _broadcast.IsValid();
		}

		void Signal ()
		{
			_signal.Signal();
		}

		void Broadcast ()
		{
			_broadcast.Signal();
		}
		
		bool Wait (CriticalSection &cs, TimeL time)
		{
			cs.Unlock();

			StaticArray< SyncEvent*, 2 >	events{ &_signal, &_broadcast };

			bool	res = SyncEvent::WaitEvents( events, false, time ) >= 0;

			cs.Lock();
			return res;
		}
	};

	
	
	//
	// Condition Variable version 2
	//

	struct ConditionVariableEmulation_v2 final : public Noncopyable
	{
	// variables
	private:
		CriticalSection	_cs;
		uint			_waiters;
		SyncEvent		_signal;
		SyncEvent		_broadcast;


	// methods
	public:
		ConditionVariableEmulation_v2 () :
		  _waiters(0),
		  _signal( SyncEvent::AUTO_RESET ),
		  _broadcast( SyncEvent::MANUAL_RESET )
		{
		}

		~ConditionVariableEmulation_v2 ()
		{
		}

		bool IsValid () const
		{
			return _signal.IsValid() and _broadcast.IsValid() and _cs.IsValid();
		}

		void Signal ()
		{
			_cs.Lock();
			bool	have_waiters = _waiters > 0;
			_cs.Unlock();

			if ( have_waiters )
				_signal.Signal();
		}

		void Broadcast ()
		{
			_cs.Lock();
			bool	have_waiters = _waiters > 0;
			_cs.Unlock();

			if ( have_waiters )
				_broadcast.Signal();
		}
		
		bool Wait (CriticalSection &cs, TimeL time)
		{
			_cs.Lock();
			++_waiters;
			_cs.Unlock();

			cs.Unlock();

			StaticArray< SyncEvent*, 2 >	events{ &_signal, &_broadcast };

			int	res = SyncEvent::WaitEvents( events, false, time );
			
			_cs.Lock();
			--_waiters;

			bool last_waiter = ( res == 1/*broadcast*/ and _waiters == 0 );
			_cs.Unlock();
			
			if ( last_waiter )
				_broadcast.Reset();

			cs.Lock();
			return res >= 0;
		}
	};


	using ConditionVariableEmulation = ConditionVariableEmulation_v2;


}	// OS
}	// GX_STL
