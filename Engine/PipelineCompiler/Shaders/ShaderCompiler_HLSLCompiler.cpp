// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Engine/PipelineCompiler/Shaders/ShaderCompiler.h"

namespace PipelineCompiler
{

/*
=================================================
	_CompileHLSL
=================================================
*/
	bool ShaderCompiler::_CompileHLSL (const Config &cfg, const _ShaderData &shader, OUT String &log, OUT BinaryArray &result) const
	{
		TODO( "_CompileHLSL" );
		return false;
	}
	
/*
=================================================
	_ValidateHLSLSource
=================================================
*/
	bool ShaderCompiler::_ValidateHLSLSource (EShader::type shaderType, StringCRef src) const
	{
		TODO( "_ValidateHLSLSource" );
		return false;
	}
	
/*
=================================================
	_ValidateHLSLBinary
=================================================
*/
	bool ShaderCompiler::_ValidateHLSLBinary (EShader::type shaderType, BinArrayCRef bin) const
	{
		TODO( "_ValidateHLSLBinary" );
		return false;
	}

}	// PipelineCompiler
