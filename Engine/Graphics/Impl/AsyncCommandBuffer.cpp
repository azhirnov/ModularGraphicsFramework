// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Engine/Graphics/Impl/GraphicsBaseModule.h"
#include "Engine/Graphics/Shared/Commands.h"
#include "Engine/Graphics/Impl/GraphicsObjectsConstructor.h"

namespace Engine
{
namespace Graphics
{
	using namespace Engine::Platforms;


	//
	// Async Command Buffer
	//

	class AsyncCommandBuffer final : public GraphicsBaseModule
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
											GraphicsMsg::CmdBeginAsync,
											GraphicsMsg::CmdEndAsync
										> >
										::Append< CmdBufferMsgList_t >;

		using SupportedEvents_t		= GraphicsBaseModule::SupportedEvents_t;


		using WaitSemaphores_t		= GraphicsMsg::CmdBeginAsync::WaitSemaphores_t;
		using Callback_t			= GraphicsMsg::CmdBeginAsync::Callback_t;
		using ESyncMode				= GraphicsMsg::CmdBeginAsync::EMode;

		struct PendingCommand : CompileTime::FastCopyable
		{
			WaitSemaphores_t	waitSemaphores;
			ESyncMode			syncMode;
			GpuSemaphoreId		externalSemaphore	 = Uninitialized;
			GpuSemaphoreId		beforeFrameExecuting = Uninitialized;
			Callback_t			onCompleted;
			ModulePtr			command;
		};

		struct WaitingCommand : CompileTime::FastCopyable
		{
			ModulePtr			cmdBuffer;
			Callback_t			onCompleted;
			GpuFenceId			fence			= GpuFenceId::Unknown;	// wait before destroy command buffer
			uint				frameLatency	= 2;					// number of frames to skip before wait fence
		};

		using PendingCommands_t		= FixedSizeArray< PendingCommand, 8 >;
		using WaitingsCommands_t	= FixedSizeArray< WaitingCommand, 32 >;


	// constants
	private:
		static const TypeIdList		_msgTypes;
		static const TypeIdList		_eventTypes;


	// variables
	private:
		ModulePtr			_builder;
		ModulePtr			_cmdBufferMngr;
		ModulePtr			_syncManager;

		PendingCommands_t	_pendingCommands;		// commands that ready to execute
		WaitingsCommands_t	_waitingCommands;		// keep pointer until command buffer executing

		UntypedID_t			_cmdBufferId;
		uint				_currCmdIndex;


	// methods
	public:
		AsyncCommandBuffer (GlobalSystemsRef gs, const CreateInfo::AsyncCommandBuffer &ci);
		~AsyncCommandBuffer ();


	// message handlers
	private:
		bool _Link (const Message< ModuleMsg::Link > &);
		bool _Compose (const Message< ModuleMsg::Compose > &);
		bool _Delete (const Message< ModuleMsg::Delete > &);
		bool _AttachModule (const Message< ModuleMsg::AttachModule > &);
		bool _DetachModule (const Message< ModuleMsg::DetachModule > &);

		bool _CmdBeginAsync (const Message< GraphicsMsg::CmdBeginAsync > &);
		bool _CmdEndAsync (const Message< GraphicsMsg::CmdEndAsync > &);

		// event handlers
		bool _OnCmdBeginFrame (const Message< GraphicsMsg::OnCmdBeginFrame > &);

	private:
		bool _SubmitInFrame (const ModulePtr &cmdBuf);
		bool _SubmitBetweenFrames (const ModulePtr &cmdBuf);
		bool _SubmitBeforeFrame (const ModulePtr &cmdBuf);
		bool _SubmitSync (const ModulePtr &cmdBuf);
		bool _SubmitAsync (const ModulePtr &cmdBuf);
	};
//-----------------------------------------------------------------------------


	const TypeIdList	AsyncCommandBuffer::_msgTypes{ UninitializedT< SupportedMessages_t >() };
	const TypeIdList	AsyncCommandBuffer::_eventTypes{ UninitializedT< SupportedEvents_t >() };

/*
=================================================
	constructor
=================================================
*/
	AsyncCommandBuffer::AsyncCommandBuffer (GlobalSystemsRef gs, const CreateInfo::AsyncCommandBuffer &ci) :
		GraphicsBaseModule( gs, ModuleConfig{ AsyncCommandBufferModuleID, UMax }, &_msgTypes, &_eventTypes ),
		_cmdBufferMngr{ ci.cmdBufMngr },	_cmdBufferId{ 0 },
		_currCmdIndex{ UMax }
	{
		SetDebugName( "AsyncCommandBuffer" );

		_SubscribeOnMsg( this, &AsyncCommandBuffer::_OnModuleAttached_Impl );
		_SubscribeOnMsg( this, &AsyncCommandBuffer::_OnModuleDetached_Impl );
		_SubscribeOnMsg( this, &AsyncCommandBuffer::_AttachModule );
		_SubscribeOnMsg( this, &AsyncCommandBuffer::_DetachModule );
		_SubscribeOnMsg( this, &AsyncCommandBuffer::_FindModule_Impl );
		_SubscribeOnMsg( this, &AsyncCommandBuffer::_ModulesDeepSearch_Impl );
		_SubscribeOnMsg( this, &AsyncCommandBuffer::_Link );
		_SubscribeOnMsg( this, &AsyncCommandBuffer::_Compose );
		_SubscribeOnMsg( this, &AsyncCommandBuffer::_Delete );
		_SubscribeOnMsg( this, &AsyncCommandBuffer::_OnManagerChanged );
		_SubscribeOnMsg( this, &AsyncCommandBuffer::_CmdBeginAsync );
		_SubscribeOnMsg( this, &AsyncCommandBuffer::_CmdEndAsync );

		_AttachSelfToManager( _GetGpuThread( ci.gpuThread ), UntypedID_t(0), true );
	}
		
/*
=================================================
	desctructor
=================================================
*/
	AsyncCommandBuffer::~AsyncCommandBuffer ()
	{
	}
	
/*
=================================================
	_Link
=================================================
*/
	bool AsyncCommandBuffer::_Link (const Message< ModuleMsg::Link > &msg)
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


		CHECK_ERR( _cmdBufferMngr );
		_cmdBufferMngr->Subscribe( this, &AsyncCommandBuffer::_OnCmdBeginFrame );

		return Module::_Link_Impl( msg );
	}

/*
=================================================
	_Compose
=================================================
*/
	bool AsyncCommandBuffer::_Compose (const Message< ModuleMsg::Compose > &msg)
	{
		if ( _IsComposedState( GetState() ) )
			return true;	// already composed

		CHECK_ERR( GetState() == EState::Linked );
		
		Message< GpuMsg::GetDeviceInfo >	req_dev;
		_GetManager()->Send( req_dev );
		CHECK_COMPOSING( _syncManager = req_dev->result->syncManager );
		
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
	bool AsyncCommandBuffer::_Delete (const Message< ModuleMsg::Delete > &msg)
	{
		_SendForEachAttachments( msg );

		_builder			= null;
		_cmdBufferMngr	= null;
		_syncManager		= null;

		return Module::_Delete_Impl( msg );
	}
	
/*
=================================================
	_AttachModule
=================================================
*/
	bool AsyncCommandBuffer::_AttachModule (const Message< ModuleMsg::AttachModule > &msg)
	{
		const bool	is_builder	= msg->newModule->GetSupportedEvents().HasAllTypes< CmdBufferMsgList_t >();

		CHECK( _Attach( msg->name, msg->newModule, is_builder ) );

		if ( is_builder )
		{
			CHECK( _SetState( EState::Initial ) );
			
			_builder		= null;
			_currCmdIndex	= UMax;
		}
		return true;
	}
	
/*
=================================================
	_DetachModule
=================================================
*/
	bool AsyncCommandBuffer::_DetachModule (const Message< ModuleMsg::DetachModule > &msg)
	{
		CHECK( _Detach( msg->oldModule ) );

		if ( msg->oldModule->GetSupportedEvents().HasAllTypes< CmdBufferMsgList_t >() )
		{
			CHECK( _SetState( EState::Initial ) );

			_builder		= null;
			_currCmdIndex	= UMax;
		}
		return true;
	}

/*
=================================================
	_CmdBeginAsync
=================================================
*/
	bool AsyncCommandBuffer::_CmdBeginAsync (const Message< GraphicsMsg::CmdBeginAsync > &msg)
	{
		CHECK_ERR( _IsComposedState( GetState() ) );

		// wait before start recording
		if ( not msg->waitFences.Empty() ) {
			_syncManager->Send< GpuMsg::ClientWaitFence >({ msg->waitFences });
		}

		// begin recording
		Message< GpuMsg::CmdBegin >		begin;
		CHECK( _builder->Send( begin ) );

		// validate
		switch ( msg->syncMode )
		{
			case ESyncMode::Async :
				// nothing to check
				break;

			case ESyncMode::Sync :
				CHECK_ERR( msg->beforeFrameExecuting == GpuSemaphoreId::Unknown and
						   msg->externalSemaphore == GpuSemaphoreId::Unknown );
				break;

			case ESyncMode::InFrame :
			case ESyncMode::BeforeFrame :
				CHECK_ERR( msg->waitSemaphores.Empty() );	// may be sync or perfomance issues, so don't use it
				CHECK_ERR( msg->beforeFrameExecuting == GpuSemaphoreId::Unknown );
				break;

			case ESyncMode::BetweenFrames :
				CHECK_ERR( msg->beforeFrameExecuting == GpuSemaphoreId::Unknown );
				break;

			default :
				RETURN_ERR( "unsupported sync mode" );
		}

		// setup
		_pendingCommands.PushBack({});
		_pendingCommands.Back().waitSemaphores.Append( msg->waitSemaphores );
		_pendingCommands.Back().syncMode = msg->syncMode;

		return true;
	}
	
/*
=================================================
	_CmdEndAsync
=================================================
*/
	bool AsyncCommandBuffer::_CmdEndAsync (const Message< GraphicsMsg::CmdEndAsync > &msg)
	{
		CHECK_ERR( _IsComposedState( GetState() ) );

		// end recording
		Message< GpuMsg::CmdEnd >	end;
		CHECK( _builder->Send( end ) );

		switch ( _pendingCommands.Back().syncMode )
		{
			case ESyncMode::Async :			return _SubmitAsync( *end->result );
			case ESyncMode::InFrame :		return _SubmitInFrame( *end->result );
			case ESyncMode::BetweenFrames :	return _SubmitBetweenFrames( *end->result );
			case ESyncMode::BeforeFrame :	return _SubmitBeforeFrame( *end->result );
			case ESyncMode::Sync :
			default :						return _SubmitSync( *end->result );
		}
	}
	
/*
=================================================
	_SubmitInFrame
=================================================
*/
	bool AsyncCommandBuffer::_SubmitInFrame (const ModulePtr &cmdBuf)
	{
		CHECK( _cmdBufferMngr->Send< GraphicsMsg::CmdAppend >({ cmdBuf }) );

		auto&	cmd = _pendingCommands.Back();

		if ( cmd.externalSemaphore != GpuSemaphoreId::Unknown )
		{
			Message< GraphicsMsg::CmdAddFrameDependency >	deps;
			deps->signalSemaphores.PushBack( cmd.externalSemaphore );

			CHECK( _cmdBufferMngr->Send( deps ) );
		}

		cmd.onCompleted.SafeCall(0);

		_pendingCommands.PopBack();
		return true;
	}
	
/*
=================================================
	_SubmitSync
=================================================
*/
	bool AsyncCommandBuffer::_SubmitSync (const ModulePtr &cmdBuf)
	{
		auto&	cmd = _pendingCommands.Back();

		// create fence
		Message< GpuMsg::CreateFence >	fence_ctor;
		CHECK( _syncManager->Send( fence_ctor ) );

		// submit commands
		Message< GpuMsg::SubmitGraphicsQueueCommands >	submit;
		submit->commands << cmdBuf;
		submit->fence	 = *fence_ctor->result;
		submit->waitSemaphores.Append( cmd.waitSemaphores );
		CHECK( _GetManager()->Send( submit ) );
		
		cmd.onCompleted.SafeCall(0);
		_pendingCommands.PopBack();

		// wait until executing completed and delete fence
		CHECK( _syncManager->Send< GpuMsg::ClientWaitFence >({ submit->fence }) );
		CHECK( _syncManager->Send< GpuMsg::DestroyFence >({ submit->fence }) );
	
		return true;
	}
	
/*
=================================================
	_SubmitAsync
=================================================
*/
	bool AsyncCommandBuffer::_SubmitAsync (const ModulePtr &cmdBuf)
	{
		auto&	cmd = _pendingCommands.Back();

		// submit commands
		Message< GpuMsg::SubmitGraphicsQueueCommands >	submit;
		
		_waitingCommands.PushBack({});
		_waitingCommands.Back().cmdBuffer	 = cmdBuf;
		_waitingCommands.Back().frameLatency = 2;	// TODO: must be bufferChainLength+1
		_waitingCommands.Back().onCompleted	 = RVREF(cmd.onCompleted); 

		submit->commands << cmdBuf;
		
		// add fence
		{
			Message< GpuMsg::CreateFence >	fence_ctor;
			CHECK( _syncManager->Send( fence_ctor ) );
			
			submit->fence					= *fence_ctor->result;
			_waitingCommands.Back().fence	= *fence_ctor->result;
		}

		// add semaphores
		{
			submit->waitSemaphores.Append( cmd.waitSemaphores );

			if ( cmd.externalSemaphore != GpuSemaphoreId::Unknown ) {
				submit->signalSemaphores.PushBack( cmd.externalSemaphore );
			}

			if ( cmd.beforeFrameExecuting != GpuSemaphoreId::Unknown )
			{
				Message< GraphicsMsg::CmdAddFrameDependency >	deps;

				submit->signalSemaphores.PushBack( cmd.beforeFrameExecuting );
				deps->waitSemaphores.PushBack({ cmd.beforeFrameExecuting, EPipelineStage::bits() | EPipelineStage::BottomOfPipe });	// TODO
				
				CHECK( _cmdBufferMngr->Send( deps ) );
			}
		}

		CHECK( _GetManager()->Send( submit ) );

		_pendingCommands.PopBack();
		return true;
	}
	
/*
=================================================
	_SubmitBetweenFrames
=================================================
*/
	bool AsyncCommandBuffer::_SubmitBetweenFrames (const ModulePtr &cmdBuf)
	{
		_pendingCommands.Back().command = cmdBuf;

		Message< GpuMsg::CreateSemaphore >	sem_ctor1;
		Message< GpuMsg::CreateSemaphore >	sem_ctor2;

		CHECK( _syncManager->Send( sem_ctor1 ) );
		CHECK( _syncManager->Send( sem_ctor2 ) );

		Message< GraphicsMsg::CmdAddFrameDependency >	deps;
		deps->signalSemaphores.PushBack( *sem_ctor1->result );

		_cmdBufferMngr->Send( deps );

		_pendingCommands.Back().waitSemaphores.PushBack({ *sem_ctor1->result, EPipelineStage::bits() | EPipelineStage::BottomOfPipe });
		_pendingCommands.Back().beforeFrameExecuting = *sem_ctor2->result;
		return true;
	}
	
/*
=================================================
	_SubmitBetweenFrames
=================================================
*/
	bool AsyncCommandBuffer::_SubmitBeforeFrame (const ModulePtr &cmdBuf)
	{
		Message< GpuMsg::CreateSemaphore >	sem_ctor;
		CHECK( _syncManager->Send( sem_ctor ) );

		ASSERT( _pendingCommands.Back().beforeFrameExecuting == GpuSemaphoreId::Unknown );

		_pendingCommands.Back().beforeFrameExecuting = *sem_ctor->result;

		_SubmitAsync( cmdBuf );
		return true;
	}

/*
=================================================
	_OnCmdBeginFrame
=================================================
*/
	bool AsyncCommandBuffer::_OnCmdBeginFrame (const Message< GraphicsMsg::OnCmdBeginFrame > &msg)
	{
		FOR( i, _waitingCommands )
		{
			auto&	cmd = _waitingCommands[i];

			if ( cmd.frameLatency > 0 )
			{
				--cmd.frameLatency;
				continue;
			}

			if ( cmd.fence != GpuFenceId::Unknown )
			{
				CHECK( _syncManager->Send< GpuMsg::ClientWaitFence >({ cmd.fence }) );
				CHECK( _syncManager->Send< GpuMsg::DestroyFence >({ cmd.fence }) );
			}

			if ( cmd.cmdBuffer ) {
				cmd.cmdBuffer->Send< ModuleMsg::Delete >({});
			}

			cmd.onCompleted.SafeCall( msg->cmdIndex );

			_waitingCommands.Erase( i );
			--i;
		}

		while ( not _pendingCommands.Empty() )
		{
			CHECK( _SubmitAsync( _pendingCommands.Back().command ) );
		}

		_currCmdIndex = msg->cmdIndex;
		return true;
	}
//-----------------------------------------------------------------------------
	

/*
=================================================
	CreateAsyncCommandBuffer
=================================================
*/
	ModulePtr  GraphicsObjectsConstructor::CreateAsyncCommandBuffer (GlobalSystemsRef gs, const CreateInfo::AsyncCommandBuffer &ci)
	{
		return New< AsyncCommandBuffer >( gs, ci );
	}

}	// Graphics
}	// Engine
