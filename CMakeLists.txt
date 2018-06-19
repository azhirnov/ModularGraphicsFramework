# auto generated file
cmake_minimum_required (VERSION 3.6.0)

message( STATUS "==========================================================================\n" )
message( STATUS "project 'ModularGraphicsFramework' generation started" )

project( "ModularGraphicsFramework" LANGUAGES CXX )
set_property( GLOBAL PROPERTY USE_FOLDERS ON )
message( STATUS "Compiler: ${CMAKE_CXX_COMPILER_ID} (${CMAKE_CXX_COMPILER_VERSION})" )
message( STATUS "Compiler name: ${DETECTED_COMPILER}" )
message( STATUS "target system: ${CMAKE_SYSTEM_NAME} (${CMAKE_SYSTEM_VERSION})" )
message( STATUS "host system: ${CMAKE_HOST_SYSTEM_NAME} (${CMAKE_HOST_SYSTEM_VERSION})" )

include( "Core/compilers.cmake" )

set( MAIN_BINARY_DIR "${CMAKE_BINARY_DIR}/bin" CACHE INTERNAL "" FORCE )
set( CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${MAIN_BINARY_DIR}" )
set( CMAKE_LIBRARY_OUTPUT_DIRECTORY "${MAIN_BINARY_DIR}" )
set( CMAKE_RUNTIME_OUTPUT_DIRECTORY "${MAIN_BINARY_DIR}" )
add_subdirectory( "External/cmake" "External_cmake" )
#-----------------------------------
include( "External/cmake/options.cmake" )
#-----------------------------------

add_subdirectory( "Core" "Core" )

add_subdirectory( "Engine" "Engine" )

add_subdirectory( "Projects/ShaderEditor" "Projects_ShaderEditor" )

add_subdirectory( "Utils" "Utils" )

message( STATUS "project 'ModularGraphicsFramework' generation ended" )
message( STATUS "\n==========================================================================" )

