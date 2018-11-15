// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Core/Config/Engine.Config.h"

#ifdef GRAPHICS_API_OPENGL

#include "Engine/Platforms/Public/GPU/CommandBuffer.h"
#include "Engine/Platforms/Public/GPU/Image.h"
#include "Engine/Platforms/Public/GPU/Buffer.h"
#include "Engine/Platforms/Public/GPU/Pipeline.h"
#include "Engine/Platforms/OpenGL/450/GL4BaseModule.h"
#include "Engine/Platforms/OpenGL/OpenGLObjectsConstructor.h"

namespace Engine
{
namespace PlatformGL
{

	//
	// OpenGL Command Buffer Builder
	//

	class GL4CommandBuilder final : public GL4BaseModule
	{
	// types
	private:
		using SupportedMessages_t	= GL4BaseModule::SupportedMessages_t::Append< MessageListFrom<
											GpuMsg::CmdBegin,
											GpuMsg::CmdEnd,
											GpuMsg::CmdBlitGLFramebuffers,
											GpuMsg::SetCommandBufferDependency,
											GpuMsg::GetCommandBufferState
										> >;

		using SupportedEvents_t		= GL4BaseModule::SupportedEvents_t;

		using CmdBufferMsg_t		= MessageListFrom< GpuMsg::SetGLCommandBufferQueue, GpuMsg::SetCommandBufferDependency,
														GpuMsg::SetCommandBufferState, GpuMsg::GetCommandBufferState >;

		using UsedResources_t		= Set< ModulePtr >;
		using CommandArray_t		= Array< GpuMsg::SetGLCommandBufferQueue::Command >;
		
		using DynamicStates_t		= EPipelineDynamicState::bits;
		using ERecordingState		= GpuMsg::SetCommandBufferState::EState;
		
		enum class EScope
		{
			None,
			Command,
			RenderPass,
		};


	// constants
	private:
		static const TypeIdList		_eventTypes;


	// variables
	private:
		UsedResources_t			_resources;
		CommandArray_t			_commands;
		BinaryArray				_bufferData;	// used to store data for UpdateBuffer command
		BinaryArray				_pushConstData;	// used to store data for PushConstants command
		ModulePtr				_cmdBuffer;		// current command buffer
		EScope					_scope;
		DynamicStates_t			_dynamicStates;


	// methods
	public:
		GL4CommandBuilder (UntypedID_t, GlobalSystemsRef gs, const CreateInfo::GpuCommandBuilder &ci);
		~GL4CommandBuilder ();


	// message handlers
	private:
		bool _Compose (const ModuleMsg::Compose &);
		bool _Delete (const ModuleMsg::Delete &);
		bool _SetCommandBufferDependency (const GpuMsg::SetCommandBufferDependency &);
		bool _GetCommandBufferState (const GpuMsg::GetCommandBufferState &);
		
		bool _CmdBegin (const GpuMsg::CmdBegin &);
		bool _CmdEnd (const GpuMsg::CmdEnd &);

		bool _CmdSetViewport (const GpuMsg::CmdSetViewport &);
		bool _CmdSetScissor (const GpuMsg::CmdSetScissor &);
		bool _CmdSetDepthBounds (const GpuMsg::CmdSetDepthBounds &);
		bool _CmdSetBlendColor (const GpuMsg::CmdSetBlendColor &);
		bool _CmdSetDepthBias (const GpuMsg::CmdSetDepthBias &);
		bool _CmdSetLineWidth (const GpuMsg::CmdSetLineWidth &);
		bool _CmdSetStencilCompareMask (const GpuMsg::CmdSetStencilCompareMask &);
		bool _CmdSetStencilWriteMask (const GpuMsg::CmdSetStencilWriteMask &);
		bool _CmdSetStencilReference (const GpuMsg::CmdSetStencilReference &);
		bool _CmdBeginRenderPass (const GpuMsg::CmdBeginRenderPass &);
		bool _CmdEndRenderPass (const GpuMsg::CmdEndRenderPass &);
		bool _CmdNextSubpass (const GpuMsg::CmdNextSubpass &);
		bool _CmdBindGraphicsPipeline (const GpuMsg::CmdBindGraphicsPipeline &);
		bool _CmdBindComputePipeline (const GpuMsg::CmdBindComputePipeline &);
		bool _CmdBindVertexBuffers (const GpuMsg::CmdBindVertexBuffers &);
		bool _CmdBindIndexBuffer (const GpuMsg::CmdBindIndexBuffer &);
		bool _CmdDraw (const GpuMsg::CmdDraw &);
		bool _CmdDrawIndexed (const GpuMsg::CmdDrawIndexed &);
		bool _CmdDrawIndirect (const GpuMsg::CmdDrawIndirect &);
		bool _CmdDrawIndexedIndirect (const GpuMsg::CmdDrawIndexedIndirect &);
		bool _CmdDispatch (const GpuMsg::CmdDispatch &);
		bool _CmdDispatchIndirect (const GpuMsg::CmdDispatchIndirect &);
		bool _CmdExecute (const GpuMsg::CmdExecute &);
		bool _CmdBindGraphicsResourceTable (const GpuMsg::CmdBindGraphicsResourceTable &);
		bool _CmdBindComputeResourceTable (const GpuMsg::CmdBindComputeResourceTable &);
		bool _CmdCopyBuffer (const GpuMsg::CmdCopyBuffer &);
		bool _CmdCopyImage (const GpuMsg::CmdCopyImage &);
		bool _CmdCopyBufferToImage (const GpuMsg::CmdCopyBufferToImage &);
		bool _CmdCopyImageToBuffer (const GpuMsg::CmdCopyImageToBuffer &);
		bool _CmdBlitImage (const GpuMsg::CmdBlitImage &);
		bool _CmdResolveImage (const GpuMsg::CmdResolveImage &);
		bool _CmdBlitGLFramebuffers (const GpuMsg::CmdBlitGLFramebuffers &);
		bool _CmdUpdateBuffer (const GpuMsg::CmdUpdateBuffer &);
		bool _CmdFillBuffer (const GpuMsg::CmdFillBuffer &);
		bool _CmdClearAttachments (const GpuMsg::CmdClearAttachments &);
		bool _CmdClearColorImage (const GpuMsg::CmdClearColorImage &);
		bool _CmdClearDepthStencilImage (const GpuMsg::CmdClearDepthStencilImage &);
		bool _CmdPipelineBarrier (const GpuMsg::CmdPipelineBarrier &);
		bool _CmdPushConstants (const GpuMsg::CmdPushConstants &);
		bool _CmdPushNamedConstants (const GpuMsg::CmdPushNamedConstants &);
		bool _CmdDebugMarker (const GpuMsg::CmdDebugMarker &);
		bool _CmdPushDebugGroup (const GpuMsg::CmdPushDebugGroup &);
		bool _CmdPopDebugGroup (const GpuMsg::CmdPopDebugGroup &);
		bool _CmdBeginQuery (const GpuMsg::CmdBeginQuery &);
		bool _CmdEndQuery (const GpuMsg::CmdEndQuery &);
		bool _CmdCopyQueryPoolResults (const GpuMsg::CmdCopyQueryPoolResults &);
		bool _CmdWriteTimestamp (const GpuMsg::CmdWriteTimestamp &);
		bool _CmdResetQueryPool (const GpuMsg::CmdResetQueryPool &);
	};
//-----------------------------------------------------------------------------


	
	const TypeIdList	GL4CommandBuilder::_eventTypes{ UninitializedT< SupportedEvents_t >() };

/*
=================================================
	constructor
=================================================
*/
	GL4CommandBuilder::GL4CommandBuilder (UntypedID_t id, GlobalSystemsRef gs, const CreateInfo::GpuCommandBuilder &ci) :
		GL4BaseModule( gs, ModuleConfig{ id, UMax }, &_eventTypes ),
		_scope( EScope::None )
	{
		SetDebugName( "GL4CommandBuilder" );
		
		_SubscribeOnMsg( this, &GL4CommandBuilder::_OnModuleAttached_Impl );
		_SubscribeOnMsg( this, &GL4CommandBuilder::_OnModuleDetached_Impl );
		_SubscribeOnMsg( this, &GL4CommandBuilder::_AttachModule_Impl );
		_SubscribeOnMsg( this, &GL4CommandBuilder::_DetachModule_Impl );
		_SubscribeOnMsg( this, &GL4CommandBuilder::_FindModule_Impl );
		_SubscribeOnMsg( this, &GL4CommandBuilder::_ModulesDeepSearch_Impl );
		_SubscribeOnMsg( this, &GL4CommandBuilder::_Link_Impl );
		_SubscribeOnMsg( this, &GL4CommandBuilder::_Compose );
		_SubscribeOnMsg( this, &GL4CommandBuilder::_Delete );
		_SubscribeOnMsg( this, &GL4CommandBuilder::_OnManagerChanged );
		_SubscribeOnMsg( this, &GL4CommandBuilder::_GetDeviceInfo );
		_SubscribeOnMsg( this, &GL4CommandBuilder::_GetGLDeviceInfo );
		_SubscribeOnMsg( this, &GL4CommandBuilder::_GetGLPrivateClasses );
		_SubscribeOnMsg( this, &GL4CommandBuilder::_GetCommandBufferState );
		_SubscribeOnMsg( this, &GL4CommandBuilder::_SetCommandBufferDependency );

		_SubscribeOnMsg( this, &GL4CommandBuilder::_CmdSetViewport );
		_SubscribeOnMsg( this, &GL4CommandBuilder::_CmdSetScissor );
		_SubscribeOnMsg( this, &GL4CommandBuilder::_CmdSetDepthBounds );
		_SubscribeOnMsg( this, &GL4CommandBuilder::_CmdSetBlendColor );
		_SubscribeOnMsg( this, &GL4CommandBuilder::_CmdSetDepthBias );
		_SubscribeOnMsg( this, &GL4CommandBuilder::_CmdSetLineWidth );
		_SubscribeOnMsg( this, &GL4CommandBuilder::_CmdSetStencilCompareMask );
		_SubscribeOnMsg( this, &GL4CommandBuilder::_CmdSetStencilWriteMask );
		_SubscribeOnMsg( this, &GL4CommandBuilder::_CmdSetStencilReference );
		_SubscribeOnMsg( this, &GL4CommandBuilder::_CmdBegin );
		_SubscribeOnMsg( this, &GL4CommandBuilder::_CmdEnd );
		_SubscribeOnMsg( this, &GL4CommandBuilder::_CmdBeginRenderPass );
		_SubscribeOnMsg( this, &GL4CommandBuilder::_CmdEndRenderPass );
		_SubscribeOnMsg( this, &GL4CommandBuilder::_CmdNextSubpass );
		_SubscribeOnMsg( this, &GL4CommandBuilder::_CmdBindGraphicsPipeline );
		_SubscribeOnMsg( this, &GL4CommandBuilder::_CmdBindComputePipeline );
		_SubscribeOnMsg( this, &GL4CommandBuilder::_CmdBindVertexBuffers );
		_SubscribeOnMsg( this, &GL4CommandBuilder::_CmdBindIndexBuffer );
		_SubscribeOnMsg( this, &GL4CommandBuilder::_CmdDraw );
		_SubscribeOnMsg( this, &GL4CommandBuilder::_CmdDrawIndexed );
		_SubscribeOnMsg( this, &GL4CommandBuilder::_CmdDrawIndirect );
		_SubscribeOnMsg( this, &GL4CommandBuilder::_CmdDrawIndexedIndirect );
		_SubscribeOnMsg( this, &GL4CommandBuilder::_CmdDispatch );
		_SubscribeOnMsg( this, &GL4CommandBuilder::_CmdDispatchIndirect );
		_SubscribeOnMsg( this, &GL4CommandBuilder::_CmdExecute );
		_SubscribeOnMsg( this, &GL4CommandBuilder::_CmdBindGraphicsResourceTable );
		_SubscribeOnMsg( this, &GL4CommandBuilder::_CmdBindComputeResourceTable );
		_SubscribeOnMsg( this, &GL4CommandBuilder::_CmdCopyBuffer );
		_SubscribeOnMsg( this, &GL4CommandBuilder::_CmdCopyImage );
		_SubscribeOnMsg( this, &GL4CommandBuilder::_CmdCopyBufferToImage );
		_SubscribeOnMsg( this, &GL4CommandBuilder::_CmdCopyImageToBuffer );
		_SubscribeOnMsg( this, &GL4CommandBuilder::_CmdBlitImage );
		_SubscribeOnMsg( this, &GL4CommandBuilder::_CmdResolveImage );
		_SubscribeOnMsg( this, &GL4CommandBuilder::_CmdBlitGLFramebuffers );
		_SubscribeOnMsg( this, &GL4CommandBuilder::_CmdUpdateBuffer );
		_SubscribeOnMsg( this, &GL4CommandBuilder::_CmdFillBuffer );
		_SubscribeOnMsg( this, &GL4CommandBuilder::_CmdClearAttachments );
		_SubscribeOnMsg( this, &GL4CommandBuilder::_CmdClearColorImage );
		_SubscribeOnMsg( this, &GL4CommandBuilder::_CmdClearDepthStencilImage );
		_SubscribeOnMsg( this, &GL4CommandBuilder::_CmdPipelineBarrier );
		_SubscribeOnMsg( this, &GL4CommandBuilder::_CmdPushConstants );
		_SubscribeOnMsg( this, &GL4CommandBuilder::_CmdPushNamedConstants );
		_SubscribeOnMsg( this, &GL4CommandBuilder::_CmdDebugMarker );
		_SubscribeOnMsg( this, &GL4CommandBuilder::_CmdPushDebugGroup );
		_SubscribeOnMsg( this, &GL4CommandBuilder::_CmdPopDebugGroup );
		_SubscribeOnMsg( this, &GL4CommandBuilder::_CmdBeginQuery );
		_SubscribeOnMsg( this, &GL4CommandBuilder::_CmdEndQuery );
		_SubscribeOnMsg( this, &GL4CommandBuilder::_CmdCopyQueryPoolResults );
		_SubscribeOnMsg( this, &GL4CommandBuilder::_CmdWriteTimestamp );
		_SubscribeOnMsg( this, &GL4CommandBuilder::_CmdResetQueryPool );

		ASSERT( _ValidateMsgSubscriptions< SupportedMessages_t >() );

		_AttachSelfToManager( _GetGPUThread( ci.gpuThread ), UntypedID_t(0), true );
	}
		
/*
=================================================
	desctructor
=================================================
*/
	GL4CommandBuilder::~GL4CommandBuilder ()
	{
	}
	
/*
=================================================
	_Compose
=================================================
*/
	bool GL4CommandBuilder::_Compose (const ModuleMsg::Compose &msg)
	{
		if ( _IsComposedState( GetState() ) )
			return true;	// already composed

		CHECK_ERR( GetState() == EState::Linked );


		_SendForEachAttachments( msg );
		
		// very paranoic check
		CHECK( _ValidateAllSubscriptions() );

		CHECK( _SetState( EState::ComposedMutable ) );
		
		_SendUncheckedEvent( ModuleMsg::AfterCompose{} );
		return true;
	}
	
/*
=================================================
	_Delete
=================================================
*/
	bool GL4CommandBuilder::_Delete (const ModuleMsg::Delete &msg)
	{
		_resources.Clear();
		_commands.Clear();

		_SendForEachAttachments( msg );

		return Module::_Delete_Impl( msg );
	}
	
/*
=================================================
	_SetCommandBufferDependency
=================================================
*/
	bool GL4CommandBuilder::_SetCommandBufferDependency (const GpuMsg::SetCommandBufferDependency &msg)
	{
		_resources.AddArray( msg.resources.Get() );
		return true;
	}
	
/*
=================================================
	_GetCommandBufferState
=================================================
*/
	bool GL4CommandBuilder::_GetCommandBufferState (const GpuMsg::GetCommandBufferState &msg)
	{
		if ( not _cmdBuffer )
			return false;
			
		return _cmdBuffer->Send( msg );
	}
	
/*
=================================================
	_CmdBegin
=================================================
*/
	bool GL4CommandBuilder::_CmdBegin (const GpuMsg::CmdBegin &msg)
	{
		CHECK_ERR( _IsComposedState( GetState() ) );
		CHECK_ERR( not _cmdBuffer );
		CHECK_ERR( _scope == EScope::None );
		
		_resources.Clear();		_resources.Reserve( 128 );
		_commands.Clear();		_commands.Reserve( 128 );
		
		// use target command buffer
		if ( msg.targetCmdBuffer )
		{
			CHECK_ERR( msg.targetCmdBuffer->SupportsAllMessages< CmdBufferMsg_t >() );

			_cmdBuffer = msg.targetCmdBuffer;
		}
		else
		// create new command buffer
		{
			CHECK_ERR( GlobalSystems()->modulesFactory->Create(
							GLCommandBufferModuleID,
							GlobalSystems(),
							CreateInfo::GpuCommandBuffer{
								CommandBufferDescription{ msg.flags }
							},
							OUT _cmdBuffer ) );

			_cmdBuffer->SetDebugName( String(GetDebugName()) << "_Buffer" );

			CHECK_ERR( _Attach( "", _cmdBuffer ) );
		}

		ModuleUtils::Initialize({ _cmdBuffer });
		_cmdBuffer->Send( GpuMsg::SetCommandBufferState{ ERecordingState::Recording });
		
		// check buffer state
		CHECK_ERR( _cmdBuffer->Request(GpuMsg::GetCommandBufferState{}) == ERecordingState::Recording );
		
		_scope = EScope::Command;
		return true;
	}
	
/*
=================================================
	_GpuCmdEnd
=================================================
*/
	bool GL4CommandBuilder::_CmdEnd (const GpuMsg::CmdEnd &msg)
	{
		CHECK_ERR( _cmdBuffer );
		CHECK_ERR( _scope == EScope::Command );
		
		_cmdBuffer->Send( GpuMsg::SetGLCommandBufferQueue{ RVREF(_commands), RVREF(_bufferData), RVREF(_pushConstData) } );
		_cmdBuffer->Send( GpuMsg::SetCommandBufferDependency{ RVREF(_resources) } );
		_cmdBuffer->Send( GpuMsg::SetCommandBufferState{ ERecordingState::Executable } );

		msg.result.Set( _cmdBuffer );

		_cmdBuffer	= null;
		_scope		= EScope::None;

		_resources.Clear();
		_commands.Clear();
		_bufferData.Clear();
		_pushConstData.Clear();

		return true;
	}
	
/*
=================================================
	_CmdSetViewport
=================================================
*/
	bool GL4CommandBuilder::_CmdSetViewport (const GpuMsg::CmdSetViewport &msg)
	{
		CHECK_ERR( _cmdBuffer );
		// TODO: check dynamic states
		
		_commands.PushBack({ msg, __FILE__, __LINE__ });
		return true;
	}
	
/*
=================================================
	_CmdSetScissor
=================================================
*/
	bool GL4CommandBuilder::_CmdSetScissor (const GpuMsg::CmdSetScissor &msg)
	{
		CHECK_ERR( _cmdBuffer );
		// TODO: check dynamic states
		
		_commands.PushBack({ msg, __FILE__, __LINE__ });
		return true;
	}
	
/*
=================================================
	_CmdSetDepthBounds
=================================================
*/
	bool GL4CommandBuilder::_CmdSetDepthBounds (const GpuMsg::CmdSetDepthBounds &msg)
	{
		CHECK_ERR( _cmdBuffer );
		// TODO: check dynamic states
		
		_commands.PushBack({ msg, __FILE__, __LINE__ });
		return true;
	}
	
/*
=================================================
	_CmdSetBlendColor
=================================================
*/
	bool GL4CommandBuilder::_CmdSetBlendColor (const GpuMsg::CmdSetBlendColor &msg)
	{
		CHECK_ERR( _cmdBuffer );
		// TODO: check dynamic states
		
		_commands.PushBack({ msg, __FILE__, __LINE__ });
		return true;
	}
	
/*
=================================================
	_CmdSetDepthBias
=================================================
*/
	bool GL4CommandBuilder::_CmdSetDepthBias (const GpuMsg::CmdSetDepthBias &msg)
	{
		CHECK_ERR( _cmdBuffer );
		// TODO: check dynamic states
		
		_commands.PushBack({ msg, __FILE__, __LINE__ });
		return true;
	}
	
/*
=================================================
	_CmdSetLineWidth
=================================================
*/
	bool GL4CommandBuilder::_CmdSetLineWidth (const GpuMsg::CmdSetLineWidth &msg)
	{
		CHECK_ERR( _cmdBuffer );
		// TODO: check dynamic states
		
		_commands.PushBack({ msg, __FILE__, __LINE__ });
		return true;
	}
	
/*
=================================================
	_CmdSetStencilCompareMask
=================================================
*/
	bool GL4CommandBuilder::_CmdSetStencilCompareMask (const GpuMsg::CmdSetStencilCompareMask &msg)
	{
		CHECK_ERR( _cmdBuffer );
		// TODO: check dynamic states
		
		_commands.PushBack({ msg, __FILE__, __LINE__ });
		return true;
	}
	
/*
=================================================
	_CmdSetStencilWriteMask
=================================================
*/
	bool GL4CommandBuilder::_CmdSetStencilWriteMask (const GpuMsg::CmdSetStencilWriteMask &msg)
	{
		CHECK_ERR( _cmdBuffer );
		// TODO: check dynamic states
		
		_commands.PushBack({ msg, __FILE__, __LINE__ });
		return true;
	}
	
/*
=================================================
	_CmdSetStencilReference
=================================================
*/
	bool GL4CommandBuilder::_CmdSetStencilReference (const GpuMsg::CmdSetStencilReference &msg)
	{
		CHECK_ERR( _cmdBuffer );
		// TODO: check dynamic states
		
		_commands.PushBack({ msg, __FILE__, __LINE__ });
		return true;
	}
	
/*
=================================================
	_CmdBeginRenderPass
=================================================
*/
	bool GL4CommandBuilder::_CmdBeginRenderPass (const GpuMsg::CmdBeginRenderPass &msg)
	{
		CHECK_ERR( _cmdBuffer );
		CHECK_ERR( _scope == EScope::Command );
		CHECK_ERR( msg.framebuffer );
		CHECK_ERR( msg.renderPass );
		
		_scope = EScope::RenderPass;

		_commands.PushBack({ msg, __FILE__, __LINE__ });
		return true;
	}
	
/*
=================================================
	_CmdEndRenderPass
=================================================
*/
	bool GL4CommandBuilder::_CmdEndRenderPass (const GpuMsg::CmdEndRenderPass &msg)
	{
		CHECK_ERR( _cmdBuffer );
		CHECK_ERR( _scope == EScope::RenderPass );
		
		_scope			= EScope::Command;
		_dynamicStates	= Uninitialized;

		_commands.PushBack({ msg, __FILE__, __LINE__ });
		return true;
	}
	
/*
=================================================
	_CmdNextSubpass
=================================================
*/
	bool GL4CommandBuilder::_CmdNextSubpass (const GpuMsg::CmdNextSubpass &)
	{
		RETURN_ERR( "not supported" );	// TODO: emulate?
	}
	
/*
=================================================
	_CmdBindGraphicsPipeline
=================================================
*/
	bool GL4CommandBuilder::_CmdBindGraphicsPipeline (const GpuMsg::CmdBindGraphicsPipeline &msg)
	{
		CHECK_ERR( _cmdBuffer );
		CHECK_ERR( _scope == EScope::RenderPass );
		CHECK_ERR( msg.pipeline );
		
		_dynamicStates = msg.pipeline->Request(GpuMsg::GetGraphicsPipelineDescription{}).dynamicStates;

		_commands.PushBack({ msg, __FILE__, __LINE__ });
		return true;
	}
	
/*
=================================================
	_CmdBindComputePipeline
=================================================
*/
	bool GL4CommandBuilder::_CmdBindComputePipeline (const GpuMsg::CmdBindComputePipeline &msg)
	{
		CHECK_ERR( _cmdBuffer );
		CHECK_ERR( _scope == EScope::Command );
		CHECK_ERR( msg.pipeline );
		
		_dynamicStates = Uninitialized;

		_commands.PushBack({ msg, __FILE__, __LINE__ });
		return true;
	}
	
/*
=================================================
	_CmdBindVertexBuffers
=================================================
*/
	bool GL4CommandBuilder::_CmdBindVertexBuffers (const GpuMsg::CmdBindVertexBuffers &msg)
	{
		CHECK_ERR( _cmdBuffer );
		CHECK_ERR( _scope == EScope::RenderPass );
		
		_commands.PushBack({ msg, __FILE__, __LINE__ });
		return true;
	}
	
/*
=================================================
	_CmdBindIndexBuffer
=================================================
*/
	bool GL4CommandBuilder::_CmdBindIndexBuffer (const GpuMsg::CmdBindIndexBuffer &msg)
	{
		CHECK_ERR( _cmdBuffer );
		CHECK_ERR( _scope == EScope::RenderPass );
		CHECK_ERR( msg.buffer );
		
		_commands.PushBack({ msg, __FILE__, __LINE__ });
		return true;
	}
	
/*
=================================================
	_CmdDraw
=================================================
*/
	bool GL4CommandBuilder::_CmdDraw (const GpuMsg::CmdDraw &msg)
	{
		CHECK_ERR( _cmdBuffer );
		CHECK_ERR( _scope == EScope::RenderPass );
		
		_commands.PushBack({ msg, __FILE__, __LINE__ });
		return true;
	}
	
/*
=================================================
	_CmdDrawIndexed
=================================================
*/
	bool GL4CommandBuilder::_CmdDrawIndexed (const GpuMsg::CmdDrawIndexed &msg)
	{
		CHECK_ERR( _cmdBuffer );
		CHECK_ERR( _scope == EScope::RenderPass );
		
		_commands.PushBack({ msg, __FILE__, __LINE__ });
		return true;
	}
	
/*
=================================================
	_CmdDrawIndirect
=================================================
*/
	bool GL4CommandBuilder::_CmdDrawIndirect (const GpuMsg::CmdDrawIndirect &msg)
	{
		CHECK_ERR( _cmdBuffer );
		CHECK_ERR( _scope == EScope::RenderPass );
		CHECK_ERR( msg.indirectBuffer );
		
		_commands.PushBack({ msg, __FILE__, __LINE__ });
		return true;
	}
	
/*
=================================================
	_CmdDrawIndexedIndirect
=================================================
*/
	bool GL4CommandBuilder::_CmdDrawIndexedIndirect (const GpuMsg::CmdDrawIndexedIndirect &msg)
	{
		CHECK_ERR( _cmdBuffer );
		CHECK_ERR( _scope == EScope::RenderPass );
		CHECK_ERR( msg.indirectBuffer );
		
		_commands.PushBack({ msg, __FILE__, __LINE__ });
		return true;
	}
	
/*
=================================================
	_CmdDispatch
=================================================
*/
	bool GL4CommandBuilder::_CmdDispatch (const GpuMsg::CmdDispatch &msg)
	{
		CHECK_ERR( _cmdBuffer );
		CHECK_ERR( _scope == EScope::Command );
		
		_commands.PushBack({ msg, __FILE__, __LINE__ });
		return true;
	}
	
/*
=================================================
	_CmdDispatchIndirect
=================================================
*/
	bool GL4CommandBuilder::_CmdDispatchIndirect (const GpuMsg::CmdDispatchIndirect &msg)
	{
		CHECK_ERR( _cmdBuffer );
		CHECK_ERR( _scope == EScope::RenderPass );
		CHECK_ERR( msg.indirectBuffer );
		
		_commands.PushBack({ msg, __FILE__, __LINE__ });
		return true;
	}
	
/*
=================================================
	_CmdExecute
=================================================
*/
	bool GL4CommandBuilder::_CmdExecute (const GpuMsg::CmdExecute &msg)
	{
		CHECK_ERR( _cmdBuffer );
		CHECK_ERR( _scope == EScope::Command or _scope == EScope::RenderPass );
		
		_commands.PushBack({ msg, __FILE__, __LINE__ });
		return true;
	}
	
/*
=================================================
	_CmdBindGraphicsResourceTable
=================================================
*/
	bool GL4CommandBuilder::_CmdBindGraphicsResourceTable (const GpuMsg::CmdBindGraphicsResourceTable &msg)
	{
		CHECK_ERR( _cmdBuffer );
		CHECK_ERR( _scope == EScope::RenderPass );
		CHECK_ERR( msg.resourceTable );
		
		_commands.PushBack({ msg, __FILE__, __LINE__ });
		return true;
	}
	
/*
=================================================
	_CmdBindComputeResourceTable
=================================================
*/
	bool GL4CommandBuilder::_CmdBindComputeResourceTable (const GpuMsg::CmdBindComputeResourceTable &msg)
	{
		CHECK_ERR( _cmdBuffer );
		CHECK_ERR( _scope == EScope::Command );
		CHECK_ERR( msg.resourceTable );
		
		_commands.PushBack({ msg, __FILE__, __LINE__ });
		return true;
	}
	
/*
=================================================
	_CmdCopyBuffer
=================================================
*/
	bool GL4CommandBuilder::_CmdCopyBuffer (const GpuMsg::CmdCopyBuffer &msg)
	{
		CHECK_ERR( _cmdBuffer );
		CHECK_ERR( _scope == EScope::Command );
		CHECK_ERR( msg.srcBuffer and msg.dstBuffer );
		
		_commands.PushBack({ msg, __FILE__, __LINE__ });
		return true;
	}
	
/*
=================================================
	_CmdCopyImage
=================================================
*/
	bool GL4CommandBuilder::_CmdCopyImage (const GpuMsg::CmdCopyImage &msg)
	{
		CHECK_ERR( _cmdBuffer );
		CHECK_ERR( _scope == EScope::Command );
		CHECK_ERR( msg.srcImage and msg.dstImage );
		
		_commands.PushBack({ msg, __FILE__, __LINE__ });
		return true;
	}
	
/*
=================================================
	_CmdCopyBufferToImage
=================================================
*/
	bool GL4CommandBuilder::_CmdCopyBufferToImage (const GpuMsg::CmdCopyBufferToImage &msg)
	{
		CHECK_ERR( _cmdBuffer );
		CHECK_ERR( _scope == EScope::Command );
		CHECK_ERR( msg.srcBuffer and msg.dstImage );
		
		_commands.PushBack({ msg, __FILE__, __LINE__ });
		return true;
	}
	
/*
=================================================
	_CmdCopyImageToBuffer
=================================================
*/
	bool GL4CommandBuilder::_CmdCopyImageToBuffer (const GpuMsg::CmdCopyImageToBuffer &msg)
	{
		CHECK_ERR( _cmdBuffer );
		CHECK_ERR( _scope == EScope::Command );
		CHECK_ERR( msg.srcImage and msg.dstBuffer );
		
		_commands.PushBack({ msg, __FILE__, __LINE__ });
		return true;
	}
	
/*
=================================================
	_CmdBlitImage
=================================================
*/
	bool GL4CommandBuilder::_CmdBlitImage (const GpuMsg::CmdBlitImage &msg)
	{
		CHECK_ERR( _cmdBuffer );
		CHECK_ERR( _scope == EScope::Command );
		CHECK_ERR( msg.srcImage and msg.dstImage );
		
		_commands.PushBack({ msg, __FILE__, __LINE__ });
		return true;
	}
	
/*
=================================================
	_CmdResolveImage
=================================================
*/
	bool GL4CommandBuilder::_CmdResolveImage (const GpuMsg::CmdResolveImage &msg)
	{
		CHECK_ERR( _cmdBuffer );
		CHECK_ERR( _scope == EScope::Command );
		CHECK_ERR( msg.srcImage and msg.dstImage );
		
		_commands.PushBack({ msg, __FILE__, __LINE__ });
		return true;
	}

/*
=================================================
	_CmdBlitGLFramebuffers
=================================================
*/
	bool GL4CommandBuilder::_CmdBlitGLFramebuffers (const GpuMsg::CmdBlitGLFramebuffers &msg)
	{
		CHECK_ERR( _cmdBuffer );
		CHECK_ERR( _scope == EScope::Command );
		CHECK_ERR( msg.srcFramebuffer and msg.dstFramebuffer );
		
		_commands.PushBack({ msg, __FILE__, __LINE__ });
		return true;
	}

/*
=================================================
	_CmdUpdateBuffer
=================================================
*/
	bool GL4CommandBuilder::_CmdUpdateBuffer (const GpuMsg::CmdUpdateBuffer &msg)
	{
		CHECK_ERR( _cmdBuffer );
		CHECK_ERR( _scope == EScope::Command );

		_commands.PushBack({ GpuMsg::GLCmdUpdateBuffer{ msg, _bufferData.Size() }, __FILE__, __LINE__ });
		
		_bufferData.Append( msg.data );
		return true;
	}
	
/*
=================================================
	_CmdFillBuffer
=================================================
*/
	bool GL4CommandBuilder::_CmdFillBuffer (const GpuMsg::CmdFillBuffer &msg)
	{
		CHECK_ERR( _cmdBuffer );
		CHECK_ERR( _scope == EScope::Command );
		CHECK_ERR( msg.dstBuffer );
		
		_commands.PushBack({ msg, __FILE__, __LINE__ });
		return true;
	}
	
/*
=================================================
	_CmdClearAttachments
=================================================
*/
	bool GL4CommandBuilder::_CmdClearAttachments (const GpuMsg::CmdClearAttachments &msg)
	{
		CHECK_ERR( _cmdBuffer );
		CHECK_ERR( _scope == EScope::RenderPass );
		
		_commands.PushBack({ msg, __FILE__, __LINE__ });
		return true;
	}
	
/*
=================================================
	_CmdClearColorImage
=================================================
*/
	bool GL4CommandBuilder::_CmdClearColorImage (const GpuMsg::CmdClearColorImage &msg)
	{
		CHECK_ERR( _cmdBuffer );
		CHECK_ERR( _scope == EScope::Command );
		CHECK_ERR( msg.image );
		
		_commands.PushBack({ msg, __FILE__, __LINE__ });
		return true;
	}
	
/*
=================================================
	_CmdClearDepthStencilImage
=================================================
*/
	bool GL4CommandBuilder::_CmdClearDepthStencilImage (const GpuMsg::CmdClearDepthStencilImage &msg)
	{
		CHECK_ERR( _cmdBuffer );
		CHECK_ERR( _scope == EScope::Command );
		CHECK_ERR( msg.image );
		
		_commands.PushBack({ msg, __FILE__, __LINE__ });
		return true;
	}
	
/*
=================================================
	_CmdPipelineBarrier
=================================================
*/
	bool GL4CommandBuilder::_CmdPipelineBarrier (const GpuMsg::CmdPipelineBarrier &msg)
	{
		CHECK_ERR( _cmdBuffer );
		CHECK_ERR( _scope == EScope::Command );
		
		_commands.PushBack({ msg, __FILE__, __LINE__ });
		return true;
	}
	
/*
=================================================
	_CmdPushConstants
=================================================
*/
	bool GL4CommandBuilder::_CmdPushConstants (const GpuMsg::CmdPushConstants &)
	{
		CHECK_ERR( _cmdBuffer );
		CHECK_ERR( _scope == EScope::Command or _scope == EScope::RenderPass );
		
		TODO("");
		return true;
	}
	
/*
=================================================
	_CmdPushNamedConstants
=================================================
*/
	bool GL4CommandBuilder::_CmdPushNamedConstants (const GpuMsg::CmdPushNamedConstants &)
	{
		CHECK_ERR( _cmdBuffer );
		CHECK_ERR( _scope == EScope::Command or _scope == EScope::RenderPass );
		
		TODO("");
		return true;
	}
	
/*
=================================================
	_CmdPushNamedConstants
=================================================
*/
	bool GL4CommandBuilder::_CmdDebugMarker (const GpuMsg::CmdDebugMarker &msg)
	{
		CHECK_ERR( _cmdBuffer );
		CHECK_ERR( _scope != EScope::None );
		
		_commands.PushBack({ msg, __FILE__, __LINE__ });
		return true;
	}
	
/*
=================================================
	_CmdPushNamedConstants
=================================================
*/
	bool GL4CommandBuilder::_CmdPushDebugGroup (const GpuMsg::CmdPushDebugGroup &msg)
	{
		CHECK_ERR( _cmdBuffer );
		CHECK_ERR( _scope != EScope::None );
		
		_commands.PushBack({ msg, __FILE__, __LINE__ });
		return true;
	}
	
/*
=================================================
	_CmdPushNamedConstants
=================================================
*/
	bool GL4CommandBuilder::_CmdPopDebugGroup (const GpuMsg::CmdPopDebugGroup &msg)
	{
		CHECK_ERR( _cmdBuffer );
		CHECK_ERR( _scope != EScope::None );
		
		_commands.PushBack({ msg, __FILE__, __LINE__ });
		return true;
	}
	
/*
=================================================
	_CmdBeginQuery
=================================================
*/
	bool GL4CommandBuilder::_CmdBeginQuery (const GpuMsg::CmdBeginQuery &msg)
	{
		CHECK_ERR( _cmdBuffer );
		CHECK_ERR( _scope == EScope::Command or _scope == EScope::RenderPass );
		CHECK_ERR( msg.queryPool );
		
		_commands.PushBack({ msg, __FILE__, __LINE__ });
		return true;
	}
	
/*
=================================================
	_CmdEndQuery
=================================================
*/
	bool GL4CommandBuilder::_CmdEndQuery (const GpuMsg::CmdEndQuery &msg)
	{
		CHECK_ERR( _cmdBuffer );
		CHECK_ERR( _scope == EScope::Command or _scope == EScope::RenderPass );
		CHECK_ERR( msg.queryPool );
		
		_commands.PushBack({ msg, __FILE__, __LINE__ });
		return true;
	}
	
/*
=================================================
	_CmdCopyQueryPoolResults
=================================================
*/
	bool GL4CommandBuilder::_CmdCopyQueryPoolResults (const GpuMsg::CmdCopyQueryPoolResults &msg)
	{
		CHECK_ERR( _cmdBuffer );
		CHECK_ERR( _scope == EScope::Command );
		CHECK_ERR( msg.queryPool and msg.dstBuffer );
		
		_commands.PushBack({ msg, __FILE__, __LINE__ });
		return true;
	}
	
/*
=================================================
	_CmdWriteTimestamp
=================================================
*/
	bool GL4CommandBuilder::_CmdWriteTimestamp (const GpuMsg::CmdWriteTimestamp &msg)
	{
		CHECK_ERR( _cmdBuffer );
		CHECK_ERR( _scope == EScope::Command );
		CHECK_ERR( msg.queryPool );
		
		_commands.PushBack({ msg, __FILE__, __LINE__ });
		return true;
	}
	
/*
=================================================
	_CmdResetQueryPool
=================================================
*/
	bool GL4CommandBuilder::_CmdResetQueryPool (const GpuMsg::CmdResetQueryPool &msg)
	{
		CHECK_ERR( _cmdBuffer );
		CHECK_ERR( _scope == EScope::Command );
		CHECK_ERR( msg.queryPool );
		
		_commands.PushBack({ msg, __FILE__, __LINE__ });
		return true;
	}

}	// PlatformGL
//-----------------------------------------------------------------------------
	
namespace Platforms
{
	ModulePtr OpenGLObjectsConstructor::CreateGL4CommandBuilder (ModuleMsg::UntypedID_t id, GlobalSystemsRef gs, const CreateInfo::GpuCommandBuilder &ci)
	{
		return New< PlatformGL::GL4CommandBuilder >( id, gs, ci );
	}
}	// Platforms
}	// Engine

#endif	// GRAPHICS_API_OPENGL
