// Copyright © 2014-2017  Zhirnov Andrey. All rights reserved.

#pragma once


#ifndef GX_EXPORT
# ifdef __GX_BUILD_LIB__
#	define GX_EXPORT
# else
#	ifdef __GX_BUILD_DLL__
#		define GX_EXPORT				GX_DLL_EXPORT
#	else
#		define GX_EXPORT				GX_DLL_IMPORT
#	endif
# endif	// __GX_BUILD_LIB__
#endif	// GX_EXPORT


#if not defined( CALLBACK )
#	ifdef PLATFORM_WINDOWS
#		define CALLBACK					__stdcall
#	else
#		define CALLBACK
#	endif
#endif


#if not defined( ARGS )
#	define ARGS( _arg_ )				_args_
#endif

#if not defined( __GX_EMPTY_ARGS )
#	define __GX_EMPTY_ARGS( _args_ )	{}
#endif


#if defined( __GX_DEBUG__ )
#	define DEBUG_ONLY					ARGS
#	define RELEASE_ONLY					__GX_EMPTY_ARGS
#else
#	define DEBUG_ONLY					__GX_EMPTY_ARGS
#	define RELEASE_ONLY					ARGS
#endif	// __GX_DEBUG__


#if defined( PLATFORM_TYPE_MOBILE )
#	define MOBILE_ONLY					ARGS
#	define DESCTOP_ONLY					__GX_EMPTY_ARGS
#else
#	define MOBILE_ONLY					__GX_EMPTY_ARGS
#	define DESCTOP_ONLY					ARGS
#endif	// PLATFORM_TYPE_MOBILE


// Force Inline //
#if defined(__GX_DEBUG__)
#	define forceinline					inline
#else
#	define forceinline					GX_FORCE_INLINE
#endif


// Assume //
#if defined(__GX_FAST__)
#	define ASSUME( _condition_ )		GX_ASSUME( _condition_ )
#else
#	define ASSUME( _condition_ )		ASSERT( _condition_ )
#endif


// Debug Console //
#if not defined(__GX_FAST__)
#	define DEBUG_CONSOLE( _msg_ )		__debug_console_write( (_msg_), __FILE__, __LINE__ )
#else
#	define DEBUG_CONSOLE( _msg_ )		{}
#endif


// Static Assert //
#define STATIC_ASSERT( ... ) \
	static_assert(	AUXDEF_GETRAW( AUXDEF_GETARG_0( __VA_ARGS__ ) ), \
					AUXDEF_GETRAW( AUXDEF_GETARG_1( __VA_ARGS__, TOSTRING(__VA_ARGS__) ) ) )

#define WARNING( _message_ )			ASSERT( false, _message_ )
#define STATIC_WARNING( _message_ )		STATIC_ASSERT( false, _message_ )
#define CONSOLE_OUTPUT( ... )			::printf( __VA_ARGS__ )

// for deferred programming
#define TODO( _message_ )				ASSERT( false, _message_ )


// temporary code //
#if defined( GX_TEMP_CODE )
#	define TEMP_CODE_MARKER()			{} // nothing
#	define TEMP_CODE					ARGS
#else
#	define TEMP_CODE_MARKER()			STATIC_WARNING( "temporary code!!!" )
#	define TEMP_CODE					__GX_EMPTY_ARGS
#endif



// Other //

// convert to string
#define GX_TO_ANSI_STRING(...)			#__VA_ARGS__
#define GX_TO_WIDE_STRING(...)			L#__VA_ARGS__
#undef  GX_TEXT

#ifdef GX_UNICODE
#	define GX_IS_UNICODE				1
#	define TOSTRING						GX_TO_WIDE_STRING
#	define GX_TEXT(...)					L__VA_ARGS__
#else
#	define GX_IS_UNICODE				0
#	define TOSTRING						GX_TO_ANSI_STRING
#	define GX_TEXT(...)					__VA_ARGS__
#endif


// Real type size string
#ifdef GX_REAL_TYPE_SIZE
#	if GX_REAL_TYPE_SIZE == 32
#		define GX_REAL_TYPE_SIZE_STR	"32"
#	endif
#	if GX_REAL_TYPE_SIZE == 64
#		define GX_REAL_TYPE_SIZE_STR	"64"
#	endif
#	ifndef GX_REAL_TYPE_SIZE_STR
#		error GX_REAL_TYPE_SIZE_STR must be defined!
#	endif
#endif


// Short file name
#define GX_SHORT_FILENAME				(::GX_STL::GXTypes::FileAddress::ToShortPath( __FILE__, 2 ))


// helpers for function parameters
#define OUT			// output argument
#define INOUT		// input and output argument (not recomended)


// convert to right value reference
#define RVREF(...)			::GX_STL::TypeTraits::ToRValueRef( __VA_ARGS__ )

#define FW					::GX_STL::TypeTraits::Forward


// to disable 'unreferenced formal parameter' compiler error
#define GX_UNUSED(...)		(void)( __VA_ARGS__ )


// lambda
// code style: LAMBDA(=) (args) {{ code }};
#ifdef GX_LAMBDA_SUPPORTED
#	define LAMBDA(...)		[ __VA_ARGS__ ]
#else
#	define LAMBDA(...)		STATIC_WARNING("lambda not supported!!!")
#endif


// default constructor arguments with Uninitialized
#define GX_DEFCTOR			::GX_STL::GXTypes::UninitializedType = ::GX_STL::GXTypes::Uninitialized


// for each
#define FOR( _counter_, _container_ ) \
	for (usize _counter_ = 0; _counter_ < (_container_).Count(); ++_counter_)

#define FORv( _counter_, _value_, _container_ ) \
	for (usize _counter_ = (_value_); _counter_ < (_container_).Count(); ++_counter_)

// TODO: deprecated?
//#define FORc( _counter_, _container_ ) \
//	for (; _counter_ < (_container_).Count(); ++_counter_)

#define FOR_rev( _counter_, _container_ ) \
	for (usize _counter_ = (_container_).Count()-1; _counter_ < (_container_).Count(); --_counter_)

// TODO: deprecated?
//#define FORv_rev( _counter_, _value_, _container_ ) \
//	for (usize _counter_ = (_value_); _counter_ < (_container_).Count(); --_counter_)

// TODO: deprecated?
//#define FORc_rev( _counter_, _container_ ) \
//	for (; _counter_ < (_container_).Count(); --_counter_)



// range for
#define FOR_range( _counter_, _begin_, _end_ ) \
	for (usize _counter_ = _begin_; _counter_ < _end_; ++_counter_)

// TODO: deprecated?
//#define FOR_range_rev( _counter_, _begin_, _end_ ) \
//	for (usize _counter_ = _end_-1; (_counter_ >= _begin_ and _counter_ < _end_); --_counter_)

// TODO: deprecated?
//#define FOR_range_t( _type_, _counter_, _begin_, _end_ ) \
//	for (_type_ _counter_ = _begin_; _counter_ < _end_; ++_counter_) 


