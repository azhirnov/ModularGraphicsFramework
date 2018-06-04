// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/Platforms/OpenCL/120/cl2.h"

#ifdef COMPUTE_API_OPENCL

namespace cl
{

	// 120
# if COMPUTE_API_OPENCL < 200

	typedef cl_bitfield cl_queue_properties;

	cl_command_queue clCreateCommandQueueWithProperties (cl_context context,
  														 cl_device_id device,
  														 const cl_queue_properties *properties,
  														 cl_int *errcode_ret);

#endif
	
	// 110
# if COMPUTE_API_OPENCL < 120

	typedef struct _cl_image_desc {
		cl_mem_object_type image_type;
		size_t image_width;
		size_t image_height;
		size_t image_depth;
		size_t image_array_size;
		size_t image_row_pitch;
		size_t image_slice_pitch;
		cl_uint num_mip_levels;
		cl_uint num_samples;
		cl_mem mem_object;
	} cl_image_desc;

	cl_mem clCreateImage (cl_context context,
  						  cl_mem_flags flags,
  						  const cl_image_format *image_format,
  						  const cl_image_desc *image_desc,
  						  void *host_ptr,
  						  cl_int *errcode_ret);
	
#  if defined( GRAPHICS_API_OPENGL )
	cl_mem clCreateFromGLTexture (cl_context context,
  								  cl_mem_flags flags,
  								  cl_GLenum texture_target,
  								  cl_GLint miplevel,
  								  cl_GLuint texture,
  								  cl_int * errcode_ret);
#  endif

	cl_int clEnqueueMarkerWithWaitList (cl_command_queue command_queue,
  										cl_uint num_events_in_wait_list,
  										const cl_event *event_wait_list,
  										cl_event *event);

	cl_int clEnqueueBarrierWithWaitList (cl_command_queue command_queue,
  										 cl_uint num_events_in_wait_list,
  										 const cl_event *event_wait_list,
  										 cl_event *event);

	cl_int clEnqueueFillBuffer (cl_command_queue command_queue,
  								cl_mem buffer,
  								const void *pattern,
  								size_t pattern_size,
  								size_t offset,
  								size_t size,
  								cl_uint num_events_in_wait_list,
  								const cl_event *event_wait_list,
  								cl_event *event);

	cl_int clEnqueueFillImage  (cl_command_queue command_queue,
  								cl_mem image,
  								const void *fill_color,
  								const size_t *origin,
  								const size_t *region,
  								cl_uint num_events_in_wait_list,
  								const cl_event *event_wait_list,
  								cl_event *event);

#endif


}	// cl

#endif	// COMPUTE_API_OPENCL
