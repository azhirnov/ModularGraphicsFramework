
#ifdef ANGELSCRIPT
void main ()
{
	ResourcePacker	rpack;

	// setup compute pipeline compiler
	{
		Pipeline_ConverterConfig	cfg;
		
		cfg.searchForSharedTypes	= true;
		cfg.addPaddingToStructs		= true;
		cfg.optimizeSource			= false;
		cfg.optimizeBindings		= false;
		cfg.minimalRebuild			= true;
		cfg.nameSpace				= "Pipelines";
		cfg.targets					|= EShaderFormat_GLSL_450;
		cfg.targets					|= EShaderFormat_VK_100_SPIRV;
		cfg.targets					|= EShaderFormat_CL_120;
		cfg.targets					|= EShaderFormat_Soft_100_Exe;
		//cfg.targets					|= EShaderFormat_HLSL_11;
		//cfg.targets					|= EShaderFormat_HLSL_11_BC;
		
		rpack.SetConfig( cfg );
	}

	// convert compute pipelines
	{
		rpack.AddAllPipelines( "Compute/Pipelines" );
		rpack.ConvertPipelines( "Compute/Pipelines" );
		
		//rpack.AddAllPipelines( "MultiGPU/Pipelines" );
		//rpack.ConvertPipelines( "MultiGPU/Pipelines" );
		
		rpack.AddAllPipelines( "Compiler/Pipelines" );
		rpack.ConvertPipelines( "Compiler/Pipelines" );
	}
	
	// setup graphics pipeline compiler
	{
		Pipeline_ConverterConfig	cfg;
		
		cfg.searchForSharedTypes	= true;
		cfg.addPaddingToStructs		= true;
		cfg.optimizeSource			= false;
		cfg.optimizeBindings		= false;
		cfg.minimalRebuild			= true;
		cfg.nameSpace				= "Pipelines";
		cfg.targets					|= EShaderFormat_GLSL_450;
		cfg.targets					|= EShaderFormat_VK_100_SPIRV;
		cfg.targets					|= EShaderFormat_Soft_100_Exe;
		
		rpack.SetConfig( cfg );
	}
	
	// convert graphics pipelines
	{
		rpack.AddAllPipelines( "Graphics/Pipelines" );
		rpack.ConvertPipelines( "Graphics/Pipelines" );
	}
}
#endif	// ANGELSCRIPT
