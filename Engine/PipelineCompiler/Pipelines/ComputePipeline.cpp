// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Engine/PipelineCompiler/Pipelines/ComputePipeline.h"

namespace PipelineCompiler
{
	
/*
=================================================
	constructor
=================================================
*/
	ComputePipeline::ComputePipeline (StringCRef name) : BasePipeline(name)
	{
	}
		
/*
=================================================
	Prepare
----
	pass 1
=================================================
*/
	bool ComputePipeline::Prepare (const ConverterConfig &cfg)
	{
		ShaderDisasembly	disasm;
		CHECK_ERR( _DisasembleShader( cfg, INOUT shader, OUT disasm ) );
		
		// unite struct and block types from shaders
		_structTypes.Clear();
		CHECK_ERR( _MergeStructTypes( disasm.structTypes, INOUT _structTypes ) );
		
		// update offsets by packing
		CHECK_ERR( _CalculateOffsets( INOUT _structTypes ) );

		_originTypes = _structTypes;
		
		CHECK_ERR( _UpdateBindings() );
		CHECK_ERR( _UpdateDescriptorSets() );

		_lastEditTime = Max( _lastEditTime, shader.LastEditTime() );

		return true;
	}
	
/*
=================================================
	Convert
----
	pass 2
=================================================
*/
	bool ComputePipeline::Convert (OUT String &src, Ptr<ISerializer> ser, const ConverterConfig &constCfg) const
	{
		ConverterConfig		cfg = constCfg;
		
		src.Clear();
		src << ser->Comment( "This is generated file" );
		src << ser->Comment( "Origin file: '"_str << Path() << "'" );
		src << ser->Comment( "Created at: "_str << ToString( OS::Date().Now() ) ) << '\n';
		src << ser->BeginFile( false );
		
		// add c++ types localy
		if ( not cfg.searchForSharedTypes )
		{
			/*cfg._glslTypes = "";
			
			CHECK_ERR( _CalculateOffsets( INOUT _structTypes ) );
		
			if ( cfg.addPaddingToStructs )
			{
				CHECK_ERR( _AddPaddingToStructs( INOUT _structTypes ) );
				CHECK_ERR( _CalculateOffsets( INOUT _structTypes ) );
			}

			String	ser_str;
			CHECK_ERR( _AllStructsToString( _structTypes, ser, OUT ser_str, OUT cfg._glslTypes ) );

			src << ser_str;*/
		}

		FOR( i, cfg.includings ) {
			src << ser->Include( cfg.includings[i] );
		}

		// mark place for c++ source
		if ( constCfg.target[ EShaderDstFormat::CPP_Module ] )
		{
			src << ser->Comment( "C++ shader" );
		}

		src << '\n' << ser->DeclNamespace( cfg.nameSpace );
		src << ser->BeginScope() << '\n';
		

		// serialize descriptor
		src << ser->DeclFunction( "void", "Create_"_str << Name(), {{"PipelineTemplateDescriptor&", "descr"}} );
		src << ser->BeginScope();
		src << ser->AssignVariable( "\tdescr", "PipelineTemplateDescriptor()" );
		src << ser->ToString( "\tdescr.supportedShaders", EShader::bits() | EShader::Compute ) << '\n';
		src << ser->ToString( "\tdescr.localGroupSize", localGroupSize );

		CHECK_ERR( _ConvertLayout( "\tdescr.layout", INOUT src, ser ) );
		
		CHECK_ERR( _ConvertComputeShader( INOUT src, ser, cfg ) );

		src << ser->EndScope();	// function
		src << ser->EndScope();	// namespace
		src << ser->EndFile( false );
		return true;
	}
	
/*
=================================================
	_ConvertComputeShader
----
	pass 2
=================================================
*/
	bool ComputePipeline::_ConvertComputeShader (INOUT String &src, Ptr<ISerializer> ser, const ConverterConfig &cfg) const
	{
		CHECK_ERR( shader.IsEnabled() );

		CompiledShader	compiled;
		CHECK_ERR( _CompileShader( shader, cfg, OUT compiled ) );

		String	name = "\t" + ser->CallFunction( "descr.Compute", Uninitialized );

		src << ser->ShaderSrcGLSL( name, compiled.glsl )
			<< ser->ShaderBinGLSL( name, compiled.glslBinary )
			<< ser->ShaderBinSPIRV( name, compiled.spirv )
			<< ser->ShaderSrcSPIRV( name, compiled.spirvAsm )
			<< ser->ShaderSrcCL( name, compiled.cl )
			<< ser->ShaderBinCL( name, compiled.clAsm );
		
		if ( not compiled.cpp.Empty() )
		{
			usize	pos;
			if ( src.Find( "C++ shader", OUT pos ) )
			{
				StringParser::ToNextLine( src, INOUT pos );

				const String	func_name = "sw_"_str << Name() << "_comp";

				src.Insert( ser->ShaderSrcCPP_Impl( name, compiled.cpp, func_name ), pos );

				src << ser->ShaderSrcCPP( name, func_name );
			}
		}

		src << '\n';

		if ( cfg.validation ) {
			CHECK_ERR( _ValidateShader( EShader::Compute, compiled ) );
		}
		return true;
	}

}	// PipelineCompiler
