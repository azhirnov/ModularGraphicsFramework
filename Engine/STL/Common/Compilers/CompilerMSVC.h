// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'
//
// see http://en.cppreference.com/w/cpp/compiler_support
//

#pragma once

#ifdef  COMPILER_MSVC


// Version //

#define COMPILER_VERSION		_MSC_VER

/*
	MSVC++ 15.0 _MSC_VER == 1910 (Visual Studio 2017)
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

// branch prediction optimization (not supported in VS)
#define GX_BRANCH_EXPECT( _expr_ )			(_expr_)
#define GX_BRANCH_EXPECT_FALSE( _expr_ )	(_expr_)

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
#if COMPILER_VERSION >= 2000	// TODO
#	define GX_DEPRECATED( _reason_ )		[[ deprecated(_reason_) ]]
#else
#	define GX_DEPRECATED( _reason_ )
#endif


// literal operator
#if COMPILER_VERSION >= 1900
#	define GX_LITERAL_OPERATOR_SUPPORTED	1
#endif


// notify compiler to generate error if function result unused
#if COMPILER_VERSION >= 2000	// TODO
#	define GX_CHECK_RESULT					[[nodiscard]]
#elif COMPILER_VERSION >= 1700
#	define GX_CHECK_RESULT					_Check_return_	// Compile with '/analize' key
#else
#	define GX_CHECK_RESULT
#endif


// if constexpr
#if COMPILER_VERSION >= 1911
#	define if_constexpr			if constexpr
#else
#	define if_constexpr			if
#endif


// 'auto' keyword in template parameters
#if COMPILER_VERSION >= 2000
#	define GX_AUTO_IN_TEMPLATE_SUPPORTED	1
#endif


// for many other features
#if COMPILER_VERSION >= 1900	// TODO: check version
#	define GX_CPP11_SUPPORTED		1
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