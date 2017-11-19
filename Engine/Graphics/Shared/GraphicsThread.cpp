// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Engine/Graphics/Shared/GraphicsContext.h"
#include "Engine/Platforms/Windows/WinMessages.h"

namespace Engine
{
namespace Graphics
{

	//
	// Graphics Thread
	//
	
	class GraphicsThread final : public Module
	{
	// types
	private:
		using SupportedMessages_t	= Module::SupportedMessages_t::Append< MessageListFrom<
											ModuleMsg::AddToManager,
											ModuleMsg::RemoveFromManager,
											ModuleMsg::OnManagerChanged
										> >;
		using SupportedEvents_t		= Module::SupportedEvents_t::Append< MessageListFrom<
											GpuMsg::DeviceCreated,
											GpuMsg::DeviceBeforeDestroy,
											OSMsg::WindowAfterDestroy
										> >;
		
		using WindowMsgList_t		= MessageListFrom< OSMsg::GetWinWindowHandle >;
		using WindowEventList_t		= MessageListFrom< OSMsg::WindowCreated, OSMsg::WindowBeforeDestroy, OSMsg::OnWinWindowRawMessage >;

		using GpuThreadMsgList_t	= MessageListFrom< GpuMsg::ThreadBeginFrame, GpuMsg::ThreadEndFrame, GpuMsg::GetGraphicsModules >;
		using GpuThreadEventList_t	= MessageListFrom< GpuMsg::DeviceCreated, GpuMsg::DeviceBeforeDestroy >;


	// constants
	private:
		static const TypeIdList		_msgTypes;
		static const TypeIdList		_eventTypes;

		
	// variables
	private:

		
	// methods
	public:
		GraphicsThread (GlobalSystemsRef gs, const CreateInfo::GraphicsThread &ci);
		~GraphicsThread ();

		
	// message handlers
	private:
		bool _Delete (const Message< ModuleMsg::Delete > &);
		bool _AddToManager (const Message< ModuleMsg::AddToManager > &);
		bool _RemoveFromManager (const Message< ModuleMsg::RemoveFromManager > &);
		
	// events
		bool _Update (const Message< ModuleMsg::Update > &);
		bool _GpuDeviceCreated (const Message< GpuMsg::DeviceCreated > &);
		bool _GpuDeviceBeforeDestroy (const Message< GpuMsg::DeviceBeforeDestroy > &);
		bool _WindowAfterDestroy (const Message< OSMsg::WindowAfterDestroy > &);

	private:
		bool _Initialize (const CreateInfo::GraphicsThread &ci);
	};
//-----------------------------------------------------------------------------



	const TypeIdList	GraphicsThread::_msgTypes{ UninitializedT< SupportedMessages_t >() };
	const TypeIdList	GraphicsThread::_eventTypes{ UninitializedT< SupportedEvents_t >() };
	
/*
=================================================
	constructor
=================================================
*/
	GraphicsThread::GraphicsThread (GlobalSystemsRef gs, const CreateInfo::GraphicsThread &ci) :
		Module( gs, ModuleConfig{ GraphicsThreadModuleID, 1 }, &_msgTypes, &_eventTypes )
	{
		SetDebugName( "GraphicsThread" );

		_SubscribeOnMsg( this, &GraphicsThread::_OnModuleAttached_Impl );
		_SubscribeOnMsg( this, &GraphicsThread::_OnModuleDetached_Impl );
		_SubscribeOnMsg( this, &GraphicsThread::_AttachModule_Impl );
		_SubscribeOnMsg( this, &GraphicsThread::_DetachModule_Impl );
		_SubscribeOnMsg( this, &GraphicsThread::_FindModule_Empty );
		_SubscribeOnMsg( this, &GraphicsThread::_ModulesDeepSearch_Empty );
		_SubscribeOnMsg( this, &GraphicsThread::_Update_Empty );
		_SubscribeOnMsg( this, &GraphicsThread::_Link_Impl );
		_SubscribeOnMsg( this, &GraphicsThread::_Compose_Impl );
		_SubscribeOnMsg( this, &GraphicsThread::_Delete );
		_SubscribeOnMsg( this, &GraphicsThread::_AddToManager );
		_SubscribeOnMsg( this, &GraphicsThread::_RemoveFromManager );
		_SubscribeOnMsg( this, &GraphicsThread::_OnManagerChanged_Empty );
		
		CHECK( _ValidateMsgSubscriptions() );

		_AttachSelfToManager( ci.context, GraphicsContextModuleID, false );
		
		CHECK( _Initialize( ci ) );
	}
	
/*
=================================================
	_Initialize
=================================================
*/
	bool GraphicsThread::_Initialize (const CreateInfo::GraphicsThread &ci)
	{
		auto	thread = GlobalSystems()->Get< ParallelThread >();
		CHECK_ERR( thread );

		if ( ci.window.IsDefined() )
		{
			thread->AddModule( 0, *ci.window );
			thread->AddModule( InputThreadModuleID, CreateInfo::InputThread{} );
			
			// TODO: use SearchModule message
			ModulePtr	window;
			CHECK_ATTACHMENT(( window = thread->GetModuleByMsgEvent< WindowMsgList_t, WindowEventList_t >() ));

			window->Subscribe( this, &GraphicsThread::_WindowAfterDestroy );

			//if ( ci.keyInputID )
			//	window->AddModule( ci.keyInputID, CreateInfo::RawInputHandler{} );

			//if ( ci.mouseInputID )
			//	window->AddModule( ci.mouseInputID, CreateInfo::RawInputHandler{} );
		}

		thread->AddModule( 0, ci.gpuThread );

		ModulePtr	gpu_thread;
		CHECK_ERR(( gpu_thread = thread->GetModuleByMsgEvent< GpuThreadMsgList_t, GpuThreadEventList_t >() ));
		
		gpu_thread->Subscribe( this, &GraphicsThread::_Update );
		gpu_thread->Subscribe( this, &GraphicsThread::_GpuDeviceCreated );
		gpu_thread->Subscribe( this, &GraphicsThread::_GpuDeviceBeforeDestroy );
		
		return true;
	}

/*
=================================================
	destructor
=================================================
*/
	GraphicsThread::~GraphicsThread ()
	{
		LOG( "GraphicsThread finalized", ELog::Debug );
	}
	
/*
=================================================
	_Delete
=================================================
*/
	bool GraphicsThread::_Delete (const Message< ModuleMsg::Delete > &msg)
	{
		auto	thread	= GlobalSystems()->Get< ParallelThread >();
		
		if ( thread )
		{
			auto	window		= thread->GetModuleByMsgEvent< WindowMsgList_t, WindowEventList_t >();
			auto	gpu_thread	= thread->GetModuleByMsgEvent< GpuThreadMsgList_t, GpuThreadEventList_t >();

			if ( window )
				window->UnsubscribeAll( this );

			if ( gpu_thread )
				gpu_thread->UnsubscribeAll( this );
		}
		return _Delete_Impl( msg );
	}

/*
=================================================
	_AddToManager
=================================================
*/
	bool GraphicsThread::_AddToManager (const Message< ModuleMsg::AddToManager > &)
	{
		return true;
	}
	
/*
=================================================
	_RemoveFromManager
=================================================
*/
	bool GraphicsThread::_RemoveFromManager (const Message< ModuleMsg::RemoveFromManager > &)
	{
		return true;
	}
	
/*
=================================================
	_RemoveFromManager
=================================================
*/
	bool GraphicsThread::_Update (const Message< ModuleMsg::Update > &msg)
	{
		return true;
	}
	
/*
=================================================
	_RemoveFromManager
=================================================
*/
	bool GraphicsThread::_GpuDeviceCreated (const Message< GpuMsg::DeviceCreated > &msg)
	{
		_SendEvent( msg );
		return true;
	}
	
/*
=================================================
	_RemoveFromManager
=================================================
*/
	bool GraphicsThread::_GpuDeviceBeforeDestroy (const Message< GpuMsg::DeviceBeforeDestroy > &msg)
	{
		_SendEvent( msg );
		return true;
	}
		
/*
=================================================
	_WindowAfterDestroy
=================================================
*/
	bool GraphicsThread::_WindowAfterDestroy (const Message< OSMsg::WindowAfterDestroy > &msg)
	{
		_SendEvent( msg );
		return true;
	}
//-----------------------------------------------------------------------------


	ModulePtr GraphicsContext::_CreateGraphicsThread (GlobalSystemsRef gs, const CreateInfo::GraphicsThread &ci)
	{
		return New< GraphicsThread >( gs, ci );
	}

}	// Graphics
}	// Engine
