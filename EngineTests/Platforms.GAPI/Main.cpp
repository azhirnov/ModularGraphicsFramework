// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Common.h"

extern void Test_ComputeApi (StringCRef device, bool debug);
extern void Test_GraphicsApi (StringCRef device, bool debug);
extern void Test_Sharing (StringCRef device, bool debug);
extern void Test_PipelineCompiler (StringCRef device, bool debug);
extern void Test_MultiGPU (StringCRef device, bool debug);
extern void Test_Window ();


/*
=================================================
	main
=================================================
*/
int main ()
{
	// for example: intel HD 630, GTX 1050 Ti, ...
	// warning: some device name did not contains vendor name
	const String	device	= "";
	const bool		debug	= true;

	Logger::GetInstance()->Open( "log", false );
	
	Test_PipelineCompiler( device, debug );
	Test_ComputeApi( device, debug );
	Test_GraphicsApi( device, debug );
	Test_Sharing( device, debug );
	//Test_MultiGPU( device, debug );
	//Test_Window();

	return 0;
}
