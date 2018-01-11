// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "cl2.h"

#if defined( COMPUTE_API_OPENCL )

namespace cl
{

# ifdef COMPILER_MSVC
#	pragma warning (push)
#	pragma warning (disable : 4100)	// 'identifier' : unreferenced formal parameter
# endif
	
	// get function address from driver //
#	define OPENCL2_GET_PROC( _p_, _n_, _d_ ) \
		++counter; \
		if ( (_n_ = (_p_)CL2_GetProcAddress( TOSTRING( _n_ ) )) == null ) \
		{ \
			_n_ = _d_; \
			missing++; \
		}

	// Default Functions //
#	undef  CL2_BUILD_DEFFUNC
#	define CL2_BUILD_DEFFUNC( _retType_, _funcName_, _funcParams_, _retValue_ ) \
		_retType_ CL_API_CALL clDefault##_funcName_ _funcParams_ \
		{ \
			using namespace Engine; \
			LOG( "Used dummy function cl" TOSTRING( _funcName_ ), ELog::Debug ); \
			return _retValue_; \
		}
	
	// pointers to functions //
#	undef  CL2_BUILDFUNC
#	define CL2_BUILDFUNC( _retType_, _funcName_, _funcParams_, _retValue_ ) \
		PFNCL##_funcName_##PROC		cl##_funcName_ = &clDefault##_funcName_;
	

	CL2_CORE_FUNCTIONS( CL2_BUILD_DEFFUNC )
	CL2_CORE_FUNCTIONS( CL2_BUILDFUNC )

	CL2_GL_FUNCTIONS( CL2_BUILD_DEFFUNC )
	CL2_GL_FUNCTIONS( CL2_BUILDFUNC )
		
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
	CL2_GetProcAddress
=================================================
*/
	GX_STL::SharedLibFunction_t  CL2_GetProcAddress (const char *name)
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
	CL2_Init
=================================================
*/
	bool CL2_Init ()
	{
		using namespace Engine;

		int	missing = 0;	// number of missing function addresses
		int counter = 0;	// number of all functions
		
		CHECK_ERR( LibOpenCL::Instance()->Load() );
		
#		define CL2_GETPROC( _retType_, _funcName_, _funcParams_, _retValue_ ) \
			OPENCL2_GET_PROC( PFNCL##_funcName_##PROC,	cl##_funcName_,	&clDefault##_funcName_ );
		
		CL2_CORE_FUNCTIONS( CL2_GETPROC )
		CL2_GL_FUNCTIONS( CL2_GETPROC )

#		undef CL2_GETPROC


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
	CL2_Delete
=================================================
*/
	void CL2_Delete ()
	{
		LibOpenCL::Instance()->Unload();
	}

/*
=================================================
	CL2_CheckErrors
=================================================
*/
	bool CL2_CheckErrors (cl_int errorCode, const char *clcall, const char *func, const char *file, int line)
	{
		using namespace Engine;

		if ( errorCode == CL_SUCCESS )
			return true;

		static const char *	errorsNames[] =
		{
			"CL_SUCCESS",
			"CL_DEVICE_NOT_FOUND",
			"CL_DEVICE_NOT_AVAILABLE",
			"CL_COMPILER_NOT_AVAILABLE",
			"CL_MEM_OBJECT_ALLOCATION_FAILURE",
			"CL_OUT_OF_RESOURCES",
			"CL_OUT_OF_HOST_MEMORY",
			"CL_PROFILING_INFO_NOT_AVAILABLE",
			"CL_MEM_COPY_OVERLAP",
			"CL_IMAGE_FORMAT_MISMATCH",
			"CL_IMAGE_FORMAT_NOT_SUPPORTED",
			"CL_BUILD_PROGRAM_FAILURE",
			"CL_MAP_FAILURE",
			"13",
			"14",
			"15",
			"16",
			"17",
			"18",
			"19",
			"20",
			"21",
			"22",
			"23",
			"24",
			"25",
			"26",
			"27",
			"28",
			"29",
			"CL_INVALID_VALUE",
			"CL_INVALID_DEVICE_TYPE",
			"CL_INVALID_PLATFORM",
			"CL_INVALID_DEVICE",
			"CL_INVALID_CONTEXT",
			"CL_INVALID_QUEUE_PROPERTIES",
			"CL_INVALID_COMMAND_QUEUE",
			"CL_INVALID_HOST_PTR",
			"CL_INVALID_MEM_OBJECT",
			"CL_INVALID_IMAGE_FORMAT_DESCRIPTOR",
			"CL_INVALID_IMAGE_SIZE",
			"CL_INVALID_SAMPLER",
			"CL_INVALID_BINARY",
			"CL_INVALID_BUILD_OPTIONS",
			"CL_INVALID_PROGRAM",
			"CL_INVALID_PROGRAM_EXECUTABLE",
			"CL_INVALID_KERNEL_NAME",
			"CL_INVALID_KERNEL_DEFINITION",
			"CL_INVALID_KERNEL",
			"CL_INVALID_ARG_INDEX",
			"CL_INVALID_ARG_VALUE",
			"CL_INVALID_ARG_SIZE",
			"CL_INVALID_KERNEL_ARGS",
			"CL_INVALID_WORK_DIMENSION",
			"CL_INVALID_WORK_GROUP_SIZE",
			"CL_INVALID_WORK_ITEM_SIZE",
			"CL_INVALID_GLOBAL_OFFSET",
			"CL_INVALID_EVENT_WAIT_LIST",
			"CL_INVALID_EVENT",
			"CL_INVALID_OPERATION",
			"CL_INVALID_GL_OBJECT",
			"CL_INVALID_BUFFER_SIZE",
			"CL_INVALID_MIP_LEVEL",
			"CL_INVALID_GLOBAL_WORK_SIZE"
		};

		String	msg( "OpenCL error: " );

		if ( errorCode <= 0 and -errorCode < (isize)CountOf( errorsNames ) )
			msg << errorsNames[ -errorCode ];
		else
		{
			switch ( errorCode )
			{
#			if defined( GRAPHICS_API_OPENGL )
				case CL_INVALID_GL_SHAREGROUP_REFERENCE_KHR :	msg << "CL_INVALID_GL_SHAREGROUP_REFERENCE_KHR";	break;
#			endif
				default :										msg << "code " << String().FormatI( errorCode, 10 );
			}
		}

		msg << ", in " << clcall;
		msg << ", function: " << func;

		LOG( msg.cstr(), ELog::Error, file, line );
		return false;
	}


}	// cl

#endif	// COMPUTE_API_OPENCL
