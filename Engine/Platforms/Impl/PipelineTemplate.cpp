// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'
/*
	This is default implementation, override this module to create your own shader manager.

	This is a single thread module!
*/

#include "Engine/Platforms/Public/GPU/Pipeline.h"
#include "Engine/Platforms/Public/GPU/Context.h"
#include "Engine/Platforms/Public/GPU/Thread.h"

#ifdef GRAPHICS_API_VULKAN
#include "Engine/Platforms/Vulkan/110/Vk1Messages.h"
#include "Engine/Platforms/Vulkan/VulkanObjectsConstructor.h"
#endif

#ifdef GRAPHICS_API_OPENGL
#include "Engine/Platforms/OpenGL/450/GL4Messages.h"
#include "Engine/Platforms/OpenGL/450/GL4Enums.h"
#include "Engine/Platforms/OpenGL/OpenGLObjectsConstructor.h"
#endif

#ifdef COMPUTE_API_OPENCL
#include "Engine/Platforms/OpenCL/120/CL1Messages.h"
#include "Engine/Platforms/OpenCL/OpenCLObjectsConstructor.h"
#endif

#ifdef GRAPHICS_API_DIRECTX
#include "Engine/Platforms/DirectX/Impl/DX11Messages.h"
#include "Engine/Platforms/DirectX/DirectXObjectsConstructor.h"
#endif

#ifdef GRAPHICS_API_SOFT
#include "Engine/Platforms/Soft/Impl/SWMessages.h"
#include "Engine/Platforms/Soft/SoftRendererObjectsConstructor.h"
#endif


namespace Engine
{
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
											GpuMsg::CreateGraphicsPipelineDescription,
											GpuMsg::CreateComputePipelineDescription,
											GpuMsg::CreateGraphicsPipeline,
											GpuMsg::CreateComputePipeline,
											GpuMsg::GetPipelineTemplateInfo,
											GpuMsg::GetPipelineLayoutDescription
										>;
		using SupportedEvents_t		= MessageListFrom< 
											ModuleMsg::Delete
										>;

		using Vk1Shaders_t			= StaticArray< /*VkShaderModule*/ ulong, EShader::_Count >;
		using GL4Shaders_t			= StaticArray< /*GLuint program*/ uint, EShader::_Count >;
		using CL1Shaders_t			= StaticArray< /*cl_program*/ void*, EShader::_Count >;		// compute only

		using SWInvoke_t			= PipelineTemplateDescription::ShaderSource::SWInvoke_t;
		using SWShaders_t			= StaticArray< SWInvoke_t, EShader::_Count >;

		using ShaderFormats_t		= FixedSizeArray< EShaderLangFormat::type, 16 >;
		
		using VkDeviceMsgList_t		= MessageListFrom< GpuMsg::GetDeviceInfo, GpuMsg::GetVkDeviceInfo >;
		using GLDeviceMsgList_t		= MessageListFrom< GpuMsg::GetDeviceInfo, GpuMsg::GetGLDeviceInfo >;
		using CLDeviceMsgList_t		= MessageListFrom< GpuMsg::GetDeviceInfo, GpuMsg::GetCLDeviceInfo >;
		using SWDeviceMsgList_t		= MessageListFrom< GpuMsg::GetDeviceInfo, GpuMsg::GetSWDeviceInfo >;
		using DeviceEventList_t		= MessageListFrom< GpuMsg::DeviceCreated, GpuMsg::DeviceBeforeDestroy >;


		struct VulkanShaders
		{
			Vk1Shaders_t	shaders;
			void *			device			= null;
			uint			version			= 0;
		};

		struct OpenGLShaders
		{
			GL4Shaders_t	shaders;
			uint			version			= 0;
			bool			isInitialized	= false;
		};

		struct OpenCLShaders
		{
			CL1Shaders_t	shaders;
			void *			device			= null;
			void *			context			= null;
			uint			version			= 0;
		};

		struct SWRendererShaders
		{
			SWShaders_t		shaders;
			uint			version			= 0;
			bool			isInitialized	= false;
		};
		

	// constants
	private:
		static const TypeIdList		_eventTypes;


	// variables
	private:
		PipelineTemplateDescription		_descr;

		VulkanShaders					_vkData;
		OpenGLShaders					_glData;
		OpenCLShaders					_clData;
		SWRendererShaders				_swData;


	// methods
	public:
		PipelineTemplate (UntypedID_t, GlobalSystemsRef gs, const CreateInfo::PipelineTemplate &ci);
		~PipelineTemplate ();


	// message handlers
	private:
		bool _Delete (const ModuleMsg::Delete &);
		bool _CreateGraphicsPipelineDescription (const GpuMsg::CreateGraphicsPipelineDescription &);
		bool _CreateComputePipelineDescription (const GpuMsg::CreateComputePipelineDescription &);
		bool _CreateGraphicsPipeline (const GpuMsg::CreateGraphicsPipeline &);
		bool _CreateComputePipeline (const GpuMsg::CreateComputePipeline &);
		bool _GetPipelineTemplateInfo (const GpuMsg::GetPipelineTemplateInfo &);
		bool _GetPipelineLayoutDescription (const GpuMsg::GetPipelineLayoutDescription &);
		

#	ifdef GRAPHICS_API_VULKAN
		bool _GetVkShaderModuleIDs (const GpuMsg::GetVkShaderModuleIDs &);
		bool _VkDeviceBeforeDestroy (const GpuMsg::DeviceBeforeDestroy &);
		bool _AttachToVulkanDevice ();
		bool _CreateVulkanShaders ();
		bool _DeleteVulkanShaders ();
		bool _HasVulkanShaders () const;
#	endif
		
#	ifdef GRAPHICS_API_OPENGL
		bool _GetGLShaderModuleIDs (const GpuMsg::GetGLShaderModuleIDs &);
		bool _GLDeviceBeforeDestroy (const GpuMsg::DeviceBeforeDestroy &);
		bool _AttachToOpenGLDevice ();
		bool _CreateOpenGLShaders ();
		bool _DeleteOpenGLShaders ();
		bool _HasOpenGLShaders () const;
#	endif
		
#	ifdef COMPUTE_API_OPENCL
		bool _GetCLShaderModuleIDs (const GpuMsg::GetCLShaderModuleIDs &);
		bool _CLDeviceBeforeDestroy (const GpuMsg::DeviceBeforeDestroy &);
		bool _AttachToOpenCLDevice ();
		bool _CreateOpenCLShaders ();
		bool _DeleteOpenCLShaders ();
		bool _HasOpenCLShaders () const;
#	endif
		
#	ifdef GRAPHICS_API_SOFT
		bool _GetSWShaderModuleIDs (const GpuMsg::GetSWShaderModuleIDs &);
		bool _SWDeviceBeforeDestroy (const GpuMsg::DeviceBeforeDestroy &);
		bool _AttachToSWRendererDevice ();
		bool _CreateSWRendererShaders ();
		bool _DeleteSWRendererShaders ();
		bool _HasSWRendererShaders () const;
#	endif

		bool _GetGraphicsPipelineDescription (const VertexInputState &vertexInput, EPrimitive::type topology,
											 OUT GraphicsPipelineDescription &result) const;

		bool _GetComputePipelineDescription (OUT ComputePipelineDescription &result) const;
	};
//-----------------------------------------------------------------------------


	
	const TypeIdList	PipelineTemplate::_eventTypes{ UninitializedT< SupportedEvents_t >() };
	
/*
=================================================
	constructor
=================================================
*/
	PipelineTemplate::PipelineTemplate (UntypedID_t id, GlobalSystemsRef gs, const CreateInfo::PipelineTemplate &ci) :
		Module( gs, ModuleConfig{ id, 1 }, &_eventTypes ),
		_descr( ci.descr )
	{
		SetDebugName( "PipelineTemplate" );

		_SubscribeOnMsg( this, &PipelineTemplate::_Delete );

#		ifdef GRAPHICS_API_VULKAN
		_SubscribeOnMsg( this, &PipelineTemplate::_GetVkShaderModuleIDs );
#		endif
		
#		ifdef GRAPHICS_API_OPENGL
		_SubscribeOnMsg( this, &PipelineTemplate::_GetGLShaderModuleIDs );
#		endif
		
#		ifdef COMPUTE_API_OPENCL
		_SubscribeOnMsg( this, &PipelineTemplate::_GetCLShaderModuleIDs );
#		endif
		
#		ifdef GRAPHICS_API_SOFT
		_SubscribeOnMsg( this, &PipelineTemplate::_GetSWShaderModuleIDs );
#		endif

		_SubscribeOnMsg( this, &PipelineTemplate::_CreateGraphicsPipelineDescription );
		_SubscribeOnMsg( this, &PipelineTemplate::_CreateComputePipelineDescription );
		_SubscribeOnMsg( this, &PipelineTemplate::_CreateGraphicsPipeline );
		_SubscribeOnMsg( this, &PipelineTemplate::_CreateComputePipeline );
		_SubscribeOnMsg( this, &PipelineTemplate::_GetPipelineTemplateInfo );
		_SubscribeOnMsg( this, &PipelineTemplate::_GetPipelineLayoutDescription );

		ASSERT( _ValidateMsgSubscriptions< SupportedMessages_t >() );
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
		ASSERT( not _HasOpenCLShaders() );
		ASSERT( not _HasSWRendererShaders() );
	}

/*
=================================================
	_Delete
=================================================
*/
	bool PipelineTemplate::_Delete (const ModuleMsg::Delete &msg)
	{
#	ifdef GRAPHICS_API_VULKAN
		if ( _vkData.device )
			_DeleteVulkanShaders();
#	endif
		
#	ifdef GRAPHICS_API_OPENGL
		if ( _glData.isInitialized )
			_DeleteOpenGLShaders();
#	endif
		
#	ifdef COMPUTE_API_OPENCL
		if ( _clData.device )
			_DeleteOpenCLShaders();
#	endif
		
#	ifdef GRAPHICS_API_SOFT
		if ( _swData.isInitialized )
			_DeleteSWRendererShaders();
#	endif

		_vkData	= Uninitialized;
		_glData	= Uninitialized;
		_clData	= Uninitialized;
		_swData	= Uninitialized;

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
	
/*
=================================================
	_HasOpenCLShaders
=================================================
*/
	bool PipelineTemplate::_HasOpenCLShaders () const
	{
		if ( _clData.shaders[ EShader::Compute ] )
			return true;

		return false;
	}
	
/*
=================================================
	_HasSWRendererShaders
=================================================
*/
	bool PipelineTemplate::_HasSWRendererShaders () const
	{
		if ( _swData.shaders[ EShader::Compute ] )
			return true;

		return false;
	}
	
/*
=================================================
	_GetPipelineTemplateInfo
=================================================
*/
	bool PipelineTemplate::_GetPipelineTemplateInfo (const GpuMsg::GetPipelineTemplateInfo &msg)
	{
		GpuMsg::GetPipelineTemplateInfo::Info	info;

		FOR( i, _descr.shaders )
		{
			auto&	sh	= _descr.shaders[i];

			if ( not sh.Empty() )
				info.shaders |= EShader::type(i);
		}

		msg.result.Set( info );
		return true;
	}
		
/*
=================================================
	_GetPipelineLayoutDescription
=================================================
*/
	bool PipelineTemplate::_GetPipelineLayoutDescription (const GpuMsg::GetPipelineLayoutDescription &msg)
	{
		msg.result.Set( _descr.layout );
		return true;
	}
//-----------------------------------------------------------------------------


#ifdef GRAPHICS_API_VULKAN
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
		dev = GlobalSystems()->parallelThread->GetModuleByMsgEvent< VkDeviceMsgList_t, DeviceEventList_t >();

		if ( not dev )
			return false;

		GpuMsg::GetVkDeviceInfo		req_dev;
		dev->Send( req_dev );

		if ( not req_dev.result or
			 req_dev.result->logicalDevice == VK_NULL_HANDLE )
		{
			return false;
		}
		
		GpuMsg::GetComputeSettings	req_settings;
		dev->Send( req_settings );

		dev->Subscribe( this, &PipelineTemplate::_VkDeviceBeforeDestroy );

		_vkData.device	= req_dev.result->logicalDevice;
		_vkData.version	= req_settings.result->GetAPIVersion();
		return true;
	}
	
/*
=================================================
	_VkDeviceBeforeDestroy
=================================================
*/
	bool PipelineTemplate::_VkDeviceBeforeDestroy (const GpuMsg::DeviceBeforeDestroy &)
	{
		ModulePtr	dev;
		dev = GlobalSystems()->parallelThread->GetModuleByMsgEvent< VkDeviceMsgList_t, DeviceEventList_t >();

		if ( dev )
			dev->UnsubscribeAll( this );

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

		ShaderFormats_t		formats;

		if ( _vkData.version >= 110 )
			formats << (EShaderLangFormat::Vulkan_110 | EShaderLangFormat::SPIRV);

		if ( _vkData.version >= 100 )
			formats << (EShaderLangFormat::Vulkan_100 | EShaderLangFormat::SPIRV);

		FOR( i, _vkData.shaders )
		{
			if ( not _descr.supportedShaders[EShader::type(i)] )
				continue;
			

			// find existing shader source
			ArrayCRef<uint>			data;
			EShaderLangFormat::type	fmt;

			for (usize j = 0; j < formats.Count() and data.Empty(); ++j)
			{
				fmt  = formats[j];
				data = _descr.shaders[i].GetSpirv( fmt );
			}
			CHECK_ERR( not data.Empty() );


			// create shader module
			VkShaderModuleCreateInfo	shader_info = {};
			shader_info.sType		= VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
			shader_info.codeSize	= usize(data.Size());
			shader_info.pCode		= Cast<vk::uint32_t const*>( data.ptr() );

			VkShaderModule	shader = {};
			VK_CHECK( vkCreateShaderModule( VkDevice(_vkData.device), &shader_info, null, OUT &shader ) );

			_vkData.shaders[i] = ReferenceCast<ulong>(shader);
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
				vkDestroyShaderModule( VkDevice(_vkData.device), ReferenceCast<VkShaderModule>(_vkData.shaders[i]), null );
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
	bool PipelineTemplate::_GetVkShaderModuleIDs (const GpuMsg::GetVkShaderModuleIDs &msg)
	{
		if ( not _HasVulkanShaders() )
		{
			CHECK_ERR( _AttachToVulkanDevice() );
			CHECK_ERR( _CreateVulkanShaders() );
			CHECK_ERR( _HasVulkanShaders() );
		}

		using Shaders_t = GpuMsg::GetVkShaderModuleIDs::Shaders_t;

		Shaders_t	result;

		FOR( i, _vkData.shaders )
		{
			if ( _vkData.shaders[i] == 0 )
				continue;

			Shaders_t::Value_t	module;

			module.type		= EShader::type(i);
			module.id		= ReferenceCast<vk::VkShaderModule>( _vkData.shaders[i] );
			module.entry	= "main";

			result.PushBack( RVREF(module) );
		}

		msg.result.Set( RVREF(result) );
		return true;
	}
#endif	// GRAPHICS_API_VULKAN
//-----------------------------------------------------------------------------
	

#ifdef GRAPHICS_API_OPENGL
/*
=================================================
	_AttachToOpenGLDevice
=================================================
*/
	bool PipelineTemplate::_AttachToOpenGLDevice ()
	{
		if ( _glData.isInitialized )
			return true;
		
		ModulePtr	dev;
		dev = GlobalSystems()->parallelThread->GetModuleByMsgEvent< GLDeviceMsgList_t, DeviceEventList_t >();

		if ( not dev )
			return false;
		
		GpuMsg::GetComputeSettings	req_settings;
		dev->Send( req_settings );

		dev->Subscribe( this, &PipelineTemplate::_GLDeviceBeforeDestroy );

		_glData.version			= req_settings.result->GetAPIVersion();
		_glData.isInitialized	= true;
		return true;
	}

/*
=================================================
	_GLDeviceBeforeDestroy
=================================================
*/
	bool PipelineTemplate::_GLDeviceBeforeDestroy (const GpuMsg::DeviceBeforeDestroy &)
	{
		ModulePtr	dev;
		dev = GlobalSystems()->parallelThread->GetModuleByMsgEvent< GLDeviceMsgList_t, DeviceEventList_t >();

		if ( dev )
			dev->UnsubscribeAll( this );

		if ( _glData.isInitialized )
			_DeleteOpenGLShaders();

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
		GL_CALL( glGetProgramiv( prog, GL_LINK_STATUS, OUT &link_status ) );
		
		const bool	linked = ( link_status == GL_TRUE );

		GLint	log_size = 0;
		GL_CALL( glGetProgramiv( prog, GL_INFO_LOG_LENGTH, OUT &log_size ) );

		if ( log_size > 5 )
		{
			String	info;
			info.Reserve( log_size + 1 );

			GL_CALL( glGetProgramInfoLog( prog, log_size, null, OUT info.ptr() ) );
			info.CalculateLength();
			
			LOG( "OpenGL Program compilation message: "_str << info, linked ? ELog::Debug : ELog::Error );
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
		
		ShaderFormats_t		formats;

		// TODO: SPIRV
		//if ( _glData.version >= 460 )
		//	formats << (EShaderLangFormat::OpenGL_460 | EShaderLangFormat::SPIRV);
		
		//if ( _glData.version >= 450 )
		//	formats << (EShaderLangFormat::OpenGL_450 | EShaderLangFormat::SPIRV);
		
		if ( _glData.version >= 460 )
			formats << (EShaderLangFormat::OpenGL_460 | EShaderLangFormat::HighLevel);
		
		if ( _glData.version >= 450 )
			formats << (EShaderLangFormat::OpenGL_450 | EShaderLangFormat::HighLevel);


		FOR( i, _glData.shaders )
		{
			if ( not _descr.supportedShaders[EShader::type(i)] )
				continue;
			
			// find existing shader source
			StringCRef				src;
			EShaderLangFormat::type	fmt;

			for (usize j = 0; j < formats.Count() and src.Empty(); ++j)
			{
				fmt  = formats[j];
				src = _descr.shaders[i].GetString( fmt );
			}
			CHECK_ERR( not src.Empty() );


			// create shader program
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
	bool PipelineTemplate::_GetGLShaderModuleIDs (const GpuMsg::GetGLShaderModuleIDs &msg)
	{
		if ( not _HasOpenGLShaders() )
		{
			CHECK_ERR( _AttachToOpenGLDevice() );
			CHECK_ERR( _CreateOpenGLShaders() );
			CHECK_ERR( _HasOpenGLShaders() );
		}
		
		using Shaders_t = GpuMsg::GetGLShaderModuleIDs::Shaders_t;

		Shaders_t	result;

		FOR( i, _glData.shaders )
		{
			if ( _glData.shaders[i] == 0 )
				continue;

			Shaders_t::Value_t	module;

			module.type		= EShader::type(i);
			module.id		= _glData.shaders[i];
			module.entry	= "main";

			result.PushBack( RVREF(module) );
		}

		msg.result.Set( RVREF(result) );
		return true;
	}
#endif	// GRAPHICS_API_OPENGL
//-----------------------------------------------------------------------------

	
#ifdef COMPUTE_API_OPENCL
/*
=================================================
	_AttachToOpenCLDevice
=================================================
*/
	bool PipelineTemplate::_AttachToOpenCLDevice ()
	{
		if ( _clData.device )
			return true;

		ModulePtr	dev;
		dev = GlobalSystems()->parallelThread->GetModuleByMsgEvent< CLDeviceMsgList_t, DeviceEventList_t >();

		if ( not dev )
			return false;

		GpuMsg::GetCLDeviceInfo		req_dev;
		GpuMsg::GetComputeSettings	req_settings;
		dev->Send( req_dev );
		dev->Send( req_settings );

		dev->Subscribe( this, &PipelineTemplate::_CLDeviceBeforeDestroy );

		if ( not req_dev.result.IsDefined()	or
			 req_dev.result->device == null	or
			 req_dev.result->context == null )
		{
			return false;
		}

		_clData.device	= req_dev.result->device;
		_clData.context	= req_dev.result->context;
		_clData.version	= req_settings.result->GetAPIVersion();
		return true;
	}
	
/*
=================================================
	_CLDeviceBeforeDestroy
=================================================
*/
	bool PipelineTemplate::_CLDeviceBeforeDestroy (const GpuMsg::DeviceBeforeDestroy &)
	{
		ModulePtr	dev;
		dev = GlobalSystems()->parallelThread->GetModuleByMsgEvent< CLDeviceMsgList_t, DeviceEventList_t >();

		if ( dev )
			dev->UnsubscribeAll( this );

		if ( _clData.device )
		{
			_DeleteOpenCLShaders();

			_clData.device	= null;
			_clData.context	= null;
		}
		return true;
	}
	
/*
=================================================
	_CreateOpenCLShaders
=================================================
*/
	bool PipelineTemplate::_CreateOpenCLShaders ()
	{
		using namespace cl;
		
		CHECK_ERR( _clData.device and _clData.context );

		_DeleteOpenCLShaders();
		CHECK_ERR( _descr.supportedShaders[EShader::Compute] );
		

		ShaderFormats_t		formats;
		{
			// TODO: SPIRV
			//if ( _clData.version >= 210 )
			//	formats << (EShaderLangFormat::OpenCL_210 | EShaderLangFormat::SPIRV);
			
			// TODO: binary
			//if ( _clData.version >= 210 )
			//	formats << (EShaderLangFormat::OpenCL_210 | EShaderLangFormat::CL_Asm);

			if ( _clData.version >= 210 )
				formats << (EShaderLangFormat::OpenCL_210 | EShaderLangFormat::HighLevel);
			
			//if ( _clData.version >= 120 )
			//	formats << (EShaderLangFormat::OpenCL_120 | EShaderLangFormat::CL_Asm);

			if ( _clData.version >= 120 )
				formats << (EShaderLangFormat::OpenCL_120 | EShaderLangFormat::HighLevel);
		}


		// find existing shader source
		StringCRef				data;
		EShaderLangFormat::type	fmt;

		for (usize j = 0; j < formats.Count() and data.Empty(); ++j)
		{
			fmt  = formats[j];
			data = _descr.shaders[EShader::Compute].GetString( fmt );
		}
		CHECK_ERR( not data.Empty() );


		// compile program
		const cl_device_id	devices[]	= { cl_device_id(_clData.device) };
		usize				log_size	= 0;
		cl_int				cl_err		= 0;
		cl_program			prog_id		= null;
		const char*			src			= data.cstr();
		static const char	options[]	=				// TODO: add options to description
			" -cl-single-precision-constant"
			" -cl-mad-enable"
			//" -cl-denorms-are-zero"
			//" -cl-finite-math-only"
			" -cl-std=CL1.2"
			;
		
		CL_CALL( (prog_id = clCreateProgramWithSource(
								cl_context(_clData.context),
								1,
								&src,
								null,
								OUT &cl_err )), cl_err );
		CHECK_ERR( prog_id != null );

		cl_err = clBuildProgram( prog_id, cl_uint(CountOf( devices )), devices, options, null, null );
		const bool compiled = (cl_err == CL_SUCCESS);
		
		CL_CALL( clGetProgramBuildInfo( prog_id, devices[0], CL_PROGRAM_BUILD_LOG, 0, null, OUT &log_size ) );
		
		if ( log_size > 5 )
		{
			String	info;
			info.Reserve( log_size + 1 );

			CL_CALL( clGetProgramBuildInfo( prog_id, devices[0], CL_PROGRAM_BUILD_LOG, log_size, OUT info.ptr(), null ) );
			info.CalculateLength();
		
			LOG( "OpenCL compute program build message:\n"_str << info, compiled ? ELog::Debug : ELog::Warning );
		}
		
		if ( not compiled ) {
			CL_CALL( clReleaseProgram( prog_id ) );
			return false;
		}

		_clData.shaders[EShader::Compute] = prog_id;
		return true;
	}
	
/*
=================================================
	_DeleteOpenCLShaders
=================================================
*/
	bool PipelineTemplate::_DeleteOpenCLShaders ()
	{
		using namespace cl;

		FOR( i, _clData.shaders )
		{
			if ( _clData.shaders[i] )
			{
				CL_CALL( clReleaseProgram( cl_program(_clData.shaders[i]) ) );
				_clData.shaders[i] = null;
			}
		}
		return true;
	}

/*
=================================================
	_GetCLShaderModuleIDs
=================================================
*/
	bool PipelineTemplate::_GetCLShaderModuleIDs (const GpuMsg::GetCLShaderModuleIDs &msg)
	{
		if ( not _HasOpenCLShaders() )
		{
			CHECK_ERR( _AttachToOpenCLDevice() );
			CHECK_ERR( _CreateOpenCLShaders() );
			CHECK_ERR( _HasOpenCLShaders() );
		}
		
		using Shaders_t = GpuMsg::GetCLShaderModuleIDs::Shaders_t;

		Shaders_t	result;

		FOR( i, _clData.shaders )
		{
			if ( _clData.shaders[i] == 0 )
				continue;

			Shaders_t::Value_t	module;

			module.type		= EShader::type(i);
			module.id		= cl::cl_program(_clData.shaders[i]);
			module.entry	= "Main";	// Intel compiler generate error for 'main' kernel

			result.PushBack( RVREF(module) );
		}

		msg.result.Set( RVREF(result) );
		return true;
	}
#endif	// COMPUTE_API_OPENCL
//-----------------------------------------------------------------------------
	

#ifdef GRAPHICS_API_SOFT
	
/*
=================================================
	_AttachToSWRendererDevice
=================================================
*/
	bool PipelineTemplate::_AttachToSWRendererDevice ()
	{
		if ( _swData.isInitialized )
			return true;

		ModulePtr	dev;
		dev = GlobalSystems()->parallelThread->GetModuleByMsgEvent< SWDeviceMsgList_t, DeviceEventList_t >();

		if ( not dev )
			return false;
		
		GpuMsg::GetComputeSettings	req_settings;
		dev->Send( req_settings );

		dev->Subscribe( this, &PipelineTemplate::_SWDeviceBeforeDestroy );

		_swData.isInitialized	= true;
		_swData.version			= req_settings.result->GetAPIVersion();
		return true;
	}
	
/*
=================================================
	_CreateSWRendererShaders
=================================================
*/
	bool PipelineTemplate::_CreateSWRendererShaders ()
	{
		_DeleteSWRendererShaders();
		
		CHECK_ERR( _descr.supportedShaders[EShader::Compute] );
		
		SWInvoke_t	func = _descr.shaders[EShader::Compute].GetInvocable( EShaderLangFormat::Software_100 | EShaderLangFormat::CPP_Invocable );
		CHECK_ERR( func );
		
		_swData.shaders[EShader::Compute] = func;
		return true;
	}

/*
=================================================
	_DeleteSWRendererShaders
=================================================
*/
	bool PipelineTemplate::_DeleteSWRendererShaders ()
	{
		_swData.shaders.Clear();
		return true;
	}

/*
=================================================
	_SWDeviceBeforeDestroy
=================================================
*/
	bool PipelineTemplate::_SWDeviceBeforeDestroy (const GpuMsg::DeviceBeforeDestroy &)
	{
		ModulePtr	dev;
		dev = GlobalSystems()->parallelThread->GetModuleByMsgEvent< SWDeviceMsgList_t, DeviceEventList_t >();

		if ( dev )
			dev->UnsubscribeAll( this );

		if ( _swData.isInitialized )
			_DeleteSWRendererShaders();

		_swData.isInitialized = false;
		return true;
	}

/*
=================================================
	_GetSWShaderModuleIDs
=================================================
*/
	bool PipelineTemplate::_GetSWShaderModuleIDs (const GpuMsg::GetSWShaderModuleIDs &msg)
	{
		if ( not _HasSWRendererShaders() )
		{
			CHECK_ERR( _AttachToSWRendererDevice() );
			CHECK_ERR( _CreateSWRendererShaders() );
			CHECK_ERR( _HasSWRendererShaders() );
		}

		using Shaders_t = GpuMsg::GetSWShaderModuleIDs::Shaders_t;

		Shaders_t	result;

		FOR( i, _swData.shaders )
		{
			if ( _swData.shaders[i] == 0 )
				continue;

			Shaders_t::Value_t	module;

			module.type		= EShader::type(i);
			module.func		= _swData.shaders[i];

			result.PushBack( RVREF(module) );
		}

		msg.result.Set( RVREF(result) );
		return true;
	}
#endif	// GRAPHICS_API_SOFT
//-----------------------------------------------------------------------------

/*
=================================================
	_GetGraphicsPipelineDescription
=================================================
*/
	bool PipelineTemplate::_GetGraphicsPipelineDescription (const VertexInputState &vertexInput, EPrimitive::type topology,
															OUT GraphicsPipelineDescription &result) const
	{
		// copy from template
		result.dynamicStates		= _descr.dynamicStates;
		result.fragOutput			= _descr.fragOutput;
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
	_GetComputePipelineDescription
=================================================
*/
	bool PipelineTemplate::_GetComputePipelineDescription (OUT ComputePipelineDescription &result) const
	{
		CHECK_ERR( _descr.supportedShaders[ EShader::Compute ] );

		result.localGroupSize	= _descr.localGroupSize;

		return true;
	}

/*
=================================================
	_CreateGraphicsPipelineDescription
=================================================
*/
	bool PipelineTemplate::_CreateGraphicsPipelineDescription (const GpuMsg::CreateGraphicsPipelineDescription &msg)
	{
		GraphicsPipelineDescription	result;
		CHECK_ERR( _GetGraphicsPipelineDescription( msg.vertexInput, msg.topology, OUT result ) );

		msg.result.Set({ result, _descr.layout });
		return true;
	}
	
/*
=================================================
	_CreateComputePipelineDescription
=================================================
*/
	bool PipelineTemplate::_CreateComputePipelineDescription (const GpuMsg::CreateComputePipelineDescription &msg)
	{
		ComputePipelineDescription	result;
		CHECK_ERR( _GetComputePipelineDescription( OUT result ) );

		CHECK_ERR( not msg.localGroupSize.IsDefined() );	// TODO
		
		//if ( msg.localGroupSize.IsDefined() )
		//	result.localGroupSize = *msg.localGroupSize;

		msg.result.Set({ result, _descr.layout });
		return true;
	}
	
/*
=================================================
	_CreateGraphicsPipeline
=================================================
*/
	bool PipelineTemplate::_CreateGraphicsPipeline (const GpuMsg::CreateGraphicsPipeline &msg)
	{
		GraphicsPipelineDescription	gpp_descr;
		CHECK_ERR( _GetGraphicsPipelineDescription( msg.vertexInput, msg.topology, OUT gpp_descr ) );

		ModulePtr	pipeline;
		CHECK_ERR( GlobalSystems()->modulesFactory->Create(
					msg.moduleID,
					GlobalSystems(),
					CreateInfo::GraphicsPipeline{
						msg.gpuThread,
						gpp_descr,
						_descr.layout
					},
					OUT pipeline
		));

		pipeline->Send( ModuleMsg::AttachModule{ this });

		if ( msg.renderPass ) {
			pipeline->Send( ModuleMsg::AttachModule{ msg.renderPass });
		}

		msg.result.Set( pipeline );
		return true;
	}
	
/*
=================================================
	_CreateComputePipeline
=================================================
*/
	bool PipelineTemplate::_CreateComputePipeline (const GpuMsg::CreateComputePipeline &msg)
	{
		ComputePipelineDescription	cpp_descr;
		CHECK_ERR( _GetComputePipelineDescription( OUT cpp_descr ) );
		
		CHECK_ERR( not msg.localGroupSize.IsDefined() );	// TODO
		
		//if ( msg.localGroupSize.IsDefined() )
		//	result.localGroupSize = *msg.localGroupSize;

		ModulePtr	pipeline;
		CHECK_ERR( GlobalSystems()->modulesFactory->Create(
					msg.moduleID,
					GlobalSystems(),
					CreateInfo::ComputePipeline{
						msg.gpuThread,
						cpp_descr,
						_descr.layout
					},
					OUT pipeline
		));
		
		pipeline->Send( ModuleMsg::AttachModule{ this });

		msg.result.Set( pipeline );
		return true;
	}
//-----------------------------------------------------------------------------
	

	
	
#ifdef GRAPHICS_API_VULKAN
	ModulePtr VulkanObjectsConstructor::CreatePipelineTemplate (ModuleMsg::UntypedID_t id, GlobalSystemsRef gs, const CreateInfo::PipelineTemplate &ci)
	{
		return New< PipelineTemplate >( id, gs, ci );
	}
#endif
	
#ifdef GRAPHICS_API_OPENGL
	ModulePtr OpenGLObjectsConstructor::CreatePipelineTemplate (ModuleMsg::UntypedID_t id, GlobalSystemsRef gs, const CreateInfo::PipelineTemplate &ci)
	{
		return New< PipelineTemplate >( id, gs, ci );
	}
#endif
	
#ifdef COMPUTE_API_OPENCL
	ModulePtr OpenCLObjectsConstructor::CreatePipelineTemplate (ModuleMsg::UntypedID_t id, GlobalSystemsRef gs, const CreateInfo::PipelineTemplate &ci)
	{
		return New< PipelineTemplate >( id, gs, ci );
	}
#endif
	
#ifdef GRAPHICS_API_DIRECTX
	ModulePtr DirectXObjectsConstructor::CreatePipelineTemplate (ModuleMsg::UntypedID_t id, GlobalSystemsRef gs, const CreateInfo::PipelineTemplate &ci)
	{
		return New< PipelineTemplate >( id, gs, ci );
	}
#endif
	
#ifdef GRAPHICS_API_SOFT
	ModulePtr SoftRendererObjectsConstructor::CreatePipelineTemplate (ModuleMsg::UntypedID_t id, GlobalSystemsRef gs, const CreateInfo::PipelineTemplate &ci)
	{
		return New< PipelineTemplate >( id, gs, ci );
	}
#endif

}	// Platform
}	// Engine
