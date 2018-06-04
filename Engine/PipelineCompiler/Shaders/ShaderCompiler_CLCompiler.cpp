// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Engine/PipelineCompiler/Shaders/glslang_include.h"
#include "Engine/PipelineCompiler/Shaders/ShaderCompiler.h"
#include "Engine/Platforms/OpenCL/120/CL1Messages.h"


namespace PipelineCompiler
{
# ifdef COMPUTE_API_OPENCL

	using namespace cl;

/*
=================================================
	_CompileCL
=================================================
*/
	bool ShaderCompiler::_CompileCL (const Config &, const _ShaderData &shader, OUT String &log, OUT BinaryArray &result)
	{
		CHECK_ERR( InitializeContext() );
		
		Message< GpuMsg::GetCLDeviceInfo >		req_dev;
		CHECK( _app->CLThread()->Send( req_dev ) );

		Array<const char*>	parts;

		FOR( i, shader.src ) {
			parts.PushBack( shader.src[i].cstr() );
		}
		
		const cl_device_id	devices[]	= { req_dev->result->device };
		usize				log_size	= 0;
		cl_int				cl_err		= 0;
		cl_program			prog_id		= null;
		static const char	options[]	=
			" -cl-single-precision-constant"
			" -cl-mad-enable"
			" -cl-denorms-are-zero"
			" -cl-finite-math-only"
			" -cl-std=CL1.2"
			;
		
		CL_CALL( (prog_id = clCreateProgramWithSource(
								req_dev->result->context,
								parts.Count(),
								parts.ptr(),
								null,
								&cl_err )), cl_err );
		CHECK_ERR( prog_id != null );

		cl_err = clBuildProgram( prog_id, (cl_uint)CountOf(devices), devices, options, null, null );
		const bool compiled = (cl_err == CL_SUCCESS);
		
		CL_CALL( clGetProgramBuildInfo( prog_id, devices[0], CL_PROGRAM_BUILD_LOG, 0, null, OUT &log_size ) );
		
		if ( log_size > 5 )
		{
			log.Reserve( log.Length() + log_size + 128 );

			CL_CALL( clGetProgramBuildInfo( prog_id, devices[0], CL_PROGRAM_BUILD_LOG, log_size, OUT log.End(), null ) );
			log.CalculateLength();
		}

		if ( compiled )
		{
			size_t	sizes[ CountOf(devices) ] = {};
			CL_CALL( clGetProgramInfo( prog_id, CL_PROGRAM_BINARY_SIZES, sizeof(sizes), OUT sizes, null ) );

			CHECK_ERR( sizes[0] > 0 );
			result.Resize( sizes[0], false );

			unsigned char* binaries[ CountOf(devices) ] = { result.ptr() };
			CL_CALL( clGetProgramInfo( prog_id, CL_PROGRAM_BINARIES, (size_t)result.Size(), OUT binaries, OUT sizes ) );
		}
		
		CL_CALL( clReleaseProgram( prog_id ) );

		return compiled;
	}
	
/*
=================================================
	_ValidateCLSource
=================================================
*/
	bool ShaderCompiler::_ValidateCLSource (EShader::type, StringCRef) const
	{
		TODO( "_ValidateCLSource" );
		return false;
	}
	
/*
=================================================
	_ValidateCLBinary
=================================================
*/
	bool ShaderCompiler::_ValidateCLBinary (EShader::type, BinArrayCRef) const
	{
		TODO( "_ValidateCLBinary" );
		return false;
	}

# else

	bool ShaderCompiler::_CompileCL (const Config &, const _ShaderData &shader, OUT String &log, OUT BinaryArray &result)
	{
		return false;
	}
	
	bool ShaderCompiler::_ValidateCLSource (EShader::type, StringCRef) const
	{
		return false;
	}
	
	bool ShaderCompiler::_ValidateCLBinary (EShader::type, BinArrayCRef) const
	{
		return false;
	}

# endif	// COMPUTE_API_OPENCL

}	// PipelineCompiler

