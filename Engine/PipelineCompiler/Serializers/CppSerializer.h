// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/PipelineCompiler/Serializers/ISerializer.h"

namespace PipelineCompiler
{

	//
	// C++ Serializer
	//

	class CppSerializer final : public ISerializer
	{
	// types
	private:
		struct PipelineLayoutToStringFunc;

		using StructTypes	= Stack<Pair< /*typename*/String, /*tests*/String >>;


	// variables
	private:
		mutable StructTypes		_structStack;
		mutable int				_fileCounter	= 0;
		mutable int				_scopeCounter	= 0;


	// methods
	public:
		CppSerializer ();
		~CppSerializer ();

		String  ToString (StringCRef name, const RenderState &value) const override;
		String	ToString (StringCRef name, EPipelineDynamicState::bits value) const override;
		String  ToString (StringCRef name, const VertexAttribs &value) const override;
		String  ToString (StringCRef name, const FragmentOutputState &value) const override;
		String  ToString (StringCRef name, const PipelineLayoutDescriptor &value) const override;
		String  ToString (StringCRef name, EPrimitive::bits value) const override;
		String  ToString (StringCRef name, EShader::bits value) const override;

		String  ToString (StringCRef value) const override;
		String	ToString (EShaderVariable::type value) const override;
		
		String	BeginFile (bool isIncludable) const override;
		String	EndFile (bool isIncludable) const override;

		String	BeginScope () const override;
		String	EndScope () const override;
		
		String	DeclVariable (StringCRef typeName, StringCRef name, StringCRef value) const override;
		String  AssignVariable (StringCRef name, StringCRef value) const override;
		String	DeclFunction (StringCRef result, StringCRef name, ArrayCRef<Pair<StringCRef, StringCRef>> args,
							  bool isForwardDeclaration) const override;
		String  CallFunction (StringCRef name, ArrayCRef<StringCRef> args) const override;
		String  DeclNamespace (StringCRef name) const override; 

		String	BeginStruct (StringCRef typeName, uint sizeOf) const override;
		String	StructField (StringCRef name, StringCRef typeName, uint arraySize, uint offset, uint align, uint sizeOf) const override;
		String	EndStruct () const override;
		
		String	Include (StringCRef filename) const override;
		String	Comment (StringCRef comment) const override;
		
		String	ShaderSrcGLSL (StringCRef name, BinArrayCRef shaderSrc) const override;
		String	ShaderBinGLSL (StringCRef name, BinArrayCRef shaderSrc) const override;
		String	ShaderFileSrcGLSL (StringCRef name, BinArrayCRef shaderSrc) const override;
		String	ShaderFileBinGLSL (StringCRef name, BinArrayCRef shaderSrc) const override;

		String	ShaderBinSPIRV (StringCRef name, BinArrayCRef shaderSrc) const override;
		String	ShaderSrcSPIRV (StringCRef name, BinArrayCRef shaderSrc) const override;
		String	ShaderFileSrcSPIRV (StringCRef name, BinArrayCRef shaderSrc) const override;
		String	ShaderFileBinSPIRV (StringCRef name, BinArrayCRef shaderSrc) const override;
		
		String	ShaderBinCL (StringCRef name, BinArrayCRef shaderSrc) const override;
		String	ShaderSrcCL (StringCRef name, BinArrayCRef shaderSrc) const override;
		String	ShaderFileSrcCL (StringCRef name, BinArrayCRef shaderSrc) const override;
		String	ShaderFileBinCL (StringCRef name, BinArrayCRef shaderSrc) const override;
		
		String	ShaderSrcCPP (StringCRef name, BinArrayCRef shaderSrc) const override;

		String	GetSourceFileExt () const override	{ return "cpp"; }
		String	GetHeaderFileExt () const override	{ return "h"; }

	private:
		static String  ToString (BinArrayCRef value);
		static String  ToString (ArrayCRef<uint> value);

		static String  ToString (EBlendFunc::type value);
		static String  ToString (EBlendEq::type value);
		static String  ToString (ELogicOp::type value);
		static String  ToString (ECompareFunc::type value);
		static String  ToString (EStencilOp::type value);
		static String  ToString (EPolygonMode::type value);
		static String  ToString (EPolygonFace::type value);
		static String  ToString (EPrimitive::type value);
		static String  ToString (MultiSamples value);
		static String  ToString (StringCRef name, const RenderState::ColorBuffer &state);
		static String  ToString (StringCRef name, const RenderState::ColorBuffersState &state);
		static String  ToString (StringCRef name, const RenderState::DepthBufferState &state);
		static String  ToString (StringCRef name, const RenderState::StencilFaceState &state);
		static String  ToString (StringCRef name, const RenderState::StencilBufferState &state);
		static String  ToString (StringCRef name, const RenderState::RasterizationState &state);
		static String  ToString (StringCRef name, const RenderState::InputAssemblyState &state);
		static String  ToString (StringCRef name, const RenderState::MultisampleState &state);
		static String  ToString (EVertexAttribute::type value);
		static String  ToString (EFragOutput::type value);
		static String  ToString (EImage::type value);
		static String  ToString (EShader::bits value);
		static String  ToString (EPixelFormatClass::type value);
		static String  ToString (EPixelFormat::type value);
		
		static String  ExtractIndent (StringCRef str);
	};


}	// PipelineCompiler
