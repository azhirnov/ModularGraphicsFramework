// Copyright © 2014-2017  Zhirnov Andrey. All rights reserved.

#pragma once

#include "Engine/STL/Types/Ptr.h"
#include "Engine/STL/ThreadSafe/Singleton.h"

#ifdef PLATFORM_ANDROID
#	include "Engine/STL/OS/Android/OSAndroid.h"
#endif

#ifdef GX_JNI_ENABLED

namespace GX_STL
{
namespace  Java
{
	
#	ifdef PLATFORM_ANDROID
		using namespace android;
#	endif


	//
	// Java Environment
	//

	class JavaEnvironment
	{
	// types
	public:
		enum {
			JNI_VERSION	= JNI_VERSION_1_6
		};

	// variables
	private:
		Ptr< JNIEnv >		_env;
		bool				_mustBeDetached;


	// methods
	public:
		JavaEnvironment () : _mustBeDetached(false)
		{}


		JavaEnvironment (JNIEnv *env) : _mustBeDetached(false)
		{
			Set( _env );
		}


		~JavaEnvironment ()
		{
			Set( null );
		}


		bool Attach ()
		{
			Ptr< JavaVM >	vm = GetJavaVM();
			CHECK_ERR( vm.IsNotNull() );
				
			Detach();

			_env = null;

			void * env = null;

			jint status = vm->GetEnv( &env, JNI_VERSION );

			_env = (JNIEnv *) env;

			if ( status == JNI_OK )
			{
				CHECK_ERR( _env.IsNotNull() );
				_mustBeDetached = false;
				return true;
			}

			if ( status == JNI_EDETACHED )
			{
				CHECK_ERR( vm->AttachCurrentThread( &_env.ref(), null ) >= 0 );
				CHECK_ERR( _env.IsNotNull() );
				_mustBeDetached = true;
				return true;
			}

			RETURN_ERR( "can't get java environment" );
		}


		void Detach ()
		{
			if ( _mustBeDetached )
			{
				Ptr< JavaVM >	vm = GetJavaVM();
				CHECK_ERR( vm.IsNotNull(), );

				vm->DetachCurrentThread();

				_env			= null;
				_mustBeDetached	= false;
			}
		}


		void Set (JNIEnv *env)
		{
			Detach();
			_env			= env;
			_mustBeDetached	= false;
		}


		bool IsNull () const
		{
			return _env.IsNull();
		}


		bool IsNotNull () const
		{
			return _env.IsNotNull();
		}


		JNIEnv * Get () const
		{
			return _env;
		}
			

		void IncGlobalRef (jobject obj) const
		{
			_env->NewGlobalRef( obj );
		}


		void DecGlobalRef (jobject obj) const
		{
			_env->DeleteGlobalRef( obj );
		}


		void IncLocalRef (jobject obj) const
		{
			_env->NewLocalRef( obj );
		}


		void DecLocalRef (jobject obj) const
		{
			_env->DeleteLocalRef( obj );
		}


		bool CheckException () const
		{
			return _env->ExceptionCheck();
		}


		bool GetExceptionMessage (INOUT String &msg) const;


	//		JavaVM		//
	// types
	private:
		struct _JavaVirtualMachine
		{
			Ptr< JavaVM >	javaVM;

			static _JavaVirtualMachine * Get ()
			{
				return GXTypes::SingletonMultiThread::Instance< _JavaVirtualMachine >();
			}
		};


	// methods
	public:
		static void				SetJavaVM (JavaVM *jvm)		{ _JavaVirtualMachine::Get()->javaVM = jvm; }
		static Ptr< JavaVM >	GetJavaVM ()				{ return _JavaVirtualMachine::Get()->javaVM; }
	};



	//
	// Java Environment Wrapper
	//

	class JavaEnv : protected JavaEnvironment
	{
	public:
		JavaEnv ()
		{
			Attach();
		}


		JavaEnv (JNIEnv *env) : JavaEnvironment(env)
		{
			if ( IsNull() )
				Attach();
		}


		JNIEnv * operator -> () const
		{
			return Get();
		}
		

		JNIEnv * operator * () const
		{
			return Get();
		}


		void CatchException () const
		{
			if ( CheckException() )
				PrintExeption();
		}


		void PrintExeption () const
		{
			//String	err("Catched JNI Exception: ");
			//_env.GetExceptionMessage( err );
		}
	};

	
}	// Java
}	// GX_STL

#endif	// GX_JNI_ENABLED
