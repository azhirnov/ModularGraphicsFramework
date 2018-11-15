// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Core/Config/Engine.Config.h"

#ifdef COMPUTE_API_OPENCL

#include "Engine/Platforms/Public/GPU/Sync.h"
#include "Engine/Platforms/OpenCL/120/CL1BaseModule.h"
#include "Engine/Platforms/OpenCL/OpenCLObjectsConstructor.h"

namespace Engine
{
namespace PlatformCL
{
	using namespace cl;

	
	//
	// OpenCL Sync Manager
	//

	class CL1SyncManager final : public CL1BaseModule
	{
	// types
	private:
		using SupportedMessages_t	= CL1BaseModule::SupportedMessages_t::Append< MessageListFrom<
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

		using SupportedEvents_t		= CL1BaseModule::SupportedEvents_t;

		using SyncUnion_t			= Union< cl_event >;
		using SyncArray_t			= Map< ulong, SyncUnion_t >;

		struct _DeleteSync_Func;


	// constants
	private:
		static const TypeIdList		_eventTypes;


	// variables
	private:
		SyncArray_t		_syncs;
		ulong			_counter;


	// methods
	public:
		CL1SyncManager (UntypedID_t, GlobalSystemsRef gs, const CreateInfo::GpuSyncManager &ci);
		~CL1SyncManager ();


	// message handlers
	private:
		bool _Delete (const ModuleMsg::Delete &);
		bool _OnManagerChanged2 (const ModuleMsg::OnManagerChanged &msg);

		bool _ClientWaitDeviceIdle (const GpuMsg::ClientWaitDeviceIdle &);
		bool _CreateFence (const GpuMsg::CreateFence &);
		bool _DestroyFence (const GpuMsg::DestroyFence &);
		bool _CLFenceSync (const GpuMsg::CLFenceSync &);
		bool _ClientWaitFence (const GpuMsg::ClientWaitFence &);
		bool _CreateEvent (const GpuMsg::CreateEvent &);
		bool _DestroyEvent (const GpuMsg::DestroyEvent &);
		bool _GetCLEvent (const GpuMsg::GetCLEvent &);
		bool _SetEvent (const GpuMsg::SetEvent &);
		bool _ResetEvent (const GpuMsg::ResetEvent &);
		bool _CreateSemaphore (const GpuMsg::CreateSemaphore &);
		bool _DestroySemaphore (const GpuMsg::DestroySemaphore &);
		bool _CLSemaphoreEnqueue (const GpuMsg::CLSemaphoreEnqueue &);
		bool _WaitCLSemaphore (const GpuMsg::WaitCLSemaphore &);
	};
//-----------------------------------------------------------------------------


	
	const TypeIdList	CL1SyncManager::_eventTypes{ UninitializedT< SupportedEvents_t >() };

/*
=================================================
	constructor
=================================================
*/
	CL1SyncManager::CL1SyncManager (UntypedID_t id, GlobalSystemsRef gs, const CreateInfo::GpuSyncManager &ci) :
		CL1BaseModule( gs, ModuleConfig{ id, 1 }, &_eventTypes ),
		_counter{ 0 }
	{
		SetDebugName( "CL1SyncManager" );

		_SubscribeOnMsg( this, &CL1SyncManager::_OnModuleAttached_Impl );
		_SubscribeOnMsg( this, &CL1SyncManager::_OnModuleDetached_Impl );
		_SubscribeOnMsg( this, &CL1SyncManager::_AttachModule_Empty );
		_SubscribeOnMsg( this, &CL1SyncManager::_DetachModule_Empty );
		_SubscribeOnMsg( this, &CL1SyncManager::_FindModule_Impl );
		_SubscribeOnMsg( this, &CL1SyncManager::_ModulesDeepSearch_Impl );
		_SubscribeOnMsg( this, &CL1SyncManager::_Link_Impl );
		_SubscribeOnMsg( this, &CL1SyncManager::_Compose_Impl );
		_SubscribeOnMsg( this, &CL1SyncManager::_Delete );
		_SubscribeOnMsg( this, &CL1SyncManager::_OnManagerChanged2 );
		_SubscribeOnMsg( this, &CL1SyncManager::_GetDeviceInfo );
		_SubscribeOnMsg( this, &CL1SyncManager::_GetCLDeviceInfo );
		_SubscribeOnMsg( this, &CL1SyncManager::_GetCLPrivateClasses );
		_SubscribeOnMsg( this, &CL1SyncManager::_ClientWaitDeviceIdle );
		_SubscribeOnMsg( this, &CL1SyncManager::_CreateFence );
		_SubscribeOnMsg( this, &CL1SyncManager::_DestroyFence );
		_SubscribeOnMsg( this, &CL1SyncManager::_CLFenceSync );
		_SubscribeOnMsg( this, &CL1SyncManager::_ClientWaitFence );
		_SubscribeOnMsg( this, &CL1SyncManager::_CreateEvent );
		_SubscribeOnMsg( this, &CL1SyncManager::_DestroyEvent );
		_SubscribeOnMsg( this, &CL1SyncManager::_GetCLEvent );
		_SubscribeOnMsg( this, &CL1SyncManager::_SetEvent );
		_SubscribeOnMsg( this, &CL1SyncManager::_ResetEvent );
		_SubscribeOnMsg( this, &CL1SyncManager::_CreateSemaphore );
		_SubscribeOnMsg( this, &CL1SyncManager::_DestroySemaphore );
		_SubscribeOnMsg( this, &CL1SyncManager::_CLSemaphoreEnqueue );
		_SubscribeOnMsg( this, &CL1SyncManager::_WaitCLSemaphore );

		ASSERT( _ValidateMsgSubscriptions< SupportedMessages_t >() );

		_AttachSelfToManager( _GetGPUThread( ci.gpuThread ), UntypedID_t(0), true );
	}
	
/*
=================================================
	destructor
=================================================
*/
	CL1SyncManager::~CL1SyncManager ()
	{
		ASSERT( _syncs.Empty() );
	}
	
/*
=================================================
	_DeleteSync_Func
=================================================
*/
	struct CL1SyncManager::_DeleteSync_Func
	{
		_DeleteSync_Func ()
		{}

		void operator () (cl_event ev) const
		{
			if ( ev != null ) {
				CL_CALL( clReleaseEvent( ev ) );
			}
		}
	};

/*
=================================================
	_Delete
=================================================
*/
	bool CL1SyncManager::_Delete (const ModuleMsg::Delete &msg)
	{
		_DeleteSync_Func	func;

		FOR( i, _syncs ) {
			_syncs[i].second.Accept( func );
		}

		_syncs.Clear();

		return Module::_Delete_Impl( msg );
	}
	
/*
=================================================
	_OnManagerChanged2
=================================================
*/
	bool CL1SyncManager::_OnManagerChanged2 (const ModuleMsg::OnManagerChanged &msg)
	{
		_OnManagerChanged( msg );
		
		if ( msg.newManager )
		{
			msg.newManager->UnsubscribeAll( this );
		}
		return true;
	}
	
/*
=================================================
	_ClientWaitDeviceIdle
=================================================
*/
	bool CL1SyncManager::_ClientWaitDeviceIdle (const GpuMsg::ClientWaitDeviceIdle &)
	{
		CL_CALL( clFinish( GetCommandQueue() ) );
		return true;
	}

/*
=================================================
	_CreateFence
=================================================
*/
	bool CL1SyncManager::_CreateFence (const GpuMsg::CreateFence &msg)
	{
		for (;;)
		{
			++_counter;

			if ( not _syncs.IsExist( _counter ) )
				break;
		}
		_syncs.Add( _counter, SyncUnion_t{ cl_event(null) } );

		msg.result.Set( GpuFenceId(_counter) );
		return true;
	}
	
/*
=================================================
	_DestroyFence
=================================================
*/
	bool CL1SyncManager::_DestroyFence (const GpuMsg::DestroyFence &msg)
	{
		SyncArray_t::iterator	iter;

		if ( _syncs.Find( ulong(msg.id), OUT iter ) )
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
	bool CL1SyncManager::_CLFenceSync (const GpuMsg::CLFenceSync &msg)
	{
		SyncArray_t::iterator	iter;
		CHECK_ERR( _syncs.Find( ulong(msg.fenceId), OUT iter ) );
		CHECK_ERR( iter->second.Is< cl_event >() );
		
		cl_event&	fence = iter->second.Get< cl_event >();

		if ( fence ) {
			CL_CALL( clReleaseEvent( fence ) );
		}
		
		cl_int		cl_err = 0;
		CL_CALL(( (fence = clCreateUserEvent( GetContext(), OUT &cl_err )), cl_err ));

		CL_CALL( clEnqueueMarkerWithWaitList( GetCommandQueue(), 0, null, OUT &fence ) );

		msg.result.Set( fence );
		return true;
	}

/*
=================================================
	_ClientWaitFence
=================================================
*/
	bool CL1SyncManager::_ClientWaitFence (const GpuMsg::ClientWaitFence &msg)
	{
		using CLEvents_t	= FixedSizeArray< cl_event, GpuMsg::ClientWaitFence::Fences_t::MemoryContainer_t::SIZE >;

		CLEvents_t	events;

		for (auto& wfence : msg.fences)
		{
			SyncArray_t::iterator	iter;
			CHECK_ERR( _syncs.Find( ulong(wfence), OUT iter ) );

			cl_event	fence = iter->second.Get< cl_event >();
			CHECK_ERR( fence );

			events.PushBack( fence );
		}
		
		if ( not events.Empty() ) {
			CL_CALL( clWaitForEvents( cl_uint(events.Count()), events.RawPtr() ) );
		}
		return true;
	}
	
/*
=================================================
	_CreateEvent
=================================================
*/
	bool CL1SyncManager::_CreateEvent (const GpuMsg::CreateEvent &msg)
	{
		for (;;)
		{
			++_counter;

			if ( not _syncs.IsExist( _counter ) )
				break;
		}
		_syncs.Add( _counter, SyncUnion_t{ cl_event(null) } );

		msg.result.Set( GpuEventId(_counter) );
		return true;
	}
	
/*
=================================================
	_DestroyEvent
=================================================
*/
	bool CL1SyncManager::_DestroyEvent (const GpuMsg::DestroyEvent &msg)
	{
		SyncArray_t::iterator	iter;

		if ( _syncs.Find( ulong(msg.id), OUT iter ) )
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
	bool CL1SyncManager::_GetCLEvent (const GpuMsg::GetCLEvent &msg)
	{
		SyncArray_t::iterator	iter;
		CHECK_ERR( _syncs.Find( ulong(msg.eventId), OUT iter ) );
		CHECK_ERR( iter->second.Is< cl_event >() );
		
		cl_event &	event = iter->second.Get< cl_event >();

		if ( event == null )
		{
			cl_int		cl_err = 0;
			CL_CALL(( (event = clCreateUserEvent( GetContext(), OUT &cl_err )), cl_err ));
		}
		
		msg.result.Set( event );
		return true;
	}

/*
=================================================
	_SetEvent
=================================================
*/
	bool CL1SyncManager::_SetEvent (const GpuMsg::SetEvent &msg)
	{
		SyncArray_t::iterator	iter;
		CHECK_ERR( _syncs.Find( ulong(msg.id), OUT iter ) );
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
	bool CL1SyncManager::_ResetEvent (const GpuMsg::ResetEvent &msg)
	{
		SyncArray_t::iterator	iter;
		CHECK_ERR( _syncs.Find( ulong(msg.id), OUT iter ) );
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
	bool CL1SyncManager::_CreateSemaphore (const GpuMsg::CreateSemaphore &msg)
	{
		for (;;)
		{
			++_counter;

			if ( not _syncs.IsExist( _counter ) )
				break;
		}
		_syncs.Add( _counter, SyncUnion_t{ cl_event(null) } );

		msg.result.Set( GpuSemaphoreId(_counter) );
		return true;
	}
	
/*
=================================================
	_DestroySemaphore
=================================================
*/
	bool CL1SyncManager::_DestroySemaphore (const GpuMsg::DestroySemaphore &msg)
	{
		SyncArray_t::iterator	iter;

		if ( _syncs.Find( ulong(msg.id), OUT iter ) )
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
	bool CL1SyncManager::_CLSemaphoreEnqueue (const GpuMsg::CLSemaphoreEnqueue &msg)
	{
		SyncArray_t::iterator	iter;
		CHECK_ERR( _syncs.Find( ulong(msg.semId), OUT iter ) );
		CHECK_ERR( iter->second.Is< cl_event >() );

		cl_event&	sem = iter->second.Get< cl_event >();
		
		if ( sem ) {
			CL_CALL( clReleaseEvent( sem ) );
		}

		cl_int		cl_err = 0;
		CL_CALL(( (sem = clCreateUserEvent( GetContext(), OUT &cl_err )), cl_err ));

		CL_CALL( clEnqueueMarkerWithWaitList( GetCommandQueue(), 0, null, OUT &sem ) );

		msg.result.Set( sem );
		return true;
	}
	
/*
=================================================
	_WaitCLSemaphore
=================================================
*/
	bool CL1SyncManager::_WaitCLSemaphore (const GpuMsg::WaitCLSemaphore &msg)
	{
		using Semaphores_t	= FixedSizeArray< cl_event, GpuMsg::WaitCLSemaphore::Semaphores_t::MemoryContainer_t::SIZE >;

		Semaphores_t	ids;

		for (auto& wsem : msg.semaphores)
		{
			SyncArray_t::iterator	iter;
			CHECK_ERR( _syncs.Find( ulong(wsem), OUT iter ) );
			CHECK_ERR( iter->second.Is< cl_event >() );

			cl_event	sem = iter->second.Get< cl_event >();
			CHECK_ERR( sem );

			ids.PushBack( sem );
		}

		if ( not ids.Empty() ) {
			CL_CALL( clEnqueueMarkerWithWaitList( GetCommandQueue(), cl_uint(ids.Count()), ids.RawPtr(), null ) );
		}
		return true;
	}

}	// PlatformCL
//-----------------------------------------------------------------------------

namespace Platforms
{
	ModulePtr OpenCLObjectsConstructor::CreateCL1SyncManager (ModuleMsg::UntypedID_t id, GlobalSystemsRef gs, const CreateInfo::GpuSyncManager &ci)
	{
		return New< PlatformCL::CL1SyncManager >( id, gs, ci );
	}

}	// Platforms
}	// Engine

#endif	// COMPUTE_API_OPENCL
