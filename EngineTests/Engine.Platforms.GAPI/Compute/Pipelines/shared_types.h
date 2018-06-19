// This is generated file
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
		STATIC_ASSERT( (offsetof( DynamicBuffer_Struct, f2 ) == 16) and (sizeof( f2 ) == 8) );
		STATIC_ASSERT( (offsetof( DynamicBuffer_Struct, i3 ) == 32) and (sizeof( i3 ) == 16) );
		STATIC_ASSERT( (offsetof( DynamicBuffer_Struct, b2 ) == 48) and (sizeof( b2 ) == 8) );
	}
};

// Packing: Std140
struct BufferAlign_Struct final
{
	int4  i4;    // offset: 0, align: 16
	float2  f2;    // offset: 16, align: 8
	Bool32  b1;    // offset: 24, align: 4
	float  _padding0;    // offset: 28, align: 4
	uint3  u3;    // offset: 32, align: 16
	int  i1;    // offset: 44, align: 4
	Bool32_3  b3;    // offset: 48, align: 16
	float  f1;    // offset: 60, align: 4

	BufferAlign_Struct ()
	{
		STATIC_ASSERT( sizeof(BufferAlign_Struct) == 64 );
		STATIC_ASSERT( (offsetof( BufferAlign_Struct, i4 ) == 0) and (sizeof( i4 ) == 16) );
		STATIC_ASSERT( (offsetof( BufferAlign_Struct, f2 ) == 16) and (sizeof( f2 ) == 8) );
		STATIC_ASSERT( (offsetof( BufferAlign_Struct, b1 ) == 24) and (sizeof( b1 ) == 4) );
		STATIC_ASSERT( (offsetof( BufferAlign_Struct, u3 ) == 32) and (sizeof( u3 ) == 12) );
		STATIC_ASSERT( (offsetof( BufferAlign_Struct, i1 ) == 44) and (sizeof( i1 ) == 4) );
		STATIC_ASSERT( (offsetof( BufferAlign_Struct, b3 ) == 48) and (sizeof( b3 ) == 12) );
		STATIC_ASSERT( (offsetof( BufferAlign_Struct, f1 ) == 60) and (sizeof( f1 ) == 4) );
	}
};

// Packing: Std140
struct BufferAlign_SSBO final
{
	BufferAlign_Struct  src;    // offset: 0, align: 16
	BufferAlign_Struct  dst1;    // offset: 64, align: 16
	BufferAlign_Struct  dst2;    // offset: 128, align: 16

	BufferAlign_SSBO ()
	{
		STATIC_ASSERT( sizeof(BufferAlign_SSBO) == 192 );
		STATIC_ASSERT( (offsetof( BufferAlign_SSBO, src ) == 0) and (sizeof( src ) == 64) );
		STATIC_ASSERT( (offsetof( BufferAlign_SSBO, dst1 ) == 64) and (sizeof( dst1 ) == 64) );
		STATIC_ASSERT( (offsetof( BufferAlign_SSBO, dst2 ) == 128) and (sizeof( dst2 ) == 64) );
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
		STATIC_ASSERT( (offsetof( DynamicBuffer_SSBO, i4 ) == 16) and (sizeof( i4 ) == 16) );
	}
};

}	// Pipelines

namespace GX_STL::CompileTime::_ctime_hidden_ {
	template <> struct _IsPOD< Pipelines::DynamicBuffer_Struct > { static const bool value = true; };
	template <> struct _IsPOD< Pipelines::BufferAlign_Struct > { static const bool value = true; };
	template <> struct _IsPOD< Pipelines::BufferAlign_SSBO > { static const bool value = true; };
	template <> struct _IsPOD< Pipelines::DynamicBuffer_SSBO > { static const bool value = true; };
	template <> struct _IsPOD< Pipelines::DynamicBuffer_SSBO::Element > { static const bool value = true; };
}

