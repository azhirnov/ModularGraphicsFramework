// This is generated file, don't change anything!

#include "cl_source_vfs.h"

namespace cl_vfs
{
	extern void VFS_vload (OUT String &src)
	{
		src << R"#(// _ptr_ -- float4*

#define vload1( _offset_, _ptr_ )			(_ptr_[(_offset_)>>2])
#define vstore1( _data_, _offset_, _ptr_ )	(_ptr_[(_offset_)>>2] = (_data_))

#define vload_float1( _offset_, _ptr_ )		vload1( _offset_, _ptr_ )
#define vload_float2( _offset_, _ptr_ )		vload2( _offset_, _ptr_ )
#define vload_float3( _offset_, _ptr_ )		vload3( _offset_, _ptr_ )
#define vload_float4( _offset_, _ptr_ )		vload4( _offset_, _ptr_ )

#define vload_int1( _offset_, _ptr_ )		as_int(vload1( _offset_, _ptr_ ))
#define vload_int2( _offset_, _ptr_ )		as_int2(vload2( _offset_, _ptr_ ))
#define vload_int3( _offset_, _ptr_ )		as_int3(vload3( _offset_, _ptr_ ))
#define vload_int4( _offset_, _ptr_ )		as_int4(vload4( _offset_, _ptr_ ))

#define vload_uint1( _offset_, _ptr_ )		as_uint(vload1( _offset_, _ptr_ ))
#define vload_uint2( _offset_, _ptr_ )		as_uint2(vload2( _offset_, _ptr_ ))
#define vload_uint3( _offset_, _ptr_ )		as_uint3(vload3( _offset_, _ptr_ ))
#define vload_uint4( _offset_, _ptr_ )		as_uint4(vload4( _offset_, _ptr_ ))

#define vload_double1( _offset_, _ptr_ ) 	as_double(vload2( _offset_, _ptr_ ))
#define vload_double2( _offset_, _ptr_ ) 	as_double2(vload4( _offset_, _ptr_ ))
#define vload_double3( _offset_, _ptr_ ) 	(double3)(as_double2(vload4( _offset_, _ptr_ )), as_double(vload2( (_offset_) + 16, _ptr_ )))
#define vload_double4( _offset_, _ptr_ ) 	(double4)(as_double2(vload4( _offset_, _ptr_ )), as_double2(vload4( (_offset_) + 16, _ptr_ )))

#define vload_long1( _offset_, _ptr_ )		as_long(vload2( _offset_, _ptr_ ))
#define vload_long2( _offset_, _ptr_ ) 		as_long2(vload4( _offset_, _ptr_ ))
#define vload_long3( _offset_, _ptr_ )		(long3)(as_long2(vload4( _offset_, _ptr_ )), as_long(vload2( (_offset_) + 16, _ptr_ )))
#define vload_long4( _offset_, _ptr_ ) 		(long4)(as_long2(vload4( _offset_, _ptr_ )), as_long2(vload4( (_offset_) + 16, _ptr_ )))

#define vload_ulong1( _offset_, _ptr_ ) 	as_ulong(vload2( _offset_, _ptr_ ))
#define vload_ulong2( _offset_, _ptr_ ) 	as_ulong2(vload4( _offset_, _ptr_ ))
#define vload_ulong3( _offset_, _ptr_ ) 	(ulong3)(as_ulong2(vload4( _offset_, _ptr_ )), as_ulong(vload2( (_offset_) + 16, _ptr_ )))
#define vload_ulong4( _offset_, _ptr_ ) 	(ulong4)(as_ulong2(vload4( _offset_, _ptr_ )), as_ulong2(vload4( (_offset_) + 16, _ptr_ )))


#define vload_float2x2( _offset_, _ptr_ )	__float2x2( vload2( _offset_, _ptr_ ), vload2( (_offset_) + 16, _ptr_ ) )
#define vload_float2x3( _offset_, _ptr_ )	__float2x3( vload3( _offset_, _ptr_ ), vload3( (_offset_) + 16, _ptr_ ) )
#define vload_float2x4( _offset_, _ptr_ )	__float2x4( vload4( _offset_, _ptr_ ), vload4( (_offset_) + 16, _ptr_ ) )

#define vload_float3x2( _offset_, _ptr_ )	__float3x2( vload2( _offset_, _ptr_ ), vload2( (_offset_) + 16, _ptr_ ), vload2( (_offset_) + 32, _ptr_ ) )
#define vload_float3x3( _offset_, _ptr_ )	__float3x3( vload3( _offset_, _ptr_ ), vload3( (_offset_) + 16, _ptr_ ), vload3( (_offset_) + 32, _ptr_ ) )
#define vload_float3x4( _offset_, _ptr_ )	__float3x4( vload4( _offset_, _ptr_ ), vload4( (_offset_) + 16, _ptr_ ), vload4( (_offset_) + 32, _ptr_ ) )

#define vload_float4x2( _offset_, _ptr_ )	__float4x2( vload2( _offset_, _ptr_ ), vload2( (_offset_) + 16, _ptr_ ), vload2( (_offset_) + 32, _ptr_ ), vload2( (_offset_) + 48, _ptr_ ) )
#define vload_float4x3( _offset_, _ptr_ )	__float4x3( vload3( _offset_, _ptr_ ), vload3( (_offset_) + 16, _ptr_ ), vload3( (_offset_) + 32, _ptr_ ), vload3( (_offset_) + 48, _ptr_ ) )
#define vload_float4x4( _offset_, _ptr_ )	__float4x4( vload4( _offset_, _ptr_ ), vload4( (_offset_) + 16, _ptr_ ), vload4( (_offset_) + 32, _ptr_ ), vload4( (_offset_) + 48, _ptr_ ) )

#define vload_double2x2( _offset_, _ptr_ )	__double2x2( vload_double2( _offset_, _ptr_ ), vload_double2( (_offset_) + 32, _ptr_ ) )
#define vload_double2x3( _offset_, _ptr_ )	__double2x3( vload_double3( _offset_, _ptr_ ), vload_double3( (_offset_) + 32, _ptr_ ) )
#define vload_double2x4( _offset_, _ptr_ )	__double2x4( vload_double4( _offset_, _ptr_ ), vload_double4( (_offset_) + 32, _ptr_ ) )

#define vload_double3x2( _offset_, _ptr_ )	__double3x2( vload_double2( _offset_, _ptr_ ), vload_double2( (_offset_) + 32, _ptr_ ), vload_double2( (_offset_) + 64, _ptr_ ) )
#define vload_double3x3( _offset_, _ptr_ )	__double3x3( vload_double3( _offset_, _ptr_ ), vload_double3( (_offset_) + 32, _ptr_ ), vload_double3( (_offset_) + 64, _ptr_ ) )
#define vload_double3x4( _offset_, _ptr_ )	__double3x4( vload_double4( _offset_, _ptr_ ), vload_double4( (_offset_) + 32, _ptr_ ), vload_double4( (_offset_) + 64, _ptr_ ) )

#define vload_double4x2( _offset_, _ptr_ )	__double4x2( vload_double2( _offset_, _ptr_ ), vload_double2( (_offset_) + 32, _ptr_ ), vload_double2( (_offset_) + 64, _ptr_ ), vload_double2( (_offset_) + 96, _ptr_ ) )
#define vload_double4x3( _offset_, _ptr_ )	__double4x3( vload_double3( _offset_, _ptr_ ), vload_double3( (_offset_) + 32, _ptr_ ), vload_double3( (_offset_) + 64, _ptr_ ), vload_double3( (_offset_) + 96, _ptr_ ) )
#define vload_double4x4( _offset_, _ptr_ )	__double4x4( vload_double4( _offset_, _ptr_ ), vload_double4( (_offset_) + 32, _ptr_ ), vload_double4( (_offset_) + 64, _ptr_ ), vload_double4( (_offset_) + 96, _ptr_ ) )


#define vstore_float1( _data_, _offset_, _ptr_ )	vstore1( _data_, _offset_, _ptr_ )
#define vstore_float2( _data_, _offset_, _ptr_ )	vstore2( _data_, _offset_, _ptr_ )
#define vstore_float3( _data_, _offset_, _ptr_ )	vstore3( _data_, _offset_, _ptr_ )
#define vstore_float4( _data_, _offset_, _ptr_ )	vstore4( _data_, _offset_, _ptr_ )

#define vstore_int1( _data_, _offset_, _ptr_ )		vstore1( as_float(_data_), _offset_, _ptr_ )
#define vstore_int2( _data_, _offset_, _ptr_ )		vstore2( as_float2(_data_), _offset_, _ptr_ )
#define vstore_int3( _data_, _offset_, _ptr_ )		vstore3( as_float3(_data_), _offset_, _ptr_ )
#define vstore_int4( _data_, _offset_, _ptr_ )		vstore4( as_float4(_data_), _offset_, _ptr_ )

#define vstore_uint1( _data_, _offset_, _ptr_ )		vstore1( as_float(_data_), _offset_, _ptr_ )
#define vstore_uint2( _data_, _offset_, _ptr_ )		vstore2( as_float2(_data_), _offset_, _ptr_ )
#define vstore_uint3( _data_, _offset_, _ptr_ )		vstore3( as_float3(_data_), _offset_, _ptr_ )
#define vstore_uint4( _data_, _offset_, _ptr_ )		vstore4( as_float4(_data_), _offset_, _ptr_ )

#define vstore_double1( _data_, _offset_, _ptr_ )	vstore2( as_float2((_data_).x), _offset_, _ptr_ )
#define vstore_double2( _data_, _offset_, _ptr_ )	vstore4( as_float4((_data_).xy), _offset_, _ptr_ )
#define vstore_double3( _data_, _offset_, _ptr_ )	{ vstore4( as_float4((_data_).xy), _offset_, _ptr_ ); vstore2( as_float2((_data_).z), (_offset_) + 16, _ptr_ ); }
#define vstore_double4( _data_, _offset_, _ptr_ )	{ vstore4( as_float4((_data_).xy), _offset_, _ptr_ ); vstore4( as_float4((_data_).zw), (_offset_) + 16, _ptr_ ); }

#define vstore_long1( _data_, _offset_, _ptr_ )		vstore2( as_float2((_data_).x), _offset_, _ptr_ )
#define vstore_long2( _data_, _offset_, _ptr_ )		vstore4( as_float4((_data_).xy), _offset_, _ptr_ )
#define vstore_long3( _data_, _offset_, _ptr_ )		{ vstore4( as_float4((_data_).xy), _offset_, _ptr_ ); vstore2( as_float2((_data_).z), (_offset_) + 16, _ptr_ ); }
#define vstore_long4( _data_, _offset_, _ptr_ )		{ vstore4( as_float4((_data_).xy), _offset_, _ptr_ ); vstore4( as_float4((_data_).zw), (_offset_) + 16, _ptr_ ); }

#define vstore_ulong1( _data_, _offset_, _ptr_ )	vstore2( as_float2((_data_).x), _offset_, _ptr_ )
#define vstore_ulong2( _data_, _offset_, _ptr_ )	vstore4( as_float4((_data_).xy), _offset_, _ptr_ )
#define vstore_ulong3( _data_, _offset_, _ptr_ )	{ vstore4( as_float4((_data_).xy), _offset_, _ptr_ ); vstore2( as_float2((_data_).z), (_offset_) + 16, _ptr_ ); }
#define vstore_ulong4( _data_, _offset_, _ptr_ )	{ vstore4( as_float4((_data_).xy), _offset_, _ptr_ ); vstore4( as_float4((_data_).zw), (_offset_) + 16, _ptr_ ); }


#define vstore_float2x2( _data_, _offset_, _ptr_ )	{ vstore2( (_data_)[0], _offset_, _ptr_ ); vstore2( (_data_)[1], (_offset_) + 16, _ptr_ ); }
#define vstore_float2x3( _data_, _offset_, _ptr_ )	{ vstore3( (_data_)[0], _offset_, _ptr_ ); vstore3( (_data_)[1], (_offset_) + 16, _ptr_ ); }
#define vstore_float2x4( _data_, _offset_, _ptr_ )	{ vstore4( (_data_)[0], _offset_, _ptr_ ); vstore4( (_data_)[1], (_offset_) + 16, _ptr_ ); }

#define vstore_float3x2( _data_, _offset_, _ptr_ )	{ vstore2( (_data_)[0], _offset_, _ptr_ ); vstore2( (_data_)[1], (_offset_) + 16, _ptr_ ); vstore2( (_data_)[2], (_offset_) + 32, _ptr_ ); }
#define vstore_float3x3( _data_, _offset_, _ptr_ )	{ vstore3( (_data_)[0], _offset_, _ptr_ ); vstore3( (_data_)[1], (_offset_) + 16, _ptr_ ); vstore3( (_data_)[2], (_offset_) + 32, _ptr_ ); }
#define vstore_float3x4( _data_, _offset_, _ptr_ )	{ vstore4( (_data_)[0], _offset_, _ptr_ ); vstore4( (_data_)[1], (_offset_) + 16, _ptr_ ); vstore4( (_data_)[2], (_offset_) + 32, _ptr_ ); }

#define vstore_float4x2( _data_, _offset_, _ptr_ )	{ vstore2( (_data_)[0], _offset_, _ptr_ ); vstore2( (_data_)[1], (_offset_) + 16, _ptr_ ); vstore2( (_data_)[2], (_offset_) + 32, _ptr_ ); vstore2( (_data_)[3], (_offset_) + 48, _ptr_ ); }
#define vstore_float4x3( _data_, _offset_, _ptr_ )	{ vstore3( (_data_)[0], _offset_, _ptr_ ); vstore3( (_data_)[1], (_offset_) + 16, _ptr_ ); vstore3( (_data_)[2], (_offset_) + 32, _ptr_ ); vstore2( (_data_)[3], (_offset_) + 48, _ptr_ ); }
#define vstore_float4x4( _data_, _offset_, _ptr_ )	{ vstore4( (_data_)[0], _offset_, _ptr_ ); vstore4( (_data_)[1], (_offset_) + 16, _ptr_ ); vstore4( (_data_)[2], (_offset_) + 32, _ptr_ ); vstore2( (_data_)[3], (_offset_) + 48, _ptr_ ); }
)#";

	}
}	// cl_vfs
