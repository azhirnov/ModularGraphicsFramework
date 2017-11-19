// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Engine/Platforms/Shared/GPU/CommandBuffer.h"
#include "Engine/Platforms/Shared/GPU/Image.h"
#include "Engine/Platforms/Shared/GPU/Buffer.h"
#include "Engine/Platforms/Shared/GPU/Pipeline.h"
#include "Engine/Platforms/OpenCL/Impl/CL2BaseModule.h"
#include "Engine/Platforms/OpenCL/OpenCLContext.h"

#if defined( COMPUTE_API_OPENCL )

namespace Engine
{
namespace PlatformCL
{

	//
	// OpenCL Command Buffer Builder
	//

	class CL2CommandBuilder final : public CL2BaseModule
	{
	// types
	private:
		using SupportedMessages_t	= CL2BaseModule::SupportedMessages_t::Append< MessageListFrom<
											GpuMsg::CmdBegin,
											GpuMsg::CmdEnd,
											GpuMsg::CmdBindComputePipeline,
											GpuMsg::CmdDispatch,
											GpuMsg::CmdExecute,
											GpuMsg::CmdBindComputeResourceTable,
											GpuMsg::CmdCopyBuffer,
											GpuMsg::CmdCopyImage,
											GpuMsg::CmdCopyBufferToImage,
											GpuMsg::CmdCopyImageToBuffer,
											GpuMsg::CmdUpdateBuffer,
											GpuMsg::CmdFillBuffer,
											GpuMsg::CmdClearColorImage,
											GpuMsg::CmdPipelineBarrier,
											GpuMsg::SetCommandBufferDependency,
											GpuMsg::GetCommandBufferState
										> >;

		using SupportedEvents_t		= CL2BaseModule::SupportedEvents_t;

		using CmdBufferMsg_t		= MessageListFrom< GpuMsg::SetCLCommandBufferQueue, GpuMsg::SetCommandBufferDependency,
														GpuMsg::SetCommandBufferState, GpuMsg::GetCommandBufferState >;

		using UsedResources_t		= Set< ModulePtr >;
		using CommandArray_t		= Array< GpuMsg::SetCLCommandBufferQueue::Command >;

		using ERecordingState		= GpuMsg::SetCommandBufferState::EState;


	// constants
	private:
		static const TypeIdList		_msgTypes;
		static const TypeIdList		_eventTypes;


	// variables
	private:
		UsedResources_t			_resources;
		CommandArray_t			_commands;
		ModulePtr				_cmdBuffer;		// current command buffer


	// methods
	public:
		CL2CommandBuilder (GlobalSystemsRef gs, const CreateInfo::GpuCommandBuilder &ci);
		~CL2CommandBuilder ();


	// message handlers
	private:
		bool _Compose (const Message< ModuleMsg::Compose > &);
		bool _Delete (const Message< ModuleMsg::Delete > &);
		bool _SetCommandBufferDependency (const Message< GpuMsg::SetCommandBufferDependency > &);
		bool _GetCommandBufferState (const Message< GpuMsg::GetCommandBufferState > &);
		
		bool _CmdBegin (const Message< GpuMsg::CmdBegin > &);
		bool _CmdEnd (const Message< GpuMsg::CmdEnd > &);
		bool _CmdBindComputePipeline (const Message< GpuMsg::CmdBindComputePipeline > &);
		bool _CmdDispatch (const Message< GpuMsg::CmdDispatch > &);
		bool _CmdExecute (const Message< GpuMsg::CmdExecute > &);
		bool _CmdBindComputeResourceTable (const Message< GpuMsg::CmdBindComputeResourceTable > &);
		bool _CmdCopyBuffer (const Message< GpuMsg::CmdCopyBuffer > &);
		bool _CmdCopyImage (const Message< GpuMsg::CmdCopyImage > &);
		bool _CmdCopyBufferToImage (const Message< GpuMsg::CmdCopyBufferToImage > &);
		bool _CmdCopyImageToBuffer (const Message< GpuMsg::CmdCopyImageToBuffer > &);
		bool _CmdUpdateBuffer (const Message< GpuMsg::CmdUpdateBuffer > &);
		bool _CmdFillBuffer (const Message< GpuMsg::CmdFillBuffer > &);
		bool _CmdClearColorImage (const Message< GpuMsg::CmdClearColorImage > &);
		bool _CmdPipelineBarrier (const Message< GpuMsg::CmdPipelineBarrier > &);
	};
//-----------------------------------------------------------------------------


	
	const TypeIdList	CL2CommandBuilder::_msgTypes{ UninitializedT< SupportedMessages_t >() };
	const TypeIdList	CL2CommandBuilder::_eventTypes{ UninitializedT< SupportedEvents_t >() };

/*
=================================================
	constructor
=================================================
*/
	CL2CommandBuilder::CL2CommandBuilder (GlobalSystemsRef gs, const CreateInfo::GpuCommandBuilder &ci) :
		CL2BaseModule( gs, ModuleConfig{ CLCommandBuilderModuleID, ~0u }, &_msgTypes, &_eventTypes )
	{
		SetDebugName( "CL2CommandBuilder" );

		_SubscribeOnMsg( this, &CL2CommandBuilder::_OnModuleAttached_Impl );
		_SubscribeOnMsg( this, &CL2CommandBuilder::_OnModuleDetached_Impl );
		_SubscribeOnMsg( this, &CL2CommandBuilder::_AttachModule_Impl );
		_SubscribeOnMsg( this, &CL2CommandBuilder::_DetachModule_Impl );
		_SubscribeOnMsg( this, &CL2CommandBuilder::_FindModule_Impl );
		_SubscribeOnMsg( this, &CL2CommandBuilder::_ModulesDeepSearch_Impl );
		_SubscribeOnMsg( this, &CL2CommandBuilder::_Link_Impl );
		_SubscribeOnMsg( this, &CL2CommandBuilder::_Compose );
		_SubscribeOnMsg( this, &CL2CommandBuilder::_Delete );
		_SubscribeOnMsg( this, &CL2CommandBuilder::_OnManagerChanged );
		_SubscribeOnMsg( this, &CL2CommandBuilder::_GetDeviceInfo );
		_SubscribeOnMsg( this, &CL2CommandBuilder::_GetCLDeviceInfo );
		_SubscribeOnMsg( this, &CL2CommandBuilder::_GetCLPrivateClasses );
		_SubscribeOnMsg( this, &CL2CommandBuilder::_DeviceBeforeDestroy );
		_SubscribeOnMsg( this, &CL2CommandBuilder::_GetCommandBufferState );
		_SubscribeOnMsg( this, &CL2CommandBuilder::_SetCommandBufferDependency );

		_SubscribeOnMsg( this, &CL2CommandBuilder::_CmdBegin );
		_SubscribeOnMsg( this, &CL2CommandBuilder::_CmdEnd );
		_SubscribeOnMsg( this, &CL2CommandBuilder::_CmdBindComputePipeline );
		_SubscribeOnMsg( this, &CL2CommandBuilder::_CmdDispatch );
		_SubscribeOnMsg( this, &CL2CommandBuilder::_CmdExecute );
		_SubscribeOnMsg( this, &CL2CommandBuilder::_CmdBindComputeResourceTable );
		_SubscribeOnMsg( this, &CL2CommandBuilder::_CmdCopyBuffer );
		_SubscribeOnMsg( this, &CL2CommandBuilder::_CmdCopyImage );
		_SubscribeOnMsg( this, &CL2CommandBuilder::_CmdCopyBufferToImage );
		_SubscribeOnMsg( this, &CL2CommandBuilder::_CmdCopyImageToBuffer );
		_SubscribeOnMsg( this, &CL2CommandBuilder::_CmdUpdateBuffer );
		_SubscribeOnMsg( this, &CL2CommandBuilder::_CmdFillBuffer );
		_SubscribeOnMsg( this, &CL2CommandBuilder::_CmdClearColorImage );
		_SubscribeOnMsg( this, &CL2CommandBuilder::_CmdPipelineBarrier );
		
		CHECK( _ValidateMsgSubscriptions() );

		_AttachSelfToManager( ci.gpuThread, CLThreadModuleID, true );
	}
		
/*
=================================================
	desctructor
=================================================
*/
	CL2CommandBuilder::~CL2CommandBuilder ()
	{
	}
	
/*
=================================================
	_Compose
=================================================
*/
	bool CL2CommandBuilder::_Compose (const Message< ModuleMsg::Compose > &msg)
	{
		if ( _IsComposedState( GetState() ) )
			return true;	// already composed

		CHECK_ERR( GetState() == EState::Linked );


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
	bool CL2CommandBuilder::_Delete (const Message< ModuleMsg::Delete > &msg)
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
	bool CL2CommandBuilder::_SetCommandBufferDependency (const Message< GpuMsg::SetCommandBufferDependency > &msg)
	{
		_resources.AddArray( msg->resources.Get() );
		return true;
	}
	
/*
=================================================
	_GetCommandBufferState
=================================================
*/
	bool CL2CommandBuilder::_GetCommandBufferState (const Message< GpuMsg::GetCommandBufferState > &msg)
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
	bool CL2CommandBuilder::_CmdBegin (const Message< GpuMsg::CmdBegin > &msg)
	{
		CHECK_ERR( not _cmdBuffer );
		
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
			CHECK_ERR( GlobalSystems()->Get< ModulesFactory >()->Create(
							CLCommandBufferModuleID,
							GlobalSystems(),
							CreateInfo::GpuCommandBuffer{
								_GetManager(),
								this,
								CommandBufferDescriptor{ msg->isSecondary, true }
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

		return true;
	}
	
/*
=================================================
	_GpuCmdEnd
=================================================
*/
	bool CL2CommandBuilder::_CmdEnd (const Message< GpuMsg::CmdEnd > &msg)
	{
		CHECK_ERR( _cmdBuffer );
		
		SendTo( _cmdBuffer, Message< GpuMsg::SetCLCommandBufferQueue >{ RVREF(_commands) } );
		SendTo( _cmdBuffer, Message< GpuMsg::SetCommandBufferDependency >{ RVREF(_resources) } );
		SendTo( _cmdBuffer, Message< GpuMsg::SetCommandBufferState >{ ERecordingState::Executable } );

		msg->cmdBuffer.Set( _cmdBuffer );

		_cmdBuffer = null;
		_resources.Clear();
		_commands.Clear();

		return true;
	}
	
/*
=================================================
	_CmdBindComputePipeline
=================================================
*/
	bool CL2CommandBuilder::_CmdBindComputePipeline (const Message< GpuMsg::CmdBindComputePipeline > &msg)
	{
		CHECK_ERR( _cmdBuffer );
		CHECK_ERR( msg->pipeline );

		_commands.PushBack({ msg.Data(), __FILE__, __LINE__ });
		return false;
	}
	
/*
=================================================
	_CmdDispatch
=================================================
*/
	bool CL2CommandBuilder::_CmdDispatch (const Message< GpuMsg::CmdDispatch > &msg)
	{
		CHECK_ERR( _cmdBuffer );
		
		_commands.PushBack({ msg.Data(), __FILE__, __LINE__ });
		return true;
	}

/*
=================================================
	_CmdExecute
=================================================
*/
	bool CL2CommandBuilder::_CmdExecute (const Message< GpuMsg::CmdExecute > &msg)
	{
		CHECK_ERR( _cmdBuffer );
		
		_commands.PushBack({ msg.Data(), __FILE__, __LINE__ });
		return true;
	}
	
/*
=================================================
	_CmdBindComputeResourceTable
=================================================
*/
	bool CL2CommandBuilder::_CmdBindComputeResourceTable (const Message< GpuMsg::CmdBindComputeResourceTable > &msg)
	{
		CHECK_ERR( _cmdBuffer );
		
		_commands.PushBack({ msg.Data(), __FILE__, __LINE__ });
		return true;
	}
	
/*
=================================================
	_CmdCopyBuffer
=================================================
*/
	bool CL2CommandBuilder::_CmdCopyBuffer (const Message< GpuMsg::CmdCopyBuffer > &msg)
	{
		CHECK_ERR( _cmdBuffer );
		CHECK_ERR( msg->srcBuffer and msg->dstBuffer );
		
		_commands.PushBack({ msg.Data(), __FILE__, __LINE__ });
		return true;
	}
	
/*
=================================================
	_CmdCopyImage
=================================================
*/
	bool CL2CommandBuilder::_CmdCopyImage (const Message< GpuMsg::CmdCopyImage > &msg)
	{
		CHECK_ERR( _cmdBuffer );
		CHECK_ERR( msg->srcImage and msg->dstImage );
		
		_commands.PushBack({ msg.Data(), __FILE__, __LINE__ });
		return true;
	}
	
/*
=================================================
	_CmdCopyBufferToImage
=================================================
*/
	bool CL2CommandBuilder::_CmdCopyBufferToImage (const Message< GpuMsg::CmdCopyBufferToImage > &msg)
	{
		CHECK_ERR( _cmdBuffer );
		CHECK_ERR( msg->srcBuffer and msg->dstImage );
		
		_commands.PushBack({ msg.Data(), __FILE__, __LINE__ });
		return true;
	}
	
/*
=================================================
	_CmdCopyImageToBuffer
=================================================
*/
	bool CL2CommandBuilder::_CmdCopyImageToBuffer (const Message< GpuMsg::CmdCopyImageToBuffer > &msg)
	{
		CHECK_ERR( _cmdBuffer );
		CHECK_ERR( msg->srcImage and msg->dstBuffer );
		
		_commands.PushBack({ msg.Data(), __FILE__, __LINE__ });
		return true;
	}
	
/*
=================================================
	_GpuCmdUpdateBuffer
=================================================
*/
	bool CL2CommandBuilder::_CmdUpdateBuffer (const Message< GpuMsg::CmdUpdateBuffer > &msg)
	{
		CHECK_ERR( _cmdBuffer );
		CHECK_ERR( msg->dstBuffer and not msg->data.Empty() and msg->data.Size() < 65536_b );
		
		_commands.PushBack({ msg.Data(), __FILE__, __LINE__ });
		return true;
	}
	
/*
=================================================
	_CmdFillBuffer
=================================================
*/
	bool CL2CommandBuilder::_CmdFillBuffer (const Message< GpuMsg::CmdFillBuffer > &msg)
	{
		CHECK_ERR( _cmdBuffer );
		CHECK_ERR( msg->dstBuffer );
		
		_commands.PushBack({ msg.Data(), __FILE__, __LINE__ });
		return true;
	}
	
/*
=================================================
	_CmdClearColorImage
=================================================
*/
	bool CL2CommandBuilder::_CmdClearColorImage (const Message< GpuMsg::CmdClearColorImage > &msg)
	{
		CHECK_ERR( _cmdBuffer );
		CHECK_ERR( msg->image );
		
		_commands.PushBack({ msg.Data(), __FILE__, __LINE__ });
		return true;
	}
	
/*
=================================================
	_CmdPipelineBarrier
=================================================
*/
	bool CL2CommandBuilder::_CmdPipelineBarrier (const Message< GpuMsg::CmdPipelineBarrier > &msg)
	{
		CHECK_ERR( _cmdBuffer );
		
		_commands.PushBack({ msg.Data(), __FILE__, __LINE__ });
		return true;
	}

}	// PlatformCL
//-----------------------------------------------------------------------------
	
namespace Platforms
{
	ModulePtr OpenCLContext::_CreateCL2CommandBuilder (GlobalSystemsRef gs, const CreateInfo::GpuCommandBuilder &ci)
	{
		return New< PlatformCL::CL2CommandBuilder >( gs, ci );
	}
}	// Platforms
}	// Engine

#endif	// COMPUTE_API_OPENCL
