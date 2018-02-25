// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Engine/STL/OS/Windows/SyncPrimitives.h"
#include "Engine/STL/Algorithms/ArrayUtils.h"
#include "Engine/STL/Math/BinaryMath.h"

#if defined( PLATFORM_WINDOWS ) and not defined( PLATFORM_SDL )

#include "Engine/STL/OS/Windows/Library.h"
#include "Engine/STL/OS/Base/ReadWriteSyncEmulation.h"
#include "Engine/STL/OS/Base/ConditionVariableEmulation.h"

#include "Engine/STL/OS/Windows/WinHeader.h"

namespace GX_STL
{
namespace OS
{
	
/*
=================================================
	constructor
=================================================
*/
	CriticalSection::CriticalSection () :
		_crSection( UninitializedT<CRITICAL_SECTION>() ),
		_inited(false)
	{
		::InitializeCriticalSection( &_crSection.Get<CRITICAL_SECTION>() );
		_inited = true;
	}
	
/*
=================================================
	destructor
=================================================
*/
	CriticalSection::~CriticalSection ()
	{
		_Delete();
	}
	
/*
=================================================
	_Delete
=================================================
*/
	void CriticalSection::_Delete ()
	{
		if ( _inited )
		{
			::DeleteCriticalSection( &_crSection.Get<CRITICAL_SECTION>() );

			_inited = false;
		}
	}
	
/*
=================================================
	Lock
=================================================
*/
	void CriticalSection::Lock ()
	{
		ASSERT( IsValid() );
		::EnterCriticalSection( &_crSection.Get<CRITICAL_SECTION>() );
	}
	
/*
=================================================
	TryLock
=================================================
*/
	bool CriticalSection::TryLock ()
	{
		ASSERT( IsValid() );
		return ::TryEnterCriticalSection( &_crSection.Get<CRITICAL_SECTION>() ) != 0;
	}
	
/*
=================================================
	Unlock
=================================================
*/
	void CriticalSection::Unlock ()
	{
		ASSERT( IsValid() );
		::LeaveCriticalSection( &_crSection.Get<CRITICAL_SECTION>() );
	}

//-----------------------------------------------------------------------------
	


	typedef VOID	(CALLBACK *PInitializeSRWLockProc_t)		 (OUT   PSRWLOCK SRWLock);
	typedef VOID	(CALLBACK *PAcquireSRWLockExclusiveProc_t)	 (INOUT PSRWLOCK SRWLock);
	typedef BOOLEAN	(CALLBACK *PTryAcquireSRWLockExclusiveProc_t)(INOUT PSRWLOCK SRWLock);
	typedef VOID	(CALLBACK *PReleaseSRWLockExclusiveProc_t)	 (INOUT PSRWLOCK SRWLock);
	typedef VOID	(CALLBACK *PAcquireSRWLockSharedProc_t)		 (INOUT PSRWLOCK SRWLock);
	typedef BOOLEAN	(CALLBACK *PTryAcquireSRWLockSharedProc_t)	 (INOUT PSRWLOCK SRWLock);
	typedef VOID	(CALLBACK *PReleaseSRWLockSharedProc_t)		 (INOUT PSRWLOCK SRWLock);
	typedef	VOID	(CALLBACK *PDeleteSRWLockProc_t)			 (INOUT PSRWLOCK SRWLock);



	template <typename T, typename B>
	inline T *& HandleToPointerRef (B &h)
	{
		return reinterpret_cast< T *&>( h->Ptr );
	}

	template <typename T, typename B>
	inline T * HandleToPointer (const B& h)
	{
		return PointerCast< T >( h->Ptr );
	}

	template <typename T>
	inline bool SetPointer (T &to, Library::Func_t from)
	{
		if ( from != null ) {
			to = ReferenceCast< T >( from );
			return true;
		}
		return false;
	}

	typedef ReadWriteSyncEmulation		TDefReadWriteSync;
	typedef ConditionVariableEmulation	TDefConditionVariable;


	VOID CALLBACK DefInitializeSRWLock (/*out*/ PSRWLOCK SRWLock)
	{
		HandleToPointerRef< TDefReadWriteSync >( SRWLock ) = new TDefReadWriteSync;
	}


	VOID CALLBACK DefAcquireSRWLockExclusive (/*inout*/ PSRWLOCK SRWLock)
	{
		HandleToPointer< TDefReadWriteSync >( SRWLock )->LockWrite();
	}


	BOOLEAN CALLBACK DefTryAcquireSRWLockExclusive (/*inout*/ PSRWLOCK SRWLock)
	{
		return HandleToPointer< TDefReadWriteSync >( SRWLock )->TryLockWrite();
	}


	VOID CALLBACK DefReleaseSRWLockExclusive (/*inout*/ PSRWLOCK SRWLock)
	{
		HandleToPointer< TDefReadWriteSync >( SRWLock )->UnlockWrite();
	}


	VOID CALLBACK DefAcquireSRWLockShared (/*inout*/ PSRWLOCK SRWLock)
	{
		HandleToPointer< TDefReadWriteSync >( SRWLock )->LockRead();
	}


	BOOLEAN CALLBACK DefTryAcquireSRWLockShared (/*inout*/ PSRWLOCK SRWLock)
	{
		return HandleToPointer< TDefReadWriteSync >( SRWLock )->TryLockRead();
	}


	VOID CALLBACK DefReleaseSRWLockShared (/*inout*/ PSRWLOCK SRWLock)
	{
		HandleToPointer< TDefReadWriteSync >( SRWLock )->UnlockRead();
	}
	

	VOID CALLBACK DefDeleteSRWLock (/*inout*/ PSRWLOCK SRWLock)
	{
		delete HandleToPointer< TDefReadWriteSync >( SRWLock );
		HandleToPointerRef< TDefReadWriteSync >( SRWLock ) = null;
	}
	

	VOID CALLBACK WinDeleteSRWLock (/*inout*/ PSRWLOCK SRWLock)
	{
		GX_UNUSED( SRWLock );
		return;
	}
	
	

	static PInitializeSRWLockProc_t				_InitializeSRWLock				= & DefInitializeSRWLock;
	static PAcquireSRWLockExclusiveProc_t		_AcquireSRWLockExclusive		= & DefAcquireSRWLockExclusive;
	static PTryAcquireSRWLockExclusiveProc_t	_TryAcquireSRWLockExclusive		= & DefTryAcquireSRWLockExclusive;
	static PReleaseSRWLockExclusiveProc_t		_ReleaseSRWLockExclusive		= & DefReleaseSRWLockExclusive;
	static PAcquireSRWLockSharedProc_t			_AcquireSRWLockShared			= & DefAcquireSRWLockShared;
	static PTryAcquireSRWLockSharedProc_t		_TryAcquireSRWLockShared		= & DefTryAcquireSRWLockShared;
	static PReleaseSRWLockSharedProc_t			_ReleaseSRWLockShared			= & DefReleaseSRWLockShared;
	static PDeleteSRWLockProc_t					_DeleteSRWLock					= & DefDeleteSRWLock;
	bool										ReadWriteSync::_isInitialized	= false;
	

	void ReadWriteSync::_InitSRWFuncPointers()
	{
		Library		s_lib("kernel32.dll");
		bool		res = true;

		if ( s_lib.IsValid() )
		{
			res &= SetPointer( _InitializeSRWLock,			s_lib.GetProc("InitializeSRWLock") );
			res &= SetPointer( _AcquireSRWLockExclusive,	s_lib.GetProc("AcquireSRWLockExclusive") );
			res &= SetPointer( _TryAcquireSRWLockExclusive,	s_lib.GetProc("TryAcquireSRWLockExclusive") );
			res &= SetPointer( _ReleaseSRWLockExclusive,	s_lib.GetProc("ReleaseSRWLockExclusive") );
			res &= SetPointer( _AcquireSRWLockShared,		s_lib.GetProc("AcquireSRWLockShared") );
			res &= SetPointer( _TryAcquireSRWLockShared,	s_lib.GetProc("TryAcquireSRWLockShared") );
			res &= SetPointer( _ReleaseSRWLockShared,		s_lib.GetProc("ReleaseSRWLockShared") );
			_DeleteSRWLock = & WinDeleteSRWLock;
		}

		if ( !res )
		{
			_InitializeSRWLock			= & DefInitializeSRWLock;
			_AcquireSRWLockExclusive	= & DefAcquireSRWLockExclusive;
			_TryAcquireSRWLockExclusive	= & DefTryAcquireSRWLockExclusive;
			_ReleaseSRWLockExclusive	= & DefReleaseSRWLockExclusive;
			_AcquireSRWLockShared		= & DefAcquireSRWLockShared;
			_TryAcquireSRWLockShared	= & DefTryAcquireSRWLockShared;
			_ReleaseSRWLockShared		= & DefReleaseSRWLockShared;
			_DeleteSRWLock				= & DefDeleteSRWLock;
		}

		_isInitialized = true;
	}

//-----------------------------------------------------------------------------



/*
=================================================
	constructor
=================================================
*/
	ReadWriteSync::ReadWriteSync () :
		_srw( UninitializedT<SRWLOCK>() )
	{
		if ( not _isInitialized )
			_InitSRWFuncPointers();

		_InitializeSRWLock( &_srw.Get<SRWLOCK>() );
	}
	
/*
=================================================
	destructor
=================================================
*/
	ReadWriteSync::~ReadWriteSync ()
	{
		_DeleteSRWLock( &_srw.Get<SRWLOCK>() );
	}
	
/*
=================================================
	LockWrite
=================================================
*/
	void ReadWriteSync::LockWrite ()
	{
		_AcquireSRWLockExclusive( &_srw.Get<SRWLOCK>() );
	}
	
/*
=================================================
	TryLockWrite
=================================================
*/
	bool ReadWriteSync::TryLockWrite ()
	{
		return _TryAcquireSRWLockExclusive( &_srw.Get<SRWLOCK>() ) != 0;
	}
	
/*
=================================================
	UnlockWrite
=================================================
*/
	void ReadWriteSync::UnlockWrite ()
	{
		_ReleaseSRWLockExclusive( &_srw.Get<SRWLOCK>() );
	}
	
/*
=================================================
	LockRead
=================================================
*/
	void ReadWriteSync::LockRead ()
	{
		_AcquireSRWLockShared( &_srw.Get<SRWLOCK>() );
	}
	
/*
=================================================
	TryLockRead
=================================================
*/
	bool ReadWriteSync::TryLockRead ()
	{
		return _TryAcquireSRWLockShared( &_srw.Get<SRWLOCK>() ) != 0;
	}
	
/*
=================================================
	UnlockRead
=================================================
*/
	void ReadWriteSync::UnlockRead ()
	{
		_ReleaseSRWLockShared( &_srw.Get<SRWLOCK>() );
	}

//-----------------------------------------------------------------------------
	


	typedef VOID	(CALLBACK *PInitializeConditionVariableProc_t)(OUT   PCONDITION_VARIABLE ConditionVariable);
	typedef VOID	(CALLBACK *PWakeConditionVariableProc_t)	  (INOUT PCONDITION_VARIABLE ConditionVariable);
	typedef VOID	(CALLBACK *PWakeAllConditionVariableProc_t)	  (INOUT PCONDITION_VARIABLE ConditionVariable);
	typedef BOOL	(CALLBACK *PSleepConditionVariableCSProc_t)	  (INOUT PCONDITION_VARIABLE ConditionVariable,
																	INOUT PCRITICAL_SECTION CriticalSection,
																	        DWORD dwMilliseconds);
	typedef BOOL	(CALLBACK *PSleepConditionVariableSRWProc_t)  (INOUT PCONDITION_VARIABLE ConditionVariable,
																	INOUT PSRWLOCK SRWLock,
																	        DWORD dwMilliseconds,
																	        ULONG Flags);
	typedef VOID	(CALLBACK *PDeleteConditionVariableProc_t)	  (INOUT PCONDITION_VARIABLE ConditionVariable);
		


	static VOID CALLBACK DefInitializeConditionVariable (OUT PCONDITION_VARIABLE cv)
	{
		HandleToPointerRef< TDefConditionVariable >( cv ) = new TDefConditionVariable;
	}


	static VOID CALLBACK DefWakeConditionVariable (INOUT PCONDITION_VARIABLE cv)
	{
		HandleToPointer< TDefConditionVariable >( cv )->Signal();
	}


	static VOID CALLBACK DefWakeAllConditionVariable (INOUT PCONDITION_VARIABLE cv)
	{
		HandleToPointer< TDefConditionVariable >( cv )->Broadcast();
	}


	static BOOL CALLBACK DefSleepConditionVariableCS (INOUT PCONDITION_VARIABLE /*cv*/,
									INOUT PCRITICAL_SECTION /*cs*/, DWORD /*dwMilliseconds*/)
	{
		//CriticalSection	tmp( *cs );
		//BOOL	res = HandleToPointer< TDefConditionVariable >( cv )->Wait( tmp, dwMilliseconds );
		//tmp._inited = false;
		//return res;
		TODO("");
		return false;
	}


	static BOOL CALLBACK DefSleepConditionVariableSRW (INOUT PCONDITION_VARIABLE cv,
									INOUT PSRWLOCK SRWLock, DWORD dwMilliseconds, ULONG Flags)
	{
		//return HandleToPointer< TDefConditionVariable >( cv )->Sleep( SRWLock, dwMilliseconds, Flags );
		GX_UNUSED( cv, SRWLock, dwMilliseconds, Flags );
		TODO("");
		return false;
	}
	

	static VOID CALLBACK DefDeleteConditionVariable (INOUT PCONDITION_VARIABLE cv)
	{
		delete HandleToPointer< TDefConditionVariable >( cv );
		HandleToPointerRef< TDefConditionVariable >( cv ) = null;
	}
	

	static VOID CALLBACK WinDeleteConditionVariable(/*inout*/ PCONDITION_VARIABLE cv)
	{
		GX_UNUSED( cv );
	}


	static PInitializeConditionVariableProc_t	_InitializeConditionVariable		= & DefInitializeConditionVariable;
	static PWakeConditionVariableProc_t			_WakeConditionVariable				= & DefWakeConditionVariable;
	static PWakeAllConditionVariableProc_t		_WakeAllConditionVariable			= & DefWakeAllConditionVariable;
	static PSleepConditionVariableCSProc_t		_SleepConditionVariableCS			= & DefSleepConditionVariableCS;
	static PSleepConditionVariableSRWProc_t		_SleepConditionVariableSRW			= & DefSleepConditionVariableSRW;
	static PDeleteConditionVariableProc_t		_DeleteConditionVariable			= & DefDeleteConditionVariable;
	bool										ConditionVariable::_isInitialized	= false;
		

	void ConditionVariable::_InitCondVarFuncPointers ()
	{
		Library		s_lib("kernel32.dll");
		bool		res = true;

		if ( s_lib.IsValid() )
		{
			res &= SetPointer( _InitializeConditionVariable,s_lib.GetProc("InitializeConditionVariable") );
			res &= SetPointer( _WakeConditionVariable,		s_lib.GetProc("WakeConditionVariable") );
			res &= SetPointer( _WakeAllConditionVariable,	s_lib.GetProc("WakeAllConditionVariable") );
			res &= SetPointer( _SleepConditionVariableCS,	s_lib.GetProc("SleepConditionVariableCS") );
			res &= SetPointer( _SleepConditionVariableSRW,	s_lib.GetProc("SleepConditionVariableSRW") );
			_DeleteConditionVariable = & WinDeleteConditionVariable;
		}

		if ( !res )
		{
			_InitializeConditionVariable	= & DefInitializeConditionVariable;
			_WakeConditionVariable			= & DefWakeConditionVariable;
			_WakeAllConditionVariable		= & DefWakeAllConditionVariable;
			_SleepConditionVariableCS		= & DefSleepConditionVariableCS;
			_SleepConditionVariableSRW		= & DefSleepConditionVariableSRW;
			_DeleteConditionVariable		= & DefDeleteConditionVariable;
		}

		_isInitialized = true;
	}

//-----------------------------------------------------------------------------
	

	
/*
=================================================
	constructor
=================================================
*/
	ConditionVariable::ConditionVariable () :
		_cv( UninitializedT<CONDITION_VARIABLE>() ),
		_inited(false)
	{
		if ( not _isInitialized )
			_InitCondVarFuncPointers();

		_Create();
	}
	
/*
=================================================
	destructor
=================================================
*/
	ConditionVariable::~ConditionVariable ()
	{
		_Destroy();
	}
	
/*
=================================================
	_Create
=================================================
*/
	bool ConditionVariable::_Create ()
	{
		_Destroy();
		_InitializeConditionVariable( &_cv.Get<CONDITION_VARIABLE>() );
		_inited = true;
		return IsValid();
	}
	
/*
=================================================
	_Destroy
=================================================
*/
	void ConditionVariable::_Destroy ()
	{
		if ( IsValid() )
		{
			_inited = false;
			_DeleteConditionVariable( &_cv.Get<CONDITION_VARIABLE>() );
		}
	}
	
/*
=================================================
	Signal
=================================================
*/
	void ConditionVariable::Signal ()
	{
		ASSERT( IsValid() );
		_WakeConditionVariable( &_cv.Get<CONDITION_VARIABLE>() );
	}
	
/*
=================================================
	Broadcast
=================================================
*/
	void ConditionVariable::Broadcast ()
	{
		ASSERT( IsValid() );
		_WakeAllConditionVariable( &_cv.Get<CONDITION_VARIABLE>() );
	}
	
/*
=================================================
	Wait
=================================================
*/
	bool ConditionVariable::Wait (CriticalSection &cs, TimeL time)
	{
		ASSERT( IsValid() );
		return _SleepConditionVariableCS( &_cv.Get<CONDITION_VARIABLE>(),
										  &cs._crSection.Get<CRITICAL_SECTION>(),
										  (uint) time.MilliSeconds() ) != FALSE;
	}

//-----------------------------------------------------------------------------
	


/*
=================================================
	constructor
=================================================
*/
	SyncEvent::SyncEvent (EFlags flags) :
		_event( UninitializedT<HANDLE>() )
	{
		_Create( flags );
	}
	
/*
=================================================
	destructor
=================================================
*/
	SyncEvent::~SyncEvent ()
	{
		_Delete();
	}
	
/*
=================================================
	Signal
=================================================
*/
	bool SyncEvent::Signal ()
	{
		return ::SetEvent( _event.Get<HANDLE>() ) != FALSE;
	}
	
/*
=================================================
	Reset
=================================================
*/
	bool SyncEvent::Reset ()
	{
		return ::ResetEvent( _event.Get<HANDLE>() ) != FALSE;
	}
	
/*
=================================================
	Wait
=================================================
*/
	bool SyncEvent::Wait ()
	{
		return ::WaitForSingleObject( _event.Get<HANDLE>(), INFINITE ) == WAIT_OBJECT_0;
	}

	bool SyncEvent::Wait (TimeL time)
	{
		return ::WaitForSingleObject( _event.Get<HANDLE>(), (uint) time.MilliSeconds() ) == WAIT_OBJECT_0;
	}
	
/*
=================================================
	WaitEvents
=================================================
*/
	int SyncEvent::WaitEvents (ArrayRef<Self *> events, bool waitAll, TimeL time)
	{
		FixedSizeArray< HANDLE, 16 >	events_arr;

		FOR( i, events ) {
			events_arr.PushBack( events[i]->_event.Get<HANDLE>() );
		}

		DWORD	res		= ::WaitForMultipleObjects( (DWORD)events_arr.Count(), events_arr.ptr(), waitAll, (uint) time.MilliSeconds() );
		bool	success	= (res >= WAIT_OBJECT_0 and res <= WAIT_OBJECT_0 + events_arr.Count());

		return success ? res - WAIT_OBJECT_0 : -1;
	}
	
/*
=================================================
	_Create
=================================================
*/
	bool SyncEvent::_Create (EFlags flags)
	{
		_Delete();
		_event = ::CreateEventA( null,
								 not EnumEq( flags, AUTO_RESET ),
								 EnumEq( flags, INIT_STATE_SIGNALED ),
								 (const char *)null );
		return IsValid();
	}
	
/*
=================================================
	_Delete
=================================================
*/
	bool SyncEvent::_Delete()
	{
		bool	ret = true;

		if ( IsValid() )
		{
			ret		= ::CloseHandle( _event.Get<HANDLE>() ) != FALSE;
			_event	= null;
		}
		return ret;
	}

//-----------------------------------------------------------------------------
	


/*
=================================================
	constructor
=================================================
*/
	Semaphore::Semaphore (uint initialValue) : _sem( UninitializedT<HANDLE>() )
	{
		_Create( initialValue );
	}
	
/*
=================================================
	destructor
=================================================
*/
	Semaphore::~Semaphore ()
	{
		_Destroy();
	}
	
/*
=================================================
	Lock
=================================================
*/
	void Semaphore::Lock ()
	{
		uint result = ::WaitForSingleObject( _sem.Get<HANDLE>(), INFINITE );
		ASSERT( result == WAIT_OBJECT_0 );
	}
	
/*
=================================================
	TryLock
=================================================
*/
	bool Semaphore::TryLock ()
	{
		uint result = ::WaitForSingleObject( _sem.Get<HANDLE>(), 1 );
		return result == WAIT_OBJECT_0;
	}
	
/*
=================================================
	Unlock
=================================================
*/
	void Semaphore::Unlock ()
	{
		bool	res = ::ReleaseSemaphore( _sem.Get<HANDLE>(), 1, null ) != FALSE;
		ASSERT( res );
	}
	
/*
=================================================
	GetValue
=================================================
*/
	uint Semaphore::GetValue ()
	{
		TODO( "GetValue" );
		return 0;
	}
	
/*
=================================================
	_Create
=================================================
*/
	bool Semaphore::_Create (uint initialValue)
	{
		_sem = ::CreateSemaphoreA( null, initialValue, MaxValue<int>(), (const char *)null );
		return IsValid();
	}
	
/*
=================================================
	_Delete
=================================================
*/
	void Semaphore::_Destroy ()
	{
		if ( _sem.IsNotNull<HANDLE>() )
		{
			::CloseHandle( _sem.Get<HANDLE>() );

			_sem.Get<HANDLE>() = null;
		}
	}
		
//-----------------------------------------------------------------------------


}	// OS
}	// GX_STL

#endif	// PLATFORM_WINDOWS