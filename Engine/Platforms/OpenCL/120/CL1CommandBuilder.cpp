// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Engine/Config/Engine.Config.h"

#ifdef COMPUTE_API_OPENCL

#include "Engine/Platforms/Public/GPU/CommandBuffer.h"
#include "Engine/Platforms/Public/GPU/Image.h"
#include "Engine/Platforms/Public/GPU/Buffer.h"
#include "Engine/Platforms/Public/GPU/Pipeline.h"
#include "Engine/Platforms/OpenCL/120/CL1BaseModule.h"
#include "Engine/Platforms/OpenCL/OpenCLObjectsConstructor.h"

namespace Engine
{
namespace PlatformCL
{

	//
	// OpenCL Command Buffer Builder
	//

	class CL1CommandBuilder final : public CL1BaseModule
	{
	// types
	private:
		using SupportedMessages_t	= CL1BaseModule::SupportedMessages_t::Append< MessageListFrom<
											GpuMsg::CmdBegin,
											GpuMsg::CmdEnd,
											GpuMsg::SetCommandBufferDependency,
											GpuMsg::GetCommandBufferState
										> >::Append< GpuMsg::DefaultComputeCommands_t >;

		using SupportedEvents_t		= CL1BaseModule::SupportedEvents_t;

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
		BinaryArray				_bufferData;	// used to store data for UpdateBuffer command
		BinaryArray				_pushConstData;	// used to store data for PushConstants command
		ModulePtr				_cmdBuffer;		// current command buffer


	// methods
	public:
		CL1CommandBuilder (UntypedID_t, GlobalSystemsRef gs, const CreateInfo::GpuCommandBuilder &ci);
		~CL1CommandBuilder ();


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
		bool _CmdPushConstants (const Message< GpuMsg::CmdPushConstants > &);
		bool _CmdPushNamedConstants (const Message< GpuMsg::CmdPushNamedConstants > &);
		bool _CmdDebugMarker (const Message< GpuMsg::CmdDebugMarker > &);
		bool _CmdPushDebugGroup (const Message< GpuMsg::CmdPushDebugGroup > &);
		bool _CmdPopDebugGroup (const Message< GpuMsg::CmdPopDebugGroup > &);
	};
//-----------------------------------------------------------------------------


	
	const TypeIdList	CL1CommandBuilder::_msgTypes{ UninitializedT< SupportedMessages_t >() };
	const TypeIdList	CL1CommandBuilder::_eventTypes{ UninitializedT< SupportedEvents_t >() };

/*
=================================================
	constructor
=================================================
*/
	CL1CommandBuilder::CL1CommandBuilder (UntypedID_t id, GlobalSystemsRef gs, const CreateInfo::GpuCommandBuilder &ci) :
		CL1BaseModule( gs, ModuleConfig{ id, UMax }, &_msgTypes, &_eventTypes )
	{
		SetDebugName( "CL1CommandBuilder" );

		_SubscribeOnMsg( this, &CL1CommandBuilder::_OnModuleAttached_Impl );
		_SubscribeOnMsg( this, &CL1CommandBuilder::_OnModuleDetached_Impl );
		_SubscribeOnMsg( this, &CL1CommandBuilder::_AttachModule_Impl );
		_SubscribeOnMsg( this, &CL1CommandBuilder::_DetachModule_Impl );
		_SubscribeOnMsg( this, &CL1CommandBuilder::_FindModule_Impl );
		_SubscribeOnMsg( this, &CL1CommandBuilder::_ModulesDeepSearch_Impl );
		_SubscribeOnMsg( this, &CL1CommandBuilder::_Link_Impl );
		_SubscribeOnMsg( this, &CL1CommandBuilder::_Compose );
		_SubscribeOnMsg( this, &CL1CommandBuilder::_Delete );
		_SubscribeOnMsg( this, &CL1CommandBuilder::_OnManagerChanged );
		_SubscribeOnMsg( this, &CL1CommandBuilder::_GetDeviceInfo );
		_SubscribeOnMsg( this, &CL1CommandBuilder::_GetCLDeviceInfo );
		_SubscribeOnMsg( this, &CL1CommandBuilder::_GetCLPrivateClasses );
		_SubscribeOnMsg( this, &CL1CommandBuilder::_GetCommandBufferState );
		_SubscribeOnMsg( this, &CL1CommandBuilder::_SetCommandBufferDependency );

		_SubscribeOnMsg( this, &CL1CommandBuilder::_CmdBegin );
		_SubscribeOnMsg( this, &CL1CommandBuilder::_CmdEnd );
		_SubscribeOnMsg( this, &CL1CommandBuilder::_CmdBindComputePipeline );
		_SubscribeOnMsg( this, &CL1CommandBuilder::_CmdDispatch );
		_SubscribeOnMsg( this, &CL1CommandBuilder::_CmdExecute );
		_SubscribeOnMsg( this, &CL1CommandBuilder::_CmdBindComputeResourceTable );
		_SubscribeOnMsg( this, &CL1CommandBuilder::_CmdCopyBuffer );
		_SubscribeOnMsg( this, &CL1CommandBuilder::_CmdCopyImage );
		_SubscribeOnMsg( this, &CL1CommandBuilder::_CmdCopyBufferToImage );
		_SubscribeOnMsg( this, &CL1CommandBuilder::_CmdCopyImageToBuffer );
		_SubscribeOnMsg( this, &CL1CommandBuilder::_CmdUpdateBuffer );
		_SubscribeOnMsg( this, &CL1CommandBuilder::_CmdFillBuffer );
		_SubscribeOnMsg( this, &CL1CommandBuilder::_CmdClearColorImage );
		_SubscribeOnMsg( this, &CL1CommandBuilder::_CmdPipelineBarrier );
		_SubscribeOnMsg( this, &CL1CommandBuilder::_CmdPushConstants );
		_SubscribeOnMsg( this, &CL1CommandBuilder::_CmdPushNamedConstants );
		_SubscribeOnMsg( this, &CL1CommandBuilder::_CmdDebugMarker );
		_SubscribeOnMsg( this, &CL1CommandBuilder::_CmdPushDebugGroup );
		_SubscribeOnMsg( this, &CL1CommandBuilder::_CmdPopDebugGroup );

		CHECK( _ValidateMsgSubscriptions() );

		_AttachSelfToManager( _GetGPUThread( ci.gpuThread ), UntypedID_t(0), true );
	}
		
/*
=================================================
	desctructor
=================================================
*/
	CL1CommandBuilder::~CL1CommandBuilder ()
	{
	}
	
/*
=================================================
	_Compose
=================================================
*/
	bool CL1CommandBuilder::_Compose (const Message< ModuleMsg::Compose > &msg)
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
	bool CL1CommandBuilder::_Delete (const Message< ModuleMsg::Delete > &msg)
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
	bool CL1CommandBuilder::_SetCommandBufferDependency (const Message< GpuMsg::SetCommandBufferDependency > &msg)
	{
		_resources.AddArray( msg->resources.Get() );
		return true;
	}
	
/*
=================================================
	_GetCommandBufferState
=================================================
*/
	bool CL1CommandBuilder::_GetCommandBufferState (const Message< GpuMsg::GetCommandBufferState > &msg)
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
	bool CL1CommandBuilder::_CmdBegin (const Message< GpuMsg::CmdBegin > &msg)
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
			CHECK_ERR( GlobalSystems()->modulesFactory->Create(
							CLCommandBufferModuleID,
							GlobalSystems(),
							CreateInfo::GpuCommandBuffer{
								CommandBufferDescriptor{ msg->flags }
							},
							OUT _cmdBuffer ) );

			CHECK_ERR( _Attach( "", _cmdBuffer ) );
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
	bool CL1CommandBuilder::_CmdEnd (const Message< GpuMsg::CmdEnd > &msg)
	{
		CHECK_ERR( _cmdBuffer );
		
		SendTo( _cmdBuffer, Message< GpuMsg::SetCLCommandBufferQueue >{ RVREF(_commands), RVREF(_bufferData), RVREF(_pushConstData) } );
		SendTo( _cmdBuffer, Message< GpuMsg::SetCommandBufferDependency >{ RVREF(_resources) } );
		SendTo( _cmdBuffer, Message< GpuMsg::SetCommandBufferState >{ ERecordingState::Executable } );

		msg->result.Set( _cmdBuffer );

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
	bool CL1CommandBuilder::_CmdBindComputePipeline (const Message< GpuMsg::CmdBindComputePipeline > &msg)
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
	bool CL1CommandBuilder::_CmdDispatch (const Message< GpuMsg::CmdDispatch > &msg)
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
	bool CL1CommandBuilder::_CmdExecute (const Message< GpuMsg::CmdExecute > &msg)
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
	bool CL1CommandBuilder::_CmdBindComputeResourceTable (const Message< GpuMsg::CmdBindComputeResourceTable > &msg)
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
	bool CL1CommandBuilder::_CmdCopyBuffer (const Message< GpuMsg::CmdCopyBuffer > &msg)
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
	bool CL1CommandBuilder::_CmdCopyImage (const Message< GpuMsg::CmdCopyImage > &msg)
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
	bool CL1CommandBuilder::_CmdCopyBufferToImage (const Message< GpuMsg::CmdCopyBufferToImage > &msg)
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
	bool CL1CommandBuilder::_CmdCopyImageToBuffer (const Message< GpuMsg::CmdCopyImageToBuffer > &msg)
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
	bool CL1CommandBuilder::_CmdUpdateBuffer (const Message< GpuMsg::CmdUpdateBuffer > &msg)
	{
		CHECK_ERR( _cmdBuffer );
		CHECK_ERR( msg->dstBuffer and not msg->data.Empty() and msg->data.Size() < 65536_b );

		_commands.PushBack({ GpuMsg::CLCmdUpdateBuffer{ msg.Data(), _bufferData.Size() }, __FILE__, __LINE__ });
		
		_bufferData.Append( msg->data );
		return true;
	}
	
/*
=================================================
	_CmdFillBuffer
=================================================
*/
	bool CL1CommandBuilder::_CmdFillBuffer (const Message< GpuMsg::CmdFillBuffer > &msg)
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
	bool CL1CommandBuilder::_CmdClearColorImage (const Message< GpuMsg::CmdClearColorImage > &msg)
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
	bool CL1CommandBuilder::_CmdPipelineBarrier (const Message< GpuMsg::CmdPipelineBarrier > &msg)
	{
		CHECK_ERR( _cmdBuffer );
		
		_commands.PushBack({ msg.Data(), __FILE__, __LINE__ });
		return true;
	}
	
/*
=================================================
	_CmdPushConstants
=================================================
*/
	bool CL1CommandBuilder::_CmdPushConstants (const Message< GpuMsg::CmdPushConstants > &msg)
	{
		CHECK_ERR( _cmdBuffer );
		
		_commands.PushBack({ msg.Data(), __FILE__, __LINE__ });
		return true;
	}
	
/*
=================================================
	_CmdPushNamedConstants
=================================================
*/
	bool CL1CommandBuilder::_CmdPushNamedConstants (const Message< GpuMsg::CmdPushNamedConstants > &msg)
	{
		CHECK_ERR( _cmdBuffer );
		
		_commands.PushBack({ msg.Data(), __FILE__, __LINE__ });
		return true;
	}
	
/*
=================================================
	_CmdDebugMarker
=================================================
*/
	bool CL1CommandBuilder::_CmdDebugMarker (const Message< GpuMsg::CmdDebugMarker > &msg)
	{
		CHECK_ERR( _cmdBuffer );
		
		_commands.PushBack({ msg.Data(), __FILE__, __LINE__ });
		return true;
	}
	
/*
=================================================
	_CmdPushDebugGroup
=================================================
*/
	bool CL1CommandBuilder::_CmdPushDebugGroup (const Message< GpuMsg::CmdPushDebugGroup > &msg)
	{
		CHECK_ERR( _cmdBuffer );
		
		_commands.PushBack({ msg.Data(), __FILE__, __LINE__ });
		return true;
	}
	
/*
=================================================
	_CmdPopDebugGroup
=================================================
*/
	bool CL1CommandBuilder::_CmdPopDebugGroup (const Message< GpuMsg::CmdPopDebugGroup > &msg)
	{
		CHECK_ERR( _cmdBuffer );
		
		_commands.PushBack({ msg.Data(), __FILE__, __LINE__ });
		return true;
	}

}	// PlatformCL
//-----------------------------------------------------------------------------
	
namespace Platforms
{
	ModulePtr OpenCLObjectsConstructor::CreateCL1CommandBuilder (ModuleMsg::UntypedID_t id, GlobalSystemsRef gs, const CreateInfo::GpuCommandBuilder &ci)
	{
		return New< PlatformCL::CL1CommandBuilder >( id, gs, ci );
	}
}	// Platforms
}	// Engine

#endif	// COMPUTE_API_OPENCL
