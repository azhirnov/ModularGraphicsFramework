# auto generated file
cmake_minimum_required (VERSION 3.6.0)

#==================================================================================================
# project: Utils.CMake
#==================================================================================================
if (NOT DEFINED ANDROID)
	set( SOURCES 
		"CMake/Proj/GenCompilers.cpp"
		"CMake/Proj/GenCore.cpp"
		"CMake/Proj/GenEngine.cpp"
		"CMake/Proj/GenEngineSamples.cpp"
		"CMake/Proj/GenShaderEditor.cpp"
		"CMake/Proj/GenUtils.cpp"
		"CMake/Proj/Main.cpp"
		"CMake/Proj/PipelineBuilding.h"
		"CMake/Builder/ClangConstants.h"
		"CMake/Builder/CMakeBuilder.cpp"
		"CMake/Builder/CMakeBuilder.h"
		"CMake/Builder/CMakeBuilder_Compiler.cpp"
		"CMake/Builder/CMakeBuilder_ExternalProjects.cpp"
		"CMake/Builder/CMakeBuilder_Project.cpp"
		"CMake/Builder/GccConstants.h"
		"CMake/Builder/MSVisualStudioConstants.h" )
	if (DEFINED ANDROID)
		add_library( "Utils.CMake" SHARED ${SOURCES} )
	else()
		add_executable( "Utils.CMake" ${SOURCES} )
	endif()
	set_property( TARGET "Utils.CMake" PROPERTY FOLDER "Utils" )
	target_include_directories( "Utils.CMake" PUBLIC "C:/Projects/graphxgenengine/External" )
	target_include_directories( "Utils.CMake" PUBLIC "${EXTERNALS_PATH}" )
	target_include_directories( "Utils.CMake" PUBLIC "C:/Projects/graphxgenengine/Core/.." )
	target_link_libraries( "Utils.CMake" "Core.STL" )
	# compiler
	set_target_properties( "Utils.CMake" PROPERTIES LINK_FLAGS_RELEASE ${PROJECTS_SHARED_LINKER_FLAGS_RELEASE} )
	set_target_properties( "Utils.CMake" PROPERTIES LINK_FLAGS_PROFILE ${PROJECTS_SHARED_LINKER_FLAGS_PROFILE} )
	target_compile_definitions( "Utils.CMake" PRIVATE $<$<CONFIG:Debug>: ${PROJECTS_SHARED_DEFINES_DEBUG}> )
	set_target_properties( "Utils.CMake" PROPERTIES LINK_FLAGS_DEBUGANALYZE ${PROJECTS_SHARED_LINKER_FLAGS_DEBUGANALYZE} )
	target_compile_definitions( "Utils.CMake" PRIVATE $<$<CONFIG:DebugAnalyze>: ${PROJECTS_SHARED_DEFINES_DEBUGANALYZE}> )
	target_compile_options( "Utils.CMake" PRIVATE $<$<CONFIG:Profile>: ${PROJECTS_SHARED_CXX_FLAGS_PROFILE}> )
	target_compile_options( "Utils.CMake" PRIVATE $<$<CONFIG:Release>: ${PROJECTS_SHARED_CXX_FLAGS_RELEASE}> )
	target_compile_definitions( "Utils.CMake" PRIVATE $<$<CONFIG:Profile>: ${PROJECTS_SHARED_DEFINES_PROFILE}> )
	target_compile_definitions( "Utils.CMake" PRIVATE $<$<CONFIG:Release>: ${PROJECTS_SHARED_DEFINES_RELEASE}> )
	set_target_properties( "Utils.CMake" PROPERTIES LINK_FLAGS_DEBUG ${PROJECTS_SHARED_LINKER_FLAGS_DEBUG} )
	target_compile_options( "Utils.CMake" PRIVATE $<$<CONFIG:DebugAnalyze>: ${PROJECTS_SHARED_CXX_FLAGS_DEBUGANALYZE}> )
	target_compile_options( "Utils.CMake" PRIVATE $<$<CONFIG:Debug>: ${PROJECTS_SHARED_CXX_FLAGS_DEBUG}> )
	
endif()
