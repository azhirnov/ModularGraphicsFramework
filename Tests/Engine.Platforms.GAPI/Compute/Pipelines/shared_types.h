// This is generated file
// Created at: 2018/04/10 - 21:16:05

#pragma once

namespace Pipelines
{
// Packing: Std140
struct DynamicBuffer_Struct final
{
	int2  i2;    // offset: 0, align: 8
	Bool32  b1;    // offset: 8, align: 4
	float  _padding0;    // offset: 12, align: 4
	float2  f2;    // offset: 16, align: 8
	float2  _padding1;    // offset: 24, align: 8
	int4  i3;    // offset: 32, align: 16
	Bool32_2  b2;    // offset: 48, align: 8
	float2  _padding2;    // offset: 56, align: 8

	DynamicBuffer_Struct ()
	{
		STATIC_ASSERT( sizeof(DynamicBuffer_Struct) == 64 );
		STATIC_ASSERT( (offsetof( DynamicBuffer_Struct, i2 ) == 0) and (sizeof( i2 ) == 8) );
		STATIC_ASSERT( (offsetof( DynamicBuffer_Struct, b1 ) == 8) and (sizeof( b1 ) == 4) );
		STATIC_ASSERT( (offsetof( DynamicBuffer_Struct, _padding0 ) == 12) and (sizeof( _padding0 ) == 4) );
		STATIC_ASSERT( (offsetof( DynamicBuffer_Struct, f2 ) == 16) and (sizeof( f2 ) == 8) );
		STATIC_ASSERT( (offsetof( DynamicBuffer_Struct, _padding1 ) == 24) and (sizeof( _padding1 ) == 8) );
		STATIC_ASSERT( (offsetof( DynamicBuffer_Struct, i3 ) == 32) and (sizeof( i3 ) == 16) );
		STATIC_ASSERT( (offsetof( DynamicBuffer_Struct, b2 ) == 48) and (sizeof( b2 ) == 8) );
		STATIC_ASSERT( (offsetof( DynamicBuffer_Struct, _padding2 ) == 56) and (sizeof( _padding2 ) == 8) );
	}
};

// Packing: Std430
struct FindMSB_SSBO final
{
	uint  results[16];    // offset: 0, align: 4

	FindMSB_SSBO ()
	{
		STATIC_ASSERT( sizeof(FindMSB_SSBO) == 64 );
		STATIC_ASSERT( (offsetof( FindMSB_SSBO, results ) == 0) and (sizeof( results ) == 64) );
	}
};

// Packing: Std140
struct BufferAlign_Struct final
{
	int4  i4;    // offset: 0, align: 16
	float2  f2;    // offset: 16, align: 8
	Bool32  b1;    // offset: 24, align: 4
	float  _padding0;    // offset: 28, align: 4
	uint4  u3;    // offset: 32, align: 16
	int  i1;    // offset: 48, align: 4
	float  _padding1;    // offset: 52, align: 4
	float  _padding2;    // offset: 56, align: 4
	float  _padding3;    // offset: 60, align: 4
	Bool32_4  b3;    // offset: 64, align: 16
	float  f1;    // offset: 80, align: 4
	float  _padding4;    // offset: 84, align: 4
	float  _padding5;    // offset: 88, align: 4
	float  _padding6;    // offset: 92, align: 4

	BufferAlign_Struct ()
	{
		STATIC_ASSERT( sizeof(BufferAlign_Struct) == 96 );
		STATIC_ASSERT( (offsetof( BufferAlign_Struct, i4 ) == 0) and (sizeof( i4 ) == 16) );
		STATIC_ASSERT( (offsetof( BufferAlign_Struct, f2 ) == 16) and (sizeof( f2 ) == 8) );
		STATIC_ASSERT( (offsetof( BufferAlign_Struct, b1 ) == 24) and (sizeof( b1 ) == 4) );
		STATIC_ASSERT( (offsetof( BufferAlign_Struct, _padding0 ) == 28) and (sizeof( _padding0 ) == 4) );
		STATIC_ASSERT( (offsetof( BufferAlign_Struct, u3 ) == 32) and (sizeof( u3 ) == 16) );
		STATIC_ASSERT( (offsetof( BufferAlign_Struct, i1 ) == 48) and (sizeof( i1 ) == 4) );
		STATIC_ASSERT( (offsetof( BufferAlign_Struct, _padding1 ) == 52) and (sizeof( _padding1 ) == 4) );
		STATIC_ASSERT( (offsetof( BufferAlign_Struct, _padding2 ) == 56) and (sizeof( _padding2 ) == 4) );
		STATIC_ASSERT( (offsetof( BufferAlign_Struct, _padding3 ) == 60) and (sizeof( _padding3 ) == 4) );
		STATIC_ASSERT( (offsetof( BufferAlign_Struct, b3 ) == 64) and (sizeof( b3 ) == 16) );
		STATIC_ASSERT( (offsetof( BufferAlign_Struct, f1 ) == 80) and (sizeof( f1 ) == 4) );
		STATIC_ASSERT( (offsetof( BufferAlign_Struct, _padding4 ) == 84) and (sizeof( _padding4 ) == 4) );
		STATIC_ASSERT( (offsetof( BufferAlign_Struct, _padding5 ) == 88) and (sizeof( _padding5 ) == 4) );
		STATIC_ASSERT( (offsetof( BufferAlign_Struct, _padding6 ) == 92) and (sizeof( _padding6 ) == 4) );
	}
};

// Packing: Std430
struct AtomicAdd_Struct final
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
struct FindLSB_SSBO final
{
	uint  results[16];    // offset: 0, align: 4

	FindLSB_SSBO ()
	{
		STATIC_ASSERT( sizeof(FindLSB_SSBO) == 64 );
		STATIC_ASSERT( (offsetof( FindLSB_SSBO, results ) == 0) and (sizeof( results ) == 64) );
	}
};

// Packing: Std140
struct BufferAlign_SSBO final
{
	BufferAlign_Struct  src;    // offset: 0, align: 16
	BufferAlign_Struct  dst1;    // offset: 96, align: 16
	BufferAlign_Struct  dst2;    // offset: 192, align: 16

	BufferAlign_SSBO ()
	{
		STATIC_ASSERT( sizeof(BufferAlign_SSBO) == 288 );
		STATIC_ASSERT( (offsetof( BufferAlign_SSBO, src ) == 0) and (sizeof( src ) == 96) );
		STATIC_ASSERT( (offsetof( BufferAlign_SSBO, dst1 ) == 96) and (sizeof( dst1 ) == 96) );
		STATIC_ASSERT( (offsetof( BufferAlign_SSBO, dst2 ) == 192) and (sizeof( dst2 ) == 96) );
	}
};

// Packing: Std430
struct AtomicAdd_SSBO final
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

// Packing: Std140
struct DynamicBuffer_SSBO final
{
	float2  f2;    // offset: 0, align: 8
	float2  _padding0;    // offset: 8, align: 8
	int4  i4;    // offset: 16, align: 16

	// Element  arr[];   offset: 32, align: 16
	struct Element final
	{
		DynamicBuffer_Struct  arr;    // offset: 0, align: 16

		Element ()
		{
			STATIC_ASSERT( sizeof(Element) == 64 );
			STATIC_ASSERT( (offsetof( Element, arr ) == 0) and (sizeof( arr ) == 64) );
		}
	};

	DynamicBuffer_SSBO ()
	{
		STATIC_ASSERT( sizeof(DynamicBuffer_SSBO) == 32 );
		STATIC_ASSERT( (offsetof( DynamicBuffer_SSBO, f2 ) == 0) and (sizeof( f2 ) == 8) );
		STATIC_ASSERT( (offsetof( DynamicBuffer_SSBO, _padding0 ) == 8) and (sizeof( _padding0 ) == 8) );
		STATIC_ASSERT( (offsetof( DynamicBuffer_SSBO, i4 ) == 16) and (sizeof( i4 ) == 16) );
	}
};

};
