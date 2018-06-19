
add_definitions( -DAMD_EXTENSIONS -DNV_EXTENSIONS -DENABLE_HLSL )
add_subdirectory( "${EXTERNALS_PATH}/glslang" "${CMAKE_BINARY_DIR}/glslang_bin" )

add_library( "glsl" INTERFACE )
target_link_libraries( "glsl" INTERFACE "glslang" "HLSL" "OGLCompiler" "OSDependent" "SPIRV" "SPIRV-Tools" "SPIRV-Tools-opt" "SPVRemapper" )
