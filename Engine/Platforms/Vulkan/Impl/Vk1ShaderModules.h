// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/Platforms/Vulkan/Impl/Vk1BaseModule.h"
#include "Engine/Platforms/Shared/GPU/Pipeline.h"

#if defined( GRAPHICS_API_VULKAN )

namespace Engine
{
namespace PlatformVK
{
	
	//
	// Vulkan Shader Modules
	//

	class Vk1ShaderModules final : public Vk1BaseModule
	{
	// types
	private:
		using SupportedMessages_t	= Vk1BaseModule::SupportedMessages_t::Append< MessageListFrom<
											ModuleMsg::GetGpuShaderModuleIDs
										> >;

		using SupportedEvents_t		= Vk1BaseModule::SupportedEvents_t;


	// constants
	private:
		static const Runtime::VirtualTypeList	_msgTypes;
		static const Runtime::VirtualTypeList	_eventTypes;


	// variables
	private:
		Map< EShader::type, vk::VkShaderModule >	_modules;


	// methods
	public:
		Vk1ShaderModules (const GlobalSystemsRef gs, const CreateInfo::GpuShaderModulesFromFile &ci);
		Vk1ShaderModules (const GlobalSystemsRef gs, const CreateInfo::GpuShaderModulesFromBuffer &ci);
		~Vk1ShaderModules ();

		static OModID::type		GetStaticID ()				{ return "vk1.shaders"_OModID; }


	// message handlers
	private:
		bool _Link (const Message< ModuleMsg::Link > &);
		bool _Compose (const  Message< ModuleMsg::Compose > &);
		bool _Delete (const Message< ModuleMsg::Delete > &);
		bool _GetGpuShaderModuleIDs (const Message< ModuleMsg::GetGpuShaderModuleIDs > &);

	private:
		bool _IsCreated () const;
		bool _CompileShader (BinArrayCRef sprivSource, OUT vk::VkShaderModule &shader);
		bool _LoadShaders (ArrayCRef< String > filenames);
		bool _CompileShaders (const Map< EShader::type, BinaryArray > &shaders);
	};


}	// PlatformVK
}	// Engine

#endif	// GRAPHICS_API_VULKAN
