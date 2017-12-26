// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Engine/Platforms/Shared/GPU/Context.h"
#include "Engine/Platforms/Shared/GPU/Buffer.h"
#include "Engine/Platforms/Shared/GPU/Image.h"
#include "Engine/Platforms/Shared/GPU/Thread.h"
#include "Engine/Platforms/Shared/GPU/Pipeline.h"
#include "Engine/Platforms/OpenCL/Impl/CL2Messages.h"
#include "Engine/Platforms/OpenCL/OpenCLObjectsConstructor.h"

#if defined( COMPUTE_API_OPENCL )

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
											ModuleMsg::RemoveFromManager
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
		OpenCLContext (GlobalSystemsRef gs, const CreateInfo::GpuContext &ci);
		~OpenCLContext ();

		
	// message handlers
	private:
		bool _AddToManager (const Message< ModuleMsg::AddToManager > &);
		bool _RemoveFromManager (const Message< ModuleMsg::RemoveFromManager > &);
		
	private:
		static ModulePtr _CreateOpenCLThread (GlobalSystemsRef, const CreateInfo::GpuThread &);
		static ModulePtr _CreateOpenCLContext (GlobalSystemsRef, const CreateInfo::GpuContext &);
		static ModulePtr _CreateCL2Image (GlobalSystemsRef, const CreateInfo::GpuImage &);
		static ModulePtr _CreateCL2Memory (GlobalSystemsRef, const CreateInfo::GpuMemory &);
		static ModulePtr _CreateCL2Buffer (GlobalSystemsRef, const CreateInfo::GpuBuffer &);
		static ModulePtr _CreateCL2Sampler (GlobalSystemsRef, const CreateInfo::GpuSampler &);
		static ModulePtr _CreatePipelineTemplate (GlobalSystemsRef, const CreateInfo::PipelineTemplate &);
		static ModulePtr _CreateCL2CommandBuffer (GlobalSystemsRef, const CreateInfo::GpuCommandBuffer &);
		static ModulePtr _CreateCL2CommandBuilder (GlobalSystemsRef, const CreateInfo::GpuCommandBuilder &);
		static ModulePtr _CreateCL2ComputePipeline (GlobalSystemsRef, const CreateInfo::ComputePipeline &);
		static ModulePtr _CreateCL2PipelineResourceTable (GlobalSystemsRef, const CreateInfo::PipelineResourceTable &);
	};
//-----------------------------------------------------------------------------



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

		ModulePtr	module = msg->module.Lock();

		if ( not module )
			return false;

		ASSERT( _threads.IsExist( module ) );

		_threads.Erase( module );
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
		compute.memory			= CLMemoryModuleID;
		compute.resourceTable	= CLPipelineResourceTableModuleID;
		compute.uniforms		= CLUniformsModuleID;
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

		CHECK( mf->Register( CLImageModuleID, &CreateCL2Image ) );
		CHECK( mf->Register( CLMemoryModuleID, &CreateCL2Memory ) );
		CHECK( mf->Register( CLBufferModuleID, &CreateCL2Buffer ) );
		//CHECK( mf->Register( CLSamplerModuleID, &CreateCL2Sampler ) );
		CHECK( mf->Register( CLCommandBufferModuleID, &CreateCL2CommandBuffer ) );
		CHECK( mf->Register( CLCommandBuilderModuleID, &CreateCL2CommandBuilder ) );
		CHECK( mf->Register( CLComputePipelineModuleID, &CreateCL2ComputePipeline ) );
		CHECK( mf->Register( CLPipelineResourceTableModuleID, &CreateCL2PipelineResourceTable ) );

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
	CreateOpenCLContext
=================================================
*/
	ModulePtr OpenCLObjectsConstructor:: CreateOpenCLContext (GlobalSystemsRef gs, const CreateInfo::GpuContext &ci)
	{
		return New< OpenCLContext >( gs, ci );
	}

}	// Platforms
}	// Engine

#endif	// COMPUTE_API_OPENCL
