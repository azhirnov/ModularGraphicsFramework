// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Engine/Config/Engine.Config.h"

#ifdef GRAPHICS_API_SOFT

#include "Engine/Platforms/Public/GPU/Pipeline.h"
#include "Engine/Platforms/Public/GPU/RenderPass.h"
#include "Engine/Platforms/Soft/Impl/SWBaseModule.h"
#include "Engine/Platforms/Soft/SoftRendererObjectsConstructor.h"

namespace Engine
{
namespace PlatformSW
{
	
	//
	// Software Renderer Graphics Pipeline
	//

	class SWGraphicsPipeline final : public SWBaseModule
	{
	// types
	private:
		using SupportedMessages_t	= SWBaseModule::SupportedMessages_t::Append< MessageListFrom<
											GpuMsg::GetGraphicsPipelineDescriptor,
											GpuMsg::GetPipelineLayoutDescriptor,
											GpuMsg::GetSWPipelineStage
										> >;

		using SupportedEvents_t		= SWBaseModule::SupportedEvents_t;

		using Descriptor			= GraphicsPipelineDescriptor;


	// constants
	private:
		static const TypeIdList		_msgTypes;
		static const TypeIdList		_eventTypes;


	// variables
	private:
		Descriptor		_descr;


	// methods
	public:
		SWGraphicsPipeline (GlobalSystemsRef gs, const CreateInfo::GraphicsPipeline &ci);
		~SWGraphicsPipeline ();


	// message handlers
	private:
		bool _Compose (const Message< ModuleMsg::Compose > &);
		bool _Delete (const Message< ModuleMsg::Delete > &);
		bool _AttachModule (const Message< ModuleMsg::AttachModule > &);
		bool _DetachModule (const Message< ModuleMsg::DetachModule > &);

		bool _GetGraphicsPipelineDescriptor (const Message< GpuMsg::GetGraphicsPipelineDescriptor > &);
		bool _GetPipelineLayoutDescriptor (const Message< GpuMsg::GetPipelineLayoutDescriptor > &);
		
	private:
		bool _IsCreated () const;

		bool _CreatePipeline ();
		void _DestroyPipeline ();
		
		bool _ValidateRenderPass () const;
	};
//-----------------------------------------------------------------------------


	
	const TypeIdList	SWGraphicsPipeline::_msgTypes{ UninitializedT< SupportedMessages_t >() };
	const TypeIdList	SWGraphicsPipeline::_eventTypes{ UninitializedT< SupportedEvents_t >() };

/*
=================================================
	constructor
=================================================
*
	SWGraphicsPipeline::SWGraphicsPipeline (GlobalSystemsRef gs, const CreateInfo::GraphicsPipeline &ci) :
		SWBaseModule( gs, ModuleConfig{ SWGraphicsPipelineModuleID, UMax }, &_msgTypes, &_eventTypes ),
		_descr( ci.descr ),				_shaders( ci.shaders ),
		_renderPass( ci.renderPass )
	{
		SetDebugName( "SWGraphicsPipeline" );

		_SubscribeOnMsg( this, &SWGraphicsPipeline::_OnModuleAttached_Impl );
		_SubscribeOnMsg( this, &SWGraphicsPipeline::_OnModuleDetached_Impl );
		_SubscribeOnMsg( this, &SWGraphicsPipeline::_AttachModule );
		_SubscribeOnMsg( this, &SWGraphicsPipeline::_DetachModule );
		_SubscribeOnMsg( this, &SWGraphicsPipeline::_FindModule_Impl );
		_SubscribeOnMsg( this, &SWGraphicsPipeline::_ModulesDeepSearch_Impl );
		_SubscribeOnMsg( this, &SWGraphicsPipeline::_Link_Impl );
		_SubscribeOnMsg( this, &SWGraphicsPipeline::_Compose );
		_SubscribeOnMsg( this, &SWGraphicsPipeline::_Delete );
		_SubscribeOnMsg( this, &SWGraphicsPipeline::_OnManagerChanged );
		_SubscribeOnMsg( this, &SWGraphicsPipeline::_GetGraphicsPipelineDescriptor );
		_SubscribeOnMsg( this, &SWGraphicsPipeline::_GetDeviceInfo );
		_SubscribeOnMsg( this, &SWGraphicsPipeline::_GetSWDeviceInfo );
		_SubscribeOnMsg( this, &SWGraphicsPipeline::_GetSWPrivateClasses );
		_SubscribeOnMsg( this, &SWGraphicsPipeline::_GetPipelineLayoutDescriptor );
		
		CHECK( _ValidateMsgSubscriptions() );

		_AttachSelfToManager( _GetGPUThread( ci.gpuThread ), UntypedID_t(0), true );
	}
	
/*
=================================================
	destructor
=================================================
*
	SWGraphicsPipeline::~SWGraphicsPipeline ()
	{
		ASSERT( not _IsCreated() );
	}
	
/*
=================================================
	_Compose
=================================================
*
	bool SWGraphicsPipeline::_Compose (const Message< ModuleMsg::Compose > &msg)
	{
		if ( _IsComposedState( GetState() ) )
			return true;	// already composed

		CHECK_ERR( GetState() == EState::Linked );

		CHECK_COMPOSING( _CreatePipeline() );

		_SendForEachAttachments( msg );
		
		// very paranoic check
		CHECK( _ValidateAllSubscriptions() );
		
		CHECK( _SetState( EState::ComposedMutable ) );
		
		_SendUncheckedEvent< ModuleMsg::AfterCompose >({});
		return true;
	}
	
/*
=================================================
	_Delete
=================================================
*
	bool SWGraphicsPipeline::_Delete (const Message< ModuleMsg::Delete > &msg)
	{
		_DestroyPipeline();

		return Module::_Delete_Impl( msg );
	}
	
/*
=================================================
	_GetGraphicsPipelineDescriptor
=================================================
*
	bool SWGraphicsPipeline::_GetGraphicsPipelineDescriptor (const Message< GpuMsg::GetGraphicsPipelineDescriptor > &msg)
	{
		msg->result.Set( _descr );
		return true;
	}
	
/*
=================================================
	_GetPipelineLayoutDescriptor
=================================================
*
	bool SWGraphicsPipeline::_GetPipelineLayoutDescriptor (const Message< GpuMsg::GetPipelineLayoutDescriptor > &msg)
	{
		msg->result.Set( _descr.layout );
		return true;
	}

/*
=================================================
	_IsCreated
=================================================
*
	bool SWGraphicsPipeline::_IsCreated () const
	{
	}

/*
=================================================
	_CreatePipeline
=================================================
*
	bool SWGraphicsPipeline::_CreatePipeline ()
	{
	}
	
/*
=================================================
	_DestroyPipeline
=================================================
*
	void SWGraphicsPipeline::_DestroyPipeline ()
	{
	}
	
/*
=================================================
	_ValidateRenderPass
=================================================
*
	bool SWGraphicsPipeline::_ValidateRenderPass () const
	{
	}*/
//-----------------------------------------------------------------------------



	//
	// Software Renderer Compute Pipeline
	//
	
	class SWComputePipeline final : public SWBaseModule
	{
	// types
	private:
		using SupportedMessages_t	= SWBaseModule::SupportedMessages_t::Append< MessageListFrom<
											GpuMsg::GetComputePipelineDescriptor,
											GpuMsg::GetPipelineLayoutDescriptor,
											GpuMsg::GetSWPipelineStage
										> >;

		using SupportedEvents_t		= SWBaseModule::SupportedEvents_t;
		
		using ShadersMsgList_t		= MessageListFrom< GpuMsg::GetSWShaderModuleIDs >;

		using Descriptor			= ComputePipelineDescriptor;
		using ShaderFunc_t			= PipelineTemplateDescriptor::ShaderSource::SWInvoke;


	// constants
	private:
		static const TypeIdList		_msgTypes;
		static const TypeIdList		_eventTypes;


	// variables
	private:
		Descriptor		_descr;
		ShaderFunc_t	_func;


	// methods
	public:
		SWComputePipeline (UntypedID_t, GlobalSystemsRef gs, const CreateInfo::ComputePipeline &ci);
		~SWComputePipeline ();


	// message handlers
	private:
		bool _Link (const Message< ModuleMsg::Link > &);
		bool _Compose (const Message< ModuleMsg::Compose > &);
		bool _Delete (const Message< ModuleMsg::Delete > &);
		bool _AttachModule (const Message< ModuleMsg::AttachModule > &);
		bool _DetachModule (const Message< ModuleMsg::DetachModule > &);

		bool _GetComputePipelineDescriptor (const Message< GpuMsg::GetComputePipelineDescriptor > &);
		bool _GetPipelineLayoutDescriptor (const Message< GpuMsg::GetPipelineLayoutDescriptor > &);
		bool _GetSWPipelineStage (const Message< GpuMsg::GetSWPipelineStage > &);

	private:
		bool _IsCreated () const;

		bool _CreatePipeline ();
		void _DestroyPipeline ();
	};
//-----------------------------------------------------------------------------



	const TypeIdList	SWComputePipeline::_msgTypes{ UninitializedT< SupportedMessages_t >() };
	const TypeIdList	SWComputePipeline::_eventTypes{ UninitializedT< SupportedEvents_t >() };

/*
=================================================
	constructor
=================================================
*/
	SWComputePipeline::SWComputePipeline (UntypedID_t id, GlobalSystemsRef gs, const CreateInfo::ComputePipeline &ci) :
		SWBaseModule( gs, ModuleConfig{ id, UMax }, &_msgTypes, &_eventTypes ),
		_descr{ ci.descr },		_func{ null }
	{
		SetDebugName( "SWComputePipeline" );

		_SubscribeOnMsg( this, &SWComputePipeline::_OnModuleAttached_Impl );
		_SubscribeOnMsg( this, &SWComputePipeline::_OnModuleDetached_Impl );
		_SubscribeOnMsg( this, &SWComputePipeline::_AttachModule );
		_SubscribeOnMsg( this, &SWComputePipeline::_DetachModule );
		_SubscribeOnMsg( this, &SWComputePipeline::_FindModule_Impl );
		_SubscribeOnMsg( this, &SWComputePipeline::_ModulesDeepSearch_Impl );
		_SubscribeOnMsg( this, &SWComputePipeline::_Link );
		_SubscribeOnMsg( this, &SWComputePipeline::_Compose );
		_SubscribeOnMsg( this, &SWComputePipeline::_Delete );
		_SubscribeOnMsg( this, &SWComputePipeline::_OnManagerChanged );
		_SubscribeOnMsg( this, &SWComputePipeline::_GetComputePipelineDescriptor );
		_SubscribeOnMsg( this, &SWComputePipeline::_GetDeviceInfo );
		_SubscribeOnMsg( this, &SWComputePipeline::_GetSWDeviceInfo );
		_SubscribeOnMsg( this, &SWComputePipeline::_GetSWPrivateClasses );
		_SubscribeOnMsg( this, &SWComputePipeline::_GetPipelineLayoutDescriptor );
		_SubscribeOnMsg( this, &SWComputePipeline::_GetSWPipelineStage );
		
		CHECK( _ValidateMsgSubscriptions() );

		_AttachSelfToManager( _GetGPUThread( ci.gpuThread ), UntypedID_t(0), true );
	}
	
/*
=================================================
	destructor
=================================================
*/
	SWComputePipeline::~SWComputePipeline ()
	{
		_DestroyPipeline();
	}
	
/*
=================================================
	_Link
=================================================
*/
	bool SWComputePipeline::_Link (const Message< ModuleMsg::Link > &msg)
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
	bool SWComputePipeline::_Compose (const Message< ModuleMsg::Compose > &msg)
	{
		if ( _IsComposedState( GetState() ) )
			return true;	// already composed

		CHECK_ERR( GetState() == EState::Linked );

		CHECK_COMPOSING( _CreatePipeline() );

		_SendForEachAttachments( msg );
		
		// very paranoic check
		CHECK( _ValidateAllSubscriptions() );
		
		CHECK( _SetState( EState::ComposedMutable ) );
		
		_SendUncheckedEvent< ModuleMsg::AfterCompose >({});
		return true;
	}
	
/*
=================================================
	_AttachModule
=================================================
*/
	bool SWComputePipeline::_AttachModule (const Message< ModuleMsg::AttachModule > &msg)
	{
		CHECK_ERR( msg->newModule );

		// render pass and shader must be unique
		bool	is_dependent = msg->newModule->GetSupportedMessages().HasAllTypes< ShadersMsgList_t >();

		if ( _Attach( msg->name, msg->newModule, is_dependent ) and is_dependent )
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
	bool SWComputePipeline::_DetachModule (const Message< ModuleMsg::DetachModule > &msg)
	{
		CHECK_ERR( msg->oldModule );
		
		bool	is_dependent = msg->oldModule->GetSupportedMessages().HasAllTypes< ShadersMsgList_t >();

		if ( _Detach( msg->oldModule ) and is_dependent )
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
	bool SWComputePipeline::_Delete (const Message< ModuleMsg::Delete > &msg)
	{
		_DestroyPipeline();
		
		_descr = Uninitialized;

		return Module::_Delete_Impl( msg );
	}
	
/*
=================================================
	_GetComputePipelineDescriptor
=================================================
*/
	bool SWComputePipeline::_GetComputePipelineDescriptor (const Message< GpuMsg::GetComputePipelineDescriptor > &msg)
	{
		msg->result.Set( _descr );
		return true;
	}
	
/*
=================================================
	_GetPipelineLayoutDescriptor
=================================================
*/
	bool SWComputePipeline::_GetPipelineLayoutDescriptor (const Message< GpuMsg::GetPipelineLayoutDescriptor > &msg)
	{
		msg->result.Set( _descr.layout );
		return true;
	}

/*
=================================================
	_IsCreated
=================================================
*/
	bool SWComputePipeline::_IsCreated () const
	{
		return _func != null;
	}
	
/*
=================================================
	_CreatePipeline
=================================================
*/
	bool SWComputePipeline::_CreatePipeline ()
	{
		CHECK_ERR( not _IsCreated() );
		
		// get shader
		ModulePtr	shaders;
		CHECK_ERR( shaders = GetModuleByMsg< ShadersMsgList_t >() );

		// get shader modules
		Message< GpuMsg::GetSWShaderModuleIDs >		req_shader_ids;
		SendTo( shaders, req_shader_ids );
		CHECK_ERR( req_shader_ids->result and not req_shader_ids->result->Empty() );
		
		usize	cs_index = UMax;

		FOR( i, *req_shader_ids->result ) {
			if ( (*req_shader_ids->result)[i].type == EShader::Compute ) {
				cs_index = i;
				break;
			}
		}
		CHECK_ERR( cs_index < req_shader_ids->result->Count() );	// compute shader not found

		_func = (*req_shader_ids->result)[cs_index].func;
		CHECK_ERR( _func );

		return true;
	}
	
/*
=================================================
	_DestroyPipeline
=================================================
*/
	void SWComputePipeline::_DestroyPipeline ()
	{
		_func	= null;
	}
	
/*
=================================================
	_GetSWPipelineStage
=================================================
*/
	bool SWComputePipeline::_GetSWPipelineStage (const Message< GpuMsg::GetSWPipelineStage > &msg)
	{
		msg->result.Set({ _func });
		return true;
	}
	
}	// PlatformSW
//-----------------------------------------------------------------------------

namespace Platforms
{
	/*ModulePtr SoftRendererObjectsConstructor::CreateSWGraphicsPipeline (GlobalSystemsRef gs, const CreateInfo::GraphicsPipeline &msg)
	{
		return New< PlatformSW::SWGraphicsPipeline >( gs, msg );
	}*/

	ModulePtr SoftRendererObjectsConstructor::CreateSWComputePipeline (ModuleMsg::UntypedID_t id, GlobalSystemsRef gs, const CreateInfo::ComputePipeline &msg)
	{
		return New< PlatformSW::SWComputePipeline >( id, gs, msg );
	}

}	// Platforms
}	// Engine

#endif	// GRAPHICS_API_SOFT
