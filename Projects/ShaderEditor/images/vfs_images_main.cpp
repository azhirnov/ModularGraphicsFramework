// This is generated file, don't change anything!

#include "vfs_images_main.h"
#include "Core/STL/Containers/HashMap.h"

namespace vfs_images
{

	extern void VFS_images_greynoise (OUT BinaryArray &data);

	void (* GetBuiltinFileLoader (StringCRef filename)) (OUT BinaryArray &)
	{
		static const HashMap< String, void (*) (OUT BinaryArray &) > vfs = {
			{ "greynoise.png", &VFS_images_greynoise }		};

		usize idx = UMax;
		if ( vfs.FindIndex( filename, OUT idx ) ) {
			return vfs[idx].second;
		}
		return null;
	}

}
