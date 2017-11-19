// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Engine/Graphics/Shared/GraphicsContext.h"

namespace Engine
{
namespace Graphics
{

	const TypeIdList	GraphicsContext::_msgTypes{ UninitializedT< SupportedMessages_t >() };
	const TypeIdList	GraphicsContext::_eventTypes{ UninitializedT< SupportedEvents_t >() };
	
/*
=================================================
	constructor
=================================================
*/
	GraphicsContext::GraphicsContext (GlobalSystemsRef gs, const CreateInfo::GraphicsContext &ci) :
		Module( gs, ModuleConfig{ GraphicsContextModuleID, 1 }, &_msgTypes, &_eventTypes )
	{
		SetDebugName( "GraphicsContext" );

		_SubscribeOnMsg( this, &GraphicsContext::_OnModuleAttached_Impl );
		_SubscribeOnMsg( this, &GraphicsContext::_OnModuleDetached_Impl );
		_SubscribeOnMsg( this, &GraphicsContext::_AttachModule_Impl );
		_SubscribeOnMsg( this, &GraphicsContext::_DetachModule_Impl );
		_SubscribeOnMsg( this, &GraphicsContext::_FindModule_Empty );
		_SubscribeOnMsg( this, &GraphicsContext::_ModulesDeepSearch_Empty );
		_SubscribeOnMsg( this, &GraphicsContext::_Update_Empty );
		_SubscribeOnMsg( this, &GraphicsContext::_Link_Impl );
		_SubscribeOnMsg( this, &GraphicsContext::_Compose_Impl );
		_SubscribeOnMsg( this, &GraphicsContext::_Delete_Impl );
		_SubscribeOnMsg( this, &GraphicsContext::_AddToManager );
		_SubscribeOnMsg( this, &GraphicsContext::_RemoveFromManager );
		
		CHECK( _ValidateMsgSubscriptions() );
		
		auto	ms = GetMainSystemInstace();

		ms->AddModule( 0, CreateInfo::GpuContext{ ci.settings } );
	}
	
/*
=================================================
	destructor
=================================================
*/
	GraphicsContext::~GraphicsContext ()
	{
		LOG( "GraphicsContext finalized", ELog::Debug );
	}
	
/*
=================================================
	Register
=================================================
*/
	void GraphicsContext::Register (GlobalSystemsRef gs)
	{
		auto	mf = gs->Get< ModulesFactory >();

		//CHECK( mf->Register( FontModuleID, &_CreateFont ) );
		//CHECK( mf->Register( FontManagerModuleID, &_CreateFontManager ) );
		CHECK( mf->Register( BatchRendererModuleID, &_CreateBatchRenderer ) );
		CHECK( mf->Register( GraphicsThreadModuleID, &_CreateGraphicsThread ) );
		CHECK( mf->Register( GraphicsContextModuleID, &_CreateGraphicsContext ) );
	}
	
/*
=================================================
	Unregister
=================================================
*/
	void GraphicsContext::Unregister (GlobalSystemsRef gs)
	{
		auto	mf = gs->Get< ModulesFactory >();

		mf->UnregisterAll( FontModuleID );
		mf->UnregisterAll( FontManagerModuleID );
		mf->UnregisterAll( BatchRendererModuleID );
		mf->UnregisterAll( GraphicsThreadModuleID );
		mf->UnregisterAll( GraphicsContextModuleID );
	}
	
/*
=================================================
	_AddToManager
=================================================
*/
	bool GraphicsContext::_AddToManager (const Message< ModuleMsg::AddToManager > &msg)
	{
		CHECK_ERR( msg->module );
		CHECK_ERR( msg->module->GetModuleID() == GraphicsThreadModuleID );	// TODO: check messages instead of ID
		ASSERT( not _threads.IsExist( msg->module ) );

		_threads.Add( msg->module );
		return true;
	}
	
/*
=================================================
	_RemoveFromManager
=================================================
*/
	bool GraphicsContext::_RemoveFromManager (const Message< ModuleMsg::RemoveFromManager > &msg)
	{
		CHECK_ERR( msg->module );

		ModulePtr	module = msg->module.Lock();

		if ( not module )
			return false;

		CHECK_ERR( module->GetModuleID() == GraphicsThreadModuleID );	// TODO: check messages instead of ID
		ASSERT( _threads.IsExist( module ) );

		_threads.Erase( module );
		return true;
	}
	
/*
=================================================
	_CreateGraphicsContext
=================================================
*/
	ModulePtr GraphicsContext::_CreateGraphicsContext (GlobalSystemsRef gs, const CreateInfo::GraphicsContext &ci)
	{
		return New< GraphicsContext >( gs, ci );
	}

}	// Graphics
}	// Engine
