// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Engine/STL/Engine.STL.h"

using namespace GX_STL;
using namespace GX_STL::GXTypes;
using namespace GX_STL::GXMath;

#ifndef __GX_DEBUG__
#	error In release configuration tests don't show errors!
#endif


extern void Test_CompileTime_TypeInfo ();
extern void Test_CompileTime_TypeList ();
extern void Test_CompileTime_TypeTraits ();
extern void Test_CompileTime_TemplateMath ();
extern void Test_CompileTime_StaticFloat ();
extern void Test_CompileTime_TypeQualifier ();
extern void Test_CompileTime_MainType ();
extern void Test_CompileTime_StringToID ();

extern void Test_Runtime_VirtualTypelist ();

extern void Test_Containers_Array ();
extern void Test_Containers_Queue ();
extern void Test_Containers_CircularQueue ();
extern void Test_Containers_String ();
extern void Test_Containers_Set ();
extern void Test_Containers_List ();
extern void Test_Containers_Map ();
extern void Test_Containers_HashSet ();

extern void Test_Dimensions_PhysicsValue ();

extern void Test_Math_Abs ();
extern void Test_Math_Color ();
extern void Test_Math_ColorFormat ();
extern void Test_Math_ImageUtils ();
extern void Test_Math_Transform ();
extern void Test_Math_Factorial ();
extern void Test_Math_Clamp_Wrap ();
extern void Test_Math_Matrix ();
extern void Test_Math_FloorCeilTruncRoundFract ();
extern void Test_Math_Bit ();

extern void Test_Types_ExtString ();
extern void Test_Types_FileAddress ();
extern void Test_Types_Function ();
extern void Test_Types_StringParser ();
extern void Test_Types_Union ();
extern void Test_Types_Cast ();

extern void Test_OS_Date ();
extern void Test_OS_FileSystem ();

extern void Test_SimpleScript ();
extern void Test_Temp ();


void main ()
{
	InitializeSTLMath();

	Logger::GetInstance()->Open( "log", false );

	Test_Temp();

	Test_CompileTime_TypeInfo();
	Test_CompileTime_TypeList();
	Test_CompileTime_TypeTraits();
	Test_CompileTime_TemplateMath();
	Test_CompileTime_MainType();
	Test_CompileTime_StaticFloat();
	Test_CompileTime_TypeQualifier();
	Test_CompileTime_StringToID();

	Test_Runtime_VirtualTypelist();

	Test_Containers_Array();
	Test_Containers_Queue();
	Test_Containers_CircularQueue();
	Test_Containers_String();
	Test_Containers_Set();
	Test_Containers_Map();
	Test_Containers_HashSet();

	Test_Dimensions_PhysicsValue();

	Test_Math_Abs();
	Test_Math_Color();
	Test_Math_ColorFormat();
	Test_Math_ImageUtils();
	Test_Math_Transform();
	Test_Math_Factorial();
	Test_Math_Clamp_Wrap();
	Test_Math_Matrix();
	Test_Math_FloorCeilTruncRoundFract();
	Test_Math_Bit();

	//Test_Types_ExtString();
	Test_Types_FileAddress();
	Test_Types_Function();
	Test_Types_StringParser();
	Test_Types_Union();
	Test_Types_Cast();

	Test_OS_Date();
	Test_OS_FileSystem();

	Test_SimpleScript();

	WARNING( "Tests Finished!" );

	Logger::GetInstance()->Close();
	
	DEBUG_ONLY( RefCountedObject::s_ChenckNotReleasedObjects() );
}
