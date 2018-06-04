cmake_minimum_required (VERSION 3.6.0)

project( "MiniZ" LANGUAGES C )

set( SOURCE_PATH "${EXTERNALS_PATH}/miniz" )

include_directories( "${SOURCE_PATH}" )

add_library( "MiniZ" STATIC
	"${SOURCE_PATH}/miniz.c"
	"${SOURCE_PATH}/miniz.h"
	"${SOURCE_PATH}/miniz_common.h"
	"${SOURCE_PATH}/miniz_tdef.c"
	"${SOURCE_PATH}/miniz_tdef.h"
	"${SOURCE_PATH}/miniz_tinfl.c"
	"${SOURCE_PATH}/miniz_tinfl.h"
	"${SOURCE_PATH}/miniz_zip.c"
	"${SOURCE_PATH}/miniz_zip.h"
)
