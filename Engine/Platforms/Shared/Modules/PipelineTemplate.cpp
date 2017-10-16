// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'
/*
	This is default implementation, override this module to create your own shader manager.

	This is a single thread module!
*/

#include "Engine/Platforms/Shared/GPU/Pipeline.h"
#include "Engine/Platforms/Shared/GPU/Context.h"
#include "Engine/Platforms/Shared/GPU/Thread.h"

#if defined( GRAPHICS_API_VULKAN )
#include "Engine/Platforms/Vulkan/Impl/Vk1Messages.h"
#include "Engine/Platforms/Vulkan/VulkanContext.h"
#endif

#if defined( GRAPHICS_API_OPENGL )
#include "Engine/Platforms/OpenGL/Impl/GL4Messages.h"
#include "Engine/Platforms/OpenGL/Impl/GL4Enums.h"
#include "Engine/Platforms/OpenGL/OpenGLContext.h"
#endif


namespace Engine
{
namespace GpuMsg
{

#	if not defined( GRAPHICS_API_VULKAN )
	// default
	struct GetVkShaderModuleIDs
	{
	};
#	endif
	
#	if not defined( GRAPHICS_API_OPENGL )
	// default
	struct GetGLShaderModuleIDs
	{
	};
#	endif

}	// GpuMsg

namespace Platforms
{

	//
	// Pipeline Template
	//
	
	class PipelineTemplate final : public Module
	{
	// types
	private:
		using SupportedMessages_t	= MessageListFrom< 
											ModuleMsg::Delete,
											GpuMsg::GetVkShaderModuleIDs,
											GpuMsg::GetGLShaderModuleIDs,
											GpuMsg::GetVkLogicDevice,
											GpuMsg::CreateGraphicsPipelineDescriptor,
											GpuMsg::CreateComputePipelineDescriptor,
											GpuMsg::CreateGraphicsPipeline,
											GpuMsg::CreateComputePipeline
										>;
		using SupportedEvents_t		= MessageListFrom< 
											ModuleMsg::Delete
										>;

		using Vk1Shaders		= StaticArray< /*VkShaderModule*/ ulong, EShader::_Count >;
		using GL4Shaders		= StaticArray< /*GLuint program*/ uint, EShader::_Count >;
		
		using VkDeviceMsgList	= MessageListFrom< GpuMsg::GetVkLogicDevice/*, GpuMsg::DeviceBeforeDestroy*/ >;


		struct VulkanShaders
		{
			Vk1Shaders		shaders;
			void *			device			= null;
		};

		struct OpenGLShaders
		{
			GL4Shaders		shaders;
			bool			isInitialized	= false;
		};
		

	// constants
	private:
		static const Runtime::VirtualTypeList	_msgTypes;
		static const Runtime::VirtualTypeList	_eventTypes;


	// variables
	private:
		PipelineTemplateDescriptor		_descr;

		VulkanShaders					_vkData;
		OpenGLShaders					_glData;


	// methods
	public:
		PipelineTemplate (const GlobalSystemsRef gs, const CreateInfo::PipelineTemplate &ci);
		~PipelineTemplate ();


	// message handlers
	private:
		bool _Delete (const  Message< ModuleMsg::Delete > &);
		bool _GetVkShaderModuleIDs (const Message< GpuMsg::GetVkShaderModuleIDs > &);
		bool _GetGLShaderModuleIDs (const Message< GpuMsg::GetGLShaderModuleIDs > &);
		bool _GetVkLogicDevice (const Message< GpuMsg::GetVkLogicDevice > &);
		bool _CreateGraphicsPipelineDescriptor (const Message< GpuMsg::CreateGraphicsPipelineDescriptor > &);
		bool _CreateComputePipelineDescriptor (const Message< GpuMsg::CreateComputePipelineDescriptor > &);
		bool _CreateGraphicsPipeline (const  Message< GpuMsg::CreateGraphicsPipeline > &);
		bool _CreateComputePipeline (const  Message< GpuMsg::CreateComputePipeline > &);

		bool _VkDeviceBeforeDestroy (const  Message< GpuMsg::DeviceBeforeDestroy > &);
		bool _GLDeviceBeforeDestroy (const  Message< GpuMsg::DeviceBeforeDestroy > &);

	private:
		bool _AttachToVulkanDevice ();
		bool _CreateVulkanShaders ();
		bool _DeleteVulkanShaders ();
		bool _HasVulkanShaders () const;

		bool _AttachToOpenGLDevice ();
		bool _CreateOpenGLShaders ();
		bool _DeleteOpenGLShaders ();
		bool _HasOpenGLShaders () const;

		bool _GetGraphicsPipelineDescriptor (const VertexInputState &vertexInput, EPrimitive::type topology,
											 OUT GraphicsPipelineDescriptor &result) const;

		bool _GetComputePipelineDescriptor (OUT ComputePipelineDescriptor &result) const;
	};
//-----------------------------------------------------------------------------


	
	const Runtime::VirtualTypeList	PipelineTemplate::_msgTypes{ UninitializedT< SupportedMessages_t >() };
	const Runtime::VirtualTypeList	PipelineTemplate::_eventTypes{ UninitializedT< SupportedEvents_t >() };
	
/*
=================================================
	constructor
=================================================
*/
	PipelineTemplate::PipelineTemplate (const GlobalSystemsRef gs, const CreateInfo::PipelineTemplate &ci) :
		Module( gs, ModuleConfig{ PipelineTemplateModuleID, 1 }, &_msgTypes, &_eventTypes ),
		_descr( ci.descr )
	{
		SetDebugName( "PipelineTemplate" );

		_SubscribeOnMsg( this, &PipelineTemplate::_Delete );
		_SubscribeOnMsg( this, &PipelineTemplate::_GetVkShaderModuleIDs );
		_SubscribeOnMsg( this, &PipelineTemplate::_GetGLShaderModuleIDs );
		_SubscribeOnMsg( this, &PipelineTemplate::_GetVkLogicDevice );
		_SubscribeOnMsg( this, &PipelineTemplate::_CreateGraphicsPipelineDescriptor );
		_SubscribeOnMsg( this, &PipelineTemplate::_CreateComputePipelineDescriptor );
		_SubscribeOnMsg( this, &PipelineTemplate::_CreateGraphicsPipeline );
		_SubscribeOnMsg( this, &PipelineTemplate::_CreateComputePipeline );

		CHECK( _ValidateMsgSubscriptions() );
	}
	
/*
=================================================
	destructor
=================================================
*/
	PipelineTemplate::~PipelineTemplate ()
	{
		ASSERT( not _HasVulkanShaders() );
		ASSERT( not _HasOpenGLShaders() );
	}

/*
=================================================
	_Delete
=================================================
*/
	bool PipelineTemplate::_Delete (const  Message< ModuleMsg::Delete > &msg)
	{
		if ( _vkData.device )
			_DeleteVulkanShaders();

		if ( _glData.isInitialized )
			_DeleteOpenGLShaders();

		_vkData	= Uninitialized;
		_glData	= Uninitialized;

		_descr	= Uninitialized;

		return Module::_Delete_Impl( msg );
	}
	
/*
=================================================
	_HasVulkanShaders
=================================================
*/
	bool PipelineTemplate::_HasVulkanShaders () const
	{
		if ( _vkData.shaders[ EShader::Vertex ] and _vkData.shaders[ EShader::Fragment ] )
			return true;

		if ( _vkData.shaders[ EShader::Compute ] )
			return true;

		return false;
	}
	
/*
=================================================
	_HasOpenGLShaders
=================================================
*/
	bool PipelineTemplate::_HasOpenGLShaders () const
	{
		if ( _glData.shaders[ EShader::Vertex ] and _glData.shaders[ EShader::Fragment ] )
			return true;

		if ( _glData.shaders[ EShader::Compute ] )
			return true;

		return false;
	}


#if defined( GRAPHICS_API_VULKAN )
/*
=================================================
	_AttachToVulkanDevice
=================================================
*/
	bool PipelineTemplate::_AttachToVulkanDevice ()
	{
		if ( _vkData.device )
			return true;

		ModulePtr	dev;
		dev = GlobalSystems()->Get< ParallelThread >()->GetModuleByMsgList< VkDeviceMsgList >();

		if ( not dev )
			return false;

		Message< GpuMsg::GetVkLogicDevice >	req_dev;
		SendTo( dev, req_dev );

		dev->Subscribe( this, &PipelineTemplate::_VkDeviceBeforeDestroy );

		if ( not req_dev->result.IsDefined() )
			return false;

		_vkData.device = req_dev->result.Get();
		return true;
	}
	
/*
=================================================
	_VkDeviceBeforeDestroy
=================================================
*/
	bool PipelineTemplate::_VkDeviceBeforeDestroy (const  Message< GpuMsg::DeviceBeforeDestroy > &msg)
	{
		if ( _vkData.device )
		{
			_DeleteVulkanShaders();

			_vkData.device = null;
		}
		return true;
	}

/*
=================================================
	_CreateVulkanShaders
=================================================
*/
	bool PipelineTemplate::_CreateVulkanShaders ()
	{
		using namespace vk;
		
		CHECK_ERR( _vkData.device );

		_DeleteVulkanShaders();

		FOR( i, _vkData.shaders )
		{
			if ( not _descr.supportedShaders[EShader::type(i)] )
				continue;
			
			ArrayCRef<uint>		data = _descr.shaders[i].GetSPIRV();
			CHECK_ERR( not data.Empty() );

			VkShaderModuleCreateInfo	shader_info = {};
			shader_info.sType		= VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
			shader_info.codeSize	= (usize) data.Size();
			shader_info.pCode		= (vk::uint32_t const*) data.ptr();

			VK_CHECK( vkCreateShaderModule( VkDevice(_vkData.device), &shader_info, null, OUT &_vkData.shaders[i] ) );
		}
		return true;
	}
	
/*
=================================================
	_DeleteVulkanShaders
=================================================
*/
	bool PipelineTemplate::_DeleteVulkanShaders ()
	{
		using namespace vk;

		CHECK_ERR( _vkData.device );

		FOR( i, _vkData.shaders )
		{
			if ( _vkData.shaders[i] )
			{
				vkDestroyShaderModule( VkDevice(_vkData.device), (VkShaderModule)_vkData.shaders[i], null );
				_vkData.shaders[i] = 0;
			}
		}
		return true;
	}

/*
=================================================
	_GetVkShaderModuleIDs
=================================================
*/
	bool PipelineTemplate::_GetVkShaderModuleIDs (const Message< GpuMsg::GetVkShaderModuleIDs > &msg)
	{
		if ( not _HasVulkanShaders() )
		{
			CHECK_ERR( _AttachToVulkanDevice() );
			CHECK_ERR( _CreateVulkanShaders() );
			CHECK_ERR( _HasVulkanShaders() );
		}

		using ShaderModule = GpuMsg::GetVkShaderModuleIDs::ShaderModule;

		Array< ShaderModule >	result;		result.Reserve( _vkData.shaders.Count() );

		FOR( i, _vkData.shaders )
		{
			if ( _vkData.shaders[i] == 0 )
				continue;

			ShaderModule	module;
			module.type		= EShader::type(i);
			module.id		= _vkData.shaders[i];
			module.entry	= "main";

			result.PushBack( RVREF(module) );
		}

		msg->result.Set( RVREF(result) );
		return true;
	}
	
/*
=================================================
	_GetVkLogicDevice
=================================================
*/
	bool PipelineTemplate::_GetVkLogicDevice (const Message< GpuMsg::GetVkLogicDevice > &msg)
	{
		ModulePtr	dev;
		dev = GlobalSystems()->Get< ParallelThread >()->GetModuleByMsgList< VkDeviceMsgList >();

		if ( dev )
			SendTo( dev, msg );

		return true;
	}

#else

	bool PipelineTemplate::_GetVkShaderModuleIDs (const Message< GpuMsg::GetVkShaderModuleIDs > &msg)
	{
		return false;
	}

	bool PipelineTemplate::_GetVkLogicDevice (const Message< GpuMsg::GetVkLogicDevice > &msg)
	{
		return false;
	}

#endif
	

#if defined( GRAPHICS_API_OPENGL )
/*
=================================================
	_AttachToOpenGLDevice
=================================================
*/
	bool PipelineTemplate::_AttachToOpenGLDevice ()
	{
		// TODO
		_glData.isInitialized = true;
		return true;
	}

/*
=================================================
	_GLDeviceBeforeDestroy
=================================================
*/
	bool PipelineTemplate::_GLDeviceBeforeDestroy (const  Message< GpuMsg::DeviceBeforeDestroy > &msg)
	{
		// TODO
		_glData.isInitialized = false;
		return true;
	}

/*
=================================================
	CreateGLProgram
=================================================
*/
	bool CreateGLProgram (OUT gl::GLuint &prog, EShader::type shaderType, const char *src)
	{
		using namespace gl;

		GL_CALL( prog = glCreateShaderProgramv( PlatformGL::GL4Enum( shaderType ), 1, &src ) );

		GLint	link_status = 0;
		GL_CALL( glGetProgramiv( prog, GL_LINK_STATUS, &link_status ) );
		
		const bool	linked = ( link_status == GL_TRUE );

		GLint	log_size = 0;
		GL_CALL( glGetProgramiv( prog, GL_INFO_LOG_LENGTH, &log_size ) );

		if ( log_size > 5 )
		{
			String	info;
			info.Reserve( log_size + 1 );

			GL_CALL( glGetProgramInfoLog( prog, log_size, null, info.ptr() ) );
			info.CalculateLength();
			
			LOG( ("Program compilation message: "_str << info).cstr(), linked ? ELog::Debug : ELog::Error );
		}

		if ( not linked )
		{
			GL_CALL( glDeleteProgram( prog ) );
			prog = 0;
		}

		CHECK_ERR( linked );
		return true;
	}

/*
=================================================
	_CreateOpenGLShaders
=================================================
*/
	bool PipelineTemplate::_CreateOpenGLShaders ()
	{
		CHECK_ERR( _glData.isInitialized );

		_DeleteOpenGLShaders();

		FOR( i, _glData.shaders )
		{
			if ( not _descr.supportedShaders[EShader::type(i)] )
				continue;
			
			StringCRef	src = _descr.shaders[i].GetGLSL();
			CHECK_ERR( not src.Empty() );

			CHECK_ERR( CreateGLProgram( OUT _glData.shaders[i], EShader::type(i), src.cstr() ) );
		}
		return true;
	}
	
/*
=================================================
	_DeleteOpenGLShaders
=================================================
*/
	bool PipelineTemplate::_DeleteOpenGLShaders ()
	{
		using namespace gl;
		
		CHECK_ERR( _glData.isInitialized );

		FOR( i, _glData.shaders )
		{
			if ( _glData.shaders[i] )
			{
				GL_CALL( glDeleteProgram( _glData.shaders[i] ) );
				_glData.shaders[i] = 0;
			}
		}
		return true;
	}

/*
=================================================
	_GetGLShaderModuleIDs
=================================================
*/
	bool PipelineTemplate::_GetGLShaderModuleIDs (const Message< GpuMsg::GetGLShaderModuleIDs > &msg)
	{
		if ( not _HasOpenGLShaders() )
		{
			CHECK_ERR( _AttachToOpenGLDevice() );
			CHECK_ERR( _CreateOpenGLShaders() );
			CHECK_ERR( _HasOpenGLShaders() );
		}
		
		using ShaderModule = GpuMsg::GetGLShaderModuleIDs::ShaderModule;

		Array< ShaderModule >	result;		result.Reserve( _glData.shaders.Count() );

		FOR( i, _glData.shaders )
		{
			if ( _glData.shaders[i] == 0 )
				continue;

			ShaderModule	module;
			module.type		= EShader::type(i);
			module.id		= _glData.shaders[i];
			module.entry	= "main";

			result.PushBack( RVREF(module) );
		}

		msg->result.Set( RVREF(result) );
		return true;
	}

#else
	
/*
=================================================
	_GetGLShaderModuleIDs
=================================================
*/
	bool PipelineTemplate::_GetGLShaderModuleIDs (const Message< GpuMsg::GetGLShaderModuleIDs > &msg)
	{
		return false;
	}
#endif
	
/*
=================================================
	_GetGraphicsPipelineDescriptor
=================================================
*/
	bool PipelineTemplate::_GetGraphicsPipelineDescriptor (const VertexInputState &vertexInput, EPrimitive::type topology,
															OUT GraphicsPipelineDescriptor &result) const
	{
		// copy from template
		result.dynamicStates		= _descr.dynamicStates;
		result.fragOutput			= _descr.fragOutput;
		result.layout				= _descr.layout;
		result.patchControlPoints	= _descr.patchControlPoints;
		result.renderState			= _descr.renderState;
		result.subpass				= _descr.subpass;
		
		// copy from input
		CHECK_ERR( _descr.supportedPrimitives[ topology ] );
		CHECK_ERR( vertexInput.Merge( _descr.attribs, OUT result.vertexInput ) );

		result.renderState.inputAssembly.topology = topology;
		return true;
	}
	
/*
=================================================
	_GetComputePipelineDescriptor
=================================================
*/
	bool PipelineTemplate::_GetComputePipelineDescriptor (OUT ComputePipelineDescriptor &result) const
	{
		result.layout	= _descr.layout;
		result.subpass	= _descr.subpass;

		return true;
	}

/*
=================================================
	_CreateGraphicsPipelineDescriptor
=================================================
*/
	bool PipelineTemplate::_CreateGraphicsPipelineDescriptor (const  Message< GpuMsg::CreateGraphicsPipelineDescriptor > &msg)
	{
		GraphicsPipelineDescriptor	result;
		CHECK_ERR( _GetGraphicsPipelineDescriptor( msg->vertexInput, msg->topology, OUT result ) );

		msg->result.Set( result );
		return true;
	}
	
/*
=================================================
	_CreateComputePipelineDescriptor
=================================================
*/
	bool PipelineTemplate::_CreateComputePipelineDescriptor (const  Message< GpuMsg::CreateComputePipelineDescriptor > &msg)
	{
		ComputePipelineDescriptor	result;
		CHECK_ERR( _GetComputePipelineDescriptor( OUT result ) );

		msg->result.Set( result );
		return true;
	}
	
/*
=================================================
	_CreateGraphicsPipeline
=================================================
*/
	bool PipelineTemplate::_CreateGraphicsPipeline (const  Message< GpuMsg::CreateGraphicsPipeline > &msg)
	{
		GraphicsPipelineDescriptor	gpp_descr;
		CHECK_ERR( _GetGraphicsPipelineDescriptor( msg->vertexInput, msg->topology, OUT gpp_descr ) );

		ModulePtr	pipeline;
		CHECK_ERR( GlobalSystems()->Get< ModulesFactory >()->Create(
					"vk.g-ppln"_OModID,
					GlobalSystems(),
					CreateInfo::GraphicsPipeline{
						msg->gpuThread,
						gpp_descr,
						this,
						msg->renderPass
					},
					OUT pipeline
		));

		msg->result.Set( pipeline );
		return true;
	}
	
/*
=================================================
	_CreateComputePipeline
=================================================
*/
	bool PipelineTemplate::_CreateComputePipeline (const  Message< GpuMsg::CreateComputePipeline > &msg)
	{
		ComputePipelineDescriptor	cpp_descr;
		CHECK_ERR( _GetComputePipelineDescriptor( OUT cpp_descr ) );
		
		ModulePtr	pipeline;
		CHECK_ERR( GlobalSystems()->Get< ModulesFactory >()->Create(
					"vk.c-ppln"_OModID,
					GlobalSystems(),
					CreateInfo::ComputePipeline{
						msg->gpuThread,
						cpp_descr,
						this
					},
					OUT pipeline
		));

		msg->result.Set( pipeline );
		return true;
	}
//-----------------------------------------------------------------------------
	

	
	
#if defined( GRAPHICS_API_VULKAN )
	ModulePtr VulkanContext::_CreatePipelineTemplate (const GlobalSystemsRef gs, const CreateInfo::PipelineTemplate &ci)
	{
		return New< PipelineTemplate >( gs, ci );
	}
#endif
	
#if defined( GRAPHICS_API_OPENGL )
	ModulePtr OpenGLContext::_CreatePipelineTemplate (const GlobalSystemsRef gs, const CreateInfo::PipelineTemplate &ci)
	{
		return New< PipelineTemplate >( gs, ci );
	}
#endif

}	// Platform
}	// Engine
