
set( GLSLANG_OUTPUT "${CMAKE_BINARY_DIR}/glslang_bin" )

# also define this in 'options.cmake' for library compatibility
set( GLSLANG_DEFINES AMD_EXTENSIONS NV_EXTENSIONS ENABLE_HLSL )
string( REPLACE ";" "${EXTERNAL_LIST_SEPARATOR}" GLSLANG_DEFINES "${GLSLANG_DEFINES}" )

ExternalProject_Add( "External.glslang"
		DEPENDS			"External.Download"
		LIST_SEPARATOR	"${EXTERNAL_LIST_SEPARATOR}"
		# configure
		SOURCE_DIR		"${EXTERNALS_PATH}/glslang"
		CMAKE_GENERATOR	"${CMAKE_GENERATOR}"
		CMAKE_GENERATOR_TOOLSET	"${CMAKE_GENERATOR_TOOLSET}"
		CMAKE_ARGS		"-DCMAKE_CONFIGURATION_TYPES=${EXTERNAL_CONFIGURATION_TYPES}"
						"-DCMAKE_SYSTEM_VERSION=${CMAKE_SYSTEM_VERSION}"
						"-DCMAKE_DEBUG_POSTFIX="
						"-DCMAKE_RELEASE_POSTFIX="
						"-DCMAKE_LIBRARY_OUTPUT_DIRECTORY=${GLSLANG_OUTPUT}"
						"-DCMAKE_ARCHIVE_OUTPUT_DIRECTORY=${GLSLANG_OUTPUT}"
						"-DEXTERNALS_PATH=${EXTERNALS_PATH}"
						${EXTERNAL_BUILD_TARGET_FLAGS} ${GLSLANG_DEFINES}
		LOG_CONFIGURE 	1
		# build
		BINARY_DIR		"${GLSLANG_OUTPUT}"
		BUILD_COMMAND	"${CMAKE_COMMAND}"
						--build .
						--target SPIRV
						--config $<CONFIG>
		LOG_BUILD 		1
		# install
		INSTALL_DIR 	""
		INSTALL_COMMAND ""
		LOG_INSTALL 	1
		# test
		TEST_COMMAND 	""
	)
	
ExternalProject_Get_Property( "External.glslang" BINARY_DIR )
set_property( TARGET "External.glslang" PROPERTY FOLDER "External" )

set( GLSLANG_DEPS "glslang" "HLSL" "OGLCompiler" "OSDependent" "SPIRV" "SPIRV-Tools" "SPIRV-Tools-opt" "SPVRemapper" )
set( GLSLANG_DEPS_SRC "" )
set( GLSLANG_DEPS_DST "" )


if ( ${CONFIGURATION_DEPENDENT_PATH} )
	foreach ( DEP ${GLSLANG_DEPS} )
		set( GLSLANG_DEPS_SRC "${GLSLANG_DEPS_SRC}" "${BINARY_DIR}/$<CONFIG>/${CMAKE_STATIC_LIBRARY_PREFIX}${DEP}${CMAKE_STATIC_LIBRARY_SUFFIX}" )
	endforeach ()
else ()
	foreach ( DEP ${GLSLANG_DEPS} )
		set( GLSLANG_DEPS_SRC "${GLSLANG_DEPS_SRC}" "${BINARY_DIR}/${CMAKE_STATIC_LIBRARY_PREFIX}${DEP}${CMAKE_STATIC_LIBRARY_SUFFIX}" )
	endforeach ()
endif ()


foreach ( SRC ${GLSLANG_DEPS_SRC} )
	get_filename_component( DST_NAME "${SRC}" NAME )
	set( DST "${BINARY_DIR}/lib/${DST_NAME}" )
	set( GLSLANG_DEPS_DST "${GLSLANG_DEPS_DST}" "${DST}" )

	add_custom_command (
		TARGET "External.glslang" POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_if_different "${SRC}" "${DST}"
		COMMENT "Copying ${DST_NAME} library..."
		VERBATIM
	)
endforeach ()


add_library( "glsl" INTERFACE )
add_dependencies( "glsl" "External.glslang" )
set_property( TARGET "glsl" PROPERTY INTERFACE_LINK_LIBRARIES ${GLSLANG_DEPS_DST} )

