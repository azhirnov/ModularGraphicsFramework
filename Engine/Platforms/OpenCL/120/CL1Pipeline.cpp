// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Core/Config/Engine.Config.h"

#ifdef COMPUTE_API_OPENCL

#include "Engine/Platforms/Public/GPU/Pipeline.h"
#include "Engine/Platforms/Public/GPU/RenderPass.h"
#include "Engine/Platforms/OpenCL/120/CL1BaseModule.h"
#include "Engine/Platforms/OpenCL/OpenCLObjectsConstructor.h"

namespace Engine
{
namespace PlatformCL
{

	//
	// OpenCL Compute Pipeline
	//
	
	class CL1ComputePipeline final : public CL1BaseModule
	{
	// types
	private:
		using SupportedMessages_t	= CL1BaseModule::SupportedMessages_t::Append< MessageListFrom<
											GpuMsg::GetComputePipelineDescription,
											GpuMsg::GetPipelineLayoutDescription,
											GpuMsg::GetCLComputePipelineID
										> >;

		using SupportedEvents_t		= CL1BaseModule::SupportedEvents_t;
		
		using ShadersMsgList_t		= MessageListFrom< GpuMsg::GetCLShaderModuleIDs >;

		using Description_t			= ComputePipelineDescription;
		using LayoutDesc_t			= PipelineLayoutDescription;


	// constants
	private:
		static const TypeIdList		_eventTypes;


	// variables
	private:
		cl::cl_program		_programId;
		cl::cl_kernel		_kernelId;
		Description_t		_descr;
		LayoutDesc_t		_layoutDesc;
		usize				_preferredMultipleOfWorkGroupSize;


	// methods
	public:
		CL1ComputePipeline (UntypedID_t, GlobalSystemsRef gs, const CreateInfo::ComputePipeline &ci);
		~CL1ComputePipeline ();


	// message handlers
	private:
		bool _Link (const ModuleMsg::Link &);
		bool _Compose (const ModuleMsg::Compose &);
		bool _Delete (const ModuleMsg::Delete &);
		bool _AttachModule (const ModuleMsg::AttachModule &);
		bool _DetachModule (const ModuleMsg::DetachModule &);

		bool _GetCLComputePipelineID (const GpuMsg::GetCLComputePipelineID &);
		bool _GetComputePipelineDescription (const GpuMsg::GetComputePipelineDescription &);
		bool _GetPipelineLayoutDescription (const GpuMsg::GetPipelineLayoutDescription &);

	private:
		bool _IsCreated () const;
		bool _CreatePipeline ();
		void _DestroyPipeline ();
	};
//-----------------------------------------------------------------------------


	
	const TypeIdList	CL1ComputePipeline::_eventTypes{ UninitializedT< SupportedEvents_t >() };

/*
=================================================
	constructor
=================================================
*/
	CL1ComputePipeline::CL1ComputePipeline (UntypedID_t id, GlobalSystemsRef gs, const CreateInfo::ComputePipeline &ci) :
		CL1BaseModule( gs, ModuleConfig{ id, UMax }, &_eventTypes ),
		_programId{ null },			_kernelId{ null },
		_descr{ ci.descr },			_layoutDesc{ ci.layout },
		_preferredMultipleOfWorkGroupSize(0)
	{
		SetDebugName( "CL1ComputePipeline" );

		_SubscribeOnMsg( this, &CL1ComputePipeline::_OnModuleAttached_Impl );
		_SubscribeOnMsg( this, &CL1ComputePipeline::_OnModuleDetached_Impl );
		_SubscribeOnMsg( this, &CL1ComputePipeline::_AttachModule );
		_SubscribeOnMsg( this, &CL1ComputePipeline::_DetachModule );
		_SubscribeOnMsg( this, &CL1ComputePipeline::_FindModule_Impl );
		_SubscribeOnMsg( this, &CL1ComputePipeline::_ModulesDeepSearch_Impl );
		_SubscribeOnMsg( this, &CL1ComputePipeline::_Link );
		_SubscribeOnMsg( this, &CL1ComputePipeline::_Compose );
		_SubscribeOnMsg( this, &CL1ComputePipeline::_Delete );
		_SubscribeOnMsg( this, &CL1ComputePipeline::_OnManagerChanged );
		_SubscribeOnMsg( this, &CL1ComputePipeline::_GetCLComputePipelineID );
		_SubscribeOnMsg( this, &CL1ComputePipeline::_GetComputePipelineDescription );
		_SubscribeOnMsg( this, &CL1ComputePipeline::_GetDeviceInfo );
		_SubscribeOnMsg( this, &CL1ComputePipeline::_GetCLDeviceInfo );
		_SubscribeOnMsg( this, &CL1ComputePipeline::_GetCLPrivateClasses );
		_SubscribeOnMsg( this, &CL1ComputePipeline::_GetPipelineLayoutDescription );
		
		ASSERT( _ValidateMsgSubscriptions< SupportedMessages_t >() );

		_AttachSelfToManager( _GetGPUThread( ci.gpuThread ), UntypedID_t(0), true );
	}
	
/*
=================================================
	destructor
=================================================
*/
	CL1ComputePipeline::~CL1ComputePipeline ()
	{
		ASSERT( not _IsCreated() );
	}
	
/*
=================================================
	_Link
=================================================
*/
	bool CL1ComputePipeline::_Link (const ModuleMsg::Link &msg)
	{
		if ( _IsComposedOrLinkedState( GetState() ) )
			return true;	// already linked

		CHECK_LINKING( GetModuleByMsg< ShadersMsgList_t >() );

		return Module::_Link_Impl( msg );
	}
	
/*
=================================================
	_Compose
=================================================
*/
	bool CL1ComputePipeline::_Compose (const ModuleMsg::Compose &msg)
	{
		if ( _IsComposedState( GetState() ) )
			return true;	// already composed

		CHECK_ERR( GetState() == EState::Linked );

		CHECK_COMPOSING( _CreatePipeline() );

		_SendForEachAttachments( msg );
		
		// very paranoic check
		CHECK( _ValidateAllSubscriptions() );
		
		CHECK( _SetState( EState::ComposedMutable ) );
		
		_SendUncheckedEvent( ModuleMsg::AfterCompose{} );
		return true;
	}
	
/*
=================================================
	_AttachModule
=================================================
*/
	bool CL1ComputePipeline::_AttachModule (const ModuleMsg::AttachModule &msg)
	{
		CHECK_ERR( msg.newModule );

		// render pass and shader must be unique
		bool	is_dependent = msg.newModule->SupportsAllMessages< ShadersMsgList_t >();

		if ( _Attach( msg.name, msg.newModule ) and is_dependent )
		{
			CHECK( _SetState( EState::Initial ) );
			_DestroyPipeline();
		}
		return true;
	}
	
/*
=================================================
	_DetachModule
=================================================
*/
	bool CL1ComputePipeline::_DetachModule (const ModuleMsg::DetachModule &msg)
	{
		CHECK_ERR( msg.oldModule );
		
		bool	is_dependent = msg.oldModule->SupportsAllMessages< ShadersMsgList_t >();

		if ( _Detach( msg.oldModule ) and is_dependent )
		{
			CHECK( _SetState( EState::Initial ) );
			_DestroyPipeline();
		}
		return true;
	}
	
/*
=================================================
	_Delete
=================================================
*/
	bool CL1ComputePipeline::_Delete (const ModuleMsg::Delete &msg)
	{
		_DestroyPipeline();
		
		_descr = Uninitialized;

		return Module::_Delete_Impl( msg );
	}
	
/*
=================================================
	_GetCLComputePipelineID
=================================================
*/
	bool CL1ComputePipeline::_GetCLComputePipelineID (const GpuMsg::GetCLComputePipelineID &msg)
	{
		ASSERT( _IsCreated() );

		msg.result.Set({ _programId, _kernelId });
		return true;
	}
	
/*
=================================================
	_GetComputePipelineDescription
=================================================
*/
	bool CL1ComputePipeline::_GetComputePipelineDescription (const GpuMsg::GetComputePipelineDescription &msg)
	{
		msg.result.Set( _descr );
		return true;
	}
	
/*
=================================================
	_GetPipelineLayoutDescription
=================================================
*/
	bool CL1ComputePipeline::_GetPipelineLayoutDescription (const GpuMsg::GetPipelineLayoutDescription &msg)
	{
		msg.result.Set( _layoutDesc );
		return true;
	}

/*
=================================================
	_IsCreated
=================================================
*/
	bool CL1ComputePipeline::_IsCreated () const
	{
		return _programId != null and _kernelId != null;
	}
	
/*
=================================================
	_CreatePipeline
=================================================
*/
	bool CL1ComputePipeline::_CreatePipeline ()
	{
		using namespace cl;
		
		CHECK_ERR( not _IsCreated() );
		
		// get shader
		ModulePtr	shaders;
		CHECK_ERR( shaders = GetModuleByMsg< ShadersMsgList_t >() );

		// get shader modules
		GpuMsg::GetCLShaderModuleIDs	req_shader_ids;
		shaders->Send( req_shader_ids );
		CHECK_ERR( req_shader_ids.result and req_shader_ids.result->Count() == 1 );

		const auto&		comp_sh	= req_shader_ids.result->Front();
		CHECK_ERR( comp_sh.type == EShader::Compute );

		_programId = comp_sh.id;

		cl_int	cl_err = 0;
		CL_CHECK(( (_kernelId = clCreateKernel( _programId, comp_sh.entry.cstr(), OUT &cl_err )), cl_err ));

		CHECK_ERR( _IsCreated() );
		return true;
	}
	
/*
=================================================
	_DestroyPipeline
=================================================
*/
	void CL1ComputePipeline::_DestroyPipeline ()
	{
		using namespace cl;

		if ( _kernelId ) {
			CL_CALL( clReleaseKernel( _kernelId ) );
		}

		_programId	= null;
		_kernelId	= null;
	}

}	// PlatformCL
//-----------------------------------------------------------------------------

namespace Platforms
{
	ModulePtr OpenCLObjectsConstructor::CreateCL1ComputePipeline (ModuleMsg::UntypedID_t id, GlobalSystemsRef gs, const CreateInfo::ComputePipeline &ci)
	{
		return New< PlatformCL::CL1ComputePipeline >( id, gs, ci );
	}

}	// Platforms
}	// Engine

#endif	// COMPUTE_API_OPENCL
