// Copyright © 2014-2017  Zhirnov Andrey. All rights reserved.

#pragma once

#include "JavaEnvironment.h"

#ifdef GX_JNI_ENABLED

namespace GX_STL
{
namespace  Java
{
	
	//
	// Array
	//

	template <typename T>
	struct JavaArray {};

#	define JNI_ARRAY_TYPE( _type_, _type2_ ) \
		template <> \
		struct JavaArray < _type_ > \
		{ \
		/* types */ \
		public: \
			typedef _type_			value_t; \
			typedef _type_##Array	jarray_t; \
		\
		/* variables */ \
		private: \
			Ptr< _type_ >	_data; \
			jarray_t		_arr; \
			usize			_count; \
			bool			_readOnly; \
			bool const		_fromNative; 	/* true if created in native code */ \
		\
		/* methods */ \
		public: \
			JavaArray () : \
				_data(null), _arr(null), _count(0), _readOnly(false), _fromNative(false) \
			{} \
			\
			\
			explicit \
			JavaArray (jarray_t arr, bool readOnly = false) : \
				_arr(arr), _data(null), _count(0), _readOnly(readOnly), _fromNative(false) \
			{ \
				JavaEnv je; \
				\
				_data  = je->Get##_type2_##ArrayElements( arr, 0 ); \
				_count = je->GetArrayLength( arr ); \
			} \
			\
			\
			explicit \
			JavaArray (ArrayCRef<_type_> nativeArray) : \
				_arr(null), _data(null), _count(0), _readOnly(false), _fromNative(true) \
			{ \
				JavaEnv je; \
				\
				_arr   = je->New##_type2_##Array( nativeArray.Count() ); \
				_data  = je->Get##_type2_##ArrayElements( _arr, null ); \
				_count = je->GetArrayLength( _arr ); \
				\
				FOR( i, nativeArray ) { \
					_data[i] = nativeArray[i]; \
				} \
			} \
			\
			\
			~JavaArray () \
			{ \
				JavaEnv je; \
				\
				if ( _fromNative ) \
				{ \
					if ( _arr != null ) \
						je->DeleteLocalRef( _arr ); \
				} \
				else \
				if ( _data and _arr != null ) \
					je->Release##_type2_##ArrayElements( _arr, _data.ptr(), _readOnly ? JNI_ABORT : 0 ); \
			} \
			\
			\
			const _type_ *	ptr ()				const	{ return _data; } \
			_type_ *		ptr ()						{ ASSERT( not _readOnly );  return _data; } \
			usize			Count ()			const	{ return _count; } \
			usize			Size ()				const	{ return _count * sizeof(value_t); } \
			bool			IsReadOnly ()		const	{ return _readOnly; } \
			\
			jarray_t		jarray ()			const	{ ASSUME( _arr != null ); return _arr; } \
			\
			_type_ const &	operator [] (int i)	const	{ return ptr()[i]; } \
			_type_ &		operator [] (int i)			{ return ptr()[i]; } \
			\
			operator ArrayCRef< _type_ > () const	{ return ArrayCRef< _type_ >( ptr(), Count() ); } \
			operator ArrayRef< _type_ >		  ()		{ return ArrayRef< _type_ >( ptr(), Count() ); } \
			\
			\
			template <typename B> \
			ArrayCRef< B >	To () const \
			{ \
				STATIC_ASSERT( sizeof(B) == sizeof(value_t) ); \
				return ArrayCRef< B >( (B *)ptr(), Count() ); \
			} \
			\
			\
			template <typename B> \
			ArrayCRef< B >	Cast () const \
			{ \
				ASSERT( Size() % sizeof(B) == 0 ); \
				return ArrayCRef< B >( (B *) ptr(), Size() / sizeof(B) ); \
			} \
		}

	//JNI_ARRAY_TYPE( jboolean,	Boolean	);
	JNI_ARRAY_TYPE( jbyte,		Byte	);
	JNI_ARRAY_TYPE( jchar,		Char	);
	JNI_ARRAY_TYPE( jshort,		Short	);
	JNI_ARRAY_TYPE( jint,		Int		);
	JNI_ARRAY_TYPE( jlong,		Long	);
	JNI_ARRAY_TYPE( jfloat,		Float	);
	JNI_ARRAY_TYPE( jdouble,	Double	);

#	undef JNI_ARRAY_TYPE


}	// Java
}	// GX_STL

#endif	// GX_JNI_ENABLED
