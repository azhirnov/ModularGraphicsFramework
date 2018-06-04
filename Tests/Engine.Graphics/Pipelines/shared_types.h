// This is generated file
#pragma once

namespace Pipelines
{
// Packing: vertex
struct NativeVertex_default3 final : CompileTime::PODStruct
{
	float2  at_Position;    // offset: 0, align: 4
	float2  at_Texcoord;    // offset: 8, align: 4
	float4  at_Color;    // offset: 16, align: 4

	NativeVertex_default3 (const float2 &at_Position, const float2 &at_Texcoord, const float4 &at_Color) :
		at_Position{at_Position}, at_Texcoord{at_Texcoord}, at_Color{at_Color}
	{}

	NativeVertex_default3 ()
	{
		STATIC_ASSERT( sizeof(NativeVertex_default3) == 32 );
		STATIC_ASSERT( (offsetof( NativeVertex_default3, at_Position ) == 0) and (sizeof( at_Position ) == 8) );
		STATIC_ASSERT( (offsetof( NativeVertex_default3, at_Texcoord ) == 8) and (sizeof( at_Texcoord ) == 8) );
		STATIC_ASSERT( (offsetof( NativeVertex_default3, at_Color ) == 16) and (sizeof( at_Color ) == 16) );
	}
};

};
