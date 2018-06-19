// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Core/Config/Engine.Config.h"

#ifdef GRAPHICS_API_OPENGL

#include "Engine/Platforms/OpenGL/OpenGLObjectsConstructor.h"
#include "Engine/Platforms/OpenGL/450/GL4Device.h"
#include "Engine/Platforms/OpenGL/450/GL4SystemFramebuffer.h"
#include "Engine/Platforms/OpenGL/450/GL4FlippedSystemFramebuffer.h"
#include "Engine/Platforms/Public/GPU/RenderPass.h"
#include "Engine/Platforms/Public/GPU/CommandBuffer.h"

namespace Engine
{
namespace PlatformGL
{
	using namespace gl;

/*
=================================================
	constructor
=================================================
*/
	GL4Device::GL4Device (GlobalSystemsRef gs) :
		BaseObject( gs ),
		_colorPixelFormat( EPixelFormat::Unknown ),
		_depthStencilPixelFormat( EPixelFormat::Unknown ),
		_currentImageIndex( 0 ),	_swapchainLength( 1 ),
		_numExtensions( 0 ),		_initialized( false ),
		_frameStarted( false ),		_vulkanCompatibility( true ),
		_debugReportCounter( 0 )
	{
		SetDebugName( "GL4Device" );
	}
	
/*
=================================================
	destructor
=================================================
*/
	GL4Device::~GL4Device ()
	{
		ASSERT( not _initialized );
		
		if ( _debugReportCounter > 0 )
		{
			WARNING( "There are a few warnings, check debug output!" );
		}
	}

/*
=================================================
	Initialize
=================================================
*/
	bool GL4Device::Initialize (const uint2 &surfaceSize, EPixelFormat::type colorFormat, EPixelFormat::type depthStencilFormat, MultiSamples samples)
	{
		CHECK_ERR( GL4_Init() );

		_UpdateProperties();
		
		CHECK_ERR( GL4_GetVersion() >= 450 or IsExtensionSupported("GL_EXT_direct_state_access") );

		_surfaceSize			 = surfaceSize;
		_colorPixelFormat		 = colorFormat;
		_depthStencilPixelFormat = depthStencilFormat;
		_samples				 = samples;

		CHECK_ERR( _CreateRenderPass() );
		CHECK_ERR( _CreateFramebuffer() );

		ModuleUtils::Initialize({ _framebuffer, _renderPass });

		_initialized = true;
		return true;
	}
/*
=================================================
	Deinitialize
=================================================
*/
	void GL4Device::Deinitialize ()
	{
		_surfaceSize			 = Uninitialized;
		_colorPixelFormat		 = Uninitialized;
		_depthStencilPixelFormat = Uninitialized;
		_samples				 = Uninitialized;

		_framebuffer		= null;
		_renderPass			= null;

		_currentImageIndex	= 0;
		_swapchainLength	= 1;

		_numExtensions		= 0;
		_initialized		= false;
		_frameStarted		= false;

		GL4_Delete();
	}
	
/*
=================================================
	InitDebugReport
=================================================
*/
	bool GL4Device::InitDebugReport ()
	{
		DEBUG_ONLY(
			GL_CALL( glDebugMessageCallback( &_StaticDebugCallback, this ) );

			// disable notifications
			GL_CALL( glDebugMessageControl( GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION,
											0, null, GL_FALSE ) );
		)
		return true;
	}
	
/*
=================================================
	_StaticDebugCallback
=================================================
*/
	void GL4Device::_StaticDebugCallback (GLenum source, GLenum type, GLuint /*id*/, GLenum severity,
										   GLsizei length, const GLchar* message, const void* userParam)
	{
		String	str;

		switch ( severity )
		{
			case GL_DEBUG_SEVERITY_HIGH 			: str << "[High]";			break;
			case GL_DEBUG_SEVERITY_MEDIUM 			: str << "[Medium]";		break;
			case GL_DEBUG_SEVERITY_LOW 				: str << "[Low]";			break;
			case GL_DEBUG_SEVERITY_NOTIFICATION		: str << "[Notification]";	break;
			default									: str << "[unknown]";		WARNING( "unknown severity" );
		}

		str << " src: ";

		switch ( source )
		{
			case GL_DEBUG_SOURCE_API				: str << "OpenGL";			break;
			case GL_DEBUG_SOURCE_WINDOW_SYSTEM		: str << "OS";				break;
			case GL_DEBUG_SOURCE_SHADER_COMPILER	: str << "GL_Compiler";		break;
			case GL_DEBUG_SOURCE_THIRD_PARTY		: str << "Third_Party";		break;
			case GL_DEBUG_SOURCE_APPLICATION 		: str << "Application";		break;
			case GL_DEBUG_SOURCE_OTHER 				: str << "Other";			break;
			default									: str << "Unknown";			WARNING( "unknown source" );
		}

		str << ", type: ";

		switch ( type )
		{
			case GL_DEBUG_TYPE_ERROR 				: str << "Error";				break;
			case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR	: str << "Deprecated";			break;
			case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR	: str << "Undefined_Behavior";	break;
			case GL_DEBUG_TYPE_PORTABILITY 			: str << "Portability";			break;
			case GL_DEBUG_TYPE_PERFORMANCE 			: str << "Perfomance";			break;
			case GL_DEBUG_TYPE_MARKER 				: str << "Marker";				break;
			case GL_DEBUG_TYPE_PUSH_GROUP 			: str << "Push_Group";			break;
			case GL_DEBUG_TYPE_POP_GROUP 			: str << "Pop_Group";			break;
			case GL_DEBUG_TYPE_OTHER 				: str << "Other";				break;
			default									: str << "Unknown";				WARNING( "unknown type" );
		}

		str << ", message:\n";

		if ( message != null and length > 0 )
			str += message;

		LOG( str, ELog::Debug );

		Cast< GL4Device const *>(userParam)->_debugReportCounter++;
	}
	
/*
=================================================
	BeginFrame
=================================================
*/
	bool GL4Device::BeginFrame ()
	{
		DBG_CHECK_ERR( _initialized );
		CHECK_ERR( not _frameStarted );

		_frameStarted = true;
		return true;
	}
	
/*
=================================================
	EndFrame
=================================================
*/
	bool GL4Device::EndFrame ()
	{
		DBG_CHECK_ERR( _initialized );
		CHECK_ERR( _frameStarted );

		if ( _vulkanCompatibility and _framebuffer )
		{
			_framebuffer.ToPtr< GL4FlippedSystemFramebuffer >()->Present();
		}

		_currentImageIndex = (_currentImageIndex+1) % _swapchainLength;
		_frameStarted = false;
		return true;
	}
	
/*
=================================================
	OnResize
=================================================
*/
	bool GL4Device::OnResize (const uint2 &size)
	{
		CHECK_ERR( not _frameStarted );

		_surfaceSize = size;

		if ( _framebuffer )
		{
			if ( _vulkanCompatibility )
				_framebuffer.ToPtr< GL4FlippedSystemFramebuffer >()->Resize( _surfaceSize );
			else
				_framebuffer.ToPtr< GL4SystemFramebuffer >()->Resize( _surfaceSize );
		}

		return true;
	}
	
/*
=================================================
	IsExtensionSupported
=================================================
*/
	bool GL4Device::IsExtensionSupported (StringCRef name)
	{
		const char *	ext		= null;
		GLint			count	= 0;
		
		GL_CALL( glGetIntegerv( GL_NUM_EXTENSIONS, OUT &count ) );

		for (int i = 0; i < count; ++i)
		{
			GL_CALL( ext = Cast<const char *>(glGetStringi( GL_EXTENSIONS, i )) );

			if ( name.EqualsIC( ext ) )
				return true;
		}
		return false;
	}
	
/*
=================================================
	SetObjectName
=================================================
*/
	bool GL4Device::SetObjectName (gl::GLuint id, StringCRef name, EGpuObject::type type) const
	{
		if ( name.Empty() or id == 0  )
			return false;

		GL_CALL( glObjectLabel( GL4Enum( type ), id, GLsizei(name.Length()), name.ptr() ) );
		return true;
	}
	
/*
=================================================
	SetObjectName
=================================================
*/
	bool GL4Device::SetObjectName (void* ptr, StringCRef name, EGpuObject::type type) const
	{
		CHECK_ERR( type == EGpuObject::Fence );

		if ( name.Empty() or ptr == null  )
			return false;

		GL_CALL( glObjectPtrLabel( ptr, GLsizei(name.Length()), name.ptr() ) );
		return true;
	}
	
/*
=================================================
	_CreateRenderPass
=================================================
*/
	bool GL4Device::_CreateRenderPass ()
	{
		CHECK_ERR( not _renderPass );

		ModulePtr	module;
		CHECK_ERR( GlobalSystems()->modulesFactory->Create(
					GLRenderPassModuleID,
					GlobalSystems(),
					CreateInfo::GpuRenderPass{
						null,
						RenderPassDescrBuilder::CreateForSurface( _colorPixelFormat, _depthStencilPixelFormat,
																  _vulkanCompatibility ? EImageLayout::TransferSrcOptimal : EImageLayout::PresentSrc )
					},
					OUT module ) );

		_renderPass = module;
		return true;
	}
	
/*
=================================================
	_CreateFramebuffer
=================================================
*/
	bool GL4Device::_CreateFramebuffer ()
	{
		CHECK_ERR( _renderPass );
		CHECK_ERR( not _framebuffer );

		if ( _vulkanCompatibility )
		{
			auto fb = New< GL4FlippedSystemFramebuffer >( GlobalSystems() );
			fb->Send( ModuleMsg::AttachModule{ _renderPass });
			CHECK_ERR( fb->CreateFramebuffer( _surfaceSize, _colorPixelFormat, _depthStencilPixelFormat, _samples ) );
			_framebuffer = fb;
		}
		else
		{
			auto fb = New< GL4SystemFramebuffer >( GlobalSystems() );
			fb->Send( ModuleMsg::AttachModule{ _renderPass });
			CHECK_ERR( fb->CreateFramebuffer( _surfaceSize, _colorPixelFormat, _depthStencilPixelFormat, _samples ) );
			_framebuffer = fb;
		}
		return true;
	}
	
/*
=================================================
	_UpdateProperties
=================================================
*/
	void GL4Device::_UpdateProperties ()
	{
		GLint	idata[4] = {};

		GL_CALL( glGetIntegerv( GL_MAX_COMPUTE_WORK_GROUP_INVOCATIONS, OUT idata ) );
		_properties.maxComputeWorkGroupInvocations = idata[0];

		GL_CALL( glGetIntegeri_v( GL_MAX_COMPUTE_WORK_GROUP_COUNT, 0, OUT &idata[0] ) );
		GL_CALL( glGetIntegeri_v( GL_MAX_COMPUTE_WORK_GROUP_COUNT, 1, OUT &idata[1] ) );
		GL_CALL( glGetIntegeri_v( GL_MAX_COMPUTE_WORK_GROUP_COUNT, 2, OUT &idata[2] ) );
		_properties.maxComputeWorkGroupCount = uint3( idata[0], idata[1], idata[2] );
		
		GL_CALL( glGetIntegeri_v( GL_MAX_COMPUTE_WORK_GROUP_SIZE, 0, OUT &idata[0] ) );
		GL_CALL( glGetIntegeri_v( GL_MAX_COMPUTE_WORK_GROUP_SIZE, 1, OUT &idata[1] ) );
		GL_CALL( glGetIntegeri_v( GL_MAX_COMPUTE_WORK_GROUP_SIZE, 2, OUT &idata[2] ) );
		_properties.maxComputeWorkGroupSize = uint3( idata[0], idata[1], idata[2] );

		_properties.explicitMemoryObjects = false; //IsExtensionSupported( "GL_EXT_memory_object" );
	}
	
/*
=================================================
	WriteInfo
=================================================
*/
	void GL4Device::WriteInfo ()
	{
		String	log;
		GLint	idata[4] = {};
		float	fdata[4] = {};

		log	<< "OpenGL info\n---------------"
			<< "\nRenderer:                " << Cast<const char*>(glGetString( GL_RENDERER ))
			<< "\nVersion:                 " << Cast<const char*>(glGetString( GL_VERSION ))
			<< "\nVendor:                  " << Cast<const char*>(glGetString( GL_VENDOR ))
			<< "\nGLSL:                    " << Cast<const char*>(glGetString( GL_SHADING_LANGUAGE_VERSION ));

		GL_CALL( glGetIntegerv( GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, OUT idata ) );
		log << "\nMax anisotropy:          " << idata[0];
		
		GL_CALL( glGetIntegerv( GL_MAX_VERTEX_ATTRIBS, OUT idata ) );
		log << "\nMax attribs:             " << idata[0];
			
		GL_CALL( glGetIntegerv( GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, OUT idata ) );
		log << "\nMax texture units:       " << idata[0];
		
		GL_CALL( glGetIntegerv( GL_MAX_IMAGE_UNITS, OUT idata ) );
		log << "\nMax image units:         " << idata[0];
		
		GL_CALL( glGetIntegerv( GL_MAX_COMBINED_IMAGE_UNITS_AND_FRAGMENT_OUTPUTS, OUT idata ) );
		log << "\nMax image and output:    " << idata[0];
		
		GL_CALL( glGetIntegerv( GL_MAX_TEXTURE_SIZE, OUT idata ) );
		log << "\nMax texture size:        " << idata[0] << " px";
		
		GL_CALL( glGetIntegerv( GL_MAX_3D_TEXTURE_SIZE, OUT idata ) );
		log << "\nMax texture 3d size:     " << idata[0] << " px";
		
		GL_CALL( glGetIntegerv( GL_MAX_ARRAY_TEXTURE_LAYERS, OUT idata ) );
		log << "\nMax texture layers:      " << idata[0];
		
		GL_CALL( glGetIntegerv( GL_MAX_CUBE_MAP_TEXTURE_SIZE, OUT idata ) );
		log << "\nMax cube map size:       " << idata[0] << " px";
		
		GL_CALL( glGetIntegerv( GL_MAX_RENDERBUFFER_SIZE, OUT idata ) );
		log << "\nMax render buf size:     " << idata[0] << " px";
		
		GL_CALL( glGetIntegerv( GL_MAX_TEXTURE_BUFFER_SIZE, OUT idata ) );
		log << "\nMax texture buf size:    " << ToString( BytesU( idata[0] ) );
		
		GL_CALL( glGetIntegerv( GL_MAX_VIEWPORT_DIMS, OUT idata ) );
		log << "\nMax viewport dim:        " << idata[0] << " px";
		
		GL_CALL( glGetIntegerv( GL_MAX_VIEWPORTS, OUT idata ) );
		log << "\nMax viewports:           " << idata[0];
		
		GL_CALL( glGetIntegerv( GL_MAX_COLOR_TEXTURE_SAMPLES, OUT idata ) );
		log << "\nMax color tex samples:   " << idata[0];
		
		GL_CALL( glGetIntegerv( GL_MAX_DEPTH_TEXTURE_SAMPLES, OUT idata ) );
		log << "\nMax depth tex samples:   " << idata[0];
		
		GL_CALL( glGetIntegerv( GL_MAX_INTEGER_SAMPLES, OUT idata ) );
		log << "\nMax integer samples:     " << idata[0];
		
		GL_CALL( glGetIntegerv( GL_MAX_COLOR_ATTACHMENTS, OUT idata ) );
		log << "\nMax color attachments:   " << idata[0];
		
		GL_CALL( glGetIntegerv( GL_MAX_DRAW_BUFFERS, OUT idata ) );
		log << "\nMax draw buffers:        " << idata[0];
		
		GL_CALL( glGetIntegerv( GL_MAX_DUAL_SOURCE_DRAW_BUFFERS, OUT idata ) );
		log << "\nMax dual draw buffers:   " << idata[0];

		GL_CALL( glGetIntegerv( GL_MAX_UNIFORM_BUFFER_BINDINGS, OUT idata ) );
		log << "\nMax UBO bindings:        " << idata[0];
		
		GL_CALL( glGetIntegerv( GL_MAX_UNIFORM_BLOCK_SIZE, OUT idata ) );
		log << "\nMax UBO size:            " << ToString( BytesU( idata[0] ) );
		
		GL_CALL( glGetIntegerv( GL_MAX_SHADER_STORAGE_BUFFER_BINDINGS, OUT idata ) );
		log << "\nMax SSBO bindings:       " << idata[0];
			
		GL_CALL( glGetIntegerv( GL_MAX_SHADER_STORAGE_BLOCK_SIZE, OUT idata ) );
		log << "\nMax SSBO size:           " << ToString( BytesU( idata[0] ) );
		
		GL_CALL( glGetIntegerv( GL_MAX_COMBINED_SHADER_STORAGE_BLOCKS, OUT idata ) );
		log << "\nMax SSBO blocks:         " << idata[0];

		GL_CALL( glGetIntegerv( GL_MAX_COMBINED_SHADER_OUTPUT_RESOURCES, OUT idata ) );
		log << "\nMax SSBO out blocks:     " << idata[0];

		GL_CALL( glGetIntegerv( GL_MAX_TESS_GEN_LEVEL, OUT idata ) );
		log << "\nMax tess gen level:      " << idata[0];
		
		GL_CALL( glGetIntegerv( GL_MAX_PATCH_VERTICES, OUT idata ) );
		log << "\nMax patch vertices:      " << idata[0];

		GL_CALL( glGetIntegerv( GL_MAX_VERTEX_ATTRIB_BINDINGS, OUT idata ) );
		log << "\nMax vb bindings:         " << idata[0];
		
		GL_CALL( glGetFloatv( GL_ALIASED_LINE_WIDTH_RANGE, OUT fdata ) );
		log << "\nAliased line width:      " << fdata[0] << " - " << fdata[1];
		
		GL_CALL( glGetFloatv( GL_SMOOTH_LINE_WIDTH_RANGE, OUT fdata ) );
		log << "\nSmooth line width:       " << fdata[0] << " - " << fdata[1];
		
		GL_CALL( glGetFloatv( GL_SMOOTH_LINE_WIDTH_GRANULARITY, OUT fdata ) );
		log << "\nSmooth line granularity: " << fdata[0];

		GL_CALL( glGetFloatv( GL_POINT_SIZE_RANGE, OUT fdata ) );
		log << "\nPoint size:              " << fdata[0] << " - " << fdata[1];

		GL_CALL( glGetFloatv( GL_POINT_SIZE_GRANULARITY, OUT fdata ) );
		log << "\nPoint size granularity:  " << fdata[0];
		
		GL_CALL( glGetIntegerv( GL_MAX_COMPUTE_WORK_GROUP_INVOCATIONS, OUT idata ) );
		log << "\nMax invocations:         " << idata[0];

		GL_CALL( glGetIntegeri_v( GL_MAX_COMPUTE_WORK_GROUP_COUNT, 0, OUT &idata[0] ) );
		GL_CALL( glGetIntegeri_v( GL_MAX_COMPUTE_WORK_GROUP_COUNT, 1, OUT &idata[1] ) );
		GL_CALL( glGetIntegeri_v( GL_MAX_COMPUTE_WORK_GROUP_COUNT, 2, OUT &idata[2] ) );
		log << "\nMax compute groups:      " << idata[0] << ", " << idata[1] << ", " << idata[2];
		
		GL_CALL( glGetIntegeri_v( GL_MAX_COMPUTE_WORK_GROUP_SIZE, 0, OUT &idata[0] ) );
		GL_CALL( glGetIntegeri_v( GL_MAX_COMPUTE_WORK_GROUP_SIZE, 1, OUT &idata[1] ) );
		GL_CALL( glGetIntegeri_v( GL_MAX_COMPUTE_WORK_GROUP_SIZE, 2, OUT &idata[2] ) );
		log << "\nMax local groups:        " << idata[0] << ", " << idata[1] << ", " << idata[2];

		//GL_CALL( glGetIntegerv( GL_MAX_COMPUTE_VARIABLE_GROUP_INVOCATIONS_ARB, &idata[0] ) );
		//log << "\nMax compute invocations: " << idata[0];

		log << "\n---------------";

		// TODO: GL_MAX_FRAMEBUFFER_WIDTH, GL_MAX_FRAMEBUFFER_HEIGHT,
		//		GL_MAX_FRAMEBUFFER_LAYERS, GL_MAX_FRAMEBUFFER_SAMPLES,
		//		GL_MAX_SERVER_WAIT_TIMEOUT

		LOG( log, ELog::Debug | ELog::SpoilerFlag );

		// write all extensions
		#if 1
			log.Clear();
			log << "OpenGL Extensions\n---------------\n";

			GLint			num_ext;
			const char *	ext_str;
			
			GL_CALL( glGetIntegerv( GL_NUM_EXTENSIONS, &num_ext ) );
			
			for (GLint i = 0; i < num_ext; ++i)
			{
				GL_CALL( ext_str = Cast<const char *>(glGetStringi( GL_EXTENSIONS, i )) );

				log << (i == 0 ? "" : ((i & 3) == 0 ? ",\n" : ", ")) << ext_str;
			}
			
			log << "\n---------------";

			LOG( log, ELog::Debug | ELog::SpoilerFlag );
		#endif
	}

}	// PlatformGL
}	// Engine

#endif	// GRAPHICS_API_OPENGL
