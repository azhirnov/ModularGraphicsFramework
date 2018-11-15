// This is generated file
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

// Packing: vertex
struct NativeVertex_default2 final
{
	float2  at_Position;    // offset: 0, align: 4
	float2  at_Texcoord;    // offset: 8, align: 4

	NativeVertex_default2 (const float2 &at_Position, const float2 &at_Texcoord) :
		at_Position{at_Position}, at_Texcoord{at_Texcoord}
	{}

	template <typename Vertex>
	ND_ static VertexInputState  GetAttribs ()
	{
		using Self = NativeVertex_default2;
		return VertexInputState()
			.Add( "at_Position", &Self::at_Position, CompileTime::IsFloat<decltype(Vertex::at_Position)> != CompileTime::IsFloat<decltype(Self::at_Position)> )
			.Add( "at_Texcoord", &Self::at_Texcoord, CompileTime::IsFloat<decltype(Vertex::at_Texcoord)> != CompileTime::IsFloat<decltype(Self::at_Texcoord)> )
			.Bind( "", SizeOf<Vertex> );
	}

	NativeVertex_default2 ()
	{
		STATIC_ASSERT( sizeof(NativeVertex_default2) == 16 );
		STATIC_ASSERT( (offsetof( NativeVertex_default2, at_Position ) == 0) and (sizeof( at_Position ) == 8) );
		STATIC_ASSERT( (offsetof( NativeVertex_default2, at_Texcoord ) == 8) and (sizeof( at_Texcoord ) == 8) );
	}
};

}	// Pipelines

namespace GX_STL::CompileTime::_ctime_hidden_ {
	template <> struct _IsPOD< Pipelines::UB > { static const bool value = true; };
	template <> struct _IsPOD< Pipelines::NativeVertex_default2 > { static const bool value = true; };
}

