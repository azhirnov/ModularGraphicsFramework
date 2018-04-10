
set( SDL2_TARGET "SDL2-static" )

ExternalProject_Add( "External.SDL2"
		DEPENDS			"External.Download"
		LIST_SEPARATOR	"${EXTERNAL_LIST_SEPARATOR}"
		# configure
		SOURCE_DIR		"${EXTERNALS_PATH}/SDL2"
		CMAKE_GENERATOR	"${CMAKE_GENERATOR}"
		CMAKE_GENERATOR_TOOLSET	"${CMAKE_GENERATOR_TOOLSET}"
		CMAKE_ARGS		"-DCMAKE_CONFIGURATION_TYPES=${EXTERNAL_CONFIGURATION_TYPES}"
						"-DCMAKE_SYSTEM_VERSION=${CMAKE_SYSTEM_VERSION}"
						"-DCMAKE_DEBUG_POSTFIX="
						"-DCMAKE_RELEASE_POSTFIX="
						${EXTERNAL_BUILD_TARGET_FLAGS}
		LOG_CONFIGURE 	1
		# build
		BINARY_DIR		"${CMAKE_BINARY_DIR}/SDL2_bin"
		BUILD_COMMAND	"${CMAKE_COMMAND}"
						--build .
						--target ${SDL2_TARGET}
						--config $<CONFIG>
		LOG_BUILD 		1
		# install
		INSTALL_DIR 	""
		INSTALL_COMMAND ""
		LOG_INSTALL 	1
		# test
		TEST_COMMAND 	""
	)
	
ExternalProject_Get_Property( "External.SDL2" BINARY_DIR )
set_property( TARGET "External.SDL2" PROPERTY FOLDER "External" )

set( SDL2_LIB_NAME "${CMAKE_STATIC_LIBRARY_PREFIX}${SDL2_TARGET}${CMAKE_STATIC_LIBRARY_SUFFIX}" )

add_library( "SDL2" STATIC IMPORTED GLOBAL )
add_dependencies( "SDL2" "External.SDL2" )
set_property( TARGET "SDL2" PROPERTY IMPORTED_LOCATION "${BINARY_DIR}/lib/${SDL2_LIB_NAME}" )

if (WIN32)
	set_property( TARGET "SDL2" PROPERTY INTERFACE_LINK_LIBRARIES user32 gdi32 winmm imm32 ole32 oleaut32 version uuid )
endif ()

if ( ${CONFIGURATION_DEPENDENT_PATH} )
	add_custom_command (
		TARGET "External.SDL2" POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_if_different
				"${BINARY_DIR}/$<CONFIG>/${SDL2_LIB_NAME}"
				"${BINARY_DIR}/lib/${SDL2_LIB_NAME}"
		COMMENT "Copying SDL2 library..."
		VERBATIM
	)
else ()
	add_custom_command (
		TARGET "External.SDL2" POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_if_different
				"${BINARY_DIR}/${SDL2_LIB_NAME}"
				"${BINARY_DIR}/lib/${SDL2_LIB_NAME}"
		COMMENT "Copying SDL2 library..."
		VERBATIM
	)
endif ()
