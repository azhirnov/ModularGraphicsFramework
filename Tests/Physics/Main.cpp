// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Tests/Physics/Common.h"
#include "Engine/STL/Log/Logger.h"

extern void Test_Dimensions_PhysicsValue ();
extern void Test_Algorithms_Orbit ();

using namespace GX_STL;
using namespace GX_STL::GXTypes;
using namespace GX_STL::GXMath;

int main ()
{
	InitializeSTLMath();

	Logger::GetInstance()->Open( "log", false );

	Test_Dimensions_PhysicsValue();

	Test_Algorithms_Orbit();
	
	TEST( !"Tests Finished!" );

	Logger::GetInstance()->Close();
	
	DEBUG_ONLY( DbgRefCountedObject::s_ChenckNotReleasedObjects() );
	DEBUG_ONLY( RefCounter2::s_CheckAllocations() );

	return 0;
}
