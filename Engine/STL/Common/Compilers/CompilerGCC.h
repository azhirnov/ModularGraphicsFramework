// Copyright © 2014-2017  Zhirnov Andrey. All rights reserved.
//
// see http://en.cppreference.com/w/cpp/compiler_support
//

#pragma once

#ifdef  COMPILER_GCC


// Version //

#define COMPILER_VERSION		(((__GNUC__)*100) + (__GNUC_MINOR__*10) + (__GNUC_PATCHLEVEL__))

//-------------------------------------------------------------------



// Export //

#if defined( PLATFORM_WINDOWS )
#	define GX_DLL_EXPORT			__attribute__ (dllexport)
#	define GX_DLL_IMPORT			__attribute__ (dllimport)
#	define GX_DLL_PRIVATE			
#else
#	define GX_DLL_EXPORT			__attribute__ (visibility("default"))
#	define GX_DLL_IMPORT			__attribute__ (visibility("default"))
#	define GX_DLL_PRIVATE			__attribute__ (visibility("hidden"))
#endif

//-------------------------------------------------------------------



// Warning //

// not all enums listed in switch block
//#pragma GCC diagnostic error "-Wswitch"

//-------------------------------------------------------------------



// Compiler Settings //

// function name as string
#if defined( PLATFORM_ANDROID )
#	define GX_FUNCTION_NAME			"unknown function"
#else
#	define GX_FUNCTION_NAME			__func__
#endif

// always inline function
#define	GX_FORCE_INLINE				__inline__ __attribute__((always_inline))

// never inline function
#define GX_NO_INLINE				__attribute__ ((noinline))

// 
#define	GX_NOOP						// TODO

// value is always true
#define	GX_ASSUME( _condition_ )	{ if ( !(_condition_) ) __builtin_unreachable(); }

// 
#define GX_NO_VTABLE				// TODO

// function return unique pointer
#if defined( PLATFORM_ANDROID )
#	define	GX_RESTRICT
#else
#	define	GX_RESTRICT				__restrict__
#endif

// 
#define	GX_NO_ALIAS					// TODO

// 
#define	GX_RESTRICT_PTR				// TODO

// null pointer
#if COMPILER_VERSION >= 460
#	define null						nullptr
#else
#	define null						__null
#endif

// function not returns any value
// compilation warning for this function never generated
#define GX_NO_RETURN				__attribute__ ((noreturn))

// 
#define __CDECL						// nothing

// pragma directive inside the macro
#define GX_PRAGMA( ... )			_Pragma( TOASTRING( __VA_ARGS__ ) )

//-------------------------------------------------------------------



// C++ 11 features //

// constexpr
#if COMPILER_VERSION >= 460
#	define GX_CONSTEXPR_SUPPORTED			1
#endif


// final, override, constexpr, decltype
#if COMPILER_VERSION < 470
#	define final
#	define override
#	define constexpr
#	define decltype							__typeof__
#	define noexcept							throw()
#endif


// set align of struct in bytes
#if COMPILER_VERSION < 480
#	define	alignas( ... )					__attribute__( packed, aligned( __VA_ARGS__ ) )
#endif


// returns align of type in bytes
#if COMPILER_VERSION < 450
#	define alignof( ... )					__alignof( __VA_ARGS__ )	// or __alignof__
#endif


// thread_local
#if COMPILER_VERSION < 480
#	define thread_local						__thread
#endif


// variable arguments in macro
#if COMPILER_VERSION >= 430
#	define GX_VA_ARGS_IN_MACRO_SUPPORTED	1
#	define ARGS(...)						__VA_ARGS__
#	define __GX_EMPTY_ARGS(...)		
#endif


// right value reference
#if COMPILER_VERSION >= 460
#	define GX_RVALUE_REFERENCE_SUPPORTED	1
#endif


// auto
#if COMPILER_VERSION >= 450
#	define GX_AUTO_SUPPERTED				1
#endif


// thread safe local static variable
#if COMPILER_VERSION >= 460
#	define GX_THREADSAFE_STATIC_SUPPORTED	1
#endif


// extern template
#if COMPILER_VERSION >= 330
#	define GX_EXTERN_TEMPLATE_SUPPORTED		1
#endif


// lambda
#if COMPILER_VERSION >= 450
#	define GX_LAMBDA_SUPPORTED				1
#endif


// Unrestricted unions 
#if COMPILER_VERSION >= 460
#	define GX_UNRESTRICTED_UNIONS_SUPPORTED	1
#endif


// Variadic templates 
#if COMPILER_VERSION >= 440
#	define GX_VARIADIC_TEMPLATES_SUPPORTED	1
#endif


// Non-static data member initializers 
#if COMPILER_VERSION >= 470
#	define GX_FIELD_INITIALIZERS_SUPPORTED	1
#endif


// literal operator
#if COMPILER_VERSION >= 500		// TODO: check version
#	define	GX_LITERAL_OPERATOR_SUPPORTED	1
#endif


// notify compiler to generate error if function result unused
#if COMPILER_VERSION >= 400
#	define GX_CHECK_RESULT					__attribute__ ((warn_unused_result))
#endif

//-------------------------------------------------------------------



// Other //

#if defined( PLATFORM_ANDROID )
#	define GX_BREAK_POINT()		{}
#else
#	define GX_BREAK_POINT()		__asm__("int3")		// or asm("int $3");
#endif


#endif	// COMPILER_GCC