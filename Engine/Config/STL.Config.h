// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

// Define this in project preprocessor only //
//#define __GX_DEBUG__
//#define __GX_FAST__
//#define __GX_NO_EXCEPTIONS__
#define __GX_BUILD_LIB__
//-------------------------------------------------------------------


// Checks //
#if defined(__GX_DEBUG__) && defined(__GX_FAST__)
#	error Incorrect project configuration!
#endif

//-------------------------------------------------------------------


// Platforms //

// use SDL
//#define PLATFORM_SDL

// use STD
//#define GX_USE_STD

//-------------------------------------------------------------------



// Options //

// enable type identifier based on builtin 'typeid'
#define GX_USE_STD_TYPEID


// compile time calculation of types limits, otherwise used constant values.
//#define GX_MATH_REAL_LIMITS


// use single or double precision float-point types.
#define GX_REAL_TYPE_SIZE	32


// all string must be in unicode.
// (TODO)
//#define GX_UNICODE


// enable temporary code, otherwise enabled static asserts instead of temporary code.
//#define GX_TEMP_CODE


// runtime error checks
#if !defined( __GX_FAST__ )
#	define __GX_ERROR_CHECKS__
#endif


// runtime error checks for OS functions
#if !defined( __GX_FAST__ )
#	define __GX_OS_ERROR_CHECKS__
#endif


// log show warning window in runtime
#if defined( __GX_DEBUG__ )
#	define __GX_LOG_SHOW_WARNING_ENABLED__
#endif


// log to file/console enabled
#if !defined( __GX_FAST__ )
#	define __GX_LOG_WRITE_ENABLED__
#endif


// files
//#define GX_LZMA_ENABLED
//#define GX_ZLIB_ENABLED


// scripts
//#define GX_ANGELSCRIPT_ENABLED


// database
//#define GX_SQLITE_DATABASE_ENABLED


// physics types (this types slow down compilation)
#define GX_PHYSICS_DIMENSIONS_ENABLED


// Java
#if defined( PLATFORM_ANDROID )
#	define GX_JNI_ENABLED
#endif

//-------------------------------------------------------------------



// Constants //

namespace GX_STL
{
namespace GlobalConst
{
	// size of static buffer for delegate
	static const int	STL_DelegateBufferSize				= sizeof(void*) * 6;
	
	// string //
	static const int	STL_StringDefaultRadix				= 10;

	// memory container //
	// newSize = (requiered_size * nominator) / denominator + minSize
	static const int	STL_MemContainerResizingMinSize		= 16;
	static const int	STL_MemContainerResizingNominator	= 1;	// must be > 0
	static const int	STL_MemContainerResizingDenominator	= 2;	// must be > 0

	static const int	STL_MemContainerMaxStaticSize		= 1 << 12;	// 2Kb

}	// GlobalConst
}	// GX_STL

//-------------------------------------------------------------------



// Checks //
#ifndef GX_RVALUE_REFERENCE_SUPPORTED
#	error right value reference must be supported!
#endif

#ifndef GX_VA_ARGS_IN_MACRO_SUPPORTED
#	error variable arguments in macro must be supported!
#endif

#ifndef GX_AUTO_SUPPERTED
#	error auto must be supported!
#endif

#ifndef GX_LITERAL_OPERATOR_SUPPORTED
#	error literal operator must be supported!
#endif
