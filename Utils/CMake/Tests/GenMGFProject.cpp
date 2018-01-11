// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "../Builder/CMakeBuilder.h"

using namespace CMake;

#define ENABLE_STL
#define ENABLE_ENGINE
#define ENABLE_UTILS
#define ENABLE_PROJECTS
//#define ENABLE_LUNARGLASS

extern void GenMGFProject ()
{
	CHECK( OS::FileSystem::FindAndSetCurrentDir( "Utils/CMake/Builder", 5 ) );
	CHECK( OS::FileSystem::SetCurrentDirectory( "../../.." ) );

	CMakeBuilder	builder{ "", "ModularGraphicsFramework" };

	builder.Projects_IncludeDirectory( "Engine/.." );

	// compilers
	{
		// Visual C++
		auto	msvc = builder.AddMSVisualStudioCompiler();
		{
			Array<String>	shared_linker_cfg{ VS_Linker::OptReferences, VS_Linker::COMDATFolding, VS_Linker::NoIncrementalLinking };

			Array<String>	shared_cxx_flags{ VS::CppLastest, VS::MultiThreadCompilation, VS::NoMinimalRebuild, VS::RemUnrefCode,
											  VS::NoFunctionLevelLinking, VS::FloatPointStrict, VS::FloatPointNoExceptions };

			Array<String>	release_cxx_flags{ VS::InlineAll, VS::InlineAll, VS::Intrinsic, VS::OptFavorFastCode, VS::OptOmitFramePointers, VS::MultiThreadedDll,
											   VS::OptFiberSafe, VS::OptWholeProgram, VS::StringPooling, VS::NoSecurityCheck };

			Set<uint>	errors				= { VS::ReturningAddressOfLocalVariable, VS::TypeNameMismatch, VS::UninitializedVarUsed,
												VS::TooManyParamsForMacro, VS::RecursiveOnAllControlPaths,
												VS::UnrecognizedEscapeSequence, VS::UnreachableCode, VS::MultipleAssignmentOperators,
												VS::InconsistentDllLinkage, VS::ClassNeedsDllInterface };
			Set<uint>	enabled_warnings	= { VS::InitOrder, VS::UnknownMacro, VS::UnsafeFunctionorVariable, VS::ConditionalExprIsConstant, VS::ReintCastBetwenRelatedClasses };
			Set<uint>	disabled_warnings	= { VS::DecoratedNameWasTruncated, VS::CastTruncatesConstantValue, VS::UnusedInlineFunction,
												VS::ConversionSignedUnsignedMismatch };

			errors.AddArray( VS::ReturnType );
			errors.AddArray( VS::MemberNoOverride );
			errors.AddArray( VS::OperatorHasNoEffect );
			errors.AddArray( VS::Extensions );
			errors.AddArray( VS::Format );

			enabled_warnings.AddArray( VS::Unused );
			enabled_warnings.AddArray( VS::Shadow );
			enabled_warnings.AddArray( VS::Conversion );
			enabled_warnings.AddArray( VS::ConversionSign );

			disabled_warnings.AddArray( VS::DeletedConstructor );
			disabled_warnings.AddArray( VS::FunctionNotInlined );
			disabled_warnings.AddArray( VS::Switch );

			FOR( i, enabled_warnings )
			{
				if ( errors.IsExist( enabled_warnings[i] ) )
					errors.Erase( enabled_warnings[i] );
			}
			FOR( i, disabled_warnings )
			{
				if ( enabled_warnings.IsExist( disabled_warnings[i] ) )
					enabled_warnings.Erase( disabled_warnings[i] );

				if ( errors.IsExist( disabled_warnings[i] ) )
					errors.Erase( disabled_warnings[i] );
			}

			shared_cxx_flags << VS::WarningsToErrors( errors );
			shared_cxx_flags << VS::EnableWarnings( enabled_warnings );
			shared_cxx_flags << VS::DisableWarnings( disabled_warnings );

			auto	debug_cfg = msvc->AddConfiguration( "Debug" );
			{
				debug_cfg->AddTargetCxxFlags( shared_cxx_flags )
						 ->AddTargetCxxFlags({ VS::WarningLevel4, VS::NoWarningsAsErrors, VS::SDLChecks, VS::OptDisabled, VS::NoInline, VS::Exceptions,
												VS::OptNoOmitFramePointers, VS::NoStringPooling, VS::SecurityCheck, /*VS::ControlFlowGuard,*/ VS::RTTI,
												VS::MultiThreadedDebugDll, VS::NoStaticAnalyze, VS::DbgProgramDatabase, VS::StackFrameAndUninitVarCheck });
				debug_cfg->AddTargetLinkerFlags( shared_linker_cfg )->AddTargetLinkerFlags({ VS_Linker::DebugFull, VS_Linker::LinkTimeCodeGen });
				debug_cfg->AddTargetDefinitions({ "__GX_DEBUG__" })->AddGlobalDefinitions({ "_DEBUG"/*, "DEBUG"*/ });
			}

			auto	analyze_cfg = msvc->AddConfiguration( "DebugAnalyze" );
			{
				analyze_cfg->AddTargetCxxFlags( shared_cxx_flags )
							->AddTargetCxxFlags({ VS::WarningLevel4, VS::NoWarningsAsErrors, VS::SDLChecks, VS::OptDisabled, VS::NoInline, VS::Exceptions,
												  VS::OptNoOmitFramePointers, VS::NoStringPooling, VS::SecurityCheck, VS::ControlFlowGuard, VS::RTTI,
												  VS::MultiThreadedDebugDll, VS::StaticAnalyze, VS::DbgProgramDatabase, VS::StackFrameAndUninitVarCheck });
				analyze_cfg->AddTargetLinkerFlags( shared_linker_cfg )->AddTargetLinkerFlags({ VS_Linker::DebugFull, VS_Linker::LinkTimeCodeGen });
				analyze_cfg->AddTargetDefinitions({ "__GX_DEBUG__", "__GX_ANALYZE__" })->AddGlobalDefinitions({ "_DEBUG"/*, "DEBUG"*/ });

			}

			auto	profile_cfg = msvc->AddConfiguration( "Profile" );
			{
				profile_cfg->AddTargetCxxFlags( shared_cxx_flags )->AddTargetCxxFlags( release_cxx_flags )
						->AddTargetCxxFlags({ VS::WarningLevel3, VS::OptDisabled, VS::NoStaticAnalyze, VS::Exceptions, VS::RTTI });
				profile_cfg->AddTargetLinkerFlags( shared_linker_cfg )
							->AddTargetLinkerFlags({ VS_Linker::LinkTimeCodeGen, VS_Linker::Debug, VS_Linker::Profile });
				profile_cfg->AddTargetDefinitions({ "GX_ENABLE_PROFILING" })->AddGlobalDefinitions({ "_NDEBUG", "NDEBUG" });
			}
	
			auto	release_cfg = msvc->AddConfiguration( "Release" );
			{
				// remove some compiler warnings
				FOR( i, shared_cxx_flags ) {
					usize pos;
					if ( shared_cxx_flags[i].Find( "4100 ", pos ) ) {
						usize j = pos;
						for (; j < shared_cxx_flags[i].Length() and shared_cxx_flags[i][j] != '/'; --j) {} // move to linker option start
						shared_cxx_flags[i].Erase( j, pos + 4 - j );
					}
				}

				release_cfg->AddTargetCxxFlags( shared_cxx_flags )->AddTargetCxxFlags( release_cxx_flags )
							->AddTargetCxxFlags({ VS::WarningLevel3, VS::OptFull, VS::NoStaticAnalyze, VS::NoExceptions, VS::NoRTTI });
				release_cfg->AddTargetLinkerFlags( shared_linker_cfg )
							->AddTargetLinkerFlags({ VS_Linker::LinkTimeCodeGen, VS_Linker::Release, VS_Linker::RandomBaseAddress });
				release_cfg->AddTargetDefinitions({ "__GX_FAST__", "__GX_NO_EXCEPTIONS__" })->AddGlobalDefinitions({ "_NDEBUG", "NDEBUG" });
			}
		}

		// GCC
		auto	gcc = builder.AddGCCompiler();
		{
			Array<String>	shared_cxx_flags{ GCC::Cpp1z };

			//shared_cxx_flags.Append({ GCC::Shadow, });

			shared_cxx_flags.Append({ /*GCC::Pedantic,*/ GCC::CharSubscripts, GCC::DoublePromotion, GCC::Format, GCC::Main, GCC::MissingBraces, GCC::MissingIncludeDirs,
									  GCC::Unused, GCC::Uninititalized, GCC::MayBeUninitialized, GCC::UnknownPragmas, GCC::Pragmas, GCC::StrictAliasing, GCC::StrictOverflow,
									  GCC::Undef, GCC::EndifLabels, GCC::FreeNonheapObject, GCC::PointerArith, GCC::CastQual, GCC::CastAlign, GCC::WriteStrings,
									  /*GCC::Conversion,*/ GCC::ConversionNull, GCC::ZeroAsNullConst, GCC::EnumCompare, GCC::SignCompare, /*GCC::SignConvertsion,*/ GCC::SizeofPointerMemaccess,
									  GCC::LogicalOp });
			
			shared_cxx_flags.PushBack( GCC::WarningsToErrors({ GCC::InitSelf, GCC::Parentheses, GCC::ReturnLocalAddr, GCC::ReturnType, GCC::NonTemplateFriend,
															   GCC::ArrayBounds, GCC::DivByZero, GCC::Address, GCC::MissingFieldInit, /*GCC::AlignedNew,*/
															   GCC::PlacementNew }) );

			shared_cxx_flags.PushBack( GCC::DisableWarnings({ GCC::NonTemplateFriend }) );


			auto	debug_cfg = gcc->AddConfiguration( "Debug" );
			{
				debug_cfg->AddTargetCxxFlags( shared_cxx_flags )
						 ->AddTargetCxxFlags({ GCC::DebugGddb, /*GCC::Sanitize_Undefined,*/ GCC::CheckIncompleteType, GCC::OptDebug });
				//debug_cfg->AddTargetLinkerFlags({ "-lubsan" });
				debug_cfg->AddTargetDefinitions({ "__GX_DEBUG__" })->AddGlobalDefinitions({ "_DEBUG"/*, "DEBUG"*/ });
			}

			auto	analyze_cfg = gcc->AddConfiguration( "DebugAnalyze" );
			{
				analyze_cfg->AddTargetCxxFlags( shared_cxx_flags )
						  ->AddTargetCxxFlags({ GCC::DebugGddb, GCC::Sanitize_Undefined, GCC::CheckIncompleteType, GCC::OptDebug });
				//debug_cfg->AddTargetLinkerFlags({ "-lubsan" });
				analyze_cfg->AddTargetDefinitions({ "__GX_DEBUG__", "__GX_ANALYZE__" })->AddGlobalDefinitions({ "_DEBUG"/*, "DEBUG"*/ });
			}

			auto	profile_cfg = gcc->AddConfiguration( "Profile" );
			{
				profile_cfg->AddTargetCxxFlags( shared_cxx_flags )
							->AddTargetCxxFlags({ GCC::Opt2, GCC::InlineAll });
				profile_cfg->AddTargetDefinitions({ "GX_ENABLE_PROFILING" })->AddGlobalDefinitions({ "_NDEBUG", "NDEBUG" });
			}

			auto	release_cfg = gcc->AddConfiguration( "Release" );
			{
				release_cfg->AddTargetCxxFlags( shared_cxx_flags )
							->AddTargetCxxFlags({ GCC::Opt3, GCC::OptFast, GCC::OptOmitFramePointers, GCC::InlineAll });
				release_cfg->AddTargetDefinitions({ "__GX_FAST__", "__GX_NO_EXCEPTIONS__" })->AddGlobalDefinitions({ "_NDEBUG", "NDEBUG" });
			}
		}

		// Clang
		auto	clang = builder.AddClangCompiler();
		{
			auto	debug_cfg = clang->AddConfiguration( "Debug" );
			{
				debug_cfg->AddTargetDefinitions({ "__GX_DEBUG__" })->AddGlobalDefinitions({ "_DEBUG"/*, "DEBUG"*/ });
			}

			auto	analyze_cfg = clang->AddConfiguration( "DebugAnalyze" );
			{
				analyze_cfg->AddTargetDefinitions({ "__GX_DEBUG__", "__GX_ANALYZE__" })->AddGlobalDefinitions({ "_DEBUG"/*, "DEBUG"*/ });
			}

			auto	profile_cfg = clang->AddConfiguration( "Profile" );
			{
				profile_cfg->AddTargetDefinitions({ "GX_ENABLE_PROFILING" })->AddGlobalDefinitions({ "_NDEBUG", "NDEBUG" });
			}

			auto	release_cfg = clang->AddConfiguration( "Release" );
			{
				release_cfg->AddTargetDefinitions({ "__GX_FAST__", "__GX_NO_EXCEPTIONS__" })->AddGlobalDefinitions({ "_NDEBUG", "NDEBUG" });
			}
		}
	}

	// projects
	{
		// Engine //
	#ifdef ENABLE_STL
		auto	engine_config = builder.AddLibrary( "Engine.Config", "Engine/Config" );
		{
			engine_config->AddFoldersRecursive( "" );
		}

		auto	engine_stl = builder.AddLibrary( "Engine.STL", "Engine/STL" );
		{
			engine_stl->AddFoldersRecursive( "" );
			engine_stl->LinkLibrary( engine_config );
		}
		
		auto	test_stl = builder.AddExecutable( "Tests.STL", "Tests/STL" );
		{
			test_stl->AddFoldersRecursive( "" );
			test_stl->LinkLibrary( engine_stl );
		}
	#endif	// ENABLE_STL


	#ifdef ENABLE_ENGINE
		auto	engine_base = builder.AddLibrary( "Engine.Base", "Engine/Base" );
		{
			engine_base->AddFoldersRecursive( "" );
			engine_base->LinkLibrary( engine_stl );
		}

		auto	engine_platforms = builder.AddLibrary( "Engine.Platforms", "Engine/Platforms" );
		{
			engine_platforms->AddFoldersRecursive( "" );
			engine_platforms->LinkLibrary( engine_base );
			// OpenGL
			engine_platforms->LinkLibrary( "opengl32.lib", "WIN32" );
			// DirectX
			engine_platforms->LinkLibrary({ "dxguid.lib"_str, "dxgi.lib"_str }, "WIN32");
			// VR
			engine_platforms->LinkLibrary( "dxgi.lib", "WIN32" );
			// Windows Platform
			engine_platforms->LinkLibrary( "Shcore.lib", "(MSVC AND WIN32)" )->LinkLibrary( "Dxva2.lib", "WIN32" );
		}
		
		auto	engine_graphics = builder.AddLibrary( "Engine.Graphics", "Engine/Graphics" );
		{
			engine_graphics->AddFoldersRecursive( "" );
			engine_graphics->LinkLibrary( engine_platforms );
		}
		
		auto	engine_scene = builder.AddLibrary( "Engine.Scene", "Engine/Scene" );
		{
			engine_scene->AddFoldersRecursive( "" );
			engine_scene->LinkLibrary( engine_graphics );
		}

		// Tests //
		auto	test_engine_base = builder.AddExecutable( "Tests.Engine.Base", "Tests/Engine.Base" );
		{
			test_engine_base->AddFoldersRecursive( "" );
			test_engine_base->LinkLibrary( engine_platforms );
		}
		
		auto	test_engine_graphics = builder.AddExecutable( "Tests.Engine.Graphics", "Tests/Engine.Graphics" );
		{
			test_engine_graphics->AddFoldersRecursive( "" );
			test_engine_graphics->LinkLibrary( engine_graphics );
		}
		
		auto	test_engine_scene = builder.AddExecutable( "Tests.Engine.Scene", "Tests/Engine.Scene" );
		{
			test_engine_scene->AddFoldersRecursive( "" );
			test_engine_scene->LinkLibrary( engine_scene );
		}
	#endif	// ENABLE_ENGINE


	#ifdef ENABLE_LUNARGLASS
		builder.SearchVSProjects( "build_LunarGLASS", "External/LunarGLASS" );

		auto	engine_pipeline_compiler = builder.AddLibrary( "Engine.PipelineCompiler", "Engine/PipelineCompiler" );
		{
			engine_pipeline_compiler->EnableIf( "WIN32" );
			engine_pipeline_compiler->ProjFolder( "EngineTools" );
			engine_pipeline_compiler->AddFoldersRecursive( "" );

			engine_pipeline_compiler->IncludeDirectory( "External/LunarGLASS/glslang/.." );
			engine_pipeline_compiler->IncludeDirectory( "External/LunarGLASS/glslang" );
			engine_pipeline_compiler->IncludeDirectory( "External/LunarGLASS/.." );
			engine_pipeline_compiler->IncludeDirectory( "External/LunarGLASS" );
			engine_pipeline_compiler->IncludeDirectory( "External/LunarGLASS/Core/LLVM/llvm-3.4/include/llvm/.." );
			engine_pipeline_compiler->IncludeDirectory( "build_LunarGLASS/Core/LLVM/llvm-3.4/include/llvm/.." );

			engine_pipeline_compiler->LinkLibrary( engine_platforms );

			engine_pipeline_compiler->SearchLibraries( "build_LunarGLASS",
								   { "GLSLBackend", "glslangFrontend", "SpvFrontend", "core",
								     "LLVMAsmParser", "LLVMLinker", "LLVMipo", "LLVMScalarOpts", "LLVMInstCombine", "LLVMTransformUtils",
									 "LLVMipa", "LLVMAnalysis", "LLVMTarget", "LLVMCore", "LLVMSupport",
									 "glslang", "HLSL", "OSDependent", "OGLCompiler", "SPIRV", "SPVRemapper", "SPIRV-Tools", "SPIRV-Tools-opt" }, "NOT MSVC");

			engine_pipeline_compiler->AddDependency( {	"GLSLBackend", "glslangFrontend", "SpvFrontend", "core",
														"LLVMAsmParser", "LLVMLinker", "LLVMipo", "LLVMScalarOpts", "LLVMInstCombine", "LLVMTransformUtils",
														"LLVMipa", "LLVMAnalysis", "LLVMTarget", "LLVMCore", "LLVMSupport",
														"glslang", "HLSL", "OSDependent", "OGLCompiler", "SPIRV", "SPVRemapper", "SPIRV-Tools", "SPIRV-Tools-opt" }, "MSVC" );

			// this doesn't work but if you see warning you will go to change property in project
			engine_pipeline_compiler->AddSource(R"#(
if (MSVC)
    set_target_properties( "Engine.PipelineCompiler" PROPERTIES STATIC_LIBRARY_FLAGS "\" LinkLibraryDependencies=\"true" )
endif()
)#");
		}

		auto	test_pipeline_compiler = builder.AddExecutable( "Tests.PipelineCompiler", "Tests/PipelineCompiler" );
		{
			test_pipeline_compiler->EnableIf( "WIN32" );
			test_pipeline_compiler->AddFolder( "" );
			test_pipeline_compiler->AddFolder( "Pipelines" );
			test_pipeline_compiler->LinkLibrary( engine_pipeline_compiler );
		}
	#endif	// ENABLE_LUNARGLASS

	#ifdef ENABLE_PROJECTS
		auto	proj_shader_editor = builder.AddExecutable( "Projects.ShaderEditor", "Projects/ShaderEditor" );
		{
			proj_shader_editor->AddFoldersRecursive( "" );
			proj_shader_editor->LinkLibrary( engine_scene );
		}

	# ifdef ENABLE_LUNARGLASS
		auto	proj_shader_editor_tools = builder.AddExecutable( "Projects.ShaderEditor.Tools", "Projects/ShaderEditorTools" );
		{
			proj_shader_editor_tools->AddFoldersRecursive( "" );
			proj_shader_editor_tools->LinkLibrary( engine_pipeline_compiler );
		}
	# endif	// ENABLE_LUNARGLASS
	#endif	// ENABLE_PROJECTS

		// Utils //
	#ifdef ENABLE_UTILS
		auto	util_cmake = builder.AddExecutable( "Utils.CMake", "Utils/CMake" );
		{
			util_cmake->AddFoldersRecursive( "" );
			util_cmake->LinkLibrary( engine_stl );
		}
	#endif	// ENABLE_UTILS
	}

	// save
	builder.Save();
}
