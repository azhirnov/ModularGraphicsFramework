// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Engine/Config/Engine.Config.h"

#ifdef COMPUTE_API_OPENCL

#include "cl2.h"
#include "Engine/STL/ThreadSafe/Singleton.h"

namespace cl
{

# ifdef COMPILER_MSVC
#	pragma warning (push)
#	pragma warning (disable : 4100)	// 'identifier' : unreferenced formal parameter
# endif
	
	// get function address from driver //
#	define OPENCL1_GET_PROC( _p_, _n_, _d_ ) \
		++counter; \
		if ( (_n_ = (_p_)CL1_GetProcAddress( TOSTRING( _n_ ) )) == null ) \
		{ \
			_n_ = _d_; \
			missing++; \
		}

	// Default Functions //
#	undef  CL1_BUILD_DEFFUNC
#	define CL1_BUILD_DEFFUNC( _retType_, _funcName_, _funcParams_, _retValue_ ) \
		_retType_ CL_API_CALL clDefault##_funcName_ _funcParams_ \
		{ \
			using namespace Engine; \
			LOG( "Used dummy function cl" TOSTRING( _funcName_ ), ELog::Debug ); \
			return _retValue_; \
		}
	
	// pointers to functions //
#	undef  CL1_BUILDFUNC
#	define CL1_BUILDFUNC( _retType_, _funcName_, _funcParams_, _retValue_ ) \
		PFNCL##_funcName_##PROC		cl##_funcName_ = &clDefault##_funcName_;
	

	CL1_CORE_FUNCTIONS( CL1_BUILD_DEFFUNC )
	CL1_CORE_FUNCTIONS( CL1_BUILDFUNC )

	CL1_GL_FUNCTIONS( CL1_BUILD_DEFFUNC )
	CL1_GL_FUNCTIONS( CL1_BUILDFUNC )
		
# ifdef COMPILER_MSVC
#	pragma warning (pop)
# endif

/*
=================================================
	LibOpenCL
=================================================
*/
	struct LibOpenCL
	{
	// types
	private:
		using Func_t	= GX_STL::OS::Library::Func_t;

	// variables
	private:
		GX_STL::OS::Library		_library;

	// methods
	public:
		LibOpenCL ()
		{}

		bool Load ()
		{
			CHECK_ERR( _library.Load( GetDefaultName() ) );
			return true;
		}

		void Unload ()
		{
			_library.Unload();
		}
		
		Func_t GetProc (const char * address) const
		{
			Func_t res = null;

			if ( (res = _library.GetProc( address )) != null )
				return res;

			return null;
		}

		static LibOpenCL * Instance ()
		{
			return Engine::SingletonMultiThread::Instance< LibOpenCL >();
		}

#		if defined( PLATFORM_WINDOWS )
		static Engine::StringCRef  GetDefaultName ()
		{
			return "OpenCL.dll";
		}
#		endif
	};
	
/*
=================================================
	CL1_GetProcAddress
=================================================
*/
	GX_STL::SharedLibFunction_t  CL1_GetProcAddress (const char *name)
	{
		using namespace Engine;
		SharedLibFunction_t	ptr = LibOpenCL::Instance()->GetProc( name );

		DEBUG_ONLY(
		if ( ptr == null ) {
			LOG( (String("Can't get address of ") << name).cstr(), ELog::Debug );
		})
		return ptr;
	}

/*
=================================================
	CL1_Init
=================================================
*/
	bool CL1_Init ()
	{
		using namespace Engine;

		int	missing = 0;	// number of missing function addresses
		int counter = 0;	// number of all functions
		
		CHECK_ERR( LibOpenCL::Instance()->Load() );
		
#		define CL1_GETPROC( _retType_, _funcName_, _funcParams_, _retValue_ ) \
			OPENCL1_GET_PROC( PFNCL##_funcName_##PROC,	cl##_funcName_,	&clDefault##_funcName_ );
		
		CL1_CORE_FUNCTIONS( CL1_GETPROC )
		CL1_GL_FUNCTIONS( CL1_GETPROC )

#		undef CL1_GETPROC


		if ( missing > 0 )
		{
			LOG( (String("While get address of OpenCL functions: ") <<
				 missing << " missing, " << (counter - missing) << " successfully.").cstr(),
				 ELog::Debug );
		}

		return true;
	}

/*
=================================================
	CL1_Delete
=================================================
*/
	void CL1_Delete ()
	{
		LibOpenCL::Instance()->Unload();
	}

/*
=================================================
	CL1_CheckErrors
=================================================
*/
	bool CL1_CheckErrors (cl_int errorCode, const char *clcall, const char *func, const char *file, int line)
	{
		using namespace Engine;

		if ( errorCode == CL_SUCCESS )
			return true;

		static const Map< cl_int, String >	errorCodes = {
			{ CL_DEVICE_NOT_FOUND, "CL_DEVICE_NOT_FOUND" },
			{ CL_DEVICE_NOT_AVAILABLE, "CL_DEVICE_NOT_AVAILABLE" },
			{ CL_COMPILER_NOT_AVAILABLE, "CL_COMPILER_NOT_AVAILABLE" },
			{ CL_MEM_OBJECT_ALLOCATION_FAILURE, "CL_MEM_OBJECT_ALLOCATION_FAILURE" },
			{ CL_OUT_OF_RESOURCES, "CL_OUT_OF_RESOURCES" },
			{ CL_OUT_OF_HOST_MEMORY, "CL_OUT_OF_HOST_MEMORY" },
			{ CL_PROFILING_INFO_NOT_AVAILABLE, "CL_PROFILING_INFO_NOT_AVAILABLE" },
			{ CL_MEM_COPY_OVERLAP, "CL_MEM_COPY_OVERLAP" },
			{ CL_IMAGE_FORMAT_MISMATCH, "CL_IMAGE_FORMAT_MISMATCH" },
			{ CL_IMAGE_FORMAT_NOT_SUPPORTED, "CL_IMAGE_FORMAT_NOT_SUPPORTED" },
			{ CL_BUILD_PROGRAM_FAILURE, "CL_BUILD_PROGRAM_FAILURE" },
			{ CL_MAP_FAILURE, "CL_MAP_FAILURE" },
			{ CL_INVALID_VALUE, "CL_INVALID_VALUE" },
			{ CL_INVALID_DEVICE_TYPE, "CL_INVALID_DEVICE_TYPE" },
			{ CL_INVALID_PLATFORM, "CL_INVALID_PLATFORM" },
			{ CL_INVALID_DEVICE, "CL_INVALID_DEVICE" },
			{ CL_INVALID_CONTEXT, "CL_INVALID_CONTEXT" },
			{ CL_INVALID_QUEUE_PROPERTIES, "CL_INVALID_QUEUE_PROPERTIES" },
			{ CL_INVALID_COMMAND_QUEUE, "CL_INVALID_COMMAND_QUEUE" },
			{ CL_INVALID_HOST_PTR, "CL_INVALID_HOST_PTR" },
			{ CL_INVALID_MEM_OBJECT, "CL_INVALID_MEM_OBJECT" },
			{ CL_INVALID_IMAGE_FORMAT_DESCRIPTOR, "CL_INVALID_IMAGE_FORMAT_DESCRIPTOR" },
			{ CL_INVALID_IMAGE_SIZE, "CL_INVALID_IMAGE_SIZE" },
			{ CL_INVALID_SAMPLER, "CL_INVALID_SAMPLER" },
			{ CL_INVALID_BINARY, "CL_INVALID_BINARY" },
			{ CL_INVALID_BUILD_OPTIONS, "CL_INVALID_BUILD_OPTIONS" },
			{ CL_INVALID_PROGRAM, "CL_INVALID_PROGRAM" },
			{ CL_INVALID_PROGRAM_EXECUTABLE, "CL_INVALID_PROGRAM_EXECUTABLE" },
			{ CL_INVALID_KERNEL_NAME, "CL_INVALID_KERNEL_NAME" },
			{ CL_INVALID_KERNEL_DEFINITION, "CL_INVALID_KERNEL_DEFINITION" },
			{ CL_INVALID_KERNEL, "CL_INVALID_KERNEL" },
			{ CL_INVALID_ARG_INDEX, "CL_INVALID_ARG_INDEX" },
			{ CL_INVALID_ARG_VALUE, "CL_INVALID_ARG_VALUE" },
			{ CL_INVALID_ARG_SIZE, "CL_INVALID_ARG_SIZE" },
			{ CL_INVALID_KERNEL_ARGS, "CL_INVALID_KERNEL_ARGS" },
			{ CL_INVALID_WORK_DIMENSION, "CL_INVALID_WORK_DIMENSION" },
			{ CL_INVALID_WORK_GROUP_SIZE, "CL_INVALID_WORK_GROUP_SIZE" },
			{ CL_INVALID_WORK_ITEM_SIZE, "CL_INVALID_WORK_ITEM_SIZE" },
			{ CL_INVALID_GLOBAL_OFFSET, "CL_INVALID_GLOBAL_OFFSET" },
			{ CL_INVALID_EVENT_WAIT_LIST, "CL_INVALID_EVENT_WAIT_LIST" },
			{ CL_INVALID_EVENT, "CL_INVALID_EVENT" },
			{ CL_INVALID_OPERATION, "CL_INVALID_OPERATION" },
			{ CL_INVALID_GL_OBJECT, "CL_INVALID_GL_OBJECT" },
			{ CL_INVALID_BUFFER_SIZE, "CL_INVALID_BUFFER_SIZE" },
			{ CL_INVALID_MIP_LEVEL, "CL_INVALID_MIP_LEVEL" },
			{ CL_INVALID_GLOBAL_WORK_SIZE, "CL_INVALID_GLOBAL_WORK_SIZE" },
			{ CL_INVALID_PROPERTY, "CL_INVALID_PROPERTY" },
#		if COMPUTE_API_OPENCL >= 120
			{ CL_INVALID_IMAGE_DESCRIPTOR, "CL_INVALID_IMAGE_DESCRIPTOR" },
			{ CL_INVALID_COMPILER_OPTIONS, "CL_INVALID_COMPILER_OPTIONS" },
			{ CL_INVALID_LINKER_OPTIONS, "CL_INVALID_LINKER_OPTIONS" },
			{ CL_INVALID_DEVICE_PARTITION_COUNT, "CL_INVALID_DEVICE_PARTITION_COUNT" },
#		endif
#		if COMPUTE_API_OPENCL >= 200
			{ CL_INVALID_PIPE_SIZE, "CL_INVALID_PIPE_SIZE" },
			{ CL_INVALID_DEVICE_QUEUE, "CL_INVALID_DEVICE_QUEUE" },
#		endif
#		if defined( GRAPHICS_API_OPENGL )
			{ CL_INVALID_GL_SHAREGROUP_REFERENCE_KHR, "CL_INVALID_GL_SHAREGROUP_REFERENCE_KHR" },
#		endif
#		if 0
			{ CL_PLATFORM_NOT_FOUND_KHR, "CL_PLATFORM_NOT_FOUND_KHR" },
			{ CL_INVALID_D3D10_DEVICE_KHR, "CL_INVALID_D3D10_DEVICE_KHR" },
			{ CL_INVALID_D3D10_RESOURCE_KHR, "CL_INVALID_D3D10_RESOURCE_KHR" },
			{ CL_D3D10_RESOURCE_ALREADY_ACQUIRED_KHR, "CL_D3D10_RESOURCE_ALREADY_ACQUIRED_KHR" },
			{ CL_D3D10_RESOURCE_NOT_ACQUIRED_KHR, "CL_D3D10_RESOURCE_NOT_ACQUIRED_KHR" },
			{ CL_INVALID_D3D11_DEVICE_KHR, "CL_INVALID_D3D11_DEVICE_KHR" },
			{ CL_INVALID_D3D11_RESOURCE_KHR, "CL_INVALID_D3D11_RESOURCE_KHR" },
			{ CL_D3D11_RESOURCE_ALREADY_ACQUIRED_KHR, "CL_D3D11_RESOURCE_ALREADY_ACQUIRED_KHR" },
			{ CL_D3D11_RESOURCE_NOT_ACQUIRED_KHR, "CL_D3D11_RESOURCE_NOT_ACQUIRED_KHR" },
			{ CL_INVALID_D3D9_DEVICE_NV/*CL_INVALID_DX9_DEVICE_INTEL*/, "CL_INVALID_D3D9_DEVICE_NV/INTEL" },
			{ CL_INVALID_D3D9_RESOURCE_NV/*CL_INVALID_DX9_RESOURCE_INTEL*/, "CL_INVALID_DX9_RESOURCE_NV/INTEL" },
			{ CL_D3D9_RESOURCE_ALREADY_ACQUIRED_NV/*CL_DX9_RESOURCE_ALREADY_ACQUIRED_INTEL*/, "CL_D3D9_RESOURCE_ALREADY_ACQUIRED_NV/INTEL" },
			{ CL_D3D9_RESOURCE_NOT_ACQUIRED_NV/*CL_DX9_RESOURCE_NOT_ACQUIRED_INTEL*/, "CL_DX9_RESOURCE_NOT_ACQUIRED_NV/INTEL" },
			{ CL_EGL_RESOURCE_NOT_ACQUIRED_KHR, "CL_EGL_RESOURCE_NOT_ACQUIRED_KHR" },
			{ CL_INVALID_EGL_OBJECT_KHR, "CL_INVALID_EGL_OBJECT_KHR" },
			{ CL_INVALID_ACCELERATOR_INTEL, "CL_INVALID_ACCELERATOR_INTEL" },
			{ CL_INVALID_ACCELERATOR_TYPE_INTEL, "CL_INVALID_ACCELERATOR_TYPE_INTEL" },
			{ CL_INVALID_ACCELERATOR_DESCRIPTOR_INTEL, "CL_INVALID_ACCELERATOR_DESCRIPTOR_INTEL" },
			{ CL_ACCELERATOR_TYPE_NOT_SUPPORTED_INTEL, "CL_ACCELERATOR_TYPE_NOT_SUPPORTED_INTEL" },
			{ CL_INVALID_VA_API_MEDIA_ADAPTER_INTEL, "CL_INVALID_VA_API_MEDIA_ADAPTER_INTEL" },
			{ CL_INVALID_VA_API_MEDIA_SURFACE_INTEL, "CL_INVALID_VA_API_MEDIA_SURFACE_INTEL" },
			{ CL_VA_API_MEDIA_SURFACE_ALREADY_ACQUIRED_INTEL, "CL_VA_API_MEDIA_SURFACE_ALREADY_ACQUIRED_INTEL" },
			{ CL_VA_API_MEDIA_SURFACE_NOT_ACQUIRED_INTEL, "CL_VA_API_MEDIA_SURFACE_NOT_ACQUIRED_INTEL" },
#		endif
			{ -9999, "(NV) kernel: Illegal read or write to a buffer" }
		};

		String	msg( "OpenCL error: " );
		usize	idx = UMax;

		if ( errorCodes.FindIndex( errorCode, OUT idx ) )
			msg << errorCodes[idx].second;
		else
			msg << "code " << String().FormatI( errorCode, 10 );

		msg << ", in " << clcall;
		msg << ", function: " << func;

		LOG( msg, ELog::Error, file, line );
		return false;
	}


}	// cl

#endif	// COMPUTE_API_OPENCL
