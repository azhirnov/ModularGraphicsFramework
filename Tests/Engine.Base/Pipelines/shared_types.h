// This is generated file
#pragma once

namespace Pipelines
{
// Packing: vertex
struct NativeVertex_default2 final : CompileTime::PODStruct
{
	float2  at_Position;    // offset: 0, align: 4
	float2  at_Texcoord;    // offset: 8, align: 4

	NativeVertex_default2 (const float2 &at_Position, const float2 &at_Texcoord) :
		at_Position{at_Position}, at_Texcoord{at_Texcoord}
	{}

	NativeVertex_default2 ()
	{
		STATIC_ASSERT( sizeof(NativeVertex_default2) == 16 );
		STATIC_ASSERT( (offsetof( NativeVertex_default2, at_Position ) == 0) and (sizeof( at_Position ) == 8) );
		STATIC_ASSERT( (offsetof( NativeVertex_default2, at_Texcoord ) == 8) and (sizeof( at_Texcoord ) == 8) );
	}
};

// Packing: Std140
struct UB final : CompileTime::PODStruct
{
	float4  color;    // offset: 0, align: 16

	UB ()
	{
		STATIC_ASSERT( sizeof(UB) == 16 );
		STATIC_ASSERT( (offsetof( UB, color ) == 0) and (sizeof( color ) == 16) );
	}
};

};
