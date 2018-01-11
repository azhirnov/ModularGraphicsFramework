// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/PipelineCompiler/Common/Common.h"
#include "Engine/PipelineCompiler/Shaders/DeserializedShader.h"

class TIntermNode;

namespace glslang
{
	class TType;
	class TProgram;
	class TIntermediate;
	struct TSourceLoc;
}

namespace PipelineCompiler
{

	//
	// Shader Program Compiler
	//

	class ShaderCompiler
	{
	// types
	public:
		static const uint	GLSL_VERSION	= 450;
		static const uint	GLSL_ES_VERSION	= 310;
		static const uint	HLSL_VERSION	= 1100;
		static const uint	VULKAN_VERSION	= 100;
		static const uint	SPIRV_VERSION	= 0x00001000;

		struct FieldTypeInfo
		{
			String					name;
			String					typeName;
			EShaderVariable::type	type		= EShaderVariable::Unknown;
			uint					index		= 0;		// index of field in struct, update index if 'name' was changed

			bool operator == (const FieldTypeInfo &right) const;
		};

		using ReplaceStructTypesFunc_t	= Delegate< bool (StringCRef typeName, INOUT FieldTypeInfo &field) >;


		struct Config
		{
			ReplaceStructTypesFunc_t	typeReplacer;
			EShaderSrcFormat::type		source				= EShaderSrcFormat::GXSL;
			EShaderDstFormat::type		target				= EShaderDstFormat::GLSL_Source;
			uint						inlineThreshold		= 1000;
			uint						loopUnrollThreshold	= 350;
			bool						obfuscate			= false;
			bool						filterInactive		= false;
			bool						skipExternals		= false;		// GLSL source only		// uniforms, buffers, in/out
			bool						optimize			= false;
		};


	private:
		struct _GLSLangResult;
		struct _ShaderData;


	// variables
	private:


	// methods
	public:
		ShaderCompiler ();
		~ShaderCompiler ();

		bool Translate (EShader::type shaderType, ArrayCRef<StringCRef> source, StringCRef entryPoint,
						const Config &cfg, OUT String &log, OUT BinaryArray &result);

		bool Deserialize (EShaderSrcFormat::type shaderFmt, EShader::type shaderType, ArrayCRef<StringCRef> source, StringCRef entryPoint,
						  OUT String &log, OUT DeserializedShader &result);

		static Ptr<ShaderCompiler>	Instance ();


	private:
		bool _GLSLangParse (const Config &cfg, const _ShaderData &shader, OUT String &log, OUT _GLSLangResult &result) const;

		bool _Compile (const glslang::TIntermediate* intermediate, const Config &cfg, OUT String &log, OUT BinaryArray &result) const;

		bool _CompileWithLunarGOO (const Config &cfg, const _GLSLangResult &glslangData, OUT String &log, OUT BinaryArray &result) const;

		bool _CopySource (const _ShaderData &data, OUT BinaryArray &result) const;

		bool _CheckGLAErrors (OUT String *log = null) const;
		
		bool _OnCompilationFailed (EShader::type shaderType, EShaderSrcFormat::type fmt, ArrayCRef<StringCRef> source, INOUT String &log) const;

		static void _GenerateResources (OUT struct TBuiltInResource& resources);
		

	// Replace types
	private:
		bool _ReplaceTypes (const _GLSLangResult &data, const Config &cfg) const;


	// GLSL deserializer
	private:
		static bool _RecursiveProcessAggregateNode (TIntermNode* root, TIntermNode* node, INOUT uint &uid, INOUT DeserializedShader &result);
		static bool _RecursiveProcessNode (TIntermNode* root, TIntermNode* node, INOUT uint &uid, INOUT DeserializedShader &result);
		
		static bool _DeserializeFunction (TIntermNode* node, INOUT uint &uid, INOUT DeserializedShader &result);

		static bool _DeserializeFunctionArg (TIntermNode* node, glslang::TType const &ttype, glslang::TSourceLoc const &loc,
											 const DeserializedShader::FunctionArgument *parent, OUT DeserializedShader::FunctionArgument &result);

		static bool _DeserializeVariable (TIntermNode* node, glslang::TType const &ttype, glslang::TSourceLoc const &loc,
										  const DeserializedShader::_Variable *parent, OUT DeserializedShader::_Variable &result);

		static bool _DeserializeBufferVariable (TIntermNode* node, glslang::TType const &ttype, glslang::TSourceLoc const &loc,
												const DeserializedShader::BufferVariable *parent, OUT DeserializedShader::BufferVariable &result);

		static bool _DeserializeExternalObjects (TIntermNode* node, INOUT uint &uid, INOUT DeserializedShader &result);

		static bool _DeserializeIOVariable (TIntermNode* node, glslang::TType const &ttype, glslang::TSourceLoc const &loc,
											const DeserializedShader::IOVariable *parent, OUT DeserializedShader::IOVariable &result);

		static bool _DeserializeUniform (TIntermNode* node, glslang::TType const &ttype, glslang::TSourceLoc const &loc,
										 const DeserializedShader::Uniform *parent, OUT DeserializedShader::Uniform &result);
		
		static bool _DeserializeConstant (TIntermNode* node, glslang::TType const &ttype, glslang::TSourceLoc const &loc,
										  OUT DeserializedShader::Constant &result);

		static bool _ProcessShaderInfo (glslang::TIntermediate const* intermediate, INOUT DeserializedShader &result);


	// GXSL/GLSL to GLSL translator
	private:
		bool _TranslateGXSLtoGLSL (const Config &cfg, const _GLSLangResult &glslangData, OUT String &log, OUT BinaryArray &result) const;
		bool _OptimizeGLSL (const Config &cfg, const _ShaderData &shader, OUT String &log, OUT BinaryArray &result) const;
		bool _CompileGLSL (const Config &cfg, const _ShaderData &shader, OUT String &log, OUT BinaryArray &result) const;


	// GXSL/GLSL to CPP translator
	private:
		bool _TranslateGXSLtoCPP (const Config &cfg, const _GLSLangResult &glslangData, OUT String &log, OUT BinaryArray &result) const;


	// GLSL to HLSL translator
	private:
		bool _TranslateGXSLtoHLSL (const Config &cfg, const _GLSLangResult &glslangData, OUT String &log, OUT BinaryArray &result) const;
		bool _CompileHLSL (const Config &cfg, const _ShaderData &shader, OUT String &log, OUT BinaryArray &result) const;


	// GXSL/GLSL to OpenCL translator
	private:
		bool _TranslateGXSLtoCL (const Config &cfg, const _GLSLangResult &glslangData, OUT String &log, OUT BinaryArray &result) const;
		bool _CompileCL (const Config &cfg, const _ShaderData &shader, OUT String &log, OUT BinaryArray &result) const;
	};

}	// PipelineCompiler
