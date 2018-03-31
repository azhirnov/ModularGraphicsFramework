cmake_minimum_required (VERSION 3.6.0)

project( "AngelScript_Dependencies" LANGUAGES CXX )
set_property( GLOBAL PROPERTY USE_FOLDERS ON )

#----------------------------------------------------------------------------------
# settings

set( SOURCE_PATH "${EXTERNALS_PATH}/AngelScript" )


#----------------------------------------------------------------------------------
# dependent projects

add_definitions( -DAS_USE_NAMESPACE )

add_subdirectory( "${EXTERNALS_PATH}/AngelScript/sdk/angelscript/projects/cmake" "angelscript" )

add_library( "AngelScript_Dependencies" STATIC
	"${SOURCE_PATH}/sdk/add_on/scriptstdstring/scriptstdstring.cpp"
	"${SOURCE_PATH}/sdk/add_on/scriptstdstring/scriptstdstring.h"
	"${SOURCE_PATH}/sdk/add_on/scriptstdstring/scriptstdstring_utils.cpp"
)

target_include_directories( "AngelScript_Dependencies" PRIVATE "${SOURCE_PATH}/sdk/angelscript/include" )

target_link_libraries( "AngelScript_Dependencies" PUBLIC "angelscript" )
