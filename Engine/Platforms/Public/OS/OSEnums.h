// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/Platforms/Public/Common.h"

namespace Engine
{
namespace Platforms
{
	
	//
	// Directory Type
	//

	struct EDirectory
	{
		enum type
		{
			Internal		= 0,	//	path to internal storage, example: data/data/<package>/files
			InternalCache,			//	path to instrnal storage cache, example: data/data/<package>/cache
			External,				//	path to external storage, example: /mnt/sdcard/data/Android/<package>/files
			ExternalCache,			//	path to external storage cache, example: /mnt/sdcard/data/Android/<package>/cache
			SDCard,					//	path to sdcard storage, example: /mnt/sdcard/
			Library,				//	path to libraries folder, example: <package>/bin/
									// TODO: for android:
									//	getContext().getApplicationInfo().nativeLibraryDir;
									// String libraryPath = getContext().getApplicationInfo().dataDir + "/lib";
			_Count
		};

		ND_ static const char * Name (type value)
		{
			DEBUG_ONLY(
				static const char *	names[] = {
					"internal",
					"internal_cache",
					"external",
					"external_cache",
					"sdcard",
					"library"
				};
				STATIC_ASSERT( CountOf(names) == _Count );
				return names[ value ];
			)

			RELEASE_ONLY(
				return "";
			)
		}
	};



	//
	// Display Orientation
	//

	struct EDisplayOrientation
	{
		enum type
		{
			Default				= -1,
			Sensor				= -2,
			Portrait			= 0,
			PortraitReverse		= 180,
			Landscape			= 90,
			LandscapeReverse	= 270,
			
			Orient_0_deg		= Portrait,
			Orient_90_deg		= Landscape,
			Orient_180_deg		= PortraitReverse,
			Orient_270_deg		= LandscapeReverse,
		};
	};


}	// Platforms
}	// Engine
