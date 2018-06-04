// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

namespace GX_STL
{

	//
	// Log Message Type
	//

	struct ELog
	{
		enum type : unsigned
		{
			Debug			= 0,
			Info,
			Warning,
			Error,
			Fatal,
			_SrcFile,
			_Count,
			Unknown			= unsigned(-1),

			// for CHECK_* macro
			_DefError		= Error,
			_DefWarning		= Warning,

			// flags
			SpoilerFlag		= 1 << 8,					// spoiler closed by default
			OpenSpoilerFlag	= SpoilerFlag | (1 << 9),	// spoiler opened by default
			_FlagsMask		= 0xF,
		};

		forceinline static bool IsError (type e)
		{
			return (e & _FlagsMask) > Info;
		}

		forceinline friend ELog::type operator | (ELog::type left, ELog::type right)	{ return ELog::type(int(left) | int(right)); }
		forceinline friend ELog::type operator & (ELog::type left, ELog::type right)	{ return ELog::type(int(left) & int(right)); }
	};


	extern int  __write_to_log (const char *msg, ELog::type type, const char *file, int line);
	extern void __debug_console_write (const char *msg, const char *file, int line);

}	// GX_STL


#ifndef __GX_LOG_EXT

#	if !defined( __GX_LOG_WRITE_ENABLED__ )
#		define __GX_LOG_EXT( _text_, _type_, _file_, _line_ )	{}
#	else
#		define __GX_LOG_EXT( _text_, _type_, _file_, _line_ ) \
		{ \
			switch ( ::GX_STL::__write_to_log( (_text_), (_type_), (_file_), (_line_) ) ) \
			{ \
				case /*retry*/-1 :	GX_BREAK_POINT();		break; \
				case /*abort*/-2 :	::exit( EXIT_FAILURE );	break; \
			} \
		}
#	endif	// __GX_FAST__

#endif	// __GX_LOG_EXT


// text, level, file, line
#define LOG( _text_, ... ) \
		__GX_LOG_EXT(	_text_, \
						AUXDEF_GETARG_0( __VA_ARGS__ ), \
						AUXDEF_GETARG_1( __VA_ARGS__, __FILE__ ), \
						AUXDEF_GETARG_2( __VA_ARGS__, __FILE__, __LINE__ ) )
