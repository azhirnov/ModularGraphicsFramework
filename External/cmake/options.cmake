
if (${ENABLE_SDL})
	add_definitions( -DPLATFORM_SDL )
endif ()

if (${ENABLE_ANGELSCRIPT})
	add_definitions( -DGX_ENABLE_ANGELSCRIPT )
endif ()

if (${ENABLE_GLSLANG})
	add_definitions( -DGX_ENABLE_GLSLANG -DAMD_EXTENSIONS -DNV_EXTENSIONS -DENABLE_HLSL )
endif ()

if (${ENABLE_FREEIMAGE})
	add_definitions( -DGX_ENABLE_FREEIMAGE )
endif ()

if (${ENABLE_LODEPNG})
	add_definitions( -DGX_ENABLE_LODEPNG )
endif ()

if (${ENABLE_DEVIL})
	add_definitions( -DGX_ENABLE_DEVIL )
endif ()

if (${DOWNLOAD_EXTERNALS} OR EXISTS "${EXTERNALS_PATH}/glm")
	add_definitions( -DGX_ENABLE_GLM )
endif ()

if (${ENABLE_LZ4})
	add_definitions( -DGX_ENABLE_LZ4 )
endif ()

if (${ENABLE_MINIZ})
	add_definitions( -DGX_ENABLE_MINIZ )
endif ()

if (D3DCOMPILER_LIBRARY)
	add_definitions( -DGX_ENABLE_DXCOMPILER )
endif ()
