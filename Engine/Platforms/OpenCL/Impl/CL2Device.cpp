// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Engine/Platforms/OpenCL/Impl/CL2Device.h"

#if defined( COMPUTE_API_OPENCL )

# if defined( PLATFORM_WINDOWS )
#	include "Engine/STL/OS/Windows/WinHeader.h"
# endif

namespace Engine
{
namespace PlatformCL
{
	using namespace cl;
	
/*
=================================================
	constructor
=================================================
*/
	CL2Device::CL2Device (GlobalSystemsRef gs) :
		BaseObject( gs ),
		_platform( null ),			_device( null ),
		_context( null ),			_queue( null ),
		_version( 0 ),				_isShared( false ),
		_clLibInitialized( false )
	{
	}
	
/*
=================================================
	destructor
=================================================
*/
	CL2Device::~CL2Device ()
	{
		ASSERT( not HasPlatform() );
		ASSERT( not HasDevice() );
	}
	
/*
=================================================
	CreateDevice
=================================================
*/
	bool CL2Device::CreateDevice (cl_device_id id)
	{
		CHECK_ERR( not HasPlatform() );
		CHECK_ERR( not HasDevice() );

		//Deinitialize();

		CHECK_ERR( _LoadFunctions() );
		CHECK_ERR( id != null );
		
		// check is 'id' is valid device
		cl_bool		dev_available = false;
		CL_CHECK( clGetDeviceInfo( id, CL_DEVICE_AVAILABLE, sizeof(dev_available), &dev_available, null ) );
		CHECK_ERR( dev_available );
		_device	= id;

		// get platform for this device
		CL_CHECK( clGetDeviceInfo( id, CL_DEVICE_PLATFORM, sizeof(_platform), &_platform, null ) );
		CHECK_ERR( _platform != null );
		
		//CHECK_ERR( _ChoosePlatform( version, deviceName ) );
		//CHECK_ERR( _ChooseDevice( version, deviceName ) );

		CHECK_ERR( _CheckVersion() );
		return true;
	}
	
/*
=================================================
	Deinitialize
=================================================
*/
	void CL2Device::Deinitialize ()
	{
		_DeleteQueue();

		if ( _context != null )
		{
			CL_CALL( clReleaseContext( _context ) );
			_context = null;
		}

		_platform			= null;
		_device				= null;
		_version			= 0;

		_isShared			= false;
		_clLibInitialized	= false;

		CL2_Delete();
	}
	
/*
=================================================
	_ChoosePlatform
=================================================
*
	bool CL2Device::_ChoosePlatform (uint version, StringCRef deviceName)
	{
		Array< cl_platform_id >		platforms;
		String						info;		info.Reserve( 1 << 9 );
		cl_uint						num_platforms = 0;

		CL_CHECK( clGetPlatformIDs( 0, null, &num_platforms ) );
		CHECK_ERR( num_platforms > 0 );

		platforms.Resize( num_platforms );

		CL_CHECK( clGetPlatformIDs( num_platforms, platforms.ptr(), null ) );

		cl_platform_id	name_match_platform		= null;
		cl_platform_id	ver_match_platform		= null;
		cl_platform_id	max_version_platform	= null;
		uint			max_version				= 0;
		
		// search platform for name, version and type
		FOR( i, platforms )
		{
			// profile
			CL_CALL( clGetPlatformInfo( platforms[i], CL_PLATFORM_PROFILE, info.Capacity(), info.ptr(), null ) );
			info.CalculateLength();

			if ( info != "FULL_PROFILE" )
				continue;
			
			// name
			CL_CALL( clGetPlatformInfo( platforms[i], CL_PLATFORM_NAME, info.Capacity(), info.ptr(), null ) );
			info.CalculateLength();
			
			if ( info.HasSubStringIC( deviceName ) )
			{
				name_match_platform = platforms[i];
			}

			// version
			CL_CALL( clGetPlatformInfo( platforms[i], CL_PLATFORM_VERSION, info.Capacity(), info.ptr(), null ) );
			info.CalculateLength();

			// OpenCL x.x ....
			uint	cl_ver		= 0;
			usize	major_pos	= 0;
			usize	minor_pos	= 0;
			
			if ( info.Find( ' ', OUT major_pos ) and info.Find( '.', OUT minor_pos ) )
			{
				ASSERT( major_pos >= /*OpenCL* /6 and major_pos < minor_pos );

				cl_ver = StringUtils::ToInt32( info.SubString(minor_pos+1) );
				cl_ver = cl_ver > 10 ? (cl_ver % 100) : (cl_ver % 10) * 10;
				cl_ver += (StringUtils::ToInt32(info.SubString(major_pos+1)) % 10) * 100;

				if ( cl_ver > max_version ) {
					max_version_platform = platforms[i];
					max_version			 = cl_ver;
				}

				if ( ver_match_platform == null and cl_ver == version )
					ver_match_platform = platforms[i];
			}
		}

		// Choose best platform
		/*if ( name_match_platform )
			_platform = name_match_platform;
		else
		if ( ver_match_platform )
			_platform = ver_match_platform;
		else* /
		if ( max_version_platform )
			_platform = max_version_platform;
		else
			_platform = platforms.Front();

		CHECK_ERR( _platform != null );
		return true;
	}
	
/*
=================================================
	_ChooseDevice
=================================================
*
	bool CL2Device::_ChooseDevice (uint version, StringCRef deviceName)
	{
		Array< cl_device_id >		devices;
		String						info;		info.Reserve( 1 << 9 );
		cl_uint						num_devices = 0;
	
		CL_CHECK( clGetDeviceIDs( _platform, CL_DEVICE_TYPE_ALL, 0, null, &num_devices ) );
		CHECK_ERR( num_devices != 0 );
		
		devices.Resize( num_devices );

		CL_CHECK( clGetDeviceIDs( _platform, CL_DEVICE_TYPE_ALL, num_devices, devices.ptr(), null ) );

		cl_device_id	gpu_device			= null;
		cl_device_id	cpu_device			= null;
		cl_device_id	name_match_device	= null;

		// search device for name and type
		FOR( i, devices )
		{
			cl_device_type	dev_type = 0;

			CL_CALL( clGetDeviceInfo( devices[i], CL_DEVICE_TYPE, sizeof(dev_type), &dev_type, null ) );

			if ( gpu_device == null and (dev_type & CL_DEVICE_TYPE_GPU) )
				gpu_device = devices[i];

			if ( cpu_device == null and (dev_type & CL_DEVICE_TYPE_CPU) )
				cpu_device = devices[i];
				
			// name
			CL_CALL( clGetDeviceInfo( devices[i], CL_DEVICE_NAME, info.Capacity(), info.ptr(), null ) );
			info.CalculateLength();

			if ( name_match_device == null and info.HasSubStringIC( deviceName ) )
			{
				name_match_device = devices[i];
			}
				
			// vendor name
			CL_CALL( clGetDeviceInfo( devices[i], CL_DEVICE_VENDOR, info.Capacity(), info.ptr(), null ) );
			info.CalculateLength();
				
			if ( name_match_device == null and info.HasSubStringIC( deviceName ) )
			{
				name_match_device = devices[i];
			}
		}

		// choose best device
		if ( name_match_device )
			_device = name_match_device;
		else
		if ( gpu_device )
			_device = gpu_device;
		else
		if ( cpu_device )
			_device = cpu_device;
		else
			_device = devices.Front();

		CHECK_ERR( _device != null );
		return true;
	}
	
/*
=================================================
	GetDeviceInfo
=================================================
*/
	bool CL2Device::GetDeviceInfo (OUT Array<DeviceInfo> &deviceInfo) const
	{
		Array< cl_platform_id >		platforms;
		Array< cl_device_id >		devices;
		cl_uint						num_platforms	= 0;
		cl_uint						num_devices		= 0;
		cl_uint						value32			= 0;
		size_t						size			= 0;
		cl_ulong					value64			= 0;
		cl_bool						valueb			= false;
		String						info;			info.Reserve( 1 << 9 );

		CHECK_ERR( _LoadFunctions() );

		CL_CALL( clGetPlatformIDs( 0, null, &num_platforms ) );
		CHECK_ERR( num_platforms > 0 );
		
		platforms.Resize( num_platforms );

		CL_CALL( clGetPlatformIDs( num_platforms, platforms.ptr(), null ) );
		
		FOR( i, platforms )
		{
			if ( platforms[i] == null )
				continue;

			CL_CALL( clGetPlatformInfo( platforms[i], CL_PLATFORM_PROFILE, info.Capacity(), info.ptr(), null ) );
			info.CalculateLength();

			if ( info != "FULL_PROFILE" )
				continue;
			
			CL_CALL( clGetPlatformInfo( platforms[i], CL_PLATFORM_VERSION, info.Capacity(), info.ptr(), null ) );
			info.CalculateLength();

			// OpenCL x.x ....
			uint	cl_ver		= 0;
			usize	major_pos	= 0;
			usize	minor_pos	= 0;
			
			if ( info.Find( ' ', OUT major_pos ) and info.Find( '.', OUT minor_pos ) )
			{
				ASSERT( major_pos >= /*OpenCL*/6 and major_pos < minor_pos );

				cl_ver = StringUtils::ToInt32( info.SubString(minor_pos+1) );
				cl_ver = cl_ver > 10 ? (cl_ver % 100) : (cl_ver % 10) * 10;
				cl_ver += (StringUtils::ToInt32(info.SubString(major_pos+1)) % 10) * 100;
			}
			
			CL_CALL( clGetDeviceIDs( platforms[i], CL_DEVICE_TYPE_ALL, 0, null, &num_devices ) );
			CHECK_ERR( num_devices != 0 );
		
			devices.Resize( num_devices );

			CL_CALL( clGetDeviceIDs( platforms[i], CL_DEVICE_TYPE_ALL, num_devices, devices.ptr(), null ) );

			FOR( j, devices )
			{
				if ( devices[j] == null )
					continue;
				
				CL_CALL( clGetDeviceInfo( devices[j], CL_DEVICE_AVAILABLE, sizeof(valueb), &valueb, null ) );
				if ( not valueb )
					continue;

				cl_device_type	dev_type = 0;
				DeviceInfo		dev_info;

				dev_info.id			= devices[j];
				dev_info.version	= cl_ver;

				CL_CALL( clGetDeviceInfo( devices[j], CL_DEVICE_TYPE, sizeof(dev_type), &dev_type, null ) );
				dev_info.isGPU	= !!(dev_type & CL_DEVICE_TYPE_GPU);
				dev_info.isCPU	= !!(dev_type & CL_DEVICE_TYPE_CPU);
				
				CL_CALL( clGetDeviceInfo( devices[j], CL_DEVICE_NAME, info.Capacity(), info.ptr(), null ) );
				dev_info.device	= info.ptr();
				
				CL_CALL( clGetDeviceInfo( devices[j], CL_DEVICE_VENDOR, info.Capacity(), info.ptr(), null ) );
				dev_info.vendor = info.ptr();
				
				CL_CALL( clGetDeviceInfo( devices[j], CL_DEVICE_MAX_CLOCK_FREQUENCY, sizeof(value32), &value32, null ) );
				dev_info.frequencyMHz = value32;
				
				CL_CALL( clGetDeviceInfo( devices[j], CL_DEVICE_MAX_COMPUTE_UNITS, sizeof(value32), &value32, null ) );
				dev_info.computeUnits = value32;
		
				CL_CALL( clGetDeviceInfo( devices[j], CL_DEVICE_ADDRESS_BITS, sizeof(value32), &value32, null ) );
				dev_info.is64bit = (value32 >= 64);

				CL_CALL( clGetDeviceInfo( devices[j], CL_DEVICE_MAX_WORK_GROUP_SIZE, sizeof(size), &size, null ) );
				dev_info.maxInvocations = size;
		
				CL_CALL( clGetDeviceInfo( devices[j], CL_DEVICE_GLOBAL_MEM_SIZE, sizeof(value64), &value64, null ) );
				dev_info.globalMemory.SetBytes( value64 );
		
				CL_CALL( clGetDeviceInfo( devices[j], CL_DEVICE_ENDIAN_LITTLE, sizeof(valueb), &valueb, null ) );
				dev_info.littleEndian = !!valueb;
				
				CL_CALL( clGetDeviceInfo( devices[j], CL_DEVICE_IMAGE_SUPPORT, sizeof(valueb), &valueb, null ) );
				dev_info.imageSupported = !!valueb;

				CL_CALL( clGetDeviceInfo( devices[j], CL_DEVICE_HOST_UNIFIED_MEMORY, sizeof(valueb), &valueb, null ) );
				dev_info.unifiedMemory = !!valueb;

				CL_CALL( clGetDeviceInfo( devices[j], CL_DEVICE_COMPILER_AVAILABLE, sizeof(valueb), &valueb, null ) );
				dev_info.compilerSupported = !!valueb;

				cl_device_exec_capabilities	cap;
				CL_CALL( clGetDeviceInfo( devices[j], CL_DEVICE_EXECUTION_CAPABILITIES, sizeof(cap), &cap, null ) );
				dev_info.compilerSupported &= !!(cap & CL_EXEC_KERNEL);

				CL_CALL( clGetDeviceInfo( devices[j], CL_DEVICE_LINKER_AVAILABLE, sizeof(valueb), &valueb, null ) );
				dev_info.compilerSupported &= !!valueb;
				
				CL_CALL( clGetDeviceInfo( devices[j], CL_DEVICE_EXTENSIONS, 0, null, &size ) );
				info.Resize( size );
				
				CL_CALL( clGetDeviceInfo( devices[j], CL_DEVICE_EXTENSIONS, info.Capacity(), info.ptr(), null ) );
				info.CalculateLength();
				
				dev_info.glSharing	= info.HasSubStringIC("cl_khr_gl_sharing");
				dev_info.dxSharing	= info.HasSubStringIC("cl_khr_d3d11_sharing");

				deviceInfo.PushBack( RVREF(dev_info) );
			}
		}

		CHECK_ERR( not deviceInfo.Empty() );
		return true;
	}
	
/*
=================================================
	_LoadFunctions
=================================================
*/
	bool CL2Device::_LoadFunctions () const
	{
		if ( _clLibInitialized )
			return true;

		CHECK_ERR( CL2_Init() );
		_clLibInitialized = true;

		return true;
	}

/*
=================================================
	CreateQueue
=================================================
*/
	bool CL2Device::CreateQueue ()
	{
		CHECK_ERR( HasPlatform() and HasDevice() and HasContext() );

		_DeleteQueue();

		cl_int	cl_err = 0;
		CL_CALL( (_queue = clCreateCommandQueueWithProperties( _context, _device, 0, &cl_err )), cl_err );
		CHECK_ERR( _queue != null );

		return true;
	}
	
/*
=================================================
	_DeleteQueue
=================================================
*/
	void CL2Device::_DeleteQueue ()
	{
		if ( _queue == null )
			return;

		CL_CALL( clReleaseCommandQueue( _queue ) );
		_queue = null;
	}
	
/*
=================================================
	_CheckVersion
=================================================
*/
	bool CL2Device::_CheckVersion ()
	{
		char	buf[256] = {};

		CL_CALL( clGetPlatformInfo( _platform, CL_PLATFORM_VERSION, CountOf(buf), buf, null ) );

		_version = (uint) RoundToInt( StringUtils::ToDouble( StringCRef( buf ).SubString( /*OpenCL*/6 ) ) * 100.0 );

		CHECK_ERR( _version >= MIN_VERSION );
		return true;
	}
	

# if defined( PLATFORM_WINDOWS ) and defined( GRAPHICS_API_OPENGL )
/*
=================================================
	CreateGLSharedContext
=================================================
*/
	bool CL2Device::CreateGLSharedContext ()
	{
		CHECK_ERR( HasPlatform() and HasDevice() );

		HDC		dc	= wglGetCurrentDC();
		HGLRC	rc	= wglGetCurrentContext();

		cl_context_properties properties[] =
		{
			CL_CONTEXT_PLATFORM,	(cl_context_properties) _platform,
			CL_GL_CONTEXT_KHR,		(cl_context_properties) rc,
			CL_WGL_HDC_KHR,			(cl_context_properties) dc,
			0,	0
		};

		if ( rc == null or dc == null )
			properties[2] = properties[3] = 0;
		
		cl_int	cl_err = 0;
		CL_CALL( (_context = clCreateContext( properties, 1, &_device, null, null, &cl_err )), cl_err );
		CHECK_ERR( _context != null );

		_isShared = (rc != null and dc != null);
		return true;
	}

# elif defined( PLATFORM_BASE_POSIX ) and defined( GRAPHICS_API_OPENGL )
/*
=================================================
	CreateGLSharedContext
=================================================
*/
	bool CL2Device::CreateGLSharedContext ()
	{
		CHECK_ERR( HasPlatform() and HasDevice() );

		Display *	display	= glXGetCurrentDisplay();
		GLXContext	context	= glXGetCurrentContext();

		cl_context_properties properties[] =
		{
			CL_CONTEXT_PLATFORM,	(cl_context_properties) _platform,
			CL_GL_CONTEXT_KHR,		(cl_context_properties) context,
			CL_GLX_DISPLAY_KHR,		(cl_context_properties) display,
			0,	0
		};

		if ( rc == null or dc == null )
			properties[2] = properties[3] = 0;
		
		cl_int	cl_err = 0;
		CL_CALL( (_context = clCreateContext( properties, 1, &_device, null, null, &cl_err )), cl_err );
		CHECK_ERR( _context != null );

		_isShared = (rc != null and dc != null);
		return true;
	}

#endif

/*
=================================================
	CreateContext
=================================================
*/
	bool CL2Device::CreateContext ()
	{
		CHECK_ERR( HasPlatform() and HasDevice() );

		cl_context_properties properties[] =
		{
			CL_CONTEXT_PLATFORM,	(cl_context_properties) _platform,
			0,	0
		};
		
		cl_int	cl_err = 0;
		CL_CALL( (_context = clCreateContext( properties, 1, &_device, null, null, &cl_err )), cl_err );
		CHECK_ERR( _context != null );

		_isShared = false;
		return true;
	}
	
/*
=================================================
	Synchronize
=================================================
*/
	void CL2Device::Synchronize ()
	{
		CL_CALL( clFinish( _queue ) );
	}
	
/*
=================================================
	Barrier
=================================================
*/
	void CL2Device::Barrier ()
	{
		CL_CALL( clEnqueueBarrierWithWaitList( _queue, 0, null, null ) );
	}

/*
=================================================
	WriteInfo
=================================================
*/
	bool CL2Device::WriteInfo ()
	{
		CHECK_ERR( HasPlatform() and HasDevice() );

		String		log;
		String		info;
		cl_bool		valueb   = false;
		cl_uint		value32  = 0;
		cl_ulong	value64	 = 0;
		size_t		size	 = 0;
		usize3		size3;

		info.Reserve( 1 << 9 );

		log << "OpenCL info\n---------------";

		clGetPlatformInfo( _platform, CL_PLATFORM_NAME, info.Capacity(), info.ptr(), null );
		log << "\nPlatform:         " << info.cstr();
		
		clGetPlatformInfo( _platform, CL_PLATFORM_VERSION, info.Capacity(), info.ptr(), null );
		log << "\nVersion:          " << info.cstr();
		
		clGetDeviceInfo( _device, CL_DEVICE_NAME, info.Capacity(), info.ptr(), null );
		log << "\nDevice:           " << info.cstr();
		
		clGetDeviceInfo( _device, CL_DRIVER_VERSION, info.Capacity(), info.ptr(), null );
		log << "\nDriver:           " << info.cstr();
		
		CL_CALL( clGetDeviceInfo( _device, CL_DEVICE_MAX_CLOCK_FREQUENCY, sizeof(value32), &value32, null ) );
		log << "\nFrequency:        " << value32 << " MHz";

		CL_CALL( clGetDeviceInfo( _device, CL_DEVICE_MAX_COMPUTE_UNITS, sizeof(value32), &value32, null ) );
		log << "\nCompute Units:    " << value32;

		CL_CALL( clGetDeviceInfo( _device, CL_DEVICE_LOCAL_MEM_SIZE, sizeof(value64), &value64, null ) );
		log << "\nLocal Mem:        " << ToString( Bytes<ulong>::FromBytes( value64 ) );

		CL_CALL( clGetDeviceInfo( _device, CL_DEVICE_MAX_WORK_GROUP_SIZE, sizeof(size), &size, null ) );
		log << "\nMax invocations:  " << size;
		
		CL_CALL( clGetDeviceInfo( _device, CL_DEVICE_MAX_WORK_ITEM_SIZES, sizeof(size3), size3.ptr(), null ) );
		log << "\nMax local groups: " << ToString( size3 );

		CL_CALL( clGetDeviceInfo( _device, CL_DEVICE_ADDRESS_BITS, sizeof(value32), &value32, null ) );
		log << "\nAddress bits:     " << value32;

		CL_CALL( clGetDeviceInfo( _device, CL_DEVICE_GLOBAL_MEM_CACHE_SIZE, sizeof(value64), &value64, null ) );
		log << "\nGlobal Cache:     " << ToString( Bytes<ulong>::FromBytes( value64 ) );		// L1 cache

		CL_CALL( clGetDeviceInfo( _device, CL_DEVICE_GLOBAL_MEM_SIZE, sizeof(value64), &value64, null ) );
		log << "\nGlobal Mem:       " << ToString( Bytes<ulong>::FromBytes( value64 ) );
		_totalMemory.SetBytes( value64 );
		
		CL_CALL( clGetDeviceInfo( _device, CL_DEVICE_MAX_CONSTANT_BUFFER_SIZE, sizeof(value64), &value64, null ) );
		log << "\nConst Buffer:     " << ToString( Bytes<ulong>::FromBytes( value64 ) );

		CL_CALL( clGetDeviceInfo( _device, CL_DEVICE_MAX_MEM_ALLOC_SIZE, sizeof(value64), &value64, null ) );
		log << "\nMax alloc size:   " << ToString( Bytes<ulong>::FromBytes( value64 ) );
		
		CL_CALL( clGetDeviceInfo( _device, CL_DEVICE_MAX_CONSTANT_ARGS, sizeof(value32), &value32, null ) );
		log << "\nConst args:       " << value32;
		
		CL_CALL( clGetDeviceInfo( _device, CL_DEVICE_MAX_PARAMETER_SIZE, sizeof(size), &size, null ) );
		log << "\nParam size:       " << ToString( BytesU( size ) );

		CL_CALL( clGetDeviceInfo( _device, CL_DEVICE_ENDIAN_LITTLE, sizeof(valueb), &valueb, null ) );
		log << "\nEncoding:         " << ( valueb ? "Little endian" : "Big endian");
		
		CL_CALL( clGetDeviceInfo( _device, CL_DEVICE_MEM_BASE_ADDR_ALIGN, sizeof(value32), &value32, null ) );
		log << "\nMem align:        " << ToString( Bits<uint>( value32 ) );

		CL_CALL( clGetDeviceInfo( _device, CL_DEVICE_IMAGE2D_MAX_WIDTH, sizeof(size), &size, null ) );
		log << "\nImage2D width:    " << size << " px";
		
		CL_CALL( clGetDeviceInfo( _device, CL_DEVICE_IMAGE2D_MAX_HEIGHT, sizeof(size), &size, null ) );
		log << "\nImage2D height:   " << size << " px";
		
		CL_CALL( clGetDeviceInfo( _device, CL_DEVICE_IMAGE3D_MAX_WIDTH, sizeof(size), &size, null ) );
		log << "\nImage3D width:    " << size << " px";
		
		CL_CALL( clGetDeviceInfo( _device, CL_DEVICE_IMAGE3D_MAX_HEIGHT, sizeof(size), &size, null ) );
		log << "\nImage3D height:   " << size << " px";
		
		CL_CALL( clGetDeviceInfo( _device, CL_DEVICE_IMAGE3D_MAX_DEPTH, sizeof(size), &size, null ) );
		log << "\nImage3D depth:    " << size << " px";
		
#		if COMPUTE_API_OPENCL >= 120
		CL_CALL( clGetDeviceInfo( _device, CL_DEVICE_IMAGE_MAX_BUFFER_SIZE, sizeof(size), &size, null ) );
		log << "\nBuffer size:      " << size << " px";
		
		CL_CALL( clGetDeviceInfo( _device, CL_DEVICE_IMAGE_MAX_ARRAY_SIZE, sizeof(size), &size, null ) );
		log << "\nImageArray:       " << size << " layers";

		CL_CALL( clGetDeviceInfo( _device, CL_DEVICE_BUILT_IN_KERNELS, info.Capacity(), info.ptr(), null ) );
		log << "\nKernels:          " << info.cstr();
#		endif

		CL_CALL( clGetDeviceInfo( _device, CL_DEVICE_EXTENSIONS, 0, null, &size ) );
		info.Reserve( size );

		LOG( log.cstr(), ELog::Debug | ELog::SpoilerFlag );

		// write extensions
		log.Clear();
		log << "OpenCL Extensions\n---------------\n";

		CL_CALL( clGetDeviceInfo( _device, CL_DEVICE_EXTENSIONS, info.Capacity(), info.ptr(), null ) );
		info.CalculateLength();

		const usize	maxLineLength	= 100;
		usize		pos				= 0;
		usize		last_pos		= 0;

		for (; info.Find( ' ', OUT pos, pos+1 ); )
		{
			if ( pos - last_pos > maxLineLength )
			{
				log << info.SubString( last_pos, pos - last_pos ) << '\n';

				last_pos = pos+1;
			}
		}

		log << info.SubString( pos+1 );
		
		LOG( log.cstr(), ELog::Debug | ELog::SpoilerFlag );
		return true;
	}


}	// PlatformCL
}	// Engine

#endif	// COMPUTE_API_OPENCL
