// This is generated file
// Created at: 2017/10/17 - 12:23:33

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

};
