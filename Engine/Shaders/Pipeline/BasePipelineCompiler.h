// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/Shaders/Pipeline/Pipeline.h"

namespace ShaderEditor
{
namespace PipelineNodes
{

	//
	// Base Pipeline Compiler
	//

	class BasePipelineCompiler : public GX_STL::GXTypes::StaticRefCountedObject
	{
	// types
	protected:
		template <typename T>				using Array		= GX_STL::GXTypes::Array<T>;
		template <typename F, typename S>	using Pair		= GX_STL::GXTypes::Pair< F, S >;
		template <typename K, typename V>	using Map		= GX_STL::GXTypes::Map< K, V >;
		template <typename T>				using HashSet	= GX_STL::GXTypes::HashSet< T >;

		using usize			= GX_STL::GXTypes::usize;
		using GLuint		= GX_STL::GXTypes::uint;
		using String		= GX_STL::GXTypes::String;
		using StringCRef	= GX_STL::GXTypes::StringCRef;
		using ModulePtr		= Engine::Base::ModulePtr;
		using EShader		= Engine::Platforms::EShader;
		using NodePtr		= _ShaderNodesHidden_::ISrcNodePtr;
		using ENodeType		= _ShaderNodesHidden_::ISrcNode::ENodeType;


	// variables
	private:


	// methods
	public:
		BasePipelineCompiler () {}

		virtual bool Compile (const Array<NodePtr> &nodes, EShader::type shaderType, OUT ModulePtr &program) = 0;
	};


}	// PipelineNodes
}	// ShaderEditor
