// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/PipelineCompiler/Common/ShaderEnums.h"
#include "Engine/PipelineCompiler/Common/ArraySize.h"

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

		ND_ virtual String  ToString (StringCRef name, const RenderState &value) const = 0;
		ND_ virtual String	ToString (StringCRef name, EPipelineDynamicState::bits value) const = 0;
		ND_ virtual String  ToString (StringCRef name, const VertexAttribs &value) const = 0;
		ND_ virtual String  ToString (StringCRef name, const FragmentOutputState &value) const = 0;
		ND_ virtual String  ToString (StringCRef name, const PipelineLayoutDescription &value) const = 0;
		ND_ virtual String  ToString (StringCRef name, EPrimitive::bits value) const = 0;
		ND_ virtual String  ToString (StringCRef name, EShader::bits value) const = 0;
		ND_ virtual String	ToString (StringCRef name, const uint3 &value) const = 0;
		ND_ virtual String	ToString (StringCRef name, bool value) const = 0;

		ND_ virtual String  ToString (StringCRef value) const = 0;
		ND_ virtual String	ToString (EShaderVariable::type value) const = 0;

		ND_ virtual String	BeginFile (bool isIncludable) const = 0;
		ND_ virtual String	EndFile (bool isIncludable) const = 0;

		ND_ virtual String	BeginScope () const = 0;
		ND_ virtual String	EndScope () const = 0;

		ND_ virtual String  BeginNamespace (StringCRef name) const = 0;
		ND_ virtual String  EndNamespace () const = 0;

		ND_ virtual String	DeclVariable (StringCRef typeName, StringCRef name, StringCRef value = Uninitialized) const = 0;
		ND_ virtual String  AssignVariable (StringCRef name, StringCRef value) const = 0;
		ND_ virtual String	DeclFunction (StringCRef result, StringCRef name, ArrayCRef<Pair<StringCRef, StringCRef>> args,
										  bool isForwardDeclaration = false) const = 0;
		ND_ virtual String  CallFunction (StringCRef name, ArrayCRef<StringCRef> args) const = 0;

		ND_ virtual String	BeginStruct (StringCRef typeName, uint sizeOf, bool isPOD) const = 0;
		ND_ virtual String	StructField (StringCRef name, StringCRef typeName, ArraySize arraySize, uint offset, uint align, uint sizeOf) const = 0;
		ND_ virtual String	StructCtorForInitializerList () const = 0;
		ND_ virtual String	StructVertexAttribBinding () const = 0;
		ND_ virtual String	EndStruct () const = 0;

		ND_ virtual String	Include (StringCRef filename) const = 0;
		ND_ virtual String	Comment (StringCRef comment) const = 0;

		ND_ virtual String	ShaderToString (EShaderFormat::type fmt, StringCRef name, BinArrayCRef shaderSrc, bool inFile = false) const = 0;
		ND_ virtual String	ShaderSrcCPP_Impl (StringCRef name, BinArrayCRef shaderSrc, StringCRef funcName) const = 0;
		ND_ virtual String	ShaderSrcCPP (StringCRef name, StringCRef funcName) const = 0;

		ND_ virtual String	GetSourceFileExt () const = 0;
		ND_ virtual String	GetHeaderFileExt () const = 0;
	};


}	// PipelineCompiler
