# auto generated file
cmake_minimum_required (VERSION 3.6.0)

if ( WIN32 )
	set( CMAKE_SYSTEM_VERSION "8.1" CACHE TYPE INTERNAL FORCE )
endif()
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

