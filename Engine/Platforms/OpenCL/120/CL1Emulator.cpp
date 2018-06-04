// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Engine/Platforms/OpenCL/120/CL1Emulator.h"

#ifdef COMPUTE_API_OPENCL

# if defined( GRAPHICS_API_OPENGL )
#	include "Engine/Platforms/OpenGL/450/gl4.h"
# endif

namespace cl
{
	
# if COMPUTE_API_OPENCL < 200
/*
=================================================
	clCreateCommandQueueWithProperties
=================================================
*/
	cl_command_queue clCreateCommandQueueWithProperties (cl_context context,
  														 cl_device_id device,
  														 const cl_queue_properties *properties,
  														 cl_int *errcode_ret)
	{
		cl_command_queue_properties	prop = 0;

		if ( properties != null )
		{
			for (int i = 0; properties[i] != 0; i += 2)
			{
				if ( properties[i] == CL_QUEUE_PROPERTIES )
				{
					prop = properties[i+1];
					break;
				}
			}
		}
		return clCreateCommandQueue( context, device, prop, errcode_ret );
	}

#endif
	
# if COMPUTE_API_OPENCL < 120
/*
=================================================
	clCreateImage
=================================================
*/
	cl_mem clCreateImage (cl_context context,
  						  cl_mem_flags flags,
  						  const cl_image_format *image_format,
  						  const cl_image_desc *image_desc,
  						  void *host_ptr,
  						  cl_int *errcode_ret)
	{
		const bool		is_3d = image_desc->image_depth > 1 or image_desc->image_array_size > 1;
		const size_t	depth = ::Engine::GXMath::Max( image_desc->image_depth, image_desc->image_array_size );

		if ( is_3d )
		{
			return clCreateImage3D( context, flags, image_format, image_desc->image_width, image_desc->image_height,
									depth, image_desc->image_row_pitch, image_desc->image_slice_pitch,
									host_ptr, errcode_ret );
		}
		else
		{
			return clCreateImage2D( context, flags, image_format, image_desc->image_width, image_desc->image_height,
									image_desc->image_row_pitch, host_ptr, errcode_ret );
		}
	}
	
#  if defined( GRAPHICS_API_OPENGL )
/*
=================================================
	clCreateFromGLTexture
=================================================
*/
	cl_mem clCreateFromGLTexture (cl_context context,
  								  cl_mem_flags flags,
  								  cl_GLenum texture_target,
  								  cl_GLint miplevel,
  								  cl_GLuint texture,
  								  cl_int *errcode_ret)
	{
		using namespace gl;

		switch ( texture_target )
		{
			case GL_TEXTURE_2D :
			case GL_TEXTURE_CUBE_MAP_POSITIVE_X :
			case GL_TEXTURE_CUBE_MAP_POSITIVE_Y :
			case GL_TEXTURE_CUBE_MAP_POSITIVE_Z :
			case GL_TEXTURE_CUBE_MAP_NEGATIVE_X :
			case GL_TEXTURE_CUBE_MAP_NEGATIVE_Y :
			case GL_TEXTURE_CUBE_MAP_NEGATIVE_Z :
				return clCreateFromGLTexture2D( context, flags, texture_target, miplevel, texture, errcode_ret );

			case GL_TEXTURE_2D_ARRAY :
			case GL_TEXTURE_3D :
				return clCreateFromGLTexture3D( context, flags, texture_target, miplevel, texture, errcode_ret );
		}

		*errcode_ret = CL_INVALID_VALUE;
		return null;
	}

#  endif

/*
=================================================
	clEnqueueMarkerWithWaitList
=================================================
*/
	cl_int clEnqueueMarkerWithWaitList (cl_command_queue command_queue,
  										cl_uint num_events_in_wait_list,
  										const cl_event *event_wait_list,
  										cl_event *event)
	{
		if ( num_events_in_wait_list > 0 )
		{
			cl_int err = clEnqueueWaitForEvents( command_queue, num_events_in_wait_list, event_wait_list );

			if ( err != CL_SUCCESS )
				return err;
		}
		return clEnqueueMarker( command_queue, event );
	}

/*
=================================================
	clEnqueueBarrierWithWaitList
=================================================
*/
	cl_int clEnqueueBarrierWithWaitList (cl_command_queue command_queue,
  										 cl_uint num_events_in_wait_list,
  										 const cl_event *event_wait_list,
  										 cl_event *event)
	{
		if ( event != null )
			*event = null;

		if ( num_events_in_wait_list > 0 )
		{
			cl_int err = clEnqueueWaitForEvents( command_queue, num_events_in_wait_list, event_wait_list );

			if ( err != CL_SUCCESS )
				return err;
		}
		return clEnqueueBarrier( command_queue );
	}
	
/*
=================================================
	clEnqueueFillBuffer
=================================================
*/
	cl_int clEnqueueFillBuffer (cl_command_queue command_queue,
  								cl_mem buffer,
  								const void *pattern,
  								size_t pattern_size,
  								size_t offset,
  								size_t size,
  								cl_uint num_events_in_wait_list,
  								const cl_event *event_wait_list,
  								cl_event *event)
	{
		if ( event != null )
			*event = null;

		return -1000;
	}
	
/*
=================================================
	clEnqueueFillImage
=================================================
*/
	cl_int clEnqueueFillImage  (cl_command_queue command_queue,
  								cl_mem image,
  								const void *fill_color,
  								const size_t *origin,
  								const size_t *region,
  								cl_uint num_events_in_wait_list,
  								const cl_event *event_wait_list,
  								cl_event *event)
	{
		if ( event != null )
			*event = null;

		return -1000;
	}

#endif


}	// cl

#endif	// COMPUTE_API_OPENCL
