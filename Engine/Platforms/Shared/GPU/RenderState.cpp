// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Engine/Platforms/Shared/GPU/RenderState.h"
#include "Engine/Platforms/Shared/GPU/Enums.ToString.h"

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
			<< "\t	blendFuncSrcColor: " << EBlendFunc::ToString( blendFuncSrc.color ) << '\n'
			<< "\t	blendFuncSrcAlpha: " << EBlendFunc::ToString( blendFuncSrc.alpha ) << '\n'
			<< "\t	blendFuncDstColor: " << EBlendFunc::ToString( blendFuncDst.color ) << '\n'
			<< "\t	blendFuncDstAlpha: " << EBlendFunc::ToString( blendFuncDst.alpha ) << '\n'
			<< "\t	blendModeColor:    " << EBlendEq::ToString( blendMode.color )      << '\n'
			<< "\t	blendModeAlpha:    " << EBlendEq::ToString( blendMode.alpha )      << '\n'
			<< "\t	colorMask:       (" << GXTypes::ToString( colorMask ) << ")\n}";
	}
)	// DEBUG_ONLY

/*
=================================================
	ColorBuffer::operator ==
=================================================
*/
	bool RenderState::ColorBuffer::operator == (const Self &right) const
	{
		return	blend	==	right.blend		and
				(not blend ? true : (
					blendFuncSrc	== right.blendFuncSrc	and
					blendFuncDst	== right.blendFuncDst	and
					blendMode		== right.blendMode		and
					All( colorMask	== right.colorMask )
				));
	}
//-----------------------------------------------------------------------------



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
	operator ==
=================================================
*/
	bool RenderState::ColorBuffersState::operator == (const Self &right) const
	{
		return	All( blendColor	== right.blendColor )	and
					 logicOp	== right.logicOp		and
					 buffers	== right.buffers;
	}
//-----------------------------------------------------------------------------



/*
=================================================
	ToString
=================================================
*/
DEBUG_ONLY(	
	String RenderState::DepthBufferState::ToString () const
	{
		return String( "DepthBufferState {\n" )
			<< "	enabled:  " << enabled << '\n'
			<< "	func:     " << ECompareFunc::ToString( func ) << '\n'
			<< "	range:    (" << GXTypes::ToString( range ) << ")\n"
			<< "	write:    " << write << "\n}";
	}
)	// DEBUG_ONLY

/*
=================================================
	operator ==
=================================================
*/
	bool RenderState::DepthBufferState::operator == (const Self &right) const
	{
		return	enabled		== right.enabled	and
				(not enabled ? true : (
					write	== right.write	and
					func	== right.func	and
					range	== right.range
				));
	}
//-----------------------------------------------------------------------------



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
	operator ==
=================================================
*/
	bool RenderState::StencilFaceState::operator == (const Self &right) const
	{
		return	sfail		== right.sfail		and
				dfail		== right.dfail		and
				dppass		== right.dppass		and
				func		== right.func		and
				funcRef		== right.funcRef	and
				funcMask	== right.funcMask	and
				mask		== right.mask;
	}
//-----------------------------------------------------------------------------



/*
=================================================
	ToString
=================================================
*/
DEBUG_ONLY(	
	String RenderState::StencilBufferState::ToString () const
	{
		return String( "StencilBufferState {\n" )
			<< "	enabled:  " << enabled << '\n'
			<< "	front:    " << front.ToString() << '\n'
			<< "	back:     " << back.ToString() << "\n}";

	}
)	// DEBUG_ONLY

/*
=================================================
	operator ==
=================================================
*/
	bool RenderState::StencilBufferState::operator == (const Self &right) const
	{
		return	enabled		== right.enabled	and
				(not enabled ? true : (
					front	== right.front		and
					back	== right.back
				));
	}
//-----------------------------------------------------------------------------



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
	operator ==
=================================================
*/
	bool RenderState::InputAssemblyState::operator == (const Self &right) const
	{
		return	topology			== right.topology	and
				primitiveRestart	== right.primitiveRestart;
	}
//-----------------------------------------------------------------------------



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
	operator ==
=================================================
*/
	bool RenderState::RasterizationState::operator == (const Self &right) const
	{
		return	polygonMode		== right.polygonMode			and
				(polygonMode	!= EPolygonMode::Line ?	true :
					lineWidth	== right.lineWidth)				and
				depthBiasConstFactor	== right.depthBiasConstFactor	and		// TODO
				depthBiasClamp			== right.depthBiasClamp			and
				depthBiasSlopeFactor	== right.depthBiasSlopeFactor	and
				depthBias				== right.depthBias				and
				depthClamp				== right.depthClamp				and
				rasterizerDiscard		== right.rasterizerDiscard		and
				frontFaceCCW			== right.frontFaceCCW			and
				cullMode				== right.cullMode;
	}
//-----------------------------------------------------------------------------



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
	operator ==
=================================================
*/
	bool RenderState::MultisampleState::operator == (const Self &right) const
	{
		return	samples				== right.samples			and
				minSampleShading	== right.minSampleShading	and
				sampleShading		== right.sampleShading		and
				alphaToCoverage		== right.alphaToCoverage	and
				alphaToOne			== right.alphaToOne;
	}
//-----------------------------------------------------------------------------



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

/*
=================================================
	operator ==
=================================================
*/
	bool RenderState::operator == (const Self &right) const
	{
		return	color			== right.color			and
				depth			== right.depth			and
				stencil			== right.stencil		and
				inputAssembly	== right.inputAssembly	and
				rasterization	== right.rasterization	and
				multisample		== right.multisample;
	}
//-----------------------------------------------------------------------------

}	// Platforms
}	// Engine
