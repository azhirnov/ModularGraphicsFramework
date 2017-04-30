// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Engine/Platforms/OpenGL/Impl/GL4Texture.h"

#if defined( GRAPHICS_API_OPENGL )

namespace Engine
{
namespace PlatformGL
{
	
	const Runtime::VirtualTypeList	GL4Texture::_msgTypes{ UninitializedT< SupportedMessages_t >() };
	const Runtime::VirtualTypeList	GL4Texture::_eventTypes{ UninitializedT< SupportedEvents_t >() };

/*
=================================================
	constructor
=================================================
*/
	GL4Texture::GL4Texture (const GlobalSystemsRef gs, const CreateInfo::GpuTexture &ci) :
		GL4BaseModule( gs, GLSystemsRef(null), GetStaticID(), &_msgTypes, &_eventTypes ),
		_descr( ci.descr ),
		_texID( 0 )
	{
		_SubscribeOnMsg( this, &GL4Texture::_OnModuleAttached_Impl );
		_SubscribeOnMsg( this, &GL4Texture::_OnModuleDetached_Impl );
		_SubscribeOnMsg( this, &GL4Texture::_AttachModule_Impl );
		_SubscribeOnMsg( this, &GL4Texture::_DetachModule_Impl );
		_SubscribeOnMsg( this, &GL4Texture::_OnManagerChanged_Empty );
		_SubscribeOnMsg( this, &GL4Texture::_FindModule_Impl );
		_SubscribeOnMsg( this, &GL4Texture::_ModulesDeepSearch_Impl );
		_SubscribeOnMsg( this, &GL4Texture::_Update_Impl );
		_SubscribeOnMsg( this, &GL4Texture::_Link_Impl );
		_SubscribeOnMsg( this, &GL4Texture::_Delete_Impl );
		
		CHECK( _ValidateMsgSubscriptions() );
		
		//_AttachSelfToManager( ci.gpuThread, OpenGLThread::GetStaticID(), true );

		Send( Message< ModuleMsg::AttachModule >{ this, ci.dataSource } );
	}
	
/*
=================================================
	destructor
=================================================
*/
	GL4Texture::~GL4Texture ()
	{
		ASSERT( _texID == 0 );
	}


}	// PlatformGL
}	// Engine

#endif	// GRAPHICS_API_OPENGL
