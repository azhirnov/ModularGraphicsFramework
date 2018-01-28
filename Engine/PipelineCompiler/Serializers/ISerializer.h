// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/PipelineCompiler/Common/ShaderEnums.h"

namespace PipelineCompiler
{

	//
	// Serializer interface
	//

	class ISerializer : public RefCountedObject<>
	{
	// interface
	public:
		virtual ~ISerializer () {}

		virtual String  ToString (StringCRef name, const RenderState &value) const = 0;
		virtual String	ToString (StringCRef name, EPipelineDynamicState::bits value) const = 0;
		virtual String  ToString (StringCRef name, const VertexAttribs &value) const = 0;
		virtual String  ToString (StringCRef name, const FragmentOutputState &value) const = 0;
		virtual String  ToString (StringCRef name, const PipelineLayoutDescriptor &value) const = 0;
		virtual String  ToString (StringCRef name, EPrimitive::bits value) const = 0;
		virtual String  ToString (StringCRef name, EShader::bits value) const = 0;

		virtual String  ToString (StringCRef value) const = 0;
		virtual String	ToString (EShaderVariable::type value) const = 0;

		virtual String	BeginFile (bool isIncludable) const = 0;
		virtual String	EndFile (bool isIncludable) const = 0;

		virtual String	BeginScope () const = 0;
		virtual String	EndScope () const = 0;

		virtual String	DeclVariable (StringCRef typeName, StringCRef name, StringCRef value = Uninitialized) const = 0;
		virtual String  AssignVariable (StringCRef name, StringCRef value) const = 0;
		virtual String	DeclFunction (StringCRef result, StringCRef name, ArrayCRef<Pair<StringCRef, StringCRef>> args,
									  bool isForwardDeclaration = false) const = 0;
		virtual String  CallFunction (StringCRef name, ArrayCRef<StringCRef> args) const = 0;
		virtual String  DeclNamespace (StringCRef name) const = 0; 

		virtual String	BeginStruct (StringCRef typeName, uint sizeOf) const = 0;
		virtual String	StructField (StringCRef name, StringCRef typeName, uint arraySize, uint offset, uint align, uint sizeOf) const = 0;
		virtual String	EndStruct () const = 0;

		virtual String	Include (StringCRef filename) const = 0;
		virtual String	Comment (StringCRef comment) const = 0;

		virtual String	ShaderSrcGLSL (StringCRef name, BinArrayCRef shaderSrc) const = 0;
		virtual String	ShaderBinGLSL (StringCRef name, BinArrayCRef shaderSrc) const = 0;
		virtual String	ShaderFileSrcGLSL (StringCRef name, BinArrayCRef shaderSrc) const = 0;
		virtual String	ShaderFileBinGLSL (StringCRef name, BinArrayCRef shaderSrc) const = 0;

		virtual String	ShaderBinSPIRV (StringCRef name, BinArrayCRef shaderSrc) const = 0;
		virtual String	ShaderSrcSPIRV (StringCRef name, BinArrayCRef shaderSrc) const = 0;
		virtual String	ShaderFileSrcSPIRV (StringCRef name, BinArrayCRef shaderSrc) const = 0;
		virtual String	ShaderFileBinSPIRV (StringCRef name, BinArrayCRef shaderSrc) const = 0;
		
		virtual String	ShaderBinCL (StringCRef name, BinArrayCRef shaderSrc) const = 0;
		virtual String	ShaderSrcCL (StringCRef name, BinArrayCRef shaderSrc) const = 0;
		virtual String	ShaderFileSrcCL (StringCRef name, BinArrayCRef shaderSrc) const = 0;
		virtual String	ShaderFileBinCL (StringCRef name, BinArrayCRef shaderSrc) const = 0;
		
		virtual String	ShaderSrcCPP (StringCRef name, BinArrayCRef shaderSrc) const = 0;

		virtual String	GetSourceFileExt () const = 0;
		virtual String	GetHeaderFileExt () const = 0;
	};


}	// PipelineCompiler
