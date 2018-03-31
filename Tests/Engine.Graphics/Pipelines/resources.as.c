
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
		cfg.nameSpace				= "Pipelines";
		cfg.targets					|= EShaderDstFormat_GLSL_Source;
		cfg.targets					|= EShaderDstFormat_SPIRV_Binary;
		cfg.targets					|= EShaderDstFormat_CL_Source;
		//cfg.targets				|= EShaderDstFormat_CPP_Module;
		cfg.Include( "pipelines_common.h" );

		rpack.SetConfig( cfg );
	}

	// convert compute pipelines
	{
		rpack.AddAllPipelines( "" );
		rpack.ConvertPipelines( "" );
	}
}
#endif	// ANGELSCRIPT
