// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Engine/Platforms/Shared/GPU/RenderState.h"

namespace Engine
{
namespace Platforms
{
/*
=================================================
	ToString
=================================================
*/
DEBUG_ONLY(	
	String RenderState::ColorBuffer::ToString () const
	{
		return String( "\tColorBuffer {\n" )
			<< "\t	blend:             " << blend << '\n'
			<< "\t	blendFuncSrcColor: " << EBlendFunc::ToString( blendFuncSrcColor ) << '\n'
			<< "\t	blendFuncSrcAlpha: " << EBlendFunc::ToString( blendFuncSrcAlpha ) << '\n'
			<< "\t	blendFuncDstColor: " << EBlendFunc::ToString( blendFuncDstColor ) << '\n'
			<< "\t	blendFuncDstAlpha: " << EBlendFunc::ToString( blendFuncDstAlpha ) << '\n'
			<< "\t	blendModeColor:    " << EBlendEq::ToString( blendModeColor )      << '\n'
			<< "\t	blendModeAlpha:    " << EBlendEq::ToString( blendModeAlpha )      << '\n'
			<< "\t	colorMask:       (" << GXTypes::ToString( colorMask ) << ")\n}";
	}
)	// DEBUG_ONLY

/*
=================================================
	ToString
=================================================
*/
DEBUG_ONLY(	
	String RenderState::ColorBuffersState::ToString () const
	{
		String	str( "ColorBuffersState {\n" );

		FOR( i, buffers ) {
			str << "	[" << i << ']' << buffers[i].ToString() << '\n';
		}

		str << "	logicOp:     " << ELogicOp::ToString( logicOp ) << '\n'
			<< "	blendColor: (" << GXTypes::ToString( blendColor ) << ")\n}\n";

		return str;
	}
)	// DEBUG_ONLY

/*
=================================================
	ToString
=================================================
*/
DEBUG_ONLY(	
	String RenderState::DepthBufferState::ToString () const
	{
		return String( "DepthBufferState {\n" )
			<< "	test:  " << test << '\n'
			<< "	func:  " << ECompareFunc::ToString( func ) << '\n'
			<< "	range: (" << GXTypes::ToString( range ) << ")\n"
			<< "	write: " << write << "\n}";
	}
)	// DEBUG_ONLY

/*
=================================================
	ToString
=================================================
*/
DEBUG_ONLY(	
	String RenderState::StencilFaceState::ToString () const
	{
		return String( "StencilFaceState {\n" )
			<< "\t	sfail:    " << EStencilOp::ToString( sfail ) << '\n'
			<< "\t	dfail:    " << EStencilOp::ToString( dfail ) << '\n'
			<< "\t	dppass:   " << EStencilOp::ToString( dppass ) << '\n'
			<< "\t	func:     " << ECompareFunc::ToString( func ) << '\n'
			<< "\t	funcRef:  " << String().FormatI( funcRef, 16 ) << '\n'
			<< "\t	funcMask: " << String().FormatI( funcMask, 16 ) << '\n'
			<< "\t	mask:     " << String().FormatI( mask, 16 ) << "\n\t}";
	}
)	// DEBUG_ONLY

/*
=================================================
	ToString
=================================================
*/
DEBUG_ONLY(	
	String RenderState::StencilBufferState::ToString () const
	{
		return String( "StencilBufferState {\n" )
			<< "	test:  " << test << '\n'
			<< "	front: " << front.ToString() << '\n'
			<< "	back:  " << back.ToString() << "\n}";

	}
)	// DEBUG_ONLY

/*
=================================================
	ToString
=================================================
*/
DEBUG_ONLY(	
	String RenderState::InputAssemblyState::ToString () const
	{
		return String( "InputAssemblyState {\n" )
			<< "	topology:         " << EPrimitive::ToString( topology ) << '\n'
			<< "	primitiveRestart: " << primitiveRestart << "\n}";
	}
)	// DEBUG_ONLY

/*
=================================================
	ToString
=================================================
*/
DEBUG_ONLY(	
	String RenderState::RasterizationState::ToString () const
	{
		return String( "RasterizationState {\n" )
			<< "	polygonMode:          " << EPolygonMode::ToString( polygonMode ) << '\n'
			<< "	lineWidth:            " << lineWidth << '\n'
			<< "	depthBiasConstFactor: " << depthBiasConstFactor << '\n'
			<< "	depthBiasClamp:       " << depthBiasClamp << '\n'
			<< "	depthBiasSlopeFactor: " << depthBiasSlopeFactor << '\n'
			<< "	depthBias:            " << depthBias << '\n'
			<< "	depthClamp:           " << depthClamp << '\n'
			<< "	rasterizerDiscard:    " << rasterizerDiscard << '\n'
			<< "	frontFaceCCW:         " << frontFaceCCW << '\n'
			<< "	cullMode:             " << EPolygonFace::ToString( cullMode ) << "\n}";
	}
)	// DEBUG_ONLY

/*
=================================================
	ToString
=================================================
*/
DEBUG_ONLY(	
	String RenderState::MultisampleState::ToString () const
	{
		return String( "MultisampleState {\n" )
			<< "	samples:          " << samples.Get() << '\n'
			<< "	minSampleShading: " << minSampleShading << '\n'
			<< "	sampleShading:    " << sampleShading << '\n'
			<< "	alphaToCoverage:  " << alphaToCoverage << '\n'
			<< "	alphaToOne:       " << alphaToOne << "\n}";
	}
)	// DEBUG_ONLY

/*
=================================================
	ToString
=================================================
*/
DEBUG_ONLY(	
	String RenderState::ToString () const
	{
		return String() 
					<< color.ToString() << '\n'
					<< depth.ToString() << '\n'
					<< stencil.ToString() << '\n'
					<< inputAssembly.ToString() << '\n'
					<< rasterization.ToString() << '\n'
					<< multisample.ToString();
	}
)	// DEBUG_ONLY

}	// Platforms
}	// Engine
