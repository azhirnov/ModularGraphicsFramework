// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Core/Config/Engine.Config.h"

#ifdef GRAPHICS_API_VULKAN

#include "Engine/Platforms/Public/GPU/Query.h"
#include "Engine/Platforms/Vulkan/110/Vk1BaseModule.h"
#include "Engine/Platforms/Vulkan/VulkanObjectsConstructor.h"

namespace Engine
{
namespace PlatformVK
{
	
	//
	// Vulkan Query Pool
	//

	class Vk1QueryPool final : public Vk1BaseModule
	{
	// types
	private:
		using SupportedMessages_t	= Vk1BaseModule::SupportedMessages_t::Append< MessageListFrom<
											GpuMsg::GetQueryPoolDescription,
											GpuMsg::GetVkQueryPoolID
										> >;

		using SupportedEvents_t		= Vk1BaseModule::SupportedEvents_t;


	// constants
	private:
		static const TypeIdList		_eventTypes;


	// variables
	private:
		QueryPoolDescription	_descr;
		vk::VkQueryPool			_queryId;


	// methods
	public:
		Vk1QueryPool (UntypedID_t, GlobalSystemsRef gs, const CreateInfo::GpuQueryPool &ci);
		~Vk1QueryPool ();


	// message handlers
	private:
		bool _Compose (const ModuleMsg::Compose &);
		bool _Delete (const ModuleMsg::Delete &);

		bool _GetVkQueryPoolID (const GpuMsg::GetVkQueryPoolID &);
		bool _GetQueryPoolDescription (const GpuMsg::GetQueryPoolDescription &);


	private:
		bool _IsCreated () const;
		bool _CreateQueryPool ();
		void _DestroyQueryPool ();
	};
//-----------------------------------------------------------------------------


	
	const TypeIdList	Vk1QueryPool::_eventTypes{ UninitializedT< SupportedEvents_t >() };

/*
=================================================
	constructor
=================================================
*/
	Vk1QueryPool::Vk1QueryPool (UntypedID_t id, GlobalSystemsRef gs, const CreateInfo::GpuQueryPool &ci) :
		Vk1BaseModule( gs, ModuleConfig{ id, UMax }, &_eventTypes ),
		_descr( ci.descr ),			_queryId{ VK_NULL_HANDLE }
	{
		SetDebugName( "Vk1QueryPool" );

		_SubscribeOnMsg( this, &Vk1QueryPool::_OnModuleAttached_Impl );
		_SubscribeOnMsg( this, &Vk1QueryPool::_OnModuleDetached_Impl );
		_SubscribeOnMsg( this, &Vk1QueryPool::_AttachModule_Impl );
		_SubscribeOnMsg( this, &Vk1QueryPool::_DetachModule_Impl );
		_SubscribeOnMsg( this, &Vk1QueryPool::_FindModule_Impl );
		_SubscribeOnMsg( this, &Vk1QueryPool::_ModulesDeepSearch_Impl );
		_SubscribeOnMsg( this, &Vk1QueryPool::_Link_Impl );
		_SubscribeOnMsg( this, &Vk1QueryPool::_Compose );
		_SubscribeOnMsg( this, &Vk1QueryPool::_Delete );
		_SubscribeOnMsg( this, &Vk1QueryPool::_OnManagerChanged );
		_SubscribeOnMsg( this, &Vk1QueryPool::_GetVkQueryPoolID );
		_SubscribeOnMsg( this, &Vk1QueryPool::_GetQueryPoolDescription );
		_SubscribeOnMsg( this, &Vk1QueryPool::_GetDeviceInfo );
		_SubscribeOnMsg( this, &Vk1QueryPool::_GetVkDeviceInfo );
		_SubscribeOnMsg( this, &Vk1QueryPool::_GetVkPrivateClasses );

		_AttachSelfToManager( _GetGPUThread( ci.gpuThread ), UntypedID_t(0), true );
		
		ASSERT( _descr.count > 0 );
	}
	
/*
=================================================
	destructor
=================================================
*/
	Vk1QueryPool::~Vk1QueryPool ()
	{
		ASSERT( not _IsCreated() );
	}
	
/*
=================================================
	_Compose
=================================================
*/
	bool Vk1QueryPool::_Compose (const ModuleMsg::Compose &)
	{
		if ( _IsComposedState( GetState() ) or _IsCreated() )
			return true;	// already composed

		CHECK_ERR( GetState() == EState::Linked );

		CHECK_COMPOSING( _CreateQueryPool() );
		
		return _DefCompose( false );
	}
	
/*
=================================================
	_Delete
=================================================
*/
	bool Vk1QueryPool::_Delete (const ModuleMsg::Delete &msg)
	{
		_DestroyQueryPool();

		return Module::_Delete_Impl( msg );
	}
	
/*
=================================================
	_GetVkQueryPoolID
=================================================
*/
	bool Vk1QueryPool::_GetVkQueryPoolID (const GpuMsg::GetVkQueryPoolID &msg)
	{
		ASSERT( _IsCreated() );

		msg.result.Set( _queryId );
		return true;
	}
	
/*
=================================================
	_GetQueryPoolDescription
=================================================
*/
	bool Vk1QueryPool::_GetQueryPoolDescription (const GpuMsg::GetQueryPoolDescription &msg)
	{
		msg.result.Set( _descr );
		return true;
	}
	
/*
=================================================
	_IsCreated
=================================================
*/
	bool Vk1QueryPool::_IsCreated () const
	{
		return _queryId != VK_NULL_HANDLE;
	}
	
/*
=================================================
	_CreateQueryPool
=================================================
*/
	bool Vk1QueryPool::_CreateQueryPool ()
	{
		using namespace vk;

		VkQueryPoolCreateInfo	info = {};
		info.sType				= VK_STRUCTURE_TYPE_QUERY_POOL_CREATE_INFO;
		info.queryType			= Vk1Enum( _descr.queryType );
		info.queryCount			= _descr.count;
		info.pipelineStatistics	= Vk1Enum( _descr.statisticFlags );

		VK_CHECK( vkCreateQueryPool( GetVkDevice(), &info, null, OUT &_queryId ));
		return true;
	}
	
/*
=================================================
	_DestroyQueryPool
=================================================
*/
	void Vk1QueryPool::_DestroyQueryPool ()
	{
		using namespace vk;
		
		auto	dev = GetVkDevice();
		
		if ( dev != VK_NULL_HANDLE and _queryId != VK_NULL_HANDLE )
		{
			vkDestroyQueryPool( dev, _queryId, null );
		}

		_queryId	= VK_NULL_HANDLE;
		_descr		= Uninitialized;
	}


}	// PlatformVK
//-----------------------------------------------------------------------------

namespace Platforms
{
	ModulePtr VulkanObjectsConstructor::CreateVk1QueryPool (ModuleMsg::UntypedID_t id, GlobalSystemsRef gs, const CreateInfo::GpuQueryPool &ci)
	{
		return New< PlatformVK::Vk1QueryPool >( id, gs, ci );
	}
}	// Platforms
}	// Engine

#endif	// GRAPHICS_API_VULKAN
