// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Engine/Config/Engine.Config.h"

#ifdef GRAPHICS_API_OPENGL

#include "Engine/Platforms/Public/GPU/Pipeline.h"
#include "Engine/Platforms/Public/GPU/RenderPass.h"
#include "Engine/Platforms/OpenGL/450/GL4BaseModule.h"
#include "Engine/Platforms/OpenGL/OpenGLObjectsConstructor.h"

namespace Engine
{
namespace PlatformGL
{
	using namespace gl;

	
	//
	// OpenGL Graphics Pipeline
	//

	class GL4GraphicsPipeline final : public GL4BaseModule
	{
	// types
	private:
		using SupportedMessages_t	= GL4BaseModule::SupportedMessages_t::Append< MessageListFrom<
											GpuMsg::GetGraphicsPipelineDescriptor,
											GpuMsg::GetGLGraphicsPipelineID,
											GpuMsg::GetPipelineLayoutDescriptor,
											GpuMsg::GetGLPipelineLayoutPushConstants
										> >;

		using SupportedEvents_t		= GL4BaseModule::SupportedEvents_t;
		
		using ShadersMsgList_t		= MessageListFrom< GpuMsg::GetGLShaderModuleIDs >;
		using RenderPassMsgList_t	= MessageListFrom< GpuMsg::GetRenderPassDescriptor >;

		using Descriptor			= GraphicsPipelineDescriptor;
		using Programs_t			= StaticArray< GLuint, Platforms::EShader::_Count >;
		using PushConstants_t		= GpuMsg::GetGLPipelineLayoutPushConstants::PushConstants_t;


	// constants
	private:
		static const TypeIdList		_msgTypes;
		static const TypeIdList		_eventTypes;


	// variables
	private:
		GLuint			_pipelineId;
		Programs_t		_programs;
		GLuint			_vertexAttribs;
		Descriptor		_descr;
		PushConstants_t	_pushConstants;


	// methods
	public:
		GL4GraphicsPipeline (UntypedID_t, GlobalSystemsRef gs, const CreateInfo::GraphicsPipeline &ci);
		~GL4GraphicsPipeline ();


	// message handlers
	private:
		bool _Link (const Message< ModuleMsg::Link > &);
		bool _Compose (const Message< ModuleMsg::Compose > &);
		bool _Delete (const Message< ModuleMsg::Delete > &);
		bool _AttachModule (const Message< ModuleMsg::AttachModule > &);
		bool _DetachModule (const Message< ModuleMsg::DetachModule > &);

		bool _GetGLGraphicsPipelineID (const Message< GpuMsg::GetGLGraphicsPipelineID > &);
		bool _GetGraphicsPipelineDescriptor (const Message< GpuMsg::GetGraphicsPipelineDescriptor > &);
		bool _GetPipelineLayoutDescriptor (const Message< GpuMsg::GetPipelineLayoutDescriptor > &);
		bool _GetGLPipelineLayoutPushConstants (const Message< GpuMsg::GetGLPipelineLayoutPushConstants > &);
		
	private:
		bool _IsCreated () const;

		bool _CreatePipeline ();
		void _DestroyPipeline ();

		bool _CreateVertexArray ();
		void _DestroyVertexArray ();
		
		bool _ValidateRenderPass (const ModulePtr &renderPass) const;
	};
//-----------------------------------------------------------------------------

	
/*
=================================================
	MakePushConstantsCache_Func
=================================================
*/
	struct MakePushConstantsCache_Func
	{
		using PushConstants_t = GpuMsg::GetGLPipelineLayoutPushConstants::PushConstants_t;

		PushConstants_t&	pushConstMap;

		MakePushConstantsCache_Func (OUT PushConstants_t &cache) : pushConstMap{cache}
		{}

		template <typename T>
		void operator () (const T &) const
		{}

		void operator () (const PipelineLayoutDescriptor::PushConstant &pc) const
		{
			pushConstMap.Add( pc.name, { pc.stageFlags, pc.offset, pc.size } );
		}
	};

/*
=================================================
	MakePushConstantsCache
=================================================
*/
	static void MakePushConstantsCache (const PipelineLayoutDescriptor &layout,
										OUT GpuMsg::GetGLPipelineLayoutPushConstants::PushConstants_t &cache)
	{
		cache.Clear();

		MakePushConstantsCache_Func		func( cache );

		FOR( i, layout.GetUniforms() ) {
			layout.GetUniforms()[i].Apply( func );
		}
	}
//-----------------------------------------------------------------------------


	
	const TypeIdList	GL4GraphicsPipeline::_msgTypes{ UninitializedT< SupportedMessages_t >() };
	const TypeIdList	GL4GraphicsPipeline::_eventTypes{ UninitializedT< SupportedEvents_t >() };

/*
=================================================
	constructor
=================================================
*/
	GL4GraphicsPipeline::GL4GraphicsPipeline (UntypedID_t id, GlobalSystemsRef gs, const CreateInfo::GraphicsPipeline &ci) :
		GL4BaseModule( gs, ModuleConfig{ id, UMax }, &_msgTypes, &_eventTypes ),
		_pipelineId( 0 ),				_vertexAttribs( 0 ),
		_descr( ci.descr )
	{
		SetDebugName( "GL4GraphicsPipeline" );

		_SubscribeOnMsg( this, &GL4GraphicsPipeline::_OnModuleAttached_Impl );
		_SubscribeOnMsg( this, &GL4GraphicsPipeline::_OnModuleDetached_Impl );
		_SubscribeOnMsg( this, &GL4GraphicsPipeline::_AttachModule );
		_SubscribeOnMsg( this, &GL4GraphicsPipeline::_DetachModule );
		_SubscribeOnMsg( this, &GL4GraphicsPipeline::_FindModule_Impl );
		_SubscribeOnMsg( this, &GL4GraphicsPipeline::_ModulesDeepSearch_Impl );
		_SubscribeOnMsg( this, &GL4GraphicsPipeline::_Link );
		_SubscribeOnMsg( this, &GL4GraphicsPipeline::_Compose );
		_SubscribeOnMsg( this, &GL4GraphicsPipeline::_Delete );
		_SubscribeOnMsg( this, &GL4GraphicsPipeline::_OnManagerChanged );
		_SubscribeOnMsg( this, &GL4GraphicsPipeline::_GetGLGraphicsPipelineID );
		_SubscribeOnMsg( this, &GL4GraphicsPipeline::_GetGraphicsPipelineDescriptor );
		_SubscribeOnMsg( this, &GL4GraphicsPipeline::_GetDeviceInfo );
		_SubscribeOnMsg( this, &GL4GraphicsPipeline::_GetGLDeviceInfo );
		_SubscribeOnMsg( this, &GL4GraphicsPipeline::_GetGLPrivateClasses );
		_SubscribeOnMsg( this, &GL4GraphicsPipeline::_GetPipelineLayoutDescriptor );
		_SubscribeOnMsg( this, &GL4GraphicsPipeline::_GetGLPipelineLayoutPushConstants );
		
		CHECK( _ValidateMsgSubscriptions() );

		_AttachSelfToManager( _GetGPUThread( ci.gpuThread ), UntypedID_t(0), true );

		MakePushConstantsCache( _descr.layout, OUT _pushConstants );
	}
	
/*
=================================================
	destructor
=================================================
*/
	GL4GraphicsPipeline::~GL4GraphicsPipeline ()
	{
		ASSERT( not _IsCreated() );
	}
	
/*
=================================================
	_Link
=================================================
*/
	bool GL4GraphicsPipeline::_Link (const Message< ModuleMsg::Link > &msg)
	{
		if ( _IsComposedOrLinkedState( GetState() ) )
			return true;	// already linked
		
		if ( not GetModuleByMsg< RenderPassMsgList_t >() )
		{
			// validate render pass
			Message< GpuMsg::GetDeviceInfo >	req_dev;
			CHECK( _GetManager()->Send( req_dev ) );

			_descr.subpass	= 0;

			CHECK_ERR( _Attach( "", req_dev->result->renderPass, true ) );

			LOG( "used default render pass", ELog::Debug );
		}

		CHECK_LINKING( GetModuleByMsg< ShadersMsgList_t >() );

		return Module::_Link_Impl( msg );
	}
	
/*
=================================================
	_Compose
=================================================
*/
	bool GL4GraphicsPipeline::_Compose (const Message< ModuleMsg::Compose > &msg)
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
	bool GL4GraphicsPipeline::_AttachModule (const Message< ModuleMsg::AttachModule > &msg)
	{
		CHECK_ERR( msg->newModule );

		// render pass and shader must be unique
		bool	is_dependent =  msg->newModule->GetSupportedMessages().HasAllTypes< RenderPassMsgList_t >() or
								msg->newModule->GetSupportedMessages().HasAllTypes< ShadersMsgList_t >();

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
	bool GL4GraphicsPipeline::_DetachModule (const Message< ModuleMsg::DetachModule > &msg)
	{
		CHECK_ERR( msg->oldModule );
		
		bool	is_dependent =  msg->oldModule->GetSupportedMessages().HasAllTypes< RenderPassMsgList_t >() or
								msg->oldModule->GetSupportedMessages().HasAllTypes< ShadersMsgList_t >();

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
	bool GL4GraphicsPipeline::_Delete (const Message< ModuleMsg::Delete > &msg)
	{
		_DestroyPipeline();
		
		_descr = Uninitialized;

		return Module::_Delete_Impl( msg );
	}
	
/*
=================================================
	_GetGLGraphicsPipelineID
=================================================
*/
	bool GL4GraphicsPipeline::_GetGLGraphicsPipelineID (const Message< GpuMsg::GetGLGraphicsPipelineID > &msg)
	{
		msg->result.Set({ _programs, _pipelineId, _vertexAttribs });
		return true;
	}
	
/*
=================================================
	_GetGraphicsPipelineDescriptor
=================================================
*/
	bool GL4GraphicsPipeline::_GetGraphicsPipelineDescriptor (const Message< GpuMsg::GetGraphicsPipelineDescriptor > &msg)
	{
		msg->result.Set( _descr );
		return true;
	}
	
/*
=================================================
	_GetPipelineLayoutDescriptor
=================================================
*/
	bool GL4GraphicsPipeline::_GetPipelineLayoutDescriptor (const Message< GpuMsg::GetPipelineLayoutDescriptor > &msg)
	{
		msg->result.Set( _descr.layout );
		return true;
	}
	
/*
=================================================
	_GetGLPipelineLayoutPushConstants
=================================================
*/
	bool GL4GraphicsPipeline::_GetGLPipelineLayoutPushConstants (const Message< GpuMsg::GetGLPipelineLayoutPushConstants > &msg)
	{
		msg->result.Set( _pushConstants );
		return true;
	}

/*
=================================================
	_IsCreated
=================================================
*/
	bool GL4GraphicsPipeline::_IsCreated () const
	{
		return _pipelineId != 0;
	}
	
/*
=================================================
	ValidateGLPipeline
=================================================
*/
	static bool ValidateGLPipeline (GLuint pipelineId)
	{
		GL_CALL( glValidateProgramPipeline( pipelineId ) );

		GLint	status = 0;
		GL_CALL( glGetProgramPipelineiv( pipelineId, GL_VALIDATE_STATUS, &status ) );

		const bool	valid = !!status;

		if ( not valid )
		{
			GLint	log_len = 0;
			GL_CALL( glGetProgramPipelineiv( pipelineId, GL_INFO_LOG_LENGTH, &log_len ) );

			String	log;	log.Reserve( log_len + 64 );

			GL_CALL( glGetProgramPipelineInfoLog( pipelineId, log_len, &log_len, log.ptr() ) );

			LOG( "OpenGL pipeline compilation failed: "_str << log, ELog::Warning );
		}
		return valid;
	}

/*
=================================================
	_CreatePipeline
=================================================
*/
	bool GL4GraphicsPipeline::_CreatePipeline ()
	{
		CHECK_ERR( not _IsCreated() );
		CHECK_ERR( _GetManager() );
		
		// get render pass
		ModulePtr	render_pass;
		CHECK_ERR( render_pass = GetModuleByMsg< RenderPassMsgList_t >() );

		CHECK_ERR( _ValidateRenderPass( render_pass ) );

		
		// get shader
		ModulePtr	shaders;
		CHECK_ERR( shaders = GetModuleByMsg< ShadersMsgList_t >() );

		// get shader modules
		Message< GpuMsg::GetGLShaderModuleIDs >		req_shader_ids;
		SendTo( shaders, req_shader_ids );
		CHECK_ERR( req_shader_ids->result and not req_shader_ids->result->Empty() );

		GL_CALL( glGenProgramPipelines( 1, &_pipelineId ) );
		CHECK_ERR( _pipelineId != 0 );

		FOR( i, *req_shader_ids->result )
		{
			const auto&	sh = (*req_shader_ids->result)[i];

			GL_CALL( glUseProgramStages( _pipelineId, GL4Enum(EShader::bits().Set( sh.type )), sh.id ) );
		}


		// validate pipeline
		CHECK_ERR( ValidateGLPipeline( _pipelineId ) );
		CHECK_ERR( _CreateVertexArray() );

		GetDevice()->SetObjectName( _pipelineId, GetDebugName(), EGpuObject::Pipeline );
		GetDevice()->SetObjectName( _vertexAttribs, GetDebugName(), EGpuObject::VertexArray );
		return true;
	}
	
/*
=================================================
	_DestroyPipeline
=================================================
*/
	void GL4GraphicsPipeline::_DestroyPipeline ()
	{
		if ( _pipelineId != 0 )
		{
			GL_CALL( glDeleteProgramPipelines( 1, &_pipelineId ) );
		}

		_DestroyVertexArray();

		_pipelineId	= 0;
		_programs	= Uninitialized;
	}
	
/*
=================================================
	_CreateVertexArray
=================================================
*/
	bool GL4GraphicsPipeline::_CreateVertexArray ()
	{
		using Value_t = EVertexAttribute::ValueType;

		CHECK_ERR( _vertexAttribs == 0 );
		
		GL_CALL( glGenVertexArrays( 1, &_vertexAttribs ) );
		CHECK_ERR( _vertexAttribs != 0 );
		GL_CALL( glBindVertexArray( _vertexAttribs ) );

		const auto	attribs		= _descr.vertexInput.Attribs();
		const auto	bindings	= _descr.vertexInput.Bindings();

		FOR( i, attribs )
		{
			const auto&				attr	= attribs[i].second;
			EVertexAttribute::type	vs_type	= attr.ToDstType();
			GL4VertexAttribType		type;
			uint					size;
			bool					norm = false;
			CHECK_ERR( GL4Enum( attr.type, OUT type, OUT size, OUT norm ) );

			GL_CALL( glEnableVertexAttribArray( attr.index ) );
			GL_CALL( glVertexAttribBinding( attr.index, attr.bindingIndex ) );
			
			if ( Value_t::IsDouble( vs_type ) )
			{
				GL_CALL( glVertexAttribLFormat( attr.index, size, type, (GLsizei) isize(attr.offset) ) );
			}
			else
			if ( Value_t::IsInteger( vs_type ) )
			{
				GL_CALL( glVertexAttribIFormat( attr.index, size, type, (GLsizei) isize(attr.offset) ) );
			}
			else
			{
				CHECK( Value_t::IsFloat( vs_type ) );
				GL_CALL( glVertexAttribFormat( attr.index, size, type, norm, (GLsizei) isize(attr.offset) ) );
			}
		}

		FOR( i, bindings )
		{
			const auto&		bind = bindings[i].second;

			ASSERT( bind.rate == EVertexInputRate::Vertex );
			GL_CALL( glVertexBindingDivisor( bind.index, 0 ) );
		}
		
		GL_CALL( glBindVertexArray( 0 ) );
		return true;
	}
	
/*
=================================================
	_DestroyVertexArray
=================================================
*/
	void GL4GraphicsPipeline::_DestroyVertexArray ()
	{
		if ( _vertexAttribs != 0 )
		{
			GL_CALL( glDeleteVertexArrays( 1, &_vertexAttribs ) );
		}
		_vertexAttribs = 0;
	}

/*
=================================================
	_ValidateRenderPass
=================================================
*/
	bool GL4GraphicsPipeline::_ValidateRenderPass (const ModulePtr &renderPass) const
	{
		Message< GpuMsg::GetRenderPassDescriptor >	req_descr;
		renderPass->Send( req_descr );

		const auto&	descr = *req_descr->result;

		CHECK_ERR( _descr.subpass < descr.Subpasses().Count() );
		
		const auto&	subpass	= descr.Subpasses()[ _descr.subpass ];

		CHECK_ERR( _descr.fragOutput.Count() == subpass.colors.Count() );

		// can't make mistake when number of attachment is 1
		if ( _descr.fragOutput.Count() <= 1 )
			return true;

		FOR( i, subpass.colors )
		{
			auto	frag = _descr.fragOutput( subpass.colors[i].name );
			CHECK_ERR( frag and frag->index == i );
		}

		return true;
	}
//-----------------------------------------------------------------------------



	//
	// OpenGL Compute Pipeline
	//
	
	class GL4ComputePipeline final : public GL4BaseModule
	{
	// types
	private:
		using SupportedMessages_t	= GL4BaseModule::SupportedMessages_t::Append< MessageListFrom<
											GpuMsg::GetComputePipelineDescriptor,
											GpuMsg::GetGLComputePipelineID,
											GpuMsg::GetPipelineLayoutDescriptor,
											GpuMsg::GetGLPipelineLayoutPushConstants
										> >;

		using SupportedEvents_t		= GL4BaseModule::SupportedEvents_t;
		
		using ShadersMsgList_t		= MessageListFrom< GpuMsg::GetGLShaderModuleIDs >;

		using Descriptor			= ComputePipelineDescriptor;
		using PushConstants_t		= GpuMsg::GetGLPipelineLayoutPushConstants::PushConstants_t;


	// constants
	private:
		static const TypeIdList		_msgTypes;
		static const TypeIdList		_eventTypes;


	// variables
	private:
		GLuint			_pipelineId;
		GLuint			_programId;
		Descriptor		_descr;
		PushConstants_t	_pushConstants;


	// methods
	public:
		GL4ComputePipeline (UntypedID_t, GlobalSystemsRef gs, const CreateInfo::ComputePipeline &ci);
		~GL4ComputePipeline ();


	// message handlers
	private:
		bool _Link (const Message< ModuleMsg::Link > &);
		bool _Compose (const Message< ModuleMsg::Compose > &);
		bool _Delete (const Message< ModuleMsg::Delete > &);
		bool _AttachModule (const Message< ModuleMsg::AttachModule > &);
		bool _DetachModule (const Message< ModuleMsg::DetachModule > &);

		bool _GetGLComputePipelineID (const Message< GpuMsg::GetGLComputePipelineID > &);
		bool _GetComputePipelineDescriptor (const Message< GpuMsg::GetComputePipelineDescriptor > &);
		bool _GetPipelineLayoutDescriptor (const Message< GpuMsg::GetPipelineLayoutDescriptor > &);
		bool _GetGLPipelineLayoutPushConstants (const Message< GpuMsg::GetGLPipelineLayoutPushConstants > &);

	private:
		bool _IsCreated () const;

		bool _CreatePipeline ();
		void _DestroyPipeline ();
	};
//-----------------------------------------------------------------------------


	const TypeIdList	GL4ComputePipeline::_msgTypes{ UninitializedT< SupportedMessages_t >() };
	const TypeIdList	GL4ComputePipeline::_eventTypes{ UninitializedT< SupportedEvents_t >() };

/*
=================================================
	constructor
=================================================
*/
	GL4ComputePipeline::GL4ComputePipeline (UntypedID_t id, GlobalSystemsRef gs, const CreateInfo::ComputePipeline &ci) :
		GL4BaseModule( gs, ModuleConfig{ id, UMax }, &_msgTypes, &_eventTypes ),
		_pipelineId( 0 ),		_programId( 0 ),
		_descr( ci.descr )
	{
		SetDebugName( "GL4ComputePipeline" );

		_SubscribeOnMsg( this, &GL4ComputePipeline::_OnModuleAttached_Impl );
		_SubscribeOnMsg( this, &GL4ComputePipeline::_OnModuleDetached_Impl );
		_SubscribeOnMsg( this, &GL4ComputePipeline::_AttachModule );
		_SubscribeOnMsg( this, &GL4ComputePipeline::_DetachModule );
		_SubscribeOnMsg( this, &GL4ComputePipeline::_FindModule_Impl );
		_SubscribeOnMsg( this, &GL4ComputePipeline::_ModulesDeepSearch_Impl );
		_SubscribeOnMsg( this, &GL4ComputePipeline::_Link );
		_SubscribeOnMsg( this, &GL4ComputePipeline::_Compose );
		_SubscribeOnMsg( this, &GL4ComputePipeline::_Delete );
		_SubscribeOnMsg( this, &GL4ComputePipeline::_OnManagerChanged );
		_SubscribeOnMsg( this, &GL4ComputePipeline::_GetGLComputePipelineID );
		_SubscribeOnMsg( this, &GL4ComputePipeline::_GetComputePipelineDescriptor );
		_SubscribeOnMsg( this, &GL4ComputePipeline::_GetDeviceInfo );
		_SubscribeOnMsg( this, &GL4ComputePipeline::_GetGLDeviceInfo );
		_SubscribeOnMsg( this, &GL4ComputePipeline::_GetGLPrivateClasses );
		_SubscribeOnMsg( this, &GL4ComputePipeline::_GetPipelineLayoutDescriptor );
		_SubscribeOnMsg( this, &GL4ComputePipeline::_GetGLPipelineLayoutPushConstants );
		
		CHECK( _ValidateMsgSubscriptions() );

		_AttachSelfToManager( _GetGPUThread( ci.gpuThread ), UntypedID_t(0), true );
		
		MakePushConstantsCache( _descr.layout, OUT _pushConstants );
	}
	
/*
=================================================
	destructor
=================================================
*/
	GL4ComputePipeline::~GL4ComputePipeline ()
	{
		_DestroyPipeline();
	}
	
/*
=================================================
	_Link
=================================================
*/
	bool GL4ComputePipeline::_Link (const Message< ModuleMsg::Link > &msg)
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
	bool GL4ComputePipeline::_Compose (const Message< ModuleMsg::Compose > &msg)
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
	bool GL4ComputePipeline::_AttachModule (const Message< ModuleMsg::AttachModule > &msg)
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
	bool GL4ComputePipeline::_DetachModule (const Message< ModuleMsg::DetachModule > &msg)
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
	bool GL4ComputePipeline::_Delete (const Message< ModuleMsg::Delete > &msg)
	{
		_DestroyPipeline();
		
		_descr = Uninitialized;

		return Module::_Delete_Impl( msg );
	}
	
/*
=================================================
	_GetGLComputePipelineID
=================================================
*/
	bool GL4ComputePipeline::_GetGLComputePipelineID (const Message< GpuMsg::GetGLComputePipelineID > &msg)
	{
		msg->result.Set({ _pipelineId, _programId });
		return true;
	}
	
/*
=================================================
	_GetComputePipelineDescriptor
=================================================
*/
	bool GL4ComputePipeline::_GetComputePipelineDescriptor (const Message< GpuMsg::GetComputePipelineDescriptor > &msg)
	{
		msg->result.Set( _descr );
		return true;
	}
	
/*
=================================================
	_GetPipelineLayoutDescriptor
=================================================
*/
	bool GL4ComputePipeline::_GetPipelineLayoutDescriptor (const Message< GpuMsg::GetPipelineLayoutDescriptor > &msg)
	{
		msg->result.Set( _descr.layout );
		return true;
	}
	
/*
=================================================
	_GetGLPipelineLayoutPushConstants
=================================================
*/
	bool GL4ComputePipeline::_GetGLPipelineLayoutPushConstants (const Message< GpuMsg::GetGLPipelineLayoutPushConstants > &msg)
	{
		msg->result.Set( _pushConstants );
		return true;
	}

/*
=================================================
	_IsCreated
=================================================
*/
	bool GL4ComputePipeline::_IsCreated () const
	{
		return _pipelineId != 0;
	}
	
/*
=================================================
	_CreatePipeline
=================================================
*/
	bool GL4ComputePipeline::_CreatePipeline ()
	{
		CHECK_ERR( not _IsCreated() );
		
		// get shader
		ModulePtr	shaders;
		CHECK_ERR( shaders = GetModuleByMsg< ShadersMsgList_t >() );

		// get shader modules
		Message< GpuMsg::GetGLShaderModuleIDs >		req_shader_ids;
		SendTo( shaders, req_shader_ids );
		CHECK_ERR( req_shader_ids->result and not req_shader_ids->result->Empty() );

		GL_CALL( glGenProgramPipelines( 1, &_pipelineId ) );
		CHECK_ERR( _pipelineId != 0 );

		FOR( i, *req_shader_ids->result )
		{
			const auto&	sh = (*req_shader_ids->result)[i];

			GL_CALL( glUseProgramStages( _pipelineId, GL4Enum(EShader::bits().Set( sh.type )), sh.id ) );
		}

		// validate pipeline
		CHECK_ERR( ValidateGLPipeline( _pipelineId ) );

		GetDevice()->SetObjectName( _pipelineId, GetDebugName(), EGpuObject::Pipeline );
		return true;
	}
	
/*
=================================================
	_DestroyPipeline
=================================================
*/
	void GL4ComputePipeline::_DestroyPipeline ()
	{
		if ( _pipelineId != 0 )
		{
			GL_CALL( glDeleteProgramPipelines( 1, &_pipelineId ) );
		}

		_pipelineId	= 0;
	}

}	// PlatformGL
//-----------------------------------------------------------------------------

namespace Platforms
{
	ModulePtr OpenGLObjectsConstructor::CreateGL4GraphicsPipeline (ModuleMsg::UntypedID_t id, GlobalSystemsRef gs, const CreateInfo::GraphicsPipeline &msg)
	{
		return New< PlatformGL::GL4GraphicsPipeline >( id, gs, msg );
	}

	ModulePtr OpenGLObjectsConstructor::CreateGL4ComputePipeline (ModuleMsg::UntypedID_t id, GlobalSystemsRef gs, const CreateInfo::ComputePipeline &msg)
	{
		return New< PlatformGL::GL4ComputePipeline >( id, gs, msg );
	}

}	// Platforms
}	// Engine

#endif	// GRAPHICS_API_OPENGL