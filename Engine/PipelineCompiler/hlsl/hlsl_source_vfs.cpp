// This is generated file, don't change anything!

#include "hlsl_source_vfs.h"
#include "Core/STL/Containers/HashMap.h"

namespace hlsl_vfs
{
	extern void VFS_vload (OUT String &src);

	bool LoadFile (StringCRef filename, OUT String &src)
	{
		static const HashMap< String, void (*) (OUT String &) > vfs = {
			{ "vload.hl", &VFS_vload }
		};

		usize idx = UMax;
		if ( vfs.FindIndex( filename, OUT idx ) ) {
			vfs[idx].second( OUT src );
			return true;
		}
		return false;
	}
}	// hlsl_vfs
