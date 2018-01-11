// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/Platforms/OpenCL/Impl/cl2.h"
#include "Engine/Platforms/OpenCL/Impl/CL2Enums.h"
#include "Engine/Platforms/OpenCL/Impl/CL2Emulator.h"

#if defined( COMPUTE_API_OPENCL )

namespace Engine
{
namespace PlatformCL
{

	//
	// OpenCL Device
	//

	class CL2Device final : public BaseObject
	{
	// types
	public:
		static constexpr uint	MIN_VERSION = COMPUTE_API_OPENCL;

		struct DeviceInfo
		{
			cl::cl_device_id	id					= null;
			String				vendor;
			String				device;
			BytesUL				globalMemory;
			uint				version				= 0;
			uint				frequencyMHz		= 0;		// processor frequency, indicates performance
			uint				computeUnits		= 0;		// number of processors, indicates performance
			uint				maxInvocations		= 0;		// max size of local group
			bool				isGPU				= false;
			bool				isCPU				= false;
			bool				is64bit				= false;	// 'true' if has 64 bit address space
			bool				imageSupported		= false;	// 'true' if supports images (2d, ...)
			bool				compilerSupported	= false;	// 'true' if supports program compilation
			bool				littleEndian		= true;		// it is very important
			bool				unifiedMemory		= false;	// indicates integrated GPU device
			bool				glSharing			= false;	// 'true' if suports sharing with OpenGL
			bool				dxSharing			= false;	// 'true' if supports sharing with DirectX 11
		};


	// variables
	private:
		cl::cl_platform_id		_platform;
		cl::cl_device_id		_device;
		cl::cl_context			_context;
		cl::cl_command_queue	_queue;

		Bytes<ulong>			_totalMemory;
		uint					_version;
		bool					_isShared;

		mutable bool			_clLibInitialized;


	// methods
	public:
		CL2Device (GlobalSystemsRef gs);
		~CL2Device ();

		bool GetDeviceInfo (OUT Array<DeviceInfo> &info) const;

		bool CreateDevice (cl::cl_device_id id);
		bool CreateContext ();
		bool CreateGLSharedContext ();
		bool CreateQueue ();
		bool WriteInfo ();
		void Deinitialize ();
		
		void Synchronize ();
		void Barrier ();

		bool					IsShared ()						const	{ return _isShared; }
		uint					GetVersion ()					const	{ return _version; }
		
		//ulong3 const&			GetMaxThreads ()				const	{ return _maxThreads; }
		//ulong3				GetMaxWorkGroupCount ()			const	{ return GetMaxThreads() / GetMaxLocalGroupSize(); }
		//ulong3 const&			GetMaxLocalGroupSize ()			const	{ return _maxLocalGroupSize; }
		//ulong					GetMaxLocalGroupInvocations ()	const	{ return _maxInvocations; }

		//usize					GetComputeUnitsCount ()			const	{ return _maxComputeUnits; }

		Bytes<ulong>			GetTotalMemory ()				const	{ return _totalMemory; }
		Bytes<ulong>			GetAvailableMemory ()			const;

		cl::cl_platform_id		GetPlatform ()					const	{ return _platform; }
		cl::cl_device_id		GetDevice ()					const	{ return _device; }
		cl::cl_context			GetContext ()					const	{ return _context; }
		cl::cl_command_queue	GetCommandQueue ()				const	{ return _queue; }

		bool					HasPlatform ()					const	{ return _platform != null; }
		bool					HasDevice ()					const	{ return _device != null; }
		bool					HasContext ()					const	{ return _context != null; }
		bool					HasCommandQueue ()				const	{ return _queue != null; }

	private:
		//bool _ChoosePlatform (uint version, StringCRef deviceName);
		//bool _ChooseDevice (uint version, StringCRef deviceName);
		bool _CheckVersion ();
		void _DeleteQueue ();

		bool _LoadFunctions () const;
	};


}	// PlatformCL
}	// Engine

#endif	// COMPUTE_API_OPENCL
