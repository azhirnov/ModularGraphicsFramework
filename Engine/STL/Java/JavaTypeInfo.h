// Copyright © 2014-2017  Zhirnov Andrey. All rights reserved.

#pragma once

#include "JavaEnvironment.h"

#ifdef GX_JNI_ENABLED

namespace GX_STL
{
namespace  Java
{
	
	//
	// Java Type Code
	//

	template <typename T>	struct JniTypeName {};

#	define TYPE_CODE( _type_, _code_ ) \
		template <> \
		struct JniTypeName< _type_ >	\
		{ \
			typedef _type_	type; \
			\
			static void Add (INOUT String &s)  { s << _code_; } \
		}

	TYPE_CODE( void,			'V' );
	TYPE_CODE( jboolean,		'Z' );
	TYPE_CODE( jbyte,			'B' );
	TYPE_CODE( jchar,			'C' );
	TYPE_CODE( jshort,			'S' );
	TYPE_CODE( jint,			'I' );
	TYPE_CODE( jlong,			'J' );
	TYPE_CODE( jfloat,			'F' );
	TYPE_CODE( jdouble,			'D' );
	TYPE_CODE( jstring,			"Ljava/lang/String;" );
	TYPE_CODE( jobject,			"Ljava/lang/Object;" );
	TYPE_CODE( jthrowable,		"Ljava/lang/Throwable;" );
		
	TYPE_CODE( jbyteArray,		"[B" );
	TYPE_CODE( jcharArray,		"[C" );
	TYPE_CODE( jshortArray,		"[S" );
	TYPE_CODE( jintArray,		"[I" );
	TYPE_CODE( jlongArray,		"[J" );
	TYPE_CODE( jfloatArray,		"[F" );
	TYPE_CODE( jdoubleArray,	"[D" );

#	undef TYPE_CODE


	template <typename T>
	struct JniTypeName< T *>
	{
		typedef T *		type;

		static void Add (INOUT String &s)  { s << '['; JniTypeName<T>::Add( s ); }
	};


#	define GX_DECL_JAVA_TYPE( _name_, _javaName_ ) \
		\
		namespace GX_STL { \
		namespace Java { \
			struct _name_ { \
				typedef jobject	type; \
				\
				type	obj; \
				\
				_name_ () : obj(null) {} \
				_name_ (const _name_ &other) : obj(other.obj) {} \
				_name_ (type obj) : obj(obj) {} \
				\
				operator type () const	{ return obj; } \
			}; \
			\
			template <> \
			struct JniTypeName< _name_ >	\
			{ \
				typedef _name_::type	type; \
				\
				static void Add (INOUT String &s)	{ s << 'L' << _javaName_ << ';'; } \
			}; \
		}}

	
}	// Java
}	// GX_STL

#endif	// GX_JNI_ENABLED
