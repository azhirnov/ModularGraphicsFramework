// Copyright © 2014-2017  Zhirnov Andrey. All rights reserved.

#pragma once

#include "Ptr.h"

namespace GX_STL
{
namespace GXTypes
{

	//
	// Mutable
	//

	template <typename T>
	struct Mutable
	{
	// variables
	private:
		mutable T	_value;


	// methods
	public:
		Mutable (GX_DEFCTOR);


	};



	//
	// Immutable
	//

	template <typename T>
	struct Immutable
	{
	};


}	// GXTypes
}	// GX_STL
