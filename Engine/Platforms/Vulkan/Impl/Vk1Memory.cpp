// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Engine/Platforms/Vulkan/Impl/Vk1Memory.h"
#include "Engine/Platforms/Vulkan/VulkanThread.h"
#include "Engine/Platforms/Vulkan/VulkanThread.h"

#if defined( GRAPHICS_API_VULKAN )

namespace Engine
{
namespace Platforms
{
	const Runtime::VirtualTypeList	Vk1Memory::_msgTypes{ UninitializedT< SupportedMessages_t >() };
	const Runtime::VirtualTypeList	Vk1Memory::_eventTypes{ UninitializedT< SupportedEvents_t >() };

/*
=================================================
	constructor
=================================================
*/
	Vk1Memory::Vk1Memory (const GlobalSystemsRef gs, const CreateInfo::GpuMemory &ci) :
		Module( gs, GetStaticID(), &_msgTypes, &_eventTypes ),
		_descr{ ci.size, ci.memFlags },
		_access( ci.access ),
		_isMapped( false )
	{
		_SubscribeOnMsg( this, &Vk1Memory::_OnModuleAttached_Impl );
		_SubscribeOnMsg( this, &Vk1Memory::_OnModuleDetached_Impl );
		_SubscribeOnMsg( this, &Vk1Memory::_AttachModule_Empty );
		_SubscribeOnMsg( this, &Vk1Memory::_DetachModule_Empty );
		_SubscribeOnMsg( this, &Vk1Memory::_FindModule_Empty );
		_SubscribeOnMsg( this, &Vk1Memory::_ModulesDeepSearch_Empty );
		_SubscribeOnMsg( this, &Vk1Memory::_Delete_Impl );
		_SubscribeOnMsg( this, &Vk1Memory::_GetStreamDescriptor );
		_SubscribeOnMsg( this, &Vk1Memory::_ReadFromStream );
		_SubscribeOnMsg( this, &Vk1Memory::_WriteToStream );
		_SubscribeOnMsg( this, &Vk1Memory::_MapGpuMemoryToCpu );
		_SubscribeOnMsg( this, &Vk1Memory::_FlushGpuMemoryRange );
		_SubscribeOnMsg( this, &Vk1Memory::_UnmapGpuMemory );
		_SubscribeOnMsg( this, &Vk1Memory::_CopyGpuMemory );

		CHECK( _ValidateMsgSubscriptions() );

		_AttachSelfToManager( ci.gpuThread, VulkanThread::GetStaticID(), true );
	}

/*
=================================================
	destructor
=================================================
*/
	Vk1Memory::~Vk1Memory ()
	{
	}

/*
=================================================
	_GetStreamDescriptor
=================================================
*/
	bool Vk1Memory::_GetStreamDescriptor (const Message< ModuleMsg::GetStreamDescriptor > &msg)
	{
		StreamDescriptor	descr;


		msg->result.Set( descr );
		return true;
	}
	
/*
=================================================
	_ReadFromStream
=================================================
*/
	bool Vk1Memory::_ReadFromStream (const Message< ModuleMsg::ReadFromStream > &msg)
	{
		//_SendEvent( Message< ModuleMsg::DataRegionChanged >{ this, EMemoryAccess::CpuRead, offset, BytesU(readn) } );
		return true;
	}
	
/*
=================================================
	_WriteToStream
=================================================
*/
	bool Vk1Memory::_WriteToStream (const Message< ModuleMsg::WriteToStream > &msg)
	{
		return true;
	}
	
/*
=================================================
	_MapGpuMemoryToCpu
=================================================
*/
	bool Vk1Memory::_MapGpuMemoryToCpu (const Message< ModuleMsg::MapGpuMemoryToCpu > &msg)
	{
		return true;
	}
	
/*
=================================================
	_FlushGpuMemoryRange
=================================================
*/
	bool Vk1Memory::_FlushGpuMemoryRange (const Message< ModuleMsg::FlushGpuMemoryRange > &msg)
	{
		return true;
	}
	
/*
=================================================
	_UnmapGpuMemory
=================================================
*/
	bool Vk1Memory::_UnmapGpuMemory (const Message< ModuleMsg::UnmapGpuMemory > &msg)
	{
		return true;
	}
	
/*
=================================================
	_CopyGpuMemory
=================================================
*/
	bool Vk1Memory::_CopyGpuMemory (const Message< ModuleMsg::CopyGpuMemory > &msg)
	{
		return true;
	}


}	// Platforms
}	// Engine

#endif	// GRAPHICS_API_VULKAN
