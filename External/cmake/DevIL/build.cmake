
if (WIN32)

	set( DEVIL_LIBS_NAMES "DevIL" "ILU" )	# ILUT
	set( DEVIL_LIBS "" )
	set( DEVIL_DLLS "" )

	if ("${CMAKE_SIZEOF_VOID_P}" STREQUAL "4")
		set( DEVIL_LIB_PATH "${EXTERNALS_PATH}/DevIL/lib/x86/Release" )
	elseif ("${CMAKE_SIZEOF_VOID_P}" STREQUAL "8")
		set( DEVIL_LIB_PATH "${EXTERNALS_PATH}/DevIL/lib/x64/Release" )
	else ()
		message( FATAL_ERROR "can't detect 32 or 64-bit platform" )
	endif()
	
	foreach ( LIB ${DEVIL_LIBS_NAMES} )
		set( DEVIL_LIBS "${DEVIL_LIBS}" "${DEVIL_LIB_PATH}/${LIB}.lib" )
		set( DEVIL_DLLS "${DEVIL_DLLS}" "${DEVIL_LIB_PATH}/${LIB}.dll" )
	endforeach ()

else ()
	message( FATAL_ERROR "no DevIL libraries for this platform!" )
endif ()


add_library( "External.DevIL" STATIC "DevIL/main.cpp" )
set_property( TARGET "External.DevIL" PROPERTY FOLDER "External" )

foreach ( SRC ${DEVIL_DLLS} )
	get_filename_component( DST_NAME "${SRC}" NAME )

	if ( ${CONFIGURATION_DEPENDENT_PATH} )
		set( DST "${CMAKE_BINARY_DIR}/$<CONFIG>/${DST_NAME}" )
	else ()
		set( DST "./${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${DST_NAME}" )
	endif ()

	add_custom_command (
		TARGET "External.DevIL" POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_if_different "${SRC}" "${DST}"
		COMMENT "Copying ${DST_NAME} library..."
		VERBATIM
	)
endforeach ()

add_library( "DevIL" INTERFACE )
add_dependencies( "DevIL" "External.DevIL" )
set_property( TARGET "DevIL" PROPERTY INTERFACE_LINK_LIBRARIES ${DEVIL_LIBS} )

# TODO: copy dll to binary directory
