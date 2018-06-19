// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/ResourcePacker/Packer/Common.h"
#include "Engine/PipelineCompiler/Pipelines/GraphicsPipeline.h"
#include "Engine/PipelineCompiler/Pipelines/ComputePipeline.h"
#include "Engine/PipelineCompiler/Pipelines/PipelineManager.h"

namespace ResPack
{

	//
	// Script Pipeline
	//

	class ScriptPipeline
	{
	// types
	private:
		class _Ppln : public PipelineCompiler::BasePipeline
		{public:
			using ShaderModule	= BasePipeline::ShaderModule;
		};

		class _GPpln : public PipelineCompiler::GraphicsPipeline
		{public:
			using Shaders		= PipelineCompiler::GraphicsPipeline::Shaders;
		};

		class _CPpln : public PipelineCompiler::ComputePipeline
		{};


	public:
		using ConverterConfig		= PipelineCompiler::PipelineManager::ConverterConfig;
		using ScriptEnginePtr		= GXScript::ScriptEnginePtr;
		using ShaderModule			= _Ppln::ShaderModule;
		using Shaders				= _GPpln::Shaders;
		using EShaderSrcFormat		= PipelineCompiler::EShaderSrcFormat;
		using EShaderDstFormat		= PipelineCompiler::EShaderDstFormat;
		using EPrimitive			= PipelineCompiler::EPrimitive;
		using RenderState			= PipelineCompiler::RenderState;
		using EPipelineDynamicState	= PipelineCompiler::EPipelineDynamicState;
		using EBlendFunc			= PipelineCompiler::EBlendFunc;
		using EBlendEq				= PipelineCompiler::EBlendEq;
		using ELogicOp				= PipelineCompiler::ELogicOp;
		using EStencilOp			= PipelineCompiler::EStencilOp;
		using ECompareFunc			= PipelineCompiler::ECompareFunc;
		using OptionalFloat2		= Optional< GXMath::float2 >;
		using EPolygonMode			= PipelineCompiler::EPolygonMode;
		using EPolygonFace			= PipelineCompiler::EPolygonFace;
		using SampleMask			= RenderState::MultisampleState::SampleMask;
		using MultiSamples			= PipelineCompiler::MultiSamples;
		using EBlendFuncPair		= RenderState::ColorBuffer::_Pair< EBlendFunc >;
		using EBlendEqPair			= RenderState::ColorBuffer::_Pair< EBlendEq >;
		using UNormClampedF			= UNormClamped<float>;


	// methods
	public:
		ScriptPipeline () {}
		~ScriptPipeline () {}

		static void BindAll (ScriptEnginePtr se);

	private:
		static void _Bind_ShaderModule (ScriptEnginePtr se);
		static void _Bind_Shaders (ScriptEnginePtr se);
		static void _Bind_ConverterConfig (ScriptEnginePtr se);

		static void _Bind_RenderState (ScriptEnginePtr se);
		static void _Bind_ColorBuffersState (ScriptEnginePtr se);
		static void _Bind_DepthBufferState (ScriptEnginePtr se);
		static void _Bind_StencilBufferState (ScriptEnginePtr se);
		static void _Bind_InputAssemblyState (ScriptEnginePtr se);
		static void _Bind_RasterizationState (ScriptEnginePtr se);
		static void _Bind_MultisampleState (ScriptEnginePtr se);

		static void _Bind_ColorBuffer (ScriptEnginePtr se);
		static void _Bind_StencilFaceState (ScriptEnginePtr se);
		static void _Bind_EShaderSrcFormat (ScriptEnginePtr se);
		static void _Bind_EPrimitive (ScriptEnginePtr se);
		static void _Bind_EPipelineDynamicState (ScriptEnginePtr se);
		static void _Bind_EBlendFunc (ScriptEnginePtr se);
		static void _Bind_EBlendEq (ScriptEnginePtr se);
		static void _Bind_ELogicOp (ScriptEnginePtr se);
		static void _Bind_EStencilOp (ScriptEnginePtr se);
		static void _Bind_ECompareFunc (ScriptEnginePtr se);
		static void _Bind_OptionalFloat2 (ScriptEnginePtr se);
		static void _Bind_EPolygonMode (ScriptEnginePtr se);
		static void _Bind_EPolygonFace (ScriptEnginePtr se);
		static void _Bind_SampleMask (ScriptEnginePtr se);
		static void _Bind_MultiSamples (ScriptEnginePtr se);
		static void _Bind_EBlendFuncPair (ScriptEnginePtr se);
		static void _Bind_EBlendEqPair (ScriptEnginePtr se);
		static void _Bind_UNormClampedF (ScriptEnginePtr se);
		static void _Bind_EShaderDstFormat (ScriptEnginePtr se);
	};

}	// ResPack

namespace GXScript
{
	GX_DECL_SCRIPT_TYPE( ResPack::ScriptPipeline::ConverterConfig,					"Pipeline_ConverterConfig" );
	GX_DECL_SCRIPT_TYPE( ResPack::ScriptPipeline::Shaders,							"Pipeline_Shaders" );
	GX_DECL_SCRIPT_TYPE( ResPack::ScriptPipeline::ShaderModule,						"Pipeline_ShaderModule" );
	GX_DECL_SCRIPT_TYPE( ResPack::ScriptPipeline::RenderState,						"Pipeline_RenderState" );
	GX_DECL_SCRIPT_TYPE( ResPack::ScriptPipeline::RenderState::ColorBuffer,			"Pipeline_RenderState_ColorBuffer" );
	GX_DECL_SCRIPT_TYPE( ResPack::ScriptPipeline::RenderState::ColorBuffersState,	"Pipeline_RenderState_ColorBuffersState" );
	GX_DECL_SCRIPT_TYPE( ResPack::ScriptPipeline::RenderState::DepthBufferState,	"Pipeline_RenderState_DepthBufferState" );
	GX_DECL_SCRIPT_TYPE( ResPack::ScriptPipeline::RenderState::StencilFaceState,	"Pipeline_RenderState_StencilFaceState" );
	GX_DECL_SCRIPT_TYPE( ResPack::ScriptPipeline::RenderState::StencilBufferState,	"Pipeline_RenderState_StencilBufferState" );
	GX_DECL_SCRIPT_TYPE( ResPack::ScriptPipeline::RenderState::InputAssemblyState,	"Pipeline_RenderState_InputAssemblyState" );
	GX_DECL_SCRIPT_TYPE( ResPack::ScriptPipeline::RenderState::RasterizationState,	"Pipeline_RenderState_RasterizationState" );
	GX_DECL_SCRIPT_TYPE( ResPack::ScriptPipeline::RenderState::MultisampleState,	"Pipeline_RenderState_MultisampleState" );
	GX_DECL_SCRIPT_TYPE( ResPack::ScriptPipeline::EShaderSrcFormat::type,			"EShaderSrcFormat" );
	GX_DECL_SCRIPT_TYPE( ResPack::ScriptPipeline::EShaderDstFormat::type,			"EShaderDstFormat" );
	GX_DECL_SCRIPT_TYPE( ResPack::ScriptPipeline::EShaderDstFormat::bits,			"EShaderDstFormatBits" );
	GX_DECL_SCRIPT_TYPE( ResPack::ScriptPipeline::EPrimitive::type,					"EPrimitive" );
	GX_DECL_SCRIPT_TYPE( ResPack::ScriptPipeline::EPrimitive::bits,					"EPrimitiveBits" );
	GX_DECL_SCRIPT_TYPE( ResPack::ScriptPipeline::EPipelineDynamicState::type,		"EPipelineDynamicState" );
	GX_DECL_SCRIPT_TYPE( ResPack::ScriptPipeline::EPipelineDynamicState::bits,		"EPipelineDynamicStateBits" );
	GX_DECL_SCRIPT_TYPE( ResPack::ScriptPipeline::EBlendFunc::type,					"EBlendFunc" );
	GX_DECL_SCRIPT_TYPE( ResPack::ScriptPipeline::EBlendEq::type,					"EBlendEq" );
	GX_DECL_SCRIPT_TYPE( ResPack::ScriptPipeline::ELogicOp::type,					"ELogicOp" );
	GX_DECL_SCRIPT_TYPE( ResPack::ScriptPipeline::EStencilOp::type,					"EStencilOp" );
	GX_DECL_SCRIPT_TYPE( ResPack::ScriptPipeline::ECompareFunc::type,				"ECompareFunc" );
	GX_DECL_SCRIPT_TYPE( ResPack::ScriptPipeline::OptionalFloat2,					"Pipeline_OptionalFloat2" );
	GX_DECL_SCRIPT_TYPE( ResPack::ScriptPipeline::EPolygonMode::type,				"EPolygonMode" );
	GX_DECL_SCRIPT_TYPE( ResPack::ScriptPipeline::EPolygonFace::type,				"EPolygonFace" );
	GX_DECL_SCRIPT_TYPE( ResPack::ScriptPipeline::SampleMask,						"Pipeline_SampleMask" );
	GX_DECL_SCRIPT_TYPE( ResPack::ScriptPipeline::MultiSamples,						"Pipeline_MultiSamples" );
	GX_DECL_SCRIPT_TYPE( ResPack::ScriptPipeline::EBlendFuncPair,					"Pipeline_EBlendFuncPair" );
	GX_DECL_SCRIPT_TYPE( ResPack::ScriptPipeline::EBlendEqPair,						"Pipeline_EBlendEqPair" );
	GX_DECL_SCRIPT_TYPE( ResPack::ScriptPipeline::UNormClampedF,					"UNormClampedF" );

}	// GXScript