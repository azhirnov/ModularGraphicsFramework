// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Engine/Platforms/Shared/GPU/Pipeline.h"
#include "Engine/Platforms/Shared/GPU/RenderPass.h"
#include "Engine/Platforms/OpenGL/Impl/GL4BaseModule.h"
#include "Engine/Platforms/OpenGL/OpenGLObjectsConstructor.h"

#if defined( GRAPHICS_API_OPENGL )

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
											GpuMsg::GetPipelineLayoutDescriptor
										> >;

		using SupportedEvents_t		= GL4BaseModule::SupportedEvents_t;

		using Descriptor			= GraphicsPipelineDescriptor;
		using Programs_t			= StaticArray< GLuint, Platforms::EShader::_Count >;


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
		ModulePtr		_shaders;			// TOOD: use as attachment
		ModulePtr		_renderPass;		// TOOD: use as attachment


	// methods
	public:
		GL4GraphicsPipeline (GlobalSystemsRef gs, const CreateInfo::GraphicsPipeline &ci);
		~GL4GraphicsPipeline ();


	// message handlers
	private:
		bool _Compose (const Message< ModuleMsg::Compose > &);
		bool _Delete (const Message< ModuleMsg::Delete > &);
		bool _GetGLGraphicsPipelineID (const Message< GpuMsg::GetGLGraphicsPipelineID > &);
		bool _GetGraphicsPipelineDescriptor (const Message< GpuMsg::GetGraphicsPipelineDescriptor > &);
		bool _GetPipelineLayoutDescriptor (const Message< GpuMsg::GetPipelineLayoutDescriptor > &);
		
	private:
		bool _IsCreated () const;

		bool _CreatePipeline ();
		void _DestroyPipeline ();

		bool _CreateVertexArray ();
		void _DestroyVertexArray ();
		
		bool _ValidateRenderPass () const;
	};
	


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
											GpuMsg::GetPipelineLayoutDescriptor
										> >;

		using SupportedEvents_t		= GL4BaseModule::SupportedEvents_t;
		
		using Descriptor			= ComputePipelineDescriptor;


	// constants
	private:
		static const TypeIdList		_msgTypes;
		static const TypeIdList		_eventTypes;


	// variables
	private:
		GLuint			_pipelineId;
		GLuint			_programId;
		Descriptor		_descr;
		ModulePtr		_shaders;		// TOOD: use as attachment


	// methods
	public:
		GL4ComputePipeline (GlobalSystemsRef gs, const CreateInfo::ComputePipeline &ci);
		~GL4ComputePipeline ();


	// message handlers
	private:
		bool _Compose (const Message< ModuleMsg::Compose > &);
		bool _Delete (const Message< ModuleMsg::Delete > &);
		bool _GetGLComputePipelineID (const Message< GpuMsg::GetGLComputePipelineID > &);
		bool _GetComputePipelineDescriptor (const Message< GpuMsg::GetComputePipelineDescriptor > &);
		bool _GetPipelineLayoutDescriptor (const Message< GpuMsg::GetPipelineLayoutDescriptor > &);

	private:
		bool _IsCreated () const;

		bool _CreatePipeline ();
		void _DestroyPipeline ();
	};
//-----------------------------------------------------------------------------


	
	const TypeIdList	GL4GraphicsPipeline::_msgTypes{ UninitializedT< SupportedMessages_t >() };
	const TypeIdList	GL4GraphicsPipeline::_eventTypes{ UninitializedT< SupportedEvents_t >() };

/*
=================================================
	constructor
=================================================
*/
	GL4GraphicsPipeline::GL4GraphicsPipeline (GlobalSystemsRef gs, const CreateInfo::GraphicsPipeline &ci) :
		GL4BaseModule( gs, ModuleConfig{ GLGraphicsPipelineModuleID, UMax }, &_msgTypes, &_eventTypes ),
		_pipelineId( 0 ),				_vertexAttribs( 0 ),
		_descr( ci.descr ),				_shaders( ci.shaders ),
		_renderPass( ci.renderPass )
	{
		SetDebugName( "GL4GraphicsPipeline" );

		_SubscribeOnMsg( this, &GL4GraphicsPipeline::_OnModuleAttached_Impl );
		_SubscribeOnMsg( this, &GL4GraphicsPipeline::_OnModuleDetached_Impl );
		_SubscribeOnMsg( this, &GL4GraphicsPipeline::_AttachModule_Impl );
		_SubscribeOnMsg( this, &GL4GraphicsPipeline::_DetachModule_Impl );
		_SubscribeOnMsg( this, &GL4GraphicsPipeline::_FindModule_Impl );
		_SubscribeOnMsg( this, &GL4GraphicsPipeline::_ModulesDeepSearch_Impl );
		_SubscribeOnMsg( this, &GL4GraphicsPipeline::_Link_Impl );
		_SubscribeOnMsg( this, &GL4GraphicsPipeline::_Compose );
		_SubscribeOnMsg( this, &GL4GraphicsPipeline::_Delete );
		_SubscribeOnMsg( this, &GL4GraphicsPipeline::_OnManagerChanged );
		_SubscribeOnMsg( this, &GL4GraphicsPipeline::_GetGLGraphicsPipelineID );
		_SubscribeOnMsg( this, &GL4GraphicsPipeline::_GetGraphicsPipelineDescriptor );
		_SubscribeOnMsg( this, &GL4GraphicsPipeline::_GetDeviceInfo );
		_SubscribeOnMsg( this, &GL4GraphicsPipeline::_GetGLDeviceInfo );
		_SubscribeOnMsg( this, &GL4GraphicsPipeline::_GetGLPrivateClasses );
		_SubscribeOnMsg( this, &GL4GraphicsPipeline::_GetPipelineLayoutDescriptor );
		
		CHECK( _ValidateMsgSubscriptions() );

		_AttachSelfToManager( ci.gpuThread, GLThreadModuleID, true );
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

		CHECK( _SetState( EState::ComposedImmutable ) );
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
	_IsCreated
=================================================
*/
	bool GL4GraphicsPipeline::_IsCreated () const
	{
		return _pipelineId != 0;
	}
	
/*
=================================================
	_CreatePipeline
=================================================
*/
	bool GL4GraphicsPipeline::_CreatePipeline ()
	{
		CHECK_ERR( not _IsCreated() );
		
		// validate render pass
		if ( not _renderPass )
		{
			_renderPass		= GetDevice()->GetDefaultRenderPass();
			_descr.subpass	= 0;

			LOG( "used default render pass", ELog::Debug );
		}
		CHECK_ERR( _ValidateRenderPass() );

		// get shader modules
		Message< GpuMsg::GetGLShaderModuleIDs >		req_shader_ids;
		SendTo( _shaders, req_shader_ids );
		CHECK_ERR( req_shader_ids->result and not req_shader_ids->result->Empty() );

		GL_CALL( glGenProgramPipelines( 1, &_pipelineId ) );
		CHECK_ERR( _pipelineId != 0 );

		FOR( i, *req_shader_ids->result )
		{
			const auto&	sh = (*req_shader_ids->result)[i];

			GL_CALL( glUseProgramStages( _pipelineId, GL4Enum(EShader::bits().Set( sh.type )), sh.id ) );
		}

		// validate pipeline
		GL_CALL( glValidateProgramPipeline( _pipelineId ) );

		GLint	status = 0;
		GL_CALL( glGetProgramPipelineiv( _pipelineId, GL_VALIDATE_STATUS, &status ) );

		const bool	valid = !!status;

		if ( not valid )
		{
			GLint	log_len = 0;
			GL_CALL( glGetProgramPipelineiv( _pipelineId, GL_INFO_LOG_LENGTH, &log_len ) );

			String	log;	log.Reserve( log_len + 64 );

			GL_CALL( glGetProgramPipelineInfoLog( _pipelineId, log_len, &log_len, log.ptr() ) );

			LOG( ("OpenGL pipeline compilation failed: "_str << log).cstr(), ELog::Warning );
		}

		CHECK_ERR( valid );

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
		_descr		= Uninitialized;
		_shaders	= null;
		_renderPass	= null;
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
			bool					norm;
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
	bool GL4GraphicsPipeline::_ValidateRenderPass () const
	{
		Message< GpuMsg::GetRenderPassDescriptor >	req_descr;
		_renderPass->Send( req_descr );

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


	
	const TypeIdList	GL4ComputePipeline::_msgTypes{ UninitializedT< SupportedMessages_t >() };
	const TypeIdList	GL4ComputePipeline::_eventTypes{ UninitializedT< SupportedEvents_t >() };

/*
=================================================
	constructor
=================================================
*/
	GL4ComputePipeline::GL4ComputePipeline (GlobalSystemsRef gs, const CreateInfo::ComputePipeline &ci) :
		GL4BaseModule( gs, ModuleConfig{ GLComputePipelineModuleID, UMax }, &_msgTypes, &_eventTypes ),
		_pipelineId( 0 ),		_programId( 0 ),
		_descr( ci.descr ),		_shaders( ci.shaders )
	{
		SetDebugName( "GL4ComputePipeline" );

		_SubscribeOnMsg( this, &GL4ComputePipeline::_OnModuleAttached_Impl );
		_SubscribeOnMsg( this, &GL4ComputePipeline::_OnModuleDetached_Impl );
		_SubscribeOnMsg( this, &GL4ComputePipeline::_AttachModule_Impl );
		_SubscribeOnMsg( this, &GL4ComputePipeline::_DetachModule_Impl );
		_SubscribeOnMsg( this, &GL4ComputePipeline::_FindModule_Impl );
		_SubscribeOnMsg( this, &GL4ComputePipeline::_ModulesDeepSearch_Impl );
		_SubscribeOnMsg( this, &GL4ComputePipeline::_Link_Impl );
		_SubscribeOnMsg( this, &GL4ComputePipeline::_Compose );
		_SubscribeOnMsg( this, &GL4ComputePipeline::_Delete );
		_SubscribeOnMsg( this, &GL4ComputePipeline::_OnManagerChanged );
		_SubscribeOnMsg( this, &GL4ComputePipeline::_GetGLComputePipelineID );
		_SubscribeOnMsg( this, &GL4ComputePipeline::_GetComputePipelineDescriptor );
		_SubscribeOnMsg( this, &GL4ComputePipeline::_GetDeviceInfo );
		_SubscribeOnMsg( this, &GL4ComputePipeline::_GetGLDeviceInfo );
		_SubscribeOnMsg( this, &GL4ComputePipeline::_GetGLPrivateClasses );
		_SubscribeOnMsg( this, &GL4ComputePipeline::_GetPipelineLayoutDescriptor );
		
		CHECK( _ValidateMsgSubscriptions() );

		_AttachSelfToManager( ci.gpuThread, GLThreadModuleID, true );
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

		CHECK( _SetState( EState::ComposedImmutable ) );
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
		TODO( "" );

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
		_descr		= Uninitialized;
		_shaders	= null;
	}

}	// PlatformGL
//-----------------------------------------------------------------------------

namespace Platforms
{
	ModulePtr OpenGLObjectsConstructor::CreateGL4GraphicsPipeline (GlobalSystemsRef gs, const CreateInfo::GraphicsPipeline &msg)
	{
		return New< PlatformGL::GL4GraphicsPipeline >( gs, msg );
	}

	ModulePtr OpenGLObjectsConstructor::CreateGL4ComputePipeline (GlobalSystemsRef gs, const CreateInfo::ComputePipeline &msg)
	{
		return New< PlatformGL::GL4ComputePipeline >( gs, msg );
	}

}	// Platforms
}	// Engine

#endif	// GRAPHICS_API_OPENGL
