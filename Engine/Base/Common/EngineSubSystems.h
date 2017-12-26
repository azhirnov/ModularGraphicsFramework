// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'
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

		T *			operator -> ()	const			{ return _value; }
	};

}	// Base


	//
	// SubSystems Helper
	//
	
	template <typename AllowedSetter>
	struct SubSystemsHelper
	{
	// types
		template <typename T>
		struct SubSysProperty
		{
			friend T;
			friend AllowedSetter;

		// variables
		private:
			Ptr< T >	_ptr;

		// methods
		public:
			SubSysProperty () {}

			T *  operator -> ()				{ return _ptr; }
			T *  Ptr ()						{ return _ptr; }

			explicit operator bool () const	{ return bool(_ptr); }

		private:
			void Set (T *newPtr)	{ ASSERT( (_ptr == null) == (newPtr != null) );  _ptr = newPtr; }
		};


	// variables
		SubSysProperty< Base::ParallelThread >	parallelThread;
		SubSysProperty< Base::TaskModule >		taskModule;
		SubSysProperty< Base::MainSystem >		mainSystem;
		SubSysProperty< Base::ModulesFactory >	modulesFactory;
		SubSysProperty< Base::FileManager >		fileManager;
	};



	//
	// Global SubSystems
	//

	struct GlobalSubSystems : SubSystemsHelper< Base::ThreadManager >
	{};

	using GlobalSystemsRef	= Base::ConstReference< GlobalSubSystems >;

}	// Engine
