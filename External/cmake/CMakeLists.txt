# Compile external projects
# https://cmake.org/cmake/help/v3.0/module/ExternalProject.html

cmake_minimum_required (VERSION 3.6.0)

cmake_policy( SET CMP0022 NEW )

project( "Externals" LANGUAGES CXX )
set_property( GLOBAL PROPERTY USE_FOLDERS ON )


#----------------------------------------------------------------------------------
# setup

set( EXTERNALS_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../downloaded" CACHE STRING "Path to external projects sources" )

set( DOWNLOAD_EXTERNALS ON CACHE BOOL "download source from repository" )
set( ENABLE_SDL ON CACHE BOOL "use SDL2 library" )
set( ENABLE_ANGELSCRIPT ON CACHE BOOL "use AngelScript" )
set( ENABLE_GLSLANG ON CACHE BOOL "use glslang" )
set( ENABLE_FREEIMAGE OFF CACHE BOOL "use FreeImage" )
set( ENABLE_LZ4 OFF CACHE BOOL "use LZ4" )
set( ENABLE_MINIZ ON CACHE BOOL "use MiniZ" )
set( ENABLE_LODEPNG OFF CACHE BOOL "use lodepng" )
set( ENABLE_DEVIL ON CACHE BOOL "use DevIL" )

message( STATUS "EXTERNALS_PATH: ${EXTERNALS_PATH}" )

if (NOT ${DOWNLOAD_EXTERNALS} AND NOT EXISTS ${EXTERNALS_PATH})
	message( FATAL_ERROR "Invalid external projects path!" )
endif ()

set( EXTERNAL_LIST_SEPARATOR "|" )
string( REPLACE ";" "${EXTERNAL_LIST_SEPARATOR}" EXTERNAL_CONFIGURATION_TYPES "${CMAKE_CONFIGURATION_TYPES}" )
set( EXTERNAL_BUILD_TARGET_FLAGS "-DEXTERNALS_PATH=${EXTERNALS_PATH}" )

foreach ( CONFIG ${CMAKE_CONFIGURATION_TYPES} )
	string( TOUPPER ${CONFIG} OUT_CONFIG )

	set( EXTERNAL_BUILD_TARGET_FLAGS
		"${EXTERNAL_BUILD_TARGET_FLAGS}"
		"-DCMAKE_C_FLAGS_${OUT_CONFIG}=${CMAKE_C_FLAGS_${OUT_CONFIG}}"
		"-DCMAKE_CXX_FLAGS_${OUT_CONFIG}=${CMAKE_CXX_FLAGS_${OUT_CONFIG}}"
		"-DCMAKE_EXE_LINKER_FLAGS_${OUT_CONFIG}=${CMAKE_EXE_LINKER_FLAGS_${OUT_CONFIG}}"
		"-DCMAKE_SHARED_LINKER_FLAGS_${OUT_CONFIG}=${CMAKE_SHARED_LINKER_FLAGS_${OUT_CONFIG}}" )
endforeach ()


#----------------------------------------------------------------------------------
# projects

include( ExternalProject )

# AllExternals
add_library( "External.All" STATIC
	"CMakeLists.txt"
	"download.cmake"
	"options.cmake"
	"SDL2/build.cmake"
	"AngelScript/build.cmake"
	"AngelScript/CMakeLists.txt"
	"glslang/build.cmake"
	"FreeImage/build.cmake"
	"lz4/build.cmake"
	"lz4/CMakeLists.txt"
	"miniz/build.cmake"
	"miniz/CMakeLists.txt"
	"lodepng/build.cmake"
	"DevIL/build.cmake"
	"DirectX/dx11.cmake"
)
set_target_properties( "External.All" PROPERTIES LINKER_LANGUAGE CXX )
set_property( TARGET "External.All" PROPERTY FOLDER "External" )
source_group( "" FILES "CMakeLists.txt" "download.cmake" "options.cmake" )
source_group( "SDL2" FILES "SDL2/build.cmake" )
source_group( "AngelScript" FILES "AngelScript/build.cmake" "AngelScript/CMakeLists.txt" )
source_group( "glslang" FILES "glslang/build.cmake" )
source_group( "FreeImage" FILES "FreeImage/build.cmake" )
source_group( "lz4" FILES "lz4/build.cmake" "lz4/CMakeLists.txt" )
source_group( "miniz" FILES "miniz/build.cmake" "miniz/CMakeLists.txt" )
source_group( "lodepng" FILES "lodepng/build.cmake" )
source_group( "DevIL" FILES "DevIL/build.cmake" )
source_group( "DirectX" FILES "DirectX/dx11.cmake" )


# download
if (${DOWNLOAD_EXTERNALS})
	include( "download.cmake" )
else()
	add_library( "External.Download" INTERFACE )
endif ()


# SDL2
if (${ENABLE_SDL})
	include( "SDL2/build.cmake" )
	add_dependencies( "External.All" SDL2 )
endif ()


# AngelScript
if (${ENABLE_ANGELSCRIPT})
	include( "AngelScript/build.cmake" )
	add_dependencies( "External.All" AngelScript )
endif ()


# glslang
if (${ENABLE_GLSLANG})
	include( "glslang/build.cmake" )
	add_dependencies( "External.All" glsl )
endif ()


# FreeImage
if (${ENABLE_FREEIMAGE})
	include( "FreeImage/build.cmake" )
	add_dependencies( "External.All" FreeImage )
endif ()


# lz4
if (${ENABLE_LZ4})
	include( "lz4/build.cmake" )
	add_dependencies( "External.All" LZ4 )
endif ()


# MiniZ
if (${ENABLE_MINIZ})
	include( "miniz/build.cmake" )
	add_dependencies( "External.All" MiniZ )
endif ()


# lodepng
if (${ENABLE_LODEPNG})
	include( "lodepng/build.cmake" )
	add_dependencies( "External.All" lodepng )
endif ()


# DevIL
if (${ENABLE_DEVIL})
	include( "DevIL/build.cmake" )
	add_dependencies( "External.All" DevIL )
endif ()


# DirectX
include( "DirectX/dx11.cmake" )
