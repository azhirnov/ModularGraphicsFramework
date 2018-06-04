
set( LODEPNG_TARGET "lodepng" )

ExternalProject_Add( "External.lodepng"
		DEPENDS			"External.Download"
		LIST_SEPARATOR	"${EXTERNAL_LIST_SEPARATOR}"
		# configure
		SOURCE_DIR		"${EXTERNALS_PATH}/lodepng"
		CMAKE_GENERATOR	"${CMAKE_GENERATOR}"
		CMAKE_GENERATOR_TOOLSET	"${CMAKE_GENERATOR_TOOLSET}"
		CMAKE_ARGS		"-DCMAKE_CONFIGURATION_TYPES=${EXTERNAL_CONFIGURATION_TYPES}"
						"-DCMAKE_SYSTEM_VERSION=${CMAKE_SYSTEM_VERSION}"
						"-DCMAKE_DEBUG_POSTFIX="
						"-DCMAKE_RELEASE_POSTFIX="
						${EXTERNAL_BUILD_TARGET_FLAGS}
		LOG_CONFIGURE 	1
		# build
		BINARY_DIR		"${CMAKE_BINARY_DIR}/lodepng_bin"
		BUILD_COMMAND	"${CMAKE_COMMAND}"
						--build .
						--target ${LODEPNG_TARGET}
						--config $<CONFIG>
		LOG_BUILD 		1
		# install
		INSTALL_DIR 	""
		INSTALL_COMMAND ""
		LOG_INSTALL 	1
		# test
		TEST_COMMAND 	""
	)
	
ExternalProject_Get_Property( "External.lodepng" BINARY_DIR )
set_property( TARGET "External.lodepng" PROPERTY FOLDER "External" )

set( LODEPNG_LIB_NAME "${CMAKE_STATIC_LIBRARY_PREFIX}${LODEPNG_TARGET}${CMAKE_STATIC_LIBRARY_SUFFIX}" )

add_library( "lodepng" STATIC IMPORTED GLOBAL )
add_dependencies( "lodepng" "External.lodepng" )
set_property( TARGET "lodepng" PROPERTY IMPORTED_LOCATION "${BINARY_DIR}/lib/${LODEPNG_LIB_NAME}" )

if ( ${CONFIGURATION_DEPENDENT_PATH} )
	add_custom_command (
		TARGET "External.lodepng" POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_if_different
				"${BINARY_DIR}/$<CONFIG>/${LODEPNG_LIB_NAME}"
				"${BINARY_DIR}/lib/${LODEPNG_LIB_NAME}"
		COMMENT "Copying lodepng library..."
		VERBATIM
	)
else ()
	add_custom_command (
		TARGET "External.lodepng" POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_if_different
				"${BINARY_DIR}/${LODEPNG_LIB_NAME}"
				"${BINARY_DIR}/lib/${LODEPNG_LIB_NAME}"
		COMMENT "Copying lodepng library..."
		VERBATIM
	)
endif ()
