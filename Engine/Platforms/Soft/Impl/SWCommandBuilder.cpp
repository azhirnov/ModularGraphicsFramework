// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Engine/Config/Engine.Config.h"

#ifdef GRAPHICS_API_SOFT

#include "Engine/Platforms/Public/GPU/CommandBuffer.h"
#include "Engine/Platforms/Public/GPU/Image.h"
#include "Engine/Platforms/Public/GPU/Buffer.h"
#include "Engine/Platforms/Public/GPU/Pipeline.h"
#include "Engine/Platforms/Soft/Impl/SWBaseModule.h"
#include "Engine/Platforms/Soft/SoftRendererObjectsConstructor.h"

namespace Engine
{
namespace PlatformSW
{

	//
	// Software Renderer Command Buffer Builder
	//

	class SWCommandBuilder final : public SWBaseModule
	{
	// types
	private:
		using SupportedMessages_t	= SWBaseModule::SupportedMessages_t::Append< MessageListFrom<
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
											GpuMsg::CmdPushConstants,
											GpuMsg::CmdPushNamedConstants,
											GpuMsg::SetCommandBufferDependency,
											GpuMsg::GetCommandBufferState
										> >;

		using SupportedEvents_t		= SWBaseModule::SupportedEvents_t;

		using CmdBufferMsg_t		= MessageListFrom< GpuMsg::SetSWCommandBufferQueue, GpuMsg::SetCommandBufferDependency,
														GpuMsg::SetCommandBufferState, GpuMsg::GetCommandBufferState >;

		using UsedResources_t		= Set< ModulePtr >;
		using CommandArray_t		= Array< GpuMsg::SetSWCommandBufferQueue::Command >;

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
		SWCommandBuilder (GlobalSystemsRef gs, const CreateInfo::GpuCommandBuilder &ci);
		~SWCommandBuilder ();


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
	};
//-----------------------------------------------------------------------------


	
	const TypeIdList	SWCommandBuilder::_msgTypes{ UninitializedT< SupportedMessages_t >() };
	const TypeIdList	SWCommandBuilder::_eventTypes{ UninitializedT< SupportedEvents_t >() };

/*
=================================================
	constructor
=================================================
*/
	SWCommandBuilder::SWCommandBuilder (GlobalSystemsRef gs, const CreateInfo::GpuCommandBuilder &ci) :
		SWBaseModule( gs, ModuleConfig{ SWCommandBuilderModuleID, UMax }, &_msgTypes, &_eventTypes )
	{
		SetDebugName( "SWCommandBuilder" );

		_SubscribeOnMsg( this, &SWCommandBuilder::_OnModuleAttached_Impl );
		_SubscribeOnMsg( this, &SWCommandBuilder::_OnModuleDetached_Impl );
		_SubscribeOnMsg( this, &SWCommandBuilder::_AttachModule_Impl );
		_SubscribeOnMsg( this, &SWCommandBuilder::_DetachModule_Impl );
		_SubscribeOnMsg( this, &SWCommandBuilder::_FindModule_Impl );
		_SubscribeOnMsg( this, &SWCommandBuilder::_ModulesDeepSearch_Impl );
		_SubscribeOnMsg( this, &SWCommandBuilder::_Link_Impl );
		_SubscribeOnMsg( this, &SWCommandBuilder::_Compose );
		_SubscribeOnMsg( this, &SWCommandBuilder::_Delete );
		_SubscribeOnMsg( this, &SWCommandBuilder::_OnManagerChanged );
		_SubscribeOnMsg( this, &SWCommandBuilder::_GetDeviceInfo );
		_SubscribeOnMsg( this, &SWCommandBuilder::_GetSWDeviceInfo );
		_SubscribeOnMsg( this, &SWCommandBuilder::_GetSWPrivateClasses );
		_SubscribeOnMsg( this, &SWCommandBuilder::_GetCommandBufferState );
		_SubscribeOnMsg( this, &SWCommandBuilder::_SetCommandBufferDependency );

		_SubscribeOnMsg( this, &SWCommandBuilder::_CmdBegin );
		_SubscribeOnMsg( this, &SWCommandBuilder::_CmdEnd );
		_SubscribeOnMsg( this, &SWCommandBuilder::_CmdBindComputePipeline );
		_SubscribeOnMsg( this, &SWCommandBuilder::_CmdDispatch );
		_SubscribeOnMsg( this, &SWCommandBuilder::_CmdExecute );
		_SubscribeOnMsg( this, &SWCommandBuilder::_CmdBindComputeResourceTable );
		_SubscribeOnMsg( this, &SWCommandBuilder::_CmdCopyBuffer );
		_SubscribeOnMsg( this, &SWCommandBuilder::_CmdCopyImage );
		_SubscribeOnMsg( this, &SWCommandBuilder::_CmdCopyBufferToImage );
		_SubscribeOnMsg( this, &SWCommandBuilder::_CmdCopyImageToBuffer );
		_SubscribeOnMsg( this, &SWCommandBuilder::_CmdUpdateBuffer );
		_SubscribeOnMsg( this, &SWCommandBuilder::_CmdFillBuffer );
		_SubscribeOnMsg( this, &SWCommandBuilder::_CmdClearColorImage );
		_SubscribeOnMsg( this, &SWCommandBuilder::_CmdPipelineBarrier );
		_SubscribeOnMsg( this, &SWCommandBuilder::_CmdPushConstants );
		_SubscribeOnMsg( this, &SWCommandBuilder::_CmdPushNamedConstants );
		
		CHECK( _ValidateMsgSubscriptions() );

		_AttachSelfToManager( _GetGPUThread( ci.gpuThread ), UntypedID_t(0), true );
	}
		
/*
=================================================
	desctructor
=================================================
*/
	SWCommandBuilder::~SWCommandBuilder ()
	{
	}
	
/*
=================================================
	_Compose
=================================================
*/
	bool SWCommandBuilder::_Compose (const Message< ModuleMsg::Compose > &msg)
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
	bool SWCommandBuilder::_Delete (const Message< ModuleMsg::Delete > &msg)
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
	bool SWCommandBuilder::_SetCommandBufferDependency (const Message< GpuMsg::SetCommandBufferDependency > &msg)
	{
		_resources.AddArray( msg->resources.Get() );
		return true;
	}
	
/*
=================================================
	_GetCommandBufferState
=================================================
*/
	bool SWCommandBuilder::_GetCommandBufferState (const Message< GpuMsg::GetCommandBufferState > &msg)
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
	bool SWCommandBuilder::_CmdBegin (const Message< GpuMsg::CmdBegin > &msg)
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
							SWCommandBufferModuleID,
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

		return true;
	}
	
/*
=================================================
	_GpuCmdEnd
=================================================
*/
	bool SWCommandBuilder::_CmdEnd (const Message< GpuMsg::CmdEnd > &msg)
	{
		CHECK_ERR( _cmdBuffer );
		
		SendTo( _cmdBuffer, Message< GpuMsg::SetSWCommandBufferQueue >{ RVREF(_commands), RVREF(_bufferData), RVREF(_pushConstData) } );
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
	bool SWCommandBuilder::_CmdBindComputePipeline (const Message< GpuMsg::CmdBindComputePipeline > &msg)
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
	bool SWCommandBuilder::_CmdDispatch (const Message< GpuMsg::CmdDispatch > &msg)
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
	bool SWCommandBuilder::_CmdExecute (const Message< GpuMsg::CmdExecute > &msg)
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
	bool SWCommandBuilder::_CmdBindComputeResourceTable (const Message< GpuMsg::CmdBindComputeResourceTable > &msg)
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
	bool SWCommandBuilder::_CmdCopyBuffer (const Message< GpuMsg::CmdCopyBuffer > &msg)
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
	bool SWCommandBuilder::_CmdCopyImage (const Message< GpuMsg::CmdCopyImage > &msg)
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
	bool SWCommandBuilder::_CmdCopyBufferToImage (const Message< GpuMsg::CmdCopyBufferToImage > &msg)
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
	bool SWCommandBuilder::_CmdCopyImageToBuffer (const Message< GpuMsg::CmdCopyImageToBuffer > &msg)
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
	bool SWCommandBuilder::_CmdUpdateBuffer (const Message< GpuMsg::CmdUpdateBuffer > &msg)
	{
		CHECK_ERR( _cmdBuffer );
		CHECK_ERR( msg->dstBuffer and not msg->data.Empty() and msg->data.Size() < BytesU(SWDeviceProperties.limits.maxUpdateBufferSize) );
		
		_commands.PushBack({ msg.Data(), __FILE__, __LINE__ });
		return true;
	}
	
/*
=================================================
	_CmdFillBuffer
=================================================
*/
	bool SWCommandBuilder::_CmdFillBuffer (const Message< GpuMsg::CmdFillBuffer > &msg)
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
	bool SWCommandBuilder::_CmdClearColorImage (const Message< GpuMsg::CmdClearColorImage > &msg)
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
	bool SWCommandBuilder::_CmdPipelineBarrier (const Message< GpuMsg::CmdPipelineBarrier > &msg)
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
	bool SWCommandBuilder::_CmdPushConstants (const Message< GpuMsg::CmdPushConstants > &msg)
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
	bool SWCommandBuilder::_CmdPushNamedConstants (const Message< GpuMsg::CmdPushNamedConstants > &msg)
	{
		CHECK_ERR( _cmdBuffer );
		
		_commands.PushBack({ msg.Data(), __FILE__, __LINE__ });
		return true;
	}

}	// PlatformSW
//-----------------------------------------------------------------------------
	
namespace Platforms
{
	ModulePtr SoftRendererObjectsConstructor::CreateSWCommandBuilder (GlobalSystemsRef gs, const CreateInfo::GpuCommandBuilder &ci)
	{
		return New< PlatformSW::SWCommandBuilder >( gs, ci );
	}
}	// Platforms
}	// Engine

#endif	// GRAPHICS_API_SOFT
