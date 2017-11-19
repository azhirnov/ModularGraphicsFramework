// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Engine/Platforms/OpenGL/OpenGLContext.h"

#if defined( GRAPHICS_API_OPENGL )

namespace Engine
{
namespace Platforms
{
	
	const TypeIdList	OpenGLContext::_msgTypes{ UninitializedT< SupportedMessages_t >() };
	const TypeIdList	OpenGLContext::_eventTypes{ UninitializedT< SupportedEvents_t >() };

/*
=================================================
	constructor
=================================================
*/
	OpenGLContext::OpenGLContext (GlobalSystemsRef gs, const CreateInfo::GpuContext &ci) :
		Module( gs, ModuleConfig{ GLContextModuleID, 1 }, &_msgTypes, &_eventTypes ),
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
		_SubscribeOnMsg( this, &OpenGLContext::_GetGraphicsModules );
		
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
		CHECK_ERR( msg->module->GetSupportedMessages().HasAllTypes< GLThreadMsgList_t >() );
		CHECK_ERR( msg->module->GetSupportedEvents().HasAllTypes< GLThreadEventList_t >() );
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

		ModulePtr	module = msg->module.Lock();

		if ( not module )
			return false;

		ASSERT( _threads.IsExist( module ) );

		_threads.Erase( module );
		return true;
	}
		
/*
=================================================
	_GetGraphicsModules
=================================================
*/	
	bool OpenGLContext::_GetGraphicsModules (const Message< GpuMsg::GetGraphicsModules > &msg)
	{
		msg->compute.Set( GetComputeModules() );
		msg->graphics.Set( GetGraphicsModules() );
		return true;
	}
	
/*
=================================================
	GetGraphicsModules
=================================================
*/	
	GraphicsModuleIDs OpenGLContext::GetGraphicsModules ()
	{
		GraphicsModuleIDs	graphics;
		graphics.pipeline		= GLGraphicsPipelineModuleID;
		graphics.framebuffer	= GLFramebufferModuleID;
		graphics.renderPass		= GLRenderPassModuleID;
		graphics.context		= GLContextModuleID;
		graphics.thread			= GLThreadModuleID;
		graphics.commandBuffer	= GLCommandBufferModuleID;
		graphics.commandBuilder	= GLCommandBuilderModuleID;
		graphics.buffer			= GLBufferModuleID;
		graphics.image			= GLImageModuleID;
		graphics.sampler		= GLSamplerModuleID;
		graphics.memory			= GLMemoryModuleID;
		graphics.resourceTable	= GLPipelineResourceTableModuleID;
		graphics.uniforms		= GLUniformsModuleID;
		return graphics;
	}
	
/*
=================================================
	GetComputeModules
=================================================
*/	
	ComputeModuleIDs OpenGLContext::GetComputeModules ()
	{
		ComputeModuleIDs	compute;
		compute.pipeline		= GLComputePipelineModuleID;
		compute.context			= GLContextModuleID;
		compute.thread			= GLThreadModuleID;
		compute.commandBuffer	= GLCommandBufferModuleID;
		compute.commandBuilder	= GLCommandBuilderModuleID;
		compute.buffer			= GLBufferModuleID;
		compute.image			= GLImageModuleID;
		compute.memory			= GLMemoryModuleID;
		compute.resourceTable	= GLPipelineResourceTableModuleID;
		compute.uniforms		= GLUniformsModuleID;
		return compute;
	}

/*
=================================================
	Register
=================================================
*/
	void OpenGLContext::Register (GlobalSystemsRef gs)
	{
		auto	mf = gs->Get< ModulesFactory >();

		CHECK( mf->Register( GLContextModuleID, &_CreateOpenGLContext ) );
		CHECK( mf->Register( GLThreadModuleID, &_CreateOpenGLThread ) );
		
		CHECK( mf->Register( GLImageModuleID, &_CreateGL4Image ) );
		CHECK( mf->Register( GLMemoryModuleID, &_CreateGL4Memory ) );
		CHECK( mf->Register( GLBufferModuleID, &_CreateGL4Buffer ) );
		CHECK( mf->Register( GLSamplerModuleID, &_CreateGL4Sampler ) );
		CHECK( mf->Register( GLRenderPassModuleID, &_CreateGL4RenderPass ) );
		CHECK( mf->Register( GLFramebufferModuleID, &_CreateGL4Framebuffer ) );
		CHECK( mf->Register( GLCommandBufferModuleID, &_CreateGL4CommandBuffer ) );
		CHECK( mf->Register( GLCommandBuilderModuleID, &_CreateGL4CommandBuilder ) );
		CHECK( mf->Register( GLGraphicsPipelineModuleID, &_CreateGL4GraphicsPipeline ) );
		CHECK( mf->Register( GLPipelineResourceTableModuleID, &_CreateGL4PipelineResourceTable ) );

		if ( not mf->IsRegistered< CreateInfo::PipelineTemplate >( PipelineTemplateModuleID ) )
			CHECK( mf->Register( PipelineTemplateModuleID, &_CreatePipelineTemplate ) );
	}
	
/*
=================================================
	Unregister
=================================================
*/
	void OpenGLContext::Unregister (GlobalSystemsRef gs)
	{
		auto	mf = gs->Get< ModulesFactory >();

		mf->UnregisterAll( GLContextModuleID );
		mf->UnregisterAll( GLThreadModuleID );
		
		mf->UnregisterAll( GLImageModuleID );
		mf->UnregisterAll( GLMemoryModuleID );
		mf->UnregisterAll( GLBufferModuleID );
		mf->UnregisterAll( GLSamplerModuleID );
		mf->UnregisterAll( GLRenderPassModuleID );
		mf->UnregisterAll( GLFramebufferModuleID );
		mf->UnregisterAll( GLCommandBufferModuleID );
		mf->UnregisterAll( GLCommandBuilderModuleID );
		mf->UnregisterAll( GLGraphicsPipelineModuleID );
		mf->UnregisterAll( GLPipelineResourceTableModuleID );

		//mf->UnregisterAll< Platforms::PipelineTemplate >();	// TODO
	}

/*
=================================================
	_CreateOpenGLContext
=================================================
*/
	ModulePtr OpenGLContext::_CreateOpenGLContext (GlobalSystemsRef gs, const CreateInfo::GpuContext &ci)
	{
		return New< OpenGLContext >( gs, ci );
	}


}	// Platforms
}	// Engine

#endif	// GRAPHICS_API_OPENGL
