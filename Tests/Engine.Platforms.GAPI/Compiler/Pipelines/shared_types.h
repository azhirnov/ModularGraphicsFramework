// This is generated file
#pragma once

namespace Pipelines
{
// Packing: Std430
struct UnnamedBuffer_SSBO final : CompileTime::PODStruct
{
	uint  data[2];    // offset: 0, align: 4
	uint  result;    // offset: 8, align: 4

	UnnamedBuffer_SSBO ()
	{
		STATIC_ASSERT( sizeof(UnnamedBuffer_SSBO) == 12 );
		STATIC_ASSERT( (offsetof( UnnamedBuffer_SSBO, data ) == 0) and (sizeof( data ) == 8) );
		STATIC_ASSERT( (offsetof( UnnamedBuffer_SSBO, result ) == 8) and (sizeof( result ) == 4) );
	}
};

// Packing: Std430
struct FindLSB_SSBO final : CompileTime::PODStruct
{
	uint  results[16];    // offset: 0, align: 4

	FindLSB_SSBO ()
	{
		STATIC_ASSERT( sizeof(FindLSB_SSBO) == 64 );
		STATIC_ASSERT( (offsetof( FindLSB_SSBO, results ) == 0) and (sizeof( results ) == 64) );
	}
};

// Packing: Std430
struct GlobalToLocal_SSBO final : CompileTime::PODStruct
{
	uint  data[4];    // offset: 0, align: 4
	uint  results[2];    // offset: 16, align: 4

	GlobalToLocal_SSBO ()
	{
		STATIC_ASSERT( sizeof(GlobalToLocal_SSBO) == 24 );
		STATIC_ASSERT( (offsetof( GlobalToLocal_SSBO, data ) == 0) and (sizeof( data ) == 16) );
		STATIC_ASSERT( (offsetof( GlobalToLocal_SSBO, results ) == 16) and (sizeof( results ) == 8) );
	}
};

// Packing: Std430
struct VecSwizzle_SSBO final : CompileTime::PODStruct
{
	uint  results[8];    // offset: 0, align: 4

	VecSwizzle_SSBO ()
	{
		STATIC_ASSERT( sizeof(VecSwizzle_SSBO) == 32 );
		STATIC_ASSERT( (offsetof( VecSwizzle_SSBO, results ) == 0) and (sizeof( results ) == 32) );
	}
};

// Packing: Std430
struct AtomicAdd_Struct final : CompileTime::PODStruct
{
	int  value;    // offset: 0, align: 4
	Bool32  found;    // offset: 4, align: 4

	AtomicAdd_Struct ()
	{
		STATIC_ASSERT( sizeof(AtomicAdd_Struct) == 8 );
		STATIC_ASSERT( (offsetof( AtomicAdd_Struct, value ) == 0) and (sizeof( value ) == 4) );
		STATIC_ASSERT( (offsetof( AtomicAdd_Struct, found ) == 4) and (sizeof( found ) == 4) );
	}
};

// Packing: Std430
struct InlineAll_SSBO final : CompileTime::PODStruct
{
	uint  results[16];    // offset: 0, align: 4

	InlineAll_SSBO ()
	{
		STATIC_ASSERT( sizeof(InlineAll_SSBO) == 64 );
		STATIC_ASSERT( (offsetof( InlineAll_SSBO, results ) == 0) and (sizeof( results ) == 64) );
	}
};

// Packing: Std430
struct FindMSB_SSBO final : CompileTime::PODStruct
{
	uint  results[16];    // offset: 0, align: 4

	FindMSB_SSBO ()
	{
		STATIC_ASSERT( sizeof(FindMSB_SSBO) == 64 );
		STATIC_ASSERT( (offsetof( FindMSB_SSBO, results ) == 0) and (sizeof( results ) == 64) );
	}
};

// Packing: Std430
struct AtomicAdd_SSBO final : CompileTime::PODStruct
{
	AtomicAdd_Struct  st;    // offset: 0, align: 4
	uint  result;    // offset: 8, align: 4
	uint  resultList[10];    // offset: 12, align: 4

	AtomicAdd_SSBO ()
	{
		STATIC_ASSERT( sizeof(AtomicAdd_SSBO) == 52 );
		STATIC_ASSERT( (offsetof( AtomicAdd_SSBO, st ) == 0) and (sizeof( st ) == 8) );
		STATIC_ASSERT( (offsetof( AtomicAdd_SSBO, result ) == 8) and (sizeof( result ) == 4) );
		STATIC_ASSERT( (offsetof( AtomicAdd_SSBO, resultList ) == 12) and (sizeof( resultList ) == 40) );
	}
};

};
