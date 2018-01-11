// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'
/*
	This is default implementation, override this module to create your own shader manager.

	This is a single thread module!
*/

#include "Engine/Platforms/Shared/GPU/Pipeline.h"
#include "Engine/Platforms/Shared/GPU/Context.h"
#include "Engine/Platforms/Shared/GPU/Thread.h"

#if defined( GRAPHICS_API_VULKAN )
#include "Engine/Platforms/Vulkan/Impl/Vk1Messages.h"
#include "Engine/Platforms/Vulkan/VulkanObjectsConstructor.h"
#endif

#if defined( GRAPHICS_API_OPENGL )
#include "Engine/Platforms/OpenGL/Impl/GL4Messages.h"
#include "Engine/Platforms/OpenGL/Impl/GL4Enums.h"
#include "Engine/Platforms/OpenGL/OpenGLObjectsConstructor.h"
#endif

#if defined( COMPUTE_API_OPENCL )
#include "Engine/Platforms/OpenCL/Impl/CL2Messages.h"
#include "Engine/Platforms/OpenCL/OpenCLObjectsConstructor.h"
#endif

#if defined( GRAPHICS_API_DIRECTX )
#include "Engine/Platforms/DirectX/Impl/DX11Messages.h"
#include "Engine/Platforms/DirectX/DirectXObjectsConstructor.h"
#endif

#if defined( GRAPHICS_API_SOFT )
#include "Engine/Platforms/Soft/Impl/SWMessages.h"
#include "Engine/Platforms/Soft/SoftRendererObjectsConstructor.h"
#endif


namespace Engine
{
namespace GpuMsg
{

#	if not defined( GRAPHICS_API_VULKAN )
	// default
	struct GetVkShaderModuleIDs
	{};

	struct GetVkDeviceInfo
	{};
#	endif
	
#	if not defined( GRAPHICS_API_OPENGL )
	// default
	struct GetGLShaderModuleIDs
	{};

	struct GetGLDeviceInfo
	{};
#	endif

#	if not defined( COMPUTE_API_OPENCL )
	// default
	struct GetCLShaderModuleIDs
	{};

	struct GetCLDeviceInfo
	{};
#	endif
	
#	if not defined( GRAPHICS_API_SOFT )
	// default
	struct GetSWShaderModuleIDs
	{};

	struct GetSWDeviceInfo
	{};
#	endif
	
#	if not defined( GRAPHICS_API_DIRECTX )
	struct GetDXShaderModuleIDs
	{};

	struct GetDXDeviceInfo
	{};
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
											GpuMsg::GetCLShaderModuleIDs,
											GpuMsg::GetSWShaderModuleIDs,
											GpuMsg::CreateGraphicsPipelineDescriptor,
											GpuMsg::CreateComputePipelineDescriptor,
											GpuMsg::CreateGraphicsPipeline,
											GpuMsg::CreateComputePipeline
										>;
		using SupportedEvents_t		= MessageListFrom< 
											ModuleMsg::Delete
										>;

		using Vk1Shaders			= StaticArray< /*VkShaderModule*/ ulong, EShader::_Count >;
		using GL4Shaders			= StaticArray< /*GLuint program*/ uint, EShader::_Count >;
		using CL2Shaders			= StaticArray< /*cl_program*/ void*, EShader::_Count >;		// compute only
		using SWShaders				= StaticArray< PipelineTemplateDescriptor::ShaderSource::SWInvoke, EShader::_Count >;
		
		using VkDeviceMsgList_t		= MessageListFrom< GpuMsg::GetDeviceInfo, GpuMsg::GetVkDeviceInfo >;
		using GLDeviceMsgList_t		= MessageListFrom< GpuMsg::GetDeviceInfo, GpuMsg::GetGLDeviceInfo >;
		using CLDeviceMsgList_t		= MessageListFrom< GpuMsg::GetDeviceInfo, GpuMsg::GetCLDeviceInfo >;
		using SWDeviceMsgList_t		= MessageListFrom< GpuMsg::GetDeviceInfo, GpuMsg::GetSWDeviceInfo >;
		using DeviceEventList_t		= MessageListFrom< GpuMsg::DeviceCreated, GpuMsg::DeviceBeforeDestroy >;


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

		struct OpenCLShaders
		{
			CL2Shaders		shaders;
			void *			device			= null;
			void *			context			= null;
		};

		struct SWRendererShaders
		{
			SWShaders		shaders;
			bool			isInitialized	= false;
		};
		

	// constants
	private:
		static const TypeIdList		_msgTypes;
		static const TypeIdList		_eventTypes;


	// variables
	private:
		PipelineTemplateDescriptor		_descr;

		VulkanShaders					_vkData;
		OpenGLShaders					_glData;
		OpenCLShaders					_clData;
		SWRendererShaders				_swData;


	// methods
	public:
		PipelineTemplate (GlobalSystemsRef gs, const CreateInfo::PipelineTemplate &ci);
		~PipelineTemplate ();


	// message handlers
	private:
		bool _Delete (const Message< ModuleMsg::Delete > &);
		bool _CreateGraphicsPipelineDescriptor (const Message< GpuMsg::CreateGraphicsPipelineDescriptor > &);
		bool _CreateComputePipelineDescriptor (const Message< GpuMsg::CreateComputePipelineDescriptor > &);
		bool _CreateGraphicsPipeline (const Message< GpuMsg::CreateGraphicsPipeline > &);
		bool _CreateComputePipeline (const Message< GpuMsg::CreateComputePipeline > &);

		bool _GetVkShaderModuleIDs (const Message< GpuMsg::GetVkShaderModuleIDs > &);
		bool _GetGLShaderModuleIDs (const Message< GpuMsg::GetGLShaderModuleIDs > &);
		bool _GetCLShaderModuleIDs (const Message< GpuMsg::GetCLShaderModuleIDs > &);
		bool _GetSWShaderModuleIDs (const Message< GpuMsg::GetSWShaderModuleIDs > &);

		bool _VkDeviceBeforeDestroy (const Message< GpuMsg::DeviceBeforeDestroy > &);
		bool _GLDeviceBeforeDestroy (const Message< GpuMsg::DeviceBeforeDestroy > &);
		bool _CLDeviceBeforeDestroy (const Message< GpuMsg::DeviceBeforeDestroy > &);
		bool _SWDeviceBeforeDestroy (const Message< GpuMsg::DeviceBeforeDestroy > &);

	private:
		bool _AttachToVulkanDevice ();
		bool _CreateVulkanShaders ();
		bool _DeleteVulkanShaders ();
		bool _HasVulkanShaders () const;

		bool _AttachToOpenGLDevice ();
		bool _CreateOpenGLShaders ();
		bool _DeleteOpenGLShaders ();
		bool _HasOpenGLShaders () const;

		bool _AttachToOpenCLDevice ();
		bool _CreateOpenCLShaders ();
		bool _DeleteOpenCLShaders ();
		bool _HasOpenCLShaders () const;

		bool _AttachToSWRendererDevice ();
		bool _DeleteSWRendererShaders ();

		bool _GetGraphicsPipelineDescriptor (const VertexInputState &vertexInput, EPrimitive::type topology,
											 OUT GraphicsPipelineDescriptor &result) const;

		bool _GetComputePipelineDescriptor (OUT ComputePipelineDescriptor &result) const;
	};
//-----------------------------------------------------------------------------


	
	const TypeIdList	PipelineTemplate::_msgTypes{ UninitializedT< SupportedMessages_t >() };
	const TypeIdList	PipelineTemplate::_eventTypes{ UninitializedT< SupportedEvents_t >() };
	
/*
=================================================
	constructor
=================================================
*/
	PipelineTemplate::PipelineTemplate (GlobalSystemsRef gs, const CreateInfo::PipelineTemplate &ci) :
		Module( gs, ModuleConfig{ PipelineTemplateModuleID, 1 }, &_msgTypes, &_eventTypes ),
		_descr( ci.descr )
	{
		SetDebugName( "PipelineTemplate" );

		_SubscribeOnMsg( this, &PipelineTemplate::_Delete );
		_SubscribeOnMsg( this, &PipelineTemplate::_GetVkShaderModuleIDs );
		_SubscribeOnMsg( this, &PipelineTemplate::_GetGLShaderModuleIDs );
		_SubscribeOnMsg( this, &PipelineTemplate::_GetCLShaderModuleIDs );
		_SubscribeOnMsg( this, &PipelineTemplate::_GetSWShaderModuleIDs );
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
		ASSERT( not _HasOpenCLShaders() );
	}

/*
=================================================
	_Delete
=================================================
*/
	bool PipelineTemplate::_Delete (const Message< ModuleMsg::Delete > &msg)
	{
		if ( _vkData.device )
			_DeleteVulkanShaders();

		if ( _glData.isInitialized )
			_DeleteOpenGLShaders();
		
		if ( _clData.device )
			_DeleteOpenCLShaders();

		if ( _swData.isInitialized )
			_DeleteSWRendererShaders();

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
//-----------------------------------------------------------------------------


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
		dev = GlobalSystems()->parallelThread->GetModuleByMsgEvent< VkDeviceMsgList_t, DeviceEventList_t >();

		if ( not dev )
			return false;

		Message< GpuMsg::GetVkDeviceInfo >	req_dev;
		SendTo( dev, req_dev );

		dev->Subscribe( this, &PipelineTemplate::_VkDeviceBeforeDestroy );

		if ( not req_dev->result.IsDefined() or
			 req_dev->result->logicalDevice == VK_NULL_HANDLE )
		{
			return false;
		}

		_vkData.device = req_dev->result->logicalDevice;
		return true;
	}
	
/*
=================================================
	_VkDeviceBeforeDestroy
=================================================
*/
	bool PipelineTemplate::_VkDeviceBeforeDestroy (const Message< GpuMsg::DeviceBeforeDestroy > &)
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
	bool PipelineTemplate::_GetVkShaderModuleIDs (const Message< GpuMsg::GetVkShaderModuleIDs > &msg)
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

		msg->result.Set( RVREF(result) );
		return true;
	}
	
/*
=================================================
	_GetVkLogicDevice
=================================================
*
	bool PipelineTemplate::_GetVkLogicDevice (const Message< GpuMsg::GetVkLogicDevice > &msg)
	{
		ModulePtr	dev;
		dev = GlobalSystems()->parallelThread->GetModuleByMsgList< VkDeviceMsgList_t >();

		if ( dev )
			SendTo( dev, msg );

		return true;
	}*/

#else

	bool PipelineTemplate::_GetVkShaderModuleIDs (const Message< GpuMsg::GetVkShaderModuleIDs > &) {
		return false;
	}

	/*bool PipelineTemplate::_GetVkLogicDevice (const Message< GpuMsg::GetVkLogicDevice > &)
	{
		return false;
	}*/

	bool PipelineTemplate::_DeleteVulkanShaders () {
		return false;
	}

#endif	// GRAPHICS_API_VULKAN
//-----------------------------------------------------------------------------
	

#if defined( GRAPHICS_API_OPENGL )
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

		dev->Subscribe( this, &PipelineTemplate::_GLDeviceBeforeDestroy );

		_glData.isInitialized = true;
		return true;
	}

/*
=================================================
	_GLDeviceBeforeDestroy
=================================================
*/
	bool PipelineTemplate::_GLDeviceBeforeDestroy (const Message< GpuMsg::DeviceBeforeDestroy > &)
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
			
			LOG( ("OpenGL Program compilation message: "_str << info).cstr(), linked ? ELog::Debug : ELog::Error );
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

		msg->result.Set( RVREF(result) );
		return true;
	}

#else
	
	bool PipelineTemplate::_GetGLShaderModuleIDs (const Message< GpuMsg::GetGLShaderModuleIDs > &) {
		return false;
	}

	bool PipelineTemplate::_DeleteOpenGLShaders () {
		return false;
	}
#endif	// GRAPHICS_API_OPENGL
//-----------------------------------------------------------------------------

	
#if defined( COMPUTE_API_OPENCL )
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

		Message< GpuMsg::GetCLDeviceInfo >	req_dev;
		SendTo( dev, req_dev );

		dev->Subscribe( this, &PipelineTemplate::_CLDeviceBeforeDestroy );

		if ( not req_dev->result.IsDefined()	or
			 req_dev->result->device == null	or
			 req_dev->result->context == null )
		{
			return false;
		}

		_clData.device	= req_dev->result->device;
		_clData.context	= req_dev->result->context;
		return true;
	}
	
/*
=================================================
	_CLDeviceBeforeDestroy
=================================================
*/
	bool PipelineTemplate::_CLDeviceBeforeDestroy (const Message< GpuMsg::DeviceBeforeDestroy > &)
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

		StringCRef		data = _descr.shaders[EShader::Compute].GetCL();
		CHECK_ERR( not data.Empty() );
		
		const cl_device_id	devices[]	= { (cl_device_id)_clData.device };
		usize				log_size	= 0;
		cl_int				cl_err		= 0;
		cl_program			prog_id		= null;
		const char*			src			= data.cstr();
		static const char	options[]	=				// TODO: add options to descriptor
			" -cl-single-precision-constant"
			" -cl-mad-enable"
			" -cl-denorms-are-zero"
			" -cl-finite-math-only"
			" -cl-std=CL1.2"
//#			if COMPUTE_API_OPENCL >= 200
//			" -cl-kernel-arg-info"
//#			endif
			;
		
		CL_CALL( (prog_id = clCreateProgramWithSource(
								(cl_context) _clData.context,
								1,
								&src,
								null,
								&cl_err )), cl_err );
		CHECK_ERR( prog_id != null );

		cl_err = clBuildProgram( prog_id, (cl_uint)CountOf(devices), devices, options, null, null );
		const bool compiled = (cl_err == CL_SUCCESS);
		
		CL_CALL( clGetProgramBuildInfo( prog_id, devices[0], CL_PROGRAM_BUILD_LOG, 0, null, &log_size ) );
		
		if ( log_size > 5 )
		{
			String	info;
			info.Reserve( log_size + 128 );
			info << "OpenCL compute program build " << (compiled ? "message" : "error") << ":\n";

			CL_CALL( clGetProgramBuildInfo( prog_id, devices[0], CL_PROGRAM_BUILD_LOG, log_size, info.End(), null ) );
			info.CalculateLength();
		
			LOG( info.cstr(), compiled ? ELog::Debug : ELog::Warning );
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
				CL_CALL( clReleaseProgram( (cl_program)_clData.shaders[i] ) );
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
	bool PipelineTemplate::_GetCLShaderModuleIDs (const Message< GpuMsg::GetCLShaderModuleIDs > &msg)
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
			module.id		= (cl::cl_program) _clData.shaders[i];
			module.entry	= "main";

			result.PushBack( RVREF(module) );
		}

		msg->result.Set( RVREF(result) );
		return true;
	}

#else

	bool PipelineTemplate::_GetCLShaderModuleIDs (const Message< GpuMsg::GetCLShaderModuleIDs > &) {
		return false;
	}

	bool PipelineTemplate::_DeleteOpenCLShaders () {
		return false;
	}
#endif	// COMPUTE_API_OPENCL
//-----------------------------------------------------------------------------
	

#if defined( GRAPHICS_API_SOFT )
	
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

		dev->Subscribe( this, &PipelineTemplate::_SWDeviceBeforeDestroy );

		_swData.isInitialized = true;
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
	bool PipelineTemplate::_SWDeviceBeforeDestroy (const Message< GpuMsg::DeviceBeforeDestroy > &)
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
	bool PipelineTemplate::_GetSWShaderModuleIDs (const Message< GpuMsg::GetSWShaderModuleIDs > &msg)
	{
		CHECK_ERR( _AttachToSWRendererDevice() );
		
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

		msg->result.Set( RVREF(result) );
		return true;
	}
#else

	bool PipelineTemplate::_GetSWShaderModuleIDs (const Message< GpuMsg::GetSWShaderModuleIDs > &) {
		return false;
	}

	bool PipelineTemplate::_DeleteSWRendererShaders () {
		return false;
	}

#endif	// GRAPHICS_API_SOFT
//-----------------------------------------------------------------------------

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
		CHECK_ERR( _descr.supportedShaders[ EShader::Compute ] );

		result.layout			= _descr.layout;
		result.localGroupSize	= _descr.localGroupSize;

		return true;
	}

/*
=================================================
	_CreateGraphicsPipelineDescriptor
=================================================
*/
	bool PipelineTemplate::_CreateGraphicsPipelineDescriptor (const Message< GpuMsg::CreateGraphicsPipelineDescriptor > &msg)
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
	bool PipelineTemplate::_CreateComputePipelineDescriptor (const Message< GpuMsg::CreateComputePipelineDescriptor > &msg)
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
	bool PipelineTemplate::_CreateGraphicsPipeline (const Message< GpuMsg::CreateGraphicsPipeline > &msg)
	{
		GraphicsPipelineDescriptor	gpp_descr;
		CHECK_ERR( _GetGraphicsPipelineDescriptor( msg->vertexInput, msg->topology, OUT gpp_descr ) );

		ModulePtr	pipeline;
		CHECK_ERR( GlobalSystems()->modulesFactory->Create(
					msg->moduleID,
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
	bool PipelineTemplate::_CreateComputePipeline (const Message< GpuMsg::CreateComputePipeline > &msg)
	{
		ComputePipelineDescriptor	cpp_descr;
		CHECK_ERR( _GetComputePipelineDescriptor( OUT cpp_descr ) );
		
		ModulePtr	pipeline;
		CHECK_ERR( GlobalSystems()->modulesFactory->Create(
					msg->moduleID,
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
	ModulePtr VulkanObjectsConstructor::CreatePipelineTemplate (GlobalSystemsRef gs, const CreateInfo::PipelineTemplate &ci)
	{
		return New< PipelineTemplate >( gs, ci );
	}
#endif
	
#if defined( GRAPHICS_API_OPENGL )
	ModulePtr OpenGLObjectsConstructor::CreatePipelineTemplate (GlobalSystemsRef gs, const CreateInfo::PipelineTemplate &ci)
	{
		return New< PipelineTemplate >( gs, ci );
	}
#endif
	
#if defined( COMPUTE_API_OPENCL )
	ModulePtr OpenCLObjectsConstructor::CreatePipelineTemplate (GlobalSystemsRef gs, const CreateInfo::PipelineTemplate &ci)
	{
		return New< PipelineTemplate >( gs, ci );
	}
#endif
	
#if defined( GRAPHICS_API_DIRECTX )
	ModulePtr DirectXObjectsConstructor::CreatePipelineTemplate (GlobalSystemsRef gs, const CreateInfo::PipelineTemplate &ci)
	{
		return New< PipelineTemplate >( gs, ci );
	}
#endif

}	// Platform
}	// Engine
