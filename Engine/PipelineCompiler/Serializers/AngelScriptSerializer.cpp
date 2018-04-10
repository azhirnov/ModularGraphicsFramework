// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Engine/PipelineCompiler/Serializers/AngelScriptSerializer.h"

namespace PipelineCompiler
{
	
/*
=================================================
	constructor
=================================================
*/
	AngelScriptSerializer::AngelScriptSerializer ()
	{
	}
	
/*
=================================================
	destructor
=================================================
*/
	AngelScriptSerializer::~AngelScriptSerializer ()
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
	String  AngelScriptSerializer::ToString (EBlendFunc::type value)
	{
		switch ( value )
		{
			case EBlendFunc::Zero :					return "EBlendFunc_Zero";
			case EBlendFunc::One :					return "EBlendFunc_One";
			case EBlendFunc::SrcColor :				return "EBlendFunc_SrcColor";
			case EBlendFunc::OneMinusSrcColor :		return "EBlendFunc_OneMinusSrcColor";
			case EBlendFunc::DstColor :				return "EBlendFunc_DstColor";
			case EBlendFunc::OneMinusDstColor :		return "EBlendFunc_OneMinusDstColor";
			case EBlendFunc::SrcAlpha :				return "EBlendFunc_SrcAlpha";
			case EBlendFunc::OneMinusSrcAlpha :		return "EBlendFunc_OneMinusSrcAlpha";
			case EBlendFunc::DstAlpha :				return "EBlendFunc_DstAlpha";
			case EBlendFunc::OneMinusDstAlpha :		return "EBlendFunc_OneMinusDstAlpha";
			case EBlendFunc::ConstColor :			return "EBlendFunc_ConstColor";
			case EBlendFunc::OneMinusConstColor :	return "EBlendFunc_OneMinusConstColor";
			case EBlendFunc::ConstAlpha :			return "EBlendFunc_ConstAlpha";
			case EBlendFunc::OneMinusConstAlpha :	return "EBlendFunc_OneMinusConstAlpha";
			case EBlendFunc::SrcAlphaSaturate :		return "EBlendFunc_SrcAlphaSaturate";
		}
		RETURN_ERR( "unknown blend function type!" );
	}
	
/*
=================================================
	ToString (EBlendEq)
=================================================
*/
	String  AngelScriptSerializer::ToString (EBlendEq::type value)
	{
		switch ( value )
		{
			case EBlendEq::Add :		return "EBlendEq_Add";
			case EBlendEq::Sub :		return "EBlendEq_Sub";
			case EBlendEq::RevSub :		return "EBlendEq_RevSub";
			case EBlendEq::Min :		return "EBlendEq_Min";
			case EBlendEq::Max :		return "EBlendEq_Max";
		}
		RETURN_ERR( "unknown blend equation type!" );
	}
	
/*
=================================================
	ToString (ELogicOp)
=================================================
*/
	String  AngelScriptSerializer::ToString (ELogicOp::type value)
	{
		switch ( value )
		{
			case ELogicOp::None :			return "ELogicOp_None";
			case ELogicOp::Clear :			return "ELogicOp_Clear";
			case ELogicOp::Set :			return "ELogicOp_Set";
			case ELogicOp::Copy :			return "ELogicOp_Copy";
			case ELogicOp::CopyInverted :	return "ELogicOp_CopyInverted";
			case ELogicOp::Noop :			return "ELogicOp_Noop";
			case ELogicOp::Invert :			return "ELogicOp_Invert";
			case ELogicOp::And :			return "ELogicOp:_And";
			case ELogicOp::NotAnd :			return "ELogicOp_NotAnd";
			case ELogicOp::Or :				return "ELogicOp_Or";
			case ELogicOp::NotOr :			return "ELogicOp_NotOr";
			case ELogicOp::Xor :			return "ELogicOp_Xor";
			case ELogicOp::Equiv :			return "ELogicOp_Equiv";
			case ELogicOp::AndReverse :		return "ELogicOp_AndReverse";
			case ELogicOp::AndInverted :	return "ELogicOp_AndInverted";
			case ELogicOp::OrReverse :		return "ELogicOp_OrReverse";
			case ELogicOp::OrInverted :		return "ELogicOp_OrInverted";
		}
		RETURN_ERR( "unknown logic operation type!" );
	}

/*
=================================================
	ToString (ECompareFunc)
=================================================
*/
	String  AngelScriptSerializer::ToString (ECompareFunc::type value)
	{
		switch ( value )
		{
			//case ECompareFunc::None
			case ECompareFunc::Never :		return "ECompareFunc_Never";
			case ECompareFunc::Less :		return "ECompareFunc_Less";
			case ECompareFunc::Equal :		return "ECompareFunc_Equal";
			case ECompareFunc::LEqual :		return "ECompareFunc_LEqual";
			case ECompareFunc::Greater :	return "ECompareFunc_Greater";
			case ECompareFunc::NotEqual :	return "ECompareFunc_NotEqual";
			case ECompareFunc::GEqual :		return "ECompareFunc_GEqual";
			case ECompareFunc::Always :		return "ECompareFunc_Always";
		}
		RETURN_ERR( "unknown comparison function type!" );
	}
	
/*
=================================================
	ToString (EStencilOp)
=================================================
*/
	String  AngelScriptSerializer::ToString (EStencilOp::type value)
	{
		switch ( value )
		{
			case EStencilOp::Keep :			return "EStencilOp_Keep";
			case EStencilOp::Zero :			return "EStencilOp_Zero";
			case EStencilOp::Replace :		return "EStencilOp_Replace";
			case EStencilOp::Incr :			return "EStencilOp_Incr";
			case EStencilOp::IncrWrap :		return "EStencilOp_IncrWrap";
			case EStencilOp::Decr :			return "EStencilOp_Decr";
			case EStencilOp::DecrWrap :		return "EStencilOp_DecrWrap";
			case EStencilOp::Invert :		return "EStencilOp_Invert";
		}
		RETURN_ERR( "unknown stencil operation type!" );
	}
	
/*
=================================================
	ToString (EPolygonMode)
=================================================
*/
	String  AngelScriptSerializer::ToString (EPolygonMode::type value)
	{
		switch ( value )
		{
			case EPolygonMode::Point :	return "EPolygonMode_Point";
			case EPolygonMode::Line :	return "EPolygonMode_Line";
			case EPolygonMode::Fill :	return "EPolygonMode_Fill";
		}
		RETURN_ERR( "unknown polygon mode!" );
	}
	
/*
=================================================
	ToString (EPolygonFace)
=================================================
*/
	String  AngelScriptSerializer::ToString (EPolygonFace::type value)
	{
		switch ( value )
		{
			case EPolygonFace::None :			return "EPolygonFace_None";
			case EPolygonFace::Front :			return "EPolygonFace_Front";
			case EPolygonFace::Back :			return "EPolygonFace_Back";
			case EPolygonFace::FontAndBack :	return "EPolygonFace_FontAndBack";
		}
		RETURN_ERR( "unknown polygon mode!" );
	}
	
/*
=================================================
	ToString (EPrimitive)
=================================================
*/
	String  AngelScriptSerializer::ToString (EPrimitive::type value)
	{
		switch ( value )
		{
			case EPrimitive::Point :			return "EPrimitive_Point";
			case EPrimitive::LineList :			return "EPrimitive_LineList";
			case EPrimitive::LineStrip :		return "EPrimitive_LineStrip";
			case EPrimitive::TriangleList :		return "EPrimitive_TriangleList";
			case EPrimitive::TriangleStrip :	return "EPrimitive_TriangleStrip";
			case EPrimitive::Patch :			return "EPrimitive_Patch";
		}
		RETURN_ERR( "unknown primitive type!" );
	}
	
/*
=================================================
	ToString (MultiSamples)
=================================================
*/
	String  AngelScriptSerializer::ToString (MultiSamples value)
	{
		return "MultiSamples(PowOf2Value<uint>( "_str << value.GetPowerOf2() << " ))";
	}
	
/*
=================================================
	ToString (EVertexAttribute)
=================================================
*/
	String  AngelScriptSerializer::ToString (EVertexAttribute::type value)
	{
		switch ( value )
		{
			case EVertexAttribute::Byte :			return "EVertexAttribute_Byte";
			case EVertexAttribute::Byte2 :			return "EVertexAttribute_Byte2";
			case EVertexAttribute::Byte3 :			return "EVertexAttribute_Byte3";
			case EVertexAttribute::Byte4 :			return "EVertexAttribute_Byte4";
			case EVertexAttribute::Byte_Norm :		return "EVertexAttribute_Byte_Norm";
			case EVertexAttribute::Byte2_Norm :		return "EVertexAttribute_Byte2_Norm";
			case EVertexAttribute::Byte3_Norm :		return "EVertexAttribute_Byte3_Norm";
			case EVertexAttribute::Byte4_Norm :		return "EVertexAttribute_Byte4_Norm";
			case EVertexAttribute::UByte :			return "EVertexAttribute_UByte";
			case EVertexAttribute::UByte2 :			return "EVertexAttribute_UByte2";
			case EVertexAttribute::UByte3 :			return "EVertexAttribute_UByte3";
			case EVertexAttribute::UByte4 :			return "EVertexAttribute_UByte4";
			case EVertexAttribute::UByte_Norm :		return "EVertexAttribute_UByte_Norm";
			case EVertexAttribute::UByte2_Norm :	return "EVertexAttribute_UByte2_Norm";
			case EVertexAttribute::UByte3_Norm :	return "EVertexAttribute_UByte3_Norm";
			case EVertexAttribute::UByte4_Norm :	return "EVertexAttribute_UByte4_Norm";
			case EVertexAttribute::Short :			return "EVertexAttribute_Short";
			case EVertexAttribute::Short2 :			return "EVertexAttribute_Short2";
			case EVertexAttribute::Short3 :			return "EVertexAttribute_Short3";
			case EVertexAttribute::Short4 :			return "EVertexAttribute_Short4";
			case EVertexAttribute::Short_Norm :		return "EVertexAttribute_Short_Norm";
			case EVertexAttribute::Short2_Norm :	return "EVertexAttribute_Short2_Norm";
			case EVertexAttribute::Short3_Norm :	return "EVertexAttribute_Short3_Norm";
			case EVertexAttribute::Short4_Norm :	return "EVertexAttribute_Short4_Norm";
			case EVertexAttribute::UShort :			return "EVertexAttribute_UShort";
			case EVertexAttribute::UShort2 :		return "EVertexAttribute_UShort2";
			case EVertexAttribute::UShort3 :		return "EVertexAttribute_UShort3";
			case EVertexAttribute::UShort4 :		return "EVertexAttribute_UShort4";
			case EVertexAttribute::UShort_Norm :	return "EVertexAttribute_UShort_Norm";
			case EVertexAttribute::UShort2_Norm :	return "EVertexAttribute_UShort2_Norm";
			case EVertexAttribute::UShort3_Norm :	return "EVertexAttribute_UShort3_Norm";
			case EVertexAttribute::UShort4_Norm :	return "EVertexAttribute_UShort4_Norm";
			case EVertexAttribute::Int :			return "EVertexAttribute_Int";
			case EVertexAttribute::Int2 :			return "EVertexAttribute_Int2";
			case EVertexAttribute::Int3 :			return "EVertexAttribute_Int3";
			case EVertexAttribute::Int4 :			return "EVertexAttribute_Int4";
			case EVertexAttribute::Int_Norm :		return "EVertexAttribute_Int_Norm";
			case EVertexAttribute::Int2_Norm :		return "EVertexAttribute_Int2_Norm";
			case EVertexAttribute::Int3_Norm :		return "EVertexAttribute_Int3_Norm";
			case EVertexAttribute::Int4_Norm :		return "EVertexAttribute_Int4_Norm";
			case EVertexAttribute::UInt :			return "EVertexAttribute_UInt";
			case EVertexAttribute::UInt2 :			return "EVertexAttribute_UInt2";
			case EVertexAttribute::UInt3 :			return "EVertexAttribute_UInt3";
			case EVertexAttribute::UInt4 :			return "EVertexAttribute_UInt4";
			case EVertexAttribute::UInt_Norm :		return "EVertexAttribute_UInt_Norm";
			case EVertexAttribute::UInt2_Norm :		return "EVertexAttribute_UInt2_Norm";
			case EVertexAttribute::UInt3_Norm :		return "EVertexAttribute_UInt3_Norm";
			case EVertexAttribute::UInt4_Norm :		return "EVertexAttribute_UInt4_Norm";
			case EVertexAttribute::Long :			return "EVertexAttribute_Long";
			case EVertexAttribute::Long2 :			return "EVertexAttribute_Long2";
			case EVertexAttribute::Long3 :			return "EVertexAttribute_Long3";
			case EVertexAttribute::Long4 :			return "EVertexAttribute_Long4";
			case EVertexAttribute::Long_Norm :		return "EVertexAttribute_Long_Norm";
			case EVertexAttribute::Long2_Norm :		return "EVertexAttribute_Long2_Norm";
			case EVertexAttribute::Long3_Norm :		return "EVertexAttribute_Long3_Norm";
			case EVertexAttribute::Long4_Norm :		return "EVertexAttribute_Long4_Norm";
			case EVertexAttribute::ULong :			return "EVertexAttribute_ULong";
			case EVertexAttribute::ULong2 :			return "EVertexAttribute_ULong2";
			case EVertexAttribute::ULong3 :			return "EVertexAttribute_ULong3";
			case EVertexAttribute::ULong4 :			return "EVertexAttribute_ULong4";
			case EVertexAttribute::ULong_Norm :		return "EVertexAttribute_ULong_Norm";
			case EVertexAttribute::ULong2_Norm :	return "EVertexAttribute_ULong2_Norm";
			case EVertexAttribute::ULong3_Norm :	return "EVertexAttribute_ULong3_Norm";
			case EVertexAttribute::ULong4_Norm :	return "EVertexAttribute_ULong4_Norm";
			case EVertexAttribute::Half :			return "EVertexAttribute_Half";
			case EVertexAttribute::Half2 :			return "EVertexAttribute_Half2";
			case EVertexAttribute::Half3 :			return "EVertexAttribute_Half3";
			case EVertexAttribute::Half4 :			return "EVertexAttribute_Half4";
			case EVertexAttribute::Float :			return "EVertexAttribute_Float";
			case EVertexAttribute::Float2 :			return "EVertexAttribute_Float2";
			case EVertexAttribute::Float3 :			return "EVertexAttribute_Float3";
			case EVertexAttribute::Float4 :			return "EVertexAttribute_Float4";
			case EVertexAttribute::Double :			return "EVertexAttribute_Double";
			case EVertexAttribute::Double2 :		return "EVertexAttribute_Double2";
			case EVertexAttribute::Double3 :		return "EVertexAttribute_Double3";
			case EVertexAttribute::Double4 :		return "EVertexAttribute_Double4";
		}
		RETURN_ERR( "unknown vertex attrib type!" );
	}
	
/*
=================================================
	ToString (EFragOutput)
=================================================
*/
	String  AngelScriptSerializer::ToString (EFragOutput::type value)
	{
		switch ( value )
		{
			case EFragOutput::Int4 :	return "EFragOutput_Int4";
			case EFragOutput::UInt4 :	return "EFragOutput_UInt4";
			case EFragOutput::Float4 :	return "EFragOutput_Float4";
		}
		RETURN_ERR( "unknown fragment output type!" );
	}

/*
=================================================
	ExtractIndent
=================================================
*/
	String  AngelScriptSerializer::ExtractIndent (StringCRef str)
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
	String  AngelScriptSerializer::ToString (StringCRef name, const RenderState &state) const
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
	String  AngelScriptSerializer::ToString (StringCRef name, const RenderState::ColorBuffer &state)
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
	String  AngelScriptSerializer::ToString (StringCRef name, const RenderState::ColorBuffersState &state)
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
	String  AngelScriptSerializer::ToString (StringCRef name, const RenderState::DepthBufferState &state)
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
	String  AngelScriptSerializer::ToString (StringCRef name, const RenderState::StencilFaceState &state)
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
	String  AngelScriptSerializer::ToString (StringCRef name, const RenderState::StencilBufferState &state)
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
	String  AngelScriptSerializer::ToString (StringCRef name, const RenderState::RasterizationState &state)
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
	String  AngelScriptSerializer::ToString (StringCRef name, const RenderState::InputAssemblyState &state)
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
	String  AngelScriptSerializer::ToString (StringCRef name, const RenderState::MultisampleState &state)
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
	String  AngelScriptSerializer::ToString (StringCRef name, EPipelineDynamicState::bits value) const
	{
		String	str;

		FOR( i, value )
		{
			const auto	t = EPipelineDynamicState::type(i);

			if ( not value[t] )
				continue;

			if ( not str.Empty() )
				str << " | ";

			switch ( t )
			{
				case EPipelineDynamicState::Viewport :				str << "EPipelineDynamicState_Viewport";			break;
				case EPipelineDynamicState::Scissor :				str << "EPipelineDynamicState_Scissor";				break;
				case EPipelineDynamicState::LineWidth :				str << "EPipelineDynamicState_LineWidth";			break;
				case EPipelineDynamicState::DepthBias :				str << "EPipelineDynamicState_DepthBias";			break;
				case EPipelineDynamicState::BlendConstants :		str << "EPipelineDynamicState_BlendConstants";		break;
				case EPipelineDynamicState::DepthBounds :			str << "EPipelineDynamicState_DepthBounds";			break;
				case EPipelineDynamicState::StencilCompareMask :	str << "EPipelineDynamicState_StencilCompareMask";	break;
				case EPipelineDynamicState::StencilWriteMask :		str << "EPipelineDynamicState_StencilWriteMask";	break;
				case EPipelineDynamicState::StencilReference :		str << "EPipelineDynamicState_StencilReference";	break;
				default :											RETURN_ERR( "unknown dynamic state!" );
			}
		}

		name >> (" = " >> str);
		str << ";\n";
		return str;
	}
	
/*
=================================================
	ToString (VertexAttribs)
=================================================
*/
	String  AngelScriptSerializer::ToString (StringCRef name, const VertexAttribs &attribs) const
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
	String  AngelScriptSerializer::ToString (StringCRef name, const FragmentOutputState &state) const
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
	struct AngelScriptSerializer::PipelineLayoutToStringFunc
	{
	// types
		using TextureUniform	= PipelineLayoutDescriptor::TextureUniform;
		using SamplerUniform	= PipelineLayoutDescriptor::SamplerUniform;
		using ImageUniform		= PipelineLayoutDescriptor::ImageUniform;
		using UniformBuffer		= PipelineLayoutDescriptor::UniformBuffer;
		using StorageBuffer		= PipelineLayoutDescriptor::StorageBuffer;
		using PushConstant		= PipelineLayoutDescriptor::PushConstant;
		using SubpassInput		= PipelineLayoutDescriptor::SubpassInput;


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
				<< ", " << ToString( img.format ) << ", " << ToString( img.access ) << ", "
				<< img.binding << ", " << img.uniqueIndex << ", " << ToString( img.stageFlags ) << " )";
		}


		void operator () (const UniformBuffer &ub) const
		{
			ASSERT( ub.binding != UMax and ub.uniqueIndex != UMax );
			ASSERT( ub.size > BytesU(0) );

			str << '\n' << indent << "\t\t.AddUniformBuffer( \"" << ub.name << "\", " << usize(ub.size) << "_b, "
				<< ub.binding << ", " << ub.uniqueIndex << ", " << ToString( ub.stageFlags ) << " )";
		}


		void operator () (const StorageBuffer &sb) const
		{
			ASSERT( sb.binding != UMax and sb.uniqueIndex != UMax );
			ASSERT( sb.staticSize > BytesUL(0) or sb.arrayStride > BytesUL(0) );

			str << '\n' << indent << "\t\t.AddStorageBuffer( \"" << sb.name << "\", " << usize(sb.staticSize) << "_b, "
				<< usize(sb.arrayStride) << "_b, " << ToString( sb.access ) << ", " << sb.binding << ", "
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
	};
	
/*
=================================================
	ToString (PipelineLayoutDescriptor)
=================================================
*/
	String  AngelScriptSerializer::ToString (StringCRef name, const PipelineLayoutDescriptor &value) const
	{
		String	str;	str << name << " = PipelineLayoutDescriptor_Builder()";
		
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
	String  AngelScriptSerializer::ToString (StringCRef name, EPrimitive::bits value) const
	{
		String	str;

		FOR( i, value )
		{
			const auto	t = EPrimitive::type(i);

			if ( not value[t] )
				continue;

			if ( not str.Empty() )
				str << " | ";

			str << ToString( t );
		}

		name >> (" = " >> str);
		str << ";\n";
		return str;
	}
	
/*
=================================================
	ToString (EShader)
=================================================
*/
	String  AngelScriptSerializer::ToString (StringCRef name, EShader::bits value) const
	{
		return String(name) << " = " << ToString( value ) << ";\n";
	}
	
/*
=================================================
	ToString (uint3)
=================================================
*/
	String	AngelScriptSerializer::ToString (StringCRef name, const uint3 &value) const
	{
		return String(name) << " = uint3(" << value.x << ", " << value.y << ", " << value.z << ");\n";
	}

/*
=================================================
	ToString (StringCRef)
=================================================
*/
	String  AngelScriptSerializer::ToString (StringCRef value) const
	{
		const usize	max_len = 16000;	// 16380 is max for MSVS

		ASSERT( not value.HasSubString( ")#" ) );

		if ( value.Length() > max_len )
		{
			String		result;
			StringCRef	temp = value;
			usize		pos  = max_len;

			while ( temp.Length() > max_len )
			{
				// find best place to separate string
				bool	found	= false;
				usize	p1		= pos;

				for (uint i = 0; i < 10; ++i)	// 10 lines
				{
					StringParser::ToPrevLine( temp, INOUT p1 );
					usize		p2 = pos+1;
					StringCRef	res;
					StringParser::ReadCurrLine( temp, INOUT p2, OUT res );

					if ( res.Empty() )
					{
						pos		= p1;
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
					StringParser::ToPrevLine( temp, INOUT pos );
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
	String  AngelScriptSerializer::ToString (EShaderVariable::type value) const
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
	String  AngelScriptSerializer::ToString (BinArrayCRef value)
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
	String  AngelScriptSerializer::ToString (ArrayCRef<uint> value)
	{
		String	str;	str.Reserve( value.Count() * 14 );

		FOR( i, value ) {
			str << (i ? ", " : "") << (i%12 == 0 ? "\n" : "") << "0x" << String().FormatAlignedI( value[i], 8, '0', 16 );
		}
		return str;
	}
	
/*
=================================================
	ToString (EShaderMemoryModel)
=================================================
*/
	String  AngelScriptSerializer::ToString (EShaderMemoryModel::type value)
	{
		switch ( value )
		{
			case EShaderMemoryModel::Default :		return "EShaderMemoryModel_Default";
			case EShaderMemoryModel::Coherent :		return "EShaderMemoryModel_Coherent";
			case EShaderMemoryModel::Volatile :		return "EShaderMemoryModel_Volatile";
			case EShaderMemoryModel::Restrict :		return "EShaderMemoryModel_Restrict";
			case EShaderMemoryModel::ReadOnly :		return "EShaderMemoryModel_ReadOnly";
			case EShaderMemoryModel::WriteOnly :	return "EShaderMemoryModel_WriteOnly";
		}
		RETURN_ERR( "not supported" );
	}

/*
=================================================
	Include
=================================================
*/
	String	AngelScriptSerializer::Include (StringCRef filename) const
	{
		return "#include \""_str << filename << "\"\n";
	}
	
/*
=================================================
	Comment
=================================================
*/
	String	AngelScriptSerializer::Comment (StringCRef comment) const
	{
		return "// "_str << comment << "\n";
	}
	
/*
=================================================
	ShaderSrcGLSL
=================================================
*/
	String	AngelScriptSerializer::ShaderSrcGLSL (StringCRef name, BinArrayCRef shaderSrc, bool inFile) const
	{
		String	str;
		if ( not shaderSrc.Empty() )
		{
			if ( inFile )
				RETURN_ERR( "not supported" )
			else
				str << name << ".StringGLSL( \n" << ToString(StringCRef( (const char*)shaderSrc.ptr(), shaderSrc.Count() )) << " );\n";
		}
		return str;
	}
	
/*
=================================================
	ShaderBinGLSL
=================================================
*/
	String	AngelScriptSerializer::ShaderBinGLSL (StringCRef name, BinArrayCRef shaderSrc, bool inFile) const
	{
		String	str;
		if ( not shaderSrc.Empty() )
		{
			if ( inFile )
				RETURN_ERR( "not supported" )
			else
				str << name << ".ArrayGLSLBin({ " << ToString(StringCRef( (const char*)shaderSrc.ptr(), shaderSrc.Count() )) << " });\n";
				//str << name << ".ArrayGLSLBin({ " << ToString(ArrayCRef<ubyte>::From( shaderSrc )) << " });\n";
		}
		return str;
	}
	
/*
=================================================
	ShaderBinSPIRV
=================================================
*/
	String	AngelScriptSerializer::ShaderBinSPIRV (StringCRef name, BinArrayCRef shaderSrc, bool inFile) const
	{
		String	str;
		if ( not shaderSrc.Empty() )
		{
			if ( inFile )
				RETURN_ERR( "not supported" )
			else
				str << name << ".ArraySPIRV({ " << ToString(ArrayCRef<uint>::From( shaderSrc )) << " });\n";
		}
		return str;
	}
	
/*
=================================================
	ShaderSrcSPIRV
=================================================
*/
	String	AngelScriptSerializer::ShaderSrcSPIRV (StringCRef name, BinArrayCRef shaderSrc, bool inFile) const
	{
		String	str;
		if ( not shaderSrc.Empty() )
		{
			if ( inFile )
				RETURN_ERR( "not supported" )
			else
				str << name << ".StringSpirvAsm( \n" << ToString(StringCRef( (const char*)shaderSrc.ptr(), shaderSrc.Count() )) << " );\n";
		}
		return str;
	}

/*
=================================================
	ShaderBinCL
=================================================
*/
	String  AngelScriptSerializer::ShaderBinCL (StringCRef name, BinArrayCRef shaderSrc, bool inFile) const
	{
		String	str;
		if ( not shaderSrc.Empty() )
		{
			if ( inFile )
				RETURN_ERR( "not supported" )
			else
				str << name << ".StringCLAsm( \n" << ToString(StringCRef( (const char*)shaderSrc.ptr(), shaderSrc.Count() )) << " );\n";
		}
		return str;
	}
	
/*
=================================================
	ShaderSrcCL
=================================================
*/
	String  AngelScriptSerializer::ShaderSrcCL (StringCRef name, BinArrayCRef shaderSrc, bool inFile) const
	{
		String	str;
		if ( not shaderSrc.Empty() )
		{
			if ( inFile )
				RETURN_ERR( "not supported" )
			else
				str << name << ".StringCL( \n" << ToString(StringCRef( (const char*)shaderSrc.ptr(), shaderSrc.Count() )) << " );\n";
		}
		return str;
	}
	
/*
=================================================
	ShaderSrcCPP
=================================================
*/
	String  AngelScriptSerializer::ShaderSrcCPP (StringCRef name, StringCRef funcName) const
	{
		RETURN_ERR( "not supported" );
	}
	
/*
=================================================
	ShaderSrcCPP_Impl
=================================================
*/
	String	AngelScriptSerializer::ShaderSrcCPP_Impl (StringCRef name, BinArrayCRef shaderSrc, StringCRef funcName) const
	{
		String	str;
		if ( not shaderSrc.Empty() )
		{
			const usize	start	= str.Length();
			usize		pos		= 0;

			str << StringCRef( (const char*)shaderSrc.ptr(), shaderSrc.Count() );

			str.FindAndChange( "##main##", funcName, OUT pos, start );
		}
		return str;
	}

/*
=================================================
	DeclVariable
=================================================
*/
	String	AngelScriptSerializer::DeclVariable (StringCRef typeName, StringCRef name, StringCRef value) const
	{
		return String(typeName) << "  " << name << (value.Empty() ? "" : " = "_str << value) << ";\n";
	}
	
/*
=================================================
	AssignVariable
=================================================
*/
	String  AngelScriptSerializer::AssignVariable (StringCRef name, StringCRef value) const
	{
		return String(name) << " = " << value << ";\n";
	}

/*
=================================================
	DeclFunction
=================================================
*/
	String	AngelScriptSerializer::DeclFunction (StringCRef result, StringCRef name, ArrayCRef<Pair<StringCRef, StringCRef>> args, bool isForwardDeclaration) const
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
	String  AngelScriptSerializer::CallFunction (StringCRef name, ArrayCRef<StringCRef> args) const
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
	String  AngelScriptSerializer::DeclNamespace (StringCRef name) const
	{
		return "namespace "_str << name << "\n";
	}

/*
=================================================
	BeginFile
=================================================
*/
	String	AngelScriptSerializer::BeginFile (bool isIncludable) const
	{
		++_fileCounter;
		return isIncludable ? "#pragma once\n" : "";
	}
	
/*
=================================================
	EndFile
=================================================
*/
	String	AngelScriptSerializer::EndFile (bool) const
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
	String	AngelScriptSerializer::BeginScope () const
	{
		++_scopeCounter;
		return "{\n";
	}
	
/*
=================================================
	EndScope
=================================================
*/
	String	AngelScriptSerializer::EndScope () const
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
	String	AngelScriptSerializer::BeginStruct (StringCRef typeName, uint sizeOf) const
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
	String	AngelScriptSerializer::StructField (StringCRef name, StringCRef typeName, uint arraySize, uint offset, uint align, uint sizeOf) const
	{
		String	indent;		indent.Resize( _structStack.Count()-2, '\t' );

		_structStack.Get().second << indent << "\t\tSTATIC_ASSERT( (offsetof( " << _structStack.Get().first << ", " << name << " ) == "
								  << offset << ") and (sizeof( " << name << " ) == " << sizeOf << ") );\n";

		String	type_name = typeName;

		// TODO: use Bool type...
		if ( typeName.StartsWith("bool") )
		{
			if ( typeName.Length() == 4 )	type_name = "Bool32";								else
			if ( typeName.Length() == 5 )	type_name = "Bool32_"_str << typeName.SubString( 4 );
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
	StructCtorForInitializerList
=================================================
*/
	String  AngelScriptSerializer::StructCtorForInitializerList () const
	{
		TODO( "" );
		return "";
	}

/*
=================================================
	EndStruct
=================================================
*/
	String	AngelScriptSerializer::EndStruct () const
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
	String  AngelScriptSerializer::ToString (EImage::type value)
	{
		switch ( value )
		{
			case EImage::Tex1D :			return "EImage_Tex1D";
			case EImage::Tex1DArray :		return "EImage_Tex1DArray";
			case EImage::Tex2D :			return "EImage_Tex2D";
			case EImage::Tex2DArray :		return "EImage_Tex2DArray";
			case EImage::Tex2DMS :			return "EImage_Tex2DMS";
			case EImage::Tex2DMSArray :		return "EImage_Tex2DMSArray";
			case EImage::TexCube :			return "EImage_TexCube";
			case EImage::TexCubeArray :		return "EImage_TexCubeArray";
			case EImage::Tex3D :			return "EImage_Tex3D";
			case EImage::Buffer :			return "EImage_Buffer";
		}
		RETURN_ERR( "unsupported texture type!" );
	}

/*
=================================================
	ToString (EShader)
=================================================
*/
	String  AngelScriptSerializer::ToString (EShader::bits value)
	{
		String str;

		FOR( i, value )
		{
			const auto	t = EShader::type(i);

			if ( not value[t] )
				continue;

			if ( not str.Empty() )
				str << " | ";

			switch ( t )
			{
				case EShader::Vertex :			str << "EShader_Vertex";			break;
				case EShader::TessControl :		str << "EShader_TessControl";		break;
				case EShader::TessEvaluation :	str << "EShader_TessEvaluation";	break;
				case EShader::Geometry :		str << "EShader_Geometry";			break;
				case EShader::Fragment :		str << "EShader_Fragment";			break;
				case EShader::Compute :			str << "EShader_Compute";			break;
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
	String  AngelScriptSerializer::ToString (EPixelFormatClass::type value)
	{
		if ( EnumEq( value, EPixelFormatClass::Any ) )
			return "EPixelFormatClass_Any";

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
				case EPixelFormatClass::R :						str << "EPixelFormatClass_R";						break;
				case EPixelFormatClass::RG :					str << "EPixelFormatClass_RG";						break;
				case EPixelFormatClass::RGB :					str << "EPixelFormatClass_RGB";						break;
				case EPixelFormatClass::RGBA :					str << "EPixelFormatClass_RGBA";					break;
				case EPixelFormatClass::AnyColorChannels :		str << "EPixelFormatClass_AnyColorChannels";		break;
				case EPixelFormatClass::Depth :					str << "EPixelFormatClass_Depth";					break;
				case EPixelFormatClass::DepthStencil :			str << "EPixelFormatClass_DepthStencil";			break;
				case EPixelFormatClass::AnyDepthStencil :		str << "EPixelFormatClass_AnyDepthStencil";			break;
				case EPixelFormatClass::AnyChannel :			str << "EPixelFormatClass_AnyChannel";				break;
				case EPixelFormatClass::LinearColorSpace :		str << "EPixelFormatClass_LinearColorSpace";		break;
				case EPixelFormatClass::NonLinearColorSpace :	str << "EPixelFormatClass_NonLinearColorSpace";		break;
				case EPixelFormatClass::AnyColorSpace :			str << "EPixelFormatClass_AnyColorSpace";			break;
				case EPixelFormatClass::Float64 :				str << "EPixelFormatClass_Float64";					break;
				case EPixelFormatClass::Float32 :				str << "EPixelFormatClass_Float32";					break;
				case EPixelFormatClass::Float16 :				str << "EPixelFormatClass_Float16";					break;
				case EPixelFormatClass::AnyFloat :				str << "EPixelFormatClass_AnyFloat";				break;
				case EPixelFormatClass::Int32 :					str << "EPixelFormatClass_Int32";					break;
				case EPixelFormatClass::Int16 :					str << "EPixelFormatClass_Int16";					break;
				case EPixelFormatClass::Int8 :					str << "EPixelFormatClass_Int8";					break;
				case EPixelFormatClass::PackedInt :				str << "EPixelFormatClass_PackedInt";				break;
				case EPixelFormatClass::AnyInt :				str << "EPixelFormatClass_AnyInt";					break;
				case EPixelFormatClass::UInt32 :				str << "EPixelFormatClass_UInt32";					break;
				case EPixelFormatClass::UInt16 :				str << "EPixelFormatClass_UInt16";					break;
				case EPixelFormatClass::UInt8 :					str << "EPixelFormatClass_UInt8";					break;
				case EPixelFormatClass::PackedUInt :			str << "EPixelFormatClass_PackedUInt";				break;
				case EPixelFormatClass::AnyUInt :				str << "EPixelFormatClass_AnyUInt";					break;
				case EPixelFormatClass::AnyInteger :			str << "EPixelFormatClass_AnyInteger";				break;
				case EPixelFormatClass::UNorm16 :				str << "EPixelFormatClass_UNorm16";					break;
				case EPixelFormatClass::UNorm8 :				str << "EPixelFormatClass_UNorm8";					break;
				case EPixelFormatClass::PackedUNorm :			str << "EPixelFormatClass_PackedUNorm";				break;
				case EPixelFormatClass::AnyUNorm :				str << "EPixelFormatClass_AnyUNorm";				break;
				case EPixelFormatClass::SNorm16 :				str << "EPixelFormatClass_SNorm16";					break;
				case EPixelFormatClass::SNorm8 :				str << "EPixelFormatClass_SNorm8";					break;
				case EPixelFormatClass::PackedSNorm :			str << "EPixelFormatClass_PackedSNorm";				break;
				case EPixelFormatClass::AnySNorm :				str << "EPixelFormatClass_AnySNorm";				break;
				case EPixelFormatClass::AnyNorm :				str << "EPixelFormatClass_AnyNorm";					break;
				case EPixelFormatClass::AnyType :				str << "EPixelFormatClass_AnyType";					break;
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
	String  AngelScriptSerializer::ToString (EPixelFormat::type value)
	{
		switch ( value )
		{
			case EPixelFormat::RGBA16_SNorm :				return "EPixelFormat_RGBA16_SNorm";
			case EPixelFormat::RGBA8_SNorm :				return "EPixelFormat_RGBA8_SNorm";
			case EPixelFormat::RGB16_SNorm :				return "EPixelFormat_RGB16_SNorm";
			case EPixelFormat::RGB8_SNorm :					return "EPixelFormat_RGB8_SNorm";
			case EPixelFormat::RG16_SNorm :					return "EPixelFormat_RG16_SNorm";
			case EPixelFormat::RG8_SNorm :					return "EPixelFormat_RG8_SNorm";
			case EPixelFormat::R16_SNorm :					return "EPixelFormat_R16_SNorm";
			case EPixelFormat::R8_SNorm :					return "EPixelFormat_R8_SNorm";
			case EPixelFormat::RGBA16_UNorm :				return "EPixelFormat_RGBA16_UNorm";
			case EPixelFormat::RGBA8_UNorm :				return "EPixelFormat_RGBA8_UNorm";
			case EPixelFormat::RGB16_UNorm :				return "EPixelFormat_RGB16_UNorm";
			case EPixelFormat::RGB8_UNorm :					return "EPixelFormat_RGB8_UNorm";
			case EPixelFormat::RG16_UNorm :					return "EPixelFormat_RG16_UNorm";
			case EPixelFormat::RG8_UNorm :					return "EPixelFormat_RG8_UNorm";
			case EPixelFormat::R16_UNorm :					return "EPixelFormat_R16_UNorm";
			case EPixelFormat::R8_UNorm :					return "EPixelFormat_R8_UNorm";
			case EPixelFormat::RGB10_A2_UNorm :				return "EPixelFormat_RGB10_A2_UNorm";
			case EPixelFormat::RGBA4_UNorm :				return "EPixelFormat_RGBA4_UNorm";
			case EPixelFormat::RGB5_A1_UNorm :				return "EPixelFormat_RGB5_A1_UNorm";
			case EPixelFormat::RGB_5_6_5_UNorm :			return "EPixelFormat_RGB_5_6_5_UNorm";
			case EPixelFormat::BGR8_UNorm :					return "EPixelFormat_BGR8_UNorm";
			case EPixelFormat::BGRA8_UNorm :				return "EPixelFormat_BGRA8_UNorm";
			case EPixelFormat::sRGB8 :						return "EPixelFormat_sRGB8";
			case EPixelFormat::sRGB8_A8 :					return "EPixelFormat_sRGB8_A8";
			case EPixelFormat::R8I :						return "EPixelFormat_R8I";
			case EPixelFormat::RG8I :						return "EPixelFormat_RG8I";
			case EPixelFormat::RGB8I :						return "EPixelFormat_RGB8I";
			case EPixelFormat::RGBA8I :						return "EPixelFormat_RGBA8I";
			case EPixelFormat::R16I :						return "EPixelFormat_R16I";
			case EPixelFormat::RG16I :						return "EPixelFormat_RG16I";
			case EPixelFormat::RGB16I :						return "EPixelFormat_RGB16I";
			case EPixelFormat::RGBA16I :					return "EPixelFormat_RGBA16I";
			case EPixelFormat::R32I :						return "EPixelFormat_R32I";
			case EPixelFormat::RG32I :						return "EPixelFormat_RG32I";
			case EPixelFormat::RGB32I :						return "EPixelFormat_RGB32I";
			case EPixelFormat::RGBA32I :					return "EPixelFormat_RGBA32I";
			case EPixelFormat::R8U :						return "EPixelFormat_R8U";
			case EPixelFormat::RG8U :						return "EPixelFormat_RG8U";
			case EPixelFormat::RGB8U :						return "EPixelFormat_RGB8U";
			case EPixelFormat::RGBA8U :						return "EPixelFormat_RGBA8U";
			case EPixelFormat::R16U :						return "EPixelFormat_R16U";
			case EPixelFormat::RG16U :						return "EPixelFormat_RG16U";
			case EPixelFormat::RGB16U :						return "EPixelFormat_RGB16U";
			case EPixelFormat::RGBA16U :					return "EPixelFormat_RGBA16U";
			case EPixelFormat::R32U :						return "EPixelFormat_R32U";
			case EPixelFormat::RG32U :						return "EPixelFormat_RG32U";
			case EPixelFormat::RGB32U :						return "EPixelFormat_RGB32U";
			case EPixelFormat::RGBA32U :					return "EPixelFormat_RGBA32U";
			case EPixelFormat::RGB10_A2U :					return "EPixelFormat_RGB10_A2U";
			case EPixelFormat::R16F :						return "EPixelFormat_R16F";
			case EPixelFormat::RG16F :						return "EPixelFormat_RG16F";
			case EPixelFormat::RGB16F :						return "EPixelFormat_RGB16F";
			case EPixelFormat::RGBA16F :					return "EPixelFormat_RGBA16F";
			case EPixelFormat::R32F :						return "EPixelFormat_R32F";
			case EPixelFormat::RG32F :						return "EPixelFormat_RG32F";
			case EPixelFormat::RGB32F :						return "EPixelFormat_RGB32F";
			case EPixelFormat::RGBA32F :					return "EPixelFormat_RGBA32F";
			case EPixelFormat::RGB_11_11_10F :				return "EPixelFormat_RGB_11_11_10F";
			case EPixelFormat::Depth16 :					return "EPixelFormat_Depth16";
			case EPixelFormat::Depth24 :					return "EPixelFormat_Depth24";
			case EPixelFormat::Depth32 :					return "EPixelFormat_Depth32";
			case EPixelFormat::Depth32F :					return "EPixelFormat_Depth32F";
			case EPixelFormat::Depth16_Stencil8 :			return "EPixelFormat_Depth16_Stencil8";
			case EPixelFormat::Depth24_Stencil8 :			return "EPixelFormat_Depth24_Stencil8";
			case EPixelFormat::Depth32F_Stencil8 :			return "EPixelFormat_Depth32F_Stencil8";
			case EPixelFormat::BC1_RGB8_UNorm :				return "EPixelFormat_BC1_RGB8_UNorm";
			case EPixelFormat::BC1_RGB8_A1_UNorm :			return "EPixelFormat_BC1_RGB8_A1_UNorm";
			case EPixelFormat::BC2_RGBA8_UNorm :			return "EPixelFormat_BC2_RGBA8_UNorm";
			case EPixelFormat::BC3_RGBA8_UNorm :			return "EPixelFormat_BC3_RGBA8_UNorm";
			case EPixelFormat::BC4_RED8_SNorm :				return "EPixelFormat_BC4_RED8_SNorm";
			case EPixelFormat::BC4_RED8_UNorm :				return "EPixelFormat_BC4_RED8_UNorm";
			case EPixelFormat::BC5_RG8_SNorm :				return "EPixelFormat_BC5_RG8_SNorm";
			case EPixelFormat::BC5_RG8_UNorm :				return "EPixelFormat_BC5_RG8_UNorm";
			case EPixelFormat::BC7_RGBA8_UNorm :			return "EPixelFormat_BC7_RGBA8_UNorm";
			case EPixelFormat::BC7_SRGB8_A8_UNorm :			return "EPixelFormat_BC7_SRGB8_A8_UNorm";
			case EPixelFormat::BC6H_RGB16F :				return "EPixelFormat_BC6H_RGB16F";
			case EPixelFormat::BC6H_RGB16F_Unsigned :		return "EPixelFormat_BC6H_RGB16F_Unsigned";
			case EPixelFormat::ETC2_RGB8_UNorm :			return "EPixelFormat_ETC2_RGB8_UNorm";
			case EPixelFormat::ECT2_SRGB8_UNorm :			return "EPixelFormat_ECT2_SRGB8_UNorm";
			case EPixelFormat::ETC2_RGB8_A1_UNorm :			return "EPixelFormat_ETC2_RGB8_A1_UNorm";
			case EPixelFormat::ETC2_SRGB8_A1_UNorm :		return "EPixelFormat_ETC2_SRGB8_A1_UNorm";
			case EPixelFormat::ETC2_RGBA8_UNorm :			return "EPixelFormat_ETC2_RGBA8_UNorm";
			case EPixelFormat::ETC2_SRGB8_A8_UNorm :		return "EPixelFormat_ETC2_SRGB8_A8_UNorm";
			case EPixelFormat::EAC_R11_SNorm :				return "EPixelFormat_EAC_R11_SNorm";
			case EPixelFormat::EAC_R11_UNorm :				return "EPixelFormat_EAC_R11_UNorm";
			case EPixelFormat::EAC_RG11_SNorm :				return "EPixelFormat_EAC_RG11_SNorm";
			case EPixelFormat::EAC_RG11_UNorm :				return "EPixelFormat_EAC_RG11_UNorm";
			case EPixelFormat::ASTC_RGBA_4x4 :				return "EPixelFormat_ASTC_RGBA_4x4";
			case EPixelFormat::ASTC_RGBA_5x4 :				return "EPixelFormat_ASTC_RGBA_5x4";
			case EPixelFormat::ASTC_RGBA_5x5 :				return "EPixelFormat_ASTC_RGBA_5x5";
			case EPixelFormat::ASTC_RGBA_6x5 :				return "EPixelFormat_ASTC_RGBA_6x5";
			case EPixelFormat::ASTC_RGBA_6x6 :				return "EPixelFormat_ASTC_RGBA_6x6";
			case EPixelFormat::ASTC_RGBA_8x5 :				return "EPixelFormat_ASTC_RGBA_8x5";
			case EPixelFormat::ASTC_RGBA_8x6 :				return "EPixelFormat_ASTC_RGBA_8x6";
			case EPixelFormat::ASTC_RGBA_8x8 :				return "EPixelFormat_ASTC_RGBA_8x8";
			case EPixelFormat::ASTC_RGBA_10x5 :				return "EPixelFormat_ASTC_RGBA_10x5";
			case EPixelFormat::ASTC_RGBA_10x6 :				return "EPixelFormat_ASTC_RGBA_10x6";
			case EPixelFormat::ASTC_RGBA_10x8 :				return "EPixelFormat_ASTC_RGBA_10x8";
			case EPixelFormat::ASTC_RGBA_10x10 :			return "EPixelFormat_ASTC_RGBA_10x10";
			case EPixelFormat::ASTC_RGBA_12x10 :			return "EPixelFormat_ASTC_RGBA_12x10";
			case EPixelFormat::ASTC_RGBA_12x12 :			return "EPixelFormat_ASTC_RGBA_12x12";
			case EPixelFormat::ASTC_SRGB8_A8_4x4 :			return "EPixelFormat_ASTC_SRGB8_A8_4x4";
			case EPixelFormat::ASTC_SRGB8_A8_5x4 :			return "EPixelFormat_ASTC_SRGB8_A8_5x4";
			case EPixelFormat::ASTC_SRGB8_A8_5x5 :			return "EPixelFormat_ASTC_SRGB8_A8_5x5";
			case EPixelFormat::ASTC_SRGB8_A8_6x5 :			return "EPixelFormat_ASTC_SRGB8_A8_6x5";
			case EPixelFormat::ASTC_SRGB8_A8_6x6 :			return "EPixelFormat_ASTC_SRGB8_A8_6x6";
			case EPixelFormat::ASTC_SRGB8_A8_8x5 :			return "EPixelFormat_ASTC_SRGB8_A8_8x5";
			case EPixelFormat::ASTC_SRGB8_A8_8x6 :			return "EPixelFormat_ASTC_SRGB8_A8_8x6";
			case EPixelFormat::ASTC_SRGB8_A8_8x8 :			return "EPixelFormat_ASTC_SRGB8_A8_8x8";
			case EPixelFormat::ASTC_SRGB8_A8_10x5 :			return "EPixelFormat_ASTC_SRGB8_A8_10x5";
			case EPixelFormat::ASTC_SRGB8_A8_10x6 :			return "EPixelFormat_ASTC_SRGB8_A8_10x6";
			case EPixelFormat::ASTC_SRGB8_A8_10x8 :			return "EPixelFormat_ASTC_SRGB8_A8_10x8";
			case EPixelFormat::ASTC_SRGB8_A8_10x10 :		return "EPixelFormat_ASTC_SRGB8_A8_10x10";
			case EPixelFormat::ASTC_SRGB8_A8_12x10 :		return "EPixelFormat_ASTC_SRGB8_A8_12x10";
			case EPixelFormat::ASTC_SRGB8_A8_12x12 :		return "EPixelFormat_ASTC_SRGB8_A8_12x12";
		}

		RETURN_ERR( "unsupported pixel format", "EPixelFormat::type("_str << uint(value) << ")" );
	}

}	// PipelineCompiler
