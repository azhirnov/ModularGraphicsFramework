// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Engine/ResourcePacker/Pipelines/ScriptPipeline.h"

namespace ResPack
{
	using namespace GXScript;
	
/*
=================================================
	BindAll
=================================================
*/
	void ScriptPipeline::BindAll (ScriptEnginePtr se)
	{
		_Bind_RenderState( se );
		_Bind_Shaders( se );
		_Bind_ConverterConfig( se );
	}

/*
=================================================
	_Bind_Shaders
=================================================
*/
	void ScriptPipeline::_Bind_Shaders (ScriptEnginePtr se)
	{
		_Bind_ShaderModule( se );

		ClassBinder< Shaders >	binder{ se };

		binder.CreateClassValue();
		binder.AddProperty( &Shaders::vertex,			"vertex" );
		binder.AddProperty( &Shaders::tessControl,		"tessControl" );
		binder.AddProperty( &Shaders::tessEvaluation,	"tessEvaluation" );
		binder.AddProperty( &Shaders::geometry,			"geometry" );
		binder.AddProperty( &Shaders::fragment,			"fragment" );
	}
	
/*
=================================================
	_Bind_ConverterConfig
=================================================
*/
	void ScriptPipeline::_Bind_ConverterConfig (ScriptEnginePtr se)
	{
		struct ConverterConfigUtils {
			static void Include (ConverterConfig &self, const String &path) {
				self.includings.PushBack( String(path) );
			}

			static void SetDefaults (ConverterConfig &self) {
				self.searchForSharedTypes	= true;
				self.addPaddingToStructs	= true;
				self.optimizeSource			= false;
				self.optimizeBindings		= true;
				self.includings				<< "common.h";
				self.nameSpace				= "Pipelines";
				self.target					|= EShaderDstFormat::GLSL_Source;
				self.target					|= EShaderDstFormat::SPIRV_Binary;
				self.target					|= EShaderDstFormat::CL_Source;
				self.target					|= EShaderDstFormat::CPP_Module;
			}
		};

		_Bind_EShaderDstFormat( se );

		ClassBinder< ConverterConfig >	binder{ se };

		binder.CreateClassValue();
		binder.AddProperty( &ConverterConfig::target,				"targets" );
		binder.AddProperty( &ConverterConfig::errorIfFileExist,		"errorIfFileExist" );
		binder.AddProperty( &ConverterConfig::optimizeSource,		"optimizeSource" );
		binder.AddProperty( &ConverterConfig::searchForSharedTypes,	"searchForSharedTypes" );
		binder.AddProperty( &ConverterConfig::addPaddingToStructs,	"addPaddingToStructs" );
		binder.AddProperty( &ConverterConfig::optimizeBindings,		"optimizeBindings" );
		binder.AddProperty( &ConverterConfig::obfuscate,			"obfuscate" );
		binder.AddProperty( &ConverterConfig::validation,			"validation" );
		binder.AddProperty( &ConverterConfig::nameSpace,			"nameSpace" );

		binder.AddMethodFromGlobal( &ConverterConfigUtils::Include,		"Include" );
		binder.AddMethodFromGlobal( &ConverterConfigUtils::SetDefaults,	"SetDefaults" );
	}

/*
=================================================
	_Bind_ShaderModule
=================================================
*/
	void ScriptPipeline::_Bind_ShaderModule (ScriptEnginePtr se)
	{
		struct ShaderModuleFunc {
			static void AddSource (ShaderModule *module, const String &src) {
				module->Source( src );
			}

			static void Load (ShaderModule *module, const String &src) {
				module->Load( src );
			}

			static void LoadSelf (ShaderModule *module) {
				StringCRef		fname	= ScriptHelper::CurrentFileName();
				File::RFilePtr	file	= File::HddRFile::New( fname );
				const usize		len		= usize(file->RemainingSize());
				String			src;	src.Resize( len );

				CHECK_ERR( file->Read( src.ptr(), src.LengthInBytes() ), void() );
				CHECK_ERR( not src.Empty(), void() );

				module->Source( src );
			}
		};

		ClassBinder< ShaderModule >	binder{ se };
		
		binder.CreateClassValue();
		binder.AddMethodFromGlobal( &ShaderModuleFunc::AddSource,	"Source" );
		binder.AddMethodFromGlobal( &ShaderModuleFunc::Load,		"Load" );
		binder.AddMethodFromGlobal( &ShaderModuleFunc::LoadSelf,	"LoadSelf" );
	}

/*
=================================================
	_Bind_RenderState
=================================================
*/
	void ScriptPipeline::_Bind_RenderState (ScriptEnginePtr se)
	{
		_Bind_EShaderSrcFormat( se );
		_Bind_EPrimitive( se );
		_Bind_EPipelineDynamicState( se );
		_Bind_EBlendFunc( se);
		_Bind_EBlendEq( se );
		_Bind_ELogicOp( se );
		_Bind_EStencilOp( se );
		_Bind_ECompareFunc( se );
		_Bind_OptionalFloat2( se );
		_Bind_EPolygonMode( se );
		_Bind_EPolygonFace( se );
		_Bind_SampleMask( se );
		_Bind_MultiSamples( se );
		_Bind_EBlendFuncPair( se );
		_Bind_EBlendEqPair( se );
		_Bind_UNormClampedF( se );
		_Bind_ColorBuffer( se );
		_Bind_StencilFaceState( se );

		_Bind_ColorBuffersState( se );
		_Bind_DepthBufferState( se );
		_Bind_StencilBufferState( se );
		_Bind_InputAssemblyState( se );
		_Bind_RasterizationState( se );
		_Bind_MultisampleState( se );

		using RS = RenderState;

		ClassBinder< RS >	binder{ se };
		
		binder.CreateClassValue();
		binder.AddProperty( &RS::color,			"color" );
		binder.AddProperty( &RS::depth,			"depth" );
		binder.AddProperty( &RS::stencil,		"stencil" );
		binder.AddProperty( &RS::inputAssembly,	"inputAssembly" );
		binder.AddProperty( &RS::rasterization,	"rasterization" );
		binder.AddProperty( &RS::multisample,	"multisample" );
	}
	
/*
=================================================
	_Bind_ColorBuffer
=================================================
*/
	void ScriptPipeline::_Bind_ColorBuffer (ScriptEnginePtr se)
	{
		using CB = RenderState::ColorBuffer;

		ClassBinder< CB >	binder{ se };

		binder.CreateClassValue();
		binder.AddProperty( &CB::blendFuncSrc,	"blendFuncSrc" );
		binder.AddProperty( &CB::blendFuncDst,	"blendFuncDst" );
		binder.AddProperty( &CB::blendMode,		"blendMode" );
		//binder.AddProperty( &CB::colorMask,		"colorMask" );
		binder.AddProperty( &CB::blend,			"blend" );
	}

/*
=================================================
	_Bind_ColorBuffersState
=================================================
*/
	void ScriptPipeline::_Bind_ColorBuffersState (ScriptEnginePtr se)
	{
		using CB = RenderState::ColorBuffersState;

		ClassBinder< CB >	binder{ se };
		CB					temp;

		binder.CreateClassValue();
		//binder.AddProperty( &CB::blendColor,	"blendColor" );
		binder.AddProperty( &CB::logicOp,		"logicOp" );
		//binder.AddProperty( &CB::buffers,		"buffers" );

		for (usize i = 0; i < CB::ColorBuffers_t::STATIC_COUNT; ++i)
		{
			binder.AddProperty( temp, temp.buffers[i], "buffer"_str << i );
		}

		binder.AddMethod( &CB::SetBlend,				"SetBlend" );
		binder.AddMethod( &CB::SetBlendFuncDst,			"SetBlendFuncDst" );
		binder.AddMethod( &CB::SetBlendFuncDstAlpha,	"SetBlendFuncDstAlpha" );
		binder.AddMethod( &CB::SetBlendFuncDstColor,	"SetBlendFuncDstColor" );
		binder.AddMethod( &CB::SetBlendFuncSrc,			"SetBlendFuncSrc" );
		binder.AddMethod( &CB::SetBlendFuncSrcAlpha,	"SetBlendFuncSrcAlpha" );
		binder.AddMethod( &CB::SetBlendFuncSrcColor,	"SetBlendFuncSrcColor" );
		binder.AddMethod( &CB::SetBlendMode,			"SetBlendMode" );
		binder.AddMethod( &CB::SetBlendModeAlpha,		"SetBlendModeAlpha" );
		binder.AddMethod( &CB::SetBlendModeColor,		"SetBlendModeColor" );
		//binder.AddMethod( &CB::SetColorMask,			"SetColorMask" );
	}
	
/*
=================================================
	_Bind_DepthBufferState
=================================================
*/
	void ScriptPipeline::_Bind_DepthBufferState (ScriptEnginePtr se)
	{
		using DB = RenderState::DepthBufferState;

		ClassBinder< DB >	binder{ se };
		
		binder.CreateClassValue();
		binder.AddProperty( &DB::func,		"func" );
		binder.AddProperty( &DB::range,		"range" );
		binder.AddProperty( &DB::write,		"write" );
		binder.AddProperty( &DB::enabled,	"enabled" );
	}
	
/*
=================================================
	_Bind_StencilFaceState
=================================================
*/
	void ScriptPipeline::_Bind_StencilFaceState (ScriptEnginePtr se)
	{
		using SF = RenderState::StencilFaceState;

		ClassBinder< SF >	binder{ se };
		
		binder.CreateClassValue();
		binder.AddProperty( &SF::sfail,		"sfail" );
		binder.AddProperty( &SF::dfail,		"dfail" );
		binder.AddProperty( &SF::dppass,	"dppass" );
		binder.AddProperty( &SF::func,		"func" );
		binder.AddProperty( &SF::funcRef,	"funcRef" );
		binder.AddProperty( &SF::funcMask,	"funcMask" );
		binder.AddProperty( &SF::mask,		"mask" );
	}
	
/*
=================================================
	_Bind_StencilBufferState
=================================================
*/
	void ScriptPipeline::_Bind_StencilBufferState (ScriptEnginePtr se)
	{
		using SB = RenderState::StencilBufferState;

		ClassBinder< SB >	binder{ se };
		
		binder.CreateClassValue();
		binder.AddProperty( &SB::front,		"front" );
		binder.AddProperty( &SB::back,		"back" );
		binder.AddProperty( &SB::enabled,	"enabled" );
	}
	
/*
=================================================
	_Bind_InputAssemblyState
=================================================
*/
	void ScriptPipeline::_Bind_InputAssemblyState (ScriptEnginePtr se)
	{
		using IA = RenderState::InputAssemblyState;

		ClassBinder< IA >	binder{ se };
		
		binder.CreateClassValue();
		binder.AddProperty( &IA::topology,			"topology" );
		binder.AddProperty( &IA::primitiveRestart,	"primitiveRestart" );
	}
	
/*
=================================================
	_Bind_RasterizationState
=================================================
*/
	void ScriptPipeline::_Bind_RasterizationState (ScriptEnginePtr se)
	{
		using RS = RenderState::RasterizationState;

		ClassBinder< RS >	binder{ se };
		
		binder.CreateClassValue();
		binder.AddProperty( &RS::polygonMode,			"polygonMode" );

		binder.AddProperty( &RS::lineWidth,				"lineWidth" );
		
		binder.AddProperty( &RS::depthBiasConstFactor,	"depthBiasConstFactor" );
		binder.AddProperty( &RS::depthBiasClamp,		"depthBiasClamp" );
		binder.AddProperty( &RS::depthBiasSlopeFactor,	"depthBiasSlopeFactor" );
		binder.AddProperty( &RS::depthBias,				"depthBias" );

		binder.AddProperty( &RS::depthClamp,			"depthClamp" );
		binder.AddProperty( &RS::rasterizerDiscard,		"rasterizerDiscard" );

		binder.AddProperty( &RS::cullMode,				"cullMode" );
		binder.AddProperty( &RS::frontFaceCCW,			"frontFaceCCW" );
	}
	
/*
=================================================
	_Bind_MultisampleState
=================================================
*/
	void ScriptPipeline::_Bind_MultisampleState (ScriptEnginePtr se)
	{
		using MS = RenderState::MultisampleState;

		ClassBinder< MS >	binder{ se };
		
		binder.CreateClassValue();
		binder.AddProperty( &MS::sampleMask,		"sampleMask" );
		binder.AddProperty( &MS::samples,			"samples" );

		binder.AddProperty( &MS::minSampleShading,	"minSampleShading" );
		binder.AddProperty( &MS::sampleShading,		"sampleShading" );

		binder.AddProperty( &MS::alphaToCoverage,	"alphaToCoverage" );
		binder.AddProperty( &MS::alphaToOne,		"alphaToOne" );
	}
	
/*
=================================================
	_Bind_EShaderSrcFormat
=================================================
*/
	void ScriptPipeline::_Bind_EShaderSrcFormat (ScriptEnginePtr se)
	{
		EnumBinder< EShaderSrcFormat::type >	binder{ se };
		
		binder.Create();
		binder.AddValue( "GLSL", EShaderSrcFormat::GLSL );
		binder.AddValue( "GXSL", EShaderSrcFormat::GXSL );
	}
	
/*
=================================================
	_Bind_EPrimitive
=================================================
*/
	void ScriptPipeline::_Bind_EPrimitive (ScriptEnginePtr se)
	{
		// enum
		{
			EnumBinder< EPrimitive::type >	binder{ se };
		
			binder.Create();
			binder.AddValue( "Point",			EPrimitive::Point );
			binder.AddValue( "LineList",		EPrimitive::LineList );
			binder.AddValue( "LineStrip",		EPrimitive::LineStrip );
			binder.AddValue( "TriangleList",	EPrimitive::TriangleList );
			binder.AddValue( "TriangleStrip",	EPrimitive::TriangleStrip );
			binder.AddValue( "Patch",			EPrimitive::Patch );
		}

		// bitfield
		{
			EnumBitfieldBinder< EPrimitive::bits >	binder{ se };

			binder.BindDefaults();
		}
	}
	
/*
=================================================
	_Bind_EPipelineDynamicState
=================================================
*/
	void ScriptPipeline::_Bind_EPipelineDynamicState (ScriptEnginePtr se)
	{
		// enum
		{
			EnumBinder< EPipelineDynamicState::type >	binder{ se };
		
			binder.Create();
			binder.AddValue( "Viewport",			EPipelineDynamicState::Viewport );
			binder.AddValue( "Scissor",				EPipelineDynamicState::Scissor );
			binder.AddValue( "LineWidth",			EPipelineDynamicState::LineWidth );
			binder.AddValue( "DepthBias",			EPipelineDynamicState::DepthBias );
			binder.AddValue( "BlendConstants",		EPipelineDynamicState::BlendConstants );
			binder.AddValue( "DepthBounds",			EPipelineDynamicState::DepthBounds );
			binder.AddValue( "StencilCompareMask",	EPipelineDynamicState::StencilCompareMask );
			binder.AddValue( "StencilWriteMask",	EPipelineDynamicState::StencilWriteMask );
			binder.AddValue( "StencilReference",	EPipelineDynamicState::StencilReference );
		}

		// bitfield
		{
			EnumBitfieldBinder< EPipelineDynamicState::bits >	binder{ se };

			binder.BindDefaults();
		}
	}
	
/*
=================================================
	_Bind_EBlendFunc
=================================================
*/
	void ScriptPipeline::_Bind_EBlendFunc (ScriptEnginePtr se)
	{
		EnumBinder< EBlendFunc::type >	binder{ se };
		
		binder.Create();
		binder.AddValue( "Zero",				EBlendFunc::Zero );
		binder.AddValue( "One",					EBlendFunc::One );
		binder.AddValue( "SrcColor",			EBlendFunc::SrcColor );
		binder.AddValue( "OneMinusSrcColor",	EBlendFunc::OneMinusSrcColor );
		binder.AddValue( "DstColor",			EBlendFunc::DstColor );
		binder.AddValue( "OneMinusDstColor",	EBlendFunc::OneMinusDstColor );
		binder.AddValue( "SrcAlpha",			EBlendFunc::SrcAlpha );
		binder.AddValue( "OneMinusSrcAlpha",	EBlendFunc::OneMinusSrcAlpha );
		binder.AddValue( "DstAlpha",			EBlendFunc::DstAlpha );
		binder.AddValue( "OneMinusDstAlpha",	EBlendFunc::OneMinusDstAlpha );
		binder.AddValue( "ConstColor",			EBlendFunc::ConstColor );
		binder.AddValue( "OneMinusConstColor",	EBlendFunc::OneMinusConstColor );
		binder.AddValue( "ConstAlpha",			EBlendFunc::ConstAlpha );
		binder.AddValue( "OneMinusConstAlpha",	EBlendFunc::OneMinusConstAlpha );
		binder.AddValue( "SrcAlphaSaturate",	EBlendFunc::SrcAlphaSaturate );
	}
	
/*
=================================================
	_Bind_EBlendEq
=================================================
*/
	void ScriptPipeline::_Bind_EBlendEq (ScriptEnginePtr se)
	{
		EnumBinder< EBlendEq::type >	binder{ se };
		
		binder.Create();
		binder.AddValue( "Add",		EBlendEq::Add );
		binder.AddValue( "Sub",		EBlendEq::Sub );
		binder.AddValue( "RevSub",	EBlendEq::RevSub );
		binder.AddValue( "Min",		EBlendEq::Min );
		binder.AddValue( "Max",		EBlendEq::Max );
	}
	
/*
=================================================
	_Bind_ELogicOp
=================================================
*/
	void ScriptPipeline::_Bind_ELogicOp (ScriptEnginePtr se)
	{
		EnumBinder< ELogicOp::type >	binder{ se };
		
		binder.Create();
		binder.AddValue( "None",			ELogicOp::None );
		binder.AddValue( "Clear",			ELogicOp::Clear );
		binder.AddValue( "Set",				ELogicOp::Set );
		binder.AddValue( "Copy",			ELogicOp::Copy );
		binder.AddValue( "CopyInverted",	ELogicOp::CopyInverted );
		binder.AddValue( "Noop",			ELogicOp::Noop );
		binder.AddValue( "Invert",			ELogicOp::Invert );
		binder.AddValue( "And",				ELogicOp::And );
		binder.AddValue( "NotAnd",			ELogicOp::NotAnd );
		binder.AddValue( "Or",				ELogicOp::Or );
		binder.AddValue( "NotOr",			ELogicOp::NotOr );
		binder.AddValue( "Xor",				ELogicOp::Xor );
		binder.AddValue( "Equiv",			ELogicOp::Equiv );
		binder.AddValue( "AndReverse",		ELogicOp::AndReverse );
		binder.AddValue( "AndInverted",		ELogicOp::AndInverted );
		binder.AddValue( "OrReverse",		ELogicOp::OrReverse );
		binder.AddValue( "OrInverted",		ELogicOp::OrInverted );
	}
	
/*
=================================================
	_Bind_EStencilOp
=================================================
*/
	void ScriptPipeline::_Bind_EStencilOp (ScriptEnginePtr se)
	{
		EnumBinder< EStencilOp::type >	binder{ se };
		
		binder.Create();
		binder.AddValue( "Keep",		EStencilOp::Keep );
		binder.AddValue( "Zero",		EStencilOp::Zero );
		binder.AddValue( "Replace",		EStencilOp::Replace );
		binder.AddValue( "Incr",		EStencilOp::Incr );
		binder.AddValue( "IncrWrap",	EStencilOp::IncrWrap );
		binder.AddValue( "Decr",		EStencilOp::Decr );
		binder.AddValue( "DecrWrap",	EStencilOp::DecrWrap );
		binder.AddValue( "Invert",		EStencilOp::Invert );
	}
	
/*
=================================================
	_Bind_ECompareFunc
=================================================
*/
	void ScriptPipeline::_Bind_ECompareFunc (ScriptEnginePtr se)
	{
		EnumBinder< ECompareFunc::type >	binder{ se };
		
		binder.Create();
		binder.AddValue( "None",		ECompareFunc::None );
		binder.AddValue( "Never",		ECompareFunc::Never );
		binder.AddValue( "Less",		ECompareFunc::Less );
		binder.AddValue( "Equal",		ECompareFunc::Equal );
		binder.AddValue( "LEqual",		ECompareFunc::LEqual );
		binder.AddValue( "Greater",		ECompareFunc::Greater );
		binder.AddValue( "NotEqual",	ECompareFunc::NotEqual );
		binder.AddValue( "GEqual",		ECompareFunc::GEqual );
		binder.AddValue( "Always",		ECompareFunc::Always );
	}
	
/*
=================================================
	_Bind_OptionalFloat2
=================================================
*/
	void ScriptPipeline::_Bind_OptionalFloat2 (ScriptEnginePtr se)
	{
		struct OptionalFloat2Utils {
			static GXMath::float2  Get (OptionalFloat2 &self) {
				return self.GetOrCreate();
			}

			static void Set (OptionalFloat2 &self, const GXMath::float2 &value) {
				self = value;
			}
		};

		ClassBinder< OptionalFloat2 >	binder{ se };

		binder.CreateClassValue();
		binder.AddMethodFromGlobal( &OptionalFloat2Utils::Get,	"Get" );
		binder.AddMethodFromGlobal( &OptionalFloat2Utils::Set,	"Set" );
	}
	
/*
=================================================
	_Bind_EPolygonMode
=================================================
*/
	void ScriptPipeline::_Bind_EPolygonMode (ScriptEnginePtr se)
	{
		EnumBinder< EPolygonMode::type >	binder{ se };
		
		binder.Create();
		binder.AddValue( "Point",	EPolygonMode::Point );
		binder.AddValue( "Line",	EPolygonMode::Line );
		binder.AddValue( "Fill",	EPolygonMode::Fill );
	}
	
/*
=================================================
	_Bind_EPolygonFace
=================================================
*/
	void ScriptPipeline::_Bind_EPolygonFace (ScriptEnginePtr se)
	{
		EnumBinder< EPolygonFace::type >	binder{ se };
		
		binder.Create();
		binder.AddValue( "None",		EPolygonFace::None );
		binder.AddValue( "Front",		EPolygonFace::Front );
		binder.AddValue( "Back",		EPolygonFace::Back );
		binder.AddValue( "FontAndBack",	EPolygonFace::FontAndBack );
	}
	
/*
=================================================
	_Bind_SampleMask
=================================================
*/
	void ScriptPipeline::_Bind_SampleMask (ScriptEnginePtr se)
	{
		ClassBinder< SampleMask >	binder{ se };

		binder.CreateClassValue();
		binder.AddMethod( &SampleMask::Set,		"Set" );
		binder.AddMethod( &SampleMask::Reset,	"Reset" );
		binder.AddMethod( &SampleMask::Count,	"Count" );
	}
	
/*
=================================================
	_Bind_MultiSamples
=================================================
*/
	void ScriptPipeline::_Bind_MultiSamples (ScriptEnginePtr se)
	{
		struct MultiSamplesUtils {
			static void Set (MultiSamples &self, uint value) {
				self = MultiSamples(value);
			}
		};

		ClassBinder< MultiSamples >		binder{ se };
		
		binder.CreateClassValue();
		binder.AddMethod( &MultiSamples::Get,	"Get" );
		binder.Operators().BinaryAssign( EBinaryOperator::Assign, &MultiSamplesUtils::Set );
	}
	
/*
=================================================
	_Bind_EBlendFuncPair
=================================================
*/
	void ScriptPipeline::_Bind_EBlendFuncPair (ScriptEnginePtr se)
	{
		ClassBinder< EBlendFuncPair >		binder{ se };
		
		binder.CreateClassValue();
		binder.AddProperty( &EBlendFuncPair::color,	"color" );
		binder.AddProperty( &EBlendFuncPair::alpha,	"alpha" );
	}
	
/*
=================================================
	_Bind_EBlendEqPair
=================================================
*/
	void ScriptPipeline::_Bind_EBlendEqPair (ScriptEnginePtr se)
	{
		ClassBinder< EBlendEqPair >		binder{ se };
		
		binder.CreateClassValue();
		binder.AddProperty( &EBlendEqPair::color,	"color" );
		binder.AddProperty( &EBlendEqPair::alpha,	"alpha" );
	}
	
/*
=================================================
	_Bind_UNormClampedF
=================================================
*/
	void ScriptPipeline::_Bind_UNormClampedF (ScriptEnginePtr se)
	{
		struct UNormClampedFUtils {
			static void Set (UNormClampedF &self, float val) {
				self = val;
			}
		};

		ClassBinder< UNormClampedF >		binder{ se };
		
		binder.CreateClassValue();
		binder.Operators().BinaryAssign( EBinaryOperator::Assign, &UNormClampedFUtils::Set );
	}
	
/*
=================================================
	_Bind_EShaderDstFormat
=================================================
*/
	void ScriptPipeline::_Bind_EShaderDstFormat (ScriptEnginePtr se)
	{
		// enum
		{
			EnumBinder< EShaderDstFormat::type >	binder{ se };
		
			binder.Create();
			binder.AddValue( "GLSL_Source",			EShaderDstFormat::GLSL_Source );
			//binder.AddValue( "GLSL_VulkanSource",	EShaderDstFormat::GLSL_VulkanSource );
			binder.AddValue( "GLSL_Binary",			EShaderDstFormat::GLSL_Binary );
			//binder.AddValue( "GLSL_ES_Source",	EShaderDstFormat::GLSL_ES_Source );
			//binder.AddValue( "GLSL_ES_Binary",	EShaderDstFormat::GLSL_ES_Binary );
			binder.AddValue( "CL_Source",			EShaderDstFormat::CL_Source );
			binder.AddValue( "CL_Binary",			EShaderDstFormat::CL_Binary );
			//binder.AddValue( "HLSL_Source",		EShaderDstFormat::HLSL_Source );
			//binder.AddValue( "HLSL_Binary",		EShaderDstFormat::HLSL_Binary );
			binder.AddValue( "SPIRV_Asm",			EShaderDstFormat::SPIRV_Source );
			binder.AddValue( "SPIRV_Binary",		EShaderDstFormat::SPIRV_Binary );
			//binder.AddValue( "Metal_Source",		EShaderDstFormat::Metal_Source );
			//binder.AddValue( "Metal_Binary",		EShaderDstFormat::Metal_Binary );
			binder.AddValue( "CPP_Module",			EShaderDstFormat::CPP_Module );
		}

		// bitfield
		{
			EnumBitfieldBinder< EShaderDstFormat::bits >	binder{ se };

			binder.BindDefaults();
		}
	}

}	// ResPack
