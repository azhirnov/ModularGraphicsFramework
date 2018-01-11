// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/Platforms/Common/Common.h"

#if defined( GRAPHICS_API_OPENGL )

namespace gl
{
#	define GL4_APIENTRY		CALLBACK

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
	
	typedef void (GL4_APIENTRY  *GLDEBUGPROC) (GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *message, const void *userParam);
	
	typedef  struct {
        GLuint  count;
        GLuint  instanceCount;
        GLuint  first;
        GLuint  baseInstance;
    } GLDrawArraysIndirectCommand;

	typedef  struct {
        GLuint  count;
        GLuint  instanceCount;
        GLuint  firstIndex;
        GLuint  baseVertex;
        GLuint  baseInstance;
    } GLDrawElementsIndirectCommand;


# if GRAPHICS_API_OPENGL == 440
#	include "External/opengl/opengl44_core.h"
# elif GRAPHICS_API_OPENGL == 450
#	include "External/opengl/opengl45_core.h"
# else
#	error Unsupported OpenGL version!
# endif

	
	// typedefs //
#	define GL4_BUILDTYPEDEF( _retType_, _funcName_, _funcParams_, _retValue_ ) \
		extern "C" typedef _retType_ (GL4_APIENTRY * PFNGL##_funcName_##PROC)  _funcParams_;


	// pointers to functions //
#	define GL4_BUILDFUNC( _retType_, _funcName_, _funcParams_, _retValue_ ) \
		extern PFNGL##_funcName_##PROC	gl##_funcName_;
	

	GL4_CORE_FUNCTIONS( GL4_BUILDTYPEDEF )
	GL4_CORE_FUNCTIONS( GL4_BUILDFUNC )


#	undef GL4_BUILDTYPEDEF
#	undef GL4_BUILDFUNC


	// API
	bool GL4_Init ();
	void GL4_Delete ();
	void GL4_WriteInfo ();

	int  GL4_GetVersion ();
	int  GL4_GetShaderVersion ();
	
	void GL4_ClearErrors (int maxErrors = 10);
	bool GL4_CheckErrors (const char *glcall, const char *func, const char *file, int line);
	

	// macro
# if !defined( __GX_GRAPHICS_API_ERROR_CHECKS__ )
	
#	define GL_CALL( ... )		{ __VA_ARGS__; }
#	define GL_CHECK( ... )		{ __VA_ARGS__; }

# else

#	define GL_CALL( ... ) \
	{ \
		__VA_ARGS__; \
		::gl::GL4_CheckErrors( TOSTRING( __VA_ARGS__ ), GX_FUNCTION_NAME, __FILE__, __LINE__ ); \
	}

#	define __GL_CALL_R( _func_, _ret_, ... ) \
	{ \
		_func_; \
		if ( not ::gl::GL4_CheckErrors( TOSTRING( _func_ ), GX_FUNCTION_NAME, __FILE__, __LINE__ ) ) \
			return _ret_; \
	}

	// Warning: different behavior on Debug and Release!
#	define GL_CHECK( ... ) \
		__GL_CALL_R( AUXDEF_GETARG_0( __VA_ARGS__ ), AUXDEF_GETARG_1( __VA_ARGS__, Uninitialized ) )

# endif	// __GX_GRAPHICS_API_ERROR_CHECKS__
	

}	// gl

#endif	// GRAPHICS_API_OPENGL
