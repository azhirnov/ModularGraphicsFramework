// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Engine/PipelineCompiler/Shaders/ShaderCompiler.h"

namespace PipelineCompiler
{

/*
=================================================
	_CompileCL
=================================================
*/
	bool ShaderCompiler::_CompileCL (const Config &cfg, const _ShaderData &shader, OUT String &log, OUT BinaryArray &result) const
	{
		TODO( "_CompileCL" );
		return false;
	}
	
/*
=================================================
	_ValidateCLSource
=================================================
*/
	bool ShaderCompiler::_ValidateCLSource (EShader::type shaderType, StringCRef src) const
	{
		TODO( "_ValidateCLSource" );
		return false;
	}
	
/*
=================================================
	_ValidateCLBinary
=================================================
*/
	bool ShaderCompiler::_ValidateCLBinary (EShader::type shaderType, BinArrayCRef bin) const
	{
		TODO( "_ValidateCLBinary" );
		return false;
	}

}	// PipelineCompiler
