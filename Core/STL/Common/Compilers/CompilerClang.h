// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'
//
// see http://en.cppreference.com/w/cpp/compiler_support
//

#pragma once

#ifdef COMPILER_CLANG


// Version //

#define COMPILER_VERSION		(((__clang_major__)*100) + (__clang_minor__*10) + (__clang_patchlevel__))

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

// branch prediction optimization
#define GX_BRANCH_EXPECT( _expr_ )			__builtin_expect( !!(_expr_), 1 )
#define GX_BRANCH_EXPECT_FALSE( _expr_ )	__builtin_expect( !!(_expr_), 0 )

// calling conventions
#define GX_CDECL					__attribute__((cdecl))
#define GX_THISCALL					__attribute__((thiscall))
#define GX_STDCALL					__attribute__((stdcall))
//-------------------------------------------------------------------



// C++ 11/14/17 features //

// constexpr
#if __has_feature( cxx_constexpr )
#	define GX_CONSTEXPR_SUPPORTED			1
#endif


// variable arguments in macro
#if 1
#	define GX_VA_ARGS_IN_MACRO_SUPPORTED	1
#	define ARGS(...)						__VA_ARGS__
#	define __GX_EMPTY_ARGS(...)		
#endif


// right value reference
#if __has_feature( cxx_rvalue_references )
#	define GX_RVALUE_REFERENCE_SUPPORTED	1
#endif


// auto
#if __has_feature( cxx_auto_type )
#	define GX_AUTO_SUPPERTED				1
#endif


// thread safe local static variable
#if 1
#	define GX_THREADSAFE_STATIC_SUPPORTED	1
#endif


// extern template
#if 1
#	define GX_EXTERN_TEMPLATE_SUPPORTED		1
#endif


// lambda
#if __has_feature( cxx_lambdas )
#	define GX_LAMBDA_SUPPORTED				1
#endif


// Unrestricted unions 
#if __has_feature( cxx_unrestricted_unions )
#	define GX_UNRESTRICTED_UNIONS_SUPPORTED	1
#endif


// Variadic templates 
#if __has_feature( cxx_variadic_templates )
#	define GX_VARIADIC_TEMPLATES_SUPPORTED	1
#endif


// Non-static data member initializers 
#if __has_feature( cxx_nonstatic_member_init )
#	define GX_FIELD_INITIALIZERS_SUPPORTED	1
#endif


// literal operator
#if __has_feature( cxx_user_literals )
#	define	GX_LITERAL_OPERATOR_SUPPORTED	1
#endif

// deprecated attribute
#if __has_feature( cxx_attributes )
#	define GX_DEPRECATED( _reason_ )		[[ deprecated(_reason_) ]]
#else
#	define GX_DEPRECATED( _reason_ )
#endif


// notify compiler to generate error if function result unused
#if __has_feature( cxx_attributes )
#	define GX_NODISCARD						[[nodiscard]]
#else
#	define GX_NODISCARD
#endif


// if constexpr
#if COMPILER_VERSION >= 390
#	define if_constexpr			if constexpr
#else
#	define if_constexpr			if
#endif


// 'auto' keyword in template parameters
#if 0 //COMPILER_VERSION >= 400
#	define GX_AUTO_IN_TEMPLATE_SUPPORTED	1
#endif


// atomic
#if __has_feature(c_atomic)
#	define GX_ATOMIC_SUPPORTED	1
#endif


// TODO
#if 1 //!defined( PLATFORM_ANDROID )
#	define GX_RTTI_SUPPORTED
#endif


// maybe unused attribute
#if __has_feature( cxx_attributes )
#	define GX_MAYBE_UNUSED			[[maybe_unused]]
#else
#	define GX_MAYBE_UNUSED
#endif

//-------------------------------------------------------------------



// Other //

//#define GX_PRAGMA		_Pragma

#if defined( PLATFORM_ANDROID )
#	define GX_BREAK_POINT()			{}
#else
#	include <csignal>
#	define GX_BREAK_POINT()			std::raise(SIGINT)
//#	define GX_BREAK_POINT()			__asm__("int3")		// or asm("int $3");
#endif


// TODO
// compiler barrier to prevent instruction reordering
//#define GX_COMPILER_BARRIER()


// TODO: check
#define GX_MAX_STRING_LENGTH_BYTES	(5u << 20)


#endif	// COMPILER_CLANG
