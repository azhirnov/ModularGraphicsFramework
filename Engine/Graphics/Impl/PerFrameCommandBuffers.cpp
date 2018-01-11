// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Engine/Graphics/Impl/GraphicsBaseModule.h"
#include "Engine/Graphics/Shared/Commands.h"
#include "Engine/Graphics/Impl/GraphicsObjectsConstructor.h"

namespace Engine
{
namespace Graphics
{

	//
	// Per Frame Command Buffers
	//

	class PerFrameCommandBuffers final : public GraphicsBaseModule
	{
	// types
	private:
		using CmdBufferMsgList_t	= MessageListFrom<
											GpuMsg::CmdBegin,
											GpuMsg::CmdEnd,
											GpuMsg::CmdSetViewport,
											GpuMsg::CmdSetScissor,
											GpuMsg::CmdSetDepthBounds,
											GpuMsg::CmdSetBlendColor,
											GpuMsg::CmdSetDepthBias,
											GpuMsg::CmdSetLineWidth,
											GpuMsg::CmdSetStencilCompareMask,
											GpuMsg::CmdSetStencilWriteMask,
											GpuMsg::CmdSetStencilReference,
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
											GpuMsg::CmdUpdateBuffer,
											GpuMsg::CmdFillBuffer,
											GpuMsg::CmdClearAttachments,
											GpuMsg::CmdClearColorImage,
											GpuMsg::CmdClearDepthStencilImage,
											GpuMsg::CmdPipelineBarrier,
											GpuMsg::SetCommandBufferDependency,
											GpuMsg::GetCommandBufferState
										>;
		
		using SupportedMessages_t	= GraphicsBaseModule::SupportedMessages_t::Append< MessageListFrom<
											GraphicsMsg::CmdBegin,
											GraphicsMsg::CmdEnd,
											GraphicsMsg::CmdBeginFrame,
											GraphicsMsg::CmdEndFrame,
											GraphicsMsg::CmdGetCurrentFramebuffer,
											GraphicsMsg::CmdAddFrameDependency
										> >
										::Append< CmdBufferMsgList_t >;

		using SupportedEvents_t		= GraphicsBaseModule::SupportedEvents_t::Append< MessageListFrom<
											GraphicsMsg::OnCmdBeginFrame,
											GraphicsMsg::OnCmdEndFrame
										> >;

		using SyncMngrMsgList_t		= MessageListFrom< 
										GpuMsg::CreateFence,
										GpuMsg::DestroyFence
									>;

		using CmdBuffers_t			= FixedSizeArray< ModulePtr, 16 >;

		using Semaphores_t			= FixedSizeArray< GpuSemaphoreId, 16 >;
		using WaitSemaphores_t		= FixedSizeArray<Pair< GpuSemaphoreId, EPipelineStage::bits >, 16 >;
		using Fences_t				= FixedSizeArray< GpuFenceId, 8 >;

		struct PerFrame : CompileTime::FastCopyable
		{
			GpuFenceId			fence		= Uninitialized;	// wait before reuse command buffers
			CmdBuffers_t		commands;						// used command buffers
			CmdBuffers_t		freeBuffers;					// available command buffers
			Fences_t			waitFences;						// wait before begin frame (in client side)
			WaitSemaphores_t	waitSemaphores;					// wait before begin executing (in gpu side)
			Semaphores_t		signalSemaphores;				// signal after executing (in gpu side)
		};

		using PerFrameArray_t		= FixedSizeArray< PerFrame, 4 >;

		enum class EScope
		{
			None,
			Frame,
			Command,
		};


	// constants
	private:
		static const TypeIdList		_msgTypes;
		static const TypeIdList		_eventTypes;


	// variables
	private:
		ModulePtr			_syncManager;
		ModulePtr			_builder;
		UntypedID_t			_cmdBufferId;

		PerFrameArray_t		_perFrame;
		const uint			_bufferChainLength;
		uint				_bufferIndex;

		EScope				_scope;
		ModulePtr			_framebuffer;


	// methods
	public:
		PerFrameCommandBuffers (GlobalSystemsRef gs, const CreateInfo::PerFrameCommandBuffers &ci);
		~PerFrameCommandBuffers ();


	// message handlers
	private:
		bool _Link (const Message< ModuleMsg::Link > &);
		bool _Compose (const Message< ModuleMsg::Compose > &);
		bool _Delete (const Message< ModuleMsg::Delete > &);
		bool _AttachModule (const Message< ModuleMsg::AttachModule > &);
		bool _DetachModule (const Message< ModuleMsg::DetachModule > &);

		bool _CmdBegin (const Message< GraphicsMsg::CmdBegin > &);
		bool _CmdEnd (const Message< GraphicsMsg::CmdEnd > &);
		bool _CmdBeginFrame (const Message< GraphicsMsg::CmdBeginFrame > &);
		bool _CmdEndFrame (const Message< GraphicsMsg::CmdEndFrame > &);
		bool _CmdGetCurrentFramebuffer (const Message< GraphicsMsg::CmdGetCurrentFramebuffer > &);
		bool _CmdAddFrameDependency (const Message< GraphicsMsg::CmdAddFrameDependency > &);
	};
//-----------------------------------------------------------------------------


	const TypeIdList	PerFrameCommandBuffers::_msgTypes{ UninitializedT< SupportedMessages_t >() };
	const TypeIdList	PerFrameCommandBuffers::_eventTypes{ UninitializedT< SupportedEvents_t >() };

/*
=================================================
	constructor
=================================================
*/
	PerFrameCommandBuffers::PerFrameCommandBuffers (GlobalSystemsRef gs, const CreateInfo::PerFrameCommandBuffers &ci) :
		GraphicsBaseModule( gs, ModuleConfig{ PerFrameCommandBuffersModuleID, UMax }, &_msgTypes, &_eventTypes ),
		_bufferChainLength{ Max( 2u, ci.bufferChainLength ) },		_bufferIndex{ 0 },
		_scope{ EScope::None }
	{
		SetDebugName( "PerFrameCommandBuffers" );

		_SubscribeOnMsg( this, &PerFrameCommandBuffers::_OnModuleAttached_Impl );
		_SubscribeOnMsg( this, &PerFrameCommandBuffers::_OnModuleDetached_Impl );
		_SubscribeOnMsg( this, &PerFrameCommandBuffers::_AttachModule );
		_SubscribeOnMsg( this, &PerFrameCommandBuffers::_DetachModule );
		_SubscribeOnMsg( this, &PerFrameCommandBuffers::_FindModule_Impl );
		_SubscribeOnMsg( this, &PerFrameCommandBuffers::_ModulesDeepSearch_Impl );
		_SubscribeOnMsg( this, &PerFrameCommandBuffers::_Link );
		_SubscribeOnMsg( this, &PerFrameCommandBuffers::_Compose );
		_SubscribeOnMsg( this, &PerFrameCommandBuffers::_Delete );
		_SubscribeOnMsg( this, &PerFrameCommandBuffers::_OnManagerChanged );
		_SubscribeOnMsg( this, &PerFrameCommandBuffers::_CmdBegin );
		_SubscribeOnMsg( this, &PerFrameCommandBuffers::_CmdEnd );
		_SubscribeOnMsg( this, &PerFrameCommandBuffers::_CmdBeginFrame );
		_SubscribeOnMsg( this, &PerFrameCommandBuffers::_CmdEndFrame );
		_SubscribeOnMsg( this, &PerFrameCommandBuffers::_CmdGetCurrentFramebuffer );
		_SubscribeOnMsg( this, &PerFrameCommandBuffers::_CmdAddFrameDependency );

		_AttachSelfToManager( _GetGpuThread( ci.gpuThread ), UntypedID_t(0), true );

		_perFrame.Resize( _bufferChainLength );
	}
		
/*
=================================================
	desctructor
=================================================
*/
	PerFrameCommandBuffers::~PerFrameCommandBuffers ()
	{
	}
	
/*
=================================================
	_Link
=================================================
*/
	bool PerFrameCommandBuffers::_Link (const Message< ModuleMsg::Link > &msg)
	{
		if ( _IsComposedOrLinkedState( GetState() ) )
			return true;	// already linked

		CHECK_ERR( GetState() == EState::Initial or GetState() == EState::LinkingFailed );
		CHECK_ERR( _GetManager() );


		Message< GpuMsg::GetGraphicsModules >	req_ids;
		CHECK( _GetManager()->Send( req_ids ) );

		_cmdBufferId = req_ids->graphics->commandBuffer;


		// create builder
		_builder = GetModuleByMsg< CmdBufferMsgList_t >();

		if ( not _builder )
		{
			CHECK_ERR( GlobalSystems()->modulesFactory->Create(
										req_ids->graphics->commandBuilder,
										GlobalSystems(),
										CreateInfo::GpuCommandBuilder{ _GetManager() },
										OUT _builder ) );
			ModuleUtils::Initialize({ _builder });

			CHECK_ERR( _Attach( "builder", _builder, true ) );
		}

		CHECK_ERR( _CopySubscriptions< CmdBufferMsgList_t >( _builder ) );

		return Module::_Link_Impl( msg );
	}

/*
=================================================
	_Compose
=================================================
*/
	bool PerFrameCommandBuffers::_Compose (const Message< ModuleMsg::Compose > &msg)
	{
		if ( _IsComposedState( GetState() ) )
			return true;	// already composed

		CHECK_ERR( GetState() == EState::Linked );

		_syncManager = _GetManager()->GetModuleByMsg< SyncMngrMsgList_t >();
		CHECK_COMPOSING( _syncManager );

		FOR( i, _perFrame )
		{
			Message< GpuMsg::CreateFence >	req_fence;
			CHECK( _syncManager->Send( req_fence ) );

			_perFrame[i].fence = *req_fence->result;
		}
		
		_SendForEachAttachments( msg );
		
		// very paranoic check
		CHECK( _ValidateAllSubscriptions() );

		CHECK( _SetState( EState::ComposedMutable ) );
		return true;
	}
	
/*
=================================================
	_Delete
=================================================
*/
	bool PerFrameCommandBuffers::_Delete (const Message< ModuleMsg::Delete > &msg)
	{
		_SendForEachAttachments( msg );

		FOR( i, _perFrame )
		{
			auto&	cmds	= _perFrame[i].commands;
			auto	fence	= _perFrame[i].fence;

			if ( fence != GpuFenceId::Unknown )
			{
				CHECK( _syncManager->Send< GpuMsg::ClientWaitFence >({ fence }) );
				CHECK( _syncManager->Send< GpuMsg::DestroyFence >({ fence }) );
			}

			FOR( j, cmds ) {
				cmds[j]->Send( msg );
			}
		}

		_perFrame.Clear();

		_syncManager = null;

		return Module::_Delete_Impl( msg );
	}
	
/*
=================================================
	_AttachModule
=================================================
*/
	bool PerFrameCommandBuffers::_AttachModule (const Message< ModuleMsg::AttachModule > &msg)
	{
		const bool	is_builder	= msg->newModule->GetSupportedEvents().HasAllTypes< CmdBufferMsgList_t >();

		CHECK( _Attach( msg->name, msg->newModule, is_builder ) );

		if ( is_builder )
		{
			CHECK( _SetState( EState::Initial ) );
			_builder = null;
		}
		return true;
	}
	
/*
=================================================
	_DetachModule
=================================================
*/
	bool PerFrameCommandBuffers::_DetachModule (const Message< ModuleMsg::DetachModule > &msg)
	{
		CHECK( _Detach( msg->oldModule ) );

		if ( msg->oldModule->GetSupportedEvents().HasAllTypes< CmdBufferMsgList_t >() )
		{
			CHECK( _SetState( EState::Initial ) );
			_builder = null;
		}
		return true;
	}

/*
=================================================
	_CmdBeginFrame
=================================================
*/
	bool PerFrameCommandBuffers::_CmdBeginFrame (const Message< GraphicsMsg::CmdBeginFrame > &msg)
	{
		CHECK_ERR( _IsComposedState( GetState() ) );
		
		_bufferIndex = (_bufferIndex + 1) % _bufferChainLength;

		// wait until executing will be completed
		{
			auto&	per_frame	= _perFrame[ _bufferIndex ];
			per_frame.waitFences.PushFront( per_frame.fence );

			CHECK( _syncManager->Send< GpuMsg::ClientWaitFence >({ per_frame.waitFences }) );
		
			ModuleUtils::Send( per_frame.commands, Message< GpuMsg::SetCommandBufferState >{ GpuMsg::SetCommandBufferState::EState::Completed });

			per_frame.freeBuffers.Append( per_frame.commands );
			per_frame.waitFences.Clear();
			per_frame.commands.Clear();
		}

		// begin frame
		{
			Message< GpuMsg::ThreadBeginFrame >		begin;
			CHECK( _GetManager()->Send( begin ) );

			msg->result.Set({ begin->result->framebuffer, begin->result->frameIndex, _bufferIndex });

			_framebuffer = begin->result->framebuffer;
			_scope		 = EScope::Frame;

			_SendEvent< GraphicsMsg::OnCmdBeginFrame >({ _bufferIndex });
		}
		return true;
	}
	
/*
=================================================
	_CmdEndFrame
=================================================
*/
	bool PerFrameCommandBuffers::_CmdEndFrame (const Message< GraphicsMsg::CmdEndFrame > &msg)
	{
		CHECK_ERR( _scope == EScope::Frame );
		CHECK_ERR( msg->framebuffer == null or msg->framebuffer == _framebuffer );
		
		auto&	per_frame = _perFrame[ _bufferIndex ];

		Message< GpuMsg::ThreadEndFrame >	end;
		end->fence			= per_frame.fence;
		end->commands		= per_frame.commands;
		end->framebuffer	= _framebuffer;
		end->waitSemaphores.Append( per_frame.waitSemaphores );
		end->signalSemaphores.Append( per_frame.signalSemaphores );

		CHECK( _GetManager()->Send( end ) );

		per_frame.waitSemaphores.Clear();

		_framebuffer = null;
		_scope		 = EScope::None;
		
		_SendEvent< GraphicsMsg::OnCmdEndFrame >({ _bufferIndex });
		return true;
	}
	
/*
=================================================
	_CmdGetCurrentFramebuffer
=================================================
*/
	bool PerFrameCommandBuffers::_CmdGetCurrentFramebuffer (const Message< GraphicsMsg::CmdGetCurrentFramebuffer > &msg)
	{
		msg->result.Set( _framebuffer );
		return true;
	}
	
/*
=================================================
	_CmdAddFrameDependency
=================================================
*/
	bool PerFrameCommandBuffers::_CmdAddFrameDependency (const Message< GraphicsMsg::CmdAddFrameDependency > &msg)
	{
		CHECK_ERR( _IsComposedState( GetState() ) );

		const uint	next_idx	= (_bufferIndex+1) % _bufferChainLength;
		auto &		per_frame	= _perFrame[ next_idx ];

		per_frame.waitFences.Append( msg->waitFences );
		per_frame.waitSemaphores.Append( msg->waitSemaphores );
		per_frame.signalSemaphores.Append( msg->signalSemaphores );
		return true;
	}

/*
=================================================
	_CmdBegin
=================================================
*/
	bool PerFrameCommandBuffers::_CmdBegin (const Message< GraphicsMsg::CmdBegin > &)
	{
		CHECK_ERR( _scope == EScope::Frame );

		auto&	per_frame = _perFrame[ _bufferIndex ];

		// find or create command buffer
		if ( per_frame.freeBuffers.Empty() )
		{
			ModulePtr	cmd_buf;
			CHECK_ERR( GlobalSystems()->modulesFactory->Create(
							_cmdBufferId,
							GlobalSystems(),
							CreateInfo::GpuCommandBuffer{
								_GetManager(),
								_builder,
								CommandBufferDescriptor{ ECmdBufferCreate::bits() | ECmdBufferCreate::ImplicitResetable }
							},
							OUT cmd_buf ) );

			_builder->Send< ModuleMsg::AttachModule >({ "", cmd_buf });

			per_frame.commands.PushBack( cmd_buf );
		}
		else
		{
			per_frame.commands.PushBack( per_frame.freeBuffers.Back() );
			per_frame.freeBuffers.PopBack();
		}


		Message< GpuMsg::CmdBegin >		begin;
		begin->targetCmdBuffer	= per_frame.commands.Back();

		CHECK( _builder->Send( begin ) );

		_scope = EScope::Command;
		return true;
	}
	
/*
=================================================
	_CmdEnd
=================================================
*/
	bool PerFrameCommandBuffers::_CmdEnd (const Message< GraphicsMsg::CmdEnd > &)
	{
		CHECK_ERR( _scope == EScope::Command );

		CHECK( _builder->Send< GpuMsg::CmdEnd >({}) );

		_scope = EScope::Frame;
		return true;
	}
//-----------------------------------------------------------------------------
	

/*
=================================================
	CreatePerFrameCommandBuffers
=================================================
*/
	ModulePtr  GraphicsObjectsConstructor::CreatePerFrameCommandBuffers (GlobalSystemsRef gs, const CreateInfo::PerFrameCommandBuffers &ci)
	{
		return New< PerFrameCommandBuffers >( gs, ci );
	}

}	// Graphics
}	// Engine
