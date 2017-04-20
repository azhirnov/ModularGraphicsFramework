// Copyright © 2014-2017  Zhirnov Andrey. All rights reserved.
//
// see http://en.cppreference.com/w/cpp/compiler_support
//

#pragma once

#ifdef  COMPILER_MSVC


// Version //

#define COMPILER_VERSION		_MSC_VER

/*
	MSVC++ 15.0 _MSC_VER == 2000 (Visual Studio 2017)
	MSVC++ 14.0 _MSC_VER == 1900 (Visual Studio 2015)
	MSVC++ 12.0 _MSC_VER == 1800 (Visual Studio 2013)
	MSVC++ 11.0 _MSC_VER == 1700 (Visual Studio 2012)
	MSVC++ 10.0 _MSC_VER == 1600 (Visual Studio 2010)
	MSVC++ 9.0  _MSC_VER == 1500 (Visual Studio 2008)
	MSVC++ 8.0  _MSC_VER == 1400 (Visual Studio 2005)
	MSVC++ 7.1  _MSC_VER == 1310 (Visual Studio 2003)
	MSVC++ 7.0  _MSC_VER == 1300
	MSVC++ 6.0  _MSC_VER == 1200
	MSVC++ 5.0  _MSC_VER == 1100
*/

//-------------------------------------------------------------------



// Export //

#define GX_DLL_EXPORT			__declspec( dllexport )
#define GX_DLL_IMPORT			__declspec( dllimport )
#define GX_DLL_PRIVATE			


//-------------------------------------------------------------------


// Warnings //


// disable warnings for levels 1-3
#pragma warning (disable: 4996)			// this function or variable may be unsafe
#pragma warning (disable: 4503)			// decorated name length exceeded, name was truncated (for debug info???)
#pragma warning (disable: 4838)			// conversion from int to uint
//#pragma warning (disable: 4251)		// class '%class_name%' must have dll-interface...


// disable warnings for level 4
#pragma warning (disable: 4201)			// nonstandard extension used : nameless struct/union
#pragma warning (disable: 4459)			// declaration of '...' hides global declaration
#pragma warning (disable: 4127)			// conditional expression is constant
#pragma warning (disable: 4458)			// declaration of '...' hides class member
//#pragma warning (disable: 4481)		// nonstandard extension used
//#pragma warning (disable: 4127)		// conditional expression is constant
//#pragma warning (disable: 4512)		// assignment operator could not be generated
//#pragma warning (disable: 4480)		// nonstandard extension used: specifying underlying type for enum
//#pragma warning (disable: 4189)		// 'variable': local variable is initialized but not referenced


// warning to errors
#pragma warning (error: 4018)			// signed/unsigned mismatch
#pragma warning (error: 4287)			// unsigned/negative constant mismatch
#pragma warning (error: 4715)			// 'function' : not all control paths return a value
#pragma warning (error: 4172)			// returning address of local variable or temporary
#pragma warning (error: 4099)			// 'identifier' : type name first seen using 'objecttype1' now seen using 'objecttype2'
#pragma warning (error: 4263)			// 'function' : member function does not override any base class virtual member function
#pragma warning (error: 4264)			// 'virtual_function' : no override available for virtual member function from base 'class'; function is hidden
#pragma warning (error: 4266)			// 'function' : no override available for virtual member function from base 'type'; function is hidden
#pragma warning (error: 4552)			// 'operator' : operator has no effect; expected operator with side-effect
#pragma warning (error: 4553)			// 'operator' : operator has no effect; did you intend 'operator'?
#pragma warning (error: 4002)			// too many actual parameters for macro
#pragma warning (error: 4244)			// 'initializing': conversion from '...' to '...', possible loss of data
#pragma warning (error: 4239)			// nonstandard extension used: 'argument': conversion from 'type' to 'type &'
#pragma warning (error: 4717)			// 'function' : recursive on all control paths, function will cause runtime stack overflow
#pragma warning (error: 4456)			// declaration of 'identifier' hides previous local declaration
#pragma warning (error: 4129)			// 'character': unrecognized character escape sequence
#pragma warning (error: 4702)			// unreachable code


// to check some errors
#if 1
#	pragma warning (disable: 4100)		// unreferenced formal parameter
#	pragma warning (disable: 4310)		// cast truncates constant value
#	pragma warning (disable: 4245)		// 'return': conversion from signed to unsigned
#	pragma warning (disable: 4365)		// signed/unsigned mismatch
#	pragma warning (disable: 4389)		// '==': signed/unsigned mismatch
#	pragma warning (disable: 4505)		// unreferenced local function has been removed
#else
#	pragma warning (error: 4100)		// unreferenced formal parameter
#	pragma warning (error: 4310)		// cast truncates constant value
#	pragma warning (error: 4245)		// 'return': conversion from signed to unsigned
#	pragma warning (error: 4365)		// signed/unsigned mismatch
#	pragma warning (error: 4389)		// '==': signed/unsigned mismatch
#	pragma warning (error: 4505)		// unreferenced local function has been removed

	// not all enums listed in switch block
#	pragma warning (error: 4061)
#	pragma warning (error: 4062)
#	pragma warning (error: 4063)		// case 'number' is not a valid value for switch of enum 'type'
#endif


// disable warning on exceptions
#ifdef __GX_NO_EXCEPTIONS__
#	pragma warning (disable: 4530)		// C++ exception handler used, but unwind semantics are not enabled.
#	pragma warning (disable: 4577)		// 'noexcept' used with no exception handling mode specified; termination on exception is not guaranteed.
#endif	// __GX_NO_EXCEPTIONS__


#undef  _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS

#undef  _ALLOW_KEYWORD_MACROS
#define _ALLOW_KEYWORD_MACROS

//-------------------------------------------------------------------



// Compiler Settings //

// function name as string
#define	GX_FUNCTION_NAME			__FUNCTION__

// always inline function
#define	GX_FORCE_INLINE				__forceinline

// never inline function
#define GX_NO_INLINE				__declspec( noinline )

// no code be generated for the arguments
// Warning: no analogs for other compilers!
#define	GX_NOOP						__noop

// value is always true
#define	GX_ASSUME( ... )			__assume( __VA_ARGS__ )

// don't use virtual table for struct/class
// Warning: no analogs for other compilers!
#define GX_NO_VTABLE				__declspec( novtable )

// function return unique pointer
#define	GX_RESTRICT					__declspec( restrict )

// function does not modify or reference visible global state and only modifies the memory
// pointed to directly by pointer parameters (first-level indirections). (MDSN)
// Warning: no analogs for other compilers!
#define	GX_NO_ALIAS					__declspec( noalias )

// type qualifier for pointer types
// another pointer cannot be used to access the data pointed to by the __restrict pointer. (MDSN)
// Warning: no analogs for other compilers!
#define	GX_RESTRICT_PTR				__restrict

// function not returns any value
// compilation warning for this function never generated
#define GX_NO_RETURN				__declspec( noreturn )

//
#define __CDECL						__cdecl

// null pointer
#if COMPILER_VERSION > 1500
#	define null						nullptr
#endif

// pragma directive inside the macro
#define GX_PRAGMA( ... )			__pragma( __VA_ARGS__ )

//-------------------------------------------------------------------



// C++ 11/14 features //

// C++11, constexpr
#if COMPILER_VERSION > 1800
#	define GX_CONSTEXPR_SUPPORTED			1
#endif

// final, constexpr, noexcept
#if COMPILER_VERSION <= 1800
#	define final
#	define constexpr
#	define noexcept							throw()
#endif


// alignas
#if COMPILER_VERSION < 1900
#	define	alignas( ... )					__declspec( align( __VA_ARGS__ ) )
#endif


// alignof
#if COMPILER_VERSION < 1900
#	define alignof( ... )					__alignof( __VA_ARGS__ )
#endif


// override, decltype
#if COMPILER_VERSION <= 1500
#	define override
#	define decltype( _value_ )				STATIC_WARNING( "decltype not supported" )
#endif


// declare variable is local variable for thread
// in WinXP it is for static linking only!
#if COMPILER_VERSION < 1900
#	define thread_local						__declspec( thread )
#endif


// variable arguments in macro
#if COMPILER_VERSION > 1500
#	define GX_VA_ARGS_IN_MACRO_SUPPORTED	1
#	define ARGS(...)						__VA_ARGS__
#	define __GX_EMPTY_ARGS(...)		
#endif


// right value reference
#if COMPILER_VERSION >= 1600
#	define GX_RVALUE_REFERENCE_SUPPORTED	1
#endif


// auto
#if COMPILER_VERSION >= 1600
#	define GX_AUTO_SUPPERTED				1
#endif


// thread safe local static variable
#if COMPILER_VERSION >= 1600
#	define GX_THREADSAFE_STATIC_SUPPORTED	1
#endif


// extern template
#if COMPILER_VERSION >= 1500
#	define GX_EXTERN_TEMPLATE_SUPPORTED		1
#endif


// lambda
#if COMPILER_VERSION >= 1700
#	define GX_LAMBDA_SUPPORTED				1
#endif


// Unrestricted unions 
#if COMPILER_VERSION >= 1900
#	define GX_UNRESTRICTED_UNIONS_SUPPORTED	1
#endif


// Variadic templates 
#if COMPILER_VERSION >= 1800
#	define GX_VARIADIC_TEMPLATES_SUPPORTED	1
#endif


// Non-static data member initializers 
#if COMPILER_VERSION >= 1800
#	define GX_FIELD_INITIALIZERS_SUPPORTED	1
#endif


// deprecated attribute
#if COMPILER_VERSION >= 2000
#	define GX_DEPRECATED( _reason_ )		[[ deprecated(_reason_) ]]
#else
#	define GX_DEPRECATED( _reason_ )
#endif


// literal operator
#if COMPILER_VERSION >= 1900
#	define GX_LITERAL_OPERATOR_SUPPORTED	1
#endif


// notify compiler to generate error if function result unused
#if COMPILER_VERSION >= 1700
#	define GX_CHECK_RESULT					_Check_return_	// Compile with '/analize' key
#endif

//-------------------------------------------------------------------



// Other //

//#define GX_BREAK_POINT()		__asm { int 3 }
#define GX_BREAK_POINT()		__debugbreak()


#ifndef and
#	define and		&&
#endif

#ifndef or
#	define or		||
#endif

#ifndef not
#	define not		!
#endif


// Intrin for MSVC and Windows Platform //
#include <intrin.h>

#endif	// COMPILER_MSVC