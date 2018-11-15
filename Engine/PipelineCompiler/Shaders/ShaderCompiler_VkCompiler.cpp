// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Engine/PipelineCompiler/Shaders/glslang_include.h"
#include "glslang/SPIRV/doc.h"
#include "glslang/SPIRV/disassemble.h"
#include "glslang/SPIRV/GlslangToSpv.h"
#include "glslang/SPIRV/GLSL.std.450.h"

#include "Engine/PipelineCompiler/Shaders/ShaderCompiler.h"

#include <sstream>
#include "glslang/External/SPIRV-Tools/include/spirv-tools/libspirv.h"

namespace PipelineCompiler
{

/*
=================================================
	_ValidateSPIRV
=================================================
*/
	bool ShaderCompiler::_ValidateSPIRV (EShader::type shaderType, EShaderFormat::type api, BinArrayCRef data) const
	{
		spv_context	ctx = spvContextCreate( SPV_ENV_VULKAN_1_0 );	// TODO
		CHECK_ERR( ctx != null );

		spv_const_binary_t	binary		= { Cast<uint32_t const*>( data.ptr() ), usize(data.Size()) / sizeof(uint32_t) };
		spv_diagnostic		diagnostic	= null;
		bool				result		= true;

		if ( spvValidate( ctx, &binary, OUT &diagnostic ) != SPV_SUCCESS )
		{
			// write errors
			result = false;
		}
		
		spvDiagnosticDestroy( diagnostic );
		spvContextDestroy( ctx );

		return result;
	}
	
/*
=================================================
	_CompileGLSLtoSPIRV
=================================================
*/
	bool ShaderCompiler::_CompileGLSLtoSPIRV (const Config &cfg, const _GLSLangResult &glslangData, OUT String &log, OUT BinaryArray &result) const
	{
		const glslang::TIntermediate* intermediate = glslangData.prog.getIntermediate( glslangData.shader->getStage() );
		CHECK_ERR( intermediate );

		std::vector<unsigned int>	spirv;
		glslang::SpvOptions			spv_options;
		spv::SpvBuildLogger			logger;

		spv_options.generateDebugInfo	= not cfg.optimize;
		spv_options.disableOptimizer	= not cfg.optimize;
		spv_options.optimizeSize		= cfg.optimize;
		
		glslang::GlslangToSpv( *intermediate, OUT spirv, &logger, &spv_options );

		log << StringCRef(logger.getAllMessages());
		CHECK_ERR( not spirv.empty() );

		switch ( EShaderFormat::GetStorage( cfg.target ) )
		{
			case EShaderFormat::Source :
			{
				std::stringstream	spv_source;

				spv::Parameterize();
				spv::Disassemble( OUT spv_source, spirv );

				result = BinArrayCRef::FromStd( spv_source.str() );
				break;
			}

			case EShaderFormat::Binary :
			{
				result = BinArrayCRef::FromStd( spirv );
				break;
			}

			default :
				RETURN_ERR( "not supported" );
		}

		return true;
	}
	
/*
=================================================
	_CompileSPIRVAsm
=================================================
*/
	bool ShaderCompiler::_CompileSPIRVAsm (const Config &cfg, StringCRef spirvAsm, OUT String &log, OUT BinaryArray &result) const
	{
		result.Clear();

		spv_context	ctx = spvContextCreate( SPV_ENV_VULKAN_1_0 );	// TODO
		CHECK_ERR( ctx != null );

		spv_binary		binary		= null;
		spv_diagnostic	diagnostic	= null;

		if ( spvTextToBinary( ctx, spirvAsm.ptr(), spirvAsm.Count(), OUT &binary, OUT &diagnostic ) == SPV_SUCCESS )
		{
			result = BinArrayCRef::From(ArrayCRef<uint>{ binary->code, binary->wordCount });
		}
		
		spvBinaryDestroy( binary );
		spvDiagnosticDestroy( diagnostic );
		spvContextDestroy( ctx );

		CHECK_ERR( not result.Empty() );
		return true;
	}
	
/*
=================================================
	_DisasambleSPIRV
=================================================
*/
	bool ShaderCompiler::_DisasambleSPIRV (const Config &cfg, ArrayCRef<uint> spirv, OUT String &log, OUT BinaryArray &result) const
	{
		result.Clear();

		spv_context	ctx = spvContextCreate( SPV_ENV_VULKAN_1_0 );	// TODO
		CHECK_ERR( ctx != null );

		spv_text		text		= null;
		spv_diagnostic	diagnostic	= null;

		if ( spvBinaryToText( ctx, spirv.ptr(), spirv.Count(), 0, &text, &diagnostic ) == SPV_SUCCESS )
		{
			result = BinArrayCRef::From(StringCRef{ text->str, text->length });
		}
		
		spvTextDestroy( text );
		spvDiagnosticDestroy( diagnostic );
		spvContextDestroy( ctx );

		CHECK_ERR( not result.Empty() );
		return true;
	}

}	// PipelineCompiler
