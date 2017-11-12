// Copyright �  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Engine/Base/Threads/ParallelThreadImpl.h"
#include "Engine/Base/Modules/ModuleAsyncTasks.h"


namespace Engine
{
namespace Base
{

/*
=================================================
	constructor
=================================================
*/
	ParallelThread::ParallelThread (const GlobalSystemsRef gs,
					const ModuleConfig &config,
					const Runtime::VirtualTypeList *msgTypes,
					const Runtime::VirtualTypeList *eventTypes) :
		Module( gs, config, msgTypes, eventTypes )
	{
		GlobalSystems()->GetSetter< ParallelThread >().Set( this );
	}
	
/*
=================================================
	destructor
=================================================
*/
	ParallelThread::~ParallelThread ()
	{
		GlobalSystems()->GetSetter< ParallelThread >().Set( null );
	}
//-----------------------------------------------------------------------------


	
	const Runtime::VirtualTypeList	ParallelThreadImpl::_msgTypes{ UninitializedT< SupportedMessages_t >() };
	const Runtime::VirtualTypeList	ParallelThreadImpl::_eventTypes{ UninitializedT< SupportedEvents_t >() };

/*
=================================================
	constructor
=================================================
*/
	ParallelThreadImpl::ParallelThreadImpl (const GlobalSystemsRef gs, const CreateInfo::Thread &info) :
		ParallelThread( gs, ModuleConfig{ ParallelThreadModuleID, 1 }, &_msgTypes, &_eventTypes ),
		_onStarted( RVREF( info.onStarted.Get() ) ),
		_isLooping( false )
	{
		SetDebugName( info.name );

		_SubscribeOnMsg( this, &ParallelThreadImpl::_OnModuleAttached_Impl );
		_SubscribeOnMsg( this, &ParallelThreadImpl::_OnModuleDetached_Impl );
		_SubscribeOnMsg( this, &ParallelThreadImpl::_AttachModule_Impl );
		_SubscribeOnMsg( this, &ParallelThreadImpl::_DetachModule_Impl );
		_SubscribeOnMsg( this, &ParallelThreadImpl::_OnManagerChanged_Empty );
		_SubscribeOnMsg( this, &ParallelThreadImpl::_FindModule_Impl );
		_SubscribeOnMsg( this, &ParallelThreadImpl::_ModulesDeepSearch_Impl );
		_SubscribeOnMsg( this, &ParallelThreadImpl::_Link );
		_SubscribeOnMsg( this, &ParallelThreadImpl::_Compose );
		_SubscribeOnMsg( this, &ParallelThreadImpl::_Delete );
		
		CHECK( _ValidateMsgSubscriptions() );
	}
	
/*
=================================================
	destructor
=================================================
*/
	ParallelThreadImpl::~ParallelThreadImpl ()
	{
		CHECK( GetThreadID() == ThreadID::GetCurrent() );

		LOG( "ParallelThread finalized", ELog::Debug );

		ASSERT( not _isLooping );
	}
	
/*
=================================================
	_Link
=================================================
*/
	bool ParallelThreadImpl::_Link (const Message< ModuleMsg::Link > &msg)
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
	bool ParallelThreadImpl::_Compose (const Message< ModuleMsg::Compose > &msg)
	{
		CHECK_ERR( msg.Sender() and msg.Sender() == _GetManager() );

		CHECK_ERR( Module::_DefCompose( false ) );
		return true;
	}

/*
=================================================
	_Delete
=================================================
*/
	bool ParallelThreadImpl::_Delete (const Message< ModuleMsg::Delete > &msg)
	{
		CHECK_ERR( msg.Sender() and msg.Sender() == _GetManager() );

		_isLooping = false;

		_SendForEachAttachments< ModuleMsg::Delete >({});

		CHECK_ERR( Module::_Delete_Impl( msg ) );
		return true;
	}

/*
=================================================
	_OnEnter
=================================================
*/
	void ParallelThreadImpl::_OnEnter ()
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
	void ParallelThreadImpl::_Loop ()
	{
		CHECK( GetThreadID() == ThreadID::GetCurrent() );

		while ( _isLooping )
		{
			const TimeD		dt = _timer.GetTimeDelta();		_timer.Start();

			// update attached modules
			_SendForEachAttachments< ModuleMsg::Update >({ dt });

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
	void ParallelThreadImpl::_OnExit ()
	{
		CHECK( GetThreadID() == ThreadID::GetCurrent() );

		const TimeD		dt = _timer.GetTimeDelta();		_timer.Start();

		// last update to proccess messages
		_SendForEachAttachments< ModuleMsg::Update >({ dt });
	}
	
/*
=================================================
	_SyncUpdate
=================================================
*/
	void ParallelThreadImpl::_SyncUpdate ()
	{
		CHECK( GetThreadID() == ThreadID::GetCurrent() );
		CHECK_ERR( _isLooping, void() );

		const TimeD		dt = _timer.GetTimeDelta();		_timer.Start();

		// update attached modules
		_SendForEachAttachments< ModuleMsg::Update >({ dt });
	}
	
/*
=================================================
	_Wait
=================================================
*/
	void ParallelThreadImpl::_Wait ()
	{
		_thread.Wait();
	}

}	// Base
}	// Engine