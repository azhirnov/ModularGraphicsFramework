// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Engine/Platforms/OpenGL/300es/gles3.h"
#include "Engine/Platforms/OpenGL/Windows/GLWinLibrary.h"
#include "Engine/STL/ThreadSafe/Singleton.h"

#if defined( GRAPHICS_API_OPENGLES ) and GRAPHICS_API_OPENGLES >= 300

namespace gles
{
	using Engine::PlatformGL::GLLibrary;

# ifdef COMPILER_MSVC
#	pragma warning (push)
#	pragma warning (disable : 4100)	// 'identifier' : unreferenced formal parameter
# endif

	// get function address from driver //
#	define OPENGLES3_GET_PROC( _p_, _n_, _d_ ) \
		++counter; \
		if ( (_n_ = (_p_)GLES3_GetProcAddress( TOSTRING( _n_ ) )) == null ) \
		{ \
			_n_ = _d_; \
			missing++; \
		}

	// Default Functions //
#	undef  GLES3_BUILD_DEFFUNC
#	define GLES3_BUILD_DEFFUNC( _retType_, _funcName_, _funcParams_, _retValue_ ) \
		_retType_ GLES_APIENTRY glesDefault##_funcName_ _funcParams_ \
		{ \
			using namespace Engine; \
			LOG( "Used dummy function gles" TOSTRING( _funcName_ ), ELog::Warning ); \
			return _retValue_; \
		}
	
	// pointers to functions //
#	undef  GLES3_BUILDFUNC
#	define GLES3_BUILDFUNC( _retType_, _funcName_, _funcParams_, _retValue_ ) \
		PFNGLES##_funcName_##PROC	gl##_funcName_ = &glesDefault##_funcName_;
	

	GLES3_CORE_FUNCTIONS( GLES3_BUILD_DEFFUNC )
	GLES3_CORE_FUNCTIONS( GLES3_BUILDFUNC )

	static int	gles3Version = 0;

# ifdef COMPILER_MSVC
#	pragma warning (pop)
# endif

/*
=================================================
	GLES3_GetProcAddress
=================================================
*/
	GX_STL::SharedLibFunction_t  GLES3_GetProcAddress (const char *name)
	{
		using namespace Engine;
		SharedLibFunction_t	ptr = SingletonSingleThread::Instance< GLLibrary >()->GetProc( name );
		
		DEBUG_ONLY(
		if ( ptr == null ) {
			LOG( "Can't get address of "_str << name, ELog::Debug );
		})
		return ptr;
	}

/*
=================================================
	GLES3_Init
=================================================
*/
	bool GLES3_Init ()
	{
		using namespace Engine;

		int	missing = 0;	// number of missing function addresses
		int	counter = 0;	// number of all functions

		CHECK_ERR( SingletonSingleThread::Instance< GLLibrary >()->Load() );
		
#		define GLES3_GETPROC( _retType_, _funcName_, _funcParams_, _retValue_ ) \
			OPENGLES3_GET_PROC( PFNGLES##_funcName_##PROC, gl##_funcName_, &glesDefault##_funcName_ );
		
		GLES3_CORE_FUNCTIONS( GLES3_GETPROC )

#		undef GLES3_GETPROC
		
		{
			int	major = 0;
			int	minor = 0;
			ES_CALL( glGetIntegerv( GL_MAJOR_VERSION, &major ) );
			ES_CALL( glGetIntegerv( GL_MINOR_VERSION, &minor ) );
			gles3Version = major * 100 + minor * 10;
		}

		if ( missing > 0 )
		{
			LOG( (String("While get address of OpenGL functions: ") <<
				 missing << " missing, " << (counter - missing) << " successfully.").cstr(),
				 ELog::Debug );
		}
		
		return true;
	}
	
/*
=================================================
	GLES3_Delete
=================================================
*/
	void GLES3_Delete ()
	{
		using namespace Engine;

		#define GLES3_ZEROFUNC( _retType_, _funcName_, _funcParams_, _retValue_ ) \
			gl##_funcName_ = &glesDefault##_funcName_;

		GLES3_CORE_FUNCTIONS( GLES3_ZEROFUNC )
			
		#undef GLES3_ZEROFUNC

		SingletonSingleThread::Instance< GLLibrary >()->Unload();
	}
	
/*
=================================================
	GLES3_WriteInfo
=================================================
*/
	void GLES3_WriteInfo ()
	{
		using namespace Engine;

		String	log;
		GLint	idata[4] = {};
		float	fdata[4] = {};

		log	<< "OpenGL info\n---------------"
			<< "\nRenderer:                " << (const char*)glGetString( GL_RENDERER )
			<< "\nVersion:                 " << (const char*)glGetString( GL_VERSION )
			<< "\nVendor:                  " << (const char*)glGetString( GL_VENDOR )
			<< "\nGLSL:                    " << (const char*)glGetString( GL_SHADING_LANGUAGE_VERSION );

		//ES_CALL( glGetIntegerv( GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, idata ) );
		//log << "\nMax anisotropy:          " << idata[0];
		
		ES_CALL( glGetIntegerv( GL_MAX_VERTEX_ATTRIBS, idata ) );
		log << "\nMax attribs:             " << idata[0];
			
		ES_CALL( glGetIntegerv( GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, idata ) );
		log << "\nMax texture units:       " << idata[0];
		
		ES_CALL( glGetIntegerv( GL_MAX_IMAGE_UNITS, idata ) );
		log << "\nMax image units:         " << idata[0];
		
		//ES_CALL( glGetIntegerv( GL_MAX_COMBINED_IMAGE_UNITS_AND_FRAGMENT_OUTPUTS, idata ) );
		//log << "\nMax image and output:    " << idata[0];
		
		ES_CALL( glGetIntegerv( GL_MAX_TEXTURE_SIZE, idata ) );
		log << "\nMax texture size:        " << idata[0] << " px";
		
		ES_CALL( glGetIntegerv( GL_MAX_3D_TEXTURE_SIZE, idata ) );
		log << "\nMax texture 3d size:     " << idata[0] << " px";
		
		ES_CALL( glGetIntegerv( GL_MAX_ARRAY_TEXTURE_LAYERS, idata ) );
		log << "\nMax texture layers:      " << idata[0];
		
		ES_CALL( glGetIntegerv( GL_MAX_CUBE_MAP_TEXTURE_SIZE, idata ) );
		log << "\nMax cube map size:       " << idata[0] << " px";
		
		ES_CALL( glGetIntegerv( GL_MAX_RENDERBUFFER_SIZE, idata ) );
		log << "\nMax render buf size:     " << idata[0] << " px";
		
		ES_CALL( glGetIntegerv( GL_MAX_TEXTURE_BUFFER_SIZE, idata ) );
		log << "\nMax texture buf size:    " << ToString( BytesU( idata[0] ) );
		
		ES_CALL( glGetIntegerv( GL_MAX_VIEWPORT_DIMS, idata ) );
		log << "\nMax viewport dim:        " << idata[0] << " px";
		
		//ES_CALL( glGetIntegerv( GL_MAX_VIEWPORTS, idata ) );
		//log << "\nMax viewports:           " << idata[0];
		
		ES_CALL( glGetIntegerv( GL_MAX_COLOR_TEXTURE_SAMPLES, idata ) );
		log << "\nMax color tex samples:   " << idata[0];
		
		ES_CALL( glGetIntegerv( GL_MAX_DEPTH_TEXTURE_SAMPLES, idata ) );
		log << "\nMax depth tex samples:   " << idata[0];
		
		ES_CALL( glGetIntegerv( GL_MAX_INTEGER_SAMPLES, idata ) );
		log << "\nMax integer samples:     " << idata[0];
		
		ES_CALL( glGetIntegerv( GL_MAX_COLOR_ATTACHMENTS, idata ) );
		log << "\nMax color attachments:   " << idata[0];
		
		ES_CALL( glGetIntegerv( GL_MAX_DRAW_BUFFERS, idata ) );
		log << "\nMax draw buffers:        " << idata[0];
		
		//ES_CALL( glGetIntegerv( GL_MAX_DUAL_SOURCE_DRAW_BUFFERS, idata ) );
		//log << "\nMax dual draw buffers:   " << idata[0];

		ES_CALL( glGetIntegerv( GL_MAX_UNIFORM_BUFFER_BINDINGS, idata ) );
		log << "\nMax UBO bindings:        " << idata[0];
		
		ES_CALL( glGetIntegerv( GL_MAX_UNIFORM_BLOCK_SIZE, idata ) );
		log << "\nMax UBO size:            " << ToString( BytesU( idata[0] ) );
		
		ES_CALL( glGetIntegerv( GL_MAX_SHADER_STORAGE_BUFFER_BINDINGS, idata ) );
		log << "\nMax SSBO bindings:       " << idata[0];
			
		ES_CALL( glGetIntegerv( GL_MAX_SHADER_STORAGE_BLOCK_SIZE, idata ) );
		log << "\nMax SSBO size:           " << ToString( BytesU( idata[0] ) );
		
		ES_CALL( glGetIntegerv( GL_MAX_COMBINED_SHADER_STORAGE_BLOCKS, idata ) );
		log << "\nMax SSBO blocks:         " << idata[0];

		ES_CALL( glGetIntegerv( GL_MAX_COMBINED_SHADER_OUTPUT_RESOURCES, idata ) );
		log << "\nMax SSBO out blocks:     " << idata[0];

		ES_CALL( glGetIntegerv( GL_MAX_TESS_GEN_LEVEL, idata ) );
		log << "\nMax tess gen level:      " << idata[0];
		
		ES_CALL( glGetIntegerv( GL_MAX_PATCH_VERTICES, idata ) );
		log << "\nMax patch vertices:      " << idata[0];

		ES_CALL( glGetIntegerv( GL_MAX_VERTEX_ATTRIB_BINDINGS, idata ) );
		log << "\nMax vb bindings:         " << idata[0];
		
		ES_CALL( glGetFloatv( GL_ALIASED_LINE_WIDTH_RANGE, fdata ) );
		log << "\nAliased line width:      " << fdata[0] << " - " << fdata[1];
		
		//ES_CALL( glGetFloatv( GL_SMOOTH_LINE_WIDTH_RANGE, fdata ) );
		//log << "\nSmooth line width:       " << fdata[0] << " - " << fdata[1];
		
		//ES_CALL( glGetFloatv( GL_SMOOTH_LINE_WIDTH_GRANULARITY, fdata ) );
		//log << "\nSmooth line granularity: " << fdata[0];

		//ES_CALL( glGetFloatv( GL_POINT_SIZE_RANGE, fdata ) );
		//log << "\nPoint size:              " << fdata[0] << " - " << fdata[1];

		//ES_CALL( glGetFloatv( GL_POINT_SIZE_GRANULARITY, fdata ) );
		//log << "\nPoint size granularity:  " << fdata[0];
		
		ES_CALL( glGetIntegeri_v( GL_MAX_COMPUTE_WORK_GROUP_COUNT, 0, &idata[0] ) );
		ES_CALL( glGetIntegeri_v( GL_MAX_COMPUTE_WORK_GROUP_COUNT, 1, &idata[1] ) );
		ES_CALL( glGetIntegeri_v( GL_MAX_COMPUTE_WORK_GROUP_COUNT, 2, &idata[2] ) );
		log << "\nMax compute groups:      " << idata[0] << ", " << idata[1] << ", " << idata[2];

		//ES_CALL( glGetIntegerv( GL_MAX_COMPUTE_VARIABLE_GROUP_INVOCATIONS_ARB, &idata[0] ) );
		//log << "\nMax compute invocations: " << idata[0];

		log << "\n---------------";

		LOG( log, ELog::Debug | ELog::SpoilerFlag );

		// write all extensions
		#if 1
			log.Clear();
			log << "OpenGL ES Extensions\n---------------\n";

			GLint			num_ext;
			const char *	ext_str;
			
			ES_CALL( glGetIntegerv( GL_NUM_EXTENSIONS, &num_ext ) );
			
			for (GLint i = 0; i < num_ext; ++i)
			{
				ES_CALL( ext_str = (const char *) glGetStringi( GL_EXTENSIONS, i ) );

				log << (i == 0 ? "" : ((i & 3) == 0 ? ",\n" : ", ")) << ext_str;
			}
			
			log << "\n---------------";

			LOG( log, ELog::Debug | ELog::SpoilerFlag );
		#endif
	}

/*
=================================================
	GLES3_GetVersion
=================================================
*/
	int  GLES3_GetVersion ()
	{
		return gles3Version;
	}
	
/*
=================================================
	GLES3_GetShaderVersion
=================================================
*/
	int  GLES3_GetShaderVersion ()
	{
		return gles3Version;
	}

/*
=================================================
	GLES3_ClearErrors
=================================================
*/
	void GLES3_ClearErrors (int maxErrors)
	{
		for (int i = 0; i < maxErrors and glGetError() != GL_NO_ERROR; ++i) {}
	}
	
/*
=================================================
	GLES3_CheckErrors
=================================================
*/
	bool GLES3_CheckErrors (const char *glcall, const char *func, const char *file, int line)
	{
		using namespace Engine;
		
		String	msg;
		GLenum	err = glGetError();

		if ( err == GL_NO_ERROR )
			return true;

		msg << "OpenGL ES error: ";

		switch ( err )
		{
			case GL_INVALID_ENUM:
				msg << "GL_INVALID_ENUM";
				break;

			case GL_INVALID_VALUE:
				msg << "GL_INVALID_VALUE";
				break;

			case GL_INVALID_OPERATION:
				msg << "GL_INVALID_OPERATION";
				break;

			case GL_INVALID_FRAMEBUFFER_OPERATION:
				msg << "GL_INVALID_FRAMEBUFFER_OPERATION";
				break;

			case GL_OUT_OF_MEMORY:
				msg << "GL_OUT_OF_MEMORY";
				break;

			default:
				msg << "code 0x" << String().FormatI( err, 16 );
				break;
		}
		
		msg << ", in " << glcall << ", function " << func;

		LOG( msg, ELog::Error, file, line );
		return false;
	}


}	// gles

#endif	// GRAPHICS_API_OPENGLES