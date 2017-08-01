// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Engine/Base/Threads/ParallelThread.h"
#include "Engine/Base/Threads/ThreadManager.h"
#include "Engine/Base/Modules/ModuleAsyncTasks.h"


namespace Engine
{
namespace Base
{
	
	const Runtime::VirtualTypeList	ParallelThread::_msgTypes{ UninitializedT< SupportedMessages_t >() };
	const Runtime::VirtualTypeList	ParallelThread::_eventTypes{ UninitializedT< SupportedEvents_t >() };

/*
=================================================
	constructor
=================================================
*/
	ParallelThread::ParallelThread (const GlobalSystemsRef gs, const CreateInfo::Thread &info) :
		Module( gs, ModuleConfig{ GetStaticID(), 1 }, &_msgTypes, &_eventTypes ),
		_onStarted( RVREF( info.onStarted.Get() ) ),
		_isLooping( false )
	{
		SetDebugName( info.name );

		GlobalSystems()->GetSetter< ParallelThread >().Set( this );

		_SubscribeOnMsg( this, &ParallelThread::_OnModuleAttached_Impl );
		_SubscribeOnMsg( this, &ParallelThread::_OnModuleDetached_Impl );
		_SubscribeOnMsg( this, &ParallelThread::_AttachModule_Impl );
		_SubscribeOnMsg( this, &ParallelThread::_DetachModule_Impl );
		_SubscribeOnMsg( this, &ParallelThread::_OnManagerChanged_Empty );
		_SubscribeOnMsg( this, &ParallelThread::_FindModule_Impl );
		_SubscribeOnMsg( this, &ParallelThread::_ModulesDeepSearch_Impl );
		_SubscribeOnMsg( this, &ParallelThread::_Link );
		_SubscribeOnMsg( this, &ParallelThread::_Compose );
		_SubscribeOnMsg( this, &ParallelThread::_Delete );
		
		CHECK( _ValidateMsgSubscriptions() );
	}
	
/*
=================================================
	destructor
=================================================
*/
	ParallelThread::~ParallelThread ()
	{
		CHECK( GetThreadID() == ThreadID::GetCurrent() );

		LOG( "ParallelThread finalized", ELog::Debug );

		GlobalSystems()->GetSetter< ParallelThread >().Set( null );

		ASSERT( not _isLooping );
	}
	
/*
=================================================
	_Link
=================================================
*/
	bool ParallelThread::_Link (const Message< ModuleMsg::Link > &msg)
	{
		CHECK_ERR( msg.Sender() and msg.Sender() == _GetManager() );

		CHECK_ERR( Module::_Link_Impl( msg ) );
		return true;
	}
	
/*
=================================================
	_Compose
=================================================
*/
	bool ParallelThread::_Compose (const Message< ModuleMsg::Compose > &msg)
	{
		CHECK_ERR( msg.Sender() and msg.Sender() == _GetManager() );

		CHECK_ERR( Module::_Compose_Impl( msg ) );
		return true;
	}

/*
=================================================
	_Delete
=================================================
*/
	bool ParallelThread::_Delete (const Message< ModuleMsg::Delete > &msg)
	{
		CHECK_ERR( msg.Sender() and msg.Sender() == _GetManager() );

		_isLooping = false;

		_SendForEachAttachments( Message< ModuleMsg::Delete >{} );

		CHECK_ERR( Module::_Delete_Impl( msg ) );
		return true;
	}

/*
=================================================
	_OnEnter
=================================================
*/
	void ParallelThread::_OnEnter ()
	{
		CHECK( GetThreadID() == ThreadID::GetCurrent() );

		_isLooping	= true;
		
		_timer.Start();
		
		if ( _onStarted )
		{
			_onStarted( this );
			_onStarted = null;
		}
	}

/*
=================================================
	_Loop
=================================================
*/
	void ParallelThread::_Loop ()
	{
		CHECK( GetThreadID() == ThreadID::GetCurrent() );

		while ( _isLooping )
		{
			const TimeD		dt = _timer.GetTimeDelta();		_timer.Start();

			// update attached modules
			_SendForEachAttachments( Message< ModuleMsg::Update >{ dt } );

			// calc time to sleep
			const TimeD		upd_dt = _timer.GetTimeDelta();
			double			factor = upd_dt.Seconds() / dt.Seconds();

			if ( factor > 0.5 )
				OS::Thread::Yield();
			else
				OS::Thread::Sleep( TimeL::FromMilliSeconds( 10 ) );
		}
	}
	
/*
=================================================
	_OnExit
=================================================
*/
	void ParallelThread::_OnExit ()
	{
		/*CHECK( GetThreadID() == ThreadID::GetCurrent() );

		const TimeD		dt = _timer.GetTimeDelta();		_timer.Start();

		// last update to proccess messages
		_SendForEachAttachments( Message< ModuleMsg::Update >{ dt } );*/
	}
	
/*
=================================================
	_SyncUpdate
=================================================
*/
	void ParallelThread::_SyncUpdate ()
	{
		CHECK( GetThreadID() == ThreadID::GetCurrent() );
		CHECK_ERR( _isLooping, void() );

		const TimeD		dt = _timer.GetTimeDelta();		_timer.Start();

		// update attached modules
		_SendForEachAttachments( Message< ModuleMsg::Update >{ dt } );
	}


}	// Base
}	// Engine
