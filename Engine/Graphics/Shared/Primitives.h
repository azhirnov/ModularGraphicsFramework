// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/Graphics/Shared/Vertices.h"
#include "Engine/Graphics/Shared/BatchRenderer.h"

namespace Engine
{
namespace Graphics
{
namespace DefPrimitives
{

	//
	// Batch Memory
	//

	template <	typename VertType,	usize VertCapacity,
				typename IdxType,	usize IdxCapacity
			 >
	struct BatchMemory
	{
	// types
	private:
		using Vertices		= FixedSizeArray< VertType, VertCapacity >;
		using Indices		= FixedSizeArray< IdxType,  IdxCapacity  >;

	public:
		using Vertex_t		= VertType;
		using Index_t		= IdxType;


	// variables
	protected:
		Vertices			_vertices;
		Indices				_indices;
		EPrimitive::type	_primitive	= EPrimitive::TriangleList;


	// methods
	public:
		explicit BatchMemory (EPrimitive::type primitive = EPrimitive::TriangleList) :
			_primitive( primitive )
		{}

	protected:
		GraphicsMsg::AddBatch _ToBatch () const
		{
			if ( _indices.Empty() )
				return GraphicsMsg::AddBatch{ ArrayCRef<VertType>(_vertices), _primitive };
			else
				return GraphicsMsg::AddBatch{ ArrayCRef<VertType>(_vertices), ArrayCRef<IdxType>(_indices), _primitive };
		}
	};
	


	//
	// Rectangle Batch
	//

	template <typename VertexType>
	struct Rectangle : BatchMemory< VertexType, 4, ushort, 4 >
	{
	// variables
		RectF		position;
		RectF		texcoord;
		color4u		color;

	// methods
		Rectangle () : BatchMemory(EPrimitive::TriangleStrip)
		{}

		Rectangle (const RectF &pos, color4u color) : Rectangle{ pos, RectF{}, color }
		{}

		Rectangle (const RectF &pos, const RectF &texc, color4u color = color4u::Default::White()) :
			BatchMemory( EPrimitive::TriangleStrip ),
			position(pos), texcoord(texc), color(color)
		{}

		operator GraphicsMsg::AddBatch ()
		{
			_vertices.Resize( 4 );
			_vertices[0] = Vertex_t( float2(position.left,  position.top),		float2(texcoord.left,  texcoord.bottom),	color );
			_vertices[1] = Vertex_t( float2(position.left,  position.bottom),	float2(texcoord.left,  texcoord.top),		color );
			_vertices[2] = Vertex_t( float2(position.right, position.top),		float2(texcoord.right, texcoord.bottom),	color );
			_vertices[3] = Vertex_t( float2(position.right, position.bottom),	float2(texcoord.right, texcoord.top),		color );

			_indices.Resize( 4 );
			_indices[0] = 0;
			_indices[1] = 1;
			_indices[2] = 2;
			_indices[3] = 3;

			return _ToBatch();
		}
	};

	using Rectangle2D	= Rectangle< DefVertices::Vertex2D >;


}	// DefPrimitives
}	// Graphics
}	// Engine
