// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "PipelineBuilding.h"

CMakeBuilderPtr  GenCompilers ()
{
	CMakeBuilderPtr		builder{ new CMakeBuilder{ "", "" } };

	builder->SetSecondary();

	// compilers
	{
		// Visual C++
		auto	msvc = builder->AddMSVisualStudioCompiler();
		{
			msvc->AddSource( "set( CONFIGURATION_DEPENDENT_PATH ON CACHE INTERNAL \"\" FORCE )" );

			const String	dbg_lib = VS::MultiThreadedDebugDll;
			const String	rel_lib = VS::MultiThreadedDll;

			Array<String>	shared_linker_cfg	= { VS_Linker::OptReferences, VS_Linker::COMDATFolding, VS_Linker::NoIncrementalLinking };

			Array<String>	shared_cxx_flags	= { VS::CppLastest, VS::MultiThreadCompilation, VS::NoMinimalRebuild, VS::RemUnrefCode,
													VS::NoFunctionLevelLinking, VS::FloatPointStrict, VS::FloatPointNoExceptions };

			Array<String>	release_cxx_flags	= { VS::InlineAll, VS::Intrinsic, VS::OptFavorFastCode, VS::OptOmitFramePointers,
													VS::OptFiberSafe, VS::OptWholeProgram, VS::StringPooling, VS::NoSecurityCheck, rel_lib };

			Set<uint>		errors				= { VS::ReturningAddressOfLocalVariable, VS::TypeNameMismatch, VS::UninitializedVarUsed,
													VS::TooManyParamsForMacro, VS::RecursiveOnAllControlPaths, VS::IllegalConversion,
													VS::UnrecognizedEscapeSequence, VS::MultipleAssignmentOperators,
													VS::InconsistentDllLinkage, VS::ClassNeedsDllInterface, VS::EmptyControlledStatement,
													VS::AssignInConditionalExpr, VS::ShiftCountNegativeOrBig, VS::ForcingValueToBool,
													VS::OperatorPrecedence };
			Set<uint>		enabled_warnings	= { VS::InitOrder, VS::UnknownMacro, VS::UnsafeFunctionorVariable, VS::ConditionalExprIsConstant,
													VS::ReintCastBetwenRelatedClasses, VS::UnreachableCode };
			Set<uint>		disabled_warnings	= { VS::DecoratedNameWasTruncated, VS::CastTruncatesConstantValue, VS::UnusedInlineFunction,
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
			enabled_warnings.AddArray( VS::MayBeUninitialized );

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
						  ->AddGlobalCxxFlags({ VS::Define( "_DEBUG" ), dbg_lib, VS::OptDisabled, VS::MultiThreadCompilation, VS::DbgProgramDatabase })
						  ->AddGlobalLinkerFlags({ VS_Linker::DebugFull/*, VS_Linker::LinkTimeCodeGen*/ });

				debug_cfg->AddTargetCxxFlags( shared_cxx_flags )
						 ->AddTargetCxxFlags({ VS::WarningLevel4, VS::NoWarningsAsErrors, VS::SDLChecks, VS::OptDisabled, VS::NoInline, VS::Exceptions,
												VS::OptNoOmitFramePointers, VS::NoStringPooling, VS::SecurityCheck, /*VS::ControlFlowGuard,*/ VS::RTTI,
												dbg_lib, VS::NoStaticAnalyze, VS::DbgProgramDatabase, VS::StackFrameAndUninitVarCheck });
				debug_cfg->AddTargetLinkerFlags( shared_linker_cfg )->AddTargetLinkerFlags({ VS_Linker::DebugFull/*, VS_Linker::LinkTimeCodeGen*/ });
				debug_cfg->AddTargetDefinitions({ "__GX_DEBUG__" });
			}

			auto	analyze_cfg = msvc->AddConfiguration( "DebugAnalyze" );
			{
				analyze_cfg->AddGlobalCFlags({ VS::Define( "_DEBUG" ), dbg_lib, VS::OptDisabled, VS::MultiThreadCompilation })
							->AddGlobalCxxFlags({ VS::Define( "_DEBUG" ), dbg_lib, VS::OptDisabled, VS::MultiThreadCompilation, VS::DbgProgramDatabase })
							->AddGlobalLinkerFlags({ VS_Linker::DebugFull/*, VS_Linker::LinkTimeCodeGen*/ });

				analyze_cfg->AddTargetCxxFlags( shared_cxx_flags )
							->AddTargetCxxFlags({ VS::WarningLevel4, VS::NoWarningsAsErrors, VS::SDLChecks, VS::OptDisabled, VS::NoInline, VS::Exceptions,
												  VS::OptNoOmitFramePointers, VS::NoStringPooling, VS::SecurityCheck, VS::ControlFlowGuard, VS::RTTI,
												  dbg_lib, VS::StaticAnalyze, VS::DbgProgramDatabase, VS::StackFrameAndUninitVarCheck });
				analyze_cfg->AddTargetLinkerFlags( shared_linker_cfg )->AddTargetLinkerFlags({ VS_Linker::DebugFull/*, VS_Linker::LinkTimeCodeGen*/ });
				analyze_cfg->AddTargetDefinitions({ "__GX_DEBUG__", "__GX_ANALYZE__" });
			}

			auto	profile_cfg = msvc->AddConfiguration( "Profile" );
			{
				profile_cfg->AddGlobalCFlags({ VS::Defines({ "_NDEBUG", "NDEBUG" }), rel_lib, VS::OptDisabled, VS::MultiThreadCompilation })
							->AddGlobalCxxFlags({ VS::Defines({ "_NDEBUG", "NDEBUG" }), rel_lib, VS::OptDisabled, VS::MultiThreadCompilation })
							->AddGlobalLinkerFlags({ /*VS_Linker::LinkTimeCodeGen,*/ VS_Linker::Debug, VS_Linker::Profile });

				profile_cfg->AddTargetCxxFlags( shared_cxx_flags )->AddTargetCxxFlags( release_cxx_flags )
						->AddTargetCxxFlags({ VS::WarningLevel3, VS::OptDisabled, VS::NoStaticAnalyze, VS::Exceptions, VS::RTTI });
				profile_cfg->AddTargetLinkerFlags( shared_linker_cfg )
							->AddTargetLinkerFlags({ /*VS_Linker::LinkTimeCodeGen,*/ VS_Linker::Debug, VS_Linker::Profile });
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
							->AddTargetCxxFlags({ VS::WarningLevel3, VS::OptFull, VS::NoStaticAnalyze, VS::NoExceptions/*, VS::NoRTTI*/ });
				release_cfg->AddTargetLinkerFlags( shared_linker_cfg )
							->AddTargetLinkerFlags({ VS_Linker::LinkTimeCodeGen, VS_Linker::Release, VS_Linker::RandomBaseAddress });
				release_cfg->AddTargetDefinitions({ /*"__GX_FAST__",*/ "__GX_NO_EXCEPTIONS__" });
			}

			builder->SetSystemVersion( "8.1", "WIN32" );
		}

		// GCC
		auto	gcc = builder->AddGCCompiler();
		{
			gcc->AddSource( "set( CONFIGURATION_DEPENDENT_PATH OFF CACHE INTERNAL \"\" FORCE )" );

			Array<String>	shared_cxx_flags{ GCC::Cpp1z };
			Array<String>	shared_linked_flags{ GccLinker::Static, GccLinker::StaticLibGCC, GccLinker::StaticLibStdCPP };

			shared_cxx_flags.Append({ /*GCC::Pedantic,*/ GCC::CharSubscripts, GCC::DoublePromotion, GCC::Format, GCC::Main, GCC::MissingBraces, GCC::MissingIncludeDirs,
									  GCC::Uninititalized, GCC::MayBeUninitialized, GCC::UnknownPragmas, GCC::Pragmas, GCC::StrictAliasing, GCC::StrictOverflow,
									  GCC::Undef, GCC::EndifLabels, GCC::FreeNonheapObject, GCC::PointerArith, GCC::CastAlign, GCC::WriteStrings,
									  /*GCC::Conversion,*/ GCC::ConversionNull, /*GCC::ZeroAsNullConst,*/ GCC::EnumCompare, GCC::SignCompare, /*GCC::SignConvertsion,*/
									  GCC::SizeofPointerMemaccess, /*GCC::SizeofPointerDiv,*/ GCC::LogicalOp, GCC::Address/*, GCC::OldStyleCast*/ });

			shared_cxx_flags.PushBack( GCC::DisableWarnings({ GCC::Unused, GCC::NonTemplateFriend, GCC::ZeroAsNullConst, GCC::Shadow, GCC::EnumCompare,
															  GCC::Narrowing, GCC::Attribute, GCC::InvalidOffsetof }) );
			
			shared_cxx_flags.PushBack( GCC::WarningsToErrors({ GCC::InitSelf, GCC::Parentheses, GCC::ReturnLocalAddr, GCC::ReturnType,
															   GCC::ArrayBounds, GCC::DivByZero, GCC::MissingFieldInit, /*GCC::AlignedNew,*/
															   GCC::PlacementNew, GCC::SignCompare, GCC::CastQual, GCC::CastAlign, GCC::LiteralSuffix,
															   GCC::ShadowLocal, GCC::DeleteIncomplete, GCC::SubobjectLinkage, GCC::ODR,
															   GCC::OldStyleDeclaration, GCC::OldStyleDefinition }) );


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
				release_cfg->AddTargetDefinitions({ /*"__GX_FAST__",*/ "__GX_NO_EXCEPTIONS__" });
			}
		}

		// Clang (Windows, Linux)
		#if 0
		auto	clang = builder->AddClangCompiler()->EnableIf( "WIN32 OR UNIX" );
		{
			clang->AddSource( "set( CONFIGURATION_DEPENDENT_PATH OFF CACHE INTERNAL \"\" FORCE )" );

			Array<String>	shared_cxx_flags{ Clang::Cpp1z };

			//shared_cxx_flags.Append({ Clang::Shadow, });

			shared_cxx_flags.Append({ /*Clang::Pedantic,*/ Clang::CharSubscripts, Clang::DoublePromotion, Clang::Format, Clang::Main, Clang::MissingBraces, Clang::MissingIncludeDirs,
									  Clang::Unused, Clang::Uninititalized, Clang::MayBeUninitialized, Clang::UnknownPragmas, Clang::Pragmas, Clang::StrictAliasing, Clang::StrictOverflow,
									  Clang::Undef, Clang::EndifLabels, Clang::FreeNonheapObject, Clang::PointerArith, Clang::WriteStrings,
									  /*Clang::Conversion,*/ Clang::ConversionNull, Clang::ZeroAsNullConst, Clang::EnumCompare, Clang::SignCompare, /*Clang::SignConvertsion,*/ Clang::SizeofPointerMemaccess,
									  Clang::LogicalOp, Clang::RTTI, Clang::Exceptions, Clang::LoopAnalysis, Clang::IncrementBool });
			
			shared_cxx_flags.PushBack( Clang::WarningsToErrors({ Clang::InitSelf, Clang::Parentheses, Clang::ReturnLocalAddr, Clang::ReturnType, Clang::NonTemplateFriend,
																Clang::ArrayBounds, Clang::DivByZero, Clang::Address, Clang::MissingFieldInit, /*Clang::AlignedNew,*/
																Clang::PlacementNew, Clang::CastQual, Clang::CastAlign, Clang::UnknownWarning, Clang::UserDefinedLiterals,
																Clang::KeywordMacro, Clang::LargeByValueCopy, Clang::InstantiationAfterSpec, Clang::MethodSignatures,
															    Clang::SelfAssign, Clang::SelfMove, Clang::InfiniteRecursion, Clang::PessimizingMove, Clang::DanglingElse }) );

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
				release_cfg->AddTargetDefinitions({ /*"__GX_FAST__",*/ "__GX_NO_EXCEPTIONS__" });
			}
		}
		#endif

		// Clang MacOS
		auto	clang_apple = builder->AddClangCompiler( "CLANG_APPLE" )->EnableIf("APPLE");
		{
			clang_apple->AddSource( "set( CONFIGURATION_DEPENDENT_PATH ON CACHE INTERNAL \"\" FORCE )" );

			Array<String>	shared_cxx_flags{ Clang::Cpp1z };

			shared_cxx_flags.Append({ Clang::CharSubscripts, Clang::DoublePromotion, Clang::Format, Clang::Main, Clang::MissingBraces, Clang::MissingIncludeDirs,
									  Clang::Uninititalized, Clang::UnknownPragmas, Clang::Pragmas, Clang::StrictAliasing, Clang::StrictOverflow,
									  Clang::Undef, Clang::EndifLabels, Clang::PointerArith, Clang::WriteStrings,
									  Clang::ConversionNull, Clang::EnumCompare, Clang::SignCompare, Clang::SizeofPointerMemaccess,
									  Clang::RTTI, Clang::Exceptions, Clang::LoopAnalysis, Clang::IncrementBool });
			
			shared_cxx_flags.PushBack( Clang::WarningsToErrors({ Clang::InitSelf, Clang::Parentheses, Clang::ReturnLocalAddr, Clang::ReturnType, Clang::UserDefinedLiterals,
																Clang::ArrayBounds, Clang::DivByZero, Clang::Address, Clang::MissingFieldInit, Clang::InstantiationAfterSpec,
																Clang::CastQual, Clang::UnknownWarning, Clang::KeywordMacro, Clang::LargeByValueCopy, Clang::MethodSignatures,
																Clang::SelfAssign, Clang::SelfMove, Clang::InfiniteRecursion, Clang::PessimizingMove, Clang::DanglingElse }) );

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
				release_cfg->AddTargetDefinitions({ /*"__GX_FAST__",*/ "__GX_NO_EXCEPTIONS__" });
			}
		}

		// Clang Android
		auto	clang_android = builder->AddClangCompiler( "CLANG_ANDROID" )->EnableIf("DEFINED ANDROID");
		{
			clang_android->AddSource( "set( CONFIGURATION_DEPENDENT_PATH OFF CACHE INTERNAL \"\" FORCE )" );

			Array<String>	shared_cxx_flags{ Clang::Cpp1z };
			Array<String>	shared_linked_flags{ ClangLinker::Static };

			//builder->Projects_LinkLibrary( "dl log android EGL OpenSLES", "ANDROID" );

			shared_cxx_flags.Append({ Clang::CharSubscripts, Clang::DoublePromotion, Clang::Format, Clang::Main, Clang::MissingBraces, Clang::MissingIncludeDirs,
									  Clang::Uninititalized, Clang::UnknownPragmas, Clang::Pragmas, Clang::StrictAliasing, Clang::StrictOverflow,
									  Clang::Undef, Clang::EndifLabels, Clang::PointerArith, Clang::WriteStrings,
									  Clang::ConversionNull, Clang::EnumCompare, Clang::SignCompare, Clang::SizeofPointerMemaccess,
									  Clang::RTTI, Clang::Exceptions, Clang::LoopAnalysis, Clang::IncrementBool });
			
			shared_cxx_flags.PushBack( Clang::WarningsToErrors({ Clang::InitSelf, Clang::Parentheses, Clang::ReturnLocalAddr, Clang::ReturnType, Clang::UserDefinedLiterals,
																Clang::ArrayBounds, Clang::DivByZero, Clang::Address, Clang::MissingFieldInit, Clang::InstantiationAfterSpec,
																Clang::CastQual, Clang::UnknownWarning, Clang::KeywordMacro, Clang::LargeByValueCopy, Clang::DanglingElse,
															    Clang::MethodSignatures, Clang::SelfAssign, Clang::SelfMove, Clang::InfiniteRecursion, Clang::PessimizingMove }) );

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
				release_cfg->AddTargetDefinitions({ /*"__GX_FAST__",*/ "__GX_NO_EXCEPTIONS__" });
			}
		}
	}	// compilers

	return builder;
}
