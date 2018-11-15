// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/ResourcePacker/Pipelines/ScriptPipeline.h"

namespace ResPack
{

	//
	// Script Compute Pipeline
	//

	class ScriptComputePipeline final : public ScriptPipeline
	{
	// types
	private:

		//
		// Pipeline
		//
		class Pipeline final : public PipelineCompiler::ComputePipeline
		{
		// methods
		public:
			explicit Pipeline (StringCRef name) : ComputePipeline(name) {}

			EShaderFormat::type &	GetShaderFormat ()			{ return shaderFormat; }
			ShaderModule &			GetShader ()				{ return shader; }
			GXMath::uint3 &			GetLocalGroup ()			{ return localGroupSize; }
		};
		SHARED_POINTER( Pipeline );


	// variables
	private:
		PipelinePtr				_pipeline;

		EShaderFormat::type		_shaderFormat	= EShaderFormat::DefaultSrc;
		ShaderModule			_compute;
		GXMath::uint3			_localGroup;


	// methods
	public:
		ScriptComputePipeline ();
		explicit ScriptComputePipeline (const PipelineCompiler::ComputePipeline *);

		~ScriptComputePipeline ();
		
		static void Bind (ScriptEnginePtr se);
	};

}	// ResPack

namespace GXScript
{
	GX_DECL_SCRIPT_TYPE( ResPack::ScriptComputePipeline,	"ComputePipeline" );

}	// GXScript
