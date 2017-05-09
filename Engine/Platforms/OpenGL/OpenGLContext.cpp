// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Engine/Platforms/OpenGL/OpenGLContext.h"

#if defined( GRAPHICS_API_OPENGL )

namespace Engine
{
namespace Platforms
{
	
	const Runtime::VirtualTypeList	OpenGLContext::_msgTypes{ UninitializedT< SupportedMessages_t >() };
	const Runtime::VirtualTypeList	OpenGLContext::_eventTypes{ UninitializedT< SupportedEvents_t >() };

/*
=================================================
	constructor
=================================================
*/
	OpenGLContext::OpenGLContext (const GlobalSystemsRef gs, const CreateInfo::GpuContext &ci) :
		Module( gs, ModuleConfig{ GetStaticID(), 1 }, &_msgTypes, &_eventTypes ),
		_createInfo( ci )
	{
		SetDebugName( "OpenGLContext" );

		_SubscribeOnMsg( this, &OpenGLContext::_OnModuleAttached_Impl );
		_SubscribeOnMsg( this, &OpenGLContext::_OnModuleDetached_Impl );
		_SubscribeOnMsg( this, &OpenGLContext::_AttachModule_Impl );
		_SubscribeOnMsg( this, &OpenGLContext::_DetachModule_Impl );
		_SubscribeOnMsg( this, &OpenGLContext::_FindModule_Empty );
		_SubscribeOnMsg( this, &OpenGLContext::_ModulesDeepSearch_Empty );
		_SubscribeOnMsg( this, &OpenGLContext::_Update_Empty );
		_SubscribeOnMsg( this, &OpenGLContext::_Link_Impl );
		_SubscribeOnMsg( this, &OpenGLContext::_Compose_Impl );
		_SubscribeOnMsg( this, &OpenGLContext::_Delete_Impl );
		_SubscribeOnMsg( this, &OpenGLContext::_AddToManager );
		_SubscribeOnMsg( this, &OpenGLContext::_RemoveFromManager );
		
		CHECK( _ValidateMsgSubscriptions() );
	}
	
/*
=================================================
	destructor
=================================================
*/
	OpenGLContext::~OpenGLContext ()
	{
		LOG( "OpenGLContext finalized", ELog::Debug );

		ASSERT( _threads.Empty() );
	}
	
/*
=================================================
	_AddToManager
=================================================
*/
	bool OpenGLContext::_AddToManager (const Message< ModuleMsg::AddToManager > &msg)
	{
		CHECK_ERR( msg->module );
		CHECK_ERR( msg->module->GetModuleID() == OpenGLThread::GetStaticID() );
		ASSERT( not _threads.IsExist( msg->module ) );

		_threads.Add( msg->module );
		return true;
	}
	
/*
=================================================
	_RemoveFromManager
=================================================
*/
	bool OpenGLContext::_RemoveFromManager (const Message< ModuleMsg::RemoveFromManager > &msg)
	{
		CHECK_ERR( msg->module );
		CHECK_ERR( msg->module->GetModuleID() == OpenGLThread::GetStaticID() );
		ASSERT( _threads.IsExist( msg->module ) );

		_threads.Erase( msg->module );
		return true;
	}
		
/*
=================================================
	Register
=================================================
*/
	void OpenGLContext::Register (const GlobalSystemsRef gs)
	{
		auto	mf = gs->Get< ModulesFactory >();

		CHECK( mf->Register( OpenGLContext::GetStaticID(), &_CreateOpenGLContext ) );
		CHECK( mf->Register( OpenGLThread::GetStaticID(), &_CreateOpenGLThread ) );
	}
	
/*
=================================================
	Unregister
=================================================
*/
	void OpenGLContext::Unregister (const GlobalSystemsRef gs)
	{
		auto	mf = gs->Get< ModulesFactory >();

		mf->UnregisterAll< OpenGLContext >();
		mf->UnregisterAll< OpenGLThread >();
	}

/*
=================================================
	_CreateOpenGLThread
=================================================
*/
	ModulePtr OpenGLContext::_CreateOpenGLThread (const GlobalSystemsRef gs, const CreateInfo::GpuThread &ci)
	{
		return GXTypes::New< OpenGLThread >( gs, ci );
	}
	
/*
=================================================
	_CreateOpenGLContext
=================================================
*/
	ModulePtr OpenGLContext:: _CreateOpenGLContext (const GlobalSystemsRef gs, const CreateInfo::GpuContext &ci)
	{
		return GXTypes::New< OpenGLContext >( gs, ci );
	}


}	// Platforms
}	// Engine

#endif	// GRAPHICS_API_OPENGL
