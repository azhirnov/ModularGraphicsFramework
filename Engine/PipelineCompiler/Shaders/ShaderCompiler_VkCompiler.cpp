// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Engine/PipelineCompiler/Shaders/glslang_include.h"
#include "glslang/SPIRV/doc.h"
#include "glslang/SPIRV/disassemble.h"
#include "glslang/SPIRV/GlslangToSpv.h"
#include "glslang/SPIRV/GLSL.std.450.h"

#include "Engine/PipelineCompiler/Shaders/ShaderCompiler.h"

#include <sstream>

namespace PipelineCompiler
{

/*
=================================================
	_ValidateSPIRV
=================================================
*/
	bool ShaderCompiler::_ValidateSPIRV (EShader::type, BinArrayCRef) const
	{
		TODO( "_ValidateSPIRV" );
		return false;
	}
	
/*
=================================================
	_CompileSPIRV
=================================================
*/
	bool ShaderCompiler::_CompileSPIRV (const Config &cfg, const _GLSLangResult &glslangData, OUT String &log, OUT BinaryArray &result) const
	{
		GX_UNUSED( log );

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

		if ( cfg.target == EShaderDstFormat::SPIRV_Source )
		{
			std::stringstream	spv_source;

			spv::Parameterize();
			spv::Disassemble( OUT spv_source, spirv );

			result = BinArrayCRef::FromStd( spv_source.str() );
		}
		else
		{
			result = BinArrayCRef::FromStd( spirv );
		}
		return true;
	}

}	// PipelineCompiler
