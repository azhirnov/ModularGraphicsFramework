// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Core/Config/Engine.Config.h"

#ifdef GRAPHICS_API_SOFT

#include "Engine/Platforms/Soft/Impl/SWDevice.h"
#include "Engine/Platforms/Soft/Impl/SWDeviceProperties.h"

namespace Engine
{
namespace PlatformSW
{

/*
=================================================
	constructor
=================================================
*/
	SWDevice::SWDevice (GlobalSystemsRef gs) :
		BaseObject( gs ),
		_debugReportCounter{ 0 },	_debugReportEnabled{ false },
		_initialized{ false }
	{
	}
	
/*
=================================================
	destructor
=================================================
*/
	SWDevice::~SWDevice ()
	{
		CHECK( not _initialized );

		if ( _debugReportCounter > 0 )
		{
			WARNING( "There are a few warnings, check debug output!" );
		}
	}
	
/*
=================================================
	Initialize
=================================================
*/
	bool SWDevice::Initialize ()
	{
		_initialized = true;

		_UpdateProperties();
		return true;
	}
	
/*
=================================================
	Deinitialize
=================================================
*/
	void SWDevice::Deinitialize ()
	{
		_initialized = false;
	}
	
/*
=================================================
	Resize
=================================================
*/
	void SWDevice::Resize (const uint2 &size)
	{
		_surfaceSize = size;
	}

/*
=================================================
	DispatchCompute
=================================================
*/
	bool SWDevice::DispatchCompute (const uint3 &workGroups, const ModulePtr &pipeline, const ModulePtr &resourceTable)
	{
		return _shaderModel.DispatchCompute( workGroups, pipeline, resourceTable );
	}
		
/*
=================================================
	InitDebugReport
=================================================
*/
	void SWDevice::InitDebugReport ()
	{
		_debugReportEnabled = true;
	}

/*
=================================================
	DebugReport
=================================================
*/
	void SWDevice::DebugReport (StringCRef msg, EDbgReport::bits flags, StringCRef file, int line) const
	{
		if ( not _debugReportEnabled )
			return;

		ELog::type	log = ELog::Debug;
		String		str = "[";

		FOR( i, flags )
		{
			const auto	t = EDbgReport::type(i);

			if ( not flags[t] )
				continue;

			switch ( t )
			{
				case EDbgReport::Debug :		str << "Debug ";		log = ELog::Debug;	break;
				case EDbgReport::Perfomance :	str << "Perfomance ";	log = ELog::Debug;	break;
				case EDbgReport::Info :			str << "Info ";			log = ELog::Info;	break;
				case EDbgReport::Warning :		str << "Warning ";		log = ELog::Info;	break;
				case EDbgReport::Error :		str << "Error ";		log = ELog::Error;	break;
				default :						WARNING( "unknown" );
			}
		}

		str << "] " << msg;
		LOG( str, log, file.cstr(), line );

		++_debugReportCounter;
	}
	
/*
=================================================
	_UpdateProperties
=================================================
*/
	void SWDevice::_UpdateProperties ()
	{
		_properties.maxComputeWorkGroupInvocations	= SWDeviceProperties.limits.maxComputeWorkGroupInvocations;
		_properties.maxComputeWorkGroupSize			= SWDeviceProperties.limits.maxComputeWorkGroupSize;
		_properties.maxComputeWorkGroupCount		= SWDeviceProperties.limits.maxComputeWorkGroupCount;

		_properties.explicitMemoryObjects	= false;	// TODO
	}

}	// PlatformSW
}	// Engine

#endif	// GRAPHICS_API_SOFT
