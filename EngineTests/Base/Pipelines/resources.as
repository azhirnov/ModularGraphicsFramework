
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
		cfg.targets					|= EShaderFormat_GLSL_450;
		cfg.targets					|= EShaderFormat_VK_100_SPIRV;
		cfg.targets					|= EShaderFormat_CL_120;
		//cfg.targets				|= EShaderFormat_Soft_100_Exe;

		rpack.SetConfig( cfg );
	}

	// convert compute pipelines
	{
		rpack.AddAllPipelines( "" );
		rpack.ConvertPipelines( "" );
	}
}
#endif	// ANGELSCRIPT