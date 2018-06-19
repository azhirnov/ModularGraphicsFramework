// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/ResourcePacker/Pipelines/ScriptPipeline.h"

namespace ResPack
{

	//
	// Script Graphics Pipeline
	//

	class ScriptGraphicsPipeline final : public ScriptPipeline
	{
	// types
	private:

		//
		// Pipeline
		//
		class Pipeline final : public PipelineCompiler::GraphicsPipeline
		{
		// methods
		public:
			explicit Pipeline (StringCRef name) : GraphicsPipeline(name) {}

			EShaderSrcFormat::type &		GetShaderFormat ()			{ return shaderFormat; }
			Shaders &						GetShaders ()				{ return shaders; }
			RenderState &					GetRenderState ()			{ return renderState; }
			EPrimitive::bits &				GetSupportedTopology ()		{ return supportedTopology; }
			EPipelineDynamicState::bits&	GetDynamicStates ()			{ return dynamicStates; }
			uint &							GetPatchControlPoints ()	{ return patchControlPoints; }
		};
		SHARED_POINTER( Pipeline );


	// variables
	private:
		PipelinePtr						_pipeline;

		EShaderSrcFormat::type			_shaderFormat		= EShaderSrcFormat::GLSL;
		Shaders							_shaders;
		RenderState						_renderState;
		EPrimitive::bits				_supportedTopology;
		EPipelineDynamicState::bits		_dynamicStates;
		uint 							_patchControlPoints	= 0;


	// methods
	public:
		ScriptGraphicsPipeline ();
		explicit ScriptGraphicsPipeline (const PipelineCompiler::GraphicsPipeline *);

		~ScriptGraphicsPipeline ();

		static void Bind (ScriptEnginePtr se);
	};

}	// ResPack

namespace GXScript
{
	GX_DECL_SCRIPT_TYPE( ResPack::ScriptGraphicsPipeline,	"GraphicsPipeline" );

}	// GXScript
