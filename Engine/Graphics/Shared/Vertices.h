// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/Graphics/Shared/IDs.h"

namespace Engine
{
namespace Graphics
{
namespace DefVertices
{

	//
	// Vertex 2D
	//
	struct Vertex2D : CompileTime::PODStruct
	{
	// types
		using Self	= Vertex2D;

	// variables
		float2		position;
		float2		texcoord;
		color4u		color;
		
	// methods
		Vertex2D () {}
		
		Vertex2D (const float2 &pos, const float2 &texc, const color4u &color) :
			position(pos), texcoord(texc), color(color)
		{}

		static VertexInputState  GetAttribs ()
		{
			return VertexInputState()
				.Add( "position",	&Self::position )
				.Add( "texcoord",	&Self::texcoord )
				.Add( "color",		&Self::color,	true )
				.Bind( "", SizeOf<Self>() );
		}

		static TypeId GetTypeId ()
		{
			return TypeIdOf< Self >();
		}
	};



	//
	// Vertex 3D
	//
	struct Vertex3D : CompileTime::PODStruct
	{
	// types
		using Self	= Vertex3D;

	// variables
		float3		position;
		float2		texcoord;
		color4u		color;
		
	// methods
		Vertex3D () {}
		
		Vertex3D (const float3 &pos, const float2 &texc, const color4u &color) :
			position(pos), texcoord(texc), color(color)
		{}

		static VertexInputState  GetAttribs ()
		{
			return VertexInputState()
				.Add( "position",	&Self::position )
				.Add( "texcoord",	&Self::texcoord )
				.Add( "color",		&Self::color )
				.Bind( "", SizeOf<Self>() );
		}

		static TypeId GetTypeId ()
		{
			return TypeIdOf< Self >();
		}
	};
	

}	// DefVertices
}	// Graphics
}	// Engine
