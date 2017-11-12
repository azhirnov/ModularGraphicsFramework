// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/Platforms/OpenCL/Impl/CL2BaseModule.h"
#include "Engine/Platforms/Shared/GPU/Pipeline.h"
#include "Engine/Platforms/Shared/GPU/RenderPass.h"
#include "Engine/Platforms/Shared/GPU/VertexInputState.h"
#include "Engine/Platforms/OpenCL/OpenCLContext.h"

#if defined( COMPUTE_API_OPENCL )

namespace Engine
{
namespace PlatformCL
{

	//
	// OpenCL Compute Pipeline
	//
	
	class CL2ComputePipeline final : public CL2BaseModule
	{
	// types
	private:
		using SupportedMessages_t	= CL2BaseModule::SupportedMessages_t::Append< MessageListFrom<
											GpuMsg::GetComputePipelineDescriptor,
											GpuMsg::GetPipelineLayoutDescriptor,
											GpuMsg::GetCLComputePipelineID
										> >;

		using SupportedEvents_t		= CL2BaseModule::SupportedEvents_t;
		
		using Descriptor			= ComputePipelineDescriptor;


	// constants
	private:
		static const TypeIdList		_msgTypes;
		static const TypeIdList		_eventTypes;


	// variables
	private:
		cl::cl_program		_programId;
		cl::cl_kernel		_kernelId;
		Descriptor			_descr;
		ModulePtr			_shaders;		// use as attachment
		usize				_preferredMultipleOfWorkGroupSize;


	// methods
	public:
		CL2ComputePipeline (GlobalSystemsRef gs, const CreateInfo::ComputePipeline &ci);
		~CL2ComputePipeline ();


	// message handlers
	private:
		bool _Compose (const Message< ModuleMsg::Compose > &);
		bool _Delete (const Message< ModuleMsg::Delete > &);
		bool _GetCLComputePipelineID (const Message< GpuMsg::GetCLComputePipelineID > &);
		bool _GetComputePipelineDescriptor (const Message< GpuMsg::GetComputePipelineDescriptor > &);
		bool _GetPipelineLayoutDescriptor (const Message< GpuMsg::GetPipelineLayoutDescriptor > &);

	private:
		bool _IsCreated () const;
		bool _CreatePipeline ();
		void _DestroyPipeline ();
	};
//-----------------------------------------------------------------------------


	
	const TypeIdList	CL2ComputePipeline::_msgTypes{ UninitializedT< SupportedMessages_t >() };
	const TypeIdList	CL2ComputePipeline::_eventTypes{ UninitializedT< SupportedEvents_t >() };

/*
=================================================
	constructor
=================================================
*/
	CL2ComputePipeline::CL2ComputePipeline (GlobalSystemsRef gs, const CreateInfo::ComputePipeline &ci) :
		CL2BaseModule( gs, ModuleConfig{ CLComputePipelineModuleID, ~0u }, &_msgTypes, &_eventTypes ),
		_programId( null ),		_kernelId( null ),
		_descr( ci.descr ),		_shaders( ci.shaders ),
		_preferredMultipleOfWorkGroupSize(0)
	{
		SetDebugName( "CL2ComputePipeline" );

		_SubscribeOnMsg( this, &CL2ComputePipeline::_OnModuleAttached_Impl );
		_SubscribeOnMsg( this, &CL2ComputePipeline::_OnModuleDetached_Impl );
		_SubscribeOnMsg( this, &CL2ComputePipeline::_AttachModule_Impl );
		_SubscribeOnMsg( this, &CL2ComputePipeline::_DetachModule_Impl );
		_SubscribeOnMsg( this, &CL2ComputePipeline::_FindModule_Impl );
		_SubscribeOnMsg( this, &CL2ComputePipeline::_ModulesDeepSearch_Impl );
		_SubscribeOnMsg( this, &CL2ComputePipeline::_Link_Impl );
		_SubscribeOnMsg( this, &CL2ComputePipeline::_Compose );
		_SubscribeOnMsg( this, &CL2ComputePipeline::_Delete );
		_SubscribeOnMsg( this, &CL2ComputePipeline::_OnManagerChanged );
		_SubscribeOnMsg( this, &CL2ComputePipeline::_DeviceBeforeDestroy );
		_SubscribeOnMsg( this, &CL2ComputePipeline::_GetCLComputePipelineID );
		_SubscribeOnMsg( this, &CL2ComputePipeline::_GetComputePipelineDescriptor );
		_SubscribeOnMsg( this, &CL2ComputePipeline::_GetDeviceInfo );
		_SubscribeOnMsg( this, &CL2ComputePipeline::_GetCLDeviceInfo );
		_SubscribeOnMsg( this, &CL2ComputePipeline::_GetCLPrivateClasses );
		_SubscribeOnMsg( this, &CL2ComputePipeline::_GetPipelineLayoutDescriptor );
		
		CHECK( _ValidateMsgSubscriptions() );

		_AttachSelfToManager( ci.gpuThread, Platforms::CLThreadModuleID, true );
	}
	
/*
=================================================
	destructor
=================================================
*/
	CL2ComputePipeline::~CL2ComputePipeline ()
	{
		ASSERT( not _IsCreated() );
	}
	
/*
=================================================
	_Compose
=================================================
*/
	bool CL2ComputePipeline::_Compose (const Message< ModuleMsg::Compose > &msg)
	{
		if ( _IsComposedState( GetState() ) )
			return true;	// already composed

		CHECK_ERR( GetState() == EState::Linked );

		CHECK_COMPOSING( _CreatePipeline() );

		_SendForEachAttachments( msg );
		
		// very paranoic check
		CHECK( _ValidateAllSubscriptions() );

		CHECK( _SetState( EState::ComposedImmutable ) );
		return true;
	}
	
/*
=================================================
	_Delete
=================================================
*/
	bool CL2ComputePipeline::_Delete (const Message< ModuleMsg::Delete > &msg)
	{
		_DestroyPipeline();

		return Module::_Delete_Impl( msg );
	}
	
/*
=================================================
	_GetCLComputePipelineID
=================================================
*/
	bool CL2ComputePipeline::_GetCLComputePipelineID (const Message< GpuMsg::GetCLComputePipelineID > &msg)
	{
		msg->result.Set({ _programId, _kernelId });
		return true;
	}
	
/*
=================================================
	_GetComputePipelineDescriptor
=================================================
*/
	bool CL2ComputePipeline::_GetComputePipelineDescriptor (const Message< GpuMsg::GetComputePipelineDescriptor > &msg)
	{
		msg->result.Set( _descr );
		return true;
	}
	
/*
=================================================
	_GetPipelineLayoutDescriptor
=================================================
*/
	bool CL2ComputePipeline::_GetPipelineLayoutDescriptor (const Message< GpuMsg::GetPipelineLayoutDescriptor > &msg)
	{
		msg->result.Set( _descr.layout );
		return true;
	}

/*
=================================================
	_IsCreated
=================================================
*/
	bool CL2ComputePipeline::_IsCreated () const
	{
		return _programId != null and _kernelId != null;
	}
	
/*
=================================================
	_CreatePipeline
=================================================
*/
	bool CL2ComputePipeline::_CreatePipeline ()
	{
		using namespace cl;
		
		CHECK_ERR( not _IsCreated() );
		
		// get shader modules
		Message< GpuMsg::GetCLShaderModuleIDs >		req_shader_ids;
		SendTo( _shaders, req_shader_ids );
		CHECK_ERR( req_shader_ids->result and req_shader_ids->result->Count() == 1 );

		const auto&		comp_sh	= req_shader_ids->result.Get().Front();
		CHECK_ERR( comp_sh.type == EShader::Compute );

		_programId = comp_sh.id;

		cl_int	cl_err = 0;
		CL_CHECK( ((_kernelId = clCreateKernel( _programId, comp_sh.entry.cstr(), &cl_err )), cl_err) );

		CHECK_ERR( _IsCreated() );
		return true;
	}
	
/*
=================================================
	_DestroyPipeline
=================================================
*/
	void CL2ComputePipeline::_DestroyPipeline ()
	{
		using namespace cl;

		if ( _kernelId ) {
			CL_CALL( clReleaseKernel( _kernelId ) );
		}

		_programId	= null;
		_kernelId	= null;
		_descr		= Uninitialized;
	}

}	// PlatformCL

namespace Platforms
{
	ModulePtr OpenCLContext::_CreateCL2ComputePipeline (GlobalSystemsRef gs, const CreateInfo::ComputePipeline &ci)
	{
		return New< PlatformCL::CL2ComputePipeline >( gs, ci );
	}

}	// Platforms
}	// Engine

#endif	// COMPUTE_API_OPENCL
