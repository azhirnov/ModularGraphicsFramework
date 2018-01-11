// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#pragma warning (push)
#pragma warning (disable: 4456)
#pragma warning (disable: 4100)
#pragma warning (disable: 4996)

#define AMD_EXTENSIONS
#define NV_EXTENSIONS
#define ENABLE_HLSL

// glslang includes
#include "glslang/glslang/Include/ShHandle.h"
#include "glslang/glslang/Include/revision.h"
#include "glslang/glslang/Public/ShaderLang.h"
#include "glslang/SPIRV/doc.h"
#include "glslang/SPIRV/disassemble.h"
#include "glslang/SPIRV/GlslangToSpv.h"
#include "glslang/SPIRV/GLSL.std.450.h"
#include "glslang/StandAlone/Worklist.h"
#include "glslang/glslang/OSDependent/osinclude.h"
#include "glslang/glslang/MachineIndependent/localintermediate.h"
#include "glslang/glslang/Include/intermediate.h"

// LunarGLASS includes
#include "LunarGLASS/Frontends/glslang/GlslangToTop.h"
#include "LunarGLASS/Frontends/SPIRV/SpvToTop.h"
#include "LunarGLASS/Core/Options.h"
#include "LunarGLASS/Standalone/OptionParse.h"
#include "LunarGLASS/Backends/GLSL/GlslManager.h"
#include "LunarGLASS/Core/Exceptions.h"

#pragma warning (pop)


#include "Engine/PipelineCompiler/Common/Common.h"
#include "Engine/PipelineCompiler/Shaders/ShaderCompiler.h"
	
namespace PipelineCompiler
{

/*
=================================================
	ConvertShaderType
=================================================
*/
	inline EShLanguage  ConvertShaderType (EShader::type type)
	{
		switch ( type )
		{
			case EShader::Vertex :			return EShLanguage::EShLangVertex;
			case EShader::TessControl :		return EShLanguage::EShLangTessControl;
			case EShader::TessEvaluation :	return EShLanguage::EShLangTessEvaluation;
			case EShader::Geometry :		return EShLanguage::EShLangGeometry;
			case EShader::Fragment :		return EShLanguage::EShLangFragment;
			case EShader::Compute :			return EShLanguage::EShLangCompute;
		}
		RETURN_ERR( "unsupported shader type", EShLangCount );
	}
	

	
	//
	// _GLSLangResult
	//
	struct ShaderCompiler::_GLSLangResult
	{
	// variables
		glslang::TProgram				prog;
		UniquePtr< glslang::TShader >	shader;
	};


	//
	// Shader Data
	//
	struct ShaderCompiler::_ShaderData
	{
		EShader::type		type;
		Array<StringCRef>	src;
		StringCRef			entry;
	};

	
	// check shader compilation, do not show warnings or write to log,
	// just add errors to log string and break compilation.
#	define CHECK_COMP( _func_ ) \
		{if ( _func_ ) {} else { \
			DEBUG_ONLY( log << "error: " << __LINE__ << ": " << GX_TO_ANSI_STRING( _func_ ) << " - failed, file: \"" << GX_SHORT_FILENAME << "\"\n" ); \
			return false; \
		}}


}	// PipelineCompiler
