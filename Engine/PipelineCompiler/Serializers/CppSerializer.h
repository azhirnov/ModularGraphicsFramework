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

		struct StructInfo
		{
			String		typeName;
			String		tests;
			String		fieldAsArguments;
			String		fieldInitialized;
		};

		using StructTypes_t		= Stack< StructInfo >;
		using NamespaceStack_t	= Stack< String >;


	// variables
	private:
		mutable StructTypes_t		_structStack;
		mutable NamespaceStack_t	_nsStack;
		mutable String				_podTypes;
		mutable int					_fileCounter	= 0;
		mutable int					_scopeCounter	= 0;


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
		String	ToString (StringCRef name, const uint3 &value) const override;

		String  ToString (StringCRef value) const override;
		String	ToString (EShaderVariable::type value) const override;
		
		String	BeginFile (bool isIncludable) const override;
		String	EndFile (bool isIncludable) const override;

		String	BeginScope () const override;
		String	EndScope () const override;
		
		String  BeginNamespace (StringCRef name) const override;
		String  EndNamespace () const override;

		String	DeclVariable (StringCRef typeName, StringCRef name, StringCRef value) const override;
		String  AssignVariable (StringCRef name, StringCRef value) const override;
		String	DeclFunction (StringCRef result, StringCRef name, ArrayCRef<Pair<StringCRef, StringCRef>> args,
							  bool isForwardDeclaration) const override;
		String  CallFunction (StringCRef name, ArrayCRef<StringCRef> args) const override;

		String	BeginStruct (StringCRef typeName, uint sizeOf, bool isPOD) const override;
		String	StructField (StringCRef name, StringCRef typeName, uint arraySize, uint offset, uint align, uint sizeOf) const override;
		String	StructCtorForInitializerList () const override;
		String	EndStruct () const override;
		
		String	Include (StringCRef filename) const override;
		String	Comment (StringCRef comment) const override;
		
		String	ShaderSrcGLSL (StringCRef name, BinArrayCRef shaderSrc, bool inFile) const override;
		String	ShaderBinGLSL (StringCRef name, BinArrayCRef shaderSrc, bool inFile) const override;

		String	ShaderBinSPIRV (StringCRef name, BinArrayCRef shaderSrc, bool inFile) const override;
		String	ShaderSrcSPIRV (StringCRef name, BinArrayCRef shaderSrc, bool inFile) const override;
		
		String	ShaderBinCL (StringCRef name, BinArrayCRef shaderSrc, bool inFile) const override;
		String	ShaderSrcCL (StringCRef name, BinArrayCRef shaderSrc, bool inFile) const override;
		
		String	ShaderSrcCPP_Impl (StringCRef name, BinArrayCRef shaderSrc, StringCRef funcName) const override;
		String	ShaderSrcCPP (StringCRef name, StringCRef funcName) const override;
		
		String	ShaderBinHLSL (StringCRef name, BinArrayCRef shaderSrc, bool inFile) const override;
		String	ShaderSrcHLSL (StringCRef name, BinArrayCRef shaderSrc, bool inFile) const override;

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
		static String  ToString (EShaderMemoryModel::type value);
		
		static String  ExtractIndent (StringCRef str);
	};


}	// PipelineCompiler
