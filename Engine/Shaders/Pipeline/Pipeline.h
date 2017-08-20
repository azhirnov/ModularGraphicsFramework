// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/Shaders/Shader/ShaderNodes.h"

namespace ShaderEditor
{
namespace PipelineNodes
{

	//
	// Pipeline
	//

	struct Pipeline
	{
	// types
	protected:
		struct CompiledProgram
		{
		};

		//using CompiledShader	= ShaderNodes::Shader::CompiledShader;

	private:
		using EShader			= Engine::Platforms::EShader;
		using AttachedShaders_t	= GX_STL::GXTypes::Map< EShader::type, ShaderNodes::Shader* >;


	// variables
	private:
		AttachedShaders_t	_attachedShaders;


	// methods
	protected:
		Pipeline ();

		bool _CompileShaders ();

	public:
		bool AddShader (ShaderNodes::Shader* shader);
	};



	//
	// Graphics Pipeline
	//

	struct GraphicsPipeline : Pipeline
	{
	// variables
	protected:
		CompiledProgram		_program;


	// methods
	protected:
		GraphicsPipeline ();

	public:
		void Run (const GX_STL::GXMath::uint3 &areaSize);
	};



	//
	// Compute Pipeline
	//

	struct ComputePipeline : Pipeline
	{
	// variables
	protected:
		CompiledProgram		_program;


	// methods
	protected:
		ComputePipeline ();

	public:
		void Run (const GX_STL::GXMath::uint3 &globalSize,
				  const GX_STL::GXMath::uint3 &localSize = GX_STL::GXMath::uint3(0));
	};



}	// PipelineNodes
}	// ShaderEditor
