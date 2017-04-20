// Copyright © 2014-2017  Zhirnov Andrey. All rights reserved.

#pragma once

#include "JavaEnvironment.h"
#include "JavaClass.h"
#include "JavaArray.h"
#include "JavaString.h"

#ifdef GX_JNI_ENABLED

namespace GX_STL
{
namespace  Java
{
	
	//
	// Helpers
	//

	namespace _java_hidden_
	{

		//
		// Java Method Caller
		//

		template <typename R>
		struct JavaMethodCaller
		{
			typedef R													result_t;
			typedef JavaMethodResult< R >								result_wrap_t;
			typedef JavaMethodCaller< typename JniTypeName< R >::type >	base_t;
			typedef typename base_t::result_t							base_result_t;
			typedef typename base_t::result_wrap_t						base_result_wrap_t;
			
			static result_wrap_t	CallStatic (const JavaClass &javaClass, jmethodID methodID, va_list args)
			{
				return (result_t) (base_result_t) base_t::CallStatic( javaClass, methodID, args );
			}
			
			static result_wrap_t	CallNonvirtual (const JavaObject &obj, const JavaClass &javaClass, jmethodID methodID, va_list args)
			{
				return (result_t) (base_result_t) base_t::CallNonvirtual( obj, javaClass, methodID, args );
			}
			
			static result_wrap_t	Call (const JavaObject &obj, jmethodID methodID, va_list args)
			{
				return (result_t) (base_result_t) base_t::Call( obj, methodID, args );
			}
		};


#		define _DECL_METHOD_CALLER( _type_, _typeName_ ) \
			template <> \
			struct JavaMethodCaller< _type_ > \
			{ \
				typedef _type_						result_t; \
				typedef JavaMethodResult< _type_ >	result_wrap_t; \
				\
				static result_wrap_t	CallStatic (const JavaClass &javaClass, jmethodID methodID, va_list args) \
				{ \
					return JavaEnv()->CallStatic##_typeName_##MethodV( javaClass.Get(), methodID, args ); \
				} \
				\
				static result_wrap_t	CallNonvirtual (const JavaObject &obj, const JavaClass &javaClass, jmethodID methodID, va_list args) \
				{ \
					return JavaEnv()->CallNonvirtual##_typeName_##MethodV( obj.Get(), javaClass.Get(), methodID, args ); \
				} \
				\
				static result_wrap_t	Call (const JavaObject &obj, jmethodID methodID, va_list args) \
				{ \
					return JavaEnv()->Call##_typeName_##MethodV( obj.Get(), methodID, args ); \
				} \
			};

		_DECL_METHOD_CALLER( jobject,	Object	)
		_DECL_METHOD_CALLER( jboolean,	Boolean	)
		_DECL_METHOD_CALLER( jbyte,		Byte	)
		_DECL_METHOD_CALLER( jchar,		Char	)
		_DECL_METHOD_CALLER( jshort,	Short	)
		_DECL_METHOD_CALLER( jint,		Int		)
		_DECL_METHOD_CALLER( jlong,		Long	)
		_DECL_METHOD_CALLER( jfloat,	Float	)
		_DECL_METHOD_CALLER( jdouble,	Double	)

#		undef _DECL_METHOD_CALLER
		

		template <>
		struct JavaMethodCaller< void >
		{
			typedef void						result_t;
			typedef JavaMethodResult< void >	result_wrap_t;
			
			static result_wrap_t	CallStatic (const JavaClass &javaClass, jmethodID methodID, va_list args)
			{
				JavaEnv()->CallStaticVoidMethodV( javaClass.Get(), methodID, args );
				return result_wrap_t();
			}
			
			static result_wrap_t	CallNonvirtual (const JavaObject &obj, const JavaClass &javaClass, jmethodID methodID, va_list args)
			{
				JavaEnv()->CallNonvirtualVoidMethodV( obj.Get(), javaClass.Get(), methodID, args );
				return result_wrap_t();
			}
			
			static result_wrap_t	Call (const JavaObject &obj, jmethodID methodID, va_list args)
			{
				JavaEnv()->CallVoidMethodV( obj.Get(), methodID, args );
				return result_wrap_t();
			}
		};
		
		
		template <>
		struct JavaMethodCaller< jstring >
		{
			typedef jstring						result_t;
			typedef JavaMethodResult< jstring >	result_wrap_t;
			
			static result_wrap_t	CallStatic (const JavaClass &javaClass, jmethodID methodID, va_list args)
			{
				return (jstring) JavaEnv()->CallStaticObjectMethodV( javaClass.Get(), methodID, args );
			}
			
			static result_wrap_t	CallNonvirtual (const JavaObject &obj, const JavaClass &javaClass, jmethodID methodID, va_list args)
			{
				return (jstring) JavaEnv()->CallNonvirtualObjectMethodV( obj.Get(), javaClass.Get(), methodID, args );
			}
			
			static result_wrap_t	Call (const JavaObject &obj, jmethodID methodID, va_list args)
			{
				return (jstring) JavaEnv()->CallObjectMethodV( obj.Get(), methodID, args );
			}
		};



		//
		// Type To Java Type
		//

		template <typename T>
		inline T TypeToJavaType (const T &t);

#		define _DECL_TYPE_TO_JAVA_TYPE( _type_ ) \
			inline _type_ TypeToJavaType (const _type_ &t) { \
				return t; \
			}
		
#		define _DECL_TYPE_TO_JAVA_TYPE2( _jtype_, _type_ ) \
			inline _jtype_ TypeToJavaType (const _type_ &t) { \
				return _jtype_( t ); \
			}
			
		_DECL_TYPE_TO_JAVA_TYPE(	jobject		)
		_DECL_TYPE_TO_JAVA_TYPE(	jstring		)
		_DECL_TYPE_TO_JAVA_TYPE(	jboolean	)
		_DECL_TYPE_TO_JAVA_TYPE(	jbyte		)
		_DECL_TYPE_TO_JAVA_TYPE(	jchar		)
		_DECL_TYPE_TO_JAVA_TYPE(	jshort		)
		_DECL_TYPE_TO_JAVA_TYPE(	jint		)
		_DECL_TYPE_TO_JAVA_TYPE(	jlong		)
		_DECL_TYPE_TO_JAVA_TYPE(	jfloat		)
		_DECL_TYPE_TO_JAVA_TYPE(	jdouble		)

		_DECL_TYPE_TO_JAVA_TYPE2(	jboolean,	bool )
		_DECL_TYPE_TO_JAVA_TYPE2(	jint,		uint )
		_DECL_TYPE_TO_JAVA_TYPE2(	jlong,		ulong )

#		undef _DECL_TYPE_TO_JAVA_TYPE2
#		undef _DECL_TYPE_TO_JAVA_TYPE
		
		inline jobject TypeToJavaType (const JavaObject &obj) {
			return obj.Get();
		}

		inline jstring TypeToJavaType (const UTFString &str) {
			return str.jstr();
		}

		template <typename T>
		inline typename JavaArray<T>::jarray_t  TypeToJavaType (const JavaArray<T> &arr) {
			return arr.jarray();
		}


		
		//
		// Java Static Method
		//

		template <typename Func>
		class JavaStaticMethod
		{
		// types
		private:
			typedef JavaMethodType< Func >				method_t;
			typedef typename method_t::result_t			result_t;
			typedef typename method_t::result_wrap_t	result_wrap_t;


		// variables
		private:
			JavaClass	_class;
			jmethodID	_methodID;


		// methods
		public:
			JavaStaticMethod () : _class(), _methodID(null)
			{}


			JavaStaticMethod (const JavaClass &javaClass, StringCRef methodName) : _class(), _methodID(null)
			{
				Create( javaClass, methodName );
			}
			

			bool Create (const JavaClass &javaClass, StringCRef methodName)
			{
				SetClass( javaClass );
				
				if ( _class.IsNotNull() )
				{
					_methodID = _class.GetMethodID( methodName, method_t::Signature(), true );
				}

				return IsNotNull();
			}


			void SetClass (const JavaClass &javaClass)
			{
				_class = javaClass;
			}


			bool IsNull () const
			{
				return _class.IsNull() or _methodID == null;
			}


			bool IsNotNull () const
			{
				return not IsNull();
			}


			jmethodID Get () const
			{
				return _methodID;
			}


			JavaClass const & GetClass () const
			{
				return _class;
			}


		protected:
			result_t _Call (int empty, ...) const
			{
				CHECK_ERR( IsNotNull(), result_t() );

				va_list args;
				va_start( args, empty );

				result_wrap_t result = JavaMethodCaller< result_t >::CallStatic( _class, _methodID, args );

				va_end( args );
				return (result_t) result;
			}
		};



		//
		// Java Method
		//

		template <typename Func>
		class JavaMethod
		{
		// types
		private:
			typedef JavaMethodType< Func >				method_t;
			typedef typename method_t::result_t			result_t;
			typedef typename method_t::result_wrap_t	result_wrap_t;


		// variables
		private:
			JavaObject	_obj;
			jmethodID	_methodID;


		// methods
		public:
			JavaMethod () : _obj(), _methodID(null)
			{}


			JavaMethod (const JavaObject &obj, StringCRef methodName) : _obj(), _methodID(null)
			{
				Create( obj, methodName );
			}


			bool Create (const JavaObject &obj, StringCRef methodName)
			{
				SetObject( obj );

				if ( _obj.IsNotNull() )
				{
					const JavaClass	java_class( _obj );

					_methodID = java_class.GetMethodID( methodName, method_t::Signature(), false );
				}

				return IsNotNull();
			}


			void SetObject (const JavaObject &obj)
			{
				_obj = obj;
			}

			
			bool IsNull () const
			{
				return _obj.IsNull() or _methodID == null;
			}


			bool IsNotNull () const
			{
				return not IsNull();
			}


			jmethodID Get () const
			{
				return _methodID;
			}


			JavaObject const & GetObject () const
			{
				return _obj;
			}


		protected:
			result_t _Call (int empty, ...) const
			{
				CHECK_ERR( IsNotNull(), result_t() );

				va_list args;
				va_start( args, empty );

				result_wrap_t result = JavaMethodCaller< result_t >::Call( _obj, _methodID, args );

				va_end( args );
				return (result_t) result;
			}
		};



		//
		// Java Object Constructor
		//

		template <typename Func>
		class JavaObjectConstructor
		{
		// types
		private:
			typedef JavaMethodType< Func >				method_t;
			typedef typename method_t::result_t			result_t;
			typedef typename method_t::result_wrap_t	result_wrap_t;


		// variables
		protected:
			JavaClass	_class;
			jmethodID	_methodID;


		// methods
		public:
			explicit
			JavaObjectConstructor (const JavaClass &javaClass) : _class(javaClass), _methodID(null)
			{
				if ( _class.IsNotNull() )
				{
					_methodID = _class.GetMethodID( "<init>", method_t::Signature(), false );
				}
			}
			
			bool IsNull () const
			{
				return _class.IsNull() or _methodID == null;
			}


			bool IsNotNull () const
			{
				return not IsNull();
			}


			jmethodID Get () const
			{
				return _methodID;
			}


			JavaClass const & GetClass () const
			{
				return _class;
			}


		protected:
			jobject _Call (int empty, ...) const
			{
				CHECK_ERR( IsNotNull(), jobject(null) );

				va_list args;
				va_start( args, empty );

				const jobject	obj = JavaEnv()->NewObjectV( _class.Get(), _methodID, args );

				va_end( args );
				return obj;
			}
		};


	}	// _java_hidden_

	

	template <typename Func>
	class JavaStaticMethod;
	
	template <typename Func>
	class JavaMethod;

	template <typename Func>
	class JavaObjectConstructor;
	
	
#	define _DECL_JAVA_METHOD(	_templateParams_, \
								_templateArgs_, \
								_funcTemplate_, \
								_funcArgsDecl_, \
								_funcArgs_, \
								_result_ ) \
	\
	template < _templateParams_ > \
	class JavaStaticMethod< _result_ (_templateArgs_) > : \
		public _java_hidden_::JavaStaticMethod< _result_ (_templateArgs_) > \
	{ \
	private: \
		typedef JavaStaticMethod< _result_ (_templateArgs_) >					Self; \
		typedef _java_hidden_::JavaStaticMethod< _result_ (_templateArgs_) >		base_t; \
		\
	public: \
		JavaStaticMethod () : base_t()	{} \
		JavaStaticMethod (const JavaClass &javaClass, StringCRef methodName) : base_t( javaClass, methodName ) {} \
		\
		_funcTemplate_ \
		_result_ Call (_funcArgsDecl_) const \
		{ \
			return base_t::_Call( _funcArgs_ ); \
		} \
	}; \
	\
	template < _templateParams_ > \
	class JavaMethod< _result_ (_templateArgs_) > : \
		public _java_hidden_::JavaMethod< _result_ (_templateArgs_) > \
	{ \
	public: \
		typedef JavaMethod< _result_ (_templateArgs_) >					Self; \
		typedef _java_hidden_::JavaMethod< _result_ (_templateArgs_) >	base_t; \
		\
	public: \
		JavaMethod () : base_t()	{} \
		JavaMethod (const JavaObject &obj, StringCRef methodName) : base_t( obj, methodName )	{} \
		\
		_funcTemplate_ \
		_result_ Call (_funcArgsDecl_) const \
		{ \
			return base_t::_Call( _funcArgs_ ); \
		} \
	}; \
	\
	template < _templateParams_ > \
	class JavaObjectConstructor< _result_ (_templateArgs_) > : \
		public _java_hidden_::JavaObjectConstructor< _result_ (_templateArgs_) > \
	{ \
	public: \
		typedef JavaObjectConstructor< _result_ (_templateArgs_) >					Self; \
		typedef _java_hidden_::JavaObjectConstructor< _result_ (_templateArgs_) >	base_t; \
		\
	public: \
		explicit \
		JavaObjectConstructor (const JavaClass &javaClass) : base_t(javaClass) {} \
		\
		_funcTemplate_ \
		JavaObject Call (_funcArgsDecl_) const \
		{ \
			return JavaObject( base_t::_Call( _funcArgs_ ) ); \
		} \
		\
		STATIC_ASSERT( CompileTime::IsVoid< _result_ >(), "result type must be void" ); \
	}

#	define _J( _x_ )	_java_hidden_::TypeToJavaType( _x_ )
#	define _R( _x_ )	const _x_ &

	_DECL_JAVA_METHOD(
		ARGS( typename Ret,
				typename T0, typename T1, typename T2, typename T3, typename T4,
				typename T5, typename T6, typename T7, typename T8, typename T9,
				typename T10, typename T11, typename T12, typename T13, typename T14 ),
		ARGS( T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14 ),
		ARGS( template < 
				typename B0, typename B1, typename B2, typename B3, typename B4,
				typename B5, typename B6, typename B7, typename B8, typename B9,
				typename B10, typename B11, typename B12, typename B13, typename B14 > ),
		ARGS( _R(B0) a0, _R(B1) a1, _R(B2) a2, _R(B3) a3, _R(B4) a4, _R(B5) a5, _R(B6) a6, _R(B7) a7, _R(B8) a8,
			  _R(B9) a9, _R(B10) a10, _R(B11) a11, _R(B12) a12, _R(B13) a13, _R(B14) a14 ),
		ARGS( 15, _J(a0), _J(a1), _J(a2), _J(a3), _J(a4), _J(a5), _J(a6), _J(a7), _J(a8), _J(a9), _J(a10),
			  _J(a11), _J(a12), _J(a13), _J(a14) ),
		Ret );
		
	_DECL_JAVA_METHOD(
		ARGS( typename Ret,
				typename T0, typename T1, typename T2, typename T3, typename T4,
				typename T5, typename T6, typename T7, typename T8, typename T9,
				typename T10, typename T11, typename T12, typename T13 ),
		ARGS( T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13 ),
		ARGS( template < 
				typename B0, typename B1, typename B2, typename B3, typename B4,
				typename B5, typename B6, typename B7, typename B8, typename B9,
				typename B10, typename B11, typename B12, typename B13 > ),
		ARGS( _R(B0) a0, _R(B1) a1, _R(B2) a2, _R(B3) a3, _R(B4) a4, _R(B5) a5, _R(B6) a6, _R(B7) a7, _R(B8) a8,
			  _R(B9) a9, _R(B10) a10, _R(B11) a11, _R(B12) a12, _R(B13) a13 ),
		ARGS( 14, _J(a0), _J(a1), _J(a2), _J(a3), _J(a4), _J(a5), _J(a6), _J(a7), _J(a8), _J(a9), _J(a10),
			  _J(a11), _J(a12), _J(a13) ),
		Ret );
	
	_DECL_JAVA_METHOD(
		ARGS( typename Ret,
				typename T0, typename T1, typename T2, typename T3, typename T4,
				typename T5, typename T6, typename T7, typename T8, typename T9,
				typename T10, typename T11, typename T12 ),
		ARGS( T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12 ),
		ARGS( template < 
				typename B0, typename B1, typename B2, typename B3, typename B4,
				typename B5, typename B6, typename B7, typename B8, typename B9,
				typename B10, typename B11, typename B12 > ),
		ARGS( _R(B0) a0, _R(B1) a1, _R(B2) a2, _R(B3) a3, _R(B4) a4, _R(B5) a5, _R(B6) a6, _R(B7) a7,
			  _R(B8) a8, _R(B9) a9, _R(B10) a10, _R(B11) a11, _R(B12) a12 ),
		ARGS( 13, _J(a0), _J(a1), _J(a2), _J(a3), _J(a4), _J(a5), _J(a6), _J(a7), _J(a8), _J(a9),
			  _J(a10), _J(a11), _J(a12) ),
		Ret );
	
	_DECL_JAVA_METHOD(
		ARGS( typename Ret,
				typename T0, typename T1, typename T2, typename T3, typename T4,
				typename T5, typename T6, typename T7, typename T8, typename T9,
				typename T10, typename T11 ),
		ARGS( T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11 ),
		ARGS( template < 
				typename B0, typename B1, typename B2, typename B3, typename B4,
				typename B5, typename B6, typename B7, typename B8, typename B9,
				typename B10, typename B11 > ),
		ARGS( _R(B0) a0, _R(B1) a1, _R(B2) a2, _R(B3) a3, _R(B4) a4, _R(B5) a5, _R(B6) a6,
			  _R(B7) a7, _R(B8) a8, _R(B9) a9, _R(B10) a10, _R(B11) a11 ),
		ARGS( 12, _J(a0), _J(a1), _J(a2), _J(a3), _J(a4), _J(a5), _J(a6), _J(a7), _J(a8), _J(a9), _J(a10), _J(a11) ),
		Ret );
	
	_DECL_JAVA_METHOD(
		ARGS( typename Ret,
				typename T0, typename T1, typename T2, typename T3, typename T4,
				typename T5, typename T6, typename T7, typename T8, typename T9,
				typename T10 ),
		ARGS( T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10 ),
		ARGS( template < 
				typename B0, typename B1, typename B2, typename B3, typename B4,
				typename B5, typename B6, typename B7, typename B8, typename B9,
				typename B10 > ),
		ARGS( _R(B0) a0, _R(B1) a1, _R(B2) a2, _R(B3) a3, _R(B4) a4, _R(B5) a5, _R(B6) a6,
			  _R(B7) a7, _R(B8) a8, _R(B9) a9, _R(B10) a10 ),
		ARGS( 11, _J(a0), _J(a1), _J(a2), _J(a3), _J(a4), _J(a5), _J(a6), _J(a7), _J(a8), _J(a9), _J(a10) ),
		Ret );
	
	_DECL_JAVA_METHOD(
		ARGS( typename Ret,
				typename T0, typename T1, typename T2, typename T3, typename T4,
				typename T5, typename T6, typename T7, typename T8, typename T9 ),
		ARGS( T0, T1, T2, T3, T4, T5, T6, T7, T8, T9 ),
		ARGS( template < 
				typename B0, typename B1, typename B2, typename B3, typename B4,
				typename B5, typename B6, typename B7, typename B8, typename B9 > ),
		ARGS( _R(B0) a0, _R(B1) a1, _R(B2) a2, _R(B3) a3, _R(B4) a4, _R(B5) a5, _R(B6) a6, _R(B7) a7, _R(B8) a8, _R(B9) a9 ),
		ARGS( 10, _J(a0), _J(a1), _J(a2), _J(a3), _J(a4), _J(a5), _J(a6), _J(a7), _J(a8), _J(a9) ),
		Ret );
	
	_DECL_JAVA_METHOD(
		ARGS( typename Ret,
				typename T0, typename T1, typename T2, typename T3, typename T4,
				typename T5, typename T6, typename T7, typename T8 ),
		ARGS( T0, T1, T2, T3, T4, T5, T6, T7, T8 ),
		ARGS( template < 
				typename B0, typename B1, typename B2, typename B3, typename B4,
				typename B5, typename B6, typename B7, typename B8 > ),
		ARGS( _R(B0) a0, _R(B1) a1, _R(B2) a2, _R(B3) a3, _R(B4) a4, _R(B5) a5, _R(B6) a6, _R(B7) a7, _R(B8) a8 ),
		ARGS( 9, _J(a0), _J(a1), _J(a2), _J(a3), _J(a4), _J(a5), _J(a6), _J(a7), _J(a8) ),
		Ret );
	
	_DECL_JAVA_METHOD(
		ARGS( typename Ret,
				typename T0, typename T1, typename T2, typename T3, typename T4,
				typename T5, typename T6, typename T7 ),
		ARGS( T0, T1, T2, T3, T4, T5, T6, T7 ),
		ARGS( template < 
				typename B0, typename B1, typename B2, typename B3, typename B4,
				typename B5, typename B6, typename B7 > ),
		ARGS( _R(B0) a0, _R(B1) a1, _R(B2) a2, _R(B3) a3, _R(B4) a4, _R(B5) a5, _R(B6) a6, _R(B7) a7 ),
		ARGS( 8, _J(a0), _J(a1), _J(a2), _J(a3), _J(a4), _J(a5), _J(a6), _J(a7) ),
		Ret );
	
	_DECL_JAVA_METHOD(
		ARGS( typename Ret,
				typename T0, typename T1, typename T2, typename T3, typename T4,
				typename T5, typename T6 ),
		ARGS( T0, T1, T2, T3, T4, T5, T6 ),
		ARGS( template < 
				typename B0, typename B1, typename B2, typename B3, typename B4,
				typename B5, typename B6 > ),
		ARGS( _R(B0) a0, _R(B1) a1, _R(B2) a2, _R(B3) a3, _R(B4) a4, _R(B5) a5, _R(B6) a6 ),
		ARGS( 7, _J(a0), _J(a1), _J(a2), _J(a3), _J(a4), _J(a5), _J(a6) ),
		Ret );
	
	_DECL_JAVA_METHOD(
		ARGS( typename Ret,
				typename T0, typename T1, typename T2, typename T3, typename T4,
				typename T5 ),
		ARGS( T0, T1, T2, T3, T4, T5 ),
		ARGS( template < 
				typename B0, typename B1, typename B2, typename B3, typename B4,
				typename B5 > ),
		ARGS( _R(B0) a0, _R(B1) a1, _R(B2) a2, _R(B3) a3, _R(B4) a4, _R(B5) a5 ),
		ARGS( 6, _J(a0), _J(a1), _J(a2), _J(a3), _J(a4), _J(a5) ),
		Ret );
	
	_DECL_JAVA_METHOD(
		ARGS( typename Ret, typename T0, typename T1, typename T2, typename T3, typename T4 ),
		ARGS( T0, T1, T2, T3, T4 ),
		ARGS( template < typename B0, typename B1, typename B2, typename B3, typename B4 > ),
		ARGS( _R(B0) a0, _R(B1) a1, _R(B2) a2, _R(B3) a3, _R(B4) a4 ),
		ARGS( 5, _J(a0), _J(a1), _J(a2), _J(a3), _J(a4) ),
		Ret );
	
	_DECL_JAVA_METHOD(
		ARGS( typename Ret, typename T0, typename T1, typename T2, typename T3 ),
		ARGS( T0, T1, T2, T3 ),
		ARGS( template < typename B0, typename B1, typename B2, typename B3 > ),
		ARGS( _R(B0) a0, _R(B1) a1, _R(B2) a2, _R(B3) a3 ),
		ARGS( 4, _J(a0), _J(a1), _J(a2), _J(a3) ),
		Ret );
	
	_DECL_JAVA_METHOD(
		ARGS( typename Ret, typename T0, typename T1, typename T2 ),
		ARGS( T0, T1, T2 ),
		ARGS( template < typename B0, typename B1, typename B2 > ),
		ARGS( _R(B0) a0, _R(B1) a1, _R(B2) a2 ),
		ARGS( 3, _J(a0), _J(a1), _J(a2) ),
		Ret );
	
	_DECL_JAVA_METHOD(
		ARGS( typename Ret, typename T0, typename T1 ),
		ARGS( T0, T1 ),
		ARGS( template < typename B0, typename B1 > ),
		ARGS( _R(B0) a0, _R(B1) a1 ),
		ARGS( 2, _J(a0), _J(a1) ),
		Ret );
	
	_DECL_JAVA_METHOD(
		ARGS( typename Ret, typename T0 ),
		ARGS( T0 ),
		ARGS( template < typename B0 > ),
		ARGS( _R(B0) a0 ),
		ARGS( 1, _J(a0) ),
		Ret );
	
	_DECL_JAVA_METHOD(
		ARGS( typename Ret ),
		void,
		,
		,
		0,
		Ret );
	

#	undef _R
#	undef _J
#	undef _DECL_JAVA_METHOD


}	// Java
}	// GX_STL

#endif	// GX_JNI_ENABLED
