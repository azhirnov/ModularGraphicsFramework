// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Core/Config/Engine.Config.h"

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
											GpuMsg::SetCommandBufferDependency,
											GpuMsg::GetCommandBufferState
										> >::Append< GpuMsg::DefaultComputeCommands_t >;

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
		SWCommandBuilder (UntypedID_t, GlobalSystemsRef gs, const CreateInfo::GpuCommandBuilder &ci);
		~SWCommandBuilder ();


	// message handlers
	private:
		bool _Compose (const ModuleMsg::Compose &);
		bool _Delete (const ModuleMsg::Delete &);
		bool _SetCommandBufferDependency (const GpuMsg::SetCommandBufferDependency &);
		bool _GetCommandBufferState (const GpuMsg::GetCommandBufferState &);
		
		bool _CmdBegin (const GpuMsg::CmdBegin &);
		bool _CmdEnd (const GpuMsg::CmdEnd &);
		bool _CmdBindComputePipeline (const GpuMsg::CmdBindComputePipeline &);
		bool _CmdDispatch (const GpuMsg::CmdDispatch &);
		bool _CmdDispatchIndirect (const GpuMsg::CmdDispatchIndirect &);
		bool _CmdExecute (const GpuMsg::CmdExecute &);
		bool _CmdBindComputeResourceTable (const GpuMsg::CmdBindComputeResourceTable &);
		bool _CmdCopyBuffer (const GpuMsg::CmdCopyBuffer &);
		bool _CmdCopyImage (const GpuMsg::CmdCopyImage &);
		bool _CmdCopyBufferToImage (const GpuMsg::CmdCopyBufferToImage &);
		bool _CmdCopyImageToBuffer (const GpuMsg::CmdCopyImageToBuffer &);
		bool _CmdUpdateBuffer (const GpuMsg::CmdUpdateBuffer &);
		bool _CmdFillBuffer (const GpuMsg::CmdFillBuffer &);
		bool _CmdClearColorImage (const GpuMsg::CmdClearColorImage &);
		bool _CmdPipelineBarrier (const GpuMsg::CmdPipelineBarrier &);
		bool _CmdPushConstants (const GpuMsg::CmdPushConstants &);
		bool _CmdPushNamedConstants (const GpuMsg::CmdPushNamedConstants &);
		bool _CmdDebugMarker (const GpuMsg::CmdDebugMarker &);
		bool _CmdPushDebugGroup (const GpuMsg::CmdPushDebugGroup &);
		bool _CmdPopDebugGroup (const GpuMsg::CmdPopDebugGroup &);
	};
//-----------------------------------------------------------------------------


	
	const TypeIdList	SWCommandBuilder::_msgTypes{ UninitializedT< SupportedMessages_t >() };
	const TypeIdList	SWCommandBuilder::_eventTypes{ UninitializedT< SupportedEvents_t >() };

/*
=================================================
	constructor
=================================================
*/
	SWCommandBuilder::SWCommandBuilder (UntypedID_t id, GlobalSystemsRef gs, const CreateInfo::GpuCommandBuilder &ci) :
		SWBaseModule( gs, ModuleConfig{ id, UMax }, &_msgTypes, &_eventTypes )
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
		_SubscribeOnMsg( this, &SWCommandBuilder::_CmdDispatchIndirect );
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
		_SubscribeOnMsg( this, &SWCommandBuilder::_CmdDebugMarker );
		_SubscribeOnMsg( this, &SWCommandBuilder::_CmdPushDebugGroup );
		_SubscribeOnMsg( this, &SWCommandBuilder::_CmdPopDebugGroup );

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
	bool SWCommandBuilder::_Compose (const ModuleMsg::Compose &msg)
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
	bool SWCommandBuilder::_Delete (const ModuleMsg::Delete &msg)
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
	bool SWCommandBuilder::_SetCommandBufferDependency (const GpuMsg::SetCommandBufferDependency &msg)
	{
		_resources.AddArray( msg.resources.Get() );
		return true;
	}
	
/*
=================================================
	_GetCommandBufferState
=================================================
*/
	bool SWCommandBuilder::_GetCommandBufferState (const GpuMsg::GetCommandBufferState &msg)
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
	bool SWCommandBuilder::_CmdBegin (const GpuMsg::CmdBegin &msg)
	{
		CHECK_ERR( not _cmdBuffer );
		
		_resources.Clear();		_resources.Reserve( 128 );
		_commands.Clear();		_commands.Reserve( 128 );
		
		// use target command buffer
		if ( msg.targetCmdBuffer )
		{
			CHECK_ERR( msg.targetCmdBuffer->GetSupportedMessages().HasAllTypes< CmdBufferMsg_t >() );

			_cmdBuffer = msg.targetCmdBuffer;
		}
		else
		// create new command buffer
		{
			CHECK_ERR( GlobalSystems()->modulesFactory->Create(
							SWCommandBufferModuleID,
							GlobalSystems(),
							CreateInfo::GpuCommandBuffer{
								CommandBufferDescription{ msg.flags }
							},
							OUT _cmdBuffer ) );

			CHECK_ERR( _Attach( "", _cmdBuffer ) );
		}

		ModuleUtils::Initialize({ _cmdBuffer });
		_cmdBuffer->Send( GpuMsg::SetCommandBufferState{ ERecordingState::Recording });
		
		// check buffer state
		GpuMsg::GetCommandBufferState	req_state;
		_cmdBuffer->Send( req_state );
		CHECK_ERR( *req_state.result == ERecordingState::Recording );

		return true;
	}
	
/*
=================================================
	_GpuCmdEnd
=================================================
*/
	bool SWCommandBuilder::_CmdEnd (const GpuMsg::CmdEnd &msg)
	{
		CHECK_ERR( _cmdBuffer );
		
		_cmdBuffer->Send( GpuMsg::SetSWCommandBufferQueue{ RVREF(_commands), RVREF(_bufferData), RVREF(_pushConstData) });
		_cmdBuffer->Send( GpuMsg::SetCommandBufferDependency{ RVREF(_resources) });
		_cmdBuffer->Send( GpuMsg::SetCommandBufferState{ ERecordingState::Executable });

		msg.result.Set( _cmdBuffer );

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
	bool SWCommandBuilder::_CmdBindComputePipeline (const GpuMsg::CmdBindComputePipeline &msg)
	{
		CHECK_ERR( _cmdBuffer );
		CHECK_ERR( msg.pipeline );

		_commands.PushBack({ msg, __FILE__, __LINE__ });
		return false;
	}
	
/*
=================================================
	_CmdDispatch
=================================================
*/
	bool SWCommandBuilder::_CmdDispatch (const GpuMsg::CmdDispatch &msg)
	{
		CHECK_ERR( _cmdBuffer );
		
		_commands.PushBack({ msg, __FILE__, __LINE__ });
		return true;
	}
	
/*
=================================================
	_CmdDispatchIndirect
=================================================
*/
	bool SWCommandBuilder::_CmdDispatchIndirect (const GpuMsg::CmdDispatchIndirect &msg)
	{
		CHECK_ERR( _cmdBuffer );
		
		_commands.PushBack({ msg, __FILE__, __LINE__ });
		return true;
	}
	
/*
=================================================
	_CmdExecute
=================================================
*/
	bool SWCommandBuilder::_CmdExecute (const GpuMsg::CmdExecute &msg)
	{
		CHECK_ERR( _cmdBuffer );
		
		_commands.PushBack({ msg, __FILE__, __LINE__ });
		return true;
	}
	
/*
=================================================
	_CmdBindComputeResourceTable
=================================================
*/
	bool SWCommandBuilder::_CmdBindComputeResourceTable (const GpuMsg::CmdBindComputeResourceTable &msg)
	{
		CHECK_ERR( _cmdBuffer );
		
		_commands.PushBack({ msg, __FILE__, __LINE__ });
		return true;
	}
	
/*
=================================================
	_CmdCopyBuffer
=================================================
*/
	bool SWCommandBuilder::_CmdCopyBuffer (const GpuMsg::CmdCopyBuffer &msg)
	{
		CHECK_ERR( _cmdBuffer );
		CHECK_ERR( msg.srcBuffer and msg.dstBuffer );
		
		_commands.PushBack({ msg, __FILE__, __LINE__ });
		return true;
	}
	
/*
=================================================
	_CmdCopyImage
=================================================
*/
	bool SWCommandBuilder::_CmdCopyImage (const GpuMsg::CmdCopyImage &msg)
	{
		CHECK_ERR( _cmdBuffer );
		CHECK_ERR( msg.srcImage and msg.dstImage );
		
		_commands.PushBack({ msg, __FILE__, __LINE__ });
		return true;
	}
	
/*
=================================================
	_CmdCopyBufferToImage
=================================================
*/
	bool SWCommandBuilder::_CmdCopyBufferToImage (const GpuMsg::CmdCopyBufferToImage &msg)
	{
		CHECK_ERR( _cmdBuffer );
		CHECK_ERR( msg.srcBuffer and msg.dstImage );
		
		_commands.PushBack({ msg, __FILE__, __LINE__ });
		return true;
	}
	
/*
=================================================
	_CmdCopyImageToBuffer
=================================================
*/
	bool SWCommandBuilder::_CmdCopyImageToBuffer (const GpuMsg::CmdCopyImageToBuffer &msg)
	{
		CHECK_ERR( _cmdBuffer );
		CHECK_ERR( msg.srcImage and msg.dstBuffer );
		
		_commands.PushBack({ msg, __FILE__, __LINE__ });
		return true;
	}
	
/*
=================================================
	_GpuCmdUpdateBuffer
=================================================
*/
	bool SWCommandBuilder::_CmdUpdateBuffer (const GpuMsg::CmdUpdateBuffer &msg)
	{
		CHECK_ERR( _cmdBuffer );
		CHECK_ERR( msg.dstBuffer and not msg.data.Empty() and msg.data.Size() < BytesU(SWDeviceProperties.limits.maxUpdateBufferSize) );
		
		_commands.PushBack({ msg, __FILE__, __LINE__ });
		return true;
	}
	
/*
=================================================
	_CmdFillBuffer
=================================================
*/
	bool SWCommandBuilder::_CmdFillBuffer (const GpuMsg::CmdFillBuffer &msg)
	{
		CHECK_ERR( _cmdBuffer );
		CHECK_ERR( msg.dstBuffer );
		
		_commands.PushBack({ msg, __FILE__, __LINE__ });
		return true;
	}
	
/*
=================================================
	_CmdClearColorImage
=================================================
*/
	bool SWCommandBuilder::_CmdClearColorImage (const GpuMsg::CmdClearColorImage &msg)
	{
		CHECK_ERR( _cmdBuffer );
		CHECK_ERR( msg.image );
		
		_commands.PushBack({ msg, __FILE__, __LINE__ });
		return true;
	}
	
/*
=================================================
	_CmdPipelineBarrier
=================================================
*/
	bool SWCommandBuilder::_CmdPipelineBarrier (const GpuMsg::CmdPipelineBarrier &msg)
	{
		CHECK_ERR( _cmdBuffer );
		
		_commands.PushBack({ msg, __FILE__, __LINE__ });
		return true;
	}
	
/*
=================================================
	_CmdPushConstants
=================================================
*/
	bool SWCommandBuilder::_CmdPushConstants (const GpuMsg::CmdPushConstants &msg)
	{
		CHECK_ERR( _cmdBuffer );
		
		_commands.PushBack({ msg, __FILE__, __LINE__ });
		return true;
	}
	
/*
=================================================
	_CmdPushNamedConstants
=================================================
*/
	bool SWCommandBuilder::_CmdPushNamedConstants (const GpuMsg::CmdPushNamedConstants &msg)
	{
		CHECK_ERR( _cmdBuffer );
		
		_commands.PushBack({ msg, __FILE__, __LINE__ });
		return true;
	}
	
/*
=================================================
	_CmdDebugMarker
=================================================
*/
	bool SWCommandBuilder::_CmdDebugMarker (const GpuMsg::CmdDebugMarker &msg)
	{
		CHECK_ERR( _cmdBuffer );
		
		_commands.PushBack({ msg, __FILE__, __LINE__ });
		return true;
	}

/*
=================================================
	_CmdPushDebugGroup
=================================================
*/
	bool SWCommandBuilder::_CmdPushDebugGroup (const GpuMsg::CmdPushDebugGroup &msg)
	{
		CHECK_ERR( _cmdBuffer );
		
		_commands.PushBack({ msg, __FILE__, __LINE__ });
		return true;
	}

/*
=================================================
	_CmdPopDebugGroup
=================================================
*/
	bool SWCommandBuilder::_CmdPopDebugGroup (const GpuMsg::CmdPopDebugGroup &msg)
	{
		CHECK_ERR( _cmdBuffer );
		
		_commands.PushBack({ msg, __FILE__, __LINE__ });
		return true;
	}

}	// PlatformSW
//-----------------------------------------------------------------------------
	
namespace Platforms
{
	ModulePtr SoftRendererObjectsConstructor::CreateSWCommandBuilder (ModuleMsg::UntypedID_t id, GlobalSystemsRef gs, const CreateInfo::GpuCommandBuilder &ci)
	{
		return New< PlatformSW::SWCommandBuilder >( id, gs, ci );
	}
}	// Platforms
}	// Engine

#endif	// GRAPHICS_API_SOFT
