// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Engine/Platforms/OpenGL/450/gl4.h"
#include "Engine/Platforms/OpenGL/Windows/GLWinLibrary.h"
#include "Engine/STL/ThreadSafe/Singleton.h"

#ifdef GRAPHICS_API_OPENGL

namespace gl
{
	using Engine::PlatformGL::GLLibrary;

# ifdef COMPILER_MSVC
#	pragma warning (push)
#	pragma warning (disable : 4100)	// 'identifier' : unreferenced formal parameter
# endif

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
			LOG( "Used dummy function gl" TOSTRING( _funcName_ ), ELog::Warning ); \
			return _retValue_; \
		}
	
	// pointers to functions //
#	undef  GL4_BUILDFUNC
#	define GL4_BUILDFUNC( _retType_, _funcName_, _funcParams_, _retValue_ ) \
		PFNGL##_funcName_##PROC		gl##_funcName_ = &glDefault##_funcName_;
	

	GL4_CORE_FUNCTIONS( GL4_BUILD_DEFFUNC )
	GL4_CORE_FUNCTIONS( GL4_BUILDFUNC )

	static int	gl4Version = 0;

# ifdef COMPILER_MSVC
#	pragma warning (pop)
# endif

/*
=================================================
	GL4_GetProcAddress
=================================================
*/
	GX_STL::SharedLibFunction_t  GL4_GetProcAddress (const char *name)
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

		LOG( msg, ELog::Error, file, line );
		return false;
	}


}	// gl

#endif	// GRAPHICS_API_OPENGL