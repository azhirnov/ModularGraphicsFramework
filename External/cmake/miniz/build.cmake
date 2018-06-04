
ExternalProject_Add( "External.MiniZ"
		DEPENDS			"External.Download"
		LIST_SEPARATOR	"${EXTERNAL_LIST_SEPARATOR}"
		# configure
		SOURCE_DIR		"${CMAKE_CURRENT_SOURCE_DIR}/miniz"
		CMAKE_GENERATOR	"${CMAKE_GENERATOR}"
		CMAKE_GENERATOR_TOOLSET	"${CMAKE_GENERATOR_TOOLSET}"
		CMAKE_ARGS		"-DCMAKE_CONFIGURATION_TYPES=${EXTERNAL_CONFIGURATION_TYPES}"
						"-DCMAKE_SYSTEM_VERSION=${CMAKE_SYSTEM_VERSION}"
						"-DCMAKE_DEBUG_POSTFIX="
						"-DCMAKE_RELEASE_POSTFIX="
						${EXTERNAL_BUILD_TARGET_FLAGS}
		LOG_CONFIGURE 	1
		# build
		BINARY_DIR		"${CMAKE_BINARY_DIR}/MiniZ_bin"
		BUILD_COMMAND	"${CMAKE_COMMAND}"
						--build .
						--target MiniZ
						--config $<CONFIG>
		LOG_BUILD 		1
		# install
		INSTALL_DIR 	""
		INSTALL_COMMAND ""
		LOG_INSTALL 	1
		# test
		TEST_COMMAND 	""
	)
	
ExternalProject_Get_Property( "External.MiniZ" BINARY_DIR )
set_property( TARGET "External.MiniZ" PROPERTY FOLDER "External" )

set( MINIZ_LIB_NAME "${CMAKE_STATIC_LIBRARY_PREFIX}MiniZ${CMAKE_STATIC_LIBRARY_SUFFIX}" )

add_library( "MiniZ" STATIC IMPORTED GLOBAL )
add_dependencies( "MiniZ" "External.MiniZ" )
set_property( TARGET "MiniZ" PROPERTY IMPORTED_LOCATION "${BINARY_DIR}/lib/${MINIZ_LIB_NAME}" )

if ( ${CONFIGURATION_DEPENDENT_PATH} )
	add_custom_command (
		TARGET "External.MiniZ" POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_if_different
				"${BINARY_DIR}/$<CONFIG>/${MINIZ_LIB_NAME}"
				"${BINARY_DIR}/lib/${MINIZ_LIB_NAME}"
		COMMENT "Copying MiniZ library..."
		VERBATIM
	)
else ()
	add_custom_command (
		TARGET "External.MiniZ" POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_if_different
				"${BINARY_DIR}/${MINIZ_LIB_NAME}"
				"${BINARY_DIR}/lib/${MINIZ_LIB_NAME}"
		COMMENT "Copying MiniZ library..."
		VERBATIM
	)
endif ()
