// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/Platforms/Soft/ShaderLang/SWLangCommon.h"
#include "Engine/STL/OS/OSLowLevel.h"

#ifdef GRAPHICS_API_SOFT

namespace SWShaderLang
{
namespace Impl
{

	//
	// Barrier
	//
	struct Barrier final
	{
		friend class SWShaderHelper;

	// types
	private:
		using Atomic_t	= GX_STL::GXTypes::Atomic<int>;

	// variables
	private:
		Ptr<Atomic_t>	_atomic;
		bool			_signaled	= false;

	// methods
	private:
		explicit Barrier (Atomic_t &atomic);
		
		Barrier& operator = (const Barrier &) = default;
		Barrier& operator = (Barrier &&) = default;

	public:
		Barrier () {}
		~Barrier ();

		void Wait ();
	};

	
	inline Barrier::Barrier (Atomic_t &atomic) :
		_atomic{&atomic}, _signaled{false}
	{}
	
	inline Barrier::~Barrier ()
	{
		if ( not _signaled ) {
			--(*_atomic);
		}
	}

	inline void Barrier::Wait ()
	{
		if ( _signaled )
			return;

		--(*_atomic);

		while ( (*_atomic) > 0 )
		{
			GX_STL::OS::CurrentThread::Yield();
		};

		_signaled = true;
	}

//-----------------------------------------------------------------------------

}	// Impl
}	// SWShaderLang

#endif	// GRAPHICS_API_SOFT
