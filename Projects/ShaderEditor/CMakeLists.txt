# auto generated file
cmake_minimum_required (VERSION 3.6.0)

#==================================================================================================
# project: Projects.ShaderEditor
#==================================================================================================
set( SOURCES 
	"res/greynoise.png"
	"Pipelines/all_pipelines.h"
	"Pipelines/drawtexturedquad.cpp"
	"Pipelines/DrawTexturedQuad.ppln"
	"Pipelines/experimental.cpp"
	"Pipelines/Experimental.ppln"
	"Pipelines/glowballs.cpp"
	"Pipelines/Glowballs.ppln"
	"Pipelines/ShaderToy.tmpl"
	"Pipelines/ShaderToyComp.tmpl"
	"Pipelines/shared_types.h"
	"Pipelines/sireniandawn1.cpp"
	"Pipelines/SirenianDawn1.ppln"
	"Pipelines/sireniandawn2.cpp"
	"Pipelines/SirenianDawn2.ppln"
	"Pipelines/skyline.cpp"
	"Pipelines/Skyline.ppln"
	"Pipelines/voronnoirecursion.cpp"
	"Pipelines/VoronnoiRecursion.ppln"
	"CMakeLists.txt"
	"Common.h"
	"README.md"
	"Renderer.cpp"
	"Renderer.h"
	"resources.as"
	"ShaderEditorApp.cpp"
	"ShaderEditorApp.h"
	"ShaderEditorApp_Samples.cpp"
	"images/greynoise.cpp"
	"images/vfs_images_main.cpp"
	"images/vfs_images_main.h" )
if (DEFINED ANDROID)
	add_library( "Projects.ShaderEditor" SHARED ${SOURCES} )
else()
	add_executable( "Projects.ShaderEditor" ${SOURCES} )
endif()
source_group( "res" FILES "res/greynoise.png" )
source_group( "Pipelines" FILES "Pipelines/all_pipelines.h" "Pipelines/drawtexturedquad.cpp" "Pipelines/DrawTexturedQuad.ppln" "Pipelines/experimental.cpp" "Pipelines/Experimental.ppln" "Pipelines/glowballs.cpp" "Pipelines/Glowballs.ppln" "Pipelines/ShaderToy.tmpl" "Pipelines/ShaderToyComp.tmpl" "Pipelines/shared_types.h" "Pipelines/sireniandawn1.cpp" "Pipelines/SirenianDawn1.ppln" "Pipelines/sireniandawn2.cpp" "Pipelines/SirenianDawn2.ppln" "Pipelines/skyline.cpp" "Pipelines/Skyline.ppln" "Pipelines/voronnoirecursion.cpp" "Pipelines/VoronnoiRecursion.ppln" )
source_group( "" FILES "CMakeLists.txt" "Common.h" "README.md" "Renderer.cpp" "Renderer.h" "resources.as" "ShaderEditorApp.cpp" "ShaderEditorApp.h" "ShaderEditorApp_Samples.cpp" )
source_group( "images" FILES "images/greynoise.cpp" "images/vfs_images_main.cpp" "images/vfs_images_main.h" )
set_property( TARGET "Projects.ShaderEditor" PROPERTY FOLDER "Projects" )
target_include_directories( "Projects.ShaderEditor" PUBLIC "../../Core/.." )
target_include_directories( "Projects.ShaderEditor" PUBLIC "" )
target_include_directories( "Projects.ShaderEditor" PUBLIC "${EXTERNALS_PATH}" )
target_include_directories( "Projects.ShaderEditor" PUBLIC "../../External" )
target_link_libraries( "Projects.ShaderEditor" "Engine.ImportExport" )
target_link_libraries( "Projects.ShaderEditor" "Engine.Profilers" )
target_link_libraries( "Projects.ShaderEditor" "Engine.Scene" )
add_dependencies( "Projects.ShaderEditor" Deps_Projects.ShaderEditor )
# compiler
target_compile_options( "Projects.ShaderEditor" PRIVATE $<$<CONFIG:DebugAnalyze>: ${PROJECTS_SHARED_CXX_FLAGS_DEBUGANALYZE}> )
target_compile_definitions( "Projects.ShaderEditor" PRIVATE $<$<CONFIG:Debug>: ${PROJECTS_SHARED_DEFINES_DEBUG}> )
target_compile_definitions( "Projects.ShaderEditor" PRIVATE $<$<CONFIG:DebugAnalyze>: ${PROJECTS_SHARED_DEFINES_DEBUGANALYZE}> )
set_target_properties( "Projects.ShaderEditor" PROPERTIES LINK_FLAGS_RELEASE ${PROJECTS_SHARED_LINKER_FLAGS_RELEASE} )
target_compile_definitions( "Projects.ShaderEditor" PRIVATE $<$<CONFIG:Profile>: ${PROJECTS_SHARED_DEFINES_PROFILE}> )
target_compile_options( "Projects.ShaderEditor" PRIVATE $<$<CONFIG:Release>: ${PROJECTS_SHARED_CXX_FLAGS_RELEASE}> )
set_target_properties( "Projects.ShaderEditor" PROPERTIES LINK_FLAGS_DEBUG ${PROJECTS_SHARED_LINKER_FLAGS_DEBUG} )
set_target_properties( "Projects.ShaderEditor" PROPERTIES LINK_FLAGS_DEBUGANALYZE ${PROJECTS_SHARED_LINKER_FLAGS_DEBUGANALYZE} )
target_compile_options( "Projects.ShaderEditor" PRIVATE $<$<CONFIG:Profile>: ${PROJECTS_SHARED_CXX_FLAGS_PROFILE}> )
set_target_properties( "Projects.ShaderEditor" PROPERTIES LINK_FLAGS_PROFILE ${PROJECTS_SHARED_LINKER_FLAGS_PROFILE} )
target_compile_options( "Projects.ShaderEditor" PRIVATE $<$<CONFIG:Debug>: ${PROJECTS_SHARED_CXX_FLAGS_DEBUG}> )
target_compile_definitions( "Projects.ShaderEditor" PRIVATE $<$<CONFIG:Release>: ${PROJECTS_SHARED_DEFINES_RELEASE}> )

#-----------------------------------
if (NOT DEFINED RESOURCE_PACKER_EXE)
	message( FATAL_ERROR "RESOURCE_PACKER_EXE is not defined" )
endif ()
add_custom_target( "Deps_Projects.ShaderEditor"
	COMMAND ${RESOURCE_PACKER_EXE} -R "${CMAKE_CURRENT_SOURCE_DIR}/resources.as"
	DEPENDS ${RESOURCE_PACKER_EXE} "${CMAKE_CURRENT_SOURCE_DIR}/resources.as"
	COMMENT "Pack resources for 'Projects.ShaderEditor'..."
	VERBATIM
)
add_dependencies( "Deps_Projects.ShaderEditor" "Engine.ResourcePacker" )
set_property( TARGET "Deps_Projects.ShaderEditor" PROPERTY FOLDER "_deps_" )

#-----------------------------------


