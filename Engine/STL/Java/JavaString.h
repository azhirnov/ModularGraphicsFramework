// Copyright © 2014-2017  Zhirnov Andrey. All rights reserved.

#pragma once

#include "JavaEnvironment.h"

#ifdef GX_JNI_ENABLED

namespace GX_STL
{
namespace  Java
{

	
	//
	// UTF8 String
	//

	struct UTFString
	{
	// variables
	private:
		Ptr<const char>	_data;
		usize			_len;
		jstring			_str;
		bool const		_fromNative;	// true if created in native code

			
	// methods
	public:
		UTFString () : _data(null), _str(null), _len(0), _fromNative(false)
		{}


		explicit
		UTFString (jstring str) : _data(null), _str(null), _len(0), _fromNative(false)
		{
			JavaEnv je;

			_str  = str;
			_data = je->GetStringUTFChars( str, 0 );
			_len  = je->GetStringUTFLength( str );
		}


		explicit
		UTFString (StringCRef str) : _data(null), _str(null), _len(0), _fromNative(true)
		{
			JavaEnv je;

			_str  = je->NewStringUTF( str.cstr() );
		}


		UTFString (UTFString &&str) : _data(str._data), _len(str._len), _str(str._str), _fromNative(str._fromNative)
		{
			str._data	= null;
			str._str	= null;
			str._len	= 0;
		}

		~UTFString ()
		{
			JavaEnv je;

			if ( _fromNative )
			{
				if ( _str != null )
					je->DeleteLocalRef( _str );
			}
			else
			if ( _data.IsNotNull() and _str != null)
				je->ReleaseStringUTFChars( _str, _data.ptr() );
		}


		jstring			jstr ()		const	{ return _str; }

		const char *	cstr ()		const	{ return _data; }
		usize			Length ()	const	{ return _len; }

		operator StringCRef ()	const	{ return StringCRef( cstr(), Length() ); }
	};


	//
	// UTF16 String
	//

	struct WString
	{
	// variables
	public:
		Ptr<const wchar>	_data;
		usize				_len;
		jstring				_str;
		bool const			_fromNative;	// true if created in native code

		
	// methods
	public:
		WString () : _data(null), _str(null), _len(0), _fromNative(false)
		{}


		explicit
		WString (jstring str) : _data(null), _str(null), _len(0), _fromNative(false)
		{
			//STATIC_ASSERT( sizeof(wchar) == sizeof(jchar) );
			
			JavaEnv je;

			_str  = str;
			_data = (const wchar *)je->GetStringChars( str, null );
			_len  = je->GetStringLength( str );
		}


		explicit
		WString (WStringCRef str) : _data(null), _str(null), _len(0), _fromNative(true)
		{
			JavaEnv je;

			_str  = je->NewString( (const jchar *)str.cstr(), str.Length() );
		}


		WString (WString &&str) : _data(str._data), _str(str._str), _len(str._len), _fromNative(str._fromNative)
		{
			str._data	= null;
			str._str	= null;
			str._len	= 0;
		}


		~WString ()
		{
			JavaEnv je;

			if ( _fromNative )
			{
				if ( _str != null )
					je->DeleteLocalRef( _str );
			}
			else
			if ( _data.IsNotNull() and _str != null )
				je->ReleaseStringChars( _str, (const jchar *)_data.ptr() );
		}


		jstring			jstr ()		const	{ return _str; }

		const wchar *	cstr ()		const	{ return _data; }
		usize			Length ()	const	{ return _len; }

		operator	WStringCRef ()	const	{ return WStringCRef( cstr(), Length() ); }
	};


}	// Java
}	// GX_STL

#endif	// GX_JNI_ENABLED
