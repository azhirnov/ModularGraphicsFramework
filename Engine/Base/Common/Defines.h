// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once


#define CHECK_LINKING( _expr_ ) \
	{if ( not (_expr) ) { \
		CHECK( _SetState( EState::LinkingFailed ) ); \
		return true; \
	}}

#define CHECK_ATTACHMENT( _expr_ ) \
	{if ( not (_expr_) ) { \
		CHECK( _SetState( EState::IncompleteAttachment ) ); \
		return true; \
	}}

#define CHECK_COMPOSING( _expr_ ) \
	{if ( not (_expr_) ) { \
		CHECK( _SetState( EState::ComposingFailed ) ); \
		return true; \
	}}
