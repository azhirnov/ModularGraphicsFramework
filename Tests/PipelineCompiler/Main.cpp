// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Common.h"

using namespace PipelineCompiler;

#pragma pack(push, 1)
//#include "../out/shared_types.h"
#pragma pack(pop)

extern bool Test_PipelineCompilation ();
extern bool Test_DeserializeGLSL ();
extern bool Test_DeserializeGLSL_2 ();
extern bool Test_DeserializeGLSL_3 ();
extern bool Test_TranslateCL ();
extern bool Test_CompileGxsl ();


void main ()
{
	InitializeSTLMath();
	Logger::GetInstance()->Open( "log", false );
	CHECK( OS::FileSystem::FindAndSetCurrentDir( "../../../Tests/PipelineCompiler", 5 ) );


	//Test_DeserializeGLSL();
	//Test_DeserializeGLSL_2();
	//Test_DeserializeGLSL_3();
	Test_PipelineCompilation();
	//Test_CompileGxsl();
	//Test_TranslateCL();


	Logger::GetInstance()->Close();
}
