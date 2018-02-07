// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "GccConstants.h"

namespace CMake
{
namespace Clang
{
	using namespace GCC;


	static constexpr char	Comment[]			= "-Wcomment";
	static constexpr char	UndefinedInline[]	= "-Wundefined-inline";
	static constexpr char	Deprecated[]		= "-Wdeprecated-declarations";

	static constexpr char	Narrowing[]			= "-Wc++11-narrowing";
	static constexpr char	Cxx14Extensions[]	= "-Wc++14-extensions";
	static constexpr char	Cxx1ZExtensions[]	= "-Wc++1z-extensions";

	static constexpr char	PedanticErrors[]	= "-pedantic-errors";


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
