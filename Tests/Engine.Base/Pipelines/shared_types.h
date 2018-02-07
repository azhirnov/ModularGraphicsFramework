// This is generated file
// Created at: 2018/02/02 - 00:40:12

#pragma once

namespace Pipelines
{
// Packing: Std140
struct UB final
{
	float4  color;    // offset: 0, align: 16

	UB ()
	{
		STATIC_ASSERT( sizeof(UB) == 16 );
		STATIC_ASSERT( (offsetof( UB, color ) == 0) and (sizeof( color ) == 16) );
	}
};

// Packing: Std140
struct SSBO final
{
	float4  data;    // offset: 0, align: 16

	SSBO ()
	{
		STATIC_ASSERT( sizeof(SSBO) == 16 );
		STATIC_ASSERT( (offsetof( SSBO, data ) == 0) and (sizeof( data ) == 16) );
	}
};

};
