// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Engine/Platforms/Vulkan/Impl/Vk1ShaderModules.h"
#include "Engine/Platforms/Vulkan/VulkanThread.h"

#if defined( GRAPHICS_API_VULKAN )

namespace Engine
{
namespace PlatformVK
{
	
	const Runtime::VirtualTypeList	Vk1ShaderModules::_msgTypes{ UninitializedT< SupportedMessages_t >() };
	const Runtime::VirtualTypeList	Vk1ShaderModules::_eventTypes{ UninitializedT< SupportedEvents_t >() };

/*
=================================================
	constructor
=================================================
*/
	Vk1ShaderModules::Vk1ShaderModules (const GlobalSystemsRef gs, const CreateInfo::GpuShaderModulesFromFile &ci) :
		Vk1BaseModule( gs, ci.gpuThread, ModuleConfig{ GetStaticID(), ~0u }, &_msgTypes, &_eventTypes )
	{
		SetDebugName( "Vk1ShaderModules" );

		_SubscribeOnMsg( this, &Vk1ShaderModules::_OnModuleAttached_Impl );
		_SubscribeOnMsg( this, &Vk1ShaderModules::_OnModuleDetached_Impl );
		_SubscribeOnMsg( this, &Vk1ShaderModules::_AttachModule_Impl );
		_SubscribeOnMsg( this, &Vk1ShaderModules::_DetachModule_Impl );
		_SubscribeOnMsg( this, &Vk1ShaderModules::_FindModule_Impl );
		_SubscribeOnMsg( this, &Vk1ShaderModules::_ModulesDeepSearch_Impl );
		_SubscribeOnMsg( this, &Vk1ShaderModules::_Link_Impl );
		_SubscribeOnMsg( this, &Vk1ShaderModules::_Compose_Impl );
		_SubscribeOnMsg( this, &Vk1ShaderModules::_Delete );
		_SubscribeOnMsg( this, &Vk1ShaderModules::_OnManagerChanged );
		_SubscribeOnMsg( this, &Vk1ShaderModules::_GpuDeviceBeforeDestory );
		_SubscribeOnMsg( this, &Vk1ShaderModules::_GetGpuShaderModuleIDs );

		CHECK( _ValidateMsgSubscriptions() );

		_AttachSelfToManager( ci.gpuThread, VulkanThread::GetStaticID(), true );

		if ( _LoadShaders( ci.filenames ) )
			CHECK( _SetState( EState::ComposedImmutable ) )
		else
			CHECK( _SetState( EState::ComposingFailed ) );
	}
	
/*
=================================================
	constructor
=================================================
*/
	Vk1ShaderModules::Vk1ShaderModules (const GlobalSystemsRef gs, const CreateInfo::GpuShaderModulesFromBuffer &ci) :
		Vk1BaseModule( gs, ci.gpuThread, ModuleConfig{ GetStaticID(), ~0u }, &_msgTypes, &_eventTypes )
	{
		SetDebugName( "Vk1ShaderModules" );

		_SubscribeOnMsg( this, &Vk1ShaderModules::_OnModuleAttached_Impl );
		_SubscribeOnMsg( this, &Vk1ShaderModules::_OnModuleDetached_Impl );
		_SubscribeOnMsg( this, &Vk1ShaderModules::_AttachModule_Impl );
		_SubscribeOnMsg( this, &Vk1ShaderModules::_DetachModule_Impl );
		_SubscribeOnMsg( this, &Vk1ShaderModules::_FindModule_Impl );
		_SubscribeOnMsg( this, &Vk1ShaderModules::_ModulesDeepSearch_Impl );
		_SubscribeOnMsg( this, &Vk1ShaderModules::_Link_Impl );
		_SubscribeOnMsg( this, &Vk1ShaderModules::_Compose_Impl );
		_SubscribeOnMsg( this, &Vk1ShaderModules::_Delete );
		_SubscribeOnMsg( this, &Vk1ShaderModules::_OnManagerChanged );
		_SubscribeOnMsg( this, &Vk1ShaderModules::_GpuDeviceBeforeDestory );
		_SubscribeOnMsg( this, &Vk1ShaderModules::_GetGpuShaderModuleIDs );

		CHECK( _ValidateMsgSubscriptions() );

		_AttachSelfToManager( ci.gpuThread, VulkanThread::GetStaticID(), true );

		if ( _CompileShaders( ci.shaderData ) )
			CHECK( _SetState( EState::ComposedImmutable ) )
		else
			CHECK( _SetState( EState::ComposingFailed ) );
	}
	
/*
=================================================
	destructor
=================================================
*/
	Vk1ShaderModules::~Vk1ShaderModules ()
	{
		ASSERT( not _IsCreated() );
	}
	
/*
=================================================
	_Delete
=================================================
*/
	bool Vk1ShaderModules::_Delete (const Message< ModuleMsg::Delete > &msg)
	{
		using namespace vk;

		FOR( i, _modules ) {
			vkDestroyShaderModule( GetLogicalDevice(), _modules[i].second, null );
		}
		_modules.Clear();

		return Module::_Delete_Impl( msg );
	}
	
/*
=================================================
	_GetGpuShaderModuleIDs
=================================================
*/
	bool Vk1ShaderModules::_GetGpuShaderModuleIDs (const Message< ModuleMsg::GetGpuShaderModuleIDs > &msg)
	{
		using ShaderModule = ModuleMsg::GetGpuShaderModuleIDs::ShaderModule;

		Array< ShaderModule >	result;		result.Resize( _modules.Count(), false );

		FOR( i, _modules )
		{
			result[i].type	= _modules[i].first;
			result[i].id	= _modules[i].second;
			result[i].entry	= "main";	// TODO
		}

		msg->result.Set( RVREF( result ) );
		return true;
	}

/*
=================================================
	_IsCreated
=================================================
*/
	bool Vk1ShaderModules::_IsCreated () const
	{
		return not _modules.Empty();
	}
	
/*
=================================================
	_CompileShader
=================================================
*/
	bool Vk1ShaderModules::_CompileShader (BinArrayCRef sprivSource, OUT vk::VkShaderModule &shader)
	{
		using namespace vk;

		VkShaderModuleCreateInfo	shader_info = {};
		shader_info.sType		= VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
		shader_info.codeSize	= (usize) sprivSource.Size();
		shader_info.pCode		= (vk::uint32_t*) sprivSource.ptr();

		VK_CHECK( vkCreateShaderModule( GetLogicalDevice(), &shader_info, null, OUT &shader ) );
		return true;
	}
	
/*
=================================================
	_LoadShaders
=================================================
*/
	bool Vk1ShaderModules::_LoadShaders (ArrayCRef< String > filenames)
	{
		Ptr< FileManager >	fm = GlobalSystems()->Get< FileManager >();
		RFilePtr			file;
		BinaryArray			data;

		FOR( i, filenames )
		{
			CHECK_ERR( fm->OpenForRead( filenames[i], OUT file ) );

			data.Resize( (usize) file->Size() );
			file->Read( BinArrayRef(data) );

			vk::VkShaderModule	shader;

			CHECK_ERR( _CompileShader( data, OUT shader ) );

			StringCRef		ext			= FileAddress::GetExtension( filenames[i] );
			EShader::type	shader_type	= EShader::Unknown;

			if ( ext == "frag" )	shader_type = EShader::Fragment;		else
			if ( ext == "vert" )	shader_type = EShader::Vertex;			else
			if ( ext == "cont" )	shader_type = EShader::TessControl;		else
			if ( ext == "eval" )	shader_type = EShader::TessEvaluation;	else
			if ( ext == "geom" )	shader_type = EShader::Geometry;		else
			if ( ext == "comp" )	shader_type = EShader::Compute;

			CHECK_ERR( shader_type != EShader::Unknown and
					   not _modules.IsExist( shader_type ) );

			_modules.Add( shader_type, shader );
		}
		return true;
	}
	
/*
=================================================
	_CompileShaders
=================================================
*/
	bool Vk1ShaderModules::_CompileShaders (const Map< EShader::type, BinaryArray > &shaders)
	{
		FOR( i, shaders )
		{
			vk::VkShaderModule	shader;
			EShader::type		shader_type = shaders[i].first;

			CHECK_ERR( _CompileShader( shaders[i].second, OUT shader ) );
			
			CHECK_ERR( shader_type != EShader::Unknown and
					   not _modules.IsExist( shader_type ) );
			
			_modules.Add( shader_type, shader );
		}
		return true;
	}


}	// PlatformVK
}	// Engine

#endif	// GRAPHICS_API_VULKAN
