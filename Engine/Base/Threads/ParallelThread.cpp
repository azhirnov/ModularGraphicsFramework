// Copyright © 2014-2017  Zhirnov Andrey. All rights reserved.

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
	ParallelThread::ParallelThread (const SubSystemsRef gs, const CreateInfo::Thread &info) :
		Module( gs, GetStaticID(), &_msgTypes, &_eventTypes ),
		_isLooping( false )
	{
		SetDebugName( info.name );

		GlobalSystems()->GetSetter< ParallelThread >().Set( this );

		_SubscribeOnMsg( this, &ParallelThread::_OnModuleAttached );
		_SubscribeOnMsg( this, &ParallelThread::_OnModuleDetached );
		_SubscribeOnMsg( this, &ParallelThread::_AttachModule_Impl );
		_SubscribeOnMsg( this, &ParallelThread::_DetachModule_Impl );
		_SubscribeOnMsg( this, &ParallelThread::_FindModule_Impl );
		_SubscribeOnMsg( this, &ParallelThread::_Link );
		_SubscribeOnMsg( this, &ParallelThread::_Compose );
		_SubscribeOnMsg( this, &ParallelThread::_Delete );
		
		CHECK( _ValidateMsgSubscriptions() );

		if ( not info.manager )
			GXTypes::New< AttachModuleToManagerAsyncTask >( this, ThreadManager::GetStaticID() )->Execute()->Wait();
		else
			_SetManager( info.manager );
	}
	
/*
=================================================
	destructor
=================================================
*/
	ParallelThread::~ParallelThread ()
	{
		LOG( "ParallelThread finalized", ELog::Debug );

		GlobalSystems()->GetSetter< ParallelThread >().Set( null );

		ASSERT( not _isLooping );
	}
	
/*
=================================================
	_Link
=================================================
*/
	void ParallelThread::_Link (const Message< ModuleMsg::Link > &msg)
	{
		CHECK_ERR( msg.Sender() and msg.Sender() == _GetManager(), void() );

		Module::_Link_Impl( msg );
	}
	
/*
=================================================
	_Compose
=================================================
*/
	void ParallelThread::_Compose (const Message< ModuleMsg::Compose > &msg)
	{
		CHECK_ERR( msg.Sender() and msg.Sender() == _GetManager(), void() );

		Module::_Compose_Impl( msg );
	}

/*
=================================================
	_Delete
=================================================
*/
	void ParallelThread::_Delete (const Message< ModuleMsg::Delete > &msg)
	{
		CHECK_ERR( msg.Sender() and msg.Sender() == _GetManager(), void() );

		_isLooping = false;
		
		Modules_t	tmp = _GetAttachments();

		_SendForEachAttachments( Message< ModuleMsg::Delete >( this ) );

		Module::_Delete_Impl( msg );

		GXTypes::New< DetachModuleFromManagerAsyncTask >( this, ThreadManager::GetStaticID() )->Execute();
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
			_SendForEachAttachments( Message<ModuleMsg::Update>{ this, dt } );

			// calc time to sleep
			const TimeD		upd_dt = _timer.GetTimeDelta();
			double			factor = upd_dt.Seconds() / dt.Seconds();

			if ( factor > 0.5 )
				OS::Thread::Yield();
			else
				OS::Thread::Sleep( TimeU::FromMilliSeconds( 10 ) );
		}
	}
	
/*
=================================================
	_OnExit
=================================================
*/
	void ParallelThread::_OnExit ()
	{
		CHECK( GetThreadID() == ThreadID::GetCurrent() );

		const TimeD		dt = _timer.GetTimeDelta();		_timer.Start();

		// last update to proccess messages
		_SendForEachAttachments( Message<ModuleMsg::Update>{ this, dt } );
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
		_SendForEachAttachments( Message<ModuleMsg::Update>{ this, dt } );
	}


}	// Base
}	// Engine
