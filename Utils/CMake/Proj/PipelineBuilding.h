// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "../Builder/CMakeBuilder.h"

#define ENABLE_STL
#define ENABLE_ENGINE
#define ENABLE_UTILS
#define ENABLE_ENGINE_SAMPLES
#define ENABLE_PROJECTS
#define ENABLE_EXTERNALS
//#define ENABLE_PIPELINE_BUILDING

//#define ENABLE_SCU			// single compilation unit per thread
#define NUM_THREADS	8

using namespace CMake;

using CMakeBuilderPtr = UniquePtr<CMakeBuilder>;


#if not defined(ENABLE_EXTERNALS) and not defined(ENABLE_ENGINE)
#	undef ENABLE_PIPELINE_BUILDING
#endif

#ifdef ENABLE_PIPELINE_BUILDING
inline void PackResources (CMakeBuilder::CMakeProject* proj, StringCRef resourceScript, const CMakeBuilder::CMakeProject* engineResPack)
{
	String	res_script;
	res_script << '"' << FileAddress::BuildPath( "${CMAKE_CURRENT_SOURCE_DIR}", FileAddress::BuildPath( proj->GetRelativePath(), resourceScript ) ) << '"';
	//res_script << "\"${CMAKE_SOURCE_DIR}/" << FileAddress::BuildPath( proj->GetRelativePath(), resourceScript ) << '"';

	proj->AddDependency( "Deps_"_str << proj->GetName() );
	proj->AddSource(
		"if (NOT DEFINED RESOURCE_PACKER_EXE)\n"
		"	message( FATAL_ERROR \"RESOURCE_PACKER_EXE is not defined\" )\n"
		"endif ()\n"_str
		<<
		"add_custom_target( \"Deps_" << proj->GetName() << "\"\n"
		"	COMMAND ${RESOURCE_PACKER_EXE} -R " << res_script << "\n"
		"	DEPENDS ${RESOURCE_PACKER_EXE} " << res_script << "\n"
		"	COMMENT \"Pack resources for '" << proj->GetName() << "'...\"\n"
		"	VERBATIM\n"
		")\n"
		"add_dependencies( \"Deps_" << proj->GetName() << "\" \"" << engineResPack->GetName() << "\" )\n"
		"set_property( TARGET \"Deps_" << proj->GetName() << "\" PROPERTY FOLDER \"_deps_\" )\n"
	);
}
#endif	// ENABLE_PIPELINE_BUILDING


CMakeBuilderPtr  GenCompilers		 ();
CMakeBuilderPtr  GenCore			 (Ptr<CMakeBuilder> compilers);
CMakeBuilderPtr  GenEngine			 (Ptr<CMakeBuilder> core);
CMakeBuilderPtr  GenUtils			 (Ptr<CMakeBuilder> core);
CMakeBuilderPtr  GenShaderEditor	 (Ptr<CMakeBuilder> engine);
