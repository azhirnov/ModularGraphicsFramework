# Compile external projects
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
set( ENABLE_LUNARGLASS ON CACHE BOOL "use LunarGLASS" )

if (MSVC)
	set( CONFIGURATION_DEPENDENT_PATH ON )
else()
	set( CONFIGURATION_DEPENDENT_PATH OFF )
endif()


message( STATUS "EXTERNALS_PATH: ${EXTERNALS_PATH}" )

if (NOT ${DOWNLOAD_EXTERNALS} AND NOT EXISTS ${EXTERNALS_PATH})
	message( FATAL_ERROR "Invalid external projects path!" )
endif ()

set( EXTERNAL_LIST_SEPARATOR "|" )
string( REPLACE ";" "${EXTERNAL_LIST_SEPARATOR}" EXTERNAL_CONFIGURATION_TYPES "${CMAKE_CONFIGURATION_TYPES}" )
set( EXTERNAL_BUILD_TARGET_FLAGS "" )

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
	"SDL2/build.cmake"
	"AngelScript/build.cmake"
	"LunarGLASS/build.cmake"
	"LunarGLASS/CMakeLists.txt"
	"LunarGLASS/main.cpp"
)
set_property( TARGET "External.All" PROPERTY FOLDER "External" )
source_group( "" FILES "CMakeLists.txt" "download.cmake" )
source_group( "SDL2" FILES "SDL2/build.cmake" )
source_group( "AngelScript" FILES "AngelScript/build.cmake" )
source_group( "LunarGLASS" FILES "LunarGLASS/build.cmake" "LunarGLASS/CMakeLists.txt" "LunarGLASS/main.cpp" )


# download
if (DOWNLOAD_EXTERNALS)
	include( "download.cmake" )
else()
	add_library( "External.Download" INTERFACE )
endif ()


# SDL2
if (ENABLE_SDL)
	include( "SDL2/build.cmake" )
	add_dependencies( "External.All" SDL2 )
endif ()


# AngelScript
if (ENABLE_ANGELSCRIPT)
	include( "AngelScript/build.cmake" )
	add_dependencies( "External.All" AngelScript )
endif ()


# LunarGLASS
if (ENABLE_LUNARGLASS)
	include( "LunarGLASS/build.cmake" )
	add_dependencies( "External.All" LunarGLASS )
endif ()


# FreeImage


