
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
		cfg.targets					|= EShaderDstFormat_CL_Source;
		cfg.targets					|= EShaderDstFormat_CPP_Module;
		
		rpack.SetConfig( cfg );
	}

	// convert compute pipelines
	{
		rpack.AddAllPipelines( "Compute/Pipelines" );
		rpack.ConvertPipelines( "Compute/Pipelines" );

		rpack.AddAllPipelines( "Graphics/Pipelines" );
		rpack.ConvertPipelines( "Graphics/Pipelines" );
	}
}
#endif	// ANGELSCRIPT
