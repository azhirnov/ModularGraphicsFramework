// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/Platforms/Public/Common.h"

#if defined( GRAPHICS_API_OPENGLES ) and GRAPHICS_API_OPENGLES >= 300

namespace gles
{
#	define GLES_APIENTRY		CALLBACK

	typedef GX_STL::GXTypes::isize		ptrdiff_t;
	typedef GX_STL::GXTypes::ilong		int64_t;
	typedef GX_STL::GXTypes::ulong		uint64_t;

	typedef unsigned	int		GLenum;
	typedef unsigned	char	GLboolean;
	typedef unsigned	int		GLbitfield;
	typedef signed		char	GLbyte;
	typedef signed		short	GLshort;
	typedef signed		int		GLint;
	typedef signed		int		GLsizei;
	typedef unsigned	char	GLubyte;
	typedef unsigned	short	GLushort;
	typedef unsigned	int		GLuint;
	typedef float				GLfloat;
	typedef float				GLclampf;
	typedef double				GLdouble;
	typedef double				GLclampd;
	typedef void				GLvoid;

	typedef char				GLchar;
	typedef ptrdiff_t			GLintptr;
	typedef ptrdiff_t			GLsizeiptr;
	typedef unsigned short		GLhalf;

	typedef int64_t				GLint64EXT;
	typedef uint64_t			GLuint64EXT;
	typedef int64_t				GLint64;
	typedef uint64_t			GLuint64;
	typedef struct __GLsync *	GLsync;

	struct _cl_context;
	struct _cl_event;
	
	using GLDEBUGPROC		= void (GLES_APIENTRY *) (GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *message, const void *userParam);
	using GLDEBUGPROCKHR	= GLDEBUGPROC;

	struct GLESDrawArraysIndirectCommand {
        GLuint  count;
        GLuint  instanceCount;
        GLuint  first;
        GLuint  baseInstance;
    };

	struct GLESDrawElementsIndirectCommand {
        GLuint  count;
        GLuint  instanceCount;
        GLuint  firstIndex;
        GLuint  baseVertex;
        GLuint  baseInstance;
    };


# if GRAPHICS_API_OPENGLES == 300
#	include "External/opengles/opengles30.h"

# elif GRAPHICS_API_OPENGLES == 310
#	include "External/opengles/opengles31.h"

# elif GRAPHICS_API_OPENGLES == 320
#	include "External/opengles/opengles32.h"

# else
#	error Unsupported OpenGL ES version!
# endif

	
	// typedefs //
#	define GLES3_BUILDTYPEDEF( _retType_, _funcName_, _funcParams_, _retValue_ ) \
		extern "C" typedef _retType_ (GLES_APIENTRY * PFNGLES##_funcName_##PROC)  _funcParams_;


	// pointers to functions //
#	define GLES3_BUILDFUNC( _retType_, _funcName_, _funcParams_, _retValue_ ) \
		extern PFNGLES##_funcName_##PROC	gl##_funcName_;
	

	GLES3_CORE_FUNCTIONS( GLES3_BUILDTYPEDEF )
	GLES3_CORE_FUNCTIONS( GLES3_BUILDFUNC )


#	undef GLES3_BUILDTYPEDEF
#	undef GLES3_BUILDFUNC


	// API
	bool GLES3_Init ();
	void GLES3_Delete ();
	void GLES3_WriteInfo ();

	int  GLES3_GetVersion ();
	int  GLES3_GetShaderVersion ();
	
	void GLES3_ClearErrors (int maxErrors = 10);
	bool GLES3_CheckErrors (const char *glcall, const char *func, const char *file, int line);
	

	// macro
# if !defined( __GX_GRAPHICS_API_ERROR_CHECKS__ )
	
#	define ES_CALL( ... )		{ __VA_ARGS__; }
#	define ES_CHECK( ... )		{ __VA_ARGS__; }

# else

#	define ES_CALL( ... ) \
	{ \
		__VA_ARGS__; \
		::gles::GLES3_CheckErrors( TOSTRING( __VA_ARGS__ ), GX_FUNCTION_NAME, __FILE__, __LINE__ ); \
	}

#	define __ES_CALL_R( _func_, _ret_, ... ) \
	{ \
		_func_; \
		if ( not ::gles::GLES3_CheckErrors( TOSTRING( _func_ ), GX_FUNCTION_NAME, __FILE__, __LINE__ ) ) \
			return _ret_; \
	}

	// Warning: different behavior on Debug and Release!
#	define ES_CHECK( ... ) \
		__ES_CALL_R( AUXDEF_GETARG_0( __VA_ARGS__ ), AUXDEF_GETARG_1( __VA_ARGS__, Uninitialized ) )

# endif	// __GX_GRAPHICS_API_ERROR_CHECKS__
	

}	// gles

#endif	// GRAPHICS_API_OPENGLES
