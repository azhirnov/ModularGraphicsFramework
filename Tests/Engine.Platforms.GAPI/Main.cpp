// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Common.h"

extern void Test_ComputeApi (StringCRef device);
extern void Test_GraphicsApi (StringCRef device);
extern void Test_Sharing (StringCRef device);
extern void Test_PipelineCompiler (StringCRef device);
extern void Test_MultiGPU (StringCRef device);


int main ()
{
	// for example: intel HD 630, GTX 1050 Ti, ...
	// warning: some device name did not contains vendor name
	const String	device = "";

	Logger::GetInstance()->Open( "log", false );
	
	Test_PipelineCompiler( device );
	Test_ComputeApi( device );
	//Test_GraphicsApi( device );
	Test_Sharing( device );
	//Test_MultiGPU( device );

	return 0;
}
