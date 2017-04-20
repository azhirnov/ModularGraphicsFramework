// Copyright © 2014-2017  Zhirnov Andrey. All rights reserved.

#pragma once

#include "JavaEnvironment.h"
#include "JavaMethodType.h"
#include "JavaTypeInfo.h"

#ifdef GX_JNI_ENABLED

namespace GX_STL
{
namespace  Java
{
	class JavaObject;

	
	//
	// Java Class
	//

	class JavaClass
	{
	// variables
	private:
		jclass	_class;


	// methods
	public:
		JavaClass () : _class(null)
		{}


		explicit
		JavaClass (jclass javaClass) : _class(javaClass)
		{
			_IncRef();
		}


		explicit
		JavaClass (StringCRef className) : _class(null)
		{
			JavaEnv je;

			_class = je->FindClass( className.cstr() );
			_IncRef();
		}

		
		explicit
		JavaClass (const JavaObject &obj);
		

		JavaClass (JavaClass &&javaClass) : _class(javaClass._class)
		{
			javaClass._class = null;
		}


		JavaClass (const JavaClass &other) : _class(other._class)
		{
			_IncRef();
		}


		~JavaClass ()
		{
			_DecRef();
		}
			

		JavaClass &	operator = (const JavaClass &right)
		{
			_DecRef();

			_class = right._class;
			_IncRef();

			return *this;
		}


		bool IsNull () const
		{
			return _class == null;
		}


		bool IsNotNull () const
		{
			return not IsNull();
		}


		jclass Get () const
		{
			return _class;
		}


		jmethodID GetMethodID (StringCRef name, StringCRef signature, bool isStatic) const
		{
			ASSERT( not name.Empty() );
			ASSERT( not signature.Empty() );
			ASSERT( IsNotNull() );
			
			JavaEnv je;

			return isStatic ? 
				je->GetStaticMethodID( _class, name.cstr(), signature.cstr() ) :
				je->GetMethodID( _class, name.cstr(), signature.cstr() );
		}


		template <typename T>
		static JavaClass FromType ()
		{
			typedef typename JniTypeName< T >::type	_is_jni_type;

			JavaEnv je;

			String	class_name;

			JniTypeName< T >::Add( class_name );

			// remove first 'L'
			if ( class_name.Length() > 1 and class_name.Front() == 'L' )
				class_name.Erase( 0 );

			// remove last ';'
			if ( class_name.Length() > 1 and class_name.Back() == ';' )
				class_name.PopBack();

			return JavaClass( class_name );
		}


	private:
		void _IncRef ()
		{
			if ( IsNotNull() )
			{
				JavaEnv je;
				_class = (jclass) je->NewGlobalRef( (jobject) _class );
			}
		}


		void _DecRef ()
		{
			if ( IsNotNull() )
			{
				JavaEnv je;
				
				je->DeleteGlobalRef( _class );
				_class = null;
			}
		}
	};
	


	//
	// Java Object
	//

	class JavaObject
	{
	// variables
	private:
		jobject		_obj;


	// methods
	public:
		JavaObject () : _obj(null)
		{}


		explicit
		JavaObject (jobject obj) : _obj(obj)
		{
			_IncRef();
		}

		
		explicit
		JavaObject (const JavaClass &javaClass) : _obj(null)
		{
			Create( javaClass );
		}
		

		JavaObject (const JavaObject &obj) : _obj(obj._obj)
		{
			_IncRef();
		}
		

		JavaObject (JavaObject &&obj) : _obj(obj._obj)
		{
			obj._obj = null;
		}


		~JavaObject ()
		{
			_DecRef();
		}


		JavaObject &	operator = (const JavaObject &right)
		{
			_DecRef();

			_obj = right._obj;
			_IncRef();

			return *this;
		}


		bool Create (const JavaClass &javaClass)
		{
			return _Create< void () >( javaClass, 0 );
		}


		bool Create (const JavaClass &javaClass, const JavaObject &other)
		{
			return _Create< void (jobject) >( javaClass, other.Get() );
		}
			

		bool IsNull () const
		{
			return _obj == null;
		}


		bool IsNotNull () const
		{
			return not IsNull();
		}


		jobject Get () const
		{
			return _obj;
		}


	private:
		
		template <typename Func>
		bool _Create (const JavaClass &javaClass, ...)
		{
			typedef JavaMethodType< Func >	ctor_t;

			const jmethodID	ctor_id = javaClass.GetMethodID( "<init>", ctor_t::Signature(), false );
			CHECK_ERR( ctor_id != null );
			
			va_list args;
			va_start( args, javaClass );

			_DecRef();

			_obj = JavaEnv()->NewObjectV( javaClass.Get(), ctor_id, args );
			
			_IncRef();
			
			va_end( args );

			CHECK_ERR( IsNotNull() );
			return true;
		}


		void _IncRef ()
		{
			if ( IsNotNull() )
			{
				JavaEnv je;
				
				_obj = je->NewGlobalRef( _obj );
			}
		}

		void _DecRef ()
		{
			if ( IsNotNull() )
			{
				JavaEnv je;

				je->DeleteGlobalRef( _obj );
				_obj = null;
			}
		}
	};


	
	inline JavaClass::JavaClass (const JavaObject &obj) : _class(null)
	{
		if ( obj.IsNotNull() )
		{
			JavaEnv je;

			_class = je->GetObjectClass( obj.Get() );
			_IncRef();
		}
	}


}	// Java
}	// GX_STL

#endif	// GX_JNI_ENABLED
