// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/Platforms/Vulkan/Impl/Vk1BaseModule.h"
#include "Engine/Platforms/Shared/GPU/Buffer.h"
#include "Engine/Platforms/Shared/GPU/Memory.h"

#if defined( GRAPHICS_API_VULKAN )

namespace Engine
{
namespace PlatformVK
{
	
	//
	// Vulkan Buffer
	//

	class Vk1Buffer final : public Vk1BaseModule
	{
	// types
	private:
		using SupportedMessages_t	= Vk1BaseModule::SupportedMessages_t::Append< MessageListFrom<
											ModuleMsg::GetGpuBufferDescriptor,
											ModuleMsg::GetVkBufferID,
											ModuleMsg::OnGpuMemoryBindingChanged
										> >;

		using SupportedEvents_t		= Vk1BaseModule::SupportedEvents_t;


	// constants
	private:
		static const Runtime::VirtualTypeList	_msgTypes;
		static const Runtime::VirtualTypeList	_eventTypes;


	// variables
	private:
		BufferDescriptor		_descr;
		vk::VkBuffer			_bufferId;
		bool					_isBindedToMemory;


	// methods
	public:
		Vk1Buffer (const GlobalSystemsRef gs, const CreateInfo::GpuBuffer &ci);
		~Vk1Buffer ();

		static OModID::type		GetStaticID ()				{ return "vk1.buffer"_OModID; }


	//protected:
		BufferDescriptor const&	GetDescriptor ()	const	{ return _descr; }
		vk::VkBuffer			GetBufferID ()		const	{ return _bufferId; }


	// message handlers
	private:
		bool _Link (const Message< ModuleMsg::Link > &);
		bool _Compose (const  Message< ModuleMsg::Compose > &);
		bool _Delete (const Message< ModuleMsg::Delete > &);
		bool _GetVkBufferID (const Message< ModuleMsg::GetVkBufferID > &);
		bool _GetGpuBufferDescriptor (const Message< ModuleMsg::GetGpuBufferDescriptor > &);
		bool _OnGpuMemoryBindingChanged (const Message< ModuleMsg::OnGpuMemoryBindingChanged > &);

	private:
		bool _IsCreated () const;
		bool _CreateBuffer ();
		void _DestroyBuffer ();
	};


}	// PlatformVK
}	// Engine

#endif	// GRAPHICS_API_VULKAN
