// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

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
		explicit Noncopyable (const Noncopyable &) = delete;
		explicit Noncopyable (Noncopyable &&) = delete;

		Noncopyable& operator = (const Noncopyable &) = delete;
		Noncopyable& operator = (Noncopyable &&) = delete;
	};


}	// GXTypes
}	// GX_STL
