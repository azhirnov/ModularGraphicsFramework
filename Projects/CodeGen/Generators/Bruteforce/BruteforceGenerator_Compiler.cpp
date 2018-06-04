// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Generators/Bruteforce/BruteforceGenerator.h"
#include "Engine/PipelineCompiler/Shaders/ShaderCompiler.h"

#ifdef CG_RUN_ON_SOFT
#ifdef GRAPHICS_API_SOFT
namespace SWShaderLang {
	extern void sw_bruteforce (const Impl::SWShaderHelper &_helper_);
}
#endif	// GRAPHICS_API_SOFT
#endif

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

		ci.descr.layout = PipelineLayoutDescriptor::Builder()
								.AddStorageBuffer( "ssb_input",  _inputBufferSize,  0_b, EShaderMemoryModel::ReadOnly, 0, 0, EShader::Compute )
								.AddStorageBuffer( "ssb_output", _outputBufferSize, 0_b, EShaderMemoryModel::Coherent, 1, 1, EShader::Compute )
								.Finish();

		switch ( cfg.target )
		{
			case EShaderDstFormat::GLSL_Source :
				ci.descr.Compute().StringGLSL( StringCRef::From( compiled ) );
				break;

			case EShaderDstFormat::CL_Source :
				ci.descr.Compute().StringCL( StringCRef::From( compiled ) );
				break;
				
			case EShaderDstFormat::SPIRV_Binary :
				ci.descr.Compute().ArraySPIRV( ArrayCRef<uint>::From( compiled ) );
				break;

			#ifdef CG_RUN_ON_SOFT
			case EShaderDstFormat::CPP_Module :
				ci.descr.Compute().FuncSW( &SWShaderLang::sw_bruteforce );
				break;
			#endif

			default :
				RETURN_ERR( "unsupported shader format!" );
		}
		return true;
	}
	
}	// CodeGen
