// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'
/*
	https://clang.llvm.org/docs/DiagnosticsReference.html
*/

#pragma once

#include "GccConstants.h"

namespace CMake
{
namespace Clang
{
	using namespace GCC;


	static constexpr char	Comment[]				= "-Wcomment";
	static constexpr char	UndefinedInline[]		= "-Wundefined-inline";
	static constexpr char	Deprecated[]			= "-Wdeprecated-declarations";

	static constexpr char	Narrowing[]				= "-Wc++11-narrowing";
	static constexpr char	Cxx14Extensions[]		= "-Wc++14-extensions";
	static constexpr char	Cxx1ZExtensions[]		= "-Wc++1z-extensions";

	static constexpr char	PedanticErrors[]		= "-pedantic-errors";
	static constexpr char	UnknownWarning[]		= "-Wunknown-warning-option";
	static constexpr char	ReturnLocalAddr[]		= "-Wreturn-stack-address";

	static constexpr char	UserDefinedLiterals[]	= "-Wuser-defined-literals";

	static constexpr char	ShadowAll[]				= "-Wshadow-all";

	static constexpr char	UnusedArgument[]		= "-Wunused-argument";

	static constexpr char	LoopAnalysis[]			= "-Wloop-nalysis";

	static constexpr char	IncrementBool[]			= "-Wincrement-bool";

	static constexpr char	KeywordMacro[]			= "-Wkeyword-macro";

	static constexpr char	LargeByValueCopy[]		= "-Wlarge-by-value-copy";

	static constexpr char	InstantiationAfterSpec[]	= "-Winstantiation-after-specialization";

	static constexpr char	MethodSignatures[]			= "-Wmethod-signatures";

	static constexpr char	SelfAssign[]				= "-Wself-assign";
	static constexpr char	SelfMove[]					= "-Wself-move";

	static constexpr char	InfiniteRecursion[]			= "-Winfinite-recursion";

	static constexpr char	PessimizingMove[]			= "-Wpessimizing-move";

	static constexpr char	DanglingElse[]				= "-Wdangling-else";

	// Microsoft extensions
	static constexpr char	MSExtensions[]				= "-fms-extensions";
	static constexpr char	MSCompatibility[]			= "-fms-compatibility";
	static constexpr char	DelayedTemplateParsing[]	= "-fdelayed-template-parsing";

}	// Clang


namespace ClangLinker
{
	using namespace GccLinker;

}	// ClangLinker
}	// CMake
