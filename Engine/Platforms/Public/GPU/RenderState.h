// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/Platforms/Public/GPU/RenderStateEnums.h"
#include "Engine/Platforms/Public/GPU/MultiSamples.h"
#include "Engine/STL/Containers/StaticBitArray.h"

namespace Engine
{
namespace Platforms
{

	//
	// Render State
	//
	
	struct RenderState final : public CompileTime::PODStruct		// TODO: copy from StaticArray<...>
	{
	// types
	public:
		using Self	= RenderState;


		//
		// Color Buffer
		//
		struct ColorBuffer final : public CompileTime::PODStruct
		{
		// types
			using Self	= ColorBuffer;

			template <typename T>
			struct _Pair final
			{
				using type = typename T::type;

				type	color;
				type	alpha;

				_Pair (GX_DEFCTOR) : color(T::Unknown), alpha(T::Unknown) {}
				_Pair (type rgba) : color(rgba), alpha(rgba) {}
				_Pair (type rgb, type a) : color(rgb), alpha(a) {}

				bool operator == (const _Pair<T> &right) const;
			};


		// variables
			_Pair< EBlendFunc >	blendFuncSrc,
								blendFuncDst;
			_Pair< EBlendEq >	blendMode;
			bool4				colorMask;
			bool				blend;
		
		// methods
			ColorBuffer (GX_DEFCTOR) :
				blendFuncSrc( EBlendFunc::One, EBlendFunc::One ),
				blendFuncDst( EBlendFunc::Zero,	EBlendFunc::Zero ),
				blendMode( EBlendEq::Add, EBlendEq::Add ),
				colorMask( true ),	blend( false )
			{}

			bool operator == (const Self &right) const;

			DEBUG_ONLY( String ToString () const; )
		};

		//
		// Color Buffers State
		//
		struct ColorBuffersState final : public CompileTime::PODStruct
		{
		// types
			using ColorBuffers_t	= StaticArray< ColorBuffer, GlobalConst::GAPI_MaxColorBuffers >;
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
			
			bool operator == (const Self &right) const;

			DEBUG_ONLY( String ToString () const; )
		};
		
		//
		// Stencil Face State
		//
		struct StencilFaceState final : public CompileTime::PODStruct
		{
		// types
			using Self	= StencilFaceState;

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
				funcRef( 0 ),					funcMask( UMax ),
				mask( UMax )
			{}
			
			bool operator == (const Self &right) const;

			DEBUG_ONLY( String ToString () const; )
		};

		//
		// Stencil Buffer State
		//
		struct StencilBufferState final : public CompileTime::PODStruct
		{
		// types
			using Self	= StencilBufferState;

		// variables
			StencilFaceState	front;
			StencilFaceState	back;
			bool				enabled;

		// methods
			StencilBufferState (GX_DEFCTOR) :
				front(), back(), enabled( false )
			{}
			
			bool operator == (const Self &right) const;

			DEBUG_ONLY( String ToString () const; )
		};

		//
		// Depth Buffer State
		//
		struct DepthBufferState final : public CompileTime::PODStruct
		{
		// types
			using Self	= DepthBufferState;

		// variables
			ECompareFunc::type	func;
			Optional<float2>	range;
			bool				write;
			bool				enabled;

		// methods
			DepthBufferState (GX_DEFCTOR) :
				func( ECompareFunc::LEqual ),	range(),
				write( true ),					enabled( false )
			{}
				
			bool operator == (const Self &right) const;

			DEBUG_ONLY( String ToString () const; )
		};

		//
		// Input Assembly State
		//
		struct InputAssemblyState final : public CompileTime::PODStruct
		{
		// types
			using Self	= InputAssemblyState;

		// variables
			EPrimitive::type	topology;
			bool				primitiveRestart;	// if 'true' then index with -1 value will restarting the assembly of primitives

		// methods
			InputAssemblyState (GX_DEFCTOR) :
				topology( EPrimitive::Unknown ),
				primitiveRestart( false )
			{}
			
			bool operator == (const Self &right) const;

			DEBUG_ONLY( String ToString () const; )
		};

		//
		// Rasterization State
		//
		struct RasterizationState final : public CompileTime::PODStruct
		{
		// types
			using Self	= RasterizationState;

		// variables
			EPolygonMode::type	polygonMode;

			float				lineWidth;

			float				depthBiasConstFactor;
			float				depthBiasClamp;
			float				depthBiasSlopeFactor;
			bool				depthBias;

			bool				depthClamp;
			bool				rasterizerDiscard;

			EPolygonFace::type	cullMode;
			bool				frontFaceCCW;

		// methods
			RasterizationState (GX_DEFCTOR) :
				polygonMode( EPolygonMode::Fill ),	lineWidth( 1.0f ),
				depthBiasConstFactor( 0.0f ),		depthBiasClamp( 0.0f ),
				depthBiasSlopeFactor( 0.0f ),		depthBias( false ),
				depthClamp( false ),				rasterizerDiscard( false ),
				cullMode( EPolygonFace::None ),		frontFaceCCW( true )
			{}
			
			bool operator == (const Self &right) const;

			DEBUG_ONLY( String ToString () const; )
		};

		//
		// Multisample State
		//
		struct MultisampleState final : public CompileTime::PODStruct
		{
		// types
			using Self			= MultisampleState;
			using SampleMask	= StaticBitArray< 4*32 >;

		// variables
			SampleMask			sampleMask;
			MultiSamples		samples;

			UNormClamped<float>	minSampleShading;
			bool				sampleShading;

			bool				alphaToCoverage;
			bool				alphaToOne;

		// methods
			MultisampleState (GX_DEFCTOR) :
				sampleMask{ SampleMask{}.SetAll() },
				samples( 1 ),				minSampleShading(),		sampleShading( false ),
				alphaToCoverage( false ),	alphaToOne( false )
			{}
			
			bool operator == (const Self &right) const;

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
		
		bool operator == (const Self &right) const;

		DEBUG_ONLY( String ToString () const; )
	};
	

	
/*
=================================================
	operator ==
=================================================
*/
	template <typename T>
	inline bool RenderState::ColorBuffer::_Pair<T>::operator == (const _Pair<T> &right) const
	{
		return	color == right.color and
				alpha == right.alpha;
	}

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
			buffers[i].blendFuncSrc.color = value;
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
			buffers[i].blendFuncSrc.alpha = value;
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
			buffers[i].blendFuncSrc = value;
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
			buffers[i].blendFuncDst.color = value;
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
			buffers[i].blendFuncDst.alpha = value;
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
			buffers[i].blendFuncDst = value;
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
			buffers[i].blendMode.color = value;
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
			buffers[i].blendMode.alpha = value;
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
			buffers[i].blendMode = value;
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
