// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Engine/Platforms/OpenCL/OpenCLContext.h"

#if defined( COMPUTE_API_OPENCL )

namespace Engine
{
namespace Platforms
{
	
	const TypeIdList	OpenCLContext::_msgTypes{ UninitializedT< SupportedMessages_t >() };
	const TypeIdList	OpenCLContext::_eventTypes{ UninitializedT< SupportedEvents_t >() };

/*
=================================================
	constructor
=================================================
*/
	OpenCLContext::OpenCLContext (GlobalSystemsRef gs, const CreateInfo::GpuContext &ci) :
		Module( gs, ModuleConfig{ CLContextModuleID, 1 }, &_msgTypes, &_eventTypes ),
		_createInfo( ci )
	{
		SetDebugName( "OpenCLContext" );

		_SubscribeOnMsg( this, &OpenCLContext::_OnModuleAttached_Impl );
		_SubscribeOnMsg( this, &OpenCLContext::_OnModuleDetached_Impl );
		_SubscribeOnMsg( this, &OpenCLContext::_AttachModule_Impl );
		_SubscribeOnMsg( this, &OpenCLContext::_DetachModule_Impl );
		_SubscribeOnMsg( this, &OpenCLContext::_FindModule_Empty );
		_SubscribeOnMsg( this, &OpenCLContext::_ModulesDeepSearch_Empty );
		_SubscribeOnMsg( this, &OpenCLContext::_Update_Empty );
		_SubscribeOnMsg( this, &OpenCLContext::_Link_Impl );
		_SubscribeOnMsg( this, &OpenCLContext::_Compose_Impl );
		_SubscribeOnMsg( this, &OpenCLContext::_Delete_Impl );
		_SubscribeOnMsg( this, &OpenCLContext::_AddToManager );
		_SubscribeOnMsg( this, &OpenCLContext::_RemoveFromManager );
		
		CHECK( _ValidateMsgSubscriptions() );
	}
	
/*
=================================================
	destructor
=================================================
*/
	OpenCLContext::~OpenCLContext ()
	{
		LOG( "OpenCLContext finalized", ELog::Debug );

		ASSERT( _threads.Empty() );
	}
	
/*
=================================================
	_AddToManager
=================================================
*/
	bool OpenCLContext::_AddToManager (const Message< ModuleMsg::AddToManager > &msg)
	{
		CHECK_ERR( msg->module );
		CHECK_ERR( msg->module->GetSupportedMessages().HasAllTypes< CLThreadMsgList_t >() );
		CHECK_ERR( msg->module->GetSupportedEvents().HasAllTypes< CLThreadEventList_t >() );
		ASSERT( not _threads.IsExist( msg->module ) );

		_threads.Add( msg->module );
		return true;
	}
	
/*
=================================================
	_RemoveFromManager
=================================================
*/
	bool OpenCLContext::_RemoveFromManager (const Message< ModuleMsg::RemoveFromManager > &msg)
	{
		CHECK_ERR( msg->module );
		ASSERT( _threads.IsExist( msg->module ) );

		_threads.Erase( msg->module );
		return true;
	}
		
/*
=================================================
	Register
=================================================
*/
	void OpenCLContext::Register (GlobalSystemsRef gs)
	{
		auto	mf = gs->Get< ModulesFactory >();

		CHECK( mf->Register( CLThreadModuleID, &_CreateOpenCLThread ) );
		CHECK( mf->Register( CLContextModuleID, &_CreateOpenCLContext ) );

		CHECK( mf->Register( CLImageModuleID, &_CreateCL2Image ) );
		CHECK( mf->Register( CLMemoryModuleID, &_CreateCL2Memory ) );
		CHECK( mf->Register( CLBufferModuleID, &_CreateCL2Buffer ) );
		//CHECK( mf->Register( CLSamplerModuleID, &_CreateCL2Sampler ) );
		CHECK( mf->Register( CLCommandBufferModuleID, &_CreateCL2CommandBuffer ) );
		CHECK( mf->Register( CLCommandBuilderModuleID, &_CreateCL2CommandBuilder ) );
		CHECK( mf->Register( CLComputePipelineModuleID, &_CreateCL2ComputePipeline ) );
		CHECK( mf->Register( CLPipelineResourceTableModuleID, &_CreateCL2PipelineResourceTable ) );

		if ( not mf->IsRegistered< CreateInfo::PipelineTemplate >( PipelineTemplateModuleID ) )
			CHECK( mf->Register( PipelineTemplateModuleID, &_CreatePipelineTemplate ) );
	}
	
/*
=================================================
	Unregister
=================================================
*/
	void OpenCLContext::Unregister (GlobalSystemsRef gs)
	{
		auto	mf = gs->Get< ModulesFactory >();

		mf->UnregisterAll( CLThreadModuleID );
		mf->UnregisterAll( CLContextModuleID );

		mf->UnregisterAll( CLImageModuleID );
		mf->UnregisterAll( CLMemoryModuleID );
		mf->UnregisterAll( CLBufferModuleID );
		mf->UnregisterAll( CLSamplerModuleID );
		mf->UnregisterAll( CLCommandBufferModuleID );
		mf->UnregisterAll( CLCommandBuilderModuleID );
		mf->UnregisterAll( CLComputePipelineModuleID );
		mf->UnregisterAll( CLPipelineResourceTableModuleID );

		//mf->UnregisterAll< Platforms::PipelineTemplate >();	// TODO
	}

/*
=================================================
	_CreateOpenCLContext
=================================================
*/
	ModulePtr OpenCLContext:: _CreateOpenCLContext (GlobalSystemsRef gs, const CreateInfo::GpuContext &ci)
	{
		return New< OpenCLContext >( gs, ci );
	}

}	// Platforms
}	// Engine

#endif	// COMPUTE_API_OPENCL
