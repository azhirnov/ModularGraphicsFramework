// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/Config/Engine.Config.h"

#ifdef GRAPHICS_API_SOFT

#include "Engine/Platforms/Soft/Impl/SWEnums.h"
#include "Engine/Platforms/Soft/Impl/SWShaderModel.h"

namespace Engine
{
namespace PlatformSW
{

	//
	// Software Renderer Device
	//
	class SWDevice final : public BaseObject
	{
	// types
	public:
		struct EDbgReport
		{
			enum type : uint
			{
				Debug,
				Perfomance,
				Info,
				Warning,
				Error,
				_Count
			};

			GX_ENUM_BITFIELD( EDbgReport );
		};


	// variables
	private:
		uint2				_surfaceSize;

		SWShaderModel		_shaderModel;
		mutable uint		_debugReportCounter;

		bool				_debugReportEnabled;
		bool				_initialized;


	// methods
	public:
		explicit SWDevice (GlobalSystemsRef gs);
		~SWDevice ();
		
		bool Initialize ();
		void Deinitialize ();

		void Resize (const uint2 &size);

		bool DispatchCompute (const uint3 &workGroups, const ModulePtr &pipeline, const ModulePtr &resourceTable);
		
		void InitDebugReport ();
		void DebugReport (StringCRef log, EDbgReport::bits flags, StringCRef file, int line) const;

		bool		IsDeviceCreated ()	const	{ return _initialized; }

		uint2		SurfaceSize ()		const	{ return _surfaceSize; }
	};


}	// PlatformSW
}	// Engine

#endif	// GRAPHICS_API_SOFT
