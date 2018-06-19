// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Core/STL/Common/Platforms.h"
#include "Core/Config/Engine.Config.h"

#ifdef COMPUTE_API_OPENCL

# if defined( PLATFORM_WINDOWS )
#	include "Core/STL/OS/Windows/WinHeader.h"
# endif

#include "Engine/Platforms/OpenCL/120/CL1Device.h"
#include "Engine/Platforms/OpenCL/120/CL1Messages.h"
#include "Core/STL/Log/ToString.h"

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
	CL1Device::CL1Device (GlobalSystemsRef gs) :
		BaseObject( gs ),
		_queue( null ),					_platform( null ),
		_device( null ),				_context( null ),				
		_sharedApi( ESharing::None ),	_version( 0 ),
		_clLibInitialized( false )
	{
	}
	
/*
=================================================
	destructor
=================================================
*/
	CL1Device::~CL1Device ()
	{
		ASSERT( not HasPlatform() );
		ASSERT( not HasDevice() );
	}
	
/*
=================================================
	CreateDevice
=================================================
*/
	bool CL1Device::CreateDevice (cl_device_id id)
	{
		CHECK_ERR( not HasPlatform() );
		CHECK_ERR( not HasDevice() );

		//Deinitialize();

		CHECK_ERR( _LoadFunctions() );
		CHECK_ERR( id != null );
		
		// check is 'id' is valid device
		cl_bool		dev_available = false;
		CL_CHECK( clGetDeviceInfo( id, CL_DEVICE_AVAILABLE, sizeof(dev_available), OUT &dev_available, null ) );
		CHECK_ERR( dev_available );
		_device	= id;

		// get platform for this device
		CL_CHECK( clGetDeviceInfo( id, CL_DEVICE_PLATFORM, sizeof(_platform), OUT &_platform, null ) );
		CHECK_ERR( _platform != null );

		CHECK_ERR( _CheckVersion() );

		_UpdateProperties();
		return true;
	}
	
/*
=================================================
	Deinitialize
=================================================
*/
	void CL1Device::Deinitialize ()
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

		_sharedApi			= ESharing::None;
		_clLibInitialized	= false;

		CL1_Delete();
	}
	
/*
=================================================
	GetDeviceInfo
=================================================
*/
	bool CL1Device::GetDeviceInfo (OUT Array<DeviceInfo> &deviceInfo) const
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

		CL_CALL( clGetPlatformIDs( 0, null, OUT &num_platforms ) );
		CHECK_ERR( num_platforms > 0 );
		
		platforms.Resize( num_platforms );
		CL_CALL( clGetPlatformIDs( num_platforms, OUT platforms.ptr(), null ) );
		
		FOR( i, platforms )
		{
			if ( platforms[i] == null )
				continue;

			CL_CALL( clGetPlatformInfo( platforms[i], CL_PLATFORM_PROFILE, info.Capacity(), OUT info.ptr(), null ) );
			info.CalculateLength();

			if ( info != "FULL_PROFILE" )
				continue;
			
			CL_CALL( clGetPlatformInfo( platforms[i], CL_PLATFORM_VERSION, info.Capacity(), OUT info.ptr(), null ) );
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
			
			CL_CALL( clGetDeviceIDs( platforms[i], cl_device_type(CL_DEVICE_TYPE_ALL), 0, null, OUT &num_devices ) );
			CHECK_ERR( num_devices != 0 );
		
			devices.Resize( num_devices );
			CL_CALL( clGetDeviceIDs( platforms[i], cl_device_type(CL_DEVICE_TYPE_ALL), num_devices, OUT devices.ptr(), null ) );

			FOR( j, devices )
			{
				if ( devices[j] == null )
					continue;
				
				CL_CALL( clGetDeviceInfo( devices[j], CL_DEVICE_AVAILABLE, sizeof(valueb), OUT &valueb, null ) );
				if ( not valueb )
					continue;

				cl_device_type	dev_type = 0;
				DeviceInfo		dev_info;

				dev_info.id			= devices[j];
				dev_info.version	= cl_ver;

				CL_CALL( clGetDeviceInfo( devices[j], CL_DEVICE_TYPE, sizeof(dev_type), OUT &dev_type, null ) );
				dev_info.isGPU	= !!(dev_type & CL_DEVICE_TYPE_GPU);
				dev_info.isCPU	= !!(dev_type & CL_DEVICE_TYPE_CPU);
				
				CL_CALL( clGetDeviceInfo( devices[j], CL_DEVICE_NAME, info.Capacity(), OUT info.ptr(), null ) );
				dev_info.device	= info.ptr();
				
				CL_CALL( clGetDeviceInfo( devices[j], CL_DEVICE_VENDOR, info.Capacity(), OUT info.ptr(), null ) );
				dev_info.vendor = info.ptr();
				
				CL_CALL( clGetDeviceInfo( devices[j], CL_DEVICE_MAX_CLOCK_FREQUENCY, sizeof(value32), OUT &value32, null ) );
				dev_info.frequencyMHz = value32;
				
				CL_CALL( clGetDeviceInfo( devices[j], CL_DEVICE_MAX_COMPUTE_UNITS, sizeof(value32), OUT &value32, null ) );
				dev_info.computeUnits = value32;
		
				CL_CALL( clGetDeviceInfo( devices[j], CL_DEVICE_ADDRESS_BITS, sizeof(value32), OUT &value32, null ) );
				dev_info.is64bit = (value32 >= 64);

				CL_CALL( clGetDeviceInfo( devices[j], CL_DEVICE_MAX_WORK_GROUP_SIZE, sizeof(size), OUT &size, null ) );
				dev_info.maxInvocations = uint(size);
		
				CL_CALL( clGetDeviceInfo( devices[j], CL_DEVICE_GLOBAL_MEM_SIZE, sizeof(value64), OUT &value64, null ) );
				dev_info.globalMemory.SetBytes( value64 );
		
				CL_CALL( clGetDeviceInfo( devices[j], CL_DEVICE_ENDIAN_LITTLE, sizeof(valueb), OUT &valueb, null ) );
				dev_info.littleEndian = !!valueb;
				
				CL_CALL( clGetDeviceInfo( devices[j], CL_DEVICE_IMAGE_SUPPORT, sizeof(valueb), OUT &valueb, null ) );
				dev_info.imageSupported = !!valueb;

				CL_CALL( clGetDeviceInfo( devices[j], CL_DEVICE_HOST_UNIFIED_MEMORY, sizeof(valueb), OUT &valueb, null ) );
				dev_info.unifiedMemory = !!valueb;

				CL_CALL( clGetDeviceInfo( devices[j], CL_DEVICE_COMPILER_AVAILABLE, sizeof(valueb), OUT &valueb, null ) );
				dev_info.compilerSupported = !!valueb;

				cl_device_exec_capabilities	cap;
				CL_CALL( clGetDeviceInfo( devices[j], CL_DEVICE_EXECUTION_CAPABILITIES, sizeof(cap), OUT &cap, null ) );
				dev_info.compilerSupported &= !!(cap & CL_EXEC_KERNEL);

				CL_CALL( clGetDeviceInfo( devices[j], CL_DEVICE_LINKER_AVAILABLE, sizeof(valueb), OUT &valueb, null ) );
				dev_info.compilerSupported &= !!valueb;
				
				CL_CALL( clGetDeviceInfo( devices[j], CL_DEVICE_EXTENSIONS, 0, null, OUT &size ) );
				info.Resize( size );
				
				CL_CALL( clGetDeviceInfo( devices[j], CL_DEVICE_EXTENSIONS, info.Capacity(), OUT info.ptr(), null ) );
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
	bool CL1Device::_LoadFunctions () const
	{
		if ( _clLibInitialized )
			return true;

		CHECK_ERR( CL1_Init() );
		_clLibInitialized = true;

		return true;
	}

/*
=================================================
	CreateQueue
=================================================
*/
	bool CL1Device::CreateQueue (bool enableProfiling)
	{
		CHECK_ERR( HasPlatform() and HasDevice() and HasContext() );

		_DeleteQueue();

		const cl_queue_properties	properties[] = {
			CL_QUEUE_PROPERTIES,	/*CL_QUEUE_OUT_OF_ORDER_EXEC_MODE_ENABLE |*/ (enableProfiling ? CL_QUEUE_PROFILING_ENABLE : 0),
			0, 0
		};

		cl_int	cl_err = 0;
		CL_CALL( (_queue = clCreateCommandQueueWithProperties( _context, _device, properties, OUT &cl_err )), cl_err );
		CHECK_ERR( _queue != null );

		return true;
	}
	
/*
=================================================
	_DeleteQueue
=================================================
*/
	void CL1Device::_DeleteQueue ()
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
	bool CL1Device::_CheckVersion ()
	{
		char	buf[256] = {};

		CL_CALL( clGetPlatformInfo( _platform, CL_PLATFORM_VERSION, CountOf(buf), OUT buf, null ) );

		_version = uint(RoundToInt( StringUtils::ToDouble( StringCRef( buf ).SubString( /*OpenCL*/6 ) ) * 100.0 ));	// TODO

		CHECK_ERR( _version >= MIN_VERSION );
		return true;
	}
	

# if defined( PLATFORM_WINDOWS ) and defined( GRAPHICS_API_OPENGL )
/*
=================================================
	CreateGLSharedContext
=================================================
*/
	bool CL1Device::CreateGLSharedContext ()
	{
		CHECK_ERR( HasPlatform() and HasDevice() );
		CHECK_ERR( not HasContext() );

		HDC		dc	= wglGetCurrentDC();
		HGLRC	rc	= wglGetCurrentContext();

		CHECK_ERR( dc and rc );

		cl_context_properties properties[] =
		{
			CL_CONTEXT_PLATFORM,	cl_context_properties( _platform ),
			CL_GL_CONTEXT_KHR,		cl_context_properties( rc ),
			CL_WGL_HDC_KHR,			cl_context_properties( dc ),
			0,	0
		};
		
		cl_int	cl_err = 0;
		CL_CALL( (_context = clCreateContext( properties, 1, &_device, null, null, OUT &cl_err )), cl_err );
		CHECK_ERR( _context != null );

		_sharedApi = ESharing::OpenGL;
		return true;
	}

# elif defined( PLATFORM_BASE_POSIX ) and defined( GRAPHICS_API_OPENGL )
/*
=================================================
	CreateGLSharedContext
=================================================
*/
	bool CL1Device::CreateGLSharedContext ()
	{
		CHECK_ERR( HasPlatform() and HasDevice() );
		CHECK_ERR( not HasContext() );

		Display *	display		= glXGetCurrentDisplay();
		GLXContext	xcontext	= glXGetCurrentContext();
		
		CHECK_ERR( display and xcontext );

		cl_context_properties properties[] =
		{
			CL_CONTEXT_PLATFORM,	(cl_context_properties) _platform,
			CL_GL_CONTEXT_KHR,		(cl_context_properties) xcontext,
			CL_GLX_DISPLAY_KHR,		(cl_context_properties) display,
			0,	0
		};
		
		cl_int	cl_err = 0;
		CL_CALL( (_context = clCreateContext( properties, 1, &_device, null, null, OUT &cl_err )), cl_err );
		CHECK_ERR( _context != null );
		
		_sharedApi = ESharing::DirectX11;
		return true;
	}

#endif

/*
=================================================
	CreateContext
=================================================
*/
	bool CL1Device::CreateContext ()
	{
		CHECK_ERR( HasPlatform() and HasDevice() );
		CHECK_ERR( not HasContext() );

		cl_context_properties properties[] =
		{
			CL_CONTEXT_PLATFORM,	cl_context_properties( _platform ),
			0,	0
		};
		
		cl_int	cl_err = 0;
		CL_CALL( (_context = clCreateContext( properties, 1, &_device, null, null, OUT &cl_err )), cl_err );
		CHECK_ERR( _context != null );

		_sharedApi = ESharing::None;
		return true;
	}
	
/*
=================================================
	Synchronize
=================================================
*/
	void CL1Device::Synchronize ()
	{
		CL_CALL( clFinish( _queue ) );
	}
	
/*
=================================================
	Barrier
=================================================
*/
	void CL1Device::Barrier ()
	{
		CL_CALL( clEnqueueBarrierWithWaitList( _queue, 0, null, null ) );
	}
	
/*
=================================================
	AddSharedObj
=================================================
*/
	bool CL1Device::AddSharedObj (const ModulePtr &owner, const ModulePtr &obj)
	{
		ESharing::type	sharing = ESharing::None;
		cl_mem			id		= null;

		if ( obj->GetSupportedMessages().HasType< GpuMsg::GetCLBufferID >() )
		{
			GpuMsg::GetCLBufferID	req_id;
			obj->Send( req_id );

			sharing	= req_id.result->sharing;
			id		= req_id.result->id;
		}
		else
		if ( obj->GetSupportedMessages().HasType< GpuMsg::GetCLImageID >() )
		{
			GpuMsg::GetCLImageID	req_id;
			obj->Send( req_id );

			sharing	= req_id.result->sharing;
			id		= req_id.result->id;
		}

		return AddSharedObj( owner, id, sharing );
	}
	
/*
=================================================
	AddSharedObj
=================================================
*/
	bool CL1Device::AddSharedObj (const ModulePtr &owner, cl_mem id, ESharing::type sharing)
	{
		if ( id == null or sharing == ESharing::None )
			return true;

		CHECK_ERR( sharing == _sharedApi );
		
		auto iter = _acquiredSharedObjects.AddOrSkip( owner.RawPtr(), {} );
		
		CHECK_ERR( not iter->second.acquired );

		iter->second.obj.Add( id );
		return true;
	}

/*
=================================================
	AcquireSharedObj
=================================================
*/
	bool CL1Device::AcquireSharedObj (const ModulePtr &owner)
	{
		SharedMemMap_t::iterator	iter;

		if ( _acquiredSharedObjects.Find( owner.RawPtr(), OUT iter ) and not iter->second.obj.Empty() )
		{
			CHECK_ERR( not iter->second.acquired );
			iter->second.acquired = true;

			ArrayCRef<cl_mem>	objects = iter->second.obj;

			#ifdef GRAPHICS_API_OPENGL
			if ( _sharedApi == ESharing::OpenGL ) {
				CL_CALL( clEnqueueAcquireGLObjects( GetCommandQueue(), cl_uint(objects.Count()), objects.ptr(), 0, null, null ) );
			} else
			#endif

			#ifdef GRAPHICS_API_DIRECTX
			if ( _sharedApi == ESharing::DirectX11 ) {
				CL_CALL( clEnqueueAcquireD3D11ObjectsKHR( GetCommandQueue(), objects.Count(), objects.ptr(), 0, null, null ) );
			} else
			if ( _sharedApi == ESharing::DirectX10 ) {
				CL_CALL( clEnqueueAcquireD3D10ObjectsKHR( GetCommandQueue(), objects.Count(), objects.ptr(), 0, null, null ) );
			} else
			#endif

			{
				RETURN_ERR( "unknown sharing api type" );
			}
		}
		return true;
	}
	
/*
=================================================
	ReleaseSharedObj
=================================================
*/
	bool CL1Device::ReleaseSharedObj (const ModulePtr &owner)
	{
		SharedMemMap_t::iterator	iter;

		if ( _acquiredSharedObjects.Find( owner.RawPtr(), OUT iter ) and not iter->second.obj.Empty() )
		{
			CHECK_ERR( iter->second.acquired );
			iter->second.acquired = false;

			ArrayCRef<cl_mem>	objects = iter->second.obj;
			
			#ifdef GRAPHICS_API_OPENGL
			if ( _sharedApi == ESharing::OpenGL ) {
				CL_CALL( clEnqueueReleaseGLObjects( GetCommandQueue(), cl_uint(objects.Count()), objects.ptr(), 0, null, null ) );
			} else
			#endif

			#ifdef GRAPHICS_API_DIRECTX
			if ( _sharedApi == ESharing::DirectX11 ) {
				CL_CALL( clEnqueueReleaseD3D11ObjectsKHR( GetCommandQueue(), objects.Count(), objects.ptr(), 0, null, null ) );
			} else
			if ( _sharedApi == ESharing::DirectX10 ) {
				CL_CALL( clEnqueueReleaseD3D10ObjectsKHR( GetCommandQueue(), objects.Count(), objects.ptr(), 0, null, null ) );
			} else
			#endif

			{
				RETURN_ERR( "unknown sharing api type" );
			}

			_acquiredSharedObjects.EraseByIter( iter );
		}
		return true;
	}

/*
=================================================
	_UpdateProperties
=================================================
*/
	void CL1Device::_UpdateProperties ()
	{
		size_t		size	 = 0;
		usize3		size3;

		CL_CALL( clGetDeviceInfo( _device, CL_DEVICE_MAX_WORK_GROUP_SIZE, sizeof(size), OUT &size, null ) );
		_properties.maxComputeWorkGroupInvocations = uint(size);
		
		CL_CALL( clGetDeviceInfo( _device, CL_DEVICE_MAX_WORK_ITEM_SIZES, sizeof(size3), OUT size3.ptr(), null ) );
		_properties.maxComputeWorkGroupSize = size3.To<uint3>();
		_properties.maxComputeWorkGroupCount = uint3(~0u) / _properties.maxComputeWorkGroupSize;

		_properties.explicitMemoryObjects = false;
	}
	
/*
=================================================
	WriteInfo
=================================================
*/
	bool CL1Device::WriteInfo ()
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

		CL_CALL( clGetPlatformInfo( _platform, CL_PLATFORM_NAME, info.Capacity(), OUT info.ptr(), null ) );
		log << "\nPlatform:         " << info.cstr();
		
		CL_CALL( clGetPlatformInfo( _platform, CL_PLATFORM_VERSION, info.Capacity(), OUT info.ptr(), null ) );
		log << "\nVersion:          " << info.cstr();
		
		CL_CALL( clGetDeviceInfo( _device, CL_DEVICE_NAME, info.Capacity(), OUT info.ptr(), null ) );
		log << "\nDevice:           " << info.cstr();
		
		CL_CALL( clGetDeviceInfo( _device, CL_DRIVER_VERSION, info.Capacity(), OUT info.ptr(), null ) );
		log << "\nDriver:           " << info.cstr();
		
		CL_CALL( clGetDeviceInfo( _device, CL_DEVICE_MAX_CLOCK_FREQUENCY, sizeof(value32), OUT &value32, null ) );
		log << "\nFrequency:        " << value32 << " MHz";

		CL_CALL( clGetDeviceInfo( _device, CL_DEVICE_MAX_COMPUTE_UNITS, sizeof(value32), OUT &value32, null ) );
		log << "\nCompute Units:    " << value32;

		CL_CALL( clGetDeviceInfo( _device, CL_DEVICE_LOCAL_MEM_SIZE, sizeof(value64), OUT &value64, null ) );
		log << "\nLocal Mem:        " << ToString( Bytes<ulong>::FromBytes( value64 ) );

		CL_CALL( clGetDeviceInfo( _device, CL_DEVICE_MAX_WORK_GROUP_SIZE, sizeof(size), OUT &size, null ) );
		log << "\nMax invocations:  " << size;
		
		CL_CALL( clGetDeviceInfo( _device, CL_DEVICE_MAX_WORK_ITEM_SIZES, sizeof(size3), OUT size3.ptr(), null ) );
		log << "\nMax local groups: " << ToString( size3 );

		CL_CALL( clGetDeviceInfo( _device, CL_DEVICE_ADDRESS_BITS, sizeof(value32), OUT &value32, null ) );
		log << "\nAddress bits:     " << value32;

		CL_CALL( clGetDeviceInfo( _device, CL_DEVICE_GLOBAL_MEM_CACHE_SIZE, sizeof(value64), OUT &value64, null ) );
		log << "\nGlobal Cache:     " << ToString( Bytes<ulong>::FromBytes( value64 ) );		// L1 cache

		CL_CALL( clGetDeviceInfo( _device, CL_DEVICE_GLOBAL_MEM_SIZE, sizeof(value64), OUT &value64, null ) );
		_totalMemory.SetBytes( value64 );
		log << "\nGlobal Mem:       " << ToString( Bytes<ulong>::FromBytes( value64 ) );
		
		CL_CALL( clGetDeviceInfo( _device, CL_DEVICE_MAX_CONSTANT_BUFFER_SIZE, sizeof(value64), OUT &value64, null ) );
		log << "\nConst Buffer:     " << ToString( Bytes<ulong>::FromBytes( value64 ) );

		CL_CALL( clGetDeviceInfo( _device, CL_DEVICE_MAX_MEM_ALLOC_SIZE, sizeof(value64), OUT &value64, null ) );
		log << "\nMax alloc size:   " << ToString( Bytes<ulong>::FromBytes( value64 ) );
		
		CL_CALL( clGetDeviceInfo( _device, CL_DEVICE_MAX_CONSTANT_ARGS, sizeof(value32), OUT &value32, null ) );
		log << "\nConst args:       " << value32;
		
		CL_CALL( clGetDeviceInfo( _device, CL_DEVICE_MAX_PARAMETER_SIZE, sizeof(size), OUT &size, null ) );
		log << "\nParam size:       " << ToString( BytesU( size ) );

		CL_CALL( clGetDeviceInfo( _device, CL_DEVICE_ENDIAN_LITTLE, sizeof(valueb), OUT &valueb, null ) );
		log << "\nEncoding:         " << ( valueb ? "Little endian" : "Big endian");
		
		CL_CALL( clGetDeviceInfo( _device, CL_DEVICE_MEM_BASE_ADDR_ALIGN, sizeof(value32), OUT &value32, null ) );
		log << "\nMem align:        " << ToString( Bits<uint>( value32 ) );

		CL_CALL( clGetDeviceInfo( _device, CL_DEVICE_IMAGE2D_MAX_WIDTH, sizeof(size), OUT &size, null ) );
		log << "\nImage2D width:    " << size << " px";
		
		CL_CALL( clGetDeviceInfo( _device, CL_DEVICE_IMAGE2D_MAX_HEIGHT, sizeof(size), OUT &size, null ) );
		log << "\nImage2D height:   " << size << " px";
		
		CL_CALL( clGetDeviceInfo( _device, CL_DEVICE_IMAGE3D_MAX_WIDTH, sizeof(size), OUT &size, null ) );
		log << "\nImage3D width:    " << size << " px";
		
		CL_CALL( clGetDeviceInfo( _device, CL_DEVICE_IMAGE3D_MAX_HEIGHT, sizeof(size), OUT &size, null ) );
		log << "\nImage3D height:   " << size << " px";
		
		CL_CALL( clGetDeviceInfo( _device, CL_DEVICE_IMAGE3D_MAX_DEPTH, sizeof(size), OUT &size, null ) );
		log << "\nImage3D depth:    " << size << " px";
		
#		if COMPUTE_API_OPENCL >= 120
		CL_CALL( clGetDeviceInfo( _device, CL_DEVICE_IMAGE_MAX_BUFFER_SIZE, sizeof(size), OUT &size, null ) );
		log << "\nBuffer size:      " << size << " px";
		
		CL_CALL( clGetDeviceInfo( _device, CL_DEVICE_IMAGE_MAX_ARRAY_SIZE, sizeof(size), OUT &size, null ) );
		log << "\nImageArray:       " << size << " layers";

		CL_CALL( clGetDeviceInfo( _device, CL_DEVICE_BUILT_IN_KERNELS, info.Capacity(), OUT info.ptr(), null ) );
		log << "\nKernels:          " << info.cstr();
#		endif

		CL_CALL( clGetDeviceInfo( _device, CL_DEVICE_EXTENSIONS, 0, null, OUT &size ) );
		info.Reserve( size );

		LOG( log, ELog::Debug | ELog::SpoilerFlag );

		// write extensions
		log.Clear();
		log << "OpenCL Extensions\n---------------\n";

		CL_CALL( clGetDeviceInfo( _device, CL_DEVICE_EXTENSIONS, info.Capacity(), OUT info.ptr(), null ) );
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
		
		LOG( log, ELog::Debug | ELog::SpoilerFlag );
		return true;
	}


}	// PlatformCL
}	// Engine

#endif	// COMPUTE_API_OPENCL
