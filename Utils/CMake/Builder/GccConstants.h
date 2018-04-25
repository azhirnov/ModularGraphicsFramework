// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'
/*
	See 
	https://gcc.gnu.org/onlinedocs/gcc-7.2.0/gcc/Warning-Options.html
	https://gcc.gnu.org/onlinedocs/gcc-7.2.0/gcc/Debugging-Options.html
	https://gcc.gnu.org/onlinedocs/gcc-7.2.0/gcc/Optimize-Options.html
	https://gcc.gnu.org/onlinedocs/gcc-7.2.0/gcc/Link-Options.html
*/

#pragma once

namespace CMake
{

// compiler options
namespace GCC
{

	static constexpr char	Cpp11[]					= "-std=c++11";
	static constexpr char	Cpp14[]					= "-std=c++14";
	static constexpr char	Cpp1y[]					= "-std=c++1y";
	static constexpr char	Cpp1z[]					= "-std=c++1z";

	static constexpr char	LibCpp[]				= "-stdlib=libc++";
	static constexpr char	LibStdCpp[]				= "-stdlib=libstdc++";
	
	static constexpr char	RTTI[]					= "-frtti";
	static constexpr char	Exceptions[]			= "-fexceptions";

	static constexpr char	PositionIndependentCode[]	= "-fpic";
	static constexpr char	PositionIndependentCode2[]	= "-fPIC";
	static constexpr char	PositionIndependentExe[]	= "-fpie";

	// Debugging
	static constexpr char	Debug[]					= "-g";
	static constexpr char	DebugGddb[]				= "-ggdb";
	static constexpr char	DebugDwarf[]			= "-gdwarf";
	static constexpr char	DebugStabs[]			= "-gstabs";

	static constexpr char	EliminateUnusedDbgSymb[]= "-feliminate-unused-debug-symbols";
	static constexpr char	CheckPointerBounds[]	= "-fcheck-pointer-bounds";
	static constexpr char	CheckIncompleteType[]	= "-fchkp-check-incomplete-type";

	static constexpr char	Sanitize_Undefined[]			= "-fsanitize=undefined";
	static constexpr char	Sanitize_All[]					= "-fsanitize=all";
	static constexpr char	NoSanitize_All[]				= "-fno-sanitize=all";
	//static constexpr char	Sanitize_Shift[]				= "-fsanitize=shift";
	//static constexpr char	Samitize_IntDivByZero[]			= "-fsanitize=integer-divide-by-zero";
	static constexpr char	Sanitize_FloatDivByZero[]		= "-fsanitize=float-divide-by-zero";
	static constexpr char	Sanitize_FloatCastOverflow[]	= "-fsanitize=float-cast-overflow";


	// Optimizations
	static constexpr char	Opt0[]					= "-O0";
	static constexpr char	Opt1[]					= "-O1";
	static constexpr char	Opt2[]					= "-O2";
	static constexpr char	Opt3[]					= "-O3";
	static constexpr char	OptMinSize[]			= "-Os";
	static constexpr char	OptFast[]				= "-Ofast";
	static constexpr char	OptDebug[]				= "-Og";
	static constexpr char	OptOmitFramePointers[]	= "-fomit-frame-pointer";
	static constexpr char	OptNoOmitFramePointers[]= "-fno-omit-frame-pointer";
	static constexpr char	NoInline[]				= "-fno-inline";
	static constexpr char	InlineAll[]				= "-finline-functions";
	static constexpr char	InlineSmallFunc[]		= "-finline-small-functions";
	static constexpr char	InlineCalledOnce[]		= "-finline-functions-called-once";


	// Warnings
	static constexpr char	AllWarnings[]			= "-Wall -Wextra";
	static constexpr char	FatalErrors[]			= "-Wfatal-errors";
	static constexpr char	WarningsAsErrors[]		= "-Werror";
	static constexpr char	NoWarnings[]			= "-w";
	static constexpr char	Pedantic[]				= "-Wpedantic";

	static constexpr char	CharSubscripts[]		= "-Wchar-subscripts";
	static constexpr char	Comment[]				= "-Wcomment";
	static constexpr char	DoublePromotion[]		= "-Wdouble-promotion";
	static constexpr char	Format[]				= "-Wformat";
	static constexpr char	NonNull[]				= "-Wnonnull";
	static constexpr char	InitSelf[]				= "-Winit-self";
	static constexpr char	IgnoredQualifiers[]		= "-Wignored-qualifiers";
	static constexpr char	Main[]					= "-Wmain";
	static constexpr char	MissingBraces[]			= "-Wmissing-braces";
	static constexpr char	MissingIncludeDirs[]	= "-Wmissing-include-dirs";
	static constexpr char	Parentheses[]			= "-Wparentheses";
	static constexpr char	SequencePoint[]			= "-Wsequence-point";
	static constexpr char	ReturnLocalAddr[]		= "-Wreturn-local-addr";
	static constexpr char	ReturnType[]			= "-Wreturn-type";

	static constexpr char	Switch[]				= "-Wswitch";
	static constexpr char	SwitchDefault[]			= "-Wswitch-default";
	static constexpr char	SwitchEnum[]			= "-Wswitch-enum";
	static constexpr char	Trigraphs[]				= "-Wtrigraphs";

	static constexpr char	UnusedButSetParam[]		= "-Wunused-but-set-parameter";
	static constexpr char	UnusedButSetVariable[]	= "-Wunused-but-set-variable";
	static constexpr char	UnusedFunction[]		= "-Wunused-function";
	static constexpr char	UnusedLabel[]			= "-Wunused-label";
	static constexpr char	UnusedLocalTypedefs[]	= "-Wunused-local-typedefs";
	static constexpr char	UnusedParameter[]		= "-Wunused-parameter";
	static constexpr char	UnusedResult[]			= "-Wunused-result";
	static constexpr char	UnusedVariable[]		= "-Wunused-variable";
	static constexpr char	UnusedValue[]			= "-Wunused-value";
	static constexpr char	Unused[]				= "-Wunused";

	static constexpr char	Uninititalized[]		= "-Wuninitialized";
	static constexpr char	MayBeUninitialized[]	= "-Wmaybe-uninitialized";
	static constexpr char	UnknownPragmas[]		= "-Wunknown-pragmas";
	static constexpr char	Pragmas[]				= "-Wpragmas";

	static constexpr char	StrictAliasing[]		= "-Wstrict-aliasing";
	static constexpr char	StrictOverflow[]		= "-Wstrict-overflow";

	static constexpr char	ArrayBounds[]			= "-Warray-bounds";		// require '-ftree-vrp'
	static constexpr char	DivByZero[]				= "-Wdiv-by-zero";
	static constexpr char	SystemHeaders[]			= "-Wsystem-headers";
	static constexpr char	Trampolines[]			= "-Wtrampolines";
	static constexpr char	FloatEqual[]			= "-Wfloat-equal";
	static constexpr char	Undef[]					= "-Wundef";
	static constexpr char	EndifLabels[]			= "-Wendif-labels";
	static constexpr char	Shadow[]				= "-Wshadow";
	static constexpr char	ShadowGlobal[]			= "-Wshadow=global";
	static constexpr char	ShadowLocal[]			= "-Wshadow=local";
	static constexpr char	FreeNonheapObject[]		= "-Wfree-nonheap-object";
	static constexpr char	UnsafeLoopOpt[]			= "-Wunsafe-loop-optimizations";

	static constexpr char	PedanticMsFormat[]		= "-Wpedantic-ms-format";	// MinGW only

	static constexpr char	PointerArith[]			= "-Wpointer-arith";
	static constexpr char	TypeLimits[]			= "-Wtype-limits";
	static constexpr char	CastQual[]				= "-Wcast-qual";
	static constexpr char	CastAlign[]				= "-Wcast-align";
	static constexpr char	WriteStrings[]			= "-Wwrite-strings";
	static constexpr char	Clobbered[]				= "-Wclobbered";
	static constexpr char	Conversion[]			= "-Wconversion";
	static constexpr char	ConversionNull[]		= "-Wconversion-null";
	static constexpr char	ZeroAsNullConst[]		= "-Wzero-as-null-pointer-constant";
	static constexpr char	UselessCast[]			= "-Wuseless-cast";
	static constexpr char	EmptyBody[]				= "-Wempty-body";
	static constexpr char	EnumCompare[]			= "-Wenum-compare";
	static constexpr char	SignCompare[]			= "-Wsign-compare";
	static constexpr char	SignConvertsion[]		= "-Wsign-conversion";
	static constexpr char	SizeofPointerMemaccess[]= "-Wsizeof-pointer-memaccess";
	//static constexpr char	SizeofPointerDiv[]		= "-Wsizeof-pointer-div";
	static constexpr char	Address[]				= "-Waddress";
	static constexpr char	LogicalOp[]				= "-Wlogical-op";
	static constexpr char	AggregateReturn[]		= "-Waggregate-return";
	static constexpr char	AggressiveLoopOpt[]		= "-Waggressive-loop-optimizations";
	static constexpr char	MissingDeclarations[]	= "-Wmissing-declarations";
	static constexpr char	MissingFieldInit[]		= "-Wmissing-field-initializers";
	static constexpr char	WarnInline[]			= "-Winline";
	static constexpr char	InvalidOffsetof[]		= "-Winvalid-offsetof";

	static constexpr char	NonTemplateFriend[]		= "-Wnon-template-friend";
	//static constexpr char	VariadicMacros[]		= "-Wvariadic-macros";

	static constexpr char	AlignedNew[]			= "-Waligned-new";
	static constexpr char	PlacementNew[]			= "-Wplacement-new";

	static constexpr char	LiteralSuffix[]			= "-Wliteral-suffix";
	static constexpr char	Narrowing[]				= "-Wnarrowing";
	static constexpr char	Terminate[]				= "-Wterminate";
	static constexpr char	Attribute[]				= "-Wattributes";

	static constexpr char	Reorder[]				= "-Wreorder";

	inline String WarningToError (StringCRef code)
	{
		const usize	off = code.StartsWithIC( "-Wno-" ) ? 5u : code.StartsWithIC( "-W" ) ? 2u : 0u;

		return "-Werror="_str << code.SubString( off );
	}

	inline String WarningsToErrors (ArrayCRef<StringCRef> codes)
	{
		String	res;
		FOR( i, codes ) {
			res << WarningToError( codes[i] ) << ' ';
		}
		return res;
	}
	
	inline String DisableWarning (StringCRef code)
	{
		const usize	off = code.StartsWithIC( "-Wno-" ) ? 5u : code.StartsWithIC( "-W" ) ? 2u : 0u;

		return "-Wno-"_str << code.SubString( off );
	}

	inline String DisableWarnings (ArrayCRef<StringCRef> codes)
	{
		String	res;
		FOR( i, codes ) {
			res << DisableWarning( codes[i] ) << ' ';
		}
		return res;
	}

	inline String Define (StringCRef definition)
	{
		return "-D"_str << definition;
	}

	inline String Defines (ArrayCRef<StringCRef> definitions)
	{
		String	res;
		FOR( i, definitions ) {
			res << Define( definitions[i] ) << ' ';
		}
		return res;
	}


}	// GCC


// linker options
namespace GccLinker
{

	static constexpr char	StaticLibGCC[]		= "-static-libgcc";
	static constexpr char	StaticLibStdCPP[]	= "-static-libstdc++";

	static constexpr char	NoDefaultLibs[]		= "-nodefaultlibs";
	static constexpr char	NoStdLib[]			= "-nostdlib";

	static constexpr char	PositionIndependentExe[]	= "-pie";

	static constexpr char	Static[]			= "-static";		// On systems that support dynamic linking, this prevents linking with the shared libraries.
	static constexpr char	Shared[]			= "-shared";		// Produce a shared object which can then be linked with other objects to form an executable.
	static constexpr char	Dynamic[]			= "-rdynamic";		// This instructs the linker to add all symbols, not only used ones, to the dynamic symbol table.
	static constexpr char	Split[]				= "-s";				// Remove all symbol table and relocation information from the executable.

	static constexpr char	Symbolic[]			= "-symbolic";

	inline String Link (StringCRef libName)
	{
		return "-l"_str << libName;
	}

	inline String Link (ArrayCRef<StringCRef> libs)
	{
		String	str;
		FOR( i, libs ) {
			str << " -l" << libs[i];
		}
		return str;
	}

}	// GccLinker

}	// CMake
