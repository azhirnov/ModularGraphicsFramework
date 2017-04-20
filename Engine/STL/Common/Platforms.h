// Copyright © 2014-2017  Zhirnov Andrey. All rights reserved.
/*
	from http://sourceforge.net/p/predef/wiki/Architectures/
*/

#pragma once

//------------------------------- OS ------------------------------//

// Android platform //
#if !defined( PLATFORM_NAME ) && \
	( defined( ANDROID ) || defined( __ANDROID__ ) )
#	define PLATFORM_ANDROID			1
#	define PLATFORM_NAME			"Android"
#	define PLATFORM_TYPE_MOBILE		1
#	define PLATFORM_BASE_POSIX		1
#	define PLATFORM_LITTLE_ENDIAN	1
#endif


// Windows CE platform //
#if !defined( PLATFORM_NAME ) && \
	defined( _WIN32_WCE )
#	define PLATFORM_WINDOWS_CE		1
#	define PLATFORM_NAME			"Windows CE"
#	define PLATFORM_TYPE_MOBILE		1
#	define PLATFORM_BASE_WINDOWS	1
#endif


// Windows platform //
#if !defined( PLATFORM_NAME ) && \
	( defined( _WIN32 ) || defined( _WIN64 ) || defined( WIN32 ) || defined( WIN64 ) || \
	  defined( __CYGWIN__ ) || defined( __MINGW32__ ) || defined( __MINGW32__ ) )
#	define PLATFORM_WINDOWS			1
#	define PLATFORM_NAME			"Windows"
#	define PLATFORM_TYPE_DESCTOP	1
#	define PLATFORM_BASE_WINDOWS	1
#	define PLATFORM_LITTLE_ENDIAN	1
#endif


// OSX platform //
#if !defined( PLATFORM_NAME ) && \
	( defined( __APPLE__ ) || defined( MACOSX ) )
# include "AvailabilityMacros.h"
# include "TargetConditionals.h"
# if defined( TARGET_OS_IPHONE )
#	define PLATFORM_IPHONE			1
#	define PLATFORM_NAME			"iPhone OS"
#	define PLATFORM_TYPE_MOBILE		1
# else
#	define PLATFORM_MACOSX			1
#	define PLATFORM_NAME			"Mac OS X"
#	define PLATFORM_TYPE_DESCTOP	1
# endif
#	define PLATFORM_BASE_POSIX		1
#	define PLATFORM_LITTLE_ENDIAN	1
#endif


// Linux platform //
#if !defined( PLATFORM_NAME ) && \
	( defined( __linux__ ) || defined( __gnu_linux__ ) )
#	define PLATFORM_LINUX			1
#	define PLATFORM_NAME			"Linux"
#	define PLATFORM_TYPE_DESCTOP	1
#	define PLATFORM_BASE_POSIX		1
#	define PLATFORM_LITTLE_ENDIAN	1
#endif


// Free BSD platform //
#if !defined( PLATFORM_NAME ) && \
	( defined(__FreeBSD__) || defined(__FreeBSD_kernel__) || defined(__DragonFly__) )
#	define PLATFORM_FREEBSD			1
#	define PLATFORM_NAME			"FreeBSD"
#	define PLATFORM_TYPE_DESCTOP	1
#	define PLATFORM_BASE_POSIX		1	// ???
#endif



//------------------------- Architecture --------------------------//

#if defined( __arm__ ) || defined( __thumb__ ) || defined( _M_ARM ) || defined( _M_ARMT )
#	define PLATFORM_CPU_ARM			1
#	define PLATFORM_CPU_NAME		"ARM"
#	define PLATFORM_BITS			32
#	define PLATFORM_BITS_NAME		"32"
#endif

#if defined( __aarch64__ )
#	define PLATFORM_CPU_ARM64		1
#	define PLATFORM_CPU_NAME		"ARM 64"
#	define PLATFORM_BITS			64
#	define PLATFORM_BITS_NAME		"64"
#endif

#if defined( __amd64__ ) || defined( __x86_64__ ) || defined( _M_X64 ) || defined( _M_AMD64 )
#	define PLATFORM_CPU_X64			1
#	define PLATFORM_CPU_AMD64		1
#	define PLATFORM_CPU_NAME		"AMD 64"
#	define PLATFORM_BITS			64
#	define PLATFORM_BITS_NAME		"64"
#endif

#if defined( __i386__ ) || defined( __i486__ ) || defined( __i586__ ) || defined( __i686__ ) || \
	defined( _M_IX86 ) || defined( _X86_ )
#	define PLATFORM_CPU_X86			1
#	define PLATFORM_CPU_INTELX86	1
#	define PLATFORM_CPU_NAME		"Intel x86"
#	define PLATFORM_BITS			32
#	define PLATFORM_BITS_NAME		"32"
#endif

#if defined( __ia64__ ) || defined( __IA64__ ) || defined( _M_IA64 ) || defined( __itanium__ )
#	define PLATFORM_CPU_X64			1
#	define PLATFORM_CPU_IA64		1
#	define PLATFORM_CPU_NAME		"Intel Itanium x64"
#	define PLATFORM_BITS			64
#	define PLATFORM_BITS_NAME		"64"
#endif

#if defined( __mips__ )
#	define PLATFORM_CPU_MIPS		1
#	define PLATFORM_CPU_NAME		"MIPS"
#	define PLATFORM_BITS			32
#	define PLATFORM_BITS_NAME		"32"
#endif



//---------------------------- Checks -----------------------------//

// Unknown
#ifndef PLATFORM_NAME
#	error Unknown platform
#endif

#ifndef PLATFORM_BITS_NAME
#	error Unknown platform bits
#endif

#if !defined( PLATFORM_TYPE_DESCTOP ) && !defined( PLATFORM_TYPE_MOBILE )
#	error Unknown platform type
#endif

#ifndef PLATFORM_CPU_NAME
#	error Unknown cpu architecture
#endif

#ifndef PLATFORM_LITTLE_ENDIAN
#	error Endianes not defined
#endif



//-------------------------- Compiler -----------------------------//

// Microsoft Visual C++
#ifdef _MSC_VER
#	define COMPILER_MSVC		1
#	define COMPILER_NAME		"MSVC"
#	include "Compilers/CompilerMSVC.h"
#endif


// GNU Compiler Collection
#ifdef __GNUC__
#	define COMPILER_GCC			1
#	define COMPILER_NAME		"GCC"
#	include "Compilers/CompilerGCC.h"
#endif


// TODO: support compilers

// Intel C Compiler
#ifdef __INTEL_COMPILER
#	define COMPILER_INTEL		1
#	define COMPILER_NAME		"ICC"
#endif


// Borland
#ifdef __BORLANDC__
#	define COMPILER_BORLAND		1
#	define COMPILER_NAME		"Borland"
#endif


// Clang
#ifdef __clang__
#	define COMPILER_CLANG		1
#	define COMPILER_NAME		"Clang"
#endif


// Unknown
#ifndef COMPILER_NAME
#	error Unknown compiler
#endif


//------------------------- Language ------------------------------//

#ifndef __cplusplus
#	error Only C++ language are supported
#endif 

