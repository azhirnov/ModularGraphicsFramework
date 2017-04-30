// Generated header file

#pragma once

typedef cl_uint cl_gl_object_type ;
typedef cl_uint cl_gl_texture_info ;
typedef cl_uint cl_gl_platform_info ;
typedef struct __GLsync * cl_GLsync ;
typedef cl_uint cl_gl_context_info ;
typedef CL_API_ENTRY cl_int ( CL_API_CALL * clGetGLContextInfoKHR_fn ) ( 
const cl_context_properties * properties , 
cl_gl_context_info param_name , 
size_t param_value_size , 
void * param_value , 
size_t * param_value_size_ret ) ;

enum
{
	CL_GL_OBJECT_BUFFER		= 0x2000 ,
	CL_GL_OBJECT_TEXTURE2D		= 0x2001 ,
	CL_GL_OBJECT_TEXTURE3D		= 0x2002 ,
	CL_GL_OBJECT_RENDERBUFFER		= 0x2003 ,
	CL_GL_TEXTURE_TARGET		= 0x2004 ,
	CL_GL_MIPMAP_LEVEL		= 0x2005 ,
	cl_khr_gl_sharing		= 1 ,
	CL_INVALID_GL_SHAREGROUP_REFERENCE_KHR		= - 1000 ,
	CL_CURRENT_DEVICE_FOR_GL_CONTEXT_KHR		= 0x2006 ,
	CL_DEVICES_FOR_GL_CONTEXT_KHR		= 0x2007 ,
	CL_GL_CONTEXT_KHR		= 0x2008 ,
	CL_EGL_DISPLAY_KHR		= 0x2009 ,
	CL_GLX_DISPLAY_KHR		= 0x200A ,
	CL_WGL_HDC_KHR		= 0x200B ,
	CL_CGL_SHAREGROUP_KHR		= 0x200C ,
};

#define CL2_GL_FUNCTIONS( _buildFunc_ ) \
	_buildFunc_( cl_mem, CreateFromGLBuffer, ( cl_context context , cl_mem_flags flags , cl_GLuint bufobj , int * errcode_ret ) , cl_mem() ) \
	_buildFunc_( cl_mem, CreateFromGLTexture2D, ( cl_context context , cl_mem_flags flags , cl_GLenum target , cl_GLint miplevel , cl_GLuint texture , cl_int * errcode_ret ) , cl_mem() ) \
	_buildFunc_( cl_mem, CreateFromGLTexture3D, ( cl_context context , cl_mem_flags flags , cl_GLenum target , cl_GLint miplevel , cl_GLuint texture , cl_int * errcode_ret ) , cl_mem() ) \
	_buildFunc_( cl_mem, CreateFromGLRenderbuffer, ( cl_context context , cl_mem_flags flags , cl_GLuint renderbuffer , cl_int * errcode_ret ) , cl_mem() ) \
	_buildFunc_( cl_int, GetGLObjectInfo, ( cl_mem memobj , cl_gl_object_type * gl_object_type , cl_GLuint * gl_object_name ) , cl_int() ) \
	_buildFunc_( cl_int, GetGLTextureInfo, ( cl_mem memobj , cl_gl_texture_info param_name , size_t param_value_size , void * param_value , size_t * param_value_size_ret ) , cl_int() ) \
	_buildFunc_( cl_int, EnqueueAcquireGLObjects, ( cl_command_queue command_queue , cl_uint num_objects , const cl_mem * mem_objects , cl_uint num_events_in_wait_list , const cl_event * event_wait_list , cl_event * event ) , cl_int() ) \
	_buildFunc_( cl_int, EnqueueReleaseGLObjects, ( cl_command_queue command_queue , cl_uint num_objects , const cl_mem * mem_objects , cl_uint num_events_in_wait_list , const cl_event * event_wait_list , cl_event * event ) , cl_int() ) \
	_buildFunc_( cl_int, GetGLContextInfoKHR, ( const cl_context_properties * properties , cl_gl_context_info param_name , size_t param_value_size , void * param_value , size_t * param_value_size_ret ) , cl_int() ) \


