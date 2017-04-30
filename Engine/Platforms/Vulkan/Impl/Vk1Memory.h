// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/Platforms/Vulkan/Impl/Vk1Enums.h"
#include "Engine/Platforms/Shared/GPU/Memory.h"

#if defined( GRAPHICS_API_VULKAN )

namespace Engine
{
namespace Platforms
{

	//
	// Vulkan Memory
	//

	class Vk1Memory final : public Module
	{
	// types
	private:
		using SupportedMessages_t	= MessageListFrom< 
											ModuleMsg::AttachModule,
											ModuleMsg::DetachModule,
											ModuleMsg::OnModuleAttached,
											ModuleMsg::OnModuleDetached,
											ModuleMsg::FindModule,
											ModuleMsg::ModulesDeepSearch,
											ModuleMsg::Delete,
											ModuleMsg::GetStreamDescriptor,
											ModuleMsg::ReadFromStream,
											ModuleMsg::WriteToStream,
											ModuleMsg::MapGpuMemoryToCpu,
											ModuleMsg::FlushGpuMemoryRange,
											ModuleMsg::UnmapGpuMemory,
											ModuleMsg::CopyGpuMemory
										>;

		using SupportedEvents_t		= MessageListFrom<
											ModuleMsg::DataRegionChanged,
											ModuleMsg::Delete
										>;


	// constants
	private:
		static const Runtime::VirtualTypeList	_msgTypes;
		static const Runtime::VirtualTypeList	_eventTypes;


	// variables
	private:
		GpuMemoryDescriptor		_descr;
		EMemoryAccess::bits		_access;
		bool					_isMapped;


	// methods
	public:
		Vk1Memory (const GlobalSystemsRef gs, const CreateInfo::GpuMemory &ci);
		~Vk1Memory ();

		static OModID::type		GetStaticID ()			{ return "vk1.memory"_OModID; }


	// message handlers
	private:
		bool _GetStreamDescriptor (const Message< ModuleMsg::GetStreamDescriptor > &);
		bool _ReadFromStream (const Message< ModuleMsg::ReadFromStream > &);
		bool _WriteToStream (const Message< ModuleMsg::WriteToStream > &);
		bool _MapGpuMemoryToCpu (const Message< ModuleMsg::MapGpuMemoryToCpu > &);
		bool _FlushGpuMemoryRange (const Message< ModuleMsg::FlushGpuMemoryRange > &);
		bool _UnmapGpuMemory (const Message< ModuleMsg::UnmapGpuMemory > &);
		bool _CopyGpuMemory (const Message< ModuleMsg::CopyGpuMemory > &);
	};


}	// Platforms
}	// Engine

#endif	// GRAPHICS_API_VULKAN
