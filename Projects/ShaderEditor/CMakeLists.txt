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
	"res/old/img.png"
	"res/old/img1.png"
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
source_group( "res\\old" FILES "res/old/img.png" "res/old/img1.png" )
source_group( "" FILES "CMakeLists.txt" "Common.h" "README.md" "Renderer.cpp" "Renderer.h" "resources.as" "ShaderEditorApp.cpp" "ShaderEditorApp.h" "ShaderEditorApp_Samples.cpp" )
source_group( "images" FILES "images/greynoise.cpp" "images/vfs_images_main.cpp" "images/vfs_images_main.h" )
set_property( TARGET "Projects.ShaderEditor" PROPERTY FOLDER "Projects" )
target_include_directories( "Projects.ShaderEditor" PUBLIC "C:/Projects/ModularGraphicsFramework/Projects/ShaderEditor" )
target_include_directories( "Projects.ShaderEditor" PUBLIC "C:/Projects/ModularGraphicsFramework/External" )
target_include_directories( "Projects.ShaderEditor" PUBLIC "${EXTERNALS_PATH}" )
target_include_directories( "Projects.ShaderEditor" PUBLIC "C:/Projects/ModularGraphicsFramework/Core/.." )
target_link_libraries( "Projects.ShaderEditor" "Engine.Profilers" )
target_link_libraries( "Projects.ShaderEditor" "Engine.Scene" )
target_link_libraries( "Projects.ShaderEditor" "Engine.ImportExport" )
# compiler
set_target_properties( "Projects.ShaderEditor" PROPERTIES LINK_FLAGS_RELEASE ${PROJECTS_SHARED_LINKER_FLAGS_RELEASE} )
set_target_properties( "Projects.ShaderEditor" PROPERTIES LINK_FLAGS_PROFILE ${PROJECTS_SHARED_LINKER_FLAGS_PROFILE} )
target_compile_definitions( "Projects.ShaderEditor" PRIVATE $<$<CONFIG:Debug>: ${PROJECTS_SHARED_DEFINES_DEBUG}> )
set_target_properties( "Projects.ShaderEditor" PROPERTIES LINK_FLAGS_DEBUGANALYZE ${PROJECTS_SHARED_LINKER_FLAGS_DEBUGANALYZE} )
target_compile_definitions( "Projects.ShaderEditor" PRIVATE $<$<CONFIG:DebugAnalyze>: ${PROJECTS_SHARED_DEFINES_DEBUGANALYZE}> )
target_compile_options( "Projects.ShaderEditor" PRIVATE $<$<CONFIG:Profile>: ${PROJECTS_SHARED_CXX_FLAGS_PROFILE}> )
target_compile_options( "Projects.ShaderEditor" PRIVATE $<$<CONFIG:Release>: ${PROJECTS_SHARED_CXX_FLAGS_RELEASE}> )
target_compile_definitions( "Projects.ShaderEditor" PRIVATE $<$<CONFIG:Profile>: ${PROJECTS_SHARED_DEFINES_PROFILE}> )
target_compile_definitions( "Projects.ShaderEditor" PRIVATE $<$<CONFIG:Release>: ${PROJECTS_SHARED_DEFINES_RELEASE}> )
set_target_properties( "Projects.ShaderEditor" PROPERTIES LINK_FLAGS_DEBUG ${PROJECTS_SHARED_LINKER_FLAGS_DEBUG} )
target_compile_options( "Projects.ShaderEditor" PRIVATE $<$<CONFIG:DebugAnalyze>: ${PROJECTS_SHARED_CXX_FLAGS_DEBUGANALYZE}> )
target_compile_options( "Projects.ShaderEditor" PRIVATE $<$<CONFIG:Debug>: ${PROJECTS_SHARED_CXX_FLAGS_DEBUG}> )



