
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
		cfg.targets					|= EShaderDstFormat_GLSL_Source;
		cfg.targets					|= EShaderDstFormat_SPIRV_Binary;
		cfg.targets					|= EShaderDstFormat_CL_Source;
		cfg.targets					|= EShaderDstFormat_CPP_Module;
		//cfg.targets					|= EShaderDstFormat_HLSL_Source;
		//cfg.targets					|= EShaderDstFormat_HLSL_Binary;
		
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
		cfg.targets					|= EShaderDstFormat_GLSL_Source;
		cfg.targets					|= EShaderDstFormat_SPIRV_Binary;
		cfg.targets					|= EShaderDstFormat_CPP_Module;
		
		rpack.SetConfig( cfg );
	}
	
	// convert graphics pipelines
	{
		rpack.AddAllPipelines( "Graphics/Pipelines" );
		rpack.ConvertPipelines( "Graphics/Pipelines" );
	}
}
#endif	// ANGELSCRIPT
