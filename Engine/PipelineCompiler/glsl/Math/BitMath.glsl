/*
	This is generated file, don't change anything!

	license: free

	depends of:
		Common/Cmp
		Math/MathDef
*/

#define BitScanReverse				findMSB
#define BitScanForward				findLSB
#define IntLog2						BitScanReverse


// ToMask
int ToMask (const int x);
int2 ToMask (const int2 x);
int3 ToMask (const int3 x);
int4 ToMask (const int4 x);

uint ToMask (const uint x);
uint2 ToMask (const uint2 x);
uint3 ToMask (const uint3 x);
uint4 ToMask (const uint4 x);

#ifdef ARB_gpu_shader_int64_enabled
ilong ToMask (const ilong x);
ilong2 ToMask (const ilong2 x);
ilong3 ToMask (const ilong3 x);
ilong4 ToMask (const ilong4 x);
#endif

#ifdef ARB_gpu_shader_int64_enabled
ulong ToMask (const ulong x);
ulong2 ToMask (const ulong2 x);
ulong3 ToMask (const ulong3 x);
ulong4 ToMask (const ulong4 x);
#endif


// BitRotateLeft, BitRotateRight
int BitRotateLeft  (const int x, uint shift);
int2 BitRotateLeft  (const int2 x, uint shift);
int2 BitRotateLeft  (const int2 x, uint2 shift);
int3 BitRotateLeft  (const int3 x, uint shift);
int3 BitRotateLeft  (const int3 x, uint3 shift);
int4 BitRotateLeft  (const int4 x, uint shift);
int4 BitRotateLeft  (const int4 x, uint4 shift);

uint BitRotateLeft  (const uint x, uint shift);
uint2 BitRotateLeft  (const uint2 x, uint shift);
uint2 BitRotateLeft  (const uint2 x, uint2 shift);
uint3 BitRotateLeft  (const uint3 x, uint shift);
uint3 BitRotateLeft  (const uint3 x, uint3 shift);
uint4 BitRotateLeft  (const uint4 x, uint shift);
uint4 BitRotateLeft  (const uint4 x, uint4 shift);

#ifdef ARB_gpu_shader_int64_enabled
ilong BitRotateLeft  (const ilong x, uint shift);
ilong2 BitRotateLeft  (const ilong2 x, uint shift);
ilong2 BitRotateLeft  (const ilong2 x, uint2 shift);
ilong3 BitRotateLeft  (const ilong3 x, uint shift);
ilong3 BitRotateLeft  (const ilong3 x, uint3 shift);
ilong4 BitRotateLeft  (const ilong4 x, uint shift);
ilong4 BitRotateLeft  (const ilong4 x, uint4 shift);
#endif

#ifdef ARB_gpu_shader_int64_enabled
ulong BitRotateLeft  (const ulong x, uint shift);
ulong2 BitRotateLeft  (const ulong2 x, uint shift);
ulong2 BitRotateLeft  (const ulong2 x, uint2 shift);
ulong3 BitRotateLeft  (const ulong3 x, uint shift);
ulong3 BitRotateLeft  (const ulong3 x, uint3 shift);
ulong4 BitRotateLeft  (const ulong4 x, uint shift);
ulong4 BitRotateLeft  (const ulong4 x, uint4 shift);
#endif

int BitRotateRight (const int x, uint shift);
int2 BitRotateRight (const int2 x, uint shift);
int2 BitRotateRight (const int2 x, uint2 shift);
int3 BitRotateRight (const int3 x, uint shift);
int3 BitRotateRight (const int3 x, uint3 shift);
int4 BitRotateRight (const int4 x, uint shift);
int4 BitRotateRight (const int4 x, uint4 shift);

uint BitRotateRight (const uint x, uint shift);
uint2 BitRotateRight (const uint2 x, uint shift);
uint2 BitRotateRight (const uint2 x, uint2 shift);
uint3 BitRotateRight (const uint3 x, uint shift);
uint3 BitRotateRight (const uint3 x, uint3 shift);
uint4 BitRotateRight (const uint4 x, uint shift);
uint4 BitRotateRight (const uint4 x, uint4 shift);

#ifdef ARB_gpu_shader_int64_enabled
ilong BitRotateRight (const ilong x, uint shift);
ilong2 BitRotateRight (const ilong2 x, uint shift);
ilong2 BitRotateRight (const ilong2 x, uint2 shift);
ilong3 BitRotateRight (const ilong3 x, uint shift);
ilong3 BitRotateRight (const ilong3 x, uint3 shift);
ilong4 BitRotateRight (const ilong4 x, uint shift);
ilong4 BitRotateRight (const ilong4 x, uint4 shift);
#endif

#ifdef ARB_gpu_shader_int64_enabled
ulong BitRotateRight (const ulong x, uint shift);
ulong2 BitRotateRight (const ulong2 x, uint shift);
ulong2 BitRotateRight (const ulong2 x, uint2 shift);
ulong3 BitRotateRight (const ulong3 x, uint shift);
ulong3 BitRotateRight (const ulong3 x, uint3 shift);
ulong4 BitRotateRight (const ulong4 x, uint shift);
ulong4 BitRotateRight (const ulong4 x, uint4 shift);
#endif


// BitScanForward, BitScanReverse
int BitScanForward (const int x);
int BitScanReverse (const int x);
int2 BitScanForward (const int2 x);
int2 BitScanReverse (const int2 x);
int3 BitScanForward (const int3 x);
int3 BitScanReverse (const int3 x);
int4 BitScanForward (const int4 x);
int4 BitScanReverse (const int4 x);

uint BitScanForward (const uint x);
uint BitScanReverse (const uint x);
uint2 BitScanForward (const uint2 x);
uint2 BitScanReverse (const uint2 x);
uint3 BitScanForward (const uint3 x);
uint3 BitScanReverse (const uint3 x);
uint4 BitScanForward (const uint4 x);
uint4 BitScanReverse (const uint4 x);

#ifdef ARB_gpu_shader_int64_enabled
ilong BitScanForward (const ilong x);
ilong BitScanReverse (const ilong x);
ilong2 BitScanForward (const ilong2 x);
ilong2 BitScanReverse (const ilong2 x);
ilong3 BitScanForward (const ilong3 x);
ilong3 BitScanReverse (const ilong3 x);
ilong4 BitScanForward (const ilong4 x);
ilong4 BitScanReverse (const ilong4 x);
#endif

#ifdef ARB_gpu_shader_int64_enabled
ulong BitScanForward (const ulong x);
ulong BitScanReverse (const ulong x);
ulong2 BitScanForward (const ulong2 x);
ulong2 BitScanReverse (const ulong2 x);
ulong3 BitScanForward (const ulong3 x);
ulong3 BitScanReverse (const ulong3 x);
ulong4 BitScanForward (const ulong4 x);
ulong4 BitScanReverse (const ulong4 x);
#endif



//---------------------------------------------------------


// ToMask
int ToMask (const int x) { return x < 0 ? 0 : x < 32 ? (int(1) << x) - 1 : int(-1); }
int2 ToMask (const int2 x) { return int2( ToMask(x[0]), ToMask(x[1]) ); }
int3 ToMask (const int3 x) { return int3( ToMask(x[0]), ToMask(x[1]), ToMask(x[2]) ); }
int4 ToMask (const int4 x) { return int4( ToMask(x[0]), ToMask(x[1]), ToMask(x[2]), ToMask(x[3]) ); }

uint ToMask (const uint x) { return x < 0 ? 0 : x < 32 ? (uint(1) << x) - 1 : uint(-1); }
uint2 ToMask (const uint2 x) { return uint2( ToMask(x[0]), ToMask(x[1]) ); }
uint3 ToMask (const uint3 x) { return uint3( ToMask(x[0]), ToMask(x[1]), ToMask(x[2]) ); }
uint4 ToMask (const uint4 x) { return uint4( ToMask(x[0]), ToMask(x[1]), ToMask(x[2]), ToMask(x[3]) ); }

#ifdef ARB_gpu_shader_int64_enabled
ilong ToMask (const ilong x) { return x < 0 ? 0 : x < 64 ? (ilong(1) << x) - 1 : ilong(-1); }
ilong2 ToMask (const ilong2 x) { return ilong2( ToMask(x[0]), ToMask(x[1]) ); }
ilong3 ToMask (const ilong3 x) { return ilong3( ToMask(x[0]), ToMask(x[1]), ToMask(x[2]) ); }
ilong4 ToMask (const ilong4 x) { return ilong4( ToMask(x[0]), ToMask(x[1]), ToMask(x[2]), ToMask(x[3]) ); }
#endif

#ifdef ARB_gpu_shader_int64_enabled
ulong ToMask (const ulong x) { return x < 0 ? 0 : x < 64 ? (ulong(1) << x) - 1 : ulong(-1); }
ulong2 ToMask (const ulong2 x) { return ulong2( ToMask(x[0]), ToMask(x[1]) ); }
ulong3 ToMask (const ulong3 x) { return ulong3( ToMask(x[0]), ToMask(x[1]), ToMask(x[2]) ); }
ulong4 ToMask (const ulong4 x) { return ulong4( ToMask(x[0]), ToMask(x[1]), ToMask(x[2]), ToMask(x[3]) ); }
#endif


// BitRotateLeft, BitRotateRight
int BitRotateLeft  (const int x, uint shift)
{
	const uint mask = 31;
	shift = shift & mask;
	return (x << shift) | (x >> ( ~(shift-1) & mask ));
}
int2 BitRotateLeft  (const int2 x, uint shift) { return BitRotateLeft( x, uint2(shift) ); }
int2 BitRotateLeft  (const int2 x, uint2 shift)
{
	const uint mask = 31;
	shift = shift & mask;
	return (x << shift) | (x >> ( ~(shift-1) & mask ));
}
int3 BitRotateLeft  (const int3 x, uint shift) { return BitRotateLeft( x, uint3(shift) ); }
int3 BitRotateLeft  (const int3 x, uint3 shift)
{
	const uint mask = 31;
	shift = shift & mask;
	return (x << shift) | (x >> ( ~(shift-1) & mask ));
}
int4 BitRotateLeft  (const int4 x, uint shift) { return BitRotateLeft( x, uint4(shift) ); }
int4 BitRotateLeft  (const int4 x, uint4 shift)
{
	const uint mask = 31;
	shift = shift & mask;
	return (x << shift) | (x >> ( ~(shift-1) & mask ));
}

uint BitRotateLeft  (const uint x, uint shift)
{
	const uint mask = 31;
	shift = shift & mask;
	return (x << shift) | (x >> ( ~(shift-1) & mask ));
}
uint2 BitRotateLeft  (const uint2 x, uint shift) { return BitRotateLeft( x, uint2(shift) ); }
uint2 BitRotateLeft  (const uint2 x, uint2 shift)
{
	const uint mask = 31;
	shift = shift & mask;
	return (x << shift) | (x >> ( ~(shift-1) & mask ));
}
uint3 BitRotateLeft  (const uint3 x, uint shift) { return BitRotateLeft( x, uint3(shift) ); }
uint3 BitRotateLeft  (const uint3 x, uint3 shift)
{
	const uint mask = 31;
	shift = shift & mask;
	return (x << shift) | (x >> ( ~(shift-1) & mask ));
}
uint4 BitRotateLeft  (const uint4 x, uint shift) { return BitRotateLeft( x, uint4(shift) ); }
uint4 BitRotateLeft  (const uint4 x, uint4 shift)
{
	const uint mask = 31;
	shift = shift & mask;
	return (x << shift) | (x >> ( ~(shift-1) & mask ));
}

#ifdef ARB_gpu_shader_int64_enabled
ilong BitRotateLeft  (const ilong x, uint shift)
{
	const uint mask = 63;
	shift = shift & mask;
	return (x << shift) | (x >> ( ~(shift-1) & mask ));
}
ilong2 BitRotateLeft  (const ilong2 x, uint shift) { return BitRotateLeft( x, uint2(shift) ); }
ilong2 BitRotateLeft  (const ilong2 x, uint2 shift)
{
	const uint mask = 63;
	shift = shift & mask;
	return (x << shift) | (x >> ( ~(shift-1) & mask ));
}
ilong3 BitRotateLeft  (const ilong3 x, uint shift) { return BitRotateLeft( x, uint3(shift) ); }
ilong3 BitRotateLeft  (const ilong3 x, uint3 shift)
{
	const uint mask = 63;
	shift = shift & mask;
	return (x << shift) | (x >> ( ~(shift-1) & mask ));
}
ilong4 BitRotateLeft  (const ilong4 x, uint shift) { return BitRotateLeft( x, uint4(shift) ); }
ilong4 BitRotateLeft  (const ilong4 x, uint4 shift)
{
	const uint mask = 63;
	shift = shift & mask;
	return (x << shift) | (x >> ( ~(shift-1) & mask ));
}
#endif

#ifdef ARB_gpu_shader_int64_enabled
ulong BitRotateLeft  (const ulong x, uint shift)
{
	const uint mask = 63;
	shift = shift & mask;
	return (x << shift) | (x >> ( ~(shift-1) & mask ));
}
ulong2 BitRotateLeft  (const ulong2 x, uint shift) { return BitRotateLeft( x, uint2(shift) ); }
ulong2 BitRotateLeft  (const ulong2 x, uint2 shift)
{
	const uint mask = 63;
	shift = shift & mask;
	return (x << shift) | (x >> ( ~(shift-1) & mask ));
}
ulong3 BitRotateLeft  (const ulong3 x, uint shift) { return BitRotateLeft( x, uint3(shift) ); }
ulong3 BitRotateLeft  (const ulong3 x, uint3 shift)
{
	const uint mask = 63;
	shift = shift & mask;
	return (x << shift) | (x >> ( ~(shift-1) & mask ));
}
ulong4 BitRotateLeft  (const ulong4 x, uint shift) { return BitRotateLeft( x, uint4(shift) ); }
ulong4 BitRotateLeft  (const ulong4 x, uint4 shift)
{
	const uint mask = 63;
	shift = shift & mask;
	return (x << shift) | (x >> ( ~(shift-1) & mask ));
}
#endif

int BitRotateRight (const int x, uint shift)
{
	const uint mask = 31;
	shift = shift & mask;
	return (x >> shift) | (x << ( ~(shift-1) & mask ));
}
int2 BitRotateRight (const int2 x, uint shift) { return BitRotateRight( x, uint2(shift) ); }
int2 BitRotateRight (const int2 x, uint2 shift)
{
	const uint mask = 31;
	shift = shift & mask;
	return (x >> shift) | (x << ( ~(shift-1) & mask ));
}
int3 BitRotateRight (const int3 x, uint shift) { return BitRotateRight( x, uint3(shift) ); }
int3 BitRotateRight (const int3 x, uint3 shift)
{
	const uint mask = 31;
	shift = shift & mask;
	return (x >> shift) | (x << ( ~(shift-1) & mask ));
}
int4 BitRotateRight (const int4 x, uint shift) { return BitRotateRight( x, uint4(shift) ); }
int4 BitRotateRight (const int4 x, uint4 shift)
{
	const uint mask = 31;
	shift = shift & mask;
	return (x >> shift) | (x << ( ~(shift-1) & mask ));
}

uint BitRotateRight (const uint x, uint shift)
{
	const uint mask = 31;
	shift = shift & mask;
	return (x >> shift) | (x << ( ~(shift-1) & mask ));
}
uint2 BitRotateRight (const uint2 x, uint shift) { return BitRotateRight( x, uint2(shift) ); }
uint2 BitRotateRight (const uint2 x, uint2 shift)
{
	const uint mask = 31;
	shift = shift & mask;
	return (x >> shift) | (x << ( ~(shift-1) & mask ));
}
uint3 BitRotateRight (const uint3 x, uint shift) { return BitRotateRight( x, uint3(shift) ); }
uint3 BitRotateRight (const uint3 x, uint3 shift)
{
	const uint mask = 31;
	shift = shift & mask;
	return (x >> shift) | (x << ( ~(shift-1) & mask ));
}
uint4 BitRotateRight (const uint4 x, uint shift) { return BitRotateRight( x, uint4(shift) ); }
uint4 BitRotateRight (const uint4 x, uint4 shift)
{
	const uint mask = 31;
	shift = shift & mask;
	return (x >> shift) | (x << ( ~(shift-1) & mask ));
}

#ifdef ARB_gpu_shader_int64_enabled
ilong BitRotateRight (const ilong x, uint shift)
{
	const uint mask = 63;
	shift = shift & mask;
	return (x >> shift) | (x << ( ~(shift-1) & mask ));
}
ilong2 BitRotateRight (const ilong2 x, uint shift) { return BitRotateRight( x, uint2(shift) ); }
ilong2 BitRotateRight (const ilong2 x, uint2 shift)
{
	const uint mask = 63;
	shift = shift & mask;
	return (x >> shift) | (x << ( ~(shift-1) & mask ));
}
ilong3 BitRotateRight (const ilong3 x, uint shift) { return BitRotateRight( x, uint3(shift) ); }
ilong3 BitRotateRight (const ilong3 x, uint3 shift)
{
	const uint mask = 63;
	shift = shift & mask;
	return (x >> shift) | (x << ( ~(shift-1) & mask ));
}
ilong4 BitRotateRight (const ilong4 x, uint shift) { return BitRotateRight( x, uint4(shift) ); }
ilong4 BitRotateRight (const ilong4 x, uint4 shift)
{
	const uint mask = 63;
	shift = shift & mask;
	return (x >> shift) | (x << ( ~(shift-1) & mask ));
}
#endif

#ifdef ARB_gpu_shader_int64_enabled
ulong BitRotateRight (const ulong x, uint shift)
{
	const uint mask = 63;
	shift = shift & mask;
	return (x >> shift) | (x << ( ~(shift-1) & mask ));
}
ulong2 BitRotateRight (const ulong2 x, uint shift) { return BitRotateRight( x, uint2(shift) ); }
ulong2 BitRotateRight (const ulong2 x, uint2 shift)
{
	const uint mask = 63;
	shift = shift & mask;
	return (x >> shift) | (x << ( ~(shift-1) & mask ));
}
ulong3 BitRotateRight (const ulong3 x, uint shift) { return BitRotateRight( x, uint3(shift) ); }
ulong3 BitRotateRight (const ulong3 x, uint3 shift)
{
	const uint mask = 63;
	shift = shift & mask;
	return (x >> shift) | (x << ( ~(shift-1) & mask ));
}
ulong4 BitRotateRight (const ulong4 x, uint shift) { return BitRotateRight( x, uint4(shift) ); }
ulong4 BitRotateRight (const ulong4 x, uint4 shift)
{
	const uint mask = 63;
	shift = shift & mask;
	return (x >> shift) | (x << ( ~(shift-1) & mask ));
}
#endif


// BitScanForward, BitScanReverse
#ifdef ARB_gpu_shader_int64_enabled
# define BIT_SCAN_FORWARD( _type_ ) \
		_type_( Greater( x >> 63, zero ) & Less( x & _type_(0x7FFFFFFFFFFFFFFFUL, zero )) ) | \
		_type_( Greater( x >> 62, zero ) & Less( x & _type_(0x3FFFFFFFFFFFFFFFUL, zero )) ) | \
		_type_( Greater( x >> 61, zero ) & Less( x & _type_(0x1FFFFFFFFFFFFFFFUL, zero )) ) | \
		_type_( Greater( x >> 60, zero ) & Less( x & _type_(0xFFFFFFFFFFFFFFFUL, zero )) ) | \
		_type_( Greater( x >> 59, zero ) & Less( x & _type_(0x7FFFFFFFFFFFFFFUL, zero )) ) | \
		_type_( Greater( x >> 58, zero ) & Less( x & _type_(0x3FFFFFFFFFFFFFFUL, zero )) ) | \
		_type_( Greater( x >> 57, zero ) & Less( x & _type_(0x1FFFFFFFFFFFFFFUL, zero )) ) | \
		_type_( Greater( x >> 56, zero ) & Less( x & _type_(0xFFFFFFFFFFFFFFUL, zero )) ) | \
		_type_( Greater( x >> 55, zero ) & Less( x & _type_(0x7FFFFFFFFFFFFFUL, zero )) ) | \
		_type_( Greater( x >> 54, zero ) & Less( x & _type_(0x3FFFFFFFFFFFFFUL, zero )) ) | \
		_type_( Greater( x >> 53, zero ) & Less( x & _type_(0x1FFFFFFFFFFFFFUL, zero )) ) | \
		_type_( Greater( x >> 52, zero ) & Less( x & _type_(0xFFFFFFFFFFFFFUL, zero )) ) | \
		_type_( Greater( x >> 51, zero ) & Less( x & _type_(0x7FFFFFFFFFFFFUL, zero )) ) | \
		_type_( Greater( x >> 50, zero ) & Less( x & _type_(0x3FFFFFFFFFFFFUL, zero )) ) | \
		_type_( Greater( x >> 49, zero ) & Less( x & _type_(0x1FFFFFFFFFFFFUL, zero )) ) | \
		_type_( Greater( x >> 48, zero ) & Less( x & _type_(0xFFFFFFFFFFFFUL, zero )) ) | \
		_type_( Greater( x >> 47, zero ) & Less( x & _type_(0x7FFFFFFFFFFFUL, zero )) ) | \
		_type_( Greater( x >> 46, zero ) & Less( x & _type_(0x3FFFFFFFFFFFUL, zero )) ) | \
		_type_( Greater( x >> 45, zero ) & Less( x & _type_(0x1FFFFFFFFFFFUL, zero )) ) | \
		_type_( Greater( x >> 44, zero ) & Less( x & _type_(0xFFFFFFFFFFFUL, zero )) ) | \
		_type_( Greater( x >> 43, zero ) & Less( x & _type_(0x7FFFFFFFFFFUL, zero )) ) | \
		_type_( Greater( x >> 42, zero ) & Less( x & _type_(0x3FFFFFFFFFFUL, zero )) ) | \
		_type_( Greater( x >> 41, zero ) & Less( x & _type_(0x1FFFFFFFFFFUL, zero )) ) | \
		_type_( Greater( x >> 40, zero ) & Less( x & _type_(0xFFFFFFFFFFUL, zero )) ) | \
		_type_( Greater( x >> 39, zero ) & Less( x & _type_(0x7FFFFFFFFFUL, zero )) ) | \
		_type_( Greater( x >> 38, zero ) & Less( x & _type_(0x3FFFFFFFFFUL, zero )) ) | \
		_type_( Greater( x >> 37, zero ) & Less( x & _type_(0x1FFFFFFFFFUL, zero )) ) | \
		_type_( Greater( x >> 36, zero ) & Less( x & _type_(0xFFFFFFFFFUL, zero )) ) | \
		_type_( Greater( x >> 35, zero ) & Less( x & _type_(0x7FFFFFFFFUL, zero )) ) | \
		_type_( Greater( x >> 34, zero ) & Less( x & _type_(0x3FFFFFFFFUL, zero )) ) | \
		_type_( Greater( x >> 33, zero ) & Less( x & _type_(0x1FFFFFFFFUL, zero )) ) | \
		_type_( Greater( x >> 32, zero ) & Less( x & _type_(0xFFFFFFFFUL, zero )) ) | \
		_type_( Greater( x >> 31, zero ) & Less( x & _type_(0x7FFFFFFFUL, zero )) ) | \
		_type_( Greater( x >> 30, zero ) & Less( x & _type_(0x3FFFFFFFUL, zero )) ) | \
		_type_( Greater( x >> 29, zero ) & Less( x & _type_(0x1FFFFFFFUL, zero )) ) | \
		_type_( Greater( x >> 28, zero ) & Less( x & _type_(0xFFFFFFFUL, zero )) ) | \
		_type_( Greater( x >> 27, zero ) & Less( x & _type_(0x7FFFFFFUL, zero )) ) | \
		_type_( Greater( x >> 26, zero ) & Less( x & _type_(0x3FFFFFFUL, zero )) ) | \
		_type_( Greater( x >> 25, zero ) & Less( x & _type_(0x1FFFFFFUL, zero )) ) | \
		_type_( Greater( x >> 24, zero ) & Less( x & _type_(0xFFFFFFUL, zero )) ) | \
		_type_( Greater( x >> 23, zero ) & Less( x & _type_(0x7FFFFFUL, zero )) ) | \
		_type_( Greater( x >> 22, zero ) & Less( x & _type_(0x3FFFFFUL, zero )) ) | \
		_type_( Greater( x >> 21, zero ) & Less( x & _type_(0x1FFFFFUL, zero )) ) | \
		_type_( Greater( x >> 20, zero ) & Less( x & _type_(0xFFFFFUL, zero )) ) | \
		_type_( Greater( x >> 19, zero ) & Less( x & _type_(0x7FFFFUL, zero )) ) | \
		_type_( Greater( x >> 18, zero ) & Less( x & _type_(0x3FFFFUL, zero )) ) | \
		_type_( Greater( x >> 17, zero ) & Less( x & _type_(0x1FFFFUL, zero )) ) | \
		_type_( Greater( x >> 16, zero ) & Less( x & _type_(0xFFFFUL, zero )) ) | \
		_type_( Greater( x >> 15, zero ) & Less( x & _type_(0x7FFFUL, zero )) ) | \
		_type_( Greater( x >> 14, zero ) & Less( x & _type_(0x3FFFUL, zero )) ) | \
		_type_( Greater( x >> 13, zero ) & Less( x & _type_(0x1FFFUL, zero )) ) | \
		_type_( Greater( x >> 12, zero ) & Less( x & _type_(0xFFFUL, zero )) ) | \
		_type_( Greater( x >> 11, zero ) & Less( x & _type_(0x7FFUL, zero )) ) | \
		_type_( Greater( x >> 10, zero ) & Less( x & _type_(0x3FFUL, zero )) ) | \
		_type_( Greater( x >> 9, zero ) & Less( x & _type_(0x1FFUL, zero )) ) | \
		_type_( Greater( x >> 8, zero ) & Less( x & _type_(0xFFUL, zero )) ) | \
		_type_( Greater( x >> 7, zero ) & Less( x & _type_(0x7FUL, zero )) ) | \
		_type_( Greater( x >> 6, zero ) & Less( x & _type_(0x3FUL, zero )) ) | \
		_type_( Greater( x >> 5, zero ) & Less( x & _type_(0x1FUL, zero )) ) | \
		_type_( Greater( x >> 4, zero ) & Less( x & _type_(0xFUL, zero )) ) | \
		_type_( Greater( x >> 3, zero ) & Less( x & _type_(0x7UL, zero )) ) | \
		_type_( Greater( x >> 2, zero ) & Less( x & _type_(0x3UL, zero )) ) | \
		_type_( Greater( x >> 1, zero ) & Less( x & _type_(0x1UL, zero )) ) | \
		_type_( Not(Not( x )) )

# define BIT_SCAN_REVERSE( _type_ ) \
		_type_(Not(Not( x >> 63 ))) | \
		_type_(Not(Not( x >> 62 ))) | \
		_type_(Not(Not( x >> 61 ))) | \
		_type_(Not(Not( x >> 60 ))) | \
		_type_(Not(Not( x >> 59 ))) | \
		_type_(Not(Not( x >> 58 ))) | \
		_type_(Not(Not( x >> 57 ))) | \
		_type_(Not(Not( x >> 56 ))) | \
		_type_(Not(Not( x >> 55 ))) | \
		_type_(Not(Not( x >> 54 ))) | \
		_type_(Not(Not( x >> 53 ))) | \
		_type_(Not(Not( x >> 52 ))) | \
		_type_(Not(Not( x >> 51 ))) | \
		_type_(Not(Not( x >> 50 ))) | \
		_type_(Not(Not( x >> 49 ))) | \
		_type_(Not(Not( x >> 48 ))) | \
		_type_(Not(Not( x >> 47 ))) | \
		_type_(Not(Not( x >> 46 ))) | \
		_type_(Not(Not( x >> 45 ))) | \
		_type_(Not(Not( x >> 44 ))) | \
		_type_(Not(Not( x >> 43 ))) | \
		_type_(Not(Not( x >> 42 ))) | \
		_type_(Not(Not( x >> 41 ))) | \
		_type_(Not(Not( x >> 40 ))) | \
		_type_(Not(Not( x >> 39 ))) | \
		_type_(Not(Not( x >> 38 ))) | \
		_type_(Not(Not( x >> 37 ))) | \
		_type_(Not(Not( x >> 36 ))) | \
		_type_(Not(Not( x >> 35 ))) | \
		_type_(Not(Not( x >> 34 ))) | \
		_type_(Not(Not( x >> 33 ))) | \
		_type_(Not(Not( x >> 32 ))) | \
		_type_(Not(Not( x >> 31 ))) | \
		_type_(Not(Not( x >> 30 ))) | \
		_type_(Not(Not( x >> 29 ))) | \
		_type_(Not(Not( x >> 28 ))) | \
		_type_(Not(Not( x >> 27 ))) | \
		_type_(Not(Not( x >> 26 ))) | \
		_type_(Not(Not( x >> 25 ))) | \
		_type_(Not(Not( x >> 24 ))) | \
		_type_(Not(Not( x >> 23 ))) | \
		_type_(Not(Not( x >> 22 ))) | \
		_type_(Not(Not( x >> 21 ))) | \
		_type_(Not(Not( x >> 20 ))) | \
		_type_(Not(Not( x >> 19 ))) | \
		_type_(Not(Not( x >> 18 ))) | \
		_type_(Not(Not( x >> 17 ))) | \
		_type_(Not(Not( x >> 16 ))) | \
		_type_(Not(Not( x >> 15 ))) | \
		_type_(Not(Not( x >> 14 ))) | \
		_type_(Not(Not( x >> 13 ))) | \
		_type_(Not(Not( x >> 12 ))) | \
		_type_(Not(Not( x >> 11 ))) | \
		_type_(Not(Not( x >> 10 ))) | \
		_type_(Not(Not( x >> 9 ))) | \
		_type_(Not(Not( x >> 8 ))) | \
		_type_(Not(Not( x >> 7 ))) | \
		_type_(Not(Not( x >> 6 ))) | \
		_type_(Not(Not( x >> 5 ))) | \
		_type_(Not(Not( x >> 4 ))) | \
		_type_(Not(Not( x >> 3 ))) | \
		_type_(Not(Not( x >> 2 ))) | \
		_type_(Not(Not( x >> 1 ))) | \
		_type_( Not(Not( x )) )

#endif // ARB_gpu_shader_int64_enabled

int BitScanForward (const int x)  { return findLSB( x ); }
int BitScanReverse (const int x)  { return findMSB( x ); }
int2 BitScanForward (const int2 x)  { return findLSB( x ); }
int2 BitScanReverse (const int2 x)  { return findMSB( x ); }
int3 BitScanForward (const int3 x)  { return findLSB( x ); }
int3 BitScanReverse (const int3 x)  { return findMSB( x ); }
int4 BitScanForward (const int4 x)  { return findLSB( x ); }
int4 BitScanReverse (const int4 x)  { return findMSB( x ); }

uint BitScanForward (const uint x)  { return findLSB( x ); }
uint BitScanReverse (const uint x)  { return findMSB( x ); }
uint2 BitScanForward (const uint2 x)  { return findLSB( x ); }
uint2 BitScanReverse (const uint2 x)  { return findMSB( x ); }
uint3 BitScanForward (const uint3 x)  { return findLSB( x ); }
uint3 BitScanReverse (const uint3 x)  { return findMSB( x ); }
uint4 BitScanForward (const uint4 x)  { return findLSB( x ); }
uint4 BitScanReverse (const uint4 x)  { return findMSB( x ); }

#ifdef ARB_gpu_shader_int64_enabled
ilong BitScanForward (const ilong x)  { return ilong( BitScanForward( ulong(x) ) ); }
ilong BitScanReverse (const ilong x)  { return ilong( BitScanReverse( ulong(x) ) ); }
ilong2 BitScanForward (const ilong2 x)  { return ilong2( BitScanForward( ulong2(x) ) ); }
ilong2 BitScanReverse (const ilong2 x)  { return ilong2( BitScanReverse( ulong2(x) ) ); }
ilong3 BitScanForward (const ilong3 x)  { return ilong3( BitScanForward( ulong3(x) ) ); }
ilong3 BitScanReverse (const ilong3 x)  { return ilong3( BitScanReverse( ulong3(x) ) ); }
ilong4 BitScanForward (const ilong4 x)  { return ilong4( BitScanForward( ulong4(x) ) ); }
ilong4 BitScanReverse (const ilong4 x)  { return ilong4( BitScanReverse( ulong4(x) ) ); }
#endif

#ifdef ARB_gpu_shader_int64_enabled
ulong BitScanForward (const ulong x)  { return (BIT_SCAN_FORWARD( ulong ))-1; }
ulong BitScanReverse (const ulong x)  { return (BIT_SCAN_REVERSE( ulong ))-1; }
ulong2 BitScanForward (const ulong2 x)  { return (BIT_SCAN_FORWARD( ulong2 ))-1; }
ulong2 BitScanReverse (const ulong2 x)  { return (BIT_SCAN_REVERSE( ulong2 ))-1; }
ulong3 BitScanForward (const ulong3 x)  { return (BIT_SCAN_FORWARD( ulong3 ))-1; }
ulong3 BitScanReverse (const ulong3 x)  { return (BIT_SCAN_REVERSE( ulong3 ))-1; }
ulong4 BitScanForward (const ulong4 x)  { return (BIT_SCAN_FORWARD( ulong4 ))-1; }
ulong4 BitScanReverse (const ulong4 x)  { return (BIT_SCAN_REVERSE( ulong4 ))-1; }
#endif

#undef BIT_SCAN_FORWARD
#undef BIT_SCAN_REVERSE
