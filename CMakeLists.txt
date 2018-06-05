# auto generated file
cmake_minimum_required (VERSION 3.6.0)

message( STATUS "==========================================================================\n" )
message( STATUS "project 'ModularGraphicsFramework' generation started" )
if ( WIN32 )
	set( CMAKE_SYSTEM_VERSION "8.1" CACHE TYPE INTERNAL FORCE )
endif()

project( "ModularGraphicsFramework" LANGUAGES CXX )
set_property( GLOBAL PROPERTY USE_FOLDERS ON )
set( ModularGraphicsFramework_VERSION_MAJOR 0 )
set( ModularGraphicsFramework_VERSION_MINOR 300 )

set( OPENGLES_VERSION "320" CACHE STRING "Define OpenGLES version, supported: 200, 300, 310, 320"  )
set( VULKAN_VERSION "100" CACHE STRING "Define Vulkan version, supported: 100, 110"  )
set( OPENGL_VERSION "450" CACHE STRING "Define OpenGL version, supported: 440, 450"  )
set( SOFTRENDER_VERSION "100" CACHE STRING "Define Software renderer version, 100 is supported"  )
set( OPENCL_VERSION "120" CACHE STRING "Define OpenCL version, supported: 110, 120, 200"  )
#==================================================================================================
# Visual Studio Compilation settings
#==================================================================================================
set( COMPILER_MSVC OFF )
string( FIND "${CMAKE_CXX_COMPILER_ID}" "MSVC" outPos )
if ( (outPos GREATER -1) )
	set( COMPILER_MSVC ON )
endif()
if ( COMPILER_MSVC )
	set( DETECTED_COMPILER "COMPILER_MSVC" )
	string( REPLACE "/EHa" " " CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}" )
	string( REPLACE "/EHsc" " " CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}" )
	string( REPLACE "/EHs" " " CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}" )
	string( REPLACE "//EHs-c-" " " CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}" )
	message( STATUS "CMAKE_C_FLAGS: ${CMAKE_C_FLAGS}" )
	message( STATUS "CMAKE_CXX_FLAGS: ${CMAKE_CXX_FLAGS}" )
	message( STATUS "CMAKE_EXE_LINKER_FLAGS: ${CMAKE_EXE_LINKER_FLAGS}" )
	message( STATUS "CMAKE_SHARED_LINKER_FLAGS: ${CMAKE_SHARED_LINKER_FLAGS}" )
	set( CMAKE_CONFIGURATION_TYPES DebugAnalyze Debug Profile Release )
	set( CMAKE_CONFIGURATION_TYPES "${CMAKE_CONFIGURATION_TYPES}" CACHE STRING "Configurations" FORCE )
	
	# DebugAnalyze
	set_property( DIRECTORY APPEND PROPERTY COMPILE_DEFINITIONS $<$<CONFIG:DebugAnalyze>: > )
	set( CMAKE_C_FLAGS_DEBUGANALYZE "${CMAKE_C_FLAGS} /D_DEBUG /MDd /Od /MP " CACHE STRING "" FORCE )
	set( CMAKE_CXX_FLAGS_DEBUGANALYZE "${CMAKE_CXX_FLAGS} /D_DEBUG /MDd /Od /MP /Zi " CACHE STRING "" FORCE )
	set( CMAKE_EXE_LINKER_FLAGS_DEBUGANALYZE "${CMAKE_EXE_LINKER_FLAGS} /DEBUG:FULL " CACHE STRING "" FORCE )
	set( CMAKE_SHARED_LINKER_FLAGS_DEBUGANALYZE "${CMAKE_SHARED_LINKER_FLAGS} /DEBUG:FULL " CACHE STRING "" FORCE )
	set( PROJECTS_SHARED_CXX_FLAGS_DEBUGANALYZE  /std:c++latest /MP /Gm- /Zc:inline /Gy- /fp:strict /fp:except- /we4002 /we4099 /we4129 /we4172 /we4201 /we4238 /we4239 /we4240 /we4251 /we4263 /we4264 /we4266 /we4273 /we4293 /we4390 /we4456 /we4457 /we4458 /we4459 /we4473 /we4474 /we4522 /we4552 /we4553 /we4554 /we4700 /we4706 /we4715 /we4716 /we4717 /we4800 /we4927  /w14018 /w14100 /w14127 /w14189 /w14244 /w14245 /w14287 /w14389 /w14505 /w14668 /w14701 /w14702 /w14703 /w14838 /w14946 /w14996 /w15038  /wd4061 /wd4062 /wd4063 /wd4310 /wd4365 /wd4455 /wd4503 /wd4514 /wd4623 /wd4625 /wd4626 /wd4710 /wd4714 /wd5026 /wd5027  /W4 /WX- /sdl /Od /Ob0 /EHsc /Oy- /GF- /GS /guard:cf /GR /MDd /analyze /Zi /RTCsu CACHE INTERNAL "" FORCE )
	set( PROJECTS_SHARED_DEFINES_DEBUGANALYZE  __GX_DEBUG__ __GX_ANALYZE__ CACHE INTERNAL "" FORCE )
	set( PROJECTS_SHARED_LINKER_FLAGS_DEBUGANALYZE " /OPT:REF /OPT:ICF /INCREMENTAL:NO /DEBUG:FULL" CACHE INTERNAL "" FORCE )
	# Debug
	set_property( DIRECTORY APPEND PROPERTY COMPILE_DEFINITIONS $<$<CONFIG:Debug>: > )
	set( CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS} /D_DEBUG /MDd /Od /MP " CACHE STRING "" FORCE )
	set( CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS} /D_DEBUG /MDd /Od /MP /Zi " CACHE STRING "" FORCE )
	set( CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS} /DEBUG:FULL " CACHE STRING "" FORCE )
	set( CMAKE_SHARED_LINKER_FLAGS_DEBUG "${CMAKE_SHARED_LINKER_FLAGS} /DEBUG:FULL " CACHE STRING "" FORCE )
	set( PROJECTS_SHARED_CXX_FLAGS_DEBUG  /std:c++latest /MP /Gm- /Zc:inline /Gy- /fp:strict /fp:except- /we4002 /we4099 /we4129 /we4172 /we4201 /we4238 /we4239 /we4240 /we4251 /we4263 /we4264 /we4266 /we4273 /we4293 /we4390 /we4456 /we4457 /we4458 /we4459 /we4473 /we4474 /we4522 /we4552 /we4553 /we4554 /we4700 /we4706 /we4715 /we4716 /we4717 /we4800 /we4927  /w14018 /w14100 /w14127 /w14189 /w14244 /w14245 /w14287 /w14389 /w14505 /w14668 /w14701 /w14702 /w14703 /w14838 /w14946 /w14996 /w15038  /wd4061 /wd4062 /wd4063 /wd4310 /wd4365 /wd4455 /wd4503 /wd4514 /wd4623 /wd4625 /wd4626 /wd4710 /wd4714 /wd5026 /wd5027  /W4 /WX- /sdl /Od /Ob0 /EHsc /Oy- /GF- /GS /GR /MDd /analyze- /Zi /RTCsu CACHE INTERNAL "" FORCE )
	set( PROJECTS_SHARED_DEFINES_DEBUG  __GX_DEBUG__ CACHE INTERNAL "" FORCE )
	set( PROJECTS_SHARED_LINKER_FLAGS_DEBUG " /OPT:REF /OPT:ICF /INCREMENTAL:NO /DEBUG:FULL" CACHE INTERNAL "" FORCE )
	# Profile
	set_property( DIRECTORY APPEND PROPERTY COMPILE_DEFINITIONS $<$<CONFIG:Profile>: > )
	set( CMAKE_C_FLAGS_PROFILE "${CMAKE_C_FLAGS} /D_NDEBUG /DNDEBUG  /MD /Od /MP " CACHE STRING "" FORCE )
	set( CMAKE_CXX_FLAGS_PROFILE "${CMAKE_CXX_FLAGS} /D_NDEBUG /DNDEBUG  /MD /Od /MP " CACHE STRING "" FORCE )
	set( CMAKE_EXE_LINKER_FLAGS_PROFILE "${CMAKE_EXE_LINKER_FLAGS} /DEBUG /PROFILE " CACHE STRING "" FORCE )
	set( CMAKE_SHARED_LINKER_FLAGS_PROFILE "${CMAKE_SHARED_LINKER_FLAGS} /DEBUG /PROFILE " CACHE STRING "" FORCE )
	set( PROJECTS_SHARED_CXX_FLAGS_PROFILE  /std:c++latest /MP /Gm- /Zc:inline /Gy- /fp:strict /fp:except- /we4002 /we4099 /we4129 /we4172 /we4201 /we4238 /we4239 /we4240 /we4251 /we4263 /we4264 /we4266 /we4273 /we4293 /we4390 /we4456 /we4457 /we4458 /we4459 /we4473 /we4474 /we4522 /we4552 /we4553 /we4554 /we4700 /we4706 /we4715 /we4716 /we4717 /we4800 /we4927  /w14018 /w14100 /w14127 /w14189 /w14244 /w14245 /w14287 /w14389 /w14505 /w14668 /w14701 /w14702 /w14703 /w14838 /w14946 /w14996 /w15038  /wd4061 /wd4062 /wd4063 /wd4310 /wd4365 /wd4455 /wd4503 /wd4514 /wd4623 /wd4625 /wd4626 /wd4710 /wd4714 /wd5026 /wd5027  /Ob2 /Oi /Ot /Oy /GT /GL /GF /GS- /MD /W3 /Od /analyze- /EHsc /GR CACHE INTERNAL "" FORCE )
	set( PROJECTS_SHARED_DEFINES_PROFILE  GX_ENABLE_PROFILING CACHE INTERNAL "" FORCE )
	set( PROJECTS_SHARED_LINKER_FLAGS_PROFILE " /OPT:REF /OPT:ICF /INCREMENTAL:NO /DEBUG /PROFILE" CACHE INTERNAL "" FORCE )
	# Release
	set_property( DIRECTORY APPEND PROPERTY COMPILE_DEFINITIONS $<$<CONFIG:Release>: > )
	set( CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS} /D_NDEBUG /DNDEBUG  /MD /Ox /MP " CACHE STRING "" FORCE )
	set( CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS} /D_NDEBUG /DNDEBUG  /MD /Ox /MP " CACHE STRING "" FORCE )
	set( CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS} /LTCG /RELEASE " CACHE STRING "" FORCE )
	set( CMAKE_SHARED_LINKER_FLAGS_RELEASE "${CMAKE_SHARED_LINKER_FLAGS} /LTCG /RELEASE " CACHE STRING "" FORCE )
	set( PROJECTS_SHARED_CXX_FLAGS_RELEASE  /std:c++latest /MP /Gm- /Zc:inline /Gy- /fp:strict /fp:except- /we4002 /we4099 /we4129 /we4172 /we4201 /we4238 /we4239 /we4240 /we4251 /we4263 /we4264 /we4266 /we4273 /we4293 /we4390 /we4456 /we4457 /we4458 /we4459 /we4473 /we4474 /we4522 /we4552 /we4553 /we4554 /we4700 /we4706 /we4715 /we4716 /we4717 /we4800 /we4927  /w14018  /w14127 /w14189 /w14244 /w14245 /w14287 /w14389 /w14505 /w14668 /w14701 /w14702 /w14703 /w14838 /w14946 /w14996 /w15038  /wd4061 /wd4062 /wd4063 /wd4310 /wd4365 /wd4455 /wd4503 /wd4514 /wd4623 /wd4625 /wd4626 /wd4710 /wd4714 /wd5026 /wd5027  /Ob2 /Oi /Ot /Oy /GT /GL /GF /GS- /MD /W3 /Ox /analyze- /EHs-c- CACHE INTERNAL "" FORCE )
	set( PROJECTS_SHARED_DEFINES_RELEASE  __GX_NO_EXCEPTIONS__ CACHE INTERNAL "" FORCE )
	set( PROJECTS_SHARED_LINKER_FLAGS_RELEASE " /OPT:REF /OPT:ICF /INCREMENTAL:NO /LTCG /RELEASE /DYNAMICBASE" CACHE INTERNAL "" FORCE )
	set( CMAKE_BUILD_TYPE "Debug")
	#--------------------------------------------
	set( CONFIGURATION_DEPENDENT_PATH ON CACHE INTERNAL "" FORCE )
	#--------------------------------------------
endif()


#==================================================================================================
# GCC Compilation settings
#==================================================================================================
set( COMPILER_GCC OFF )
string( FIND "${CMAKE_CXX_COMPILER_ID}" "GNU" outPos )
if ( (outPos GREATER -1) )
	set( COMPILER_GCC ON )
endif()
if ( COMPILER_GCC )
	set( DETECTED_COMPILER "COMPILER_GCC" )
	message( STATUS "CMAKE_C_FLAGS: ${CMAKE_C_FLAGS}" )
	message( STATUS "CMAKE_CXX_FLAGS: ${CMAKE_CXX_FLAGS}" )
	message( STATUS "CMAKE_EXE_LINKER_FLAGS: ${CMAKE_EXE_LINKER_FLAGS}" )
	message( STATUS "CMAKE_SHARED_LINKER_FLAGS: ${CMAKE_SHARED_LINKER_FLAGS}" )
	set( CMAKE_CONFIGURATION_TYPES DebugAnalyze Debug Profile Release )
	set( CMAKE_CONFIGURATION_TYPES "${CMAKE_CONFIGURATION_TYPES}" CACHE STRING "Configurations" FORCE )
	
	# DebugAnalyze
	set_property( DIRECTORY APPEND PROPERTY COMPILE_DEFINITIONS $<$<CONFIG:DebugAnalyze>: > )
	set( CMAKE_C_FLAGS_DEBUGANALYZE "${CMAKE_C_FLAGS} -D_DEBUG -Og " CACHE STRING "" FORCE )
	set( CMAKE_CXX_FLAGS_DEBUGANALYZE "${CMAKE_CXX_FLAGS} -D_DEBUG -Og " CACHE STRING "" FORCE )
	set( CMAKE_EXE_LINKER_FLAGS_DEBUGANALYZE "${CMAKE_EXE_LINKER_FLAGS} " CACHE STRING "" FORCE )
	set( CMAKE_SHARED_LINKER_FLAGS_DEBUGANALYZE "${CMAKE_SHARED_LINKER_FLAGS} " CACHE STRING "" FORCE )
	set( PROJECTS_SHARED_CXX_FLAGS_DEBUGANALYZE  -std=c++1z -Wchar-subscripts -Wdouble-promotion -Wformat -Wmain -Wmissing-braces -Wmissing-include-dirs -Wuninitialized -Wmaybe-uninitialized -Wunknown-pragmas -Wpragmas -Wstrict-aliasing -Wstrict-overflow -Wundef -Wendif-labels -Wfree-nonheap-object -Wpointer-arith -Wcast-align -Wwrite-strings -Wconversion-null -Wenum-compare -Wsign-compare -Wsizeof-pointer-memaccess -Wlogical-op -Waddress -Wno-unused -Wno-non-template-friend -Wno-zero-as-null-pointer-constant -Wno-shadow -Wno-enum-compare -Wno-narrowing -Wno-attributes -Wno-invalid-offsetof  -Werror=init-self -Werror=parentheses -Werror=return-local-addr -Werror=return-type -Werror=array-bounds -Werror=div-by-zero -Werror=missing-field-initializers -Werror=placement-new -Werror=sign-compare -Werror=cast-qual -Werror=cast-align -Werror=literal-suffix -Werror=shadow=local -Werror=delete-incomplete -Werror=subobject-linkage -Werror=odr -Werror=old-style-declaration -Werror=old-style-definition  -ggdb -fsanitize=undefined -fchkp-check-incomplete-type -Og -Wno-terminate  CACHE INTERNAL "" FORCE )
	set( PROJECTS_SHARED_DEFINES_DEBUGANALYZE  __GX_DEBUG__ __GX_ANALYZE__ CACHE INTERNAL "" FORCE )
	set( PROJECTS_SHARED_LINKER_FLAGS_DEBUGANALYZE " -static -static-libgcc -static-libstdc++" CACHE INTERNAL "" FORCE )
	# Debug
	set_property( DIRECTORY APPEND PROPERTY COMPILE_DEFINITIONS $<$<CONFIG:Debug>: > )
	set( CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS} -D_DEBUG -Og " CACHE STRING "" FORCE )
	set( CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS} -D_DEBUG -Og " CACHE STRING "" FORCE )
	set( CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS} " CACHE STRING "" FORCE )
	set( CMAKE_SHARED_LINKER_FLAGS_DEBUG "${CMAKE_SHARED_LINKER_FLAGS} " CACHE STRING "" FORCE )
	set( PROJECTS_SHARED_CXX_FLAGS_DEBUG  -std=c++1z -Wchar-subscripts -Wdouble-promotion -Wformat -Wmain -Wmissing-braces -Wmissing-include-dirs -Wuninitialized -Wmaybe-uninitialized -Wunknown-pragmas -Wpragmas -Wstrict-aliasing -Wstrict-overflow -Wundef -Wendif-labels -Wfree-nonheap-object -Wpointer-arith -Wcast-align -Wwrite-strings -Wconversion-null -Wenum-compare -Wsign-compare -Wsizeof-pointer-memaccess -Wlogical-op -Waddress -Wno-unused -Wno-non-template-friend -Wno-zero-as-null-pointer-constant -Wno-shadow -Wno-enum-compare -Wno-narrowing -Wno-attributes -Wno-invalid-offsetof  -Werror=init-self -Werror=parentheses -Werror=return-local-addr -Werror=return-type -Werror=array-bounds -Werror=div-by-zero -Werror=missing-field-initializers -Werror=placement-new -Werror=sign-compare -Werror=cast-qual -Werror=cast-align -Werror=literal-suffix -Werror=shadow=local -Werror=delete-incomplete -Werror=subobject-linkage -Werror=odr -Werror=old-style-declaration -Werror=old-style-definition  -ggdb -fchkp-check-incomplete-type -Og -Wno-terminate  CACHE INTERNAL "" FORCE )
	set( PROJECTS_SHARED_DEFINES_DEBUG  __GX_DEBUG__ CACHE INTERNAL "" FORCE )
	set( PROJECTS_SHARED_LINKER_FLAGS_DEBUG " -static -static-libgcc -static-libstdc++" CACHE INTERNAL "" FORCE )
	# Profile
	set_property( DIRECTORY APPEND PROPERTY COMPILE_DEFINITIONS $<$<CONFIG:Profile>: > )
	set( CMAKE_C_FLAGS_PROFILE "${CMAKE_C_FLAGS} -D_NDEBUG -DNDEBUG  -O2 " CACHE STRING "" FORCE )
	set( CMAKE_CXX_FLAGS_PROFILE "${CMAKE_CXX_FLAGS} -D_NDEBUG -DNDEBUG  -O2 " CACHE STRING "" FORCE )
	set( CMAKE_EXE_LINKER_FLAGS_PROFILE "${CMAKE_EXE_LINKER_FLAGS} " CACHE STRING "" FORCE )
	set( CMAKE_SHARED_LINKER_FLAGS_PROFILE "${CMAKE_SHARED_LINKER_FLAGS} " CACHE STRING "" FORCE )
	set( PROJECTS_SHARED_CXX_FLAGS_PROFILE  -std=c++1z -Wchar-subscripts -Wdouble-promotion -Wformat -Wmain -Wmissing-braces -Wmissing-include-dirs -Wuninitialized -Wmaybe-uninitialized -Wunknown-pragmas -Wpragmas -Wstrict-aliasing -Wstrict-overflow -Wundef -Wendif-labels -Wfree-nonheap-object -Wpointer-arith -Wcast-align -Wwrite-strings -Wconversion-null -Wenum-compare -Wsign-compare -Wsizeof-pointer-memaccess -Wlogical-op -Waddress -Wno-unused -Wno-non-template-friend -Wno-zero-as-null-pointer-constant -Wno-shadow -Wno-enum-compare -Wno-narrowing -Wno-attributes -Wno-invalid-offsetof  -Werror=init-self -Werror=parentheses -Werror=return-local-addr -Werror=return-type -Werror=array-bounds -Werror=div-by-zero -Werror=missing-field-initializers -Werror=placement-new -Werror=sign-compare -Werror=cast-qual -Werror=cast-align -Werror=literal-suffix -Werror=shadow=local -Werror=delete-incomplete -Werror=subobject-linkage -Werror=odr -Werror=old-style-declaration -Werror=old-style-definition  -O2 CACHE INTERNAL "" FORCE )
	set( PROJECTS_SHARED_DEFINES_PROFILE  GX_ENABLE_PROFILING CACHE INTERNAL "" FORCE )
	set( PROJECTS_SHARED_LINKER_FLAGS_PROFILE " -static -static-libgcc -static-libstdc++" CACHE INTERNAL "" FORCE )
	# Release
	set_property( DIRECTORY APPEND PROPERTY COMPILE_DEFINITIONS $<$<CONFIG:Release>: > )
	set( CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS} -D_NDEBUG -DNDEBUG  -O3 -finline-functions " CACHE STRING "" FORCE )
	set( CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS} -D_NDEBUG -DNDEBUG  -O3 -finline-functions " CACHE STRING "" FORCE )
	set( CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS} " CACHE STRING "" FORCE )
	set( CMAKE_SHARED_LINKER_FLAGS_RELEASE "${CMAKE_SHARED_LINKER_FLAGS} " CACHE STRING "" FORCE )
	set( PROJECTS_SHARED_CXX_FLAGS_RELEASE  -std=c++1z -Wchar-subscripts -Wdouble-promotion -Wformat -Wmain -Wmissing-braces -Wmissing-include-dirs -Wuninitialized -Wmaybe-uninitialized -Wunknown-pragmas -Wpragmas -Wstrict-aliasing -Wstrict-overflow -Wundef -Wendif-labels -Wfree-nonheap-object -Wpointer-arith -Wcast-align -Wwrite-strings -Wconversion-null -Wenum-compare -Wsign-compare -Wsizeof-pointer-memaccess -Wlogical-op -Waddress -Wno-unused -Wno-non-template-friend -Wno-zero-as-null-pointer-constant -Wno-shadow -Wno-enum-compare -Wno-narrowing -Wno-attributes -Wno-invalid-offsetof  -Werror=init-self -Werror=parentheses -Werror=return-local-addr -Werror=return-type -Werror=array-bounds -Werror=div-by-zero -Werror=missing-field-initializers -Werror=placement-new -Werror=sign-compare -Werror=cast-qual -Werror=cast-align -Werror=literal-suffix -Werror=shadow=local -Werror=delete-incomplete -Werror=subobject-linkage -Werror=odr -Werror=old-style-declaration -Werror=old-style-definition  -O3 -Ofast -fomit-frame-pointer -finline-functions CACHE INTERNAL "" FORCE )
	set( PROJECTS_SHARED_DEFINES_RELEASE  __GX_NO_EXCEPTIONS__ CACHE INTERNAL "" FORCE )
	set( PROJECTS_SHARED_LINKER_FLAGS_RELEASE " -static -static-libgcc -static-libstdc++" CACHE INTERNAL "" FORCE )
	set( CMAKE_BUILD_TYPE "Debug")
	#--------------------------------------------
	set( CONFIGURATION_DEPENDENT_PATH OFF CACHE INTERNAL "" FORCE )
	#--------------------------------------------
endif()


#==================================================================================================
# Clang Compilation settings
#==================================================================================================
set( COMPILER_CLANG_APPLE OFF )
string( FIND "${CMAKE_CXX_COMPILER_ID}" "Clang" outPos )
if ( (outPos GREATER -1) AND (APPLE) )
	set( COMPILER_CLANG_APPLE ON )
endif()
if ( COMPILER_CLANG_APPLE )
	set( DETECTED_COMPILER "COMPILER_CLANG_APPLE" )
	message( STATUS "CMAKE_C_FLAGS: ${CMAKE_C_FLAGS}" )
	message( STATUS "CMAKE_CXX_FLAGS: ${CMAKE_CXX_FLAGS}" )
	message( STATUS "CMAKE_EXE_LINKER_FLAGS: ${CMAKE_EXE_LINKER_FLAGS}" )
	message( STATUS "CMAKE_SHARED_LINKER_FLAGS: ${CMAKE_SHARED_LINKER_FLAGS}" )
	set( CMAKE_CONFIGURATION_TYPES DebugAnalyze Debug Profile Release )
	set( CMAKE_CONFIGURATION_TYPES "${CMAKE_CONFIGURATION_TYPES}" CACHE STRING "Configurations" FORCE )
	
	# DebugAnalyze
	set_property( DIRECTORY APPEND PROPERTY COMPILE_DEFINITIONS $<$<CONFIG:DebugAnalyze>: > )
	set( CMAKE_C_FLAGS_DEBUGANALYZE "${CMAKE_C_FLAGS} -D_DEBUG -Og " CACHE STRING "" FORCE )
	set( CMAKE_CXX_FLAGS_DEBUGANALYZE "${CMAKE_CXX_FLAGS} -D_DEBUG -Og " CACHE STRING "" FORCE )
	set( CMAKE_EXE_LINKER_FLAGS_DEBUGANALYZE "${CMAKE_EXE_LINKER_FLAGS} " CACHE STRING "" FORCE )
	set( CMAKE_SHARED_LINKER_FLAGS_DEBUGANALYZE "${CMAKE_SHARED_LINKER_FLAGS} " CACHE STRING "" FORCE )
	set( PROJECTS_SHARED_CXX_FLAGS_DEBUGANALYZE  -std=c++1z -Wchar-subscripts -Wdouble-promotion -Wformat -Wmain -Wmissing-braces -Wmissing-include-dirs -Wuninitialized -Wunknown-pragmas -Wpragmas -Wstrict-aliasing -Wstrict-overflow -Wundef -Wendif-labels -Wpointer-arith -Wwrite-strings -Wconversion-null -Wenum-compare -Wsign-compare -Wsizeof-pointer-memaccess -frtti -fexceptions -Wloop-analysis -Wincrement-bool -Werror=init-self -Werror=parentheses -Werror=return-stack-address -Werror=return-type -Werror=user-defined-literals -Werror=array-bounds -Werror=div-by-zero -Werror=address -Werror=missing-field-initializers -Werror=instantiation-after-specialization -Werror=cast-qual -Werror=unknown-warning-option -Werror=keyword-macro -Werror=large-by-value-copy -Werror=method-signatures -Werror=self-assign -Werror=self-move -Werror=infinite-recursion -Werror=pessimizing-move -Werror=dangling-else  -Wno-comment -Wno-undefined-inline -Wno-switch -Wno-c++11-narrowing -Wno-unused -Wno-c++14-extensions -Wno-c++1z-extensions  -ggdb -fsanitize=undefined -Og CACHE INTERNAL "" FORCE )
	set( PROJECTS_SHARED_DEFINES_DEBUGANALYZE  __GX_DEBUG__ __GX_ANALYZE__ CACHE INTERNAL "" FORCE )
	set( PROJECTS_SHARED_LINKER_FLAGS_DEBUGANALYZE "" CACHE INTERNAL "" FORCE )
	# Debug
	set_property( DIRECTORY APPEND PROPERTY COMPILE_DEFINITIONS $<$<CONFIG:Debug>: > )
	set( CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS} -D_DEBUG -Og " CACHE STRING "" FORCE )
	set( CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS} -D_DEBUG -Og " CACHE STRING "" FORCE )
	set( CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS} " CACHE STRING "" FORCE )
	set( CMAKE_SHARED_LINKER_FLAGS_DEBUG "${CMAKE_SHARED_LINKER_FLAGS} " CACHE STRING "" FORCE )
	set( PROJECTS_SHARED_CXX_FLAGS_DEBUG  -std=c++1z -Wchar-subscripts -Wdouble-promotion -Wformat -Wmain -Wmissing-braces -Wmissing-include-dirs -Wuninitialized -Wunknown-pragmas -Wpragmas -Wstrict-aliasing -Wstrict-overflow -Wundef -Wendif-labels -Wpointer-arith -Wwrite-strings -Wconversion-null -Wenum-compare -Wsign-compare -Wsizeof-pointer-memaccess -frtti -fexceptions -Wloop-analysis -Wincrement-bool -Werror=init-self -Werror=parentheses -Werror=return-stack-address -Werror=return-type -Werror=user-defined-literals -Werror=array-bounds -Werror=div-by-zero -Werror=address -Werror=missing-field-initializers -Werror=instantiation-after-specialization -Werror=cast-qual -Werror=unknown-warning-option -Werror=keyword-macro -Werror=large-by-value-copy -Werror=method-signatures -Werror=self-assign -Werror=self-move -Werror=infinite-recursion -Werror=pessimizing-move -Werror=dangling-else  -Wno-comment -Wno-undefined-inline -Wno-switch -Wno-c++11-narrowing -Wno-unused -Wno-c++14-extensions -Wno-c++1z-extensions  -ggdb -Og CACHE INTERNAL "" FORCE )
	set( PROJECTS_SHARED_DEFINES_DEBUG  __GX_DEBUG__ CACHE INTERNAL "" FORCE )
	set( PROJECTS_SHARED_LINKER_FLAGS_DEBUG "" CACHE INTERNAL "" FORCE )
	# Profile
	set_property( DIRECTORY APPEND PROPERTY COMPILE_DEFINITIONS $<$<CONFIG:Profile>: > )
	set( CMAKE_C_FLAGS_PROFILE "${CMAKE_C_FLAGS} -D_NDEBUG -DNDEBUG  -O2 " CACHE STRING "" FORCE )
	set( CMAKE_CXX_FLAGS_PROFILE "${CMAKE_CXX_FLAGS} -D_NDEBUG -DNDEBUG  -O2 " CACHE STRING "" FORCE )
	set( CMAKE_EXE_LINKER_FLAGS_PROFILE "${CMAKE_EXE_LINKER_FLAGS} " CACHE STRING "" FORCE )
	set( CMAKE_SHARED_LINKER_FLAGS_PROFILE "${CMAKE_SHARED_LINKER_FLAGS} " CACHE STRING "" FORCE )
	set( PROJECTS_SHARED_CXX_FLAGS_PROFILE  -std=c++1z -Wchar-subscripts -Wdouble-promotion -Wformat -Wmain -Wmissing-braces -Wmissing-include-dirs -Wuninitialized -Wunknown-pragmas -Wpragmas -Wstrict-aliasing -Wstrict-overflow -Wundef -Wendif-labels -Wpointer-arith -Wwrite-strings -Wconversion-null -Wenum-compare -Wsign-compare -Wsizeof-pointer-memaccess -frtti -fexceptions -Wloop-analysis -Wincrement-bool -Werror=init-self -Werror=parentheses -Werror=return-stack-address -Werror=return-type -Werror=user-defined-literals -Werror=array-bounds -Werror=div-by-zero -Werror=address -Werror=missing-field-initializers -Werror=instantiation-after-specialization -Werror=cast-qual -Werror=unknown-warning-option -Werror=keyword-macro -Werror=large-by-value-copy -Werror=method-signatures -Werror=self-assign -Werror=self-move -Werror=infinite-recursion -Werror=pessimizing-move -Werror=dangling-else  -Wno-comment -Wno-undefined-inline -Wno-switch -Wno-c++11-narrowing -Wno-unused -Wno-c++14-extensions -Wno-c++1z-extensions  -O2 -finline-functions CACHE INTERNAL "" FORCE )
	set( PROJECTS_SHARED_DEFINES_PROFILE  GX_ENABLE_PROFILING CACHE INTERNAL "" FORCE )
	set( PROJECTS_SHARED_LINKER_FLAGS_PROFILE "" CACHE INTERNAL "" FORCE )
	# Release
	set_property( DIRECTORY APPEND PROPERTY COMPILE_DEFINITIONS $<$<CONFIG:Release>: > )
	set( CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS} -D_NDEBUG -DNDEBUG  -O3 -finline-functions " CACHE STRING "" FORCE )
	set( CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS} -D_NDEBUG -DNDEBUG  -O3 -finline-functions " CACHE STRING "" FORCE )
	set( CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS} " CACHE STRING "" FORCE )
	set( CMAKE_SHARED_LINKER_FLAGS_RELEASE "${CMAKE_SHARED_LINKER_FLAGS} " CACHE STRING "" FORCE )
	set( PROJECTS_SHARED_CXX_FLAGS_RELEASE  -std=c++1z -Wchar-subscripts -Wdouble-promotion -Wformat -Wmain -Wmissing-braces -Wmissing-include-dirs -Wuninitialized -Wunknown-pragmas -Wpragmas -Wstrict-aliasing -Wstrict-overflow -Wundef -Wendif-labels -Wpointer-arith -Wwrite-strings -Wconversion-null -Wenum-compare -Wsign-compare -Wsizeof-pointer-memaccess -frtti -fexceptions -Wloop-analysis -Wincrement-bool -Werror=init-self -Werror=parentheses -Werror=return-stack-address -Werror=return-type -Werror=user-defined-literals -Werror=array-bounds -Werror=div-by-zero -Werror=address -Werror=missing-field-initializers -Werror=instantiation-after-specialization -Werror=cast-qual -Werror=unknown-warning-option -Werror=keyword-macro -Werror=large-by-value-copy -Werror=method-signatures -Werror=self-assign -Werror=self-move -Werror=infinite-recursion -Werror=pessimizing-move -Werror=dangling-else  -Wno-comment -Wno-undefined-inline -Wno-switch -Wno-c++11-narrowing -Wno-unused -Wno-c++14-extensions -Wno-c++1z-extensions  -O3 -Ofast -fomit-frame-pointer -finline-functions CACHE INTERNAL "" FORCE )
	set( PROJECTS_SHARED_DEFINES_RELEASE  __GX_NO_EXCEPTIONS__ CACHE INTERNAL "" FORCE )
	set( PROJECTS_SHARED_LINKER_FLAGS_RELEASE "" CACHE INTERNAL "" FORCE )
	set( CMAKE_BUILD_TYPE "Debug")
	#--------------------------------------------
	set( CONFIGURATION_DEPENDENT_PATH ON CACHE INTERNAL "" FORCE )
	#--------------------------------------------
endif()


#==================================================================================================
# Clang Compilation settings
#==================================================================================================
set( COMPILER_CLANG_ANDROID OFF )
string( FIND "${CMAKE_CXX_COMPILER_ID}" "Clang" outPos )
if ( (outPos GREATER -1) AND (DEFINED ANDROID) )
	set( COMPILER_CLANG_ANDROID ON )
endif()
if ( COMPILER_CLANG_ANDROID )
	set( DETECTED_COMPILER "COMPILER_CLANG_ANDROID" )
	message( STATUS "CMAKE_C_FLAGS: ${CMAKE_C_FLAGS}" )
	message( STATUS "CMAKE_CXX_FLAGS: ${CMAKE_CXX_FLAGS}" )
	message( STATUS "CMAKE_EXE_LINKER_FLAGS: ${CMAKE_EXE_LINKER_FLAGS}" )
	message( STATUS "CMAKE_SHARED_LINKER_FLAGS: ${CMAKE_SHARED_LINKER_FLAGS}" )
	set( CMAKE_CONFIGURATION_TYPES DebugAnalyze Debug Profile Release )
	set( CMAKE_CONFIGURATION_TYPES "${CMAKE_CONFIGURATION_TYPES}" CACHE STRING "Configurations" FORCE )
	
	# DebugAnalyze
	set_property( DIRECTORY APPEND PROPERTY COMPILE_DEFINITIONS $<$<CONFIG:DebugAnalyze>: > )
	set( CMAKE_C_FLAGS_DEBUGANALYZE "${CMAKE_C_FLAGS} -D_DEBUG -Og " CACHE STRING "" FORCE )
	set( CMAKE_CXX_FLAGS_DEBUGANALYZE "${CMAKE_CXX_FLAGS} -D_DEBUG -Og " CACHE STRING "" FORCE )
	set( CMAKE_EXE_LINKER_FLAGS_DEBUGANALYZE "${CMAKE_EXE_LINKER_FLAGS} " CACHE STRING "" FORCE )
	set( CMAKE_SHARED_LINKER_FLAGS_DEBUGANALYZE "${CMAKE_SHARED_LINKER_FLAGS} " CACHE STRING "" FORCE )
	set( PROJECTS_SHARED_CXX_FLAGS_DEBUGANALYZE  -std=c++1z -Wchar-subscripts -Wdouble-promotion -Wformat -Wmain -Wmissing-braces -Wmissing-include-dirs -Wuninitialized -Wunknown-pragmas -Wpragmas -Wstrict-aliasing -Wstrict-overflow -Wundef -Wendif-labels -Wpointer-arith -Wwrite-strings -Wconversion-null -Wenum-compare -Wsign-compare -Wsizeof-pointer-memaccess -frtti -fexceptions -Wloop-analysis -Wincrement-bool -Werror=init-self -Werror=parentheses -Werror=return-stack-address -Werror=return-type -Werror=user-defined-literals -Werror=array-bounds -Werror=div-by-zero -Werror=address -Werror=missing-field-initializers -Werror=instantiation-after-specialization -Werror=cast-qual -Werror=unknown-warning-option -Werror=keyword-macro -Werror=large-by-value-copy -Werror=dangling-else -Werror=method-signatures -Werror=self-assign -Werror=self-move -Werror=infinite-recursion -Werror=pessimizing-move  -Wno-comment -Wno-undefined-inline -Wno-switch -Wno-c++11-narrowing -Wno-unused -Wno-c++14-extensions -Wno-c++1z-extensions  -ggdb -fsanitize=undefined -Og CACHE INTERNAL "" FORCE )
	set( PROJECTS_SHARED_DEFINES_DEBUGANALYZE  __GX_DEBUG__ __GX_ANALYZE__ CACHE INTERNAL "" FORCE )
	set( PROJECTS_SHARED_LINKER_FLAGS_DEBUGANALYZE " -static" CACHE INTERNAL "" FORCE )
	# Debug
	set_property( DIRECTORY APPEND PROPERTY COMPILE_DEFINITIONS $<$<CONFIG:Debug>: > )
	set( CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS} -D_DEBUG -Og " CACHE STRING "" FORCE )
	set( CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS} -D_DEBUG -Og " CACHE STRING "" FORCE )
	set( CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS} " CACHE STRING "" FORCE )
	set( CMAKE_SHARED_LINKER_FLAGS_DEBUG "${CMAKE_SHARED_LINKER_FLAGS} " CACHE STRING "" FORCE )
	set( PROJECTS_SHARED_CXX_FLAGS_DEBUG  -std=c++1z -Wchar-subscripts -Wdouble-promotion -Wformat -Wmain -Wmissing-braces -Wmissing-include-dirs -Wuninitialized -Wunknown-pragmas -Wpragmas -Wstrict-aliasing -Wstrict-overflow -Wundef -Wendif-labels -Wpointer-arith -Wwrite-strings -Wconversion-null -Wenum-compare -Wsign-compare -Wsizeof-pointer-memaccess -frtti -fexceptions -Wloop-analysis -Wincrement-bool -Werror=init-self -Werror=parentheses -Werror=return-stack-address -Werror=return-type -Werror=user-defined-literals -Werror=array-bounds -Werror=div-by-zero -Werror=address -Werror=missing-field-initializers -Werror=instantiation-after-specialization -Werror=cast-qual -Werror=unknown-warning-option -Werror=keyword-macro -Werror=large-by-value-copy -Werror=dangling-else -Werror=method-signatures -Werror=self-assign -Werror=self-move -Werror=infinite-recursion -Werror=pessimizing-move  -Wno-comment -Wno-undefined-inline -Wno-switch -Wno-c++11-narrowing -Wno-unused -Wno-c++14-extensions -Wno-c++1z-extensions  -ggdb -Og CACHE INTERNAL "" FORCE )
	set( PROJECTS_SHARED_DEFINES_DEBUG  __GX_DEBUG__ CACHE INTERNAL "" FORCE )
	set( PROJECTS_SHARED_LINKER_FLAGS_DEBUG " -static" CACHE INTERNAL "" FORCE )
	# Profile
	set_property( DIRECTORY APPEND PROPERTY COMPILE_DEFINITIONS $<$<CONFIG:Profile>: > )
	set( CMAKE_C_FLAGS_PROFILE "${CMAKE_C_FLAGS} -D_NDEBUG -DNDEBUG  -O2 " CACHE STRING "" FORCE )
	set( CMAKE_CXX_FLAGS_PROFILE "${CMAKE_CXX_FLAGS} -D_NDEBUG -DNDEBUG  -O2 " CACHE STRING "" FORCE )
	set( CMAKE_EXE_LINKER_FLAGS_PROFILE "${CMAKE_EXE_LINKER_FLAGS} " CACHE STRING "" FORCE )
	set( CMAKE_SHARED_LINKER_FLAGS_PROFILE "${CMAKE_SHARED_LINKER_FLAGS} " CACHE STRING "" FORCE )
	set( PROJECTS_SHARED_CXX_FLAGS_PROFILE  -std=c++1z -Wchar-subscripts -Wdouble-promotion -Wformat -Wmain -Wmissing-braces -Wmissing-include-dirs -Wuninitialized -Wunknown-pragmas -Wpragmas -Wstrict-aliasing -Wstrict-overflow -Wundef -Wendif-labels -Wpointer-arith -Wwrite-strings -Wconversion-null -Wenum-compare -Wsign-compare -Wsizeof-pointer-memaccess -frtti -fexceptions -Wloop-analysis -Wincrement-bool -Werror=init-self -Werror=parentheses -Werror=return-stack-address -Werror=return-type -Werror=user-defined-literals -Werror=array-bounds -Werror=div-by-zero -Werror=address -Werror=missing-field-initializers -Werror=instantiation-after-specialization -Werror=cast-qual -Werror=unknown-warning-option -Werror=keyword-macro -Werror=large-by-value-copy -Werror=dangling-else -Werror=method-signatures -Werror=self-assign -Werror=self-move -Werror=infinite-recursion -Werror=pessimizing-move  -Wno-comment -Wno-undefined-inline -Wno-switch -Wno-c++11-narrowing -Wno-unused -Wno-c++14-extensions -Wno-c++1z-extensions  -O2 -finline-functions CACHE INTERNAL "" FORCE )
	set( PROJECTS_SHARED_DEFINES_PROFILE  GX_ENABLE_PROFILING CACHE INTERNAL "" FORCE )
	set( PROJECTS_SHARED_LINKER_FLAGS_PROFILE " -static" CACHE INTERNAL "" FORCE )
	# Release
	set_property( DIRECTORY APPEND PROPERTY COMPILE_DEFINITIONS $<$<CONFIG:Release>: > )
	set( CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS} -D_NDEBUG -DNDEBUG  -O3 -finline-functions " CACHE STRING "" FORCE )
	set( CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS} -D_NDEBUG -DNDEBUG  -O3 -finline-functions " CACHE STRING "" FORCE )
	set( CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS} " CACHE STRING "" FORCE )
	set( CMAKE_SHARED_LINKER_FLAGS_RELEASE "${CMAKE_SHARED_LINKER_FLAGS} " CACHE STRING "" FORCE )
	set( PROJECTS_SHARED_CXX_FLAGS_RELEASE  -std=c++1z -Wchar-subscripts -Wdouble-promotion -Wformat -Wmain -Wmissing-braces -Wmissing-include-dirs -Wuninitialized -Wunknown-pragmas -Wpragmas -Wstrict-aliasing -Wstrict-overflow -Wundef -Wendif-labels -Wpointer-arith -Wwrite-strings -Wconversion-null -Wenum-compare -Wsign-compare -Wsizeof-pointer-memaccess -frtti -fexceptions -Wloop-analysis -Wincrement-bool -Werror=init-self -Werror=parentheses -Werror=return-stack-address -Werror=return-type -Werror=user-defined-literals -Werror=array-bounds -Werror=div-by-zero -Werror=address -Werror=missing-field-initializers -Werror=instantiation-after-specialization -Werror=cast-qual -Werror=unknown-warning-option -Werror=keyword-macro -Werror=large-by-value-copy -Werror=dangling-else -Werror=method-signatures -Werror=self-assign -Werror=self-move -Werror=infinite-recursion -Werror=pessimizing-move  -Wno-comment -Wno-undefined-inline -Wno-switch -Wno-c++11-narrowing -Wno-unused -Wno-c++14-extensions -Wno-c++1z-extensions  -O3 -Ofast -fomit-frame-pointer -finline-functions CACHE INTERNAL "" FORCE )
	set( PROJECTS_SHARED_DEFINES_RELEASE  __GX_NO_EXCEPTIONS__ CACHE INTERNAL "" FORCE )
	set( PROJECTS_SHARED_LINKER_FLAGS_RELEASE " -static" CACHE INTERNAL "" FORCE )
	set( CMAKE_BUILD_TYPE "Debug")
	#--------------------------------------------
	set( CONFIGURATION_DEPENDENT_PATH OFF CACHE INTERNAL "" FORCE )
	#--------------------------------------------
endif()


if ( NOT DEFINED DETECTED_COMPILER )
	message( FATAL_ERROR "current compiler: '${CMAKE_CXX_COMPILER_ID}' is not configured for this project!" )
endif()

message( STATUS "Compiler: ${CMAKE_CXX_COMPILER_ID} (${CMAKE_CXX_COMPILER_VERSION})" )
message( STATUS "Compiler name: ${DETECTED_COMPILER}" )
message( STATUS "target system: ${CMAKE_SYSTEM_NAME} (${CMAKE_SYSTEM_VERSION})" )
message( STATUS "host system: ${CMAKE_HOST_SYSTEM_NAME} (${CMAKE_HOST_SYSTEM_VERSION})" )

message( STATUS "adding external projects" )


#==================================================================================================
# External: External/cmake
#==================================================================================================
message( STATUS "-----------------------------------------------------" )
message( STATUS "external project 'External/cmake' generation started" )
add_subdirectory( "External/cmake" )
#-----------------------------------
include( "External/cmake/options.cmake" )
#-----------------------------------

message( STATUS "external project 'External/cmake' generation ended" )
message( STATUS "-----------------------------------------------------" )


#==================================================================================================
# project: Engine.Config
#==================================================================================================
set( SOURCES 
	"Engine/Config/Engine.Config.h"
	"Engine/Config/Engine.Version.h"
	"Engine/Config/Main.cpp"
	"Engine/Config/STL.Config.h" )
add_library( "Engine.Config" STATIC ${SOURCES} )
source_group( "" FILES "Engine/Config/Engine.Config.h" "Engine/Config/Engine.Version.h" "Engine/Config/Main.cpp" "Engine/Config/STL.Config.h" )
set_property( TARGET "Engine.Config" PROPERTY FOLDER "Engine" )
target_include_directories( "Engine.Config" PRIVATE "Engine/Config" )
target_include_directories( "Engine.Config" PUBLIC "External" )
target_include_directories( "Engine.Config" PUBLIC "${EXTERNALS_PATH}" )
target_include_directories( "Engine.Config" PUBLIC "Engine/.." )
# compiler
set_target_properties( "Engine.Config" PROPERTIES LINK_FLAGS_RELEASE ${PROJECTS_SHARED_LINKER_FLAGS_RELEASE} )
set_target_properties( "Engine.Config" PROPERTIES LINK_FLAGS_PROFILE ${PROJECTS_SHARED_LINKER_FLAGS_PROFILE} )
target_compile_definitions( "Engine.Config" PRIVATE $<$<CONFIG:Debug>: ${PROJECTS_SHARED_DEFINES_DEBUG}> )
set_target_properties( "Engine.Config" PROPERTIES LINK_FLAGS_DEBUGANALYZE ${PROJECTS_SHARED_LINKER_FLAGS_DEBUGANALYZE} )
target_compile_definitions( "Engine.Config" PRIVATE $<$<CONFIG:DebugAnalyze>: ${PROJECTS_SHARED_DEFINES_DEBUGANALYZE}> )
target_compile_options( "Engine.Config" PRIVATE $<$<CONFIG:Profile>: ${PROJECTS_SHARED_CXX_FLAGS_PROFILE}> )
target_compile_options( "Engine.Config" PRIVATE $<$<CONFIG:Release>: ${PROJECTS_SHARED_CXX_FLAGS_RELEASE}> )
target_compile_definitions( "Engine.Config" PRIVATE $<$<CONFIG:Profile>: ${PROJECTS_SHARED_DEFINES_PROFILE}> )
target_compile_definitions( "Engine.Config" PRIVATE $<$<CONFIG:Release>: ${PROJECTS_SHARED_DEFINES_RELEASE}> )
set_target_properties( "Engine.Config" PROPERTIES LINK_FLAGS_DEBUG ${PROJECTS_SHARED_LINKER_FLAGS_DEBUG} )
target_compile_options( "Engine.Config" PRIVATE $<$<CONFIG:DebugAnalyze>: ${PROJECTS_SHARED_CXX_FLAGS_DEBUGANALYZE}> )
target_compile_options( "Engine.Config" PRIVATE $<$<CONFIG:Debug>: ${PROJECTS_SHARED_CXX_FLAGS_DEBUG}> )



#==================================================================================================
# project: Engine.STL
#==================================================================================================
set( SOURCES 
	"Engine/STL/Algorithms/ArrayUtils.h"
	"Engine/STL/Algorithms/Comparators.h"
	"Engine/STL/Algorithms/Enum.h"
	"Engine/STL/Algorithms/FileAddress.cpp"
	"Engine/STL/Algorithms/FileAddress.h"
	"Engine/STL/Algorithms/Hash.h"
	"Engine/STL/Algorithms/InvokeWithVariant.h"
	"Engine/STL/Algorithms/Range.h"
	"Engine/STL/Algorithms/Sorts.h"
	"Engine/STL/Algorithms/StringParser.cpp"
	"Engine/STL/Algorithms/StringParser.h"
	"Engine/STL/Algorithms/StringUtils.h"
	"Engine/STL/Algorithms/Swap.h"
	"Engine/STL/Types/Bitfield.h"
	"Engine/STL/Types/Bool.h"
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
	"Engine/STL/OS/Windows/FileSystem.h"
	"Engine/STL/OS/Windows/Library.h"
	"Engine/STL/OS/Windows/OSWindows.h"
	"Engine/STL/OS/Windows/PlatformUtils.h"
	"Engine/STL/OS/Windows/RandDevice.h"
	"Engine/STL/OS/Windows/SyncPrimitives.h"
	"Engine/STL/OS/Windows/Thread.h"
	"Engine/STL/OS/Windows/Timer.h"
	"Engine/STL/OS/Windows/WinAtomicOp.cpp"
	"Engine/STL/OS/Windows/WinFileSystem.cpp"
	"Engine/STL/OS/Windows/WinHeader.h"
	"Engine/STL/OS/Windows/WinLibrary.cpp"
	"Engine/STL/OS/Windows/WinPlatformUtils.cpp"
	"Engine/STL/OS/Windows/WinRandDevice.cpp"
	"Engine/STL/OS/Windows/WinSyncPrimitives.cpp"
	"Engine/STL/OS/Windows/WinThread.cpp"
	"Engine/STL/OS/Windows/WinTimer.cpp"
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
	"Engine/STL/OS/Posix/PosixFileSystem.cpp"
	"Engine/STL/OS/Posix/PosixHeader.h"
	"Engine/STL/OS/Posix/PosixLibrary.cpp"
	"Engine/STL/OS/Posix/PosixRandDevice.cpp"
	"Engine/STL/OS/Posix/PosixSyncPrimitives.cpp"
	"Engine/STL/OS/Posix/PosixTimer.cpp"
	"Engine/STL/OS/Posix/RandDevice.h"
	"Engine/STL/OS/Posix/SyncPrimitives.h"
	"Engine/STL/OS/Posix/Thread.h"
	"Engine/STL/OS/Posix/Timer.h"
	"Engine/STL/OS/Android/OSAndroid.h"
	"Engine/STL/Math/Algebra.h"
	"Engine/STL/Math/BinaryMath.h"
	"Engine/STL/Math/FastMath.h"
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
	"Engine/STL/Math/OverflowCheck.h"
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
	"Engine/STL/OS/SDL/SdlLibrary.cpp"
	"Engine/STL/OS/SDL/SdlSyncPrimitives.cpp"
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
	"Engine/STL/Containers/IndexedIterator.h"
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
	"Engine/STL/Containers/Tuple.h"
	"Engine/STL/Containers/UniBuffer.h"
	"Engine/STL/DataBase/SimpleDB.h"
	"Engine/STL/DataBase/Utf8StringUtils.cpp"
	"Engine/STL/DataBase/Utf8StringUtils.h"
	"Engine/STL/Common/Compilers/CompilerClang.h"
	"Engine/STL/Common/Compilers/CompilerGCC.h"
	"Engine/STL/Common/Compilers/CompilerMSVC.h"
	"Engine/STL/Engine.STL.h"
	"Engine/STL/Compression/Compression.h"
	"Engine/STL/Compression/LZ4Compression.h"
	"Engine/STL/Compression/MiniZCompression.h"
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
	"Engine/STL/CompileTime/DeferredTemplates.h"
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
add_library( "Engine.STL" STATIC ${SOURCES} )
source_group( "Algorithms" FILES "Engine/STL/Algorithms/ArrayUtils.h" "Engine/STL/Algorithms/Comparators.h" "Engine/STL/Algorithms/Enum.h" "Engine/STL/Algorithms/FileAddress.cpp" "Engine/STL/Algorithms/FileAddress.h" "Engine/STL/Algorithms/Hash.h" "Engine/STL/Algorithms/InvokeWithVariant.h" "Engine/STL/Algorithms/Range.h" "Engine/STL/Algorithms/Sorts.h" "Engine/STL/Algorithms/StringParser.cpp" "Engine/STL/Algorithms/StringParser.h" "Engine/STL/Algorithms/StringUtils.h" "Engine/STL/Algorithms/Swap.h" )
source_group( "Types" FILES "Engine/STL/Types/Bitfield.h" "Engine/STL/Types/Bool.h" "Engine/STL/Types/DefferedType.h" "Engine/STL/Types/Delegate.h" "Engine/STL/Types/Function.h" "Engine/STL/Types/Limit.h" "Engine/STL/Types/Noncopyable.h" "Engine/STL/Types/Noninstancable.h" "Engine/STL/Types/Optional.h" "Engine/STL/Types/Ptr.h" "Engine/STL/Types/RefCountedObject.h" "Engine/STL/Types/RefCounter.h" "Engine/STL/Types/ScopeSetter.h" "Engine/STL/Types/SharedPointer.h" "Engine/STL/Types/StaticLimit.h" "Engine/STL/Types/StaticRefCountedObject.h" "Engine/STL/Types/Union.h" "Engine/STL/Types/UniquePtr.h" "Engine/STL/Types/VariantCRef.h" "Engine/STL/Types/VariantInterface.h" "Engine/STL/Types/VariantRef.h" "Engine/STL/Types/VariantRefIndexedContainer.h" "Engine/STL/Types/WeakPointer.h" )
source_group( "Dimensions" FILES "Engine/STL/Dimensions/ByteAndBit.h" "Engine/STL/Dimensions/Percentage.h" "Engine/STL/Dimensions/PowerOfTwoValue.h" "Engine/STL/Dimensions/RadiansAndDegrees.h" )
source_group( "Log" FILES "Engine/STL/Log/ELog.h" "Engine/STL/Log/Logger.cpp" "Engine/STL/Log/Logger.h" "Engine/STL/Log/ToString.h" )
source_group( "Java" FILES "Engine/STL/Java/Java.cpp" "Engine/STL/Java/JavaArray.h" "Engine/STL/Java/JavaClass.h" "Engine/STL/Java/JavaEnvironment.h" "Engine/STL/Java/JavaMethod.h" "Engine/STL/Java/JavaMethodType.h" "Engine/STL/Java/JavaString.h" "Engine/STL/Java/JavaTypeInfo.h" )
source_group( "Math\\2D" FILES "Engine/STL/Math/2D/Circle.h" "Engine/STL/Math/2D/Line2.h" "Engine/STL/Math/2D/MathTypes2D.h" "Engine/STL/Math/2D/OrientedRectangle.h" "Engine/STL/Math/2D/Rectangle.h" )
source_group( "OS\\STD" FILES "Engine/STL/OS/STD/StdThread.cpp" "Engine/STL/OS/STD/Thread.h" "Engine/STL/OS/STD/Timer.h" )
source_group( "CompileTime\\Runtime" FILES "Engine/STL/CompileTime/Runtime/TypeIdList.h" )
source_group( "Math\\Image" FILES "Engine/STL/Math/Image/ImageUtils.h" )
source_group( "Math\\Rand" FILES "Engine/STL/Math/Rand/NormalDistribution.h" "Engine/STL/Math/Rand/Pseudorandom.h" "Engine/STL/Math/Rand/RandEngine.h" "Engine/STL/Math/Rand/Random.h" "Engine/STL/Math/Rand/RandomWithChance.h" )
source_group( "Time" FILES "Engine/STL/Time/FloatTimeImpl.h" "Engine/STL/Time/IntTimeImpl.h" "Engine/STL/Time/Time.h" "Engine/STL/Time/TimeProfiler.h" )
source_group( "Math\\Spline" FILES "Engine/STL/Math/Spline/Spline.h" )
source_group( "OS\\Windows" FILES "Engine/STL/OS/Windows/AtomicOp.h" "Engine/STL/OS/Windows/FileSystem.h" "Engine/STL/OS/Windows/Library.h" "Engine/STL/OS/Windows/OSWindows.h" "Engine/STL/OS/Windows/PlatformUtils.h" "Engine/STL/OS/Windows/RandDevice.h" "Engine/STL/OS/Windows/SyncPrimitives.h" "Engine/STL/OS/Windows/Thread.h" "Engine/STL/OS/Windows/Timer.h" "Engine/STL/OS/Windows/WinAtomicOp.cpp" "Engine/STL/OS/Windows/WinFileSystem.cpp" "Engine/STL/OS/Windows/WinHeader.h" "Engine/STL/OS/Windows/WinLibrary.cpp" "Engine/STL/OS/Windows/WinPlatformUtils.cpp" "Engine/STL/OS/Windows/WinRandDevice.cpp" "Engine/STL/OS/Windows/WinSyncPrimitives.cpp" "Engine/STL/OS/Windows/WinThread.cpp" "Engine/STL/OS/Windows/WinTimer.cpp" )
source_group( "ThreadSafe" FILES "Engine/STL/ThreadSafe/Atomic.h" "Engine/STL/ThreadSafe/AtomicPtr.h" "Engine/STL/ThreadSafe/MtFile.h" "Engine/STL/ThreadSafe/MtQueue.h" "Engine/STL/ThreadSafe/Singleton.h" )
source_group( "OS\\Posix" FILES "Engine/STL/OS/Posix/AtomicOp.h" "Engine/STL/OS/Posix/FileSystem.h" "Engine/STL/OS/Posix/Library.h" "Engine/STL/OS/Posix/OSPosix.h" "Engine/STL/OS/Posix/PlatformUtils.h" "Engine/STL/OS/Posix/PosixFileSystem.cpp" "Engine/STL/OS/Posix/PosixHeader.h" "Engine/STL/OS/Posix/PosixLibrary.cpp" "Engine/STL/OS/Posix/PosixRandDevice.cpp" "Engine/STL/OS/Posix/PosixSyncPrimitives.cpp" "Engine/STL/OS/Posix/PosixTimer.cpp" "Engine/STL/OS/Posix/RandDevice.h" "Engine/STL/OS/Posix/SyncPrimitives.h" "Engine/STL/OS/Posix/Thread.h" "Engine/STL/OS/Posix/Timer.h" )
source_group( "OS\\Android" FILES "Engine/STL/OS/Android/OSAndroid.h" )
source_group( "Math" FILES "Engine/STL/Math/Algebra.h" "Engine/STL/Math/BinaryMath.h" "Engine/STL/Math/FastMath.h" "Engine/STL/Math/Interpolations.h" "Engine/STL/Math/MathConstants.h" "Engine/STL/Math/Mathematics.h" "Engine/STL/Math/MathFunc.h" "Engine/STL/Math/MathTypeCast.h" "Engine/STL/Math/MathTypes.h" "Engine/STL/Math/Matrix.h" "Engine/STL/Math/Matrix2.h" "Engine/STL/Math/Matrix3.h" "Engine/STL/Math/Matrix4.h" "Engine/STL/Math/MatrixCR.h" "Engine/STL/Math/MatrixUtils.h" "Engine/STL/Math/OverflowCheck.h" "Engine/STL/Math/Quaternion.h" "Engine/STL/Math/Trigonometry.h" "Engine/STL/Math/Vec.h" "Engine/STL/Math/VecI.h" )
source_group( "OS\\Base" FILES "Engine/STL/OS/Base/BaseFileSystem.cpp" "Engine/STL/OS/Base/BaseFileSystem.h" "Engine/STL/OS/Base/Common.h" "Engine/STL/OS/Base/ConditionVariableEmulation.h" "Engine/STL/OS/Base/Date.cpp" "Engine/STL/OS/Base/Date.h" "Engine/STL/OS/Base/Endianes.h" "Engine/STL/OS/Base/ReadWriteSyncEmulation.h" "Engine/STL/OS/Base/ScopeLock.h" "Engine/STL/OS/Base/SemaphoreEmulator.h" "Engine/STL/OS/Base/SyncEventEmulation.h" )
source_group( "OS\\SDL" FILES "Engine/STL/OS/SDL/FileSystem.h" "Engine/STL/OS/SDL/Library.h" "Engine/STL/OS/SDL/OS_SDL.h" "Engine/STL/OS/SDL/RandDevice.h" "Engine/STL/OS/SDL/SdlLibrary.cpp" "Engine/STL/OS/SDL/SdlSyncPrimitives.cpp" "Engine/STL/OS/SDL/SdlThread.cpp" "Engine/STL/OS/SDL/SDLUtils.cpp" "Engine/STL/OS/SDL/SDLUtils.h" "Engine/STL/OS/SDL/SyncPrimitives.h" "Engine/STL/OS/SDL/Thread.h" "Engine/STL/OS/SDL/Timer.h" )
source_group( "Defines" FILES "Engine/STL/Defines/AuxiliaryDefines.h" "Engine/STL/Defines/Defines.h" "Engine/STL/Defines/EnumHelpers.h" "Engine/STL/Defines/Errors.h" "Engine/STL/Defines/MemberDetector.h" "Engine/STL/Defines/OperatorHelpers.h" "Engine/STL/Defines/PublicMacro.h" )
source_group( "Algorithms\\Crypt" FILES "Engine/STL/Algorithms/Crypt/SimpleCrypt.h" )
source_group( "Common" FILES "Engine/STL/Common/AllFunc.h" "Engine/STL/Common/Cast.h" "Engine/STL/Common/Init.h" "Engine/STL/Common/Main.cpp" "Engine/STL/Common/Platforms.h" "Engine/STL/Common/TypeId.h" "Engine/STL/Common/Types.h" "Engine/STL/Common/UMax.h" "Engine/STL/Common/Uninitialized.h" )
source_group( "Math\\Color" FILES "Engine/STL/Math/Color/Color.h" "Engine/STL/Math/Color/ColorFormats.h" "Engine/STL/Math/Color/Half.h" "Engine/STL/Math/Color/TR11G11B10F.h" "Engine/STL/Math/Color/TRGB9_E5.h" )
source_group( "Files" FILES "Engine/STL/Files/BaseFile.h" "Engine/STL/Files/CryptFile.h" "Engine/STL/Files/HDDFile.h" "Engine/STL/Files/LzmaFile.h" "Engine/STL/Files/MemFile.h" "Engine/STL/Files/SubFile.h" "Engine/STL/Files/ZipFile.h" )
source_group( "Containers" FILES "Engine/STL/Containers/Adaptors.h" "Engine/STL/Containers/AppendableAdaptor.h" "Engine/STL/Containers/Array.h" "Engine/STL/Containers/ArrayRef.h" "Engine/STL/Containers/CircularQueue.h" "Engine/STL/Containers/CopyStrategy.h" "Engine/STL/Containers/ErasableAdaptor.h" "Engine/STL/Containers/HashMap.h" "Engine/STL/Containers/HashSet.h" "Engine/STL/Containers/IndexedArray.h" "Engine/STL/Containers/IndexedIterator.h" "Engine/STL/Containers/Map.h" "Engine/STL/Containers/MapUtils.h" "Engine/STL/Containers/Pair.h" "Engine/STL/Containers/Queue.h" "Engine/STL/Containers/Set.h" "Engine/STL/Containers/Stack.h" "Engine/STL/Containers/StaticArray.h" "Engine/STL/Containers/StaticBitArray.h" "Engine/STL/Containers/String.h" "Engine/STL/Containers/StringRef.h" "Engine/STL/Containers/Tuple.h" "Engine/STL/Containers/UniBuffer.h" )
source_group( "DataBase" FILES "Engine/STL/DataBase/SimpleDB.h" "Engine/STL/DataBase/Utf8StringUtils.cpp" "Engine/STL/DataBase/Utf8StringUtils.h" )
source_group( "Common\\Compilers" FILES "Engine/STL/Common/Compilers/CompilerClang.h" "Engine/STL/Common/Compilers/CompilerGCC.h" "Engine/STL/Common/Compilers/CompilerMSVC.h" )
source_group( "" FILES "Engine/STL/Engine.STL.h" )
source_group( "Compression" FILES "Engine/STL/Compression/Compression.h" "Engine/STL/Compression/LZ4Compression.h" "Engine/STL/Compression/MiniZCompression.h" )
source_group( "Memory" FILES "Engine/STL/Memory/Allocators.h" "Engine/STL/Memory/MemFunc.h" "Engine/STL/Memory/MemoryContainer.h" "Engine/STL/Memory/MemoryViewer.h" "Engine/STL/Memory/MemPointer.h" "Engine/STL/Memory/PlacementNew.h" )
source_group( "OS" FILES "Engine/STL/OS/OSLowLevel.h" )
source_group( "Algorithms\\Filters" FILES "Engine/STL/Algorithms/Filters/GaussianFilter.h" )
source_group( "CompileTime" FILES "Engine/STL/CompileTime/CompileTimeChecks.h" "Engine/STL/CompileTime/CompileTimeDebug.h" "Engine/STL/CompileTime/CompileTimeTypes.h" "Engine/STL/CompileTime/DeferredTemplates.h" "Engine/STL/CompileTime/FunctionInfo.h" "Engine/STL/CompileTime/Map.h" "Engine/STL/CompileTime/NearType.h" "Engine/STL/CompileTime/NewTypeInfo.h" "Engine/STL/CompileTime/OldTypeInfo.h" "Engine/STL/CompileTime/PlainOldDataType.h" "Engine/STL/CompileTime/Sequence.h" "Engine/STL/CompileTime/StaticFloat.h" "Engine/STL/CompileTime/StaticFractional.h" "Engine/STL/CompileTime/StaticInteger.h" "Engine/STL/CompileTime/StringToID.h" "Engine/STL/CompileTime/TemplateMath.h" "Engine/STL/CompileTime/TemplateUtils.h" "Engine/STL/CompileTime/TypeInfoFunc.h" "Engine/STL/CompileTime/TypeList.h" "Engine/STL/CompileTime/TypeListHelpers.h" "Engine/STL/CompileTime/TypeListUtils.h" "Engine/STL/CompileTime/TypeQualifier.h" "Engine/STL/CompileTime/TypeTraits.h" )
source_group( "Math\\3D" FILES "Engine/STL/Math/3D/AxisAlignedBox.h" "Engine/STL/Math/3D/CoordTransform3.h" "Engine/STL/Math/3D/Frustum.h" "Engine/STL/Math/3D/Line3.h" "Engine/STL/Math/3D/MathTypes3D.h" "Engine/STL/Math/3D/PerspectiveCamera.h" "Engine/STL/Math/3D/Plane.h" "Engine/STL/Math/3D/Transform.h" "Engine/STL/Math/3D/Triangle.h" )
set_property( TARGET "Engine.STL" PROPERTY FOLDER "Engine" )
target_include_directories( "Engine.STL" PRIVATE "Engine/STL" )
target_include_directories( "Engine.STL" PUBLIC "External" )
target_include_directories( "Engine.STL" PUBLIC "${EXTERNALS_PATH}" )
target_include_directories( "Engine.STL" PUBLIC "Engine/.." )
if (ENABLE_MINIZ)
	target_link_libraries( "Engine.STL" "MiniZ" )
endif()
if (ENABLE_SDL)
	target_link_libraries( "Engine.STL" "SDL2" )
endif()
if (ENABLE_LZ4)
	target_link_libraries( "Engine.STL" "LZ4" )
endif()
if (UNIX)
	target_link_libraries( "Engine.STL" "dl;pthread" )
endif()
# compiler
set_target_properties( "Engine.STL" PROPERTIES LINK_FLAGS_RELEASE ${PROJECTS_SHARED_LINKER_FLAGS_RELEASE} )
set_target_properties( "Engine.STL" PROPERTIES LINK_FLAGS_PROFILE ${PROJECTS_SHARED_LINKER_FLAGS_PROFILE} )
target_compile_definitions( "Engine.STL" PRIVATE $<$<CONFIG:Debug>: ${PROJECTS_SHARED_DEFINES_DEBUG}> )
set_target_properties( "Engine.STL" PROPERTIES LINK_FLAGS_DEBUGANALYZE ${PROJECTS_SHARED_LINKER_FLAGS_DEBUGANALYZE} )
target_compile_definitions( "Engine.STL" PRIVATE $<$<CONFIG:DebugAnalyze>: ${PROJECTS_SHARED_DEFINES_DEBUGANALYZE}> )
target_compile_options( "Engine.STL" PRIVATE $<$<CONFIG:Profile>: ${PROJECTS_SHARED_CXX_FLAGS_PROFILE}> )
target_compile_options( "Engine.STL" PRIVATE $<$<CONFIG:Release>: ${PROJECTS_SHARED_CXX_FLAGS_RELEASE}> )
target_compile_definitions( "Engine.STL" PRIVATE $<$<CONFIG:Profile>: ${PROJECTS_SHARED_DEFINES_PROFILE}> )
target_compile_definitions( "Engine.STL" PRIVATE $<$<CONFIG:Release>: ${PROJECTS_SHARED_DEFINES_RELEASE}> )
set_target_properties( "Engine.STL" PROPERTIES LINK_FLAGS_DEBUG ${PROJECTS_SHARED_LINKER_FLAGS_DEBUG} )
target_compile_options( "Engine.STL" PRIVATE $<$<CONFIG:DebugAnalyze>: ${PROJECTS_SHARED_CXX_FLAGS_DEBUGANALYZE}> )
target_compile_options( "Engine.STL" PRIVATE $<$<CONFIG:Debug>: ${PROJECTS_SHARED_CXX_FLAGS_DEBUG}> )



#==================================================================================================
# project: Tests.STL
#==================================================================================================
set( SOURCES 
	"Tests/STL/Common.h"
	"Tests/STL/Debug.h"
	"Tests/STL/Main.cpp"
	"Tests/STL/Test_Algorithms_InvokeWithVariant.cpp"
	"Tests/STL/Test_Algorithms_Range.cpp"
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
	"Tests/STL/Test_Containers_Tuple.cpp"
	"Tests/STL/Test_Math_Abs.cpp"
	"Tests/STL/Test_Math_Bit.cpp"
	"Tests/STL/Test_Math_Clamp_Wrap.cpp"
	"Tests/STL/Test_Math_Color.cpp"
	"Tests/STL/Test_Math_ColorFormat.cpp"
	"Tests/STL/Test_Math_Factorial.cpp"
	"Tests/STL/Test_Math_FloorCeilTruncRoundFract.cpp"
	"Tests/STL/Test_Math_Frustum.cpp"
	"Tests/STL/Test_Math_ImageUtils.cpp"
	"Tests/STL/Test_Math_Matrix.cpp"
	"Tests/STL/Test_Math_OverflowCheck.cpp"
	"Tests/STL/Test_Math_Plane.cpp"
	"Tests/STL/Test_Math_Transform.cpp"
	"Tests/STL/Test_OS_Atomic.cpp"
	"Tests/STL/Test_OS_Date.cpp"
	"Tests/STL/Test_OS_FileSystem.cpp"
	"Tests/STL/Test_Runtime_VirtualTypelist.cpp"
	"Tests/STL/Test_Temp.cpp"
	"Tests/STL/Test_Types_Cast.cpp"
	"Tests/STL/Test_Types_FileAddress.cpp"
	"Tests/STL/Test_Types_Function.cpp"
	"Tests/STL/Test_Types_StringParser.cpp"
	"Tests/STL/Test_Types_Time.cpp"
	"Tests/STL/Test_Types_Union.cpp"
	"Tests/STL/Test_Type_Optional.cpp" )
if (DEFINED ANDROID)
	add_library( "Tests.STL" SHARED ${SOURCES} )
else()
	add_executable( "Tests.STL" ${SOURCES} )
endif()
source_group( "" FILES "Tests/STL/Common.h" "Tests/STL/Debug.h" "Tests/STL/Main.cpp" "Tests/STL/Test_Algorithms_InvokeWithVariant.cpp" "Tests/STL/Test_Algorithms_Range.cpp" "Tests/STL/Test_CompileTime_MainType.cpp" "Tests/STL/Test_CompileTime_Map.cpp" "Tests/STL/Test_CompileTime_Sequence.cpp" "Tests/STL/Test_CompileTime_StaticFloat.cpp" "Tests/STL/Test_CompileTime_StringToID.cpp" "Tests/STL/Test_CompileTime_TemplateMath.cpp" "Tests/STL/Test_CompileTime_TypeInfo.cpp" "Tests/STL/Test_CompileTime_TypeList.cpp" "Tests/STL/Test_CompileTime_TypeQualifier.cpp" "Tests/STL/Test_CompileTime_TypeTraits.cpp" "Tests/STL/Test_Containers_Adaptors.cpp" "Tests/STL/Test_Containers_Array.cpp" "Tests/STL/Test_Containers_CircularQueue.cpp" "Tests/STL/Test_Containers_HashSet.cpp" "Tests/STL/Test_Containers_IndexedArray.cpp" "Tests/STL/Test_Containers_List.cpp" "Tests/STL/Test_Containers_Map.cpp" "Tests/STL/Test_Containers_Queue.cpp" "Tests/STL/Test_Containers_Set.cpp" "Tests/STL/Test_Containers_String.cpp" "Tests/STL/Test_Containers_Tuple.cpp" "Tests/STL/Test_Math_Abs.cpp" "Tests/STL/Test_Math_Bit.cpp" "Tests/STL/Test_Math_Clamp_Wrap.cpp" "Tests/STL/Test_Math_Color.cpp" "Tests/STL/Test_Math_ColorFormat.cpp" "Tests/STL/Test_Math_Factorial.cpp" "Tests/STL/Test_Math_FloorCeilTruncRoundFract.cpp" "Tests/STL/Test_Math_Frustum.cpp" "Tests/STL/Test_Math_ImageUtils.cpp" "Tests/STL/Test_Math_Matrix.cpp" "Tests/STL/Test_Math_OverflowCheck.cpp" "Tests/STL/Test_Math_Plane.cpp" "Tests/STL/Test_Math_Transform.cpp" "Tests/STL/Test_OS_Atomic.cpp" "Tests/STL/Test_OS_Date.cpp" "Tests/STL/Test_OS_FileSystem.cpp" "Tests/STL/Test_Runtime_VirtualTypelist.cpp" "Tests/STL/Test_Temp.cpp" "Tests/STL/Test_Types_Cast.cpp" "Tests/STL/Test_Types_FileAddress.cpp" "Tests/STL/Test_Types_Function.cpp" "Tests/STL/Test_Types_StringParser.cpp" "Tests/STL/Test_Types_Time.cpp" "Tests/STL/Test_Types_Union.cpp" "Tests/STL/Test_Type_Optional.cpp" )
set_property( TARGET "Tests.STL" PROPERTY FOLDER "Tests" )
target_include_directories( "Tests.STL" PRIVATE "Tests/STL" )
target_include_directories( "Tests.STL" PUBLIC "External" )
target_include_directories( "Tests.STL" PUBLIC "${EXTERNALS_PATH}" )
target_include_directories( "Tests.STL" PUBLIC "Engine/.." )
target_link_libraries( "Tests.STL" "Engine.STL" )
# compiler
set_target_properties( "Tests.STL" PROPERTIES LINK_FLAGS_RELEASE ${PROJECTS_SHARED_LINKER_FLAGS_RELEASE} )
set_target_properties( "Tests.STL" PROPERTIES LINK_FLAGS_PROFILE ${PROJECTS_SHARED_LINKER_FLAGS_PROFILE} )
target_compile_definitions( "Tests.STL" PRIVATE $<$<CONFIG:Debug>: ${PROJECTS_SHARED_DEFINES_DEBUG}> )
set_target_properties( "Tests.STL" PROPERTIES LINK_FLAGS_DEBUGANALYZE ${PROJECTS_SHARED_LINKER_FLAGS_DEBUGANALYZE} )
target_compile_definitions( "Tests.STL" PRIVATE $<$<CONFIG:DebugAnalyze>: ${PROJECTS_SHARED_DEFINES_DEBUGANALYZE}> )
target_compile_options( "Tests.STL" PRIVATE $<$<CONFIG:Profile>: ${PROJECTS_SHARED_CXX_FLAGS_PROFILE}> )
target_compile_options( "Tests.STL" PRIVATE $<$<CONFIG:Release>: ${PROJECTS_SHARED_CXX_FLAGS_RELEASE}> )
target_compile_definitions( "Tests.STL" PRIVATE $<$<CONFIG:Profile>: ${PROJECTS_SHARED_DEFINES_PROFILE}> )
target_compile_definitions( "Tests.STL" PRIVATE $<$<CONFIG:Release>: ${PROJECTS_SHARED_DEFINES_RELEASE}> )
set_target_properties( "Tests.STL" PROPERTIES LINK_FLAGS_DEBUG ${PROJECTS_SHARED_LINKER_FLAGS_DEBUG} )
target_compile_options( "Tests.STL" PRIVATE $<$<CONFIG:DebugAnalyze>: ${PROJECTS_SHARED_CXX_FLAGS_DEBUGANALYZE}> )
target_compile_options( "Tests.STL" PRIVATE $<$<CONFIG:Debug>: ${PROJECTS_SHARED_CXX_FLAGS_DEBUG}> )



#==================================================================================================
# project: Engine.Physics
#==================================================================================================
set( SOURCES 
	"Engine/Physics/Algorithms/AcceleratedMotion.h"
	"Engine/Physics/Algorithms/Astrophysics.h"
	"Engine/Physics/Algorithms/Astrophysics_Orbit.h"
	"Engine/Physics/Dimensions/AstrophysicsConstants.h"
	"Engine/Physics/Dimensions/DefaultTypes.h"
	"Engine/Physics/Dimensions/PhysicsConstants.h"
	"Engine/Physics/Dimensions/PhysicsDimension.h"
	"Engine/Physics/Dimensions/PhysicsValue.h"
	"Engine/Physics/Dimensions/PhysicsValueMath.h"
	"Engine/Physics/Dimensions/PhysicsValueUtils.h"
	"Engine/Physics/Dimensions/PhysicsValueVec.h"
	"Engine/Physics/Dimensions/PhysicsValueVecI.h"
	"Engine/Physics/Common/Common.h"
	"Engine/Physics/Common/Main.cpp"
	"Engine/Physics/Engine.Physics.h"
	"Engine/Physics/CompileTime/GenType.h"
	"Engine/Physics/CompileTime/PhysTypeInfo.h" )
add_library( "Engine.Physics" STATIC ${SOURCES} )
source_group( "Algorithms" FILES "Engine/Physics/Algorithms/AcceleratedMotion.h" "Engine/Physics/Algorithms/Astrophysics.h" "Engine/Physics/Algorithms/Astrophysics_Orbit.h" )
source_group( "Dimensions" FILES "Engine/Physics/Dimensions/AstrophysicsConstants.h" "Engine/Physics/Dimensions/DefaultTypes.h" "Engine/Physics/Dimensions/PhysicsConstants.h" "Engine/Physics/Dimensions/PhysicsDimension.h" "Engine/Physics/Dimensions/PhysicsValue.h" "Engine/Physics/Dimensions/PhysicsValueMath.h" "Engine/Physics/Dimensions/PhysicsValueUtils.h" "Engine/Physics/Dimensions/PhysicsValueVec.h" "Engine/Physics/Dimensions/PhysicsValueVecI.h" )
source_group( "Common" FILES "Engine/Physics/Common/Common.h" "Engine/Physics/Common/Main.cpp" )
source_group( "" FILES "Engine/Physics/Engine.Physics.h" )
source_group( "CompileTime" FILES "Engine/Physics/CompileTime/GenType.h" "Engine/Physics/CompileTime/PhysTypeInfo.h" )
set_property( TARGET "Engine.Physics" PROPERTY FOLDER "Engine" )
target_include_directories( "Engine.Physics" PRIVATE "Engine/Physics" )
target_include_directories( "Engine.Physics" PUBLIC "External" )
target_include_directories( "Engine.Physics" PUBLIC "${EXTERNALS_PATH}" )
target_include_directories( "Engine.Physics" PUBLIC "Engine/.." )
target_link_libraries( "Engine.Physics" "Engine.STL" )
# compiler
set_target_properties( "Engine.Physics" PROPERTIES LINK_FLAGS_RELEASE ${PROJECTS_SHARED_LINKER_FLAGS_RELEASE} )
set_target_properties( "Engine.Physics" PROPERTIES LINK_FLAGS_PROFILE ${PROJECTS_SHARED_LINKER_FLAGS_PROFILE} )
target_compile_definitions( "Engine.Physics" PRIVATE $<$<CONFIG:Debug>: ${PROJECTS_SHARED_DEFINES_DEBUG}> )
set_target_properties( "Engine.Physics" PROPERTIES LINK_FLAGS_DEBUGANALYZE ${PROJECTS_SHARED_LINKER_FLAGS_DEBUGANALYZE} )
target_compile_definitions( "Engine.Physics" PRIVATE $<$<CONFIG:DebugAnalyze>: ${PROJECTS_SHARED_DEFINES_DEBUGANALYZE}> )
target_compile_options( "Engine.Physics" PRIVATE $<$<CONFIG:Profile>: ${PROJECTS_SHARED_CXX_FLAGS_PROFILE}> )
target_compile_options( "Engine.Physics" PRIVATE $<$<CONFIG:Release>: ${PROJECTS_SHARED_CXX_FLAGS_RELEASE}> )
target_compile_definitions( "Engine.Physics" PRIVATE $<$<CONFIG:Profile>: ${PROJECTS_SHARED_DEFINES_PROFILE}> )
target_compile_definitions( "Engine.Physics" PRIVATE $<$<CONFIG:Release>: ${PROJECTS_SHARED_DEFINES_RELEASE}> )
set_target_properties( "Engine.Physics" PROPERTIES LINK_FLAGS_DEBUG ${PROJECTS_SHARED_LINKER_FLAGS_DEBUG} )
target_compile_options( "Engine.Physics" PRIVATE $<$<CONFIG:DebugAnalyze>: ${PROJECTS_SHARED_CXX_FLAGS_DEBUGANALYZE}> )
target_compile_options( "Engine.Physics" PRIVATE $<$<CONFIG:Debug>: ${PROJECTS_SHARED_CXX_FLAGS_DEBUG}> )



#==================================================================================================
# project: Tests.Physics
#==================================================================================================
set( SOURCES 
	"Tests/Physics/Common.h"
	"Tests/Physics/Main.cpp"
	"Tests/Physics/Test_Algorithms_Orbit.cpp"
	"Tests/Physics/Test_Dimensions_PhysicsValue.cpp" )
if (DEFINED ANDROID)
	add_library( "Tests.Physics" SHARED ${SOURCES} )
else()
	add_executable( "Tests.Physics" ${SOURCES} )
endif()
source_group( "" FILES "Tests/Physics/Common.h" "Tests/Physics/Main.cpp" "Tests/Physics/Test_Algorithms_Orbit.cpp" "Tests/Physics/Test_Dimensions_PhysicsValue.cpp" )
set_property( TARGET "Tests.Physics" PROPERTY FOLDER "Tests" )
target_include_directories( "Tests.Physics" PRIVATE "Tests/Physics" )
target_include_directories( "Tests.Physics" PUBLIC "External" )
target_include_directories( "Tests.Physics" PUBLIC "${EXTERNALS_PATH}" )
target_include_directories( "Tests.Physics" PUBLIC "Engine/.." )
target_link_libraries( "Tests.Physics" "Engine.Physics" )
# compiler
set_target_properties( "Tests.Physics" PROPERTIES LINK_FLAGS_RELEASE ${PROJECTS_SHARED_LINKER_FLAGS_RELEASE} )
set_target_properties( "Tests.Physics" PROPERTIES LINK_FLAGS_PROFILE ${PROJECTS_SHARED_LINKER_FLAGS_PROFILE} )
target_compile_definitions( "Tests.Physics" PRIVATE $<$<CONFIG:Debug>: ${PROJECTS_SHARED_DEFINES_DEBUG}> )
set_target_properties( "Tests.Physics" PROPERTIES LINK_FLAGS_DEBUGANALYZE ${PROJECTS_SHARED_LINKER_FLAGS_DEBUGANALYZE} )
target_compile_definitions( "Tests.Physics" PRIVATE $<$<CONFIG:DebugAnalyze>: ${PROJECTS_SHARED_DEFINES_DEBUGANALYZE}> )
target_compile_options( "Tests.Physics" PRIVATE $<$<CONFIG:Profile>: ${PROJECTS_SHARED_CXX_FLAGS_PROFILE}> )
target_compile_options( "Tests.Physics" PRIVATE $<$<CONFIG:Release>: ${PROJECTS_SHARED_CXX_FLAGS_RELEASE}> )
target_compile_definitions( "Tests.Physics" PRIVATE $<$<CONFIG:Profile>: ${PROJECTS_SHARED_DEFINES_PROFILE}> )
target_compile_definitions( "Tests.Physics" PRIVATE $<$<CONFIG:Release>: ${PROJECTS_SHARED_DEFINES_RELEASE}> )
set_target_properties( "Tests.Physics" PROPERTIES LINK_FLAGS_DEBUG ${PROJECTS_SHARED_LINKER_FLAGS_DEBUG} )
target_compile_options( "Tests.Physics" PRIVATE $<$<CONFIG:DebugAnalyze>: ${PROJECTS_SHARED_CXX_FLAGS_DEBUGANALYZE}> )
target_compile_options( "Tests.Physics" PRIVATE $<$<CONFIG:Debug>: ${PROJECTS_SHARED_CXX_FLAGS_DEBUG}> )



#==================================================================================================
# project: Engine.Base
#==================================================================================================
set( SOURCES 
	"Engine/Base/Tasks/AsyncTask.h"
	"Engine/Base/Tasks/TaskManager.cpp"
	"Engine/Base/Tasks/TaskManager.h"
	"Engine/Base/Tasks/TaskModule.cpp"
	"Engine/Base/DataProvider/BuiltinStorageDataProvider.cpp"
	"Engine/Base/DataProvider/DataMessages.h"
	"Engine/Base/DataProvider/DataProviderManager.cpp"
	"Engine/Base/DataProvider/DataProviderObjectsConstructor.cpp"
	"Engine/Base/DataProvider/DataProviderObjectsConstructor.h"
	"Engine/Base/DataProvider/FileDataInput.cpp"
	"Engine/Base/DataProvider/FileDataOutput.cpp"
	"Engine/Base/DataProvider/FileInputStream.cpp"
	"Engine/Base/DataProvider/FileOutputStream.cpp"
	"Engine/Base/DataProvider/InMemoryDataProvider.cpp"
	"Engine/Base/DataProvider/InternetDataProvider.cpp"
	"Engine/Base/DataProvider/LocalStorageDataProvider.cpp"
	"Engine/Base/Main/MainSystem.cpp"
	"Engine/Base/Main/MainSystem.h"
	"Engine/Base/Modules/Message.h"
	"Engine/Base/Modules/MessageCache.h"
	"Engine/Base/Modules/MessageHandler.cpp"
	"Engine/Base/Modules/MessageHandler.h"
	"Engine/Base/Modules/MessageHelpers.h"
	"Engine/Base/Modules/Module.cpp"
	"Engine/Base/Modules/Module.h"
	"Engine/Base/Modules/Module.inl.h"
	"Engine/Base/Modules/Module.Send.inl.h"
	"Engine/Base/Modules/ModuleAsyncTasks.h"
	"Engine/Base/Modules/ModulesFactory.cpp"
	"Engine/Base/Modules/ModulesFactory.h"
	"Engine/Base/Modules/ModuleUtils.h"
	"Engine/Base/Common/BaseObject.h"
	"Engine/Base/Common/Common.h"
	"Engine/Base/Common/Defines.h"
	"Engine/Base/Common/EModuleGroup.h"
	"Engine/Base/Common/EngineSubSystems.h"
	"Engine/Base/Common/Enums.h"
	"Engine/Base/Common/IDs.h"
	"Engine/Base/Common/ThreadID.h"
	"Engine/Base/Engine.Base.h"
	"Engine/Base/Public/AsyncMessage.h"
	"Engine/Base/Public/CreateInfo.h"
	"Engine/Base/Public/DataProvider.h"
	"Engine/Base/Public/ModuleMessages.h"
	"Engine/Base/Public/ParallelThread.h"
	"Engine/Base/Public/ProfilingMessages.h"
	"Engine/Base/Public/TaskModule.h"
	"Engine/Base/Threads/ParallelThreadImpl.cpp"
	"Engine/Base/Threads/ParallelThreadImpl.h"
	"Engine/Base/Threads/ThreadManager.cpp"
	"Engine/Base/Threads/ThreadManager.h" )
add_library( "Engine.Base" STATIC ${SOURCES} )
source_group( "Tasks" FILES "Engine/Base/Tasks/AsyncTask.h" "Engine/Base/Tasks/TaskManager.cpp" "Engine/Base/Tasks/TaskManager.h" "Engine/Base/Tasks/TaskModule.cpp" )
source_group( "DataProvider" FILES "Engine/Base/DataProvider/BuiltinStorageDataProvider.cpp" "Engine/Base/DataProvider/DataMessages.h" "Engine/Base/DataProvider/DataProviderManager.cpp" "Engine/Base/DataProvider/DataProviderObjectsConstructor.cpp" "Engine/Base/DataProvider/DataProviderObjectsConstructor.h" "Engine/Base/DataProvider/FileDataInput.cpp" "Engine/Base/DataProvider/FileDataOutput.cpp" "Engine/Base/DataProvider/FileInputStream.cpp" "Engine/Base/DataProvider/FileOutputStream.cpp" "Engine/Base/DataProvider/InMemoryDataProvider.cpp" "Engine/Base/DataProvider/InternetDataProvider.cpp" "Engine/Base/DataProvider/LocalStorageDataProvider.cpp" )
source_group( "Main" FILES "Engine/Base/Main/MainSystem.cpp" "Engine/Base/Main/MainSystem.h" )
source_group( "Modules" FILES "Engine/Base/Modules/Message.h" "Engine/Base/Modules/MessageCache.h" "Engine/Base/Modules/MessageHandler.cpp" "Engine/Base/Modules/MessageHandler.h" "Engine/Base/Modules/MessageHelpers.h" "Engine/Base/Modules/Module.cpp" "Engine/Base/Modules/Module.h" "Engine/Base/Modules/Module.inl.h" "Engine/Base/Modules/Module.Send.inl.h" "Engine/Base/Modules/ModuleAsyncTasks.h" "Engine/Base/Modules/ModulesFactory.cpp" "Engine/Base/Modules/ModulesFactory.h" "Engine/Base/Modules/ModuleUtils.h" )
source_group( "Common" FILES "Engine/Base/Common/BaseObject.h" "Engine/Base/Common/Common.h" "Engine/Base/Common/Defines.h" "Engine/Base/Common/EModuleGroup.h" "Engine/Base/Common/EngineSubSystems.h" "Engine/Base/Common/Enums.h" "Engine/Base/Common/IDs.h" "Engine/Base/Common/ThreadID.h" )
source_group( "" FILES "Engine/Base/Engine.Base.h" )
source_group( "Public" FILES "Engine/Base/Public/AsyncMessage.h" "Engine/Base/Public/CreateInfo.h" "Engine/Base/Public/DataProvider.h" "Engine/Base/Public/ModuleMessages.h" "Engine/Base/Public/ParallelThread.h" "Engine/Base/Public/ProfilingMessages.h" "Engine/Base/Public/TaskModule.h" )
source_group( "Threads" FILES "Engine/Base/Threads/ParallelThreadImpl.cpp" "Engine/Base/Threads/ParallelThreadImpl.h" "Engine/Base/Threads/ThreadManager.cpp" "Engine/Base/Threads/ThreadManager.h" )
set_property( TARGET "Engine.Base" PROPERTY FOLDER "Engine" )
target_include_directories( "Engine.Base" PRIVATE "Engine/Base" )
target_include_directories( "Engine.Base" PUBLIC "External" )
target_include_directories( "Engine.Base" PUBLIC "${EXTERNALS_PATH}" )
target_include_directories( "Engine.Base" PUBLIC "Engine/.." )
target_link_libraries( "Engine.Base" "Engine.STL" )
# compiler
set_target_properties( "Engine.Base" PROPERTIES LINK_FLAGS_RELEASE ${PROJECTS_SHARED_LINKER_FLAGS_RELEASE} )
set_target_properties( "Engine.Base" PROPERTIES LINK_FLAGS_PROFILE ${PROJECTS_SHARED_LINKER_FLAGS_PROFILE} )
target_compile_definitions( "Engine.Base" PRIVATE $<$<CONFIG:Debug>: ${PROJECTS_SHARED_DEFINES_DEBUG}> )
set_target_properties( "Engine.Base" PROPERTIES LINK_FLAGS_DEBUGANALYZE ${PROJECTS_SHARED_LINKER_FLAGS_DEBUGANALYZE} )
target_compile_definitions( "Engine.Base" PRIVATE $<$<CONFIG:DebugAnalyze>: ${PROJECTS_SHARED_DEFINES_DEBUGANALYZE}> )
target_compile_options( "Engine.Base" PRIVATE $<$<CONFIG:Profile>: ${PROJECTS_SHARED_CXX_FLAGS_PROFILE}> )
target_compile_options( "Engine.Base" PRIVATE $<$<CONFIG:Release>: ${PROJECTS_SHARED_CXX_FLAGS_RELEASE}> )
target_compile_definitions( "Engine.Base" PRIVATE $<$<CONFIG:Profile>: ${PROJECTS_SHARED_DEFINES_PROFILE}> )
target_compile_definitions( "Engine.Base" PRIVATE $<$<CONFIG:Release>: ${PROJECTS_SHARED_DEFINES_RELEASE}> )
set_target_properties( "Engine.Base" PROPERTIES LINK_FLAGS_DEBUG ${PROJECTS_SHARED_LINKER_FLAGS_DEBUG} )
target_compile_options( "Engine.Base" PRIVATE $<$<CONFIG:DebugAnalyze>: ${PROJECTS_SHARED_CXX_FLAGS_DEBUGANALYZE}> )
target_compile_options( "Engine.Base" PRIVATE $<$<CONFIG:Debug>: ${PROJECTS_SHARED_CXX_FLAGS_DEBUG}> )



#==================================================================================================
# project: Engine.Platforms
#==================================================================================================
set( SOURCES 
	"Engine/Platforms/Vulkan/Android/VkAndSurface.cpp"
	"Engine/Platforms/Vulkan/Android/VkAndSurface.h"
	"Engine/Platforms/Soft/SoftRendererContext.cpp"
	"Engine/Platforms/Soft/SoftRendererObjectsConstructor.h"
	"Engine/Platforms/Soft/SoftRendererThread.cpp"
	"Engine/Platforms/SDL/SDLDisplay.cpp"
	"Engine/Platforms/SDL/SDLDisplay.h"
	"Engine/Platforms/SDL/SDLKeyInput.cpp"
	"Engine/Platforms/SDL/SDLMessages.h"
	"Engine/Platforms/SDL/SDLMouseInput.cpp"
	"Engine/Platforms/SDL/SDLObjectsConstructor.h"
	"Engine/Platforms/SDL/SDLPlatform.cpp"
	"Engine/Platforms/SDL/SDLWindow.cpp"
	"Engine/Platforms/OpenGL/OpenGLContext.cpp"
	"Engine/Platforms/OpenGL/OpenGLObjectsConstructor.h"
	"Engine/Platforms/OpenGL/OpenGLThread.cpp"
	"Engine/Platforms/OpenCL/120/CL1BaseModule.cpp"
	"Engine/Platforms/OpenCL/120/CL1BaseModule.h"
	"Engine/Platforms/OpenCL/120/CL1Buffer.cpp"
	"Engine/Platforms/OpenCL/120/CL1CommandBuffer.cpp"
	"Engine/Platforms/OpenCL/120/CL1CommandBuilder.cpp"
	"Engine/Platforms/OpenCL/120/CL1CommandQueue.cpp"
	"Engine/Platforms/OpenCL/120/CL1Device.cpp"
	"Engine/Platforms/OpenCL/120/CL1Device.h"
	"Engine/Platforms/OpenCL/120/CL1Emulator.cpp"
	"Engine/Platforms/OpenCL/120/CL1Emulator.h"
	"Engine/Platforms/OpenCL/120/CL1Enums.h"
	"Engine/Platforms/OpenCL/120/CL1Image.cpp"
	"Engine/Platforms/OpenCL/120/CL1Messages.h"
	"Engine/Platforms/OpenCL/120/CL1Pipeline.cpp"
	"Engine/Platforms/OpenCL/120/CL1PipelineResourceTable.cpp"
	"Engine/Platforms/OpenCL/120/CL1ResourceCache.h"
	"Engine/Platforms/OpenCL/120/CL1Sampler.cpp"
	"Engine/Platforms/OpenCL/120/CL1SamplerCache.h"
	"Engine/Platforms/OpenCL/120/CL1SyncManager.cpp"
	"Engine/Platforms/OpenCL/120/cl2.cpp"
	"Engine/Platforms/OpenCL/120/cl2.h"
	"Engine/Platforms/Vulkan/110/Vk1BaseModule.cpp"
	"Engine/Platforms/Vulkan/110/Vk1BaseModule.h"
	"Engine/Platforms/Vulkan/110/Vk1BaseObject.h"
	"Engine/Platforms/Vulkan/110/Vk1Buffer.cpp"
	"Engine/Platforms/Vulkan/110/Vk1CommandBuffer.cpp"
	"Engine/Platforms/Vulkan/110/Vk1CommandBuilder.cpp"
	"Engine/Platforms/Vulkan/110/Vk1CommandQueue.cpp"
	"Engine/Platforms/Vulkan/110/Vk1Device.cpp"
	"Engine/Platforms/Vulkan/110/Vk1Device.h"
	"Engine/Platforms/Vulkan/110/Vk1Enums.h"
	"Engine/Platforms/Vulkan/110/Vk1Framebuffer.cpp"
	"Engine/Platforms/Vulkan/110/Vk1Image.cpp"
	"Engine/Platforms/Vulkan/110/Vk1Library.h"
	"Engine/Platforms/Vulkan/110/Vk1ManagedMemory.cpp"
	"Engine/Platforms/Vulkan/110/Vk1MemoryManager.cpp"
	"Engine/Platforms/Vulkan/110/Vk1Messages.h"
	"Engine/Platforms/Vulkan/110/Vk1Pipeline.cpp"
	"Engine/Platforms/Vulkan/110/Vk1PipelineCache.cpp"
	"Engine/Platforms/Vulkan/110/Vk1PipelineCache.h"
	"Engine/Platforms/Vulkan/110/Vk1PipelineLayout.cpp"
	"Engine/Platforms/Vulkan/110/Vk1PipelineLayout.h"
	"Engine/Platforms/Vulkan/110/Vk1PipelineResourceTable.cpp"
	"Engine/Platforms/Vulkan/110/Vk1RenderPass.cpp"
	"Engine/Platforms/Vulkan/110/Vk1RenderPass.h"
	"Engine/Platforms/Vulkan/110/Vk1ResourceCache.h"
	"Engine/Platforms/Vulkan/110/Vk1Sampler.cpp"
	"Engine/Platforms/Vulkan/110/Vk1SamplerCache.h"
	"Engine/Platforms/Vulkan/110/Vk1SwapchainImage.h"
	"Engine/Platforms/Vulkan/110/Vk1SyncManager.cpp"
	"Engine/Platforms/Vulkan/110/vulkan1.cpp"
	"Engine/Platforms/Vulkan/110/vulkan1.h"
	"Engine/Platforms/Vulkan/110/vulkan1_platform.cpp"
	"Engine/Platforms/Vulkan/110/vulkan1_platform.h"
	"Engine/Platforms/Vulkan/110/vulkan1_utils.h"
	"Engine/Platforms/OpenGL/300es/gles3.cpp"
	"Engine/Platforms/OpenGL/300es/gles3.h"
	"Engine/Platforms/OpenCL/OpenCLContext.cpp"
	"Engine/Platforms/OpenCL/OpenCLObjectsConstructor.h"
	"Engine/Platforms/OpenCL/OpenCLThread.cpp"
	"Engine/Platforms/Input/InputManager.cpp"
	"Engine/Platforms/Input/InputManager.h"
	"Engine/Platforms/Input/InputThread.cpp"
	"Engine/Platforms/Soft/ShaderLang/gen_Image.h"
	"Engine/Platforms/Soft/ShaderLang/gen_Texture.h"
	"Engine/Platforms/Soft/ShaderLang/SWLang.h"
	"Engine/Platforms/Soft/ShaderLang/SWLangArray.h"
	"Engine/Platforms/Soft/ShaderLang/SWLangBarrier.h"
	"Engine/Platforms/Soft/ShaderLang/SWLangBuffer.h"
	"Engine/Platforms/Soft/ShaderLang/SWLangCommon.h"
	"Engine/Platforms/Soft/ShaderLang/SWLangGLM.h"
	"Engine/Platforms/Soft/ShaderLang/SWLangImage.cpp"
	"Engine/Platforms/Soft/ShaderLang/SWLangImage.h"
	"Engine/Platforms/Soft/ShaderLang/SWLangShared.h"
	"Engine/Platforms/Soft/ShaderLang/SWLangTexture.cpp"
	"Engine/Platforms/Soft/ShaderLang/SWLangTexture.h"
	"Engine/Platforms/Soft/ShaderLang/SWLangTexture2D.h"
	"Engine/Platforms/Soft/ShaderLang/SWShaderHelper.h"
	"Engine/Platforms/Vulkan/VulkanContext.cpp"
	"Engine/Platforms/Vulkan/VulkanObjectsConstructor.h"
	"Engine/Platforms/Vulkan/VulkanThread.cpp"
	"Engine/Platforms/Soft/Impl/SWBaseModule.cpp"
	"Engine/Platforms/Soft/Impl/SWBaseModule.h"
	"Engine/Platforms/Soft/Impl/SWBuffer.cpp"
	"Engine/Platforms/Soft/Impl/SWCommandBuffer.cpp"
	"Engine/Platforms/Soft/Impl/SWCommandBuilder.cpp"
	"Engine/Platforms/Soft/Impl/SWCommandQueue.cpp"
	"Engine/Platforms/Soft/Impl/SWDevice.cpp"
	"Engine/Platforms/Soft/Impl/SWDevice.h"
	"Engine/Platforms/Soft/Impl/SWDeviceProperties.h"
	"Engine/Platforms/Soft/Impl/SWEnums.h"
	"Engine/Platforms/Soft/Impl/SWImage.cpp"
	"Engine/Platforms/Soft/Impl/SWMemory.cpp"
	"Engine/Platforms/Soft/Impl/SWMessages.h"
	"Engine/Platforms/Soft/Impl/SWPipeline.cpp"
	"Engine/Platforms/Soft/Impl/SWPipelineResourceTable.cpp"
	"Engine/Platforms/Soft/Impl/SWSampler.cpp"
	"Engine/Platforms/Soft/Impl/SWSamplerCache.h"
	"Engine/Platforms/Soft/Impl/SWShaderModel.cpp"
	"Engine/Platforms/Soft/Impl/SWShaderModel.h"
	"Engine/Platforms/Soft/Impl/SWSyncManager.cpp"
	"Engine/Platforms/Soft/Impl/SWSyncObjects.h"
	"Engine/Platforms/Windows/WinDisplay.cpp"
	"Engine/Platforms/Windows/WinDisplay.h"
	"Engine/Platforms/Windows/WinKeyInput.cpp"
	"Engine/Platforms/Windows/WinMessages.h"
	"Engine/Platforms/Windows/WinMouseInput.cpp"
	"Engine/Platforms/Windows/WinObjectsConstructor.h"
	"Engine/Platforms/Windows/WinPlatform.cpp"
	"Engine/Platforms/Windows/WinWindow.cpp"
	"Engine/Platforms/Public/GPU/Buffer.h"
	"Engine/Platforms/Public/GPU/BufferEnums.h"
	"Engine/Platforms/Public/GPU/CommandBuffer.h"
	"Engine/Platforms/Public/GPU/CommandEnums.h"
	"Engine/Platforms/Public/GPU/Context.cpp"
	"Engine/Platforms/Public/GPU/Context.h"
	"Engine/Platforms/Public/GPU/Enums.ToString.h"
	"Engine/Platforms/Public/GPU/FragmentOutputState.h"
	"Engine/Platforms/Public/GPU/Framebuffer.h"
	"Engine/Platforms/Public/GPU/IDs.h"
	"Engine/Platforms/Public/GPU/Image.cpp"
	"Engine/Platforms/Public/GPU/Image.h"
	"Engine/Platforms/Public/GPU/ImageEnums.h"
	"Engine/Platforms/Public/GPU/ImageLayer.h"
	"Engine/Platforms/Public/GPU/ImageSwizzle.h"
	"Engine/Platforms/Public/GPU/Memory.h"
	"Engine/Platforms/Public/GPU/MemoryEnums.h"
	"Engine/Platforms/Public/GPU/MipmapLevel.h"
	"Engine/Platforms/Public/GPU/MultiSamples.h"
	"Engine/Platforms/Public/GPU/ObjectEnums.h"
	"Engine/Platforms/Public/GPU/Pipeline.cpp"
	"Engine/Platforms/Public/GPU/Pipeline.h"
	"Engine/Platforms/Public/GPU/PipelineLayout.cpp"
	"Engine/Platforms/Public/GPU/PipelineLayout.h"
	"Engine/Platforms/Public/GPU/PixelFormatEnums.h"
	"Engine/Platforms/Public/GPU/RenderPass.cpp"
	"Engine/Platforms/Public/GPU/RenderPass.h"
	"Engine/Platforms/Public/GPU/RenderPassEnums.h"
	"Engine/Platforms/Public/GPU/RenderState.cpp"
	"Engine/Platforms/Public/GPU/RenderState.h"
	"Engine/Platforms/Public/GPU/RenderStateEnums.h"
	"Engine/Platforms/Public/GPU/Sampler.cpp"
	"Engine/Platforms/Public/GPU/Sampler.h"
	"Engine/Platforms/Public/GPU/SamplerEnums.h"
	"Engine/Platforms/Public/GPU/ShaderEnums.h"
	"Engine/Platforms/Public/GPU/Sync.h"
	"Engine/Platforms/Public/GPU/Thread.h"
	"Engine/Platforms/Public/GPU/VertexAttribs.h"
	"Engine/Platforms/Public/GPU/VertexDescr.h"
	"Engine/Platforms/Public/GPU/VertexEnums.h"
	"Engine/Platforms/Public/GPU/VertexInputState.cpp"
	"Engine/Platforms/Public/GPU/VertexInputState.h"
	"Engine/Platforms/Public/GPU/VR.h"
	"Engine/Platforms/Impl/Main.cpp"
	"Engine/Platforms/Impl/PipelineTemplate.cpp"
	"Engine/Platforms/Engine.Platforms.h"
	"Engine/Platforms/Soft/Windows/SwWinSurface.cpp"
	"Engine/Platforms/Soft/Windows/SwWinSurface.h"
	"Engine/Platforms/OpenGL/Windows/GLWinContext.cpp"
	"Engine/Platforms/OpenGL/Windows/GLWinContext.h"
	"Engine/Platforms/OpenGL/Windows/GLWinLibrary.cpp"
	"Engine/Platforms/OpenGL/Windows/GLWinLibrary.h"
	"Engine/Platforms/Soft/Android/SwAndSurface.cpp"
	"Engine/Platforms/Soft/Android/SwAndSurface.h"
	"Engine/Platforms/Public/Common.h"
	"Engine/Platforms/Public/Tools/AsyncCommandsEmulator.h"
	"Engine/Platforms/Public/Tools/GPUThreadHelper.cpp"
	"Engine/Platforms/Public/Tools/GPUThreadHelper.h"
	"Engine/Platforms/Public/Tools/ImageUtils.cpp"
	"Engine/Platforms/Public/Tools/ImageUtils.h"
	"Engine/Platforms/Public/Tools/ImageViewHashMap.h"
	"Engine/Platforms/Public/Tools/MemoryMapperHelper.cpp"
	"Engine/Platforms/Public/Tools/MemoryMapperHelper.h"
	"Engine/Platforms/Public/Tools/SamplerUtils.cpp"
	"Engine/Platforms/Public/Tools/SamplerUtils.h"
	"Engine/Platforms/Public/Tools/WindowHelper.cpp"
	"Engine/Platforms/Public/Tools/WindowHelper.h"
	"Engine/Platforms/OpenGL/450/gl4.cpp"
	"Engine/Platforms/OpenGL/450/gl4.h"
	"Engine/Platforms/OpenGL/450/GL4BaseModule.cpp"
	"Engine/Platforms/OpenGL/450/GL4BaseModule.h"
	"Engine/Platforms/OpenGL/450/GL4Buffer.cpp"
	"Engine/Platforms/OpenGL/450/GL4CommandBuffer.cpp"
	"Engine/Platforms/OpenGL/450/GL4CommandBuilder.cpp"
	"Engine/Platforms/OpenGL/450/GL4CommandQueue.cpp"
	"Engine/Platforms/OpenGL/450/GL4Device.cpp"
	"Engine/Platforms/OpenGL/450/GL4Device.h"
	"Engine/Platforms/OpenGL/450/GL4Emulator.h"
	"Engine/Platforms/OpenGL/450/GL4Enums.h"
	"Engine/Platforms/OpenGL/450/GL4FlippedSystemFramebuffer.h"
	"Engine/Platforms/OpenGL/450/GL4Framebuffer.cpp"
	"Engine/Platforms/OpenGL/450/GL4Image.cpp"
	"Engine/Platforms/OpenGL/450/GL4Memory.cpp"
	"Engine/Platforms/OpenGL/450/GL4Messages.h"
	"Engine/Platforms/OpenGL/450/GL4Pipeline.cpp"
	"Engine/Platforms/OpenGL/450/GL4PipelineResourceTable.cpp"
	"Engine/Platforms/OpenGL/450/GL4RenderPass.cpp"
	"Engine/Platforms/OpenGL/450/GL4ResourceCache.h"
	"Engine/Platforms/OpenGL/450/GL4Sampler.cpp"
	"Engine/Platforms/OpenGL/450/GL4SamplerCache.h"
	"Engine/Platforms/OpenGL/450/GL4SyncManager.cpp"
	"Engine/Platforms/OpenGL/450/GL4SystemFramebuffer.h"
	"Engine/Platforms/Public/OS/Display.h"
	"Engine/Platforms/Public/OS/IDs.h"
	"Engine/Platforms/Public/OS/Input.h"
	"Engine/Platforms/Public/OS/OSEnums.h"
	"Engine/Platforms/Public/OS/Platform.h"
	"Engine/Platforms/Public/OS/ScreenCoords.h"
	"Engine/Platforms/Public/OS/Window.h"
	"Engine/Platforms/Vulkan/Windows/VkWinSurface.cpp"
	"Engine/Platforms/Vulkan/Windows/VkWinSurface.h"
	"Engine/Platforms/VR/Emulator/EmulatorVRThread.cpp"
	"Engine/Platforms/VR/VRObjectsConstructor.cpp"
	"Engine/Platforms/VR/VRObjectsConstructor.h" )
add_library( "Engine.Platforms" STATIC ${SOURCES} )
source_group( "Vulkan\\Android" FILES "Engine/Platforms/Vulkan/Android/VkAndSurface.cpp" "Engine/Platforms/Vulkan/Android/VkAndSurface.h" )
source_group( "Soft" FILES "Engine/Platforms/Soft/SoftRendererContext.cpp" "Engine/Platforms/Soft/SoftRendererObjectsConstructor.h" "Engine/Platforms/Soft/SoftRendererThread.cpp" )
source_group( "SDL" FILES "Engine/Platforms/SDL/SDLDisplay.cpp" "Engine/Platforms/SDL/SDLDisplay.h" "Engine/Platforms/SDL/SDLKeyInput.cpp" "Engine/Platforms/SDL/SDLMessages.h" "Engine/Platforms/SDL/SDLMouseInput.cpp" "Engine/Platforms/SDL/SDLObjectsConstructor.h" "Engine/Platforms/SDL/SDLPlatform.cpp" "Engine/Platforms/SDL/SDLWindow.cpp" )
source_group( "OpenGL" FILES "Engine/Platforms/OpenGL/OpenGLContext.cpp" "Engine/Platforms/OpenGL/OpenGLObjectsConstructor.h" "Engine/Platforms/OpenGL/OpenGLThread.cpp" )
source_group( "OpenCL\\120" FILES "Engine/Platforms/OpenCL/120/CL1BaseModule.cpp" "Engine/Platforms/OpenCL/120/CL1BaseModule.h" "Engine/Platforms/OpenCL/120/CL1Buffer.cpp" "Engine/Platforms/OpenCL/120/CL1CommandBuffer.cpp" "Engine/Platforms/OpenCL/120/CL1CommandBuilder.cpp" "Engine/Platforms/OpenCL/120/CL1CommandQueue.cpp" "Engine/Platforms/OpenCL/120/CL1Device.cpp" "Engine/Platforms/OpenCL/120/CL1Device.h" "Engine/Platforms/OpenCL/120/CL1Emulator.cpp" "Engine/Platforms/OpenCL/120/CL1Emulator.h" "Engine/Platforms/OpenCL/120/CL1Enums.h" "Engine/Platforms/OpenCL/120/CL1Image.cpp" "Engine/Platforms/OpenCL/120/CL1Messages.h" "Engine/Platforms/OpenCL/120/CL1Pipeline.cpp" "Engine/Platforms/OpenCL/120/CL1PipelineResourceTable.cpp" "Engine/Platforms/OpenCL/120/CL1ResourceCache.h" "Engine/Platforms/OpenCL/120/CL1Sampler.cpp" "Engine/Platforms/OpenCL/120/CL1SamplerCache.h" "Engine/Platforms/OpenCL/120/CL1SyncManager.cpp" "Engine/Platforms/OpenCL/120/cl2.cpp" "Engine/Platforms/OpenCL/120/cl2.h" )
source_group( "Vulkan\\110" FILES "Engine/Platforms/Vulkan/110/Vk1BaseModule.cpp" "Engine/Platforms/Vulkan/110/Vk1BaseModule.h" "Engine/Platforms/Vulkan/110/Vk1BaseObject.h" "Engine/Platforms/Vulkan/110/Vk1Buffer.cpp" "Engine/Platforms/Vulkan/110/Vk1CommandBuffer.cpp" "Engine/Platforms/Vulkan/110/Vk1CommandBuilder.cpp" "Engine/Platforms/Vulkan/110/Vk1CommandQueue.cpp" "Engine/Platforms/Vulkan/110/Vk1Device.cpp" "Engine/Platforms/Vulkan/110/Vk1Device.h" "Engine/Platforms/Vulkan/110/Vk1Enums.h" "Engine/Platforms/Vulkan/110/Vk1Framebuffer.cpp" "Engine/Platforms/Vulkan/110/Vk1Image.cpp" "Engine/Platforms/Vulkan/110/Vk1Library.h" "Engine/Platforms/Vulkan/110/Vk1ManagedMemory.cpp" "Engine/Platforms/Vulkan/110/Vk1MemoryManager.cpp" "Engine/Platforms/Vulkan/110/Vk1Messages.h" "Engine/Platforms/Vulkan/110/Vk1Pipeline.cpp" "Engine/Platforms/Vulkan/110/Vk1PipelineCache.cpp" "Engine/Platforms/Vulkan/110/Vk1PipelineCache.h" "Engine/Platforms/Vulkan/110/Vk1PipelineLayout.cpp" "Engine/Platforms/Vulkan/110/Vk1PipelineLayout.h" "Engine/Platforms/Vulkan/110/Vk1PipelineResourceTable.cpp" "Engine/Platforms/Vulkan/110/Vk1RenderPass.cpp" "Engine/Platforms/Vulkan/110/Vk1RenderPass.h" "Engine/Platforms/Vulkan/110/Vk1ResourceCache.h" "Engine/Platforms/Vulkan/110/Vk1Sampler.cpp" "Engine/Platforms/Vulkan/110/Vk1SamplerCache.h" "Engine/Platforms/Vulkan/110/Vk1SwapchainImage.h" "Engine/Platforms/Vulkan/110/Vk1SyncManager.cpp" "Engine/Platforms/Vulkan/110/vulkan1.cpp" "Engine/Platforms/Vulkan/110/vulkan1.h" "Engine/Platforms/Vulkan/110/vulkan1_platform.cpp" "Engine/Platforms/Vulkan/110/vulkan1_platform.h" "Engine/Platforms/Vulkan/110/vulkan1_utils.h" )
source_group( "OpenGL\\300es" FILES "Engine/Platforms/OpenGL/300es/gles3.cpp" "Engine/Platforms/OpenGL/300es/gles3.h" )
source_group( "OpenCL" FILES "Engine/Platforms/OpenCL/OpenCLContext.cpp" "Engine/Platforms/OpenCL/OpenCLObjectsConstructor.h" "Engine/Platforms/OpenCL/OpenCLThread.cpp" )
source_group( "Input" FILES "Engine/Platforms/Input/InputManager.cpp" "Engine/Platforms/Input/InputManager.h" "Engine/Platforms/Input/InputThread.cpp" )
source_group( "Soft\\ShaderLang" FILES "Engine/Platforms/Soft/ShaderLang/gen_Image.h" "Engine/Platforms/Soft/ShaderLang/gen_Texture.h" "Engine/Platforms/Soft/ShaderLang/SWLang.h" "Engine/Platforms/Soft/ShaderLang/SWLangArray.h" "Engine/Platforms/Soft/ShaderLang/SWLangBarrier.h" "Engine/Platforms/Soft/ShaderLang/SWLangBuffer.h" "Engine/Platforms/Soft/ShaderLang/SWLangCommon.h" "Engine/Platforms/Soft/ShaderLang/SWLangGLM.h" "Engine/Platforms/Soft/ShaderLang/SWLangImage.cpp" "Engine/Platforms/Soft/ShaderLang/SWLangImage.h" "Engine/Platforms/Soft/ShaderLang/SWLangShared.h" "Engine/Platforms/Soft/ShaderLang/SWLangTexture.cpp" "Engine/Platforms/Soft/ShaderLang/SWLangTexture.h" "Engine/Platforms/Soft/ShaderLang/SWLangTexture2D.h" "Engine/Platforms/Soft/ShaderLang/SWShaderHelper.h" )
source_group( "Vulkan" FILES "Engine/Platforms/Vulkan/VulkanContext.cpp" "Engine/Platforms/Vulkan/VulkanObjectsConstructor.h" "Engine/Platforms/Vulkan/VulkanThread.cpp" )
source_group( "Soft\\Impl" FILES "Engine/Platforms/Soft/Impl/SWBaseModule.cpp" "Engine/Platforms/Soft/Impl/SWBaseModule.h" "Engine/Platforms/Soft/Impl/SWBuffer.cpp" "Engine/Platforms/Soft/Impl/SWCommandBuffer.cpp" "Engine/Platforms/Soft/Impl/SWCommandBuilder.cpp" "Engine/Platforms/Soft/Impl/SWCommandQueue.cpp" "Engine/Platforms/Soft/Impl/SWDevice.cpp" "Engine/Platforms/Soft/Impl/SWDevice.h" "Engine/Platforms/Soft/Impl/SWDeviceProperties.h" "Engine/Platforms/Soft/Impl/SWEnums.h" "Engine/Platforms/Soft/Impl/SWImage.cpp" "Engine/Platforms/Soft/Impl/SWMemory.cpp" "Engine/Platforms/Soft/Impl/SWMessages.h" "Engine/Platforms/Soft/Impl/SWPipeline.cpp" "Engine/Platforms/Soft/Impl/SWPipelineResourceTable.cpp" "Engine/Platforms/Soft/Impl/SWSampler.cpp" "Engine/Platforms/Soft/Impl/SWSamplerCache.h" "Engine/Platforms/Soft/Impl/SWShaderModel.cpp" "Engine/Platforms/Soft/Impl/SWShaderModel.h" "Engine/Platforms/Soft/Impl/SWSyncManager.cpp" "Engine/Platforms/Soft/Impl/SWSyncObjects.h" )
source_group( "Windows" FILES "Engine/Platforms/Windows/WinDisplay.cpp" "Engine/Platforms/Windows/WinDisplay.h" "Engine/Platforms/Windows/WinKeyInput.cpp" "Engine/Platforms/Windows/WinMessages.h" "Engine/Platforms/Windows/WinMouseInput.cpp" "Engine/Platforms/Windows/WinObjectsConstructor.h" "Engine/Platforms/Windows/WinPlatform.cpp" "Engine/Platforms/Windows/WinWindow.cpp" )
source_group( "Public\\GPU" FILES "Engine/Platforms/Public/GPU/Buffer.h" "Engine/Platforms/Public/GPU/BufferEnums.h" "Engine/Platforms/Public/GPU/CommandBuffer.h" "Engine/Platforms/Public/GPU/CommandEnums.h" "Engine/Platforms/Public/GPU/Context.cpp" "Engine/Platforms/Public/GPU/Context.h" "Engine/Platforms/Public/GPU/Enums.ToString.h" "Engine/Platforms/Public/GPU/FragmentOutputState.h" "Engine/Platforms/Public/GPU/Framebuffer.h" "Engine/Platforms/Public/GPU/IDs.h" "Engine/Platforms/Public/GPU/Image.cpp" "Engine/Platforms/Public/GPU/Image.h" "Engine/Platforms/Public/GPU/ImageEnums.h" "Engine/Platforms/Public/GPU/ImageLayer.h" "Engine/Platforms/Public/GPU/ImageSwizzle.h" "Engine/Platforms/Public/GPU/Memory.h" "Engine/Platforms/Public/GPU/MemoryEnums.h" "Engine/Platforms/Public/GPU/MipmapLevel.h" "Engine/Platforms/Public/GPU/MultiSamples.h" "Engine/Platforms/Public/GPU/ObjectEnums.h" "Engine/Platforms/Public/GPU/Pipeline.cpp" "Engine/Platforms/Public/GPU/Pipeline.h" "Engine/Platforms/Public/GPU/PipelineLayout.cpp" "Engine/Platforms/Public/GPU/PipelineLayout.h" "Engine/Platforms/Public/GPU/PixelFormatEnums.h" "Engine/Platforms/Public/GPU/RenderPass.cpp" "Engine/Platforms/Public/GPU/RenderPass.h" "Engine/Platforms/Public/GPU/RenderPassEnums.h" "Engine/Platforms/Public/GPU/RenderState.cpp" "Engine/Platforms/Public/GPU/RenderState.h" "Engine/Platforms/Public/GPU/RenderStateEnums.h" "Engine/Platforms/Public/GPU/Sampler.cpp" "Engine/Platforms/Public/GPU/Sampler.h" "Engine/Platforms/Public/GPU/SamplerEnums.h" "Engine/Platforms/Public/GPU/ShaderEnums.h" "Engine/Platforms/Public/GPU/Sync.h" "Engine/Platforms/Public/GPU/Thread.h" "Engine/Platforms/Public/GPU/VertexAttribs.h" "Engine/Platforms/Public/GPU/VertexDescr.h" "Engine/Platforms/Public/GPU/VertexEnums.h" "Engine/Platforms/Public/GPU/VertexInputState.cpp" "Engine/Platforms/Public/GPU/VertexInputState.h" "Engine/Platforms/Public/GPU/VR.h" )
source_group( "Impl" FILES "Engine/Platforms/Impl/Main.cpp" "Engine/Platforms/Impl/PipelineTemplate.cpp" )
source_group( "" FILES "Engine/Platforms/Engine.Platforms.h" )
source_group( "Soft\\Windows" FILES "Engine/Platforms/Soft/Windows/SwWinSurface.cpp" "Engine/Platforms/Soft/Windows/SwWinSurface.h" )
source_group( "OpenGL\\Windows" FILES "Engine/Platforms/OpenGL/Windows/GLWinContext.cpp" "Engine/Platforms/OpenGL/Windows/GLWinContext.h" "Engine/Platforms/OpenGL/Windows/GLWinLibrary.cpp" "Engine/Platforms/OpenGL/Windows/GLWinLibrary.h" )
source_group( "Soft\\Android" FILES "Engine/Platforms/Soft/Android/SwAndSurface.cpp" "Engine/Platforms/Soft/Android/SwAndSurface.h" )
source_group( "Public" FILES "Engine/Platforms/Public/Common.h" )
source_group( "Public\\Tools" FILES "Engine/Platforms/Public/Tools/AsyncCommandsEmulator.h" "Engine/Platforms/Public/Tools/GPUThreadHelper.cpp" "Engine/Platforms/Public/Tools/GPUThreadHelper.h" "Engine/Platforms/Public/Tools/ImageUtils.cpp" "Engine/Platforms/Public/Tools/ImageUtils.h" "Engine/Platforms/Public/Tools/ImageViewHashMap.h" "Engine/Platforms/Public/Tools/MemoryMapperHelper.cpp" "Engine/Platforms/Public/Tools/MemoryMapperHelper.h" "Engine/Platforms/Public/Tools/SamplerUtils.cpp" "Engine/Platforms/Public/Tools/SamplerUtils.h" "Engine/Platforms/Public/Tools/WindowHelper.cpp" "Engine/Platforms/Public/Tools/WindowHelper.h" )
source_group( "OpenGL\\450" FILES "Engine/Platforms/OpenGL/450/gl4.cpp" "Engine/Platforms/OpenGL/450/gl4.h" "Engine/Platforms/OpenGL/450/GL4BaseModule.cpp" "Engine/Platforms/OpenGL/450/GL4BaseModule.h" "Engine/Platforms/OpenGL/450/GL4Buffer.cpp" "Engine/Platforms/OpenGL/450/GL4CommandBuffer.cpp" "Engine/Platforms/OpenGL/450/GL4CommandBuilder.cpp" "Engine/Platforms/OpenGL/450/GL4CommandQueue.cpp" "Engine/Platforms/OpenGL/450/GL4Device.cpp" "Engine/Platforms/OpenGL/450/GL4Device.h" "Engine/Platforms/OpenGL/450/GL4Emulator.h" "Engine/Platforms/OpenGL/450/GL4Enums.h" "Engine/Platforms/OpenGL/450/GL4FlippedSystemFramebuffer.h" "Engine/Platforms/OpenGL/450/GL4Framebuffer.cpp" "Engine/Platforms/OpenGL/450/GL4Image.cpp" "Engine/Platforms/OpenGL/450/GL4Memory.cpp" "Engine/Platforms/OpenGL/450/GL4Messages.h" "Engine/Platforms/OpenGL/450/GL4Pipeline.cpp" "Engine/Platforms/OpenGL/450/GL4PipelineResourceTable.cpp" "Engine/Platforms/OpenGL/450/GL4RenderPass.cpp" "Engine/Platforms/OpenGL/450/GL4ResourceCache.h" "Engine/Platforms/OpenGL/450/GL4Sampler.cpp" "Engine/Platforms/OpenGL/450/GL4SamplerCache.h" "Engine/Platforms/OpenGL/450/GL4SyncManager.cpp" "Engine/Platforms/OpenGL/450/GL4SystemFramebuffer.h" )
source_group( "Public\\OS" FILES "Engine/Platforms/Public/OS/Display.h" "Engine/Platforms/Public/OS/IDs.h" "Engine/Platforms/Public/OS/Input.h" "Engine/Platforms/Public/OS/OSEnums.h" "Engine/Platforms/Public/OS/Platform.h" "Engine/Platforms/Public/OS/ScreenCoords.h" "Engine/Platforms/Public/OS/Window.h" )
source_group( "Vulkan\\Windows" FILES "Engine/Platforms/Vulkan/Windows/VkWinSurface.cpp" "Engine/Platforms/Vulkan/Windows/VkWinSurface.h" )
source_group( "VR\\Emulator" FILES "Engine/Platforms/VR/Emulator/EmulatorVRThread.cpp" )
source_group( "VR" FILES "Engine/Platforms/VR/VRObjectsConstructor.cpp" "Engine/Platforms/VR/VRObjectsConstructor.h" )
set_property( TARGET "Engine.Platforms" PROPERTY FOLDER "Engine" )
target_include_directories( "Engine.Platforms" PRIVATE "Engine/Platforms" )
target_include_directories( "Engine.Platforms" PUBLIC "External" )
target_include_directories( "Engine.Platforms" PUBLIC "${EXTERNALS_PATH}" )
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
if (DEFINED OPENGLES_VERSION AND NOT (OPENGLES_VERSION EQUAL "0"))
	target_compile_definitions( "Engine.Platforms" PUBLIC "GRAPHICS_API_OPENGLES=${OPENGLES_VERSION}" )
endif()
if (DEFINED VULKAN_VERSION AND NOT (VULKAN_VERSION EQUAL "0"))
	target_compile_definitions( "Engine.Platforms" PUBLIC "GRAPHICS_API_VULKAN=${VULKAN_VERSION}" )
endif()
if (DEFINED OPENCL_VERSION AND NOT (OPENCL_VERSION EQUAL "0"))
	target_compile_definitions( "Engine.Platforms" PUBLIC "COMPUTE_API_OPENCL=${OPENCL_VERSION}" )
endif()
if (DEFINED SOFTRENDER_VERSION AND NOT (SOFTRENDER_VERSION EQUAL "0"))
	target_compile_definitions( "Engine.Platforms" PUBLIC "GRAPHICS_API_SOFT=${SOFTRENDER_VERSION}" )
endif()
if (DEFINED OPENGL_VERSION AND NOT (OPENGL_VERSION EQUAL "0"))
	target_compile_definitions( "Engine.Platforms" PUBLIC "GRAPHICS_API_OPENGL=${OPENGL_VERSION}" )
endif()
# compiler
set_target_properties( "Engine.Platforms" PROPERTIES LINK_FLAGS_RELEASE ${PROJECTS_SHARED_LINKER_FLAGS_RELEASE} )
set_target_properties( "Engine.Platforms" PROPERTIES LINK_FLAGS_PROFILE ${PROJECTS_SHARED_LINKER_FLAGS_PROFILE} )
target_compile_definitions( "Engine.Platforms" PRIVATE $<$<CONFIG:Debug>: ${PROJECTS_SHARED_DEFINES_DEBUG}> )
set_target_properties( "Engine.Platforms" PROPERTIES LINK_FLAGS_DEBUGANALYZE ${PROJECTS_SHARED_LINKER_FLAGS_DEBUGANALYZE} )
target_compile_definitions( "Engine.Platforms" PRIVATE $<$<CONFIG:DebugAnalyze>: ${PROJECTS_SHARED_DEFINES_DEBUGANALYZE}> )
target_compile_options( "Engine.Platforms" PRIVATE $<$<CONFIG:Profile>: ${PROJECTS_SHARED_CXX_FLAGS_PROFILE}> )
target_compile_options( "Engine.Platforms" PRIVATE $<$<CONFIG:Release>: ${PROJECTS_SHARED_CXX_FLAGS_RELEASE}> )
target_compile_definitions( "Engine.Platforms" PRIVATE $<$<CONFIG:Profile>: ${PROJECTS_SHARED_DEFINES_PROFILE}> )
target_compile_definitions( "Engine.Platforms" PRIVATE $<$<CONFIG:Release>: ${PROJECTS_SHARED_DEFINES_RELEASE}> )
set_target_properties( "Engine.Platforms" PROPERTIES LINK_FLAGS_DEBUG ${PROJECTS_SHARED_LINKER_FLAGS_DEBUG} )
target_compile_options( "Engine.Platforms" PRIVATE $<$<CONFIG:DebugAnalyze>: ${PROJECTS_SHARED_CXX_FLAGS_DEBUGANALYZE}> )
target_compile_options( "Engine.Platforms" PRIVATE $<$<CONFIG:Debug>: ${PROJECTS_SHARED_CXX_FLAGS_DEBUG}> )



#==================================================================================================
# project: Engine.Graphics
#==================================================================================================
set( SOURCES 
	"Engine/Graphics/Impl/AsyncCommandBuffer.cpp"
	"Engine/Graphics/Impl/BatchRenderer.cpp"
	"Engine/Graphics/Impl/CommandBufferManager.cpp"
	"Engine/Graphics/Impl/Font.cpp"
	"Engine/Graphics/Impl/FontManager.cpp"
	"Engine/Graphics/Impl/GraphicsBaseModule.cpp"
	"Engine/Graphics/Impl/GraphicsBaseModule.h"
	"Engine/Graphics/Impl/GraphicsObjectsConstructor.h"
	"Engine/Graphics/Impl/Main.cpp"
	"Engine/Graphics/Impl/MemoryInputStream.cpp"
	"Engine/Graphics/Impl/MemoryOutputStream.cpp"
	"Engine/Graphics/Impl/TextureLineAtlas.h"
	"Engine/Graphics/Engine.Graphics.h"
	"Engine/Graphics/Public/BatchRenderer.h"
	"Engine/Graphics/Public/Commands.h"
	"Engine/Graphics/Public/Common.h"
	"Engine/Graphics/Public/Font.h"
	"Engine/Graphics/Public/IDs.h"
	"Engine/Graphics/Public/MemStream.h"
	"Engine/Graphics/Public/Primitives.h"
	"Engine/Graphics/Public/Vertices.h" )
add_library( "Engine.Graphics" STATIC ${SOURCES} )
source_group( "Impl" FILES "Engine/Graphics/Impl/AsyncCommandBuffer.cpp" "Engine/Graphics/Impl/BatchRenderer.cpp" "Engine/Graphics/Impl/CommandBufferManager.cpp" "Engine/Graphics/Impl/Font.cpp" "Engine/Graphics/Impl/FontManager.cpp" "Engine/Graphics/Impl/GraphicsBaseModule.cpp" "Engine/Graphics/Impl/GraphicsBaseModule.h" "Engine/Graphics/Impl/GraphicsObjectsConstructor.h" "Engine/Graphics/Impl/Main.cpp" "Engine/Graphics/Impl/MemoryInputStream.cpp" "Engine/Graphics/Impl/MemoryOutputStream.cpp" "Engine/Graphics/Impl/TextureLineAtlas.h" )
source_group( "" FILES "Engine/Graphics/Engine.Graphics.h" )
source_group( "Public" FILES "Engine/Graphics/Public/BatchRenderer.h" "Engine/Graphics/Public/Commands.h" "Engine/Graphics/Public/Common.h" "Engine/Graphics/Public/Font.h" "Engine/Graphics/Public/IDs.h" "Engine/Graphics/Public/MemStream.h" "Engine/Graphics/Public/Primitives.h" "Engine/Graphics/Public/Vertices.h" )
set_property( TARGET "Engine.Graphics" PROPERTY FOLDER "Engine" )
target_include_directories( "Engine.Graphics" PRIVATE "Engine/Graphics" )
target_include_directories( "Engine.Graphics" PUBLIC "External" )
target_include_directories( "Engine.Graphics" PUBLIC "${EXTERNALS_PATH}" )
target_include_directories( "Engine.Graphics" PUBLIC "Engine/.." )
target_link_libraries( "Engine.Graphics" "Engine.Platforms" )
# compiler
set_target_properties( "Engine.Graphics" PROPERTIES LINK_FLAGS_RELEASE ${PROJECTS_SHARED_LINKER_FLAGS_RELEASE} )
set_target_properties( "Engine.Graphics" PROPERTIES LINK_FLAGS_PROFILE ${PROJECTS_SHARED_LINKER_FLAGS_PROFILE} )
target_compile_definitions( "Engine.Graphics" PRIVATE $<$<CONFIG:Debug>: ${PROJECTS_SHARED_DEFINES_DEBUG}> )
set_target_properties( "Engine.Graphics" PROPERTIES LINK_FLAGS_DEBUGANALYZE ${PROJECTS_SHARED_LINKER_FLAGS_DEBUGANALYZE} )
target_compile_definitions( "Engine.Graphics" PRIVATE $<$<CONFIG:DebugAnalyze>: ${PROJECTS_SHARED_DEFINES_DEBUGANALYZE}> )
target_compile_options( "Engine.Graphics" PRIVATE $<$<CONFIG:Profile>: ${PROJECTS_SHARED_CXX_FLAGS_PROFILE}> )
target_compile_options( "Engine.Graphics" PRIVATE $<$<CONFIG:Release>: ${PROJECTS_SHARED_CXX_FLAGS_RELEASE}> )
target_compile_definitions( "Engine.Graphics" PRIVATE $<$<CONFIG:Profile>: ${PROJECTS_SHARED_DEFINES_PROFILE}> )
target_compile_definitions( "Engine.Graphics" PRIVATE $<$<CONFIG:Release>: ${PROJECTS_SHARED_DEFINES_RELEASE}> )
set_target_properties( "Engine.Graphics" PROPERTIES LINK_FLAGS_DEBUG ${PROJECTS_SHARED_LINKER_FLAGS_DEBUG} )
target_compile_options( "Engine.Graphics" PRIVATE $<$<CONFIG:DebugAnalyze>: ${PROJECTS_SHARED_CXX_FLAGS_DEBUGANALYZE}> )
target_compile_options( "Engine.Graphics" PRIVATE $<$<CONFIG:Debug>: ${PROJECTS_SHARED_CXX_FLAGS_DEBUG}> )



#==================================================================================================
# project: Engine.Profilers
#==================================================================================================
set( SOURCES 
	"Engine/Profilers/Impl/FPSCounter.cpp"
	"Engine/Profilers/Impl/Main.cpp"
	"Engine/Profilers/Impl/ProfilerObjectsConstructor.h"
	"Engine/Profilers/Engine.Profilers.h"
	"Engine/Profilers/Public/Common.h"
	"Engine/Profilers/Public/GpuStatistic.h"
	"Engine/Profilers/Public/IDs.h" )
add_library( "Engine.Profilers" STATIC ${SOURCES} )
source_group( "Impl" FILES "Engine/Profilers/Impl/FPSCounter.cpp" "Engine/Profilers/Impl/Main.cpp" "Engine/Profilers/Impl/ProfilerObjectsConstructor.h" )
source_group( "" FILES "Engine/Profilers/Engine.Profilers.h" )
source_group( "Public" FILES "Engine/Profilers/Public/Common.h" "Engine/Profilers/Public/GpuStatistic.h" "Engine/Profilers/Public/IDs.h" )
set_property( TARGET "Engine.Profilers" PROPERTY FOLDER "Engine" )
target_include_directories( "Engine.Profilers" PRIVATE "Engine/Profilers" )
target_include_directories( "Engine.Profilers" PUBLIC "External" )
target_include_directories( "Engine.Profilers" PUBLIC "${EXTERNALS_PATH}" )
target_include_directories( "Engine.Profilers" PUBLIC "Engine/.." )
target_link_libraries( "Engine.Profilers" "Engine.Platforms" )
# compiler
set_target_properties( "Engine.Profilers" PROPERTIES LINK_FLAGS_RELEASE ${PROJECTS_SHARED_LINKER_FLAGS_RELEASE} )
set_target_properties( "Engine.Profilers" PROPERTIES LINK_FLAGS_PROFILE ${PROJECTS_SHARED_LINKER_FLAGS_PROFILE} )
target_compile_definitions( "Engine.Profilers" PRIVATE $<$<CONFIG:Debug>: ${PROJECTS_SHARED_DEFINES_DEBUG}> )
set_target_properties( "Engine.Profilers" PROPERTIES LINK_FLAGS_DEBUGANALYZE ${PROJECTS_SHARED_LINKER_FLAGS_DEBUGANALYZE} )
target_compile_definitions( "Engine.Profilers" PRIVATE $<$<CONFIG:DebugAnalyze>: ${PROJECTS_SHARED_DEFINES_DEBUGANALYZE}> )
target_compile_options( "Engine.Profilers" PRIVATE $<$<CONFIG:Profile>: ${PROJECTS_SHARED_CXX_FLAGS_PROFILE}> )
target_compile_options( "Engine.Profilers" PRIVATE $<$<CONFIG:Release>: ${PROJECTS_SHARED_CXX_FLAGS_RELEASE}> )
target_compile_definitions( "Engine.Profilers" PRIVATE $<$<CONFIG:Profile>: ${PROJECTS_SHARED_DEFINES_PROFILE}> )
target_compile_definitions( "Engine.Profilers" PRIVATE $<$<CONFIG:Release>: ${PROJECTS_SHARED_DEFINES_RELEASE}> )
set_target_properties( "Engine.Profilers" PROPERTIES LINK_FLAGS_DEBUG ${PROJECTS_SHARED_LINKER_FLAGS_DEBUG} )
target_compile_options( "Engine.Profilers" PRIVATE $<$<CONFIG:DebugAnalyze>: ${PROJECTS_SHARED_CXX_FLAGS_DEBUGANALYZE}> )
target_compile_options( "Engine.Profilers" PRIVATE $<$<CONFIG:Debug>: ${PROJECTS_SHARED_CXX_FLAGS_DEBUG}> )



#==================================================================================================
# project: Engine.ImportExport
#==================================================================================================
set( SOURCES 
	"Engine/ImportExport/Impl/GX_ImageFormat.h"
	"Engine/ImportExport/Impl/GX_ImageLoader.cpp"
	"Engine/ImportExport/Impl/GX_ImageSaver.cpp"
	"Engine/ImportExport/Impl/ImportExportObjectsConstructor.h"
	"Engine/ImportExport/Impl/Main.cpp"
	"Engine/ImportExport/Engine.ImportExport.h"
	"Engine/ImportExport/README.md"
	"Engine/ImportExport/Public/Common.h"
	"Engine/ImportExport/Public/IDs.h"
	"Engine/ImportExport/Public/Image.h" )
add_library( "Engine.ImportExport" STATIC ${SOURCES} )
source_group( "Impl" FILES "Engine/ImportExport/Impl/GX_ImageFormat.h" "Engine/ImportExport/Impl/GX_ImageLoader.cpp" "Engine/ImportExport/Impl/GX_ImageSaver.cpp" "Engine/ImportExport/Impl/ImportExportObjectsConstructor.h" "Engine/ImportExport/Impl/Main.cpp" )
source_group( "" FILES "Engine/ImportExport/Engine.ImportExport.h" "Engine/ImportExport/README.md" )
source_group( "Public" FILES "Engine/ImportExport/Public/Common.h" "Engine/ImportExport/Public/IDs.h" "Engine/ImportExport/Public/Image.h" )
set_property( TARGET "Engine.ImportExport" PROPERTY FOLDER "Engine" )
target_include_directories( "Engine.ImportExport" PRIVATE "Engine/ImportExport" )
target_include_directories( "Engine.ImportExport" PUBLIC "External" )
target_include_directories( "Engine.ImportExport" PUBLIC "${EXTERNALS_PATH}" )
target_include_directories( "Engine.ImportExport" PUBLIC "Engine/.." )
target_link_libraries( "Engine.ImportExport" "Engine.Graphics" )
# compiler
set_target_properties( "Engine.ImportExport" PROPERTIES LINK_FLAGS_RELEASE ${PROJECTS_SHARED_LINKER_FLAGS_RELEASE} )
set_target_properties( "Engine.ImportExport" PROPERTIES LINK_FLAGS_PROFILE ${PROJECTS_SHARED_LINKER_FLAGS_PROFILE} )
target_compile_definitions( "Engine.ImportExport" PRIVATE $<$<CONFIG:Debug>: ${PROJECTS_SHARED_DEFINES_DEBUG}> )
set_target_properties( "Engine.ImportExport" PROPERTIES LINK_FLAGS_DEBUGANALYZE ${PROJECTS_SHARED_LINKER_FLAGS_DEBUGANALYZE} )
target_compile_definitions( "Engine.ImportExport" PRIVATE $<$<CONFIG:DebugAnalyze>: ${PROJECTS_SHARED_DEFINES_DEBUGANALYZE}> )
target_compile_options( "Engine.ImportExport" PRIVATE $<$<CONFIG:Profile>: ${PROJECTS_SHARED_CXX_FLAGS_PROFILE}> )
target_compile_options( "Engine.ImportExport" PRIVATE $<$<CONFIG:Release>: ${PROJECTS_SHARED_CXX_FLAGS_RELEASE}> )
target_compile_definitions( "Engine.ImportExport" PRIVATE $<$<CONFIG:Profile>: ${PROJECTS_SHARED_DEFINES_PROFILE}> )
target_compile_definitions( "Engine.ImportExport" PRIVATE $<$<CONFIG:Release>: ${PROJECTS_SHARED_DEFINES_RELEASE}> )
set_target_properties( "Engine.ImportExport" PROPERTIES LINK_FLAGS_DEBUG ${PROJECTS_SHARED_LINKER_FLAGS_DEBUG} )
target_compile_options( "Engine.ImportExport" PRIVATE $<$<CONFIG:DebugAnalyze>: ${PROJECTS_SHARED_CXX_FLAGS_DEBUGANALYZE}> )
target_compile_options( "Engine.ImportExport" PRIVATE $<$<CONFIG:Debug>: ${PROJECTS_SHARED_CXX_FLAGS_DEBUG}> )



#==================================================================================================
# project: Engine.Scene
#==================================================================================================
set( SOURCES 
	"Engine/Scene/Surface/OffscreenSurface.cpp"
	"Engine/Scene/Surface/VideoRecorderSurface.cpp"
	"Engine/Scene/Surface/VRSurface.cpp"
	"Engine/Scene/Surface/WindowSurface.cpp"
	"Engine/Scene/Renderer/SceneRendererMainThread.cpp"
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
	"Engine/Scene/Public/Camera.h"
	"Engine/Scene/Public/Common.h"
	"Engine/Scene/Public/FrameGraph.h"
	"Engine/Scene/Public/IDs.h"
	"Engine/Scene/Public/Scene.h"
	"Engine/Scene/Public/Surface.h"
	"Engine/Scene/Logic/SceneLogicMainThread.cpp" )
add_library( "Engine.Scene" STATIC ${SOURCES} )
source_group( "Surface" FILES "Engine/Scene/Surface/OffscreenSurface.cpp" "Engine/Scene/Surface/VideoRecorderSurface.cpp" "Engine/Scene/Surface/VRSurface.cpp" "Engine/Scene/Surface/WindowSurface.cpp" )
source_group( "Renderer" FILES "Engine/Scene/Renderer/SceneRendererMainThread.cpp" )
source_group( "Camera" FILES "Engine/Scene/Camera/FreeCamera.cpp" "Engine/Scene/Camera/FreeVRCamera.cpp" )
source_group( "Impl" FILES "Engine/Scene/Impl/BaseSceneModule.cpp" "Engine/Scene/Impl/BaseSceneModule.h" "Engine/Scene/Impl/Main.cpp" "Engine/Scene/Impl/SceneMainThread.cpp" "Engine/Scene/Impl/SceneManager.cpp" "Engine/Scene/Impl/SceneObjectConstructor.h" )
source_group( "" FILES "Engine/Scene/Engine.Scene.h" )
source_group( "Physics" FILES "Engine/Scene/Physics/ScenePhysicsMainThread.cpp" )
source_group( "Public" FILES "Engine/Scene/Public/Camera.h" "Engine/Scene/Public/Common.h" "Engine/Scene/Public/FrameGraph.h" "Engine/Scene/Public/IDs.h" "Engine/Scene/Public/Scene.h" "Engine/Scene/Public/Surface.h" )
source_group( "Logic" FILES "Engine/Scene/Logic/SceneLogicMainThread.cpp" )
set_property( TARGET "Engine.Scene" PROPERTY FOLDER "Engine" )
target_include_directories( "Engine.Scene" PRIVATE "Engine/Scene" )
target_include_directories( "Engine.Scene" PUBLIC "External" )
target_include_directories( "Engine.Scene" PUBLIC "${EXTERNALS_PATH}" )
target_include_directories( "Engine.Scene" PUBLIC "Engine/.." )
target_link_libraries( "Engine.Scene" "Engine.Graphics" )
# compiler
set_target_properties( "Engine.Scene" PROPERTIES LINK_FLAGS_RELEASE ${PROJECTS_SHARED_LINKER_FLAGS_RELEASE} )
set_target_properties( "Engine.Scene" PROPERTIES LINK_FLAGS_PROFILE ${PROJECTS_SHARED_LINKER_FLAGS_PROFILE} )
target_compile_definitions( "Engine.Scene" PRIVATE $<$<CONFIG:Debug>: ${PROJECTS_SHARED_DEFINES_DEBUG}> )
set_target_properties( "Engine.Scene" PROPERTIES LINK_FLAGS_DEBUGANALYZE ${PROJECTS_SHARED_LINKER_FLAGS_DEBUGANALYZE} )
target_compile_definitions( "Engine.Scene" PRIVATE $<$<CONFIG:DebugAnalyze>: ${PROJECTS_SHARED_DEFINES_DEBUGANALYZE}> )
target_compile_options( "Engine.Scene" PRIVATE $<$<CONFIG:Profile>: ${PROJECTS_SHARED_CXX_FLAGS_PROFILE}> )
target_compile_options( "Engine.Scene" PRIVATE $<$<CONFIG:Release>: ${PROJECTS_SHARED_CXX_FLAGS_RELEASE}> )
target_compile_definitions( "Engine.Scene" PRIVATE $<$<CONFIG:Profile>: ${PROJECTS_SHARED_DEFINES_PROFILE}> )
target_compile_definitions( "Engine.Scene" PRIVATE $<$<CONFIG:Release>: ${PROJECTS_SHARED_DEFINES_RELEASE}> )
set_target_properties( "Engine.Scene" PROPERTIES LINK_FLAGS_DEBUG ${PROJECTS_SHARED_LINKER_FLAGS_DEBUG} )
target_compile_options( "Engine.Scene" PRIVATE $<$<CONFIG:DebugAnalyze>: ${PROJECTS_SHARED_CXX_FLAGS_DEBUGANALYZE}> )
target_compile_options( "Engine.Scene" PRIVATE $<$<CONFIG:Debug>: ${PROJECTS_SHARED_CXX_FLAGS_DEBUG}> )



#==================================================================================================
# project: Engine.PipelineCompiler
#==================================================================================================
set( SOURCES 
	"Engine/PipelineCompiler/Serializers/AngelScriptSerializer.cpp"
	"Engine/PipelineCompiler/Serializers/AngelScriptSerializer.h"
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
	"Engine/PipelineCompiler/README.md"
	"Engine/PipelineCompiler/Shaders/DeserializedShader.cpp"
	"Engine/PipelineCompiler/Shaders/DeserializedShader.h"
	"Engine/PipelineCompiler/Shaders/glslang_Include.h"
	"Engine/PipelineCompiler/Shaders/ShaderCompiler.cpp"
	"Engine/PipelineCompiler/Shaders/ShaderCompiler.h"
	"Engine/PipelineCompiler/Shaders/ShaderCompiler_CLCompiler.cpp"
	"Engine/PipelineCompiler/Shaders/ShaderCompiler_CLTranslator.cpp"
	"Engine/PipelineCompiler/Shaders/ShaderCompiler_CPPTranslator.cpp"
	"Engine/PipelineCompiler/Shaders/ShaderCompiler_GLCompiler.cpp"
	"Engine/PipelineCompiler/Shaders/ShaderCompiler_GLParser.cpp"
	"Engine/PipelineCompiler/Shaders/ShaderCompiler_GLSLTranslator.cpp"
	"Engine/PipelineCompiler/Shaders/ShaderCompiler_HLSLCompiler.cpp"
	"Engine/PipelineCompiler/Shaders/ShaderCompiler_HLSLTranslator.cpp"
	"Engine/PipelineCompiler/Shaders/ShaderCompiler_NameValidator.cpp"
	"Engine/PipelineCompiler/Shaders/ShaderCompiler_NameValidator.h"
	"Engine/PipelineCompiler/Shaders/ShaderCompiler_ReplaceTypes.cpp"
	"Engine/PipelineCompiler/Shaders/ShaderCompiler_Translator.cpp"
	"Engine/PipelineCompiler/Shaders/ShaderCompiler_Translator.h"
	"Engine/PipelineCompiler/Shaders/ShaderCompiler_Utils.h"
	"Engine/PipelineCompiler/Shaders/ShaderCompiler_VkCompiler.cpp"
	"Engine/PipelineCompiler/cl/cl_source_vfs.cpp"
	"Engine/PipelineCompiler/cl/cl_source_vfs.h"
	"Engine/PipelineCompiler/cl/funcs.cpp"
	"Engine/PipelineCompiler/cl/matrix.cpp"
	"Engine/PipelineCompiler/glsl/AABBox.cpp"
	"Engine/PipelineCompiler/glsl/Billow.cpp"
	"Engine/PipelineCompiler/glsl/BitMath.cpp"
	"Engine/PipelineCompiler/glsl/Blur.cpp"
	"Engine/PipelineCompiler/glsl/Cellular.cpp"
	"Engine/PipelineCompiler/glsl/Cmp.cpp"
	"Engine/PipelineCompiler/glsl/Color.cpp"
	"Engine/PipelineCompiler/glsl/Colors.cpp"
	"Engine/PipelineCompiler/glsl/DefaultSDF.cpp"
	"Engine/PipelineCompiler/glsl/Defines.cpp"
	"Engine/PipelineCompiler/glsl/DHash.cpp"
	"Engine/PipelineCompiler/glsl/Emitters.cpp"
	"Engine/PipelineCompiler/glsl/FBM.cpp"
	"Engine/PipelineCompiler/glsl/Gabor.cpp"
	"Engine/PipelineCompiler/glsl/GlobalIndex.cpp"
	"Engine/PipelineCompiler/glsl/glsl_source_vfs.cpp"
	"Engine/PipelineCompiler/glsl/glsl_source_vfs.h"
	"Engine/PipelineCompiler/glsl/Gravity.cpp"
	"Engine/PipelineCompiler/glsl/Hash.cpp"
	"Engine/PipelineCompiler/glsl/Hash2.cpp"
	"Engine/PipelineCompiler/glsl/IHash.cpp"
	"Engine/PipelineCompiler/glsl/Line2.cpp"
	"Engine/PipelineCompiler/glsl/Line3.cpp"
	"Engine/PipelineCompiler/glsl/Magnetism.cpp"
	"Engine/PipelineCompiler/glsl/Math.cpp"
	"Engine/PipelineCompiler/glsl/MathDef.cpp"
	"Engine/PipelineCompiler/glsl/Matrix.cpp"
	"Engine/PipelineCompiler/glsl/Perlin.cpp"
	"Engine/PipelineCompiler/glsl/Plane.cpp"
	"Engine/PipelineCompiler/glsl/Quaternion.cpp"
	"Engine/PipelineCompiler/glsl/Ray.cpp"
	"Engine/PipelineCompiler/glsl/Rect.cpp"
	"Engine/PipelineCompiler/glsl/Simplex.cpp"
	"Engine/PipelineCompiler/glsl/Turbulence.cpp"
	"Engine/PipelineCompiler/glsl/TypeInfo.cpp"
	"Engine/PipelineCompiler/glsl/Utils.cpp"
	"Engine/PipelineCompiler/glsl/Voronoi.cpp"
	"Engine/PipelineCompiler/glsl/VoronoiLines.cpp"
	"Engine/PipelineCompiler/glsl/VoronoiNoise.cpp"
	"Engine/PipelineCompiler/glsl/_NoiseUtils.cpp" )
add_library( "Engine.PipelineCompiler" STATIC ${SOURCES} )
source_group( "Serializers" FILES "Engine/PipelineCompiler/Serializers/AngelScriptSerializer.cpp" "Engine/PipelineCompiler/Serializers/AngelScriptSerializer.h" "Engine/PipelineCompiler/Serializers/CppSerializer.cpp" "Engine/PipelineCompiler/Serializers/CppSerializer.h" "Engine/PipelineCompiler/Serializers/ISerializer.h" )
source_group( "Common" FILES "Engine/PipelineCompiler/Common/Common.h" "Engine/PipelineCompiler/Common/Packing.h" "Engine/PipelineCompiler/Common/ShaderEnums.h" "Engine/PipelineCompiler/Common/ToGLSL.h" )
source_group( "Pipelines" FILES "Engine/PipelineCompiler/Pipelines/BasePipeline.cpp" "Engine/PipelineCompiler/Pipelines/BasePipeline.h" "Engine/PipelineCompiler/Pipelines/BasePipeline_Pass1.cpp" "Engine/PipelineCompiler/Pipelines/BasePipeline_Pass2.cpp" "Engine/PipelineCompiler/Pipelines/ComputePipeline.cpp" "Engine/PipelineCompiler/Pipelines/ComputePipeline.h" "Engine/PipelineCompiler/Pipelines/GraphicsPipeline.cpp" "Engine/PipelineCompiler/Pipelines/GraphicsPipeline.h" "Engine/PipelineCompiler/Pipelines/PipelineManager.cpp" "Engine/PipelineCompiler/Pipelines/PipelineManager.h" )
source_group( "" FILES "Engine/PipelineCompiler/README.md" )
source_group( "Shaders" FILES "Engine/PipelineCompiler/Shaders/DeserializedShader.cpp" "Engine/PipelineCompiler/Shaders/DeserializedShader.h" "Engine/PipelineCompiler/Shaders/glslang_Include.h" "Engine/PipelineCompiler/Shaders/ShaderCompiler.cpp" "Engine/PipelineCompiler/Shaders/ShaderCompiler.h" "Engine/PipelineCompiler/Shaders/ShaderCompiler_CLCompiler.cpp" "Engine/PipelineCompiler/Shaders/ShaderCompiler_CLTranslator.cpp" "Engine/PipelineCompiler/Shaders/ShaderCompiler_CPPTranslator.cpp" "Engine/PipelineCompiler/Shaders/ShaderCompiler_GLCompiler.cpp" "Engine/PipelineCompiler/Shaders/ShaderCompiler_GLParser.cpp" "Engine/PipelineCompiler/Shaders/ShaderCompiler_GLSLTranslator.cpp" "Engine/PipelineCompiler/Shaders/ShaderCompiler_HLSLCompiler.cpp" "Engine/PipelineCompiler/Shaders/ShaderCompiler_HLSLTranslator.cpp" "Engine/PipelineCompiler/Shaders/ShaderCompiler_NameValidator.cpp" "Engine/PipelineCompiler/Shaders/ShaderCompiler_NameValidator.h" "Engine/PipelineCompiler/Shaders/ShaderCompiler_ReplaceTypes.cpp" "Engine/PipelineCompiler/Shaders/ShaderCompiler_Translator.cpp" "Engine/PipelineCompiler/Shaders/ShaderCompiler_Translator.h" "Engine/PipelineCompiler/Shaders/ShaderCompiler_Utils.h" "Engine/PipelineCompiler/Shaders/ShaderCompiler_VkCompiler.cpp" )
source_group( "cl" FILES "Engine/PipelineCompiler/cl/cl_source_vfs.cpp" "Engine/PipelineCompiler/cl/cl_source_vfs.h" "Engine/PipelineCompiler/cl/funcs.cpp" "Engine/PipelineCompiler/cl/matrix.cpp" )
source_group( "glsl" FILES "Engine/PipelineCompiler/glsl/AABBox.cpp" "Engine/PipelineCompiler/glsl/Billow.cpp" "Engine/PipelineCompiler/glsl/BitMath.cpp" "Engine/PipelineCompiler/glsl/Blur.cpp" "Engine/PipelineCompiler/glsl/Cellular.cpp" "Engine/PipelineCompiler/glsl/Cmp.cpp" "Engine/PipelineCompiler/glsl/Color.cpp" "Engine/PipelineCompiler/glsl/Colors.cpp" "Engine/PipelineCompiler/glsl/DefaultSDF.cpp" "Engine/PipelineCompiler/glsl/Defines.cpp" "Engine/PipelineCompiler/glsl/DHash.cpp" "Engine/PipelineCompiler/glsl/Emitters.cpp" "Engine/PipelineCompiler/glsl/FBM.cpp" "Engine/PipelineCompiler/glsl/Gabor.cpp" "Engine/PipelineCompiler/glsl/GlobalIndex.cpp" "Engine/PipelineCompiler/glsl/glsl_source_vfs.cpp" "Engine/PipelineCompiler/glsl/glsl_source_vfs.h" "Engine/PipelineCompiler/glsl/Gravity.cpp" "Engine/PipelineCompiler/glsl/Hash.cpp" "Engine/PipelineCompiler/glsl/Hash2.cpp" "Engine/PipelineCompiler/glsl/IHash.cpp" "Engine/PipelineCompiler/glsl/Line2.cpp" "Engine/PipelineCompiler/glsl/Line3.cpp" "Engine/PipelineCompiler/glsl/Magnetism.cpp" "Engine/PipelineCompiler/glsl/Math.cpp" "Engine/PipelineCompiler/glsl/MathDef.cpp" "Engine/PipelineCompiler/glsl/Matrix.cpp" "Engine/PipelineCompiler/glsl/Perlin.cpp" "Engine/PipelineCompiler/glsl/Plane.cpp" "Engine/PipelineCompiler/glsl/Quaternion.cpp" "Engine/PipelineCompiler/glsl/Ray.cpp" "Engine/PipelineCompiler/glsl/Rect.cpp" "Engine/PipelineCompiler/glsl/Simplex.cpp" "Engine/PipelineCompiler/glsl/Turbulence.cpp" "Engine/PipelineCompiler/glsl/TypeInfo.cpp" "Engine/PipelineCompiler/glsl/Utils.cpp" "Engine/PipelineCompiler/glsl/Voronoi.cpp" "Engine/PipelineCompiler/glsl/VoronoiLines.cpp" "Engine/PipelineCompiler/glsl/VoronoiNoise.cpp" "Engine/PipelineCompiler/glsl/_NoiseUtils.cpp" )
set_property( TARGET "Engine.PipelineCompiler" PROPERTY FOLDER "EngineTools" )
target_include_directories( "Engine.PipelineCompiler" PRIVATE "Engine/PipelineCompiler" )
target_include_directories( "Engine.PipelineCompiler" PUBLIC "External" )
target_include_directories( "Engine.PipelineCompiler" PUBLIC "${EXTERNALS_PATH}" )
target_include_directories( "Engine.PipelineCompiler" PUBLIC "Engine/.." )
target_include_directories( "Engine.PipelineCompiler" PRIVATE "${EXTERNALS_PATH}/glslang" )
target_link_libraries( "Engine.PipelineCompiler" "Engine.STL" )
if (D3DCOMPILER_LIBRARY)
	target_link_libraries( "Engine.PipelineCompiler" "${D3DCOMPILER_LIBRARY}" )
endif()
if (ENABLE_GLSLANG)
	target_link_libraries( "Engine.PipelineCompiler" "glsl" )
endif()
# compiler
set_target_properties( "Engine.PipelineCompiler" PROPERTIES LINK_FLAGS_RELEASE ${PROJECTS_SHARED_LINKER_FLAGS_RELEASE} )
set_target_properties( "Engine.PipelineCompiler" PROPERTIES LINK_FLAGS_PROFILE ${PROJECTS_SHARED_LINKER_FLAGS_PROFILE} )
target_compile_definitions( "Engine.PipelineCompiler" PRIVATE $<$<CONFIG:Debug>: ${PROJECTS_SHARED_DEFINES_DEBUG}> )
set_target_properties( "Engine.PipelineCompiler" PROPERTIES LINK_FLAGS_DEBUGANALYZE ${PROJECTS_SHARED_LINKER_FLAGS_DEBUGANALYZE} )
target_compile_definitions( "Engine.PipelineCompiler" PRIVATE $<$<CONFIG:DebugAnalyze>: ${PROJECTS_SHARED_DEFINES_DEBUGANALYZE}> )
target_compile_options( "Engine.PipelineCompiler" PRIVATE $<$<CONFIG:Profile>: ${PROJECTS_SHARED_CXX_FLAGS_PROFILE}> )
target_compile_options( "Engine.PipelineCompiler" PRIVATE $<$<CONFIG:Release>: ${PROJECTS_SHARED_CXX_FLAGS_RELEASE}> )
target_compile_definitions( "Engine.PipelineCompiler" PRIVATE $<$<CONFIG:Profile>: ${PROJECTS_SHARED_DEFINES_PROFILE}> )
target_compile_definitions( "Engine.PipelineCompiler" PRIVATE $<$<CONFIG:Release>: ${PROJECTS_SHARED_DEFINES_RELEASE}> )
set_target_properties( "Engine.PipelineCompiler" PROPERTIES LINK_FLAGS_DEBUG ${PROJECTS_SHARED_LINKER_FLAGS_DEBUG} )
target_compile_options( "Engine.PipelineCompiler" PRIVATE $<$<CONFIG:DebugAnalyze>: ${PROJECTS_SHARED_CXX_FLAGS_DEBUGANALYZE}> )
target_compile_options( "Engine.PipelineCompiler" PRIVATE $<$<CONFIG:Debug>: ${PROJECTS_SHARED_CXX_FLAGS_DEBUG}> )



#==================================================================================================
# project: Engine.Scipt
#==================================================================================================
set( SOURCES 
	"Engine/Script/Impl/ClassBinder.h"
	"Engine/Script/Impl/EnumBinder.h"
	"Engine/Script/Impl/ScriptBinder.cpp"
	"Engine/Script/Impl/ScriptEngine.cpp"
	"Engine/Script/Impl/ScriptEngine.h"
	"Engine/Script/Impl/ScriptEngine.inl.h"
	"Engine/Script/Impl/ScriptModule.cpp"
	"Engine/Script/Impl/ScriptModule.h"
	"Engine/Script/Impl/ScriptTypes.h"
	"Engine/Script/Bindings/BindScalarMath.cpp"
	"Engine/Script/Bindings/BindString.cpp"
	"Engine/Script/Bindings/BindVectorMath.cpp"
	"Engine/Script/Bindings/DefaultBindings.h"
	"Engine/Script/Engine.Script.h" )
add_library( "Engine.Scipt" STATIC ${SOURCES} )
source_group( "Impl" FILES "Engine/Script/Impl/ClassBinder.h" "Engine/Script/Impl/EnumBinder.h" "Engine/Script/Impl/ScriptBinder.cpp" "Engine/Script/Impl/ScriptEngine.cpp" "Engine/Script/Impl/ScriptEngine.h" "Engine/Script/Impl/ScriptEngine.inl.h" "Engine/Script/Impl/ScriptModule.cpp" "Engine/Script/Impl/ScriptModule.h" "Engine/Script/Impl/ScriptTypes.h" )
source_group( "Bindings" FILES "Engine/Script/Bindings/BindScalarMath.cpp" "Engine/Script/Bindings/BindString.cpp" "Engine/Script/Bindings/BindVectorMath.cpp" "Engine/Script/Bindings/DefaultBindings.h" )
source_group( "" FILES "Engine/Script/Engine.Script.h" )
set_property( TARGET "Engine.Scipt" PROPERTY FOLDER "Engine" )
target_include_directories( "Engine.Scipt" PRIVATE "Engine/Script" )
target_include_directories( "Engine.Scipt" PUBLIC "External" )
target_include_directories( "Engine.Scipt" PUBLIC "${EXTERNALS_PATH}" )
target_include_directories( "Engine.Scipt" PUBLIC "Engine/.." )
target_link_libraries( "Engine.Scipt" "Engine.STL" )
target_link_libraries( "Engine.Scipt" "AngelScript" )
# compiler
set_target_properties( "Engine.Scipt" PROPERTIES LINK_FLAGS_RELEASE ${PROJECTS_SHARED_LINKER_FLAGS_RELEASE} )
set_target_properties( "Engine.Scipt" PROPERTIES LINK_FLAGS_PROFILE ${PROJECTS_SHARED_LINKER_FLAGS_PROFILE} )
target_compile_definitions( "Engine.Scipt" PRIVATE $<$<CONFIG:Debug>: ${PROJECTS_SHARED_DEFINES_DEBUG}> )
set_target_properties( "Engine.Scipt" PROPERTIES LINK_FLAGS_DEBUGANALYZE ${PROJECTS_SHARED_LINKER_FLAGS_DEBUGANALYZE} )
target_compile_definitions( "Engine.Scipt" PRIVATE $<$<CONFIG:DebugAnalyze>: ${PROJECTS_SHARED_DEFINES_DEBUGANALYZE}> )
target_compile_options( "Engine.Scipt" PRIVATE $<$<CONFIG:Profile>: ${PROJECTS_SHARED_CXX_FLAGS_PROFILE}> )
target_compile_options( "Engine.Scipt" PRIVATE $<$<CONFIG:Release>: ${PROJECTS_SHARED_CXX_FLAGS_RELEASE}> )
target_compile_definitions( "Engine.Scipt" PRIVATE $<$<CONFIG:Profile>: ${PROJECTS_SHARED_DEFINES_PROFILE}> )
target_compile_definitions( "Engine.Scipt" PRIVATE $<$<CONFIG:Release>: ${PROJECTS_SHARED_DEFINES_RELEASE}> )
set_target_properties( "Engine.Scipt" PROPERTIES LINK_FLAGS_DEBUG ${PROJECTS_SHARED_LINKER_FLAGS_DEBUG} )
target_compile_options( "Engine.Scipt" PRIVATE $<$<CONFIG:DebugAnalyze>: ${PROJECTS_SHARED_CXX_FLAGS_DEBUGANALYZE}> )
target_compile_options( "Engine.Scipt" PRIVATE $<$<CONFIG:Debug>: ${PROJECTS_SHARED_CXX_FLAGS_DEBUG}> )



#==================================================================================================
# project: Tests.Scipt
#==================================================================================================
set( SOURCES 
	"Tests/Script/Common.h"
	"Tests/Script/Main.cpp"
	"Tests/Script/Test_Eval.cpp"
	"Tests/Script/Test_ScriptClass.cpp"
	"Tests/Script/Test_ScriptScalarMath.cpp"
	"Tests/Script/Test_ScriptString.cpp"
	"Tests/Script/Test_ScriptVectorMath.cpp" )
if (DEFINED ANDROID)
	add_library( "Tests.Scipt" SHARED ${SOURCES} )
else()
	add_executable( "Tests.Scipt" ${SOURCES} )
endif()
source_group( "" FILES "Tests/Script/Common.h" "Tests/Script/Main.cpp" "Tests/Script/Test_Eval.cpp" "Tests/Script/Test_ScriptClass.cpp" "Tests/Script/Test_ScriptScalarMath.cpp" "Tests/Script/Test_ScriptString.cpp" "Tests/Script/Test_ScriptVectorMath.cpp" )
set_property( TARGET "Tests.Scipt" PROPERTY FOLDER "Tests" )
target_include_directories( "Tests.Scipt" PRIVATE "Tests/Script" )
target_include_directories( "Tests.Scipt" PUBLIC "External" )
target_include_directories( "Tests.Scipt" PUBLIC "${EXTERNALS_PATH}" )
target_include_directories( "Tests.Scipt" PUBLIC "Engine/.." )
target_link_libraries( "Tests.Scipt" "Engine.Scipt" )
# compiler
set_target_properties( "Tests.Scipt" PROPERTIES LINK_FLAGS_RELEASE ${PROJECTS_SHARED_LINKER_FLAGS_RELEASE} )
set_target_properties( "Tests.Scipt" PROPERTIES LINK_FLAGS_PROFILE ${PROJECTS_SHARED_LINKER_FLAGS_PROFILE} )
target_compile_definitions( "Tests.Scipt" PRIVATE $<$<CONFIG:Debug>: ${PROJECTS_SHARED_DEFINES_DEBUG}> )
set_target_properties( "Tests.Scipt" PROPERTIES LINK_FLAGS_DEBUGANALYZE ${PROJECTS_SHARED_LINKER_FLAGS_DEBUGANALYZE} )
target_compile_definitions( "Tests.Scipt" PRIVATE $<$<CONFIG:DebugAnalyze>: ${PROJECTS_SHARED_DEFINES_DEBUGANALYZE}> )
target_compile_options( "Tests.Scipt" PRIVATE $<$<CONFIG:Profile>: ${PROJECTS_SHARED_CXX_FLAGS_PROFILE}> )
target_compile_options( "Tests.Scipt" PRIVATE $<$<CONFIG:Release>: ${PROJECTS_SHARED_CXX_FLAGS_RELEASE}> )
target_compile_definitions( "Tests.Scipt" PRIVATE $<$<CONFIG:Profile>: ${PROJECTS_SHARED_DEFINES_PROFILE}> )
target_compile_definitions( "Tests.Scipt" PRIVATE $<$<CONFIG:Release>: ${PROJECTS_SHARED_DEFINES_RELEASE}> )
set_target_properties( "Tests.Scipt" PROPERTIES LINK_FLAGS_DEBUG ${PROJECTS_SHARED_LINKER_FLAGS_DEBUG} )
target_compile_options( "Tests.Scipt" PRIVATE $<$<CONFIG:DebugAnalyze>: ${PROJECTS_SHARED_CXX_FLAGS_DEBUGANALYZE}> )
target_compile_options( "Tests.Scipt" PRIVATE $<$<CONFIG:Debug>: ${PROJECTS_SHARED_CXX_FLAGS_DEBUG}> )



#==================================================================================================
# project: Engine.ResourcePacker
#==================================================================================================
set( SOURCES 
	"Engine/ResourcePacker/Images/DevILConverter.cpp"
	"Engine/ResourcePacker/Images/ImageConverter.cpp"
	"Engine/ResourcePacker/Images/ImageConverter.h"
	"Engine/ResourcePacker/FilePacker/BinaryFilePacker.cpp"
	"Engine/ResourcePacker/FilePacker/BinaryFilePacker.h"
	"Engine/ResourcePacker/FilePacker/CppFileSystemPacker.cpp"
	"Engine/ResourcePacker/FilePacker/CppFileSystemPacker.h"
	"Engine/ResourcePacker/FilePacker/IFileSystemPacker.h"
	"Engine/ResourcePacker/Pipelines/PipelineConverter.cpp"
	"Engine/ResourcePacker/Pipelines/PipelineConverter.h"
	"Engine/ResourcePacker/Pipelines/ScriptComputePipeline.cpp"
	"Engine/ResourcePacker/Pipelines/ScriptComputePipeline.h"
	"Engine/ResourcePacker/Pipelines/ScriptGraphicsPipeline.cpp"
	"Engine/ResourcePacker/Pipelines/ScriptGraphicsPipeline.h"
	"Engine/ResourcePacker/Pipelines/ScriptPipeline.cpp"
	"Engine/ResourcePacker/Pipelines/ScriptPipeline.h"
	"Engine/ResourcePacker/Main.cpp"
	"Engine/ResourcePacker/Packer/Common.h"
	"Engine/ResourcePacker/Packer/ResourcePacker.cpp"
	"Engine/ResourcePacker/Packer/ResourcePacker.h"
	"Engine/ResourcePacker/Packer/ScriptHelper.cpp" )
if (DEFINED ANDROID)
	add_library( "Engine.ResourcePacker" SHARED ${SOURCES} )
else()
	add_executable( "Engine.ResourcePacker" ${SOURCES} )
endif()
source_group( "Images" FILES "Engine/ResourcePacker/Images/DevILConverter.cpp" "Engine/ResourcePacker/Images/ImageConverter.cpp" "Engine/ResourcePacker/Images/ImageConverter.h" )
source_group( "FilePacker" FILES "Engine/ResourcePacker/FilePacker/BinaryFilePacker.cpp" "Engine/ResourcePacker/FilePacker/BinaryFilePacker.h" "Engine/ResourcePacker/FilePacker/CppFileSystemPacker.cpp" "Engine/ResourcePacker/FilePacker/CppFileSystemPacker.h" "Engine/ResourcePacker/FilePacker/IFileSystemPacker.h" )
source_group( "Pipelines" FILES "Engine/ResourcePacker/Pipelines/PipelineConverter.cpp" "Engine/ResourcePacker/Pipelines/PipelineConverter.h" "Engine/ResourcePacker/Pipelines/ScriptComputePipeline.cpp" "Engine/ResourcePacker/Pipelines/ScriptComputePipeline.h" "Engine/ResourcePacker/Pipelines/ScriptGraphicsPipeline.cpp" "Engine/ResourcePacker/Pipelines/ScriptGraphicsPipeline.h" "Engine/ResourcePacker/Pipelines/ScriptPipeline.cpp" "Engine/ResourcePacker/Pipelines/ScriptPipeline.h" )
source_group( "" FILES "Engine/ResourcePacker/Main.cpp" )
source_group( "Packer" FILES "Engine/ResourcePacker/Packer/Common.h" "Engine/ResourcePacker/Packer/ResourcePacker.cpp" "Engine/ResourcePacker/Packer/ResourcePacker.h" "Engine/ResourcePacker/Packer/ScriptHelper.cpp" )
set_property( TARGET "Engine.ResourcePacker" PROPERTY FOLDER "EngineTools" )
target_include_directories( "Engine.ResourcePacker" PRIVATE "Engine/ResourcePacker" )
target_include_directories( "Engine.ResourcePacker" PUBLIC "External" )
target_include_directories( "Engine.ResourcePacker" PUBLIC "${EXTERNALS_PATH}" )
target_include_directories( "Engine.ResourcePacker" PUBLIC "Engine/.." )
if (ENABLE_FREEIMAGE)
	target_link_libraries( "Engine.ResourcePacker" "FreeImage" )
endif()
target_link_libraries( "Engine.ResourcePacker" "Engine.Scipt" )
if (ENABLE_LODEPNG)
	target_link_libraries( "Engine.ResourcePacker" "lodepng" )
endif()
if (ENABLE_DEVIL)
	target_link_libraries( "Engine.ResourcePacker" "DevIL" )
endif()
target_link_libraries( "Engine.ResourcePacker" "Engine.PipelineCompiler" )
# compiler
set_target_properties( "Engine.ResourcePacker" PROPERTIES LINK_FLAGS_RELEASE ${PROJECTS_SHARED_LINKER_FLAGS_RELEASE} )
set_target_properties( "Engine.ResourcePacker" PROPERTIES LINK_FLAGS_PROFILE ${PROJECTS_SHARED_LINKER_FLAGS_PROFILE} )
target_compile_definitions( "Engine.ResourcePacker" PRIVATE $<$<CONFIG:Debug>: ${PROJECTS_SHARED_DEFINES_DEBUG}> )
set_target_properties( "Engine.ResourcePacker" PROPERTIES LINK_FLAGS_DEBUGANALYZE ${PROJECTS_SHARED_LINKER_FLAGS_DEBUGANALYZE} )
target_compile_definitions( "Engine.ResourcePacker" PRIVATE $<$<CONFIG:DebugAnalyze>: ${PROJECTS_SHARED_DEFINES_DEBUGANALYZE}> )
target_compile_options( "Engine.ResourcePacker" PRIVATE $<$<CONFIG:Profile>: ${PROJECTS_SHARED_CXX_FLAGS_PROFILE}> )
target_compile_options( "Engine.ResourcePacker" PRIVATE $<$<CONFIG:Release>: ${PROJECTS_SHARED_CXX_FLAGS_RELEASE}> )
target_compile_definitions( "Engine.ResourcePacker" PRIVATE $<$<CONFIG:Profile>: ${PROJECTS_SHARED_DEFINES_PROFILE}> )
target_compile_definitions( "Engine.ResourcePacker" PRIVATE $<$<CONFIG:Release>: ${PROJECTS_SHARED_DEFINES_RELEASE}> )
set_target_properties( "Engine.ResourcePacker" PROPERTIES LINK_FLAGS_DEBUG ${PROJECTS_SHARED_LINKER_FLAGS_DEBUG} )
target_compile_options( "Engine.ResourcePacker" PRIVATE $<$<CONFIG:DebugAnalyze>: ${PROJECTS_SHARED_CXX_FLAGS_DEBUGANALYZE}> )
target_compile_options( "Engine.ResourcePacker" PRIVATE $<$<CONFIG:Debug>: ${PROJECTS_SHARED_CXX_FLAGS_DEBUG}> )

#-----------------------------------
if ( CONFIGURATION_DEPENDENT_PATH )
	set( RESOURCE_PACKER_EXE "${CMAKE_CURRENT_BINARY_DIR}/$<CONFIG>/Engine.ResourcePacker${CMAKE_EXECUTABLE_SUFFIX}" )
else ()
	set( RESOURCE_PACKER_EXE "${CMAKE_CURRENT_BINARY_DIR}/Engine.ResourcePacker${CMAKE_EXECUTABLE_SUFFIX}" )
endif ()

#-----------------------------------


#==================================================================================================
# project: Tests.Engine.Base
#==================================================================================================
set( SOURCES 
	"Tests/Engine.Base/Graphics/GApp.cpp"
	"Tests/Engine.Base/Graphics/GApp.h"
	"Tests/Engine.Base/Graphics/Test.GWindow.cpp"
	"Tests/Engine.Base/Pipelines/all_pipelines.h"
	"Tests/Engine.Base/Pipelines/default.cpp"
	"Tests/Engine.Base/Pipelines/Default.ppln"
	"Tests/Engine.Base/Pipelines/default2.cpp"
	"Tests/Engine.Base/Pipelines/Default2.ppln"
	"Tests/Engine.Base/Pipelines/resources.as"
	"Tests/Engine.Base/Pipelines/shared_types.h"
	"Tests/Engine.Base/Window/Test.Window.cpp"
	"Tests/Engine.Base/Common.h"
	"Tests/Engine.Base/Main.cpp" )
if (DEFINED ANDROID)
	add_library( "Tests.Engine.Base" SHARED ${SOURCES} )
else()
	add_executable( "Tests.Engine.Base" ${SOURCES} )
endif()
source_group( "Graphics" FILES "Tests/Engine.Base/Graphics/GApp.cpp" "Tests/Engine.Base/Graphics/GApp.h" "Tests/Engine.Base/Graphics/Test.GWindow.cpp" )
source_group( "Pipelines" FILES "Tests/Engine.Base/Pipelines/all_pipelines.h" "Tests/Engine.Base/Pipelines/default.cpp" "Tests/Engine.Base/Pipelines/Default.ppln" "Tests/Engine.Base/Pipelines/default2.cpp" "Tests/Engine.Base/Pipelines/Default2.ppln" "Tests/Engine.Base/Pipelines/resources.as" "Tests/Engine.Base/Pipelines/shared_types.h" )
source_group( "Window" FILES "Tests/Engine.Base/Window/Test.Window.cpp" )
source_group( "" FILES "Tests/Engine.Base/Common.h" "Tests/Engine.Base/Main.cpp" )
set_property( TARGET "Tests.Engine.Base" PROPERTY FOLDER "Tests" )
target_include_directories( "Tests.Engine.Base" PRIVATE "Tests/Engine.Base" )
target_include_directories( "Tests.Engine.Base" PUBLIC "External" )
target_include_directories( "Tests.Engine.Base" PUBLIC "${EXTERNALS_PATH}" )
target_include_directories( "Tests.Engine.Base" PUBLIC "Engine/.." )
target_link_libraries( "Tests.Engine.Base" "Engine.Profilers" )
target_link_libraries( "Tests.Engine.Base" "Engine.Platforms" )
add_dependencies( "Tests.Engine.Base" "Deps_Tests.Engine.Base" )
# compiler
set_target_properties( "Tests.Engine.Base" PROPERTIES LINK_FLAGS_RELEASE ${PROJECTS_SHARED_LINKER_FLAGS_RELEASE} )
set_target_properties( "Tests.Engine.Base" PROPERTIES LINK_FLAGS_PROFILE ${PROJECTS_SHARED_LINKER_FLAGS_PROFILE} )
target_compile_definitions( "Tests.Engine.Base" PRIVATE $<$<CONFIG:Debug>: ${PROJECTS_SHARED_DEFINES_DEBUG}> )
set_target_properties( "Tests.Engine.Base" PROPERTIES LINK_FLAGS_DEBUGANALYZE ${PROJECTS_SHARED_LINKER_FLAGS_DEBUGANALYZE} )
target_compile_definitions( "Tests.Engine.Base" PRIVATE $<$<CONFIG:DebugAnalyze>: ${PROJECTS_SHARED_DEFINES_DEBUGANALYZE}> )
target_compile_options( "Tests.Engine.Base" PRIVATE $<$<CONFIG:Profile>: ${PROJECTS_SHARED_CXX_FLAGS_PROFILE}> )
target_compile_options( "Tests.Engine.Base" PRIVATE $<$<CONFIG:Release>: ${PROJECTS_SHARED_CXX_FLAGS_RELEASE}> )
target_compile_definitions( "Tests.Engine.Base" PRIVATE $<$<CONFIG:Profile>: ${PROJECTS_SHARED_DEFINES_PROFILE}> )
target_compile_definitions( "Tests.Engine.Base" PRIVATE $<$<CONFIG:Release>: ${PROJECTS_SHARED_DEFINES_RELEASE}> )
set_target_properties( "Tests.Engine.Base" PROPERTIES LINK_FLAGS_DEBUG ${PROJECTS_SHARED_LINKER_FLAGS_DEBUG} )
target_compile_options( "Tests.Engine.Base" PRIVATE $<$<CONFIG:DebugAnalyze>: ${PROJECTS_SHARED_CXX_FLAGS_DEBUGANALYZE}> )
target_compile_options( "Tests.Engine.Base" PRIVATE $<$<CONFIG:Debug>: ${PROJECTS_SHARED_CXX_FLAGS_DEBUG}> )

#-----------------------------------
if (NOT DEFINED RESOURCE_PACKER_EXE)
	message( FATAL_ERROR "RESOURCE_PACKER_EXE is not defined" )
endif ()
add_custom_target( "Deps_Tests.Engine.Base"
	COMMAND ${RESOURCE_PACKER_EXE} -R "${CMAKE_SOURCE_DIR}/Tests/Engine.Base/Pipelines/resources.as"
	DEPENDS ${RESOURCE_PACKER_EXE} "${CMAKE_SOURCE_DIR}/Tests/Engine.Base/Pipelines/resources.as"
	COMMENT "Pack resources for 'Tests.Engine.Base'..."
	VERBATIM
)
add_dependencies( "Deps_Tests.Engine.Base" "Engine.ResourcePacker" )
set_property( TARGET "Deps_Tests.Engine.Base" PROPERTY FOLDER "_deps_" )

#-----------------------------------


#==================================================================================================
# project: Tests.Engine.Graphics
#==================================================================================================
set( SOURCES 
	"Tests/Engine.Graphics/Pipelines/all_pipelines.h"
	"Tests/Engine.Graphics/Pipelines/default3.cpp"
	"Tests/Engine.Graphics/Pipelines/Default3.ppln"
	"Tests/Engine.Graphics/Pipelines/resources.as"
	"Tests/Engine.Graphics/Pipelines/shared_types.h"
	"Tests/Engine.Graphics/GApp.cpp"
	"Tests/Engine.Graphics/GApp.h"
	"Tests/Engine.Graphics/Main.cpp" )
if (DEFINED ANDROID)
	add_library( "Tests.Engine.Graphics" SHARED ${SOURCES} )
else()
	add_executable( "Tests.Engine.Graphics" ${SOURCES} )
endif()
source_group( "Pipelines" FILES "Tests/Engine.Graphics/Pipelines/all_pipelines.h" "Tests/Engine.Graphics/Pipelines/default3.cpp" "Tests/Engine.Graphics/Pipelines/Default3.ppln" "Tests/Engine.Graphics/Pipelines/resources.as" "Tests/Engine.Graphics/Pipelines/shared_types.h" )
source_group( "" FILES "Tests/Engine.Graphics/GApp.cpp" "Tests/Engine.Graphics/GApp.h" "Tests/Engine.Graphics/Main.cpp" )
set_property( TARGET "Tests.Engine.Graphics" PROPERTY FOLDER "Tests" )
target_include_directories( "Tests.Engine.Graphics" PRIVATE "Tests/Engine.Graphics" )
target_include_directories( "Tests.Engine.Graphics" PUBLIC "External" )
target_include_directories( "Tests.Engine.Graphics" PUBLIC "${EXTERNALS_PATH}" )
target_include_directories( "Tests.Engine.Graphics" PUBLIC "Engine/.." )
target_link_libraries( "Tests.Engine.Graphics" "Engine.Profilers" )
target_link_libraries( "Tests.Engine.Graphics" "Engine.ImportExport" )
add_dependencies( "Tests.Engine.Graphics" "Deps_Tests.Engine.Graphics" )
# compiler
set_target_properties( "Tests.Engine.Graphics" PROPERTIES LINK_FLAGS_RELEASE ${PROJECTS_SHARED_LINKER_FLAGS_RELEASE} )
set_target_properties( "Tests.Engine.Graphics" PROPERTIES LINK_FLAGS_PROFILE ${PROJECTS_SHARED_LINKER_FLAGS_PROFILE} )
target_compile_definitions( "Tests.Engine.Graphics" PRIVATE $<$<CONFIG:Debug>: ${PROJECTS_SHARED_DEFINES_DEBUG}> )
set_target_properties( "Tests.Engine.Graphics" PROPERTIES LINK_FLAGS_DEBUGANALYZE ${PROJECTS_SHARED_LINKER_FLAGS_DEBUGANALYZE} )
target_compile_definitions( "Tests.Engine.Graphics" PRIVATE $<$<CONFIG:DebugAnalyze>: ${PROJECTS_SHARED_DEFINES_DEBUGANALYZE}> )
target_compile_options( "Tests.Engine.Graphics" PRIVATE $<$<CONFIG:Profile>: ${PROJECTS_SHARED_CXX_FLAGS_PROFILE}> )
target_compile_options( "Tests.Engine.Graphics" PRIVATE $<$<CONFIG:Release>: ${PROJECTS_SHARED_CXX_FLAGS_RELEASE}> )
target_compile_definitions( "Tests.Engine.Graphics" PRIVATE $<$<CONFIG:Profile>: ${PROJECTS_SHARED_DEFINES_PROFILE}> )
target_compile_definitions( "Tests.Engine.Graphics" PRIVATE $<$<CONFIG:Release>: ${PROJECTS_SHARED_DEFINES_RELEASE}> )
set_target_properties( "Tests.Engine.Graphics" PROPERTIES LINK_FLAGS_DEBUG ${PROJECTS_SHARED_LINKER_FLAGS_DEBUG} )
target_compile_options( "Tests.Engine.Graphics" PRIVATE $<$<CONFIG:DebugAnalyze>: ${PROJECTS_SHARED_CXX_FLAGS_DEBUGANALYZE}> )
target_compile_options( "Tests.Engine.Graphics" PRIVATE $<$<CONFIG:Debug>: ${PROJECTS_SHARED_CXX_FLAGS_DEBUG}> )

#-----------------------------------
if (NOT DEFINED RESOURCE_PACKER_EXE)
	message( FATAL_ERROR "RESOURCE_PACKER_EXE is not defined" )
endif ()
add_custom_target( "Deps_Tests.Engine.Graphics"
	COMMAND ${RESOURCE_PACKER_EXE} -R "${CMAKE_SOURCE_DIR}/Tests/Engine.Graphics/Pipelines/resources.as"
	DEPENDS ${RESOURCE_PACKER_EXE} "${CMAKE_SOURCE_DIR}/Tests/Engine.Graphics/Pipelines/resources.as"
	COMMENT "Pack resources for 'Tests.Engine.Graphics'..."
	VERBATIM
)
add_dependencies( "Deps_Tests.Engine.Graphics" "Engine.ResourcePacker" )
set_property( TARGET "Deps_Tests.Engine.Graphics" PROPERTY FOLDER "_deps_" )

#-----------------------------------


#==================================================================================================
# project: Tests.Engine.Platforms.GAPI
#==================================================================================================
set( SOURCES 
	"Tests/Engine.Platforms.GAPI/Compute/CApp.cpp"
	"Tests/Engine.Platforms.GAPI/Compute/CApp.h"
	"Tests/Engine.Platforms.GAPI/Compute/CApp_BufferAlign.cpp"
	"Tests/Engine.Platforms.GAPI/Compute/CApp_BufferRange.cpp"
	"Tests/Engine.Platforms.GAPI/Compute/CApp_ClearBuffer.cpp"
	"Tests/Engine.Platforms.GAPI/Compute/CApp_ConvertFloatImage2D.cpp"
	"Tests/Engine.Platforms.GAPI/Compute/CApp_CopyBuffer.cpp"
	"Tests/Engine.Platforms.GAPI/Compute/CApp_CopyBufferToImage2D.cpp"
	"Tests/Engine.Platforms.GAPI/Compute/CApp_CopyImage2D.cpp"
	"Tests/Engine.Platforms.GAPI/Compute/CApp_CopyImage2DToBuffer.cpp"
	"Tests/Engine.Platforms.GAPI/Compute/CApp_DynamicBuffer.cpp"
	"Tests/Engine.Platforms.GAPI/Compute/CApp_Image2DBilinearFilter.cpp"
	"Tests/Engine.Platforms.GAPI/Compute/CApp_Image2DNearestFilter.cpp"
	"Tests/Engine.Platforms.GAPI/Compute/CApp_ShaderBarrier.cpp"
	"Tests/Engine.Platforms.GAPI/Compute/CApp_UpdateBuffer.cpp"
	"Tests/Engine.Platforms.GAPI/Compute/Test.ComputeApi.cpp"
	"Tests/Engine.Platforms.GAPI/Sharing/SApp.cpp"
	"Tests/Engine.Platforms.GAPI/Sharing/SApp.h"
	"Tests/Engine.Platforms.GAPI/Sharing/SApp_BufferSharing.cpp"
	"Tests/Engine.Platforms.GAPI/Sharing/SApp_ImageSharing.cpp"
	"Tests/Engine.Platforms.GAPI/Sharing/Test.Sharing.cpp"
	"Tests/Engine.Platforms.GAPI/Compute/Pipelines/all_pipelines.h"
	"Tests/Engine.Platforms.GAPI/Compute/Pipelines/bufferalign.cpp"
	"Tests/Engine.Platforms.GAPI/Compute/Pipelines/BufferAlign.ppln"
	"Tests/Engine.Platforms.GAPI/Compute/Pipelines/copyfloatimage2d.cpp"
	"Tests/Engine.Platforms.GAPI/Compute/Pipelines/CopyFloatImage2D.ppln"
	"Tests/Engine.Platforms.GAPI/Compute/Pipelines/dynamicbuffer.cpp"
	"Tests/Engine.Platforms.GAPI/Compute/Pipelines/DynamicBuffer.ppln"
	"Tests/Engine.Platforms.GAPI/Compute/Pipelines/image2dbilinearfilter.cpp"
	"Tests/Engine.Platforms.GAPI/Compute/Pipelines/Image2DBilinearFilter.ppln"
	"Tests/Engine.Platforms.GAPI/Compute/Pipelines/image2dnearestfilter.cpp"
	"Tests/Engine.Platforms.GAPI/Compute/Pipelines/Image2DNearestFilter.ppln"
	"Tests/Engine.Platforms.GAPI/Compute/Pipelines/shaderbarrier.cpp"
	"Tests/Engine.Platforms.GAPI/Compute/Pipelines/ShaderBarrier.ppln"
	"Tests/Engine.Platforms.GAPI/Compute/Pipelines/shared_types.h"
	"Tests/Engine.Platforms.GAPI/Graphics/GApp.cpp"
	"Tests/Engine.Platforms.GAPI/Graphics/GApp.h"
	"Tests/Engine.Platforms.GAPI/Graphics/GApp_Texture2DBilinearFilter.cpp"
	"Tests/Engine.Platforms.GAPI/Graphics/GApp_Texture2DNearestFilter.cpp"
	"Tests/Engine.Platforms.GAPI/Graphics/Test.GraphicsApi.cpp"
	"Tests/Engine.Platforms.GAPI/Graphics/Pipelines/all_pipelines.h"
	"Tests/Engine.Platforms.GAPI/Graphics/Pipelines/shared_types.h"
	"Tests/Engine.Platforms.GAPI/Graphics/Pipelines/texture2dbilinearfilter.cpp"
	"Tests/Engine.Platforms.GAPI/Graphics/Pipelines/Texture2DBilinearFilter.ppln"
	"Tests/Engine.Platforms.GAPI/Graphics/Pipelines/texture2dnearestfilter.cpp"
	"Tests/Engine.Platforms.GAPI/Graphics/Pipelines/Texture2DNearestFilter.ppln"
	"Tests/Engine.Platforms.GAPI/Compiler/Pipelines/all_pipelines.h"
	"Tests/Engine.Platforms.GAPI/Compiler/Pipelines/atomicadd.cpp"
	"Tests/Engine.Platforms.GAPI/Compiler/Pipelines/AtomicAdd.ppln"
	"Tests/Engine.Platforms.GAPI/Compiler/Pipelines/findlsb.cpp"
	"Tests/Engine.Platforms.GAPI/Compiler/Pipelines/FindLSB.ppln"
	"Tests/Engine.Platforms.GAPI/Compiler/Pipelines/findmsb.cpp"
	"Tests/Engine.Platforms.GAPI/Compiler/Pipelines/FindMSB.ppln"
	"Tests/Engine.Platforms.GAPI/Compiler/Pipelines/globaltolocal.cpp"
	"Tests/Engine.Platforms.GAPI/Compiler/Pipelines/GlobalToLocal.ppln"
	"Tests/Engine.Platforms.GAPI/Compiler/Pipelines/inlineall.cpp"
	"Tests/Engine.Platforms.GAPI/Compiler/Pipelines/InlineAll.ppln"
	"Tests/Engine.Platforms.GAPI/Compiler/Pipelines/shared_types.h"
	"Tests/Engine.Platforms.GAPI/Compiler/Pipelines/unnamedbuffer.cpp"
	"Tests/Engine.Platforms.GAPI/Compiler/Pipelines/UnnamedBuffer.ppln"
	"Tests/Engine.Platforms.GAPI/Compiler/Pipelines/vecswizzle.cpp"
	"Tests/Engine.Platforms.GAPI/Compiler/Pipelines/VecSwizzle.ppln"
	"Tests/Engine.Platforms.GAPI/Common.h"
	"Tests/Engine.Platforms.GAPI/Main.cpp"
	"Tests/Engine.Platforms.GAPI/resources.as"
	"Tests/Engine.Platforms.GAPI/MultiGPU/Test.MultiGPU.cpp"
	"Tests/Engine.Platforms.GAPI/Compiler/PApp.cpp"
	"Tests/Engine.Platforms.GAPI/Compiler/PApp.h"
	"Tests/Engine.Platforms.GAPI/Compiler/PApp_AtomicAdd.cpp"
	"Tests/Engine.Platforms.GAPI/Compiler/PApp_FindLSB.cpp"
	"Tests/Engine.Platforms.GAPI/Compiler/PApp_FindMSB.cpp"
	"Tests/Engine.Platforms.GAPI/Compiler/PApp_GlobalToLocal.cpp"
	"Tests/Engine.Platforms.GAPI/Compiler/PApp_InlineAll.cpp"
	"Tests/Engine.Platforms.GAPI/Compiler/PApp_UnnamedBuffer.cpp"
	"Tests/Engine.Platforms.GAPI/Compiler/PApp_VecSwizzle.cpp"
	"Tests/Engine.Platforms.GAPI/Compiler/Test.PipelineCompiler.cpp" )
if (DEFINED ANDROID)
	add_library( "Tests.Engine.Platforms.GAPI" SHARED ${SOURCES} )
else()
	add_executable( "Tests.Engine.Platforms.GAPI" ${SOURCES} )
endif()
source_group( "Compute" FILES "Tests/Engine.Platforms.GAPI/Compute/CApp.cpp" "Tests/Engine.Platforms.GAPI/Compute/CApp.h" "Tests/Engine.Platforms.GAPI/Compute/CApp_BufferAlign.cpp" "Tests/Engine.Platforms.GAPI/Compute/CApp_BufferRange.cpp" "Tests/Engine.Platforms.GAPI/Compute/CApp_ClearBuffer.cpp" "Tests/Engine.Platforms.GAPI/Compute/CApp_ConvertFloatImage2D.cpp" "Tests/Engine.Platforms.GAPI/Compute/CApp_CopyBuffer.cpp" "Tests/Engine.Platforms.GAPI/Compute/CApp_CopyBufferToImage2D.cpp" "Tests/Engine.Platforms.GAPI/Compute/CApp_CopyImage2D.cpp" "Tests/Engine.Platforms.GAPI/Compute/CApp_CopyImage2DToBuffer.cpp" "Tests/Engine.Platforms.GAPI/Compute/CApp_DynamicBuffer.cpp" "Tests/Engine.Platforms.GAPI/Compute/CApp_Image2DBilinearFilter.cpp" "Tests/Engine.Platforms.GAPI/Compute/CApp_Image2DNearestFilter.cpp" "Tests/Engine.Platforms.GAPI/Compute/CApp_ShaderBarrier.cpp" "Tests/Engine.Platforms.GAPI/Compute/CApp_UpdateBuffer.cpp" "Tests/Engine.Platforms.GAPI/Compute/Test.ComputeApi.cpp" )
source_group( "Sharing" FILES "Tests/Engine.Platforms.GAPI/Sharing/SApp.cpp" "Tests/Engine.Platforms.GAPI/Sharing/SApp.h" "Tests/Engine.Platforms.GAPI/Sharing/SApp_BufferSharing.cpp" "Tests/Engine.Platforms.GAPI/Sharing/SApp_ImageSharing.cpp" "Tests/Engine.Platforms.GAPI/Sharing/Test.Sharing.cpp" )
source_group( "Compute\\Pipelines" FILES "Tests/Engine.Platforms.GAPI/Compute/Pipelines/all_pipelines.h" "Tests/Engine.Platforms.GAPI/Compute/Pipelines/bufferalign.cpp" "Tests/Engine.Platforms.GAPI/Compute/Pipelines/BufferAlign.ppln" "Tests/Engine.Platforms.GAPI/Compute/Pipelines/copyfloatimage2d.cpp" "Tests/Engine.Platforms.GAPI/Compute/Pipelines/CopyFloatImage2D.ppln" "Tests/Engine.Platforms.GAPI/Compute/Pipelines/dynamicbuffer.cpp" "Tests/Engine.Platforms.GAPI/Compute/Pipelines/DynamicBuffer.ppln" "Tests/Engine.Platforms.GAPI/Compute/Pipelines/image2dbilinearfilter.cpp" "Tests/Engine.Platforms.GAPI/Compute/Pipelines/Image2DBilinearFilter.ppln" "Tests/Engine.Platforms.GAPI/Compute/Pipelines/image2dnearestfilter.cpp" "Tests/Engine.Platforms.GAPI/Compute/Pipelines/Image2DNearestFilter.ppln" "Tests/Engine.Platforms.GAPI/Compute/Pipelines/shaderbarrier.cpp" "Tests/Engine.Platforms.GAPI/Compute/Pipelines/ShaderBarrier.ppln" "Tests/Engine.Platforms.GAPI/Compute/Pipelines/shared_types.h" )
source_group( "Graphics" FILES "Tests/Engine.Platforms.GAPI/Graphics/GApp.cpp" "Tests/Engine.Platforms.GAPI/Graphics/GApp.h" "Tests/Engine.Platforms.GAPI/Graphics/GApp_Texture2DBilinearFilter.cpp" "Tests/Engine.Platforms.GAPI/Graphics/GApp_Texture2DNearestFilter.cpp" "Tests/Engine.Platforms.GAPI/Graphics/Test.GraphicsApi.cpp" )
source_group( "Graphics\\Pipelines" FILES "Tests/Engine.Platforms.GAPI/Graphics/Pipelines/all_pipelines.h" "Tests/Engine.Platforms.GAPI/Graphics/Pipelines/shared_types.h" "Tests/Engine.Platforms.GAPI/Graphics/Pipelines/texture2dbilinearfilter.cpp" "Tests/Engine.Platforms.GAPI/Graphics/Pipelines/Texture2DBilinearFilter.ppln" "Tests/Engine.Platforms.GAPI/Graphics/Pipelines/texture2dnearestfilter.cpp" "Tests/Engine.Platforms.GAPI/Graphics/Pipelines/Texture2DNearestFilter.ppln" )
source_group( "Compiler\\Pipelines" FILES "Tests/Engine.Platforms.GAPI/Compiler/Pipelines/all_pipelines.h" "Tests/Engine.Platforms.GAPI/Compiler/Pipelines/atomicadd.cpp" "Tests/Engine.Platforms.GAPI/Compiler/Pipelines/AtomicAdd.ppln" "Tests/Engine.Platforms.GAPI/Compiler/Pipelines/findlsb.cpp" "Tests/Engine.Platforms.GAPI/Compiler/Pipelines/FindLSB.ppln" "Tests/Engine.Platforms.GAPI/Compiler/Pipelines/findmsb.cpp" "Tests/Engine.Platforms.GAPI/Compiler/Pipelines/FindMSB.ppln" "Tests/Engine.Platforms.GAPI/Compiler/Pipelines/globaltolocal.cpp" "Tests/Engine.Platforms.GAPI/Compiler/Pipelines/GlobalToLocal.ppln" "Tests/Engine.Platforms.GAPI/Compiler/Pipelines/inlineall.cpp" "Tests/Engine.Platforms.GAPI/Compiler/Pipelines/InlineAll.ppln" "Tests/Engine.Platforms.GAPI/Compiler/Pipelines/shared_types.h" "Tests/Engine.Platforms.GAPI/Compiler/Pipelines/unnamedbuffer.cpp" "Tests/Engine.Platforms.GAPI/Compiler/Pipelines/UnnamedBuffer.ppln" "Tests/Engine.Platforms.GAPI/Compiler/Pipelines/vecswizzle.cpp" "Tests/Engine.Platforms.GAPI/Compiler/Pipelines/VecSwizzle.ppln" )
source_group( "" FILES "Tests/Engine.Platforms.GAPI/Common.h" "Tests/Engine.Platforms.GAPI/Main.cpp" "Tests/Engine.Platforms.GAPI/resources.as" )
source_group( "MultiGPU" FILES "Tests/Engine.Platforms.GAPI/MultiGPU/Test.MultiGPU.cpp" )
source_group( "Compiler" FILES "Tests/Engine.Platforms.GAPI/Compiler/PApp.cpp" "Tests/Engine.Platforms.GAPI/Compiler/PApp.h" "Tests/Engine.Platforms.GAPI/Compiler/PApp_AtomicAdd.cpp" "Tests/Engine.Platforms.GAPI/Compiler/PApp_FindLSB.cpp" "Tests/Engine.Platforms.GAPI/Compiler/PApp_FindMSB.cpp" "Tests/Engine.Platforms.GAPI/Compiler/PApp_GlobalToLocal.cpp" "Tests/Engine.Platforms.GAPI/Compiler/PApp_InlineAll.cpp" "Tests/Engine.Platforms.GAPI/Compiler/PApp_UnnamedBuffer.cpp" "Tests/Engine.Platforms.GAPI/Compiler/PApp_VecSwizzle.cpp" "Tests/Engine.Platforms.GAPI/Compiler/Test.PipelineCompiler.cpp" )
set_property( TARGET "Tests.Engine.Platforms.GAPI" PROPERTY FOLDER "Tests" )
target_include_directories( "Tests.Engine.Platforms.GAPI" PRIVATE "Tests/Engine.Platforms.GAPI" )
target_include_directories( "Tests.Engine.Platforms.GAPI" PUBLIC "External" )
target_include_directories( "Tests.Engine.Platforms.GAPI" PUBLIC "${EXTERNALS_PATH}" )
target_include_directories( "Tests.Engine.Platforms.GAPI" PUBLIC "Engine/.." )
target_link_libraries( "Tests.Engine.Platforms.GAPI" "Engine.Platforms" )
add_dependencies( "Tests.Engine.Platforms.GAPI" "Deps_Tests.Engine.Platforms.GAPI" )
# compiler
set_target_properties( "Tests.Engine.Platforms.GAPI" PROPERTIES LINK_FLAGS_RELEASE ${PROJECTS_SHARED_LINKER_FLAGS_RELEASE} )
set_target_properties( "Tests.Engine.Platforms.GAPI" PROPERTIES LINK_FLAGS_PROFILE ${PROJECTS_SHARED_LINKER_FLAGS_PROFILE} )
target_compile_definitions( "Tests.Engine.Platforms.GAPI" PRIVATE $<$<CONFIG:Debug>: ${PROJECTS_SHARED_DEFINES_DEBUG}> )
set_target_properties( "Tests.Engine.Platforms.GAPI" PROPERTIES LINK_FLAGS_DEBUGANALYZE ${PROJECTS_SHARED_LINKER_FLAGS_DEBUGANALYZE} )
target_compile_definitions( "Tests.Engine.Platforms.GAPI" PRIVATE $<$<CONFIG:DebugAnalyze>: ${PROJECTS_SHARED_DEFINES_DEBUGANALYZE}> )
target_compile_options( "Tests.Engine.Platforms.GAPI" PRIVATE $<$<CONFIG:Profile>: ${PROJECTS_SHARED_CXX_FLAGS_PROFILE}> )
target_compile_options( "Tests.Engine.Platforms.GAPI" PRIVATE $<$<CONFIG:Release>: ${PROJECTS_SHARED_CXX_FLAGS_RELEASE}> )
target_compile_definitions( "Tests.Engine.Platforms.GAPI" PRIVATE $<$<CONFIG:Profile>: ${PROJECTS_SHARED_DEFINES_PROFILE}> )
target_compile_definitions( "Tests.Engine.Platforms.GAPI" PRIVATE $<$<CONFIG:Release>: ${PROJECTS_SHARED_DEFINES_RELEASE}> )
set_target_properties( "Tests.Engine.Platforms.GAPI" PROPERTIES LINK_FLAGS_DEBUG ${PROJECTS_SHARED_LINKER_FLAGS_DEBUG} )
target_compile_options( "Tests.Engine.Platforms.GAPI" PRIVATE $<$<CONFIG:DebugAnalyze>: ${PROJECTS_SHARED_CXX_FLAGS_DEBUGANALYZE}> )
target_compile_options( "Tests.Engine.Platforms.GAPI" PRIVATE $<$<CONFIG:Debug>: ${PROJECTS_SHARED_CXX_FLAGS_DEBUG}> )

#-----------------------------------
if (NOT DEFINED RESOURCE_PACKER_EXE)
	message( FATAL_ERROR "RESOURCE_PACKER_EXE is not defined" )
endif ()
add_custom_target( "Deps_Tests.Engine.Platforms.GAPI"
	COMMAND ${RESOURCE_PACKER_EXE} -R "${CMAKE_SOURCE_DIR}/Tests/Engine.Platforms.GAPI/resources.as"
	DEPENDS ${RESOURCE_PACKER_EXE} "${CMAKE_SOURCE_DIR}/Tests/Engine.Platforms.GAPI/resources.as"
	COMMENT "Pack resources for 'Tests.Engine.Platforms.GAPI'..."
	VERBATIM
)
add_dependencies( "Deps_Tests.Engine.Platforms.GAPI" "Engine.ResourcePacker" )
set_property( TARGET "Deps_Tests.Engine.Platforms.GAPI" PROPERTY FOLDER "_deps_" )

#-----------------------------------


#==================================================================================================
# project: Projects.CodeGen
#==================================================================================================
set( SOURCES 
	"Projects/CodeGen/Generators/Bruteforce/Bruteforce.glsl"
	"Projects/CodeGen/Generators/Bruteforce/BruteforceGenerator.cpp"
	"Projects/CodeGen/Generators/Bruteforce/BruteforceGenerator.h"
	"Projects/CodeGen/Generators/Bruteforce/BruteforceGenerator_Compiler.cpp"
	"Projects/CodeGen/Generators/Bruteforce/sw_bruteforce.cpp"
	"Projects/CodeGen/Tests/all_pipelines.h"
	"Projects/CodeGen/Tests/biginttest.cpp"
	"Projects/CodeGen/Tests/BigIntTest.ppln"
	"Projects/CodeGen/Tests/bruteforcetest.cpp"
	"Projects/CodeGen/Tests/BruteforceTest.ppln"
	"Projects/CodeGen/Tests/resources.as"
	"Projects/CodeGen/Tests/shared_types.h"
	"Projects/CodeGen/Tests/Test_BigInteger.cpp"
	"Projects/CodeGen/Tests/Test_Bruteforce.cpp"
	"Projects/CodeGen/Tests/Test_GpuBigInt.cpp"
	"Projects/CodeGen/Tests/Test_GpuBruteforce.cpp"
	"Projects/CodeGen/Generators/Fitness/FitnessFunctions.cpp"
	"Projects/CodeGen/Generators/Fitness/FitnessFunctions.h"
	"Projects/CodeGen/Generators/Fitness/Fitness_Bitwise.glsl"
	"Projects/CodeGen/Generators/Fitness/Fitness_FloatLinear.glsl"
	"Projects/CodeGen/CodeGenApp.cpp"
	"Projects/CodeGen/CodeGenApp.h"
	"Projects/CodeGen/CodeGenApp_Script.cpp"
	"Projects/CodeGen/Common.h"
	"Projects/CodeGen/README.md"
	"Projects/CodeGen/Generators/BigInt.glsl"
	"Projects/CodeGen/Generators/BigInteger.h"
	"Projects/CodeGen/Generators/Commands.cpp"
	"Projects/CodeGen/Generators/Commands.h"
	"Projects/CodeGen/Generators/Constants.inl.h"
	"Projects/CodeGen/Generators/GenFunctionSerializer.cpp"
	"Projects/CodeGen/Generators/GenFunctionSerializer.h"
	"Projects/CodeGen/Generators/IGenerator.h"
	"Projects/CodeGen/Generators/Mixed.h"
	"Projects/CodeGen/Generators/ResultSearch.cpp"
	"Projects/CodeGen/Generators/ResultSearch.h"
	"Projects/CodeGen/Generators/TestCase.h"
	"Projects/CodeGen/Generators/Utils.h" )
if (DEFINED ANDROID)
	add_library( "Projects.CodeGen" SHARED ${SOURCES} )
else()
	add_executable( "Projects.CodeGen" ${SOURCES} )
endif()
source_group( "Generators\\Bruteforce" FILES "Projects/CodeGen/Generators/Bruteforce/Bruteforce.glsl" "Projects/CodeGen/Generators/Bruteforce/BruteforceGenerator.cpp" "Projects/CodeGen/Generators/Bruteforce/BruteforceGenerator.h" "Projects/CodeGen/Generators/Bruteforce/BruteforceGenerator_Compiler.cpp" "Projects/CodeGen/Generators/Bruteforce/sw_bruteforce.cpp" )
source_group( "Tests" FILES "Projects/CodeGen/Tests/all_pipelines.h" "Projects/CodeGen/Tests/biginttest.cpp" "Projects/CodeGen/Tests/BigIntTest.ppln" "Projects/CodeGen/Tests/bruteforcetest.cpp" "Projects/CodeGen/Tests/BruteforceTest.ppln" "Projects/CodeGen/Tests/resources.as" "Projects/CodeGen/Tests/shared_types.h" "Projects/CodeGen/Tests/Test_BigInteger.cpp" "Projects/CodeGen/Tests/Test_Bruteforce.cpp" "Projects/CodeGen/Tests/Test_GpuBigInt.cpp" "Projects/CodeGen/Tests/Test_GpuBruteforce.cpp" )
source_group( "Generators\\Fitness" FILES "Projects/CodeGen/Generators/Fitness/FitnessFunctions.cpp" "Projects/CodeGen/Generators/Fitness/FitnessFunctions.h" "Projects/CodeGen/Generators/Fitness/Fitness_Bitwise.glsl" "Projects/CodeGen/Generators/Fitness/Fitness_FloatLinear.glsl" )
source_group( "" FILES "Projects/CodeGen/CodeGenApp.cpp" "Projects/CodeGen/CodeGenApp.h" "Projects/CodeGen/CodeGenApp_Script.cpp" "Projects/CodeGen/Common.h" "Projects/CodeGen/README.md" )
source_group( "Generators" FILES "Projects/CodeGen/Generators/BigInt.glsl" "Projects/CodeGen/Generators/BigInteger.h" "Projects/CodeGen/Generators/Commands.cpp" "Projects/CodeGen/Generators/Commands.h" "Projects/CodeGen/Generators/Constants.inl.h" "Projects/CodeGen/Generators/GenFunctionSerializer.cpp" "Projects/CodeGen/Generators/GenFunctionSerializer.h" "Projects/CodeGen/Generators/IGenerator.h" "Projects/CodeGen/Generators/Mixed.h" "Projects/CodeGen/Generators/ResultSearch.cpp" "Projects/CodeGen/Generators/ResultSearch.h" "Projects/CodeGen/Generators/TestCase.h" "Projects/CodeGen/Generators/Utils.h" )
set_property( TARGET "Projects.CodeGen" PROPERTY FOLDER "Projects" )
target_include_directories( "Projects.CodeGen" PRIVATE "Projects/CodeGen" )
target_include_directories( "Projects.CodeGen" PUBLIC "External" )
target_include_directories( "Projects.CodeGen" PUBLIC "${EXTERNALS_PATH}" )
target_include_directories( "Projects.CodeGen" PUBLIC "Engine/.." )
target_link_libraries( "Projects.CodeGen" "Engine.Scipt" )
target_link_libraries( "Projects.CodeGen" "Engine.Platforms" )
target_link_libraries( "Projects.CodeGen" "Engine.PipelineCompiler" )
# compiler
set_target_properties( "Projects.CodeGen" PROPERTIES LINK_FLAGS_RELEASE ${PROJECTS_SHARED_LINKER_FLAGS_RELEASE} )
set_target_properties( "Projects.CodeGen" PROPERTIES LINK_FLAGS_PROFILE ${PROJECTS_SHARED_LINKER_FLAGS_PROFILE} )
target_compile_definitions( "Projects.CodeGen" PRIVATE $<$<CONFIG:Debug>: ${PROJECTS_SHARED_DEFINES_DEBUG}> )
set_target_properties( "Projects.CodeGen" PROPERTIES LINK_FLAGS_DEBUGANALYZE ${PROJECTS_SHARED_LINKER_FLAGS_DEBUGANALYZE} )
target_compile_definitions( "Projects.CodeGen" PRIVATE $<$<CONFIG:DebugAnalyze>: ${PROJECTS_SHARED_DEFINES_DEBUGANALYZE}> )
target_compile_options( "Projects.CodeGen" PRIVATE $<$<CONFIG:Profile>: ${PROJECTS_SHARED_CXX_FLAGS_PROFILE}> )
target_compile_options( "Projects.CodeGen" PRIVATE $<$<CONFIG:Release>: ${PROJECTS_SHARED_CXX_FLAGS_RELEASE}> )
target_compile_definitions( "Projects.CodeGen" PRIVATE $<$<CONFIG:Profile>: ${PROJECTS_SHARED_DEFINES_PROFILE}> )
target_compile_definitions( "Projects.CodeGen" PRIVATE $<$<CONFIG:Release>: ${PROJECTS_SHARED_DEFINES_RELEASE}> )
set_target_properties( "Projects.CodeGen" PROPERTIES LINK_FLAGS_DEBUG ${PROJECTS_SHARED_LINKER_FLAGS_DEBUG} )
target_compile_options( "Projects.CodeGen" PRIVATE $<$<CONFIG:DebugAnalyze>: ${PROJECTS_SHARED_CXX_FLAGS_DEBUGANALYZE}> )
target_compile_options( "Projects.CodeGen" PRIVATE $<$<CONFIG:Debug>: ${PROJECTS_SHARED_CXX_FLAGS_DEBUG}> )



#==================================================================================================
# project: Projects.ShaderEditor
#==================================================================================================
set( SOURCES 
	"Projects/ShaderEditor/res/greynoise.png"
	"Projects/ShaderEditor/Pipelines/all_pipelines.h"
	"Projects/ShaderEditor/Pipelines/drawtexturedquad.cpp"
	"Projects/ShaderEditor/Pipelines/DrawTexturedQuad.ppln"
	"Projects/ShaderEditor/Pipelines/experimental.cpp"
	"Projects/ShaderEditor/Pipelines/Experimental.ppln"
	"Projects/ShaderEditor/Pipelines/glowballs.cpp"
	"Projects/ShaderEditor/Pipelines/Glowballs.ppln"
	"Projects/ShaderEditor/Pipelines/ShaderToy.tmpl"
	"Projects/ShaderEditor/Pipelines/ShaderToyComp.tmpl"
	"Projects/ShaderEditor/Pipelines/shared_types.h"
	"Projects/ShaderEditor/Pipelines/sireniandawn1.cpp"
	"Projects/ShaderEditor/Pipelines/SirenianDawn1.ppln"
	"Projects/ShaderEditor/Pipelines/sireniandawn2.cpp"
	"Projects/ShaderEditor/Pipelines/SirenianDawn2.ppln"
	"Projects/ShaderEditor/Pipelines/skyline.cpp"
	"Projects/ShaderEditor/Pipelines/Skyline.ppln"
	"Projects/ShaderEditor/Pipelines/voronnoirecursion.cpp"
	"Projects/ShaderEditor/Pipelines/VoronnoiRecursion.ppln"
	"Projects/ShaderEditor/res/old/img.png"
	"Projects/ShaderEditor/res/old/img1.png"
	"Projects/ShaderEditor/Common.h"
	"Projects/ShaderEditor/README.md"
	"Projects/ShaderEditor/Renderer.cpp"
	"Projects/ShaderEditor/Renderer.h"
	"Projects/ShaderEditor/resources.as"
	"Projects/ShaderEditor/ShaderEditorApp.cpp"
	"Projects/ShaderEditor/ShaderEditorApp.h"
	"Projects/ShaderEditor/ShaderEditorApp_Samples.cpp"
	"Projects/ShaderEditor/images/greynoise.cpp"
	"Projects/ShaderEditor/images/vfs_images_main.cpp"
	"Projects/ShaderEditor/images/vfs_images_main.h" )
if (DEFINED ANDROID)
	add_library( "Projects.ShaderEditor" SHARED ${SOURCES} )
else()
	add_executable( "Projects.ShaderEditor" ${SOURCES} )
endif()
source_group( "res" FILES "Projects/ShaderEditor/res/greynoise.png" )
source_group( "Pipelines" FILES "Projects/ShaderEditor/Pipelines/all_pipelines.h" "Projects/ShaderEditor/Pipelines/drawtexturedquad.cpp" "Projects/ShaderEditor/Pipelines/DrawTexturedQuad.ppln" "Projects/ShaderEditor/Pipelines/experimental.cpp" "Projects/ShaderEditor/Pipelines/Experimental.ppln" "Projects/ShaderEditor/Pipelines/glowballs.cpp" "Projects/ShaderEditor/Pipelines/Glowballs.ppln" "Projects/ShaderEditor/Pipelines/ShaderToy.tmpl" "Projects/ShaderEditor/Pipelines/ShaderToyComp.tmpl" "Projects/ShaderEditor/Pipelines/shared_types.h" "Projects/ShaderEditor/Pipelines/sireniandawn1.cpp" "Projects/ShaderEditor/Pipelines/SirenianDawn1.ppln" "Projects/ShaderEditor/Pipelines/sireniandawn2.cpp" "Projects/ShaderEditor/Pipelines/SirenianDawn2.ppln" "Projects/ShaderEditor/Pipelines/skyline.cpp" "Projects/ShaderEditor/Pipelines/Skyline.ppln" "Projects/ShaderEditor/Pipelines/voronnoirecursion.cpp" "Projects/ShaderEditor/Pipelines/VoronnoiRecursion.ppln" )
source_group( "res\\old" FILES "Projects/ShaderEditor/res/old/img.png" "Projects/ShaderEditor/res/old/img1.png" )
source_group( "" FILES "Projects/ShaderEditor/Common.h" "Projects/ShaderEditor/README.md" "Projects/ShaderEditor/Renderer.cpp" "Projects/ShaderEditor/Renderer.h" "Projects/ShaderEditor/resources.as" "Projects/ShaderEditor/ShaderEditorApp.cpp" "Projects/ShaderEditor/ShaderEditorApp.h" "Projects/ShaderEditor/ShaderEditorApp_Samples.cpp" )
source_group( "images" FILES "Projects/ShaderEditor/images/greynoise.cpp" "Projects/ShaderEditor/images/vfs_images_main.cpp" "Projects/ShaderEditor/images/vfs_images_main.h" )
set_property( TARGET "Projects.ShaderEditor" PROPERTY FOLDER "Projects" )
target_include_directories( "Projects.ShaderEditor" PRIVATE "Projects/ShaderEditor" )
target_include_directories( "Projects.ShaderEditor" PUBLIC "External" )
target_include_directories( "Projects.ShaderEditor" PUBLIC "${EXTERNALS_PATH}" )
target_include_directories( "Projects.ShaderEditor" PUBLIC "Engine/.." )
target_link_libraries( "Projects.ShaderEditor" "Engine.Profilers" )
target_link_libraries( "Projects.ShaderEditor" "Engine.Scene" )
target_link_libraries( "Projects.ShaderEditor" "Engine.ImportExport" )
add_dependencies( "Projects.ShaderEditor" "Deps_Projects.ShaderEditor" )
# compiler
set_target_properties( "Projects.ShaderEditor" PROPERTIES LINK_FLAGS_RELEASE ${PROJECTS_SHARED_LINKER_FLAGS_RELEASE} )
set_target_properties( "Projects.ShaderEditor" PROPERTIES LINK_FLAGS_PROFILE ${PROJECTS_SHARED_LINKER_FLAGS_PROFILE} )
target_compile_definitions( "Projects.ShaderEditor" PRIVATE $<$<CONFIG:Debug>: ${PROJECTS_SHARED_DEFINES_DEBUG}> )
set_target_properties( "Projects.ShaderEditor" PROPERTIES LINK_FLAGS_DEBUGANALYZE ${PROJECTS_SHARED_LINKER_FLAGS_DEBUGANALYZE} )
target_compile_definitions( "Projects.ShaderEditor" PRIVATE $<$<CONFIG:DebugAnalyze>: ${PROJECTS_SHARED_DEFINES_DEBUGANALYZE}> )
target_compile_options( "Projects.ShaderEditor" PRIVATE $<$<CONFIG:Profile>: ${PROJECTS_SHARED_CXX_FLAGS_PROFILE}> )
target_compile_options( "Projects.ShaderEditor" PRIVATE $<$<CONFIG:Release>: ${PROJECTS_SHARED_CXX_FLAGS_RELEASE}> )
target_compile_definitions( "Projects.ShaderEditor" PRIVATE $<$<CONFIG:Profile>: ${PROJECTS_SHARED_DEFINES_PROFILE}> )
target_compile_definitions( "Projects.ShaderEditor" PRIVATE $<$<CONFIG:Release>: ${PROJECTS_SHARED_DEFINES_RELEASE}> )
set_target_properties( "Projects.ShaderEditor" PROPERTIES LINK_FLAGS_DEBUG ${PROJECTS_SHARED_LINKER_FLAGS_DEBUG} )
target_compile_options( "Projects.ShaderEditor" PRIVATE $<$<CONFIG:DebugAnalyze>: ${PROJECTS_SHARED_CXX_FLAGS_DEBUGANALYZE}> )
target_compile_options( "Projects.ShaderEditor" PRIVATE $<$<CONFIG:Debug>: ${PROJECTS_SHARED_CXX_FLAGS_DEBUG}> )

#-----------------------------------
if (NOT DEFINED RESOURCE_PACKER_EXE)
	message( FATAL_ERROR "RESOURCE_PACKER_EXE is not defined" )
endif ()
add_custom_target( "Deps_Projects.ShaderEditor"
	COMMAND ${RESOURCE_PACKER_EXE} -R "${CMAKE_SOURCE_DIR}/Projects/ShaderEditor/resources.as"
	DEPENDS ${RESOURCE_PACKER_EXE} "${CMAKE_SOURCE_DIR}/Projects/ShaderEditor/resources.as"
	COMMENT "Pack resources for 'Projects.ShaderEditor'..."
	VERBATIM
)
add_dependencies( "Deps_Projects.ShaderEditor" "Engine.ResourcePacker" )
set_property( TARGET "Deps_Projects.ShaderEditor" PROPERTY FOLDER "_deps_" )

#-----------------------------------


#==================================================================================================
# project: Utils.CMake
#==================================================================================================
if (NOT DEFINED ANDROID)
	set( SOURCES 
		"Utils/CMake/Tests/GenMGFProject.cpp"
		"Utils/CMake/Tests/Main.cpp"
		"Utils/CMake/Builder/ClangConstants.h"
		"Utils/CMake/Builder/CMakeBuilder.cpp"
		"Utils/CMake/Builder/CMakeBuilder.h"
		"Utils/CMake/Builder/CMakeBuilder_Compiler.cpp"
		"Utils/CMake/Builder/CMakeBuilder_ExternalProjects.cpp"
		"Utils/CMake/Builder/CMakeBuilder_Project.cpp"
		"Utils/CMake/Builder/GccConstants.h"
		"Utils/CMake/Builder/MSVisualStudioConstants.h" )
	if (DEFINED ANDROID)
		add_library( "Utils.CMake" SHARED ${SOURCES} )
	else()
		add_executable( "Utils.CMake" ${SOURCES} )
	endif()
	source_group( "Tests" FILES "Utils/CMake/Tests/GenMGFProject.cpp" "Utils/CMake/Tests/Main.cpp" )
	source_group( "Builder" FILES "Utils/CMake/Builder/ClangConstants.h" "Utils/CMake/Builder/CMakeBuilder.cpp" "Utils/CMake/Builder/CMakeBuilder.h" "Utils/CMake/Builder/CMakeBuilder_Compiler.cpp" "Utils/CMake/Builder/CMakeBuilder_ExternalProjects.cpp" "Utils/CMake/Builder/CMakeBuilder_Project.cpp" "Utils/CMake/Builder/GccConstants.h" "Utils/CMake/Builder/MSVisualStudioConstants.h" )
	set_property( TARGET "Utils.CMake" PROPERTY FOLDER "Utils" )
	target_include_directories( "Utils.CMake" PRIVATE "Utils/CMake" )
	target_include_directories( "Utils.CMake" PUBLIC "External" )
	target_include_directories( "Utils.CMake" PUBLIC "${EXTERNALS_PATH}" )
	target_include_directories( "Utils.CMake" PUBLIC "Engine/.." )
	target_link_libraries( "Utils.CMake" "Engine.STL" )
	# compiler
	set_target_properties( "Utils.CMake" PROPERTIES LINK_FLAGS_RELEASE ${PROJECTS_SHARED_LINKER_FLAGS_RELEASE} )
	set_target_properties( "Utils.CMake" PROPERTIES LINK_FLAGS_PROFILE ${PROJECTS_SHARED_LINKER_FLAGS_PROFILE} )
	target_compile_definitions( "Utils.CMake" PRIVATE $<$<CONFIG:Debug>: ${PROJECTS_SHARED_DEFINES_DEBUG}> )
	set_target_properties( "Utils.CMake" PROPERTIES LINK_FLAGS_DEBUGANALYZE ${PROJECTS_SHARED_LINKER_FLAGS_DEBUGANALYZE} )
	target_compile_definitions( "Utils.CMake" PRIVATE $<$<CONFIG:DebugAnalyze>: ${PROJECTS_SHARED_DEFINES_DEBUGANALYZE}> )
	target_compile_options( "Utils.CMake" PRIVATE $<$<CONFIG:Profile>: ${PROJECTS_SHARED_CXX_FLAGS_PROFILE}> )
	target_compile_options( "Utils.CMake" PRIVATE $<$<CONFIG:Release>: ${PROJECTS_SHARED_CXX_FLAGS_RELEASE}> )
	target_compile_definitions( "Utils.CMake" PRIVATE $<$<CONFIG:Profile>: ${PROJECTS_SHARED_DEFINES_PROFILE}> )
	target_compile_definitions( "Utils.CMake" PRIVATE $<$<CONFIG:Release>: ${PROJECTS_SHARED_DEFINES_RELEASE}> )
	set_target_properties( "Utils.CMake" PROPERTIES LINK_FLAGS_DEBUG ${PROJECTS_SHARED_LINKER_FLAGS_DEBUG} )
	target_compile_options( "Utils.CMake" PRIVATE $<$<CONFIG:DebugAnalyze>: ${PROJECTS_SHARED_CXX_FLAGS_DEBUGANALYZE}> )
	target_compile_options( "Utils.CMake" PRIVATE $<$<CONFIG:Debug>: ${PROJECTS_SHARED_CXX_FLAGS_DEBUG}> )
	
endif()


message( STATUS "project 'ModularGraphicsFramework' generation ended" )
message( STATUS "\n==========================================================================" )

