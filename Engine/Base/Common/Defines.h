// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once


#define CHECK_LINKING( _expr_ ) \
	{if ( not (_expr_) ) { \
		__GX_CHECK_WARN( (_expr_), GX_TO_ANSI_STRING( _expr_ ), ::GX_STL::ELog::_DefWarning ); \
		CHECK( _SetState( EState::LinkingFailed ) ); \
		return true; \
	}}

#define CHECK_ATTACHMENT( _expr_ ) \
	{if ( not (_expr_) ) { \
		__GX_CHECK_WARN( (_expr_), GX_TO_ANSI_STRING( _expr_ ), ::GX_STL::ELog::_DefWarning ); \
		CHECK( _SetState( EState::IncompleteAttachment ) ); \
		return true; \
	}}

#define CHECK_COMPOSING( _expr_ ) \
	{if ( not (_expr_) ) { \
		__GX_CHECK_WARN( (_expr_), GX_TO_ANSI_STRING( _expr_ ), ::GX_STL::ELog::_DefWarning ); \
		CHECK( _SetState( EState::ComposingFailed ) ); \
		return true; \
	}}


#if defined(GX_ENABLE_DEBUGGING) or defined(GX_ENABLE_PROFILING)
#	define GX_PROFILE_MSG		ARGS
#else
#	define GX_PROFILE_MSG		__GX_EMPTY_ARGS
#endif
