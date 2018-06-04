cmake_minimum_required (VERSION 3.6.0)

project( "LZ4" LANGUAGES C )

set( SOURCE_PATH "${EXTERNALS_PATH}/lz4/lib" )

include_directories( "${SOURCE_PATH}" )

add_definitions( -DLZ4_DLL_EXPORT=1 )

add_library( "LZ4" STATIC
	"${SOURCE_PATH}/lz4.c"
	"${SOURCE_PATH}/lz4.h"
	"${SOURCE_PATH}/lz4frame.c"
	"${SOURCE_PATH}/lz4frame.h"
	"${SOURCE_PATH}/lz4frame_static.h"
	"${SOURCE_PATH}/lz4hc.c"
	"${SOURCE_PATH}/lz4hc.h"
	"${SOURCE_PATH}/xxhash.c"
	"${SOURCE_PATH}/xxhash.h"
)
