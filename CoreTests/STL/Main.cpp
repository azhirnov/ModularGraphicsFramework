// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "CoreTests/STL/Common.h"

using namespace GX_STL;
using namespace GX_STL::GXTypes;
using namespace GX_STL::GXMath;


extern void Test_CompileTime_TypeInfo ();
extern void Test_CompileTime_TypeList ();
extern void Test_CompileTime_TypeTraits ();
extern void Test_CompileTime_TemplateMath ();
extern void Test_CompileTime_StaticFloat ();
extern void Test_CompileTime_TypeQualifier ();
extern void Test_CompileTime_MainType ();
extern void Test_CompileTime_StringToID ();
extern void Test_CompileTime_Sequence ();
extern void Test_CompileTime_Map ();

extern void Test_Runtime_VirtualTypelist ();

extern void Test_Containers_Array ();
extern void Test_Containers_Queue ();
extern void Test_Containers_CircularQueue ();
extern void Test_Containers_String ();
extern void Test_Containers_Set ();
extern void Test_Containers_List ();
extern void Test_Containers_Map ();
extern void Test_Containers_HashSet ();
extern void Test_Containers_IndexedArray ();
extern void Test_Containers_Adaptors ();
extern void Test_Containers_Tuple ();

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
extern void Test_Math_Frustum ();
extern void Test_Math_Plane ();
extern void Test_Math_OverflowCheck ();

extern void Test_Types_FileAddress ();
extern void Test_Types_Function ();
extern void Test_Types_StringParser ();
extern void Test_Types_Union ();
extern void Test_Types_Cast ();
extern void Test_Types_Time ();
extern void Test_Type_Optional ();

extern void Test_Algorithms_InvokeWithVariant ();
extern void Test_Algorithms_Range ();

extern void Test_OS_Atomic ();
extern void Test_OS_Date ();
extern void Test_OS_FileSystem ();

extern void Test_Temp ();


int main ()
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
	Test_CompileTime_Sequence();
	Test_CompileTime_Map();

	Test_Runtime_VirtualTypelist();

	Test_Containers_Array();
	Test_Containers_Queue();
	Test_Containers_CircularQueue();
	Test_Containers_String();
	Test_Containers_Set();
	Test_Containers_Map();
	Test_Containers_HashSet();
	Test_Containers_IndexedArray();
	Test_Containers_Adaptors();
	Test_Containers_Tuple();

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
	Test_Math_Frustum();
	Test_Math_Plane();
	Test_Math_OverflowCheck();

	Test_Types_FileAddress();
	Test_Types_Function();
	Test_Types_StringParser();
	Test_Types_Union();
	Test_Types_Cast();
	Test_Types_Time();
	Test_Type_Optional();

	Test_Algorithms_InvokeWithVariant();
	Test_Algorithms_Range();

	Test_OS_Atomic();
	Test_OS_Date();
	Test_OS_FileSystem();
	
	LOG( "Tests Finished!", ELog::Info );

	Logger::GetInstance()->Close();
	
	DEBUG_ONLY( DbgRefCountedObject::s_ChenckNotReleasedObjects() );
	DEBUG_ONLY( RefCounter2::s_CheckAllocations() );

	return 0;
}
