// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/Platforms/Shared/GPU/RenderStateEnums.h"
#include "Engine/Platforms/Shared/GPU/MultiSamples.h"

namespace Engine
{
namespace Platforms
{

	//
	// Render State
	//
	
	struct RenderState : public CompileTime::PODStruct		// TODO: copy from StaticArray<...>
	{
	// types
	public:

		//
		// Color Buffer
		//
		struct ColorBuffer : public CompileTime::PODStruct
		{
		// types
			using Self	= ColorBuffer;

		// variables
			EBlendFunc::type	blendFuncSrcColor,
								blendFuncSrcAlpha,
								blendFuncDstColor,
								blendFuncDstAlpha;
			EBlendEq::type		blendModeColor,
								blendModeAlpha;
			bool4				colorMask;
			bool				blend;
		
		// methods
			ColorBuffer (GX_DEFCTOR) :
				blendFuncSrcColor( EBlendFunc::One ),	blendFuncSrcAlpha( EBlendFunc::One ),
				blendFuncDstColor( EBlendFunc::Zero ),	blendFuncDstAlpha( EBlendFunc::Zero ),
				blendModeColor( EBlendEq::Add ),		blendModeAlpha( EBlendEq::Add ),
				colorMask( true ),						blend( false )
			{}

			Self& SetBlendFuncSrc (EBlendFunc::type value);
			Self& SetBlendFuncDst (EBlendFunc::type value);

			Self& SetBlendMode (EBlendEq::type value);

			DEBUG_ONLY( String ToString () const; )
		};

		//
		// Color Buffers State
		//
		struct ColorBuffersState : public CompileTime::PODStruct
		{
		// types
			using ColorBuffers_t	= StaticArray< ColorBuffer, GlobalConst::Graphics_MaxColorBuffers >;
			using Self				= ColorBuffersState;

		// variables
			ColorBuffers_t		buffers;
			ELogicOp::type		logicOp;
			color4f				blendColor;

		// methods
			ColorBuffersState (GX_DEFCTOR) :
				logicOp(ELogicOp::None),	blendColor(1.0f)
			{}

			Self& SetBlend (bool enabled);

			Self& SetBlendFuncSrcColor (EBlendFunc::type value);
			Self& SetBlendFuncSrcAlpha (EBlendFunc::type value);
			Self& SetBlendFuncSrc (EBlendFunc::type value);

			Self& SetBlendFuncDstColor (EBlendFunc::type value);
			Self& SetBlendFuncDstAlpha (EBlendFunc::type value);
			Self& SetBlendFuncDst (EBlendFunc::type value);

			Self& SetBlendModeColor (EBlendEq::type value);
			Self& SetBlendModeAlpha (EBlendEq::type value);
			Self& SetBlendMode (EBlendEq::type value);

			Self& SetColorMask (bool4 value);

			DEBUG_ONLY( String ToString () const; )
		};
		
		//
		// Stencil Face State
		//
		struct StencilFaceState : public CompileTime::PODStruct
		{
		// variables
			EStencilOp::type	sfail,
								dfail,
								dppass;
			ECompareFunc::type	func;
			uint				funcRef,
								funcMask,
								mask;

		// methods
			StencilFaceState (GX_DEFCTOR) :
				sfail( EStencilOp::Keep ),		dfail( EStencilOp::Keep ),
				dppass( EStencilOp::Keep ),		func( ECompareFunc::Always ),
				funcRef( 0 ),					funcMask( -1 ),
				mask( -1 )
			{}

			DEBUG_ONLY( String ToString () const; )
		};

		//
		// Stencil Buffer State
		//
		struct StencilBufferState : public CompileTime::PODStruct
		{
		// variables
			StencilFaceState	front;
			StencilFaceState	back;
			bool				test;

		// methods
			StencilBufferState (GX_DEFCTOR) :
				front(), back(), test( false )
			{}

			DEBUG_ONLY( String ToString () const; )
		};

		//
		// Depth Buffer State
		//
		struct DepthBufferState : public CompileTime::PODStruct
		{
		// variables
			ECompareFunc::type	func;
			Optional<float2>	range;
			bool				write;
			bool				test;

		// methods
			DepthBufferState (GX_DEFCTOR) :
				func( ECompareFunc::LEqual ),	range(),
				write( true ),					test( false )
			{}
				
			DEBUG_ONLY( String ToString () const; )
		};

		//
		// Input Assembly State
		//
		struct InputAssemblyState : public CompileTime::PODStruct
		{
		// variables
			EPrimitive::type	topology;
			bool				primitiveRestart;

		// methods
			InputAssemblyState (GX_DEFCTOR) :
				topology( EPrimitive::Unknown ),
				primitiveRestart( false )
			{}

			DEBUG_ONLY( String ToString () const; )
		};

		//
		// Rasterization State
		//
		struct RasterizationState : public CompileTime::PODStruct
		{
		// variables
			EPolygonMode::type	polygonMode;

			float				lineWidth;

			float				depthBiasConstFactor;
			float				depthBiasClamp;
			float				depthBiasSlopeFactor;
			bool				depthBias;

			bool				depthClamp;
			bool				rasterizerDiscard;

			bool				frontFaceCCW;
			EPolygonFace::type	cullMode;

		// methods
			RasterizationState (GX_DEFCTOR) :
				polygonMode( EPolygonMode::Fill ),	lineWidth( 1.0f ),
				depthBiasConstFactor( 0.0f ),		depthBiasClamp( 0.0f ),
				depthBiasSlopeFactor( 0.0f ),		depthBias( false ),
				depthClamp( false ),				rasterizerDiscard( false ),
				cullMode( EPolygonFace::None ),		frontFaceCCW( true )
			{}

			DEBUG_ONLY( String ToString () const; )
		};

		//
		// Multisample State
		//
		struct MultisampleState : public CompileTime::PODStruct
		{
		// variables
			MultiSamples		samples;
			UNormClamped<float>	minSampleShading;
			bool				sampleShading;

			bool				alphaToCoverage;
			bool				alphaToOne;

		// methods
			MultisampleState (GX_DEFCTOR) :
				samples( 1 ),				minSampleShading(),
				alphaToCoverage( false ),	alphaToOne( false )
			{}

			DEBUG_ONLY( String ToString () const; )
		};


	// variables
	public:
		ColorBuffersState	color;
		DepthBufferState	depth;
		StencilBufferState	stencil;
		InputAssemblyState	inputAssembly;
		RasterizationState	rasterization;
		MultisampleState	multisample;


	// methods
	public:
		RenderState (GX_DEFCTOR)
		{}

		DEBUG_ONLY( String ToString () const; )
	};

			
	
/*
=================================================
	SetBlendFuncSrc
=================================================
*/
	inline RenderState::ColorBuffer&  RenderState::ColorBuffer::SetBlendFuncSrc (EBlendFunc::type value)
	{
		blendFuncSrcColor = blendFuncSrcAlpha = value;
		return *this;
	}
	
/*
=================================================
	SetBlendFuncDst
=================================================
*/
	inline RenderState::ColorBuffer&  RenderState::ColorBuffer::SetBlendFuncDst (EBlendFunc::type value)
	{
		blendFuncDstColor = blendFuncDstAlpha = value;
		return *this;
	}
	
/*
=================================================
	SetBlendMode
=================================================
*/
	inline RenderState::ColorBuffer&  RenderState::ColorBuffer::SetBlendMode (EBlendEq::type value)
	{
		blendModeColor = blendModeAlpha = value;
		return *this;
	}

//-----------------------------------------------------------------------------
	

	
/*
=================================================
	SetBlendMode
=================================================
*/
	inline RenderState::ColorBuffersState&  RenderState::ColorBuffersState::SetBlend (bool enabled)
	{
		FOR( i, buffers ) {
			buffers[i].blend = enabled;
		}
		return *this;
	}
	
/*
=================================================
	SetBlendMode
=================================================
*/
	inline RenderState::ColorBuffersState&  RenderState::ColorBuffersState::SetBlendFuncSrcColor (EBlendFunc::type value)
	{
		FOR( i, buffers ) {
			buffers[i].blendFuncSrcColor = value;
		}
		return *this;
	}
	
/*
=================================================
	SetBlendMode
=================================================
*/
	inline RenderState::ColorBuffersState&  RenderState::ColorBuffersState::SetBlendFuncSrcAlpha (EBlendFunc::type value)
	{
		FOR( i, buffers ) {
			buffers[i].blendFuncSrcAlpha = value;
		}
		return *this;
	}
	
/*
=================================================
	SetBlendMode
=================================================
*/
	inline RenderState::ColorBuffersState&  RenderState::ColorBuffersState::SetBlendFuncSrc (EBlendFunc::type value)
	{
		FOR( i, buffers ) {
			buffers[i].SetBlendFuncSrc( value );
		}
		return *this;
	}
	
/*
=================================================
	SetBlendMode
=================================================
*/
	inline RenderState::ColorBuffersState&  RenderState::ColorBuffersState::SetBlendFuncDstColor (EBlendFunc::type value)
	{
		FOR( i, buffers ) {
			buffers[i].blendFuncDstColor = value;
		}
		return *this;
	}
	
/*
=================================================
	SetBlendMode
=================================================
*/
	inline RenderState::ColorBuffersState&  RenderState::ColorBuffersState::SetBlendFuncDstAlpha (EBlendFunc::type value)
	{
		FOR( i, buffers ) {
			buffers[i].blendFuncDstAlpha = value;
		}
		return *this;
	}
	
/*
=================================================
	SetBlendMode
=================================================
*/
	inline RenderState::ColorBuffersState&  RenderState::ColorBuffersState::SetBlendFuncDst (EBlendFunc::type value)
	{
		FOR( i, buffers ) {
			buffers[i].SetBlendFuncDst( value );
		}
		return *this;
	}
	
/*
=================================================
	SetBlendMode
=================================================
*/
	inline RenderState::ColorBuffersState&  RenderState::ColorBuffersState::SetBlendModeColor (EBlendEq::type value)
	{
		FOR( i, buffers ) {
			buffers[i].blendModeColor = value;
		}
		return *this;
	}
	
/*
=================================================
	SetBlendMode
=================================================
*/
	inline RenderState::ColorBuffersState&  RenderState::ColorBuffersState::SetBlendModeAlpha (EBlendEq::type value)
	{
		FOR( i, buffers ) {
			buffers[i].blendModeAlpha = value;
		}
		return *this;
	}
	
/*
=================================================
	SetBlendMode
=================================================
*/
	inline RenderState::ColorBuffersState&  RenderState::ColorBuffersState::SetBlendMode (EBlendEq::type value)
	{
		FOR( i, buffers ) {
			buffers[i].SetBlendMode( value );
		}
		return *this;
	}
	
/*
=================================================
	SetBlendMode
=================================================
*/
	inline RenderState::ColorBuffersState&  RenderState::ColorBuffersState::SetColorMask (bool4 value)
	{
		FOR( i, buffers ) {
			buffers[i].colorMask = value;
		}
		return *this;
	}

//-----------------------------------------------------------------------------


}	// Platforms
}	// Engine
