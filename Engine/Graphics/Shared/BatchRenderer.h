// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/Graphics/Shared/IDs.h"

namespace Engine
{
namespace CreateInfo
{
	//
	// Batch Renderer
	//
	struct BatchRenderer
	{
		ModulePtr		gpuThread;

		// default pipeline templates.
		ModulePtr		coloredShader;
		ModulePtr		texturedShader;
		ModulePtr		multitexturedShader;

		// memory will be dynamicaly allocated, but to minimize memory fragmentation
		// you may set custom value to 'blockSize'.
		BytesU			blockSize				= 32_Kb;

		// all vertices will be aligned to this value.
		// min value 4, max 256.
		BytesU			vertexAlign				= 16_b;
	};

}	// CreateInfo


namespace GraphicsMsg
{

	//
	// Begin Batch Renderer
	//
	struct BeginBatchRenderer
	{
		ModulePtr		renderPass;		// needed to create pipelines, if null then used default render pass
	};


	//
	// Flush Batch Renderer
	//
	struct FlushBatchRenderer
	{
	// variables
		ModulePtr			framebuffer;
		GXMath::RectU		viewport;
		ModulePtr			cmdBuilder;
		
	// methods
		FlushBatchRenderer (const ModulePtr &framebuffer, const GXMath::RectU &viewport, const ModulePtr &cmdBuilder) :
			framebuffer( framebuffer ), viewport( viewport ), cmdBuilder( cmdBuilder )
		{}
	};


	//
	// Set Default Material to Batch Renderer
	//
	struct BatchRendererSetMaterial
	{
	// types
		using LayerName_t	= StaticString<32>;

	// variables
		LayerName_t		layer;
		ModulePtr		image0;
		ModulePtr		sampler0;
		ModulePtr		image1;
		ModulePtr		sampler1;

	// methods
		BatchRendererSetMaterial ()
		{}
	};


	//
	// Set Custom Material to Batch Renderer
	//
	struct BatchRendererSetCustomMaterial
	{
	// types
		using LayerName_t	= BatchRendererSetMaterial::LayerName_t;

	// variables
		LayerName_t			layer;
		ModulePtr			pipeline;
		ModulePtr			resourceTable;

	// methods
		BatchRendererSetCustomMaterial (StringCRef layerName, const ModulePtr &pipeline, const ModulePtr &resourceTable) :
			layer( layerName ), pipeline( pipeline ), resourceTable( resourceTable )
		{}
	};


	//
	// Add Batch to Batch Renderer
	//
	struct AddBatch
	{
	// types
		using VertexInputState	= Platforms::VertexInputState;
		using EIndex			= Platforms::EIndex;
		using EPrimitive		= Platforms::EPrimitive;

	// variables
		BinArrayCRef			vertices;
		BinArrayCRef			indices;
		VertexInputState		attribs;	// TODO: optimize
		EIndex::type			indexType	= EIndex::Unknown;
		EPrimitive::type		primitive	= EPrimitive::TriangleList;
		// TODO: continue strip flag?

	// methods
		AddBatch (BinArrayCRef verts, const VertexInputState &attr, EPrimitive::type primitive = EPrimitive::TriangleList) :
			vertices( verts ), attribs( attr ), primitive( primitive )
		{}

		AddBatch (BinArrayCRef verts, BinArrayCRef indices, const VertexInputState &attr,
				  EIndex::type indexType, EPrimitive::type primitive = EPrimitive::TriangleList) :
			vertices( verts ), indices( indices ), attribs( attr ), indexType( indexType ), primitive( primitive )
		{}

		template <typename VertexType>
		explicit AddBatch (ArrayCRef<VertexType> vertices, EPrimitive::type primitive = EPrimitive::TriangleList) :
			vertices( BinArrayCRef::From(vertices) ),
			attribs( VertexType::GetAttribs() ),
			primitive( primitive )
		{}

		template <typename VertexType, typename IndexType>
		AddBatch (ArrayCRef<VertexType> vertices, ArrayCRef<IndexType> indices, EPrimitive::type primitive = EPrimitive::TriangleList) :
			vertices( BinArrayCRef::From(vertices) ),
			indices( BinArrayCRef::From(indices) ),
			attribs( VertexType::GetAttribs() ),
			indexType( EIndex::From<IndexType>() ),
			primitive( primitive )
		{}
	};

}	// GraphicsMsg
}	// Engine
