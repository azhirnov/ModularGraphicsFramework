// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/Graphics/Public/Vertices.h"
#include "Engine/Graphics/Public/BatchRenderer.h"

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
		ND_ GraphicsMsg::AddBatch  _ToBatch () const
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
	// types
		using Base_t = BatchMemory< VertexType, 4, ushort, 4 >;

	// variables
		RectF		position;
		RectF		texcoord;
		color4u		color;

	// methods
		Rectangle () : Base_t(EPrimitive::TriangleStrip)
		{}

		Rectangle (const RectF &pos, color4u color) : Rectangle{ pos, RectF{}, color }
		{}

		Rectangle (const RectF &pos, const RectF &texc, color4u color = color4u::Default::White()) :
			Base_t( EPrimitive::TriangleStrip ),
			position(pos), texcoord(texc), color(color)
		{}

		ND_ operator GraphicsMsg::AddBatch ()
		{
			this->_vertices.Resize( 4 );
			this->_vertices[0] = typename Base_t::Vertex_t( float2(position.left,  position.top),		float2(texcoord.left,  texcoord.bottom),	color );
			this->_vertices[1] = typename Base_t::Vertex_t( float2(position.left,  position.bottom),	float2(texcoord.left,  texcoord.top),		color );
			this->_vertices[2] = typename Base_t::Vertex_t( float2(position.right, position.top),		float2(texcoord.right, texcoord.bottom),	color );
			this->_vertices[3] = typename Base_t::Vertex_t( float2(position.right, position.bottom),	float2(texcoord.right, texcoord.top),		color );

			this->_indices.Resize( 4 );
			this->_indices[0] = 0;
			this->_indices[1] = 1;
			this->_indices[2] = 2;
			this->_indices[3] = 3;

			return this->_ToBatch();
		}
	};

	using Rectangle2D	= Rectangle< DefVertices::Vertex2D >;


}	// DefPrimitives
}	// Graphics
}	// Engine
