// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "Core/STL/Common/Init.h"
#include "JavaEnvironment.h"
#include "JavaMethod.h"

#ifdef PLATFORM_ANDROID

namespace GX_STL
{
namespace android
{
extern "C"
{
	
/*
=================================================
	JNI_OnLoad
=================================================
*/
	JNIEXPORT jint JNI_OnLoad (JavaVM* vm, void* reserved)
	{
		Java::JavaEnvironment::SetJavaVM( vm );

		return Java::JavaEnvironment::JNI_VERSION;
	}
	
/*
=================================================
	JNI_OnUnload
=================================================
*/
	JNIEXPORT void JNI_OnUnload (JavaVM* vm, void* reserved)
	{
		Java::JavaEnvironment::SetJavaVM( null );
	}

}	// extern "C"
}	// android
}	// GX_STL

#endif	// PLATFORM_ANDROID


namespace GXJava
{
	
/*
=================================================
	GetExceptionMessage
=================================================
*/
	bool JavaEnvironment::GetExceptionMessage (INOUT String &msg) const
	{
		JavaObject		throwable( _env->ExceptionOccurred() );
		CHECK_ERR( throwable );

		JavaMethod< jstring () >	throwable_get_message( throwable, "getMessage" );
		CHECK_ERR( throwable_get_message );

		UTFString	str( throwable_get_message.Call() );

		msg = (StringCRef) str;
		return true;
	}


}	// GXJava
