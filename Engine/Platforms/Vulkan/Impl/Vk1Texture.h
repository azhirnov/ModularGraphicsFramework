// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/Platforms/Vulkan/Impl/Vk1BaseModule.h"
#include "Engine/Platforms/Shared/GPU/Texture.h"
#include "Engine/Platforms/Shared/GPU/Memory.h"

#if defined( GRAPHICS_API_VULKAN )

namespace Engine
{
namespace PlatformVK
{
	
	//
	// Vulkan Texture (Image)
	//

	class Vk1Texture final : public Vk1BaseModule
	{
	// types
	private:
		using SupportedMessages_t	= Vk1BaseModule::SupportedMessages_t::Append< MessageListFrom<
											ModuleMsg::GetGpuTextureDescriptor,
											ModuleMsg::GetVkTextureID,
											ModuleMsg::GetVkTextureView,
											ModuleMsg::OnGpuMemoryBindingChanged
										> >;

		using SupportedEvents_t		= Vk1BaseModule::SupportedEvents_t;

		using Utils					= TextureUtils;


	// constants
	private:
		static const Runtime::VirtualTypeList	_msgTypes;
		static const Runtime::VirtualTypeList	_eventTypes;


	// variables
	private:
		TextureDescriptor		_descr;
		vk::VkImage				_imageId;
		vk::VkImageView			_imageView;
		bool					_isBindedToMemory;


	// methods
	public:
		Vk1Texture (const GlobalSystemsRef gs, const CreateInfo::GpuTexture &ci);
		~Vk1Texture ();

		static OModID::type			GetStaticID ()				{ return "vk1.texture"_OModID; }


	//protected:
		TextureDescriptor const&	GetDescriptor ()	const	{ return _descr; }
		vk::VkImage					GetImageID ()		const	{ return _imageId; }
		vk::VkImageView				GetImageView ()		const	{ return _imageView; }


	// message handlers
	private:
		bool _Link (const Message< ModuleMsg::Link > &);
		bool _Compose (const  Message< ModuleMsg::Compose > &);
		bool _Delete (const Message< ModuleMsg::Delete > &);
		bool _GetVkTextureID (const Message< ModuleMsg::GetVkTextureID > &);
		bool _GetVkTextureView (const Message< ModuleMsg::GetVkTextureView > &);
		bool _GetGpuTextureDescriptor (const Message< ModuleMsg::GetGpuTextureDescriptor > &);
		bool _OnGpuMemoryBindingChanged (const Message< ModuleMsg::OnGpuMemoryBindingChanged > &);

	private:
		bool _IsCreated () const;

		bool _CreateTexture ();
		bool _CreateView ();
		void _DestroyAll ();

		static vk::VkImageType  _GetImageType (ETexture::type type);
	};


}	// PlatformVK
}	// Engine

#endif	// GRAPHICS_API_VULKAN
