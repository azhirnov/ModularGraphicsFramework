# Import-Export Modules

Supported loaders:
- GXImage (WIP)
- GXMesh (WIP)

Supported savers:
- GXImage (WIP)
- GXMesh (WIP)
 
 
## GXImage format
Header
```cpp
[ 0] ulong	version;	// file type and version (magic number)
[ 8] uint	width;		// in pixels
[12] uint	height;		// in pixels
[16] uint	depth;		// in pixels
[20] uint	layers;
[24] uint	maxLevels;
[28] uint	samples;
[32] uint	flags;		// CubeMap, ...
[36] uint	pixelFormat;
[40] uint	blockSize;	// in bytes		// can be used for streaming, may be (max_row_pitch * N)
[44] uint	levelsSize;	// in bytes		// 
[48] -----------

// 1D, 2D, 3D mipmap level
struct Level
{
	uint	memOffset;		// in bytes		// pixels = memOffset + baseOffset
	uint3	dimension;		// in pixels
	uint	layer;
	uint	level;
	uint	rowPitch;		// in bytes		// for 2D, 3D
	uint	slicePitch;		// in bytes		// for 3D
};

[48] Mipmaps	levels[header.layers * header.maxLevels];
```

Data can be placed separately from header.
 
 
## GXMesh format
TODO
