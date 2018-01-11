// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Engine/Platforms/Shared/GPU/CommandBuffer.h"
#include "Engine/Platforms/Shared/GPU/Framebuffer.h"
#include "Engine/Platforms/Shared/GPU/Image.h"
#include "Engine/Platforms/Shared/GPU/Buffer.h"
#include "Engine/Platforms/Shared/GPU/Memory.h"
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
	// OpenGL Command Buffer
	//

	class GL4CommandBuffer final : public GL4BaseModule
	{
	// types
	private:
		using SupportedMessages_t	= GL4BaseModule::SupportedMessages_t::Append< MessageListFrom<
											GpuMsg::GetCommandBufferDescriptor,
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
		using Command_t				= GpuMsg::SetGLCommandBufferQueue::Command;
		using CommandArray_t		= Array< Command_t >;
		using CmdDataTypes_t		= GpuMsg::SetGLCommandBufferQueue::Data_t::TypeList_t;

		using ERecordingState		= GpuMsg::SetCommandBufferState::EState;

		struct GLBuffer : CompileTime::PODStruct
		{
			GLuint		id	= 0;
			BytesU		offset;
			BytesU		stride;

			GLBuffer () {}
			GLBuffer (GLuint id, BytesUL off) : id(id), offset(off) {}
		};

		using PipelineStages_t		= StaticArray< GLuint, EShader::_Count >;
		using VertexBuffers_t		= StaticArray< GLBuffer, GlobalConst::Graphics_MaxAttribs >;
		using RenderPassData_t		= GpuMsg::GetGLRenderPassID::RenderPassID;
		using ClearValues_t			= GpuMsg::CmdBeginRenderPass::ClearValues_t;
		using RenderSubpasses_t		= RenderPassDescriptor::Subpasses_t;
		using Viewports_t			= StaticArray< GpuMsg::CmdSetViewport::Viewport, GlobalConst::Graphics_MaxColorBuffers >;
		using Scissors_t			= StaticArray< RectU, GlobalConst::Graphics_MaxColorBuffers >;


	// constants
	private:
		static const TypeIdList		_msgTypes;
		static const TypeIdList		_eventTypes;


	// variables
	private:
		ModulePtr					_tempBuffer;	// for UpdateBuffer command

		// temporary framebuffer for blit image command
		GLuint						_srcBlitFramebuffer;
		GLuint						_dstBlitFramebuffer;

		CommandBufferDescriptor		_descr;
		CommandArray_t				_commands;
		UsedResources_t				_resources;
		ERecordingState				_recordingState;
		ulong3						_maxWorkGroupCount;

		// states
		PipelineStages_t			_pipelineStages;
		GLuint						_pipelineObj;
		ModulePtr					_resourceTable;
		GraphicsPipelineDescriptor	_graphicsPipelineDescr;
		ComputePipelineDescriptor	_computePipelineDescr;
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
		GL4CommandBuffer (GlobalSystemsRef gs, const CreateInfo::GpuCommandBuffer &ci);
		~GL4CommandBuffer ();


	// message handlers
	private:
		bool _Compose (const Message< ModuleMsg::Compose > &);
		bool _Delete (const Message< ModuleMsg::Delete > &);
		bool _SetGLCommandBufferQueue (const Message< GpuMsg::SetGLCommandBufferQueue > &);
		bool _GetCommandBufferDescriptor (const Message< GpuMsg::GetCommandBufferDescriptor > &);
		bool _SetCommandBufferDependency (const Message< GpuMsg::SetCommandBufferDependency > &);
		bool _SetCommandBufferState (const Message< GpuMsg::SetCommandBufferState > &);
		bool _GetCommandBufferState (const Message< GpuMsg::GetCommandBufferState > &);
		bool _ExecuteGLCommandBuffer (const Message< GpuMsg::ExecuteGLCommandBuffer > &);
		
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

		bool _CmdSetViewport (const Command_t &cmd);
		bool _CmdSetScissor (const Command_t &cmd);
		bool _CmdSetDepthBounds (const Command_t &cmd);
		bool _CmdSetBlendColor (const Command_t &cmd);
		bool _CmdSetDepthBias (const Command_t &cmd);
		bool _CmdSetLineWidth (const Command_t &cmd);
		bool _CmdSetStencilCompareMask (const Command_t &cmd);
		bool _CmdSetStencilWriteMask (const Command_t &cmd);
		bool _CmdSetStencilReference (const Command_t &cmd);
		bool _CmdBeginRenderPass (const Command_t &cmd);
		bool _CmdEndRenderPass (const Command_t &cmd);
		bool _CmdBindGraphicsPipeline (const Command_t &cmd);
		bool _CmdBindComputePipeline (const Command_t &cmd);
		bool _CmdBindVertexBuffers (const Command_t &cmd);
		bool _CmdBindIndexBuffer (const Command_t &cmd);
		bool _CmdDraw (const Command_t &cmd);
		bool _CmdDrawIndexed (const Command_t &cmd);
		bool _CmdDrawIndirect (const Command_t &cmd);
		bool _CmdDrawIndexedIndirect (const Command_t &cmd);
		bool _CmdDispatch (const Command_t &cmd);
		bool _CmdDispatchIndirect (const Command_t &cmd);
		bool _CmdExecute (const Command_t &cmd);
		bool _CmdBindGraphicsResourceTable (const Command_t &cmd);
		bool _CmdBindComputeResourceTable (const Command_t &cmd);
		bool _CmdCopyBuffer (const Command_t &cmd);
		bool _CmdCopyImage (const Command_t &cmd);
		bool _CmdCopyBufferToImage (const Command_t &cmd);
		bool _CmdCopyImageToBuffer (const Command_t &cmd);
		bool _CmdBlitImage (const Command_t &cmd);
		bool _CmdBlitGLFramebuffers (const Command_t &cmd);
		bool _CmdUpdateBuffer (const Command_t &cmd);
		bool _CmdFillBuffer (const Command_t &cmd);
		bool _CmdClearAttachments (const Command_t &cmd);
		bool _CmdClearColorImage (const Command_t &cmd);
		bool _CmdClearDepthStencilImage (const Command_t &cmd);
		bool _CmdPipelineBarrier (const Command_t &cmd);
		
		static void _ValidateDescriptor (INOUT CommandBufferDescriptor &descr);
	};
//-----------------------------------------------------------------------------



	const TypeIdList	GL4CommandBuffer::_msgTypes{ UninitializedT< SupportedMessages_t >() };
	const TypeIdList	GL4CommandBuffer::_eventTypes{ UninitializedT< SupportedEvents_t >() };

/*
=================================================
	constructor
=================================================
*/
	GL4CommandBuffer::GL4CommandBuffer (GlobalSystemsRef gs, const CreateInfo::GpuCommandBuffer &ci) :
		GL4BaseModule( gs, ModuleConfig{ GLCommandBufferModuleID, UMax }, &_msgTypes, &_eventTypes ),
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
		_SubscribeOnMsg( this, &GL4CommandBuffer::_GetCommandBufferDescriptor );
		_SubscribeOnMsg( this, &GL4CommandBuffer::_GetDeviceInfo );
		_SubscribeOnMsg( this, &GL4CommandBuffer::_GetGLDeviceInfo );
		_SubscribeOnMsg( this, &GL4CommandBuffer::_GetGLPrivateClasses );
		_SubscribeOnMsg( this, &GL4CommandBuffer::_SetGLCommandBufferQueue );
		_SubscribeOnMsg( this, &GL4CommandBuffer::_SetCommandBufferDependency );
		_SubscribeOnMsg( this, &GL4CommandBuffer::_SetCommandBufferState );
		_SubscribeOnMsg( this, &GL4CommandBuffer::_GetCommandBufferState );
		_SubscribeOnMsg( this, &GL4CommandBuffer::_ExecuteGLCommandBuffer );

		CHECK( _ValidateMsgSubscriptions() );

		_AttachSelfToManager( ci.gpuThread, GLThreadModuleID, true );

		_ValidateDescriptor( _descr );
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
	bool GL4CommandBuffer::_Compose (const Message< ModuleMsg::Compose > &msg)
	{
		if ( _IsComposedState( GetState() ) )
			return true;	// already composed

		CHECK_ERR( GetState() == EState::Linked );
		
		CHECK_COMPOSING( _Initialize() );

		_SendForEachAttachments( msg );
		
		// very paranoic check
		CHECK( _ValidateAllSubscriptions() );

		CHECK( _SetState( EState::ComposedMutable ) );
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
	_ValidateDescriptor
=================================================
*/
	void GL4CommandBuffer::_ValidateDescriptor (INOUT CommandBufferDescriptor &descr)
	{
	}

/*
=================================================
	_Delete
=================================================
*/
	bool GL4CommandBuffer::_Delete (const Message< ModuleMsg::Delete > &msg)
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
	bool GL4CommandBuffer::_SetGLCommandBufferQueue (const Message< GpuMsg::SetGLCommandBufferQueue > &msg)
	{
		_commands	= RVREF(msg->commands.Get());
		_tempBuffer	= null;

		if ( not msg->bufferData.Empty() )
		{
			CHECK_ERR( GlobalSystems()->modulesFactory->Create(
								GLBufferModuleID,
								GlobalSystems(),
								CreateInfo::GpuBuffer{
									BufferDescriptor{
										msg->bufferData.Size(),
										EBufferUsage::bits() | EBufferUsage::TransferSrc
									},
									null,
									EGpuMemory::bits() | EGpuMemory::CoherentWithCPU,
									EMemoryAccess::bits() | EMemoryAccess::GpuRead | EMemoryAccess::CpuWrite
								},
								OUT _tempBuffer ) );

			ModuleUtils::Initialize({ _tempBuffer });

			CHECK_ERR( _tempBuffer->Send< GpuMsg::WriteToGpuMemory >({ msg->bufferData }) );
		}
		return true;
	}

/*
=================================================
	_GetCommandBufferDescriptor
=================================================
*/
	bool GL4CommandBuffer::_GetCommandBufferDescriptor (const Message< GpuMsg::GetCommandBufferDescriptor > &msg)
	{
		msg->result.Set( _descr );
		return true;
	}
	
/*
=================================================
	_SetCommandBufferDependency
=================================================
*/
	bool GL4CommandBuffer::_SetCommandBufferDependency (const Message< GpuMsg::SetCommandBufferDependency > &msg)
	{
		_resources = RVREF( msg->resources.Get() );
		return true;
	}

/*
=================================================
	_GetCommandBufferDescriptor
=================================================
*/
	bool GL4CommandBuffer::_SetCommandBufferState (const Message< GpuMsg::SetCommandBufferState > &msg)
	{
		switch ( msg->newState )
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
	_GetCommandBufferDescriptor
=================================================
*/
	bool GL4CommandBuffer::_GetCommandBufferState (const Message< GpuMsg::GetCommandBufferState > &msg)
	{
		msg->result.Set( _recordingState );
		return true;
	}
	
/*
=================================================
	_ExecuteGLCommandBuffer
=================================================
*/
	bool GL4CommandBuffer::_ExecuteGLCommandBuffer (const Message< GpuMsg::ExecuteGLCommandBuffer > &)
	{
		CHECK_ERR( _recordingState == ERecordingState::Pending );

		FOR( i, _commands )
		{
			const auto&	data = _commands[i].data;
			
			switch ( data.GetCurrentIndex() )
			{
				case CmdDataTypes_t::IndexOf<GpuMsg::CmdSetViewport> :				_CmdSetViewport( data );				break;
				case CmdDataTypes_t::IndexOf<GpuMsg::CmdSetScissor> :				_CmdSetScissor( data );					break;
				case CmdDataTypes_t::IndexOf<GpuMsg::CmdSetDepthBounds> :			_CmdSetDepthBounds( data );				break;
				case CmdDataTypes_t::IndexOf<GpuMsg::CmdSetBlendColor> :			_CmdSetBlendColor( data );				break;
				case CmdDataTypes_t::IndexOf<GpuMsg::CmdSetDepthBias> :				_CmdSetDepthBias( data );				break;
				case CmdDataTypes_t::IndexOf<GpuMsg::CmdSetLineWidth> :				_CmdSetLineWidth( data );				break;
				case CmdDataTypes_t::IndexOf<GpuMsg::CmdSetStencilCompareMask> :	_CmdSetStencilCompareMask( data );		break;
				case CmdDataTypes_t::IndexOf<GpuMsg::CmdSetStencilWriteMask> :		_CmdSetStencilWriteMask( data );		break;
				case CmdDataTypes_t::IndexOf<GpuMsg::CmdSetStencilReference> :		_CmdSetStencilReference( data );		break;
				case CmdDataTypes_t::IndexOf<GpuMsg::CmdBeginRenderPass> :			_CmdBeginRenderPass( data );			break;
				case CmdDataTypes_t::IndexOf<GpuMsg::CmdEndRenderPass> :			_CmdEndRenderPass( data );				break;
				case CmdDataTypes_t::IndexOf<GpuMsg::CmdBindGraphicsPipeline> :		_CmdBindGraphicsPipeline( data );		break;
				case CmdDataTypes_t::IndexOf<GpuMsg::CmdBindComputePipeline> :		_CmdBindComputePipeline( data );		break;
				case CmdDataTypes_t::IndexOf<GpuMsg::CmdBindVertexBuffers> :		_CmdBindVertexBuffers( data );			break;
				case CmdDataTypes_t::IndexOf<GpuMsg::CmdBindIndexBuffer> :			_CmdBindIndexBuffer( data );			break;
				case CmdDataTypes_t::IndexOf<GpuMsg::CmdDraw> :						_CmdDraw( data );						break;
				case CmdDataTypes_t::IndexOf<GpuMsg::CmdDrawIndexed> :				_CmdDrawIndexed( data );				break;
				case CmdDataTypes_t::IndexOf<GpuMsg::CmdDrawIndirect> :				_CmdDrawIndirect( data );				break;
				case CmdDataTypes_t::IndexOf<GpuMsg::CmdDrawIndexedIndirect> :		_CmdDrawIndexedIndirect( data );		break;
				case CmdDataTypes_t::IndexOf<GpuMsg::CmdDispatch> :					_CmdDispatch( data );					break;
				case CmdDataTypes_t::IndexOf<GpuMsg::CmdDispatchIndirect> :			_CmdDispatchIndirect( data );			break;
				case CmdDataTypes_t::IndexOf<GpuMsg::CmdExecute> :					_CmdExecute( data );					break;
				case CmdDataTypes_t::IndexOf<GpuMsg::CmdBindGraphicsResourceTable> :_CmdBindGraphicsResourceTable( data );	break;
				case CmdDataTypes_t::IndexOf<GpuMsg::CmdBindComputeResourceTable> :	_CmdBindComputeResourceTable( data );	break;
				case CmdDataTypes_t::IndexOf<GpuMsg::CmdCopyBuffer> :				_CmdCopyBuffer( data );					break;
				case CmdDataTypes_t::IndexOf<GpuMsg::CmdCopyImage> :				_CmdCopyImage( data );					break;
				case CmdDataTypes_t::IndexOf<GpuMsg::CmdCopyBufferToImage> :		_CmdCopyBufferToImage( data );			break;
				case CmdDataTypes_t::IndexOf<GpuMsg::CmdCopyImageToBuffer> :		_CmdCopyImageToBuffer( data );			break;
				case CmdDataTypes_t::IndexOf<GpuMsg::CmdBlitImage> :				_CmdBlitImage( data );					break;
				case CmdDataTypes_t::IndexOf<GpuMsg::CmdBlitGLFramebuffers> :		_CmdBlitGLFramebuffers ( data );		break;
				case CmdDataTypes_t::IndexOf<GpuMsg::GLCmdUpdateBuffer> :			_CmdUpdateBuffer( data );				break;
				case CmdDataTypes_t::IndexOf<GpuMsg::CmdFillBuffer> :				_CmdFillBuffer( data );					break;
				case CmdDataTypes_t::IndexOf<GpuMsg::CmdClearAttachments> :			_CmdClearAttachments( data );			break;
				case CmdDataTypes_t::IndexOf<GpuMsg::CmdClearColorImage> :			_CmdClearColorImage( data );			break;
				case CmdDataTypes_t::IndexOf<GpuMsg::CmdClearDepthStencilImage> :	_CmdClearDepthStencilImage( data );		break;
				case CmdDataTypes_t::IndexOf<GpuMsg::CmdPipelineBarrier> :			_CmdPipelineBarrier( data );			break;
				default :															WARNING( "unknown command!" );
			}
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

		_SendEvent< GpuMsg::OnCommandBufferStateChanged >({ old_state, newState });
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
			_tempBuffer->Send< ModuleMsg::Delete >({});
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
		SendTo< GpuMsg::GLPipelineResourceTableApply >( _resourceTable, { _pipelineStages });
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

		if ( not state.enabled ) {
			GL_CALL( glDisable( GL_DEPTH_TEST ) );
			return;
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
		GL_CALL( glViewport( _renderPassArea.left, _renderPassArea.bottom, _renderPassArea.Width(), _renderPassArea.Height() ) );
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
						 value.first <= GL_COLOR_ATTACHMENT0 + GlobalConst::Graphics_MaxColorBuffers )
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

		GL_CALL( glInvalidateSubFramebuffer( GL_DRAW_FRAMEBUFFER, (GLsizei)buffers.Count(), buffers.ptr(),
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
		SendTo< GpuMsg::GLPipelineResourceTableApply >( _resourceTable, { _pipelineStages });
		return true;
	}
	
/*
=================================================
	_CmdSetViewport
=================================================
*/
	bool GL4CommandBuffer::_CmdSetViewport (const Command_t &cmd)
	{
		const auto&	data = cmd.data.Get< GpuMsg::CmdSetViewport >();

		FOR( i, data.viewports ) {
			_viewports[i + data.firstViewport] = data.viewports[i];
		}
		return true;
	}
	
/*
=================================================
	_CmdSetScissor
=================================================
*/
	bool GL4CommandBuffer::_CmdSetScissor (const Command_t &cmd)
	{
		const auto&	data = cmd.data.Get< GpuMsg::CmdSetScissor >();

		FOR( i, data.scissors ) {
			_scissors[i + data.firstScissor] = data.scissors[i];
		}
		return true;
	}
	
/*
=================================================
	_CmdSetDepthBounds
=================================================
*/
	bool GL4CommandBuffer::_CmdSetDepthBounds (const Command_t &cmd)
	{
		const auto&	data = cmd.data.Get< GpuMsg::CmdSetDepthBounds >();

		TODO("");
		return true;
	}
	
/*
=================================================
	_CmdSetBlendColor
=================================================
*/
	bool GL4CommandBuffer::_CmdSetBlendColor (const Command_t &cmd)
	{
		const auto&	data = cmd.data.Get< GpuMsg::CmdSetBlendColor >();

		GL_CALL( glBlendColor( data.color.R(), data.color.G(), data.color.B(), data.color.A() ) );
		return true;
	}
	
/*
=================================================
	_CmdSetDepthBias
=================================================
*/
	bool GL4CommandBuffer::_CmdSetDepthBias (const Command_t &cmd)
	{
		const auto&	data = cmd.data.Get< GpuMsg::CmdSetDepthBias >();

		// glPolygonOffset ?
		TODO("");
		return true;
	}
	
/*
=================================================
	_CmdSetLineWidth
=================================================
*/
	bool GL4CommandBuffer::_CmdSetLineWidth (const Command_t &cmd)
	{
		const auto&	data = cmd.data.Get< GpuMsg::CmdSetLineWidth >();

		GL_CALL( glLineWidth( data.width ) );
		return true;
	}
	
/*
=================================================
	_CmdSetStencilCompareMask
=================================================
*/
	bool GL4CommandBuffer::_CmdSetStencilCompareMask (const Command_t &cmd)
	{
		const auto&	data = cmd.data.Get< GpuMsg::CmdSetStencilCompareMask >();

		// glStencilFunc - mask ?
		TODO("");
		return true;
	}
	
/*
=================================================
	_CmdSetStencilWriteMask
=================================================
*/
	bool GL4CommandBuffer::_CmdSetStencilWriteMask (const Command_t &cmd)
	{
		const auto&	data = cmd.data.Get< GpuMsg::CmdSetStencilWriteMask >();

		// glStencilMask ?
		TODO("");
		return true;
	}
	
/*
=================================================
	_CmdSetStencilReference
=================================================
*/
	bool GL4CommandBuffer::_CmdSetStencilReference (const Command_t &cmd)
	{
		const auto&	data = cmd.data.Get< GpuMsg::CmdSetStencilReference >();

		// glStencilFunc - ref ?
		TODO("");
		return true;
	}
	
/*
=================================================
	_CmdBeginRenderPass
=================================================
*/
	bool GL4CommandBuffer::_CmdBeginRenderPass (const Command_t &cmd)
	{
		const auto&	data = cmd.data.Get< GpuMsg::CmdBeginRenderPass >();
		
		Message< GpuMsg::GetGLFramebufferID >		req_fb;
		Message< GpuMsg::GetFramebufferDescriptor >	req_fb_descr;
		Message< GpuMsg::GetGLRenderPassID >		req_rp;
		Message< GpuMsg::GetRenderPassDescriptor >	req_rp_descr;

		SendTo( data.framebuffer, req_fb );
		SendTo( data.framebuffer, req_fb_descr );
		SendTo( data.renderPass, req_rp );
		SendTo( data.renderPass, req_rp_descr );
		
		_renderPassData		= *req_rp->result;
		_framebufferSize	= uint3( req_fb_descr->result->size, req_fb_descr->result->layers );
		_renderPassArea		= data.area;
		_renderSubpasses	= req_rp_descr->result->Subpasses();

		GLuint	fb_id		= req_fb->result.Get(0);

		GL_CALL( glBindFramebuffer( GL_DRAW_FRAMEBUFFER, fb_id ) );
		GL_CALL( glDrawBuffers( GLsizei(_renderPassData.draw.colorBuffers.Count()), _renderPassData.draw.colorBuffers.ptr() ) );

		_ClearRenderPassAttachment( data.clearValues );
		_InvalidateRenderPassAttachment( _renderPassData.invalidateBefore );
		return true;
	}
	
/*
=================================================
	_CmdEndRenderPass
=================================================
*/
	bool GL4CommandBuffer::_CmdEndRenderPass (const Command_t &cmd)
	{
		const auto&	data = cmd.data.Get< GpuMsg::CmdEndRenderPass >();
		
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
	_CmdBindGraphicsPipeline
=================================================
*/
	bool GL4CommandBuffer::_CmdBindGraphicsPipeline (const Command_t &cmd)
	{
		const auto&	data = cmd.data.Get< GpuMsg::CmdBindGraphicsPipeline >();

		Message< GpuMsg::GetGLGraphicsPipelineID >			req_id;
		Message< GpuMsg::GetGraphicsPipelineDescriptor >	req_descr;

		SendTo( data.pipeline, req_id );
		SendTo( data.pipeline, req_descr );

		_pipelineStages			= req_id->result->programs;
		_pipelineObj			= req_id->result->pipeline;
		_vertexAttribs			= req_id->result->vertexAttribs;
		_graphicsPipelineDescr	<< req_descr->result;
		_computePipelineDescr	= Uninitialized;
		_resourceTable			= null;

		return true;
	}
	
/*
=================================================
	_CmdBindComputePipeline
=================================================
*/
	bool GL4CommandBuffer::_CmdBindComputePipeline (const Command_t &cmd)
	{
		const auto&	data = cmd.data.Get< GpuMsg::CmdBindComputePipeline >();
		
		Message< GpuMsg::GetGLComputePipelineID >		req_id;
		Message< GpuMsg::GetComputePipelineDescriptor >	req_descr;

		SendTo( data.pipeline, req_id );
		SendTo( data.pipeline, req_descr );

		_pipelineStages.Clear();
		_pipelineStages[ EShader::Compute ] = req_id->result->program;
		_pipelineObj			= req_id->result->pipeline;
		_vertexAttribs			= 0;
		_resourceTable			= null;
		_graphicsPipelineDescr	= Uninitialized;
		_computePipelineDescr	<< req_descr->result;

		return true;
	}
	
/*
=================================================
	_CmdBindVertexBuffers
=================================================
*/
	bool GL4CommandBuffer::_CmdBindVertexBuffers (const Command_t &cmd)
	{
		const auto&	data = cmd.data.Get< GpuMsg::CmdBindVertexBuffers >();

		_vertexBuffers.Clear();

		FOR( i, data.vertexBuffers )
		{
			Message< GpuMsg::GetGLBufferID >	req_id;
			SendTo( data.vertexBuffers[i], req_id );

			_vertexBuffers[ data.firstBinding+i ] = GLBuffer{ req_id->result.Get(0), data.offsets[i] }; 
		}
		return true;
	}
	
/*
=================================================
	_CmdBindIndexBuffer
=================================================
*/
	bool GL4CommandBuffer::_CmdBindIndexBuffer (const Command_t &cmd)
	{
		const auto&	data = cmd.data.Get< GpuMsg::CmdBindIndexBuffer >();
		
		Message< GpuMsg::GetGLBufferID >	req_id;
		SendTo( data.indexBuffer, req_id );

		_indexBuffer = GLBuffer{ req_id->result.Get(0),	data.offset };
		_indexType	 = data.indexType;
		return true;
	}
	
/*
=================================================
	_CmdDraw
=================================================
*/
	bool GL4CommandBuffer::_CmdDraw (const Command_t &cmd)
	{
		const auto&	data = cmd.data.Get< GpuMsg::CmdDraw >();

		CHECK_ERR( _PrepareForDraw() );
		GL_CALL( glDrawArraysInstancedBaseInstance( GL4Enum( _primitive ), data.firstVertex,
								data.vertexCount, data.instanceCount, data.firstInstance ) );
		return true;
	}
	
/*
=================================================
	_CmdDrawIndexed
=================================================
*/
	bool GL4CommandBuffer::_CmdDrawIndexed (const Command_t &cmd)
	{
		CHECK_ERR( _PrepareForDraw() );
		CHECK_ERR( _indexBuffer.id != 0 );
		CHECK_ERR( _indexType != EIndex::Unknown );

		const auto&	data	= cmd.data.Get< GpuMsg::CmdDrawIndexed >();
		const usize offset	= usize(_indexBuffer.offset + EIndex::Sizeof( _indexType ) * data.firstIndex);
		
		GL_CALL( glDrawElementsInstancedBaseInstance( GL4Enum( _primitive ), data.indexCount, GL4Enum( _indexType ),
													  (void *)offset, data.instanceCount, data.firstInstance ) );
		return true;
	}
	
/*
=================================================
	_CmdDrawIndirect
=================================================
*/
	bool GL4CommandBuffer::_CmdDrawIndirect (const Command_t &cmd)
	{
		const auto&	data = cmd.data.Get< GpuMsg::CmdDrawIndirect >();

		TODO("");
		return true;
	}
	
/*
=================================================
	_CmdDrawIndexedIndirect
=================================================
*/
	bool GL4CommandBuffer::_CmdDrawIndexedIndirect (const Command_t &cmd)
	{
		const auto&	data = cmd.data.Get< GpuMsg::CmdDrawIndexedIndirect >();

		TODO("");
		return true;
	}
	
/*
=================================================
	_CmdDispatch
=================================================
*/
	bool GL4CommandBuffer::_CmdDispatch (const Command_t &cmd)
	{
		const auto&	data = cmd.data.Get< GpuMsg::CmdDispatch >();
		
		CHECK_ERR( All( ulong3(data.groupCount) <= _maxWorkGroupCount ) );

		CHECK_ERR( _PrepareForCompute() );
		GL_CALL( glDispatchCompute( data.groupCount.x, data.groupCount.y, data.groupCount.z ) );
		return true;
	}
	
/*
=================================================
	_CmdDispatchIndirect
=================================================
*/
	bool GL4CommandBuffer::_CmdDispatchIndirect (const Command_t &cmd)
	{
		const auto&	data = cmd.data.Get< GpuMsg::CmdDispatchIndirect >();

		TODO("");
		return true;
	}
	
/*
=================================================
	_CmdExecute
=================================================
*/
	bool GL4CommandBuffer::_CmdExecute (const Command_t &cmd)
	{
		const auto&	data = cmd.data.Get< GpuMsg::CmdExecute >();
		
		ModuleUtils::Send( data.cmdBuffers, Message<GpuMsg::SetCommandBufferState>{ GpuMsg::SetCommandBufferState::EState::Pending });
		ModuleUtils::Send( data.cmdBuffers, Message<GpuMsg::ExecuteGLCommandBuffer>{} );
		//ModuleUtils::Send( data.cmdBuffers, Message<GpuMsg::SetCommandBufferState>{ GpuMsg::SetCommandBufferState::EState::Completed });

		return true;
	}
	
/*
=================================================
	_CmdBindGraphicsResourceTable
=================================================
*/
	bool GL4CommandBuffer::_CmdBindGraphicsResourceTable (const Command_t &cmd)
	{
		const auto&	data = cmd.data.Get< GpuMsg::CmdBindGraphicsResourceTable >();
		
		ASSERT( data.index == 0 );
		_resourceTable = data.resourceTable;
		return true;
	}
	
/*
=================================================
	_CmdBindComputeResourceTable
=================================================
*/
	bool GL4CommandBuffer::_CmdBindComputeResourceTable (const Command_t &cmd)
	{
		const auto&	data = cmd.data.Get< GpuMsg::CmdBindComputeResourceTable >();
		
		ASSERT( data.index == 0 );
		_resourceTable = data.resourceTable;
		return true;
	}
	
/*
=================================================
	_CmdCopyBuffer
=================================================
*/
	bool GL4CommandBuffer::_CmdCopyBuffer (const Command_t &cmd)
	{
		const auto&	data = cmd.data.Get< GpuMsg::CmdCopyBuffer >();

		TODO("");
		return true;
	}
	
/*
=================================================
	_CmdCopyImage
=================================================
*/
	bool GL4CommandBuffer::_CmdCopyImage (const Command_t &cmd)
	{
		const auto&	data = cmd.data.Get< GpuMsg::CmdCopyImage >();

		TODO("");
		return true;
	}
	
/*
=================================================
	_CmdCopyBufferToImage
=================================================
*/
	bool GL4CommandBuffer::_CmdCopyBufferToImage (const Command_t &cmd)
	{
		const auto&	data = cmd.data.Get< GpuMsg::CmdCopyBufferToImage >();

		TODO("");
		return true;
	}
	
/*
=================================================
	_CmdCopyImageToBuffer
=================================================
*/
	bool GL4CommandBuffer::_CmdCopyImageToBuffer (const Command_t &cmd)
	{
		const auto&	data = cmd.data.Get< GpuMsg::CmdCopyImageToBuffer >();

		TODO("");
		return true;
	}
	
/*
=================================================
	_CmdBlitImage
=================================================
*/
	bool GL4CommandBuffer::_CmdBlitImage (const Command_t &cmd)
	{
		const auto&	data = cmd.data.Get< GpuMsg::CmdBlitImage >();

		const auto	CheckFBStatus = LAMBDA() (GLuint fb, GLenum target) 
									{
										GLenum status = 0;
										GL_CALL( status = glCheckFramebufferStatus( target ) );
										return status == GL_FRAMEBUFFER_COMPLETE;
									};

		if ( _srcBlitFramebuffer == 0 ) {
			GL_CALL( glGenFramebuffers( 1, &_srcBlitFramebuffer ) );
		}
		if ( _dstBlitFramebuffer == 0 ) {
			GL_CALL( glGenFramebuffers( 1, &_dstBlitFramebuffer ) );
		}

		Message< GpuMsg::GetGLImageID >			req_src_id;
		Message< GpuMsg::GetGLImageID >			req_dst_id;
		Message< GpuMsg::GetImageDescriptor >	req_src_descr;
		Message< GpuMsg::GetImageDescriptor >	req_dst_descr;

		data.srcImage->Send( req_src_id );
		data.srcImage->Send( req_src_descr );
		data.dstImage->Send( req_dst_id );
		data.dstImage->Send( req_dst_descr );
		
		GL_CALL( glDisable( GL_SCISSOR_TEST ) );

		GL_CALL( glBindFramebuffer( GL_READ_FRAMEBUFFER, _srcBlitFramebuffer ) );
		GL_CALL( glFramebufferTexture( GL_READ_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, req_src_id->result.Get(0), 0 ) );
		GL_CALL( glReadBuffer( GL_COLOR_ATTACHMENT0 ) );
		CHECK_ERR( CheckFBStatus( _srcBlitFramebuffer, GL_READ_FRAMEBUFFER ) );

		GL_CALL( glBindFramebuffer( GL_DRAW_FRAMEBUFFER, _dstBlitFramebuffer ) );
		GL_CALL( glFramebufferTexture( GL_READ_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, req_src_id->result.Get(0), 0 ) );
		GL_CALL( glDrawBuffer( GL_COLOR_ATTACHMENT0 ) );
		CHECK_ERR( CheckFBStatus( _dstBlitFramebuffer, GL_DRAW_FRAMEBUFFER ) );

		const GLenum	filter = data.linearFilter ? GL_LINEAR : GL_NEAREST;

		FOR( i, data.regions )
		{
			const auto& r = data.regions[i];

			// depth image not supported, yet
			ASSERT( r.dstOffset0.z == 0 and r.dstOffset1.z == 0 );
			ASSERT( r.srcOffset0.z == 0 and r.srcOffset1.z == 0 );

			ASSERT( r.srcLayers.aspectMask == r.dstLayers.aspectMask );
			ASSERT( r.srcLayers.baseLayer == ImageLayer(0) and r.dstLayers.baseLayer == ImageLayer(0) );
			ASSERT( r.srcLayers.layerCount == 1 and r.dstLayers.layerCount == 1 );
			ASSERT( r.srcLayers.mipLevel == MipmapLevel(0) and r.dstLayers.mipLevel == MipmapLevel(0) );
			
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
	_CmdBlitGLFramebuffers
=================================================
*/
	bool GL4CommandBuffer::_CmdBlitGLFramebuffers (const Command_t &cmd)
	{
		const auto&	data = cmd.data.Get< GpuMsg::CmdBlitGLFramebuffers >();

		Message< GpuMsg::GetGLFramebufferID >	req_src_id;
		Message< GpuMsg::GetGLFramebufferID >	req_dst_id;
		
		data.srcFramebuffer->Send( req_src_id );
		data.dstFramebuffer->Send( req_dst_id );

		CHECK_ERR( req_src_id->result and req_dst_id->result );

		GL_CALL( glDisable( GL_SCISSOR_TEST ) );
		GL_CALL( glBindFramebuffer( GL_READ_FRAMEBUFFER, *req_src_id->result ) );
		GL_CALL( glBindFramebuffer( GL_DRAW_FRAMEBUFFER, *req_dst_id->result ) );

		GL_CALL( glReadBuffer( GL_COLOR_ATTACHMENT0 ) );
		GL_CALL( glDrawBuffer( GL_COLOR_ATTACHMENT0 ) );
		
		const GLenum	filter	= data.linearFilter ? GL_LINEAR : GL_NEAREST;

		const GLenum	buffers	= (data.imageAspect[ EImageAspect::Color ] ? GL_COLOR_BUFFER_BIT : 0) |
								  (data.imageAspect[ EImageAspect::Depth ] ? GL_DEPTH_BUFFER_BIT : 0) |
								  (data.imageAspect[ EImageAspect::Stencil ] ? GL_STENCIL_BUFFER_BIT : 0);
		CHECK_ERR( buffers != 0 );


		FOR( i, data.regions )
		{
			const auto&	r = data.regions[i];

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
	_CmdUpdateBuffer
=================================================
*/
	bool GL4CommandBuffer::_CmdUpdateBuffer (const Command_t &cmd)
	{
		const auto&	data = cmd.data.Get< GpuMsg::GLCmdUpdateBuffer >();
		
		Message< GpuMsg::GetGLBufferID >		req_dst_id;
		Message< GpuMsg::GetGLBufferID >		req_src_id;
		Message< GpuMsg::GetBufferDescriptor >	req_descr;
		
		data.dstBuffer->Send( req_dst_id );
		data.dstBuffer->Send( req_descr );
		_tempBuffer->Send( req_src_id );
		
		CHECK_ERR( data.dstOffset < req_descr->result->size );
		CHECK_ERR( data.size + data.dstOffset <= req_descr->result->size );

		GL_CALL( glNamedCopyBufferSubData( *req_src_id->result,
										   *req_dst_id->result,
										   GLintptr(data.srcOffset),
										   GLintptr(data.dstOffset),
										   GLsizeiptr(data.size) ) );
		return true;
	}
	
/*
=================================================
	_CmdFillBuffer
=================================================
*/
	bool GL4CommandBuffer::_CmdFillBuffer (const Command_t &cmd)
	{
		const auto&	data = cmd.data.Get< GpuMsg::CmdFillBuffer >();

		Message< GpuMsg::GetGLBufferID >		req_id;
		Message< GpuMsg::GetBufferDescriptor >	req_descr;

		data.dstBuffer->Send( req_id );
		data.dstBuffer->Send( req_descr );

		CHECK_ERR( data.dstOffset < req_descr->result->size );

		GL_CALL( glBindBuffer( GL_COPY_READ_BUFFER, *req_id->result ) );
		GL_CALL( glClearBufferSubData( GL_COPY_READ_BUFFER,
										GL_R32UI,
										GLintptr(data.dstOffset),
										GLsizeiptr(Min( data.size, req_descr->result->size - data.dstOffset )),
										GL_RED, GL_UNSIGNED_INT,
										&data.pattern ) );

		GL_CALL( glBindBuffer( GL_COPY_READ_BUFFER, 0 ) );
		return true;
	}
	
/*
=================================================
	_CmdClearAttachments
=================================================
*/
	bool GL4CommandBuffer::_CmdClearAttachments (const Command_t &cmd)
	{
		using ValueList			= GpuMsg::CmdClearAttachments::ClearValue_t::TypeList_t;
		using DepthStencil_t	= GpuMsg::CmdClearAttachments::DepthStencil_t;

		const auto&	data = cmd.data.Get< GpuMsg::CmdClearAttachments >();

		GL_CALL( glEnable( GL_SCISSOR_TEST ) );
		GL_CALL( glViewport( _renderPassArea.left, _renderPassArea.bottom, _renderPassArea.Width(), _renderPassArea.Height() ) );

		FOR( i, data.clearRects )
		{
			auto	r = data.clearRects[i];
			
			r.rect.LeftBottom()	= Max( r.rect.LeftBottom(), _renderPassArea.LeftBottom() );
			r.rect.RightTop()	= Clamp( r.rect.RightTop(), _renderPassArea.LeftBottom(), _renderPassArea.RightTop() );

			GL_CALL( glScissor( r.rect.left, r.rect.bottom, r.rect.Width(), r.rect.Height() ) );

			FOR( j, data.attachments )
			{
				const auto&	att		= data.attachments[j];
				const bool	is_ds	= att.attachmentIdx >= _renderPassData.draw.colorBuffers.Count();

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
	_CmdClearColorImage
=================================================
*/
	bool GL4CommandBuffer::_CmdClearColorImage (const Command_t &cmd)
	{
		using Colors_t	= GpuMsg::CmdClearColorImage::ClearColor_t::TypeList_t;

		const auto&	data = cmd.data.Get< GpuMsg::CmdClearColorImage >();
		
		Message< GpuMsg::GetGLImageID >			req_id;
		Message< GpuMsg::GetImageDescriptor >	req_descr;
		
		// TODO: check result
		SendTo( data.image, req_id );
		SendTo( data.image, req_descr );
		
		CHECK_ERR( req_descr->result->usage[ EImageUsage::TransferDst ] );

		GLenum	clear_fmt	= GL_RGBA;
		GLenum	clear_type	= GL_FLOAT;

		switch ( data.clearValue.GetCurrentIndex() )
		{
			case Colors_t::IndexOf<float4> :	clear_type = GL_FLOAT;			clear_fmt = GL_RGBA;			break;
			case Colors_t::IndexOf<uint4> :		clear_type = GL_UNSIGNED_INT;	clear_fmt = GL_RGBA_INTEGER;	break;
			case Colors_t::IndexOf<int4> :		clear_type = GL_INT;			clear_fmt = GL_RGBA_INTEGER;	break;
			default :							RETURN_ERR( "unsupported" );
		}
		
		FOR( i, data.ranges )
		{
			auto const&		src = data.ranges[i];

			ASSERT( src.baseLayer.Get() == 0 and src.layerCount == 1 );	// TODO: use glClearTexSubImage to clear layers

			for (uint j = 0; j < src.levelCount; ++j)
			{
				glClearTexImage( req_id->result.Get(),
								 src.baseMipLevel.Get() + j,
								 clear_fmt,
								 clear_type,
								 data.clearValue.GetData().ptr() );
			}
		}

		return true;
	}
	
/*
=================================================
	_CmdClearDepthStencilImage
=================================================
*/
	bool GL4CommandBuffer::_CmdClearDepthStencilImage (const Command_t &cmd)
	{
		const auto&	data = cmd.data.Get< GpuMsg::CmdClearDepthStencilImage >();

		TODO("");
		return true;
	}
	
/*
=================================================
	_CmdPipelineBarrier
=================================================
*/
	bool GL4CommandBuffer::_CmdPipelineBarrier (const Command_t &cmd)
	{
		const auto&	data = cmd.data.Get< GpuMsg::CmdPipelineBarrier >();

		//TODO("");
		return true;
	}

}	// PlatformGL
//-----------------------------------------------------------------------------

namespace Platforms
{
	ModulePtr OpenGLObjectsConstructor::CreateGL4CommandBuffer (GlobalSystemsRef gs, const CreateInfo::GpuCommandBuffer &ci)
	{
		return New< PlatformGL::GL4CommandBuffer >( gs, ci );
	}
}	// Platforms
}	// Engine

#endif	// GRAPHICS_API_OPENGL
