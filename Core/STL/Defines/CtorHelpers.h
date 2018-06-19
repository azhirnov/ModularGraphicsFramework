// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once


// Default copy constructors and default assign operators //
#define GX_DEFCOPYCTOR_ASSIGN( _type_ ) \
	_type_ (const _type_ &) = default; \
	_type_ (_type_ &&) = default;\
	\
	_type_& operator = (const _type_ &) = default; \
	_type_& operator = (_type_ &&) = default;



// Default copy constructor only //
#define GX_DEFCOPYCTOR_ONLY( _type_ ) \
	_type_ () = delete; \
	\
	_type_ (const _type_ &) = default; \
	_type_ (_type_ &&) = default;\
	\
	_type_& operator = (const _type_ &) = delete; \
	_type_& operator = (_type_ &&) = delete;
