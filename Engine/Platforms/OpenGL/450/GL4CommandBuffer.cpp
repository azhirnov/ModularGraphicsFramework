// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Core/Config/Engine.Config.h"

#ifdef GRAPHICS_API_OPENGL

#include "Engine/Platforms/Public/GPU/CommandBuffer.h"
#include "Engine/Platforms/Public/GPU/Framebuffer.h"
#include "Engine/Platforms/Public/GPU/Image.h"
#include "Engine/Platforms/Public/GPU/Buffer.h"
#include "Engine/Platforms/Public/GPU/Memory.h"
#include "Engine/Platforms/Public/GPU/Pipeline.h"
#include "Engine/Platforms/Public/GPU/RenderPass.h"
#include "Engine/Platforms/Public/GPU/Query.h"
#include "Engine/Platforms/OpenGL/450/GL4BaseModule.h"
#include "Engine/Platforms/OpenGL/450/GL4ResourceCache.h"
#include "Engine/Platforms/OpenGL/OpenGLObjectsConstructor.h"
#include "Engine/Platforms/Public/Tools/ImageUtils.h"

namespace Engine
{
namespace PlatformGL
{
	using namespace gl;

	
	//
	// OpenGL Command Buffer
	//

	class GL4CommandBuffer final : public GL4BaseModule
	{
	// types
	private:
		using SupportedMessages_t	= GL4BaseModule::SupportedMessages_t::Append< MessageListFrom<
											GpuMsg::GetCommandBufferDescription,
											GpuMsg::SetCommandBufferState,
											GpuMsg::GetCommandBufferState,
											GpuMsg::SetCommandBufferDependency,
											GpuMsg::SetGLCommandBufferQueue,
											GpuMsg::ExecuteGLCommandBuffer
										> >;

		using SupportedEvents_t		= GL4BaseModule::SupportedEvents_t::Append< MessageListFrom<
											GpuMsg::OnCommandBufferStateChanged
										> >;

		using UsedResources_t		= Set< ModulePtr >;
		using CommandArray_t		= Array< GpuMsg::SetGLCommandBufferQueue::Command >;
		using CmdDataTypes_t		= GpuMsg::SetGLCommandBufferQueue::Data_t::TypeList_t;

		using ERecordingState		= GpuMsg::SetCommandBufferState::EState;

		struct GLBuffer : CompileTime::PODStruct
		{
			GLuint			id	= 0;
			BytesU			offset;
			Bytes<uint>		stride;

			GLBuffer () {}
			GLBuffer (GLuint id, BytesU off) : id(id), offset(off) {}
		};

		using PipelineStages_t		= StaticArray< GLuint, EShader::_Count >;
		using VertexBuffers_t		= StaticArray< GLBuffer, GlobalConst::GAPI_MaxAttribs >;
		using RenderPassData_t		= GpuMsg::GetGLRenderPassID::RenderPassID;
		using ClearValues_t			= GpuMsg::CmdBeginRenderPass::ClearValues_t;
		using RenderSubpasses_t		= RenderPassDescription::Subpasses_t;
		using Viewports_t			= StaticArray< GpuMsg::CmdSetViewport::Viewport, GlobalConst::GAPI_MaxColorBuffers >;
		using Scissors_t			= StaticArray< RectU, GlobalConst::GAPI_MaxColorBuffers >;
		
		using ImageUtils			= PlatformTools::ImageUtils;


	// constants
	private:
		static const TypeIdList		_eventTypes;

		static const GLenum			_debugMarkerSeverity = GL_DEBUG_SEVERITY_LOW;


	// variables
	private:
		ModulePtr					_tempBuffer;		// for UpdateBuffer command
		ModulePtr					_pushConstBuffer;	// for PushConstant command

		// temporary framebuffer for blit image command
		GLuint						_srcBlitFramebuffer;
		GLuint						_dstBlitFramebuffer;

		CommandBufferDescription	_descr;
		CommandArray_t				_commands;
		UsedResources_t				_resources;
		ERecordingState				_recordingState;

		// states
		PipelineStages_t			_pipelineStages;
		GLuint						_pipelineObj;
		ModulePtr					_resourceTable;
		GraphicsPipelineDescription	_graphicsPipelineDescr;
		ComputePipelineDescription	_computePipelineDescr;
		RenderPassData_t			_renderPassData;
		RenderSubpasses_t			_renderSubpasses;

		GLuint						_vertexAttribs;
		VertexBuffers_t				_vertexBuffers;
		GLBuffer					_indexBuffer;
		EIndex::type				_indexType;
		EPrimitive::type			_primitive;

		uint3						_framebufferSize;		// width, height, layers
		RectU						_renderPassArea;
		Viewports_t					_viewports;
		Scissors_t					_scissors;


	// methods
	public:
		GL4CommandBuffer (UntypedID_t, GlobalSystemsRef gs, const CreateInfo::GpuCommandBuffer &ci);
		~GL4CommandBuffer ();


	// message handlers
	private:
		bool _Compose (const ModuleMsg::Compose &);
		bool _Delete (const ModuleMsg::Delete &);
		bool _SetGLCommandBufferQueue (const GpuMsg::SetGLCommandBufferQueue &);
		bool _GetCommandBufferDescription (const GpuMsg::GetCommandBufferDescription &);
		bool _SetCommandBufferDependency (const GpuMsg::SetCommandBufferDependency &);
		bool _SetCommandBufferState (const GpuMsg::SetCommandBufferState &);
		bool _GetCommandBufferState (const GpuMsg::GetCommandBufferState &);
		bool _ExecuteGLCommandBuffer (const GpuMsg::ExecuteGLCommandBuffer &);

	private:
		bool _Submit ();
		bool _BeginRecording ();
		bool _EndRecording ();
		bool _ResetCmdBuffer ();
		bool _OnCompleted ();

		void _ChangeState (ERecordingState newState);

		bool _Initialize ();
		void _ClearStates ();

		bool _DestroyResources ();

		bool _PrepareForDraw ();
		bool _PrepareForCompute ();

		void _ApplyColorBufferStates ();
		void _ApplyDepthState ();
		void _ApplyStencilState ();
		void _ApplyInputAssemblyState ();
		void _ApplyRasterizationState ();
		void _ApplyMultisampleState ();
		
		bool _ClearRenderPassAttachment (const ClearValues_t &value);
		bool _InvalidateRenderPassAttachment (ArrayCRef<GLenum> buffers);
		void _SetViewports ();
		
		bool _CopyImageToBuffer200 (const GpuMsg::CmdCopyImageToBuffer &data);
		bool _CopyImageToBuffer450 (const GpuMsg::CmdCopyImageToBuffer &data);

		static void _ValidateDescription (INOUT CommandBufferDescription &descr);

	public:
		bool operator () (const GpuMsg::CmdSetViewport &);
		bool operator () (const GpuMsg::CmdSetScissor &);
		bool operator () (const GpuMsg::CmdSetDepthBounds &);
		bool operator () (const GpuMsg::CmdSetBlendColor &);
		bool operator () (const GpuMsg::CmdSetDepthBias &);
		bool operator () (const GpuMsg::CmdSetLineWidth &);
		bool operator () (const GpuMsg::CmdSetStencilCompareMask &);
		bool operator () (const GpuMsg::CmdSetStencilWriteMask &);
		bool operator () (const GpuMsg::CmdSetStencilReference &);
		bool operator () (const GpuMsg::CmdBeginRenderPass &);
		bool operator () (const GpuMsg::CmdEndRenderPass &);
		bool operator () (const GpuMsg::CmdBindGraphicsPipeline &);
		bool operator () (const GpuMsg::CmdBindComputePipeline &);
		bool operator () (const GpuMsg::CmdBindVertexBuffers &);
		bool operator () (const GpuMsg::CmdBindIndexBuffer &);
		bool operator () (const GpuMsg::CmdDraw &);
		bool operator () (const GpuMsg::CmdDrawIndexed &);
		bool operator () (const GpuMsg::CmdDrawIndirect &);
		bool operator () (const GpuMsg::CmdDrawIndexedIndirect &);
		bool operator () (const GpuMsg::CmdDispatch &);
		bool operator () (const GpuMsg::CmdDispatchIndirect &);
		bool operator () (const GpuMsg::CmdExecute &);
		bool operator () (const GpuMsg::CmdBindGraphicsResourceTable &);
		bool operator () (const GpuMsg::CmdBindComputeResourceTable &);
		bool operator () (const GpuMsg::CmdCopyBuffer &);
		bool operator () (const GpuMsg::CmdCopyImage &);
		bool operator () (const GpuMsg::CmdCopyBufferToImage &);
		bool operator () (const GpuMsg::CmdCopyImageToBuffer &);
		bool operator () (const GpuMsg::CmdBlitImage &);
		bool operator () (const GpuMsg::CmdResolveImage &);
		bool operator () (const GpuMsg::CmdBlitGLFramebuffers &);
		bool operator () (const GpuMsg::GLCmdUpdateBuffer &);
		bool operator () (const GpuMsg::CmdFillBuffer &);
		bool operator () (const GpuMsg::CmdClearAttachments &);
		bool operator () (const GpuMsg::CmdClearColorImage &);
		bool operator () (const GpuMsg::CmdClearDepthStencilImage &);
		bool operator () (const GpuMsg::CmdPipelineBarrier &);
		bool operator () (const GpuMsg::GLCmdPushConstants &);
		bool operator () (const GpuMsg::CmdDebugMarker &);
		bool operator () (const GpuMsg::CmdPushDebugGroup &);
		bool operator () (const GpuMsg::CmdPopDebugGroup &);
		bool operator () (const GpuMsg::CmdBeginQuery &);
		bool operator () (const GpuMsg::CmdEndQuery &);
		bool operator () (const GpuMsg::CmdCopyQueryPoolResults &);
		bool operator () (const GpuMsg::CmdWriteTimestamp &);
		bool operator () (const GpuMsg::CmdResetQueryPool &);
	};
//-----------------------------------------------------------------------------



	const TypeIdList	GL4CommandBuffer::_eventTypes{ UninitializedT< SupportedEvents_t >() };

/*
=================================================
	constructor
=================================================
*/
	GL4CommandBuffer::GL4CommandBuffer (UntypedID_t id, GlobalSystemsRef gs, const CreateInfo::GpuCommandBuffer &ci) :
		GL4BaseModule( gs, ModuleConfig{ id, UMax }, &_eventTypes ),
		_srcBlitFramebuffer( 0 ),		_dstBlitFramebuffer( 0 ),
		_descr( ci.descr ),				_recordingState( ERecordingState::Deleted ),
		_pipelineObj( 0 ),				_vertexAttribs( 0 ),
		_indexType( EIndex::Unknown ),	_primitive( EPrimitive::Unknown )
	{
		SetDebugName( "GL4CommandBuffer" );

		_SubscribeOnMsg( this, &GL4CommandBuffer::_OnModuleAttached_Impl );
		_SubscribeOnMsg( this, &GL4CommandBuffer::_OnModuleDetached_Impl );
		_SubscribeOnMsg( this, &GL4CommandBuffer::_AttachModule_Impl );
		_SubscribeOnMsg( this, &GL4CommandBuffer::_DetachModule_Impl );
		_SubscribeOnMsg( this, &GL4CommandBuffer::_FindModule_Impl );
		_SubscribeOnMsg( this, &GL4CommandBuffer::_ModulesDeepSearch_Impl );
		_SubscribeOnMsg( this, &GL4CommandBuffer::_Link_Impl );
		_SubscribeOnMsg( this, &GL4CommandBuffer::_Compose );
		_SubscribeOnMsg( this, &GL4CommandBuffer::_Delete );
		_SubscribeOnMsg( this, &GL4CommandBuffer::_OnManagerChanged );
		_SubscribeOnMsg( this, &GL4CommandBuffer::_GetCommandBufferDescription );
		_SubscribeOnMsg( this, &GL4CommandBuffer::_GetDeviceInfo );
		_SubscribeOnMsg( this, &GL4CommandBuffer::_GetGLDeviceInfo );
		_SubscribeOnMsg( this, &GL4CommandBuffer::_GetGLPrivateClasses );
		_SubscribeOnMsg( this, &GL4CommandBuffer::_SetGLCommandBufferQueue );
		_SubscribeOnMsg( this, &GL4CommandBuffer::_SetCommandBufferDependency );
		_SubscribeOnMsg( this, &GL4CommandBuffer::_SetCommandBufferState );
		_SubscribeOnMsg( this, &GL4CommandBuffer::_GetCommandBufferState );
		_SubscribeOnMsg( this, &GL4CommandBuffer::_ExecuteGLCommandBuffer );

		ASSERT( _ValidateMsgSubscriptions< SupportedMessages_t >() );

		_AttachSelfToManager( _GetGPUThread( ci.gpuThread ), UntypedID_t(0), true );

		_ValidateDescription( _descr );
	}
	
/*
=================================================
	destructor
=================================================
*/
	GL4CommandBuffer::~GL4CommandBuffer ()
	{
	}
	
/*
=================================================
	_Compose
=================================================
*/
	bool GL4CommandBuffer::_Compose (const ModuleMsg::Compose &msg)
	{
		if ( _IsComposedState( GetState() ) )
			return true;	// already composed

		CHECK_ERR( GetState() == EState::Linked );
		
		CHECK_COMPOSING( _Initialize() );

		_SendForEachAttachments( msg );
		
		// very paranoic check
		CHECK( _ValidateAllSubscriptions() );

		CHECK( _SetState( EState::ComposedMutable ) );
		
		_SendUncheckedEvent( ModuleMsg::AfterCompose{} );
		return true;
	}
	
/*
=================================================
	_Initialize
=================================================
*/
	bool GL4CommandBuffer::_Initialize ()
	{
		_ChangeState( ERecordingState::Initial );
		return true;
	}
	
/*
=================================================
	_ValidateDescription
=================================================
*/
	void GL4CommandBuffer::_ValidateDescription (INOUT CommandBufferDescription &)
	{
	}

/*
=================================================
	_Delete
=================================================
*/
	bool GL4CommandBuffer::_Delete (const ModuleMsg::Delete &msg)
	{
		_descr = Uninitialized;

		_ChangeState( ERecordingState::Deleted );

		_resources.Clear();
		_commands.Clear();

		_DestroyResources();

		return Module::_Delete_Impl( msg );
	}

/*
=================================================
	_SetGLCommandBufferQueue
=================================================
*/
	bool GL4CommandBuffer::_SetGLCommandBufferQueue (const GpuMsg::SetGLCommandBufferQueue &msg)
	{
		_commands = RVREF(msg.commands.Get());
		
		// TODO: optimize
		if ( _tempBuffer )
		{
			_tempBuffer->Send( ModuleMsg::Delete{} );
			_tempBuffer = null;
		}

		if ( not msg.bufferData.Empty() )
		{
			CHECK_ERR( GlobalSystems()->modulesFactory->Create(
								GLBufferModuleID,
								GlobalSystems(),
								CreateInfo::GpuBuffer{
									BufferDescription{ msg.bufferData.Size(), EBufferUsage::TransferSrc },
									null,
									EGpuMemory::CoherentWithCPU,
									EMemoryAccess::GpuRead | EMemoryAccess::CpuWrite
								},
								OUT _tempBuffer ) );

			ModuleUtils::Initialize({ _tempBuffer });

			CHECK( _tempBuffer->Send( GpuMsg::WriteToGpuMemory{ msg.bufferData }) );
		}
		return true;
	}

/*
=================================================
	_GetCommandBufferDescription
=================================================
*/
	bool GL4CommandBuffer::_GetCommandBufferDescription (const GpuMsg::GetCommandBufferDescription &msg)
	{
		msg.result.Set( _descr );
		return true;
	}
	
/*
=================================================
	_SetCommandBufferDependency
=================================================
*/
	bool GL4CommandBuffer::_SetCommandBufferDependency (const GpuMsg::SetCommandBufferDependency &msg)
	{
		_resources = RVREF( msg.resources.Get() );
		return true;
	}

/*
=================================================
	_GetCommandBufferDescription
=================================================
*/
	bool GL4CommandBuffer::_SetCommandBufferState (const GpuMsg::SetCommandBufferState &msg)
	{
		switch ( msg.newState )
		{
			case ERecordingState::Initial :		CHECK( _ResetCmdBuffer() );	break;
			case ERecordingState::Recording :	CHECK( _BeginRecording() );	break;
			case ERecordingState::Executable :	CHECK( _EndRecording() );	break;
			case ERecordingState::Pending :		CHECK( _Submit() );			break;
			case ERecordingState::Completed :	CHECK( _OnCompleted() );	break;
			default :							WARNING( "unsupported state" );
		}
		return true;
	}
	
/*
=================================================
	_GetCommandBufferDescription
=================================================
*/
	bool GL4CommandBuffer::_GetCommandBufferState (const GpuMsg::GetCommandBufferState &msg)
	{
		msg.result.Set( _recordingState );
		return true;
	}
	
/*
=================================================
	_ExecuteGLCommandBuffer
=================================================
*/
	bool GL4CommandBuffer::_ExecuteGLCommandBuffer (const GpuMsg::ExecuteGLCommandBuffer &)
	{
		CHECK_ERR( _recordingState == ERecordingState::Pending );

		FOR( i, _commands ) {
			_commands[i].data.Accept( *this );
		}

		_ClearStates();
		return true;
	}

/*
=================================================
	_ResetCmdBuffer
=================================================
*/
	bool GL4CommandBuffer::_ResetCmdBuffer ()
	{
		if ( _recordingState == ERecordingState::Initial )
			return true;

		CHECK_ERR( _recordingState == ERecordingState::Pending );

		_ChangeState( ERecordingState::Initial );
		_resources.Clear();
		_commands.Clear();

		return true;
	}

/*
=================================================
	_Submit
=================================================
*/
	bool GL4CommandBuffer::_Submit ()
	{
		CHECK_ERR( _recordingState == ERecordingState::Executable );

		_ChangeState( ERecordingState::Pending );
		return true;
	}
	
/*
=================================================
	_BeginRecording
=================================================
*/
	bool GL4CommandBuffer::_BeginRecording ()
	{
		CHECK_ERR( _recordingState == ERecordingState::Initial or
				  ((_recordingState == ERecordingState::Executable or
					_recordingState == ERecordingState::Pending) and
				    _descr.flags[ECmdBufferCreate::ImplicitResetable]) );

		if ( _recordingState == ERecordingState::Pending )
		{
			_resources.Clear();
			_commands.Clear();
		}

		_ChangeState( ERecordingState::Recording );
		return true;
	}
	
/*
=================================================
	_EndRecording
=================================================
*/
	bool GL4CommandBuffer::_EndRecording ()
	{
		CHECK_ERR( _recordingState == ERecordingState::Recording );

		_ChangeState( ERecordingState::Executable );
		return true;
	}
	
/*
=================================================
	_OnCompleted
=================================================
*/
	bool GL4CommandBuffer::_OnCompleted ()
	{
		CHECK_ERR( _recordingState == ERecordingState::Pending );

		_resources.Clear();
		_commands.Clear();

		_ChangeState( ERecordingState::Executable );
		return true;
	}
		
/*
=================================================
	_ChangeState
=================================================
*/
	void GL4CommandBuffer::_ChangeState (ERecordingState newState)
	{
		const auto	old_state = _recordingState;

		_recordingState = newState;

		_SendEvent( GpuMsg::OnCommandBufferStateChanged{ old_state, newState });
	}
	
/*
=================================================
	_ClearStates
=================================================
*/
	void GL4CommandBuffer::_ClearStates ()
	{
		_pipelineStages.Clear();
		_vertexBuffers.Clear();

		_indexBuffer	= Uninitialized;

		_pipelineObj	= 0;
		_vertexAttribs	= 0;
		_resourceTable	= null;

		_indexType		= EIndex::Unknown;
		_primitive		= EPrimitive::Unknown;

		_graphicsPipelineDescr	= Uninitialized;
		_computePipelineDescr	= Uninitialized;
		_renderPassData			= Uninitialized;
	}
	
/*
=================================================
	_DestroyResources
=================================================
*/
	bool GL4CommandBuffer::_DestroyResources ()
	{
		if ( _tempBuffer )
		{
			_tempBuffer->Send( ModuleMsg::Delete{} );
			_tempBuffer = null;
		}

		if ( _srcBlitFramebuffer != 0 )
		{
			GL_CALL( glDeleteFramebuffers( 1, &_srcBlitFramebuffer ) );
			_srcBlitFramebuffer = 0;
		}

		if ( _dstBlitFramebuffer != 0 )
		{
			GL_CALL( glDeleteFramebuffers( 1, &_dstBlitFramebuffer ) );
			_dstBlitFramebuffer = 0;
		}
		return true;
	}

/*
=================================================
	_PrepareForDraw
=================================================
*/
	bool GL4CommandBuffer::_PrepareForDraw ()
	{
		// validate resources
		// TODO
		// compare vertex attribs from pipeline and ...

		// bind or create vertex attribs
		CHECK_ERR( _vertexAttribs != 0 );
		GL_CALL( glBindVertexArray( _vertexAttribs ) );

		// bind vertex buffers
		const auto	bindings = _graphicsPipelineDescr.vertexInput.Bindings();
		
		FOR( i, bindings ) {
			_vertexBuffers[ bindings[i].second.index ].stride = bindings[i].second.stride;
		}
		FOR( i, _vertexBuffers )
		{
			if ( _vertexBuffers[i].id == 0 )
				continue;

			GL_CALL( glBindVertexBuffer( GLuint(i), _vertexBuffers[i].id, GLintptr(_vertexBuffers[i].offset),
										 GLsizei(_vertexBuffers[i].stride) ) );
		}

		// bind index buffer
		GL_CALL( glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, _indexBuffer.id ) );

		// set render state
		_ApplyColorBufferStates();
		_ApplyDepthState();
		_ApplyStencilState();
		_ApplyInputAssemblyState();
		_ApplyRasterizationState();
		_ApplyMultisampleState();
		_SetViewports();

		// bind program pipeline
		CHECK_ERR( _pipelineObj != 0 );
		GL_CALL( glBindProgramPipeline( _pipelineObj ) );

		// set uniforms
		CHECK_ERR( _resourceTable );
		_resourceTable->Send( GpuMsg::GLPipelineResourceTableApply{ _pipelineStages } );
		return true;
	}
	
/*
=================================================
	_ApplyColorBufferStates
=================================================
*/
	void GL4CommandBuffer::_ApplyColorBufferStates ()
	{
		const auto&	state				= _graphicsPipelineDescr.renderState.color;
		const bool	logic_op_enabled	= ( state.logicOp != ELogicOp::None );
		
		if ( logic_op_enabled )
		{
			GL_CALL( glEnable( GL_COLOR_LOGIC_OP ) );
			GL_CALL( glLogicOp( GL4Enum( state.logicOp ) ) );
			return;
		}

		GL_CALL( glDisable( GL_COLOR_LOGIC_OP ) );
		GL_CALL( glBlendColor( state.blendColor.R(), state.blendColor.G(),
								state.blendColor.B(), state.blendColor.A() ) );

		FOR( i, state.buffers )
		{
			const uint	index	= uint(i);
			const auto&	cb		= state.buffers[i];
			
			if ( not cb.blend ) {
				GL_CALL( glDisablei( GL_BLEND, index ) );
				continue;
			}

			GL_CALL( glEnablei( GL_BLEND, index ) );
			GL_CALL( glColorMaski( index, cb.colorMask.x, cb.colorMask.y, cb.colorMask.z, cb.colorMask.w ) );
			GL_CALL( glBlendEquationSeparatei( index, GL4Enum( cb.blendMode.color ), GL4Enum( cb.blendMode.alpha ) ) );
			GL_CALL( glBlendFuncSeparatei( index, GL4Enum( cb.blendFuncSrc.color ), GL4Enum( cb.blendFuncDst.color ),
											GL4Enum( cb.blendFuncSrc.alpha ), GL4Enum( cb.blendFuncDst.alpha ) ) );
		}
	}
	
/*
=================================================
	_ApplyDepthState
=================================================
*/
	void GL4CommandBuffer::_ApplyDepthState ()
	{
		const auto&	state	= _graphicsPipelineDescr.renderState.depth;

		if ( not state.test ) {
			GL_CALL( glDisable( GL_DEPTH_TEST ) );
			return;		// TODO: enable depth write
		}

		GL_CALL( glEnable( GL_DEPTH_TEST ) );
		GL_CALL( glDepthMask( state.write ) );
		GL_CALL( glDepthFunc( GL4Enum( state.func ) ) );
	}
	
/*
=================================================
	_ApplyStencilState
=================================================
*/
	void GL4CommandBuffer::_ApplyStencilState ()
	{
		const auto&	state	= _graphicsPipelineDescr.renderState.stencil;
		
		if ( not state.enabled ) {
			GL_CALL( glDisable( GL_STENCIL_TEST ) );
			return;
		}

		GL_CALL( glEnable( GL_STENCIL_TEST ) );

		GL_CALL( glStencilMaskSeparate( GL_FRONT, state.front.mask ) );
		GL_CALL( glStencilFuncSeparate( GL_FRONT, GL4Enum( state.front.func ), state.front.funcRef, state.front.funcMask ) );
		GL_CALL( glStencilOpSeparate( GL_FRONT, GL4Enum( state.front.sfail ), GL4Enum( state.front.dfail ), GL4Enum( state.front.dppass ) ) );
		
		GL_CALL( glStencilMaskSeparate( GL_BACK, state.back.mask ) );
		GL_CALL( glStencilFuncSeparate( GL_BACK, GL4Enum( state.back.func ), state.back.funcRef, state.back.funcMask ) );
		GL_CALL( glStencilOpSeparate( GL_BACK, GL4Enum( state.back.sfail ), GL4Enum( state.back.dfail ), GL4Enum( state.back.dppass ) ) );
	}
	
/*
=================================================
	_ApplyInputAssemblyState
=================================================
*/
	void GL4CommandBuffer::_ApplyInputAssemblyState ()
	{
		const auto&	state	= _graphicsPipelineDescr.renderState.inputAssembly;

		_primitive = state.topology;

		if ( not state.primitiveRestart ) {
			GL_CALL( glDisable( GL_PRIMITIVE_RESTART ) );
			return;
		}

		GL_CALL( glEnable( GL_PRIMITIVE_RESTART ) );

		switch ( _indexType ) {
			case EIndex::Unknown :	break;
			case EIndex::UShort :	GL_CALL( glPrimitiveRestartIndex( ushort(-1) ) );	break;
			case EIndex::UInt :		GL_CALL( glPrimitiveRestartIndex( uint(-1) ) );		break;
			default :				WARNING( "unknown index type" );
		}
	}
	
/*
=================================================
	_ApplyRasterizationState
=================================================
*/
	void GL4CommandBuffer::_ApplyRasterizationState ()
	{
		const auto&	state	= _graphicsPipelineDescr.renderState.rasterization;
		
		if ( state.depthClamp )		GL_CALL( glEnable( GL_DEPTH_CLAMP ) )
		else						GL_CALL( glDisable( GL_DEPTH_CLAMP ) );

		if ( state.rasterizerDiscard )	GL_CALL( glEnable( GL_RASTERIZER_DISCARD ) )
		else							GL_CALL( glDisable( GL_RASTERIZER_DISCARD ) );

		GL_CALL( glFrontFace( state.frontFaceCCW ? GL_CCW : GL_CW ) );

		switch ( state.cullMode ) {
			case EPolygonFace::Front :	GL_CALL( glEnable( GL_CULL_FACE ) );	GL_CALL( glCullFace( GL_FRONT ) );	break;
			case EPolygonFace::Back :	GL_CALL( glEnable( GL_CULL_FACE ) );	GL_CALL( glCullFace( GL_BACK ) );	break;
			case EPolygonFace::None :	GL_CALL( glDisable( GL_CULL_FACE ) );	break;
			default :					WARNING( "unknown cull mode" );
		}
		
		GL_CALL( glLineWidth( state.lineWidth ) );

		switch ( state.polygonMode ) {
			case EPolygonMode::Fill : {
				if ( state.depthBias )	GL_CALL( glEnable( GL_POLYGON_OFFSET_FILL ) )
				else					GL_CALL( glDisable( GL_POLYGON_OFFSET_FILL ) );
				
				GL_CALL( glPolygonMode( GL_FRONT_AND_BACK, GL_FILL ) );
				break;
			}
			case EPolygonMode::Line : {
				if ( state.depthBias )	GL_CALL( glEnable( GL_POLYGON_OFFSET_LINE ) )
				else					GL_CALL( glDisable( GL_POLYGON_OFFSET_LINE ) );
				
				GL_CALL( glPolygonMode( GL_FRONT_AND_BACK, GL_LINE ) );
				break;
			}
			case EPolygonMode::Point : {
				if ( state.depthBias )	GL_CALL( glEnable( GL_POLYGON_OFFSET_POINT ) )
				else					GL_CALL( glDisable( GL_POLYGON_OFFSET_POINT ) );
				
				GL_CALL( glPolygonMode( GL_FRONT_AND_BACK, GL_POINT ) );
				break;
			}
			default :
				WARNING( "unknown polygon mode" );
		}

		// TODO: depthBiasClamp
		GL_CALL( glPolygonOffset( state.depthBiasSlopeFactor, state.depthBiasConstFactor ) );
	}
	
/*
=================================================
	_ApplyMultisampleState
=================================================
*/
	void GL4CommandBuffer::_ApplyMultisampleState ()
	{
		const auto&	state	= _graphicsPipelineDescr.renderState.multisample;
		const uint	count	= (state.samples.Get() + 31) / 32;

		for (uint i = 0; i < count; ++i) {
			GL_CALL( glSampleMaski( i, state.sampleMask[i] ) );
		}

		if ( state.sampleShading )	GL_CALL( glEnable( GL_SAMPLE_SHADING ) )
		else						GL_CALL( glDisable( GL_SAMPLE_SHADING ) );

		GL_CALL( glMinSampleShading( state.minSampleShading ) );

		if ( state.alphaToCoverage )	GL_CALL( glEnable( GL_SAMPLE_ALPHA_TO_COVERAGE ) )
		else							GL_CALL( glDisable( GL_SAMPLE_ALPHA_TO_COVERAGE ) );
		
		if ( state.alphaToOne )		GL_CALL( glEnable( GL_SAMPLE_ALPHA_TO_ONE ) )
		else						GL_CALL( glDisable( GL_SAMPLE_ALPHA_TO_ONE ) );

		// TODO
		//glSampleCoverage();
		//glSampleMaski();
	}
	
/*
=================================================
	_ClearRenderPassAttachment
=================================================
*/
	bool GL4CommandBuffer::_ClearRenderPassAttachment (const ClearValues_t &clearValues)
	{
		using EClearValue		= GpuMsg::GetGLRenderPassID::EClearValue;
		using ClearValue_t		= ClearValues_t::Value_t;
		using DepthStencil_t	= GpuMsg::CmdBeginRenderPass::DepthStencil;

		usize	ds_index	= UMax;
		usize	col_start	= UMax;

		FOR( i, clearValues ) {
			if ( clearValues[i].Is<DepthStencil_t>() ) {
				ds_index = i;
				break;
			}
			if ( col_start == UMax )
				col_start = i;
		}
		CHECK_ERR( ds_index == UMax or ds_index == 0 or ds_index == clearValues.LastIndex() );

		GL_CALL( glEnable( GL_SCISSOR_TEST ) );
		//GL_CALL( glViewport( _renderPassArea.left, _renderPassArea.bottom, _renderPassArea.Width(), _renderPassArea.Height() ) );
		GL_CALL( glScissor( _renderPassArea.left, _renderPassArea.bottom, _renderPassArea.Width(), _renderPassArea.Height() ) );

		FOR( i, _renderPassData.clear )
		{
			const auto&	value	= _renderPassData.clear[i];
			GLenum		buffer	= 0;
			uint		index	= 0;	// for color buffer

			// get buffer type
			switch ( value.first )
			{
				case GL_DEPTH_STENCIL_ATTACHMENT :
				case GL_DEPTH_STENCIL :				buffer = GL_DEPTH_STENCIL;		break;
				case GL_DEPTH_ATTACHMENT :
				case GL_DEPTH :						buffer = GL_DEPTH;				break;
				case GL_STENCIL_ATTACHMENT :
				case GL_STENCIL :					buffer = GL_STENCIL;			break;
				//case GL_COLOR :					buffer = GL_COLOR;	index = 0;	break;
				case GL_BACK_LEFT :					buffer = GL_COLOR;	index = 0;	break;
				case GL_BACK_RIGHT :				buffer = GL_COLOR;	index = 1;	break;
				default :
					if ( value.first >= GL_COLOR_ATTACHMENT0 and
						 value.first <= GL_COLOR_ATTACHMENT0 + GlobalConst::GAPI_MaxColorBuffers )
					{
						buffer	= GL_COLOR;
						index	= value.first - GL_COLOR_ATTACHMENT0;
					}
					else
						RETURN_ERR( "invalid buffer type" );
			}

			// read clear value from command
			ClearValue_t	clear_val;
			
			if ( buffer != GL_COLOR and ds_index != UMax )	clear_val = clearValues[ds_index];			else
			if ( index < clearValues.Count() )				clear_val = clearValues[col_start + index];


			// clear specific buffer with specific value
			switch ( value.second )
			{
				case EClearValue::Float : {
					ASSERT( not clear_val.IsDefined() or clear_val.Is<float4>() );
					const float4	val = clear_val.Is<float4>() ? clear_val.Get<float4>() : float4();

					GL_CALL( glClearBufferfv( buffer, index, val.ptr() ) );
					break;
				}
				case EClearValue::Int : {
					ASSERT( not clear_val.IsDefined() or clear_val.Is<int4>() );
					const int4		val = clear_val.Is<int4>() ? clear_val.Get<int4>() : int4();

					GL_CALL( glClearBufferiv( buffer, index, val.ptr() ) );
					break;
				}
				case EClearValue::UInt : {
					ASSERT( not clear_val.IsDefined() or clear_val.Is<uint4>() );
					const uint4		val = clear_val.Is<uint4>() ? clear_val.Get<uint4>() : uint4();

					GL_CALL( glClearBufferuiv( buffer, index, val.ptr() ) );
					break;
				}
				case EClearValue::FloatInt : {
					ASSERT( not clear_val.IsDefined() or clear_val.Is<DepthStencil_t>() );
					const DepthStencil_t	ds = clear_val.Is<DepthStencil_t>() ? clear_val.Get<DepthStencil_t>() : DepthStencil_t{ 1.0f, 0 };

					GL_CALL( glClearBufferfi( buffer, 0, ds.depth, ds.stencil ) );
					break;
				}
				default :
					RETURN_ERR( "unknown type" );
			}
		}
		return true;
	}
	
/*
=================================================
	_InvalidateRenderPassAttachment
=================================================
*/
	bool GL4CommandBuffer::_InvalidateRenderPassAttachment (ArrayCRef<GLenum> buffers)
	{
		if ( buffers.Empty() )
			return true;

		GL_CALL( glInvalidateSubFramebuffer( GL_DRAW_FRAMEBUFFER, GLsizei(buffers.Count()), buffers.ptr(),
											_renderPassArea.left, _renderPassArea.bottom, _renderPassArea.Width(), _renderPassArea.Height() ) );
		return true;
	}
	
/*
=================================================
	_SetViewports
=================================================
*/		
	void GL4CommandBuffer::_SetViewports ()
	{
		ASSERT( _graphicsPipelineDescr.dynamicStates[EPipelineDynamicState::Viewport] );	// TODO: get viewports from pipeline
		ASSERT( _graphicsPipelineDescr.dynamicStates[EPipelineDynamicState::Scissor] );		// TODO: get scissors from pipeline

		GL_CALL( glEnable( GL_SCISSOR_TEST ) );

		bool	scissor_was_clipped		= false;

		FOR( i, _viewports )
		{
			const auto&	v		= _viewports[i];
			auto		r		= v.rect;
			auto		s		= _scissors[i];
			const uint	index	= uint(i);

			r.LeftBottom()	= Max( r.LeftBottom(), _renderPassArea.LeftBottom() );
			r.RightTop()	= Clamp( r.RightTop(), _renderPassArea.LeftBottom(), _renderPassArea.RightTop() );

			s.LeftBottom()	= Max( s.LeftBottom(), r.LeftBottom() );
			s.RightTop()	= Clamp( s.RightTop(), r.LeftBottom(), r.RightTop() );

			DEBUG_ONLY( scissor_was_clipped |= (s != _scissors[i]) );

			GL_CALL( glViewportIndexedf( index, float(v.rect.left), float(v.rect.bottom), float(v.rect.Width()), float(v.rect.Height()) ) );
			GL_CALL( glScissorIndexed( index, s.left, s.bottom, s.Width(), s.Height() ) );
			GL_CALL( glDepthRangeIndexed( index, v.depthRange.x, v.depthRange.y ) );
		}
		
		GL_CALL( glDrawBuffers( GLsizei(_renderPassData.draw.colorBuffers.Count()), _renderPassData.draw.colorBuffers.ptr() ) );

		if ( scissor_was_clipped )
			LOG( "Scissor was clipped by viewport", ELog::Debug );
	}

/*
=================================================
	_PrepareForCompute
=================================================
*/
	bool GL4CommandBuffer::_PrepareForCompute ()
	{
		// validate resources
		// TODO
		
		// bind program pipeline
		CHECK_ERR( _pipelineObj != 0 );
		GL_CALL( glBindProgramPipeline( _pipelineObj ) );

		// set uniforms
		CHECK_ERR( _resourceTable );
		_resourceTable->Send( GpuMsg::GLPipelineResourceTableApply{ _pipelineStages });
		return true;
	}
	
/*
=================================================
	operator (CmdSetViewport)
=================================================
*/
	bool GL4CommandBuffer::operator () (const GpuMsg::CmdSetViewport &msg)
	{
		FOR( i, msg.viewports ) {
			_viewports[i + msg.firstViewport] = msg.viewports[i];
		}
		return true;
	}
	
/*
=================================================
	operator (CmdSetScissor)
=================================================
*/
	bool GL4CommandBuffer::operator () (const GpuMsg::CmdSetScissor &msg)
	{
		FOR( i, msg.scissors ) {
			_scissors[i + msg.firstScissor] = msg.scissors[i];
		}
		return true;
	}
	
/*
=================================================
	operator (CmdSetDepthBounds)
=================================================
*/
	bool GL4CommandBuffer::operator () (const GpuMsg::CmdSetDepthBounds &)
	{
		TODO("");
		return true;
	}
	
/*
=================================================
	operator (CmdSetBlendColor)
=================================================
*/
	bool GL4CommandBuffer::operator () (const GpuMsg::CmdSetBlendColor &msg)
	{
		GL_CALL( glBlendColor( msg.color.R(), msg.color.G(), msg.color.B(), msg.color.A() ) );
		return true;
	}
	
/*
=================================================
	operator (CmdSetDepthBias)
=================================================
*/
	bool GL4CommandBuffer::operator () (const GpuMsg::CmdSetDepthBias &)
	{
		// glPolygonOffset ?
		TODO("");
		return true;
	}
	
/*
=================================================
	operator (CmdSetLineWidth)
=================================================
*/
	bool GL4CommandBuffer::operator () (const GpuMsg::CmdSetLineWidth &msg)
	{
		GL_CALL( glLineWidth( msg.width ) );
		return true;
	}
	
/*
=================================================
	operator (CmdSetStencilCompareMask)
=================================================
*/
	bool GL4CommandBuffer::operator () (const GpuMsg::CmdSetStencilCompareMask &)
	{
		// glStencilFunc - mask ?
		TODO("");
		return true;
	}
	
/*
=================================================
	operator (CmdSetStencilWriteMask)
=================================================
*/
	bool GL4CommandBuffer::operator () (const GpuMsg::CmdSetStencilWriteMask &)
	{
		// glStencilMask ?
		TODO("");
		return true;
	}
	
/*
=================================================
	operator (CmdSetStencilReference)
=================================================
*/
	bool GL4CommandBuffer::operator () (const GpuMsg::CmdSetStencilReference &)
	{
		// glStencilFunc - ref ?
		TODO("");
		return true;
	}
	
/*
=================================================
	operator (CmdBeginRenderPass)
=================================================
*/
	bool GL4CommandBuffer::operator () (const GpuMsg::CmdBeginRenderPass &msg)
	{
		const auto&	fb_res = GetResourceCache()->GetFramebufferID( msg.framebuffer );

		_renderPassData		= msg.renderPass->Request( GpuMsg::GetGLRenderPassID{} );
		_framebufferSize	= uint3( fb_res.Get<1>().size, fb_res.Get<1>().layers );
		_renderPassArea		= msg.area;
		_renderSubpasses	= msg.renderPass->Request(GpuMsg::GetRenderPassDescription{}).Subpasses();

		GLuint	fb_id		= fb_res.Get<0>();

		GL_CALL( glBindFramebuffer( GL_DRAW_FRAMEBUFFER, fb_id ) );
		GL_CALL( glDrawBuffers( GLsizei(_renderPassData.draw.colorBuffers.Count()), _renderPassData.draw.colorBuffers.ptr() ) );

		_ClearRenderPassAttachment( msg.clearValues );
		_InvalidateRenderPassAttachment( _renderPassData.invalidateBefore );
		return true;
	}
	
/*
=================================================
	operator (CmdEndRenderPass)
=================================================
*/
	bool GL4CommandBuffer::operator () (const GpuMsg::CmdEndRenderPass &)
	{
		_InvalidateRenderPassAttachment( _renderPassData.invalidateAfter );

		GL_CALL( glBindFramebuffer( GL_DRAW_FRAMEBUFFER, 0 ) );

		_renderPassData		= Uninitialized;
		_framebufferSize	= Uninitialized;
		_renderPassArea		= Uninitialized;
		_renderSubpasses.Clear();
		return true;
	}
	
/*
=================================================
	operator (CmdBindGraphicsPipeline)
=================================================
*/
	bool GL4CommandBuffer::operator () (const GpuMsg::CmdBindGraphicsPipeline &msg)
	{
		const auto&	ppln_res = GetResourceCache()->GetGraphicsPipelineID( msg.pipeline );

		_pipelineStages			= ppln_res.Get<0>().programs;
		_pipelineObj			= ppln_res.Get<0>().pipeline;
		_vertexAttribs			= ppln_res.Get<0>().vertexAttribs;
		_graphicsPipelineDescr	= ppln_res.Get<1>();
		_computePipelineDescr	= Uninitialized;
		_resourceTable			= null;

		return true;
	}
	
/*
=================================================
	operator (CmdBindComputePipeline)
=================================================
*/
	bool GL4CommandBuffer::operator () (const GpuMsg::CmdBindComputePipeline &msg)
	{
		const auto&	ppln_res = GetResourceCache()->GetComputePipelineID( msg.pipeline );

		_pipelineStages.Clear();
		_pipelineStages[ EShader::Compute ] = ppln_res.Get<0>().program;
		_pipelineObj			= ppln_res.Get<0>().pipeline;
		_vertexAttribs			= 0;
		_resourceTable			= null;
		_graphicsPipelineDescr	= Uninitialized;
		_computePipelineDescr	= ppln_res.Get<1>();

		return true;
	}
	
/*
=================================================
	operator (CmdBindVertexBuffers)
=================================================
*/
	bool GL4CommandBuffer::operator () (const GpuMsg::CmdBindVertexBuffers &msg)
	{
		_vertexBuffers.Clear();

		FOR( i, msg.vertexBuffers )
		{
			const auto&	buf_res = GetResourceCache()->GetBufferID( msg.vertexBuffers[i] );

			_vertexBuffers[ msg.firstBinding+i ] = GLBuffer{ buf_res.Get<0>(), msg.offsets[i] }; 
		}
		return true;
	}
	
/*
=================================================
	operator (CmdBindIndexBuffer)
=================================================
*/
	bool GL4CommandBuffer::operator () (const GpuMsg::CmdBindIndexBuffer &msg)
	{
		const auto&	buf_res = GetResourceCache()->GetBufferID( msg.buffer );

		_indexBuffer = GLBuffer{ buf_res.Get<0>(), msg.offset };
		_indexType	 = msg.indexType;
		return true;
	}
	
/*
=================================================
	operator (CmdDraw)
=================================================
*/
	bool GL4CommandBuffer::operator () (const GpuMsg::CmdDraw &msg)
	{
		CHECK_ERR( _PrepareForDraw() );
		GL_CALL( glDrawArraysInstancedBaseInstance( GL4Enum( _primitive ), msg.firstVertex,
								msg.vertexCount, msg.instanceCount, msg.firstInstance ) );
		return true;
	}
	
/*
=================================================
	operator (CmdDrawIndexed)
=================================================
*/
	bool GL4CommandBuffer::operator () (const GpuMsg::CmdDrawIndexed &msg)
	{
		CHECK_ERR( _PrepareForDraw() );
		CHECK_ERR( _indexBuffer.id != 0 );
		CHECK_ERR( _indexType != EIndex::Unknown );

		const usize offset	= usize(_indexBuffer.offset + EIndex::SizeOf( _indexType ) * msg.firstIndex);
		
		GL_CALL( glDrawElementsInstancedBaseInstance( GL4Enum( _primitive ), msg.indexCount, GL4Enum( _indexType ),
													  ReferenceCast<const void *>(offset), msg.instanceCount, msg.firstInstance ) );
		return true;
	}
	
/*
=================================================
	operator (CmdDrawIndirect)
=================================================
*/
	bool GL4CommandBuffer::operator () (const GpuMsg::CmdDrawIndirect &)
	{
		TODO("");
		return true;
	}
	
/*
=================================================
	operator (CmdDrawIndexedIndirect)
=================================================
*/
	bool GL4CommandBuffer::operator () (const GpuMsg::CmdDrawIndexedIndirect &)
	{
		TODO("");
		return true;
	}
	
/*
=================================================
	operator (CmdDispatch)
=================================================
*/
	bool GL4CommandBuffer::operator () (const GpuMsg::CmdDispatch &msg)
	{
		const uint		invocations		= _computePipelineDescr.localGroupSize.Volume();

		const uint3		max_group_count	= GetDevice()->GetProperties().maxComputeWorkGroupCount;
		const uint3		max_local_size	= GetDevice()->GetProperties().maxComputeWorkGroupSize;
		const uint		max_invocations	= GetDevice()->GetProperties().maxComputeWorkGroupInvocations;
		
		CHECK_ERR( invocations <= max_invocations );
		CHECK_ERR( All( _computePipelineDescr.localGroupSize <= max_invocations ) );
		CHECK_ERR( All( msg.groupCount <= max_group_count ) );

		CHECK_ERR( _PrepareForCompute() );
		GL_CALL( glDispatchCompute( msg.groupCount.x, msg.groupCount.y, msg.groupCount.z ) );
		return true;
	}
	
/*
=================================================
	operator (CmdDispatchIndirect)
=================================================
*/
	bool GL4CommandBuffer::operator () (const GpuMsg::CmdDispatchIndirect &)
	{
		TODO("");
		return true;
	}
	
/*
=================================================
	operator (CmdExecute)
=================================================
*/
	bool GL4CommandBuffer::operator () (const GpuMsg::CmdExecute &msg)
	{
		ModuleUtils::Send( msg.cmdBuffers, GpuMsg::SetCommandBufferState{ GpuMsg::SetCommandBufferState::EState::Pending });
		ModuleUtils::Send( msg.cmdBuffers, GpuMsg::ExecuteGLCommandBuffer{} );
		//ModuleUtils::Send( msg.cmdBuffers, GpuMsg::SetCommandBufferState{ GpuMsg::SetCommandBufferState::EState::Completed });

		return true;
	}
	
/*
=================================================
	operator (CmdBindGraphicsResourceTable)
=================================================
*/
	bool GL4CommandBuffer::operator () (const GpuMsg::CmdBindGraphicsResourceTable &msg)
	{
		ASSERT( msg.index == 0 );
		_resourceTable = msg.resourceTable;
		return true;
	}
	
/*
=================================================
	operator (CmdBindComputeResourceTable)
=================================================
*/
	bool GL4CommandBuffer::operator () (const GpuMsg::CmdBindComputeResourceTable &msg)
	{
		ASSERT( msg.index == 0 );
		_resourceTable = msg.resourceTable;
		return true;
	}
	
/*
=================================================
	operator (CmdCopyBuffer)
=================================================
*/
	bool GL4CommandBuffer::operator () (const GpuMsg::CmdCopyBuffer &msg)
	{
		const auto	src_res	= GetResourceCache()->GetBufferID( msg.srcBuffer );
		const auto	dst_res	= GetResourceCache()->GetBufferID( msg.dstBuffer );
		
		ASSERT( src_res.Get<1>().usage[ EBufferUsage::TransferSrc ] );
		ASSERT( dst_res.Get<1>().usage[ EBufferUsage::TransferDst ] );

		for (auto& reg : msg.regions)
		{
			GL_CALL( glCopyNamedBufferSubData(	src_res.Get<0>(),
												dst_res.Get<0>(),
												GLintptr(reg.srcOffset),
												GLintptr(reg.dstOffset),
												GLsizei(reg.size) ) );
		}
		return true;
	}
	
/*
=================================================
	operator (CmdCopyImage)
=================================================
*/
	bool GL4CommandBuffer::operator () (const GpuMsg::CmdCopyImage &msg)
	{
		const auto	src_res	= GetResourceCache()->GetImageID( msg.srcImage );
		const auto	dst_res	= GetResourceCache()->GetImageID( msg.dstImage );
		
		ASSERT( src_res.Get<1>().usage[ EImageUsage::TransferSrc ] );
		ASSERT( dst_res.Get<1>().usage[ EImageUsage::TransferDst ] );
		ASSERT( msg.srcLayout == EImageLayout::TransferSrcOptimal or msg.srcLayout == EImageLayout::General );	// for vulkan compatibility
		ASSERT( msg.dstLayout == EImageLayout::TransferDstOptimal or msg.dstLayout == EImageLayout::General );
		ASSERT( EPixelFormat::BitPerPixel( src_res.Get<1>().format ) == EPixelFormat::BitPerPixel( dst_res.Get<1>().format ) );
		ASSERT( src_res.Get<1>().samples == dst_res.Get<1>().samples );

		for (auto& reg : msg.regions)
		{
			const uint	count = Min( reg.srcLayers.layerCount, reg.dstLayers.layerCount );

			for (uint layer = 0; layer < count; ++layer)
			{
				const uint	src_z = reg.srcOffset.z + (reg.srcLayers.baseLayer.Get() + layer) * src_res.Get<1>().dimension.z;
				const uint	dst_z = reg.dstOffset.z + (reg.dstLayers.baseLayer.Get() + layer) * dst_res.Get<1>().dimension.z;
				
				ASSERT( reg.srcLayers.mipLevel < src_res.Get<1>().maxLevel );
				ASSERT( reg.dstLayers.mipLevel < dst_res.Get<1>().maxLevel );
				ASSERT( reg.srcLayers.baseLayer.Get() + reg.srcLayers.layerCount <= src_res.Get<1>().dimension.w );
				ASSERT( reg.dstLayers.baseLayer.Get() + reg.dstLayers.layerCount <= dst_res.Get<1>().dimension.w );
				ASSERT(All( reg.srcOffset + reg.size <= Max(1u, src_res.Get<1>().dimension.xyz() >> reg.srcLayers.mipLevel.Get()) ));
				ASSERT(All( reg.dstOffset + reg.size <= Max(1u, dst_res.Get<1>().dimension.xyz() >> reg.dstLayers.mipLevel.Get()) ));

				GL_CALL( glCopyImageSubData( src_res.Get<0>(),
											 GL4Enum( src_res.Get<1>().imageType ),
											 reg.srcLayers.mipLevel.Get(),
											 reg.srcOffset.x, reg.srcOffset.y, src_z,
											 dst_res.Get<0>(),
											 GL4Enum( dst_res.Get<1>().imageType ),
											 reg.dstLayers.mipLevel.Get(),
											 reg.dstOffset.x, reg.dstOffset.y, dst_z,
											 reg.size.x, reg.size.y, reg.size.z ) );
			}
		}
		return true;
	}
	
/*
=================================================
	operator (CmdCopyBufferToImage)
=================================================
*/
	bool GL4CommandBuffer::operator () (const GpuMsg::CmdCopyBufferToImage &msg)
	{
		const auto	src_res	= GetResourceCache()->GetBufferID( msg.srcBuffer );
		const auto	dst_res	= GetResourceCache()->GetImageID( msg.dstImage );
		
		ASSERT( src_res.Get<1>().usage[ EBufferUsage::TransferSrc ] );
		ASSERT( dst_res.Get<1>().usage[ EImageUsage::TransferDst ] );
		ASSERT( msg.dstLayout == EImageLayout::TransferDstOptimal or msg.dstLayout == EImageLayout::General );	// for vulkan compatibility
		
		const GLenum			target	= GL4Enum( dst_res.Get<1>().imageType );
		const BytesU			bpp		= BytesU(EPixelFormat::BitPerPixel( dst_res.Get<1>().format ));
		
		GL4InternalPixelFormat	ifmt;
		GL4PixelFormat			fmt;
		GL4PixelType			type;
		CHECK_ERR( GL4Enum( dst_res.Get<1>().format, OUT ifmt, OUT fmt, OUT type ) );
		
		BytesU	row_align;
		if ( bpp % 8_b == 0 )	row_align = 8_b;	else
		if ( bpp % 4_b == 0 )	row_align = 4_b;	else
		if ( bpp % 2_b == 0 )	row_align = 2_b;	else
								row_align = 1_b;

		GL_CALL( glBindBuffer( GL_PIXEL_UNPACK_BUFFER, src_res.Get<0>() ) );
		GL_CALL( glBindTexture( target, dst_res.Get<0>() ) );

		for (auto& reg : msg.regions)
		{
			for (uint layer = 0; layer < reg.imageLayers.layerCount; ++layer)
			{
				const uint		curr_layer	= reg.imageLayers.baseLayer.Get() + layer;
				const uint		mip_level	= reg.imageLayers.mipLevel.Get();
				const uint3		level_size	= Max( ImageUtils::LevelDimension( dst_res.Get<1>().imageType, dst_res.Get<1>().dimension, mip_level ).xyz(), 1u );
				const uint3		offset		= reg.imageOffset;
				const uint3		dim			= reg.imageSize;
				const void*		pixels		= reg.bufferOffset.ToVoidPtr();

				ASSERT( reg.imageLayers.mipLevel < dst_res.Get<1>().maxLevel );
				ASSERT( reg.imageLayers.baseLayer.Get() + reg.imageLayers.layerCount <= dst_res.Get<1>().dimension.w );
				ASSERT(All( (offset + dim) <= Max(1u, dst_res.Get<1>().dimension.xyz() >> mip_level) ));
				
				GL_CALL( glPixelStorei( GL_UNPACK_ALIGNMENT, GLint(row_align) ) );
				GL_CALL( glPixelStorei( GL_UNPACK_ROW_LENGTH, GLint(reg.bufferRowLength) ) );
				GL_CALL( glPixelStorei( GL_UNPACK_IMAGE_HEIGHT, GLint(reg.bufferImageHeight) ) );

				switch ( dst_res.Get<1>().imageType )
				{
					case EImage::Tex1D :
					{
						GL_CALL( glTexSubImage1D(	target, mip_level,
													offset.x, dim.x,
													fmt, type, pixels ) );
						break;
					}
					case EImage::Tex2D :
					{
						GL_CALL( glTexSubImage2D(	target, mip_level,
													offset.x, offset.y,
													dim.x, dim.y,
													fmt, type, pixels ) );
						break;
					}
					case EImage::Tex2DArray :
					{
						GL_CALL( glTexSubImage3D(	target, mip_level,
													offset.x, offset.y, curr_layer,
													dim.x, dim.y, 1,
													fmt, type, pixels ) );
						break;
					}
					case EImage::Tex3D :
					{
						GL_CALL( glTexSubImage3D(	target, mip_level,
													offset.x, offset.y, offset.z,
													dim.x, dim.y, dim.z,
													fmt, type, pixels ) );
						break;
					}
					case EImage::TexCube :
					{
						GL_CALL( glBindTexture(		GL_TEXTURE_CUBE_MAP_POSITIVE_X + curr_layer, dst_res.Get<0>() ) );
						GL_CALL( glTexSubImage2D(	GL_TEXTURE_CUBE_MAP_POSITIVE_X + curr_layer, mip_level,
													offset.x, offset.y,
													dim.x, dim.y,
													fmt, type,
													pixels ) );
						break;
					}
					case EImage::TexCubeArray :
					{
						GL_CALL( glTexSubImage3D(	target, mip_level,
													offset.x, offset.y, curr_layer,
													dim.x, dim.y, 1,
													fmt, type,
													pixels ) );
						break;
					}
					default :
					{
						RETURN_ERR( "unknown image type!" );
					}
				}
			}
		}

		GL_CALL( glBindBuffer( GL_PIXEL_UNPACK_BUFFER, 0 ) );
		GL_CALL( glBindTexture( target, 0 ) );
		return true;
	}
	
/*
=================================================
	operator (CmdCopyImageToBuffer)
=================================================
*/
	bool GL4CommandBuffer::operator () (const GpuMsg::CmdCopyImageToBuffer &msg)
	{
		if ( GRAPHICS_API_OPENGL >= 450 and gl::GL4_GetVersion() >= 450 )
			return _CopyImageToBuffer450( msg );
		else
			return _CopyImageToBuffer200( msg );
	}
	
/*
=================================================
	_CopyImageToBuffer200
=================================================
*/
	bool GL4CommandBuffer::_CopyImageToBuffer200 (const GpuMsg::CmdCopyImageToBuffer &msg)
	{
		const auto	src_res	= GetResourceCache()->GetImageID( msg.srcImage );
		const auto	dst_res	= GetResourceCache()->GetBufferID( msg.dstBuffer );
		
		ASSERT( src_res.Get<1>().usage[ EImageUsage::TransferSrc ] );
		ASSERT( dst_res.Get<1>().usage[ EBufferUsage::TransferDst ] );
		ASSERT( msg.srcLayout == EImageLayout::TransferSrcOptimal or msg.srcLayout == EImageLayout::General );	// for vulkan compatibility
		
		const GLenum			target		= GL4Enum( src_res.Get<1>().imageType );
		const BytesU			bpp			= BytesU(EPixelFormat::BitPerPixel( src_res.Get<1>().format ));
		
		GL4InternalPixelFormat	ifmt;
		GL4PixelFormat			fmt;
		GL4PixelType			type;
		CHECK_ERR( GL4Enum( src_res.Get<1>().format, OUT ifmt, OUT fmt, OUT type ) );
		
		BytesU	row_align;
		if ( bpp % 8_b == 0 )	row_align = 8_b;	else
		if ( bpp % 4_b == 0 )	row_align = 4_b;	else
		if ( bpp % 2_b == 0 )	row_align = 2_b;	else
								row_align = 1_b;

		GL_CALL( glBindBuffer( GL_PIXEL_PACK_BUFFER, dst_res.Get<0>() ) );
		GL_CALL( glBindTexture( target, src_res.Get<0>() ) );

		for (auto& reg : msg.regions)
		{
			const uint		mip_level	= reg.imageLayers.mipLevel.Get();
			const uint4		level_size	= Max( ImageUtils::LevelDimension( src_res.Get<1>().imageType, src_res.Get<1>().dimension, mip_level ), 1u );
			void *			pixels		= const_cast<void*>( reg.bufferOffset.ToVoidPtr() );

			CHECK_ERR( All( reg.imageOffset == 0 ) and reg.imageLayers.baseLayer.Get() == 0 );
			CHECK_ERR( All( reg.imageSize == level_size.xyz() ) );
			CHECK_ERR( level_size.w == 1 and reg.imageLayers.layerCount == 1 );
			
			GL_CALL( glPixelStorei( GL_PACK_ALIGNMENT, GLint(row_align) ) );
			GL_CALL( glPixelStorei( GL_PACK_ROW_LENGTH, GLint(reg.bufferRowLength) ) );
			GL_CALL( glPixelStorei( GL_PACK_IMAGE_HEIGHT, GLint(reg.bufferImageHeight) ) );
			
			switch ( src_res.Get<1>().imageType )
			{
				case EImage::Buffer :
				case EImage::Tex1D :
				{
					GL_CALL( glGetTexImage( target, mip_level, fmt, type, OUT pixels ) );
					break;
				}
				case EImage::Tex2D :
				{
					GL_CALL( glGetTexImage( target, mip_level, fmt, type, OUT pixels ) );
					break;
				}
				/*case EImage::Tex3D :
				{
					GL_CALL( glGetTexImage( target, mip_level, fmt, type, OUT pixels ) );
					break;
				}*/
				/*case EImage::Tex2DArray :
				{
					GL_CALL( glGetTexImage( target, mip_level, fmt, type, OUT pixels ) );
					break;
				}*/
				/*case EImage::TexCube :
				{
					CHECK_ERR( msg.layer.Get() < 6 );
					
					GL_CALL( glGetTexImage( target, mip_level, fmt, type, OUT pixels ) );
					break;
				}*/
				default :
					RETURN_ERR( "unsupported image type!" );
			}
		}
		return true;
	}
	
/*
=================================================
	_CopyImageToBuffer450
=================================================
*/
	bool GL4CommandBuffer::_CopyImageToBuffer450 (const GpuMsg::CmdCopyImageToBuffer &msg)
	{
	#if GRAPHICS_API_OPENGL >= 450
		
		const auto	src_res	= GetResourceCache()->GetImageID( msg.srcImage );
		const auto	dst_res	= GetResourceCache()->GetBufferID( msg.dstBuffer );
		
		ASSERT( src_res.Get<1>().usage[ EImageUsage::TransferSrc ] );
		ASSERT( dst_res.Get<1>().usage[ EBufferUsage::TransferDst ] );
		ASSERT( msg.srcLayout == EImageLayout::TransferSrcOptimal or msg.srcLayout == EImageLayout::General );	// for vulkan compatibility
		
		const BytesU			bpp	= BytesU(EPixelFormat::BitPerPixel( src_res.Get<1>().format ));
		
		GL4InternalPixelFormat	ifmt;
		GL4PixelFormat			fmt;
		GL4PixelType			type;
		CHECK_ERR( GL4Enum( src_res.Get<1>().format, OUT ifmt, OUT fmt, OUT type ) );
		
		BytesU	row_align;
		if ( bpp % 8_b == 0 )	row_align = 8_b;	else
		if ( bpp % 4_b == 0 )	row_align = 4_b;	else
		if ( bpp % 2_b == 0 )	row_align = 2_b;	else
								row_align = 1_b;

		GL_CALL( glBindBuffer( GL_PIXEL_PACK_BUFFER, dst_res.Get<0>() ) );

		for (auto& reg : msg.regions)
		{
			for (uint layer = 0; layer < reg.imageLayers.layerCount; ++layer)
			{
				const uint		curr_layer	= reg.imageLayers.baseLayer.Get() + layer;
				const uint		mip_level	= reg.imageLayers.mipLevel.Get();
				const uint3		level_size	= Max( ImageUtils::LevelDimension( src_res.Get<1>().imageType, src_res.Get<1>().dimension, mip_level ).xyz(), 1u );
				const uint3		offset		= reg.imageOffset;
				const uint3		dim			= reg.imageSize;
				void *			pixels		= const_cast<void*>( reg.bufferOffset.ToVoidPtr() );
				const BytesU	buf_size	= dst_res.Get<1>().size - reg.bufferOffset;

				ASSERT( mip_level < src_res.Get<1>().maxLevel.Get() );
				ASSERT( reg.imageLayers.baseLayer.Get() + reg.imageLayers.layerCount <= src_res.Get<1>().dimension.w );
				ASSERT(All( (offset + dim) <= Max(1u, src_res.Get<1>().dimension.xyz() >> mip_level) ));
				
				GL_CALL( glPixelStorei( GL_PACK_ALIGNMENT, GLint(row_align) ) );
				GL_CALL( glPixelStorei( GL_PACK_ROW_LENGTH, GLint(reg.bufferRowLength) ) );
				GL_CALL( glPixelStorei( GL_PACK_IMAGE_HEIGHT, GLint(reg.bufferImageHeight) ) );

				switch ( src_res.Get<1>().imageType )
				{
					case EImage::Buffer :
					case EImage::Tex1D :
					{
						CHECK_ERR(All( reg.imageOffset.yz() == 0 ));
						CHECK_ERR(All( reg.imageSize.yz() == 1 ));
						CHECK_ERR( reg.imageSize.x > 0 );

						GL_CALL( glGetTextureSubImage(	src_res.Get<0>(), mip_level,
														reg.imageOffset.x, 0, 0,
														reg.imageSize.x, 1, 1,
														fmt, type,
														GLsizei(buf_size),
														OUT pixels ) );
						break;
					}
					case EImage::Tex2D :
					{
						CHECK_ERR(All( reg.imageSize.xy() > 0 ));
						CHECK_ERR( reg.imageOffset.z == 0 and reg.imageSize.z == 1 );

						GL_CALL( glGetTextureSubImage(	src_res.Get<0>(), mip_level,
														reg.imageOffset.x, reg.imageOffset.y, 0,
														reg.imageSize.x, reg.imageSize.y, 1,
														fmt, type,
														GLsizei(buf_size),
														OUT pixels ) );
						break;
					}
					case EImage::Tex3D :
					{
						CHECK_ERR(All( reg.imageSize.xyz() > 0 ));

						GL_CALL( glGetTextureSubImage(	src_res.Get<0>(), mip_level,
														reg.imageOffset.x, reg.imageOffset.y, reg.imageOffset.z,
														reg.imageSize.x, reg.imageSize.y, reg.imageSize.z,
														fmt, type,
														GLsizei(buf_size),
														OUT pixels ) );
						break;
					}
					case EImage::Tex2DArray :
					{
						CHECK_ERR( reg.imageOffset.z == 0 );
						CHECK_ERR( reg.imageSize.z == 1 );
						CHECK_ERR(All( reg.imageSize.xy() > 0 ));

						GL_CALL( glGetTextureSubImage(	src_res.Get<0>(), mip_level,
														reg.imageOffset.x, reg.imageOffset.y, curr_layer,
														reg.imageSize.x, reg.imageSize.y, 1,
														fmt, type,
														GLsizei(buf_size),
														OUT pixels ) );
						break;
					}
					case EImage::TexCube :
					{
						CHECK_ERR( curr_layer < 6 );
						CHECK_ERR( reg.imageOffset.z == 0 and reg.imageSize.z == 1 );
						CHECK_ERR(All( reg.imageSize.xy() > 0 ));

						GL_CALL( glGetTextureSubImage(	src_res.Get<0>(), mip_level,
														reg.imageOffset.x, reg.imageOffset.y, curr_layer,
														reg.imageSize.x, reg.imageSize.y, 1,
														fmt, type,
														GLsizei(buf_size),
														OUT pixels ) );
						break;
					}
					default :
						RETURN_ERR( "unsupported image type!" );
				}
			}
		}
		return true;

	#else
		RETURN_ERR( "not supported!" );
	#endif
	}

/*
=================================================
	operator (CmdBlitImage)
=================================================
*/
	bool GL4CommandBuffer::operator () (const GpuMsg::CmdBlitImage &msg)
	{
		const auto	CheckFBStatus = LAMBDA() (GLuint, GLenum target) 
									{
										GLenum status = 0;
										GL_CALL( status = glCheckFramebufferStatus( target ) );
										return status == GL_FRAMEBUFFER_COMPLETE;
									};

		if ( _srcBlitFramebuffer == 0 ) {
			GL_CALL( glGenFramebuffers( 1, OUT &_srcBlitFramebuffer ) );
		}
		if ( _dstBlitFramebuffer == 0 ) {
			GL_CALL( glGenFramebuffers( 1, OUT &_dstBlitFramebuffer ) );
		}
		
		const auto	src_res	= GetResourceCache()->GetImageID( msg.srcImage );
		const auto	dst_res	= GetResourceCache()->GetImageID( msg.dstImage );
		
		GL_CALL( glDisable( GL_SCISSOR_TEST ) );

		GL_CALL( glBindFramebuffer( GL_READ_FRAMEBUFFER, _srcBlitFramebuffer ) );
		GL_CALL( glFramebufferTexture( GL_READ_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, src_res.Get<0>(), 0 ) );
		GL_CALL( glReadBuffer( GL_COLOR_ATTACHMENT0 ) );
		CHECK_ERR( CheckFBStatus( _srcBlitFramebuffer, GL_READ_FRAMEBUFFER ) );

		GL_CALL( glBindFramebuffer( GL_DRAW_FRAMEBUFFER, _dstBlitFramebuffer ) );
		GL_CALL( glFramebufferTexture( GL_READ_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, dst_res.Get<0>(), 0 ) );
		GL_CALL( glDrawBuffer( GL_COLOR_ATTACHMENT0 ) );
		CHECK_ERR( CheckFBStatus( _dstBlitFramebuffer, GL_DRAW_FRAMEBUFFER ) );

		const GLenum	filter = msg.linearFilter ? GL_LINEAR : GL_NEAREST;

		for (auto& r : msg.regions)
		{
			// depth image not supported, yet
			ASSERT( r.dstOffset0.z == 0 and r.dstOffset1.z == 0 );
			ASSERT( r.srcOffset0.z == 0 and r.srcOffset1.z == 0 );

			ASSERT( r.srcLayers.aspectMask == r.dstLayers.aspectMask );
			ASSERT( r.srcLayers.baseLayer == 0_layer and r.dstLayers.baseLayer == 0_layer );
			ASSERT( r.srcLayers.layerCount == 1 and r.dstLayers.layerCount == 1 );
			ASSERT( r.srcLayers.mipLevel == 0_mipmap and r.dstLayers.mipLevel == 0_mipmap );
			
			const GLenum	buffers	= (r.srcLayers.aspectMask[ EImageAspect::Color ] ? GL_COLOR_BUFFER_BIT : 0) |
									  (r.srcLayers.aspectMask[ EImageAspect::Depth ] ? GL_DEPTH_BUFFER_BIT : 0) |
									  (r.srcLayers.aspectMask[ EImageAspect::Stencil ] ? GL_STENCIL_BUFFER_BIT : 0);
			CHECK_ERR( buffers != 0 );

			GL_CALL( glBlitFramebuffer( r.srcOffset0.x, r.srcOffset0.y, r.srcOffset1.x, r.srcOffset1.y,
										r.dstOffset0.x, r.dstOffset0.y, r.dstOffset1.x, r.dstOffset1.y,
										buffers, filter ) );
		}

		GL_CALL( glBindFramebuffer( GL_READ_FRAMEBUFFER, 0 ) );
		GL_CALL( glBindFramebuffer( GL_DRAW_FRAMEBUFFER, 0 ) );
		return true;
	}
	
/*
=================================================
	operator (CmdResolveImage)
=================================================
*/
	bool GL4CommandBuffer::operator () (const GpuMsg::CmdResolveImage &)
	{
		TODO( "_CmdResolveImage" );
		return false;
	}

/*
=================================================
	operator (CmdBlitGLFramebuffers)
=================================================
*/
	bool GL4CommandBuffer::operator () (const GpuMsg::CmdBlitGLFramebuffers &msg)
	{
		const auto	src_res	= GetResourceCache()->GetFramebufferID( msg.srcFramebuffer );
		const auto	dst_res	= GetResourceCache()->GetFramebufferID( msg.dstFramebuffer );

		GLuint	src_fb	= src_res.Get<0>();
		GLuint	dst_fb	= dst_res.Get<0>();

		GL_CALL( glDisable( GL_SCISSOR_TEST ) );
		GL_CALL( glBindFramebuffer( GL_READ_FRAMEBUFFER, src_fb ) );
		GL_CALL( glBindFramebuffer( GL_DRAW_FRAMEBUFFER, dst_fb ) );

		GL_CALL( glReadBuffer( src_fb ? GL_COLOR_ATTACHMENT0 : GL_BACK ) );
		GL_CALL( glDrawBuffer( dst_fb ? GL_COLOR_ATTACHMENT0 : GL_BACK ) );
		
		const GLenum	filter	= msg.linearFilter ? GL_LINEAR : GL_NEAREST;

		const GLenum	buffers	= (msg.imageAspect[ EImageAspect::Color ] ? GL_COLOR_BUFFER_BIT : 0) |
								  (msg.imageAspect[ EImageAspect::Depth ] ? GL_DEPTH_BUFFER_BIT : 0) |
								  (msg.imageAspect[ EImageAspect::Stencil ] ? GL_STENCIL_BUFFER_BIT : 0);
		CHECK_ERR( buffers != 0 );


		for (auto& r : msg.regions)
		{
			GL_CALL( glBlitFramebuffer( r.srcOffset0.x, r.srcOffset0.y, r.srcOffset1.x, r.srcOffset1.y,
									    r.dstOffset0.x, r.dstOffset0.y, r.dstOffset1.x, r.dstOffset1.y,
										buffers, filter ) );
		}

		GL_CALL( glBindFramebuffer( GL_READ_FRAMEBUFFER, 0 ) );
		GL_CALL( glBindFramebuffer( GL_DRAW_FRAMEBUFFER, 0 ) );

		return true;
	}

/*
=================================================
	operator (GLCmdUpdateBuffer)
=================================================
*/
	bool GL4CommandBuffer::operator () (const GpuMsg::GLCmdUpdateBuffer &msg)
	{
		const auto	src_res	= GetResourceCache()->GetBufferID( _tempBuffer );
		const auto	dst_res	= GetResourceCache()->GetBufferID( msg.dstBuffer );
		
		CHECK_ERR( msg.dstOffset < dst_res.Get<1>().size );
		CHECK_ERR( msg.size + msg.dstOffset <= dst_res.Get<1>().size );

		GL_CALL( glCopyNamedBufferSubData( src_res.Get<0>(),
										   dst_res.Get<0>(),
										   GLintptr(msg.srcOffset),
										   GLintptr(msg.dstOffset),
										   GLsizei(msg.size) ) );
		return true;
	}
	
/*
=================================================
	operator (CmdFillBuffer)
=================================================
*/
	bool GL4CommandBuffer::operator () (const GpuMsg::CmdFillBuffer &msg)
	{
		const auto	buf_res	= GetResourceCache()->GetBufferID( msg.dstBuffer );
		
		CHECK_ERR( msg.dstOffset < buf_res.Get<1>().size );
		CHECK_ERR( buf_res.Get<1>().usage[ EBufferUsage::TransferDst ] );

		const BytesU	size = Min( buf_res.Get<1>().size - msg.dstOffset, msg.size );

		GL_CALL( glBindBuffer( GL_COPY_READ_BUFFER, buf_res.Get<0>() ) );

		GL_CALL( glClearBufferSubData( GL_COPY_READ_BUFFER,
										GL_R32UI,
										GLintptr(msg.dstOffset),
										GLsizeiptr(size),
										GL_RED_INTEGER, GL_UNSIGNED_INT,
										&msg.pattern ) );

		GL_CALL( glBindBuffer( GL_COPY_READ_BUFFER, 0 ) );
		return true;
	}
	
/*
=================================================
	operator (CmdClearAttachments)
=================================================
*/
	bool GL4CommandBuffer::operator () (const GpuMsg::CmdClearAttachments &msg)
	{
		using ValueList			= GpuMsg::CmdClearAttachments::ClearValue_t::TypeList_t;
		using DepthStencil_t	= GpuMsg::CmdClearAttachments::DepthStencil_t;

		GL_CALL( glEnable( GL_SCISSOR_TEST ) );
		//GL_CALL( glViewport( _renderPassArea.left, _renderPassArea.bottom, _renderPassArea.Width(), _renderPassArea.Height() ) );

		for (auto r : msg.clearRects)
		{
			r.rect.LeftBottom()	= Max( r.rect.LeftBottom(), _renderPassArea.LeftBottom() );
			r.rect.RightTop()	= Clamp( r.rect.RightTop(), _renderPassArea.LeftBottom(), _renderPassArea.RightTop() );

			GL_CALL( glScissor( r.rect.left, r.rect.bottom, r.rect.Width(), r.rect.Height() ) );

			for (auto& att : msg.attachments)
			{
				const bool	is_ds = att.attachmentIdx >= _renderPassData.draw.colorBuffers.Count();

				CHECK_ERR( att.aspectMask[EImageAspect::Color] == not is_ds and
						   (att.aspectMask[EImageAspect::Depth] == is_ds or att.aspectMask[EImageAspect::Stencil] == is_ds) );

				if ( not is_ds )
				{
					switch ( att.clearValue.GetCurrentIndex() )
					{
						case ValueList::IndexOf<float4> : {
							glClearBufferfv( GL_COLOR, att.attachmentIdx, att.clearValue.Get<float4>().ptr() );
							break;
						}
						case ValueList::IndexOf<int4> : {
							glClearBufferiv( GL_COLOR, att.attachmentIdx, att.clearValue.Get<int4>().ptr() );
							break;
						}
						case ValueList::IndexOf<uint4> : {
							glClearBufferuiv( GL_COLOR, att.attachmentIdx, att.clearValue.Get<uint4>().ptr() );
							break;
						}
						default :
							RETURN_ERR( "unsupported clear value for color buffer" );
					}
				}
				else
				{
					CHECK_ERR( att.clearValue.Is<DepthStencil_t>() );

					const auto&	ds = att.clearValue.Get<DepthStencil_t>();

					if ( att.aspectMask[EImageAspect::Depth] and att.aspectMask[EImageAspect::Stencil] )
						GL_CALL( glClearBufferfi( GL_DEPTH_STENCIL, 0, ds.depth, ds.stencil ) )
					else
					if ( att.aspectMask[EImageAspect::Depth] )
						GL_CALL( glClearBufferfv( GL_DEPTH, 0, float4(ds.depth).ptr() ) )
					else
					if ( att.aspectMask[EImageAspect::Stencil] )
						GL_CALL( glClearBufferiv( GL_STENCIL, 0, int4(ds.stencil).ptr() ) );
				}
			}
		}
		return true;
	}
	
/*
=================================================
	operator (CmdClearColorImage)
=================================================
*/
	bool GL4CommandBuffer::operator () (const GpuMsg::CmdClearColorImage &msg)
	{
		using Colors_t	= GpuMsg::CmdClearColorImage::ClearColor_t::TypeList_t;

		const auto&	img_res = GetResourceCache()->GetImageID( msg.image );
		
		CHECK_ERR( img_res.Get<1>().usage[ EImageUsage::TransferDst ] );

		GLenum	clear_fmt	= GL_RGBA;
		GLenum	clear_type	= GL_FLOAT;

		switch ( msg.clearValue.GetCurrentIndex() )
		{
			case Colors_t::IndexOf<float4> :	clear_type = GL_FLOAT;			clear_fmt = GL_RGBA;			break;
			case Colors_t::IndexOf<uint4> :		clear_type = GL_UNSIGNED_INT;	clear_fmt = GL_RGBA_INTEGER;	break;
			case Colors_t::IndexOf<int4> :		clear_type = GL_INT;			clear_fmt = GL_RGBA_INTEGER;	break;
			default :							RETURN_ERR( "unsupported" );
		}

		ASSERT( not msg.ranges.Empty() );

		FOR( i, msg.ranges )
		{
			auto const&		src = msg.ranges[i];

			ASSERT( src.baseLayer.Get() == 0 and src.layerCount == 1 );	// TODO: use glClearTexSubImage to clear layers

			for (uint j = 0; j < src.levelCount; ++j)
			{
				GL_CALL( glClearTexImage( img_res.Get<0>(),
										  src.baseMipLevel.Get() + j,
										  clear_fmt,
										  clear_type,
										  msg.clearValue.GetData().ptr() ) );
			}
		}

		return true;
	}
	
/*
=================================================
	operator (CmdClearDepthStencilImage)
=================================================
*/
	bool GL4CommandBuffer::operator () (const GpuMsg::CmdClearDepthStencilImage &)
	{
		TODO("CmdClearDepthStencilImage");
		return true;
	}
	
/*
=================================================
	operator (CmdPipelineBarrier)
=================================================
*/
	bool GL4CommandBuffer::operator () (const GpuMsg::CmdPipelineBarrier &msg)
	{
		GLbitfield	bits = 0;

		for (auto& buf : msg.bufferBarriers)
		{
			const auto& buf_res = GetResourceCache()->GetBufferID( buf.buffer );

			FOR( i, buf_res.Get<1>().usage )
			{
				if ( not buf_res.Get<1>().usage[ EBufferUsage::type(i) ] )
					continue;

				switch ( EBufferUsage::type(i) )
				{
					case EBufferUsage::TransferSrc :
					case EBufferUsage::TransferDst :	bits |= (GL_BUFFER_UPDATE_BARRIER_BIT | GL_PIXEL_BUFFER_BARRIER_BIT);  break;
					case EBufferUsage::UniformTexel :
					case EBufferUsage::StorageTexel :	bits |= GL_TEXTURE_FETCH_BARRIER_BIT;  break;
					case EBufferUsage::Uniform :		bits |= GL_UNIFORM_BARRIER_BIT;  break;
					case EBufferUsage::Storage :		bits |= GL_SHADER_STORAGE_BARRIER_BIT;  break;
					case EBufferUsage::Index :			bits |= GL_ELEMENT_ARRAY_BARRIER_BIT;  break;
					case EBufferUsage::Vertex :			bits |= GL_VERTEX_ATTRIB_ARRAY_BARRIER_BIT;  break;
					case EBufferUsage::Indirect :		bits |= GL_COMMAND_BARRIER_BIT;  break;
					// TODO: GL_ATOMIC_COUNTER_BARRIER_BIT, GL_QUERY_BUFFER_BARRIER_BIT, GL_TRANSFORM_FEEDBACK_BARRIER_BIT
				}
			}
		}

		for (auto& img : msg.imageBarriers)
		{
			const auto&	img_res = GetResourceCache()->GetImageID( img.image );

			FOR( i, img_res.Get<1>().usage )
			{
				if ( not img_res.Get<1>().usage[ EImageUsage::type(i) ] )
					continue;

				switch ( EImageUsage::type(i) )
				{
					case EImageUsage::TransferSrc :
					case EImageUsage::TransferDst :				bits |= GL_TEXTURE_UPDATE_BARRIER_BIT;  break;
					case EImageUsage::Sampled :					bits |= GL_TEXTURE_FETCH_BARRIER_BIT;  break;
					case EImageUsage::Storage :					bits |= GL_SHADER_IMAGE_ACCESS_BARRIER_BIT;  break;
					case EImageUsage::ColorAttachment :			bits |= GL_FRAMEBUFFER_BARRIER_BIT;  break;
					case EImageUsage::DepthStencilAttachment :	bits |= GL_FRAMEBUFFER_BARRIER_BIT;  break;
					//case EImageUsage::TransientAttachment :
					//case EImageUsage::InputAttachment :
				}
			}
		}

		// TODO: memory barriers
		// TODO: check src access mask, dst access mask

		GL_CALL( glMemoryBarrier( bits ) );
		return true;
	}
	
/*
=================================================
	operator (GLCmdPushConstants)
=================================================
*/
	bool GL4CommandBuffer::operator () (const GpuMsg::GLCmdPushConstants &)
	{
		TODO("GLCmdPushConstants");
		return true;
	}
	
/*
=================================================
	operator (CmdDebugMarker)
=================================================
*/
	bool GL4CommandBuffer::operator () (const GpuMsg::CmdDebugMarker &msg)
	{
		GL_CALL( glDebugMessageInsert( GL_DEBUG_SOURCE_APPLICATION, GL_DEBUG_TYPE_MARKER, 0, _debugMarkerSeverity, GLsizei(msg.info.Length()), msg.info.cstr() ) );
		
		if ( msg.breakPoint ) {
			GX_BREAK_POINT();
		}
		return true;
	}
	
/*
=================================================
	operator (CmdPushDebugGroup)
=================================================
*/
	bool GL4CommandBuffer::operator () (const GpuMsg::CmdPushDebugGroup &msg)
	{
		GL_CALL( glDebugMessageInsert( GL_DEBUG_SOURCE_APPLICATION, GL_DEBUG_TYPE_PUSH_GROUP, 0, _debugMarkerSeverity, GLsizei(msg.info.Length()), msg.info.cstr() ) );
		return true;
	}
	
/*
=================================================
	operator (CmdPopDebugGroup)
=================================================
*/
	bool GL4CommandBuffer::operator () (const GpuMsg::CmdPopDebugGroup &)
	{
		GL_CALL( glDebugMessageInsert( GL_DEBUG_SOURCE_APPLICATION, GL_DEBUG_TYPE_POP_GROUP, 0, _debugMarkerSeverity, 0, "" ) );
		return true;
	}
	
/*
=================================================
	operator (CmdBeginQuery)
=================================================
*/
	bool GL4CommandBuffer::operator () (const GpuMsg::CmdBeginQuery &msg)
	{
		const QueryPoolDescription	desc = msg.queryPool->Request( GpuMsg::GetQueryPoolDescription{} );

		if ( desc.queryType == EQuery::Occlusion )
		{
			GLenum	target = msg.precise ? GL_SAMPLES_PASSED : GL_ANY_SAMPLES_PASSED;
			
			GLuint id = msg.queryPool->Request( GpuMsg::GetGLQueryID{ target, msg.queryIndex }).id;

			GL_CALL( glBeginQuery( target, id ) );
		}
		else
		if ( desc.queryType == EQuery::PipelineStatistic )
		{
			const auto&	queries = msg.queryPool->Request( GpuMsg::GetGLPipelineStatisticQueries{ msg.queryIndex });

			for (auto& q : queries) {
				GL_CALL( glBeginQuery( q.target, q.id ) );
			}
		}
		else
		{
			RETURN_ERR( "unsupported query type!" );
		}

		return true;
	}
	
/*
=================================================
	operator (CmdEndQuery)
=================================================
*/
	bool GL4CommandBuffer::operator () (const GpuMsg::CmdEndQuery &msg)
	{
		const QueryPoolDescription	desc = msg.queryPool->Request( GpuMsg::GetQueryPoolDescription{} );

		if ( desc.queryType == EQuery::Occlusion )
		{
			const auto&	q = msg.queryPool->Request( GpuMsg::GetGLQueryID{ 0, msg.queryIndex });

			GL_CALL( glEndQuery( q.target ) );
		}
		else
		if ( desc.queryType == EQuery::PipelineStatistic )
		{
			const auto&	queries = msg.queryPool->Request( GpuMsg::GetGLPipelineStatisticQueries{ msg.queryIndex });

			for (auto& q : queries) {
				GL_CALL( glEndQuery( q.target ) );
			}
		}
		else
		{
			RETURN_ERR( "unsupported query type!" );
		}

		return true;
	}
	
/*
=================================================
	operator (CmdCopyQueryPoolResults)
=================================================
*/
	bool GL4CommandBuffer::operator () (const GpuMsg::CmdCopyQueryPoolResults &msg)
	{
		const auto		desc	= msg.queryPool->Request( GpuMsg::GetQueryPoolDescription{} );
		const auto&		buf_res = GetResourceCache()->GetBufferID( msg.dstBuffer );

		ASSERT( buf_res.Get<1>().usage[ EBufferUsage::TransferDst ] );
		ASSERT( msg.stride >= SizeOf<GLuint64> );

		GL_CALL( glBindBuffer( GL_QUERY_BUFFER, buf_res.Get<0>() ) );

		GLuint64*	ptr		= Cast<GLuint64*>( msg.dstOffset.ToVoidPtr() );
		BytesU		offset;

		if ( desc.queryType == EQuery::Occlusion or desc.queryType == EQuery::Timestamp )
		{
			for (uint i = 0; i < msg.queryCount; ++i)
			{
				const auto&	q = msg.queryPool->Request( GpuMsg::GetGLQueryID{ 0, msg.firstQueryIndex + i });

				ASSERT( msg.dstOffset + offset + SizeOf<GLuint64> < buf_res.Get<1>().size );

				GL_CALL( glGetQueryObjectui64v( q.id, GL_QUERY_RESULT_NO_WAIT, ptr + offset ) );

				offset += msg.stride;
			}
		}
		else
		if ( desc.queryType == EQuery::PipelineStatistic )
		{
			for (uint i = 0; i < msg.queryCount; ++i)
			{
				const auto&	queries = msg.queryPool->Request( GpuMsg::GetGLPipelineStatisticQueries{ msg.firstQueryIndex + i });

				for (auto& q : queries)
				{
					ASSERT( msg.dstOffset + offset + SizeOf<GLuint64> < buf_res.Get<1>().size );

					GL_CALL( glGetQueryObjectui64v( q.id, GL_QUERY_RESULT_NO_WAIT, ptr + offset ) );

					offset += msg.stride;
				}
			}
		}
		else
		{
			RETURN_ERR( "unsupported query type!" );
		}
	
		return true;
	}
	
/*
=================================================
	operator (CmdWriteTimestamp)
=================================================
*/
	bool GL4CommandBuffer::operator () (const GpuMsg::CmdWriteTimestamp &msg)
	{
		GLenum	target	= GL_TIMESTAMP;
		GLuint	id		= msg.queryPool->Request( GpuMsg::GetGLQueryID{ target, msg.queryIndex }).id;

		GL_CALL( glQueryCounter( id, target ) );
		return true;
	}
	
/*
=================================================
	operator (CmdResetQueryPool)
=================================================
*/
	bool GL4CommandBuffer::operator () (const GpuMsg::CmdResetQueryPool &)
	{
		TODO( "" );
		return true;
	}

}	// PlatformGL
//-----------------------------------------------------------------------------

namespace Platforms
{
	ModulePtr OpenGLObjectsConstructor::CreateGL4CommandBuffer (ModuleMsg::UntypedID_t id, GlobalSystemsRef gs, const CreateInfo::GpuCommandBuffer &ci)
	{
		return New< PlatformGL::GL4CommandBuffer >( id, gs, ci );
	}
}	// Platforms
}	// Engine

#endif	// GRAPHICS_API_OPENGL
