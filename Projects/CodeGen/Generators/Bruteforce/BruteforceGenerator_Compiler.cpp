// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Generators/Bruteforce/BruteforceGenerator.h"
#include "Engine/PipelineCompiler/Shaders/ShaderCompiler.h"

namespace CodeGen
{

/*
=================================================
	_CompileProgram
=================================================
*/
	bool BruteforceGenerator::_CompileProgram (ArrayCRef<StringCRef> src, OUT CreateInfo::PipelineTemplate &ci)
	{
		using namespace PipelineCompiler;

		ShaderCompiler::Config	cfg;
		cfg.obfuscate	= false;
		cfg.optimize	= false;
		cfg.source		= EShaderSrcFormat::GXSL;

		// choose target shader format
		{
			Message< GpuMsg::GetComputeSettings >	req_settings;
			CHECK( _gpuThread->Send( req_settings ) );

			const String	api_name = req_settings->result->GetAPIName();

			if ( api_name.EqualsIC( "GL" ) )	cfg.target = EShaderDstFormat::GLSL_Source;		else
			if ( api_name.EqualsIC( "CL" ) )	cfg.target = EShaderDstFormat::CL_Source;		else
			if ( api_name.EqualsIC( "VK" ) )	cfg.target = EShaderDstFormat::SPIRV_Binary;	else
			if ( api_name.EqualsIC( "SW" ) )	cfg.target = EShaderDstFormat::CPP_Module;		else
												RETURN_ERR( "unsupported compute api" );
		}

		String			log;
		BinaryArray		compiled;

		if ( not ShaderCompiler::Instance()->Translate( EShader::Compute, src, "main", cfg, OUT log, OUT compiled ) )
		{
			RETURN_ERR( log );
		}

		ci.descr.supportedShaders	|= EShader::Compute;
		ci.descr.localGroupSize		= uint3( _localThreads, 1, 1 );
		
		const BytesU	inbuf_size		= AlignToLarge( _bigIntSize + SizeOf<float>, 16_b ) + _testCaseSize * _testsCount;
		const BytesU	outbuf_size		= _atomicsSize + _resultSize * _maxResults;

		ci.descr.layout				= PipelineLayoutDescriptor::Builder()
										.AddStorageBuffer( "ssb_input", inbuf_size, 0_b, EShaderMemoryModel::ReadOnly, 0, 0, EShader::Compute )
										.AddStorageBuffer( "ssb_output", outbuf_size, 0_b, EShaderMemoryModel::Coherent, 1, 1, EShader::Compute )
										.Finish();

		switch ( cfg.target )
		{
			case EShaderDstFormat::GLSL_Source :
				ci.descr.Compute().StringGLSL( StringCRef( (const char *)compiled.ptr() ) );
				break;

			case EShaderDstFormat::CL_Source :
				ci.descr.Compute().StringCL( StringCRef( (const char *)compiled.ptr() ) );
				break;
				
			case EShaderDstFormat::SPIRV_Binary :
				ci.descr.Compute().ArraySPIRV( ArrayCRef<uint>::From( compiled ) );
				break;

			default :
				RETURN_ERR( "unsupported shader format!" );
		}
		return true;
	}
	
}	// CodeGen
