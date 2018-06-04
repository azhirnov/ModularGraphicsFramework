
#ifdef ANGELSCRIPT
void main ()
{
	ResourcePacker	rpack;

	// setup pipeline compiler
	{
		Pipeline_ConverterConfig	cfg;
		
		cfg.searchForSharedTypes	= true;
		cfg.addPaddingToStructs		= true;
		cfg.optimizeSource			= false;
		cfg.optimizeBindings		= true;
		cfg.minimalRebuild			= true;
		cfg.nameSpace				= "Pipelines";
		cfg.targets					|= EShaderDstFormat_GLSL_Source;
		cfg.targets					|= EShaderDstFormat_SPIRV_Binary;
		//cfg.targets					|= EShaderDstFormat_CL_Source;
		cfg.targets					|= EShaderDstFormat_CPP_Module;

		rpack.SetConfig( cfg );
	}

	// convert compute pipelines
	{
		// graphics pipeline
		rpack.AddPipelineTemplate( "Pipelines/ShaderToy.tmpl", "ShadertoyTemplate" );
		
		// compute pipeline
		rpack.AddPipelineTemplate( "Pipelines/ShaderToyComp.tmpl", "ShadertoyCompTemplate" );

		rpack.AddAllPipelines( "Pipelines" );
		rpack.ConvertPipelines( "Pipelines" );
	}

	// convert images
	{
		ImagePacker	imgpack;

		imgpack.SetCPPVFS( "images" );
		imgpack.PackTexturesInFolder( "res" );
	}
}
#endif	// ANGELSCRIPT
