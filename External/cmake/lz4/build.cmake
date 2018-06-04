
ExternalProject_Add( "External.LZ4"
		DEPENDS			"External.Download"
		LIST_SEPARATOR	"${EXTERNAL_LIST_SEPARATOR}"
		# configure
		SOURCE_DIR		"${CMAKE_CURRENT_SOURCE_DIR}/lz4"
		CMAKE_GENERATOR	"${CMAKE_GENERATOR}"
		CMAKE_GENERATOR_TOOLSET	"${CMAKE_GENERATOR_TOOLSET}"
		CMAKE_ARGS		"-DCMAKE_CONFIGURATION_TYPES=${EXTERNAL_CONFIGURATION_TYPES}"
						"-DCMAKE_SYSTEM_VERSION=${CMAKE_SYSTEM_VERSION}"
						"-DCMAKE_DEBUG_POSTFIX="
						"-DCMAKE_RELEASE_POSTFIX="
						${EXTERNAL_BUILD_TARGET_FLAGS}
		LOG_CONFIGURE 	1
		# build
		BINARY_DIR		"${CMAKE_BINARY_DIR}/LZ4_bin"
		BUILD_COMMAND	"${CMAKE_COMMAND}"
						--build .
						--target LZ4
						--config $<CONFIG>
		LOG_BUILD 		1
		# install
		INSTALL_DIR 	""
		INSTALL_COMMAND ""
		LOG_INSTALL 	1
		# test
		TEST_COMMAND 	""
	)
	
ExternalProject_Get_Property( "External.LZ4" BINARY_DIR )
set_property( TARGET "External.LZ4" PROPERTY FOLDER "External" )

set( LZ4_LIB_NAME "${CMAKE_STATIC_LIBRARY_PREFIX}LZ4${CMAKE_STATIC_LIBRARY_SUFFIX}" )

add_library( "LZ4" STATIC IMPORTED GLOBAL )
add_dependencies( "LZ4" "External.LZ4" )
set_property( TARGET "LZ4" PROPERTY IMPORTED_LOCATION "${BINARY_DIR}/lib/${LZ4_LIB_NAME}" )

if ( ${CONFIGURATION_DEPENDENT_PATH} )
	add_custom_command (
		TARGET "External.LZ4" POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_if_different
				"${BINARY_DIR}/$<CONFIG>/${LZ4_LIB_NAME}"
				"${BINARY_DIR}/lib/${LZ4_LIB_NAME}"
		COMMENT "Copying LZ4 library..."
		VERBATIM
	)
else ()
	add_custom_command (
		TARGET "External.LZ4" POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_if_different
				"${BINARY_DIR}/${LZ4_LIB_NAME}"
				"${BINARY_DIR}/lib/${LZ4_LIB_NAME}"
		COMMENT "Copying LZ4 library..."
		VERBATIM
	)
endif ()
