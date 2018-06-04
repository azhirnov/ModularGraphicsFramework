// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Engine/PipelineCompiler/Shaders/ShaderCompiler.h"
#include "Engine/PipelineCompiler/Shaders/glslang_Include.h"

#ifdef GX_ENABLE_DXCOMPILER
#	include <d3dcompiler.h>
#endif

namespace PipelineCompiler
{

#ifdef GX_ENABLE_DXCOMPILER
/*
=================================================
	_CompileHLSL
=================================================
*/
	bool ShaderCompiler::_CompileHLSL (const Config &cfg, const _ShaderData &shader, OUT String &log, OUT BinaryArray &result) const
	{
		String	source;
		String	target;

		FOR( i, shader.src ) {
			source << shader.src[i] << '\n';
		}

		switch ( shader.type )
		{
			case EShader::Vertex :			target = "vs_";	break;
			case EShader::TessControl :		target = "hs_";	break;
			case EShader::TessEvaluation :	target = "ds_";	break;
			case EShader::Geometry :		target = "gs_";	break;
			case EShader::Fragment :		target = "ps_";	break;
			case EShader::Compute :			target = "cs_";	break;
			default :						RETURN_ERR( "unsupported shader type!" );
		}

		// TODO: choose version
		target << "5_0";

		UINT	flags1 = 0;
		UINT	flags2 = 0;

		flags1 |= D3DCOMPILE_PACK_MATRIX_COLUMN_MAJOR | D3DCOMPILE_WARNINGS_ARE_ERRORS;

		if ( cfg.optimize )
			flags1 |= D3DCOMPILE_OPTIMIZATION_LEVEL3;
		else
			flags1 |= D3DCOMPILE_DEBUG; //| D3DCOMPILE_SKIP_OPTIMIZATION;

		
		ID3DBlob*	dx_code			= null;
		ID3DBlob*	compiler_errors	= null;

		HRESULT hr = D3DCompile( source.cstr(), size_t(source.Size()), null, null, null, shader.entry.cstr(),
								 target.cstr(), flags1, flags2, OUT &dx_code, OUT &compiler_errors );
		
		if ( compiler_errors and compiler_errors->GetBufferSize() > 0 )
			log << Cast<const char*>( compiler_errors->GetBufferPointer() );	// TODO: parse errors

		if ( compiler_errors )
			compiler_errors->Release();

		if ( dx_code )
		{
			result = BinArrayCRef( Cast<const ubyte*>(dx_code->GetBufferPointer()), dx_code->GetBufferSize() );
			dx_code->Release();
		}
		return SUCCEEDED(hr);
	}
	
/*
=================================================
	_ValidateHLSLSource
=================================================
*/
	bool ShaderCompiler::_ValidateHLSLSource (EShader::type, StringCRef) const
	{
		TODO( "_ValidateHLSLSource" );
		return false;
	}
	
/*
=================================================
	_ValidateHLSLBinary
=================================================
*/
	bool ShaderCompiler::_ValidateHLSLBinary (EShader::type, BinArrayCRef) const
	{
		TODO( "_ValidateHLSLBinary" );
		return false;
	}

# else

	bool ShaderCompiler::_CompileHLSL (const Config &, const _ShaderData &, OUT String &, OUT BinaryArray &) const
	{
		return false;
	}

	bool ShaderCompiler::_ValidateHLSLSource (EShader::type, StringCRef) const
	{
		return false;
	}

	bool ShaderCompiler::_ValidateHLSLBinary (EShader::type, BinArrayCRef) const
	{
		return false;
	}

# endif	// GX_ENABLE_DXCOMPILER

}	// PipelineCompiler
