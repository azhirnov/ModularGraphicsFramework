/*
	Used 'int' type instead of 'uint' becouse imageLoad/imageStore take 'int' type.
*/

#if SHADER == SH_COMPUTE || SHADER == SH_FRAGMENT

#if SHADER == SH_FRAGMENT
uniform uint2	unScreenResolution = uint2(1,1);
#endif


// global linear index
int   GetGlobalIndexSize ();
int   GetGlobalIndex ();		// 0..size-1
real  GetGlobalIndexUNorm ();	//  0..1
real  GetGlobalIndexSNorm ();	// -1..1

// local linear index
int   GetLocalIndexSize ();
int   GetLocalIndex ();			// 0..size-1
real  GetLocalIndexUNorm ();	//  0..1
real  GetLocalIndexSNorm ();	// -1..1

// group linear index
int   GetGroupIndexSize ();
int   GetGroupIndex ();			// 0..size-1
real  GetGroupIndexUNorm ();	//  0..1
real  GetGroupIndexSNorm ();	// -1..1

// global coordinate in 3D
int3   GetGlobalSize ();
int3   GetGlobalCoord ();		// 0..size-1
real3  GetGlobalCoordUNorm ();	//  0..1
real3  GetGlobalCoordSNorm ();	// -1..1

// local coordinate in 3D
int3   GetLocalSize ();
int3   GetLocalCoord ();		// 0..size-1
real3  GetLocalCoordUNorm ();	//  0..1
real3  GetLocalCoordSNorm ();	// -1..1

// group coordinate in 3D
int3   GetGroupSize ();
int3   GetGroupCoord ();		// 0..size-1
real3  GetGroupCoordUNorm ();	//  0..1
real3  GetGroupCoordSNorm ();	// -1..1

// global normalized coordinate in 2D with correction
real2  GetGlobalCoordUNormCorrected ();	//  0..1
real2  GetGlobalCoordSNormCorrected ();	// -1..1

#endif


//-----------------------------------------------------------------------------


#if SHADER == SH_COMPUTE

// global linear index


// local linear index
int GetLocalIndex ()
{
	return int( gl_LocalInvocationIndex );
}


// global coordinate in 3D
int3 GetGlobalCoord ()
{
	return int3( gl_GlobalInvocationID );
}

int3 GetGlobalSize ()
{
	return GetGroupSize() * GetLocalSize();
}


// local coordinate in 3D
int3 GetLocalCoord ()
{
	return int3( gl_LocalInvocationID );
}

int3 GetLocalSize ()
{
#ifdef GL_ARB_compute_variable_group_size	// TODO: it always true
	return int3( gl_LocalGroupSizeARB );
#else
	return int3( gl_WorkGroupSize );
#endif
}


// group coordinate in 3D
int3 GetGroupCoord ()
{
	return int3( gl_WorkGroupID );
}

int3 GetGroupSize ()
{
	return int3( gl_NumWorkGroups );
}

#endif	// SH_COMPUTE
//-----------------------------------------------------------------------------


#if SHADER == SH_FRAGMENT

// local linear index
int GetLocalIndex ()
{
	int3 coord = GetLocalCoord();
	int3 size  = GetLocalSize();
	return coord.x + (coord.y * size.x) + (coord.z * size.x * size.y);
}


// global coordinate in 3D
int3  GetGlobalSize ()
{
	return int3( unScreenResolution.xy, 0 );
}

int3 GetGlobalCoord ()
{
	return int3( gl_FragCoord.xy, 0 );
}


// local coordinate in 3D
int3 GetLocalSize ()
{
	return int3(1);
}

int3  GetLocalCoord ()
{
	return int3(0);
}


// group coordinate in 3D
int3 GetGroupSize ()
{
	return GetGlobalSize();
}

int3 GetGroupCoord ()
{
	return GetGlobalCoord();
}

#endif	// SH_FRAGMENT
//-----------------------------------------------------------------------------


#if SHADER == SH_COMPUTE || SHADER == SH_FRAGMENT

// global linear index
int GetGlobalIndex ()
{
	int3 coord = GetGlobalCoord();
	int3 size  = GetGlobalSize();
	return coord.x + (coord.y * size.x) + (coord.z * size.x * size.y);
}

int GetGlobalIndexSize ()
{
	int3 size  = GetGlobalSize();
	return size.x * size.y * size.z;
}

real GetGlobalIndexUNorm ()
{
	return real(GetGlobalIndex()) / real(GetGlobalIndexSize()-1);
}

real GetGlobalIndexSNorm ()
{
	return GetGlobalIndexUNorm() * 2.0 - 1.0;
}


// local linear index
int GetLocalIndexSize ()
{
	int3 size  = GetLocalSize();
	return size.x * size.y * size.z;
}

real GetLocalIndexUNorm ()
{
	return real(GetLocalIndex()) / real(GetLocalIndexSize()-1);
}

real GetLocalIndexSNorm ()
{
	return GetLocalIndexUNorm() * 2.0 - 1.0;
}


// group linear index
int GetGroupIndex ()
{
	int3 coord = GetGroupCoord();
	int3 size  = GetGroupSize();
	return coord.x + (coord.y * size.x) + (coord.z * size.x * size.y);
}

int GetGroupIndexSize ()
{
	int3 size  = GetGroupSize();
	return size.x * size.y * size.z;
}

real GetGroupIndexUNorm ()
{
	return real(GetGroupIndex()) / real(GetGroupIndexSize()-1);
}

real GetGroupIndexSNorm ()
{
	return GetGroupIndexUNorm() * 2.0 - 1.0;
}


// global coordinate in 3D
real3 GetGlobalCoordUNorm ()
{
	return real3(GetGlobalCoord()) / real3(GetGlobalSize()-1);
}

real3 GetGlobalCoordSNorm ()
{
	return GetGlobalCoordUNorm() * 2.0 - 1.0;
}


// local coordinate in 3D
real3 GetLocalCoordUNorm ()
{
	return real3(GetLocalCoord()) / real3(GetLocalSize()-1);
}

real3 GetLocalCoordSNorm ()
{
	return GetLocalCoordUNorm() * 2.0 - 1.0;
}


// group coordinate in 3D
real3 GetGroupCoordUNorm ()
{
	return real3(GetGroupCoord()) / real3(GetGroupSize()-1);
}

real3 GetGroupCoordSNorm ()
{
	return GetGroupCoordUNorm() * 2.0 - 1.0;
}


// global normalized coordinate in 2D with correction
real2 GetGlobalCoordUNormCorrected ()
{
	real2	size	= real2(GetGlobalSize().xy - 1);
	return real2(GetGlobalCoord()) / max( size.x, size.y );
}

real2 GetGlobalCoordSNormCorrected ()
{
	real2	hsize	= real2(GetGlobalSize().xy - 1) * 0.5;
	real	msize	= max( hsize.x, hsize.y );
	return (real2(GetGlobalCoord()) - hsize) / msize;
}

#endif	// SH_COMPUTE or SH_FRAGMENT
