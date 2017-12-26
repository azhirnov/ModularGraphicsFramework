// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Common.h"

using namespace PipelineCompiler;


extern bool Test_PipelineCompilation ()
{
	PipelineManager::ConverterConfig	cfg;
	
	cfg.searchForSharedTypes	= true;
	cfg.addPaddingToStructs		= true;
	cfg.optimizeSource			= false;
	//cfg.optimizeBindings		= false;
	//cfg.skipBufferLayouts		= true;	// for vulkan
	cfg.includings				<< "common.h";
	cfg.nameSpace				= "Pipelines";
	cfg.target					|= EShaderDstFormat::GLSL_Source;
	cfg.target					|= EShaderDstFormat::SPIRV_Binary;

	PipelineManager::Instance()->Convert( "out/all_pipelines", new CppSerializer(), cfg );

	return true;
}
