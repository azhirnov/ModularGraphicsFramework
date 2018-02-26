// This is generated file
// Created at: 2018/02/25 - 23:59:15

#pragma once

namespace Pipelines
{
// Packing: Std140
struct BufferAlign_Struct final
{
	int4  i4;    // offset: 0, align: 16
	float2  f2;    // offset: 16, align: 8
	uint  b1;    // offset: 24, align: 4
	float  _padding0;    // offset: 28, align: 4
	uint4  u3;    // offset: 32, align: 16
	int  i1;    // offset: 48, align: 4
	float  _padding1;    // offset: 52, align: 4
	float  _padding2;    // offset: 56, align: 4
	float  _padding3;    // offset: 60, align: 4
	uint4  b3;    // offset: 64, align: 16
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

};
