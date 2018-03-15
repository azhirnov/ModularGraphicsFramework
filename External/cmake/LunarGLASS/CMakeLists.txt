# Includes engine dependencies
# https://cmake.org/cmake/help/v3.0/module/ExternalProject.html

cmake_minimum_required (VERSION 3.6.0)

project( "LunarGLASS_Dependencies" LANGUAGES CXX )
set_property( GLOBAL PROPERTY USE_FOLDERS ON )

#----------------------------------------------------------------------------------
# settings

set( SOURCE_PATH "${EXTERNALS_PATH}/LunarGLASS" )

include_directories( "${CMAKE_CURRENT_BINARY_DIR}/LunarGLASS/Core/LLVM/llvm-3.4/include/llvm/.." )
include_directories( "${SOURCE_PATH}/../LunarGLASS/glslang/.." )
include_directories( "${SOURCE_PATH}/../LunarGLASS/.." )
include_directories( "${SOURCE_PATH}/../LunarGLASS/glslang" )
include_directories( "${SOURCE_PATH}/../LunarGLASS" )
include_directories( "${SOURCE_PATH}/../LunarGLASS/Core/LLVM/llvm-3.4/include/llvm/.." )

set( BUILD_TESTING OFF CACHE BOOL "" FORCE )
set( LLVM_INCLUDE_DOCS OFF CACHE BOOL "" FORCE )
set( LLVM_INCLUDE_EXAMPLES OFF CACHE BOOL "" FORCE )
set( LLVM_INCLUDE_TESTS OFF CACHE BOOL "" FORCE )
set( LLVM_INCLUDE_TOOLS OFF CACHE BOOL "" FORCE )
set( SPIRV_SKIP_EXECUTABLES ON CACHE BOOL "" FORCE )
set( SPIRV_SKIP_TESTS ON CACHE BOOL "" FORCE )


#----------------------------------------------------------------------------------
# dependent projects

add_library( "LunarGLASS_Dependencies" STATIC "main.cpp" )

add_subdirectory( "${SOURCE_PATH}" "LunarGLASS" )

add_dependencies( "LunarGLASS_Dependencies" "LLVMSupport" )
add_dependencies( "LunarGLASS_Dependencies" "LLVMTableGen" )
add_dependencies( "LunarGLASS_Dependencies" "llvm-tblgen" )
add_dependencies( "LunarGLASS_Dependencies" "intrinsics_gen" )
add_dependencies( "LunarGLASS_Dependencies" "LLVMCore" )
add_dependencies( "LunarGLASS_Dependencies" "LLVMAnalysis" )
add_dependencies( "LunarGLASS_Dependencies" "LLVMAsmParser" )
add_dependencies( "LunarGLASS_Dependencies" "LLVMipa" )
add_dependencies( "LunarGLASS_Dependencies" "LLVMipo" )
add_dependencies( "LunarGLASS_Dependencies" "LLVMInstCombine" )
add_dependencies( "LunarGLASS_Dependencies" "LLVMLinker" )
add_dependencies( "LunarGLASS_Dependencies" "LLVMScalarOpts" )
add_dependencies( "LunarGLASS_Dependencies" "LLVMTransformUtils" )
add_dependencies( "LunarGLASS_Dependencies" "LLVMTarget" )

add_dependencies( "LunarGLASS_Dependencies" "glslang" )
add_dependencies( "LunarGLASS_Dependencies" "HLSL" )
add_dependencies( "LunarGLASS_Dependencies" "OGLCompiler" )
add_dependencies( "LunarGLASS_Dependencies" "OSDependent" )

add_dependencies( "LunarGLASS_Dependencies" "SPIRV" )
add_dependencies( "LunarGLASS_Dependencies" "SPIRV-Tools" )
add_dependencies( "LunarGLASS_Dependencies" "SPIRV-Tools-opt" )
add_dependencies( "LunarGLASS_Dependencies" "SPVRemapper" )

add_dependencies( "LunarGLASS_Dependencies" "core" )
add_dependencies( "LunarGLASS_Dependencies" "glslangFrontend" )
add_dependencies( "LunarGLASS_Dependencies" "GLSLBackend" )
add_dependencies( "LunarGLASS_Dependencies" "SpvFrontend" )
