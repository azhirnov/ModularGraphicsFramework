// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Engine/Config/Engine.Config.h"

#ifdef COMPUTE_API_OPENCL

#include "Engine/Platforms/Public/GPU/Sync.h"
#include "Engine/Platforms/OpenCL/Impl/CL2BaseModule.h"
#include "Engine/Platforms/OpenCL/OpenCLObjectsConstructor.h"

namespace Engine
{
namespace PlatformCL
{
	using namespace cl;

	
	//
	// OpenCL Sync Manager
	//

	class CL2SyncManager final : public CL2BaseModule
	{
	// types
	private:
		using SupportedMessages_t	= CL2BaseModule::SupportedMessages_t::Append< MessageListFrom<
											GpuMsg::ClientWaitDeviceIdle,
											GpuMsg::CreateFence,
											GpuMsg::DestroyFence,
											GpuMsg::CLFenceSync,
											GpuMsg::ClientWaitFence,
											GpuMsg::CreateEvent,
											GpuMsg::DestroyEvent,
											GpuMsg::GetCLEvent,
											GpuMsg::SetEvent,
											GpuMsg::ResetEvent,
											GpuMsg::CreateSemaphore,
											GpuMsg::DestroySemaphore,
											GpuMsg::CLSemaphoreEnqueue,
											GpuMsg::WaitCLSemaphore
										> >;

		using SupportedEvents_t		= CL2BaseModule::SupportedEvents_t;

		using SyncUnion_t			= Union< cl_event >;
		using SyncArray_t			= Map< ulong, SyncUnion_t >;

		struct _DeleteSync_Func;


	// constants
	private:
		static const TypeIdList		_msgTypes;
		static const TypeIdList		_eventTypes;


	// variables
	private:
		SyncArray_t		_syncs;
		ulong			_counter;


	// methods
	public:
		CL2SyncManager (UntypedID_t, GlobalSystemsRef gs, const CreateInfo::GpuSyncManager &ci);
		~CL2SyncManager ();


	// message handlers
	private:
		bool _Delete (const Message< ModuleMsg::Delete > &);
		bool _OnManagerChanged2 (const Message< ModuleMsg::OnManagerChanged > &msg);

		bool _ClientWaitDeviceIdle (const Message< GpuMsg::ClientWaitDeviceIdle > &);
		bool _CreateFence (const Message< GpuMsg::CreateFence > &);
		bool _DestroyFence (const Message< GpuMsg::DestroyFence > &);
		bool _CLFenceSync (const Message< GpuMsg::CLFenceSync > &);
		bool _ClientWaitFence (const Message< GpuMsg::ClientWaitFence > &);
		bool _CreateEvent (const Message< GpuMsg::CreateEvent > &);
		bool _DestroyEvent (const Message< GpuMsg::DestroyEvent > &);
		bool _GetCLEvent (const Message< GpuMsg::GetCLEvent > &);
		bool _SetEvent (const Message< GpuMsg::SetEvent > &);
		bool _ResetEvent (const Message< GpuMsg::ResetEvent > &);
		bool _CreateSemaphore (const Message< GpuMsg::CreateSemaphore > &);
		bool _DestroySemaphore (const Message< GpuMsg::DestroySemaphore > &);
		bool _CLSemaphoreEnqueue (const Message< GpuMsg::CLSemaphoreEnqueue > &);
		bool _WaitCLSemaphore (const Message< GpuMsg::WaitCLSemaphore > &);
	};
//-----------------------------------------------------------------------------


	
	const TypeIdList	CL2SyncManager::_msgTypes{ UninitializedT< SupportedMessages_t >() };
	const TypeIdList	CL2SyncManager::_eventTypes{ UninitializedT< SupportedEvents_t >() };

/*
=================================================
	constructor
=================================================
*/
	CL2SyncManager::CL2SyncManager (UntypedID_t id, GlobalSystemsRef gs, const CreateInfo::GpuSyncManager &ci) :
		CL2BaseModule( gs, ModuleConfig{ id, 1 }, &_msgTypes, &_eventTypes ),
		_counter{ 0 }
	{
		SetDebugName( "CL2SyncManager" );

		_SubscribeOnMsg( this, &CL2SyncManager::_OnModuleAttached_Impl );
		_SubscribeOnMsg( this, &CL2SyncManager::_OnModuleDetached_Impl );
		_SubscribeOnMsg( this, &CL2SyncManager::_AttachModule_Empty );
		_SubscribeOnMsg( this, &CL2SyncManager::_DetachModule_Empty );
		_SubscribeOnMsg( this, &CL2SyncManager::_FindModule_Impl );
		_SubscribeOnMsg( this, &CL2SyncManager::_ModulesDeepSearch_Impl );
		_SubscribeOnMsg( this, &CL2SyncManager::_Link_Impl );
		_SubscribeOnMsg( this, &CL2SyncManager::_Compose_Impl );
		_SubscribeOnMsg( this, &CL2SyncManager::_Delete );
		_SubscribeOnMsg( this, &CL2SyncManager::_OnManagerChanged2 );
		_SubscribeOnMsg( this, &CL2SyncManager::_GetDeviceInfo );
		_SubscribeOnMsg( this, &CL2SyncManager::_GetCLDeviceInfo );
		_SubscribeOnMsg( this, &CL2SyncManager::_GetCLPrivateClasses );
		_SubscribeOnMsg( this, &CL2SyncManager::_ClientWaitDeviceIdle );
		_SubscribeOnMsg( this, &CL2SyncManager::_CreateFence );
		_SubscribeOnMsg( this, &CL2SyncManager::_DestroyFence );
		_SubscribeOnMsg( this, &CL2SyncManager::_CLFenceSync );
		_SubscribeOnMsg( this, &CL2SyncManager::_ClientWaitFence );
		_SubscribeOnMsg( this, &CL2SyncManager::_CreateEvent );
		_SubscribeOnMsg( this, &CL2SyncManager::_DestroyEvent );
		_SubscribeOnMsg( this, &CL2SyncManager::_GetCLEvent );
		_SubscribeOnMsg( this, &CL2SyncManager::_SetEvent );
		_SubscribeOnMsg( this, &CL2SyncManager::_ResetEvent );
		_SubscribeOnMsg( this, &CL2SyncManager::_CreateSemaphore );
		_SubscribeOnMsg( this, &CL2SyncManager::_DestroySemaphore );
		_SubscribeOnMsg( this, &CL2SyncManager::_CLSemaphoreEnqueue );
		_SubscribeOnMsg( this, &CL2SyncManager::_WaitCLSemaphore );

		CHECK( _ValidateMsgSubscriptions() );

		_AttachSelfToManager( _GetGPUThread( ci.gpuThread ), UntypedID_t(0), true );
	}
	
/*
=================================================
	destructor
=================================================
*/
	CL2SyncManager::~CL2SyncManager ()
	{
		ASSERT( _syncs.Empty() );
	}
	
/*
=================================================
	_DeleteSync_Func
=================================================
*/
	struct CL2SyncManager::_DeleteSync_Func
	{
		_DeleteSync_Func ()
		{}

		void operator () (cl_event ev) const
		{
			CL_CALL( clReleaseEvent( ev ) );
		}
	};

/*
=================================================
	_Delete
=================================================
*/
	bool CL2SyncManager::_Delete (const Message< ModuleMsg::Delete > &msg)
	{
		_DeleteSync_Func	func;

		FOR( i, _syncs ) {
			_syncs[i].second.Apply( func );
		}

		_syncs.Clear();

		return Module::_Delete_Impl( msg );
	}
	
/*
=================================================
	_OnManagerChanged2
=================================================
*/
	bool CL2SyncManager::_OnManagerChanged2 (const Message< ModuleMsg::OnManagerChanged > &msg)
	{
		_OnManagerChanged( msg );
		
		if ( msg->newManager )
		{
			msg->newManager->UnsubscribeAll( this );
		}
		return true;
	}
	
/*
=================================================
	_ClientWaitDeviceIdle
=================================================
*/
	bool CL2SyncManager::_ClientWaitDeviceIdle (const Message< GpuMsg::ClientWaitDeviceIdle > &)
	{
		CL_CALL( clFinish( GetCommandQueue() ) );
		return true;
	}

/*
=================================================
	_CreateFence
=================================================
*/
	bool CL2SyncManager::_CreateFence (const Message< GpuMsg::CreateFence > &msg)
	{
		for (;;)
		{
			++_counter;

			if ( not _syncs.IsExist( _counter ) )
				break;
		}
		_syncs.Add( _counter, SyncUnion_t{ cl_event(null) } );

		msg->result.Set( GpuFenceId(_counter) );
		return true;
	}
	
/*
=================================================
	_DestroyFence
=================================================
*/
	bool CL2SyncManager::_DestroyFence (const Message< GpuMsg::DestroyFence > &msg)
	{
		SyncArray_t::iterator	iter;

		if ( _syncs.Find( ulong(msg->id), OUT iter ) )
		{
			CHECK_ERR( iter->second.Is< cl_event >() );
			
			cl_event&	fence = iter->second.Get< cl_event >();

			if ( fence ) {
				CL_CALL( clReleaseEvent( fence ) );
			}

			_syncs.EraseByIter( iter );
		}
		return true;
	}
	
/*
=================================================
	_CLFenceSync
=================================================
*/
	bool CL2SyncManager::_CLFenceSync (const Message< GpuMsg::CLFenceSync > &msg)
	{
		SyncArray_t::iterator	iter;
		CHECK_ERR( _syncs.Find( ulong(msg->fenceId), OUT iter ) );
		CHECK_ERR( iter->second.Is< cl_event >() );
		
		cl_event&	fence = iter->second.Get< cl_event >();

		if ( fence ) {
			CL_CALL( clReleaseEvent( fence ) );
		}
		
		cl_int		cl_err = 0;
		CL_CALL(( (fence = clCreateUserEvent( GetContext(), OUT &cl_err )), cl_err ));

		CL_CALL( clEnqueueMarkerWithWaitList( GetCommandQueue(), 0, null, OUT &fence ) );

		msg->result.Set( fence );
		return true;
	}

/*
=================================================
	_ClientWaitFence
=================================================
*/
	bool CL2SyncManager::_ClientWaitFence (const Message< GpuMsg::ClientWaitFence > &msg)
	{
		using CLEvents_t	= FixedSizeArray< cl_event, GpuMsg::ClientWaitFence::Fences_t::MemoryContainer_t::SIZE >;

		CLEvents_t	events;

		FOR( i, msg->fences )
		{
			SyncArray_t::iterator	iter;
			CHECK_ERR( _syncs.Find( ulong(msg->fences[i]), OUT iter ) );

			cl_event	fence = iter->second.Get< cl_event >();

			if ( fence ) {
				events.PushBack( fence );
			}
		}
		
		CL_CALL( clWaitForEvents( events.Count(), events.RawPtr() ) );
		return true;
	}
	
/*
=================================================
	_CreateEvent
=================================================
*/
	bool CL2SyncManager::_CreateEvent (const Message< GpuMsg::CreateEvent > &msg)
	{
		for (;;)
		{
			++_counter;

			if ( not _syncs.IsExist( _counter ) )
				break;
		}
		_syncs.Add( _counter, SyncUnion_t{ cl_event(null) } );

		msg->result.Set( GpuEventId(_counter) );
		return true;
	}
	
/*
=================================================
	_DestroyEvent
=================================================
*/
	bool CL2SyncManager::_DestroyEvent (const Message< GpuMsg::DestroyEvent > &msg)
	{
		SyncArray_t::iterator	iter;

		if ( _syncs.Find( ulong(msg->id), OUT iter ) )
		{
			CHECK_ERR( iter->second.Is< cl_event >() );
			
			cl_event&	event = iter->second.Get< cl_event >();

			if ( event ) {
				CL_CALL( clReleaseEvent( event ) );
			}

			_syncs.EraseByIter( iter );
		}
		return true;
	}
	
/*
=================================================
	_GetCLEvent
=================================================
*/
	bool CL2SyncManager::_GetCLEvent (const Message< GpuMsg::GetCLEvent > &msg)
	{
		SyncArray_t::iterator	iter;
		CHECK_ERR( _syncs.Find( ulong(msg->eventId), OUT iter ) );
		CHECK_ERR( iter->second.Is< cl_event >() );
		
		cl_event &	event = iter->second.Get< cl_event >();

		if ( event == null )
		{
			cl_int		cl_err = 0;
			CL_CALL(( (event = clCreateUserEvent( GetContext(), OUT &cl_err )), cl_err ));
		}
		
		msg->result.Set( event );
		return true;
	}

/*
=================================================
	_SetEvent
=================================================
*/
	bool CL2SyncManager::_SetEvent (const Message< GpuMsg::SetEvent > &msg)
	{
		SyncArray_t::iterator	iter;
		CHECK_ERR( _syncs.Find( ulong(msg->id), OUT iter ) );
		CHECK_ERR( iter->second.Is< cl_event >() );

		cl_event&	event = iter->second.Get< cl_event >();

		// don't create event, it is same to make event signaled
		if ( event == null )
		{
			return true;
		}

		cl_int	status = 0;
		CL_CALL( clGetEventInfo( event, CL_EVENT_COMMAND_EXECUTION_STATUS, sizeof(status), OUT &status, null ) );

		if ( status != CL_COMPLETE )
		{
			// this command can be used once for event
			CL_CALL( clSetUserEventStatus( event, CL_COMPLETE ) );
		}
		return true;
	}
	
/*
=================================================
	_ResetEvent
=================================================
*/
	bool CL2SyncManager::_ResetEvent (const Message< GpuMsg::ResetEvent > &msg)
	{
		SyncArray_t::iterator	iter;
		CHECK_ERR( _syncs.Find( ulong(msg->id), OUT iter ) );
		CHECK_ERR( iter->second.Is< cl_event >() );

		cl_event&	event = iter->second.Get< cl_event >();

		// create event, it is same to make event unsignaled
		if ( event == null )
		{
			return true;
		}
		
		cl_int	status = 0;
		CL_CALL( clGetEventInfo( event, CL_EVENT_COMMAND_EXECUTION_STATUS, sizeof(status), OUT &status, null ) );
		
		// event is already unsignaled, 'ResetEvent' has no effect
		CHECK_ERR( status != CL_COMPLETE );
		return true;
	}
	
/*
=================================================
	_CreateSemaphore
=================================================
*/
	bool CL2SyncManager::_CreateSemaphore (const Message< GpuMsg::CreateSemaphore > &msg)
	{
		for (;;)
		{
			++_counter;

			if ( not _syncs.IsExist( _counter ) )
				break;
		}
		_syncs.Add( _counter, SyncUnion_t{ cl_event(null) } );

		msg->result.Set( GpuSemaphoreId(_counter) );
		return true;
	}
	
/*
=================================================
	_DestroySemaphore
=================================================
*/
	bool CL2SyncManager::_DestroySemaphore (const Message< GpuMsg::DestroySemaphore > &msg)
	{
		SyncArray_t::iterator	iter;

		if ( _syncs.Find( ulong(msg->id), OUT iter ) )
		{
			CHECK_ERR( iter->second.Is< cl_event >() );
			
			cl_event&	sem = iter->second.Get< cl_event >();

			if ( sem ) {
				CL_CALL( clReleaseEvent( sem ) );
			}

			_syncs.EraseByIter( iter );
		}
		return true;
	}
	
/*
=================================================
	_CLSemaphoreEnqueue
=================================================
*/
	bool CL2SyncManager::_CLSemaphoreEnqueue (const Message< GpuMsg::CLSemaphoreEnqueue > &msg)
	{
		SyncArray_t::iterator	iter;
		CHECK_ERR( _syncs.Find( ulong(msg->semId), OUT iter ) );
		CHECK_ERR( iter->second.Is< cl_event >() );

		cl_event&	sem = iter->second.Get< cl_event >();
		
		if ( sem ) {
			CL_CALL( clReleaseEvent( sem ) );
		}

		cl_int		cl_err = 0;
		CL_CALL(( (sem = clCreateUserEvent( GetContext(), OUT &cl_err )), cl_err ));

		CL_CALL( clEnqueueMarkerWithWaitList( GetCommandQueue(), 0, null, OUT &sem ) );

		msg->result.Set( sem );
		return true;
	}
	
/*
=================================================
	_WaitCLSemaphore
=================================================
*/
	bool CL2SyncManager::_WaitCLSemaphore (const Message< GpuMsg::WaitCLSemaphore > &msg)
	{
		using Semaphores_t	= FixedSizeArray< cl_event, GpuMsg::WaitCLSemaphore::Semaphores_t::MemoryContainer_t::SIZE >;

		Semaphores_t	ids;

		FOR( i, msg->semaphores )
		{
			SyncArray_t::iterator	iter;
			CHECK_ERR( _syncs.Find( ulong(msg->semaphores[i]), OUT iter ) );
			CHECK_ERR( iter->second.Is< cl_event >() );

			cl_event	sem = iter->second.Get< cl_event >();

			if ( sem ) {
				ids.PushBack( sem );
			}
		}

		CL_CALL( clEnqueueMarkerWithWaitList( GetCommandQueue(), ids.Count(), ids.RawPtr(), null ) );
		return true;
	}

}	// PlatformCL
//-----------------------------------------------------------------------------

namespace Platforms
{
	ModulePtr OpenCLObjectsConstructor::CreateCL2SyncManager (ModuleMsg::UntypedID_t id, GlobalSystemsRef gs, const CreateInfo::GpuSyncManager &ci)
	{
		return New< PlatformCL::CL2SyncManager >( id, gs, ci );
	}

}	// Platforms
}	// Engine

#endif	// COMPUTE_API_OPENCL
