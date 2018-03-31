// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "../Builder/CMakeBuilder.h"
#include "Engine/Config/Engine.Version.h"

using namespace CMake;

#define ENABLE_STL
#define ENABLE_ENGINE
#define ENABLE_UTILS
#define ENABLE_PROJECTS
#define ENABLE_EXTERNALS

//#define ENABLE_SCU			// single compilation unit per thread
#define NUM_THREADS	8


extern void GenMGFProject ()
{
	CHECK( OS::FileSystem::FindAndSetCurrentDir( "Utils/CMake/Builder", 5 ) );
	CHECK( OS::FileSystem::SetCurrentDirectory( "../../.." ) );

	CMakeBuilder	builder{ "", "ModularGraphicsFramework" };

	builder.SetVersion( Engine::_ENGINE_VERSION_MAJ, Engine::_ENGINE_VERSION_MIN );
	builder.Projects_IncludeDirectory( "Engine/.." )
			->Projects_IncludeDirectory( "External" )
			->Projects_IncludeDirectory( "${EXTERNALS_PATH}" );

	// setup graphics api version or disable it (pass 0 to version)
	builder.AddOption( "OPENGL_VERSION", "Define OpenGL version, supported: 440, 450", 450 );
	builder.AddOption( "OPENCL_VERSION", "Define OpenCL version, supported: 110, 120, 200", 120 );
	builder.AddOption( "VULKAN_VERSION", "Define Vulkan version, supported: 100, 110", 100 );
	builder.AddOption( "SOFTRENDER_VERSION", "Define Software renderer version, 100 is supported", 100 );

	// compilers
	{
		// Visual C++
		auto	msvc = builder.AddMSVisualStudioCompiler();
		{
			msvc->AddSource( "set( CONFIGURATION_DEPENDENT_PATH ON CACHE INTERNAL \"\" FORCE )" );

			const String	dbg_lib = VS::MultiThreadedDebugDll;
			const String	rel_lib = VS::MultiThreadedDll;

			Array<String>	shared_linker_cfg{ VS_Linker::OptReferences, VS_Linker::COMDATFolding, VS_Linker::NoIncrementalLinking };

			Array<String>	shared_cxx_flags{ VS::CppLastest, VS::MultiThreadCompilation, VS::NoMinimalRebuild, VS::RemUnrefCode,
											  VS::NoFunctionLevelLinking, VS::FloatPointStrict, VS::FloatPointNoExceptions };

			Array<String>	release_cxx_flags{ VS::InlineAll, VS::InlineAll, VS::Intrinsic, VS::OptFavorFastCode, VS::OptOmitFramePointers,
											   VS::OptFiberSafe, VS::OptWholeProgram, VS::StringPooling, VS::NoSecurityCheck, rel_lib };

			Set<uint>	errors				= { VS::ReturningAddressOfLocalVariable, VS::TypeNameMismatch, VS::UninitializedVarUsed,
												VS::TooManyParamsForMacro, VS::RecursiveOnAllControlPaths, VS::IllegalConversion,
												VS::UnrecognizedEscapeSequence, VS::UnreachableCode, VS::MultipleAssignmentOperators,
												VS::InconsistentDllLinkage, VS::ClassNeedsDllInterface, VS::EmptyControlledStatement,
												VS::AssignInConditionalExpr };
			Set<uint>	enabled_warnings	= { VS::InitOrder, VS::UnknownMacro, VS::UnsafeFunctionorVariable, VS::ConditionalExprIsConstant,
												VS::ReintCastBetwenRelatedClasses };
			Set<uint>	disabled_warnings	= { VS::DecoratedNameWasTruncated, VS::CastTruncatesConstantValue, VS::UnusedInlineFunction,
												VS::ConversionSignedUnsignedMismatch, VS::ReservedLiteralSuffix };

			errors.AddArray( VS::ReturnType );
			errors.AddArray( VS::MemberNoOverride );
			errors.AddArray( VS::OperatorHasNoEffect );
			errors.AddArray( VS::Extensions );
			errors.AddArray( VS::Format );
			errors.AddArray( VS::Shadow );

			enabled_warnings.AddArray( VS::Unused );
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
				debug_cfg->AddGlobalCFlags({ VS::Define( "_DEBUG" ), dbg_lib, VS::OptDisabled, VS::MultiThreadCompilation })
						  ->AddGlobalCxxFlags({ VS::Define( "_DEBUG" ), dbg_lib, VS::OptDisabled, VS::MultiThreadCompilation })
						  ->AddGlobalLinkerFlags({ VS_Linker::DebugFull, VS_Linker::LinkTimeCodeGen });

				debug_cfg->AddTargetCxxFlags( shared_cxx_flags )
						 ->AddTargetCxxFlags({ VS::WarningLevel4, VS::NoWarningsAsErrors, VS::SDLChecks, VS::OptDisabled, VS::NoInline, VS::Exceptions,
												VS::OptNoOmitFramePointers, VS::NoStringPooling, VS::SecurityCheck, /*VS::ControlFlowGuard,*/ VS::RTTI,
												dbg_lib, VS::NoStaticAnalyze, VS::DbgProgramDatabase, VS::StackFrameAndUninitVarCheck });
				debug_cfg->AddTargetLinkerFlags( shared_linker_cfg )->AddTargetLinkerFlags({ VS_Linker::DebugFull, VS_Linker::LinkTimeCodeGen });
				debug_cfg->AddTargetDefinitions({ "__GX_DEBUG__" });
			}

			auto	analyze_cfg = msvc->AddConfiguration( "DebugAnalyze" );
			{
				analyze_cfg->AddGlobalCFlags({ VS::Define( "_DEBUG" ), dbg_lib, VS::OptDisabled, VS::MultiThreadCompilation })
							->AddGlobalCxxFlags({ VS::Define( "_DEBUG" ), dbg_lib, VS::OptDisabled, VS::MultiThreadCompilation })
							->AddGlobalLinkerFlags({ VS_Linker::DebugFull, VS_Linker::LinkTimeCodeGen });

				analyze_cfg->AddTargetCxxFlags( shared_cxx_flags )
							->AddTargetCxxFlags({ VS::WarningLevel4, VS::NoWarningsAsErrors, VS::SDLChecks, VS::OptDisabled, VS::NoInline, VS::Exceptions,
												  VS::OptNoOmitFramePointers, VS::NoStringPooling, VS::SecurityCheck, VS::ControlFlowGuard, VS::RTTI,
												  dbg_lib, VS::StaticAnalyze, VS::DbgProgramDatabase, VS::StackFrameAndUninitVarCheck });
				analyze_cfg->AddTargetLinkerFlags( shared_linker_cfg )->AddTargetLinkerFlags({ VS_Linker::DebugFull, VS_Linker::LinkTimeCodeGen });
				analyze_cfg->AddTargetDefinitions({ "__GX_DEBUG__", "__GX_ANALYZE__" });
			}

			auto	profile_cfg = msvc->AddConfiguration( "Profile" );
			{
				profile_cfg->AddGlobalCFlags({ VS::Defines({ "_NDEBUG", "NDEBUG" }), rel_lib, VS::OptDisabled, VS::MultiThreadCompilation })
							->AddGlobalCxxFlags({ VS::Defines({ "_NDEBUG", "NDEBUG" }), rel_lib, VS::OptDisabled, VS::MultiThreadCompilation })
							->AddGlobalLinkerFlags({ VS_Linker::LinkTimeCodeGen, VS_Linker::Debug, VS_Linker::Profile });

				profile_cfg->AddTargetCxxFlags( shared_cxx_flags )->AddTargetCxxFlags( release_cxx_flags )
						->AddTargetCxxFlags({ VS::WarningLevel3, VS::OptDisabled, VS::NoStaticAnalyze, VS::Exceptions, VS::RTTI });
				profile_cfg->AddTargetLinkerFlags( shared_linker_cfg )
							->AddTargetLinkerFlags({ VS_Linker::LinkTimeCodeGen, VS_Linker::Debug, VS_Linker::Profile });
				profile_cfg->AddTargetDefinitions({ "GX_ENABLE_PROFILING" });
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
				release_cfg->AddGlobalCFlags({ VS::Defines({ "_NDEBUG", "NDEBUG" }), rel_lib, VS::OptFull, VS::MultiThreadCompilation })
							->AddGlobalCxxFlags({ VS::Defines({ "_NDEBUG", "NDEBUG" }), rel_lib, VS::OptFull, VS::MultiThreadCompilation })
							->AddGlobalLinkerFlags({ VS_Linker::LinkTimeCodeGen, VS_Linker::Release });

				release_cfg->AddTargetCxxFlags( shared_cxx_flags )->AddTargetCxxFlags( release_cxx_flags )
							->AddTargetCxxFlags({ VS::WarningLevel3, VS::OptFull, VS::NoStaticAnalyze, VS::NoExceptions, VS::NoRTTI });
				release_cfg->AddTargetLinkerFlags( shared_linker_cfg )
							->AddTargetLinkerFlags({ VS_Linker::LinkTimeCodeGen, VS_Linker::Release, VS_Linker::RandomBaseAddress });
				release_cfg->AddTargetDefinitions({ "__GX_FAST__", "__GX_NO_EXCEPTIONS__" });
			}

			builder.SetSystemVersion( "8.1", "WIN32" );
		}

		// GCC
		auto	gcc = builder.AddGCCompiler();
		{
			gcc->AddSource( "set( CONFIGURATION_DEPENDENT_PATH OFF CACHE INTERNAL \"\" FORCE )" );

			Array<String>	shared_cxx_flags{ GCC::Cpp1z };
			Array<String>	shared_linked_flags{ GccLinker::Static, GccLinker::StaticLibGCC, GccLinker::StaticLibStdCPP, GccLinker::Link("dl") };

			shared_cxx_flags.Append({ /*GCC::Pedantic,*/ GCC::CharSubscripts, GCC::DoublePromotion, GCC::Format, GCC::Main, GCC::MissingBraces, GCC::MissingIncludeDirs,
									  GCC::Uninititalized, GCC::MayBeUninitialized, GCC::UnknownPragmas, GCC::Pragmas, GCC::StrictAliasing, GCC::StrictOverflow,
									  GCC::Undef, GCC::EndifLabels, GCC::FreeNonheapObject, GCC::PointerArith, GCC::CastAlign, GCC::WriteStrings,
									  /*GCC::Conversion,*/ GCC::ConversionNull, /*GCC::ZeroAsNullConst,*/ GCC::EnumCompare, GCC::SignCompare, /*GCC::SignConvertsion,*/
									  GCC::SizeofPointerMemaccess, /*GCC::SizeofPointerDiv,*/ GCC::LogicalOp });

			shared_cxx_flags.PushBack( GCC::DisableWarnings({ GCC::Unused, GCC::NonTemplateFriend, GCC::ZeroAsNullConst, GCC::Shadow, GCC::EnumCompare,
															  GCC::Narrowing, GCC::Attribute, GCC::InvalidOffsetof }) );
			
			shared_cxx_flags.PushBack( GCC::WarningsToErrors({ GCC::InitSelf, GCC::Parentheses, GCC::ReturnLocalAddr, GCC::ReturnType,
															   GCC::ArrayBounds, GCC::DivByZero, GCC::Address, GCC::MissingFieldInit, /*GCC::AlignedNew,*/
															   GCC::PlacementNew, GCC::SignCompare, GCC::CastQual, GCC::CastAlign, GCC::LiteralSuffix,
															   GCC::ShadowLocal }) );


			auto	debug_cfg = gcc->AddConfiguration( "Debug" );
			{
				debug_cfg->AddGlobalCFlags({ GCC::Define( "_DEBUG" ), GCC::OptDebug })
						  ->AddGlobalCxxFlags({ GCC::Define( "_DEBUG" ), GCC::OptDebug });

				debug_cfg->AddTargetCxxFlags( shared_cxx_flags )
						 ->AddTargetCxxFlags({ GCC::DebugGddb, /*GCC::Sanitize_Undefined,*/ GCC::CheckIncompleteType, GCC::OptDebug, GCC::DisableWarnings({ GCC::Terminate }) });
				debug_cfg->AddTargetLinkerFlags( shared_linked_flags );
				//debug_cfg->AddTargetLinkerFlags({ "-lubsan" });
				debug_cfg->AddTargetDefinitions({ "__GX_DEBUG__" });
			}

			auto	analyze_cfg = gcc->AddConfiguration( "DebugAnalyze" );
			{
				analyze_cfg->AddGlobalCFlags({ GCC::Define( "_DEBUG" ), GCC::OptDebug })
							->AddGlobalCxxFlags({ GCC::Define( "_DEBUG" ), GCC::OptDebug });

				analyze_cfg->AddTargetCxxFlags( shared_cxx_flags )
						  ->AddTargetCxxFlags({ GCC::DebugGddb, GCC::Sanitize_Undefined, GCC::CheckIncompleteType, GCC::OptDebug, GCC::DisableWarnings({ GCC::Terminate }) });
				analyze_cfg->AddTargetLinkerFlags( shared_linked_flags );
				analyze_cfg->AddTargetDefinitions({ "__GX_DEBUG__", "__GX_ANALYZE__" });
			}

			auto	profile_cfg = gcc->AddConfiguration( "Profile" );
			{
				profile_cfg->AddGlobalCFlags({ GCC::Defines({ "_NDEBUG", "NDEBUG" }), GCC::Opt2 })
							->AddGlobalCxxFlags({ GCC::Defines({ "_NDEBUG", "NDEBUG" }), GCC::Opt2 });

				profile_cfg->AddTargetCxxFlags( shared_cxx_flags )
							->AddTargetCxxFlags({ GCC::Opt2 });
				profile_cfg->AddTargetLinkerFlags( shared_linked_flags );
				profile_cfg->AddTargetDefinitions({ "GX_ENABLE_PROFILING" });
			}

			auto	release_cfg = gcc->AddConfiguration( "Release" );
			{
				release_cfg->AddGlobalCFlags({ GCC::Defines({ "_NDEBUG", "NDEBUG" }), GCC::Opt3, GCC::InlineAll })
							->AddGlobalCxxFlags({ GCC::Defines({ "_NDEBUG", "NDEBUG" }), GCC::Opt3, GCC::InlineAll });

				release_cfg->AddTargetCxxFlags( shared_cxx_flags )
							->AddTargetCxxFlags({ GCC::Opt3, GCC::OptFast, GCC::OptOmitFramePointers, GCC::InlineAll });
				release_cfg->AddTargetLinkerFlags( shared_linked_flags );
				release_cfg->AddTargetDefinitions({ "__GX_FAST__", "__GX_NO_EXCEPTIONS__" });
			}
		}

		// Clang (Windows, Linux)
		#if 0
		auto	clang = builder.AddClangCompiler()->EnableIf("WIN32 OR UNIX");
		{
			clang->AddSource( "set( CONFIGURATION_DEPENDENT_PATH OFF CACHE INTERNAL \"\" FORCE )" );

			Array<String>	shared_cxx_flags{ Clang::Cpp1z };

			//shared_cxx_flags.Append({ Clang::Shadow, });

			shared_cxx_flags.Append({ /*Clang::Pedantic,*/ Clang::CharSubscripts, Clang::DoublePromotion, Clang::Format, Clang::Main, Clang::MissingBraces, Clang::MissingIncludeDirs,
									  Clang::Unused, Clang::Uninititalized, Clang::MayBeUninitialized, Clang::UnknownPragmas, Clang::Pragmas, Clang::StrictAliasing, Clang::StrictOverflow,
									  Clang::Undef, Clang::EndifLabels, Clang::FreeNonheapObject, Clang::PointerArith, Clang::WriteStrings,
									  /*Clang::Conversion,*/ Clang::ConversionNull, Clang::ZeroAsNullConst, Clang::EnumCompare, Clang::SignCompare, /*Clang::SignConvertsion,*/ Clang::SizeofPointerMemaccess,
									  Clang::LogicalOp, Clang::RTTI, Clang::Exceptions });
			
			shared_cxx_flags.PushBack( Clang::WarningsToErrors({ Clang::InitSelf, Clang::Parentheses, Clang::ReturnLocalAddr, Clang::ReturnType, Clang::NonTemplateFriend,
															   Clang::ArrayBounds, Clang::DivByZero, Clang::Address, Clang::MissingFieldInit, /*Clang::AlignedNew,*/
															   Clang::PlacementNew, Clang::CastQual, Clang::CastAlign, Clang::UnknownWarning, Clang::UserDefinedLiterals }) );

			shared_cxx_flags.PushBack( Clang::DisableWarnings({ Clang::NonTemplateFriend, Clang::Comment, Clang::UndefinedInline, Clang::Switch, Clang::Narrowing,
															    Clang::Cxx14Extensions, Clang::Cxx1ZExtensions }) );


			auto	debug_cfg = clang->AddConfiguration( "Debug" );
			{
				debug_cfg->AddGlobalCFlags({ GCC::Define( "_DEBUG" ), Clang::OptDebug })
						  ->AddGlobalCxxFlags({ GCC::Define( "_DEBUG" ), Clang::OptDebug });

				debug_cfg->AddTargetCxxFlags( shared_cxx_flags )
						 ->AddTargetCxxFlags({ Clang::DebugGddb, /*Clang::Sanitize_Undefined,*/ Clang::CheckIncompleteType, Clang::OptDebug });
				debug_cfg->AddTargetDefinitions({ "__GX_DEBUG__" });
			}

			auto	analyze_cfg = clang->AddConfiguration( "DebugAnalyze" );
			{
				analyze_cfg->AddGlobalCFlags({ GCC::Define( "_DEBUG" ), Clang::OptDebug })
							->AddGlobalCxxFlags({ GCC::Define( "_DEBUG" ), Clang::OptDebug });

				analyze_cfg->AddTargetCxxFlags( shared_cxx_flags )
						  ->AddTargetCxxFlags({ Clang::DebugGddb, Clang::Sanitize_Undefined, Clang::CheckIncompleteType, Clang::OptDebug });
				analyze_cfg->AddTargetDefinitions({ "__GX_DEBUG__", "__GX_ANALYZE__" });
			}

			auto	profile_cfg = clang->AddConfiguration( "Profile" );
			{
				profile_cfg->AddGlobalCFlags({ GCC::Defines({ "_NDEBUG", "NDEBUG" }), Clang::Opt2 })
							->AddGlobalCxxFlags({ GCC::Defines({ "_NDEBUG", "NDEBUG" }), Clang::Opt2 });

				profile_cfg->AddTargetCxxFlags( shared_cxx_flags )
							->AddTargetCxxFlags({ Clang::Opt2, Clang::InlineAll });
				profile_cfg->AddTargetDefinitions({ "GX_ENABLE_PROFILING" });
			}

			auto	release_cfg = clang->AddConfiguration( "Release" );
			{
				release_cfg->AddGlobalCFlags({ GCC::Defines({ "_NDEBUG", "NDEBUG" }), Clang::Opt3 })
							->AddGlobalCxxFlags({ GCC::Defines({ "_NDEBUG", "NDEBUG" }), Clang::Opt3 });

				release_cfg->AddTargetCxxFlags( shared_cxx_flags )
							->AddTargetCxxFlags({ Clang::Opt3, Clang::OptFast, Clang::OptOmitFramePointers, Clang::InlineAll });
				release_cfg->AddTargetDefinitions({ "__GX_FAST__", "__GX_NO_EXCEPTIONS__" });
			}
		}
		#endif

		// Clang MacOS
		auto	clang_apple = builder.AddClangCompiler( "CLANG_APPLE" )->EnableIf("APPLE");
		{
			clang_apple->AddSource( "set( CONFIGURATION_DEPENDENT_PATH ON CACHE INTERNAL \"\" FORCE )" );

			Array<String>	shared_cxx_flags{ Clang::Cpp1z };

			shared_cxx_flags.Append({ Clang::CharSubscripts, Clang::DoublePromotion, Clang::Format, Clang::Main, Clang::MissingBraces, Clang::MissingIncludeDirs,
									  Clang::Uninititalized, Clang::UnknownPragmas, Clang::Pragmas, Clang::StrictAliasing, Clang::StrictOverflow,
									  Clang::Undef, Clang::EndifLabels, Clang::PointerArith, Clang::WriteStrings,
									  Clang::ConversionNull, Clang::EnumCompare, Clang::SignCompare, Clang::SizeofPointerMemaccess,
									  Clang::RTTI, Clang::Exceptions });
			
			shared_cxx_flags.PushBack( Clang::WarningsToErrors({ Clang::InitSelf, Clang::Parentheses, Clang::ReturnLocalAddr, Clang::ReturnType, Clang::UserDefinedLiterals,
															   Clang::ArrayBounds, Clang::DivByZero, Clang::Address, Clang::MissingFieldInit,
															   Clang::CastQual, Clang::UnknownWarning }) );

			shared_cxx_flags.PushBack( Clang::DisableWarnings({ Clang::Comment, Clang::UndefinedInline, Clang::Switch, Clang::Narrowing, Clang::Unused,
															    Clang::Cxx14Extensions, Clang::Cxx1ZExtensions }) );


			auto	debug_cfg = clang_apple->AddConfiguration( "Debug" );
			{
				debug_cfg->AddGlobalCFlags({ Clang::Define( "_DEBUG" ), Clang::OptDebug })
						  ->AddGlobalCxxFlags({ Clang::Define( "_DEBUG" ), Clang::OptDebug });

				debug_cfg->AddTargetCxxFlags( shared_cxx_flags )
						 ->AddTargetCxxFlags({ Clang::DebugGddb, Clang::OptDebug });
				debug_cfg->AddTargetDefinitions({ "__GX_DEBUG__" });
			}

			auto	analyze_cfg = clang_apple->AddConfiguration( "DebugAnalyze" );
			{
				analyze_cfg->AddGlobalCFlags({ Clang::Define( "_DEBUG" ), Clang::OptDebug })
							->AddGlobalCxxFlags({ Clang::Define( "_DEBUG" ), Clang::OptDebug });

				analyze_cfg->AddTargetCxxFlags( shared_cxx_flags )
						  ->AddTargetCxxFlags({ Clang::DebugGddb, Clang::Sanitize_Undefined, Clang::OptDebug });
				analyze_cfg->AddTargetDefinitions({ "__GX_DEBUG__", "__GX_ANALYZE__" });
			}

			auto	profile_cfg = clang_apple->AddConfiguration( "Profile" );
			{
				profile_cfg->AddGlobalCFlags({ Clang::Defines({ "_NDEBUG", "NDEBUG" }), Clang::Opt2 })
							->AddGlobalCxxFlags({ Clang::Defines({ "_NDEBUG", "NDEBUG" }), Clang::Opt2 });

				profile_cfg->AddTargetCxxFlags( shared_cxx_flags )
							->AddTargetCxxFlags({ Clang::Opt2, Clang::InlineAll });
				profile_cfg->AddTargetDefinitions({ "GX_ENABLE_PROFILING" });
			}

			auto	release_cfg = clang_apple->AddConfiguration( "Release" );
			{
				release_cfg->AddGlobalCFlags({ Clang::Defines({ "_NDEBUG", "NDEBUG" }), Clang::Opt3, Clang::InlineAll })
							->AddGlobalCxxFlags({ Clang::Defines({ "_NDEBUG", "NDEBUG" }), Clang::Opt3, Clang::InlineAll });

				release_cfg->AddTargetCxxFlags( shared_cxx_flags )
							->AddTargetCxxFlags({ Clang::Opt3, Clang::OptFast, Clang::OptOmitFramePointers, Clang::InlineAll });
				release_cfg->AddTargetDefinitions({ "__GX_FAST__", "__GX_NO_EXCEPTIONS__" });
			}
		}

		// Clang Android
		auto	clang_android = builder.AddClangCompiler( "CLANG_ANDROID" )->EnableIf("DEFINED ANDROID");
		{
			clang_android->AddSource( "set( CONFIGURATION_DEPENDENT_PATH OFF CACHE INTERNAL \"\" FORCE )" );

			Array<String>	shared_cxx_flags{ Clang::Cpp1z };
			Array<String>	shared_linked_flags{ ClangLinker::Static };

			//builder.Projects_LinkLibrary( "dl log android EGL OpenSLES", "ANDROID" );

			shared_cxx_flags.Append({ Clang::CharSubscripts, Clang::DoublePromotion, Clang::Format, Clang::Main, Clang::MissingBraces, Clang::MissingIncludeDirs,
									  Clang::Uninititalized, Clang::UnknownPragmas, Clang::Pragmas, Clang::StrictAliasing, Clang::StrictOverflow,
									  Clang::Undef, Clang::EndifLabels, Clang::PointerArith, Clang::WriteStrings,
									  Clang::ConversionNull, Clang::EnumCompare, Clang::SignCompare, Clang::SizeofPointerMemaccess,
									  Clang::RTTI, Clang::Exceptions });
			
			shared_cxx_flags.PushBack( Clang::WarningsToErrors({ Clang::InitSelf, Clang::Parentheses, Clang::ReturnLocalAddr, Clang::ReturnType, Clang::UserDefinedLiterals,
															   Clang::ArrayBounds, Clang::DivByZero, Clang::Address, Clang::MissingFieldInit,
															   Clang::CastQual, Clang::UnknownWarning }) );

			shared_cxx_flags.PushBack( Clang::DisableWarnings({ Clang::Comment, Clang::UndefinedInline, Clang::Switch, Clang::Narrowing, Clang::Unused,
															    Clang::Cxx14Extensions, Clang::Cxx1ZExtensions }) );

			auto	debug_cfg = clang_android->AddConfiguration( "Debug" );
			{
				debug_cfg->AddGlobalCFlags({ Clang::Define( "_DEBUG" ), Clang::OptDebug })
						  ->AddGlobalCxxFlags({ Clang::Define( "_DEBUG" ), Clang::OptDebug });

				debug_cfg->AddTargetCxxFlags( shared_cxx_flags )
						 ->AddTargetCxxFlags({ Clang::DebugGddb, Clang::OptDebug });
				debug_cfg->AddTargetLinkerFlags( shared_linked_flags );
				debug_cfg->AddTargetDefinitions({ "__GX_DEBUG__" });
			}

			auto	analyze_cfg = clang_android->AddConfiguration( "DebugAnalyze" );
			{
				analyze_cfg->AddGlobalCFlags({ Clang::Define( "_DEBUG" ), Clang::OptDebug })
							->AddGlobalCxxFlags({ Clang::Define( "_DEBUG" ), Clang::OptDebug });

				analyze_cfg->AddTargetCxxFlags( shared_cxx_flags )
						  ->AddTargetCxxFlags({ Clang::DebugGddb, Clang::Sanitize_Undefined, Clang::OptDebug });
				analyze_cfg->AddTargetLinkerFlags( shared_linked_flags );
				analyze_cfg->AddTargetDefinitions({ "__GX_DEBUG__", "__GX_ANALYZE__" });
			}

			auto	profile_cfg = clang_android->AddConfiguration( "Profile" );
			{
				profile_cfg->AddGlobalCFlags({ Clang::Defines({ "_NDEBUG", "NDEBUG" }), Clang::Opt2 })
							->AddGlobalCxxFlags({ Clang::Defines({ "_NDEBUG", "NDEBUG" }), Clang::Opt2 });

				profile_cfg->AddTargetCxxFlags( shared_cxx_flags )
							->AddTargetCxxFlags({ Clang::Opt2, Clang::InlineAll });
				profile_cfg->AddTargetLinkerFlags( shared_linked_flags );
				profile_cfg->AddTargetDefinitions({ "GX_ENABLE_PROFILING" });
			}

			auto	release_cfg = clang_android->AddConfiguration( "Release" );
			{
				release_cfg->AddGlobalCFlags({ Clang::Defines({ "_NDEBUG", "NDEBUG" }), Clang::Opt3, Clang::InlineAll })
							->AddGlobalCxxFlags({ Clang::Defines({ "_NDEBUG", "NDEBUG" }), Clang::Opt3, Clang::InlineAll });

				release_cfg->AddTargetCxxFlags( shared_cxx_flags )
							->AddTargetCxxFlags({ Clang::Opt3, Clang::OptFast, Clang::OptOmitFramePointers, Clang::InlineAll });
				release_cfg->AddTargetLinkerFlags( shared_linked_flags );
				release_cfg->AddTargetDefinitions({ "__GX_FAST__", "__GX_NO_EXCEPTIONS__" });
			}
		}
	}	// compilers
	//----------------------------------------------------------------------------


	// projects
	{
		// STL //
	#ifdef ENABLE_STL
		auto	engine_config = builder.AddLibrary( "Engine.Config", "Engine/Config" );
		{
			engine_config->AddFoldersRecursive( "" );
		}

		auto	engine_stl = builder.AddLibrary( "Engine.STL", "Engine/STL" );
		{
			engine_stl->AddFoldersRecursive( "" );
			engine_stl->LinkLibrary( "SDL2", "ENABLE_SDL" );
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
	//----------------------------------------------------------------------------


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
			
			engine_platforms->AddDefinition( "GRAPHICS_API_OPENGL=${OPENGL_VERSION}", "DEFINED OPENGL_VERSION AND NOT (OPENGL_VERSION EQUAL \"0\")" );
			engine_platforms->AddDefinition( "COMPUTE_API_OPENCL=${OPENCL_VERSION}", "DEFINED OPENCL_VERSION AND NOT (OPENCL_VERSION EQUAL \"0\")" );
			engine_platforms->AddDefinition( "GRAPHICS_API_VULKAN=${VULKAN_VERSION}", "DEFINED VULKAN_VERSION AND NOT (VULKAN_VERSION EQUAL \"0\")" );
			engine_platforms->AddDefinition( "GRAPHICS_API_SOFT=${SOFTRENDER_VERSION}", "DEFINED SOFTRENDER_VERSION AND NOT (SOFTRENDER_VERSION EQUAL \"0\")" );
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
		
		auto	engine_scene = builder.AddLibrary( "Engine.Scene", "Engine/Scene" );
		{
			engine_scene->AddFoldersRecursive( "" );
			engine_scene->LinkLibrary( engine_graphics );
		}
	#endif	// ENABLE_ENGINE
	//----------------------------------------------------------------------------


		// External //
	#ifdef ENABLE_EXTERNALS
		builder.AddExternal( "External/cmake" );
		builder.Projects_AddDefinition( "PLATFORM_SDL", "ENABLE_SDL" );

		auto	engine_pipeline_compiler = builder.AddLibrary( "Engine.PipelineCompiler", "Engine/PipelineCompiler" );
		{
			engine_pipeline_compiler->ProjFolder( "EngineTools" );
			engine_pipeline_compiler->AddFoldersRecursive( "" );
			engine_pipeline_compiler->LinkLibrary( engine_platforms )->LinkLibrary( "LunarGLASS" );
			engine_pipeline_compiler->IncludeDirectory( "${EXTERNALS_PATH}/LunarGLASS/glslang/.." );
			engine_pipeline_compiler->IncludeDirectory( "${EXTERNALS_PATH}/LunarGLASS/glslang" );
			engine_pipeline_compiler->IncludeDirectory( "${EXTERNALS_PATH}/LunarGLASS/.." );
			engine_pipeline_compiler->IncludeDirectory( "${EXTERNALS_PATH}/LunarGLASS" );
			engine_pipeline_compiler->IncludeDirectory( "${EXTERNALS_PATH}/LunarGLASS/Core/LLVM/llvm-3.4/include/llvm/.." );
			engine_pipeline_compiler->IncludeDirectory( "${CMAKE_BINARY_DIR}/LunarGLASS_bin/LunarGLASS/Core/LLVM/llvm-3.4/include/llvm/.." );
		}

		auto	test_pipeline_compiler = builder.AddExecutable( "Tests.PipelineCompiler", "Tests/PipelineCompiler" );
		{
			test_pipeline_compiler->AddFolder( "" );
			test_pipeline_compiler->AddFolder( "Pipelines" );
			test_pipeline_compiler->LinkLibrary( engine_pipeline_compiler );
		}
		
		auto	engine_script = builder.AddLibrary( "Engine.Scipt", "Engine/Script" );
		{
			engine_script->AddFoldersRecursive( "" );
			engine_script->LinkLibrary( engine_stl )->LinkLibrary( "AngelScript" );
		}

		auto	test_script = builder.AddExecutable( "Tests.Scipt", "Tests/Script" );
		{
			test_script->AddFoldersRecursive( "" );
			test_script->LinkLibrary( engine_script );
		}

		auto	engine_res_pack = builder.AddExecutable( "Engine.ResourcePacker", "Engine/ResourcePacker" );
		{
			engine_res_pack->ProjFolder( "EngineTools" );
			engine_res_pack->AddFoldersRecursive( "" );
			engine_res_pack->LinkLibrary( engine_pipeline_compiler )->LinkLibrary( engine_script );
			engine_res_pack->AddSource(
				"if ( CONFIGURATION_DEPENDENT_PATH )\n"
				"	set( RESOURCE_PACKER_EXE \"${CMAKE_CURRENT_BINARY_DIR}/$<CONFIG>/"_str << engine_res_pack->GetName() << "${CMAKE_EXECUTABLE_SUFFIX}\" )\n"
				"else ()\n"
				"	set( RESOURCE_PACKER_EXE \"${CMAKE_CURRENT_BINARY_DIR}/"_str << engine_res_pack->GetName() << "${CMAKE_EXECUTABLE_SUFFIX}\" )\n"
				"endif ()\n"
			);
		}
	#endif	// ENABLE_EXTERNALS
	//----------------------------------------------------------------------------
		
		
		// Engine //
	#ifdef ENABLE_ENGINE

		// Tests //
		auto	test_engine_base = builder.AddExecutable( "Tests.Engine.Base", "Tests/Engine.Base" );
		{
			test_engine_base->AddFoldersRecursive( "" );
			test_engine_base->LinkLibrary( engine_platforms )->LinkLibrary( engine_profilers );
		}
		
		auto	test_engine_graphics = builder.AddExecutable( "Tests.Engine.Graphics", "Tests/Engine.Graphics" );
		{
			test_engine_graphics->AddFoldersRecursive( "" );
			test_engine_graphics->LinkLibrary( engine_graphics )->LinkLibrary( engine_profilers );
		}

		auto	test_engine_gapi = builder.AddExecutable( "Tests.Engine.Platforms.GAPI", "Tests/Engine.Platforms.GAPI" );
		{
			test_engine_gapi->AddFoldersRecursive( "" );
			test_engine_gapi->LinkLibrary( engine_platforms );
		}
	#endif	// ENABLE_ENGINE
	//----------------------------------------------------------------------------


		// Projects //
	#ifdef ENABLE_PROJECTS
		auto	proj_shader_editor = builder.AddExecutable( "Projects.ShaderEditor", "Projects/ShaderEditor" );
		{
			proj_shader_editor->AddFoldersRecursive( "" );
			proj_shader_editor->LinkLibrary( engine_scene )->LinkLibrary( engine_profilers );
		}
	#endif	// ENABLE_PROJECTS
	//----------------------------------------------------------------------------


		// External //
	#ifdef ENABLE_EXTERNALS
		const auto	PackRes =	LAMBDA( engine_res_pack ) (auto* proj, StringCRef resourceScript)
							{
								proj->AddDependency( engine_res_pack );
								proj->AddSource(
									"if (NOT DEFINED RESOURCE_PACKER_EXE)\n"
									"	message( FATAL_ERROR \"RESOURCE_PACKER_EXE is not defined\" )\n"
									"endif ()\n"
									"add_custom_command(\n"_str
									"	TARGET \"" << proj->GetName() << "\" PRE_BUILD\n"
									"	COMMAND ${RESOURCE_PACKER_EXE} -R \"${CMAKE_SOURCE_DIR}/" << FileAddress::BuildPath( proj->GetRelativePath(), resourceScript ) << "\"\n"
									"	COMMENT \"Pack resources for '" << proj->GetName() << "'...\"\n"
									")\n" );
							};

		if ( test_engine_base )
		{
			PackRes( test_engine_base, "Pipelines/resources.as.c" );
		}

		if ( test_engine_gapi )
		{
			PackRes( test_engine_gapi, "resources.as.c" );
		}

		if ( test_engine_graphics )
		{
			PackRes( test_engine_graphics, "Pipelines/resources.as.c" );
		}

		if ( proj_shader_editor )
		{
			PackRes( proj_shader_editor, "Pipelines/resources.as.c" );
		}
	#endif	// ENABLE_EXTERNALS
	//----------------------------------------------------------------------------


		// Utils //
		auto	util_cmake = builder.AddExecutable( "Utils.CMake", "Utils/CMake" );
		{
			util_cmake->EnableIf( "NOT DEFINED ANDROID" );
			util_cmake->AddFoldersRecursive( "" );
			util_cmake->LinkLibrary( engine_stl );
		}

	#ifdef ENABLE_UTILS
	#endif	// ENABLE_UTILS
	}
	//----------------------------------------------------------------------------

	// save
	builder.Save();
}
