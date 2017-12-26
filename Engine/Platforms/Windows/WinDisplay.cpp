// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Engine/Platforms/Windows/WinDisplay.h"

#if defined( PLATFORM_WINDOWS )

#include "Engine/STL/OS/Windows/WinHeader.h"

#ifdef COMPILER_MSVC

#	pragma warning (push, 0)
#	include <VersionHelpers.h>
	// Windows 8.1
#	include <ShellScalingAPI.h>		
	// Windows Vista 
#	include <LowLevelMonitorConfigurationAPI.h>
#	include <PhysicalMonitorEnumerationAPI.h>
//#	include <HighLevelMonitorConfigurationAPI.h>
#	pragma warning (pop)

#else

#	include <VersionHelpers.h>
	// Windows Vista
#	include <PhysicalMonitorEnumerationAPI.h>

#endif	// COMPILER_MSVC


namespace Engine
{
namespace Platforms
{

/*
=================================================
	constructor
=================================================
*/
	WinDisplay::WinDisplay ()
	{
	}
	
/*
=================================================
	Update
=================================================
*/
	bool WinDisplay::Update ()
	{
		MONITORENUMPROC func = LAMBDA() (HMONITOR hMonitor, HDC, LPRECT, LPARAM dwData) -> BOOL
		{
			auto			self = ReferenceCast<WinDisplay *>( dwData );
			MONITORINFOEX	info = {};		info.cbSize = sizeof(info);

			CHECK( GetMonitorInfoA( hMonitor, OUT &info ) );

			Display		disp{ ReferenceCast<usize>(hMonitor), RectI(), RectI(), 96.0f, 0, EDisplayOrientation::Default };
			bool		has_freq	= false;
			bool		has_dpi		= false;
			const bool	is_primary	= !!(info.dwFlags & MONITORINFOF_PRIMARY);

			// get monitor info
			/*DWORD		mon_count	= 0;

			if ( IsWindowsVistaOrGreater() and GetNumberOfPhysicalMonitorsFromHMONITOR( hMonitor, OUT &mon_count ) )
			{
				FixedSizeArray< PHYSICAL_MONITOR, 8 >	monitors;	monitors.Resize( mon_count );

				if ( GetPhysicalMonitorsFromHMONITOR( hMonitor, monitors.Count(), OUT monitors.ptr() ) )
				{
					FOR( i, monitors )
					{
						if ( i == 0 )
						{
							MC_TIMING_REPORT	timing = {};
							if ( GetTimingReport( monitors[i].hPhysicalMonitor, OUT &timing ) )
							{
								disp.SetFrequency( timing.dwVerticalFrequencyInHZ );
								has_freq = true;
							}
						}
					}
					DestroyPhysicalMonitors( monitors.Count(), monitors.ptr() );
				}
			}*/

			if ( not has_freq )
			{
				disp.SetFrequency( self->_GetDisplayFrequency() );
				has_freq = true;
			}

			disp.SetWorkArea(RectI{ info.rcWork.left, info.rcWork.top, info.rcWork.right, info.rcWork.bottom });
			disp.SetFullArea(RectI{ info.rcMonitor.left, info.rcMonitor.top, info.rcMonitor.right, info.rcMonitor.bottom });
			disp.SetOrientation( disp.IsHorizontal() ? EDisplayOrientation::Landscape : EDisplayOrientation::Portrait );

			#ifdef COMPILER_MSVC
			RELEASE_ONLY( if ( IsWindows8Point1OrGreater() ) )
			{
				uint2 dpi{ 96, 96 };
				if ( GetDpiForMonitor( hMonitor, MDT_RAW_DPI, OUT &dpi.x, OUT &dpi.y ) == S_OK )
				{
					disp.SetPixelsPerInch( float2(dpi) );
					disp.SetPhysicalSize( disp.CalcPhysicalSize().To<float2>() );
					has_dpi = true;
				}
			}
			#endif	// COMPILER_MSVC

			if ( not has_dpi and is_primary )
			{
				disp.SetPhysicalSize( self->_ScreenPhysicalSize() );
				disp.SetPixelsPerInch( disp.CalcPixelsPerInch() );
				has_dpi = true;
			}

			self->_displays.PushBack( RVREF(disp) );
			self->_monitors.PushBack( HMONITOR_t(hMonitor) );
			return TRUE;
		};

		_displays.Clear();
		_monitors.Clear();

		CHECK( EnumDisplayMonitors( null, null, func, ReferenceCast<LPARAM>(this) ) );
		return true;
	}
	
/*
=================================================
	_ScreenPhysicalSize
=================================================
*/
	float2 WinDisplay::_ScreenPhysicalSize () const
	{
		float2	size;
		HDC		hDCScreen	= ::GetDC( null );	// TODO
		
		size.x = (float) ::GetDeviceCaps( hDCScreen, HORZSIZE );
		size.y = (float) ::GetDeviceCaps( hDCScreen, VERTSIZE );
		
		::ReleaseDC( null, hDCScreen );
		return size * 0.001f;
	}
	
/*
=================================================
	_GetDisplayFrequency
=================================================
*/
	uint WinDisplay::_GetDisplayFrequency () const
	{
		HDC		hDCScreen	= ::GetDC( null );
		
		uint freq = ::GetDeviceCaps( hDCScreen, VREFRESH );
		
		::ReleaseDC( null, hDCScreen );
		return freq;
	}


}	// Platforms
}	// Engine

#endif	// PLATFORM_WINDOWS
