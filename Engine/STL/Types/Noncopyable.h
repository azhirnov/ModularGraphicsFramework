// Copyright © 2014-2017  Zhirnov Andrey. All rights reserved.

#pragma once

#include "Engine/STL/CompileTime/TypeQualifier.h"

namespace GX_STL
{
namespace GXTypes
{

	//
	// Non-copyable base class
	//

	class Noncopyable : public CompileTime::TypeQualifier< CompileTime::ETypeQualifier::Def_Noncopyable >
	{
	public:
		Noncopyable () {}

		~Noncopyable () {}

	private:
		explicit
		Noncopyable (const Noncopyable &) = delete;

		Noncopyable (Noncopyable &&) = delete;

		void operator = (const Noncopyable &) = delete;
	};


}	// GXTypes
}	// GX_STL
