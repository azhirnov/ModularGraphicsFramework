// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Engine/Platforms/OpenGL/Impl/gl4.h"
#include "Engine/Platforms/OpenGL/Windows/GLWinLibrary.h"

#if defined( GRAPHICS_API_OPENGL )

namespace gl
{
	using Engine::PlatformGL::GLLibrary;
	
#	pragma warning (push)
#	pragma warning (disable : 4100)	// 'identifier' : unreferenced formal parameter

	// get function address from driver //
#	define OPENGL4_GET_PROC( _p_, _n_, _d_ ) \
		++counter; \
		if ( (_n_ = (_p_)GL4_GetProcAddress( TOSTRING( _n_ ) )) == null ) \
		{ \
			_n_ = _d_; \
			missing++; \
		}

	// Default Functions //
#	undef  GL4_BUILD_DEFFUNC
#	define GL4_BUILD_DEFFUNC( _retType_, _funcName_, _funcParams_, _retValue_ ) \
		_retType_ GL4_APIENTRY glDefault##_funcName_ _funcParams_ \
		{ \
			using namespace Engine; \
			LOG( "Used dummy function gl" TOSTRING( _funcName_ ), ELog::Debug ); \
			return _retValue_; \
		}
	
	// pointers to functions //
#	undef  GL4_BUILDFUNC
#	define GL4_BUILDFUNC( _retType_, _funcName_, _funcParams_, _retValue_ ) \
		PFNGL##_funcName_##PROC		gl##_funcName_ = &glDefault##_funcName_;
	

	GL4_CORE_FUNCTIONS( GL4_BUILD_DEFFUNC )
	GL4_CORE_FUNCTIONS( GL4_BUILDFUNC )

	static int	gl4Version = 0;
	
#	pragma warning (pop)

/*
=================================================
	GL4_GetProcAddress
=================================================
*/
	void * GL4_GetProcAddress (const char *name)
	{
		using namespace Engine;
		void * ptr = SingletonSingleThread::Instance< GLLibrary >()->GetProc( name );
		
		DEBUG_ONLY(
		if ( ptr == null ) {
			LOG( (String("Can't get address of ") << name).cstr(), ELog::Debug );
		})
		return ptr;
	}

/*
=================================================
	GL4_Init
=================================================
*/
	bool GL4_Init ()
	{
		using namespace Engine;

		int	missing = 0;	// number of missing function addresses
		int	counter = 0;	// number of all functions

		CHECK_ERR( SingletonSingleThread::Instance< GLLibrary >()->Load() );
		
#		define GL4_GETPROC( _retType_, _funcName_, _funcParams_, _retValue_ ) \
			OPENGL4_GET_PROC( PFNGL##_funcName_##PROC, gl##_funcName_, &glDefault##_funcName_ );
		
		GL4_CORE_FUNCTIONS( GL4_GETPROC )

#		undef GL4_GETPROC
		
		{
			int	major = 0;
			int	minor = 0;
			GL_CALL( glGetIntegerv( GL_MAJOR_VERSION, &major ) );
			GL_CALL( glGetIntegerv( GL_MINOR_VERSION, &minor ) );
			gl4Version = major * 100 + minor * 10;
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
	GL4_Delete
=================================================
*/
	void GL4_Delete ()
	{
		using namespace Engine;

		#define GL4_ZEROFUNC( _retType_, _funcName_, _funcParams_, _retValue_ ) \
			gl##_funcName_ = &glDefault##_funcName_;

		GL4_CORE_FUNCTIONS( GL4_ZEROFUNC )
			
		#undef GL4_ZEROFUNC

		SingletonSingleThread::Instance< GLLibrary >()->Unload();
	}
	
/*
=================================================
	GL4_WriteInfo
=================================================
*/
	void GL4_WriteInfo ()
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

		GL_CALL( glGetIntegerv( GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, idata ) );
		log << "\nMax anisotropy:          " << idata[0];
		
		GL_CALL( glGetIntegerv( GL_MAX_VERTEX_ATTRIBS, idata ) );
		log << "\nMax attribs:             " << idata[0];
			
		GL_CALL( glGetIntegerv( GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, idata ) );
		log << "\nMax texture units:       " << idata[0];
		
		GL_CALL( glGetIntegerv( GL_MAX_IMAGE_UNITS, idata ) );
		log << "\nMax image units:         " << idata[0];
		
		GL_CALL( glGetIntegerv( GL_MAX_COMBINED_IMAGE_UNITS_AND_FRAGMENT_OUTPUTS, idata ) );
		log << "\nMax image and output:    " << idata[0];
		
		GL_CALL( glGetIntegerv( GL_MAX_TEXTURE_SIZE, idata ) );
		log << "\nMax texture size:        " << idata[0] << " px";
		
		GL_CALL( glGetIntegerv( GL_MAX_3D_TEXTURE_SIZE, idata ) );
		log << "\nMax texture 3d size:     " << idata[0] << " px";
		
		GL_CALL( glGetIntegerv( GL_MAX_ARRAY_TEXTURE_LAYERS, idata ) );
		log << "\nMax texture layers:      " << idata[0];
		
		GL_CALL( glGetIntegerv( GL_MAX_CUBE_MAP_TEXTURE_SIZE, idata ) );
		log << "\nMax cube map size:       " << idata[0] << " px";
		
		GL_CALL( glGetIntegerv( GL_MAX_RENDERBUFFER_SIZE, idata ) );
		log << "\nMax render buf size:     " << idata[0] << " px";
		
		GL_CALL( glGetIntegerv( GL_MAX_TEXTURE_BUFFER_SIZE, idata ) );
		log << "\nMax texture buf size:    " << ToString( BytesU( idata[0] ) );
		
		GL_CALL( glGetIntegerv( GL_MAX_VIEWPORT_DIMS, idata ) );
		log << "\nMax viewport dim:        " << idata[0] << " px";
		
		GL_CALL( glGetIntegerv( GL_MAX_VIEWPORTS, idata ) );
		log << "\nMax viewports:           " << idata[0];
		
		GL_CALL( glGetIntegerv( GL_MAX_COLOR_TEXTURE_SAMPLES, idata ) );
		log << "\nMax color tex samples:   " << idata[0];
		
		GL_CALL( glGetIntegerv( GL_MAX_DEPTH_TEXTURE_SAMPLES, idata ) );
		log << "\nMax depth tex samples:   " << idata[0];
		
		GL_CALL( glGetIntegerv( GL_MAX_INTEGER_SAMPLES, idata ) );
		log << "\nMax integer samples:     " << idata[0];
		
		GL_CALL( glGetIntegerv( GL_MAX_COLOR_ATTACHMENTS, idata ) );
		log << "\nMax color attachments:   " << idata[0];
		
		GL_CALL( glGetIntegerv( GL_MAX_DRAW_BUFFERS, idata ) );
		log << "\nMax draw buffers:        " << idata[0];
		
		GL_CALL( glGetIntegerv( GL_MAX_DUAL_SOURCE_DRAW_BUFFERS, idata ) );
		log << "\nMax dual draw buffers:   " << idata[0];

		GL_CALL( glGetIntegerv( GL_MAX_UNIFORM_BUFFER_BINDINGS, idata ) );
		log << "\nMax UBO bindings:        " << idata[0];
		
		GL_CALL( glGetIntegerv( GL_MAX_UNIFORM_BLOCK_SIZE, idata ) );
		log << "\nMax UBO size:            " << ToString( BytesU( idata[0] ) );
		
		GL_CALL( glGetIntegerv( GL_MAX_SHADER_STORAGE_BUFFER_BINDINGS, idata ) );
		log << "\nMax SSBO bindings:       " << idata[0];
			
		GL_CALL( glGetIntegerv( GL_MAX_SHADER_STORAGE_BLOCK_SIZE, idata ) );
		log << "\nMax SSBO size:           " << ToString( BytesU( idata[0] ) );
		
		GL_CALL( glGetIntegerv( GL_MAX_COMBINED_SHADER_STORAGE_BLOCKS, idata ) );
		log << "\nMax SSBO blocks:         " << idata[0];

		GL_CALL( glGetIntegerv( GL_MAX_COMBINED_SHADER_OUTPUT_RESOURCES, idata ) );
		log << "\nMax SSBO out blocks:     " << idata[0];

		GL_CALL( glGetIntegerv( GL_MAX_TESS_GEN_LEVEL, idata ) );
		log << "\nMax tess gen level:      " << idata[0];
		
		GL_CALL( glGetIntegerv( GL_MAX_PATCH_VERTICES, idata ) );
		log << "\nMax patch vertices:      " << idata[0];

		GL_CALL( glGetIntegerv( GL_MAX_VERTEX_ATTRIB_BINDINGS, idata ) );
		log << "\nMax vb bindings:         " << idata[0];
		
		GL_CALL( glGetFloatv( GL_ALIASED_LINE_WIDTH_RANGE, fdata ) );
		log << "\nAliased line width:      " << fdata[0] << " - " << fdata[1];
		
		GL_CALL( glGetFloatv( GL_SMOOTH_LINE_WIDTH_RANGE, fdata ) );
		log << "\nSmooth line width:       " << fdata[0] << " - " << fdata[1];
		
		GL_CALL( glGetFloatv( GL_SMOOTH_LINE_WIDTH_GRANULARITY, fdata ) );
		log << "\nSmooth line granularity: " << fdata[0];

		GL_CALL( glGetFloatv( GL_POINT_SIZE_RANGE, fdata ) );
		log << "\nPoint size:              " << fdata[0] << " - " << fdata[1];

		GL_CALL( glGetFloatv( GL_POINT_SIZE_GRANULARITY, fdata ) );
		log << "\nPoint size granularity:  " << fdata[0];

		// TODO: GL_MAX_FRAMEBUFFER_WIDTH, GL_MAX_FRAMEBUFFER_HEIGHT,
		//		GL_MAX_FRAMEBUFFER_LAYERS, GL_MAX_FRAMEBUFFER_SAMPLES,
		//		GL_MAX_SERVER_WAIT_TIMEOUT

		LOG( log.cstr(), ELog::Debug | ELog::SpoilerFlag );

		// write all extensions
		#if 1
			log.Clear();
			log << "OpenGL Extensions\n---------------\n";

			GLint			num_ext;
			const char *	ext_str;
			
			GL_CALL( glGetIntegerv( GL_NUM_EXTENSIONS, &num_ext ) );
			
			for (GLint i = 0; i < num_ext; ++i)
			{
				GL_CALL( ext_str = (const char *) glGetStringi( GL_EXTENSIONS, i ) );

				log << (i == 0 ? "" : ((i & 3) == 0 ? ",\n" : ", ")) << ext_str;
			}

			LOG( log.cstr(), ELog::Debug | ELog::SpoilerFlag );
		#endif
	}

/*
=================================================
	GL4_GetVersion
=================================================
*/
	int  GL4_GetVersion ()
	{
		return gl4Version;
	}
	
/*
=================================================
	GL4_GetShaderVersion
=================================================
*/
	int  GL4_GetShaderVersion ()
	{
		if ( gl4Version >= 330 )
			return gl4Version;

		switch ( gl4Version )
		{
			case 320 :	return 150;
			case 310 :	return 140;
			case 300 :	return 130;
			case 210 :	return 120;
			case 200 :	return 110;
		}

		return 0;
	}

/*
=================================================
	GL4_ClearErrors
=================================================
*/
	void GL4_ClearErrors (int maxErrors)
	{
		for (int i = 0; i < maxErrors and glGetError() != GL_NO_ERROR; ++i) {}
	}
	
/*
=================================================
	GL4_CheckErrors
=================================================
*/
	bool GL4_CheckErrors (const char *glcall, const char *func, const char *file, int line)
	{
		using namespace Engine;
		
		String	msg;
		GLenum	err = glGetError();

		if ( err == GL_NO_ERROR )
			return true;

		msg << "OpenGL error: ";

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

		LOG( msg.cstr(), ELog::Error, file, line );
		return false;
	}


}	// gl

#endif	// GRAPHICS_API_OPENGL