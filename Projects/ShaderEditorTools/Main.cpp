// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Engine/PipelineCompiler/Shaders/ShaderCompiler.h"
#include "Engine/PipelineCompiler/Pipelines/PipelineManager.h"
#include "Engine/PipelineCompiler/Serializers/CppSerializer.h"

using namespace PipelineCompiler;

int main ()
{
	InitializeSTLMath();
	Logger::GetInstance()->Open( "log", false );
	CHECK( OS::FileSystem::FindAndSetCurrentDir( "Projects/ShaderEditor/Pipelines", 5 ) );


	PipelineManager::ConverterConfig	cfg;
	
	cfg.searchForSharedTypes	= true;
	cfg.addPaddingToStructs		= true;
	cfg.optimizeSource			= false;	// set 'false' for debugging
	//cfg.optimizeBindings		= false;
	cfg.includings				<< "Pipelines/common.h";
	cfg.nameSpace				= "Pipelines";
	cfg.target					|= EShaderDstFormat::GLSL_Source;
	cfg.target					|= EShaderDstFormat::SPIRV_Binary;
	//cfg.target					|= EShaderDstFormat::SPIRV_Source;

	PipelineManager::Instance()->Convert( "all_pipelines", new CppSerializer(), cfg );
	
	Logger::GetInstance()->Close();
	return 0;
}
