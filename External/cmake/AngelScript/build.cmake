
set( ANGELSCRIPT_TARGET "angelscript" )

ExternalProject_Add( "External.AngelScript"
		DEPENDS			Download_Externals
		LIST_SEPARATOR	"${EXTERNAL_LIST_SEPARATOR}"
		# configure
		SOURCE_DIR		"${EXTERNALS_PATH}/AngelScript/sdk/angelscript/projects/cmake"
		CMAKE_GENERATOR	"${CMAKE_GENERATOR}"
		CMAKE_GENERATOR_TOOLSET	"${CMAKE_GENERATOR_TOOLSET}"
		CMAKE_ARGS		"-DCMAKE_CONFIGURATION_TYPES=${EXTERNAL_CONFIGURATION_TYPES}"
						"-DCMAKE_SYSTEM_VERSION=${CMAKE_SYSTEM_VERSION}"
						"-DCMAKE_DEBUG_POSTFIX="
						"-DCMAKE_RELEASE_POSTFIX="
						${EXTERNAL_BUILD_TARGET_FLAGS}
		LOG_CONFIGURE 	1
		# build
		BINARY_DIR		"${CMAKE_BINARY_DIR}/AngelScript_bin"
		BUILD_COMMAND	"${CMAKE_COMMAND}"
						--build .
						--target ${ANGELSCRIPT_TARGET}
						--config $<CONFIG>
		LOG_BUILD 		1
		# install
		INSTALL_DIR 	""
		INSTALL_COMMAND ""
		LOG_INSTALL 	1
		# test
		TEST_COMMAND 	""
	)
	
ExternalProject_Get_Property( "External.AngelScript" BINARY_DIR )
set_property( TARGET "External.AngelScript" PROPERTY FOLDER "External" )

set( ANGELSCRIPT_LIB_NAME "${CMAKE_STATIC_LIBRARY_PREFIX}${ANGELSCRIPT_TARGET}${CMAKE_STATIC_LIBRARY_SUFFIX}" )

add_library( "AngelScript" STATIC IMPORTED GLOBAL )
add_dependencies( "AngelScript" "External.AngelScript" )
set_property( TARGET "AngelScript" PROPERTY IMPORTED_LOCATION "${BINARY_DIR}/lib/${ANGELSCRIPT_LIB_NAME}" )

if ( ${CONFIGURATION_DEPENDENT_PATH} )
	add_custom_command (
		TARGET "External.AngelScript" POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy
				"${BINARY_DIR}/$<CONFIG>/${ANGELSCRIPT_LIB_NAME}"
				"${BINARY_DIR}/lib/${ANGELSCRIPT_LIB_NAME}"
		COMMENT "Copying AngelScript library..."
	)
else ()
	add_custom_command (
		TARGET "External.AngelScript" POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy
				"${BINARY_DIR}/${ANGELSCRIPT_LIB_NAME}"
				"${BINARY_DIR}/lib/${ANGELSCRIPT_LIB_NAME}"
		COMMENT "Copying AngelScript library..."
	)
endif ()
