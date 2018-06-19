// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Core/Config/Engine.Config.h"

#ifdef COMPUTE_API_OPENCL

#include "Engine/Platforms/OpenCL/120/CL1Enums.h"
#include "Engine/Platforms/OpenCL/120/CL1Emulator.h"
#include "Engine/Platforms/Public/GPU/Thread.h"

namespace Engine
{
namespace PlatformCL
{

	//
	// OpenCL Device
	//

	class CL1Device final : public BaseObject
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

		using DeviceProperties_t	= GpuMsg::GetDeviceProperties::Properties;
		

	private:
		struct SharedObjects
		{
		// types
			using SharedMemSet_t = MixedSizeSet< cl::cl_mem, 16 >;

		// variables
			SharedMemSet_t	obj;
			bool			acquired = false;
		};

		using SharedMemMap_t	= Map< UntypedKey<ModulePtr>, SharedObjects >;


	// variables
	private:
		cl::cl_command_queue	_queue;		// TODO: move to CommandQueue module

		cl::cl_platform_id		_platform;
		cl::cl_device_id		_device;
		cl::cl_context			_context;

		DeviceProperties_t		_properties;

		SharedMemMap_t			_acquiredSharedObjects;
		ESharing::type			_sharedApi;

		Bytes<ulong>			_totalMemory;
		uint					_version;

		mutable bool			_clLibInitialized;


	// methods
	public:
		CL1Device (GlobalSystemsRef gs);
		~CL1Device ();

		bool GetDeviceInfo (OUT Array<DeviceInfo> &info) const;

		bool CreateDevice (cl::cl_device_id id);
		bool CreateContext ();
		bool CreateGLSharedContext ();
		bool CreateDXSharedContext ();
		bool CreateQueue (bool enableProfiling = false);
		void Deinitialize ();

		bool WriteInfo ();
		
		void Synchronize ();
		void Barrier ();

		// sharing
		bool AddSharedObj (const ModulePtr &owner, const ModulePtr &obj);
		bool AddSharedObj (const ModulePtr &owner, cl::cl_mem id, ESharing::type sharing);
		bool AcquireSharedObj (const ModulePtr &owner);
		bool ReleaseSharedObj (const ModulePtr &owner);

		ND_ bool					IsShared ()				const	{ return _sharedApi != ESharing::None; }
		ND_ ESharing::type			GetSharedApi ()			const	{ return _sharedApi; }
		ND_ uint					GetVersion ()			const	{ return _version; }

		ND_ Bytes<ulong>			GetTotalMemory ()		const	{ return _totalMemory; }
		ND_ Bytes<ulong>			GetAvailableMemory ()	const;

		ND_ cl::cl_platform_id		GetPlatform ()			const	{ return _platform; }
		ND_ cl::cl_device_id		GetDevice ()			const	{ return _device; }
		ND_ cl::cl_context			GetContext ()			const	{ return _context; }
		ND_ cl::cl_command_queue	GetCommandQueue ()		const	{ return _queue; }

		ND_ bool					HasPlatform ()			const	{ return _platform != null; }
		ND_ bool					HasDevice ()			const	{ return _device != null; }
		ND_ bool					HasContext ()			const	{ return _context != null; }
		ND_ bool					HasCommandQueue ()		const	{ return _queue != null; }

		ND_ DeviceProperties_t const&	GetProperties ()	const	{ return _properties; }


	private:
		bool _CheckVersion ();
		void _DeleteQueue ();
		void _UpdateProperties ();

		bool _LoadFunctions () const;
	};


}	// PlatformCL
}	// Engine

#endif	// COMPUTE_API_OPENCL
