// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

namespace CMake
{

// compileroptions
namespace VS
{

	// Warnings
	static constexpr char	NoWarnings[]			= "/W0";
	static constexpr char	WarningLevel1[]			= "/W1";
	static constexpr char	WarningLevel2[]			= "/W2";
	static constexpr char	WarningLevel3[]			= "/W3";
	static constexpr char	WarningLevel4[]			= "/W4";
	static constexpr char	AllWarnings[]			= "/Wall";
	static constexpr char	NoWarningsAsErrors[]	= "/WX-";
	static constexpr char	WarningsAsErrors[]		= "/WX";
	static constexpr char	StaticAnalyze[]			= "/analyze";	// enable compile-time code analizer
	static constexpr char	NoStaticAnalyze[]		= "/analyze-";


	static constexpr uint	NotAllControlPathsReturnAValue		= 4715;		// 'function' : not all control paths return a value
	static constexpr uint	FuncMustReturnValue					= 4716;		// 'function' must return a value

	static constexpr uint	ReturningAddressOfLocalVariable		= 4172;		// returning address of local variable or temporary
	static constexpr uint	TypeNameMismatch					= 4099;		// 'identifier' : type name first seen using 'objecttype1' now seen using 'objecttype2'

	static constexpr uint	ReturnType[]						= { NotAllControlPathsReturnAValue, FuncMustReturnValue, ReturningAddressOfLocalVariable };

	static constexpr uint	MemberNoOverride[]					= { 4263,	// 'function' : member function does not override any base class virtual member function
																	4264,	// 'virtual_function' : no override available for virtual member function from base 'class'; function is hidden
																	4266 };	// 'function' : no override available for virtual member function from base 'type'; function is hidden

	static constexpr uint	OperatorHasNoEffect[]				= { 4552,	// 'operator' : operator has no effect; expected operator with side-effect
																	4553 };	// 'operator' : operator has no effect; did you intend 'operator'?

	static constexpr uint	TooManyParamsForMacro				= 4002;		// too many actual parameters for macro
	static constexpr uint	ReintCastBetwenRelatedClasses		= 4946;		// reinterpret_cast used between related classes
	static constexpr uint	RecursiveOnAllControlPaths			= 4717;		// 'function' : recursive on all control paths, function will cause runtime stack overflow

	static constexpr uint	ShadowLocal							= 4456;		// declaration of 'identifier' hides previous local declaration
	static constexpr uint	ShadowGlobal						= 4459;		// declaration of 'identifier' hides global declaration
	static constexpr uint	ShadowMember						= 4458;		// declaration of 'identifier' hides class member
	static constexpr uint	ShadowArg							= 4457;		// declaration of 'identifier' hides function parameter
	static constexpr uint	Shadow[]							= { ShadowLocal, ShadowGlobal, ShadowMember, ShadowArg };

	static constexpr uint	UnrecognizedEscapeSequence			= 4129;		// 'character': unrecognized character escape sequence
	static constexpr uint	UnreachableCode						= 4702;		// unreachable code
	static constexpr uint	MultipleAssignmentOperators			= 4522;		// multiple assignment operators specified

	static constexpr uint	Format[]							= { 4473,	// not enough argument passed for format string
																	4474 };	// too many arguments passed for fomat string

	static constexpr uint	ConversionToRef						= 4239;		// nonstandard extension used: 'argument': conversion from 'type' to 'type &'
	static constexpr uint	ClassRValueUsedAsLValue				= 4238;		// nonstandard extension used: class rvalue used as lvalue
	static constexpr uint	NamelessStructOrUnion				= 4201;		// nonstandard extension used : nameless struct/union
	static constexpr uint	UnderlyingTypeForEnum				= 4480;		// nonstandard extension used: specifying underlying type for enum

	static constexpr uint	Extensions[]						= { ConversionToRef, ClassRValueUsedAsLValue, NamelessStructOrUnion, UnderlyingTypeForEnum };

	static constexpr uint	UnusedFormalParameter				= 4100;		// unreferenced formal parameter
	static constexpr uint	UnusedLocalVariable					= 4189;		// 'variable': local variable is initialized but not referenced
	static constexpr uint	UnusedLocalFunction					= 4505;		// unreferenced local function has been removed
	static constexpr uint	UnusedInlineFunction				= 4514;		// unreferenced inline function has been removed
	static constexpr uint	Unused[]							= { UnusedFormalParameter, UnusedLocalFunction, UnusedInlineFunction, UnusedLocalVariable };

	static constexpr uint	CastTruncatesConstantValue			= 4310;		// cast truncates constant value

	static constexpr uint	Conversion[]						= { 4244,	// 'initializing': conversion from '...' to '...', possible loss of data
																	4838,	// conversion from 'type_1' to 'type_2' requires a narrowing conversion
																	CastTruncatesConstantValue };
	
	static constexpr uint	ExpressionSignedUnsignedMismatch	= 4018;		// 'expression' : signed/unsigned mismatch
	static constexpr uint	ConversionSignedUnsignedMismatch	= 4365;		// 'action' : conversion from 'type_1' to 'type_2', signed/unsigned mismatch
	static constexpr uint	UnsignedNegativeConstantMismatch	= 4287;		// unsigned/negative constant mismatch
	static constexpr uint	ConversionSign[]					= { 4245,	// 'return': conversion from signed to unsigned
																	4389,	// '==': signed/unsigned mismatch
																	ExpressionSignedUnsignedMismatch,
																	ConversionSignedUnsignedMismatch,
																	UnsignedNegativeConstantMismatch };

	static constexpr uint	Switch[]							= { 4061,	// enumerator 'identifier' in switch of enum 'enumeration' is not explicitly handled by a case label
																	4062,	// enumerator 'identifier' in switch of enum 'enumeration' is not handled
																	4063 };	// case 'number' is not a valid value for switch of enum 'type'
	
	static constexpr uint	InitOrder							= 5038;		// data member '...' will be initialized after data member '...'
	static constexpr uint	UnknownMacro						= 4668;		// '...' is not defined as a preprocessor macro, replacing with '0' for '#if/#elif'
	
	static constexpr uint	DeletedConstructor[]				= { 4623,	// default constructor was implicitly defined as deleted
																	4625,	// copy constructor was implicitly defined as deleted
																	4626,	// assignment operator was implicitly defined as deleted
																	5026,	// move constructor was implicitly defined as deleted
																	5027 };	// move assignment operator was implicitly defined as deleted

	static constexpr uint	FunctionNotInlined[]				= { 4710,	// function not inlined
																	4714 };	// function '...' marked as __forceinline not inlined

	static constexpr uint	InconsistentDllLinkage				= 4273;		// inconsistent dll linage
	static constexpr uint	ClassNeedsDllInterface				= 4251;		// '...' class '...' needs to have dll-interface to be used by clients of class '...'

	static constexpr uint	UnsafeFunctionorVariable			= 4996;		// this function or variable may be unsafe
	static constexpr uint	DecoratedNameWasTruncated			= 4503;		// decorated name length exceeded, name was truncated

	static constexpr uint	ConditionalExprIsConstant			= 4127;		// conditional expression is constant	(use if constexpr)

	static constexpr uint	UninitializedVarUsed				= 4700;		// uninitialized local variable 'name' used


	inline String WarningToError (uint code)
	{
		return "/we"_str << code;
	}

	inline String WarningsToErrors (ArrayCRef<uint> codes)
	{
		String	res;
		FOR( i, codes ) {
			res << WarningToError( codes[i] ) << ' ';
		}
		return res;
	}

	inline String DisableWarning (uint code)
	{
		return "/wd"_str << code;
	}

	inline String DisableWarnings (ArrayCRef<uint> codes)
	{
		String	res;
		FOR( i, codes ) {
			res << DisableWarning( codes[i] ) << ' ';
		}
		return res;
	}

	inline String EnableWarning (uint code)
	{
		return "/w1"_str << code;
	}

	inline String EnableWarnings (ArrayCRef<uint> codes)
	{
		String	res;
		FOR( i, codes ) {
			res << EnableWarning( codes[i] ) << ' ';
		}
		return res;
	}


	// Runtime checks
	static constexpr char	SmallerTypeCheck[]		= "/RTCc";
	static constexpr char	SDLChecks[]				= "/sdl";
	static constexpr char	NoSDLChecks[]			= "/sdl-";
	static constexpr char	StackFrameCheck[]		= "/RTCs";
	static constexpr char	UninitVarCheck[]		= "/RTCu";
	static constexpr char	StackFrameAndUninitVarCheck[] = "/RTCsu";
	static constexpr char	SecurityCheck[]			= "/GS";
	static constexpr char	NoSecurityCheck[]		= "/GS-";				// override /sdl-
	static constexpr char	ControlFlowGuard[]		= "/guard:cf";

	// Optimization
	static constexpr char	OptDisabled[]			= "/Od";
	static constexpr char	OptMinSize[]			= "/O1";
	static constexpr char	OptMaxSpeed[]			= "/O2";
	static constexpr char	OptFull[]				= "/Ox";
	static constexpr char	OptFavorSmallCode[]		= "/Os";
	static constexpr char	OptFavorFastCode[]		= "/Ot";
	static constexpr char	OptNoOmitFramePointers[]= "/Oy-";
	static constexpr char	OptOmitFramePointers[]	= "/Oy";
	static constexpr char	OptFiberSafe[]			= "/GT";
	static constexpr char	OptWholeProgram[]		= "/GL";

	// Inline
	static constexpr char	NoInline[]				= "/Ob0";
	static constexpr char	InlineOnly[]			= "/Ob1";
	static constexpr char	InlineAll[]				= "/Ob2";
	static constexpr char	Intrinsic[]				= "/Oi";

	// Exceptions
	static constexpr char	ExceptionsWithSEH[]		= "/EHa";	// TODO
	static constexpr char	Exceptions[]			= "/EHsc";
	static constexpr char	ExceptionsWithExternC[]	= "/EHs";
	static constexpr char	NoExceptions[]			= "/EHs-c-";

	// Build
	static constexpr char	StringPooling[]			= "/GF";
	static constexpr char	NoStringPooling[]		= "/GF-";
	static constexpr char	MinimalRebuild[]		= "/Gm";
	static constexpr char	NoMinimalRebuild[]		= "/Gm-";
	static constexpr char	MultiThreadCompilation[]= "/MP";
	static constexpr char	ParallelLoops[]			= "/Qpar";		// enable '#pragma loop(hint_parallel(n))' directive
	static constexpr char	RemUnrefCode[]			= "/Zc:inline";
	static constexpr char	RTTI[]					= "/GR";
	static constexpr char	NoRTTI[]				= "/GR-";
	static constexpr char	OpenMP[]				= "/openmp";
	static constexpr char	CompileAsC[]			= "/TC";
	static constexpr char	CompileAsCPP[]			= "/TP";

	// Calling convention
	static constexpr char	Cdecl[]					= "/Gd";
	static constexpr char	Fascall[]				= "/Gr";
	static constexpr char	Stdcall[]				= "/Gz";
	static constexpr char	Vectorcall[]			= "/Gv";

	// Compiler
	static constexpr char	Cpp14[]					= "/std:c++14";
	static constexpr char	Cpp17[]					= "/std:c++17";
	static constexpr char	CppLastest[]			= "/std:c++latest";

	// Debug
	static constexpr char	DbgProgramDatabase[]	= "/Zi";
	static constexpr char	DbgEditAndContinue[]	= "/ZI";
	static constexpr char	DbgC7Compatible[]		= "/Z7";
	static constexpr char	FunctionLevelLinking[]	= "/Gy";	// required for edit and continue
	static constexpr char	NoFunctionLevelLinking[]= "/Gy-";

	// DLL
	static constexpr char	MultiThreadedLib[]		= "/MT";
	static constexpr char	MultiThreadedDebugLib[]	= "/MTd";
	static constexpr char	MultiThreadedDll[]		= "/MD";
	static constexpr char	MultiThreadedDebugDll[]	= "/MDd";

	// SIMD
	static constexpr char	SSE[]					= "/arch:SSE";
	static constexpr char	SSE2[]					= "/arch:SSE2";
	static constexpr char	AVX[]					= "/arch:AVX";
	static constexpr char	AVX2[]					= "/arch:AVX2";
	static constexpr char	NoSIMD[]				= "/arch:IA32";

	// Float point model
	static constexpr char	FloatPointPrecise[]		= "/fp:precise";
	static constexpr char	FloatPointStrict[]		= "/fp:strict";
	static constexpr char	FloatPointFast[]		= "/fp:fast";
	static constexpr char	FloatPointExceptions[]	= "/fp:except";
	static constexpr char	FloatPointNoExceptions[]= "/fp:except-";


}	// VS


// linker options
namespace VS_Linker
{
	static constexpr char	WarningsAsErrors[]		= "/WX";
	static constexpr char	Warnings[]				= "/WX:NO";

	static constexpr char	Debug[]					= "/DEBUG";
	static constexpr char	DebugFull[]				= "/DEBUG:FULL";
	static constexpr char	DebugFastlink[]			= "/DEBUG:FASTLINK";

	static constexpr char	LinkTimeCodeGen[]		= "/LTCG";
	static constexpr char	IncrementalCodeGen[]	= "/LTCG:incremental";
	static constexpr char	IncrementalLinking[]	= "/INCREMENTAL";
	static constexpr char	NoIncrementalLinking[]	= "/INCREMENTAL:NO";

	static constexpr char	OptReferences[]			= "/OPT:REF";		// to minimize pdb file size
	static constexpr char	COMDATFolding[]			= "/OPT:ICF";		// to minimize pdb file size

	static constexpr char	RandomBaseAddress[]		= "/DYNAMICBASE";
	static constexpr char	Release[]				= "/RELEASE";
	static constexpr char	Profile[]				= "/PROFILE";		// require /DEBUG

	inline String Version (StringCRef number)
	{
		return "/VERSION:"_str << number;
	}


}	// VS_Linker

}	// CMake
