// This is generated file
// Created at: 2017/12/25 - 22:26:58

#pragma once

namespace Pipelines
{
// Packing: Std140
struct UB final
{
	float4  color;    // offset: 0, align: 16
	float4  _padding0;    // offset: 16, align: 16

	UB ()
	{
		STATIC_ASSERT( sizeof(UB) == 32 );
		STATIC_ASSERT( (offsetof( UB, color ) == 0) and (sizeof( color ) == 16) );
		STATIC_ASSERT( (offsetof( UB, _padding0 ) == 16) and (sizeof( _padding0 ) == 16) );
	}
};

// Packing: Std140
struct UB_0 final
{
	float4  color0;    // offset: 0, align: 16
	float4  _padding0;    // offset: 16, align: 16

	UB_0 ()
	{
		STATIC_ASSERT( sizeof(UB_0) == 32 );
		STATIC_ASSERT( (offsetof( UB_0, color0 ) == 0) and (sizeof( color0 ) == 16) );
		STATIC_ASSERT( (offsetof( UB_0, _padding0 ) == 16) and (sizeof( _padding0 ) == 16) );
	}
};

// Packing: Std140
struct UB_1 final
{
	float4  color0;    // offset: 0, align: 16
	float4  color1;    // offset: 16, align: 16
	float4  _padding0;    // offset: 32, align: 16

	UB_1 ()
	{
		STATIC_ASSERT( sizeof(UB_1) == 48 );
		STATIC_ASSERT( (offsetof( UB_1, color0 ) == 0) and (sizeof( color0 ) == 16) );
		STATIC_ASSERT( (offsetof( UB_1, color1 ) == 16) and (sizeof( color1 ) == 16) );
		STATIC_ASSERT( (offsetof( UB_1, _padding0 ) == 32) and (sizeof( _padding0 ) == 16) );
	}
};

};
