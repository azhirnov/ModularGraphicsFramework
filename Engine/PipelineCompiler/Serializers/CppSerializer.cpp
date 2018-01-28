// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Engine/PipelineCompiler/Serializers/CppSerializer.h"

namespace PipelineCompiler
{
	
/*
=================================================
	constructor
=================================================
*/
	CppSerializer::CppSerializer ()
	{
	}
	
/*
=================================================
	destructor
=================================================
*/
	CppSerializer::~CppSerializer ()
	{
		ASSERT( _structStack.Count() == 1 );	// 1 becouse of default value in bottom of the stack
		ASSERT( _fileCounter == 0 );
		ASSERT( _scopeCounter == 0 );
	}
	
/*
=================================================
	ToString (EBlendFunc)
=================================================
*/
	String  CppSerializer::ToString (EBlendFunc::type value)
	{
		switch ( value )
		{
			case EBlendFunc::Zero :					return "EBlendFunc::Zero";
			case EBlendFunc::One :					return "EBlendFunc::One";
			case EBlendFunc::SrcColor :				return "EBlendFunc::SrcColor";
			case EBlendFunc::OneMinusSrcColor :		return "EBlendFunc::OneMinusSrcColor";
			case EBlendFunc::DstColor :				return "EBlendFunc::DstColor";
			case EBlendFunc::OneMinusDstColor :		return "EBlendFunc::OneMinusDstColor";
			case EBlendFunc::SrcAlpha :				return "EBlendFunc::SrcAlpha";
			case EBlendFunc::OneMinusSrcAlpha :		return "EBlendFunc::OneMinusSrcAlpha";
			case EBlendFunc::DstAlpha :				return "EBlendFunc::DstAlpha";
			case EBlendFunc::OneMinusDstAlpha :		return "EBlendFunc::OneMinusDstAlpha";
			case EBlendFunc::ConstColor :			return "EBlendFunc::ConstColor";
			case EBlendFunc::OneMinusConstColor :	return "EBlendFunc::OneMinusConstColor";
			case EBlendFunc::ConstAlpha :			return "EBlendFunc::ConstAlpha";
			case EBlendFunc::OneMinusConstAlpha :	return "EBlendFunc::OneMinusConstAlpha";
			case EBlendFunc::SrcAlphaSaturate :		return "EBlendFunc::SrcAlphaSaturate";
		}
		RETURN_ERR( "unknown blend function type!" );
	}
	
/*
=================================================
	ToString (EBlendEq)
=================================================
*/
	String  CppSerializer::ToString (EBlendEq::type value)
	{
		switch ( value )
		{
			case EBlendEq::Add :		return "EBlendEq::Add";
			case EBlendEq::Sub :		return "EBlendEq::Sub";
			case EBlendEq::RevSub :		return "EBlendEq::RevSub";
			case EBlendEq::Min :		return "EBlendEq::Min";
			case EBlendEq::Max :		return "EBlendEq::Max";
		}
		RETURN_ERR( "unknown blend equation type!" );
	}
	
/*
=================================================
	ToString (ELogicOp)
=================================================
*/
	String  CppSerializer::ToString (ELogicOp::type value)
	{
		switch ( value )
		{
			case ELogicOp::None :			return "ELogicOp::None";
			case ELogicOp::Clear :			return "ELogicOp::Clear";
			case ELogicOp::Set :			return "ELogicOp::Set";
			case ELogicOp::Copy :			return "ELogicOp::Copy";
			case ELogicOp::CopyInverted :	return "ELogicOp::CopyInverted";
			case ELogicOp::Noop :			return "ELogicOp::Noop";
			case ELogicOp::Invert :			return "ELogicOp::Invert";
			case ELogicOp::And :			return "ELogicOp::And";
			case ELogicOp::NotAnd :			return "ELogicOp::NotAnd";
			case ELogicOp::Or :				return "ELogicOp::Or";
			case ELogicOp::NotOr :			return "ELogicOp::NotOr";
			case ELogicOp::Xor :			return "ELogicOp::Xor";
			case ELogicOp::Equiv :			return "ELogicOp::Equiv";
			case ELogicOp::AndReverse :		return "ELogicOp::AndReverse";
			case ELogicOp::AndInverted :	return "ELogicOp::AndInverted";
			case ELogicOp::OrReverse :		return "ELogicOp::OrReverse";
			case ELogicOp::OrInverted :		return "ELogicOp::OrInverted";
		}
		RETURN_ERR( "unknown logic operation type!" );
	}

/*
=================================================
	ToString (ECompareFunc)
=================================================
*/
	String  CppSerializer::ToString (ECompareFunc::type value)
	{
		switch ( value )
		{
			//case ECompareFunc::None
			case ECompareFunc::Never :		return "ECompareFunc::Never";
			case ECompareFunc::Less :		return "ECompareFunc::Less";
			case ECompareFunc::Equal :		return "ECompareFunc::Equal";
			case ECompareFunc::LEqual :		return "ECompareFunc::LEqual";
			case ECompareFunc::Greater :	return "ECompareFunc::Greater";
			case ECompareFunc::NotEqual :	return "ECompareFunc::NotEqual";
			case ECompareFunc::GEqual :		return "ECompareFunc::GEqual";
			case ECompareFunc::Always :		return "ECompareFunc::Always";
		}
		RETURN_ERR( "unknown comparison function type!" );
	}
	
/*
=================================================
	ToString (EStencilOp)
=================================================
*/
	String  CppSerializer::ToString (EStencilOp::type value)
	{
		switch ( value )
		{
			case EStencilOp::Keep :			return "EStencilOp::Keep";
			case EStencilOp::Zero :			return "EStencilOp::Zero";
			case EStencilOp::Replace :		return "EStencilOp::Replace";
			case EStencilOp::Incr :			return "EStencilOp::Incr";
			case EStencilOp::IncrWrap :		return "EStencilOp::IncrWrap";
			case EStencilOp::Decr :			return "EStencilOp::Decr";
			case EStencilOp::DecrWrap :		return "EStencilOp::DecrWrap";
			case EStencilOp::Invert :		return "EStencilOp::Invert";
		}
		RETURN_ERR( "unknown stencil operation type!" );
	}
	
/*
=================================================
	ToString (EPolygonMode)
=================================================
*/
	String  CppSerializer::ToString (EPolygonMode::type value)
	{
		switch ( value )
		{
			case EPolygonMode::Point :	return "EPolygonMode::Point";
			case EPolygonMode::Line :	return "EPolygonMode::Line";
			case EPolygonMode::Fill :	return "EPolygonMode::Fill";
		}
		RETURN_ERR( "unknown polygon mode!" );
	}
	
/*
=================================================
	ToString (EPolygonFace)
=================================================
*/
	String  CppSerializer::ToString (EPolygonFace::type value)
	{
		switch ( value )
		{
			case EPolygonFace::None :			return "EPolygonFace::None";
			case EPolygonFace::Front :			return "EPolygonFace::Front";
			case EPolygonFace::Back :			return "EPolygonFace::Back";
			case EPolygonFace::FontAndBack :	return "EPolygonFace::FontAndBack";
		}
		RETURN_ERR( "unknown polygon mode!" );
	}
	
/*
=================================================
	ToString (EPrimitive)
=================================================
*/
	String  CppSerializer::ToString (EPrimitive::type value)
	{
		switch ( value )
		{
			case EPrimitive::Point :			return "EPrimitive::Point";
			case EPrimitive::LineList :			return "EPrimitive::LineList";
			case EPrimitive::LineStrip :		return "EPrimitive::LineStrip";
			case EPrimitive::TriangleList :		return "EPrimitive::TriangleList";
			case EPrimitive::TriangleStrip :	return "EPrimitive::TriangleStrip";
			case EPrimitive::Patch :			return "EPrimitive::Patch";
		}
		RETURN_ERR( "unknown primitive type!" );
	}
	
/*
=================================================
	ToString (MultiSamples)
=================================================
*/
	String  CppSerializer::ToString (MultiSamples value)
	{
		return "MultiSamples(PowOf2Value<uint>( "_str << value.GetPowerOf2() << " ))";
	}
	
/*
=================================================
	ToString (EVertexAttribute)
=================================================
*/
	String  CppSerializer::ToString (EVertexAttribute::type value)
	{
		switch ( value )
		{
			case EVertexAttribute::Byte :			return "EVertexAttribute::Byte";
			case EVertexAttribute::Byte2 :			return "EVertexAttribute::Byte2";
			case EVertexAttribute::Byte3 :			return "EVertexAttribute::Byte3";
			case EVertexAttribute::Byte4 :			return "EVertexAttribute::Byte4";
			case EVertexAttribute::Byte_Norm :		return "EVertexAttribute::Byte_Norm";
			case EVertexAttribute::Byte2_Norm :		return "EVertexAttribute::Byte2_Norm";
			case EVertexAttribute::Byte3_Norm :		return "EVertexAttribute::Byte3_Norm";
			case EVertexAttribute::Byte4_Norm :		return "EVertexAttribute::Byte4_Norm";
			case EVertexAttribute::UByte :			return "EVertexAttribute::UByte";
			case EVertexAttribute::UByte2 :			return "EVertexAttribute::UByte2";
			case EVertexAttribute::UByte3 :			return "EVertexAttribute::UByte3";
			case EVertexAttribute::UByte4 :			return "EVertexAttribute::UByte4";
			case EVertexAttribute::UByte_Norm :		return "EVertexAttribute::UByte_Norm";
			case EVertexAttribute::UByte2_Norm :	return "EVertexAttribute::UByte2_Norm";
			case EVertexAttribute::UByte3_Norm :	return "EVertexAttribute::UByte3_Norm";
			case EVertexAttribute::UByte4_Norm :	return "EVertexAttribute::UByte4_Norm";
			case EVertexAttribute::Short :			return "EVertexAttribute::Short";
			case EVertexAttribute::Short2 :			return "EVertexAttribute::Short2";
			case EVertexAttribute::Short3 :			return "EVertexAttribute::Short3";
			case EVertexAttribute::Short4 :			return "EVertexAttribute::Short4";
			case EVertexAttribute::Short_Norm :		return "EVertexAttribute::Short_Norm";
			case EVertexAttribute::Short2_Norm :	return "EVertexAttribute::Short2_Norm";
			case EVertexAttribute::Short3_Norm :	return "EVertexAttribute::Short3_Norm";
			case EVertexAttribute::Short4_Norm :	return "EVertexAttribute::Short4_Norm";
			case EVertexAttribute::UShort :			return "EVertexAttribute::UShort";
			case EVertexAttribute::UShort2 :		return "EVertexAttribute::UShort2";
			case EVertexAttribute::UShort3 :		return "EVertexAttribute::UShort3";
			case EVertexAttribute::UShort4 :		return "EVertexAttribute::UShort4";
			case EVertexAttribute::UShort_Norm :	return "EVertexAttribute::UShort_Norm";
			case EVertexAttribute::UShort2_Norm :	return "EVertexAttribute::UShort2_Norm";
			case EVertexAttribute::UShort3_Norm :	return "EVertexAttribute::UShort3_Norm";
			case EVertexAttribute::UShort4_Norm :	return "EVertexAttribute::UShort4_Norm";
			case EVertexAttribute::Int :			return "EVertexAttribute::Int";
			case EVertexAttribute::Int2 :			return "EVertexAttribute::Int2";
			case EVertexAttribute::Int3 :			return "EVertexAttribute::Int3";
			case EVertexAttribute::Int4 :			return "EVertexAttribute::Int4";
			case EVertexAttribute::Int_Norm :		return "EVertexAttribute::Int_Norm";
			case EVertexAttribute::Int2_Norm :		return "EVertexAttribute::Int2_Norm";
			case EVertexAttribute::Int3_Norm :		return "EVertexAttribute::Int3_Norm";
			case EVertexAttribute::Int4_Norm :		return "EVertexAttribute::Int4_Norm";
			case EVertexAttribute::UInt :			return "EVertexAttribute::UInt";
			case EVertexAttribute::UInt2 :			return "EVertexAttribute::UInt2";
			case EVertexAttribute::UInt3 :			return "EVertexAttribute::UInt3";
			case EVertexAttribute::UInt4 :			return "EVertexAttribute::UInt4";
			case EVertexAttribute::UInt_Norm :		return "EVertexAttribute::UInt_Norm";
			case EVertexAttribute::UInt2_Norm :		return "EVertexAttribute::UInt2_Norm";
			case EVertexAttribute::UInt3_Norm :		return "EVertexAttribute::UInt3_Norm";
			case EVertexAttribute::UInt4_Norm :		return "EVertexAttribute::UInt4_Norm";
			case EVertexAttribute::Long :			return "EVertexAttribute::Long";
			case EVertexAttribute::Long2 :			return "EVertexAttribute::Long2";
			case EVertexAttribute::Long3 :			return "EVertexAttribute::Long3";
			case EVertexAttribute::Long4 :			return "EVertexAttribute::Long4";
			case EVertexAttribute::Long_Norm :		return "EVertexAttribute::Long_Norm";
			case EVertexAttribute::Long2_Norm :		return "EVertexAttribute::Long2_Norm";
			case EVertexAttribute::Long3_Norm :		return "EVertexAttribute::Long3_Norm";
			case EVertexAttribute::Long4_Norm :		return "EVertexAttribute::Long4_Norm";
			case EVertexAttribute::ULong :			return "EVertexAttribute::ULong";
			case EVertexAttribute::ULong2 :			return "EVertexAttribute::ULong2";
			case EVertexAttribute::ULong3 :			return "EVertexAttribute::ULong3";
			case EVertexAttribute::ULong4 :			return "EVertexAttribute::ULong4";
			case EVertexAttribute::ULong_Norm :		return "EVertexAttribute::ULong_Norm";
			case EVertexAttribute::ULong2_Norm :	return "EVertexAttribute::ULong2_Norm";
			case EVertexAttribute::ULong3_Norm :	return "EVertexAttribute::ULong3_Norm";
			case EVertexAttribute::ULong4_Norm :	return "EVertexAttribute::ULong4_Norm";
			case EVertexAttribute::Half :			return "EVertexAttribute::Half";
			case EVertexAttribute::Half2 :			return "EVertexAttribute::Half2";
			case EVertexAttribute::Half3 :			return "EVertexAttribute::Half3";
			case EVertexAttribute::Half4 :			return "EVertexAttribute::Half4";
			case EVertexAttribute::Float :			return "EVertexAttribute::Float";
			case EVertexAttribute::Float2 :			return "EVertexAttribute::Float2";
			case EVertexAttribute::Float3 :			return "EVertexAttribute::Float3";
			case EVertexAttribute::Float4 :			return "EVertexAttribute::Float4";
			case EVertexAttribute::Double :			return "EVertexAttribute::Double";
			case EVertexAttribute::Double2 :		return "EVertexAttribute::Double2";
			case EVertexAttribute::Double3 :		return "EVertexAttribute::Double3";
			case EVertexAttribute::Double4 :		return "EVertexAttribute::Double4";
		}
		RETURN_ERR( "unknown vertex attrib type!" );
	}
	
/*
=================================================
	ToString (EFragOutput)
=================================================
*/
	String  CppSerializer::ToString (EFragOutput::type value)
	{
		switch ( value )
		{
			case EFragOutput::Int4 :	return "EFragOutput::Int4";
			case EFragOutput::UInt4 :	return "EFragOutput::UInt4";
			case EFragOutput::Float4 :	return "EFragOutput::Float4";
		}
		RETURN_ERR( "unknown fragment output type!" );
	}

/*
=================================================
	ExtractIndent
=================================================
*/
	String  CppSerializer::ExtractIndent (StringCRef str)
	{
		String	res;
		FOR( i, str )
		{
			if ( str[i] == ' ' or str[i] == '\t' )
				res << str[i];
			else
				break;
		}
		return res;
	}

/*
=================================================
	ToString (RenderState)
=================================================
*/
	String  CppSerializer::ToString (StringCRef name, const RenderState &state) const
	{
		String	str;
		str << ExtractIndent( name ) << "// RenderState\n";
		str << name << " = RenderState();\n";
		str << ToString( String(name) << ".color",			state.color );
		str << ToString( String(name) << ".depth",			state.depth );
		str << ToString( String(name) << ".stencil",		state.stencil );
		str << ToString( String(name) << ".rasterization",	state.rasterization );
		str << ToString( String(name) << ".multisample",	state.multisample );
		str << ToString( String(name) << ".inputAssembly",	state.inputAssembly );
		return str;
	}
	
/*
=================================================
	ToString (ColorBuffer)
=================================================
*/
	String  CppSerializer::ToString (StringCRef name, const RenderState::ColorBuffer &state)
	{
		// skip if default
		if ( state == RenderState::ColorBuffer() )
			return "";

		String	str;
		str	<< ExtractIndent( name ) << "// ColorBuffer\n"
			<< name << ".blend     = " << state.blend << "\n"
			<< name << ".colorMask = bool4( " << state.colorMask.x << ", " << state.colorMask.y << ", "
											  << state.colorMask.z << ", " << state.colorMask.w << " );\n";

		if ( state.blend )
		{
			str << name << ".blendFuncSrc.color = " << ToString( state.blendFuncSrc.color ) << ";\n"
				<< name << ".blendFuncSrc.alpha = " << ToString( state.blendFuncSrc.alpha ) << ";\n"
				<< name << ".blendFuncDst.color = " << ToString( state.blendFuncDst.color ) << ";\n"
				<< name << ".blendFuncDst.alpha = " << ToString( state.blendFuncDst.alpha ) << ";\n"
				<< name << ".blendMode.color    = " << ToString( state.blendMode.color ) << ";\n"
				<< name << ".blendMode.alpha    = " << ToString( state.blendMode.alpha ) << ";\n";
		}
		str << "\n";
		return str;
	}
	
/*
=================================================
	ToString (ColorBuffersState)
=================================================
*/
	String  CppSerializer::ToString (StringCRef name, const RenderState::ColorBuffersState &state)
	{
		// skip if default
		if ( state == RenderState::ColorBuffersState() )
			return "";

		String	str;
		str << ExtractIndent( name ) << "// ColorBuffersState\n"
			<< name << ".logicOp    = " << ToString( state.logicOp ) << ";\n"
			<< name << ".blendColor = color4f( " << state.blendColor.x << ", " << state.blendColor.y << ", "
												<< state.blendColor.z << ", " << state.blendColor.w << " );\n\n";

		FOR( i, state.buffers )
		{
			str << ToString( String(name) << ".buffers[" << i << "]", state.buffers[i] );
		}
		return str;
	}
	
/*
=================================================
	ToString (DepthBufferState)
=================================================
*/
	String  CppSerializer::ToString (StringCRef name, const RenderState::DepthBufferState &state)
	{
		// skip if default
		if ( state == RenderState::DepthBufferState() )
			return "";

		String	str;
		str << ExtractIndent( name ) << "// DepthBufferState\n"
			<< name << ".enabled = " << state.enabled << ";\n";

		if ( state.enabled )
		{
			str << name << ".write   = " << state.write << ";\n"
				<< name << ".func    = " << ToString( state.func ) << ";\n";

			if ( state.range.IsDefined() ) {
				str << name << ".range   = float2( " << state.range->x << ", " << state.range->y << " );\n";
			}
		}
		str << "\n";
		return str;
	}
	
/*
=================================================
	ToString (StencilFaceState)
=================================================
*/
	String  CppSerializer::ToString (StringCRef name, const RenderState::StencilFaceState &state)
	{
		// skip if default
		if ( state == RenderState::StencilFaceState() )
			return "";

		String	str;
		str << name << ".sfail    = " << ToString( state.sfail ) << ";\n"
			<< name << ".dfail    = " << ToString( state.dfail ) << ";\n"
			<< name << ".dppass   = " << ToString( state.dppass ) << ";\n"
			<< name << ".func     = " << ToString( state.func ) << ";\n"
			<< name << ".funcRef  = 0x" << String().FormatI( state.funcRef, 16 ) << ";\n"
			<< name << ".funcMask = 0x" << String().FormatI( state.funcMask, 16 ) << ";\n"
			<< name << ".mask     = 0x" << String().FormatI( state.mask, 16 ) << ";\n";
		return str;
	}
	
/*
=================================================
	ToString (StencilBufferState)
=================================================
*/
	String  CppSerializer::ToString (StringCRef name, const RenderState::StencilBufferState &state)
	{
		// skip if default
		if ( state == RenderState::StencilBufferState() )
			return "";

		String	str;
		str << ExtractIndent( name ) << "// StencilBufferState\n"
			<< name << ".enabled = " << state.enabled << ";\n";

		if ( state.enabled )
		{
			str << ToString( String(name) << ".front", state.front )
				<< ToString( String(name) << ".back", state.back );
		}
		str << "\n";
		return str;
	}

/*
=================================================
	ToString (RasterizationState)
=================================================
*/
	String  CppSerializer::ToString (StringCRef name, const RenderState::RasterizationState &state)
	{
		// skip if default
		if ( state == RenderState::RasterizationState() )
			return "";

		String	str;
		str << ExtractIndent( name ) << "// RasterizationState\n"
			<< name << ".polygonMode          = " << ToString( state.polygonMode ) << ";\n"
			<< name << ".lineWidth            = " << state.lineWidth << ";\n"
			<< name << ".depthBiasConstFactor = " << state.depthBiasConstFactor << ";\n"
			<< name << ".depthBiasClamp       = " << state.depthBiasClamp << ";\n"
			<< name << ".depthBiasSlopeFactor = " << state.depthBiasSlopeFactor << ";\n"
			<< name << ".depthBias            = " << state.depthBias << ";\n"
			<< name << ".depthClamp           = " << state.depthClamp << ";\n"
			<< name << ".rasterizerDiscard    = " << state.rasterizerDiscard << ";\n"
			<< name << ".frontFaceCCW         = " << state.frontFaceCCW << ";\n"
			<< name << ".cullMode             = " << ToString( state.cullMode ) << ";\n\n";
		return str;
	}
	
/*
=================================================
	ToString (InputAssemblyState)
=================================================
*/
	String  CppSerializer::ToString (StringCRef name, const RenderState::InputAssemblyState &state)
	{
		// skip if default
		if ( state == RenderState::InputAssemblyState() )
			return "";

		String	str;
		str << ExtractIndent( name ) << "// InputAssemblyState\n"
			<< name << ".topology         = " << ToString( state.topology ) << ";\n"
			<< name << ".primitiveRestart = " << state.primitiveRestart << ";\n\n";
		return str;
	}
	
/*
=================================================
	ToString (MultisampleState)
=================================================
*/
	String  CppSerializer::ToString (StringCRef name, const RenderState::MultisampleState &state)
	{
		// skip if default
		if ( state == RenderState::MultisampleState() )
			return "";

		String	str;
		str << ExtractIndent( name ) << "// MultisampleState\n"
			<< name << ".samples          = " << ToString( state.samples ) << ";\n"
			<< name << ".minSampleShading = " << state.minSampleShading.Value() << ";\n"
			<< name << ".sampleShading    = " << state.sampleShading << ";\n"
			<< name << ".alphaToCoverage  = " << state.alphaToCoverage << ";\n"
			<< name << ".alphaToOne       = " << state.alphaToOne << ";\n\n";
		return str;
	}

/*
=================================================
	ToString (EPipelineDynamicState)
=================================================
*/
	String  CppSerializer::ToString (StringCRef name, EPipelineDynamicState::bits value) const
	{
		String	str;	str << name << " = EPipelineDynamicState::bits()";

		FOR( i, value )
		{
			const auto	t = EPipelineDynamicState::type(i);

			if ( not value[t] )
				continue;

			switch ( t )
			{
				case EPipelineDynamicState::Viewport :				str << " | EPipelineDynamicState::Viewport";			break;
				case EPipelineDynamicState::Scissor :				str << " | EPipelineDynamicState::Scissor";				break;
				case EPipelineDynamicState::LineWidth :				str << " | EPipelineDynamicState::LineWidth";			break;
				case EPipelineDynamicState::DepthBias :				str << " | EPipelineDynamicState::DepthBias";			break;
				case EPipelineDynamicState::BlendConstants :		str << " | EPipelineDynamicState::BlendConstants";		break;
				case EPipelineDynamicState::DepthBounds :			str << " | EPipelineDynamicState::DepthBounds";			break;
				case EPipelineDynamicState::StencilCompareMask :	str << " | EPipelineDynamicState::StencilCompareMask";	break;
				case EPipelineDynamicState::StencilWriteMask :		str << " | EPipelineDynamicState::StencilWriteMask";	break;
				case EPipelineDynamicState::StencilReference :		str << " | EPipelineDynamicState::StencilReference";	break;
				default :											RETURN_ERR( "unknown dynamic state!" );
			}
		}
		str << ";\n";
		return str;
	}
	
/*
=================================================
	ToString (VertexAttribs)
=================================================
*/
	String  CppSerializer::ToString (StringCRef name, const VertexAttribs &attribs) const
	{
		String	str;	str << name << " = VertexAttribs()";

		FOR( i, attribs )
		{
			auto const&	attr = attribs[i];

			str << '\n' << ExtractIndent( name ) << "\t\t.Add( \"" << attr.first << "\", " 
				<< ToString( attr.second.type ) << ", " << uint(attr.second.index) << ", \""
				<< attr.second.buffer << "\" )";
		}

		str << ";\n";
		return str;
	}
	
/*
=================================================
	ToString (FragmentOutputState)
=================================================
*/
	String  CppSerializer::ToString (StringCRef name, const FragmentOutputState &state) const
	{
		String	str;	str << name << " = FragmentOutputState()";

		FOR( i, state )
		{
			auto const&	frag = state[i];

			str << '\n' << ExtractIndent( name ) << "\t\t.Add( \"" << frag.first << "\", "
				<< ToString( frag.second.type ) << ", " << uint(frag.second.index) << " )";
		}

		str << ";\n";
		return str;
	}
	
/*
=================================================
	PipelineLayoutToStringFunc
=================================================
*/
	struct CppSerializer::PipelineLayoutToStringFunc
	{
	// types
		using TextureUniform	= PipelineLayoutDescriptor::TextureUniform;
		using SamplerUniform	= PipelineLayoutDescriptor::SamplerUniform;
		using ImageUniform		= PipelineLayoutDescriptor::ImageUniform;
		using UniformBuffer		= PipelineLayoutDescriptor::UniformBuffer;
		using StorageBuffer		= PipelineLayoutDescriptor::StorageBuffer;
		using PushConstant		= PipelineLayoutDescriptor::PushConstant;
		using SubpassInput		= PipelineLayoutDescriptor::SubpassInput;
		using Uniform			= PipelineLayoutDescriptor::Uniform;


	// variables
		String&				str;
		const String		indent;


	// methods
		PipelineLayoutToStringFunc (String& str, String &&indent) :
			str(str), indent(RVREF(indent))
		{}


		void operator () (const TextureUniform &tex) const
		{
			ASSERT( tex.binding != UMax and tex.uniqueIndex != UMax );

			str << '\n' << indent << "\t\t.AddTexture( \"" << tex.name << "\", " << ToString( tex.textureType )
				<< ", " << ToString( tex.format ) << ", " << tex.binding << ", " << tex.uniqueIndex
				<< ", " << ToString( tex.stageFlags ) << " )";
		}


		void operator () (const SamplerUniform &) const
		{
			TODO( "SamplerUniform" );
		}


		void operator () (const ImageUniform &img) const
		{
			ASSERT( img.binding != UMax and img.uniqueIndex != UMax );

			str << '\n' << indent << "\t\t.AddImage( \"" << img.name << "\", " << ToString( img.imageType )
				<< ", " << ToString( img.format ) << ", " << img.writeAccess << ", " << img.readAccess << ", "
				<< img.binding << ", " << img.uniqueIndex << ", " << ToString( img.stageFlags ) << " )";
		}


		void operator () (const UniformBuffer &ub) const
		{
			ASSERT( ub.binding != UMax and ub.uniqueIndex != UMax );
			ASSERT( ub.size > BytesUL(0) );

			str << '\n' << indent << "\t\t.AddUniformBuffer( \"" << ub.name << "\", " << usize(ub.size) << "_b, "
				<< ub.binding << ", " << ub.uniqueIndex << ", " << ToString( ub.stageFlags ) << " )";
		}


		void operator () (const StorageBuffer &sb) const
		{
			ASSERT( sb.binding != UMax and sb.uniqueIndex != UMax );
			ASSERT( sb.staticSize > BytesUL(0) or sb.arrayStride > BytesUL(0) );

			str << '\n' << indent << "\t\t.AddStorageBuffer( \"" << sb.name << "\", " << usize(sb.staticSize) << "_b, "
				<< usize(sb.arrayStride) << "_b, " << sb.writeAccess << ", " << sb.readAccess << ", " << sb.binding << ", "
				<< sb.uniqueIndex << ", " << ToString( sb.stageFlags ) << " )";
		}


		void operator () (const PushConstant &pc) const
		{
			ASSERT( pc.size > 0_b );

			str << '\n' << indent << "\t\t.AddPushConstant( \"" << pc.name << "\", " << usize(pc.offset) << "_b, "
				<< usize(pc.size) << "_b, " << ToString( pc.stageFlags ) << " )";
		}


		void operator () (const SubpassInput &sp) const
		{
			TODO( "SubpassInput" );
		}


		void operator () (const Uniform &sp) const
		{
			TODO( "Uniform" );
		}
	};
	
/*
=================================================
	ToString (PipelineLayoutDescriptor)
=================================================
*/
	String  CppSerializer::ToString (StringCRef name, const PipelineLayoutDescriptor &value) const
	{
		String	str;	str << name << " = PipelineLayoutDescriptor::Builder()";
		
		PipelineLayoutToStringFunc	func( INOUT str, ExtractIndent( name ) );

		FOR( i, value.GetUniforms() ) {
			value.GetUniforms()[i].Apply( func );
		}

		str << '\n' << ExtractIndent( name ) << "\t\t.Finish();\n";
		return str;
	}
	
/*
=================================================
	ToString (EPrimitive)
=================================================
*/
	String  CppSerializer::ToString (StringCRef name, EPrimitive::bits value) const
	{
		String	str;	str << name << " = EPrimitive::bits()";

		FOR( i, value )
		{
			const auto	t = EPrimitive::type(i);

			if ( not value[t] )
				continue;

			str << " | " << ToString( t );
		}
		str << ";\n";
		return str;
	}
	
/*
=================================================
	ToString (EShader)
=================================================
*/
	String  CppSerializer::ToString (StringCRef name, EShader::bits value) const
	{
		return String(name) << " = " << ToString( value ) << ";\n";
	}
	
/*
=================================================
	ToString (StringCRef)
=================================================
*/
	String  CppSerializer::ToString (StringCRef value) const
	{
		const usize	max_len = 16000;	// 16380 is max for MSVS

		if ( value.Length() > max_len )
		{
			String		result;
			StringCRef	temp = value;
			usize		pos  = max_len;

			while ( temp.Length() > max_len )
			{
				// find best place to separate string
				bool found = false;

				for (uint i = 0; i < 100; ++i)	// 100 lines
				{
					StringParser::ToPrevLine( temp, INOUT pos );
					usize		p = pos;
					StringCRef	res;
					StringParser::ReadCurrLine( temp, INOUT p, OUT res );

					if ( res.Empty() )
					{
						result << (result.Empty() ? "" : "\n+\n") << "R\"#("_str << temp.SubString( 0, pos ) << ")#\"_str";
						temp	= temp.SubString( pos );
						pos		= Min( temp.Length(), max_len );
						found	= true;
						break;
					}
				}

				// separate in max length
				if ( not found )
				{
					pos		= Min( temp.Length(), max_len );
					result << (result.Empty() ? "" : "\n+\n") << "R\"#("_str << temp.SubString( 0, pos ) << ")#\"_str";
					temp	= temp.SubString( pos );
					pos		= Min( temp.Length(), max_len );
				}
			}

			if ( not temp.Empty() )
			{
				result << (result.Empty() ? "" : "\n+\n") << "R\"#("_str << temp << ")#\"_str";
			}

			return result;
		}
		else
			return "R\"#("_str << value << ")#\"_str";
	}
	
/*
=================================================
	ToString (EShaderVariable)
=================================================
*/
	String  CppSerializer::ToString (EShaderVariable::type value) const
	{
		switch ( value )
		{
			case EShaderVariable::Bool :		return "bool";
			case EShaderVariable::Bool2 :		return "bool2";
			case EShaderVariable::Bool3 :		return "bool3";
			case EShaderVariable::Bool4 :		return "bool4";
			case EShaderVariable::Int :			return "int";
			case EShaderVariable::Int2 :		return "int2";
			case EShaderVariable::Int3 :		return "int3";
			case EShaderVariable::Int4 :		return "int4";
			case EShaderVariable::UInt :		return "uint";
			case EShaderVariable::UInt2 :		return "uint2";
			case EShaderVariable::UInt3 :		return "uint3";
			case EShaderVariable::UInt4 :		return "uint4";
			case EShaderVariable::Long :		return "ilong";
			case EShaderVariable::Long2 :		return "ilong2";
			case EShaderVariable::Long3 :		return "ilong3";
			case EShaderVariable::Long4 :		return "ilong4";
			case EShaderVariable::ULong :		return "ulong";
			case EShaderVariable::ULong2 :		return "ulong2";
			case EShaderVariable::ULong3 :		return "ulong3";
			case EShaderVariable::ULong4 :		return "ulong4";
			case EShaderVariable::Float :		return "float";
			case EShaderVariable::Float2 :		return "float2";
			case EShaderVariable::Float3 :		return "float3";
			case EShaderVariable::Float4 :		return "float4";
			case EShaderVariable::Float2x2 :	return "float2x2";
			case EShaderVariable::Float2x3 :	return "float2x3";
			case EShaderVariable::Float2x4 :	return "float2x4";
			case EShaderVariable::Float3x2 :	return "float3x2";
			case EShaderVariable::Float3x3 :	return "float3x3";
			case EShaderVariable::Float3x4 :	return "float3x4";
			case EShaderVariable::Float4x2 :	return "float4x2";
			case EShaderVariable::Float4x3 :	return "float4x3";
			case EShaderVariable::Float4x4 :	return "float4x4";
			case EShaderVariable::Double :		return "double";
			case EShaderVariable::Double2 :		return "double2";
			case EShaderVariable::Double3 :		return "double3";
			case EShaderVariable::Double4 :		return "double4";
			case EShaderVariable::Double2x2 :	return "double2x2";
			case EShaderVariable::Double2x3 :	return "double2x3";
			case EShaderVariable::Double2x4 :	return "double2x4";
			case EShaderVariable::Double3x2 :	return "double3x2";
			case EShaderVariable::Double3x3 :	return "double3x3";
			case EShaderVariable::Double3x4 :	return "double3x4";
			case EShaderVariable::Double4x2 :	return "double4x2";
			case EShaderVariable::Double4x3 :	return "double4x3";
			case EShaderVariable::Double4x4 :	return "double4x4";
		}

		RETURN_ERR( "invalid variable type", "unknown" );
	}

/*
=================================================
	ToString (BinArrayCRef)
=================================================
*/
	String  CppSerializer::ToString (BinArrayCRef value)
	{
		String	str;	str.Reserve( value.Count() * 6 );

		FOR( i, value ) {
			str << (i ? ", " : "") << (i%12 == 0 ? "\n" : "") << "0x" << String().FormatAlignedI( value[i], 2, '0', 16 );
		}
		return str;
	}
	
/*
=================================================
	ToString (ArrayCRef<uint>)
=================================================
*/
	String  CppSerializer::ToString (ArrayCRef<uint> value)
	{
		String	str;	str.Reserve( value.Count() * 14 );

		FOR( i, value ) {
			str << (i ? ", " : "") << (i%12 == 0 ? "\n" : "") << "0x" << String().FormatAlignedI( value[i], 8, '0', 16 );
		}
		return str;
	}

/*
=================================================
	Include
=================================================
*/
	String	CppSerializer::Include (StringCRef filename) const
	{
		return "#include \""_str << filename << "\"\n";
	}
	
/*
=================================================
	Comment
=================================================
*/
	String	CppSerializer::Comment (StringCRef comment) const
	{
		return "// "_str << comment << "\n";
	}
	
/*
=================================================
	ShaderSrcGLSL
=================================================
*/
	String	CppSerializer::ShaderSrcGLSL (StringCRef name, BinArrayCRef shaderSrc) const
	{
		String	str;
		if ( not shaderSrc.Empty() )
			str << name << ".StringGLSL( \n" << ToString(StringCRef( (const char*)shaderSrc.ptr() )) << " );\n";
		return str;
	}
	
/*
=================================================
	ShaderBinGLSL
=================================================
*/
	String	CppSerializer::ShaderBinGLSL (StringCRef name, BinArrayCRef shaderSrc) const
	{
		String	str;
		if ( not shaderSrc.Empty() )
			str << name << ".ArrayGLSLBin({ " << ToString(ArrayCRef<ubyte>::From( shaderSrc )) << " });\n";
		return str;
	}
	
/*
=================================================
	ShaderFileSrcGLSL
=================================================
*/
	String	CppSerializer::ShaderFileSrcGLSL (StringCRef name, BinArrayCRef shaderSrc) const
	{
		TODO("");
		return "";
	}
	
/*
=================================================
	ShaderFileBinGLSL
=================================================
*/
	String	CppSerializer::ShaderFileBinGLSL (StringCRef name, BinArrayCRef shaderSrc) const
	{
		TODO("");
		return "";
	}
	
/*
=================================================
	ShaderBinSPIRV
=================================================
*/
	String	CppSerializer::ShaderBinSPIRV (StringCRef name, BinArrayCRef shaderSrc) const
	{
		String	str;
		if ( not shaderSrc.Empty() )
			str << name << ".ArraySPIRV({ " << ToString(ArrayCRef<uint>::From( shaderSrc )) << " });\n";
		return str;
	}
	
/*
=================================================
	ShaderSrcSPIRV
=================================================
*/
	String	CppSerializer::ShaderSrcSPIRV (StringCRef name, BinArrayCRef shaderSrc) const
	{
		String	str;
		if ( not shaderSrc.Empty() )
			str << name << ".ArrayAsmSPIRV( \n" << ToString(StringCRef( (const char*)shaderSrc.ptr() )) << " );\n";
		return str;
	}
	
/*
=================================================
	ShaderFileSrcSPIRV
=================================================
*/
	String	CppSerializer::ShaderFileSrcSPIRV (StringCRef name, BinArrayCRef shaderSrc) const
	{
		TODO("");
		return "";
	}
	
/*
=================================================
	ShaderFileBinSPIRV
=================================================
*/
	String	CppSerializer::ShaderFileBinSPIRV (StringCRef name, BinArrayCRef shaderSrc) const
	{
		TODO("");
		return "";
	}
	
/*
=================================================
	ShaderBinCL
=================================================
*/
	String  CppSerializer::ShaderBinCL (StringCRef name, BinArrayCRef shaderSrc) const
	{
		String	str;
		if ( not shaderSrc.Empty() )
			str << name << ".ArrayCLBin({ " << ToString(ArrayCRef<ubyte>::From( shaderSrc )) << " });\n";
		return str;
	}
	
/*
=================================================
	ShaderSrcCL
=================================================
*/
	String  CppSerializer::ShaderSrcCL (StringCRef name, BinArrayCRef shaderSrc) const
	{
		String	str;
		if ( not shaderSrc.Empty() )
			str << name << ".StringCL( \n" << ToString(StringCRef( (const char*)shaderSrc.ptr() )) << " );\n";
		return str;
	}
	
/*
=================================================
	ShaderFileSrcCL
=================================================
*/
	String  CppSerializer::ShaderFileSrcCL (StringCRef name, BinArrayCRef shaderSrc) const
	{
		TODO("");
		return "";
	}
	
/*
=================================================
	ShaderFileBinCL
=================================================
*/
	String  CppSerializer::ShaderFileBinCL (StringCRef name, BinArrayCRef shaderSrc) const
	{
		TODO("");
		return "";
	}
	
/*
=================================================
	ShaderSrcCPP
=================================================
*/
	String  CppSerializer::ShaderSrcCPP (StringCRef name, BinArrayCRef shaderSrc) const
	{
		String	str;
		if ( not shaderSrc.Empty() )
		{
			str << name << ".FuncSW( LAMBDA() (const SWShaderLang::SWShaderHelper &helper)\n"
				<< "{\n"
				<< StringCRef( (const char*)shaderSrc.ptr() )
				<< "\n} );\n";
		}
		return str;
	}

/*
=================================================
	DeclVariable
=================================================
*/
	String	CppSerializer::DeclVariable (StringCRef typeName, StringCRef name, StringCRef value) const
	{
		return String(typeName) << "  " << name << (value.Empty() ? "" : " = "_str << value) << ";\n";
	}
	
/*
=================================================
	AssignVariable
=================================================
*/
	String  CppSerializer::AssignVariable (StringCRef name, StringCRef value) const
	{
		return String(name) << " = " << value << ";\n";
	}

/*
=================================================
	DeclFunction
=================================================
*/
	String	CppSerializer::DeclFunction (StringCRef result, StringCRef name, ArrayCRef<Pair<StringCRef, StringCRef>> args,
										 bool isForwardDeclaration) const
	{
		String	str;
		str << result << ' ' << name << " (";

		FOR( i, args ) {
			str << (i ? ", " : "") << args[i].first << " " << args[i].second;
		}
		str << ")" << (isForwardDeclaration ? ";" : "") << "\n";
		return str;
	}
	
/*
=================================================
	CallFunction
=================================================
*/
	String  CppSerializer::CallFunction (StringCRef name, ArrayCRef<StringCRef> args) const
	{
		String	str;
		str << name << "(";

		FOR( i, args ) {
			str << (i ? ", " : " ") << args[i];
		}

		str << (args.Empty() ? "" : " ") << ")";
		return str;
	}
	
/*
=================================================
	DeclNamespace
=================================================
*/
	String  CppSerializer::DeclNamespace (StringCRef name) const
	{
		return "namespace "_str << name << "\n";
	}

/*
=================================================
	BeginFile
=================================================
*/
	String	CppSerializer::BeginFile (bool isIncludable) const
	{
		++_fileCounter;
		return isIncludable ? "#pragma once\n" : "";
	}
	
/*
=================================================
	EndFile
=================================================
*/
	String	CppSerializer::EndFile (bool) const
	{
		--_fileCounter;
		ASSERT( _fileCounter >= 0 );
		return "";
	}

/*
=================================================
	BeginScope
=================================================
*/
	String	CppSerializer::BeginScope () const
	{
		++_scopeCounter;
		return "{\n";
	}
	
/*
=================================================
	EndScope
=================================================
*/
	String	CppSerializer::EndScope () const
	{
		--_scopeCounter;
		ASSERT( _scopeCounter >= 0 );
		return "};\n";
	}

/*
=================================================
	BeginStruct
=================================================
*/
	String	CppSerializer::BeginStruct (StringCRef typeName, uint sizeOf) const
	{
		String	indent;		indent.Resize( _structStack.Count()-1, '\t' );

		if ( sizeOf > 0 )
			_structStack.Push({ typeName,  String(indent) << "\t\tSTATIC_ASSERT( sizeof(" << typeName << ") == " << sizeOf << " );\n" });
		else
			_structStack.Push({ typeName, "" });

		return String(indent) << "struct " << typeName << " final\n" << indent << "{\n";
	}
	
/*
=================================================
	StructField
=================================================
*/
	String	CppSerializer::StructField (StringCRef name, StringCRef typeName, uint arraySize, uint offset, uint align, uint sizeOf) const
	{
		String	indent;		indent.Resize( _structStack.Count()-2, '\t' );

		_structStack.Get().second << indent << "\t\tSTATIC_ASSERT( (offsetof( " << _structStack.Get().first << ", " << name << " ) == "
								  << offset << ") and (sizeof( " << name << " ) == " << sizeOf << ") );\n";

		String	type_name = typeName;

		// TODO: use Bool type...
		if ( typeName.StartsWith("bool") )
		{
			if ( typeName.Length() == 4 )	type_name = "uint";								else
			if ( typeName.Length() == 5 )	type_name = "uint"_str << typeName.SubString( 4 );
		}

		String	str;
		str << indent << "\t" << type_name << "  " << name;

		if ( arraySize > 1 )
			str << "[" << arraySize << "]";
		else
		if ( arraySize == 0 )
		{
			WARNING( "dynamic arrays are not supported!" );
			str << "[]";
		}

		str << ";    // offset: " << offset << ", align: " << align << "\n";
		return str;
	}

/*
=================================================
	EndStruct
=================================================
*/
	String	CppSerializer::EndStruct () const
	{
		String	indent;		indent.Resize( _structStack.Count()-2, '\t' );

		String	str;

		if ( not _structStack.Get().second.Empty() ) {
			str << "\n\t" << indent << _structStack.Get().first << " ()\n"
				<< indent << "\t{\n" << _structStack.Get().second << indent << "\t}\n";
		}

		_structStack.Pop();

		return str << indent << "};\n";
	}
	
/*
=================================================
	ToString (EImage)
=================================================
*/
	String  CppSerializer::ToString (EImage::type value)
	{
		switch ( value )
		{
			case EImage::Tex1D :			return "EImage::Tex1D";
			case EImage::Tex1DArray :		return "EImage::Tex1DArray";
			case EImage::Tex2D :			return "EImage::Tex2D";
			case EImage::Tex2DArray :		return "EImage::Tex2DArray";
			case EImage::Tex2DMS :			return "EImage::Tex2DMS";
			case EImage::Tex2DMSArray :		return "EImage::Tex2DMSArray";
			case EImage::TexCube :			return "EImage::TexCube";
			case EImage::TexCubeArray :		return "EImage::TexCubeArray";
			case EImage::Tex3D :			return "EImage::Tex3D";
			case EImage::Buffer :			return "EImage::Buffer";
		}
		RETURN_ERR( "unsupported texture type!" );
	}

/*
=================================================
	ToString (EShader)
=================================================
*/
	String  CppSerializer::ToString (EShader::bits value)
	{
		String str = "EShader::bits()";

		FOR( i, value )
		{
			const auto	t = EShader::type(i);

			if ( not value[t] )
				continue;

			switch ( t )
			{
				case EShader::Vertex :			str << " | EShader::Vertex";			break;
				case EShader::TessControl :		str << " | EShader::TessControl";		break;
				case EShader::TessEvaluation :	str << " | EShader::TessEvaluation";	break;
				case EShader::Geometry :		str << " | EShader::Geometry";			break;
				case EShader::Fragment :		str << " | EShader::Fragment";			break;
				case EShader::Compute :			str << " | EShader::Compute";			break;
				default :						RETURN_ERR( "unwnown shader type!" );
			}
		}
		return str;
	}

/*
=================================================
	ToString (EPixelFormatClass)
=================================================
*/
	String  CppSerializer::ToString (EPixelFormatClass::type value)
	{
		if ( EnumEq( value, EPixelFormatClass::Any ) )
			return "EPixelFormatClass::Any";

		FixedSizeArray<EPixelFormatClass::type, 64>		formats;

		// color channels
		if ( EnumEq( value, EPixelFormatClass::AnyColorChannels ) )
			formats << EPixelFormatClass::AnyColorChannels;
		else {
			if ( EnumEq( value, EPixelFormatClass::R ) )
				formats << EPixelFormatClass::R;

			if ( EnumEq( value, EPixelFormatClass::RG ) )
				formats << EPixelFormatClass::RG;

			if ( EnumEq( value, EPixelFormatClass::RGB ) )
				formats << EPixelFormatClass::RGB;

			if ( EnumEq( value, EPixelFormatClass::RGBA ) )
				formats << EPixelFormatClass::RGBA;
		}

		// depth stencil channels
		if ( EnumEq( value, EPixelFormatClass::AnyDepthStencil ) )
			formats << EPixelFormatClass::AnyDepthStencil;
		else {
			if ( EnumEq( value, EPixelFormatClass::Depth ) )
				formats << EPixelFormatClass::Depth;

			if ( EnumEq( value, EPixelFormatClass::DepthStencil ) )
				formats << EPixelFormatClass::DepthStencil;
		}

		// color space
		if ( EnumEq( value, EPixelFormatClass::AnyColorSpace ) )
			formats << EPixelFormatClass::AnyColorSpace;
		else {
			if ( EnumEq( value, EPixelFormatClass::LinearColorSpace ) )
				formats << EPixelFormatClass::LinearColorSpace;

			if ( EnumEq( value, EPixelFormatClass::NonLinearColorSpace ) )
				formats << EPixelFormatClass::NonLinearColorSpace;
		}
		
		// format type
		if ( EnumEq( value, EPixelFormatClass::AnyType ) )
			formats << EPixelFormatClass::AnyType;
		else
		{
			// float formats
			if ( EnumEq( value, EPixelFormatClass::AnyFloat ) )
				formats << EPixelFormatClass::AnyFloat;
			else {
				if ( EnumEq( value, EPixelFormatClass::Float64 ) )
					formats << EPixelFormatClass::Float64;

				if ( EnumEq( value, EPixelFormatClass::Float32 ) )
					formats << EPixelFormatClass::Float32;

				if ( EnumEq( value, EPixelFormatClass::Float16 ) )
					formats << EPixelFormatClass::Float16;
			}

			// signed/unsigned integer formats
			if ( EnumEq( value, EPixelFormatClass::AnyInteger ) )
				formats << EPixelFormatClass::AnyInteger;
			else {
				// integer formats
				if ( EnumEq( value, EPixelFormatClass::AnyInt ) )
					formats << EPixelFormatClass::AnyInt;
				else {
					if ( EnumEq( value, EPixelFormatClass::Int32 ) )
						formats << EPixelFormatClass::Int32;

					if ( EnumEq( value, EPixelFormatClass::Int16 ) )
						formats << EPixelFormatClass::Int16;

					if ( EnumEq( value, EPixelFormatClass::Int8 ) )
						formats << EPixelFormatClass::Int8;

					if ( EnumEq( value, EPixelFormatClass::PackedInt ) )
						formats << EPixelFormatClass::PackedInt;
				}

				// unsigned integer formats
				if ( EnumEq( value, EPixelFormatClass::AnyUInt ) )
					formats << EPixelFormatClass::AnyUInt;
				else {
					if ( EnumEq( value, EPixelFormatClass::UInt32 ) )
						formats << EPixelFormatClass::UInt32;

					if ( EnumEq( value, EPixelFormatClass::UInt16 ) )
						formats << EPixelFormatClass::UInt16;

					if ( EnumEq( value, EPixelFormatClass::UInt8 ) )
						formats << EPixelFormatClass::UInt8;

					if ( EnumEq( value, EPixelFormatClass::PackedUInt ) )
						formats << EPixelFormatClass::PackedUInt;
				}
			}

			// normalized formats
			if ( EnumEq( value, EPixelFormatClass::AnyNorm ) )
				formats << EPixelFormatClass::AnyNorm;
			else {
				// signed normalized formats
				if ( EnumEq( value, EPixelFormatClass::AnySNorm ) )
					formats << EPixelFormatClass::AnySNorm;
				else {
					if ( EnumEq( value, EPixelFormatClass::SNorm16 ) )
						formats << EPixelFormatClass::SNorm16;

					if ( EnumEq( value, EPixelFormatClass::SNorm8 ) )
						formats << EPixelFormatClass::SNorm8;

					if ( EnumEq( value, EPixelFormatClass::PackedSNorm ) )
						formats << EPixelFormatClass::PackedSNorm;
				}

				// unsigned normalized formats
				if ( EnumEq( value, EPixelFormatClass::AnyUNorm ) )
					formats << EPixelFormatClass::AnyUNorm;
				else {
					if ( EnumEq( value, EPixelFormatClass::UNorm16 ) )
						formats << EPixelFormatClass::UNorm16;

					if ( EnumEq( value, EPixelFormatClass::UNorm8 ) )
						formats << EPixelFormatClass::UNorm8;

					if ( EnumEq( value, EPixelFormatClass::PackedUNorm ) )
						formats << EPixelFormatClass::PackedUNorm;
				}
			}
		}
		
		String	str;

		FOR( i, formats )
		{
			if ( not str.Empty() )
				str << " | ";

			switch ( formats[i] )
			{
				case EPixelFormatClass::R :						str << "EPixelFormatClass::R";						break;
				case EPixelFormatClass::RG :					str << "EPixelFormatClass::RG";						break;
				case EPixelFormatClass::RGB :					str << "EPixelFormatClass::RGB";					break;
				case EPixelFormatClass::RGBA :					str << "EPixelFormatClass::RGBA";					break;
				case EPixelFormatClass::AnyColorChannels :		str << "EPixelFormatClass::AnyColorChannels";		break;
				case EPixelFormatClass::Depth :					str << "EPixelFormatClass::Depth";					break;
				case EPixelFormatClass::DepthStencil :			str << "EPixelFormatClass::DepthStencil";			break;
				case EPixelFormatClass::AnyDepthStencil :		str << "EPixelFormatClass::AnyDepthStencil";		break;
				case EPixelFormatClass::AnyChannel :			str << "EPixelFormatClass::AnyChannel";				break;
				case EPixelFormatClass::LinearColorSpace :		str << "EPixelFormatClass::LinearColorSpace";		break;
				case EPixelFormatClass::NonLinearColorSpace :	str << "EPixelFormatClass::NonLinearColorSpace";	break;
				case EPixelFormatClass::AnyColorSpace :			str << "EPixelFormatClass::AnyColorSpace";			break;
				case EPixelFormatClass::Float64 :				str << "EPixelFormatClass::Float64";				break;
				case EPixelFormatClass::Float32 :				str << "EPixelFormatClass::Float32";				break;
				case EPixelFormatClass::Float16 :				str << "EPixelFormatClass::Float16";				break;
				case EPixelFormatClass::AnyFloat :				str << "EPixelFormatClass::AnyFloat";				break;
				case EPixelFormatClass::Int32 :					str << "EPixelFormatClass::Int32";					break;
				case EPixelFormatClass::Int16 :					str << "EPixelFormatClass::Int16";					break;
				case EPixelFormatClass::Int8 :					str << "EPixelFormatClass::Int8";					break;
				case EPixelFormatClass::PackedInt :				str << "EPixelFormatClass::PackedInt";				break;
				case EPixelFormatClass::AnyInt :				str << "EPixelFormatClass::AnyInt";					break;
				case EPixelFormatClass::UInt32 :				str << "EPixelFormatClass::UInt32";					break;
				case EPixelFormatClass::UInt16 :				str << "EPixelFormatClass::UInt16";					break;
				case EPixelFormatClass::UInt8 :					str << "EPixelFormatClass::UInt8";					break;
				case EPixelFormatClass::PackedUInt :			str << "EPixelFormatClass::PackedUInt";				break;
				case EPixelFormatClass::AnyUInt :				str << "EPixelFormatClass::AnyUInt";				break;
				case EPixelFormatClass::AnyInteger :			str << "EPixelFormatClass::AnyInteger";				break;
				case EPixelFormatClass::UNorm16 :				str << "EPixelFormatClass::UNorm16";				break;
				case EPixelFormatClass::UNorm8 :				str << "EPixelFormatClass::UNorm8";					break;
				case EPixelFormatClass::PackedUNorm :			str << "EPixelFormatClass::PackedUNorm";			break;
				case EPixelFormatClass::AnyUNorm :				str << "EPixelFormatClass::AnyUNorm";				break;
				case EPixelFormatClass::SNorm16 :				str << "EPixelFormatClass::SNorm16";				break;
				case EPixelFormatClass::SNorm8 :				str << "EPixelFormatClass::SNorm8";					break;
				case EPixelFormatClass::PackedSNorm :			str << "EPixelFormatClass::PackedSNorm";			break;
				case EPixelFormatClass::AnySNorm :				str << "EPixelFormatClass::AnySNorm";				break;
				case EPixelFormatClass::AnyNorm :				str << "EPixelFormatClass::AnyNorm";				break;
				case EPixelFormatClass::AnyType :				str << "EPixelFormatClass::AnyType";				break;
				default :										RETURN_ERR( "unsupported format class value!" );
			}
		}

		return str;
	}
	
/*
=================================================
	ToString (EPixelFormat)
=================================================
*/
	String  CppSerializer::ToString (EPixelFormat::type value)
	{
		switch ( value )
		{
			case EPixelFormat::RGBA16_SNorm :				return "EPixelFormat::RGBA16_SNorm";
			case EPixelFormat::RGBA8_SNorm :				return "EPixelFormat::RGBA8_SNorm";
			case EPixelFormat::RGB16_SNorm :				return "EPixelFormat::RGB16_SNorm";
			case EPixelFormat::RGB8_SNorm :					return "EPixelFormat::RGB8_SNorm";
			case EPixelFormat::RG16_SNorm :					return "EPixelFormat::RG16_SNorm";
			case EPixelFormat::RG8_SNorm :					return "EPixelFormat::RG8_SNorm";
			case EPixelFormat::R16_SNorm :					return "EPixelFormat::R16_SNorm";
			case EPixelFormat::R8_SNorm :					return "EPixelFormat::R8_SNorm";
			case EPixelFormat::RGBA16_UNorm :				return "EPixelFormat::RGBA16_UNorm";
			case EPixelFormat::RGBA8_UNorm :				return "EPixelFormat::RGBA8_UNorm";
			case EPixelFormat::RGB16_UNorm :				return "EPixelFormat::RGB16_UNorm";
			case EPixelFormat::RGB8_UNorm :					return "EPixelFormat::RGB8_UNorm";
			case EPixelFormat::RG16_UNorm :					return "EPixelFormat::RG16_UNorm";
			case EPixelFormat::RG8_UNorm :					return "EPixelFormat::RG8_UNorm";
			case EPixelFormat::R16_UNorm :					return "EPixelFormat::R16_UNorm";
			case EPixelFormat::R8_UNorm :					return "EPixelFormat::R8_UNorm";
			case EPixelFormat::RGB10_A2_UNorm :				return "EPixelFormat::RGB10_A2_UNorm";
			case EPixelFormat::RGBA4_UNorm :				return "EPixelFormat::RGBA4_UNorm";
			case EPixelFormat::RGB5_A1_UNorm :				return "EPixelFormat::RGB5_A1_UNorm";
			case EPixelFormat::RGB_5_6_5_UNorm :			return "EPixelFormat::RGB_5_6_5_UNorm";
			case EPixelFormat::BGR8_UNorm :					return "EPixelFormat::BGR8_UNorm";
			case EPixelFormat::BGRA8_UNorm :				return "EPixelFormat::BGRA8_UNorm";
			case EPixelFormat::sRGB8 :						return "EPixelFormat::sRGB8";
			case EPixelFormat::sRGB8_A8 :					return "EPixelFormat::sRGB8_A8";
			case EPixelFormat::R8I :						return "EPixelFormat::R8I";
			case EPixelFormat::RG8I :						return "EPixelFormat::RG8I";
			case EPixelFormat::RGB8I :						return "EPixelFormat::RGB8I";
			case EPixelFormat::RGBA8I :						return "EPixelFormat::RGBA8I";
			case EPixelFormat::R16I :						return "EPixelFormat::R16I";
			case EPixelFormat::RG16I :						return "EPixelFormat::RG16I";
			case EPixelFormat::RGB16I :						return "EPixelFormat::RGB16I";
			case EPixelFormat::RGBA16I :					return "EPixelFormat::RGBA16I";
			case EPixelFormat::R32I :						return "EPixelFormat::R32I";
			case EPixelFormat::RG32I :						return "EPixelFormat::RG32I";
			case EPixelFormat::RGB32I :						return "EPixelFormat::RGB32I";
			case EPixelFormat::RGBA32I :					return "EPixelFormat::RGBA32I";
			case EPixelFormat::R8U :						return "EPixelFormat::R8U";
			case EPixelFormat::RG8U :						return "EPixelFormat::RG8U";
			case EPixelFormat::RGB8U :						return "EPixelFormat::RGB8U";
			case EPixelFormat::RGBA8U :						return "EPixelFormat::RGBA8U";
			case EPixelFormat::R16U :						return "EPixelFormat::R16U";
			case EPixelFormat::RG16U :						return "EPixelFormat::RG16U";
			case EPixelFormat::RGB16U :						return "EPixelFormat::RGB16U";
			case EPixelFormat::RGBA16U :					return "EPixelFormat::RGBA16U";
			case EPixelFormat::R32U :						return "EPixelFormat::R32U";
			case EPixelFormat::RG32U :						return "EPixelFormat::RG32U";
			case EPixelFormat::RGB32U :						return "EPixelFormat::RGB32U";
			case EPixelFormat::RGBA32U :					return "EPixelFormat::RGBA32U";
			case EPixelFormat::RGB10_A2U :					return "EPixelFormat::RGB10_A2U";
			case EPixelFormat::R16F :						return "EPixelFormat::R16F";
			case EPixelFormat::RG16F :						return "EPixelFormat::RG16F";
			case EPixelFormat::RGB16F :						return "EPixelFormat::RGB16F";
			case EPixelFormat::RGBA16F :					return "EPixelFormat::RGBA16F";
			case EPixelFormat::R32F :						return "EPixelFormat::R32F";
			case EPixelFormat::RG32F :						return "EPixelFormat::RG32F";
			case EPixelFormat::RGB32F :						return "EPixelFormat::RGB32F";
			case EPixelFormat::RGBA32F :					return "EPixelFormat::RGBA32F";
			case EPixelFormat::RGB_11_11_10F :				return "EPixelFormat::RGB_11_11_10F";
			case EPixelFormat::Depth16 :					return "EPixelFormat::Depth16";
			case EPixelFormat::Depth24 :					return "EPixelFormat::Depth24";
			case EPixelFormat::Depth32 :					return "EPixelFormat::Depth32";
			case EPixelFormat::Depth32F :					return "EPixelFormat::Depth32F";
			case EPixelFormat::Depth16_Stencil8 :			return "EPixelFormat::Depth16_Stencil8";
			case EPixelFormat::Depth24_Stencil8 :			return "EPixelFormat::Depth24_Stencil8";
			case EPixelFormat::Depth32F_Stencil8 :			return "EPixelFormat::Depth32F_Stencil8";
			case EPixelFormat::BC1_RGB8_UNorm :				return "EPixelFormat::BC1_RGB8_UNorm";
			case EPixelFormat::BC1_RGB8_A1_UNorm :			return "EPixelFormat::BC1_RGB8_A1_UNorm";
			case EPixelFormat::BC2_RGBA8_UNorm :			return "EPixelFormat::BC2_RGBA8_UNorm";
			case EPixelFormat::BC3_RGBA8_UNorm :			return "EPixelFormat::BC3_RGBA8_UNorm";
			case EPixelFormat::BC4_RED8_SNorm :				return "EPixelFormat::BC4_RED8_SNorm";
			case EPixelFormat::BC4_RED8_UNorm :				return "EPixelFormat::BC4_RED8_UNorm";
			case EPixelFormat::BC5_RG8_SNorm :				return "EPixelFormat::BC5_RG8_SNorm";
			case EPixelFormat::BC5_RG8_UNorm :				return "EPixelFormat::BC5_RG8_UNorm";
			case EPixelFormat::BC7_RGBA8_UNorm :			return "EPixelFormat::BC7_RGBA8_UNorm";
			case EPixelFormat::BC7_SRGB8_A8_UNorm :			return "EPixelFormat::BC7_SRGB8_A8_UNorm";
			case EPixelFormat::BC6H_RGB16F :				return "EPixelFormat::BC6H_RGB16F";
			case EPixelFormat::BC6H_RGB16F_Unsigned :		return "EPixelFormat::BC6H_RGB16F_Unsigned";
			case EPixelFormat::ETC2_RGB8_UNorm :			return "EPixelFormat::ETC2_RGB8_UNorm";
			case EPixelFormat::ECT2_SRGB8_UNorm :			return "EPixelFormat::ECT2_SRGB8_UNorm";
			case EPixelFormat::ETC2_RGB8_A1_UNorm :			return "EPixelFormat::ETC2_RGB8_A1_UNorm";
			case EPixelFormat::ETC2_SRGB8_A1_UNorm :		return "EPixelFormat::ETC2_SRGB8_A1_UNorm";
			case EPixelFormat::ETC2_RGBA8_UNorm :			return "EPixelFormat::ETC2_RGBA8_UNorm";
			case EPixelFormat::ETC2_SRGB8_A8_UNorm :		return "EPixelFormat::ETC2_SRGB8_A8_UNorm";
			case EPixelFormat::EAC_R11_SNorm :				return "EPixelFormat::EAC_R11_SNorm";
			case EPixelFormat::EAC_R11_UNorm :				return "EPixelFormat::EAC_R11_UNorm";
			case EPixelFormat::EAC_RG11_SNorm :				return "EPixelFormat::EAC_RG11_SNorm";
			case EPixelFormat::EAC_RG11_UNorm :				return "EPixelFormat::EAC_RG11_UNorm";
			case EPixelFormat::ASTC_RGBA_4x4 :				return "EPixelFormat::ASTC_RGBA_4x4";
			case EPixelFormat::ASTC_RGBA_5x4 :				return "EPixelFormat::ASTC_RGBA_5x4";
			case EPixelFormat::ASTC_RGBA_5x5 :				return "EPixelFormat::ASTC_RGBA_5x5";
			case EPixelFormat::ASTC_RGBA_6x5 :				return "EPixelFormat::ASTC_RGBA_6x5";
			case EPixelFormat::ASTC_RGBA_6x6 :				return "EPixelFormat::ASTC_RGBA_6x6";
			case EPixelFormat::ASTC_RGBA_8x5 :				return "EPixelFormat::ASTC_RGBA_8x5";
			case EPixelFormat::ASTC_RGBA_8x6 :				return "EPixelFormat::ASTC_RGBA_8x6";
			case EPixelFormat::ASTC_RGBA_8x8 :				return "EPixelFormat::ASTC_RGBA_8x8";
			case EPixelFormat::ASTC_RGBA_10x5 :				return "EPixelFormat::ASTC_RGBA_10x5";
			case EPixelFormat::ASTC_RGBA_10x6 :				return "EPixelFormat::ASTC_RGBA_10x6";
			case EPixelFormat::ASTC_RGBA_10x8 :				return "EPixelFormat::ASTC_RGBA_10x8";
			case EPixelFormat::ASTC_RGBA_10x10 :			return "EPixelFormat::ASTC_RGBA_10x10";
			case EPixelFormat::ASTC_RGBA_12x10 :			return "EPixelFormat::ASTC_RGBA_12x10";
			case EPixelFormat::ASTC_RGBA_12x12 :			return "EPixelFormat::ASTC_RGBA_12x12";
			case EPixelFormat::ASTC_SRGB8_A8_4x4 :			return "EPixelFormat::ASTC_SRGB8_A8_4x4";
			case EPixelFormat::ASTC_SRGB8_A8_5x4 :			return "EPixelFormat::ASTC_SRGB8_A8_5x4";
			case EPixelFormat::ASTC_SRGB8_A8_5x5 :			return "EPixelFormat::ASTC_SRGB8_A8_5x5";
			case EPixelFormat::ASTC_SRGB8_A8_6x5 :			return "EPixelFormat::ASTC_SRGB8_A8_6x5";
			case EPixelFormat::ASTC_SRGB8_A8_6x6 :			return "EPixelFormat::ASTC_SRGB8_A8_6x6";
			case EPixelFormat::ASTC_SRGB8_A8_8x5 :			return "EPixelFormat::ASTC_SRGB8_A8_8x5";
			case EPixelFormat::ASTC_SRGB8_A8_8x6 :			return "EPixelFormat::ASTC_SRGB8_A8_8x6";
			case EPixelFormat::ASTC_SRGB8_A8_8x8 :			return "EPixelFormat::ASTC_SRGB8_A8_8x8";
			case EPixelFormat::ASTC_SRGB8_A8_10x5 :			return "EPixelFormat::ASTC_SRGB8_A8_10x5";
			case EPixelFormat::ASTC_SRGB8_A8_10x6 :			return "EPixelFormat::ASTC_SRGB8_A8_10x6";
			case EPixelFormat::ASTC_SRGB8_A8_10x8 :			return "EPixelFormat::ASTC_SRGB8_A8_10x8";
			case EPixelFormat::ASTC_SRGB8_A8_10x10 :		return "EPixelFormat::ASTC_SRGB8_A8_10x10";
			case EPixelFormat::ASTC_SRGB8_A8_12x10 :		return "EPixelFormat::ASTC_SRGB8_A8_12x10";
			case EPixelFormat::ASTC_SRGB8_A8_12x12 :		return "EPixelFormat::ASTC_SRGB8_A8_12x12";
		}

		RETURN_ERR( "unsupported pixel format", "EPixelFormat::type("_str << uint(value) << ")" );
	}

}	// PipelineCompiler
