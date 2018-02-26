// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Engine/PipelineCompiler/Shaders/ShaderCompiler.h"
#include "Engine/PipelineCompiler/Pipelines/PipelineManager.h"
#include "Engine/PipelineCompiler/Serializers/CppSerializer.h"

using namespace PipelineCompiler;


int main ()
{
	InitializeSTLMath();
	Logger::GetInstance()->Open( "log", false );
	CHECK( OS::FileSystem::FindAndSetCurrentDir( "Tests/Engine.Platforms.GAPI/Pipelines", 5 ) );

	PipelineManager::ConverterConfig	cfg;
	
	cfg.searchForSharedTypes	= true;
	cfg.addPaddingToStructs		= true;
	cfg.optimizeSource			= false;
	//cfg.optimizeBindings		= false;
	cfg.includings				<< "common.h";
	cfg.nameSpace				= "Pipelines";
	cfg.target					|= EShaderDstFormat::GLSL_Source;
	cfg.target					|= EShaderDstFormat::SPIRV_Binary;
	cfg.target					|= EShaderDstFormat::CL_Source;
	cfg.target					|= EShaderDstFormat::CPP_Module;

	PipelineManager::Instance()->Convert( "all_pipelines", new CppSerializer(), cfg );

	ShaderCompiler::Instance()->DestroyContext();

	Logger::GetInstance()->Close();
	return 0;
}
