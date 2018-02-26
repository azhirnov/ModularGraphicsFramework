// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "../Builder/CMakeBuilder.h"
#include "Engine/Config/Engine.Version.h"

using namespace CMake;

#define ENABLE_STL
#define ENABLE_ENGINE
#define ENABLE_UTILS
#define ENABLE_PROJECTS
#define ENABLE_LUNARGLASS
#define ENABLE_EXTERNALS
//#define ENABLE_SDL			// use SDL2 instead of WinAPI and other OS functions

//#define ENABLE_SCU			// single compilation unit per thread
#define NUM_THREADS	8


extern void GenMGFProject ()
{
	CHECK( OS::FileSystem::FindAndSetCurrentDir( "Utils/CMake/Builder", 5 ) );
	CHECK( OS::FileSystem::SetCurrentDirectory( "../../.." ) );

	CMakeBuilder	builder{ "", "ModularGraphicsFramework" };

	builder.SetVersion( Engine::_ENGINE_VERSION_MAJ, Engine::_ENGINE_VERSION_MIN );
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
												VS::TooManyParamsForMacro, VS::RecursiveOnAllControlPaths, VS::IllegalConversion,
												VS::UnrecognizedEscapeSequence, VS::UnreachableCode, VS::MultipleAssignmentOperators,
												VS::InconsistentDllLinkage, VS::ClassNeedsDllInterface };
			Set<uint>	enabled_warnings	= { VS::InitOrder, VS::UnknownMacro, VS::UnsafeFunctionorVariable, VS::ConditionalExprIsConstant, VS::ReintCastBetwenRelatedClasses };
			Set<uint>	disabled_warnings	= { VS::DecoratedNameWasTruncated, VS::CastTruncatesConstantValue, VS::UnusedInlineFunction,
												VS::ConversionSignedUnsignedMismatch, VS::ReservedLiteralSuffix };

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
					usize pos = UMax;
					if ( shared_cxx_flags[i].Find( "4100 ", OUT pos ) ) {
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

			builder.SetSystemVersion( "8.1", "WIN32" );
			msvc->AddSource( "message( STATUS \"CMAKE_VS_WINDOWS_TARGET_PLATFORM_VERSION: ${CMAKE_VS_WINDOWS_TARGET_PLATFORM_VERSION}\" )\n" );
		}

		// GCC
		auto	gcc = builder.AddGCCompiler();
		{
			Array<String>	shared_cxx_flags{ GCC::Cpp1z };

			//shared_cxx_flags.Append({ GCC::Shadow, });

			shared_cxx_flags.Append({ /*GCC::Pedantic,*/ GCC::CharSubscripts, GCC::DoublePromotion, GCC::Format, GCC::Main, GCC::MissingBraces, GCC::MissingIncludeDirs,
									  GCC::Uninititalized, GCC::MayBeUninitialized, GCC::UnknownPragmas, GCC::Pragmas, GCC::StrictAliasing, GCC::StrictOverflow,
									  GCC::Undef, GCC::EndifLabels, GCC::FreeNonheapObject, GCC::PointerArith, GCC::CastAlign, GCC::WriteStrings,
									  /*GCC::Conversion,*/ GCC::ConversionNull, /*GCC::ZeroAsNullConst,*/ GCC::EnumCompare, GCC::SignCompare, /*GCC::SignConvertsion,*/
									  GCC::SizeofPointerMemaccess, /*GCC::SizeofPointerDiv,*/ GCC::LogicalOp });
			
			shared_cxx_flags.PushBack( GCC::WarningsToErrors({ GCC::InitSelf, GCC::Parentheses, GCC::ReturnLocalAddr, GCC::ReturnType, GCC::NonTemplateFriend,
															   GCC::ArrayBounds, GCC::DivByZero, GCC::Address, GCC::MissingFieldInit, /*GCC::AlignedNew,*/
															   GCC::PlacementNew, GCC::SignCompare, GCC::CastQual }) );

			shared_cxx_flags.PushBack( GCC::DisableWarnings({ GCC::Unused, GCC::NonTemplateFriend, GCC::LiteralSuffix, GCC::ZeroAsNullConst }) );


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
			Array<String>	shared_cxx_flags{ Clang::Cpp1z };

			//shared_cxx_flags.Append({ Clang::Shadow, });

			shared_cxx_flags.Append({ /*Clang::Pedantic,*/ Clang::CharSubscripts, Clang::DoublePromotion, Clang::Format, Clang::Main, Clang::MissingBraces, Clang::MissingIncludeDirs,
									  Clang::Unused, Clang::Uninititalized, Clang::MayBeUninitialized, Clang::UnknownPragmas, Clang::Pragmas, Clang::StrictAliasing, Clang::StrictOverflow,
									  Clang::Undef, Clang::EndifLabels, Clang::FreeNonheapObject, Clang::PointerArith, Clang::CastQual, Clang::CastAlign, Clang::WriteStrings,
									  /*Clang::Conversion,*/ Clang::ConversionNull, Clang::ZeroAsNullConst, Clang::EnumCompare, Clang::SignCompare, /*Clang::SignConvertsion,*/ Clang::SizeofPointerMemaccess,
									  Clang::LogicalOp, Clang::RTTI, Clang::Exceptions });
			
			shared_cxx_flags.PushBack( Clang::WarningsToErrors({ Clang::InitSelf, Clang::Parentheses, Clang::ReturnLocalAddr, Clang::ReturnType, Clang::NonTemplateFriend,
															   Clang::ArrayBounds, Clang::DivByZero, Clang::Address, Clang::MissingFieldInit, /*Clang::AlignedNew,*/
															   Clang::PlacementNew }) );

			shared_cxx_flags.PushBack( Clang::DisableWarnings({ Clang::NonTemplateFriend, Clang::Comment, Clang::UndefinedInline, Clang::Switch, Clang::Narrowing,
															    Clang::Cxx14Extensions, Clang::Cxx1ZExtensions }) );


			auto	debug_cfg = clang->AddConfiguration( "Debug" );
			{
				debug_cfg->AddTargetCxxFlags( shared_cxx_flags )
						 ->AddTargetCxxFlags({ Clang::DebugGddb, /*Clang::Sanitize_Undefined,*/ Clang::CheckIncompleteType, Clang::OptDebug });
				debug_cfg->AddTargetDefinitions({ "__GX_DEBUG__" })->AddGlobalDefinitions({ "_DEBUG"/*, "DEBUG"*/ });
			}

			auto	analyze_cfg = clang->AddConfiguration( "DebugAnalyze" );
			{
				analyze_cfg->AddTargetCxxFlags( shared_cxx_flags )
						  ->AddTargetCxxFlags({ Clang::DebugGddb, Clang::Sanitize_Undefined, Clang::CheckIncompleteType, Clang::OptDebug });
				analyze_cfg->AddTargetDefinitions({ "__GX_DEBUG__", "__GX_ANALYZE__" })->AddGlobalDefinitions({ "_DEBUG"/*, "DEBUG"*/ });
			}

			auto	profile_cfg = clang->AddConfiguration( "Profile" );
			{
				profile_cfg->AddTargetCxxFlags( shared_cxx_flags )
							->AddTargetCxxFlags({ Clang::Opt2, Clang::InlineAll });
				profile_cfg->AddTargetDefinitions({ "GX_ENABLE_PROFILING" })->AddGlobalDefinitions({ "_NDEBUG", "NDEBUG" });
			}

			auto	release_cfg = clang->AddConfiguration( "Release" );
			{
				release_cfg->AddTargetCxxFlags( shared_cxx_flags )
							->AddTargetCxxFlags({ Clang::Opt3, Clang::OptFast, Clang::OptOmitFramePointers, Clang::InlineAll });
				release_cfg->AddTargetDefinitions({ "__GX_FAST__", "__GX_NO_EXCEPTIONS__" })->AddGlobalDefinitions({ "_NDEBUG", "NDEBUG" });
			}
		}
	}

	// projects
	{
		// STL //
	#ifdef ENABLE_STL
		auto	engine_docs = builder.AddLibrary( "Docs", "docs" );
		{
			engine_docs->AddFoldersRecursive( "" );
			engine_docs->ProjFolder( "" );
		}

		auto	engine_config = builder.AddLibrary( "Engine.Config", "Engine/Config" );
		{
			engine_config->AddFoldersRecursive( "" );
		}

		auto	engine_stl = builder.AddLibrary( "Engine.STL", "Engine/STL" );
		{
			engine_stl->AddFoldersRecursive( "" );
			engine_stl->LinkLibrary( engine_config );
			
			#ifdef ENABLE_SDL
			engine_stl->AddDefinition( "PLATFORM_SDL" )->LinkLibrary( "SDL2-static" );
			#endif
		}
		
		auto	test_stl = builder.AddExecutable( "Tests.STL", "Tests/STL" );
		{
			test_stl->AddFoldersRecursive( "" );
			test_stl->LinkLibrary( engine_stl );
		}

		auto	engine_physics = builder.AddLibrary( "Engine.Physics", "Engine/Physics" );
		{
			engine_physics->AddFoldersRecursive( "" );
			engine_physics->LinkLibrary( engine_stl );
		}

		auto	test_physics = builder.AddExecutable( "Tests.Physics", "Tests/Physics" );
		{
			test_physics->AddFoldersRecursive( "" );
			test_physics->LinkLibrary( engine_physics );
		}
	#endif	// ENABLE_STL


		// Engine //
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

		auto	engine_profilers = builder.AddLibrary( "Engine.Profilers", "Engine/Profilers" );
		{
			engine_profilers->AddFoldersRecursive( "" );
			engine_profilers->LinkLibrary( engine_platforms );
		}

		auto	engine_debugger = builder.AddLibrary( "Engine.Debugger", "Engine/Debugger" );
		{
			engine_debugger->AddFoldersRecursive( "" );
			engine_debugger->LinkLibrary( engine_platforms );
		}

		auto	engine_importexport = builder.AddLibrary( "Engine.ImportExport", "Engine/ImportExport" );
		{
			engine_importexport->AddFoldersRecursive( "" );
			engine_importexport->LinkLibrary( engine_graphics );

			#ifdef ENABLE_EXTERNALS
				engine_importexport->AddDependency( "FreeImageLib.2013", "MSVC" );
			#endif
		}
		
		auto	engine_scene = builder.AddLibrary( "Engine.Scene", "Engine/Scene" );
		{
			engine_scene->AddFoldersRecursive( "" );
			engine_scene->LinkLibrary( engine_graphics );
		}
		
		auto	engine_ui = builder.AddLibrary( "Engine.UI", "Engine/UI" );
		{
			engine_ui->AddFoldersRecursive( "" );
			engine_ui->LinkLibrary( engine_graphics );
		}

		// Tests //
		auto	test_engine_base = builder.AddExecutable( "Tests.Engine.Base", "Tests/Engine.Base" );
		{
			test_engine_base->AddFoldersRecursive( "" );
			test_engine_base->LinkLibrary( engine_platforms )->LinkLibrary( engine_profilers );
		}
		
		auto	test_engine_debugger = builder.AddExecutable( "Tests.Engine.Debugger", "Tests/Engine.Debugger" );
		{
			test_engine_debugger->AddFoldersRecursive( "" );
			test_engine_debugger->AddFolder( "../Engine.Base/Pipelines" );
			test_engine_debugger->LinkLibrary( engine_debugger );
		}
		
		auto	test_engine_graphics = builder.AddExecutable( "Tests.Engine.Graphics", "Tests/Engine.Graphics" );
		{
			test_engine_graphics->AddFoldersRecursive( "" );
			test_engine_graphics->LinkLibrary( engine_importexport )->LinkLibrary( engine_profilers );
		}
		
		auto	test_engine_scene = builder.AddExecutable( "Tests.Engine.Scene", "Tests/Engine.Scene" );
		{
			test_engine_scene->AddFoldersRecursive( "" );
			test_engine_scene->LinkLibrary( engine_scene )->LinkLibrary( engine_profilers );
		}
		
		auto	test_engine_ui = builder.AddExecutable( "Tests.Engine.UI", "Tests/Engine.UI" );
		{
			test_engine_ui->AddFoldersRecursive( "" );
			test_engine_ui->LinkLibrary( engine_ui )->LinkLibrary( engine_profilers );
		}

		auto	test_engine_gapi = builder.AddExecutable( "Tests.Engine.Platforms.GAPI", "Tests/Engine.Platforms.GAPI" );
		{
			test_engine_gapi->AddFoldersRecursive( "" );
			test_engine_gapi->LinkLibrary( engine_platforms );
		}

		#ifdef ENABLE_SCU
			builder.AddExecutable( "Tests.Engine.STL.Fast" )->ProjFolder("Fast")->LinkLibrary( test_stl )->MergeCPP( NUM_THREADS );
			builder.AddExecutable( "Tests.Engine.Base.Fast" )->ProjFolder("Fast")->LinkLibrary( test_engine_base )->MergeCPP( NUM_THREADS );
			builder.AddExecutable( "Tests.Engine.Debugger.Fast" )->ProjFolder("Fast")->LinkLibrary( test_engine_debugger )->MergeCPP( NUM_THREADS );
			builder.AddExecutable( "Tests.Engine.Graphics.Fast" )->ProjFolder("Fast")->LinkLibrary( test_engine_graphics )->MergeCPP( NUM_THREADS );
			builder.AddExecutable( "Tests.Engine.Scene.Fast" )->ProjFolder("Fast")->LinkLibrary( test_engine_scene )->MergeCPP( NUM_THREADS );
			builder.AddExecutable( "Tests.Engine.UI.Fast" )->ProjFolder("Fast")->LinkLibrary( test_engine_ui )->MergeCPP( NUM_THREADS );
		#endif
	#endif	// ENABLE_ENGINE


		// External //
	#ifdef ENABLE_SDL
		builder.AddExternal( "External/SDL2" );
	#endif

#ifdef ENABLE_EXTERNALS
		builder.SearchVSProjects( "External/FreeImage", "External/FreeImage" );

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
									 "glslang", "HLSL", "OSDependent", "OGLCompiler", "SPIRV", "SPVRemapper", "SPIRV-Tools", "SPIRV-Tools-opt" }, "NOT MSVC" );

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

		auto	test_engine_gapi_tools = builder.AddExecutable( "Tests.Engine.Platforms.GAPI.Tools", "Tests/Engine.Platforms.GAPI.Tools" );
		{
			test_engine_gapi_tools->EnableIf( "WIN32" );
			test_engine_gapi_tools->AddFoldersRecursive( "" );
			test_engine_gapi_tools->LinkLibrary( engine_pipeline_compiler );
		}
	#endif	// ENABLE_LUNARGLASS
#endif	// ENABLE_EXTERNALS


		// Projects //
	#ifdef ENABLE_PROJECTS
		auto	proj_shader_editor = builder.AddExecutable( "Projects.ShaderEditor", "Projects/ShaderEditor" );
		{
			proj_shader_editor->AddFoldersRecursive( "" );
			proj_shader_editor->LinkLibrary( engine_scene )->LinkLibrary( engine_profilers );
		}

	# ifdef ENABLE_LUNARGLASS
		auto	proj_shader_editor_tools = builder.AddExecutable( "Projects.ShaderEditor.Tools", "Projects/ShaderEditorTools" );
		{
			proj_shader_editor_tools->AddFoldersRecursive( "" );
			proj_shader_editor_tools->LinkLibrary( engine_pipeline_compiler );
		}
	# endif	// ENABLE_LUNARGLASS

		auto	proj_machine_learning = builder.AddExecutable( "Projects.MachineLearning", "Projects/MachineLearning" );
		{
			proj_machine_learning->AddFoldersRecursive( "" );
			proj_machine_learning->LinkLibrary( engine_scene );
		}
	#endif	// ENABLE_PROJECTS


		// Utils //
	#ifdef ENABLE_UTILS
		auto	util_cmake = builder.AddExecutable( "Utils.CMake", "Utils/CMake" );
		{
			util_cmake->AddFoldersRecursive( "" );
			util_cmake->LinkLibrary( engine_stl );
		}

		auto	util_cpppreprocessor = builder.AddExecutable( "Utils.CppPreprocessor", "Utils/CppPreprocessor" );
		{
			util_cpppreprocessor->AddFoldersRecursive( "" );
			util_cpppreprocessor->LinkLibrary( engine_stl );
		}

		auto	util_filesearch = builder.AddExecutable( "Utils.FileSearch", "Utils/FileSearch" );
		{
			util_filesearch->AddFoldersRecursive( "" );
			util_filesearch->LinkLibrary( engine_stl );
		}

		auto	util_pvs = builder.AddExecutable( "Utils.PVS", "Utils/PVS" );
		{
			util_pvs->AddFoldersRecursive( "" );
			util_pvs->LinkLibrary( engine_stl );
		}
	#endif	// ENABLE_UTILS
	}

	// save
	builder.Save();
}
