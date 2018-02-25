// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Engine/Platforms/Shared/GPU/Context.h"
#include "Engine/Platforms/Shared/GPU/Thread.h"
#include "Engine/Platforms/Shared/GPU/Pipeline.h"
#include "Engine/Platforms/Shared/GPU/Buffer.h"
#include "Engine/Platforms/Shared/GPU/Framebuffer.h"
#include "Engine/Platforms/Shared/GPU/RenderPass.h"
#include "Engine/Platforms/Shared/GPU/Sampler.h"
#include "Engine/Platforms/Shared/GPU/Memory.h"
#include "Engine/Platforms/OpenGL/Impl/GL4Messages.h"
#include "Engine/Platforms/OpenGL/OpenGLObjectsConstructor.h"

#ifdef GRAPHICS_API_OPENGL

namespace Engine
{
namespace Platforms
{

	//
	// OpenGL Context
	//
	
	class OpenGLContext final : public Module
	{
	// types
	private:
		using SupportedMessages_t	= Module::SupportedMessages_t::Append< MessageListFrom<
											ModuleMsg::AddToManager,
											ModuleMsg::RemoveFromManager,
											GpuMsg::GetGraphicsModules
										> >;
		using SupportedEvents_t		= Module::SupportedEvents_t;

		using GLThreads_t			= Set< ModulePtr >;
		
		using GLThreadMsgList_t		= MessageListFrom< GpuMsg::ThreadBeginFrame, GpuMsg::ThreadEndFrame, GpuMsg::GetGLDeviceInfo >;
		using GLThreadEventList_t	= MessageListFrom< GpuMsg::DeviceCreated, GpuMsg::DeviceBeforeDestroy >;


	// constants
	private:
		static const TypeIdList		_msgTypes;
		static const TypeIdList		_eventTypes;

		
	// variables
	private:
		CreateInfo::GpuContext	_createInfo;

		GLThreads_t				_threads;
		

	// methods
	public:
		OpenGLContext (GlobalSystemsRef gs, const CreateInfo::GpuContext &ci);
		~OpenGLContext ();

		
	// message handlers
	private:
		bool _AddToManager (const Message< ModuleMsg::AddToManager > &);
		bool _RemoveFromManager (const Message< ModuleMsg::RemoveFromManager > &);
		bool _GetGraphicsModules (const Message< GpuMsg::GetGraphicsModules > &);
	};
//-----------------------------------------------------------------------------


	
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
		msg->compute.Set( OpenGLObjectsConstructor::GetComputeModules() );
		msg->graphics.Set( OpenGLObjectsConstructor::GetGraphicsModules() );
		return true;
	}
//-----------------------------------------------------------------------------


	
/*
=================================================
	GetGraphicsModules
=================================================
*/	
	GraphicsModuleIDs OpenGLObjectsConstructor::GetGraphicsModules ()
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
		return graphics;
	}
	
/*
=================================================
	GetComputeModules
=================================================
*/	
	ComputeModuleIDs OpenGLObjectsConstructor::GetComputeModules ()
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
		return compute;
	}

/*
=================================================
	Register
=================================================
*/
	void OpenGLObjectsConstructor::Register ()
	{
		auto	mf = GetMainSystemInstance()->GlobalSystems()->modulesFactory;

		CHECK( mf->Register( GLContextModuleID, &CreateOpenGLContext ) );
		CHECK( mf->Register( GLThreadModuleID, &CreateOpenGLThread ) );
		
		CHECK( mf->Register( GLImageModuleID, &CreateGL4Image ) );
		CHECK( mf->Register( GLMemoryModuleID, &CreateGL4Memory ) );
		CHECK( mf->Register( GLBufferModuleID, &CreateGL4Buffer ) );
		CHECK( mf->Register( GLSamplerModuleID, &CreateGL4Sampler ) );
		CHECK( mf->Register( GLRenderPassModuleID, &CreateGL4RenderPass ) );
		CHECK( mf->Register( GLFramebufferModuleID, &CreateGL4Framebuffer ) );
		CHECK( mf->Register( GLSyncManagerModuleID, &CreateGL4SyncManager ) );
		CHECK( mf->Register( GLCommandBufferModuleID, &CreateGL4CommandBuffer ) );
		CHECK( mf->Register( GLCommandBuilderModuleID, &CreateGL4CommandBuilder ) );
		CHECK( mf->Register( GLComputePipelineModuleID, &CreateGL4ComputePipeline ) );
		CHECK( mf->Register( GLGraphicsPipelineModuleID, &CreateGL4GraphicsPipeline ) );
		CHECK( mf->Register( GLPipelineResourceTableModuleID, &CreateGL4PipelineResourceTable ) );

		if ( not mf->IsRegistered< CreateInfo::PipelineTemplate >( PipelineTemplateModuleID ) )
			CHECK( mf->Register( PipelineTemplateModuleID, &CreatePipelineTemplate ) );
	}
	
/*
=================================================
	Unregister
=================================================
*/
	void OpenGLObjectsConstructor::Unregister ()
	{
		auto	mf = GetMainSystemInstance()->GlobalSystems()->modulesFactory;

		mf->UnregisterAll( GLContextModuleID );
		mf->UnregisterAll( GLThreadModuleID );
		
		mf->UnregisterAll( GLImageModuleID );
		mf->UnregisterAll( GLMemoryModuleID );
		mf->UnregisterAll( GLBufferModuleID );
		mf->UnregisterAll( GLSamplerModuleID );
		mf->UnregisterAll( GLRenderPassModuleID );
		mf->UnregisterAll( GLFramebufferModuleID );
		mf->UnregisterAll( GLSyncManagerModuleID );
		mf->UnregisterAll( GLCommandBufferModuleID );
		mf->UnregisterAll( GLCommandBuilderModuleID );
		mf->UnregisterAll( GLGraphicsPipelineModuleID );
		mf->UnregisterAll( GLPipelineResourceTableModuleID );

		//mf->UnregisterAll< Platforms::PipelineTemplate >();	// TODO
	}

/*
=================================================
	CreateOpenGLContext
=================================================
*/
	ModulePtr OpenGLObjectsConstructor::CreateOpenGLContext (GlobalSystemsRef gs, const CreateInfo::GpuContext &ci)
	{
		return New< OpenGLContext >( gs, ci );
	}


}	// Platforms
}	// Engine

#endif	// GRAPHICS_API_OPENGL
