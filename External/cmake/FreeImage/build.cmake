
set( FREEIMAGE_TARGET "FreeImage" )

ExternalProject_Add( "External.FreeImage"
		DEPENDS			"External.Download"
		LIST_SEPARATOR	"${EXTERNAL_LIST_SEPARATOR}"
		# configure
		SOURCE_DIR		"${EXTERNALS_PATH}/FreeImage"
		CMAKE_GENERATOR	"${CMAKE_GENERATOR}"
		CMAKE_GENERATOR_TOOLSET	"${CMAKE_GENERATOR_TOOLSET}"
		CMAKE_ARGS		"-DCMAKE_CONFIGURATION_TYPES=${EXTERNAL_CONFIGURATION_TYPES}"
						"-DCMAKE_SYSTEM_VERSION=${CMAKE_SYSTEM_VERSION}"
						"-DCMAKE_DEBUG_POSTFIX="
						"-DCMAKE_RELEASE_POSTFIX="
						${EXTERNAL_BUILD_TARGET_FLAGS}
		LOG_CONFIGURE 	1
		# build
		BINARY_DIR		"${CMAKE_BINARY_DIR}/FreeImage_bin"
		BUILD_COMMAND	"${CMAKE_COMMAND}"
						--build .
						--target ${FREEIMAGE_TARGET}
						--config $<CONFIG>
		LOG_BUILD 		1
		# install
		INSTALL_DIR 	""
		INSTALL_COMMAND ""
		LOG_INSTALL 	1
		# test
		TEST_COMMAND 	""
	)
	
ExternalProject_Get_Property( "External.FreeImage" BINARY_DIR )
set_property( TARGET "External.FreeImage" PROPERTY FOLDER "External" )

set( FREEIMAGE_LIB_NAME "${CMAKE_STATIC_LIBRARY_PREFIX}${FREEIMAGE_TARGET}${CMAKE_STATIC_LIBRARY_SUFFIX}" )

add_library( "FreeImage" STATIC IMPORTED GLOBAL )
add_dependencies( "FreeImage" "External.FreeImage" )
set_property( TARGET "FreeImage" PROPERTY IMPORTED_LOCATION "${BINARY_DIR}/lib/${FREEIMAGE_LIB_NAME}" )

if ( ${CONFIGURATION_DEPENDENT_PATH} )
	add_custom_command (
		TARGET "External.FreeImage" POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_if_different
				"${BINARY_DIR}/$<CONFIG>/${FREEIMAGE_LIB_NAME}"
				"${BINARY_DIR}/lib/${FREEIMAGE_LIB_NAME}"
		COMMENT "Copying FreeImage library..."
		VERBATIM
	)
else ()
	add_custom_command (
		TARGET "External.FreeImage" POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_if_different
				"${BINARY_DIR}/${FREEIMAGE_LIB_NAME}"
				"${BINARY_DIR}/lib/${FREEIMAGE_LIB_NAME}"
		COMMENT "Copying FreeImage library..."
		VERBATIM
	)
endif ()
