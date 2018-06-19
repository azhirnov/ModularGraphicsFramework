// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Core/Config/Engine.Config.h"

#ifdef COMPUTE_API_OPENCL

#include "Engine/Platforms/Public/GPU/Context.h"
#include "Engine/Platforms/Public/GPU/Buffer.h"
#include "Engine/Platforms/Public/GPU/Image.h"
#include "Engine/Platforms/Public/GPU/Thread.h"
#include "Engine/Platforms/Public/GPU/Pipeline.h"
#include "Engine/Platforms/Public/GPU/Sampler.h"
#include "Engine/Platforms/OpenCL/120/CL1Messages.h"
#include "Engine/Platforms/OpenCL/OpenCLObjectsConstructor.h"

namespace Engine
{
namespace Platforms
{

	//
	// OpenCL Context
	//
	
	class OpenCLContext final : public Module
	{
	// types
	private:
		using SupportedMessages_t	= Module::SupportedMessages_t::Append< MessageListFrom<
											ModuleMsg::AddToManager,
											ModuleMsg::RemoveFromManager,
											GpuMsg::GetGraphicsModules
										> >;
		using SupportedEvents_t		= Module::SupportedEvents_t;

		using CLThreads_t			= Set< ModulePtr >;
		
		using CLThreadMsgList_t		= MessageListFrom< GpuMsg::SubmitComputeQueueCommands, GpuMsg::GetCLDeviceInfo >;
		using CLThreadEventList_t	= MessageListFrom< GpuMsg::DeviceCreated, GpuMsg::DeviceBeforeDestroy >;


	// constants
	private:
		static const TypeIdList		_msgTypes;
		static const TypeIdList		_eventTypes;

		
	// variables
	private:
		CreateInfo::GpuContext	_createInfo;

		CLThreads_t				_threads;
		

	// methods
	public:
		OpenCLContext (UntypedID_t, GlobalSystemsRef gs, const CreateInfo::GpuContext &ci);
		~OpenCLContext ();

		
	// message handlers
	private:
		bool _AddToManager (const ModuleMsg::AddToManager &);
		bool _RemoveFromManager (const ModuleMsg::RemoveFromManager &);
		bool _GetGraphicsModules (const GpuMsg::GetGraphicsModules &);
		
	private:
		static ModulePtr _CreateOpenCLThread (GlobalSystemsRef, const CreateInfo::GpuThread &);
		static ModulePtr _CreateOpenCLContext (GlobalSystemsRef, const CreateInfo::GpuContext &);
		static ModulePtr _CreateCL1Image (GlobalSystemsRef, const CreateInfo::GpuImage &);
		static ModulePtr _CreateCL1Memory (GlobalSystemsRef, const CreateInfo::GpuMemory &);
		static ModulePtr _CreateCL1Buffer (GlobalSystemsRef, const CreateInfo::GpuBuffer &);
		static ModulePtr _CreateCL1Sampler (GlobalSystemsRef, const CreateInfo::GpuSampler &);
		static ModulePtr _CreatePipelineTemplate (GlobalSystemsRef, const CreateInfo::PipelineTemplate &);
		static ModulePtr _CreateCL1CommandBuffer (GlobalSystemsRef, const CreateInfo::GpuCommandBuffer &);
		static ModulePtr _CreateCL1CommandBuilder (GlobalSystemsRef, const CreateInfo::GpuCommandBuilder &);
		static ModulePtr _CreateCL1ComputePipeline (GlobalSystemsRef, const CreateInfo::ComputePipeline &);
		static ModulePtr _CreateCL1PipelineResourceTable (GlobalSystemsRef, const CreateInfo::PipelineResourceTable &);
	};
//-----------------------------------------------------------------------------



	const TypeIdList	OpenCLContext::_msgTypes{ UninitializedT< SupportedMessages_t >() };
	const TypeIdList	OpenCLContext::_eventTypes{ UninitializedT< SupportedEvents_t >() };

/*
=================================================
	constructor
=================================================
*/
	OpenCLContext::OpenCLContext (UntypedID_t id, GlobalSystemsRef gs, const CreateInfo::GpuContext &ci) :
		Module( gs, ModuleConfig{ id, 1 }, &_msgTypes, &_eventTypes ),
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
		_SubscribeOnMsg( this, &OpenCLContext::_GetGraphicsModules );
		
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
	bool OpenCLContext::_AddToManager (const ModuleMsg::AddToManager &msg)
	{
		CHECK_ERR( msg.module );
		CHECK_ERR( msg.module->GetSupportedMessages().HasAllTypes< CLThreadMsgList_t >() );
		CHECK_ERR( msg.module->GetSupportedEvents().HasAllTypes< CLThreadEventList_t >() );
		ASSERT( not _threads.IsExist( msg.module ) );

		_threads.Add( msg.module );
		return true;
	}
	
/*
=================================================
	_RemoveFromManager
=================================================
*/
	bool OpenCLContext::_RemoveFromManager (const ModuleMsg::RemoveFromManager &msg)
	{
		CHECK_ERR( msg.module );

		ModulePtr	module = msg.module.Lock();

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
	bool OpenCLContext::_GetGraphicsModules (const GpuMsg::GetGraphicsModules &msg)
	{
		msg.compute.Set( OpenCLObjectsConstructor::GetComputeModules() );
		return true;
	}
//-----------------------------------------------------------------------------


		
/*
=================================================
	GetComputeModules
=================================================
*/	
	ComputeModuleIDs OpenCLObjectsConstructor::GetComputeModules ()
	{
		ComputeModuleIDs	compute;
		compute.pipeline		= CLComputePipelineModuleID;
		compute.context			= CLContextModuleID;
		compute.thread			= CLThreadModuleID;
		compute.commandBuffer	= CLCommandBufferModuleID;
		compute.commandBuilder	= CLCommandBuilderModuleID;
		compute.buffer			= CLBufferModuleID;
		compute.image			= CLImageModuleID;
		compute.sampler			= CLSamplerModuleID;
		compute.resourceTable	= CLPipelineResourceTableModuleID;
		return compute;
	}

/*
=================================================
	Register
=================================================
*/
	void OpenCLObjectsConstructor::Register ()
	{
		auto	mf = GetMainSystemInstance()->GlobalSystems()->modulesFactory;

		CHECK( mf->Register( CLThreadModuleID, &CreateOpenCLThread ) );
		CHECK( mf->Register( CLContextModuleID, &CreateOpenCLContext ) );

		CHECK( mf->Register( CLImageModuleID, &CreateCL1Image ) );
		CHECK( mf->Register( CLBufferModuleID, &CreateCL1Buffer ) );
		CHECK( mf->Register( CLSamplerModuleID, &CreateCL1Sampler ) );
		CHECK( mf->Register( CLSyncManagerModuleID, &CreateCL1SyncManager ) );
		CHECK( mf->Register( CLCommandQueueModuleID, &CreateCL1CommandQueue ) );
		CHECK( mf->Register( CLCommandBufferModuleID, &CreateCL1CommandBuffer ) );
		CHECK( mf->Register( CLCommandBuilderModuleID, &CreateCL1CommandBuilder ) );
		CHECK( mf->Register( CLComputePipelineModuleID, &CreateCL1ComputePipeline ) );
		CHECK( mf->Register( CLPipelineResourceTableModuleID, &CreateCL1PipelineResourceTable ) );

		if ( not mf->IsRegistered< CreateInfo::PipelineTemplate >( PipelineTemplateModuleID ) )
			CHECK( mf->Register( PipelineTemplateModuleID, &CreatePipelineTemplate ) );
	}
	
/*
=================================================
	Unregister
=================================================
*/
	void OpenCLObjectsConstructor::Unregister ()
	{
		auto	mf = GetMainSystemInstance()->GlobalSystems()->modulesFactory;

		mf->UnregisterAll( CLThreadModuleID );
		mf->UnregisterAll( CLContextModuleID );

		mf->UnregisterAll( CLImageModuleID );
		mf->UnregisterAll( CLBufferModuleID );
		mf->UnregisterAll( CLSamplerModuleID );
		mf->UnregisterAll( CLSyncManagerModuleID );
		mf->UnregisterAll( CLCommandQueueModuleID );
		mf->UnregisterAll( CLCommandBufferModuleID );
		mf->UnregisterAll( CLCommandBuilderModuleID );
		mf->UnregisterAll( CLComputePipelineModuleID );
		mf->UnregisterAll( CLPipelineResourceTableModuleID );

		//mf->UnregisterAll< Platforms::PipelineTemplate >();	// TODO
	}

/*
=================================================
	CreateOpenCLContext
=================================================
*/
	ModulePtr OpenCLObjectsConstructor:: CreateOpenCLContext (ModuleMsg::UntypedID_t id, GlobalSystemsRef gs, const CreateInfo::GpuContext &ci)
	{
		return New< OpenCLContext >( id, gs, ci );
	}

}	// Platforms
}	// Engine

#endif	// COMPUTE_API_OPENCL
