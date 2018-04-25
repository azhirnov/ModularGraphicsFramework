// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Engine/Config/Engine.Config.h"

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
											GpuMsg::CmdSetViewport,
											GpuMsg::CmdSetScissor,
											GpuMsg::CmdSetDepthBounds,
											GpuMsg::CmdSetBlendColor,
											GpuMsg::CmdSetDepthBias,
											GpuMsg::CmdSetLineWidth,
											GpuMsg::CmdSetStencilCompareMask,
											GpuMsg::CmdSetStencilWriteMask,
											GpuMsg::CmdSetStencilReference,
											GpuMsg::CmdBegin,
											GpuMsg::CmdEnd,
											GpuMsg::CmdBeginRenderPass,
											GpuMsg::CmdEndRenderPass,
											GpuMsg::CmdNextSubpass,
											GpuMsg::CmdBindGraphicsPipeline,
											GpuMsg::CmdBindComputePipeline,
											GpuMsg::CmdBindVertexBuffers,
											GpuMsg::CmdBindIndexBuffer,
											GpuMsg::CmdDraw,
											GpuMsg::CmdDrawIndexed,
											GpuMsg::CmdDrawIndirect,
											GpuMsg::CmdDrawIndexedIndirect,
											GpuMsg::CmdDispatch,
											GpuMsg::CmdDispatchIndirect,
											GpuMsg::CmdExecute,
											GpuMsg::CmdBindGraphicsResourceTable,
											GpuMsg::CmdBindComputeResourceTable,
											GpuMsg::CmdCopyBuffer,
											GpuMsg::CmdCopyImage,
											GpuMsg::CmdCopyBufferToImage,
											GpuMsg::CmdCopyImageToBuffer,
											GpuMsg::CmdBlitImage,
											GpuMsg::CmdBlitGLFramebuffers,
											GpuMsg::CmdUpdateBuffer,
											GpuMsg::CmdFillBuffer,
											GpuMsg::CmdClearAttachments,
											GpuMsg::CmdClearColorImage,
											GpuMsg::CmdClearDepthStencilImage,
											GpuMsg::CmdPipelineBarrier,
											GpuMsg::CmdPushConstants,
											GpuMsg::CmdPushNamedConstants,
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
		static const TypeIdList		_msgTypes;
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
		bool _Compose (const Message< ModuleMsg::Compose > &);
		bool _Delete (const Message< ModuleMsg::Delete > &);
		bool _SetCommandBufferDependency (const Message< GpuMsg::SetCommandBufferDependency > &);
		bool _GetCommandBufferState (const Message< GpuMsg::GetCommandBufferState > &);
		
		bool _CmdBegin (const Message< GpuMsg::CmdBegin > &);
		bool _CmdEnd (const Message< GpuMsg::CmdEnd > &);

		bool _CmdSetViewport (const Message< GpuMsg::CmdSetViewport > &);
		bool _CmdSetScissor (const Message< GpuMsg::CmdSetScissor > &);
		bool _CmdSetDepthBounds (const Message< GpuMsg::CmdSetDepthBounds > &);
		bool _CmdSetBlendColor (const Message< GpuMsg::CmdSetBlendColor > &);
		bool _CmdSetDepthBias (const Message< GpuMsg::CmdSetDepthBias > &);
		bool _CmdSetLineWidth (const Message< GpuMsg::CmdSetLineWidth > &);
		bool _CmdSetStencilCompareMask (const Message< GpuMsg::CmdSetStencilCompareMask > &);
		bool _CmdSetStencilWriteMask (const Message< GpuMsg::CmdSetStencilWriteMask > &);
		bool _CmdSetStencilReference (const Message< GpuMsg::CmdSetStencilReference > &);
		bool _CmdBeginRenderPass (const Message< GpuMsg::CmdBeginRenderPass > &);
		bool _CmdEndRenderPass (const Message< GpuMsg::CmdEndRenderPass > &);
		bool _CmdNextSubpass (const Message< GpuMsg::CmdNextSubpass > &);
		bool _CmdBindGraphicsPipeline (const Message< GpuMsg::CmdBindGraphicsPipeline > &);
		bool _CmdBindComputePipeline (const Message< GpuMsg::CmdBindComputePipeline > &);
		bool _CmdBindVertexBuffers (const Message< GpuMsg::CmdBindVertexBuffers > &);
		bool _CmdBindIndexBuffer (const Message< GpuMsg::CmdBindIndexBuffer > &);
		bool _CmdDraw (const Message< GpuMsg::CmdDraw > &);
		bool _CmdDrawIndexed (const Message< GpuMsg::CmdDrawIndexed > &);
		bool _CmdDrawIndirect (const Message< GpuMsg::CmdDrawIndirect > &);
		bool _CmdDrawIndexedIndirect (const Message< GpuMsg::CmdDrawIndexedIndirect > &);
		bool _CmdDispatch (const Message< GpuMsg::CmdDispatch > &);
		bool _CmdDispatchIndirect (const Message< GpuMsg::CmdDispatchIndirect > &);
		bool _CmdExecute (const Message< GpuMsg::CmdExecute > &);
		bool _CmdBindGraphicsResourceTable (const Message< GpuMsg::CmdBindGraphicsResourceTable > &);
		bool _CmdBindComputeResourceTable (const Message< GpuMsg::CmdBindComputeResourceTable > &);
		bool _CmdCopyBuffer (const Message< GpuMsg::CmdCopyBuffer > &);
		bool _CmdCopyImage (const Message< GpuMsg::CmdCopyImage > &);
		bool _CmdCopyBufferToImage (const Message< GpuMsg::CmdCopyBufferToImage > &);
		bool _CmdCopyImageToBuffer (const Message< GpuMsg::CmdCopyImageToBuffer > &);
		bool _CmdBlitImage (const Message< GpuMsg::CmdBlitImage > &);
		bool _CmdBlitGLFramebuffers (const Message< GpuMsg::CmdBlitGLFramebuffers > &);
		bool _CmdUpdateBuffer (const Message< GpuMsg::CmdUpdateBuffer > &);
		bool _CmdFillBuffer (const Message< GpuMsg::CmdFillBuffer > &);
		bool _CmdClearAttachments (const Message< GpuMsg::CmdClearAttachments > &);
		bool _CmdClearColorImage (const Message< GpuMsg::CmdClearColorImage > &);
		bool _CmdClearDepthStencilImage (const Message< GpuMsg::CmdClearDepthStencilImage > &);
		bool _CmdPipelineBarrier (const Message< GpuMsg::CmdPipelineBarrier > &);
		bool _CmdPushConstants (const Message< GpuMsg::CmdPushConstants > &);
		bool _CmdPushNamedConstants (const Message< GpuMsg::CmdPushNamedConstants > &);
	};
//-----------------------------------------------------------------------------


	
	const TypeIdList	GL4CommandBuilder::_msgTypes{ UninitializedT< SupportedMessages_t >() };
	const TypeIdList	GL4CommandBuilder::_eventTypes{ UninitializedT< SupportedEvents_t >() };

/*
=================================================
	constructor
=================================================
*/
	GL4CommandBuilder::GL4CommandBuilder (UntypedID_t id, GlobalSystemsRef gs, const CreateInfo::GpuCommandBuilder &ci) :
		GL4BaseModule( gs, ModuleConfig{ id, UMax }, &_msgTypes, &_eventTypes ),
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
		_SubscribeOnMsg( this, &GL4CommandBuilder::_CmdBlitGLFramebuffers );
		_SubscribeOnMsg( this, &GL4CommandBuilder::_CmdUpdateBuffer );
		_SubscribeOnMsg( this, &GL4CommandBuilder::_CmdFillBuffer );
		_SubscribeOnMsg( this, &GL4CommandBuilder::_CmdClearAttachments );
		_SubscribeOnMsg( this, &GL4CommandBuilder::_CmdClearColorImage );
		_SubscribeOnMsg( this, &GL4CommandBuilder::_CmdClearDepthStencilImage );
		_SubscribeOnMsg( this, &GL4CommandBuilder::_CmdPipelineBarrier );
		_SubscribeOnMsg( this, &GL4CommandBuilder::_CmdPushConstants );
		_SubscribeOnMsg( this, &GL4CommandBuilder::_CmdPushNamedConstants );
		
		CHECK( _ValidateMsgSubscriptions() );

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
	bool GL4CommandBuilder::_Compose (const Message< ModuleMsg::Compose > &msg)
	{
		if ( _IsComposedState( GetState() ) )
			return true;	// already composed

		CHECK_ERR( GetState() == EState::Linked );


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
*/
	bool GL4CommandBuilder::_Delete (const Message< ModuleMsg::Delete > &msg)
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
	bool GL4CommandBuilder::_SetCommandBufferDependency (const Message< GpuMsg::SetCommandBufferDependency > &msg)
	{
		_resources.AddArray( msg->resources.Get() );
		return true;
	}
	
/*
=================================================
	_GetCommandBufferState
=================================================
*/
	bool GL4CommandBuilder::_GetCommandBufferState (const Message< GpuMsg::GetCommandBufferState > &msg)
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
	bool GL4CommandBuilder::_CmdBegin (const Message< GpuMsg::CmdBegin > &msg)
	{
		CHECK_ERR( _IsComposedState( GetState() ) );
		CHECK_ERR( not _cmdBuffer );
		CHECK_ERR( _scope == EScope::None );
		
		_resources.Clear();		_resources.Reserve( 128 );
		_commands.Clear();		_commands.Reserve( 128 );
		
		// use target command buffer
		if ( msg->targetCmdBuffer )
		{
			CHECK_ERR( msg->targetCmdBuffer->GetSupportedMessages().HasAllTypes< CmdBufferMsg_t >() );

			_cmdBuffer = msg->targetCmdBuffer;
		}
		else
		// create new command buffer
		{
			CHECK_ERR( GlobalSystems()->modulesFactory->Create(
							GLCommandBufferModuleID,
							GlobalSystems(),
							CreateInfo::GpuCommandBuffer{
								_GetManager(),
								CommandBufferDescriptor{ msg->flags }
							},
							OUT _cmdBuffer )
			);
			CHECK_ERR( _Attach( "", _cmdBuffer, false ) );
		}

		ModuleUtils::Initialize( {_cmdBuffer}, this );
		SendTo( _cmdBuffer, Message< GpuMsg::SetCommandBufferState >{ ERecordingState::Recording } );
		
		// check buffer state
		Message< GpuMsg::GetCommandBufferState >	req_state;
		SendTo( _cmdBuffer, req_state );
		CHECK_ERR( req_state->result.Get() == ERecordingState::Recording );
		
		_scope = EScope::Command;
		return true;
	}
	
/*
=================================================
	_GpuCmdEnd
=================================================
*/
	bool GL4CommandBuilder::_CmdEnd (const Message< GpuMsg::CmdEnd > &msg)
	{
		CHECK_ERR( _cmdBuffer );
		CHECK_ERR( _scope == EScope::Command );
		
		SendTo( _cmdBuffer, Message< GpuMsg::SetGLCommandBufferQueue >{ RVREF(_commands), RVREF(_bufferData), RVREF(_pushConstData) } );
		SendTo( _cmdBuffer, Message< GpuMsg::SetCommandBufferDependency >{ RVREF(_resources) } );
		SendTo( _cmdBuffer, Message< GpuMsg::SetCommandBufferState >{ ERecordingState::Executable } );

		msg->result.Set( _cmdBuffer );

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
	bool GL4CommandBuilder::_CmdSetViewport (const Message< GpuMsg::CmdSetViewport > &msg)
	{
		CHECK_ERR( _cmdBuffer );
		// TODO: check dynamic states
		
		_commands.PushBack({ msg.Data(), __FILE__, __LINE__ });
		return true;
	}
	
/*
=================================================
	_CmdSetScissor
=================================================
*/
	bool GL4CommandBuilder::_CmdSetScissor (const Message< GpuMsg::CmdSetScissor > &msg)
	{
		CHECK_ERR( _cmdBuffer );
		// TODO: check dynamic states
		
		_commands.PushBack({ msg.Data(), __FILE__, __LINE__ });
		return true;
	}
	
/*
=================================================
	_CmdSetDepthBounds
=================================================
*/
	bool GL4CommandBuilder::_CmdSetDepthBounds (const Message< GpuMsg::CmdSetDepthBounds > &msg)
	{
		CHECK_ERR( _cmdBuffer );
		// TODO: check dynamic states
		
		_commands.PushBack({ msg.Data(), __FILE__, __LINE__ });
		return true;
	}
	
/*
=================================================
	_CmdSetBlendColor
=================================================
*/
	bool GL4CommandBuilder::_CmdSetBlendColor (const Message< GpuMsg::CmdSetBlendColor > &msg)
	{
		CHECK_ERR( _cmdBuffer );
		// TODO: check dynamic states
		
		_commands.PushBack({ msg.Data(), __FILE__, __LINE__ });
		return true;
	}
	
/*
=================================================
	_CmdSetDepthBias
=================================================
*/
	bool GL4CommandBuilder::_CmdSetDepthBias (const Message< GpuMsg::CmdSetDepthBias > &msg)
	{
		CHECK_ERR( _cmdBuffer );
		// TODO: check dynamic states
		
		_commands.PushBack({ msg.Data(), __FILE__, __LINE__ });
		return true;
	}
	
/*
=================================================
	_CmdSetLineWidth
=================================================
*/
	bool GL4CommandBuilder::_CmdSetLineWidth (const Message< GpuMsg::CmdSetLineWidth > &msg)
	{
		CHECK_ERR( _cmdBuffer );
		// TODO: check dynamic states
		
		_commands.PushBack({ msg.Data(), __FILE__, __LINE__ });
		return true;
	}
	
/*
=================================================
	_CmdSetStencilCompareMask
=================================================
*/
	bool GL4CommandBuilder::_CmdSetStencilCompareMask (const Message< GpuMsg::CmdSetStencilCompareMask > &msg)
	{
		CHECK_ERR( _cmdBuffer );
		// TODO: check dynamic states
		
		_commands.PushBack({ msg.Data(), __FILE__, __LINE__ });
		return true;
	}
	
/*
=================================================
	_CmdSetStencilWriteMask
=================================================
*/
	bool GL4CommandBuilder::_CmdSetStencilWriteMask (const Message< GpuMsg::CmdSetStencilWriteMask > &msg)
	{
		CHECK_ERR( _cmdBuffer );
		// TODO: check dynamic states
		
		_commands.PushBack({ msg.Data(), __FILE__, __LINE__ });
		return true;
	}
	
/*
=================================================
	_CmdSetStencilReference
=================================================
*/
	bool GL4CommandBuilder::_CmdSetStencilReference (const Message< GpuMsg::CmdSetStencilReference > &msg)
	{
		CHECK_ERR( _cmdBuffer );
		// TODO: check dynamic states
		
		_commands.PushBack({ msg.Data(), __FILE__, __LINE__ });
		return true;
	}
	
/*
=================================================
	_CmdBeginRenderPass
=================================================
*/
	bool GL4CommandBuilder::_CmdBeginRenderPass (const Message< GpuMsg::CmdBeginRenderPass > &msg)
	{
		CHECK_ERR( _cmdBuffer );
		CHECK_ERR( _scope == EScope::Command );
		CHECK_ERR( msg->framebuffer );
		CHECK_ERR( msg->renderPass );
		
		_scope = EScope::RenderPass;

		_commands.PushBack({ msg.Data(), __FILE__, __LINE__ });
		return true;
	}
	
/*
=================================================
	_CmdEndRenderPass
=================================================
*/
	bool GL4CommandBuilder::_CmdEndRenderPass (const Message< GpuMsg::CmdEndRenderPass > &msg)
	{
		CHECK_ERR( _cmdBuffer );
		CHECK_ERR( _scope == EScope::RenderPass );
		
		_scope			= EScope::Command;
		_dynamicStates	= Uninitialized;

		_commands.PushBack({ msg.Data(), __FILE__, __LINE__ });
		return true;
	}
	
/*
=================================================
	_CmdNextSubpass
=================================================
*/
	bool GL4CommandBuilder::_CmdNextSubpass (const Message< GpuMsg::CmdNextSubpass > &)
	{
		RETURN_ERR( "not supported" );	// TODO: emulate?
	}
	
/*
=================================================
	_CmdBindGraphicsPipeline
=================================================
*/
	bool GL4CommandBuilder::_CmdBindGraphicsPipeline (const Message< GpuMsg::CmdBindGraphicsPipeline > &msg)
	{
		CHECK_ERR( _cmdBuffer );
		CHECK_ERR( _scope == EScope::RenderPass );
		CHECK_ERR( msg->pipeline );
		
		Message< GpuMsg::GetGraphicsPipelineDescriptor >	req_descr;
		SendTo( msg->pipeline, req_descr );
		
		_dynamicStates = req_descr->result->dynamicStates;

		_commands.PushBack({ msg.Data(), __FILE__, __LINE__ });
		return true;
	}
	
/*
=================================================
	_CmdBindComputePipeline
=================================================
*/
	bool GL4CommandBuilder::_CmdBindComputePipeline (const Message< GpuMsg::CmdBindComputePipeline > &msg)
	{
		CHECK_ERR( _cmdBuffer );
		CHECK_ERR( _scope == EScope::Command );
		CHECK_ERR( msg->pipeline );
		
		_dynamicStates = Uninitialized;

		_commands.PushBack({ msg.Data(), __FILE__, __LINE__ });
		return true;
	}
	
/*
=================================================
	_CmdBindVertexBuffers
=================================================
*/
	bool GL4CommandBuilder::_CmdBindVertexBuffers (const Message< GpuMsg::CmdBindVertexBuffers > &msg)
	{
		CHECK_ERR( _cmdBuffer );
		CHECK_ERR( _scope == EScope::RenderPass );
		
		_commands.PushBack({ msg.Data(), __FILE__, __LINE__ });
		return true;
	}
	
/*
=================================================
	_CmdBindIndexBuffer
=================================================
*/
	bool GL4CommandBuilder::_CmdBindIndexBuffer (const Message< GpuMsg::CmdBindIndexBuffer > &msg)
	{
		CHECK_ERR( _cmdBuffer );
		CHECK_ERR( _scope == EScope::RenderPass );
		CHECK_ERR( msg->indexBuffer );
		
		_commands.PushBack({ msg.Data(), __FILE__, __LINE__ });
		return true;
	}
	
/*
=================================================
	_CmdDraw
=================================================
*/
	bool GL4CommandBuilder::_CmdDraw (const Message< GpuMsg::CmdDraw > &msg)
	{
		CHECK_ERR( _cmdBuffer );
		CHECK_ERR( _scope == EScope::RenderPass );
		
		_commands.PushBack({ msg.Data(), __FILE__, __LINE__ });
		return true;
	}
	
/*
=================================================
	_CmdDrawIndexed
=================================================
*/
	bool GL4CommandBuilder::_CmdDrawIndexed (const Message< GpuMsg::CmdDrawIndexed > &msg)
	{
		CHECK_ERR( _cmdBuffer );
		CHECK_ERR( _scope == EScope::RenderPass );
		
		_commands.PushBack({ msg.Data(), __FILE__, __LINE__ });
		return true;
	}
	
/*
=================================================
	_CmdDrawIndirect
=================================================
*/
	bool GL4CommandBuilder::_CmdDrawIndirect (const Message< GpuMsg::CmdDrawIndirect > &msg)
	{
		CHECK_ERR( _cmdBuffer );
		CHECK_ERR( _scope == EScope::RenderPass );
		CHECK_ERR( msg->indirectBuffer );
		
		_commands.PushBack({ msg.Data(), __FILE__, __LINE__ });
		return true;
	}
	
/*
=================================================
	_CmdDrawIndexedIndirect
=================================================
*/
	bool GL4CommandBuilder::_CmdDrawIndexedIndirect (const Message< GpuMsg::CmdDrawIndexedIndirect > &msg)
	{
		CHECK_ERR( _cmdBuffer );
		CHECK_ERR( _scope == EScope::RenderPass );
		CHECK_ERR( msg->indirectBuffer );
		
		_commands.PushBack({ msg.Data(), __FILE__, __LINE__ });
		return true;
	}
	
/*
=================================================
	_CmdDispatch
=================================================
*/
	bool GL4CommandBuilder::_CmdDispatch (const Message< GpuMsg::CmdDispatch > &msg)
	{
		CHECK_ERR( _cmdBuffer );
		CHECK_ERR( _scope == EScope::Command );
		
		_commands.PushBack({ msg.Data(), __FILE__, __LINE__ });
		return true;
	}
	
/*
=================================================
	_CmdDispatchIndirect
=================================================
*/
	bool GL4CommandBuilder::_CmdDispatchIndirect (const Message< GpuMsg::CmdDispatchIndirect > &msg)
	{
		CHECK_ERR( _cmdBuffer );
		CHECK_ERR( _scope == EScope::RenderPass );
		CHECK_ERR( msg->indirectBuffer );
		
		_commands.PushBack({ msg.Data(), __FILE__, __LINE__ });
		return true;
	}
	
/*
=================================================
	_CmdExecute
=================================================
*/
	bool GL4CommandBuilder::_CmdExecute (const Message< GpuMsg::CmdExecute > &msg)
	{
		CHECK_ERR( _cmdBuffer );
		CHECK_ERR( _scope == EScope::Command );
		
		_commands.PushBack({ msg.Data(), __FILE__, __LINE__ });
		return true;
	}
	
/*
=================================================
	_CmdBindGraphicsResourceTable
=================================================
*/
	bool GL4CommandBuilder::_CmdBindGraphicsResourceTable (const Message< GpuMsg::CmdBindGraphicsResourceTable > &msg)
	{
		CHECK_ERR( _cmdBuffer );
		CHECK_ERR( _scope == EScope::RenderPass );
		CHECK_ERR( msg->resourceTable );
		
		_commands.PushBack({ msg.Data(), __FILE__, __LINE__ });
		return true;
	}
	
/*
=================================================
	_CmdBindComputeResourceTable
=================================================
*/
	bool GL4CommandBuilder::_CmdBindComputeResourceTable (const Message< GpuMsg::CmdBindComputeResourceTable > &msg)
	{
		CHECK_ERR( _cmdBuffer );
		CHECK_ERR( _scope == EScope::Command );
		CHECK_ERR( msg->resourceTable );
		
		_commands.PushBack({ msg.Data(), __FILE__, __LINE__ });
		return true;
	}
	
/*
=================================================
	_CmdCopyBuffer
=================================================
*/
	bool GL4CommandBuilder::_CmdCopyBuffer (const Message< GpuMsg::CmdCopyBuffer > &msg)
	{
		CHECK_ERR( _cmdBuffer );
		CHECK_ERR( _scope == EScope::Command );
		CHECK_ERR( msg->srcBuffer and msg->dstBuffer );
		
		_commands.PushBack({ msg.Data(), __FILE__, __LINE__ });
		return true;
	}
	
/*
=================================================
	_CmdCopyImage
=================================================
*/
	bool GL4CommandBuilder::_CmdCopyImage (const Message< GpuMsg::CmdCopyImage > &msg)
	{
		CHECK_ERR( _cmdBuffer );
		CHECK_ERR( _scope == EScope::Command );
		CHECK_ERR( msg->srcImage and msg->dstImage );
		
		_commands.PushBack({ msg.Data(), __FILE__, __LINE__ });
		return true;
	}
	
/*
=================================================
	_CmdCopyBufferToImage
=================================================
*/
	bool GL4CommandBuilder::_CmdCopyBufferToImage (const Message< GpuMsg::CmdCopyBufferToImage > &msg)
	{
		CHECK_ERR( _cmdBuffer );
		CHECK_ERR( _scope == EScope::Command );
		CHECK_ERR( msg->srcBuffer and msg->dstImage );
		
		_commands.PushBack({ msg.Data(), __FILE__, __LINE__ });
		return true;
	}
	
/*
=================================================
	_CmdCopyImageToBuffer
=================================================
*/
	bool GL4CommandBuilder::_CmdCopyImageToBuffer (const Message< GpuMsg::CmdCopyImageToBuffer > &msg)
	{
		CHECK_ERR( _cmdBuffer );
		CHECK_ERR( _scope == EScope::Command );
		CHECK_ERR( msg->srcImage and msg->dstBuffer );
		
		_commands.PushBack({ msg.Data(), __FILE__, __LINE__ });
		return true;
	}
	
/*
=================================================
	_CmdBlitImage
=================================================
*/
	bool GL4CommandBuilder::_CmdBlitImage (const Message< GpuMsg::CmdBlitImage > &msg)
	{
		CHECK_ERR( _cmdBuffer );
		CHECK_ERR( _scope == EScope::Command );
		CHECK_ERR( msg->srcImage and msg->dstImage );
		
		_commands.PushBack({ msg.Data(), __FILE__, __LINE__ });
		return true;
	}
	
/*
=================================================
	_CmdBlitGLFramebuffers
=================================================
*/
	bool GL4CommandBuilder::_CmdBlitGLFramebuffers (const Message< GpuMsg::CmdBlitGLFramebuffers > &msg)
	{
		CHECK_ERR( _cmdBuffer );
		CHECK_ERR( _scope == EScope::Command );
		CHECK_ERR( msg->srcFramebuffer and msg->dstFramebuffer );
		
		_commands.PushBack({ msg.Data(), __FILE__, __LINE__ });
		return true;
	}

/*
=================================================
	_CmdUpdateBuffer
=================================================
*/
	bool GL4CommandBuilder::_CmdUpdateBuffer (const Message< GpuMsg::CmdUpdateBuffer > &msg)
	{
		CHECK_ERR( _cmdBuffer );
		CHECK_ERR( _scope == EScope::Command );

		_commands.PushBack({ GpuMsg::GLCmdUpdateBuffer{ msg.Data(), _bufferData.Size() }, __FILE__, __LINE__ });
		
		_bufferData.Append( msg->data );
		return true;
	}
	
/*
=================================================
	_CmdFillBuffer
=================================================
*/
	bool GL4CommandBuilder::_CmdFillBuffer (const Message< GpuMsg::CmdFillBuffer > &msg)
	{
		CHECK_ERR( _cmdBuffer );
		CHECK_ERR( _scope == EScope::Command );
		CHECK_ERR( msg->dstBuffer );
		
		_commands.PushBack({ msg.Data(), __FILE__, __LINE__ });
		return true;
	}
	
/*
=================================================
	_CmdClearAttachments
=================================================
*/
	bool GL4CommandBuilder::_CmdClearAttachments (const Message< GpuMsg::CmdClearAttachments > &msg)
	{
		CHECK_ERR( _cmdBuffer );
		CHECK_ERR( _scope == EScope::RenderPass );
		
		_commands.PushBack({ msg.Data(), __FILE__, __LINE__ });
		return true;
	}
	
/*
=================================================
	_CmdClearColorImage
=================================================
*/
	bool GL4CommandBuilder::_CmdClearColorImage (const Message< GpuMsg::CmdClearColorImage > &msg)
	{
		CHECK_ERR( _cmdBuffer );
		CHECK_ERR( _scope == EScope::Command );
		CHECK_ERR( msg->image );
		
		_commands.PushBack({ msg.Data(), __FILE__, __LINE__ });
		return true;
	}
	
/*
=================================================
	_CmdClearDepthStencilImage
=================================================
*/
	bool GL4CommandBuilder::_CmdClearDepthStencilImage (const Message< GpuMsg::CmdClearDepthStencilImage > &msg)
	{
		CHECK_ERR( _cmdBuffer );
		CHECK_ERR( _scope == EScope::Command );
		CHECK_ERR( msg->image );
		
		_commands.PushBack({ msg.Data(), __FILE__, __LINE__ });
		return true;
	}
	
/*
=================================================
	_CmdPipelineBarrier
=================================================
*/
	bool GL4CommandBuilder::_CmdPipelineBarrier (const Message< GpuMsg::CmdPipelineBarrier > &msg)
	{
		CHECK_ERR( _cmdBuffer );
		CHECK_ERR( _scope == EScope::Command );
		
		_commands.PushBack({ msg.Data(), __FILE__, __LINE__ });
		return true;
	}
	
/*
=================================================
	_CmdPushConstants
=================================================
*/
	bool GL4CommandBuilder::_CmdPushConstants (const Message< GpuMsg::CmdPushConstants > &msg)
	{
		CHECK_ERR( _cmdBuffer );
		CHECK_ERR( _scope == EScope::Command );
		
		TODO("");
		return true;
	}
	
/*
=================================================
	_CmdPushNamedConstants
=================================================
*/
	bool GL4CommandBuilder::_CmdPushNamedConstants (const Message< GpuMsg::CmdPushNamedConstants > &msg)
	{
		CHECK_ERR( _cmdBuffer );
		CHECK_ERR( _scope == EScope::Command );
		
		TODO("");
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
