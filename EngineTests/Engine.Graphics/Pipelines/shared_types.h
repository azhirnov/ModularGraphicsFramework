// This is generated file
#pragma once

namespace Pipelines
{
// Packing: vertex
struct NativeVertex_default3 final
{
	float4  at_Color;    // offset: 0, align: 4
	float2  at_Position;    // offset: 16, align: 4
	float2  at_Texcoord;    // offset: 24, align: 4

	NativeVertex_default3 (const float4 &at_Color, const float2 &at_Position, const float2 &at_Texcoord) :
		at_Color{at_Color}, at_Position{at_Position}, at_Texcoord{at_Texcoord}
	{}

	template <typename Vertex>
	ND_ static VertexInputState  GetAttribs ()
	{
		using Self = NativeVertex_renderdots;
		return VertexInputState()
			.Add( "at_Color", &NativeVertex_default3::at_Color, CompileTime::IsFloat<decltype(Self::at_Color)> )
			.Add( "at_Position", &NativeVertex_default3::at_Position, CompileTime::IsFloat<decltype(Self::at_Position)> )
			.Add( "at_Texcoord", &NativeVertex_default3::at_Texcoord, CompileTime::IsFloat<decltype(Self::at_Texcoord)> )
			.Bind( "", SizeOf<Vertex> );
	}

	NativeVertex_default3 ()
	{
		STATIC_ASSERT( sizeof(NativeVertex_default3) == 32 );
		STATIC_ASSERT( (offsetof( NativeVertex_default3, at_Color ) == 0) and (sizeof( at_Color ) == 16) );
		STATIC_ASSERT( (offsetof( NativeVertex_default3, at_Position ) == 16) and (sizeof( at_Position ) == 8) );
		STATIC_ASSERT( (offsetof( NativeVertex_default3, at_Texcoord ) == 24) and (sizeof( at_Texcoord ) == 8) );
	}
};

}	// Pipelines

namespace GX_STL::CompileTime::_ctime_hidden_ {
	template <> struct _IsPOD< Pipelines::NativeVertex_default3 > { static const bool value = true; };
}

