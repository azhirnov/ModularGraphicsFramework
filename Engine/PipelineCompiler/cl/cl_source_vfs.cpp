// This is generated file, don't change anything!

#include "cl_source_vfs.h"
#include "Core/STL/Containers/HashMap.h"

namespace cl_vfs
{
	extern void VFS_funcs (OUT String &src);
	extern void VFS_matrix (OUT String &src);
	extern void VFS_vload (OUT String &src);

	bool LoadFile (StringCRef filename, OUT String &src)
	{
		static const HashMap< String, void (*) (OUT String &) > vfs = {
			{ "funcs.cl", &VFS_funcs },
			{ "matrix.cl", &VFS_matrix },
			{ "vload.cl", &VFS_vload }
		};

		usize idx = UMax;
		if ( vfs.FindIndex( filename, OUT idx ) ) {
			vfs[idx].second( OUT src );
			return true;
		}
		return false;
	}
}	// cl_vfs
