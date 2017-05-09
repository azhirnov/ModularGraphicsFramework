// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/Platforms/Vulkan/Impl/Vk1BaseModule.h"
#include "Engine/Platforms/Shared/GPU/Memory.h"

#if defined( GRAPHICS_API_VULKAN )

namespace Engine
{
namespace PlatformVK
{

	//
	// Vulkan Memory
	//

	class Vk1Memory final : public Vk1BaseModule
	{
	// types
	private:
		using SupportedMessages_t	= Vk1BaseModule::SupportedMessages_t::Append< MessageListFrom<
											ModuleMsg::GetStreamDescriptor,
											ModuleMsg::ReadFromStream,
											ModuleMsg::WriteToStream,
											ModuleMsg::MapGpuMemoryToCpu,
											ModuleMsg::FlushGpuMemoryRange,
											ModuleMsg::UnmapGpuMemory,
											ModuleMsg::CopyGpuMemory
										> >;

		using SupportedEvents_t		= Vk1BaseModule::SupportedEvents_t::Append< MessageListFrom<
											ModuleMsg::DataRegionChanged,
											ModuleMsg::OnGpuMemoryBindingChanged
										> >;

		using EBindingState			= ModuleMsg::OnGpuMemoryBindingChanged::EBindingState;


	// constants
	private:
		static const Runtime::VirtualTypeList	_msgTypes;
		static const Runtime::VirtualTypeList	_eventTypes;


	// variables
	private:
		vk::VkDeviceMemory		_mem;
		ubyte *					_mappedPtr;

		BytesUL					_size;
		BytesUL					_mappedOffset;
		BytesUL					_mappedSize;
		
		EGpuMemory::bits		_flags;
		EMemoryAccess::bits		_access;
		uint					_heapIndex;
		EBindingState			_binding;
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
		bool _Compose (const Message< ModuleMsg::Compose > &);
		bool _Delete (const Message< ModuleMsg::Delete > &);

	private:
		bool _IsCreated () const;
		bool _AllocForImage ();
		bool _AllocForBuffer ();
		void _FreeMemory ();

		static EMemoryAccess::bits _GpuMemoryToMemoryAccess (EGpuMemory::bits flags);
	};


}	// PlatformVK
}	// Engine

#endif	// GRAPHICS_API_VULKAN
