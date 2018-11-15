// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Core/Config/Engine.Config.h"

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
										> >;

		using SupportedEvents_t		= CL1BaseModule::SupportedEvents_t;

		using CmdBufferMsg_t		= MessageListFrom< GpuMsg::SetCLCommandBufferQueue, GpuMsg::SetCommandBufferDependency,
														GpuMsg::SetCommandBufferState, GpuMsg::GetCommandBufferState >;

		using UsedResources_t		= Set< ModulePtr >;
		using CommandArray_t		= Array< GpuMsg::SetCLCommandBufferQueue::Command >;

		using ERecordingState		= GpuMsg::SetCommandBufferState::EState;


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


	// methods
	public:
		CL1CommandBuilder (UntypedID_t, GlobalSystemsRef gs, const CreateInfo::GpuCommandBuilder &ci);
		~CL1CommandBuilder ();


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


	
	const TypeIdList	CL1CommandBuilder::_eventTypes{ UninitializedT< SupportedEvents_t >() };

/*
=================================================
	constructor
=================================================
*/
	CL1CommandBuilder::CL1CommandBuilder (UntypedID_t id, GlobalSystemsRef gs, const CreateInfo::GpuCommandBuilder &ci) :
		CL1BaseModule( gs, ModuleConfig{ id, UMax }, &_eventTypes )
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

		ASSERT( _ValidateMsgSubscriptions< SupportedMessages_t >() );

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
	bool CL1CommandBuilder::_Compose (const ModuleMsg::Compose &msg)
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
	bool CL1CommandBuilder::_Delete (const ModuleMsg::Delete &msg)
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
	bool CL1CommandBuilder::_SetCommandBufferDependency (const GpuMsg::SetCommandBufferDependency &msg)
	{
		_resources.AddArray( msg.resources.Get() );
		return true;
	}
	
/*
=================================================
	_GetCommandBufferState
=================================================
*/
	bool CL1CommandBuilder::_GetCommandBufferState (const GpuMsg::GetCommandBufferState &msg)
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
	bool CL1CommandBuilder::_CmdBegin (const GpuMsg::CmdBegin &msg)
	{
		CHECK_ERR( not _cmdBuffer );
		
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
							CLCommandBufferModuleID,
							GlobalSystems(),
							CreateInfo::GpuCommandBuffer{
								CommandBufferDescription{ msg.flags }
							},
							OUT _cmdBuffer ) );

			_cmdBuffer->SetDebugName( String(GetDebugName()) << "_Buffer" );

			CHECK_ERR( _Attach( "", _cmdBuffer ) );
		}

		ModuleUtils::Initialize({ _cmdBuffer });
		_cmdBuffer->Send( GpuMsg::SetCommandBufferState{ ERecordingState::Recording } );
		
		// check buffer state
		CHECK_ERR( _cmdBuffer->Request( GpuMsg::GetCommandBufferState{} ) == ERecordingState::Recording );

		return true;
	}
	
/*
=================================================
	_GpuCmdEnd
=================================================
*/
	bool CL1CommandBuilder::_CmdEnd (const GpuMsg::CmdEnd &msg)
	{
		CHECK_ERR( _cmdBuffer );
		
		_cmdBuffer->Send( GpuMsg::SetCLCommandBufferQueue{ RVREF(_commands), RVREF(_bufferData), RVREF(_pushConstData) } );
		_cmdBuffer->Send( GpuMsg::SetCommandBufferDependency{ RVREF(_resources) } );
		_cmdBuffer->Send( GpuMsg::SetCommandBufferState{ ERecordingState::Executable } );

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
	bool CL1CommandBuilder::_CmdBindComputePipeline (const GpuMsg::CmdBindComputePipeline &msg)
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
	bool CL1CommandBuilder::_CmdDispatch (const GpuMsg::CmdDispatch &msg)
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
	bool CL1CommandBuilder::_CmdExecute (const GpuMsg::CmdExecute &msg)
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
	bool CL1CommandBuilder::_CmdBindComputeResourceTable (const GpuMsg::CmdBindComputeResourceTable &msg)
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
	bool CL1CommandBuilder::_CmdCopyBuffer (const GpuMsg::CmdCopyBuffer &msg)
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
	bool CL1CommandBuilder::_CmdCopyImage (const GpuMsg::CmdCopyImage &msg)
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
	bool CL1CommandBuilder::_CmdCopyBufferToImage (const GpuMsg::CmdCopyBufferToImage &msg)
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
	bool CL1CommandBuilder::_CmdCopyImageToBuffer (const GpuMsg::CmdCopyImageToBuffer &msg)
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
	bool CL1CommandBuilder::_CmdUpdateBuffer (const GpuMsg::CmdUpdateBuffer &msg)
	{
		CHECK_ERR( _cmdBuffer );
		CHECK_ERR( msg.dstBuffer and not msg.data.Empty() and msg.data.Size() < 65536_b );

		_commands.PushBack({ GpuMsg::CLCmdUpdateBuffer{ msg, _bufferData.Size() }, __FILE__, __LINE__ });
		
		_bufferData.Append( msg.data );
		return true;
	}
	
/*
=================================================
	_CmdFillBuffer
=================================================
*/
	bool CL1CommandBuilder::_CmdFillBuffer (const GpuMsg::CmdFillBuffer &msg)
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
	bool CL1CommandBuilder::_CmdClearColorImage (const GpuMsg::CmdClearColorImage &msg)
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
	bool CL1CommandBuilder::_CmdPipelineBarrier (const GpuMsg::CmdPipelineBarrier &msg)
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
	bool CL1CommandBuilder::_CmdPushConstants (const GpuMsg::CmdPushConstants &msg)
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
	bool CL1CommandBuilder::_CmdPushNamedConstants (const GpuMsg::CmdPushNamedConstants &msg)
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
	bool CL1CommandBuilder::_CmdDebugMarker (const GpuMsg::CmdDebugMarker &msg)
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
	bool CL1CommandBuilder::_CmdPushDebugGroup (const GpuMsg::CmdPushDebugGroup &msg)
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
	bool CL1CommandBuilder::_CmdPopDebugGroup (const GpuMsg::CmdPopDebugGroup &msg)
	{
		CHECK_ERR( _cmdBuffer );
		
		_commands.PushBack({ msg, __FILE__, __LINE__ });
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
