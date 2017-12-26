# auto generated file
cmake_minimum_required (VERSION 3.6.3)

project( "ModularGraphicsFramework" LANGUAGES CXX )
set_property( GLOBAL PROPERTY USE_FOLDERS ON )
set( ModularGraphicsFramework_VERSION_MAJOR 0 )
set( ModularGraphicsFramework_VERSION_MINOR 0 )

#==================================================================================================
# Visual Studio Compilation settings
#==================================================================================================
if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
	string( REPLACE "/EHa" " " CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}" )
	string( REPLACE "/EHsc" " " CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}" )
	string( REPLACE "/EHs" " " CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}" )
	string( REPLACE "//EHs-c-" " " CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}" )
	message( STATUS "CMAKE_C_FLAGS: ${CMAKE_C_FLAGS}" )
	message( STATUS "CMAKE_CXX_FLAGS: ${CMAKE_CXX_FLAGS}" )
	message( STATUS "CMAKE_EXE_LINKER_FLAGS: ${CMAKE_EXE_LINKER_FLAGS}" )
	message( STATUS "CMAKE_SHARED_LINKER_FLAGS: ${CMAKE_SHARED_LINKER_FLAGS}" )
	if (CMAKE_CONFIGURATION_TYPES)
		set( CMAKE_CONFIGURATION_TYPES DebugAnalyze Debug Profile Release )
		set( CMAKE_CONFIGURATION_TYPES "${CMAKE_CONFIGURATION_TYPES}" CACHE STRING "Configurations" FORCE )
	endif()

	# DebugAnalyze
	set_property (DIRECTORY APPEND PROPERTY COMPILE_DEFINITIONS $<$<CONFIG:DebugAnalyze>: _DEBUG >)
	set (CMAKE_C_FLAGS_DEBUGANALYZE "${CMAKE_C_FLAGS} ")
	set (CMAKE_CXX_FLAGS_DEBUGANALYZE "${CMAKE_CXX_FLAGS} ")
	set (CMAKE_EXE_LINKER_FLAGS_DEBUGANALYZE "${CMAKE_EXE_LINKER_FLAGS} ")
	set (CMAKE_SHARED_LINKER_FLAGS_DEBUGANALYZE "${CMAKE_EXE_LINKER_FLAGS} ")
	# Debug
	set_property (DIRECTORY APPEND PROPERTY COMPILE_DEFINITIONS $<$<CONFIG:Debug>: _DEBUG >)
	set (CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS} ")
	set (CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS} ")
	set (CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS} ")
	set (CMAKE_SHARED_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS} ")
	# Profile
	set_property (DIRECTORY APPEND PROPERTY COMPILE_DEFINITIONS $<$<CONFIG:Profile>: _NDEBUG NDEBUG >)
	set (CMAKE_C_FLAGS_PROFILE "${CMAKE_C_FLAGS} ")
	set (CMAKE_CXX_FLAGS_PROFILE "${CMAKE_CXX_FLAGS} ")
	set (CMAKE_EXE_LINKER_FLAGS_PROFILE "${CMAKE_EXE_LINKER_FLAGS} ")
	set (CMAKE_SHARED_LINKER_FLAGS_PROFILE "${CMAKE_EXE_LINKER_FLAGS} ")
	# Release
	set_property (DIRECTORY APPEND PROPERTY COMPILE_DEFINITIONS $<$<CONFIG:Release>: _NDEBUG NDEBUG >)
	set (CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS} ")
	set (CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS} ")
	set (CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS} ")
	set (CMAKE_SHARED_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS} ")
	set( CMAKE_BUILD_TYPE "Debug")

endif()


#==================================================================================================
# GCC Compilation settings
#==================================================================================================
if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
	message( STATUS "CMAKE_C_FLAGS: ${CMAKE_C_FLAGS}" )
	message( STATUS "CMAKE_CXX_FLAGS: ${CMAKE_CXX_FLAGS}" )
	message( STATUS "CMAKE_EXE_LINKER_FLAGS: ${CMAKE_EXE_LINKER_FLAGS}" )
	message( STATUS "CMAKE_SHARED_LINKER_FLAGS: ${CMAKE_SHARED_LINKER_FLAGS}" )
	if (CMAKE_CONFIGURATION_TYPES)
		set( CMAKE_CONFIGURATION_TYPES DebugAnalyze Debug Profile Release )
		set( CMAKE_CONFIGURATION_TYPES "${CMAKE_CONFIGURATION_TYPES}" CACHE STRING "Configurations" FORCE )
	endif()

	# DebugAnalyze
	set_property (DIRECTORY APPEND PROPERTY COMPILE_DEFINITIONS $<$<CONFIG:DebugAnalyze>: _DEBUG >)
	set (CMAKE_C_FLAGS_DEBUGANALYZE "${CMAKE_C_FLAGS} ")
	set (CMAKE_CXX_FLAGS_DEBUGANALYZE "${CMAKE_CXX_FLAGS} ")
	set (CMAKE_EXE_LINKER_FLAGS_DEBUGANALYZE "${CMAKE_EXE_LINKER_FLAGS} ")
	set (CMAKE_SHARED_LINKER_FLAGS_DEBUGANALYZE "${CMAKE_EXE_LINKER_FLAGS} ")
	# Debug
	set_property (DIRECTORY APPEND PROPERTY COMPILE_DEFINITIONS $<$<CONFIG:Debug>: _DEBUG >)
	set (CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS} ")
	set (CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS} ")
	set (CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS} ")
	set (CMAKE_SHARED_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS} ")
	# Profile
	set_property (DIRECTORY APPEND PROPERTY COMPILE_DEFINITIONS $<$<CONFIG:Profile>: _NDEBUG NDEBUG >)
	set (CMAKE_C_FLAGS_PROFILE "${CMAKE_C_FLAGS} ")
	set (CMAKE_CXX_FLAGS_PROFILE "${CMAKE_CXX_FLAGS} ")
	set (CMAKE_EXE_LINKER_FLAGS_PROFILE "${CMAKE_EXE_LINKER_FLAGS} ")
	set (CMAKE_SHARED_LINKER_FLAGS_PROFILE "${CMAKE_EXE_LINKER_FLAGS} ")
	# Release
	set_property (DIRECTORY APPEND PROPERTY COMPILE_DEFINITIONS $<$<CONFIG:Release>: _NDEBUG NDEBUG >)
	set (CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS} ")
	set (CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS} ")
	set (CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS} ")
	set (CMAKE_SHARED_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS} ")
	set( CMAKE_BUILD_TYPE "Debug")

endif()


#==================================================================================================
# Clang Compilation settings
#==================================================================================================
if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
	message( STATUS "CMAKE_C_FLAGS: ${CMAKE_C_FLAGS}" )
	message( STATUS "CMAKE_CXX_FLAGS: ${CMAKE_CXX_FLAGS}" )
	message( STATUS "CMAKE_EXE_LINKER_FLAGS: ${CMAKE_EXE_LINKER_FLAGS}" )
	message( STATUS "CMAKE_SHARED_LINKER_FLAGS: ${CMAKE_SHARED_LINKER_FLAGS}" )
	if (CMAKE_CONFIGURATION_TYPES)
		set( CMAKE_CONFIGURATION_TYPES DebugAnalyze Debug Profile Release )
		set( CMAKE_CONFIGURATION_TYPES "${CMAKE_CONFIGURATION_TYPES}" CACHE STRING "Configurations" FORCE )
	endif()

	# DebugAnalyze
	set_property (DIRECTORY APPEND PROPERTY COMPILE_DEFINITIONS $<$<CONFIG:DebugAnalyze>: _DEBUG >)
	set (CMAKE_C_FLAGS_DEBUGANALYZE "${CMAKE_C_FLAGS} ")
	set (CMAKE_CXX_FLAGS_DEBUGANALYZE "${CMAKE_CXX_FLAGS} ")
	set (CMAKE_EXE_LINKER_FLAGS_DEBUGANALYZE "${CMAKE_EXE_LINKER_FLAGS} ")
	set (CMAKE_SHARED_LINKER_FLAGS_DEBUGANALYZE "${CMAKE_EXE_LINKER_FLAGS} ")
	# Debug
	set_property (DIRECTORY APPEND PROPERTY COMPILE_DEFINITIONS $<$<CONFIG:Debug>: _DEBUG >)
	set (CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS} ")
	set (CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS} ")
	set (CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS} ")
	set (CMAKE_SHARED_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS} ")
	# Profile
	set_property (DIRECTORY APPEND PROPERTY COMPILE_DEFINITIONS $<$<CONFIG:Profile>: _NDEBUG NDEBUG >)
	set (CMAKE_C_FLAGS_PROFILE "${CMAKE_C_FLAGS} ")
	set (CMAKE_CXX_FLAGS_PROFILE "${CMAKE_CXX_FLAGS} ")
	set (CMAKE_EXE_LINKER_FLAGS_PROFILE "${CMAKE_EXE_LINKER_FLAGS} ")
	set (CMAKE_SHARED_LINKER_FLAGS_PROFILE "${CMAKE_EXE_LINKER_FLAGS} ")
	# Release
	set_property (DIRECTORY APPEND PROPERTY COMPILE_DEFINITIONS $<$<CONFIG:Release>: _NDEBUG NDEBUG >)
	set (CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS} ")
	set (CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS} ")
	set (CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS} ")
	set (CMAKE_SHARED_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS} ")
	set( CMAKE_BUILD_TYPE "Debug")

endif()


if (MSVC)
endif()

#==================================================================================================
# project: Engine.Config
#==================================================================================================
add_library( "Engine.Config" STATIC 
		"Engine/Config/Engine.Config.h"
		"Engine/Config/Engine.Version.h"
		"Engine/Config/Main.cpp"
		"Engine/Config/STL.Config.h" )

source_group( "" FILES "Engine/Config/Engine.Config.h" "Engine/Config/Engine.Version.h" "Engine/Config/Main.cpp" "Engine/Config/STL.Config.h" )
set_property( TARGET "Engine.Config" PROPERTY FOLDER "Engine" )
target_include_directories( "Engine.Config" PRIVATE "Engine/Config" )
target_include_directories( "Engine.Config" PUBLIC "Engine/.." )
# compiler
if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
	# DebugAnalyze
	target_compile_options ( "Engine.Config" PRIVATE $<$<CONFIG:DebugAnalyze>: /std:c++latest /MP /Gm- /Zc:inline /Gy- /fp:strict /fp:except- /we4002 /we4099 /we4129 /we4172 /we4201 /we4238 /we4239 /we4251 /we4263 /we4264 /we4266 /we4273 /we4473 /we4474 /we4480 /we4522 /we4552 /we4553 /we4700 /we4702 /we4715 /we4716 /we4717  /w14018 /w14100 /w14127 /w14189 /w14244 /w14245 /w14287 /w14389 /w14456 /w14457 /w14458 /w14459 /w14505 /w14668 /w14838 /w14946 /w14996 /w15038  /wd4061 /wd4062 /wd4063 /wd4310 /wd4365 /wd4503 /wd4514 /wd4623 /wd4625 /wd4626 /wd4710 /wd4714 /wd5026 /wd5027  /W4 /WX- /sdl /Od /Ob0 /EHsc /Oy- /GF- /GS /guard:cf /GR /MDd /analyze /Zi /RTCsu >)
	target_compile_definitions( "Engine.Config" PRIVATE $<$<CONFIG:DebugAnalyze>: __GX_DEBUG__ __GX_ANALYZE__ >)
	set_target_properties( "Engine.Config" PROPERTIES LINK_FLAGS_DEBUGANALYZE " /OPT:REF /OPT:ICF /INCREMENTAL:NO /DEBUG:FULL /LTCG" )
	# Debug
	target_compile_options ( "Engine.Config" PRIVATE $<$<CONFIG:Debug>: /std:c++latest /MP /Gm- /Zc:inline /Gy- /fp:strict /fp:except- /we4002 /we4099 /we4129 /we4172 /we4201 /we4238 /we4239 /we4251 /we4263 /we4264 /we4266 /we4273 /we4473 /we4474 /we4480 /we4522 /we4552 /we4553 /we4700 /we4702 /we4715 /we4716 /we4717  /w14018 /w14100 /w14127 /w14189 /w14244 /w14245 /w14287 /w14389 /w14456 /w14457 /w14458 /w14459 /w14505 /w14668 /w14838 /w14946 /w14996 /w15038  /wd4061 /wd4062 /wd4063 /wd4310 /wd4365 /wd4503 /wd4514 /wd4623 /wd4625 /wd4626 /wd4710 /wd4714 /wd5026 /wd5027  /W4 /WX- /sdl /Od /Ob0 /EHsc /Oy- /GF- /GS /GR /MDd /analyze- /Zi /RTCsu >)
	target_compile_definitions( "Engine.Config" PRIVATE $<$<CONFIG:Debug>: __GX_DEBUG__ >)
	set_target_properties( "Engine.Config" PROPERTIES LINK_FLAGS_DEBUG " /OPT:REF /OPT:ICF /INCREMENTAL:NO /DEBUG:FULL /LTCG" )
	# Profile
	target_compile_options ( "Engine.Config" PRIVATE $<$<CONFIG:Profile>: /std:c++latest /MP /Gm- /Zc:inline /Gy- /fp:strict /fp:except- /we4002 /we4099 /we4129 /we4172 /we4201 /we4238 /we4239 /we4251 /we4263 /we4264 /we4266 /we4273 /we4473 /we4474 /we4480 /we4522 /we4552 /we4553 /we4700 /we4702 /we4715 /we4716 /we4717  /w14018 /w14100 /w14127 /w14189 /w14244 /w14245 /w14287 /w14389 /w14456 /w14457 /w14458 /w14459 /w14505 /w14668 /w14838 /w14946 /w14996 /w15038  /wd4061 /wd4062 /wd4063 /wd4310 /wd4365 /wd4503 /wd4514 /wd4623 /wd4625 /wd4626 /wd4710 /wd4714 /wd5026 /wd5027  /Ob2 /Ob2 /Oi /Ot /Oy /MD /GT /GL /GF /GS- /W3 /Od /analyze- /EHsc /GR >)
	target_compile_definitions( "Engine.Config" PRIVATE $<$<CONFIG:Profile>: GX_ENABLE_PROFILING >)
	set_target_properties( "Engine.Config" PROPERTIES LINK_FLAGS_PROFILE " /OPT:REF /OPT:ICF /INCREMENTAL:NO /LTCG /DEBUG /PROFILE" )
	# Release
	target_compile_options ( "Engine.Config" PRIVATE $<$<CONFIG:Release>: /std:c++latest /MP /Gm- /Zc:inline /Gy- /fp:strict /fp:except- /we4002 /we4099 /we4129 /we4172 /we4201 /we4238 /we4239 /we4251 /we4263 /we4264 /we4266 /we4273 /we4473 /we4474 /we4480 /we4522 /we4552 /we4553 /we4700 /we4702 /we4715 /we4716 /we4717  /w14018  /w14127 /w14189 /w14244 /w14245 /w14287 /w14389 /w14456 /w14457 /w14458 /w14459 /w14505 /w14668 /w14838 /w14946 /w14996 /w15038  /wd4061 /wd4062 /wd4063 /wd4310 /wd4365 /wd4503 /wd4514 /wd4623 /wd4625 /wd4626 /wd4710 /wd4714 /wd5026 /wd5027  /Ob2 /Ob2 /Oi /Ot /Oy /MD /GT /GL /GF /GS- /W3 /Ox /analyze- /EHs-c- /GR- >)
	target_compile_definitions( "Engine.Config" PRIVATE $<$<CONFIG:Release>: __GX_FAST__ __GX_NO_EXCEPTIONS__ >)
	set_target_properties( "Engine.Config" PROPERTIES LINK_FLAGS_RELEASE " /OPT:REF /OPT:ICF /INCREMENTAL:NO /LTCG /RELEASE /DYNAMICBASE" )
endif()


if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
	# DebugAnalyze
	target_compile_options ( "Engine.Config" PRIVATE $<$<CONFIG:DebugAnalyze>: -std=c++1z -Wchar-subscripts -Wdouble-promotion -Wformat -Wmain -Wmissing-braces -Wmissing-include-dirs -Wunused -Wuninitialized -Wmaybe-uninitialized -Wunknown-pragmas -Wpragmas -Wstrict-aliasing -Wstrict-overflow -Wundef -Wendif-labels -Wfree-nonheap-object -Wpointer-arith -Wcast-qual -Wcast-align -Wwrite-strings -Wconversion-null -Wzero-as-null-pointer-constant -Wenum-compare -Wsign-compare -Wsizeof-pointer-memaccess -Wlogical-op -Werror=init-self -Werror=parentheses -Werror=return-local-addr -Werror=return-type -Werror=non-template-friend -Werror=array-bounds -Werror=div-by-zero -Werror=address -Werror=missing-field-initializers -Werror=placement-new  -Wno-non-template-friend  -ggdb -fsanitize=undefined -fchkp-check-incomplete-type -Og >)
	target_compile_definitions( "Engine.Config" PRIVATE $<$<CONFIG:DebugAnalyze>: __GX_DEBUG__ __GX_ANALYZE__ >)
	# Debug
	target_compile_options ( "Engine.Config" PRIVATE $<$<CONFIG:Debug>: -std=c++1z -Wchar-subscripts -Wdouble-promotion -Wformat -Wmain -Wmissing-braces -Wmissing-include-dirs -Wunused -Wuninitialized -Wmaybe-uninitialized -Wunknown-pragmas -Wpragmas -Wstrict-aliasing -Wstrict-overflow -Wundef -Wendif-labels -Wfree-nonheap-object -Wpointer-arith -Wcast-qual -Wcast-align -Wwrite-strings -Wconversion-null -Wzero-as-null-pointer-constant -Wenum-compare -Wsign-compare -Wsizeof-pointer-memaccess -Wlogical-op -Werror=init-self -Werror=parentheses -Werror=return-local-addr -Werror=return-type -Werror=non-template-friend -Werror=array-bounds -Werror=div-by-zero -Werror=address -Werror=missing-field-initializers -Werror=placement-new  -Wno-non-template-friend  -ggdb -fchkp-check-incomplete-type -Og >)
	target_compile_definitions( "Engine.Config" PRIVATE $<$<CONFIG:Debug>: __GX_DEBUG__ >)
	# Profile
	target_compile_options ( "Engine.Config" PRIVATE $<$<CONFIG:Profile>: -std=c++1z -Wchar-subscripts -Wdouble-promotion -Wformat -Wmain -Wmissing-braces -Wmissing-include-dirs -Wunused -Wuninitialized -Wmaybe-uninitialized -Wunknown-pragmas -Wpragmas -Wstrict-aliasing -Wstrict-overflow -Wundef -Wendif-labels -Wfree-nonheap-object -Wpointer-arith -Wcast-qual -Wcast-align -Wwrite-strings -Wconversion-null -Wzero-as-null-pointer-constant -Wenum-compare -Wsign-compare -Wsizeof-pointer-memaccess -Wlogical-op -Werror=init-self -Werror=parentheses -Werror=return-local-addr -Werror=return-type -Werror=non-template-friend -Werror=array-bounds -Werror=div-by-zero -Werror=address -Werror=missing-field-initializers -Werror=placement-new  -Wno-non-template-friend  -O2 -finline-functions >)
	target_compile_definitions( "Engine.Config" PRIVATE $<$<CONFIG:Profile>: GX_ENABLE_PROFILING >)
	# Release
	target_compile_options ( "Engine.Config" PRIVATE $<$<CONFIG:Release>: -std=c++1z -Wchar-subscripts -Wdouble-promotion -Wformat -Wmain -Wmissing-braces -Wmissing-include-dirs -Wunused -Wuninitialized -Wmaybe-uninitialized -Wunknown-pragmas -Wpragmas -Wstrict-aliasing -Wstrict-overflow -Wundef -Wendif-labels -Wfree-nonheap-object -Wpointer-arith -Wcast-qual -Wcast-align -Wwrite-strings -Wconversion-null -Wzero-as-null-pointer-constant -Wenum-compare -Wsign-compare -Wsizeof-pointer-memaccess -Wlogical-op -Werror=init-self -Werror=parentheses -Werror=return-local-addr -Werror=return-type -Werror=non-template-friend -Werror=array-bounds -Werror=div-by-zero -Werror=address -Werror=missing-field-initializers -Werror=placement-new  -Wno-non-template-friend  -O3 -Ofast -fomit-frame-pointer -finline-functions >)
	target_compile_definitions( "Engine.Config" PRIVATE $<$<CONFIG:Release>: __GX_FAST__ __GX_NO_EXCEPTIONS__ >)
endif()


if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
	target_compile_definitions( "Engine.Config" PRIVATE $<$<CONFIG:DebugAnalyze>: __GX_DEBUG__ __GX_ANALYZE__ >)
	target_compile_definitions( "Engine.Config" PRIVATE $<$<CONFIG:Debug>: __GX_DEBUG__ >)
	target_compile_definitions( "Engine.Config" PRIVATE $<$<CONFIG:Profile>: GX_ENABLE_PROFILING >)
	target_compile_definitions( "Engine.Config" PRIVATE $<$<CONFIG:Release>: __GX_FAST__ __GX_NO_EXCEPTIONS__ >)
endif()





#==================================================================================================
# project: Engine.STL
#==================================================================================================
add_library( "Engine.STL" STATIC 
		"Engine/STL/Algorithms/ArrayUtils.h"
		"Engine/STL/Algorithms/Comparators.h"
		"Engine/STL/Algorithms/Enum.h"
		"Engine/STL/Algorithms/FileAddress.cpp"
		"Engine/STL/Algorithms/FileAddress.h"
		"Engine/STL/Algorithms/Hash.h"
		"Engine/STL/Algorithms/InvokeWithVariant.h"
		"Engine/STL/Algorithms/Sorts.h"
		"Engine/STL/Algorithms/StringParser.cpp"
		"Engine/STL/Algorithms/StringParser.h"
		"Engine/STL/Algorithms/StringUtils.h"
		"Engine/STL/Algorithms/Swap.h"
		"Engine/STL/Types/Bitfield.h"
		"Engine/STL/Types/DefferedType.h"
		"Engine/STL/Types/Delegate.h"
		"Engine/STL/Types/Function.h"
		"Engine/STL/Types/Limit.h"
		"Engine/STL/Types/Noncopyable.h"
		"Engine/STL/Types/Noninstancable.h"
		"Engine/STL/Types/Optional.h"
		"Engine/STL/Types/Ptr.h"
		"Engine/STL/Types/RefCountedObject.h"
		"Engine/STL/Types/RefCounter.h"
		"Engine/STL/Types/ScopeSetter.h"
		"Engine/STL/Types/SharedPointer.h"
		"Engine/STL/Types/StaticLimit.h"
		"Engine/STL/Types/StaticRefCountedObject.h"
		"Engine/STL/Types/Union.h"
		"Engine/STL/Types/UniquePtr.h"
		"Engine/STL/Types/VariantCRef.h"
		"Engine/STL/Types/VariantInterface.h"
		"Engine/STL/Types/VariantRef.h"
		"Engine/STL/Types/VariantRefIndexedContainer.h"
		"Engine/STL/Types/WeakPointer.h"
		"Engine/STL/Dimensions/ByteAndBit.h"
		"Engine/STL/Dimensions/Percentage.h"
		"Engine/STL/Dimensions/PhysicsDimension.h"
		"Engine/STL/Dimensions/PhysicsValue.h"
		"Engine/STL/Dimensions/PhysicsValueMath.h"
		"Engine/STL/Dimensions/PhysicsValueUtils.h"
		"Engine/STL/Dimensions/PhysicsValueVec.h"
		"Engine/STL/Dimensions/PhysicsValueVecI.h"
		"Engine/STL/Dimensions/PowerOfTwoValue.h"
		"Engine/STL/Dimensions/RadiansAndDegrees.h"
		"Engine/STL/Log/ELog.h"
		"Engine/STL/Log/Logger.cpp"
		"Engine/STL/Log/Logger.h"
		"Engine/STL/Log/ToString.h"
		"Engine/STL/Java/Java.cpp"
		"Engine/STL/Java/JavaArray.h"
		"Engine/STL/Java/JavaClass.h"
		"Engine/STL/Java/JavaEnvironment.h"
		"Engine/STL/Java/JavaMethod.h"
		"Engine/STL/Java/JavaMethodType.h"
		"Engine/STL/Java/JavaString.h"
		"Engine/STL/Java/JavaTypeInfo.h"
		"Engine/STL/Math/2D/Circle.h"
		"Engine/STL/Math/2D/Line2.h"
		"Engine/STL/Math/2D/MathTypes2D.h"
		"Engine/STL/Math/2D/OrientedRectangle.h"
		"Engine/STL/Math/2D/Rectangle.h"
		"Engine/STL/OS/STD/StdThread.cpp"
		"Engine/STL/OS/STD/Thread.h"
		"Engine/STL/OS/STD/Timer.h"
		"Engine/STL/CompileTime/Runtime/TypeIdList.h"
		"Engine/STL/Math/Image/ImageUtils.h"
		"Engine/STL/Math/Rand/NormalDistribution.h"
		"Engine/STL/Math/Rand/Pseudorandom.h"
		"Engine/STL/Math/Rand/RandEngine.h"
		"Engine/STL/Math/Rand/Random.h"
		"Engine/STL/Math/Rand/RandomWithChance.h"
		"Engine/STL/Time/FloatTimeImpl.h"
		"Engine/STL/Time/IntTimeImpl.h"
		"Engine/STL/Time/Time.h"
		"Engine/STL/Time/TimeProfiler.h"
		"Engine/STL/Math/Spline/Spline.h"
		"Engine/STL/OS/Windows/AtomicOp.h"
		"Engine/STL/OS/Windows/FileSystem.cpp"
		"Engine/STL/OS/Windows/FileSystem.h"
		"Engine/STL/OS/Windows/Library.h"
		"Engine/STL/OS/Windows/OSWindows.h"
		"Engine/STL/OS/Windows/PlatformUtils.h"
		"Engine/STL/OS/Windows/RandDevice.h"
		"Engine/STL/OS/Windows/SyncPrimitives.h"
		"Engine/STL/OS/Windows/Thread.h"
		"Engine/STL/OS/Windows/Timer.cpp"
		"Engine/STL/OS/Windows/Timer.h"
		"Engine/STL/OS/Windows/WinAtomicOp.cpp"
		"Engine/STL/OS/Windows/WinHeader.h"
		"Engine/STL/OS/Windows/WinLibrary.cpp"
		"Engine/STL/OS/Windows/WinPlatformUtils.cpp"
		"Engine/STL/OS/Windows/WinRandDevice.cpp"
		"Engine/STL/OS/Windows/WinSyncPrimitives.cpp"
		"Engine/STL/OS/Windows/WinThread.cpp"
		"Engine/STL/ThreadSafe/Atomic.h"
		"Engine/STL/ThreadSafe/AtomicPtr.h"
		"Engine/STL/ThreadSafe/MtFile.h"
		"Engine/STL/ThreadSafe/MtQueue.h"
		"Engine/STL/ThreadSafe/Singleton.h"
		"Engine/STL/OS/Posix/AtomicOp.h"
		"Engine/STL/OS/Posix/FileSystem.h"
		"Engine/STL/OS/Posix/Library.h"
		"Engine/STL/OS/Posix/OSPosix.h"
		"Engine/STL/OS/Posix/PlatformUtils.h"
		"Engine/STL/OS/Posix/RandDevice.h"
		"Engine/STL/OS/Posix/SyncPrimitives.h"
		"Engine/STL/OS/Posix/Thread.h"
		"Engine/STL/OS/Posix/Timer.h"
		"Engine/STL/OS/Android/OSAndroid.h"
		"Engine/STL/Math/Algebra.h"
		"Engine/STL/Math/BinaryMath.h"
		"Engine/STL/Math/Interpolations.h"
		"Engine/STL/Math/MathConstants.h"
		"Engine/STL/Math/Mathematics.h"
		"Engine/STL/Math/MathFunc.h"
		"Engine/STL/Math/MathTypeCast.h"
		"Engine/STL/Math/MathTypes.h"
		"Engine/STL/Math/Matrix.h"
		"Engine/STL/Math/Matrix2.h"
		"Engine/STL/Math/Matrix3.h"
		"Engine/STL/Math/Matrix4.h"
		"Engine/STL/Math/MatrixCR.h"
		"Engine/STL/Math/MatrixUtils.h"
		"Engine/STL/Math/Quaternion.h"
		"Engine/STL/Math/Trigonometry.h"
		"Engine/STL/Math/Vec.h"
		"Engine/STL/Math/VecI.h"
		"Engine/STL/OS/Base/BaseFileSystem.cpp"
		"Engine/STL/OS/Base/BaseFileSystem.h"
		"Engine/STL/OS/Base/Common.h"
		"Engine/STL/OS/Base/ConditionVariableEmulation.h"
		"Engine/STL/OS/Base/Date.cpp"
		"Engine/STL/OS/Base/Date.h"
		"Engine/STL/OS/Base/Endianes.h"
		"Engine/STL/OS/Base/ReadWriteSyncEmulation.h"
		"Engine/STL/OS/Base/ScopeLock.h"
		"Engine/STL/OS/Base/SemaphoreEmulator.h"
		"Engine/STL/OS/Base/SyncEventEmulation.h"
		"Engine/STL/OS/SDL/FileSystem.h"
		"Engine/STL/OS/SDL/Library.h"
		"Engine/STL/OS/SDL/OS_SDL.h"
		"Engine/STL/OS/SDL/RandDevice.h"
		"Engine/STL/OS/SDL/SdlThread.cpp"
		"Engine/STL/OS/SDL/SDLUtils.cpp"
		"Engine/STL/OS/SDL/SDLUtils.h"
		"Engine/STL/OS/SDL/SyncPrimitives.h"
		"Engine/STL/OS/SDL/Thread.h"
		"Engine/STL/OS/SDL/Timer.h"
		"Engine/STL/Defines/AuxiliaryDefines.h"
		"Engine/STL/Defines/Defines.h"
		"Engine/STL/Defines/EnumHelpers.h"
		"Engine/STL/Defines/Errors.h"
		"Engine/STL/Defines/MemberDetector.h"
		"Engine/STL/Defines/OperatorHelpers.h"
		"Engine/STL/Defines/PublicMacro.h"
		"Engine/STL/Algorithms/Crypt/SimpleCrypt.h"
		"Engine/STL/Common/AllFunc.h"
		"Engine/STL/Common/Cast.h"
		"Engine/STL/Common/Init.h"
		"Engine/STL/Common/Main.cpp"
		"Engine/STL/Common/Platforms.h"
		"Engine/STL/Common/TypeId.h"
		"Engine/STL/Common/Types.h"
		"Engine/STL/Common/UMax.h"
		"Engine/STL/Common/Uninitialized.h"
		"Engine/STL/Math/Color/Color.h"
		"Engine/STL/Math/Color/ColorFormats.h"
		"Engine/STL/Math/Color/Half.h"
		"Engine/STL/Math/Color/TR11G11B10F.h"
		"Engine/STL/Math/Color/TRGB9_E5.h"
		"Engine/STL/Files/BaseFile.h"
		"Engine/STL/Files/CryptFile.h"
		"Engine/STL/Files/HDDFile.h"
		"Engine/STL/Files/LzmaFile.h"
		"Engine/STL/Files/MemFile.h"
		"Engine/STL/Files/SubFile.h"
		"Engine/STL/Files/ZipFile.h"
		"Engine/STL/Containers/Adaptors.h"
		"Engine/STL/Containers/AppendableAdaptor.h"
		"Engine/STL/Containers/Array.h"
		"Engine/STL/Containers/ArrayRef.h"
		"Engine/STL/Containers/CircularQueue.h"
		"Engine/STL/Containers/CopyStrategy.h"
		"Engine/STL/Containers/ErasableAdaptor.h"
		"Engine/STL/Containers/HashMap.h"
		"Engine/STL/Containers/HashSet.h"
		"Engine/STL/Containers/IndexedArray.h"
		"Engine/STL/Containers/Map.h"
		"Engine/STL/Containers/MapUtils.h"
		"Engine/STL/Containers/Pair.h"
		"Engine/STL/Containers/Queue.h"
		"Engine/STL/Containers/Set.h"
		"Engine/STL/Containers/Stack.h"
		"Engine/STL/Containers/StaticArray.h"
		"Engine/STL/Containers/StaticBitArray.h"
		"Engine/STL/Containers/String.h"
		"Engine/STL/Containers/StringRef.h"
		"Engine/STL/Containers/UniBuffer.h"
		"Engine/STL/DataBase/SimpleDB.h"
		"Engine/STL/DataBase/Utf8StringUtils.cpp"
		"Engine/STL/DataBase/Utf8StringUtils.h"
		"Engine/STL/Common/Compilers/CompilerGCC.h"
		"Engine/STL/Common/Compilers/CompilerMSVC.h"
		"Engine/STL/Engine.STL.h"
		"Engine/STL/ScriptBindings/DefaultBindings.cpp"
		"Engine/STL/ScriptBindings/DefaultBindings.h"
		"Engine/STL/ScriptBindings/ScriptBinder.cpp"
		"Engine/STL/ScriptBindings/ScriptBinder.h"
		"Engine/STL/ScriptBindings/ScriptEngine.cpp"
		"Engine/STL/ScriptBindings/ScriptEngine.h"
		"Engine/STL/ScriptBindings/ScriptEnum.h"
		"Engine/STL/ScriptBindings/ScriptTypes.h"
		"Engine/STL/Experimental/FastMath.h"
		"Engine/STL/Experimental/Unroll.h"
		"Engine/STL/Memory/Allocators.h"
		"Engine/STL/Memory/MemFunc.h"
		"Engine/STL/Memory/MemoryContainer.h"
		"Engine/STL/Memory/MemoryViewer.h"
		"Engine/STL/Memory/MemPointer.h"
		"Engine/STL/Memory/PlacementNew.h"
		"Engine/STL/OS/OSLowLevel.h"
		"Engine/STL/Algorithms/Filters/GaussianFilter.h"
		"Engine/STL/CompileTime/CompileTimeChecks.h"
		"Engine/STL/CompileTime/CompileTimeDebug.h"
		"Engine/STL/CompileTime/CompileTimeTypes.h"
		"Engine/STL/CompileTime/FunctionInfo.h"
		"Engine/STL/CompileTime/Map.h"
		"Engine/STL/CompileTime/NearType.h"
		"Engine/STL/CompileTime/NewTypeInfo.h"
		"Engine/STL/CompileTime/OldTypeInfo.h"
		"Engine/STL/CompileTime/PlainOldDataType.h"
		"Engine/STL/CompileTime/Sequence.h"
		"Engine/STL/CompileTime/StaticFloat.h"
		"Engine/STL/CompileTime/StaticFractional.h"
		"Engine/STL/CompileTime/StaticInteger.h"
		"Engine/STL/CompileTime/StringToID.h"
		"Engine/STL/CompileTime/TemplateMath.h"
		"Engine/STL/CompileTime/TemplateUtils.h"
		"Engine/STL/CompileTime/TypeInfoFunc.h"
		"Engine/STL/CompileTime/TypeList.h"
		"Engine/STL/CompileTime/TypeListHelpers.h"
		"Engine/STL/CompileTime/TypeListUtils.h"
		"Engine/STL/CompileTime/TypeQualifier.h"
		"Engine/STL/CompileTime/TypeTraits.h"
		"Engine/STL/Math/3D/AxisAlignedBox.h"
		"Engine/STL/Math/3D/CoordTransform3.h"
		"Engine/STL/Math/3D/Frustum.h"
		"Engine/STL/Math/3D/Line3.h"
		"Engine/STL/Math/3D/MathTypes3D.h"
		"Engine/STL/Math/3D/PerspectiveCamera.h"
		"Engine/STL/Math/3D/Plane.h"
		"Engine/STL/Math/3D/Transform.h"
		"Engine/STL/Math/3D/Triangle.h" )

source_group( "Algorithms" FILES "Engine/STL/Algorithms/ArrayUtils.h" "Engine/STL/Algorithms/Comparators.h" "Engine/STL/Algorithms/Enum.h" "Engine/STL/Algorithms/FileAddress.cpp" "Engine/STL/Algorithms/FileAddress.h" "Engine/STL/Algorithms/Hash.h" "Engine/STL/Algorithms/InvokeWithVariant.h" "Engine/STL/Algorithms/Sorts.h" "Engine/STL/Algorithms/StringParser.cpp" "Engine/STL/Algorithms/StringParser.h" "Engine/STL/Algorithms/StringUtils.h" "Engine/STL/Algorithms/Swap.h" )
source_group( "Types" FILES "Engine/STL/Types/Bitfield.h" "Engine/STL/Types/DefferedType.h" "Engine/STL/Types/Delegate.h" "Engine/STL/Types/Function.h" "Engine/STL/Types/Limit.h" "Engine/STL/Types/Noncopyable.h" "Engine/STL/Types/Noninstancable.h" "Engine/STL/Types/Optional.h" "Engine/STL/Types/Ptr.h" "Engine/STL/Types/RefCountedObject.h" "Engine/STL/Types/RefCounter.h" "Engine/STL/Types/ScopeSetter.h" "Engine/STL/Types/SharedPointer.h" "Engine/STL/Types/StaticLimit.h" "Engine/STL/Types/StaticRefCountedObject.h" "Engine/STL/Types/Union.h" "Engine/STL/Types/UniquePtr.h" "Engine/STL/Types/VariantCRef.h" "Engine/STL/Types/VariantInterface.h" "Engine/STL/Types/VariantRef.h" "Engine/STL/Types/VariantRefIndexedContainer.h" "Engine/STL/Types/WeakPointer.h" )
source_group( "Dimensions" FILES "Engine/STL/Dimensions/ByteAndBit.h" "Engine/STL/Dimensions/Percentage.h" "Engine/STL/Dimensions/PhysicsDimension.h" "Engine/STL/Dimensions/PhysicsValue.h" "Engine/STL/Dimensions/PhysicsValueMath.h" "Engine/STL/Dimensions/PhysicsValueUtils.h" "Engine/STL/Dimensions/PhysicsValueVec.h" "Engine/STL/Dimensions/PhysicsValueVecI.h" "Engine/STL/Dimensions/PowerOfTwoValue.h" "Engine/STL/Dimensions/RadiansAndDegrees.h" )
source_group( "Log" FILES "Engine/STL/Log/ELog.h" "Engine/STL/Log/Logger.cpp" "Engine/STL/Log/Logger.h" "Engine/STL/Log/ToString.h" )
source_group( "Java" FILES "Engine/STL/Java/Java.cpp" "Engine/STL/Java/JavaArray.h" "Engine/STL/Java/JavaClass.h" "Engine/STL/Java/JavaEnvironment.h" "Engine/STL/Java/JavaMethod.h" "Engine/STL/Java/JavaMethodType.h" "Engine/STL/Java/JavaString.h" "Engine/STL/Java/JavaTypeInfo.h" )
source_group( "Math\\2D" FILES "Engine/STL/Math/2D/Circle.h" "Engine/STL/Math/2D/Line2.h" "Engine/STL/Math/2D/MathTypes2D.h" "Engine/STL/Math/2D/OrientedRectangle.h" "Engine/STL/Math/2D/Rectangle.h" )
source_group( "OS\\STD" FILES "Engine/STL/OS/STD/StdThread.cpp" "Engine/STL/OS/STD/Thread.h" "Engine/STL/OS/STD/Timer.h" )
source_group( "CompileTime\\Runtime" FILES "Engine/STL/CompileTime/Runtime/TypeIdList.h" )
source_group( "Math\\Image" FILES "Engine/STL/Math/Image/ImageUtils.h" )
source_group( "Math\\Rand" FILES "Engine/STL/Math/Rand/NormalDistribution.h" "Engine/STL/Math/Rand/Pseudorandom.h" "Engine/STL/Math/Rand/RandEngine.h" "Engine/STL/Math/Rand/Random.h" "Engine/STL/Math/Rand/RandomWithChance.h" )
source_group( "Time" FILES "Engine/STL/Time/FloatTimeImpl.h" "Engine/STL/Time/IntTimeImpl.h" "Engine/STL/Time/Time.h" "Engine/STL/Time/TimeProfiler.h" )
source_group( "Math\\Spline" FILES "Engine/STL/Math/Spline/Spline.h" )
source_group( "OS\\Windows" FILES "Engine/STL/OS/Windows/AtomicOp.h" "Engine/STL/OS/Windows/FileSystem.cpp" "Engine/STL/OS/Windows/FileSystem.h" "Engine/STL/OS/Windows/Library.h" "Engine/STL/OS/Windows/OSWindows.h" "Engine/STL/OS/Windows/PlatformUtils.h" "Engine/STL/OS/Windows/RandDevice.h" "Engine/STL/OS/Windows/SyncPrimitives.h" "Engine/STL/OS/Windows/Thread.h" "Engine/STL/OS/Windows/Timer.cpp" "Engine/STL/OS/Windows/Timer.h" "Engine/STL/OS/Windows/WinAtomicOp.cpp" "Engine/STL/OS/Windows/WinHeader.h" "Engine/STL/OS/Windows/WinLibrary.cpp" "Engine/STL/OS/Windows/WinPlatformUtils.cpp" "Engine/STL/OS/Windows/WinRandDevice.cpp" "Engine/STL/OS/Windows/WinSyncPrimitives.cpp" "Engine/STL/OS/Windows/WinThread.cpp" )
source_group( "ThreadSafe" FILES "Engine/STL/ThreadSafe/Atomic.h" "Engine/STL/ThreadSafe/AtomicPtr.h" "Engine/STL/ThreadSafe/MtFile.h" "Engine/STL/ThreadSafe/MtQueue.h" "Engine/STL/ThreadSafe/Singleton.h" )
source_group( "OS\\Posix" FILES "Engine/STL/OS/Posix/AtomicOp.h" "Engine/STL/OS/Posix/FileSystem.h" "Engine/STL/OS/Posix/Library.h" "Engine/STL/OS/Posix/OSPosix.h" "Engine/STL/OS/Posix/PlatformUtils.h" "Engine/STL/OS/Posix/RandDevice.h" "Engine/STL/OS/Posix/SyncPrimitives.h" "Engine/STL/OS/Posix/Thread.h" "Engine/STL/OS/Posix/Timer.h" )
source_group( "OS\\Android" FILES "Engine/STL/OS/Android/OSAndroid.h" )
source_group( "Math" FILES "Engine/STL/Math/Algebra.h" "Engine/STL/Math/BinaryMath.h" "Engine/STL/Math/Interpolations.h" "Engine/STL/Math/MathConstants.h" "Engine/STL/Math/Mathematics.h" "Engine/STL/Math/MathFunc.h" "Engine/STL/Math/MathTypeCast.h" "Engine/STL/Math/MathTypes.h" "Engine/STL/Math/Matrix.h" "Engine/STL/Math/Matrix2.h" "Engine/STL/Math/Matrix3.h" "Engine/STL/Math/Matrix4.h" "Engine/STL/Math/MatrixCR.h" "Engine/STL/Math/MatrixUtils.h" "Engine/STL/Math/Quaternion.h" "Engine/STL/Math/Trigonometry.h" "Engine/STL/Math/Vec.h" "Engine/STL/Math/VecI.h" )
source_group( "OS\\Base" FILES "Engine/STL/OS/Base/BaseFileSystem.cpp" "Engine/STL/OS/Base/BaseFileSystem.h" "Engine/STL/OS/Base/Common.h" "Engine/STL/OS/Base/ConditionVariableEmulation.h" "Engine/STL/OS/Base/Date.cpp" "Engine/STL/OS/Base/Date.h" "Engine/STL/OS/Base/Endianes.h" "Engine/STL/OS/Base/ReadWriteSyncEmulation.h" "Engine/STL/OS/Base/ScopeLock.h" "Engine/STL/OS/Base/SemaphoreEmulator.h" "Engine/STL/OS/Base/SyncEventEmulation.h" )
source_group( "OS\\SDL" FILES "Engine/STL/OS/SDL/FileSystem.h" "Engine/STL/OS/SDL/Library.h" "Engine/STL/OS/SDL/OS_SDL.h" "Engine/STL/OS/SDL/RandDevice.h" "Engine/STL/OS/SDL/SdlThread.cpp" "Engine/STL/OS/SDL/SDLUtils.cpp" "Engine/STL/OS/SDL/SDLUtils.h" "Engine/STL/OS/SDL/SyncPrimitives.h" "Engine/STL/OS/SDL/Thread.h" "Engine/STL/OS/SDL/Timer.h" )
source_group( "Defines" FILES "Engine/STL/Defines/AuxiliaryDefines.h" "Engine/STL/Defines/Defines.h" "Engine/STL/Defines/EnumHelpers.h" "Engine/STL/Defines/Errors.h" "Engine/STL/Defines/MemberDetector.h" "Engine/STL/Defines/OperatorHelpers.h" "Engine/STL/Defines/PublicMacro.h" )
source_group( "Algorithms\\Crypt" FILES "Engine/STL/Algorithms/Crypt/SimpleCrypt.h" )
source_group( "Common" FILES "Engine/STL/Common/AllFunc.h" "Engine/STL/Common/Cast.h" "Engine/STL/Common/Init.h" "Engine/STL/Common/Main.cpp" "Engine/STL/Common/Platforms.h" "Engine/STL/Common/TypeId.h" "Engine/STL/Common/Types.h" "Engine/STL/Common/UMax.h" "Engine/STL/Common/Uninitialized.h" )
source_group( "Math\\Color" FILES "Engine/STL/Math/Color/Color.h" "Engine/STL/Math/Color/ColorFormats.h" "Engine/STL/Math/Color/Half.h" "Engine/STL/Math/Color/TR11G11B10F.h" "Engine/STL/Math/Color/TRGB9_E5.h" )
source_group( "Files" FILES "Engine/STL/Files/BaseFile.h" "Engine/STL/Files/CryptFile.h" "Engine/STL/Files/HDDFile.h" "Engine/STL/Files/LzmaFile.h" "Engine/STL/Files/MemFile.h" "Engine/STL/Files/SubFile.h" "Engine/STL/Files/ZipFile.h" )
source_group( "Containers" FILES "Engine/STL/Containers/Adaptors.h" "Engine/STL/Containers/AppendableAdaptor.h" "Engine/STL/Containers/Array.h" "Engine/STL/Containers/ArrayRef.h" "Engine/STL/Containers/CircularQueue.h" "Engine/STL/Containers/CopyStrategy.h" "Engine/STL/Containers/ErasableAdaptor.h" "Engine/STL/Containers/HashMap.h" "Engine/STL/Containers/HashSet.h" "Engine/STL/Containers/IndexedArray.h" "Engine/STL/Containers/Map.h" "Engine/STL/Containers/MapUtils.h" "Engine/STL/Containers/Pair.h" "Engine/STL/Containers/Queue.h" "Engine/STL/Containers/Set.h" "Engine/STL/Containers/Stack.h" "Engine/STL/Containers/StaticArray.h" "Engine/STL/Containers/StaticBitArray.h" "Engine/STL/Containers/String.h" "Engine/STL/Containers/StringRef.h" "Engine/STL/Containers/UniBuffer.h" )
source_group( "DataBase" FILES "Engine/STL/DataBase/SimpleDB.h" "Engine/STL/DataBase/Utf8StringUtils.cpp" "Engine/STL/DataBase/Utf8StringUtils.h" )
source_group( "Common\\Compilers" FILES "Engine/STL/Common/Compilers/CompilerGCC.h" "Engine/STL/Common/Compilers/CompilerMSVC.h" )
source_group( "" FILES "Engine/STL/Engine.STL.h" )
source_group( "ScriptBindings" FILES "Engine/STL/ScriptBindings/DefaultBindings.cpp" "Engine/STL/ScriptBindings/DefaultBindings.h" "Engine/STL/ScriptBindings/ScriptBinder.cpp" "Engine/STL/ScriptBindings/ScriptBinder.h" "Engine/STL/ScriptBindings/ScriptEngine.cpp" "Engine/STL/ScriptBindings/ScriptEngine.h" "Engine/STL/ScriptBindings/ScriptEnum.h" "Engine/STL/ScriptBindings/ScriptTypes.h" )
source_group( "Experimental" FILES "Engine/STL/Experimental/FastMath.h" "Engine/STL/Experimental/Unroll.h" )
source_group( "Memory" FILES "Engine/STL/Memory/Allocators.h" "Engine/STL/Memory/MemFunc.h" "Engine/STL/Memory/MemoryContainer.h" "Engine/STL/Memory/MemoryViewer.h" "Engine/STL/Memory/MemPointer.h" "Engine/STL/Memory/PlacementNew.h" )
source_group( "OS" FILES "Engine/STL/OS/OSLowLevel.h" )
source_group( "Algorithms\\Filters" FILES "Engine/STL/Algorithms/Filters/GaussianFilter.h" )
source_group( "CompileTime" FILES "Engine/STL/CompileTime/CompileTimeChecks.h" "Engine/STL/CompileTime/CompileTimeDebug.h" "Engine/STL/CompileTime/CompileTimeTypes.h" "Engine/STL/CompileTime/FunctionInfo.h" "Engine/STL/CompileTime/Map.h" "Engine/STL/CompileTime/NearType.h" "Engine/STL/CompileTime/NewTypeInfo.h" "Engine/STL/CompileTime/OldTypeInfo.h" "Engine/STL/CompileTime/PlainOldDataType.h" "Engine/STL/CompileTime/Sequence.h" "Engine/STL/CompileTime/StaticFloat.h" "Engine/STL/CompileTime/StaticFractional.h" "Engine/STL/CompileTime/StaticInteger.h" "Engine/STL/CompileTime/StringToID.h" "Engine/STL/CompileTime/TemplateMath.h" "Engine/STL/CompileTime/TemplateUtils.h" "Engine/STL/CompileTime/TypeInfoFunc.h" "Engine/STL/CompileTime/TypeList.h" "Engine/STL/CompileTime/TypeListHelpers.h" "Engine/STL/CompileTime/TypeListUtils.h" "Engine/STL/CompileTime/TypeQualifier.h" "Engine/STL/CompileTime/TypeTraits.h" )
source_group( "Math\\3D" FILES "Engine/STL/Math/3D/AxisAlignedBox.h" "Engine/STL/Math/3D/CoordTransform3.h" "Engine/STL/Math/3D/Frustum.h" "Engine/STL/Math/3D/Line3.h" "Engine/STL/Math/3D/MathTypes3D.h" "Engine/STL/Math/3D/PerspectiveCamera.h" "Engine/STL/Math/3D/Plane.h" "Engine/STL/Math/3D/Transform.h" "Engine/STL/Math/3D/Triangle.h" )
set_property( TARGET "Engine.STL" PROPERTY FOLDER "Engine" )
target_include_directories( "Engine.STL" PRIVATE "Engine/STL" )
target_include_directories( "Engine.STL" PUBLIC "Engine/.." )
target_link_libraries( "Engine.STL" "Engine.Config" )
# compiler
if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
	# DebugAnalyze
	target_compile_options ( "Engine.STL" PRIVATE $<$<CONFIG:DebugAnalyze>: /std:c++latest /MP /Gm- /Zc:inline /Gy- /fp:strict /fp:except- /we4002 /we4099 /we4129 /we4172 /we4201 /we4238 /we4239 /we4251 /we4263 /we4264 /we4266 /we4273 /we4473 /we4474 /we4480 /we4522 /we4552 /we4553 /we4700 /we4702 /we4715 /we4716 /we4717  /w14018 /w14100 /w14127 /w14189 /w14244 /w14245 /w14287 /w14389 /w14456 /w14457 /w14458 /w14459 /w14505 /w14668 /w14838 /w14946 /w14996 /w15038  /wd4061 /wd4062 /wd4063 /wd4310 /wd4365 /wd4503 /wd4514 /wd4623 /wd4625 /wd4626 /wd4710 /wd4714 /wd5026 /wd5027  /W4 /WX- /sdl /Od /Ob0 /EHsc /Oy- /GF- /GS /guard:cf /GR /MDd /analyze /Zi /RTCsu >)
	target_compile_definitions( "Engine.STL" PRIVATE $<$<CONFIG:DebugAnalyze>: __GX_DEBUG__ __GX_ANALYZE__ >)
	set_target_properties( "Engine.STL" PROPERTIES LINK_FLAGS_DEBUGANALYZE " /OPT:REF /OPT:ICF /INCREMENTAL:NO /DEBUG:FULL /LTCG" )
	# Debug
	target_compile_options ( "Engine.STL" PRIVATE $<$<CONFIG:Debug>: /std:c++latest /MP /Gm- /Zc:inline /Gy- /fp:strict /fp:except- /we4002 /we4099 /we4129 /we4172 /we4201 /we4238 /we4239 /we4251 /we4263 /we4264 /we4266 /we4273 /we4473 /we4474 /we4480 /we4522 /we4552 /we4553 /we4700 /we4702 /we4715 /we4716 /we4717  /w14018 /w14100 /w14127 /w14189 /w14244 /w14245 /w14287 /w14389 /w14456 /w14457 /w14458 /w14459 /w14505 /w14668 /w14838 /w14946 /w14996 /w15038  /wd4061 /wd4062 /wd4063 /wd4310 /wd4365 /wd4503 /wd4514 /wd4623 /wd4625 /wd4626 /wd4710 /wd4714 /wd5026 /wd5027  /W4 /WX- /sdl /Od /Ob0 /EHsc /Oy- /GF- /GS /GR /MDd /analyze- /Zi /RTCsu >)
	target_compile_definitions( "Engine.STL" PRIVATE $<$<CONFIG:Debug>: __GX_DEBUG__ >)
	set_target_properties( "Engine.STL" PROPERTIES LINK_FLAGS_DEBUG " /OPT:REF /OPT:ICF /INCREMENTAL:NO /DEBUG:FULL /LTCG" )
	# Profile
	target_compile_options ( "Engine.STL" PRIVATE $<$<CONFIG:Profile>: /std:c++latest /MP /Gm- /Zc:inline /Gy- /fp:strict /fp:except- /we4002 /we4099 /we4129 /we4172 /we4201 /we4238 /we4239 /we4251 /we4263 /we4264 /we4266 /we4273 /we4473 /we4474 /we4480 /we4522 /we4552 /we4553 /we4700 /we4702 /we4715 /we4716 /we4717  /w14018 /w14100 /w14127 /w14189 /w14244 /w14245 /w14287 /w14389 /w14456 /w14457 /w14458 /w14459 /w14505 /w14668 /w14838 /w14946 /w14996 /w15038  /wd4061 /wd4062 /wd4063 /wd4310 /wd4365 /wd4503 /wd4514 /wd4623 /wd4625 /wd4626 /wd4710 /wd4714 /wd5026 /wd5027  /Ob2 /Ob2 /Oi /Ot /Oy /MD /GT /GL /GF /GS- /W3 /Od /analyze- /EHsc /GR >)
	target_compile_definitions( "Engine.STL" PRIVATE $<$<CONFIG:Profile>: GX_ENABLE_PROFILING >)
	set_target_properties( "Engine.STL" PROPERTIES LINK_FLAGS_PROFILE " /OPT:REF /OPT:ICF /INCREMENTAL:NO /LTCG /DEBUG /PROFILE" )
	# Release
	target_compile_options ( "Engine.STL" PRIVATE $<$<CONFIG:Release>: /std:c++latest /MP /Gm- /Zc:inline /Gy- /fp:strict /fp:except- /we4002 /we4099 /we4129 /we4172 /we4201 /we4238 /we4239 /we4251 /we4263 /we4264 /we4266 /we4273 /we4473 /we4474 /we4480 /we4522 /we4552 /we4553 /we4700 /we4702 /we4715 /we4716 /we4717  /w14018  /w14127 /w14189 /w14244 /w14245 /w14287 /w14389 /w14456 /w14457 /w14458 /w14459 /w14505 /w14668 /w14838 /w14946 /w14996 /w15038  /wd4061 /wd4062 /wd4063 /wd4310 /wd4365 /wd4503 /wd4514 /wd4623 /wd4625 /wd4626 /wd4710 /wd4714 /wd5026 /wd5027  /Ob2 /Ob2 /Oi /Ot /Oy /MD /GT /GL /GF /GS- /W3 /Ox /analyze- /EHs-c- /GR- >)
	target_compile_definitions( "Engine.STL" PRIVATE $<$<CONFIG:Release>: __GX_FAST__ __GX_NO_EXCEPTIONS__ >)
	set_target_properties( "Engine.STL" PROPERTIES LINK_FLAGS_RELEASE " /OPT:REF /OPT:ICF /INCREMENTAL:NO /LTCG /RELEASE /DYNAMICBASE" )
endif()


if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
	# DebugAnalyze
	target_compile_options ( "Engine.STL" PRIVATE $<$<CONFIG:DebugAnalyze>: -std=c++1z -Wchar-subscripts -Wdouble-promotion -Wformat -Wmain -Wmissing-braces -Wmissing-include-dirs -Wunused -Wuninitialized -Wmaybe-uninitialized -Wunknown-pragmas -Wpragmas -Wstrict-aliasing -Wstrict-overflow -Wundef -Wendif-labels -Wfree-nonheap-object -Wpointer-arith -Wcast-qual -Wcast-align -Wwrite-strings -Wconversion-null -Wzero-as-null-pointer-constant -Wenum-compare -Wsign-compare -Wsizeof-pointer-memaccess -Wlogical-op -Werror=init-self -Werror=parentheses -Werror=return-local-addr -Werror=return-type -Werror=non-template-friend -Werror=array-bounds -Werror=div-by-zero -Werror=address -Werror=missing-field-initializers -Werror=placement-new  -Wno-non-template-friend  -ggdb -fsanitize=undefined -fchkp-check-incomplete-type -Og >)
	target_compile_definitions( "Engine.STL" PRIVATE $<$<CONFIG:DebugAnalyze>: __GX_DEBUG__ __GX_ANALYZE__ >)
	# Debug
	target_compile_options ( "Engine.STL" PRIVATE $<$<CONFIG:Debug>: -std=c++1z -Wchar-subscripts -Wdouble-promotion -Wformat -Wmain -Wmissing-braces -Wmissing-include-dirs -Wunused -Wuninitialized -Wmaybe-uninitialized -Wunknown-pragmas -Wpragmas -Wstrict-aliasing -Wstrict-overflow -Wundef -Wendif-labels -Wfree-nonheap-object -Wpointer-arith -Wcast-qual -Wcast-align -Wwrite-strings -Wconversion-null -Wzero-as-null-pointer-constant -Wenum-compare -Wsign-compare -Wsizeof-pointer-memaccess -Wlogical-op -Werror=init-self -Werror=parentheses -Werror=return-local-addr -Werror=return-type -Werror=non-template-friend -Werror=array-bounds -Werror=div-by-zero -Werror=address -Werror=missing-field-initializers -Werror=placement-new  -Wno-non-template-friend  -ggdb -fchkp-check-incomplete-type -Og >)
	target_compile_definitions( "Engine.STL" PRIVATE $<$<CONFIG:Debug>: __GX_DEBUG__ >)
	# Profile
	target_compile_options ( "Engine.STL" PRIVATE $<$<CONFIG:Profile>: -std=c++1z -Wchar-subscripts -Wdouble-promotion -Wformat -Wmain -Wmissing-braces -Wmissing-include-dirs -Wunused -Wuninitialized -Wmaybe-uninitialized -Wunknown-pragmas -Wpragmas -Wstrict-aliasing -Wstrict-overflow -Wundef -Wendif-labels -Wfree-nonheap-object -Wpointer-arith -Wcast-qual -Wcast-align -Wwrite-strings -Wconversion-null -Wzero-as-null-pointer-constant -Wenum-compare -Wsign-compare -Wsizeof-pointer-memaccess -Wlogical-op -Werror=init-self -Werror=parentheses -Werror=return-local-addr -Werror=return-type -Werror=non-template-friend -Werror=array-bounds -Werror=div-by-zero -Werror=address -Werror=missing-field-initializers -Werror=placement-new  -Wno-non-template-friend  -O2 -finline-functions >)
	target_compile_definitions( "Engine.STL" PRIVATE $<$<CONFIG:Profile>: GX_ENABLE_PROFILING >)
	# Release
	target_compile_options ( "Engine.STL" PRIVATE $<$<CONFIG:Release>: -std=c++1z -Wchar-subscripts -Wdouble-promotion -Wformat -Wmain -Wmissing-braces -Wmissing-include-dirs -Wunused -Wuninitialized -Wmaybe-uninitialized -Wunknown-pragmas -Wpragmas -Wstrict-aliasing -Wstrict-overflow -Wundef -Wendif-labels -Wfree-nonheap-object -Wpointer-arith -Wcast-qual -Wcast-align -Wwrite-strings -Wconversion-null -Wzero-as-null-pointer-constant -Wenum-compare -Wsign-compare -Wsizeof-pointer-memaccess -Wlogical-op -Werror=init-self -Werror=parentheses -Werror=return-local-addr -Werror=return-type -Werror=non-template-friend -Werror=array-bounds -Werror=div-by-zero -Werror=address -Werror=missing-field-initializers -Werror=placement-new  -Wno-non-template-friend  -O3 -Ofast -fomit-frame-pointer -finline-functions >)
	target_compile_definitions( "Engine.STL" PRIVATE $<$<CONFIG:Release>: __GX_FAST__ __GX_NO_EXCEPTIONS__ >)
endif()


if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
	target_compile_definitions( "Engine.STL" PRIVATE $<$<CONFIG:DebugAnalyze>: __GX_DEBUG__ __GX_ANALYZE__ >)
	target_compile_definitions( "Engine.STL" PRIVATE $<$<CONFIG:Debug>: __GX_DEBUG__ >)
	target_compile_definitions( "Engine.STL" PRIVATE $<$<CONFIG:Profile>: GX_ENABLE_PROFILING >)
	target_compile_definitions( "Engine.STL" PRIVATE $<$<CONFIG:Release>: __GX_FAST__ __GX_NO_EXCEPTIONS__ >)
endif()





#==================================================================================================
# project: Tests.STL
#==================================================================================================
add_executable( "Tests.STL" 
		"Tests/STL/Common.h"
		"Tests/STL/Debug.h"
		"Tests/STL/Main.cpp"
		"Tests/STL/Test_Algorithms_InvokeWithVariant.cpp"
		"Tests/STL/Test_CompileTime_MainType.cpp"
		"Tests/STL/Test_CompileTime_Map.cpp"
		"Tests/STL/Test_CompileTime_Sequence.cpp"
		"Tests/STL/Test_CompileTime_StaticFloat.cpp"
		"Tests/STL/Test_CompileTime_StringToID.cpp"
		"Tests/STL/Test_CompileTime_TemplateMath.cpp"
		"Tests/STL/Test_CompileTime_TypeInfo.cpp"
		"Tests/STL/Test_CompileTime_TypeList.cpp"
		"Tests/STL/Test_CompileTime_TypeQualifier.cpp"
		"Tests/STL/Test_CompileTime_TypeTraits.cpp"
		"Tests/STL/Test_Containers_Adaptors.cpp"
		"Tests/STL/Test_Containers_Array.cpp"
		"Tests/STL/Test_Containers_CircularQueue.cpp"
		"Tests/STL/Test_Containers_HashSet.cpp"
		"Tests/STL/Test_Containers_IndexedArray.cpp"
		"Tests/STL/Test_Containers_List.cpp"
		"Tests/STL/Test_Containers_Map.cpp"
		"Tests/STL/Test_Containers_Queue.cpp"
		"Tests/STL/Test_Containers_Set.cpp"
		"Tests/STL/Test_Containers_String.cpp"
		"Tests/STL/Test_Dimensions_PhysicsValue.cpp"
		"Tests/STL/Test_Math_Abs.cpp"
		"Tests/STL/Test_Math_Bit.cpp"
		"Tests/STL/Test_Math_Clamp_Wrap.cpp"
		"Tests/STL/Test_Math_Color.cpp"
		"Tests/STL/Test_Math_ColorFormat.cpp"
		"Tests/STL/Test_Math_Factorial.cpp"
		"Tests/STL/Test_Math_FloorCeilTruncRoundFract.cpp"
		"Tests/STL/Test_Math_ImageUtils.cpp"
		"Tests/STL/Test_Math_Matrix.cpp"
		"Tests/STL/Test_Math_Transform.cpp"
		"Tests/STL/Test_OS_Atomic.cpp"
		"Tests/STL/Test_OS_Date.cpp"
		"Tests/STL/Test_OS_FileSystem.cpp"
		"Tests/STL/Test_Runtime_VirtualTypelist.cpp"
		"Tests/STL/Test_SimpleScript.cpp"
		"Tests/STL/Test_Temp.cpp"
		"Tests/STL/Test_Types_Cast.cpp"
		"Tests/STL/Test_Types_ExtString.cpp"
		"Tests/STL/Test_Types_FileAddress.cpp"
		"Tests/STL/Test_Types_Function.cpp"
		"Tests/STL/Test_Types_StringParser.cpp"
		"Tests/STL/Test_Types_Time.cpp"
		"Tests/STL/Test_Types_Union.cpp" )

source_group( "" FILES "Tests/STL/Common.h" "Tests/STL/Debug.h" "Tests/STL/Main.cpp" "Tests/STL/Test_Algorithms_InvokeWithVariant.cpp" "Tests/STL/Test_CompileTime_MainType.cpp" "Tests/STL/Test_CompileTime_Map.cpp" "Tests/STL/Test_CompileTime_Sequence.cpp" "Tests/STL/Test_CompileTime_StaticFloat.cpp" "Tests/STL/Test_CompileTime_StringToID.cpp" "Tests/STL/Test_CompileTime_TemplateMath.cpp" "Tests/STL/Test_CompileTime_TypeInfo.cpp" "Tests/STL/Test_CompileTime_TypeList.cpp" "Tests/STL/Test_CompileTime_TypeQualifier.cpp" "Tests/STL/Test_CompileTime_TypeTraits.cpp" "Tests/STL/Test_Containers_Adaptors.cpp" "Tests/STL/Test_Containers_Array.cpp" "Tests/STL/Test_Containers_CircularQueue.cpp" "Tests/STL/Test_Containers_HashSet.cpp" "Tests/STL/Test_Containers_IndexedArray.cpp" "Tests/STL/Test_Containers_List.cpp" "Tests/STL/Test_Containers_Map.cpp" "Tests/STL/Test_Containers_Queue.cpp" "Tests/STL/Test_Containers_Set.cpp" "Tests/STL/Test_Containers_String.cpp" "Tests/STL/Test_Dimensions_PhysicsValue.cpp" "Tests/STL/Test_Math_Abs.cpp" "Tests/STL/Test_Math_Bit.cpp" "Tests/STL/Test_Math_Clamp_Wrap.cpp" "Tests/STL/Test_Math_Color.cpp" "Tests/STL/Test_Math_ColorFormat.cpp" "Tests/STL/Test_Math_Factorial.cpp" "Tests/STL/Test_Math_FloorCeilTruncRoundFract.cpp" "Tests/STL/Test_Math_ImageUtils.cpp" "Tests/STL/Test_Math_Matrix.cpp" "Tests/STL/Test_Math_Transform.cpp" "Tests/STL/Test_OS_Atomic.cpp" "Tests/STL/Test_OS_Date.cpp" "Tests/STL/Test_OS_FileSystem.cpp" "Tests/STL/Test_Runtime_VirtualTypelist.cpp" "Tests/STL/Test_SimpleScript.cpp" "Tests/STL/Test_Temp.cpp" "Tests/STL/Test_Types_Cast.cpp" "Tests/STL/Test_Types_ExtString.cpp" "Tests/STL/Test_Types_FileAddress.cpp" "Tests/STL/Test_Types_Function.cpp" "Tests/STL/Test_Types_StringParser.cpp" "Tests/STL/Test_Types_Time.cpp" "Tests/STL/Test_Types_Union.cpp" )
set_property( TARGET "Tests.STL" PROPERTY FOLDER "Tests" )
target_include_directories( "Tests.STL" PRIVATE "Tests/STL" )
target_include_directories( "Tests.STL" PUBLIC "Engine/.." )
target_link_libraries( "Tests.STL" "Engine.STL" )
# compiler
if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
	# DebugAnalyze
	target_compile_options ( "Tests.STL" PRIVATE $<$<CONFIG:DebugAnalyze>: /std:c++latest /MP /Gm- /Zc:inline /Gy- /fp:strict /fp:except- /we4002 /we4099 /we4129 /we4172 /we4201 /we4238 /we4239 /we4251 /we4263 /we4264 /we4266 /we4273 /we4473 /we4474 /we4480 /we4522 /we4552 /we4553 /we4700 /we4702 /we4715 /we4716 /we4717  /w14018 /w14100 /w14127 /w14189 /w14244 /w14245 /w14287 /w14389 /w14456 /w14457 /w14458 /w14459 /w14505 /w14668 /w14838 /w14946 /w14996 /w15038  /wd4061 /wd4062 /wd4063 /wd4310 /wd4365 /wd4503 /wd4514 /wd4623 /wd4625 /wd4626 /wd4710 /wd4714 /wd5026 /wd5027  /W4 /WX- /sdl /Od /Ob0 /EHsc /Oy- /GF- /GS /guard:cf /GR /MDd /analyze /Zi /RTCsu >)
	target_compile_definitions( "Tests.STL" PRIVATE $<$<CONFIG:DebugAnalyze>: __GX_DEBUG__ __GX_ANALYZE__ >)
	set_target_properties( "Tests.STL" PROPERTIES LINK_FLAGS_DEBUGANALYZE " /OPT:REF /OPT:ICF /INCREMENTAL:NO /DEBUG:FULL /LTCG" )
	# Debug
	target_compile_options ( "Tests.STL" PRIVATE $<$<CONFIG:Debug>: /std:c++latest /MP /Gm- /Zc:inline /Gy- /fp:strict /fp:except- /we4002 /we4099 /we4129 /we4172 /we4201 /we4238 /we4239 /we4251 /we4263 /we4264 /we4266 /we4273 /we4473 /we4474 /we4480 /we4522 /we4552 /we4553 /we4700 /we4702 /we4715 /we4716 /we4717  /w14018 /w14100 /w14127 /w14189 /w14244 /w14245 /w14287 /w14389 /w14456 /w14457 /w14458 /w14459 /w14505 /w14668 /w14838 /w14946 /w14996 /w15038  /wd4061 /wd4062 /wd4063 /wd4310 /wd4365 /wd4503 /wd4514 /wd4623 /wd4625 /wd4626 /wd4710 /wd4714 /wd5026 /wd5027  /W4 /WX- /sdl /Od /Ob0 /EHsc /Oy- /GF- /GS /GR /MDd /analyze- /Zi /RTCsu >)
	target_compile_definitions( "Tests.STL" PRIVATE $<$<CONFIG:Debug>: __GX_DEBUG__ >)
	set_target_properties( "Tests.STL" PROPERTIES LINK_FLAGS_DEBUG " /OPT:REF /OPT:ICF /INCREMENTAL:NO /DEBUG:FULL /LTCG" )
	# Profile
	target_compile_options ( "Tests.STL" PRIVATE $<$<CONFIG:Profile>: /std:c++latest /MP /Gm- /Zc:inline /Gy- /fp:strict /fp:except- /we4002 /we4099 /we4129 /we4172 /we4201 /we4238 /we4239 /we4251 /we4263 /we4264 /we4266 /we4273 /we4473 /we4474 /we4480 /we4522 /we4552 /we4553 /we4700 /we4702 /we4715 /we4716 /we4717  /w14018 /w14100 /w14127 /w14189 /w14244 /w14245 /w14287 /w14389 /w14456 /w14457 /w14458 /w14459 /w14505 /w14668 /w14838 /w14946 /w14996 /w15038  /wd4061 /wd4062 /wd4063 /wd4310 /wd4365 /wd4503 /wd4514 /wd4623 /wd4625 /wd4626 /wd4710 /wd4714 /wd5026 /wd5027  /Ob2 /Ob2 /Oi /Ot /Oy /MD /GT /GL /GF /GS- /W3 /Od /analyze- /EHsc /GR >)
	target_compile_definitions( "Tests.STL" PRIVATE $<$<CONFIG:Profile>: GX_ENABLE_PROFILING >)
	set_target_properties( "Tests.STL" PROPERTIES LINK_FLAGS_PROFILE " /OPT:REF /OPT:ICF /INCREMENTAL:NO /LTCG /DEBUG /PROFILE" )
	# Release
	target_compile_options ( "Tests.STL" PRIVATE $<$<CONFIG:Release>: /std:c++latest /MP /Gm- /Zc:inline /Gy- /fp:strict /fp:except- /we4002 /we4099 /we4129 /we4172 /we4201 /we4238 /we4239 /we4251 /we4263 /we4264 /we4266 /we4273 /we4473 /we4474 /we4480 /we4522 /we4552 /we4553 /we4700 /we4702 /we4715 /we4716 /we4717  /w14018  /w14127 /w14189 /w14244 /w14245 /w14287 /w14389 /w14456 /w14457 /w14458 /w14459 /w14505 /w14668 /w14838 /w14946 /w14996 /w15038  /wd4061 /wd4062 /wd4063 /wd4310 /wd4365 /wd4503 /wd4514 /wd4623 /wd4625 /wd4626 /wd4710 /wd4714 /wd5026 /wd5027  /Ob2 /Ob2 /Oi /Ot /Oy /MD /GT /GL /GF /GS- /W3 /Ox /analyze- /EHs-c- /GR- >)
	target_compile_definitions( "Tests.STL" PRIVATE $<$<CONFIG:Release>: __GX_FAST__ __GX_NO_EXCEPTIONS__ >)
	set_target_properties( "Tests.STL" PROPERTIES LINK_FLAGS_RELEASE " /OPT:REF /OPT:ICF /INCREMENTAL:NO /LTCG /RELEASE /DYNAMICBASE" )
endif()


if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
	# DebugAnalyze
	target_compile_options ( "Tests.STL" PRIVATE $<$<CONFIG:DebugAnalyze>: -std=c++1z -Wchar-subscripts -Wdouble-promotion -Wformat -Wmain -Wmissing-braces -Wmissing-include-dirs -Wunused -Wuninitialized -Wmaybe-uninitialized -Wunknown-pragmas -Wpragmas -Wstrict-aliasing -Wstrict-overflow -Wundef -Wendif-labels -Wfree-nonheap-object -Wpointer-arith -Wcast-qual -Wcast-align -Wwrite-strings -Wconversion-null -Wzero-as-null-pointer-constant -Wenum-compare -Wsign-compare -Wsizeof-pointer-memaccess -Wlogical-op -Werror=init-self -Werror=parentheses -Werror=return-local-addr -Werror=return-type -Werror=non-template-friend -Werror=array-bounds -Werror=div-by-zero -Werror=address -Werror=missing-field-initializers -Werror=placement-new  -Wno-non-template-friend  -ggdb -fsanitize=undefined -fchkp-check-incomplete-type -Og >)
	target_compile_definitions( "Tests.STL" PRIVATE $<$<CONFIG:DebugAnalyze>: __GX_DEBUG__ __GX_ANALYZE__ >)
	# Debug
	target_compile_options ( "Tests.STL" PRIVATE $<$<CONFIG:Debug>: -std=c++1z -Wchar-subscripts -Wdouble-promotion -Wformat -Wmain -Wmissing-braces -Wmissing-include-dirs -Wunused -Wuninitialized -Wmaybe-uninitialized -Wunknown-pragmas -Wpragmas -Wstrict-aliasing -Wstrict-overflow -Wundef -Wendif-labels -Wfree-nonheap-object -Wpointer-arith -Wcast-qual -Wcast-align -Wwrite-strings -Wconversion-null -Wzero-as-null-pointer-constant -Wenum-compare -Wsign-compare -Wsizeof-pointer-memaccess -Wlogical-op -Werror=init-self -Werror=parentheses -Werror=return-local-addr -Werror=return-type -Werror=non-template-friend -Werror=array-bounds -Werror=div-by-zero -Werror=address -Werror=missing-field-initializers -Werror=placement-new  -Wno-non-template-friend  -ggdb -fchkp-check-incomplete-type -Og >)
	target_compile_definitions( "Tests.STL" PRIVATE $<$<CONFIG:Debug>: __GX_DEBUG__ >)
	# Profile
	target_compile_options ( "Tests.STL" PRIVATE $<$<CONFIG:Profile>: -std=c++1z -Wchar-subscripts -Wdouble-promotion -Wformat -Wmain -Wmissing-braces -Wmissing-include-dirs -Wunused -Wuninitialized -Wmaybe-uninitialized -Wunknown-pragmas -Wpragmas -Wstrict-aliasing -Wstrict-overflow -Wundef -Wendif-labels -Wfree-nonheap-object -Wpointer-arith -Wcast-qual -Wcast-align -Wwrite-strings -Wconversion-null -Wzero-as-null-pointer-constant -Wenum-compare -Wsign-compare -Wsizeof-pointer-memaccess -Wlogical-op -Werror=init-self -Werror=parentheses -Werror=return-local-addr -Werror=return-type -Werror=non-template-friend -Werror=array-bounds -Werror=div-by-zero -Werror=address -Werror=missing-field-initializers -Werror=placement-new  -Wno-non-template-friend  -O2 -finline-functions >)
	target_compile_definitions( "Tests.STL" PRIVATE $<$<CONFIG:Profile>: GX_ENABLE_PROFILING >)
	# Release
	target_compile_options ( "Tests.STL" PRIVATE $<$<CONFIG:Release>: -std=c++1z -Wchar-subscripts -Wdouble-promotion -Wformat -Wmain -Wmissing-braces -Wmissing-include-dirs -Wunused -Wuninitialized -Wmaybe-uninitialized -Wunknown-pragmas -Wpragmas -Wstrict-aliasing -Wstrict-overflow -Wundef -Wendif-labels -Wfree-nonheap-object -Wpointer-arith -Wcast-qual -Wcast-align -Wwrite-strings -Wconversion-null -Wzero-as-null-pointer-constant -Wenum-compare -Wsign-compare -Wsizeof-pointer-memaccess -Wlogical-op -Werror=init-self -Werror=parentheses -Werror=return-local-addr -Werror=return-type -Werror=non-template-friend -Werror=array-bounds -Werror=div-by-zero -Werror=address -Werror=missing-field-initializers -Werror=placement-new  -Wno-non-template-friend  -O3 -Ofast -fomit-frame-pointer -finline-functions >)
	target_compile_definitions( "Tests.STL" PRIVATE $<$<CONFIG:Release>: __GX_FAST__ __GX_NO_EXCEPTIONS__ >)
endif()


if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
	target_compile_definitions( "Tests.STL" PRIVATE $<$<CONFIG:DebugAnalyze>: __GX_DEBUG__ __GX_ANALYZE__ >)
	target_compile_definitions( "Tests.STL" PRIVATE $<$<CONFIG:Debug>: __GX_DEBUG__ >)
	target_compile_definitions( "Tests.STL" PRIVATE $<$<CONFIG:Profile>: GX_ENABLE_PROFILING >)
	target_compile_definitions( "Tests.STL" PRIVATE $<$<CONFIG:Release>: __GX_FAST__ __GX_NO_EXCEPTIONS__ >)
endif()





#==================================================================================================
# project: Engine.Base
#==================================================================================================
add_library( "Engine.Base" STATIC 
		"Engine/Base/Stream/InputStream.cpp"
		"Engine/Base/Stream/OutputStream.cpp"
		"Engine/Base/Stream/Stream.h"
		"Engine/Base/Stream/StreamManager.cpp"
		"Engine/Base/Stream/StreamManager.h"
		"Engine/Base/Tasks/AsyncMessage.h"
		"Engine/Base/Tasks/AsyncTask.h"
		"Engine/Base/Tasks/TaskManager.cpp"
		"Engine/Base/Tasks/TaskManager.h"
		"Engine/Base/Tasks/TaskModule.cpp"
		"Engine/Base/Tasks/TaskModule.h"
		"Engine/Base/Main/MainSystem.cpp"
		"Engine/Base/Main/MainSystem.h"
		"Engine/Base/Modules/CreateInfo.h"
		"Engine/Base/Modules/Message.h"
		"Engine/Base/Modules/MessageHandler.cpp"
		"Engine/Base/Modules/MessageHandler.h"
		"Engine/Base/Modules/MessageHelpers.h"
		"Engine/Base/Modules/Module.cpp"
		"Engine/Base/Modules/Module.h"
		"Engine/Base/Modules/Module.inl.h"
		"Engine/Base/Modules/ModuleAsyncTasks.h"
		"Engine/Base/Modules/ModuleMessages.h"
		"Engine/Base/Modules/ModulesFactory.cpp"
		"Engine/Base/Modules/ModulesFactory.h"
		"Engine/Base/Modules/ModuleUtils.h"
		"Engine/Base/Modules/ProfilingMessages.h"
		"Engine/Base/Common/BaseObject.h"
		"Engine/Base/Common/Common.h"
		"Engine/Base/Common/Defines.h"
		"Engine/Base/Common/EModuleGroup.h"
		"Engine/Base/Common/EngineSubSystems.h"
		"Engine/Base/Common/Enums.h"
		"Engine/Base/Common/IDs.h"
		"Engine/Base/Common/ThreadID.h"
		"Engine/Base/Files/FileManager.cpp"
		"Engine/Base/Files/FileManager.h"
		"Engine/Base/Engine.Base.h"
		"Engine/Base/Threads/ParallelThread.h"
		"Engine/Base/Threads/ParallelThreadImpl.cpp"
		"Engine/Base/Threads/ParallelThreadImpl.h"
		"Engine/Base/Threads/ThreadManager.cpp"
		"Engine/Base/Threads/ThreadManager.h" )

source_group( "Stream" FILES "Engine/Base/Stream/InputStream.cpp" "Engine/Base/Stream/OutputStream.cpp" "Engine/Base/Stream/Stream.h" "Engine/Base/Stream/StreamManager.cpp" "Engine/Base/Stream/StreamManager.h" )
source_group( "Tasks" FILES "Engine/Base/Tasks/AsyncMessage.h" "Engine/Base/Tasks/AsyncTask.h" "Engine/Base/Tasks/TaskManager.cpp" "Engine/Base/Tasks/TaskManager.h" "Engine/Base/Tasks/TaskModule.cpp" "Engine/Base/Tasks/TaskModule.h" )
source_group( "Main" FILES "Engine/Base/Main/MainSystem.cpp" "Engine/Base/Main/MainSystem.h" )
source_group( "Modules" FILES "Engine/Base/Modules/CreateInfo.h" "Engine/Base/Modules/Message.h" "Engine/Base/Modules/MessageHandler.cpp" "Engine/Base/Modules/MessageHandler.h" "Engine/Base/Modules/MessageHelpers.h" "Engine/Base/Modules/Module.cpp" "Engine/Base/Modules/Module.h" "Engine/Base/Modules/Module.inl.h" "Engine/Base/Modules/ModuleAsyncTasks.h" "Engine/Base/Modules/ModuleMessages.h" "Engine/Base/Modules/ModulesFactory.cpp" "Engine/Base/Modules/ModulesFactory.h" "Engine/Base/Modules/ModuleUtils.h" "Engine/Base/Modules/ProfilingMessages.h" )
source_group( "Common" FILES "Engine/Base/Common/BaseObject.h" "Engine/Base/Common/Common.h" "Engine/Base/Common/Defines.h" "Engine/Base/Common/EModuleGroup.h" "Engine/Base/Common/EngineSubSystems.h" "Engine/Base/Common/Enums.h" "Engine/Base/Common/IDs.h" "Engine/Base/Common/ThreadID.h" )
source_group( "Files" FILES "Engine/Base/Files/FileManager.cpp" "Engine/Base/Files/FileManager.h" )
source_group( "" FILES "Engine/Base/Engine.Base.h" )
source_group( "Threads" FILES "Engine/Base/Threads/ParallelThread.h" "Engine/Base/Threads/ParallelThreadImpl.cpp" "Engine/Base/Threads/ParallelThreadImpl.h" "Engine/Base/Threads/ThreadManager.cpp" "Engine/Base/Threads/ThreadManager.h" )
set_property( TARGET "Engine.Base" PROPERTY FOLDER "Engine" )
target_include_directories( "Engine.Base" PRIVATE "Engine/Base" )
target_include_directories( "Engine.Base" PUBLIC "Engine/.." )
target_link_libraries( "Engine.Base" "Engine.STL" )
# compiler
if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
	# DebugAnalyze
	target_compile_options ( "Engine.Base" PRIVATE $<$<CONFIG:DebugAnalyze>: /std:c++latest /MP /Gm- /Zc:inline /Gy- /fp:strict /fp:except- /we4002 /we4099 /we4129 /we4172 /we4201 /we4238 /we4239 /we4251 /we4263 /we4264 /we4266 /we4273 /we4473 /we4474 /we4480 /we4522 /we4552 /we4553 /we4700 /we4702 /we4715 /we4716 /we4717  /w14018 /w14100 /w14127 /w14189 /w14244 /w14245 /w14287 /w14389 /w14456 /w14457 /w14458 /w14459 /w14505 /w14668 /w14838 /w14946 /w14996 /w15038  /wd4061 /wd4062 /wd4063 /wd4310 /wd4365 /wd4503 /wd4514 /wd4623 /wd4625 /wd4626 /wd4710 /wd4714 /wd5026 /wd5027  /W4 /WX- /sdl /Od /Ob0 /EHsc /Oy- /GF- /GS /guard:cf /GR /MDd /analyze /Zi /RTCsu >)
	target_compile_definitions( "Engine.Base" PRIVATE $<$<CONFIG:DebugAnalyze>: __GX_DEBUG__ __GX_ANALYZE__ >)
	set_target_properties( "Engine.Base" PROPERTIES LINK_FLAGS_DEBUGANALYZE " /OPT:REF /OPT:ICF /INCREMENTAL:NO /DEBUG:FULL /LTCG" )
	# Debug
	target_compile_options ( "Engine.Base" PRIVATE $<$<CONFIG:Debug>: /std:c++latest /MP /Gm- /Zc:inline /Gy- /fp:strict /fp:except- /we4002 /we4099 /we4129 /we4172 /we4201 /we4238 /we4239 /we4251 /we4263 /we4264 /we4266 /we4273 /we4473 /we4474 /we4480 /we4522 /we4552 /we4553 /we4700 /we4702 /we4715 /we4716 /we4717  /w14018 /w14100 /w14127 /w14189 /w14244 /w14245 /w14287 /w14389 /w14456 /w14457 /w14458 /w14459 /w14505 /w14668 /w14838 /w14946 /w14996 /w15038  /wd4061 /wd4062 /wd4063 /wd4310 /wd4365 /wd4503 /wd4514 /wd4623 /wd4625 /wd4626 /wd4710 /wd4714 /wd5026 /wd5027  /W4 /WX- /sdl /Od /Ob0 /EHsc /Oy- /GF- /GS /GR /MDd /analyze- /Zi /RTCsu >)
	target_compile_definitions( "Engine.Base" PRIVATE $<$<CONFIG:Debug>: __GX_DEBUG__ >)
	set_target_properties( "Engine.Base" PROPERTIES LINK_FLAGS_DEBUG " /OPT:REF /OPT:ICF /INCREMENTAL:NO /DEBUG:FULL /LTCG" )
	# Profile
	target_compile_options ( "Engine.Base" PRIVATE $<$<CONFIG:Profile>: /std:c++latest /MP /Gm- /Zc:inline /Gy- /fp:strict /fp:except- /we4002 /we4099 /we4129 /we4172 /we4201 /we4238 /we4239 /we4251 /we4263 /we4264 /we4266 /we4273 /we4473 /we4474 /we4480 /we4522 /we4552 /we4553 /we4700 /we4702 /we4715 /we4716 /we4717  /w14018 /w14100 /w14127 /w14189 /w14244 /w14245 /w14287 /w14389 /w14456 /w14457 /w14458 /w14459 /w14505 /w14668 /w14838 /w14946 /w14996 /w15038  /wd4061 /wd4062 /wd4063 /wd4310 /wd4365 /wd4503 /wd4514 /wd4623 /wd4625 /wd4626 /wd4710 /wd4714 /wd5026 /wd5027  /Ob2 /Ob2 /Oi /Ot /Oy /MD /GT /GL /GF /GS- /W3 /Od /analyze- /EHsc /GR >)
	target_compile_definitions( "Engine.Base" PRIVATE $<$<CONFIG:Profile>: GX_ENABLE_PROFILING >)
	set_target_properties( "Engine.Base" PROPERTIES LINK_FLAGS_PROFILE " /OPT:REF /OPT:ICF /INCREMENTAL:NO /LTCG /DEBUG /PROFILE" )
	# Release
	target_compile_options ( "Engine.Base" PRIVATE $<$<CONFIG:Release>: /std:c++latest /MP /Gm- /Zc:inline /Gy- /fp:strict /fp:except- /we4002 /we4099 /we4129 /we4172 /we4201 /we4238 /we4239 /we4251 /we4263 /we4264 /we4266 /we4273 /we4473 /we4474 /we4480 /we4522 /we4552 /we4553 /we4700 /we4702 /we4715 /we4716 /we4717  /w14018  /w14127 /w14189 /w14244 /w14245 /w14287 /w14389 /w14456 /w14457 /w14458 /w14459 /w14505 /w14668 /w14838 /w14946 /w14996 /w15038  /wd4061 /wd4062 /wd4063 /wd4310 /wd4365 /wd4503 /wd4514 /wd4623 /wd4625 /wd4626 /wd4710 /wd4714 /wd5026 /wd5027  /Ob2 /Ob2 /Oi /Ot /Oy /MD /GT /GL /GF /GS- /W3 /Ox /analyze- /EHs-c- /GR- >)
	target_compile_definitions( "Engine.Base" PRIVATE $<$<CONFIG:Release>: __GX_FAST__ __GX_NO_EXCEPTIONS__ >)
	set_target_properties( "Engine.Base" PROPERTIES LINK_FLAGS_RELEASE " /OPT:REF /OPT:ICF /INCREMENTAL:NO /LTCG /RELEASE /DYNAMICBASE" )
endif()


if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
	# DebugAnalyze
	target_compile_options ( "Engine.Base" PRIVATE $<$<CONFIG:DebugAnalyze>: -std=c++1z -Wchar-subscripts -Wdouble-promotion -Wformat -Wmain -Wmissing-braces -Wmissing-include-dirs -Wunused -Wuninitialized -Wmaybe-uninitialized -Wunknown-pragmas -Wpragmas -Wstrict-aliasing -Wstrict-overflow -Wundef -Wendif-labels -Wfree-nonheap-object -Wpointer-arith -Wcast-qual -Wcast-align -Wwrite-strings -Wconversion-null -Wzero-as-null-pointer-constant -Wenum-compare -Wsign-compare -Wsizeof-pointer-memaccess -Wlogical-op -Werror=init-self -Werror=parentheses -Werror=return-local-addr -Werror=return-type -Werror=non-template-friend -Werror=array-bounds -Werror=div-by-zero -Werror=address -Werror=missing-field-initializers -Werror=placement-new  -Wno-non-template-friend  -ggdb -fsanitize=undefined -fchkp-check-incomplete-type -Og >)
	target_compile_definitions( "Engine.Base" PRIVATE $<$<CONFIG:DebugAnalyze>: __GX_DEBUG__ __GX_ANALYZE__ >)
	# Debug
	target_compile_options ( "Engine.Base" PRIVATE $<$<CONFIG:Debug>: -std=c++1z -Wchar-subscripts -Wdouble-promotion -Wformat -Wmain -Wmissing-braces -Wmissing-include-dirs -Wunused -Wuninitialized -Wmaybe-uninitialized -Wunknown-pragmas -Wpragmas -Wstrict-aliasing -Wstrict-overflow -Wundef -Wendif-labels -Wfree-nonheap-object -Wpointer-arith -Wcast-qual -Wcast-align -Wwrite-strings -Wconversion-null -Wzero-as-null-pointer-constant -Wenum-compare -Wsign-compare -Wsizeof-pointer-memaccess -Wlogical-op -Werror=init-self -Werror=parentheses -Werror=return-local-addr -Werror=return-type -Werror=non-template-friend -Werror=array-bounds -Werror=div-by-zero -Werror=address -Werror=missing-field-initializers -Werror=placement-new  -Wno-non-template-friend  -ggdb -fchkp-check-incomplete-type -Og >)
	target_compile_definitions( "Engine.Base" PRIVATE $<$<CONFIG:Debug>: __GX_DEBUG__ >)
	# Profile
	target_compile_options ( "Engine.Base" PRIVATE $<$<CONFIG:Profile>: -std=c++1z -Wchar-subscripts -Wdouble-promotion -Wformat -Wmain -Wmissing-braces -Wmissing-include-dirs -Wunused -Wuninitialized -Wmaybe-uninitialized -Wunknown-pragmas -Wpragmas -Wstrict-aliasing -Wstrict-overflow -Wundef -Wendif-labels -Wfree-nonheap-object -Wpointer-arith -Wcast-qual -Wcast-align -Wwrite-strings -Wconversion-null -Wzero-as-null-pointer-constant -Wenum-compare -Wsign-compare -Wsizeof-pointer-memaccess -Wlogical-op -Werror=init-self -Werror=parentheses -Werror=return-local-addr -Werror=return-type -Werror=non-template-friend -Werror=array-bounds -Werror=div-by-zero -Werror=address -Werror=missing-field-initializers -Werror=placement-new  -Wno-non-template-friend  -O2 -finline-functions >)
	target_compile_definitions( "Engine.Base" PRIVATE $<$<CONFIG:Profile>: GX_ENABLE_PROFILING >)
	# Release
	target_compile_options ( "Engine.Base" PRIVATE $<$<CONFIG:Release>: -std=c++1z -Wchar-subscripts -Wdouble-promotion -Wformat -Wmain -Wmissing-braces -Wmissing-include-dirs -Wunused -Wuninitialized -Wmaybe-uninitialized -Wunknown-pragmas -Wpragmas -Wstrict-aliasing -Wstrict-overflow -Wundef -Wendif-labels -Wfree-nonheap-object -Wpointer-arith -Wcast-qual -Wcast-align -Wwrite-strings -Wconversion-null -Wzero-as-null-pointer-constant -Wenum-compare -Wsign-compare -Wsizeof-pointer-memaccess -Wlogical-op -Werror=init-self -Werror=parentheses -Werror=return-local-addr -Werror=return-type -Werror=non-template-friend -Werror=array-bounds -Werror=div-by-zero -Werror=address -Werror=missing-field-initializers -Werror=placement-new  -Wno-non-template-friend  -O3 -Ofast -fomit-frame-pointer -finline-functions >)
	target_compile_definitions( "Engine.Base" PRIVATE $<$<CONFIG:Release>: __GX_FAST__ __GX_NO_EXCEPTIONS__ >)
endif()


if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
	target_compile_definitions( "Engine.Base" PRIVATE $<$<CONFIG:DebugAnalyze>: __GX_DEBUG__ __GX_ANALYZE__ >)
	target_compile_definitions( "Engine.Base" PRIVATE $<$<CONFIG:Debug>: __GX_DEBUG__ >)
	target_compile_definitions( "Engine.Base" PRIVATE $<$<CONFIG:Profile>: GX_ENABLE_PROFILING >)
	target_compile_definitions( "Engine.Base" PRIVATE $<$<CONFIG:Release>: __GX_FAST__ __GX_NO_EXCEPTIONS__ >)
endif()





#==================================================================================================
# project: Engine.Platforms
#==================================================================================================
add_library( "Engine.Platforms" STATIC 
		"Engine/Platforms/OpenGL/Impl/gl4.cpp"
		"Engine/Platforms/OpenGL/Impl/gl4.h"
		"Engine/Platforms/OpenGL/Impl/GL4BaseModule.cpp"
		"Engine/Platforms/OpenGL/Impl/GL4BaseModule.h"
		"Engine/Platforms/OpenGL/Impl/GL4Buffer.cpp"
		"Engine/Platforms/OpenGL/Impl/GL4CommandBuffer.cpp"
		"Engine/Platforms/OpenGL/Impl/GL4CommandBuilder.cpp"
		"Engine/Platforms/OpenGL/Impl/GL4Device.cpp"
		"Engine/Platforms/OpenGL/Impl/GL4Device.h"
		"Engine/Platforms/OpenGL/Impl/GL4Emulator.h"
		"Engine/Platforms/OpenGL/Impl/GL4Enums.h"
		"Engine/Platforms/OpenGL/Impl/GL4FlippedSystemFramebuffer.h"
		"Engine/Platforms/OpenGL/Impl/GL4Framebuffer.cpp"
		"Engine/Platforms/OpenGL/Impl/GL4Image.cpp"
		"Engine/Platforms/OpenGL/Impl/GL4Memory.cpp"
		"Engine/Platforms/OpenGL/Impl/GL4Messages.h"
		"Engine/Platforms/OpenGL/Impl/GL4Pipeline.cpp"
		"Engine/Platforms/OpenGL/Impl/GL4PipelineResourceTable.cpp"
		"Engine/Platforms/OpenGL/Impl/GL4RenderPass.cpp"
		"Engine/Platforms/OpenGL/Impl/GL4Sampler.cpp"
		"Engine/Platforms/OpenGL/Impl/GL4Sampler.h"
		"Engine/Platforms/OpenGL/Impl/GL4SystemFramebuffer.h"
		"Engine/Platforms/OpenGL/OpenGLContext.cpp"
		"Engine/Platforms/OpenGL/OpenGLObjectsConstructor.h"
		"Engine/Platforms/OpenGL/OpenGLThread.cpp"
		"Engine/Platforms/OpenCL/OpenCLContext.cpp"
		"Engine/Platforms/OpenCL/OpenCLObjectsConstructor.h"
		"Engine/Platforms/OpenCL/OpenCLThread.cpp"
		"Engine/Platforms/Shared/OS/Display.h"
		"Engine/Platforms/Shared/OS/IDs.h"
		"Engine/Platforms/Shared/OS/Input.h"
		"Engine/Platforms/Shared/OS/OSEnums.h"
		"Engine/Platforms/Shared/OS/Platform.h"
		"Engine/Platforms/Shared/OS/ScreenCoords.h"
		"Engine/Platforms/Shared/OS/VR.h"
		"Engine/Platforms/Shared/OS/Window.h"
		"Engine/Platforms/Common/Common.h"
		"Engine/Platforms/Input/InputManager.cpp"
		"Engine/Platforms/Input/InputManager.h"
		"Engine/Platforms/Input/InputThread.cpp"
		"Engine/Platforms/Shared/Main.cpp"
		"Engine/Platforms/Shared/PipelineTemplate.cpp"
		"Engine/Platforms/Vulkan/VulkanContext.cpp"
		"Engine/Platforms/Vulkan/VulkanObjectsConstructor.h"
		"Engine/Platforms/Vulkan/VulkanThread.cpp"
		"Engine/Platforms/Windows/WinDisplay.cpp"
		"Engine/Platforms/Windows/WinDisplay.h"
		"Engine/Platforms/Windows/WinKeyInput.cpp"
		"Engine/Platforms/Windows/WinMessages.h"
		"Engine/Platforms/Windows/WinMouseInput.cpp"
		"Engine/Platforms/Windows/WinObjectsConstructor.h"
		"Engine/Platforms/Windows/WinPlatform.cpp"
		"Engine/Platforms/Windows/WinWindow.cpp"
		"Engine/Platforms/Vulkan/Impl/Vk1BaseModule.cpp"
		"Engine/Platforms/Vulkan/Impl/Vk1BaseModule.h"
		"Engine/Platforms/Vulkan/Impl/Vk1BaseObject.h"
		"Engine/Platforms/Vulkan/Impl/Vk1Buffer.cpp"
		"Engine/Platforms/Vulkan/Impl/Vk1CommandBuffer.cpp"
		"Engine/Platforms/Vulkan/Impl/Vk1CommandBuilder.cpp"
		"Engine/Platforms/Vulkan/Impl/Vk1Device.cpp"
		"Engine/Platforms/Vulkan/Impl/Vk1Device.h"
		"Engine/Platforms/Vulkan/Impl/Vk1Enums.h"
		"Engine/Platforms/Vulkan/Impl/Vk1Fence.h"
		"Engine/Platforms/Vulkan/Impl/Vk1Framebuffer.cpp"
		"Engine/Platforms/Vulkan/Impl/Vk1Image.cpp"
		"Engine/Platforms/Vulkan/Impl/Vk1Library.h"
		"Engine/Platforms/Vulkan/Impl/Vk1Memory.cpp"
		"Engine/Platforms/Vulkan/Impl/Vk1MemoryManager.cpp"
		"Engine/Platforms/Vulkan/Impl/Vk1Messages.h"
		"Engine/Platforms/Vulkan/Impl/Vk1Pipeline.cpp"
		"Engine/Platforms/Vulkan/Impl/Vk1PipelineCache.cpp"
		"Engine/Platforms/Vulkan/Impl/Vk1PipelineCache.h"
		"Engine/Platforms/Vulkan/Impl/Vk1PipelineLayout.cpp"
		"Engine/Platforms/Vulkan/Impl/Vk1PipelineLayout.h"
		"Engine/Platforms/Vulkan/Impl/Vk1PipelineResourceTable.cpp"
		"Engine/Platforms/Vulkan/Impl/Vk1RenderPass.cpp"
		"Engine/Platforms/Vulkan/Impl/Vk1RenderPass.h"
		"Engine/Platforms/Vulkan/Impl/Vk1Sampler.cpp"
		"Engine/Platforms/Vulkan/Impl/Vk1Sampler.h"
		"Engine/Platforms/Vulkan/Impl/Vk1SystemFramebuffer.h"
		"Engine/Platforms/Vulkan/Impl/vulkan1.cpp"
		"Engine/Platforms/Vulkan/Impl/vulkan1.h"
		"Engine/Platforms/Vulkan/Impl/vulkan1_platform.cpp"
		"Engine/Platforms/Vulkan/Impl/vulkan1_platform.h"
		"Engine/Platforms/Vulkan/Impl/vulkan1_utils.h"
		"Engine/Platforms/Engine.Platforms.h"
		"Engine/Platforms/Shared/Tools/ImageViewHashMap.h"
		"Engine/Platforms/Shared/Tools/MemoryMapperHelper.cpp"
		"Engine/Platforms/Shared/Tools/MemoryMapperHelper.h"
		"Engine/Platforms/OpenGL/Windows/GLWinContext.cpp"
		"Engine/Platforms/OpenGL/Windows/GLWinContext.h"
		"Engine/Platforms/OpenGL/Windows/GLWinLibrary.cpp"
		"Engine/Platforms/OpenGL/Windows/GLWinLibrary.h"
		"Engine/Platforms/Shared/GPU/Buffer.h"
		"Engine/Platforms/Shared/GPU/BufferEnums.h"
		"Engine/Platforms/Shared/GPU/CommandBuffer.h"
		"Engine/Platforms/Shared/GPU/CommandEnums.h"
		"Engine/Platforms/Shared/GPU/Context.h"
		"Engine/Platforms/Shared/GPU/Enums.ToString.h"
		"Engine/Platforms/Shared/GPU/FragmentOutputState.h"
		"Engine/Platforms/Shared/GPU/Framebuffer.h"
		"Engine/Platforms/Shared/GPU/IDs.h"
		"Engine/Platforms/Shared/GPU/Image.cpp"
		"Engine/Platforms/Shared/GPU/Image.h"
		"Engine/Platforms/Shared/GPU/ImageEnums.h"
		"Engine/Platforms/Shared/GPU/ImageLayer.h"
		"Engine/Platforms/Shared/GPU/ImageSwizzle.h"
		"Engine/Platforms/Shared/GPU/Memory.h"
		"Engine/Platforms/Shared/GPU/MemoryEnums.h"
		"Engine/Platforms/Shared/GPU/MipmapLevel.h"
		"Engine/Platforms/Shared/GPU/MultiSamples.h"
		"Engine/Platforms/Shared/GPU/ObjectEnums.h"
		"Engine/Platforms/Shared/GPU/Pipeline.cpp"
		"Engine/Platforms/Shared/GPU/Pipeline.h"
		"Engine/Platforms/Shared/GPU/PipelineLayout.cpp"
		"Engine/Platforms/Shared/GPU/PipelineLayout.h"
		"Engine/Platforms/Shared/GPU/PixelFormatEnums.h"
		"Engine/Platforms/Shared/GPU/RenderPass.cpp"
		"Engine/Platforms/Shared/GPU/RenderPass.h"
		"Engine/Platforms/Shared/GPU/RenderPassEnums.h"
		"Engine/Platforms/Shared/GPU/RenderState.cpp"
		"Engine/Platforms/Shared/GPU/RenderState.h"
		"Engine/Platforms/Shared/GPU/RenderStateEnums.h"
		"Engine/Platforms/Shared/GPU/Sampler.cpp"
		"Engine/Platforms/Shared/GPU/Sampler.h"
		"Engine/Platforms/Shared/GPU/SamplerEnums.h"
		"Engine/Platforms/Shared/GPU/ShaderEnums.h"
		"Engine/Platforms/Shared/GPU/Thread.h"
		"Engine/Platforms/Shared/GPU/Uniforms.h"
		"Engine/Platforms/Shared/GPU/VertexAttribs.h"
		"Engine/Platforms/Shared/GPU/VertexDescr.h"
		"Engine/Platforms/Shared/GPU/VertexEnums.h"
		"Engine/Platforms/Shared/GPU/VertexInputState.cpp"
		"Engine/Platforms/Shared/GPU/VertexInputState.h"
		"Engine/Platforms/Shared/GPU/VR.h"
		"Engine/Platforms/OpenCL/Impl/cl2.cpp"
		"Engine/Platforms/OpenCL/Impl/cl2.h"
		"Engine/Platforms/OpenCL/Impl/CL2BaseModule.cpp"
		"Engine/Platforms/OpenCL/Impl/CL2BaseModule.h"
		"Engine/Platforms/OpenCL/Impl/CL2Buffer.cpp"
		"Engine/Platforms/OpenCL/Impl/CL2CommandBuffer.cpp"
		"Engine/Platforms/OpenCL/Impl/CL2CommandBuilder.cpp"
		"Engine/Platforms/OpenCL/Impl/CL2Device.cpp"
		"Engine/Platforms/OpenCL/Impl/CL2Device.h"
		"Engine/Platforms/OpenCL/Impl/CL2Emulator.cpp"
		"Engine/Platforms/OpenCL/Impl/CL2Emulator.h"
		"Engine/Platforms/OpenCL/Impl/CL2Enums.h"
		"Engine/Platforms/OpenCL/Impl/CL2Image.cpp"
		"Engine/Platforms/OpenCL/Impl/CL2Memory.cpp"
		"Engine/Platforms/OpenCL/Impl/CL2Messages.h"
		"Engine/Platforms/OpenCL/Impl/CL2Pipeline.cpp"
		"Engine/Platforms/OpenCL/Impl/CL2PipelineResourceTable.cpp"
		"Engine/Platforms/Vulkan/Windows/VkWinSurface.cpp"
		"Engine/Platforms/Vulkan/Windows/VkWinSurface.h" )

source_group( "OpenGL\\Impl" FILES "Engine/Platforms/OpenGL/Impl/gl4.cpp" "Engine/Platforms/OpenGL/Impl/gl4.h" "Engine/Platforms/OpenGL/Impl/GL4BaseModule.cpp" "Engine/Platforms/OpenGL/Impl/GL4BaseModule.h" "Engine/Platforms/OpenGL/Impl/GL4Buffer.cpp" "Engine/Platforms/OpenGL/Impl/GL4CommandBuffer.cpp" "Engine/Platforms/OpenGL/Impl/GL4CommandBuilder.cpp" "Engine/Platforms/OpenGL/Impl/GL4Device.cpp" "Engine/Platforms/OpenGL/Impl/GL4Device.h" "Engine/Platforms/OpenGL/Impl/GL4Emulator.h" "Engine/Platforms/OpenGL/Impl/GL4Enums.h" "Engine/Platforms/OpenGL/Impl/GL4FlippedSystemFramebuffer.h" "Engine/Platforms/OpenGL/Impl/GL4Framebuffer.cpp" "Engine/Platforms/OpenGL/Impl/GL4Image.cpp" "Engine/Platforms/OpenGL/Impl/GL4Memory.cpp" "Engine/Platforms/OpenGL/Impl/GL4Messages.h" "Engine/Platforms/OpenGL/Impl/GL4Pipeline.cpp" "Engine/Platforms/OpenGL/Impl/GL4PipelineResourceTable.cpp" "Engine/Platforms/OpenGL/Impl/GL4RenderPass.cpp" "Engine/Platforms/OpenGL/Impl/GL4Sampler.cpp" "Engine/Platforms/OpenGL/Impl/GL4Sampler.h" "Engine/Platforms/OpenGL/Impl/GL4SystemFramebuffer.h" )
source_group( "OpenGL" FILES "Engine/Platforms/OpenGL/OpenGLContext.cpp" "Engine/Platforms/OpenGL/OpenGLObjectsConstructor.h" "Engine/Platforms/OpenGL/OpenGLThread.cpp" )
source_group( "OpenCL" FILES "Engine/Platforms/OpenCL/OpenCLContext.cpp" "Engine/Platforms/OpenCL/OpenCLObjectsConstructor.h" "Engine/Platforms/OpenCL/OpenCLThread.cpp" )
source_group( "Shared\\OS" FILES "Engine/Platforms/Shared/OS/Display.h" "Engine/Platforms/Shared/OS/IDs.h" "Engine/Platforms/Shared/OS/Input.h" "Engine/Platforms/Shared/OS/OSEnums.h" "Engine/Platforms/Shared/OS/Platform.h" "Engine/Platforms/Shared/OS/ScreenCoords.h" "Engine/Platforms/Shared/OS/VR.h" "Engine/Platforms/Shared/OS/Window.h" )
source_group( "Common" FILES "Engine/Platforms/Common/Common.h" )
source_group( "Input" FILES "Engine/Platforms/Input/InputManager.cpp" "Engine/Platforms/Input/InputManager.h" "Engine/Platforms/Input/InputThread.cpp" )
source_group( "Shared" FILES "Engine/Platforms/Shared/Main.cpp" "Engine/Platforms/Shared/PipelineTemplate.cpp" )
source_group( "Vulkan" FILES "Engine/Platforms/Vulkan/VulkanContext.cpp" "Engine/Platforms/Vulkan/VulkanObjectsConstructor.h" "Engine/Platforms/Vulkan/VulkanThread.cpp" )
source_group( "Windows" FILES "Engine/Platforms/Windows/WinDisplay.cpp" "Engine/Platforms/Windows/WinDisplay.h" "Engine/Platforms/Windows/WinKeyInput.cpp" "Engine/Platforms/Windows/WinMessages.h" "Engine/Platforms/Windows/WinMouseInput.cpp" "Engine/Platforms/Windows/WinObjectsConstructor.h" "Engine/Platforms/Windows/WinPlatform.cpp" "Engine/Platforms/Windows/WinWindow.cpp" )
source_group( "Vulkan\\Impl" FILES "Engine/Platforms/Vulkan/Impl/Vk1BaseModule.cpp" "Engine/Platforms/Vulkan/Impl/Vk1BaseModule.h" "Engine/Platforms/Vulkan/Impl/Vk1BaseObject.h" "Engine/Platforms/Vulkan/Impl/Vk1Buffer.cpp" "Engine/Platforms/Vulkan/Impl/Vk1CommandBuffer.cpp" "Engine/Platforms/Vulkan/Impl/Vk1CommandBuilder.cpp" "Engine/Platforms/Vulkan/Impl/Vk1Device.cpp" "Engine/Platforms/Vulkan/Impl/Vk1Device.h" "Engine/Platforms/Vulkan/Impl/Vk1Enums.h" "Engine/Platforms/Vulkan/Impl/Vk1Fence.h" "Engine/Platforms/Vulkan/Impl/Vk1Framebuffer.cpp" "Engine/Platforms/Vulkan/Impl/Vk1Image.cpp" "Engine/Platforms/Vulkan/Impl/Vk1Library.h" "Engine/Platforms/Vulkan/Impl/Vk1Memory.cpp" "Engine/Platforms/Vulkan/Impl/Vk1MemoryManager.cpp" "Engine/Platforms/Vulkan/Impl/Vk1Messages.h" "Engine/Platforms/Vulkan/Impl/Vk1Pipeline.cpp" "Engine/Platforms/Vulkan/Impl/Vk1PipelineCache.cpp" "Engine/Platforms/Vulkan/Impl/Vk1PipelineCache.h" "Engine/Platforms/Vulkan/Impl/Vk1PipelineLayout.cpp" "Engine/Platforms/Vulkan/Impl/Vk1PipelineLayout.h" "Engine/Platforms/Vulkan/Impl/Vk1PipelineResourceTable.cpp" "Engine/Platforms/Vulkan/Impl/Vk1RenderPass.cpp" "Engine/Platforms/Vulkan/Impl/Vk1RenderPass.h" "Engine/Platforms/Vulkan/Impl/Vk1Sampler.cpp" "Engine/Platforms/Vulkan/Impl/Vk1Sampler.h" "Engine/Platforms/Vulkan/Impl/Vk1SystemFramebuffer.h" "Engine/Platforms/Vulkan/Impl/vulkan1.cpp" "Engine/Platforms/Vulkan/Impl/vulkan1.h" "Engine/Platforms/Vulkan/Impl/vulkan1_platform.cpp" "Engine/Platforms/Vulkan/Impl/vulkan1_platform.h" "Engine/Platforms/Vulkan/Impl/vulkan1_utils.h" )
source_group( "" FILES "Engine/Platforms/Engine.Platforms.h" )
source_group( "Shared\\Tools" FILES "Engine/Platforms/Shared/Tools/ImageViewHashMap.h" "Engine/Platforms/Shared/Tools/MemoryMapperHelper.cpp" "Engine/Platforms/Shared/Tools/MemoryMapperHelper.h" )
source_group( "OpenGL\\Windows" FILES "Engine/Platforms/OpenGL/Windows/GLWinContext.cpp" "Engine/Platforms/OpenGL/Windows/GLWinContext.h" "Engine/Platforms/OpenGL/Windows/GLWinLibrary.cpp" "Engine/Platforms/OpenGL/Windows/GLWinLibrary.h" )
source_group( "Shared\\GPU" FILES "Engine/Platforms/Shared/GPU/Buffer.h" "Engine/Platforms/Shared/GPU/BufferEnums.h" "Engine/Platforms/Shared/GPU/CommandBuffer.h" "Engine/Platforms/Shared/GPU/CommandEnums.h" "Engine/Platforms/Shared/GPU/Context.h" "Engine/Platforms/Shared/GPU/Enums.ToString.h" "Engine/Platforms/Shared/GPU/FragmentOutputState.h" "Engine/Platforms/Shared/GPU/Framebuffer.h" "Engine/Platforms/Shared/GPU/IDs.h" "Engine/Platforms/Shared/GPU/Image.cpp" "Engine/Platforms/Shared/GPU/Image.h" "Engine/Platforms/Shared/GPU/ImageEnums.h" "Engine/Platforms/Shared/GPU/ImageLayer.h" "Engine/Platforms/Shared/GPU/ImageSwizzle.h" "Engine/Platforms/Shared/GPU/Memory.h" "Engine/Platforms/Shared/GPU/MemoryEnums.h" "Engine/Platforms/Shared/GPU/MipmapLevel.h" "Engine/Platforms/Shared/GPU/MultiSamples.h" "Engine/Platforms/Shared/GPU/ObjectEnums.h" "Engine/Platforms/Shared/GPU/Pipeline.cpp" "Engine/Platforms/Shared/GPU/Pipeline.h" "Engine/Platforms/Shared/GPU/PipelineLayout.cpp" "Engine/Platforms/Shared/GPU/PipelineLayout.h" "Engine/Platforms/Shared/GPU/PixelFormatEnums.h" "Engine/Platforms/Shared/GPU/RenderPass.cpp" "Engine/Platforms/Shared/GPU/RenderPass.h" "Engine/Platforms/Shared/GPU/RenderPassEnums.h" "Engine/Platforms/Shared/GPU/RenderState.cpp" "Engine/Platforms/Shared/GPU/RenderState.h" "Engine/Platforms/Shared/GPU/RenderStateEnums.h" "Engine/Platforms/Shared/GPU/Sampler.cpp" "Engine/Platforms/Shared/GPU/Sampler.h" "Engine/Platforms/Shared/GPU/SamplerEnums.h" "Engine/Platforms/Shared/GPU/ShaderEnums.h" "Engine/Platforms/Shared/GPU/Thread.h" "Engine/Platforms/Shared/GPU/Uniforms.h" "Engine/Platforms/Shared/GPU/VertexAttribs.h" "Engine/Platforms/Shared/GPU/VertexDescr.h" "Engine/Platforms/Shared/GPU/VertexEnums.h" "Engine/Platforms/Shared/GPU/VertexInputState.cpp" "Engine/Platforms/Shared/GPU/VertexInputState.h" "Engine/Platforms/Shared/GPU/VR.h" )
source_group( "OpenCL\\Impl" FILES "Engine/Platforms/OpenCL/Impl/cl2.cpp" "Engine/Platforms/OpenCL/Impl/cl2.h" "Engine/Platforms/OpenCL/Impl/CL2BaseModule.cpp" "Engine/Platforms/OpenCL/Impl/CL2BaseModule.h" "Engine/Platforms/OpenCL/Impl/CL2Buffer.cpp" "Engine/Platforms/OpenCL/Impl/CL2CommandBuffer.cpp" "Engine/Platforms/OpenCL/Impl/CL2CommandBuilder.cpp" "Engine/Platforms/OpenCL/Impl/CL2Device.cpp" "Engine/Platforms/OpenCL/Impl/CL2Device.h" "Engine/Platforms/OpenCL/Impl/CL2Emulator.cpp" "Engine/Platforms/OpenCL/Impl/CL2Emulator.h" "Engine/Platforms/OpenCL/Impl/CL2Enums.h" "Engine/Platforms/OpenCL/Impl/CL2Image.cpp" "Engine/Platforms/OpenCL/Impl/CL2Memory.cpp" "Engine/Platforms/OpenCL/Impl/CL2Messages.h" "Engine/Platforms/OpenCL/Impl/CL2Pipeline.cpp" "Engine/Platforms/OpenCL/Impl/CL2PipelineResourceTable.cpp" )
source_group( "Vulkan\\Windows" FILES "Engine/Platforms/Vulkan/Windows/VkWinSurface.cpp" "Engine/Platforms/Vulkan/Windows/VkWinSurface.h" )
set_property( TARGET "Engine.Platforms" PROPERTY FOLDER "Engine" )
target_include_directories( "Engine.Platforms" PRIVATE "Engine/Platforms" )
target_include_directories( "Engine.Platforms" PUBLIC "Engine/.." )
if (WIN32)
	target_link_libraries( "Engine.Platforms" "dxguid.lib" )
endif()
if (WIN32)
	target_link_libraries( "Engine.Platforms" "opengl32.lib" )
endif()
target_link_libraries( "Engine.Platforms" "Engine.Base" )
if ((MSVC AND WIN32))
	target_link_libraries( "Engine.Platforms" "Shcore.lib" )
endif()
if (WIN32)
	target_link_libraries( "Engine.Platforms" "Dxva2.lib" )
endif()
if (WIN32)
	target_link_libraries( "Engine.Platforms" "dxgi.lib" )
endif()
# compiler
if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
	# DebugAnalyze
	target_compile_options ( "Engine.Platforms" PRIVATE $<$<CONFIG:DebugAnalyze>: /std:c++latest /MP /Gm- /Zc:inline /Gy- /fp:strict /fp:except- /we4002 /we4099 /we4129 /we4172 /we4201 /we4238 /we4239 /we4251 /we4263 /we4264 /we4266 /we4273 /we4473 /we4474 /we4480 /we4522 /we4552 /we4553 /we4700 /we4702 /we4715 /we4716 /we4717  /w14018 /w14100 /w14127 /w14189 /w14244 /w14245 /w14287 /w14389 /w14456 /w14457 /w14458 /w14459 /w14505 /w14668 /w14838 /w14946 /w14996 /w15038  /wd4061 /wd4062 /wd4063 /wd4310 /wd4365 /wd4503 /wd4514 /wd4623 /wd4625 /wd4626 /wd4710 /wd4714 /wd5026 /wd5027  /W4 /WX- /sdl /Od /Ob0 /EHsc /Oy- /GF- /GS /guard:cf /GR /MDd /analyze /Zi /RTCsu >)
	target_compile_definitions( "Engine.Platforms" PRIVATE $<$<CONFIG:DebugAnalyze>: __GX_DEBUG__ __GX_ANALYZE__ >)
	set_target_properties( "Engine.Platforms" PROPERTIES LINK_FLAGS_DEBUGANALYZE " /OPT:REF /OPT:ICF /INCREMENTAL:NO /DEBUG:FULL /LTCG" )
	# Debug
	target_compile_options ( "Engine.Platforms" PRIVATE $<$<CONFIG:Debug>: /std:c++latest /MP /Gm- /Zc:inline /Gy- /fp:strict /fp:except- /we4002 /we4099 /we4129 /we4172 /we4201 /we4238 /we4239 /we4251 /we4263 /we4264 /we4266 /we4273 /we4473 /we4474 /we4480 /we4522 /we4552 /we4553 /we4700 /we4702 /we4715 /we4716 /we4717  /w14018 /w14100 /w14127 /w14189 /w14244 /w14245 /w14287 /w14389 /w14456 /w14457 /w14458 /w14459 /w14505 /w14668 /w14838 /w14946 /w14996 /w15038  /wd4061 /wd4062 /wd4063 /wd4310 /wd4365 /wd4503 /wd4514 /wd4623 /wd4625 /wd4626 /wd4710 /wd4714 /wd5026 /wd5027  /W4 /WX- /sdl /Od /Ob0 /EHsc /Oy- /GF- /GS /GR /MDd /analyze- /Zi /RTCsu >)
	target_compile_definitions( "Engine.Platforms" PRIVATE $<$<CONFIG:Debug>: __GX_DEBUG__ >)
	set_target_properties( "Engine.Platforms" PROPERTIES LINK_FLAGS_DEBUG " /OPT:REF /OPT:ICF /INCREMENTAL:NO /DEBUG:FULL /LTCG" )
	# Profile
	target_compile_options ( "Engine.Platforms" PRIVATE $<$<CONFIG:Profile>: /std:c++latest /MP /Gm- /Zc:inline /Gy- /fp:strict /fp:except- /we4002 /we4099 /we4129 /we4172 /we4201 /we4238 /we4239 /we4251 /we4263 /we4264 /we4266 /we4273 /we4473 /we4474 /we4480 /we4522 /we4552 /we4553 /we4700 /we4702 /we4715 /we4716 /we4717  /w14018 /w14100 /w14127 /w14189 /w14244 /w14245 /w14287 /w14389 /w14456 /w14457 /w14458 /w14459 /w14505 /w14668 /w14838 /w14946 /w14996 /w15038  /wd4061 /wd4062 /wd4063 /wd4310 /wd4365 /wd4503 /wd4514 /wd4623 /wd4625 /wd4626 /wd4710 /wd4714 /wd5026 /wd5027  /Ob2 /Ob2 /Oi /Ot /Oy /MD /GT /GL /GF /GS- /W3 /Od /analyze- /EHsc /GR >)
	target_compile_definitions( "Engine.Platforms" PRIVATE $<$<CONFIG:Profile>: GX_ENABLE_PROFILING >)
	set_target_properties( "Engine.Platforms" PROPERTIES LINK_FLAGS_PROFILE " /OPT:REF /OPT:ICF /INCREMENTAL:NO /LTCG /DEBUG /PROFILE" )
	# Release
	target_compile_options ( "Engine.Platforms" PRIVATE $<$<CONFIG:Release>: /std:c++latest /MP /Gm- /Zc:inline /Gy- /fp:strict /fp:except- /we4002 /we4099 /we4129 /we4172 /we4201 /we4238 /we4239 /we4251 /we4263 /we4264 /we4266 /we4273 /we4473 /we4474 /we4480 /we4522 /we4552 /we4553 /we4700 /we4702 /we4715 /we4716 /we4717  /w14018  /w14127 /w14189 /w14244 /w14245 /w14287 /w14389 /w14456 /w14457 /w14458 /w14459 /w14505 /w14668 /w14838 /w14946 /w14996 /w15038  /wd4061 /wd4062 /wd4063 /wd4310 /wd4365 /wd4503 /wd4514 /wd4623 /wd4625 /wd4626 /wd4710 /wd4714 /wd5026 /wd5027  /Ob2 /Ob2 /Oi /Ot /Oy /MD /GT /GL /GF /GS- /W3 /Ox /analyze- /EHs-c- /GR- >)
	target_compile_definitions( "Engine.Platforms" PRIVATE $<$<CONFIG:Release>: __GX_FAST__ __GX_NO_EXCEPTIONS__ >)
	set_target_properties( "Engine.Platforms" PROPERTIES LINK_FLAGS_RELEASE " /OPT:REF /OPT:ICF /INCREMENTAL:NO /LTCG /RELEASE /DYNAMICBASE" )
endif()


if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
	# DebugAnalyze
	target_compile_options ( "Engine.Platforms" PRIVATE $<$<CONFIG:DebugAnalyze>: -std=c++1z -Wchar-subscripts -Wdouble-promotion -Wformat -Wmain -Wmissing-braces -Wmissing-include-dirs -Wunused -Wuninitialized -Wmaybe-uninitialized -Wunknown-pragmas -Wpragmas -Wstrict-aliasing -Wstrict-overflow -Wundef -Wendif-labels -Wfree-nonheap-object -Wpointer-arith -Wcast-qual -Wcast-align -Wwrite-strings -Wconversion-null -Wzero-as-null-pointer-constant -Wenum-compare -Wsign-compare -Wsizeof-pointer-memaccess -Wlogical-op -Werror=init-self -Werror=parentheses -Werror=return-local-addr -Werror=return-type -Werror=non-template-friend -Werror=array-bounds -Werror=div-by-zero -Werror=address -Werror=missing-field-initializers -Werror=placement-new  -Wno-non-template-friend  -ggdb -fsanitize=undefined -fchkp-check-incomplete-type -Og >)
	target_compile_definitions( "Engine.Platforms" PRIVATE $<$<CONFIG:DebugAnalyze>: __GX_DEBUG__ __GX_ANALYZE__ >)
	# Debug
	target_compile_options ( "Engine.Platforms" PRIVATE $<$<CONFIG:Debug>: -std=c++1z -Wchar-subscripts -Wdouble-promotion -Wformat -Wmain -Wmissing-braces -Wmissing-include-dirs -Wunused -Wuninitialized -Wmaybe-uninitialized -Wunknown-pragmas -Wpragmas -Wstrict-aliasing -Wstrict-overflow -Wundef -Wendif-labels -Wfree-nonheap-object -Wpointer-arith -Wcast-qual -Wcast-align -Wwrite-strings -Wconversion-null -Wzero-as-null-pointer-constant -Wenum-compare -Wsign-compare -Wsizeof-pointer-memaccess -Wlogical-op -Werror=init-self -Werror=parentheses -Werror=return-local-addr -Werror=return-type -Werror=non-template-friend -Werror=array-bounds -Werror=div-by-zero -Werror=address -Werror=missing-field-initializers -Werror=placement-new  -Wno-non-template-friend  -ggdb -fchkp-check-incomplete-type -Og >)
	target_compile_definitions( "Engine.Platforms" PRIVATE $<$<CONFIG:Debug>: __GX_DEBUG__ >)
	# Profile
	target_compile_options ( "Engine.Platforms" PRIVATE $<$<CONFIG:Profile>: -std=c++1z -Wchar-subscripts -Wdouble-promotion -Wformat -Wmain -Wmissing-braces -Wmissing-include-dirs -Wunused -Wuninitialized -Wmaybe-uninitialized -Wunknown-pragmas -Wpragmas -Wstrict-aliasing -Wstrict-overflow -Wundef -Wendif-labels -Wfree-nonheap-object -Wpointer-arith -Wcast-qual -Wcast-align -Wwrite-strings -Wconversion-null -Wzero-as-null-pointer-constant -Wenum-compare -Wsign-compare -Wsizeof-pointer-memaccess -Wlogical-op -Werror=init-self -Werror=parentheses -Werror=return-local-addr -Werror=return-type -Werror=non-template-friend -Werror=array-bounds -Werror=div-by-zero -Werror=address -Werror=missing-field-initializers -Werror=placement-new  -Wno-non-template-friend  -O2 -finline-functions >)
	target_compile_definitions( "Engine.Platforms" PRIVATE $<$<CONFIG:Profile>: GX_ENABLE_PROFILING >)
	# Release
	target_compile_options ( "Engine.Platforms" PRIVATE $<$<CONFIG:Release>: -std=c++1z -Wchar-subscripts -Wdouble-promotion -Wformat -Wmain -Wmissing-braces -Wmissing-include-dirs -Wunused -Wuninitialized -Wmaybe-uninitialized -Wunknown-pragmas -Wpragmas -Wstrict-aliasing -Wstrict-overflow -Wundef -Wendif-labels -Wfree-nonheap-object -Wpointer-arith -Wcast-qual -Wcast-align -Wwrite-strings -Wconversion-null -Wzero-as-null-pointer-constant -Wenum-compare -Wsign-compare -Wsizeof-pointer-memaccess -Wlogical-op -Werror=init-self -Werror=parentheses -Werror=return-local-addr -Werror=return-type -Werror=non-template-friend -Werror=array-bounds -Werror=div-by-zero -Werror=address -Werror=missing-field-initializers -Werror=placement-new  -Wno-non-template-friend  -O3 -Ofast -fomit-frame-pointer -finline-functions >)
	target_compile_definitions( "Engine.Platforms" PRIVATE $<$<CONFIG:Release>: __GX_FAST__ __GX_NO_EXCEPTIONS__ >)
endif()


if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
	target_compile_definitions( "Engine.Platforms" PRIVATE $<$<CONFIG:DebugAnalyze>: __GX_DEBUG__ __GX_ANALYZE__ >)
	target_compile_definitions( "Engine.Platforms" PRIVATE $<$<CONFIG:Debug>: __GX_DEBUG__ >)
	target_compile_definitions( "Engine.Platforms" PRIVATE $<$<CONFIG:Profile>: GX_ENABLE_PROFILING >)
	target_compile_definitions( "Engine.Platforms" PRIVATE $<$<CONFIG:Release>: __GX_FAST__ __GX_NO_EXCEPTIONS__ >)
endif()





#==================================================================================================
# project: Engine.Scene
#==================================================================================================
add_library( "Engine.Scene" STATIC 
		"Engine/Scene/Surface/OffscreenSurface.cpp"
		"Engine/Scene/Surface/VideoRecorderSurface.cpp"
		"Engine/Scene/Surface/WindowSurface.cpp"
		"Engine/Scene/Renderer/SceneRendererMainThread.cpp"
		"Engine/Scene/Shared/Camera.h"
		"Engine/Scene/Shared/Common.h"
		"Engine/Scene/Shared/IDs.h"
		"Engine/Scene/Shared/Scene.h"
		"Engine/Scene/Shared/Surface.h"
		"Engine/Scene/Camera/FreeCamera.cpp"
		"Engine/Scene/Camera/FreeVRCamera.cpp"
		"Engine/Scene/Impl/BaseSceneModule.cpp"
		"Engine/Scene/Impl/BaseSceneModule.h"
		"Engine/Scene/Impl/Main.cpp"
		"Engine/Scene/Impl/SceneMainThread.cpp"
		"Engine/Scene/Impl/SceneManager.cpp"
		"Engine/Scene/Impl/SceneObjectConstructor.h"
		"Engine/Scene/Engine.Scene.h"
		"Engine/Scene/Physics/ScenePhysicsMainThread.cpp"
		"Engine/Scene/Logic/SceneLogicMainThread.cpp" )

source_group( "Surface" FILES "Engine/Scene/Surface/OffscreenSurface.cpp" "Engine/Scene/Surface/VideoRecorderSurface.cpp" "Engine/Scene/Surface/WindowSurface.cpp" )
source_group( "Renderer" FILES "Engine/Scene/Renderer/SceneRendererMainThread.cpp" )
source_group( "Shared" FILES "Engine/Scene/Shared/Camera.h" "Engine/Scene/Shared/Common.h" "Engine/Scene/Shared/IDs.h" "Engine/Scene/Shared/Scene.h" "Engine/Scene/Shared/Surface.h" )
source_group( "Camera" FILES "Engine/Scene/Camera/FreeCamera.cpp" "Engine/Scene/Camera/FreeVRCamera.cpp" )
source_group( "Impl" FILES "Engine/Scene/Impl/BaseSceneModule.cpp" "Engine/Scene/Impl/BaseSceneModule.h" "Engine/Scene/Impl/Main.cpp" "Engine/Scene/Impl/SceneMainThread.cpp" "Engine/Scene/Impl/SceneManager.cpp" "Engine/Scene/Impl/SceneObjectConstructor.h" )
source_group( "" FILES "Engine/Scene/Engine.Scene.h" )
source_group( "Physics" FILES "Engine/Scene/Physics/ScenePhysicsMainThread.cpp" )
source_group( "Logic" FILES "Engine/Scene/Logic/SceneLogicMainThread.cpp" )
set_property( TARGET "Engine.Scene" PROPERTY FOLDER "Engine" )
target_include_directories( "Engine.Scene" PRIVATE "Engine/Scene" )
target_include_directories( "Engine.Scene" PUBLIC "Engine/.." )
target_link_libraries( "Engine.Scene" "Engine.Platforms" )
# compiler
if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
	# DebugAnalyze
	target_compile_options ( "Engine.Scene" PRIVATE $<$<CONFIG:DebugAnalyze>: /std:c++latest /MP /Gm- /Zc:inline /Gy- /fp:strict /fp:except- /we4002 /we4099 /we4129 /we4172 /we4201 /we4238 /we4239 /we4251 /we4263 /we4264 /we4266 /we4273 /we4473 /we4474 /we4480 /we4522 /we4552 /we4553 /we4700 /we4702 /we4715 /we4716 /we4717  /w14018 /w14100 /w14127 /w14189 /w14244 /w14245 /w14287 /w14389 /w14456 /w14457 /w14458 /w14459 /w14505 /w14668 /w14838 /w14946 /w14996 /w15038  /wd4061 /wd4062 /wd4063 /wd4310 /wd4365 /wd4503 /wd4514 /wd4623 /wd4625 /wd4626 /wd4710 /wd4714 /wd5026 /wd5027  /W4 /WX- /sdl /Od /Ob0 /EHsc /Oy- /GF- /GS /guard:cf /GR /MDd /analyze /Zi /RTCsu >)
	target_compile_definitions( "Engine.Scene" PRIVATE $<$<CONFIG:DebugAnalyze>: __GX_DEBUG__ __GX_ANALYZE__ >)
	set_target_properties( "Engine.Scene" PROPERTIES LINK_FLAGS_DEBUGANALYZE " /OPT:REF /OPT:ICF /INCREMENTAL:NO /DEBUG:FULL /LTCG" )
	# Debug
	target_compile_options ( "Engine.Scene" PRIVATE $<$<CONFIG:Debug>: /std:c++latest /MP /Gm- /Zc:inline /Gy- /fp:strict /fp:except- /we4002 /we4099 /we4129 /we4172 /we4201 /we4238 /we4239 /we4251 /we4263 /we4264 /we4266 /we4273 /we4473 /we4474 /we4480 /we4522 /we4552 /we4553 /we4700 /we4702 /we4715 /we4716 /we4717  /w14018 /w14100 /w14127 /w14189 /w14244 /w14245 /w14287 /w14389 /w14456 /w14457 /w14458 /w14459 /w14505 /w14668 /w14838 /w14946 /w14996 /w15038  /wd4061 /wd4062 /wd4063 /wd4310 /wd4365 /wd4503 /wd4514 /wd4623 /wd4625 /wd4626 /wd4710 /wd4714 /wd5026 /wd5027  /W4 /WX- /sdl /Od /Ob0 /EHsc /Oy- /GF- /GS /GR /MDd /analyze- /Zi /RTCsu >)
	target_compile_definitions( "Engine.Scene" PRIVATE $<$<CONFIG:Debug>: __GX_DEBUG__ >)
	set_target_properties( "Engine.Scene" PROPERTIES LINK_FLAGS_DEBUG " /OPT:REF /OPT:ICF /INCREMENTAL:NO /DEBUG:FULL /LTCG" )
	# Profile
	target_compile_options ( "Engine.Scene" PRIVATE $<$<CONFIG:Profile>: /std:c++latest /MP /Gm- /Zc:inline /Gy- /fp:strict /fp:except- /we4002 /we4099 /we4129 /we4172 /we4201 /we4238 /we4239 /we4251 /we4263 /we4264 /we4266 /we4273 /we4473 /we4474 /we4480 /we4522 /we4552 /we4553 /we4700 /we4702 /we4715 /we4716 /we4717  /w14018 /w14100 /w14127 /w14189 /w14244 /w14245 /w14287 /w14389 /w14456 /w14457 /w14458 /w14459 /w14505 /w14668 /w14838 /w14946 /w14996 /w15038  /wd4061 /wd4062 /wd4063 /wd4310 /wd4365 /wd4503 /wd4514 /wd4623 /wd4625 /wd4626 /wd4710 /wd4714 /wd5026 /wd5027  /Ob2 /Ob2 /Oi /Ot /Oy /MD /GT /GL /GF /GS- /W3 /Od /analyze- /EHsc /GR >)
	target_compile_definitions( "Engine.Scene" PRIVATE $<$<CONFIG:Profile>: GX_ENABLE_PROFILING >)
	set_target_properties( "Engine.Scene" PROPERTIES LINK_FLAGS_PROFILE " /OPT:REF /OPT:ICF /INCREMENTAL:NO /LTCG /DEBUG /PROFILE" )
	# Release
	target_compile_options ( "Engine.Scene" PRIVATE $<$<CONFIG:Release>: /std:c++latest /MP /Gm- /Zc:inline /Gy- /fp:strict /fp:except- /we4002 /we4099 /we4129 /we4172 /we4201 /we4238 /we4239 /we4251 /we4263 /we4264 /we4266 /we4273 /we4473 /we4474 /we4480 /we4522 /we4552 /we4553 /we4700 /we4702 /we4715 /we4716 /we4717  /w14018  /w14127 /w14189 /w14244 /w14245 /w14287 /w14389 /w14456 /w14457 /w14458 /w14459 /w14505 /w14668 /w14838 /w14946 /w14996 /w15038  /wd4061 /wd4062 /wd4063 /wd4310 /wd4365 /wd4503 /wd4514 /wd4623 /wd4625 /wd4626 /wd4710 /wd4714 /wd5026 /wd5027  /Ob2 /Ob2 /Oi /Ot /Oy /MD /GT /GL /GF /GS- /W3 /Ox /analyze- /EHs-c- /GR- >)
	target_compile_definitions( "Engine.Scene" PRIVATE $<$<CONFIG:Release>: __GX_FAST__ __GX_NO_EXCEPTIONS__ >)
	set_target_properties( "Engine.Scene" PROPERTIES LINK_FLAGS_RELEASE " /OPT:REF /OPT:ICF /INCREMENTAL:NO /LTCG /RELEASE /DYNAMICBASE" )
endif()


if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
	# DebugAnalyze
	target_compile_options ( "Engine.Scene" PRIVATE $<$<CONFIG:DebugAnalyze>: -std=c++1z -Wchar-subscripts -Wdouble-promotion -Wformat -Wmain -Wmissing-braces -Wmissing-include-dirs -Wunused -Wuninitialized -Wmaybe-uninitialized -Wunknown-pragmas -Wpragmas -Wstrict-aliasing -Wstrict-overflow -Wundef -Wendif-labels -Wfree-nonheap-object -Wpointer-arith -Wcast-qual -Wcast-align -Wwrite-strings -Wconversion-null -Wzero-as-null-pointer-constant -Wenum-compare -Wsign-compare -Wsizeof-pointer-memaccess -Wlogical-op -Werror=init-self -Werror=parentheses -Werror=return-local-addr -Werror=return-type -Werror=non-template-friend -Werror=array-bounds -Werror=div-by-zero -Werror=address -Werror=missing-field-initializers -Werror=placement-new  -Wno-non-template-friend  -ggdb -fsanitize=undefined -fchkp-check-incomplete-type -Og >)
	target_compile_definitions( "Engine.Scene" PRIVATE $<$<CONFIG:DebugAnalyze>: __GX_DEBUG__ __GX_ANALYZE__ >)
	# Debug
	target_compile_options ( "Engine.Scene" PRIVATE $<$<CONFIG:Debug>: -std=c++1z -Wchar-subscripts -Wdouble-promotion -Wformat -Wmain -Wmissing-braces -Wmissing-include-dirs -Wunused -Wuninitialized -Wmaybe-uninitialized -Wunknown-pragmas -Wpragmas -Wstrict-aliasing -Wstrict-overflow -Wundef -Wendif-labels -Wfree-nonheap-object -Wpointer-arith -Wcast-qual -Wcast-align -Wwrite-strings -Wconversion-null -Wzero-as-null-pointer-constant -Wenum-compare -Wsign-compare -Wsizeof-pointer-memaccess -Wlogical-op -Werror=init-self -Werror=parentheses -Werror=return-local-addr -Werror=return-type -Werror=non-template-friend -Werror=array-bounds -Werror=div-by-zero -Werror=address -Werror=missing-field-initializers -Werror=placement-new  -Wno-non-template-friend  -ggdb -fchkp-check-incomplete-type -Og >)
	target_compile_definitions( "Engine.Scene" PRIVATE $<$<CONFIG:Debug>: __GX_DEBUG__ >)
	# Profile
	target_compile_options ( "Engine.Scene" PRIVATE $<$<CONFIG:Profile>: -std=c++1z -Wchar-subscripts -Wdouble-promotion -Wformat -Wmain -Wmissing-braces -Wmissing-include-dirs -Wunused -Wuninitialized -Wmaybe-uninitialized -Wunknown-pragmas -Wpragmas -Wstrict-aliasing -Wstrict-overflow -Wundef -Wendif-labels -Wfree-nonheap-object -Wpointer-arith -Wcast-qual -Wcast-align -Wwrite-strings -Wconversion-null -Wzero-as-null-pointer-constant -Wenum-compare -Wsign-compare -Wsizeof-pointer-memaccess -Wlogical-op -Werror=init-self -Werror=parentheses -Werror=return-local-addr -Werror=return-type -Werror=non-template-friend -Werror=array-bounds -Werror=div-by-zero -Werror=address -Werror=missing-field-initializers -Werror=placement-new  -Wno-non-template-friend  -O2 -finline-functions >)
	target_compile_definitions( "Engine.Scene" PRIVATE $<$<CONFIG:Profile>: GX_ENABLE_PROFILING >)
	# Release
	target_compile_options ( "Engine.Scene" PRIVATE $<$<CONFIG:Release>: -std=c++1z -Wchar-subscripts -Wdouble-promotion -Wformat -Wmain -Wmissing-braces -Wmissing-include-dirs -Wunused -Wuninitialized -Wmaybe-uninitialized -Wunknown-pragmas -Wpragmas -Wstrict-aliasing -Wstrict-overflow -Wundef -Wendif-labels -Wfree-nonheap-object -Wpointer-arith -Wcast-qual -Wcast-align -Wwrite-strings -Wconversion-null -Wzero-as-null-pointer-constant -Wenum-compare -Wsign-compare -Wsizeof-pointer-memaccess -Wlogical-op -Werror=init-self -Werror=parentheses -Werror=return-local-addr -Werror=return-type -Werror=non-template-friend -Werror=array-bounds -Werror=div-by-zero -Werror=address -Werror=missing-field-initializers -Werror=placement-new  -Wno-non-template-friend  -O3 -Ofast -fomit-frame-pointer -finline-functions >)
	target_compile_definitions( "Engine.Scene" PRIVATE $<$<CONFIG:Release>: __GX_FAST__ __GX_NO_EXCEPTIONS__ >)
endif()


if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
	target_compile_definitions( "Engine.Scene" PRIVATE $<$<CONFIG:DebugAnalyze>: __GX_DEBUG__ __GX_ANALYZE__ >)
	target_compile_definitions( "Engine.Scene" PRIVATE $<$<CONFIG:Debug>: __GX_DEBUG__ >)
	target_compile_definitions( "Engine.Scene" PRIVATE $<$<CONFIG:Profile>: GX_ENABLE_PROFILING >)
	target_compile_definitions( "Engine.Scene" PRIVATE $<$<CONFIG:Release>: __GX_FAST__ __GX_NO_EXCEPTIONS__ >)
endif()





#==================================================================================================
# project: Tests.Engine.Base
#==================================================================================================
add_executable( "Tests.Engine.Base" 
		"Tests/Engine.Base/Graphics/GApp.cpp"
		"Tests/Engine.Base/Graphics/GApp.h"
		"Tests/Engine.Base/Graphics/Test.GWindow.cpp"
		"Tests/Engine.Base/OpenCL/Test.CLCompute.cpp"
		"Tests/Engine.Base/Pipelines/all_pipelines.h"
		"Tests/Engine.Base/Pipelines/common.h"
		"Tests/Engine.Base/Pipelines/default.cpp"
		"Tests/Engine.Base/Pipelines/default2.cpp"
		"Tests/Engine.Base/Pipelines/Default3.cpp"
		"Tests/Engine.Base/Pipelines/multitextured.cpp"
		"Tests/Engine.Base/Pipelines/shared_types.h"
		"Tests/Engine.Base/Window/Test.Window.cpp"
		"Tests/Engine.Base/Common.h"
		"Tests/Engine.Base/Main.cpp" )

source_group( "Graphics" FILES "Tests/Engine.Base/Graphics/GApp.cpp" "Tests/Engine.Base/Graphics/GApp.h" "Tests/Engine.Base/Graphics/Test.GWindow.cpp" )
source_group( "OpenCL" FILES "Tests/Engine.Base/OpenCL/Test.CLCompute.cpp" )
source_group( "Pipelines" FILES "Tests/Engine.Base/Pipelines/all_pipelines.h" "Tests/Engine.Base/Pipelines/common.h" "Tests/Engine.Base/Pipelines/default.cpp" "Tests/Engine.Base/Pipelines/default2.cpp" "Tests/Engine.Base/Pipelines/Default3.cpp" "Tests/Engine.Base/Pipelines/multitextured.cpp" "Tests/Engine.Base/Pipelines/shared_types.h" )
source_group( "Window" FILES "Tests/Engine.Base/Window/Test.Window.cpp" )
source_group( "" FILES "Tests/Engine.Base/Common.h" "Tests/Engine.Base/Main.cpp" )
set_property( TARGET "Tests.Engine.Base" PROPERTY FOLDER "Tests" )
target_include_directories( "Tests.Engine.Base" PRIVATE "Tests/Engine.Base" )
target_include_directories( "Tests.Engine.Base" PUBLIC "Engine/.." )
target_link_libraries( "Tests.Engine.Base" "Engine.Platforms" )
# compiler
if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
	# DebugAnalyze
	target_compile_options ( "Tests.Engine.Base" PRIVATE $<$<CONFIG:DebugAnalyze>: /std:c++latest /MP /Gm- /Zc:inline /Gy- /fp:strict /fp:except- /we4002 /we4099 /we4129 /we4172 /we4201 /we4238 /we4239 /we4251 /we4263 /we4264 /we4266 /we4273 /we4473 /we4474 /we4480 /we4522 /we4552 /we4553 /we4700 /we4702 /we4715 /we4716 /we4717  /w14018 /w14100 /w14127 /w14189 /w14244 /w14245 /w14287 /w14389 /w14456 /w14457 /w14458 /w14459 /w14505 /w14668 /w14838 /w14946 /w14996 /w15038  /wd4061 /wd4062 /wd4063 /wd4310 /wd4365 /wd4503 /wd4514 /wd4623 /wd4625 /wd4626 /wd4710 /wd4714 /wd5026 /wd5027  /W4 /WX- /sdl /Od /Ob0 /EHsc /Oy- /GF- /GS /guard:cf /GR /MDd /analyze /Zi /RTCsu >)
	target_compile_definitions( "Tests.Engine.Base" PRIVATE $<$<CONFIG:DebugAnalyze>: __GX_DEBUG__ __GX_ANALYZE__ >)
	set_target_properties( "Tests.Engine.Base" PROPERTIES LINK_FLAGS_DEBUGANALYZE " /OPT:REF /OPT:ICF /INCREMENTAL:NO /DEBUG:FULL /LTCG" )
	# Debug
	target_compile_options ( "Tests.Engine.Base" PRIVATE $<$<CONFIG:Debug>: /std:c++latest /MP /Gm- /Zc:inline /Gy- /fp:strict /fp:except- /we4002 /we4099 /we4129 /we4172 /we4201 /we4238 /we4239 /we4251 /we4263 /we4264 /we4266 /we4273 /we4473 /we4474 /we4480 /we4522 /we4552 /we4553 /we4700 /we4702 /we4715 /we4716 /we4717  /w14018 /w14100 /w14127 /w14189 /w14244 /w14245 /w14287 /w14389 /w14456 /w14457 /w14458 /w14459 /w14505 /w14668 /w14838 /w14946 /w14996 /w15038  /wd4061 /wd4062 /wd4063 /wd4310 /wd4365 /wd4503 /wd4514 /wd4623 /wd4625 /wd4626 /wd4710 /wd4714 /wd5026 /wd5027  /W4 /WX- /sdl /Od /Ob0 /EHsc /Oy- /GF- /GS /GR /MDd /analyze- /Zi /RTCsu >)
	target_compile_definitions( "Tests.Engine.Base" PRIVATE $<$<CONFIG:Debug>: __GX_DEBUG__ >)
	set_target_properties( "Tests.Engine.Base" PROPERTIES LINK_FLAGS_DEBUG " /OPT:REF /OPT:ICF /INCREMENTAL:NO /DEBUG:FULL /LTCG" )
	# Profile
	target_compile_options ( "Tests.Engine.Base" PRIVATE $<$<CONFIG:Profile>: /std:c++latest /MP /Gm- /Zc:inline /Gy- /fp:strict /fp:except- /we4002 /we4099 /we4129 /we4172 /we4201 /we4238 /we4239 /we4251 /we4263 /we4264 /we4266 /we4273 /we4473 /we4474 /we4480 /we4522 /we4552 /we4553 /we4700 /we4702 /we4715 /we4716 /we4717  /w14018 /w14100 /w14127 /w14189 /w14244 /w14245 /w14287 /w14389 /w14456 /w14457 /w14458 /w14459 /w14505 /w14668 /w14838 /w14946 /w14996 /w15038  /wd4061 /wd4062 /wd4063 /wd4310 /wd4365 /wd4503 /wd4514 /wd4623 /wd4625 /wd4626 /wd4710 /wd4714 /wd5026 /wd5027  /Ob2 /Ob2 /Oi /Ot /Oy /MD /GT /GL /GF /GS- /W3 /Od /analyze- /EHsc /GR >)
	target_compile_definitions( "Tests.Engine.Base" PRIVATE $<$<CONFIG:Profile>: GX_ENABLE_PROFILING >)
	set_target_properties( "Tests.Engine.Base" PROPERTIES LINK_FLAGS_PROFILE " /OPT:REF /OPT:ICF /INCREMENTAL:NO /LTCG /DEBUG /PROFILE" )
	# Release
	target_compile_options ( "Tests.Engine.Base" PRIVATE $<$<CONFIG:Release>: /std:c++latest /MP /Gm- /Zc:inline /Gy- /fp:strict /fp:except- /we4002 /we4099 /we4129 /we4172 /we4201 /we4238 /we4239 /we4251 /we4263 /we4264 /we4266 /we4273 /we4473 /we4474 /we4480 /we4522 /we4552 /we4553 /we4700 /we4702 /we4715 /we4716 /we4717  /w14018  /w14127 /w14189 /w14244 /w14245 /w14287 /w14389 /w14456 /w14457 /w14458 /w14459 /w14505 /w14668 /w14838 /w14946 /w14996 /w15038  /wd4061 /wd4062 /wd4063 /wd4310 /wd4365 /wd4503 /wd4514 /wd4623 /wd4625 /wd4626 /wd4710 /wd4714 /wd5026 /wd5027  /Ob2 /Ob2 /Oi /Ot /Oy /MD /GT /GL /GF /GS- /W3 /Ox /analyze- /EHs-c- /GR- >)
	target_compile_definitions( "Tests.Engine.Base" PRIVATE $<$<CONFIG:Release>: __GX_FAST__ __GX_NO_EXCEPTIONS__ >)
	set_target_properties( "Tests.Engine.Base" PROPERTIES LINK_FLAGS_RELEASE " /OPT:REF /OPT:ICF /INCREMENTAL:NO /LTCG /RELEASE /DYNAMICBASE" )
endif()


if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
	# DebugAnalyze
	target_compile_options ( "Tests.Engine.Base" PRIVATE $<$<CONFIG:DebugAnalyze>: -std=c++1z -Wchar-subscripts -Wdouble-promotion -Wformat -Wmain -Wmissing-braces -Wmissing-include-dirs -Wunused -Wuninitialized -Wmaybe-uninitialized -Wunknown-pragmas -Wpragmas -Wstrict-aliasing -Wstrict-overflow -Wundef -Wendif-labels -Wfree-nonheap-object -Wpointer-arith -Wcast-qual -Wcast-align -Wwrite-strings -Wconversion-null -Wzero-as-null-pointer-constant -Wenum-compare -Wsign-compare -Wsizeof-pointer-memaccess -Wlogical-op -Werror=init-self -Werror=parentheses -Werror=return-local-addr -Werror=return-type -Werror=non-template-friend -Werror=array-bounds -Werror=div-by-zero -Werror=address -Werror=missing-field-initializers -Werror=placement-new  -Wno-non-template-friend  -ggdb -fsanitize=undefined -fchkp-check-incomplete-type -Og >)
	target_compile_definitions( "Tests.Engine.Base" PRIVATE $<$<CONFIG:DebugAnalyze>: __GX_DEBUG__ __GX_ANALYZE__ >)
	# Debug
	target_compile_options ( "Tests.Engine.Base" PRIVATE $<$<CONFIG:Debug>: -std=c++1z -Wchar-subscripts -Wdouble-promotion -Wformat -Wmain -Wmissing-braces -Wmissing-include-dirs -Wunused -Wuninitialized -Wmaybe-uninitialized -Wunknown-pragmas -Wpragmas -Wstrict-aliasing -Wstrict-overflow -Wundef -Wendif-labels -Wfree-nonheap-object -Wpointer-arith -Wcast-qual -Wcast-align -Wwrite-strings -Wconversion-null -Wzero-as-null-pointer-constant -Wenum-compare -Wsign-compare -Wsizeof-pointer-memaccess -Wlogical-op -Werror=init-self -Werror=parentheses -Werror=return-local-addr -Werror=return-type -Werror=non-template-friend -Werror=array-bounds -Werror=div-by-zero -Werror=address -Werror=missing-field-initializers -Werror=placement-new  -Wno-non-template-friend  -ggdb -fchkp-check-incomplete-type -Og >)
	target_compile_definitions( "Tests.Engine.Base" PRIVATE $<$<CONFIG:Debug>: __GX_DEBUG__ >)
	# Profile
	target_compile_options ( "Tests.Engine.Base" PRIVATE $<$<CONFIG:Profile>: -std=c++1z -Wchar-subscripts -Wdouble-promotion -Wformat -Wmain -Wmissing-braces -Wmissing-include-dirs -Wunused -Wuninitialized -Wmaybe-uninitialized -Wunknown-pragmas -Wpragmas -Wstrict-aliasing -Wstrict-overflow -Wundef -Wendif-labels -Wfree-nonheap-object -Wpointer-arith -Wcast-qual -Wcast-align -Wwrite-strings -Wconversion-null -Wzero-as-null-pointer-constant -Wenum-compare -Wsign-compare -Wsizeof-pointer-memaccess -Wlogical-op -Werror=init-self -Werror=parentheses -Werror=return-local-addr -Werror=return-type -Werror=non-template-friend -Werror=array-bounds -Werror=div-by-zero -Werror=address -Werror=missing-field-initializers -Werror=placement-new  -Wno-non-template-friend  -O2 -finline-functions >)
	target_compile_definitions( "Tests.Engine.Base" PRIVATE $<$<CONFIG:Profile>: GX_ENABLE_PROFILING >)
	# Release
	target_compile_options ( "Tests.Engine.Base" PRIVATE $<$<CONFIG:Release>: -std=c++1z -Wchar-subscripts -Wdouble-promotion -Wformat -Wmain -Wmissing-braces -Wmissing-include-dirs -Wunused -Wuninitialized -Wmaybe-uninitialized -Wunknown-pragmas -Wpragmas -Wstrict-aliasing -Wstrict-overflow -Wundef -Wendif-labels -Wfree-nonheap-object -Wpointer-arith -Wcast-qual -Wcast-align -Wwrite-strings -Wconversion-null -Wzero-as-null-pointer-constant -Wenum-compare -Wsign-compare -Wsizeof-pointer-memaccess -Wlogical-op -Werror=init-self -Werror=parentheses -Werror=return-local-addr -Werror=return-type -Werror=non-template-friend -Werror=array-bounds -Werror=div-by-zero -Werror=address -Werror=missing-field-initializers -Werror=placement-new  -Wno-non-template-friend  -O3 -Ofast -fomit-frame-pointer -finline-functions >)
	target_compile_definitions( "Tests.Engine.Base" PRIVATE $<$<CONFIG:Release>: __GX_FAST__ __GX_NO_EXCEPTIONS__ >)
endif()


if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
	target_compile_definitions( "Tests.Engine.Base" PRIVATE $<$<CONFIG:DebugAnalyze>: __GX_DEBUG__ __GX_ANALYZE__ >)
	target_compile_definitions( "Tests.Engine.Base" PRIVATE $<$<CONFIG:Debug>: __GX_DEBUG__ >)
	target_compile_definitions( "Tests.Engine.Base" PRIVATE $<$<CONFIG:Profile>: GX_ENABLE_PROFILING >)
	target_compile_definitions( "Tests.Engine.Base" PRIVATE $<$<CONFIG:Release>: __GX_FAST__ __GX_NO_EXCEPTIONS__ >)
endif()





#==================================================================================================
# project: Engine.PipelineCompiler
#==================================================================================================
if (WIN32)
	add_library( "Engine.PipelineCompiler" STATIC 
			"Engine/PipelineCompiler/Serializers/CppSerializer.cpp"
			"Engine/PipelineCompiler/Serializers/CppSerializer.h"
			"Engine/PipelineCompiler/Serializers/ISerializer.h"
			"Engine/PipelineCompiler/Common/Common.h"
			"Engine/PipelineCompiler/Common/Packing.h"
			"Engine/PipelineCompiler/Common/ShaderEnums.h"
			"Engine/PipelineCompiler/Common/ToGLSL.h"
			"Engine/PipelineCompiler/Pipelines/BasePipeline.cpp"
			"Engine/PipelineCompiler/Pipelines/BasePipeline.h"
			"Engine/PipelineCompiler/Pipelines/BasePipeline_Pass1.cpp"
			"Engine/PipelineCompiler/Pipelines/BasePipeline_Pass2.cpp"
			"Engine/PipelineCompiler/Pipelines/ComputePipeline.cpp"
			"Engine/PipelineCompiler/Pipelines/ComputePipeline.h"
			"Engine/PipelineCompiler/Pipelines/GraphicsPipeline.cpp"
			"Engine/PipelineCompiler/Pipelines/GraphicsPipeline.h"
			"Engine/PipelineCompiler/Pipelines/PipelineManager.cpp"
			"Engine/PipelineCompiler/Pipelines/PipelineManager.h"
			"Engine/PipelineCompiler/Shaders/DeserializedShader.cpp"
			"Engine/PipelineCompiler/Shaders/DeserializedShader.h"
			"Engine/PipelineCompiler/Shaders/LunarGLASS_Include.h"
			"Engine/PipelineCompiler/Shaders/ShaderCompiler.cpp"
			"Engine/PipelineCompiler/Shaders/ShaderCompiler.h"
			"Engine/PipelineCompiler/Shaders/ShaderCompiler_CLCompiler.cpp"
			"Engine/PipelineCompiler/Shaders/ShaderCompiler_CLTranslator.cpp"
			"Engine/PipelineCompiler/Shaders/ShaderCompiler_CPPTranslator.cpp"
			"Engine/PipelineCompiler/Shaders/ShaderCompiler_DXTranslator.cpp"
			"Engine/PipelineCompiler/Shaders/ShaderCompiler_GLCompiler.cpp"
			"Engine/PipelineCompiler/Shaders/ShaderCompiler_GLParser.cpp"
			"Engine/PipelineCompiler/Shaders/ShaderCompiler_GXTranslator.cpp"
			"Engine/PipelineCompiler/Shaders/ShaderCompiler_ReplaceTypes.cpp"
			"Engine/PipelineCompiler/Shaders/ShaderCompiler_Translator.cpp"
			"Engine/PipelineCompiler/Shaders/ShaderCompiler_Translator.h"
			"Engine/PipelineCompiler/Shaders/ShaderCompiler_Utils.h" )
	
	source_group( "Serializers" FILES "Engine/PipelineCompiler/Serializers/CppSerializer.cpp" "Engine/PipelineCompiler/Serializers/CppSerializer.h" "Engine/PipelineCompiler/Serializers/ISerializer.h" )
	source_group( "Common" FILES "Engine/PipelineCompiler/Common/Common.h" "Engine/PipelineCompiler/Common/Packing.h" "Engine/PipelineCompiler/Common/ShaderEnums.h" "Engine/PipelineCompiler/Common/ToGLSL.h" )
	source_group( "Pipelines" FILES "Engine/PipelineCompiler/Pipelines/BasePipeline.cpp" "Engine/PipelineCompiler/Pipelines/BasePipeline.h" "Engine/PipelineCompiler/Pipelines/BasePipeline_Pass1.cpp" "Engine/PipelineCompiler/Pipelines/BasePipeline_Pass2.cpp" "Engine/PipelineCompiler/Pipelines/ComputePipeline.cpp" "Engine/PipelineCompiler/Pipelines/ComputePipeline.h" "Engine/PipelineCompiler/Pipelines/GraphicsPipeline.cpp" "Engine/PipelineCompiler/Pipelines/GraphicsPipeline.h" "Engine/PipelineCompiler/Pipelines/PipelineManager.cpp" "Engine/PipelineCompiler/Pipelines/PipelineManager.h" )
	source_group( "Shaders" FILES "Engine/PipelineCompiler/Shaders/DeserializedShader.cpp" "Engine/PipelineCompiler/Shaders/DeserializedShader.h" "Engine/PipelineCompiler/Shaders/LunarGLASS_Include.h" "Engine/PipelineCompiler/Shaders/ShaderCompiler.cpp" "Engine/PipelineCompiler/Shaders/ShaderCompiler.h" "Engine/PipelineCompiler/Shaders/ShaderCompiler_CLCompiler.cpp" "Engine/PipelineCompiler/Shaders/ShaderCompiler_CLTranslator.cpp" "Engine/PipelineCompiler/Shaders/ShaderCompiler_CPPTranslator.cpp" "Engine/PipelineCompiler/Shaders/ShaderCompiler_DXTranslator.cpp" "Engine/PipelineCompiler/Shaders/ShaderCompiler_GLCompiler.cpp" "Engine/PipelineCompiler/Shaders/ShaderCompiler_GLParser.cpp" "Engine/PipelineCompiler/Shaders/ShaderCompiler_GXTranslator.cpp" "Engine/PipelineCompiler/Shaders/ShaderCompiler_ReplaceTypes.cpp" "Engine/PipelineCompiler/Shaders/ShaderCompiler_Translator.cpp" "Engine/PipelineCompiler/Shaders/ShaderCompiler_Translator.h" "Engine/PipelineCompiler/Shaders/ShaderCompiler_Utils.h" )
	set_property( TARGET "Engine.PipelineCompiler" PROPERTY FOLDER "EngineTools" )
	target_include_directories( "Engine.PipelineCompiler" PRIVATE "Engine/PipelineCompiler" )
	target_include_directories( "Engine.PipelineCompiler" PUBLIC "Engine/.." )
	target_include_directories( "Engine.PipelineCompiler" PRIVATE "build_LunarGLASS/Core/LLVM/llvm-3.4/include/llvm/.." )
	target_include_directories( "Engine.PipelineCompiler" PRIVATE "External/LunarGLASS/glslang/.." )
	target_include_directories( "Engine.PipelineCompiler" PRIVATE "External/LunarGLASS/.." )
	target_include_directories( "Engine.PipelineCompiler" PRIVATE "External/LunarGLASS/glslang" )
	target_include_directories( "Engine.PipelineCompiler" PRIVATE "External/LunarGLASS" )
	target_include_directories( "Engine.PipelineCompiler" PRIVATE "External/LunarGLASS/Core/LLVM/llvm-3.4/include/llvm/.." )
	target_link_libraries( "Engine.PipelineCompiler" "Engine.Platforms" )
	if (MSVC)
		add_dependencies( "Engine.PipelineCompiler" "LLVMTransformUtils" )
	endif()
	if (MSVC)
		add_dependencies( "Engine.PipelineCompiler" "LLVMAsmParser" )
	endif()
	if (MSVC)
		add_dependencies( "Engine.PipelineCompiler" "OSDependent" )
	endif()
	if (MSVC)
		add_dependencies( "Engine.PipelineCompiler" "LLVMAnalysis" )
	endif()
	if (MSVC)
		add_dependencies( "Engine.PipelineCompiler" "SPVRemapper" )
	endif()
	if (MSVC)
		add_dependencies( "Engine.PipelineCompiler" "LLVMCore" )
	endif()
	if (MSVC)
		add_dependencies( "Engine.PipelineCompiler" "LLVMipa" )
	endif()
	if (MSVC)
		add_dependencies( "Engine.PipelineCompiler" "core" )
	endif()
	if (MSVC)
		add_dependencies( "Engine.PipelineCompiler" "glslangFrontend" )
	endif()
	if (MSVC)
		add_dependencies( "Engine.PipelineCompiler" "SPIRV" )
	endif()
	if (MSVC)
		add_dependencies( "Engine.PipelineCompiler" "SPIRV-Tools-opt" )
	endif()
	if (MSVC)
		add_dependencies( "Engine.PipelineCompiler" "LLVMTarget" )
	endif()
	if (MSVC)
		add_dependencies( "Engine.PipelineCompiler" "OGLCompiler" )
	endif()
	if (MSVC)
		add_dependencies( "Engine.PipelineCompiler" "HLSL" )
	endif()
	if (MSVC)
		add_dependencies( "Engine.PipelineCompiler" "LLVMSupport" )
	endif()
	if (MSVC)
		add_dependencies( "Engine.PipelineCompiler" "glslang" )
	endif()
	if (MSVC)
		add_dependencies( "Engine.PipelineCompiler" "LLVMipo" )
	endif()
	if (MSVC)
		add_dependencies( "Engine.PipelineCompiler" "LLVMInstCombine" )
	endif()
	if (MSVC)
		add_dependencies( "Engine.PipelineCompiler" "GLSLBackend" )
	endif()
	if (MSVC)
		add_dependencies( "Engine.PipelineCompiler" "LLVMScalarOpts" )
	endif()
	if (MSVC)
		add_dependencies( "Engine.PipelineCompiler" "LLVMLinker" )
	endif()
	if (MSVC)
		add_dependencies( "Engine.PipelineCompiler" "SPIRV-Tools" )
	endif()
	if (MSVC)
		add_dependencies( "Engine.PipelineCompiler" "SpvFrontend" )
	endif()
	# compiler
	if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
		# DebugAnalyze
		target_compile_options ( "Engine.PipelineCompiler" PRIVATE $<$<CONFIG:DebugAnalyze>: /std:c++latest /MP /Gm- /Zc:inline /Gy- /fp:strict /fp:except- /we4002 /we4099 /we4129 /we4172 /we4201 /we4238 /we4239 /we4251 /we4263 /we4264 /we4266 /we4273 /we4473 /we4474 /we4480 /we4522 /we4552 /we4553 /we4700 /we4702 /we4715 /we4716 /we4717  /w14018 /w14100 /w14127 /w14189 /w14244 /w14245 /w14287 /w14389 /w14456 /w14457 /w14458 /w14459 /w14505 /w14668 /w14838 /w14946 /w14996 /w15038  /wd4061 /wd4062 /wd4063 /wd4310 /wd4365 /wd4503 /wd4514 /wd4623 /wd4625 /wd4626 /wd4710 /wd4714 /wd5026 /wd5027  /W4 /WX- /sdl /Od /Ob0 /EHsc /Oy- /GF- /GS /guard:cf /GR /MDd /analyze /Zi /RTCsu >)
		target_compile_definitions( "Engine.PipelineCompiler" PRIVATE $<$<CONFIG:DebugAnalyze>: __GX_DEBUG__ __GX_ANALYZE__ >)
		set_target_properties( "Engine.PipelineCompiler" PROPERTIES LINK_FLAGS_DEBUGANALYZE " /OPT:REF /OPT:ICF /INCREMENTAL:NO /DEBUG:FULL /LTCG" )
		# Debug
		target_compile_options ( "Engine.PipelineCompiler" PRIVATE $<$<CONFIG:Debug>: /std:c++latest /MP /Gm- /Zc:inline /Gy- /fp:strict /fp:except- /we4002 /we4099 /we4129 /we4172 /we4201 /we4238 /we4239 /we4251 /we4263 /we4264 /we4266 /we4273 /we4473 /we4474 /we4480 /we4522 /we4552 /we4553 /we4700 /we4702 /we4715 /we4716 /we4717  /w14018 /w14100 /w14127 /w14189 /w14244 /w14245 /w14287 /w14389 /w14456 /w14457 /w14458 /w14459 /w14505 /w14668 /w14838 /w14946 /w14996 /w15038  /wd4061 /wd4062 /wd4063 /wd4310 /wd4365 /wd4503 /wd4514 /wd4623 /wd4625 /wd4626 /wd4710 /wd4714 /wd5026 /wd5027  /W4 /WX- /sdl /Od /Ob0 /EHsc /Oy- /GF- /GS /GR /MDd /analyze- /Zi /RTCsu >)
		target_compile_definitions( "Engine.PipelineCompiler" PRIVATE $<$<CONFIG:Debug>: __GX_DEBUG__ >)
		set_target_properties( "Engine.PipelineCompiler" PROPERTIES LINK_FLAGS_DEBUG " /OPT:REF /OPT:ICF /INCREMENTAL:NO /DEBUG:FULL /LTCG" )
		# Profile
		target_compile_options ( "Engine.PipelineCompiler" PRIVATE $<$<CONFIG:Profile>: /std:c++latest /MP /Gm- /Zc:inline /Gy- /fp:strict /fp:except- /we4002 /we4099 /we4129 /we4172 /we4201 /we4238 /we4239 /we4251 /we4263 /we4264 /we4266 /we4273 /we4473 /we4474 /we4480 /we4522 /we4552 /we4553 /we4700 /we4702 /we4715 /we4716 /we4717  /w14018 /w14100 /w14127 /w14189 /w14244 /w14245 /w14287 /w14389 /w14456 /w14457 /w14458 /w14459 /w14505 /w14668 /w14838 /w14946 /w14996 /w15038  /wd4061 /wd4062 /wd4063 /wd4310 /wd4365 /wd4503 /wd4514 /wd4623 /wd4625 /wd4626 /wd4710 /wd4714 /wd5026 /wd5027  /Ob2 /Ob2 /Oi /Ot /Oy /MD /GT /GL /GF /GS- /W3 /Od /analyze- /EHsc /GR >)
		target_compile_definitions( "Engine.PipelineCompiler" PRIVATE $<$<CONFIG:Profile>: GX_ENABLE_PROFILING >)
		set_target_properties( "Engine.PipelineCompiler" PROPERTIES LINK_FLAGS_PROFILE " /OPT:REF /OPT:ICF /INCREMENTAL:NO /LTCG /DEBUG /PROFILE" )
		# Release
		target_compile_options ( "Engine.PipelineCompiler" PRIVATE $<$<CONFIG:Release>: /std:c++latest /MP /Gm- /Zc:inline /Gy- /fp:strict /fp:except- /we4002 /we4099 /we4129 /we4172 /we4201 /we4238 /we4239 /we4251 /we4263 /we4264 /we4266 /we4273 /we4473 /we4474 /we4480 /we4522 /we4552 /we4553 /we4700 /we4702 /we4715 /we4716 /we4717  /w14018  /w14127 /w14189 /w14244 /w14245 /w14287 /w14389 /w14456 /w14457 /w14458 /w14459 /w14505 /w14668 /w14838 /w14946 /w14996 /w15038  /wd4061 /wd4062 /wd4063 /wd4310 /wd4365 /wd4503 /wd4514 /wd4623 /wd4625 /wd4626 /wd4710 /wd4714 /wd5026 /wd5027  /Ob2 /Ob2 /Oi /Ot /Oy /MD /GT /GL /GF /GS- /W3 /Ox /analyze- /EHs-c- /GR- >)
		target_compile_definitions( "Engine.PipelineCompiler" PRIVATE $<$<CONFIG:Release>: __GX_FAST__ __GX_NO_EXCEPTIONS__ >)
		set_target_properties( "Engine.PipelineCompiler" PROPERTIES LINK_FLAGS_RELEASE " /OPT:REF /OPT:ICF /INCREMENTAL:NO /LTCG /RELEASE /DYNAMICBASE" )
	endif()
	
	
	if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
		# DebugAnalyze
		target_compile_options ( "Engine.PipelineCompiler" PRIVATE $<$<CONFIG:DebugAnalyze>: -std=c++1z -Wchar-subscripts -Wdouble-promotion -Wformat -Wmain -Wmissing-braces -Wmissing-include-dirs -Wunused -Wuninitialized -Wmaybe-uninitialized -Wunknown-pragmas -Wpragmas -Wstrict-aliasing -Wstrict-overflow -Wundef -Wendif-labels -Wfree-nonheap-object -Wpointer-arith -Wcast-qual -Wcast-align -Wwrite-strings -Wconversion-null -Wzero-as-null-pointer-constant -Wenum-compare -Wsign-compare -Wsizeof-pointer-memaccess -Wlogical-op -Werror=init-self -Werror=parentheses -Werror=return-local-addr -Werror=return-type -Werror=non-template-friend -Werror=array-bounds -Werror=div-by-zero -Werror=address -Werror=missing-field-initializers -Werror=placement-new  -Wno-non-template-friend  -ggdb -fsanitize=undefined -fchkp-check-incomplete-type -Og >)
		target_compile_definitions( "Engine.PipelineCompiler" PRIVATE $<$<CONFIG:DebugAnalyze>: __GX_DEBUG__ __GX_ANALYZE__ >)
		# Debug
		target_compile_options ( "Engine.PipelineCompiler" PRIVATE $<$<CONFIG:Debug>: -std=c++1z -Wchar-subscripts -Wdouble-promotion -Wformat -Wmain -Wmissing-braces -Wmissing-include-dirs -Wunused -Wuninitialized -Wmaybe-uninitialized -Wunknown-pragmas -Wpragmas -Wstrict-aliasing -Wstrict-overflow -Wundef -Wendif-labels -Wfree-nonheap-object -Wpointer-arith -Wcast-qual -Wcast-align -Wwrite-strings -Wconversion-null -Wzero-as-null-pointer-constant -Wenum-compare -Wsign-compare -Wsizeof-pointer-memaccess -Wlogical-op -Werror=init-self -Werror=parentheses -Werror=return-local-addr -Werror=return-type -Werror=non-template-friend -Werror=array-bounds -Werror=div-by-zero -Werror=address -Werror=missing-field-initializers -Werror=placement-new  -Wno-non-template-friend  -ggdb -fchkp-check-incomplete-type -Og >)
		target_compile_definitions( "Engine.PipelineCompiler" PRIVATE $<$<CONFIG:Debug>: __GX_DEBUG__ >)
		# Profile
		target_compile_options ( "Engine.PipelineCompiler" PRIVATE $<$<CONFIG:Profile>: -std=c++1z -Wchar-subscripts -Wdouble-promotion -Wformat -Wmain -Wmissing-braces -Wmissing-include-dirs -Wunused -Wuninitialized -Wmaybe-uninitialized -Wunknown-pragmas -Wpragmas -Wstrict-aliasing -Wstrict-overflow -Wundef -Wendif-labels -Wfree-nonheap-object -Wpointer-arith -Wcast-qual -Wcast-align -Wwrite-strings -Wconversion-null -Wzero-as-null-pointer-constant -Wenum-compare -Wsign-compare -Wsizeof-pointer-memaccess -Wlogical-op -Werror=init-self -Werror=parentheses -Werror=return-local-addr -Werror=return-type -Werror=non-template-friend -Werror=array-bounds -Werror=div-by-zero -Werror=address -Werror=missing-field-initializers -Werror=placement-new  -Wno-non-template-friend  -O2 -finline-functions >)
		target_compile_definitions( "Engine.PipelineCompiler" PRIVATE $<$<CONFIG:Profile>: GX_ENABLE_PROFILING >)
		# Release
		target_compile_options ( "Engine.PipelineCompiler" PRIVATE $<$<CONFIG:Release>: -std=c++1z -Wchar-subscripts -Wdouble-promotion -Wformat -Wmain -Wmissing-braces -Wmissing-include-dirs -Wunused -Wuninitialized -Wmaybe-uninitialized -Wunknown-pragmas -Wpragmas -Wstrict-aliasing -Wstrict-overflow -Wundef -Wendif-labels -Wfree-nonheap-object -Wpointer-arith -Wcast-qual -Wcast-align -Wwrite-strings -Wconversion-null -Wzero-as-null-pointer-constant -Wenum-compare -Wsign-compare -Wsizeof-pointer-memaccess -Wlogical-op -Werror=init-self -Werror=parentheses -Werror=return-local-addr -Werror=return-type -Werror=non-template-friend -Werror=array-bounds -Werror=div-by-zero -Werror=address -Werror=missing-field-initializers -Werror=placement-new  -Wno-non-template-friend  -O3 -Ofast -fomit-frame-pointer -finline-functions >)
		target_compile_definitions( "Engine.PipelineCompiler" PRIVATE $<$<CONFIG:Release>: __GX_FAST__ __GX_NO_EXCEPTIONS__ >)
	endif()
	
	
	if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
		target_compile_definitions( "Engine.PipelineCompiler" PRIVATE $<$<CONFIG:DebugAnalyze>: __GX_DEBUG__ __GX_ANALYZE__ >)
		target_compile_definitions( "Engine.PipelineCompiler" PRIVATE $<$<CONFIG:Debug>: __GX_DEBUG__ >)
		target_compile_definitions( "Engine.PipelineCompiler" PRIVATE $<$<CONFIG:Profile>: GX_ENABLE_PROFILING >)
		target_compile_definitions( "Engine.PipelineCompiler" PRIVATE $<$<CONFIG:Release>: __GX_FAST__ __GX_NO_EXCEPTIONS__ >)
	endif()
	
	
	
	#-----------------------------------
	
	if (MSVC)
	    set_target_properties( "Engine.PipelineCompiler" PROPERTIES STATIC_LIBRARY_FLAGS "\" LinkLibraryDependencies=\"true" )
	endif()
	
	#-----------------------------------
endif()


#==================================================================================================
# project: Tests.PipelineCompiler
#==================================================================================================
if (WIN32)
	add_executable( "Tests.PipelineCompiler" 
			"Tests/PipelineCompiler/Pipelines/Default.cpp"
			"Tests/PipelineCompiler/Pipelines/Default2.cpp"
			"Tests/PipelineCompiler/Pipelines/Default3.cpp"
			"Tests/PipelineCompiler/Pipelines/Multitextured.cpp"
			"Tests/PipelineCompiler/Pipelines/Test1.cpp"
			"Tests/PipelineCompiler/Common.h"
			"Tests/PipelineCompiler/Main.cpp"
			"Tests/PipelineCompiler/Test_CompileGxsl.cpp"
			"Tests/PipelineCompiler/Test_DeserializeGLSL.cpp"
			"Tests/PipelineCompiler/Test_DeserializeGLSL_2.cpp"
			"Tests/PipelineCompiler/Test_DeserializeGLSL_3.cpp"
			"Tests/PipelineCompiler/Test_PipelineCompilation.cpp"
			"Tests/PipelineCompiler/Test_TranslateCL.cpp" )
	
	source_group( "Pipelines" FILES "Tests/PipelineCompiler/Pipelines/Default.cpp" "Tests/PipelineCompiler/Pipelines/Default2.cpp" "Tests/PipelineCompiler/Pipelines/Default3.cpp" "Tests/PipelineCompiler/Pipelines/Multitextured.cpp" "Tests/PipelineCompiler/Pipelines/Test1.cpp" )
	source_group( "" FILES "Tests/PipelineCompiler/Common.h" "Tests/PipelineCompiler/Main.cpp" "Tests/PipelineCompiler/Test_CompileGxsl.cpp" "Tests/PipelineCompiler/Test_DeserializeGLSL.cpp" "Tests/PipelineCompiler/Test_DeserializeGLSL_2.cpp" "Tests/PipelineCompiler/Test_DeserializeGLSL_3.cpp" "Tests/PipelineCompiler/Test_PipelineCompilation.cpp" "Tests/PipelineCompiler/Test_TranslateCL.cpp" )
	set_property( TARGET "Tests.PipelineCompiler" PROPERTY FOLDER "Tests" )
	target_include_directories( "Tests.PipelineCompiler" PRIVATE "Tests/PipelineCompiler" )
	target_include_directories( "Tests.PipelineCompiler" PUBLIC "Engine/.." )
	target_link_libraries( "Tests.PipelineCompiler" "Engine.PipelineCompiler" )
	# compiler
	if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
		# DebugAnalyze
		target_compile_options ( "Tests.PipelineCompiler" PRIVATE $<$<CONFIG:DebugAnalyze>: /std:c++latest /MP /Gm- /Zc:inline /Gy- /fp:strict /fp:except- /we4002 /we4099 /we4129 /we4172 /we4201 /we4238 /we4239 /we4251 /we4263 /we4264 /we4266 /we4273 /we4473 /we4474 /we4480 /we4522 /we4552 /we4553 /we4700 /we4702 /we4715 /we4716 /we4717  /w14018 /w14100 /w14127 /w14189 /w14244 /w14245 /w14287 /w14389 /w14456 /w14457 /w14458 /w14459 /w14505 /w14668 /w14838 /w14946 /w14996 /w15038  /wd4061 /wd4062 /wd4063 /wd4310 /wd4365 /wd4503 /wd4514 /wd4623 /wd4625 /wd4626 /wd4710 /wd4714 /wd5026 /wd5027  /W4 /WX- /sdl /Od /Ob0 /EHsc /Oy- /GF- /GS /guard:cf /GR /MDd /analyze /Zi /RTCsu >)
		target_compile_definitions( "Tests.PipelineCompiler" PRIVATE $<$<CONFIG:DebugAnalyze>: __GX_DEBUG__ __GX_ANALYZE__ >)
		set_target_properties( "Tests.PipelineCompiler" PROPERTIES LINK_FLAGS_DEBUGANALYZE " /OPT:REF /OPT:ICF /INCREMENTAL:NO /DEBUG:FULL /LTCG" )
		# Debug
		target_compile_options ( "Tests.PipelineCompiler" PRIVATE $<$<CONFIG:Debug>: /std:c++latest /MP /Gm- /Zc:inline /Gy- /fp:strict /fp:except- /we4002 /we4099 /we4129 /we4172 /we4201 /we4238 /we4239 /we4251 /we4263 /we4264 /we4266 /we4273 /we4473 /we4474 /we4480 /we4522 /we4552 /we4553 /we4700 /we4702 /we4715 /we4716 /we4717  /w14018 /w14100 /w14127 /w14189 /w14244 /w14245 /w14287 /w14389 /w14456 /w14457 /w14458 /w14459 /w14505 /w14668 /w14838 /w14946 /w14996 /w15038  /wd4061 /wd4062 /wd4063 /wd4310 /wd4365 /wd4503 /wd4514 /wd4623 /wd4625 /wd4626 /wd4710 /wd4714 /wd5026 /wd5027  /W4 /WX- /sdl /Od /Ob0 /EHsc /Oy- /GF- /GS /GR /MDd /analyze- /Zi /RTCsu >)
		target_compile_definitions( "Tests.PipelineCompiler" PRIVATE $<$<CONFIG:Debug>: __GX_DEBUG__ >)
		set_target_properties( "Tests.PipelineCompiler" PROPERTIES LINK_FLAGS_DEBUG " /OPT:REF /OPT:ICF /INCREMENTAL:NO /DEBUG:FULL /LTCG" )
		# Profile
		target_compile_options ( "Tests.PipelineCompiler" PRIVATE $<$<CONFIG:Profile>: /std:c++latest /MP /Gm- /Zc:inline /Gy- /fp:strict /fp:except- /we4002 /we4099 /we4129 /we4172 /we4201 /we4238 /we4239 /we4251 /we4263 /we4264 /we4266 /we4273 /we4473 /we4474 /we4480 /we4522 /we4552 /we4553 /we4700 /we4702 /we4715 /we4716 /we4717  /w14018 /w14100 /w14127 /w14189 /w14244 /w14245 /w14287 /w14389 /w14456 /w14457 /w14458 /w14459 /w14505 /w14668 /w14838 /w14946 /w14996 /w15038  /wd4061 /wd4062 /wd4063 /wd4310 /wd4365 /wd4503 /wd4514 /wd4623 /wd4625 /wd4626 /wd4710 /wd4714 /wd5026 /wd5027  /Ob2 /Ob2 /Oi /Ot /Oy /MD /GT /GL /GF /GS- /W3 /Od /analyze- /EHsc /GR >)
		target_compile_definitions( "Tests.PipelineCompiler" PRIVATE $<$<CONFIG:Profile>: GX_ENABLE_PROFILING >)
		set_target_properties( "Tests.PipelineCompiler" PROPERTIES LINK_FLAGS_PROFILE " /OPT:REF /OPT:ICF /INCREMENTAL:NO /LTCG /DEBUG /PROFILE" )
		# Release
		target_compile_options ( "Tests.PipelineCompiler" PRIVATE $<$<CONFIG:Release>: /std:c++latest /MP /Gm- /Zc:inline /Gy- /fp:strict /fp:except- /we4002 /we4099 /we4129 /we4172 /we4201 /we4238 /we4239 /we4251 /we4263 /we4264 /we4266 /we4273 /we4473 /we4474 /we4480 /we4522 /we4552 /we4553 /we4700 /we4702 /we4715 /we4716 /we4717  /w14018  /w14127 /w14189 /w14244 /w14245 /w14287 /w14389 /w14456 /w14457 /w14458 /w14459 /w14505 /w14668 /w14838 /w14946 /w14996 /w15038  /wd4061 /wd4062 /wd4063 /wd4310 /wd4365 /wd4503 /wd4514 /wd4623 /wd4625 /wd4626 /wd4710 /wd4714 /wd5026 /wd5027  /Ob2 /Ob2 /Oi /Ot /Oy /MD /GT /GL /GF /GS- /W3 /Ox /analyze- /EHs-c- /GR- >)
		target_compile_definitions( "Tests.PipelineCompiler" PRIVATE $<$<CONFIG:Release>: __GX_FAST__ __GX_NO_EXCEPTIONS__ >)
		set_target_properties( "Tests.PipelineCompiler" PROPERTIES LINK_FLAGS_RELEASE " /OPT:REF /OPT:ICF /INCREMENTAL:NO /LTCG /RELEASE /DYNAMICBASE" )
	endif()
	
	
	if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
		# DebugAnalyze
		target_compile_options ( "Tests.PipelineCompiler" PRIVATE $<$<CONFIG:DebugAnalyze>: -std=c++1z -Wchar-subscripts -Wdouble-promotion -Wformat -Wmain -Wmissing-braces -Wmissing-include-dirs -Wunused -Wuninitialized -Wmaybe-uninitialized -Wunknown-pragmas -Wpragmas -Wstrict-aliasing -Wstrict-overflow -Wundef -Wendif-labels -Wfree-nonheap-object -Wpointer-arith -Wcast-qual -Wcast-align -Wwrite-strings -Wconversion-null -Wzero-as-null-pointer-constant -Wenum-compare -Wsign-compare -Wsizeof-pointer-memaccess -Wlogical-op -Werror=init-self -Werror=parentheses -Werror=return-local-addr -Werror=return-type -Werror=non-template-friend -Werror=array-bounds -Werror=div-by-zero -Werror=address -Werror=missing-field-initializers -Werror=placement-new  -Wno-non-template-friend  -ggdb -fsanitize=undefined -fchkp-check-incomplete-type -Og >)
		target_compile_definitions( "Tests.PipelineCompiler" PRIVATE $<$<CONFIG:DebugAnalyze>: __GX_DEBUG__ __GX_ANALYZE__ >)
		# Debug
		target_compile_options ( "Tests.PipelineCompiler" PRIVATE $<$<CONFIG:Debug>: -std=c++1z -Wchar-subscripts -Wdouble-promotion -Wformat -Wmain -Wmissing-braces -Wmissing-include-dirs -Wunused -Wuninitialized -Wmaybe-uninitialized -Wunknown-pragmas -Wpragmas -Wstrict-aliasing -Wstrict-overflow -Wundef -Wendif-labels -Wfree-nonheap-object -Wpointer-arith -Wcast-qual -Wcast-align -Wwrite-strings -Wconversion-null -Wzero-as-null-pointer-constant -Wenum-compare -Wsign-compare -Wsizeof-pointer-memaccess -Wlogical-op -Werror=init-self -Werror=parentheses -Werror=return-local-addr -Werror=return-type -Werror=non-template-friend -Werror=array-bounds -Werror=div-by-zero -Werror=address -Werror=missing-field-initializers -Werror=placement-new  -Wno-non-template-friend  -ggdb -fchkp-check-incomplete-type -Og >)
		target_compile_definitions( "Tests.PipelineCompiler" PRIVATE $<$<CONFIG:Debug>: __GX_DEBUG__ >)
		# Profile
		target_compile_options ( "Tests.PipelineCompiler" PRIVATE $<$<CONFIG:Profile>: -std=c++1z -Wchar-subscripts -Wdouble-promotion -Wformat -Wmain -Wmissing-braces -Wmissing-include-dirs -Wunused -Wuninitialized -Wmaybe-uninitialized -Wunknown-pragmas -Wpragmas -Wstrict-aliasing -Wstrict-overflow -Wundef -Wendif-labels -Wfree-nonheap-object -Wpointer-arith -Wcast-qual -Wcast-align -Wwrite-strings -Wconversion-null -Wzero-as-null-pointer-constant -Wenum-compare -Wsign-compare -Wsizeof-pointer-memaccess -Wlogical-op -Werror=init-self -Werror=parentheses -Werror=return-local-addr -Werror=return-type -Werror=non-template-friend -Werror=array-bounds -Werror=div-by-zero -Werror=address -Werror=missing-field-initializers -Werror=placement-new  -Wno-non-template-friend  -O2 -finline-functions >)
		target_compile_definitions( "Tests.PipelineCompiler" PRIVATE $<$<CONFIG:Profile>: GX_ENABLE_PROFILING >)
		# Release
		target_compile_options ( "Tests.PipelineCompiler" PRIVATE $<$<CONFIG:Release>: -std=c++1z -Wchar-subscripts -Wdouble-promotion -Wformat -Wmain -Wmissing-braces -Wmissing-include-dirs -Wunused -Wuninitialized -Wmaybe-uninitialized -Wunknown-pragmas -Wpragmas -Wstrict-aliasing -Wstrict-overflow -Wundef -Wendif-labels -Wfree-nonheap-object -Wpointer-arith -Wcast-qual -Wcast-align -Wwrite-strings -Wconversion-null -Wzero-as-null-pointer-constant -Wenum-compare -Wsign-compare -Wsizeof-pointer-memaccess -Wlogical-op -Werror=init-self -Werror=parentheses -Werror=return-local-addr -Werror=return-type -Werror=non-template-friend -Werror=array-bounds -Werror=div-by-zero -Werror=address -Werror=missing-field-initializers -Werror=placement-new  -Wno-non-template-friend  -O3 -Ofast -fomit-frame-pointer -finline-functions >)
		target_compile_definitions( "Tests.PipelineCompiler" PRIVATE $<$<CONFIG:Release>: __GX_FAST__ __GX_NO_EXCEPTIONS__ >)
	endif()
	
	
	if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
		target_compile_definitions( "Tests.PipelineCompiler" PRIVATE $<$<CONFIG:DebugAnalyze>: __GX_DEBUG__ __GX_ANALYZE__ >)
		target_compile_definitions( "Tests.PipelineCompiler" PRIVATE $<$<CONFIG:Debug>: __GX_DEBUG__ >)
		target_compile_definitions( "Tests.PipelineCompiler" PRIVATE $<$<CONFIG:Profile>: GX_ENABLE_PROFILING >)
		target_compile_definitions( "Tests.PipelineCompiler" PRIVATE $<$<CONFIG:Release>: __GX_FAST__ __GX_NO_EXCEPTIONS__ >)
	endif()
	
	
	
endif()


#==================================================================================================
# project: Projects.ShaderEditor
#==================================================================================================
add_executable( "Projects.ShaderEditor" 
		"Projects/ShaderEditor/Pipelines/all_pipelines.h"
		"Projects/ShaderEditor/Pipelines/common.h"
		"Projects/ShaderEditor/Pipelines/drawtexturedquad.cpp"
		"Projects/ShaderEditor/Pipelines/experimental.cpp"
		"Projects/ShaderEditor/Pipelines/shared_types.h"
		"Projects/ShaderEditor/Pipelines/voronnoirecursion.cpp"
		"Projects/ShaderEditor/Common.h"
		"Projects/ShaderEditor/Renderer.cpp"
		"Projects/ShaderEditor/Renderer.h"
		"Projects/ShaderEditor/ShaderEditorApp.cpp"
		"Projects/ShaderEditor/ShaderEditorApp.h" )

source_group( "Pipelines" FILES "Projects/ShaderEditor/Pipelines/all_pipelines.h" "Projects/ShaderEditor/Pipelines/common.h" "Projects/ShaderEditor/Pipelines/drawtexturedquad.cpp" "Projects/ShaderEditor/Pipelines/experimental.cpp" "Projects/ShaderEditor/Pipelines/shared_types.h" "Projects/ShaderEditor/Pipelines/voronnoirecursion.cpp" )
source_group( "" FILES "Projects/ShaderEditor/Common.h" "Projects/ShaderEditor/Renderer.cpp" "Projects/ShaderEditor/Renderer.h" "Projects/ShaderEditor/ShaderEditorApp.cpp" "Projects/ShaderEditor/ShaderEditorApp.h" )
set_property( TARGET "Projects.ShaderEditor" PROPERTY FOLDER "Projects" )
target_include_directories( "Projects.ShaderEditor" PRIVATE "Projects/ShaderEditor" )
target_include_directories( "Projects.ShaderEditor" PUBLIC "Engine/.." )
target_link_libraries( "Projects.ShaderEditor" "Engine.Scene" )
# compiler
if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
	# DebugAnalyze
	target_compile_options ( "Projects.ShaderEditor" PRIVATE $<$<CONFIG:DebugAnalyze>: /std:c++latest /MP /Gm- /Zc:inline /Gy- /fp:strict /fp:except- /we4002 /we4099 /we4129 /we4172 /we4201 /we4238 /we4239 /we4251 /we4263 /we4264 /we4266 /we4273 /we4473 /we4474 /we4480 /we4522 /we4552 /we4553 /we4700 /we4702 /we4715 /we4716 /we4717  /w14018 /w14100 /w14127 /w14189 /w14244 /w14245 /w14287 /w14389 /w14456 /w14457 /w14458 /w14459 /w14505 /w14668 /w14838 /w14946 /w14996 /w15038  /wd4061 /wd4062 /wd4063 /wd4310 /wd4365 /wd4503 /wd4514 /wd4623 /wd4625 /wd4626 /wd4710 /wd4714 /wd5026 /wd5027  /W4 /WX- /sdl /Od /Ob0 /EHsc /Oy- /GF- /GS /guard:cf /GR /MDd /analyze /Zi /RTCsu >)
	target_compile_definitions( "Projects.ShaderEditor" PRIVATE $<$<CONFIG:DebugAnalyze>: __GX_DEBUG__ __GX_ANALYZE__ >)
	set_target_properties( "Projects.ShaderEditor" PROPERTIES LINK_FLAGS_DEBUGANALYZE " /OPT:REF /OPT:ICF /INCREMENTAL:NO /DEBUG:FULL /LTCG" )
	# Debug
	target_compile_options ( "Projects.ShaderEditor" PRIVATE $<$<CONFIG:Debug>: /std:c++latest /MP /Gm- /Zc:inline /Gy- /fp:strict /fp:except- /we4002 /we4099 /we4129 /we4172 /we4201 /we4238 /we4239 /we4251 /we4263 /we4264 /we4266 /we4273 /we4473 /we4474 /we4480 /we4522 /we4552 /we4553 /we4700 /we4702 /we4715 /we4716 /we4717  /w14018 /w14100 /w14127 /w14189 /w14244 /w14245 /w14287 /w14389 /w14456 /w14457 /w14458 /w14459 /w14505 /w14668 /w14838 /w14946 /w14996 /w15038  /wd4061 /wd4062 /wd4063 /wd4310 /wd4365 /wd4503 /wd4514 /wd4623 /wd4625 /wd4626 /wd4710 /wd4714 /wd5026 /wd5027  /W4 /WX- /sdl /Od /Ob0 /EHsc /Oy- /GF- /GS /GR /MDd /analyze- /Zi /RTCsu >)
	target_compile_definitions( "Projects.ShaderEditor" PRIVATE $<$<CONFIG:Debug>: __GX_DEBUG__ >)
	set_target_properties( "Projects.ShaderEditor" PROPERTIES LINK_FLAGS_DEBUG " /OPT:REF /OPT:ICF /INCREMENTAL:NO /DEBUG:FULL /LTCG" )
	# Profile
	target_compile_options ( "Projects.ShaderEditor" PRIVATE $<$<CONFIG:Profile>: /std:c++latest /MP /Gm- /Zc:inline /Gy- /fp:strict /fp:except- /we4002 /we4099 /we4129 /we4172 /we4201 /we4238 /we4239 /we4251 /we4263 /we4264 /we4266 /we4273 /we4473 /we4474 /we4480 /we4522 /we4552 /we4553 /we4700 /we4702 /we4715 /we4716 /we4717  /w14018 /w14100 /w14127 /w14189 /w14244 /w14245 /w14287 /w14389 /w14456 /w14457 /w14458 /w14459 /w14505 /w14668 /w14838 /w14946 /w14996 /w15038  /wd4061 /wd4062 /wd4063 /wd4310 /wd4365 /wd4503 /wd4514 /wd4623 /wd4625 /wd4626 /wd4710 /wd4714 /wd5026 /wd5027  /Ob2 /Ob2 /Oi /Ot /Oy /MD /GT /GL /GF /GS- /W3 /Od /analyze- /EHsc /GR >)
	target_compile_definitions( "Projects.ShaderEditor" PRIVATE $<$<CONFIG:Profile>: GX_ENABLE_PROFILING >)
	set_target_properties( "Projects.ShaderEditor" PROPERTIES LINK_FLAGS_PROFILE " /OPT:REF /OPT:ICF /INCREMENTAL:NO /LTCG /DEBUG /PROFILE" )
	# Release
	target_compile_options ( "Projects.ShaderEditor" PRIVATE $<$<CONFIG:Release>: /std:c++latest /MP /Gm- /Zc:inline /Gy- /fp:strict /fp:except- /we4002 /we4099 /we4129 /we4172 /we4201 /we4238 /we4239 /we4251 /we4263 /we4264 /we4266 /we4273 /we4473 /we4474 /we4480 /we4522 /we4552 /we4553 /we4700 /we4702 /we4715 /we4716 /we4717  /w14018  /w14127 /w14189 /w14244 /w14245 /w14287 /w14389 /w14456 /w14457 /w14458 /w14459 /w14505 /w14668 /w14838 /w14946 /w14996 /w15038  /wd4061 /wd4062 /wd4063 /wd4310 /wd4365 /wd4503 /wd4514 /wd4623 /wd4625 /wd4626 /wd4710 /wd4714 /wd5026 /wd5027  /Ob2 /Ob2 /Oi /Ot /Oy /MD /GT /GL /GF /GS- /W3 /Ox /analyze- /EHs-c- /GR- >)
	target_compile_definitions( "Projects.ShaderEditor" PRIVATE $<$<CONFIG:Release>: __GX_FAST__ __GX_NO_EXCEPTIONS__ >)
	set_target_properties( "Projects.ShaderEditor" PROPERTIES LINK_FLAGS_RELEASE " /OPT:REF /OPT:ICF /INCREMENTAL:NO /LTCG /RELEASE /DYNAMICBASE" )
endif()


if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
	# DebugAnalyze
	target_compile_options ( "Projects.ShaderEditor" PRIVATE $<$<CONFIG:DebugAnalyze>: -std=c++1z -Wchar-subscripts -Wdouble-promotion -Wformat -Wmain -Wmissing-braces -Wmissing-include-dirs -Wunused -Wuninitialized -Wmaybe-uninitialized -Wunknown-pragmas -Wpragmas -Wstrict-aliasing -Wstrict-overflow -Wundef -Wendif-labels -Wfree-nonheap-object -Wpointer-arith -Wcast-qual -Wcast-align -Wwrite-strings -Wconversion-null -Wzero-as-null-pointer-constant -Wenum-compare -Wsign-compare -Wsizeof-pointer-memaccess -Wlogical-op -Werror=init-self -Werror=parentheses -Werror=return-local-addr -Werror=return-type -Werror=non-template-friend -Werror=array-bounds -Werror=div-by-zero -Werror=address -Werror=missing-field-initializers -Werror=placement-new  -Wno-non-template-friend  -ggdb -fsanitize=undefined -fchkp-check-incomplete-type -Og >)
	target_compile_definitions( "Projects.ShaderEditor" PRIVATE $<$<CONFIG:DebugAnalyze>: __GX_DEBUG__ __GX_ANALYZE__ >)
	# Debug
	target_compile_options ( "Projects.ShaderEditor" PRIVATE $<$<CONFIG:Debug>: -std=c++1z -Wchar-subscripts -Wdouble-promotion -Wformat -Wmain -Wmissing-braces -Wmissing-include-dirs -Wunused -Wuninitialized -Wmaybe-uninitialized -Wunknown-pragmas -Wpragmas -Wstrict-aliasing -Wstrict-overflow -Wundef -Wendif-labels -Wfree-nonheap-object -Wpointer-arith -Wcast-qual -Wcast-align -Wwrite-strings -Wconversion-null -Wzero-as-null-pointer-constant -Wenum-compare -Wsign-compare -Wsizeof-pointer-memaccess -Wlogical-op -Werror=init-self -Werror=parentheses -Werror=return-local-addr -Werror=return-type -Werror=non-template-friend -Werror=array-bounds -Werror=div-by-zero -Werror=address -Werror=missing-field-initializers -Werror=placement-new  -Wno-non-template-friend  -ggdb -fchkp-check-incomplete-type -Og >)
	target_compile_definitions( "Projects.ShaderEditor" PRIVATE $<$<CONFIG:Debug>: __GX_DEBUG__ >)
	# Profile
	target_compile_options ( "Projects.ShaderEditor" PRIVATE $<$<CONFIG:Profile>: -std=c++1z -Wchar-subscripts -Wdouble-promotion -Wformat -Wmain -Wmissing-braces -Wmissing-include-dirs -Wunused -Wuninitialized -Wmaybe-uninitialized -Wunknown-pragmas -Wpragmas -Wstrict-aliasing -Wstrict-overflow -Wundef -Wendif-labels -Wfree-nonheap-object -Wpointer-arith -Wcast-qual -Wcast-align -Wwrite-strings -Wconversion-null -Wzero-as-null-pointer-constant -Wenum-compare -Wsign-compare -Wsizeof-pointer-memaccess -Wlogical-op -Werror=init-self -Werror=parentheses -Werror=return-local-addr -Werror=return-type -Werror=non-template-friend -Werror=array-bounds -Werror=div-by-zero -Werror=address -Werror=missing-field-initializers -Werror=placement-new  -Wno-non-template-friend  -O2 -finline-functions >)
	target_compile_definitions( "Projects.ShaderEditor" PRIVATE $<$<CONFIG:Profile>: GX_ENABLE_PROFILING >)
	# Release
	target_compile_options ( "Projects.ShaderEditor" PRIVATE $<$<CONFIG:Release>: -std=c++1z -Wchar-subscripts -Wdouble-promotion -Wformat -Wmain -Wmissing-braces -Wmissing-include-dirs -Wunused -Wuninitialized -Wmaybe-uninitialized -Wunknown-pragmas -Wpragmas -Wstrict-aliasing -Wstrict-overflow -Wundef -Wendif-labels -Wfree-nonheap-object -Wpointer-arith -Wcast-qual -Wcast-align -Wwrite-strings -Wconversion-null -Wzero-as-null-pointer-constant -Wenum-compare -Wsign-compare -Wsizeof-pointer-memaccess -Wlogical-op -Werror=init-self -Werror=parentheses -Werror=return-local-addr -Werror=return-type -Werror=non-template-friend -Werror=array-bounds -Werror=div-by-zero -Werror=address -Werror=missing-field-initializers -Werror=placement-new  -Wno-non-template-friend  -O3 -Ofast -fomit-frame-pointer -finline-functions >)
	target_compile_definitions( "Projects.ShaderEditor" PRIVATE $<$<CONFIG:Release>: __GX_FAST__ __GX_NO_EXCEPTIONS__ >)
endif()


if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
	target_compile_definitions( "Projects.ShaderEditor" PRIVATE $<$<CONFIG:DebugAnalyze>: __GX_DEBUG__ __GX_ANALYZE__ >)
	target_compile_definitions( "Projects.ShaderEditor" PRIVATE $<$<CONFIG:Debug>: __GX_DEBUG__ >)
	target_compile_definitions( "Projects.ShaderEditor" PRIVATE $<$<CONFIG:Profile>: GX_ENABLE_PROFILING >)
	target_compile_definitions( "Projects.ShaderEditor" PRIVATE $<$<CONFIG:Release>: __GX_FAST__ __GX_NO_EXCEPTIONS__ >)
endif()





#==================================================================================================
# project: Projects.ShaderEditor.Tools
#==================================================================================================
add_executable( "Projects.ShaderEditor.Tools" 
		"Projects/ShaderEditorTools/DrawTexturedQuad.cpp"
		"Projects/ShaderEditorTools/Experimental.cpp"
		"Projects/ShaderEditorTools/Main.cpp"
		"Projects/ShaderEditorTools/ShadertoyTemplate.h"
		"Projects/ShaderEditorTools/VoronnoiRecursion.cpp" )

source_group( "" FILES "Projects/ShaderEditorTools/DrawTexturedQuad.cpp" "Projects/ShaderEditorTools/Experimental.cpp" "Projects/ShaderEditorTools/Main.cpp" "Projects/ShaderEditorTools/ShadertoyTemplate.h" "Projects/ShaderEditorTools/VoronnoiRecursion.cpp" )
set_property( TARGET "Projects.ShaderEditor.Tools" PROPERTY FOLDER "Projects" )
target_include_directories( "Projects.ShaderEditor.Tools" PRIVATE "Projects/ShaderEditorTools" )
target_include_directories( "Projects.ShaderEditor.Tools" PUBLIC "Engine/.." )
if (WIN32)
	target_link_libraries( "Projects.ShaderEditor.Tools" "Engine.PipelineCompiler" )
endif()
# compiler
if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
	# DebugAnalyze
	target_compile_options ( "Projects.ShaderEditor.Tools" PRIVATE $<$<CONFIG:DebugAnalyze>: /std:c++latest /MP /Gm- /Zc:inline /Gy- /fp:strict /fp:except- /we4002 /we4099 /we4129 /we4172 /we4201 /we4238 /we4239 /we4251 /we4263 /we4264 /we4266 /we4273 /we4473 /we4474 /we4480 /we4522 /we4552 /we4553 /we4700 /we4702 /we4715 /we4716 /we4717  /w14018 /w14100 /w14127 /w14189 /w14244 /w14245 /w14287 /w14389 /w14456 /w14457 /w14458 /w14459 /w14505 /w14668 /w14838 /w14946 /w14996 /w15038  /wd4061 /wd4062 /wd4063 /wd4310 /wd4365 /wd4503 /wd4514 /wd4623 /wd4625 /wd4626 /wd4710 /wd4714 /wd5026 /wd5027  /W4 /WX- /sdl /Od /Ob0 /EHsc /Oy- /GF- /GS /guard:cf /GR /MDd /analyze /Zi /RTCsu >)
	target_compile_definitions( "Projects.ShaderEditor.Tools" PRIVATE $<$<CONFIG:DebugAnalyze>: __GX_DEBUG__ __GX_ANALYZE__ >)
	set_target_properties( "Projects.ShaderEditor.Tools" PROPERTIES LINK_FLAGS_DEBUGANALYZE " /OPT:REF /OPT:ICF /INCREMENTAL:NO /DEBUG:FULL /LTCG" )
	# Debug
	target_compile_options ( "Projects.ShaderEditor.Tools" PRIVATE $<$<CONFIG:Debug>: /std:c++latest /MP /Gm- /Zc:inline /Gy- /fp:strict /fp:except- /we4002 /we4099 /we4129 /we4172 /we4201 /we4238 /we4239 /we4251 /we4263 /we4264 /we4266 /we4273 /we4473 /we4474 /we4480 /we4522 /we4552 /we4553 /we4700 /we4702 /we4715 /we4716 /we4717  /w14018 /w14100 /w14127 /w14189 /w14244 /w14245 /w14287 /w14389 /w14456 /w14457 /w14458 /w14459 /w14505 /w14668 /w14838 /w14946 /w14996 /w15038  /wd4061 /wd4062 /wd4063 /wd4310 /wd4365 /wd4503 /wd4514 /wd4623 /wd4625 /wd4626 /wd4710 /wd4714 /wd5026 /wd5027  /W4 /WX- /sdl /Od /Ob0 /EHsc /Oy- /GF- /GS /GR /MDd /analyze- /Zi /RTCsu >)
	target_compile_definitions( "Projects.ShaderEditor.Tools" PRIVATE $<$<CONFIG:Debug>: __GX_DEBUG__ >)
	set_target_properties( "Projects.ShaderEditor.Tools" PROPERTIES LINK_FLAGS_DEBUG " /OPT:REF /OPT:ICF /INCREMENTAL:NO /DEBUG:FULL /LTCG" )
	# Profile
	target_compile_options ( "Projects.ShaderEditor.Tools" PRIVATE $<$<CONFIG:Profile>: /std:c++latest /MP /Gm- /Zc:inline /Gy- /fp:strict /fp:except- /we4002 /we4099 /we4129 /we4172 /we4201 /we4238 /we4239 /we4251 /we4263 /we4264 /we4266 /we4273 /we4473 /we4474 /we4480 /we4522 /we4552 /we4553 /we4700 /we4702 /we4715 /we4716 /we4717  /w14018 /w14100 /w14127 /w14189 /w14244 /w14245 /w14287 /w14389 /w14456 /w14457 /w14458 /w14459 /w14505 /w14668 /w14838 /w14946 /w14996 /w15038  /wd4061 /wd4062 /wd4063 /wd4310 /wd4365 /wd4503 /wd4514 /wd4623 /wd4625 /wd4626 /wd4710 /wd4714 /wd5026 /wd5027  /Ob2 /Ob2 /Oi /Ot /Oy /MD /GT /GL /GF /GS- /W3 /Od /analyze- /EHsc /GR >)
	target_compile_definitions( "Projects.ShaderEditor.Tools" PRIVATE $<$<CONFIG:Profile>: GX_ENABLE_PROFILING >)
	set_target_properties( "Projects.ShaderEditor.Tools" PROPERTIES LINK_FLAGS_PROFILE " /OPT:REF /OPT:ICF /INCREMENTAL:NO /LTCG /DEBUG /PROFILE" )
	# Release
	target_compile_options ( "Projects.ShaderEditor.Tools" PRIVATE $<$<CONFIG:Release>: /std:c++latest /MP /Gm- /Zc:inline /Gy- /fp:strict /fp:except- /we4002 /we4099 /we4129 /we4172 /we4201 /we4238 /we4239 /we4251 /we4263 /we4264 /we4266 /we4273 /we4473 /we4474 /we4480 /we4522 /we4552 /we4553 /we4700 /we4702 /we4715 /we4716 /we4717  /w14018  /w14127 /w14189 /w14244 /w14245 /w14287 /w14389 /w14456 /w14457 /w14458 /w14459 /w14505 /w14668 /w14838 /w14946 /w14996 /w15038  /wd4061 /wd4062 /wd4063 /wd4310 /wd4365 /wd4503 /wd4514 /wd4623 /wd4625 /wd4626 /wd4710 /wd4714 /wd5026 /wd5027  /Ob2 /Ob2 /Oi /Ot /Oy /MD /GT /GL /GF /GS- /W3 /Ox /analyze- /EHs-c- /GR- >)
	target_compile_definitions( "Projects.ShaderEditor.Tools" PRIVATE $<$<CONFIG:Release>: __GX_FAST__ __GX_NO_EXCEPTIONS__ >)
	set_target_properties( "Projects.ShaderEditor.Tools" PROPERTIES LINK_FLAGS_RELEASE " /OPT:REF /OPT:ICF /INCREMENTAL:NO /LTCG /RELEASE /DYNAMICBASE" )
endif()


if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
	# DebugAnalyze
	target_compile_options ( "Projects.ShaderEditor.Tools" PRIVATE $<$<CONFIG:DebugAnalyze>: -std=c++1z -Wchar-subscripts -Wdouble-promotion -Wformat -Wmain -Wmissing-braces -Wmissing-include-dirs -Wunused -Wuninitialized -Wmaybe-uninitialized -Wunknown-pragmas -Wpragmas -Wstrict-aliasing -Wstrict-overflow -Wundef -Wendif-labels -Wfree-nonheap-object -Wpointer-arith -Wcast-qual -Wcast-align -Wwrite-strings -Wconversion-null -Wzero-as-null-pointer-constant -Wenum-compare -Wsign-compare -Wsizeof-pointer-memaccess -Wlogical-op -Werror=init-self -Werror=parentheses -Werror=return-local-addr -Werror=return-type -Werror=non-template-friend -Werror=array-bounds -Werror=div-by-zero -Werror=address -Werror=missing-field-initializers -Werror=placement-new  -Wno-non-template-friend  -ggdb -fsanitize=undefined -fchkp-check-incomplete-type -Og >)
	target_compile_definitions( "Projects.ShaderEditor.Tools" PRIVATE $<$<CONFIG:DebugAnalyze>: __GX_DEBUG__ __GX_ANALYZE__ >)
	# Debug
	target_compile_options ( "Projects.ShaderEditor.Tools" PRIVATE $<$<CONFIG:Debug>: -std=c++1z -Wchar-subscripts -Wdouble-promotion -Wformat -Wmain -Wmissing-braces -Wmissing-include-dirs -Wunused -Wuninitialized -Wmaybe-uninitialized -Wunknown-pragmas -Wpragmas -Wstrict-aliasing -Wstrict-overflow -Wundef -Wendif-labels -Wfree-nonheap-object -Wpointer-arith -Wcast-qual -Wcast-align -Wwrite-strings -Wconversion-null -Wzero-as-null-pointer-constant -Wenum-compare -Wsign-compare -Wsizeof-pointer-memaccess -Wlogical-op -Werror=init-self -Werror=parentheses -Werror=return-local-addr -Werror=return-type -Werror=non-template-friend -Werror=array-bounds -Werror=div-by-zero -Werror=address -Werror=missing-field-initializers -Werror=placement-new  -Wno-non-template-friend  -ggdb -fchkp-check-incomplete-type -Og >)
	target_compile_definitions( "Projects.ShaderEditor.Tools" PRIVATE $<$<CONFIG:Debug>: __GX_DEBUG__ >)
	# Profile
	target_compile_options ( "Projects.ShaderEditor.Tools" PRIVATE $<$<CONFIG:Profile>: -std=c++1z -Wchar-subscripts -Wdouble-promotion -Wformat -Wmain -Wmissing-braces -Wmissing-include-dirs -Wunused -Wuninitialized -Wmaybe-uninitialized -Wunknown-pragmas -Wpragmas -Wstrict-aliasing -Wstrict-overflow -Wundef -Wendif-labels -Wfree-nonheap-object -Wpointer-arith -Wcast-qual -Wcast-align -Wwrite-strings -Wconversion-null -Wzero-as-null-pointer-constant -Wenum-compare -Wsign-compare -Wsizeof-pointer-memaccess -Wlogical-op -Werror=init-self -Werror=parentheses -Werror=return-local-addr -Werror=return-type -Werror=non-template-friend -Werror=array-bounds -Werror=div-by-zero -Werror=address -Werror=missing-field-initializers -Werror=placement-new  -Wno-non-template-friend  -O2 -finline-functions >)
	target_compile_definitions( "Projects.ShaderEditor.Tools" PRIVATE $<$<CONFIG:Profile>: GX_ENABLE_PROFILING >)
	# Release
	target_compile_options ( "Projects.ShaderEditor.Tools" PRIVATE $<$<CONFIG:Release>: -std=c++1z -Wchar-subscripts -Wdouble-promotion -Wformat -Wmain -Wmissing-braces -Wmissing-include-dirs -Wunused -Wuninitialized -Wmaybe-uninitialized -Wunknown-pragmas -Wpragmas -Wstrict-aliasing -Wstrict-overflow -Wundef -Wendif-labels -Wfree-nonheap-object -Wpointer-arith -Wcast-qual -Wcast-align -Wwrite-strings -Wconversion-null -Wzero-as-null-pointer-constant -Wenum-compare -Wsign-compare -Wsizeof-pointer-memaccess -Wlogical-op -Werror=init-self -Werror=parentheses -Werror=return-local-addr -Werror=return-type -Werror=non-template-friend -Werror=array-bounds -Werror=div-by-zero -Werror=address -Werror=missing-field-initializers -Werror=placement-new  -Wno-non-template-friend  -O3 -Ofast -fomit-frame-pointer -finline-functions >)
	target_compile_definitions( "Projects.ShaderEditor.Tools" PRIVATE $<$<CONFIG:Release>: __GX_FAST__ __GX_NO_EXCEPTIONS__ >)
endif()


if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
	target_compile_definitions( "Projects.ShaderEditor.Tools" PRIVATE $<$<CONFIG:DebugAnalyze>: __GX_DEBUG__ __GX_ANALYZE__ >)
	target_compile_definitions( "Projects.ShaderEditor.Tools" PRIVATE $<$<CONFIG:Debug>: __GX_DEBUG__ >)
	target_compile_definitions( "Projects.ShaderEditor.Tools" PRIVATE $<$<CONFIG:Profile>: GX_ENABLE_PROFILING >)
	target_compile_definitions( "Projects.ShaderEditor.Tools" PRIVATE $<$<CONFIG:Release>: __GX_FAST__ __GX_NO_EXCEPTIONS__ >)
endif()





#==================================================================================================
# project: Utils.CMake
#==================================================================================================
add_executable( "Utils.CMake" 
		"Utils/CMake/Tests/GenMGFProject.cpp"
		"Utils/CMake/Tests/Main.cpp"
		"Utils/CMake/Builder/ClangConstants.h"
		"Utils/CMake/Builder/CMakeBuilder.cpp"
		"Utils/CMake/Builder/CMakeBuilder.h"
		"Utils/CMake/Builder/GccConstants.h"
		"Utils/CMake/Builder/MSVisualStudioConstants.h" )

source_group( "Tests" FILES "Utils/CMake/Tests/GenMGFProject.cpp" "Utils/CMake/Tests/Main.cpp" )
source_group( "Builder" FILES "Utils/CMake/Builder/ClangConstants.h" "Utils/CMake/Builder/CMakeBuilder.cpp" "Utils/CMake/Builder/CMakeBuilder.h" "Utils/CMake/Builder/GccConstants.h" "Utils/CMake/Builder/MSVisualStudioConstants.h" )
set_property( TARGET "Utils.CMake" PROPERTY FOLDER "Utils" )
target_include_directories( "Utils.CMake" PRIVATE "Utils/CMake" )
target_include_directories( "Utils.CMake" PUBLIC "Engine/.." )
target_link_libraries( "Utils.CMake" "Engine.STL" )
# compiler
if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
	# DebugAnalyze
	target_compile_options ( "Utils.CMake" PRIVATE $<$<CONFIG:DebugAnalyze>: /std:c++latest /MP /Gm- /Zc:inline /Gy- /fp:strict /fp:except- /we4002 /we4099 /we4129 /we4172 /we4201 /we4238 /we4239 /we4251 /we4263 /we4264 /we4266 /we4273 /we4473 /we4474 /we4480 /we4522 /we4552 /we4553 /we4700 /we4702 /we4715 /we4716 /we4717  /w14018 /w14100 /w14127 /w14189 /w14244 /w14245 /w14287 /w14389 /w14456 /w14457 /w14458 /w14459 /w14505 /w14668 /w14838 /w14946 /w14996 /w15038  /wd4061 /wd4062 /wd4063 /wd4310 /wd4365 /wd4503 /wd4514 /wd4623 /wd4625 /wd4626 /wd4710 /wd4714 /wd5026 /wd5027  /W4 /WX- /sdl /Od /Ob0 /EHsc /Oy- /GF- /GS /guard:cf /GR /MDd /analyze /Zi /RTCsu >)
	target_compile_definitions( "Utils.CMake" PRIVATE $<$<CONFIG:DebugAnalyze>: __GX_DEBUG__ __GX_ANALYZE__ >)
	set_target_properties( "Utils.CMake" PROPERTIES LINK_FLAGS_DEBUGANALYZE " /OPT:REF /OPT:ICF /INCREMENTAL:NO /DEBUG:FULL /LTCG" )
	# Debug
	target_compile_options ( "Utils.CMake" PRIVATE $<$<CONFIG:Debug>: /std:c++latest /MP /Gm- /Zc:inline /Gy- /fp:strict /fp:except- /we4002 /we4099 /we4129 /we4172 /we4201 /we4238 /we4239 /we4251 /we4263 /we4264 /we4266 /we4273 /we4473 /we4474 /we4480 /we4522 /we4552 /we4553 /we4700 /we4702 /we4715 /we4716 /we4717  /w14018 /w14100 /w14127 /w14189 /w14244 /w14245 /w14287 /w14389 /w14456 /w14457 /w14458 /w14459 /w14505 /w14668 /w14838 /w14946 /w14996 /w15038  /wd4061 /wd4062 /wd4063 /wd4310 /wd4365 /wd4503 /wd4514 /wd4623 /wd4625 /wd4626 /wd4710 /wd4714 /wd5026 /wd5027  /W4 /WX- /sdl /Od /Ob0 /EHsc /Oy- /GF- /GS /GR /MDd /analyze- /Zi /RTCsu >)
	target_compile_definitions( "Utils.CMake" PRIVATE $<$<CONFIG:Debug>: __GX_DEBUG__ >)
	set_target_properties( "Utils.CMake" PROPERTIES LINK_FLAGS_DEBUG " /OPT:REF /OPT:ICF /INCREMENTAL:NO /DEBUG:FULL /LTCG" )
	# Profile
	target_compile_options ( "Utils.CMake" PRIVATE $<$<CONFIG:Profile>: /std:c++latest /MP /Gm- /Zc:inline /Gy- /fp:strict /fp:except- /we4002 /we4099 /we4129 /we4172 /we4201 /we4238 /we4239 /we4251 /we4263 /we4264 /we4266 /we4273 /we4473 /we4474 /we4480 /we4522 /we4552 /we4553 /we4700 /we4702 /we4715 /we4716 /we4717  /w14018 /w14100 /w14127 /w14189 /w14244 /w14245 /w14287 /w14389 /w14456 /w14457 /w14458 /w14459 /w14505 /w14668 /w14838 /w14946 /w14996 /w15038  /wd4061 /wd4062 /wd4063 /wd4310 /wd4365 /wd4503 /wd4514 /wd4623 /wd4625 /wd4626 /wd4710 /wd4714 /wd5026 /wd5027  /Ob2 /Ob2 /Oi /Ot /Oy /MD /GT /GL /GF /GS- /W3 /Od /analyze- /EHsc /GR >)
	target_compile_definitions( "Utils.CMake" PRIVATE $<$<CONFIG:Profile>: GX_ENABLE_PROFILING >)
	set_target_properties( "Utils.CMake" PROPERTIES LINK_FLAGS_PROFILE " /OPT:REF /OPT:ICF /INCREMENTAL:NO /LTCG /DEBUG /PROFILE" )
	# Release
	target_compile_options ( "Utils.CMake" PRIVATE $<$<CONFIG:Release>: /std:c++latest /MP /Gm- /Zc:inline /Gy- /fp:strict /fp:except- /we4002 /we4099 /we4129 /we4172 /we4201 /we4238 /we4239 /we4251 /we4263 /we4264 /we4266 /we4273 /we4473 /we4474 /we4480 /we4522 /we4552 /we4553 /we4700 /we4702 /we4715 /we4716 /we4717  /w14018  /w14127 /w14189 /w14244 /w14245 /w14287 /w14389 /w14456 /w14457 /w14458 /w14459 /w14505 /w14668 /w14838 /w14946 /w14996 /w15038  /wd4061 /wd4062 /wd4063 /wd4310 /wd4365 /wd4503 /wd4514 /wd4623 /wd4625 /wd4626 /wd4710 /wd4714 /wd5026 /wd5027  /Ob2 /Ob2 /Oi /Ot /Oy /MD /GT /GL /GF /GS- /W3 /Ox /analyze- /EHs-c- /GR- >)
	target_compile_definitions( "Utils.CMake" PRIVATE $<$<CONFIG:Release>: __GX_FAST__ __GX_NO_EXCEPTIONS__ >)
	set_target_properties( "Utils.CMake" PROPERTIES LINK_FLAGS_RELEASE " /OPT:REF /OPT:ICF /INCREMENTAL:NO /LTCG /RELEASE /DYNAMICBASE" )
endif()


if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
	# DebugAnalyze
	target_compile_options ( "Utils.CMake" PRIVATE $<$<CONFIG:DebugAnalyze>: -std=c++1z -Wchar-subscripts -Wdouble-promotion -Wformat -Wmain -Wmissing-braces -Wmissing-include-dirs -Wunused -Wuninitialized -Wmaybe-uninitialized -Wunknown-pragmas -Wpragmas -Wstrict-aliasing -Wstrict-overflow -Wundef -Wendif-labels -Wfree-nonheap-object -Wpointer-arith -Wcast-qual -Wcast-align -Wwrite-strings -Wconversion-null -Wzero-as-null-pointer-constant -Wenum-compare -Wsign-compare -Wsizeof-pointer-memaccess -Wlogical-op -Werror=init-self -Werror=parentheses -Werror=return-local-addr -Werror=return-type -Werror=non-template-friend -Werror=array-bounds -Werror=div-by-zero -Werror=address -Werror=missing-field-initializers -Werror=placement-new  -Wno-non-template-friend  -ggdb -fsanitize=undefined -fchkp-check-incomplete-type -Og >)
	target_compile_definitions( "Utils.CMake" PRIVATE $<$<CONFIG:DebugAnalyze>: __GX_DEBUG__ __GX_ANALYZE__ >)
	# Debug
	target_compile_options ( "Utils.CMake" PRIVATE $<$<CONFIG:Debug>: -std=c++1z -Wchar-subscripts -Wdouble-promotion -Wformat -Wmain -Wmissing-braces -Wmissing-include-dirs -Wunused -Wuninitialized -Wmaybe-uninitialized -Wunknown-pragmas -Wpragmas -Wstrict-aliasing -Wstrict-overflow -Wundef -Wendif-labels -Wfree-nonheap-object -Wpointer-arith -Wcast-qual -Wcast-align -Wwrite-strings -Wconversion-null -Wzero-as-null-pointer-constant -Wenum-compare -Wsign-compare -Wsizeof-pointer-memaccess -Wlogical-op -Werror=init-self -Werror=parentheses -Werror=return-local-addr -Werror=return-type -Werror=non-template-friend -Werror=array-bounds -Werror=div-by-zero -Werror=address -Werror=missing-field-initializers -Werror=placement-new  -Wno-non-template-friend  -ggdb -fchkp-check-incomplete-type -Og >)
	target_compile_definitions( "Utils.CMake" PRIVATE $<$<CONFIG:Debug>: __GX_DEBUG__ >)
	# Profile
	target_compile_options ( "Utils.CMake" PRIVATE $<$<CONFIG:Profile>: -std=c++1z -Wchar-subscripts -Wdouble-promotion -Wformat -Wmain -Wmissing-braces -Wmissing-include-dirs -Wunused -Wuninitialized -Wmaybe-uninitialized -Wunknown-pragmas -Wpragmas -Wstrict-aliasing -Wstrict-overflow -Wundef -Wendif-labels -Wfree-nonheap-object -Wpointer-arith -Wcast-qual -Wcast-align -Wwrite-strings -Wconversion-null -Wzero-as-null-pointer-constant -Wenum-compare -Wsign-compare -Wsizeof-pointer-memaccess -Wlogical-op -Werror=init-self -Werror=parentheses -Werror=return-local-addr -Werror=return-type -Werror=non-template-friend -Werror=array-bounds -Werror=div-by-zero -Werror=address -Werror=missing-field-initializers -Werror=placement-new  -Wno-non-template-friend  -O2 -finline-functions >)
	target_compile_definitions( "Utils.CMake" PRIVATE $<$<CONFIG:Profile>: GX_ENABLE_PROFILING >)
	# Release
	target_compile_options ( "Utils.CMake" PRIVATE $<$<CONFIG:Release>: -std=c++1z -Wchar-subscripts -Wdouble-promotion -Wformat -Wmain -Wmissing-braces -Wmissing-include-dirs -Wunused -Wuninitialized -Wmaybe-uninitialized -Wunknown-pragmas -Wpragmas -Wstrict-aliasing -Wstrict-overflow -Wundef -Wendif-labels -Wfree-nonheap-object -Wpointer-arith -Wcast-qual -Wcast-align -Wwrite-strings -Wconversion-null -Wzero-as-null-pointer-constant -Wenum-compare -Wsign-compare -Wsizeof-pointer-memaccess -Wlogical-op -Werror=init-self -Werror=parentheses -Werror=return-local-addr -Werror=return-type -Werror=non-template-friend -Werror=array-bounds -Werror=div-by-zero -Werror=address -Werror=missing-field-initializers -Werror=placement-new  -Wno-non-template-friend  -O3 -Ofast -fomit-frame-pointer -finline-functions >)
	target_compile_definitions( "Utils.CMake" PRIVATE $<$<CONFIG:Release>: __GX_FAST__ __GX_NO_EXCEPTIONS__ >)
endif()


if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
	target_compile_definitions( "Utils.CMake" PRIVATE $<$<CONFIG:DebugAnalyze>: __GX_DEBUG__ __GX_ANALYZE__ >)
	target_compile_definitions( "Utils.CMake" PRIVATE $<$<CONFIG:Debug>: __GX_DEBUG__ >)
	target_compile_definitions( "Utils.CMake" PRIVATE $<$<CONFIG:Profile>: GX_ENABLE_PROFILING >)
	target_compile_definitions( "Utils.CMake" PRIVATE $<$<CONFIG:Release>: __GX_FAST__ __GX_NO_EXCEPTIONS__ >)
endif()





