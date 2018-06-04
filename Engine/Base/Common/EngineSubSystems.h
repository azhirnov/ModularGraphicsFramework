// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'
/*
	EngineSubSystems class used for access to engine systems at any place in code.
*/

#pragma once

#include "Engine/Base/Common/Common.h"
#include "Engine/Base/Common/ThreadID.h"

namespace Engine
{
namespace Base
{

	//
	// Const Reference
	//

	template <typename T>
	struct ConstReference
	{
	// types
	public:
		using Self	= ConstReference<T>;


	// variables
	private:
		Ptr< T >	_value;


	// methods
	private:
		ConstReference () {}
		
		ConstReference (T &&value) {}

		void operator = (const Self &) {}

	public:
		explicit ConstReference (T &value)			: _value(&value) {}
		explicit ConstReference (const T *value)	: _value((T*)value) {}
				 ConstReference (const Self &other) : _value(other._value) {}
		explicit ConstReference (NullPtr_t)			: _value(null) {}

		ND_ T *		operator -> ()	const			{ return _value; }
	};

}	// Base


	//
	// Global SubSystems
	//

	struct GlobalSubSystems
	{
	// types
		template <typename T>
		struct SubSysProperty
		{
		// variables
		private:
			Ptr< T >	_ptr;

		// methods
		public:
			SubSysProperty () {}

			ND_ T *  operator -> ()				{ return _ptr; }
			ND_ T *  ptr ()						{ return _ptr; }

			ND_ explicit operator bool () const	{ return bool(_ptr); }

			void _Set (T *newPtr)				{ ASSERT( (_ptr == null) == (newPtr != null) );  _ptr = newPtr; }
		};


	// variables
		SubSysProperty< Base::Module >			parallelThread;
		SubSysProperty< Base::Module >			taskModule;
		SubSysProperty< Base::Module >			mainSystem;
		SubSysProperty< Base::ModulesFactory >	modulesFactory;
	};

	using GlobalSystemsRef	= Base::ConstReference< GlobalSubSystems >;

}	// Engine
