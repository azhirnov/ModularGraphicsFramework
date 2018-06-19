// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Engine/Base/Threads/ParallelThreadImpl.h"

namespace Engine
{
namespace Base
{
	
	const TypeIdList	ParallelThreadImpl::_msgTypes{ UninitializedT< SupportedMessages_t >() };
	const TypeIdList	ParallelThreadImpl::_eventTypes{ UninitializedT< SupportedEvents_t >() };

/*
=================================================
	constructor
=================================================
*/
	ParallelThreadImpl::ParallelThreadImpl (UntypedID_t id, GlobalSystemsRef gs, const CreateInfo::Thread &info) :
		Module( gs, ModuleConfig{ id, 1 }, &_msgTypes, &_eventTypes ),
		_onStarted( RVREF( info.onStarted.Get() ) ),
		_isLooping( false )
	{
		GlobalSystems()->parallelThread._Set( this );

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
		LOG( "ParallelThread finalized", ELog::Debug );

		ASSERT( not _isLooping );

		if ( GetThreadID() == ThreadID::GetCurrent() ) {
			GlobalSystems()->parallelThread._Set( null );
		}
	}
	
/*
=================================================
	_Link
=================================================
*/
	bool ParallelThreadImpl::_Link (const ModuleMsg::Link &msg)
	{
		//CHECK_ERR( msg.Sender() and msg.Sender() == _GetManager() );

		return Module::_Link_Impl( msg );
	}
	
/*
=================================================
	_Compose
=================================================
*/
	bool ParallelThreadImpl::_Compose (const ModuleMsg::Compose &)
	{
		if ( _IsComposedState( GetState() ) )
			return true;	// already composed

		//CHECK_ERR( msg.Sender() and msg.Sender() == _GetManager() );

		CHECK( Module::_DefCompose( false ) );
		return true;
	}

/*
=================================================
	_Delete
=================================================
*/
	bool ParallelThreadImpl::_Delete (const ModuleMsg::Delete &msg)
	{
		//CHECK_ERR( msg.Sender() and msg.Sender() == _GetManager() );

		_isLooping = false;

		_SendForEachAttachments( msg );

		_DetachSelfFromManager();
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
			_onStarted( this->GlobalSystems() );
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
			const TimeL		dt = _timer.GetTimeDelta();		_timer.Start();

			// update attached modules
			_SendForEachAttachments( ModuleMsg::Update{ dt } );

			// calc time to sleep
			const TimeD		upd_dt = TimeD(_timer.GetTimeDelta());
			double			factor = upd_dt.Seconds() / TimeD(dt).Seconds();

			if ( factor > 0.5 )
				OS::Thread::Yield();
			else
				OS::Thread::Sleep( 10_milliSec );
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

		const TimeL		dt = _timer.GetTimeDelta();		_timer.Start();

		// last update to proccess messages
		_SendForEachAttachments( ModuleMsg::Update{ dt } );

		CHECK( Module::_Delete_Impl( ModuleMsg::Delete{} ) );
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

/*
=================================================
	_SyncUpdate
=================================================
*/
	void ParallelThreadImpl::_SyncUpdate ()
	{
		CHECK( GetThreadID() == ThreadID::GetCurrent() );
		CHECK_ERR( _isLooping, void() );

		const TimeL		dt = _timer.GetTimeDelta();		_timer.Start();

		// update attached modules
		_SendForEachAttachments( ModuleMsg::Update { dt } );
	}

/*
=================================================
	_NoWait
=================================================
*/
	void ParallelThreadImpl::_NoWait ()
	{
	}

}	// Base
}	// Engine
