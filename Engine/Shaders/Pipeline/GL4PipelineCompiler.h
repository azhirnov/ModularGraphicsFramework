// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/Shaders/Pipeline/Pipeline.h"

namespace ShaderEditor
{
namespace PipelineNodes
{

	//
	// OpenGL 4 Pipeline Compiler
	//

	class GL4PipelineCompiler final : public GX_STL::GXTypes::StaticRefCountedObject
	{
	// types
	private:
		template <typename T>				using Array	= GX_STL::GXTypes::Array<T>;
		template <typename F, typename S>	using Pair	= GX_STL::GXTypes::Pair< F, S >;

		using usize			= GX_STL::GXTypes::usize;
		using NodePtr		= _ShaderNodesHidden_::ISrcNodePtr;
		using String		= GX_STL::GXTypes::String;
		using StringCRef	= GX_STL::GXTypes::StringCRef;

		struct TextureInfo : Pair< NodePtr, NodePtr >
		{
			TextureInfo (const NodePtr &image, const NodePtr &sampler) :
				 Pair< NodePtr, NodePtr >( image, sampler )
			{}

			NodePtr const&	Image ()	const	{ return first; }
			NodePtr const&	Sampler ()	const	{ return second; }
		};

		struct NodeInfo
		{
			usize		bindingIndex;
			String		typeName;
			String		name;
			String		uniform;

			NodeInfo (usize index = ~0u) :
				bindingIndex( index )
			{}

			NodeInfo (usize index, StringCRef typeName, StringCRef name) :
				bindingIndex(index), typeName(typeName), name(name)
			{}
		};
		
		using NodeInfoMap	= GX_STL::GXTypes::Map< NodePtr, NodeInfo >;
		using TexturesMap	= GX_STL::GXTypes::Map< TextureInfo, NodeInfo >;
		using ENodeType		= _ShaderNodesHidden_::ISrcNode::ENodeType;
		using ModulePtr		= Engine::Base::ModulePtr;
		using FuncSources	= GX_STL::GXTypes::HashSet< String >;
		using GLuint		= GX_STL::GXTypes::uint;
		using EShader		= Engine::Platforms::EShader;


	// variables
	private:
		NodeInfoMap		_imageNodes;
		NodeInfoMap		_bufferNodes;
		TexturesMap		_textureNodes;

		FuncSources		_funcSources;

		bool			_initialized;


	// methods
	public:
		GL4PipelineCompiler ();

		bool Compile (const Array<NodePtr> &nodes, EShader::type shaderType, OUT ModulePtr &program);


	private:
		void _Initialize ();
		void _Quit ();

		bool _CompilePass0 (EShader::type shaderType, OUT String &src);
		bool _CompilePass1 (const Array<NodePtr> &nodes, OUT String &src);
		bool _CompilePass2 (const Array<NodePtr> &nodes, OUT String &src);

		// to GLSL type
		String	_VecTypeName (const NodePtr &node) const;

		bool	_ImageTypeName (const NodePtr &imgNode, usize index, OUT String &typeName, OUT String &uniformName) const;
		bool	_TextureTypeName (const NodePtr &texNode, usize index, OUT String &typeName, OUT String &uniformName) const;
		bool	_BufferTypeName (const NodePtr &bufNode, usize index, OUT String &typeName, OUT String &uniformName) const;

		String	_NodeName (const NodePtr &node) const;
		String	_FuncArgs (const NodePtr &result);
		String	_GetConstValue (const NodePtr &node) const;
		String	_GetImageFormat (const NodePtr &node) const;

		bool	_CompileShaders (StringCRef compute);
		GLuint	_CompileGLShader (StringCRef shaderSrc) const;
		void	_ParseCompilationErrors (StringCRef source, StringCRef log, bool compiled) const;

		bool _CompileShaders (StringCRef vertex, StringCRef tessControl, StringCRef tessEval,
							  StringCRef geometry, StringCRef fragment);
	};


}	// PipelineNodes
}	// ShaderEditor
