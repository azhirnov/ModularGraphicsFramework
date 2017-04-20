// Copyright © 2014-2017  Zhirnov Andrey. All rights reserved.

#pragma once

#include "JavaEnvironment.h"
#include "JavaTypeInfo.h"

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
		// Java Method Result
		//

		template <typename T>
		struct JavaMethodResult
		{
			typedef T	type;

			T	_value;

			JavaMethodResult (T value) : _value(value) {}

			operator T () const		{ return _value; }
		};


		template <>
		struct JavaMethodResult< void >
		{
			typedef void	type;

			JavaMethodResult (void) {}

			operator void () const	{ return; }
		};



		//
		// Base Java Method Type
		//
		
		template <typename R, typename Typelist>
		struct JavaMethodTypeBase
		{
		// types
		public:
			typedef Typelist				type_list_t;
			typedef R						result_t;
			typedef JavaMethodResult< R >	result_wrap_t;
			
			static const uint	NUM_ARGS	= type_list_t::Length::value;

		private:
			struct _ProcessTypeList
			{
				String &	s;

				_ProcessTypeList (INOUT String &str) : s(str) {}

				template <typename T>
				void Process ()
				{
					JniTypeName<T>::Add( s );
				}
			};


		// variables
		private:
			static const String		_signature;


		// methods
		public:
			static StringCRef	Signature ()
			{
				return _signature;
			}


		private:
			static String _BuildSignature ()
			{
				String	str;
				
				_ProcessTypeList	op( str );

				str << '(';
				Typelist::RuntimeForEach( op );
				str << ')';

				JniTypeName<R>::Add( str );

				return str;
			}


		// compile type checks			
			STATIC_ASSERT( typename type_list_t::_is_type_list(true), "must be type list" );
		};

		
		template <typename R, typename TL>
		const String	JavaMethodTypeBase<R,TL>::_signature = JavaMethodTypeBase<R,TL>::_BuildSignature();
		

	}	// _java_hidden_



	//
	// Java Method Type
	//

	template <typename Func>
	struct JavaMethodType
	{
	};


#	define _DECL_JAVA_METHOD_TYPE(	_templateParams_, \
									_templateArgs_, \
									_result_ ) \
		\
		template < _templateParams_ > \
		struct JavaMethodType < _result_ (_templateArgs_) > : \
			_java_hidden_::JavaMethodTypeBase< _result_, typename TypeListFrom< _templateArgs_ > > \
		{ \
		}

	_DECL_JAVA_METHOD_TYPE(
		ARGS( typename Ret,
				typename T0, typename T1, typename T2, typename T3, typename T4,
				typename T5, typename T6, typename T7, typename T8, typename T9,
				typename T10, typename T11, typename T12, typename T13, typename T14 ),
		ARGS( T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14 ),
		Ret );
		
	_DECL_JAVA_METHOD_TYPE(
		ARGS( typename Ret,
				typename T0, typename T1, typename T2, typename T3, typename T4,
				typename T5, typename T6, typename T7, typename T8, typename T9,
				typename T10, typename T11, typename T12, typename T13 ),
		ARGS( T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13 ),
		Ret );
	
	_DECL_JAVA_METHOD_TYPE(
		ARGS( typename Ret,
				typename T0, typename T1, typename T2, typename T3, typename T4,
				typename T5, typename T6, typename T7, typename T8, typename T9,
				typename T10, typename T11, typename T12 ),
		ARGS( T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12 ),
		Ret );
	
	_DECL_JAVA_METHOD_TYPE(
		ARGS( typename Ret,
				typename T0, typename T1, typename T2, typename T3, typename T4,
				typename T5, typename T6, typename T7, typename T8, typename T9,
				typename T10, typename T11 ),
		ARGS( T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11 ),
		Ret );
	
	_DECL_JAVA_METHOD_TYPE(
		ARGS( typename Ret,
				typename T0, typename T1, typename T2, typename T3, typename T4,
				typename T5, typename T6, typename T7, typename T8, typename T9,
				typename T10 ),
		ARGS( T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10 ),
		Ret );
	
	_DECL_JAVA_METHOD_TYPE(
		ARGS( typename Ret,
				typename T0, typename T1, typename T2, typename T3, typename T4,
				typename T5, typename T6, typename T7, typename T8, typename T9 ),
		ARGS( T0, T1, T2, T3, T4, T5, T6, T7, T8, T9 ),
		Ret );
	
	_DECL_JAVA_METHOD_TYPE(
		ARGS( typename Ret,
				typename T0, typename T1, typename T2, typename T3, typename T4,
				typename T5, typename T6, typename T7, typename T8 ),
		ARGS( T0, T1, T2, T3, T4, T5, T6, T7, T8 ),
		Ret );
	
	_DECL_JAVA_METHOD_TYPE(
		ARGS( typename Ret,
				typename T0, typename T1, typename T2, typename T3, typename T4,
				typename T5, typename T6, typename T7 ),
		ARGS( T0, T1, T2, T3, T4, T5, T6, T7 ),
		Ret );
	
	_DECL_JAVA_METHOD_TYPE(
		ARGS( typename Ret,
				typename T0, typename T1, typename T2, typename T3, typename T4,
				typename T5, typename T6 ),
		ARGS( T0, T1, T2, T3, T4, T5, T6 ),
		Ret );
	
	_DECL_JAVA_METHOD_TYPE(
		ARGS( typename Ret,
				typename T0, typename T1, typename T2, typename T3, typename T4,
				typename T5 ),
		ARGS( T0, T1, T2, T3, T4, T5 ),
		Ret );
	
	_DECL_JAVA_METHOD_TYPE(
		ARGS( typename Ret, typename T0, typename T1, typename T2, typename T3, typename T4 ),
		ARGS( T0, T1, T2, T3, T4 ),
		Ret );
	
	_DECL_JAVA_METHOD_TYPE(
		ARGS( typename Ret, typename T0, typename T1, typename T2, typename T3 ),
		ARGS( T0, T1, T2, T3 ),
		Ret );
	
	_DECL_JAVA_METHOD_TYPE(
		ARGS( typename Ret, typename T0, typename T1, typename T2 ),
		ARGS( T0, T1, T2 ),
		Ret );
	
	_DECL_JAVA_METHOD_TYPE(
		ARGS( typename Ret, typename T0, typename T1 ),
		ARGS( T0, T1 ),
		Ret );
	
	_DECL_JAVA_METHOD_TYPE(
		ARGS( typename Ret, typename T0 ),
		ARGS( T0 ),
		Ret );
	
	_DECL_JAVA_METHOD_TYPE(
		ARGS( typename Ret ),
		void,
		Ret );

#	undef _DECL_JAVA_METHOD_TYPE


}	// Java
}	// GX_STL

#endif	// GX_JNI_ENABLED
